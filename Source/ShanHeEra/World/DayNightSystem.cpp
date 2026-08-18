#include "World/DayNightSystem.h"
#include "Core/ShanHeLog.h"

void UDayNightSystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
    UE_LOG(LogShanHe, Log, TEXT("昼夜循环系统初始化 - 一天=%.0f分钟, 当前%.1f时"), DayLengthInMinutes, CurrentHour);
}

void UDayNightSystem::Tick(float DeltaTime)
{
    // 时间推进
    float HoursPerSecond = 24.0f / (DayLengthInMinutes * 60.0f);
    CurrentHour += DeltaTime * HoursPerSecond * TimeScale;

    if (CurrentHour >= 24.0f)
    {
        CurrentHour -= 24.0f;
        CurrentDay++;
        UE_LOG(LogShanHe, Log, TEXT("新的一天：第 %d 天"), CurrentDay);
    }

    int32 IntHour = (int32)CurrentHour;
    if (IntHour != LastHour)
    {
        LastHour = IntHour;
        OnHourChanged.Broadcast(IntHour);
    }

    ETimeOfDay NewTOD = GetTimeOfDay();
    if (NewTOD != LastTimeOfDay)
    {
        LastTimeOfDay = NewTOD;
        OnTimeOfDayChanged.Broadcast(NewTOD);
    }

    UpdateLighting();
}

void UDayNightSystem::SetTimeOfDay(float InHour)
{
    CurrentHour = FMath::Clamp(InHour, 0.0f, 24.0f);
    UE_LOG(LogShanHe, Log, TEXT("设置时间：%.1f时"), CurrentHour);
}

ETimeOfDay UDayNightSystem::GetTimeOfDay() const
{
    if (CurrentHour >= 5.0f && CurrentHour < 7.0f) return ETimeOfDay::Dawn;
    if (CurrentHour >= 7.0f && CurrentHour < 11.0f) return ETimeOfDay::Morning;
    if (CurrentHour >= 11.0f && CurrentHour < 14.0f) return ETimeOfDay::Noon;
    if (CurrentHour >= 14.0f && CurrentHour < 17.0f) return ETimeOfDay::Afternoon;
    if (CurrentHour >= 17.0f && CurrentHour < 20.0f) return ETimeOfDay::Dusk;
    if (CurrentHour >= 20.0f && CurrentHour < 23.0f) return ETimeOfDay::Night;
    return ETimeOfDay::Midnight;
}

float UDayNightSystem::GetSunIntensity() const
{
    // 日出到日落之间有阳光
    if (CurrentHour < SunriseHour || CurrentHour > SunsetHour) return 0.0f;
    float DayProgress = (CurrentHour - SunriseHour) / (SunsetHour - SunriseHour);
    // 正午最强，早晚较弱（正弦曲线）
    return FMath::Sin(DayProgress * PI);
}

float UDayNightSystem::GetMoonIntensity() const
{
    if (CurrentHour > SunriseHour && CurrentHour < SunsetHour) return 0.0f;
    // 午夜最强
    float NightCenter = 0.0f; // 午夜0点
    float Dist = FMath::Abs(CurrentHour - NightCenter);
    if (Dist > 12.0f) Dist = 24.0f - Dist;
    return FMath::Max(0.0f, 1.0f - Dist / 6.0f) * 0.3f;
}

FLinearColor UDayNightSystem::GetAmbientColor() const
{
    ETimeOfDay TOD = GetTimeOfDay();
    switch (TOD)
    {
        case ETimeOfDay::Dawn: return FLinearColor(1.0f, 0.7f, 0.5f); // 橙红
        case ETimeOfDay::Morning: return FLinearColor(0.9f, 0.95f, 1.0f); // 淡蓝
        case ETimeOfDay::Noon: return FLinearColor(1.0f, 1.0f, 0.95f); // 亮白
        case ETimeOfDay::Afternoon: return FLinearColor(1.0f, 0.95f, 0.85f); // 暖白
        case ETimeOfDay::Dusk: return FLinearColor(1.0f, 0.5f, 0.3f); // 橘红
        case ETimeOfDay::Night: return FLinearColor(0.1f, 0.1f, 0.3f); // 深蓝
        case ETimeOfDay::Midnight: return FLinearColor(0.05f, 0.05f, 0.15f); // 深紫蓝
        default: return FLinearColor::White;
    }
}

float UDayNightSystem::GetFogDensity() const
{
    ETimeOfDay TOD = GetTimeOfDay();
    switch (TOD)
    {
        case ETimeOfDay::Dawn: return 0.5f; // 晨雾
        case ETimeOfDay::Dusk: return 0.3f; // 暮霭
        case ETimeOfDay::Night: return 0.2f;
        case ETimeOfDay::Midnight: return 0.15f;
        default: return 0.1f;
    }
}

bool UDayNightSystem::IsNight() const
{
    return CurrentHour < SunriseHour || CurrentHour > SunsetHour;
}

void UDayNightSystem::UpdateLighting()
{
    // 实际项目中这里会更新DirectionalLight、SkyLight、ExponentialHeightFog、SkyAtmosphere等
    // 目前通过委托通知蓝图更新
}
