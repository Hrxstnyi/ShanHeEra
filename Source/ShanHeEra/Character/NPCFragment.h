#pragma once
#include "CoreMinimal.h"
#include "Core/ShanHeEnums.h"
#include "Core/ShanHeStructs.h"
#include "NPCFragment.generated.h"

/**
 * NPC 数据片段 - 用于数千NPC同存的纯数据结构
 * 不继承UObject，不Spawn Actor，仅在玩家附近时才生成可视Actor
 */
USTRUCT(BlueprintType)
struct SHANHEERA_API FNPCIdentityFragment
{
    GENERATED_BODY()
    UPROPERTY(BlueprintReadOnly) FGuid NPCID;
    UPROPERTY(BlueprintReadOnly) FPersonName Name;
    UPROPERTY(BlueprintReadOnly) int32 Age = 30;
    UPROPERTY(BlueprintReadOnly) ESocialClass SocialClass = ESocialClass::Farmer;
    UPROPERTY(BlueprintReadOnly) bool bIsAlive = true;
};

USTRUCT(BlueprintType)
struct SHANHEERA_API FNPCStatsFragment
{
    GENERATED_BODY()
    UPROPERTY(BlueprintReadOnly) FCharacterStats Stats;
    UPROPERTY(BlueprintReadOnly) TArray<ECharacterTrait> Traits;
    UPROPERTY(BlueprintReadOnly) int32 CurrentHealth = 100;
    UPROPERTY(BlueprintReadOnly) int32 Energy = 100;
};

USTRUCT(BlueprintType)
struct SHANHEERA_API FNPCLocationFragment
{
    GENERATED_BODY()
    UPROPERTY(BlueprintReadOnly) FName CurrentCity;
    UPROPERTY(BlueprintReadOnly) FVector WorldPosition = FVector::ZeroVector;
    UPROPERTY(BlueprintReadOnly) FName TargetLocation;
    UPROPERTY(BlueprintReadOnly) float MoveSpeed = 100.0f;
};

USTRUCT(BlueprintType)
struct SHANHEERA_API FNPCOccupationFragment
{
    GENERATED_BODY()
    UPROPERTY(BlueprintReadOnly) FName Occupation;
    UPROPERTY(BlueprintReadOnly) FName Workplace;
    UPROPERTY(BlueprintReadOnly) int32 SkillLevel = 1;
    UPROPERTY(BlueprintReadOnly) int32 DailyIncome = 0;
};

USTRUCT(BlueprintType)
struct SHANHEERA_API FNPCScheduleFragment
{
    GENERATED_BODY()
    UPROPERTY(BlueprintReadOnly) TArray<FName> HourlySchedule;
    UPROPERTY(BlueprintReadOnly) FName CurrentActivity;
    UPROPERTY(BlueprintReadOnly) int32 ScheduleDay = 0;
};

USTRUCT(BlueprintType)
struct SHANHEERA_API FNPCRelationFragment
{
    GENERATED_BODY()
    UPROPERTY(BlueprintReadOnly) TMap<FGuid, int32> Relations;
    UPROPERTY(BlueprintReadOnly) FGuid FamilyID;
    UPROPERTY(BlueprintReadOnly) FGuid SpouseID;
    UPROPERTY(BlueprintReadOnly) TArray<FGuid> ChildrenIDs;
    UPROPERTY(BlueprintReadOnly) TArray<FGuid> FriendIDs;
    UPROPERTY(BlueprintReadOnly) TArray<FGuid> EnemyIDs;
};

USTRUCT(BlueprintType)
struct SHANHEERA_API FNPCNeedFragment
{
    GENERATED_BODY()
    UPROPERTY(BlueprintReadOnly) float Hunger = 100.0f;
    UPROPERTY(BlueprintReadOnly) float Thirst = 100.0f;
    UPROPERTY(BlueprintReadOnly) float Comfort = 80.0f;
    UPROPERTY(BlueprintReadOnly) float Happiness = 60.0f;
    UPROPERTY(BlueprintReadOnly) float Health = 100.0f;
};

/**
 * NPC 处理器 - 批量处理所有NPC的逻辑
 * 每帧处理数千个NPC的需求衰减、日程执行、关系变化
 */
UCLASS(BlueprintType)
class SHANHEERA_API UNPCMassProcessor : public UObject
{
    GENERATED_BODY()
public:
    UFUNCTION(BlueprintCallable, Category="山河纪元|NPC")
    void ProcessNeeds(float DeltaTime, TArray<FNPCNeedFragment>& Needs);

    UFUNCTION(BlueprintCallable, Category="山河纪元|NPC")
    void ProcessSchedule(int32 CurrentHour, TArray<FNPCScheduleFragment>& Schedules,
                         TArray<FNPCLocationFragment>& Locations);

    UFUNCTION(BlueprintCallable, Category="山河纪元|NPC")
    void ProcessRelations(TArray<FNPCRelationFragment>& Relations);
};
