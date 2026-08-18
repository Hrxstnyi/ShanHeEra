#pragma once
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Core/ShanHeStructs.h"
#include "Core/ShanHeEnums.h"
#include "HistorySystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHistoricalEvent, FHistoricalEvent, Event);

UCLASS()
class SHANHEERA_API UHistorySystem : public UObject
{
    GENERATED_BODY()
public:
    UFUNCTION(BlueprintCallable, Category="山河纪元|历史")
    void InitializeHistory();

    UFUNCTION(BlueprintCallable, Category="山河纪元|历史")
    void CheckEvents();

    UFUNCTION(BlueprintCallable, Category="山河纪元|历史")
    void SimulateDisasters(int32 Days);

    UFUNCTION(BlueprintCallable, Category="山河纪元|历史")
    void TriggerDynastyChange();

    UPROPERTY(BlueprintReadOnly, Category="山河纪元|历史")
    TArray<FHistoricalEvent> HistoricalEvents;

    UPROPERTY(BlueprintReadOnly, Category="山河纪元|历史")
    TArray<FHistoricalEvent> TriggeredEvents;

    UPROPERTY(BlueprintAssignable, Category="山河纪元|历史")
    FOnHistoricalEvent OnHistoricalEventTriggered;

private:
    void LoadEasterEggEvents(); // 爱国彩蛋剧情
    void LoadTaiwanEvents();    // 台湾相关剧情
};
