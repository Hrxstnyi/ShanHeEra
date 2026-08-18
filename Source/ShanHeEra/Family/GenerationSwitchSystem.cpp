#include "Family/GenerationSwitchSystem.h"
#include "Family/InheritanceSystem.h"
#include "Core/ShanHeLog.h"

void UGenerationSwitchSystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
    UE_LOG(LogShanHe, Log, TEXT("世代切换系统初始化 - 多代传承就绪"));
}

void UGenerationSwitchSystem::OnPlayerCharacterDeath(FGuid DeceasedID, UFamilyTree* FamilyTree)
{
    UE_LOG(LogShanHe, Warning, TEXT("玩家角色死亡！ID: %s"), *DeceasedID.ToString());

    TArray<FFamilyMember> Heirs = GetEligibleHeirs(DeceasedID, FamilyTree);
    if (Heirs.Num() == 0)
    {
        UE_LOG(LogShanHe, Error, TEXT("没有可继承的后代！游戏结束。"));
        return;
    }

    bCanSwitchGeneration = true;
    ShowHeirSelectionUI(Heirs);
}

TArray<FFamilyMember> UGenerationSwitchSystem::GetEligibleHeirs(FGuid DeceasedID, UFamilyTree* FamilyTree) const
{
    TArray<FFamilyMember> Result;
    if (!FamilyTree) return Result;

    TArray<FFamilyMember> Children = FamilyTree->GetChildren(DeceasedID);
    for (const FFamilyMember& C : Children)
    {
        // 继承人条件：活着、年满16岁
        if (C.bIsAlive && C.Age >= 16)
            Result.Add(C);
    }

    // 如果没有成年子女，看孙辈
    if (Result.Num() == 0)
    {
        for (const FFamilyMember& C : Children)
        {
            TArray<FFamilyMember> Grandchildren = FamilyTree->GetChildren(C.MemberID);
            for (const FFamilyMember& G : Grandchildren)
            {
                if (G.bIsAlive && G.Age >= 16)
                    Result.Add(G);
            }
        }
    }

    return Result;
}

void UGenerationSwitchSystem::SwitchToHeir(FGuid HeirID)
{
    if (!bCanSwitchGeneration) return;

    FGuid OldID = CurrentPlayerID;
    CurrentPlayerID = HeirID;
    CurrentGeneration++;
    TotalGenerations++;
    bCanSwitchGeneration = false;

    ApplyInheritanceBonuses(HeirID, OldID);

    OnGenerationSwitch.Broadcast(OldID, HeirID);

    UE_LOG(LogShanHe, Log, TEXT("世代切换！第%d代，新角色ID: %s"), CurrentGeneration, *HeirID.ToString());
}

void UGenerationSwitchSystem::ShowHeirSelectionUI(const TArray<FFamilyMember>& Heirs)
{
    // 实际项目中弹出UI让玩家选择继承人
    // 显示每个继承人的属性、年龄、职业、与死者的关系
    UE_LOG(LogShanHe, Log, TEXT("显示继承人选择界面，共 %d 位候选人"), Heirs.Num());
}

void UGenerationSwitchSystem::ApplyInheritanceBonuses(FGuid HeirID, FGuid DeceasedID)
{
    // 新角色继承：
    // 1. 部分财产
    // 2. 部分声望
    // 3. 家族技能(部分传承)
    // 4. 人际关系(部分继承)
    UE_LOG(LogShanHe, Log, TEXT("应用继承加成: %s 继承自 %s"), *HeirID.ToString(), *DeceasedID.ToString());
}
