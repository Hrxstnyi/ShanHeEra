#include "Dynasty/LegalSystem.h"
#include "Core/ShanHeLog.h"

void ULegalSystem::InitializeLaws()
{
    UE_LOG(LogShanHe, Log, TEXT("法律司法系统初始化 - 律法/案件/刑罚/监狱"));
}

FLawCase ULegalSystem::CreateCase(ECrimeType Crime, FGuid Defendant, FGuid Plaintiff, FName City)
{
    FLawCase Case;
    Case.CaseID = FName(*FString::Printf(TEXT("Case_%d"), FDateTime::Now().GetTicks()));
    Case.Crime = Crime;
    Case.DefendantID = Defendant;
    Case.PlaintiffID = Plaintiff;
    Case.CourtCity = City;
    Case.Severity = (int32)Crime + 1;
    AllCases.Add(Case);
    UE_LOG(LogShanHe, Log, TEXT("新案件：%s，类型：%d"), *Case.CaseID.ToString(), (int32)Crime);
    return Case;
}

EPunishmentType ULegalSystem::JudgeCase(FName CaseID, bool bIsGuilty, int32 CorruptionLevel)
{
    for (FLawCase& C : AllCases)
    {
        if (C.CaseID == CaseID)
        {
            C.bIsSolved = true;
            if (!bIsGuilty) return EPunishmentType::Amnesty;

            // 贪腐影响判决：贪腐越高，重罪轻判概率越大
            float CorruptionFactor = CorruptionLevel / 100.0f;
            if (FMath::RandRange(0.0f, 1.0f) < CorruptionFactor * 0.5f)
            {
                C.Punishment = EPunishmentType::Fine; // 花钱消灾
                JudicialFairness = FMath::Max(0.0f, JudicialFairness - 2.0f);
                return EPunishmentType::Fine;
            }

            // 根据罪行量刑
            switch (C.Crime)
            {
                case ECrimeType::Theft: C.Punishment = EPunishmentType::Beating; break;
                case ECrimeType::Assault: C.Punishment = EPunishmentType::Imprisonment; break;
                case ECrimeType::Murder: C.Punishment = EPunishmentType::Death; break;
                case ECrimeType::Corruption: C.Punishment = EPunishmentType::Exile; break;
                case ECrimeType::Treason: C.Punishment = EPunishmentType::Death; break;
                default: C.Punishment = EPunishmentType::Fine; break;
            }
            JudicialFairness = FMath::Min(100.0f, JudicialFairness + 0.5f);
            return C.Punishment;
        }
    }
    return EPunishmentType::Amnesty;
}

void ULegalSystem::SimulateCases(int32 Days)
{
    for (FLawCase& C : AllCases)
    {
        if (!C.bIsSolved)
        {
            C.DaysPending += Days;
            if (C.DaysPending > 90)
            {
                UE_LOG(LogShanHe, Warning, TEXT("案件积压：%s，超过90天未审理"), *C.CaseID.ToString());
                JudicialFairness = FMath::Max(0.0f, JudicialFairness - 0.1f * Days);
            }
        }
    }
}

TArray<FLawCase> ULegalSystem::GetPendingCases(FName City) const
{
    TArray<FLawCase> Result;
    for (const FLawCase& C : AllCases)
        if (!C.bIsSolved && C.CourtCity == City) Result.Add(C);
    return Result;
}
