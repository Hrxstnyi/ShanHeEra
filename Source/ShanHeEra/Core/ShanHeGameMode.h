#pragma once
#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Core/ShanHeStructs.h"
#include "ShanHeGameMode.generated.h"

class AShanHeGameState;
class UWorldSimulationSubsystem;
class UAudioManager;

UCLASS()
class SHANHEERA_API AShanHeGameMode : public AGameModeBase
{
    GENERATED_BODY()
public:
    AShanHeGameMode();

    virtual void BeginPlay() override;
    virtual void Tick(float DeltaSeconds) override;

    UFUNCTION(BlueprintCallable, Category="山河纪元|时间")
    void AdvanceTime(int32 Days = 1);

    UFUNCTION(BlueprintCallable, Category="山河纪元|时间")
    FDate GetCurrentDate() const;

    UFUNCTION(BlueprintCallable, Category="山河纪元|存档")
    bool SaveGame(const FString& SlotName);
    UFUNCTION(BlueprintCallable, Category="山河纪元|存档")
    bool LoadGame(const FString& SlotName);

protected:
    UPROPERTY(EditAnywhere, Category="山河纪元|配置")
    float TimeScale = 1.0f;

    UPROPERTY(EditAnywhere, Category="山河纪元|配置")
    int32 DaysPerTick = 1;

    float DayAccumulator = 0.0f;
};
