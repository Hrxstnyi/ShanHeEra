#include "Dynasty/DiplomacySystem.h"
#include "Core/ShanHeLog.h"

void UDiplomacySystem::InitializeDiplomacy()
{
    // 初始化主要势力关系
    UE_LOG(LogShanHe, Log, TEXT("外交系统初始化 - 同盟/宣战/藩属/朝贡/联姻/贸易"));
}

void UDiplomacySystem::Simulate(int32 Days)
{
    UpdateRelations(Days);
    CheckWarDecay();
    AI_DiplomacyDecisions();
}

bool UDiplomacySystem::PerformAction(EDipActionType Action, FName From, FName To, int32 Parameter)
{
    FDiplomacyRelationData* Rel = nullptr;
    for (FDiplomacyRelationData& R : Relations)
    {
        if ((R.FactionA == From && R.FactionB == To) || (R.FactionA == To && R.FactionB == From))
        { Rel = &R; break; }
    }
    if (!Rel) return false;

    switch (Action)
    {
        case EDipActionType::SendGift:
            ChangeOpinion(From, To, Parameter / 100);
            UE_LOG(LogShanHe, Log, TEXT("%s 遣使送礼给 %s，好感+%d"), *From.ToString(), *To.ToString(), Parameter/100);
            return true;
        case EDipActionType::ProposeAlliance:
            if (Rel->Opinion > 50 && !Rel->bAtWar)
            {
                Rel->Relation = EDiplomacyRelation::Allied;
                UE_LOG(LogShanHe, Log, TEXT("%s 与 %s 结盟"), *From.ToString(), *To.ToString());
                return true;
            }
            return false;
        case EDipActionType::DeclareWar:
            Rel->bAtWar = true;
            Rel->Relation = EDiplomacyRelation::Hostile;
            Rel->WarDays = 0;
            UE_LOG(LogShanHe, Warning, TEXT("%s 向 %s 宣战！"), *From.ToString(), *To.ToString());
            return true;
        case EDipActionType::PeaceOffer:
            if (Rel->bAtWar && Rel->Opinion > -50)
            {
                Rel->bAtWar = false;
                Rel->WarDays = 0;
                UE_LOG(LogShanHe, Log, TEXT("%s 与 %s 议和"), *From.ToString(), *To.ToString());
                return true;
            }
            return false;
        case EDipActionType::DemandVassal:
            if (Rel->Opinion < -30 || Rel->bAtWar)
            {
                FTributaryState Vassal;
                Vassal.VassalFaction = To;
                Vassal.SuzerainFaction = From;
                Vassal.AnnualTribute = Parameter;
                Vassal.Autonomy = 50;
                Tributaries.Add(Vassal);
                Rel->Relation = EDiplomacyRelation::Vassal;
                UE_LOG(LogShanHe, Log, TEXT("%s 臣服于 %s"), *To.ToString(), *From.ToString());
                return true;
            }
            return false;
        default:
            return false;
    }
}

EDiplomacyRelation UDiplomacySystem::GetRelation(FName A, FName B) const
{
    for (const FDiplomacyRelationData& R : Relations)
        if ((R.FactionA == A && R.FactionB == B) || (R.FactionA == B && R.FactionB == A))
            return R.Relation;
    return EDiplomacyRelation::Neutral;
}

int32 UDiplomacySystem::GetOpinion(FName A, FName B) const
{
    for (const FDiplomacyRelationData& R : Relations)
        if ((R.FactionA == A && R.FactionB == B) || (R.FactionA == B && R.FactionB == A))
            return R.Opinion;
    return 0;
}

void UDiplomacySystem::ChangeOpinion(FName A, FName B, int32 Amount)
{
    for (FDiplomacyRelationData& R : Relations)
    {
        if ((R.FactionA == A && R.FactionB == B) || (R.FactionA == B && R.FactionB == A))
        {
            R.Opinion = FMath::Clamp(R.Opinion + Amount, -100, 100);
            return;
        }
    }
}

TArray<FName> UDiplomacySystem::GetAllies(FName Faction) const
{
    TArray<FName> Result;
    for (const FDiplomacyRelationData& R : Relations)
    {
        if (R.Relation == EDiplomacyRelation::Allied && !R.bAtWar)
        {
            if (R.FactionA == Faction) Result.Add(R.FactionB);
            else if (R.FactionB == Faction) Result.Add(R.FactionA);
        }
    }
    return Result;
}

TArray<FName> UDiplomacySystem::GetEnemies(FName Faction) const
{
    TArray<FName> Result;
    for (const FDiplomacyRelationData& R : Relations)
    {
        if (R.bAtWar)
        {
            if (R.FactionA == Faction) Result.Add(R.FactionB);
            else if (R.FactionB == Faction) Result.Add(R.FactionA);
        }
    }
    return Result;
}

void UDiplomacySystem::UpdateRelations(int32 Days)
{
    for (FDiplomacyRelationData& R : Relations)
    {
        if (R.bAtWar)
        {
            R.WarDays += Days;
            R.Opinion = FMath::Max(-100, R.Opinion - Days);
        }
        else
        {
            // 和平时期关系缓慢回归中立
            if (R.Opinion > 0) R.Opinion = FMath::Max(0, R.Opinion - Days / 30);
            else if (R.Opinion < 0) R.Opinion = FMath::Min(0, R.Opinion + Days / 30);
        }
    }
}

void UDiplomacySystem::CheckWarDecay()
{
    // 长期战争可能导致厌战求和
}

void UDiplomacySystem::AI_DiplomacyDecisions()
{
    // AI势力自主外交决策
}
