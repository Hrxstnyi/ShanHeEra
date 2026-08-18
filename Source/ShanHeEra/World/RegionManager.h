#pragma once
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Core/ShanHeEnums.h"
#include "Core/ShanHeStructs.h"
#include "ShanHeRegionManager.generated.h"

USTRUCT(BlueprintType)
struct FRegionData
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FName RegionID;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FText RegionName;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) ERegionType Type;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 Population = 0;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) float Fertility = 50.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) float Wealth = 50.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) float Stability = 50.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FResourceStock Resources;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) TArray<FName> Cities;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) TArray<FName> Neighbors;
};

UCLASS()
class SHANHEERA_API URegionManager : public UObject
{
    GENERATED_BODY()
public:
    UFUNCTION(BlueprintCallable, Category="山河纪元|地域")
    void InitializeRegions();

    UFUNCTION(BlueprintCallable, Category="山河纪元|地域")
    FRegionData GetRegion(FName RegionID) const;

    UFUNCTION(BlueprintCallable, Category="山河纪元|地域")
    TArray<FRegionData> GetAllRegions() const { return Regions; }

    UPROPERTY(BlueprintReadOnly, Category="山河纪元|地域")
    TArray<FRegionData> Regions;
};
