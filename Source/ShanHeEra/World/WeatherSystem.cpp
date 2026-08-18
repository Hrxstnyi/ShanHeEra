#include "World/WeatherSystem.h"
#include "Core/ShanHeLog.h"

void UWeatherSystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
    State.CurrentTerm = ESolarTerm::Lichun;
    State.CurrentWeather = EWeatherType::Clear;
    UE_LOG(LogShanHe, Log, TEXT("24节气天气系统初始化 - 立春"));
}

void UWeatherSystem::AdvanceDay()
{
    State.DaysInTerm++;
    if (State.DaysInTerm >= 15) // 每个节气约15天
    {
        State.DaysInTerm = 0;
        int32 Next = ((int32)State.CurrentTerm + 1) % 24;
        State.CurrentTerm = (ESolarTerm)Next;
        OnSolarTermChanged.Broadcast(State.CurrentTerm);
        UE_LOG(LogShanHe, Log, TEXT("节气变化：%d"), (int32)State.CurrentTerm);
    }
    UpdateWeather();
}

ESolarTerm UWeatherSystem::GetSolarTerm(int32 Month, int32 Day) const
{
    // 简化：每月两个节气
    int32 TermIndex = (Month - 1) * 2 + (Day > 15 ? 1 : 0);
    return (ESolarTerm)FMath::Clamp(TermIndex, 0, 23);
}

float UWeatherSystem::GetAgricultureBonus(ESolarTerm Term) const
{
    // 不同节气对农业的影响
    switch (Term)
    {
        case ESolarTerm::Lichun: return 0.8f;  // 立春，准备耕种
        case ESolarTerm::Qingming: return 1.2f; // 清明，播种好时节
        case ESolarTerm::Guyu: return 1.3f;    // 谷雨，雨水充足
        case ESolarTerm::Lixia: return 1.1f;   // 立夏，生长
        case ESolarTerm::Mangzhong: return 1.2f; // 芒种，忙碌
        case ESolarTerm::Xiazhi: return 1.0f;  // 夏至
        case ESolarTerm::Dashu: return 0.7f;   // 大暑，干旱风险
        case ESolarTerm::Liqiu: return 1.1f;   // 立秋
        case ESolarTerm::Qiufen: return 1.3f;   // 秋分，收获
        case ESolarTerm::Shuangjiang: return 1.2f; // 霜降，收获季
        case ESolarTerm::Lidong: return 0.5f;  // 立冬，农闲
        case ESolarTerm::Dongzhi: return 0.3f;  // 冬至，最冷
        default: return 1.0f;
    }
}

FText UWeatherSystem::GetTermDescription(ESolarTerm Term) const
{
    switch (Term)
    {
        case ESolarTerm::Lichun: return FText::FromString(TEXT("立春 - 万物复苏，春耕开始"));
        case ESolarTerm::Qingming: return FText::FromString(TEXT("清明 - 祭祖扫墓，踏青插柳"));
        case ESolarTerm::Dongzhi: return FText::FromString(TEXT("冬至 - 昼最短夜最长，吃饺子"));
        case ESolarTerm::Dashu: return FText::FromString(TEXT("大暑 - 一年最热，注意防暑"));
        default: return FText::GetEmpty();
    }
}

void UWeatherSystem::UpdateWeather()
{
    EWeatherType NewWeather = GetWeatherForTerm(State.CurrentTerm);
    if (NewWeather != State.CurrentWeather)
    {
        State.CurrentWeather = NewWeather;
        OnWeatherChanged.Broadcast(NewWeather);
    }
    // 温度随节气变化
    int32 TermInt = (int32)State.CurrentTerm;
    State.Temperature = 15.0f + 15.0f * FMath::Sin((TermInt - 3) * PI / 12.0f);
}

EWeatherType UWeatherSystem::GetWeatherForTerm(ESolarTerm Term) const
{
    int32 T = (int32)Term;
    if (T >= 0 && T <= 5) return FMath::RandRange(0, 10) < 3 ? EWeatherType::Rain : EWeatherType::Cloudy; // 春
    if (T >= 6 && T <= 11) return FMath::RandRange(0, 10) < 5 ? EWeatherType::HeavyRain : EWeatherType::Clear; // 夏
    if (T >= 12 && T <= 17) return FMath::RandRange(0, 10) < 2 ? EWeatherType::Fog : EWeatherType::Clear; // 秋
    return FMath::RandRange(0, 10) < 6 ? EWeatherType::Snow : EWeatherType::Cloudy; // 冬
}
