#include "Family/ChildEducationSystem.h"
#include "Core/ShanHeLog.h"

FEducationProgress UChildEducationSystem::StartEducation(FGuid ChildID, TEducationType Type, int32 ChildAge)
{
    FEducationProgress Progress;
    Progress.ChildID = ChildID;
    Progress.EducationType = Type;
    Progress.StartAge = ChildAge;
    Progress.CurrentAge = ChildAge;
    UE_LOG(LogShanHe, Log, TEXT("开始教育: 类型=%d，起始年龄=%d"), (int32)Type, ChildAge);
    return Progress;
}

void UChildEducationSystem::AdvanceEducation(FEducationProgress& Progress, int32 Years)
{
    float Quality = GetEducationQuality(Progress.EducationType);
    for (int32 i = 0; i < Years; i++)
    {
        Progress.CurrentAge++;
        // 每年的成长量受教育质量影响
        switch (Progress.EducationType)
        {
            case TEducationType::Family:
                Progress.Literacy += 3.0f * Quality;
                Progress.Knowledge += 2.0f * Quality;
                break;
            case TEducationType::PrivateTutor:
                Progress.Literacy += 8.0f * Quality;
                Progress.Knowledge += 6.0f * Quality;
                break;
            case TEducationType::Academy:
                Progress.Literacy += 10.0f * Quality;
                Progress.Knowledge += 12.0f * Quality;
                break;
            case TEducationType::Official:
                Progress.Literacy += 12.0f * Quality;
                Progress.Knowledge += 15.0f * Quality;
                break;
            case TEducationType::Apprentice:
                Progress.CommerceSkill += 10.0f * Quality;
                Progress.Literacy += 2.0f * Quality;
                break;
            case TEducationType::Military:
                Progress.MartialSkill += 12.0f * Quality;
                Progress.Literacy += 1.0f * Quality;
                break;
        }
        Progress.TuitionPaid += GetTuitionCost(Progress.EducationType, 1);
    }

    // 限制上限
    Progress.Literacy = FMath::Min(100.0f, Progress.Literacy);
    Progress.Knowledge = FMath::Min(100.0f, Progress.Knowledge);
    Progress.MartialSkill = FMath::Min(100.0f, Progress.MartialSkill);
    Progress.CommerceSkill = FMath::Min(100.0f, Progress.CommerceSkill);
}

FCharacterStats UChildEducationSystem::CalculateAdultStats(const FEducationProgress& Progress, const FCharacterStats& BaseStats) const
{
    FCharacterStats Stats = BaseStats;
    // 教育成果转化为属性
    Stats.Learning = BaseStats.Learning + (int32)(Progress.Knowledge * 0.5f);
    Stats.Intelligence = BaseStats.Intelligence + (int32)(Progress.Literacy * 0.3f);
    Stats.Martial = BaseStats.Martial + (int32)(Progress.MartialSkill * 0.5f);
    Stats.Commerce = BaseStats.Commerce + (int32)(Progress.CommerceSkill * 0.5f);
    return Stats;
}

TArray<FName> UChildEducationSystem::GetAvailableCareers(const FEducationProgress& Progress) const
{
    TArray<FName> Careers;
    if (Progress.Knowledge >= 50) Careers.Add(TEXT("Scholar"));      // 读书人
    if (Progress.Knowledge >= 80) Careers.Add(TEXT("Official"));     // 官员
    if (Progress.MartialSkill >= 50) Careers.Add(TEXT("Soldier"));   // 军人
    if (Progress.MartialSkill >= 80) Careers.Add(TEXT("General"));   // 将领
    if (Progress.CommerceSkill >= 50) Careers.Add(TEXT("Merchant")); // 商人
    if (Progress.Literacy >= 30) Careers.Add(TEXT("Teacher"));       // 教书先生
    return Careers;
}

int32 UChildEducationSystem::GetTuitionCost(TEducationType Type, int32 Years) const
{
    switch (Type)
    {
        case TEducationType::Family: return 0;
        case TEducationType::PrivateTutor: return 500 * Years;     // 每年500铜钱
        case TEducationType::Academy: return 2000 * Years;          // 每年2两银
        case TEducationType::Official: return 5000 * Years;         // 每年5两银
        case TEducationType::Apprentice: return 100 * Years;        // 学徒几乎免费
        case TEducationType::Military: return 300 * Years;
        default: return 0;
    }
}

float UChildEducationSystem::GetEducationQuality(TEducationType Type) const
{
    switch (Type)
    {
        case TEducationType::Family: return 0.5f;
        case TEducationType::PrivateTutor: return 0.8f;
        case TEducationType::Academy: return 1.0f;
        case TEducationType::Official: return 1.2f;
        case TEducationType::Apprentice: return 0.9f;
        case TEducationType::Military: return 0.9f;
        default: return 0.5f;
    }
}
