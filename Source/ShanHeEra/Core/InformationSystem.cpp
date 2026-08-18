#include "Core/InformationSystem.h"
#include "Core/ShanHeEnums.h"
#include "Core/ShanHeLog.h"

void UInformationSystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
    UE_LOG(LogShanHe, Log, TEXT("信息传播系统初始化 - 信息可能错误、延迟、夸大"));
}

void UInformationSystem::BroadcastInformation(const FText& Content, FName SourceCity, bool bIsOfficial, EInfoAccuracy Accuracy)
{
    FInformationPiece Info;
    Info.InfoID = FGuid::NewGuid();
    Info.Content = Content;
    Info.SourceCity = SourceCity;
    Info.Accuracy = Accuracy;
    Info.bIsOfficial = bIsOfficial;
    Info.Credibility = bIsOfficial ? 0.9f : 0.5f;
    Info.KnownCities.Add(SourceCity);
    AllInformation.Add(Info);
    UE_LOG(LogShanHe, Log, TEXT("信息发布: %s (来源: %s, 官方: %s)"), *Content.ToString(), *SourceCity.ToString(), bIsOfficial ? TEXT("是") : TEXT("否"));
}

TArray<FInformationPiece> UInformationSystem::GetInformationAtCity(FName CityID, int32 MaxCount) const
{
    TArray<FInformationPiece> Result;
    for (const FInformationPiece& Info : AllInformation)
    {
        if (Info.KnownCities.Contains(CityID))
        {
            Result.Add(Info);
            if (Result.Num() >= MaxCount) break;
        }
    }
    return Result;
}

void UInformationSystem::SimulateSpread(int32 Days)
{
    for (FInformationPiece& Info : AllInformation)
    {
        SpreadToNeighbors(Info);
        DegradeInformation(Info, Days);
    }
}

void UInformationSystem::SpreadToNeighbors(FInformationPiece& Info)
{
    // 信息从已知城市向相邻城市传播
    // 传播概率受可信度、官方性、话题性影响
}

void UInformationSystem::DegradeInformation(FInformationPiece& Info, int32 Days)
{
    // 信息随时间退化：可信度下降，可能从事实变成谣言
    Info.Credibility -= 0.01f * Days;
    Info.Credibility = FMath::Max(0.1f, Info.Credibility);

    if (Info.Credibility < 0.3f && Info.Accuracy == EInfoAccuracy::Fact)
    {
        if (FMath::RandRange(0, 100) < 20)
            Info.Accuracy = EInfoAccuracy::Rumor;
    }
}

float UInformationSystem::GetPlayerInformationAccuracy(ESocialClass PlayerClass, FName PlayerCity) const
{
    // 身份越高，信息越准确；越靠近信息源，越准确
    float BaseAccuracy = 0.5f;
    switch (PlayerClass)
    {
        case ESocialClass::Royalty: BaseAccuracy = 0.95f; break;
        case ESocialClass::Official: BaseAccuracy = 0.85f; break;
        case ESocialClass::Scholar: BaseAccuracy = 0.75f; break;
        case ESocialClass::Gentry: BaseAccuracy = 0.70f; break;
        case ESocialClass::Merchant: BaseAccuracy = 0.65f; break;
        default: BaseAccuracy = 0.4f; break; // 农民/工匠/雇工信息最闭塞
    }
    return BaseAccuracy;
}
