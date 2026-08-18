#pragma once
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Core/ShanHeEnums.h"
#include "DisasterSystem.generated.h"

USTRUCT(BlueprintType)
struct FDisasterEvent
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FName DisasterID;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) EDisasterType Type;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FText Title;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FText Description;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FName RegionID;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 StartDay = 0;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 DurationDays = 0;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 Severity = 1; // 1-5
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 AffectedPopulation = 0;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 Casualties = 0;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) float GrainPriceMultiplier = 1.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) float StabilityImpact = 0.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) bool bIsActive = false;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) bool bHasCG = false;
};

USTRUCT(BlueprintType)
struct FPlagueData
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FName PlagueID;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FText PlagueName;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FName OriginCity;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) float InfectionRate = 0.1f;  // 日感染率
    UPROPERTY(EditAnywhere, BlueprintReadWrite) float MortalityRate = 0.05f; // 死亡率
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 Infected = 0;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 Deaths = 0;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 Recovered = 0;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) TArray<FName> InfectedCities;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 DaysSinceOutbreak = 0;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) bool bIsContained = false;
};

USTRUCT(BlueprintType)
struct FReliefEffort
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FName DisasterID;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 GrainDistributed = 0;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 SilverSpent = 0;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 MedicineSent = 0;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 WorkersMobilized = 0;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) float ReliefEffectiveness = 0.0f; // 0-1
    UPROPERTY(EditAnywhere, BlueprintReadWrite) bool bIsPlayerLed = false;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDisasterTriggered, FDisasterEvent, Disaster);

/**
 * 天灾疫病系统 - 管理自然灾害和疫病的发生、传播、影响、救灾
 * 7种天灾：洪水/干旱/蝗灾/地震/疫病/寒灾/饥荒
 * 疫病有独立的传播模型
 */
UCLASS()
class SHANHEERA_API UDisasterSystem : public UObject
{
    GENERATED_BODY()
public:
    UFUNCTION(BlueprintCallable, Category="山河纪元|天灾")
    void InitializeDisasters();

    UFUNCTION(BlueprintCallable, Category="山河纪元|天灾")
    void Simulate(int32 Days, int32 CurrentYear, int32 CurrentMonth);

    UFUNCTION(BlueprintCallable, Category="山河纪元|天灾")
    void TriggerDisaster(EDisasterType Type, FName RegionID, int32 Severity);

    UFUNCTION(BlueprintCallable, Category="山河纪元|天灾")
    void StartRelief(FName DisasterID, int32 Grain, int32 Silver, int32 Medicine, bool bPlayerLed);

    UFUNCTION(BlueprintCallable, Category="山河纪元|天灾")
    TArray<FDisasterEvent> GetActiveDisasters() const;

    UFUNCTION(BlueprintCallable, Category="山河纪元|天灾")
    FPlagueData GetActivePlague() const { return ActivePlague; }

    UPROPERTY(BlueprintAssignable, Category="山河纪元|天灾")
    FOnDisasterTriggered OnDisasterTriggered;

    UPROPERTY(BlueprintReadOnly, Category="山河纪元|天灾")
    TArray<FDisasterEvent> DisasterHistory;

    UPROPERTY(BlueprintReadOnly, Category="山河纪元|天灾")
    FPlagueData ActivePlague;

    UPROPERTY(BlueprintReadOnly, Category="山河纪元|天灾")
    TMap<FName, FReliefEffort> ReliefEfforts;

    UPROPERTY(EditAnywhere, Category="山河纪元|天灾|概率")
    float DisasterChancePerYear = 0.3f; // 每年30%概率发生天灾

private:
    void CheckRandomDisaster(int32 Year, int32 Month);
    void SimulatePlague(int32 Days);
    void UpdateDisasterEffects(int32 Days);
    void EndDisaster(FName DisasterID);
    float GetDisasterProbability(EDisasterType Type, int32 Month) const;
};
