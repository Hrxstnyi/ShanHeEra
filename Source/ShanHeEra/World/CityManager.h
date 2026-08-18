#pragma once
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Core/ShanHeStructs.h"
#include "CityManager.generated.h"

UENUM(BlueprintType)
enum class ECityLevel : uint8
{
    Village     UMETA(DisplayName="村镇"),
    Town        UMETA(DisplayName="小镇"),
    County      UMETA(DisplayName="县城"),
    Prefecture  UMETA(DisplayName="州府"),
    Capital     UMETA(DisplayName="京城"),
    Metropolis  UMETA(DisplayName="大都会")
};

USTRUCT(BlueprintType)
struct FCityData
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FName CityID;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FText CityName;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) ECityLevel Level;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FName ParentRegion;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 Population = 0;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) float Prosperity = 50.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) float Defense = 30.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FResourceStock Stock;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) bool bIsCapital = false;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) bool bIsPort = false;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) TArray<FName> Buildings;
};

UCLASS()
class SHANHEERA_API UCityManager : public UObject
{
    GENERATED_BODY()
public:
    UFUNCTION(BlueprintCallable, Category="山河纪元|城市")
    void InitializeCities();

    UFUNCTION(BlueprintCallable, Category="山河纪元|城市")
    FCityData GetCity(FName CityID) const;

    UFUNCTION(BlueprintCallable, Category="山河纪元|城市")
    TArray<FCityData> GetCitiesByRegion(FName RegionID) const;

    UPROPERTY(BlueprintReadOnly, Category="山河纪元|城市")
    TArray<FCityData> Cities;
};
