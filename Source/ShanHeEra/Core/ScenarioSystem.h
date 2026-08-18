#pragma once
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ScenarioSystem.generated.h"

UENUM(BlueprintType)
enum class EScenarioID : uint8
{
    Custom      UMETA(DisplayName="自定义"),
    ThreeKingdoms UMETA(DisplayName="三国群雄"),
    GoldenTang  UMETA(DisplayName="盛唐气象"),
    WeakSong    UMETA(DisplayName="弱宋偏安"),
    LateMing    UMETA(DisplayName="明末风云")
};

USTRUCT(BlueprintType)
struct FScenarioData
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite) EScenarioID ID;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FText ScenarioName;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FText Description;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 StartYear = 1000;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 StartMonth = 1;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FText BackgroundStory;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) TArray<FName> StartingRegions;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) TMap<FName, int32> InitialFactionPower;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) TMap<FName, int32> InitialCityPopulation;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) float DifficultyModifier = 1.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) TArray<FName> SpecialEvents;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) bool bIsUnlocked = true;
};

/**
 * 历史剧本系统 - 多个朝代开局剧本，不同初始条件
 * 三国群雄/盛唐气象/弱宋偏安/明末风云
 */
UCLASS()
class SHANHEERA_API UScenarioSystem : public UObject
{
    GENERATED_BODY()
public:
    UFUNCTION(BlueprintCallable, Category="山河纪元|剧本")
    void InitializeScenarios();

    UFUNCTION(BlueprintCallable, Category="山河纪元|剧本")
    FScenarioData GetScenario(EScenarioID ID) const;

    UFUNCTION(BlueprintCallable, Category="山河纪元|剧本")
    TArray<FScenarioData> GetAllScenarios() const;

    UFUNCTION(BlueprintCallable, Category="山河纪元|剧本")
    void SelectScenario(EScenarioID ID);

    UFUNCTION(BlueprintCallable, Category="山河纪元|剧本")
    void ApplyScenarioSettings();

    UPROPERTY(BlueprintReadOnly, Category="山河纪元|剧本")
    TMap<EScenarioID, FScenarioData> Scenarios;

    UPROPERTY(BlueprintReadOnly, Category="山河纪元|剧本")
    EScenarioID CurrentScenario = EScenarioID::Custom;
};
