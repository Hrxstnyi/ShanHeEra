#pragma once
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Core/ShanHeEnums.h"
#include "JianghuSystem.generated.h"

UENUM(BlueprintType)
enum class EJianghuFactionType : uint8
{
    Sect        UMETA(DisplayName="门派"),
    Gang        UMETA(DisplayName="帮会"),
    Escort      UMETA(DisplayName="镖局"),
    School      UMETA(DisplayName="武馆"),
    Assassin    UMETA(DisplayName="暗杀组织"),
    Merchant    UMETA(DisplayName="商帮"),
    Temple      UMETA(DisplayName="寺庙"),
    Independent UMETA(DisplayName="散修")
};

UENUM(BlueprintType)
enum class EJianghuRank : uint8
{
    Unknown     UMETA(DisplayName="无名之辈"),
    Novice      UMETA(DisplayName="初入江湖"),
    Adept       UMETA(DisplayName="小有名气"),
    Expert      UMETA(DisplayName="江湖名士"),
    Master      UMETA(DisplayName="一方人物"),
    Grandmaster UMETA(DisplayName="天下闻名"),
    Legend      UMETA(DisplayName="武林传奇")
};

USTRUCT(BlueprintType)
struct FJianghuFaction
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FName FactionID;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FText FactionName;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) EJianghuFactionType Type;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FName BaseCity;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 Members = 0;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 Influence = 0;   // 势力值
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 Wealth = 0;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 Reputation = 50; // 声望 0-100(正) / -100(邪)
    UPROPERTY(EditAnywhere, BlueprintReadWrite) TArray<FName> AlliedFactions;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) TArray<FName> EnemyFactions;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FText Description;
};

USTRUCT(BlueprintType)
struct FMartialArt
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FName SkillID;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FText SkillName;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FText Description;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 Type = 0; // 0=外功 1=内功 2=轻功 3=暗器
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 Power = 50;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 Level = 0;      // 修炼等级 0-10
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 MaxLevel = 10;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FName OriginFaction;  // 来源门派
    UPROPERTY(EditAnywhere, BlueprintReadWrite) bool bIsSecret = false; // 是否绝学
};

USTRUCT(BlueprintType)
struct FJianghuEvent
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FName EventID;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FText Title;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FText Description;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 ReputationChange = 0;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 WealthChange = 0;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) TArray<FName> RelatedFactions;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) bool bIsTriggered = false;
};

/**
 * 江湖系统 - 官方体系之外的人际社会
 * 包括：门派/帮会/镖局/武馆、江湖声望、武艺修炼、恩怨仇杀
 * 江湖不是幻想世界，是真实存在的民间力量网络
 */
UCLASS()
class SHANHEERA_API UJianghuSystem : public UObject
{
    GENERATED_BODY()
public:
    UFUNCTION(BlueprintCallable, Category="山河纪元|江湖")
    void InitializeJianghu();

    UFUNCTION(BlueprintCallable, Category="山河纪元|江湖")
    void Simulate(int32 Days);

    UFUNCTION(BlueprintCallable, Category="山河纪元|江湖")
    void ChangeReputation(FName PersonID, int32 Amount);

    UFUNCTION(BlueprintCallable, Category="山河纪元|江湖")
    EJianghuRank GetJianghuRank(int32 Reputation) const;

    UFUNCTION(BlueprintCallable, Category="山河纪元|江湖")
    void LearnMartialArt(FName PersonID, const FMartialArt& Art);

    UFUNCTION(BlueprintCallable, Category="山河纪元|江湖")
    void PracticeMartialArt(FName PersonID, FName SkillID, int32 Days);

    UFUNCTION(BlueprintCallable, Category="山河纪元|江湖")
    void TriggerJianghuEvent(FName EventID, FName PersonID);

    UFUNCTION(BlueprintCallable, Category="山河纪元|江湖")
    void AddEnemy(FName PersonA, FName PersonB, int32 Severity);

    UFUNCTION(BlueprintCallable, Category="山河纪元|江湖")
    int32 GetRelation(FName PersonA, FName PersonB) const;

    UPROPERTY(BlueprintReadOnly, Category="山河纪元|江湖")
    TArray<FJianghuFaction> Factions;

    UPROPERTY(BlueprintReadOnly, Category="山河纪元|江湖")
    TMap<FName, int32> PersonReputations;

    // 注意：以下复杂数据结构不暴露给蓝图，仅C++内部使用
    TMap<FName, TArray<FMartialArt>> PersonMartialArts;

    TMap<FName, TMap<FName, int32>> PersonRelations; // A -> (B -> 关系值)

    UPROPERTY(BlueprintReadOnly, Category="山河纪元|江湖")
    TArray<FJianghuEvent> Events;

private:
    void SimulateFactions(int32 Days);
    void SimulateConflicts(int32 Days);
    void RandomJianghuEvents();
};
