#include "Family/InheritanceSystem.h"
#include "Core/ShanHeLog.h"

TArray<FHeritageDistribution> UInheritanceSystem::DistributeEstate(
    FGuid DeceasedID, UFamilyTree* FamilyTree, EInheritanceType InheritanceType,
    const FResourceStock& TotalWealth, float TotalLand, int32 TotalReputation)
{
    if (!FamilyTree) return TArray<FHeritageDistribution>();

    switch (InheritanceType)
    {
        case EInheritanceType::Primogeniture:
            return DistributePrimogeniture(DeceasedID, FamilyTree, TotalWealth, TotalLand, TotalReputation);
        case EInheritanceType::EqualDivision:
            return DistributeEqualDivision(DeceasedID, FamilyTree, TotalWealth, TotalLand, TotalReputation);
        default:
            return DistributePrimogeniture(DeceasedID, FamilyTree, TotalWealth, TotalLand, TotalReputation);
    }
}

FGuid UInheritanceSystem::DetermineHeir(FGuid DeceasedID, UFamilyTree* FamilyTree, EInheritanceType Type) const
{
    if (!FamilyTree) return FGuid();
    TArray<FFamilyMember> Children = FamilyTree->GetChildren(DeceasedID);
    if (Children.Num() == 0) return FGuid();

    switch (Type)
    {
        case EInheritanceType::Primogeniture: // 长子继承
        {
            // 找最年长的儿子
            FFamilyMember* EldestSon = nullptr;
            for (FFamilyMember& C : Children)
            {
                if (C.bIsMale && C.bIsAlive)
                {
                    if (!EldestSon || C.Age > EldestSon->Age)
                        EldestSon = &C;
                }
            }
            return EldestSon ? EldestSon->MemberID : (Children.Num() > 0 ? Children[0].MemberID : FGuid());
        }
        case EInheritanceType::Ultimogeniture: // 幼子继承
        {
            FFamilyMember* YoungestSon = nullptr;
            for (FFamilyMember& C : Children)
            {
                if (C.bIsMale && C.bIsAlive)
                {
                    if (!YoungestSon || C.Age < YoungestSon->Age)
                        YoungestSon = &C;
                }
            }
            return YoungestSon ? YoungestSon->MemberID : FGuid();
        }
        case EInheritanceType::Meritocracy: // 选贤继承
        {
            FFamilyMember* Best = nullptr;
            for (FFamilyMember& C : Children)
            {
                if (C.bIsAlive)
                {
                    float Score = C.Stats.Intelligence + C.Stats.Charisma + C.Stats.Governance;
                    if (!Best || Score > (Best->Stats.Intelligence + Best->Stats.Charisma + Best->Stats.Governance))
                        Best = &C;
                }
            }
            return Best ? Best->MemberID : FGuid();
        }
        default:
            return Children.Num() > 0 ? Children[0].MemberID : FGuid();
    }
}

FResourceStock UInheritanceSystem::CalculateInheritance(FGuid HeirID, const TArray<FHeritageDistribution>& Distribution,
    const FResourceStock& TotalWealth) const
{
    FResourceStock Result;
    for (const FHeritageDistribution& D : Distribution)
    {
        if (D.HeirID == HeirID)
        {
            Result.Food = (int32)(TotalWealth.Food * D.WealthShare);
            Result.Silver = (int32)(TotalWealth.Silver * D.WealthShare);
            Result.Gold = (int32)(TotalWealth.Gold * D.WealthShare);
            break;
        }
    }
    return Result;
}

void UInheritanceSystem::TransferFamilySkills(FGuid FromID, FGuid ToID, float TransferRate)
{
    // 家族技能传承：父辈的技能有一定概率传给子辈
    UE_LOG(LogShanHe, Log, TEXT("家族技能传承: %s -> %s，传承率 %.0f%%"),
        *FromID.ToString(), *ToID.ToString(), TransferRate * 100);
}

TArray<FHeritageDistribution> UInheritanceSystem::DistributePrimogeniture(FGuid DeceasedID, UFamilyTree* Tree,
    const FResourceStock& Wealth, float Land, int32 Reputation)
{
    TArray<FHeritageDistribution> Result;
    FGuid HeirID = DetermineHeir(DeceasedID, Tree, EInheritanceType::Primogeniture);
    if (!HeirID.IsValid()) return Result;

    FHeritageDistribution MainHeir;
    MainHeir.HeirID = HeirID;
    MainHeir.WealthShare = 0.7f;    // 长子得70%
    MainHeir.LandShare = 0.7f;
    MainHeir.ReputationShare = 1.0f; // 声望全归继承人
    MainHeir.bInheritsTitle = true;
    Result.Add(MainHeir);

    // 其余子女分30%
    TArray<FFamilyMember> Children = Tree->GetChildren(DeceasedID);
    int32 OtherCount = Children.Num() - 1;
    if (OtherCount > 0)
    {
        float ShareEach = 0.3f / OtherCount;
        for (const FFamilyMember& C : Children)
        {
            if (C.MemberID != HeirID && C.bIsAlive)
            {
                FHeritageDistribution Other;
                Other.HeirID = C.MemberID;
                Other.WealthShare = ShareEach;
                Other.LandShare = ShareEach;
                Other.ReputationShare = 0.0f;
                Result.Add(Other);
            }
        }
    }

    UE_LOG(LogShanHe, Log, TEXT("长子继承分配完成，继承人获得70%%财产和全部声望"));
    return Result;
}

TArray<FHeritageDistribution> UInheritanceSystem::DistributeEqualDivision(FGuid DeceasedID, UFamilyTree* Tree,
    const FResourceStock& Wealth, float Land, int32 Reputation)
{
    TArray<FHeritageDistribution> Result;
    TArray<FFamilyMember> Children = Tree->GetChildren(DeceasedID);
    int32 AliveCount = 0;
    for (const FFamilyMember& C : Children) if (C.bIsAlive) AliveCount++;
    if (AliveCount == 0) return Result;

    float ShareEach = 1.0f / AliveCount;
    for (const FFamilyMember& C : Children)
    {
        if (!C.bIsAlive) continue;
        FHeritageDistribution D;
        D.HeirID = C.MemberID;
        D.WealthShare = ShareEach;
        D.LandShare = ShareEach;
        D.ReputationShare = ShareEach;
        D.bInheritsTitle = (C.bIsMale && C.Age >= 16); // 诸子均分，但头衔给长子
        Result.Add(D);
    }

    UE_LOG(LogShanHe, Log, TEXT("诸子均分完成，每人获得 %.1f%%"), ShareEach * 100);
    return Result;
}
