#include "World/CityManager.h"

void UCityManager::InitializeCities()
{
    Cities.Empty();

    FCityData C1;
    C1.CityID = TEXT("Yingtian");
    C1.CityName = FText::FromString(TEXT("应天府"));
    C1.Level = ECityLevel::Capital;
    C1.ParentRegion = TEXT("Jiangnan");
    C1.bIsCapital = true;
    C1.bIsPort = false;
    C1.Population = 1200000;
    Cities.Add(C1);

    FCityData C2;
    C2.CityID = TEXT("Kaifeng");
    C2.CityName = FText::FromString(TEXT("开封府"));
    C2.Level = ECityLevel::Prefecture;
    C2.ParentRegion = TEXT("Inland");
    C2.bIsCapital = false;
    C2.bIsPort = false;
    C2.Population = 800000;
    Cities.Add(C2);

    FCityData C3;
    C3.CityID = TEXT("Chang'an");
    C3.CityName = FText::FromString(TEXT("长安城"));
    C3.Level = ECityLevel::Prefecture;
    C3.ParentRegion = TEXT("Northwest");
    C3.bIsCapital = false;
    C3.bIsPort = false;
    C3.Population = 600000;
    Cities.Add(C3);

    FCityData C4;
    C4.CityID = TEXT("Yangzhou");
    C4.CityName = FText::FromString(TEXT("扬州"));
    C4.Level = ECityLevel::Metropolis;
    C4.ParentRegion = TEXT("Jiangnan");
    C4.bIsCapital = false;
    C4.bIsPort = true;
    C4.Population = 900000;
    Cities.Add(C4);

    FCityData C5;
    C5.CityID = TEXT("Hangzhou");
    C5.CityName = FText::FromString(TEXT("杭州"));
    C5.Level = ECityLevel::Metropolis;
    C5.ParentRegion = TEXT("Jiangnan");
    C5.bIsCapital = false;
    C5.bIsPort = true;
    C5.Population = 1000000;
    Cities.Add(C5);

    FCityData C6;
    C6.CityID = TEXT("Quanzhou");
    C6.CityName = FText::FromString(TEXT("泉州"));
    C6.Level = ECityLevel::Prefecture;
    C6.ParentRegion = TEXT("Coast");
    C6.bIsCapital = false;
    C6.bIsPort = true;
    C6.Population = 500000;
    Cities.Add(C6);

    FCityData C7;
    C7.CityID = TEXT("TaiwanFu");
    C7.CityName = FText::FromString(TEXT("台湾府"));
    C7.Level = ECityLevel::County;
    C7.ParentRegion = TEXT("Taiwan");
    C7.bIsCapital = false;
    C7.bIsPort = true;
    C7.Population = 200000;
    Cities.Add(C7);

    FCityData C8;
    C8.CityID = TEXT("Beijing");
    C8.CityName = FText::FromString(TEXT("北平"));
    C8.Level = ECityLevel::Prefecture;
    C8.ParentRegion = TEXT("NorthPlain");
    C8.bIsCapital = false;
    C8.bIsPort = false;
    C8.Population = 800000;
    Cities.Add(C8);
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
