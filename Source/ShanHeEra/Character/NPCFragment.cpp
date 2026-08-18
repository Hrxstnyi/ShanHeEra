#include "Character/NPCFragment.h"
#include "Core/ShanHeLog.h"

void UNPCMassProcessor::ProcessNeeds(float DeltaTime, TArray<FNPCNeedFragment>& Needs)
{
    for (FNPCNeedFragment& Need : Needs)
    {
        // 需求随时间衰减
        Need.Hunger = FMath::Clamp(Need.Hunger - DeltaTime * 0.5f, 0.0f, 100.0f);
        Need.Thirst = FMath::Clamp(Need.Thirst - DeltaTime * 0.8f, 0.0f, 100.0f);
        Need.Comfort = FMath::Clamp(Need.Comfort - DeltaTime * 0.1f, 0.0f, 100.0f);
        Need.Happiness = FMath::Clamp(Need.Happiness - DeltaTime * 0.05f, 0.0f, 100.0f);

        // 健康度受其他需求影响
        float AvgNeed = (Need.Hunger + Need.Thirst + Need.Comfort + Need.Happiness) / 4.0f;
        if (AvgNeed < 30.0f)
        {
            Need.Health = FMath::Clamp(Need.Health - DeltaTime * 0.3f, 0.0f, 100.0f);
        }
        else if (AvgNeed > 70.0f)
        {
            Need.Health = FMath::Clamp(Need.Health + DeltaTime * 0.1f, 0.0f, 100.0f);
        }
    }
}

void UNPCMassProcessor::ProcessSchedule(int32 CurrentHour, TArray<FNPCScheduleFragment>& Schedules,
                                        TArray<FNPCLocationFragment>& Locations)
{
    for (int32 i = 0; i < Schedules.Num() && i < Locations.Num(); i++)
    {
        FNPCScheduleFragment& Schedule = Schedules[i];
        FNPCLocationFragment& Location = Locations[i];

        if (Schedule.HourlySchedule.IsValidIndex(CurrentHour))
        {
            FName NewActivity = Schedule.HourlySchedule[CurrentHour];
            if (NewActivity != Schedule.CurrentActivity)
            {
                Schedule.CurrentActivity = NewActivity;
                UE_LOG(LogShanHe, Verbose, TEXT("NPC活动变更: %s"), *NewActivity.ToString());
            }
        }
    }
}

void UNPCMassProcessor::ProcessRelations(TArray<FNPCRelationFragment>& Relations)
{
    for (FNPCRelationFragment& Relation : Relations)
    {
        // 关系随时间缓慢衰减（向0回归）
        for (auto& Pair : Relation.Relations)
        {
            int32& Value = Pair.Value;
            if (Value > 0)
            {
                Value = FMath::Max(0, Value - 1);
            }
            else if (Value < 0)
            {
                Value = FMath::Min(0, Value + 1);
            }
        }
    }
}
