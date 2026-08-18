#pragma once
#include "CoreMinimal.h"
#include "MassEntityTypes.h"
#include "Core/ShanHeEnums.h"
#include "Core/ShanHeStructs.h"
#include "NPCFragment.generated.h"

/**
 * MassEntity 片段 - 用于数千NPC同存的纯数据结构
 * 不继承UObject，不Spawn Actor，仅在玩家附近时才生成可视Actor
 */
USTRUCT()
struct SHANHEERA_API FNPCIdentityFragment : public FMassFragment
{
    GENERATED_BODY()
    FGuid NPCID;
    FPersonName Name;
    int32 Age = 30;
    ESocialClass SocialClass = ESocialClass::Farmer;
    bool bIsAlive = true;
};

USTRUCT()
struct SHANHEERA_API FNPCStatsFragment : public FMassFragment
{
    GENERATED_BODY()
    FCharacterStats Stats;
    TArray<ECharacterTrait> Traits;
    int32 CurrentHealth = 100;
    int32 Energy = 100;
};

USTRUCT()
struct SHANHEERA_API FNPCLocationFragment : public FMassFragment
{
    GENERATED_BODY()
    FName CurrentCity;
    FVector WorldPosition = FVector::ZeroVector;
    FName TargetLocation;
    float MoveSpeed = 100.0f;
};

USTRUCT()
struct SHANHEERA_API FNPCOccupationFragment : public FMassFragment
{
    GENERATED_BODY()
    FName Occupation;       // farmer/merchant/scholar/soldier/doctor...
    FName Workplace;
    int32 SkillLevel = 1;
    int32 DailyIncome = 0;
};

USTRUCT()
struct SHANHEERA_API FNPCScheduleFragment : public FMassFragment
{
    GENERATED_BODY()
    // 每日日程：0-23小时的行为安排
    TArray<FName> HourlySchedule; // 24个元素，每个小时的行为
    FName CurrentActivity;
    int32 ScheduleDay = 0;
};

USTRUCT()
struct SHANHEERA_API FNPCRelationFragment : public FMassFragment
{
    GENERATED_BODY()
    TMap<FGuid, int32> Relations; // -100 ~ 100
    FGuid FamilyID;
    FGuid SpouseID;
    TArray<FGuid> ChildrenIDs;
    TArray<FGuid> FriendIDs;
    TArray<FGuid> EnemyIDs;
};

USTRUCT()
struct SHANHEERA_API FNPCNeedFragment : public FMassFragment
{
    GENERATED_BODY()
    float Hunger = 100.0f;     // 饱腹度
    float Thirst = 100.0f;     // 口渴度
    float Comfort = 80.0f;     // 舒适度
    float Happiness = 60.0f;   // 幸福度
    float Health = 100.0f;     // 健康度
};

/**
 * NPC处理器 - MassEntity的Processor，批量处理所有NPC的逻辑
 * 每帧处理数千个NPC的需求衰减、日程执行、关系变化
 */
UCLASS()
class SHANHEERA_API UNPCMassProcessor : public UObject
{
    GENERATED_BODY()
public:
    void ProcessNeeds(float DeltaTime, TArray<FNPCNeedFragment>& Needs);

    void ProcessSchedule(int32 CurrentHour, TArray<FNPCScheduleFragment>& Schedules,
                         TArray<FNPCLocationFragment>& Locations);

    void ProcessRelations(TArray<FNPCRelationFragment>& Relations);
};
