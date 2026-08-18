#pragma once
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "TravelEventSystem.generated.h"

UENUM(BlueprintType)
enum class ETravelEventType : uint8
{
    Bandit      UMETA(DisplayName="路遇强盗"),
    Adventure   UMETA(DisplayName="奇遇"),
    Merchant    UMETA(DisplayName="商人"),
    Refugee     UMETA(DisplayName="难民"),
    Swordsman   UMETA(DisplayName="江湖侠客"),
    Immortal    UMETA(DisplayName="仙人指路"),
    Ambush      UMETA(DisplayName="埋伏"),
    Lost        UMETA(DisplayName="迷路"),
    Weather     UMETA(DisplayName="恶劣天气"),
    Treasure    UMETA(DisplayName="发现宝藏")
};

USTRUCT(BlueprintType)
struct FTravelEvent
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FName EventID;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) ETravelEventType Type;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FText Title;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FText Description;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) float Weight = 1.0f; // 出现权重
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 MinPlayerLevel = 1;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) TArray<FName> RequiredRegions; // 限定区域
};

USTRUCT(BlueprintType)
struct FTravelEventChoice
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FText ChoiceText;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FName ResultEventID;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) TArray<FName> RequiredTags; // 需要的属性/技能
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTravelEventTriggered, FTravelEvent, Event);

/**
 * 旅行随机事件系统 - 太吾核心玩法
 * 路遇强盗/奇遇/商人/难民/江湖侠客/仙人指路
 */
UCLASS()
class SHANHEERA_API UTravelEventSystem : public UObject
{
    GENERATED_BODY()
public:
    UFUNCTION(BlueprintCallable, Category="山河纪元|旅行事件")
    void InitializeEvents();

    UFUNCTION(BlueprintCallable, Category="山河纪元|旅行事件")
    FTravelEvent RollRandomEvent(int32 PlayerLevel, FName CurrentRegion);

    UFUNCTION(BlueprintCallable, Category="山河纪元|旅行事件")
    TArray<FTravelEventChoice> GetEventChoices(FName EventID);

    UFUNCTION(BlueprintCallable, Category="山河纪元|旅行事件")
    void ResolveChoice(FName EventID, int32 ChoiceIndex);

    UFUNCTION(BlueprintCallable, Category="山河纪元|旅行事件")
    float GetTravelEventChance() const; // 每格旅行触发概率

    UPROPERTY(BlueprintAssignable, Category="山河纪元|旅行事件")
    FOnTravelEventTriggered OnEventTriggered;

    UPROPERTY(BlueprintReadOnly, Category="山河纪元|旅行事件")
    TMap<FName, FTravelEvent> EventDatabase;

    UPROPERTY(EditAnywhere, Category="山河纪元|旅行事件")
    float BaseEventChance = 0.3f; // 30%概率触发

private:
    TMap<FName, TArray<FTravelEventChoice>> ChoiceDatabase;
};
