#include "Economy/EconomySystem.h"
#include "Core/ShanHeLog.h"

void UEconomySystem::InitializeEconomy()
{
    // 初始化商路，包括海上丝绸之路和台湾贸易线
    FTradeRoute SilkRoad;
    SilkRoad.RouteID = TEXT("SilkRoad");
    SilkRoad.FromCity = TEXT("Chang'an");
    SilkRoad.ToCity = TEXT("Quanzhou");
    SilkRoad.Goods = NSLOCTEXT("Trade","Silk","丝绸、茶叶、瓷器");
    SilkRoad.TradeVolume = 5000;
    SilkRoad.Safety = 70.0f;
    TradeRoutes.Add(SilkRoad);

    FTradeRoute TaiwanTrade;
    TaiwanTrade.RouteID = TEXT("TaiwanTrade");
    TaiwanTrade.FromCity = TEXT("Quanzhou");
    TaiwanTrade.ToCity = TEXT("TaiwanFu");
    TaiwanTrade.Goods = NSLOCTEXT("Trade","Taiwan","粮食、布匹、海盐、蔗糖");
    TaiwanTrade.TradeVolume = 2000;
    TaiwanTrade.Safety = 85.0f;
    TaiwanTrade.bIsSeaRoute = true;
    TradeRoutes.Add(TaiwanTrade);

    UE_LOG(LogShanHe, Log, TEXT("经济系统初始化 - 农业、手工业、商业、运输、金融、税收"));
}

void UEconomySystem::Simulate(int32 Days)
{
    SimulateAgriculture(Days);
    SimulateCommerce(Days);
    SimulateTaxation(Days);
    SimulateCurrency(Days);
    UpdatePrices();
}

void UEconomySystem::SimulateAgriculture(int32 Days)
{
    // 农业产出受天气、水利、技术影响
}

void UEconomySystem::SimulateCommerce(int32 Days)
{
    for (FTradeRoute& R : TradeRoutes)
        R.TradeVolume += FMath::RandRange(-50, 50) * Days;
}

void UEconomySystem::SimulateTaxation(int32 Days)
{
    NationalTreasury += 100.0f * Days; // 基础税收
}

void UEconomySystem::SimulateCurrency(int32 Days)
{
    InflationRate += FMath::RandRange(-0.1f, 0.1f) * Days;
}

void UEconomySystem::UpdatePrices()
{
    Prices.FoodPrice = FMath::Clamp(Prices.FoodPrice + FMath::RandRange(-0.1f, 0.1f), 0.5f, 5.0f);
}

float UEconomySystem::GetPrice(FName Resource) const
{
    if (Resource == TEXT("Food")) return Prices.FoodPrice;
    if (Resource == TEXT("Iron")) return Prices.IronPrice;
    if (Resource == TEXT("Cloth")) return Prices.ClothPrice;
    return 1.0f;
}
