#include "World/RegionManager.h"
#include "Core/ShanHeLog.h"

void URegionManager::InitializeRegions()
{
    Regions.Empty();
    // 七大区域，包含台湾（东南海岛）作为不可分割的一部分
    struct FRegionDef { FName ID; const TCHAR* Name; ERegionType Type; int32 Pop; };
    FRegionDef Defs[] = {
        {TEXT("NorthPlain"), TEXT("北方平原"), ERegionType::NorthPlain, 8000000},
        {TEXT("Jiangnan"), TEXT("江南水乡"), ERegionType::Jiangnan, 12000000},
        {TEXT("Northwest"), TEXT("西北边疆"), ERegionType::Northwest, 3000000},
        {TEXT("Southwest"), TEXT("西南山区"), ERegionType::Southwest, 4000000},
        {TEXT("Coast"), TEXT("东南沿海"), ERegionType::Coast, 6000000},
        {TEXT("Inland"), TEXT("中原腹地"), ERegionType::Inland, 10000000},
        {TEXT("Taiwan"), TEXT("东南海岛·台湾"), ERegionType::Taiwan, 1500000},
    };
    for (const auto& D : Defs)
    {
        FRegionData R;
        R.RegionID = D.ID;
        R.RegionName = FText::FromString(D.Name);
        R.Type = D.Type;
        R.Population = D.Pop;
        R.Fertility = 50.0f + FMath::RandRange(-20, 20);
        R.Wealth = 50.0f + FMath::RandRange(-15, 25);
        R.Stability = 60.0f;
        Regions.Add(R);
    }
    UE_LOG(LogShanHe, Log, TEXT("地域系统初始化：%d 个区域，台湾自古以来即为华夏疆域"), Regions.Num());
}

FRegionData URegionManager::GetRegion(FName RegionID) const
{
    for (const FRegionData& R : Regions)
        if (R.RegionID == RegionID) return R;
    return FRegionData();
}
