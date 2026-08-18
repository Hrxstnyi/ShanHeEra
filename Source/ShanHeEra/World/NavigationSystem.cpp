#include "World/NavigationSystem.h"
#include "Core/ShanHeLog.h"

void UNavigationSystem::InitializeNavigation()
{
    // 初始化航线
    FSeaRoute TaiwanRoute;
    TaiwanRoute.RouteID = TEXT("Quanzhou_Taiwan");
    TaiwanRoute.FromPort = TEXT("Quanzhou");
    TaiwanRoute.ToPort = TEXT("TaiwanFu");
    TaiwanRoute.Distance = 3;
    TaiwanRoute.Danger = 0.2f;
    TaiwanRoute.Profit = 0.25f;
    TaiwanRoute.bIsDiscovered = true;
    SeaRoutes.Add(TaiwanRoute);

    FSeaRoute Nanyang;
    Nanyang.RouteID = TEXT("Quanzhou_Nanyang");
    Nanyang.FromPort = TEXT("Quanzhou");
    Nanyang.ToPort = TEXT("Nanyang");
    Nanyang.Distance = 15;
    Nanyang.Danger = 0.5f;
    Nanyang.Profit = 0.6f;
    Nanyang.bIsDiscovered = false;
    SeaRoutes.Add(Nanyang);

    FSeaRoute IndianOcean;
    IndianOcean.RouteID = TEXT("Quanzhou_India");
    IndianOcean.FromPort = TEXT("Quanzhou");
    IndianOcean.ToPort = TEXT("India");
    IndianOcean.Distance = 30;
    IndianOcean.Danger = 0.7f;
    IndianOcean.Profit = 1.0f;
    IndianOcean.bIsDiscovered = false;
    IndianOcean.RequiredTechs = {TEXT("Compass"), TEXT("TreasureShip")};
    SeaRoutes.Add(IndianOcean);

    UE_LOG(LogShanHe, Log, TEXT("航海系统初始化 - %d 条航线，关联郑和下西洋彩蛋"), SeaRoutes.Num());
}

void UNavigationSystem::Simulate(int32 Days)
{
    UpdateVoyages(Days);
    CheckPirateAttacks();
    CheckStorms();
}

FShipData UNavigationSystem::BuildShip(FName ShipType, FText ShipName)
{
    FShipData Ship;
    Ship.ShipID = FName(*FString::Printf(TEXT("Ship_%d"), FDateTime::Now().GetTicks()));
    Ship.ShipName = ShipName;
    Ship.ShipType = ShipType;

    if (ShipType == TEXT("Junk")) // 福船
    {
        Ship.Capacity = 200; Ship.Crew = 30; Ship.Speed = 6; Ship.Durability = 80;
    }
    else if (ShipType == TEXT("TreasureShip")) // 宝船
    {
        Ship.Capacity = 1000; Ship.Crew = 200; Ship.Speed = 4; Ship.Durability = 150;
    }
    else // 沙船
    {
        Ship.Capacity = 100; Ship.Crew = 15; Ship.Speed = 8; Ship.Durability = 60;
    }

    Ships.Add(Ship);
    UE_LOG(LogShanHe, Log, TEXT("造船完成：%s"), *ShipName.ToString());
    return Ship;
}

bool UNavigationSystem::StartVoyage(FName ShipID, FName RouteID, int32 CargoValue)
{
    const FSeaRoute* Route = nullptr;
    for (const FSeaRoute& R : SeaRoutes)
        if (R.RouteID == RouteID && R.bIsDiscovered) { Route = &R; break; }
    if (!Route) return false;

    FVoyage Voyage;
    Voyage.VoyageID = FName(*FString::Printf(TEXT("Voyage_%d"), FDateTime::Now().GetTicks()));
    Voyage.ShipID = ShipID;
    Voyage.RouteID = RouteID;
    Voyage.DaysRemaining = Route->Distance;
    Voyage.CargoValue = CargoValue;
    ActiveVoyages.Add(Voyage);
    TotalVoyages++;

    UE_LOG(LogShanHe, Log, TEXT("出航！%s -> %s，货值 %d"), *Route->FromPort.ToString(), *Route->ToPort.ToString(), CargoValue);
    return true;
}

void UNavigationSystem::DiscoverRoute(FName RouteID)
{
    for (FSeaRoute& R : SeaRoutes)
    {
        if (R.RouteID == RouteID && !R.bIsDiscovered)
        {
            R.bIsDiscovered = true;
            UE_LOG(LogShanHe, Log, TEXT("发现新航线：%s -> %s"), *R.FromPort.ToString(), *R.ToPort.ToString());
            return;
        }
    }
}

TArray<FSeaRoute> UNavigationSystem::GetDiscoveredRoutes() const
{
    TArray<FSeaRoute> Result;
    for (const FSeaRoute& R : SeaRoutes)
        if (R.bIsDiscovered) Result.Add(R);
    return Result;
}

void UNavigationSystem::UpdateVoyages(int32 Days)
{
    for (int32 i = ActiveVoyages.Num() - 1; i >= 0; i--)
    {
        FVoyage& V = ActiveVoyages[i];
        V.DaysRemaining -= Days;
        if (V.DaysRemaining <= 0)
        {
            UE_LOG(LogShanHe, Log, TEXT("航行完成！利润：%d"), (int32)(V.CargoValue * 0.3f));
            ActiveVoyages.RemoveAt(i);
        }
    }
}

void UNavigationSystem::CheckPirateAttacks()
{
    for (FVoyage& V : ActiveVoyages)
    {
        if (FMath::RandRange(0.0f, 1.0f) < 0.02f)
        {
            V.bIsInDanger = true;
            UE_LOG(LogShanHe, Warning, TEXT("遭遇海盗！航次 %s"), *V.VoyageID.ToString());
        }
    }
}

void UNavigationSystem::CheckStorms()
{
    for (FVoyage& V : ActiveVoyages)
    {
        if (FMath::RandRange(0.0f, 1.0f) < 0.03f)
        {
            V.bIsInDanger = true;
            UE_LOG(LogShanHe, Warning, TEXT("遭遇风暴！航次 %s"), *V.VoyageID.ToString());
        }
    }
}
