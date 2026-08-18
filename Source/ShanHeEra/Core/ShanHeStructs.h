#pragma once
#include "CoreMinimal.h"
#include "ShanHeEnums.h"
#include "ShanHeStructs.generated.h"
USTRUCT(BlueprintType)
struct FPersonName
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FString Surname;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FString GivenName;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FString CourtesyName;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FString ArtName;
};
USTRUCT(BlueprintType)
struct FCharacterStats
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 Strength = 50;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 Intelligence = 50;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 Charisma = 50;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 Governance = 50;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 Martial = 50;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 Commerce = 50;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 Learning = 50;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 Reputation = 0;
};
USTRUCT(BlueprintType)
struct FResourceStock
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 Food = 0;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 Copper = 0;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 Silver = 0;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 Gold = 0;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 Iron = 0;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 Cloth = 0;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 Salt = 0;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 Tea = 0;
};
USTRUCT(BlueprintType)
struct FDate
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 Year = 1;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 Month = 1;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 Day = 1;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) EEraType Era = EEraType::Unified;
};
USTRUCT(BlueprintType)
struct FHistoricalEvent
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FName EventID;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FText Title;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FText Description;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 TriggerYear = 0;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) bool bIsCG = false;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) bool bHasTriggered = false;
};
USTRUCT(BlueprintType)
struct FSubtitleEntry
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FText Text;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) float StartTime = 0.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) float Duration = 3.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FName SpeakerID;
};

// === GameplayTags 支持 ===
// 用于状态、事件、分类的标签系统，替代硬编码的FName比较
USTRUCT(BlueprintType)
struct FShanHeGameplayTags
{
    GENERATED_BODY()

    // 状态标签
    static FName Status_Alive() { return FName("Status.Alive"); }
    static FName Status_Dead() { return FName("Status.Dead"); }
    static FName Status_Combat() { return FName("Status.Combat"); }
    static FName Status_Stealth() { return FName("Status.Stealth"); }

    // 事件标签
    static FName Event_Battle() { return FName("Event.Battle"); }
    static FName Event_Disaster() { return FName("Event.Disaster"); }
    static FName Event_Festival() { return FName("Event.Festival"); }
    static FName Event_Cinematic() { return FName("Event.Cinematic"); }

    // 阵营标签
    static FName Faction_Player() { return FName("Faction.Player"); }
    static FName Faction_Enemy() { return FName("Faction.Enemy"); }
    static FName Faction_Neutral() { return FName("Faction.Neutral"); }
};

