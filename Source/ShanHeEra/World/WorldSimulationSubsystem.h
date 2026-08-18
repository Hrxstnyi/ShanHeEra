#pragma once
#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Core/ShanHeStructs.h"
#include "World/RegionManager.h"
#include "World/CityManager.h"
#include "Dynasty/CourtSystem.h"
#include "Economy/EconomySystem.h"
#include "Military/WarSystem.h"
#include "History/HistorySystem.h"
#include "Society/SocietySystem.h"
#include "ShanHeWorldSimulationSubsystem.generated.h"

UCLASS()
class SHANHEERA_API UWorldSimulationSubsystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()
public:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;

    UFUNCTION(BlueprintCallable, Category="山河纪元|世界模拟")
    void SimulateTick(int32 Days);

    UFUNCTION(BlueprintCallable, Category="山河纪元|世界模拟")
    void InitializeWorld();

    UPROPERTY(BlueprintReadOnly, Category="山河纪元|世界模拟")
    URegionManager* RegionManager;
    UPROPERTY(BlueprintReadOnly, Category="山河纪元|世界模拟")
    UCityManager* CityManager;
    UPROPERTY(BlueprintReadOnly, Category="山河纪元|世界模拟")
    UCourtSystem* CourtSystem;
    UPROPERTY(BlueprintReadOnly, Category="山河纪元|世界模拟")
    UEconomySystem* EconomySystem;
    UPROPERTY(BlueprintReadOnly, Category="山河纪元|世界模拟")
    UWarSystem* WarSystem;
    UPROPERTY(BlueprintReadOnly, Category="山河纪元|世界模拟")
    UHistorySystem* HistorySystem;
    UPROPERTY(BlueprintReadOnly, Category="山河纪元|世界模拟")
    USocietySystem* SocietySystem;

    UPROPERTY(BlueprintReadOnly, Category="山河纪元|世界模拟")
    int32 TotalSimulatedDays = 0;

private:
    void SimulateEconomy(int32 Days);
    void SimulatePolitics(int32 Days);
    void SimulateMilitary(int32 Days);
    void SimulateSociety(int32 Days);
    void SimulateDisasters(int32 Days);
    void CheckHistoricalEvents();
};
