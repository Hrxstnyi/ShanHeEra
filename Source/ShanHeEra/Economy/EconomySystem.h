#pragma once
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Core/ShanHeStructs.h"
#include "ShanHeEconomySystem.generated.h"

USTRUCT(BlueprintType)
struct FPriceData
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite) float FoodPrice = 1.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) float IronPrice = 5.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) float ClothPrice = 3.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) float SaltPrice = 2.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) float TeaPrice = 4.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) float SilkPrice = 10.0f;
};

USTRUCT(BlueprintType)
struct FTradeRoute
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FName RouteID;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FName FromCity;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FName ToCity;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FText Goods;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 TradeVolume = 0;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) float Safety = 80.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) bool bIsSeaRoute = false;
};

UCLASS()
class SHANHEERA_API UEconomySystem : public UObject
{
    GENERATED_BODY()
public:
    UFUNCTION(BlueprintCallable, Category="山河纪元|经济")
    void InitializeEconomy();

    UFUNCTION(BlueprintCallable, Category="山河纪元|经济")
    void Simulate(int32 Days);

    UFUNCTION(BlueprintCallable, Category="山河纪元|经济")
    float GetPrice(FName Resource) const;

    UFUNCTION(BlueprintCallable, Category="山河纪元|经济")
    void UpdatePrices();

    UPROPERTY(BlueprintReadOnly, Category="山河纪元|经济")
    FPriceData Prices;

    UPROPERTY(BlueprintReadOnly, Category="山河纪元|经济")
    TArray<FTradeRoute> TradeRoutes;

    UPROPERTY(BlueprintReadOnly, Category="山河纪元|经济")
    float NationalTreasury = 100000.0f;

    UPROPERTY(BlueprintReadOnly, Category="山河纪元|经济")
    float InflationRate = 0.0f;

private:
    void SimulateAgriculture(int32 Days);
    void SimulateCommerce(int32 Days);
    void SimulateTaxation(int32 Days);
    void SimulateCurrency(int32 Days);
};
