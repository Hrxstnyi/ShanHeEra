#pragma once
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "EspionageSystem.generated.h"

UENUM(BlueprintType)
enum class ESpyAction : uint8
{
    Infiltrate  UMETA(DisplayName="安插眼线"),
    Gather      UMETA(DisplayName="收集情报"),
    Assassinate UMETA(DisplayName="刺杀"),
    Counter     UMETA(DisplayName="反间计"),
    Seduce      UMETA(DisplayName="美人计"),
    Sabotage    UMETA(DisplayName="破坏"),
    Bribe       UMETA(DisplayName="贿赂")
};

USTRUCT(BlueprintType)
struct FSpyAgent
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FName AgentID;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FText AgentName;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 Stealth = 50; // 潜行能力
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 Charm = 50; // 魅力
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 Combat = 50; // 战斗
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FName TargetFaction;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) bool bIsActive = false;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 CoverLevel = 0; // 掩护等级 0-100
};

USTRUCT(BlueprintType)
struct FIntelligence
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FName IntelID;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FText Content;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FName SourceFaction;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 Reliability = 50; // 可信度 0-100
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FDateTime ObtainedDate;
};

/**
 * 情报间谍系统 - CK3玩法
 * 安插眼线、收集情报、刺杀、反间计、美人计
 */
UCLASS()
class SHANHEERA_API UEspionageSystem : public UObject
{
    GENERATED_BODY()
public:
    UFUNCTION(BlueprintCallable, Category="山河纪元|情报")
    void InitializeEspionage();

    UFUNCTION(BlueprintCallable, Category="山河纪元|情报")
    bool DeployAgent(FName AgentID, FName TargetFaction);

    UFUNCTION(BlueprintCallable, Category="山河纪元|情报")
    FIntelligence GatherIntelligence(FName TargetFaction);

    UFUNCTION(BlueprintCallable, Category="山河纪元|情报")
    bool AssassinateTarget(FName TargetID);

    UFUNCTION(BlueprintCallable, Category="山河纪元|情报")
    bool CounterIntelligence(FName TargetFaction); // 反间计

    UFUNCTION(BlueprintCallable, Category="山河纪元|情报")
    bool HoneyTrap(FName AgentID, FName TargetID); // 美人计

    UFUNCTION(BlueprintCallable, Category="山河纪元|情报")
    bool BribeOfficial(FName TargetID, int32 Amount);

    UFUNCTION(BlueprintCallable, Category="山河纪元|情报")
    void RecallAgent(FName AgentID);

    UPROPERTY(BlueprintReadOnly, Category="山河纪元|情报")
    TMap<FName, FSpyAgent> AgentRoster;

    UPROPERTY(BlueprintReadOnly, Category="山河纪元|情报")
    TArray<FIntelligence> IntelligenceCache;

    UPROPERTY(EditAnywhere, Category="山河纪元|情报")
    int32 SpyNetworkLevel = 1; // 情报网等级
};
