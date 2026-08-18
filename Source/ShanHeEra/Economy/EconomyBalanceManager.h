#pragma once
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Core/ShanHeEnums.h"
#include "EconomyBalanceManager.generated.h"

UENUM(BlueprintType)
enum class ECurrencyType : uint8
{
    Copper  UMETA(DisplayName="铜钱"),
    Silver  UMETA(DisplayName="银两"),
    Gold    UMETA(DisplayName="金锭"),
    Grain   UMETA(DisplayName="粮食(石)")
};

USTRUCT(BlueprintType)
struct FResourcePrice
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FName ResourceID;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) float BasePrice = 1.0f;      // 基础价格(铜钱)
    UPROPERTY(EditAnywhere, BlueprintReadWrite) float CurrentPrice = 1.0f;   // 当前价格
    UPROPERTY(EditAnywhere, BlueprintReadWrite) float MinPrice = 0.2f;       // 最低价格
    UPROPERTY(EditAnywhere, BlueprintReadWrite) float MaxPrice = 10.0f;      // 最高价格
    UPROPERTY(EditAnywhere, BlueprintReadWrite) float Volatility = 0.1f;     // 波动率(0-1)
    UPROPERTY(EditAnywhere, BlueprintReadWrite) float Supply = 100.0f;       // 供给量
    UPROPERTY(EditAnywhere, BlueprintReadWrite) float Demand = 100.0f;       // 需求量
    UPROPERTY(EditAnywhere, BlueprintReadWrite) TArray<float> PriceHistory;      // 近12月价格
};

USTRUCT(BlueprintType)
struct FTaxRate
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite) float LandTax = 0.05f;       // 田税 5%
    UPROPERTY(EditAnywhere, BlueprintReadWrite) float CommercialTax = 0.03f; // 商税 3%
    UPROPERTY(EditAnywhere, BlueprintReadWrite) float SaltTax = 0.20f;       // 盐税 20%
    UPROPERTY(EditAnywhere, BlueprintReadWrite) float TeaTax = 0.10f;        // 茶税 10%
    UPROPERTY(EditAnywhere, BlueprintReadWrite) float CustomsTax = 0.05f;    // 关税 5%
    UPROPERTY(EditAnywhere, BlueprintReadWrite) float PollTax = 100;         // 人头税(铜钱/年)
    UPROPERTY(EditAnywhere, BlueprintReadWrite) float CorveeDays = 20;       // 徭役天数/年
};

/**
 * 经济平衡管理器
 * 负责：价格波动、通货膨胀、税收计算、贸易利润、粮价模型
 * 所有数值经过平衡设计，确保游戏经济不会崩溃
 */
UCLASS()
class SHANHEERA_API UEconomyBalanceManager : public UObject
{
    GENERATED_BODY()
public:
    UFUNCTION(BlueprintCallable, Category="山河纪元|经济平衡")
    void InitializeBalance();

    UFUNCTION(BlueprintCallable, Category="山河纪元|经济平衡")
    void SimulatePrices(int32 Days);

    UFUNCTION(BlueprintCallable, Category="山河纪元|经济平衡")
    float CalculatePrice(FName ResourceID, float Supply, float Demand, float SeasonFactor) const;

    UFUNCTION(BlueprintCallable, Category="山河纪元|经济平衡")
    float CalculateGrainPrice(int32 Population, float HarvestRate, float SeasonFactor) const;

    UFUNCTION(BlueprintCallable, Category="山河纪元|经济平衡")
    float CalculateTradeProfit(FName FromCity, FName ToCity, FName Resource, int32 Quantity) const;

    UFUNCTION(BlueprintCallable, Category="山河纪元|经济平衡")
    float CalculateTax(float Income, ECurrencyType CurrencyType) const;

    UFUNCTION(BlueprintCallable, Category="山河纪元|经济平衡")
    float ConvertCurrency(float Amount, ECurrencyType From, ECurrencyType To) const;

    UFUNCTION(BlueprintCallable, Category="山河纪元|经济平衡")
    float GetInflationRate() const { return InflationRate; }

    UPROPERTY(BlueprintReadOnly, Category="山河纪元|经济平衡")
    TMap<FName, FResourcePrice> Prices;

    UPROPERTY(BlueprintReadOnly, Category="山河纪元|经济平衡")
    FTaxRate CurrentTaxRate;

    UPROPERTY(BlueprintReadOnly, Category="山河纪元|经济平衡")
    float InflationRate = 0.0f;

    UPROPERTY(EditAnywhere, Category="山河纪元|经济平衡|汇率")
    float CopperToSilver = 1000.0f;  // 1000铜钱 = 1两银
    UPROPERTY(EditAnywhere, Category="山河纪元|经济平衡|汇率")
    float SilverToGold = 10.0f;      // 10两银 = 1两金
    UPROPERTY(EditAnywhere, Category="山河纪元|经济平衡|汇率")
    float GrainToCopper = 500.0f;    // 1石粮 = 500铜钱(基准)

private:
    void UpdateSupplyDemand();
    void UpdateInflation(int32 Days);
    float GetSeasonFactor(int32 Month) const;
};
