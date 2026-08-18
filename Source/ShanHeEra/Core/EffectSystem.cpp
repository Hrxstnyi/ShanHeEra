#include "Core/EffectSystem.h"
#include "Core/ShanHeLog.h"

void UEffectSystem::InitializeEffects()
{
    struct FEffectDef { FName ID; EEffectType Type; const TCHAR* Path; float Dur; bool Loop; };
    FEffectDef Effects[] = {
        {TEXT("Fire_Small"), EEffectType::Fire, TEXT("/Game/Effects/Fire/Fire_Small"), 0.5f, true},
        {TEXT("Fire_Large"), EEffectType::Fire, TEXT("/Game/Effects/Fire/Fire_Large"), 1.0f, true},
        {TEXT("Water_Splash"), EEffectType::Water, TEXT("/Game/Effects/Water/Water_Splash"), 0.3f, false},
        {TEXT("Water_River"), EEffectType::Water, TEXT("/Game/Effects/Water/Water_River"), 1.0f, true},
        {TEXT("Smoke_Campfire"), EEffectType::Smoke, TEXT("/Game/Effects/Smoke/Smoke_Campfire"), 2.0f, true},
        {TEXT("Smoke_Battle"), EEffectType::Smoke, TEXT("/Game/Effects/Smoke/Smoke_Battle"), 3.0f, true},
        {TEXT("Explosion_Cannon"), EEffectType::Explosion, TEXT("/Game/Effects/Explosion/Explosion_Cannon"), 1.5f, false},
        {TEXT("Blood_Hit"), EEffectType::Blood, TEXT("/Game/Effects/Blood/Blood_Hit"), 0.3f, false},
        {TEXT("Skill_Sword"), EEffectType::Skill, TEXT("/Game/Effects/Skill/Skill_Sword"), 0.5f, false},
        {TEXT("Skill_Magic"), EEffectType::Magic, TEXT("/Game/Effects/Skill/Skill_Magic"), 1.0f, false},
        {TEXT("Weather_Rain"), EEffectType::Weather, TEXT("/Game/Effects/Weather/Weather_Rain"), 1.0f, true},
        {TEXT("Weather_Snow"), EEffectType::Weather, TEXT("/Game/Effects/Weather/Weather_Snow"), 1.0f, true},
        {TEXT("Weather_Leaves"), EEffectType::Weather, TEXT("/Game/Effects/Weather/Weather_Leaves"), 1.0f, true},
    };

    for (const auto& E : Effects)
    {
        FEffectData Data;
        Data.EffectID = E.ID; Data.Type = E.Type;
        Data.NiagaraSystemPath = E.Path;
        Data.Duration = E.Dur; Data.bIsLooping = E.Loop;
        EffectDatabase.Add(E.ID, Data);
    }

    UE_LOG(LogShanHe, Log, TEXT("特效系统初始化 - %d 种Niagara特效"), EffectDatabase.Num());
}

void UEffectSystem::SpawnEffect(FName EffectID, FVector Location, FRotator Rotation, float Scale)
{
    if (const FEffectData* Data = EffectDatabase.Find(EffectID))
    {
        ActiveEffects++;
        UE_LOG(LogShanHe, Log, TEXT("生成特效：%s at %s"), *EffectID.ToString(), *Location.ToString());
    }
}

void UEffectSystem::SpawnWeatherEffect(EEffectType WeatherType, FVector Location)
{
    FName EffectID;
    switch (WeatherType)
    {
        case EEffectType::Weather: EffectID = TEXT("Weather_Rain"); break;
        default: EffectID = TEXT("Weather_Rain"); break;
    }
    SpawnEffect(EffectID, Location, FRotator::ZeroRotator, 1.0f);
}

void UEffectSystem::StopAllEffects()
{
    ActiveEffects = 0;
    UE_LOG(LogShanHe, Log, TEXT("停止所有特效"));
}
