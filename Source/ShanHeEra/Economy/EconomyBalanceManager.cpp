#include "Economy/EconomyBalanceManager.h"
#include "Core/ShanHeLog.h"

void UEconomyBalanceManager::InitializeBalance()
{
    // 初始化基础资源价格
    struct FPriceDef { FName ID; float Base; float Vol; };
    FPriceDef Defs[] = {
        {TEXT("Grain"),    500.0f, 0.30f},   // 粮食(每石) - 波动最大
        {TEXT("Rice"),     600.0f, 0.25f},   // 米
        {TEXT("Iron"),     2000.0f, 0.15f},  // 铁(每斤)
        {TEXT("Cloth"),    300.0f, 0.20f},   // 布(每匹)
        {TEXT("Silk"),     5000.0f, 0.10f},  // 丝绸(每匹)
        {TEXT("Salt"),     200.0f, 0.25f},   // 盐(每斤)
        {TEXT("Tea"),      1000.0f, 0.15f},  // 茶(每斤)
        {TEXT("Wine"),     800.0f, 0.10f},   // 酒(每坛)
        {TEXT("Oil"),      400.0f, 0.15f},   // 油(每斤)
        {TEXT("Sugar"),    600.0f, 0.20f},   // 糖(每斤)
        {TEXT("Coal"),     100.0f, 0.20f},   // 煤(每担)
        {TEXT("Wood"),     50.0f,  0.25f},   // 木材(每根)
        {TEXT("Porcelain"),3000.0f, 0.08f},  // 瓷器(每件)
        {TEXT("Medicine"), 2000.0f, 0.30f},  // 药材(每斤)
        {TEXT("Horse"),    50000.0f,0.10f},  // 马(每匹)
        {TEXT("Cattle"),   8000.0f, 0.10f},  // 牛(每头)
        {TEXT("Book"),     2000.0f, 0.05f},  // 书(每本)
        {TEXT("Weapon"),   3000.0f, 0.08f},  // 兵器(每件)
    };

    for (const auto& D : Defs)
    {
        FResourcePrice P;
        P.ResourceID = D.ID;
        P.BasePrice = D.Base;
        P.CurrentPrice = D.Base;
        P.MinPrice = D.Base * 0.3f;
        P.MaxPrice = D.Base * 3.0f;
        P.Volatility = D.Vol;
        P.Supply = 100.0f;
        P.Demand = 100.0f;
        Prices.Add(D.ID, P);
    }

    // 初始化税率
    CurrentTaxRate = FTaxRate();

    UE_LOG(LogShanHe, Log, TEXT("经济平衡管理器初始化 - %d 种资源价格体系就绪"), Prices.Num());
}

void UEconomyBalanceManager::SimulatePrices(int32 Days)
{
    for (auto& Elem : Prices)
    {
        FResourcePrice& P = Elem.Value;
        // 价格回归基础值(供需平衡时)
        float TargetPrice = P.BasePrice * (P.Demand / FMath::Max(1.0f, P.Supply));
        float Diff = TargetPrice - P.CurrentPrice;
        P.CurrentPrice += Diff * 0.1f * Days; // 每天调整10%的差距

        // 随机波动
        P.CurrentPrice += FMath::RandRange(-P.Volatility, P.Volatility) * P.BasePrice * Days * 0.01f;

        // 限制范围
        P.CurrentPrice = FMath::Clamp(P.CurrentPrice, P.MinPrice, P.MaxPrice);
    }

    UpdateInflation(Days);
    UpdateSupplyDemand();
}

float UEconomyBalanceManager::CalculatePrice(FName ResourceID, float Supply, float Demand, float SeasonFactor) const
{
    if (const FResourcePrice* P = Prices.Find(ResourceID))
    {
        // 核心公式：价格 = 基础价 * (需求/供给) * 季节因子 * 通胀因子
        float SupplyDemandRatio = Demand / FMath::Max(1.0f, Supply);
        float Price = P->BasePrice * SupplyDemandRatio * SeasonFactor * (1.0f + InflationRate);
        return FMath::Clamp(Price, P->MinPrice, P->MaxPrice);
    }
    return 1.0f;
}

