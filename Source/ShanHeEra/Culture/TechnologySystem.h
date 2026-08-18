#pragma once
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ShanHeTechnologySystem.generated.h"

UENUM(BlueprintType)
enum class ETechCategory : uint8
{
    Agriculture UMETA(DisplayName="农业"),
    Metallurgy  UMETA(DisplayName="冶金"),
    Shipbuilding UMETA(DisplayName="造船"),
    Construction UMETA(DisplayName="建筑"),
    Medicine    UMETA(DisplayName="医学"),
    Printing    UMETA(DisplayName="印刷"),
    Weaponry    UMETA(DisplayName="兵器"),
    Textile     UMETA(DisplayName="纺织"),
    Navigation  UMETA(DisplayName="航海")
};

USTRUCT(BlueprintType)
struct FTechnology
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FName TechID;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FText TechName;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FText Description;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) ETechCategory Category;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 Tier = 1;           // 科技层级 1-5
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 ResearchCost = 100;  // 研发点数
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 CurrentProgress = 0;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) bool bIsResearched = false;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) TArray<FName> Prerequisites;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FText EffectDescription;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) bool bIsBreakthrough = false; // 突破性技术(触发彩蛋)
};

/**
 * 工匠技术研发系统 - 技术树和发明系统
 * 9大类技术，每类5层，技术突破触发历史彩蛋
 */
UCLASS()
class SHANHEERA_API UTechnologySystem : public UObject
{
    GENERATED_BODY()
public:
    UFUNCTION(BlueprintCallable, Category="山河纪元|技术")
    void InitializeTechTree();

    UFUNCTION(BlueprintCallable, Category="山河纪元|技术")
    void ResearchTech(FName TechID, int32 ResearchPoints);

    UFUNCTION(BlueprintCallable, Category="山河纪元|技术")
    bool CanResearch(FName TechID) const;

    UFUNCTION(BlueprintCallable, Category="山河纪元|技术")
    TArray<FTechnology> GetAvailableTechs() const;

    UFUNCTION(BlueprintCallable, Category="山河纪元|技术")
    TArray<FTechnology> GetTechsByCategory(ETechCategory Category) const;

    UFUNCTION(BlueprintCallable, Category="山河纪元|技术")
    float GetTechProgress(ETechCategory Category) const;

    UPROPERTY(BlueprintReadOnly, Category="山河纪元|技术")
    TArray<FTechnology> TechTree;

    UPROPERTY(BlueprintReadOnly, Category="山河纪元|技术")
    int32 TotalBreakthroughs = 0;

private:
    void OnTechResearched(FTechnology& Tech);
};
