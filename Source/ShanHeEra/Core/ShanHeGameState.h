#pragma once
#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "Core/ShanHeStructs.h"
#include "Core/ShanHeEnums.h"
#include "World/WeatherSystem.h"
#include "ShanHeGameState.generated.h"

UCLASS()
class SHANHEERA_API AShanHeGameState : public AGameStateBase
{
    GENERATED_BODY()
public:
    AShanHeGameState();

    UFUNCTION(BlueprintCallable, Category="山河纪元|时间")
    void AdvanceDays(int32 Days);

    UFUNCTION(BlueprintPure, Category="山河纪元|时间")
    FDate GetCurrentDate() const { return CurrentDate; }

    UFUNCTION(BlueprintCallable, Category="山河纪元|时间")
    void SetDate(const FDate& NewDate) { CurrentDate = NewDate; }

    /** 获取当前季节 */
    UFUNCTION(BlueprintPure, Category="山河纪元|时间")
    ESeason GetCurrentSeason() const;

    /** 获取当前节气 */
    UFUNCTION(BlueprintPure, Category="山河纪元|时间")
    ESolarTerm GetCurrentSolarTerm() const;

    /** 获取格式化日期字符串 */
    UFUNCTION(BlueprintPure, Category="山河纪元|时间")
    FString GetFormattedDate() const;

    /** 获取季节中文名 */
    UFUNCTION(BlueprintPure, Category="山河纪元|时间")
    FString GetSeasonName() const;

    /** 获取节气中文名 */
    UFUNCTION(BlueprintPure, Category="山河纪元|时间")
    FString GetSolarTermName() const;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="山河纪元|世界")
    FDate CurrentDate;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="山河纪元|世界")
    int32 TotalPopulation = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="山河纪元|世界")
    int32 ActiveWars = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="山河纪元|世界")
    float WorldStability = 50.0f;
};
