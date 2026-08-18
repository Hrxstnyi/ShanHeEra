#pragma once
#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Military/ArmyUnit.h"
#include "ShanHeBattleManager.generated.h"

UENUM(BlueprintType)
enum class EBattlePhase : uint8
{
    PreBattle   UMETA(DisplayName="战前部署"),
    Engagement  UMETA(DisplayName="接战"),
    Melee       UMETA(DisplayName="近战"),
    Climax      UMETA(DisplayName="决战"),
    Route       UMETA(DisplayName="追击/溃逃"),
    Ended       UMETA(DisplayName="战斗结束")
};

USTRUCT(BlueprintType)
struct FBattleResult
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite) EWarResult Result;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 AttackerLosses = 0;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 DefenderLosses = 0;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) float DurationDays = 0;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FText BattleReport;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBattleEnded, FBattleResult, Result);

/**
 * 战斗管理器 - 骑砍式战场战术层
 * 支持：战前部署、实时战斗、士气系统、冲锋、溃逃、追击
 * 玩家可以亲自上阵（第三人称），也可以指挥部队（战略视角）
 */
UCLASS()
class SHANHEERA_API UBattleManager : public UWorldSubsystem
{
    GENERATED_BODY()
public:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;

    UFUNCTION(BlueprintCallable, Category="山河纪元|战斗")
    void StartBattle(TArray<UArmyUnit*> Attackers, TArray<UArmyUnit*> Defenders, FName Location);

    UFUNCTION(BlueprintCallable, Category="山河纪元|战斗")
    void SimulateBattleTick(float DeltaTime);

    UFUNCTION(BlueprintCallable, Category="山河纪元|战斗")
    void OrderCharge(FName UnitID);

    UFUNCTION(BlueprintCallable, Category="山河纪元|战斗")
    void OrderRetreat(FName UnitID);

    UFUNCTION(BlueprintCallable, Category="山河纪元|战斗")
    void OrderRally(FName UnitID);

    UFUNCTION(BlueprintCallable, Category="山河纪元|战斗")
    void EndBattle();

    UPROPERTY(BlueprintReadOnly, Category="山河纪元|战斗")
    EBattlePhase CurrentPhase = EBattlePhase::PreBattle;

    UPROPERTY(BlueprintReadOnly, Category="山河纪元|战斗")
    TArray<UArmyUnit*> AttackerUnits;

    UPROPERTY(BlueprintReadOnly, Category="山河纪元|战斗")
    TArray<UArmyUnit*> DefenderUnits;

    UPROPERTY(BlueprintReadOnly, Category="山河纪元|战斗")
    FName BattleLocation;

    UPROPERTY(BlueprintAssignable, Category="山河纪元|战斗")
    FOnBattleEnded OnBattleEnded;

private:
    void ResolveEngagement();
    void CalculateMoraleChanges();
    void CheckBattleEnd();
    FBattleResult GenerateResult();
    float BattleTime = 0.0f;
};