float UEconomyBalanceManager::CalculateGrainPrice(int32 Population, float HarvestRate, float SeasonFactor) const
{
    // 粮价模型：
    // 价格 = 基准价 * (人口/收成) * 季节因子 * 通胀
    // 收成率 1.0 = 正常，<1.0 = 歉收，>1.0 = 丰收
    if (HarvestRate <= 0) HarvestRate = 0.1f;
    float BaseGrainPrice = 500.0f; // 每石500铜钱
    float PopulationFactor = Population / 1000000.0f; // 以百万人口为基准
    float Price = BaseGrainPrice * (PopulationFactor / HarvestRate) * SeasonFactor * (1.0f + InflationRate);
    return FMath::Clamp(Price, 150.0f, 5000.0f); // 最低150，最高5000(饥荒时)
}

float UEconomyBalanceManager::CalculateTradeProfit(FName FromCity, FName ToCity, FName Resource, int32 Quantity) const
{
    // 贸易利润 = (目的地价格 - 出发地价格) * 数量 - 运费 - 关税
    if (const FResourcePrice* P = Prices.Find(Resource))
    {
        // 简化：不同城市有不同的价格系数
        float FromPrice = P->CurrentPrice * FMath::RandRange(0.8f, 1.0f);
        float ToPrice = P->CurrentPrice * FMath::RandRange(1.0f, 1.3f);
        float UnitProfit = ToPrice - FromPrice;
        float GrossProfit = UnitProfit * Quantity;

        // 运费：距离越远越高(简化为固定比例)
        float TransportCost = GrossProfit * 0.15f;
        // 关税
        float Tax = GrossProfit * CurrentTaxRate.CustomsTax;

        return GrossProfit - TransportCost - Tax;
    }
    return 0.0f;
}

float UEconomyBalanceManager::CalculateTax(float Income, ECurrencyType CurrencyType) const
{
    // 简化：统一按商税计算
    return Income * CurrentTaxRate.CommercialTax;
}

float UEconomyBalanceManager::ConvertCurrency(float Amount, ECurrencyType From, ECurrencyType To) const
{
    // 先转成铜钱(基准)，再转成目标货币
    float InCopper = 0.0f;
    switch (From)
    {
        case ECurrencyType::Copper: InCopper = Amount; break;
        case ECurrencyType::Silver: InCopper = Amount * CopperToSilver; break;
        case ECurrencyType::Gold:   InCopper = Amount * CopperToSilver * SilverToGold; break;
        case ECurrencyType::Grain:  InCopper = Amount * GrainToCopper; break;
    }

    switch (To)
    {
        case ECurrencyType::Copper: return InCopper;
        case ECurrencyType::Silver: return InCopper / CopperToSilver;
        case ECurrencyType::Gold:   return InCopper / (CopperToSilver * SilverToGold);
        case ECurrencyType::Grain:  return InCopper / GrainToCopper;
        default: return InCopper;
    }
}

void UEconomyBalanceManager::UpdateSupplyDemand()
{
    // 供给和需求随时间变化
    for (auto& Elem : Prices)
    {
        FResourcePrice& P = Elem.Value;
        P.Supply += FMath::RandRange(-2.0f, 2.0f);
        P.Demand += FMath::RandRange(-2.0f, 2.0f);
        P.Supply = FMath::Clamp(P.Supply, 10.0f, 500.0f);
        P.Demand = FMath::Clamp(P.Demand, 10.0f, 500.0f);
    }
}

void UEconomyBalanceManager::UpdateInflation(int32 Days)
{
    // 通胀模型：货币供应量增加 -> 通胀上升
    // 战争/天灾 -> 供给减少 -> 通胀上升
    // 丰收 -> 粮价下降 -> 通缩压力
    InflationRate += FMath::RandRange(-0.001f, 0.002f) * Days;
    InflationRate = FMath::Clamp(InflationRate, -0.2f, 1.0f); // -20% ~ +100%
}

float UEconomyBalanceManager::GetSeasonFactor(int32 Month) const
{
    // 季节因子：不同季节对不同商品有影响
    // 粮食：秋收(9-10月)价格低，春荒(3-4月)价格高
    switch (Month)
    {
        case 1: case 2: return 1.2f;  // 冬季，粮价高
        case 3: case 4: return 1.3f;  // 春荒，粮价最高
        case 5: case 6: return 1.0f;  // 初夏，正常
        case 7: case 8: return 0.9f;  // 夏收，粮价下降
        case 9: case 10: return 0.7f; // 秋收，粮价最低
        case 11: case 12: return 0.9f;// 初冬，开始回升
        default: return 1.0f;
    }
}
