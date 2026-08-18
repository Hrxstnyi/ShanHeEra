#pragma once
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Family/FamilyTree.h"
#include "Core/ShanHeStructs.h"
#include "ShanHeInheritanceSystem.generated.h"

USTRUCT(BlueprintType)
struct FHeritageDistribution
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FGuid HeirID;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) float WealthShare = 0.0f;    // 财产份额(0-1)
    UPROPERTY(EditAnywhere, BlueprintReadWrite) float LandShare = 0.0f;      // 土地份额
    UPROPERTY(EditAnywhere, BlueprintReadWrite) float ReputationShare = 0.0f; // 声望份额
    UPROPERTY(EditAnywhere, BlueprintReadWrite) bool bInheritsTitle = false;  // 是否继承头衔
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FText InheritedItem;          // 传家宝
};

/**
 * 继承系统 - 处理家族成员死亡后的财产、头衔、声望传承
 * 支持多种继承方式：长子继承、诸子均分、幼子继承、选贤继承
 */
UCLASS()
class SHANHEERA_API UInheritanceSystem : public UObject
{
    GENERATED_BODY()
public:
    UFUNCTION(BlueprintCallable, Category="山河纪元|继承")
    TArray<FHeritageDistribution> DistributeEstate(
        FGuid DeceasedID,
        UFamilyTree* FamilyTree,
        EInheritanceType InheritanceType,
        const FResourceStock& TotalWealth,
        float TotalLand,
        int32 TotalReputation);

    UFUNCTION(BlueprintCallable, Category="山河纪元|继承")
    FGuid DetermineHeir(FGuid DeceasedID, UFamilyTree* FamilyTree, EInheritanceType Type) const;

    UFUNCTION(BlueprintCallable, Category="山河纪元|继承")
    FResourceStock CalculateInheritance(FGuid HeirID, const TArray<FHeritageDistribution>& Distribution,
                                        const FResourceStock& TotalWealth) const;

    UFUNCTION(BlueprintCallable, Category="山河纪元|继承")
    void TransferFamilySkills(FGuid FromID, FGuid ToID, float TransferRate = 0.5f);

private:
    TArray<FHeritageDistribution> DistributePrimogeniture(FGuid DeceasedID, UFamilyTree* Tree,
        const FResourceStock& Wealth, float Land, int32 Reputation);
    TArray<FHeritageDistribution> DistributeEqualDivision(FGuid DeceasedID, UFamilyTree* Tree,
        const FResourceStock& Wealth, float Land, int32 Reputation);
};
