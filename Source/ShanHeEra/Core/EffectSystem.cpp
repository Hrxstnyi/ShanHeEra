#include "Core/EffectSystem.h"
#include "Core/ShanHeLog.h"
UEffectSystem::UEffectSystem() {}
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
        {TEXT("Explosion_Giant"), EEffectType::Explosion, TEXT("/Game/Effects/Explosion/Explosion_Giant"), 2.5f, false},
        {TEXT("Blood_Hit"), EEffectType::Blood, TEXT("/Game/Effects/Blood/Blood_Hit"), 0.3f, false},
        {TEXT("Blood_Spray"), EEffectType::Blood, TEXT("/Game/Effects/Blood/Blood_Spray"), 0.5f, false},
        {TEXT("Skill_Sword"), EEffectType::Skill, TEXT("/Game/Effects/Skill/Skill_Sword"), 0.5f, false},
        {TEXT("Skill_Magic"), EEffectType::Magic, TEXT("/Game/Effects/Skill/Skill_Magic"), 1.0f, false},
        {TEXT("Skill_Archery"), EEffectType::Skill, TEXT("/Game/Effects/Skill/Skill_Archery"), 0.3f, false},
        {TEXT("Weather_Rain"), EEffectType::Weather, TEXT("/Game/Effects/Weather/Weather_Rain"), 1.0f, true},
        {TEXT("Weather_Snow"), EEffectType::Weather, TEXT("/Game/Effects/Weather/Weather_Snow"), 1.0f, true},
        {TEXT("Weather_Leaves"), EEffectType::Weather, TEXT("/Game/Effects/Weather/Weather_Leaves"), 1.0f, true},
        {TEXT("Weather_Sandstorm"), EEffectType::Weather, TEXT("/Game/Effects/Weather/Weather_Sandstorm"), 1.0f, true},
        {TEXT("UI_Click"), EEffectType::UI, TEXT("/Game/Effects/UI/UI_Click"), 0.2f, false},
        {TEXT("UI_LevelUp"), EEffectType::UI, TEXT("/Game/Effects/UI/UI_LevelUp"), 1.0f, false},
    };
    for (const auto& E : Effects)
    {
        FEffectData Data; Data.EffectID=E.ID; Data.Type = E.Type; Data.NiagaraSystemPath=E.Path;
        Data.Duration=E.Dur; Data.bIsLooping=E.Loop; EffectDatabase.Add(E.ID, Data);
    }
}
void UEffectSystem::SpawnEffect(FName EffectID, FVector Location, FRotator Rotation, float Scale)
{
    if (EffectDatabase.Find(EffectID)) ActiveEffects++;
}
void UEffectSystem::SpawnWeatherEffect(EEffectType WeatherType, FVector Location)
{
    FName EffectID = (WeatherType==EEffectType::Weather) ? TEXT("Weather_Rain") : TEXT("Weather_Rain");
    SpawnEffect(EffectID, Location, FRotator::ZeroRotator, 1.0f);
}
void UEffectSystem::StopAllEffects() { ActiveEffects=0; ActivePostEffects.Empty(); }
void UEffectSystem::ApplyPostEffect(EPostEffectType Type, float Intensity, float Duration)
{
    FPostEffectData Effect; Effect.Type=Type; Effect.Intensity=Intensity; Effect.Duration=Duration;
    ActivePostEffects.Add(Effect);
}
void UEffectSystem::SetDepthOfField(bool bEnabled, float FocalDistance, float Aperture) { bDOFEnabled=bEnabled; DOFFocalDistance=FocalDistance; DOFAperture=Aperture; }
void UEffectSystem::SetMotionBlur(float Amount) { CurrentMotionBlur=FMath::Clamp(Amount,0.f,1.f); }
void UEffectSystem::SetBloom(float Intensity) { CurrentBloom=FMath::Clamp(Intensity,0.f,2.f); }
void UEffectSystem::SetChromaticAberration(float Intensity) { ChromaticAberration=FMath::Clamp(Intensity,0.f,1.f); }
void UEffectSystem::SetVignette(float Intensity) { VignetteIntensity=FMath::Clamp(Intensity,0.f,1.f); }
void UEffectSystem::SetFilmGrain(float Intensity) { FilmGrainIntensity=FMath::Clamp(Intensity,0.f,1.f); }
void UEffectSystem::ApplyColorGrade(EColorGrade Grade, float TransitionTime) { TargetColorGrade=Grade; ColorGradeTransition=0.f; ColorGradeTransitionDuration=TransitionTime; }
void UEffectSystem::SetSaturation(float Saturation) { CurrentSaturation=FMath::Clamp(Saturation,-1.f,1.f); }
void UEffectSystem::SetColorTemperature(float Temp) { CurrentColorTemp=FMath::Clamp(Temp,-1.f,1.f); }
void UEffectSystem::SetContrast(float Contrast) { CurrentContrast=FMath::Clamp(Contrast,-1.f,1.f); }
void UEffectSystem::ScreenShake(float Intensity, float Duration) { ApplyPostEffect(EPostEffectType::ScreenShake, Intensity, Duration); }
void UEffectSystem::FlashWhite(float Intensity, float Duration) { FlashIntensity=Intensity; FlashColor=FLinearColor::White; ApplyPostEffect(EPostEffectType::FlashWhite, Intensity, Duration); }
void UEffectSystem::FlashRed(float Intensity, float Duration) { FlashIntensity=Intensity; FlashColor=FLinearColor::Red; ApplyPostEffect(EPostEffectType::FlashRed, Intensity, Duration); }
void UEffectSystem::SetSlowMotion(float Factor, float Duration) { SlowMotionFactor=FMath::Clamp(Factor,0.01f,1.f); SlowMotionTime=Duration; }
void UEffectSystem::BulletTime(float Duration) { SetSlowMotion(0.05f, Duration); SetMotionBlur(1.f); SetChromaticAberration(0.3f); }
void UEffectSystem::IrisWipe(bool bOpen, float Duration) { bIrisOpening=bOpen; ApplyPostEffect(EPostEffectType::IrisWipe, bOpen?1.f:0.f, Duration); }
void UEffectSystem::ExecuteShotEffects(const FShotContract& Shot)
{
    if (Shot.ColorGrade!=EColorGrade::None) ApplyColorGrade(Shot.ColorGrade,0.5f);
    switch(Shot.LightQuality)
    {
        case ELightQuality::HardLight: SetContrast(0.3f); break;
        case ELightQuality::SoftLight: SetContrast(-0.1f); break;
        case ELightQuality::LowKey: SetVignette(0.6f); SetContrast(0.4f); break;
        case ELightQuality::HighKey: SetBloom(0.8f); SetContrast(-0.2f); break;
        case ELightQuality::CandleLight: SetColorTemperature(0.5f); SetFilmGrain(0.2f); break;
        case ELightQuality::Moonlight: SetColorTemperature(-0.4f); SetSaturation(-0.2f); break;
        default: break;
    }
    if(Shot.bCameraShake) ScreenShake(Shot.ShakeIntensity, Shot.Duration*0.3f);
    if(Shot.SlowMotionFactor<1.f) SetSlowMotion(Shot.SlowMotionFactor, Shot.Duration);
    switch(Shot.ShotSize)
    {
        case EShotSize::CloseUp: case EShotSize::ExtremeCloseUp: case EShotSize::Macro:
            SetDepthOfField(true, 500.f, Shot.Aperture); break;
        case EShotSize::Medium: case EShotSize::MediumCloseUp:
            SetDepthOfField(true, 1000.f, Shot.Aperture); break;
        default: SetDepthOfField(false); break;
    }
}
void UEffectSystem::Tick(float DeltaTime) { UpdatePostEffects(DeltaTime); UpdateColorGrade(DeltaTime); UpdateFlash(DeltaTime); UpdateSlowMotion(DeltaTime); }
void UEffectSystem::UpdatePostEffects(float DeltaTime)
{
    for(int32 i=ActivePostEffects.Num()-1; i>=0; --i)
    {
        ActivePostEffects[i].Duration-=DeltaTime;
        if(ActivePostEffects[i].Duration<=0) ActivePostEffects.RemoveAt(i);
    }
}
void UEffectSystem::UpdateColorGrade(float DeltaTime)
{
    if(TargetColorGrade==CurrentColorGrade) return;
    ColorGradeTransition+=DeltaTime;
    float Alpha=FMath::Clamp(ColorGradeTransition/ColorGradeTransitionDuration,0.f,1.f);
    if(Alpha>=1.f)
    {
        CurrentColorGrade=TargetColorGrade;
        switch(CurrentColorGrade)
        {
            case EColorGrade::WarmGolden: CurrentColorTemp=0.4f; CurrentSaturation=0.1f; CurrentContrast=0.1f; break;
            case EColorGrade::Desaturated: CurrentSaturation=-0.5f; CurrentContrast=0.2f; break;
            case EColorGrade::ColdBlue: CurrentColorTemp=-0.5f; CurrentSaturation=-0.1f; CurrentContrast=0.2f; break;
            case EColorGrade::Sepia: CurrentColorTemp=0.3f; CurrentSaturation=-0.6f; CurrentContrast=0.1f; FilmGrainIntensity=0.3f; break;
            case EColorGrade::HighContrast: CurrentContrast=0.5f; CurrentSaturation=0.2f; break;
            case EColorGrade::TealAndOrange: CurrentColorTemp=0.1f; CurrentContrast=0.3f; break;
            case EColorGrade::InkWash: CurrentSaturation=-0.8f; CurrentContrast=0.4f; break;
            case EColorGrade::FestivalRed: CurrentColorTemp=0.3f; CurrentSaturation=0.3f; break;
            case EColorGrade::NightNeon: CurrentColorTemp=-0.3f; CurrentSaturation=0.4f; CurrentBloom=0.8f; break;
            default: CurrentColorTemp=0.f; CurrentSaturation=0.f; CurrentContrast=0.f; break;
        }
    }
}
void UEffectSystem::UpdateFlash(float DeltaTime)
{
    if(FlashIntensity>0) { FlashIntensity=FMath::FInterpTo(FlashIntensity,0.f,DeltaTime,8.f); if(FlashIntensity<0.01f) FlashIntensity=0.f; }
}
void UEffectSystem::UpdateSlowMotion(float DeltaTime)
{
    if(SlowMotionTime>0) { SlowMotionTime-=DeltaTime; if(SlowMotionTime<=0) { SlowMotionFactor=1.f; SetMotionBlur(0.5f); SetChromaticAberration(0.f); } }
}
