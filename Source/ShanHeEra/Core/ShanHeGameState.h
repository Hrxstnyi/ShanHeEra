#pragma once
#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "Core/ShanHeStructs.h"
#include "ShanHeGameState.generated.h"

UCLASS()
class SHANHEERA_API AShanHeGameState : public AGameStateBase
{
    GENERATED_BODY()
public:
    AShanHeGameState();

    UFUNCTION(BlueprintCallable, Category="山河纪元|时间")
    void AdvanceDays(int32 Days);

    UFUNCTION(BlueprintCallable, Category="山河纪元|时间")
    FDate GetCurrentDate() const { return CurrentDate; }

    UFUNCTION(BlueprintCallable, Category="山河纪元|时间")
    void SetDate(const FDate& NewDate) { CurrentDate = NewDate; }

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="山河纪元|世界")
    FDate CurrentDate;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="山河纪元|世界")
    int32 TotalPopulation = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="山河纪元|世界")
    int32 ActiveWars = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="山河纪元|世界")
    float WorldStability = 50.0f;
};
