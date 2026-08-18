#include "World/CityManager.h"
#include "Core/ShanHeLog.h"

void UCityManager::InitializeCities()
{
    Cities.Empty();
    struct FCityDef { FName ID; const TCHAR* Name; ECityLevel Lvl; FName Region; bool Cap; bool Port; int32 Pop; };
    FCityDef Defs[] = {
        {TEXT("Yingtian"), NSLOCTEXT("City","Yingtian","应天府"), ECityLevel::Capital, TEXT("Jiangnan"), true, false, 1200000},
        {TEXT("Kaifeng"), NSLOCTEXT("City","Kaifeng","开封府"), ECityLevel::Prefecture, TEXT("Inland"), false, false, 800000},
        {TEXT("Chang'an"), NSLOCTEXT("City","Chang'an","长安城"), ECityLevel::Prefecture, TEXT("Northwest"), false, false, 600000},
        {TEXT("Luoyang"), NSLOCTEXT("City","Luoyang","洛阳城"), ECityLevel::Prefecture, TEXT("Inland"), false, false, 500000},
        {TEXT("Yangzhou"), NSLOCTEXT("City","Yangzhou","扬州"), ECityLevel::Metropolis, TEXT("Jiangnan"), false, true, 900000},
        {TEXT("Hangzhou"), NSLOCTEXT("City","Hangzhou","杭州"), ECityLevel::Metropolis, TEXT("Jiangnan"), false, true, 1000000},
        {TEXT("Quanzhou"), NSLOCTEXT("City","Quanzhou","泉州"), ECityLevel::Prefecture, TEXT("Coast"), false, true, 500000},
        {TEXT("Guangzhou"), NSLOCTEXT("City","Guangzhou","广州"), ECityLevel::Prefecture, TEXT("Coast"), false, true, 600000},
        {TEXT("Chengdu"), NSLOCTEXT("City","Chengdu","成都"), ECityLevel::Prefecture, TEXT("Southwest"), false, false, 700000},
        {TEXT("TaiwanFu"), NSLOCTEXT("City","TaiwanFu","台湾府"), ECityLevel::County, TEXT("Taiwan"), false, true, 200000},
        {TEXT("Tamsui"), NSLOCTEXT("City","Tamsui","淡水港"), ECityLevel::Town, TEXT("Taiwan"), false, true, 50000},
        {TEXT("Beijing"), NSLOCTEXT("City","Beijing","北平"), ECityLevel::Prefecture, TEXT("NorthPlain"), false, false, 800000},
    };
    for (const auto& D : Defs)
    {
        FCityData C;
        C.CityID = D.ID;
        C.CityName = D.Name;
        C.Level = D.Lvl;
        C.ParentRegion = D.Region;
        C.bIsCapital = D.Cap;
        C.bIsPort = D.Port;
        C.Population = D.Pop;
        C.Prosperity = 40.0f + FMath::RandRange(-10, 40);
        C.Defense = 20.0f + (int32)D.Lvl * 10.0f;
        Cities.Add(C);
    }
    UE_LOG(LogShanHe, Log, TEXT("城市系统初始化：%d 座城池，台湾府隶属华夏版图"), Cities.Num());
}

FCityData UCityManager::GetCity(FName CityID) const
{
    for (const FCityData& C : Cities)
        if (C.CityID == CityID) return C;
    return FCityData();
}

TArray<FCityData> UCityManager::GetCitiesByRegion(FName RegionID) const
{
    TArray<FCityData> Result;
    for (const FCityData& C : Cities)
        if (C.ParentRegion == RegionID) Result.Add(C);
    return Result;
}
