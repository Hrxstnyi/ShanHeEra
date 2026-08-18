#include "Core/CameraSystem.h"
#include "Core/ShanHeLog.h"
#include "GameFramework/Actor.h"
#include "Camera/CameraComponent.h"
UCameraSystem::UCameraSystem() {}
void UCameraSystem::SwitchCameraMode(ECameraMode NewMode)
{
    if (NewMode == CurrentMode) return;
    ECameraMode OldMode = CurrentMode;
    CurrentMode = NewMode;
    bIsTransitioning = true;
    switch (NewMode)
    {
        case ECameraMode::ThirdPerson: CurrentFocalLength=35.f; CurrentAperture=4.f; bDepthOfFieldEnabled=false; CurrentDutchAngle=0.f; break;
        case ECameraMode::Strategy: CurrentFocalLength=24.f; CurrentAperture=8.f; bDepthOfFieldEnabled=false; CurrentDutchAngle=0.f; break;
        case ECameraMode::Battle: CurrentFocalLength=50.f; CurrentAperture=2.8f; bDepthOfFieldEnabled=true; CurrentFocalDistance=500.f; CurrentDutchAngle=0.f; break;
        case ECameraMode::Cinematic: CurrentFocalLength=35.f; CurrentAperture=2.8f; bDepthOfFieldEnabled=true; CurrentFocalDistance=1000.f; break;
        default: break;
    }
}
void UCameraSystem::SetCameraTarget(AActor* Target) { CameraTarget = Target; }
void UCameraSystem::Zoom(float Amount)
{
    switch (CurrentMode)
    {
        case ECameraMode::ThirdPerson: ThirdPersonDistance=FMath::Clamp(ThirdPersonDistance+Amount*10,150.f,600.f); break;
        case ECameraMode::Strategy: StrategyHeight=FMath::Clamp(StrategyHeight+Amount*50,500.f,5000.f); break;
        case ECameraMode::Battle: BattleDistance=FMath::Clamp(BattleDistance+Amount*5,100.f,400.f); break;
        case ECameraMode::Cinematic: CurrentFocalLength=FMath::Clamp(CurrentFocalLength+Amount*5,14.f,200.f); break;
        default: break;
    }
}
void UCameraSystem::Rotate(float YawInput, float PitchInput) {}
void UCameraSystem::DollyPushIn(float Distance, float Duration, EEndingProfile Ending)
{
    bIsAnimating=true; AnimateStartLocation=GetCurrentCameraLocation();
    FVector Forward=GetCurrentCameraRotation().Vector();
    AnimateEndLocation=AnimateStartLocation+Forward*Distance;
    AnimateStartRotation=GetCurrentCameraRotation(); AnimateEndRotation=AnimateStartRotation;
    AnimateTime=0.f; AnimateDuration=Duration; CurrentEndingProfile=Ending;
}
void UCameraSystem::DollyPullBack(float Distance, float Duration, EEndingProfile Ending) { DollyPushIn(-Distance,Duration,Ending); }
void UCameraSystem::LateralTrack(float Distance, float Duration, bool bRight)
{
    bIsAnimating=true; AnimateStartLocation=GetCurrentCameraLocation();
    FVector Right=GetCurrentCameraRotation().RotateVector(FVector::RightVector());
    AnimateEndLocation=AnimateStartLocation+Right*(bRight?Distance:-Distance);
    AnimateStartRotation=GetCurrentCameraRotation(); AnimateEndRotation=AnimateStartRotation;
    AnimateTime=0.f; AnimateDuration=Duration;
}
void UCameraSystem::Orbit(float Radius, float Degrees, float Duration, bool bClockwise)
{
    if(!CameraTarget) return;
    bOrbiting=true; OrbitCenter=CameraTarget->GetActorLocation(); OrbitRadius=Radius;
    OrbitStartAngle=GetCurrentCameraRotation().Yaw;
    OrbitEndAngle=OrbitStartAngle+(bClockwise?Degrees:-Degrees);
    OrbitHeight=GetCurrentCameraLocation().Z-OrbitCenter.Z;
    AnimateTime=0.f; AnimateDuration=Duration;
}
void UCameraSystem::Pan(float Degrees, float Duration)
{
    bIsAnimating=true; AnimateStartLocation=GetCurrentCameraLocation(); AnimateEndLocation=AnimateStartLocation;
    AnimateStartRotation=GetCurrentCameraRotation(); AnimateEndRotation=AnimateStartRotation; AnimateEndRotation.Yaw+=Degrees;
    AnimateTime=0.f; AnimateDuration=Duration;
}
void UCameraSystem::Tilt(float Degrees, float Duration)
{
    bIsAnimating=true; AnimateStartLocation=GetCurrentCameraLocation(); AnimateEndLocation=AnimateStartLocation;
    AnimateStartRotation=GetCurrentCameraRotation(); AnimateEndRotation=AnimateStartRotation; AnimateEndRotation.Pitch+=Degrees;
    AnimateTime=0.f; AnimateDuration=Duration;
}
void UCameraSystem::Pedestal(float Height, float Duration)
{
    bIsAnimating=true; AnimateStartLocation=GetCurrentCameraLocation();
    AnimateEndLocation=AnimateStartLocation+FVector(0,0,Height);
    AnimateStartRotation=GetCurrentCameraRotation(); AnimateEndRotation=AnimateStartRotation;
    AnimateTime=0.f; AnimateDuration=Duration;
}
void UCameraSystem::CraneReveal(float Height, float Distance, float Duration)
{
    bIsAnimating=true; AnimateStartLocation=GetCurrentCameraLocation();
    FVector Forward=GetCurrentCameraRotation().Vector();
    AnimateEndLocation=AnimateStartLocation+FVector(Forward.X*Distance,Forward.Y*Distance,Height);
    AnimateStartRotation=GetCurrentCameraRotation(); AnimateEndRotation=AnimateStartRotation; AnimateEndRotation.Pitch-=15.f;
    AnimateTime=0.f; AnimateDuration=Duration;
}
void UCameraSystem::RackFocus(float FromFocalDistance, float ToFocalDistance, float Duration)
{
    bDepthOfFieldEnabled=true; CurrentFocalDistance=FromFocalDistance;
    bIsAnimating=true; AnimateStartLocation=GetCurrentCameraLocation(); AnimateEndLocation=AnimateStartLocation;
    AnimateStartRotation=GetCurrentCameraRotation(); AnimateEndRotation=AnimateStartRotation;
    AnimateTime=0.f; AnimateDuration=Duration; CurrentFocalDistance=ToFocalDistance;
}
void UCameraSystem::DollyZoom(float Distance, float Duration)
{
    bDollyZooming=true; DollyZoomStartFocal=CurrentFocalLength;
    DollyZoomEndFocal=CurrentFocalLength+(Distance>0?-15.f:15.f);
    DollyZoomEndFocal=FMath::Clamp(DollyZoomEndFocal,14.f,200.f);
    DollyPushIn(Distance,Duration,EEndingProfile::Resolve);
}
void UCameraSystem::SetHandheld(bool bEnabled, float Intensity) { bHandheldEnabled=bEnabled; HandheldIntensity=Intensity; }
void UCameraSystem::WhipPan(float Degrees, float Duration) { Pan(Degrees,Duration); }
void UCameraSystem::SetBreathing(bool bEnabled, float Intensity) { bBreathingEnabled=bEnabled; BreathingPhase=0.f; }
void UCameraSystem::SetFocalLength(float NewFocalLength) { CurrentFocalLength=FMath::Clamp(NewFocalLength,14.f,200.f); }
void UCameraSystem::SetAperture(float NewAperture) { CurrentAperture=FMath::Clamp(NewAperture,1.f,16.f); }
void UCameraSystem::SetDepthOfField(bool bEnabled, float FocalDistance, float Aperture) { bDepthOfFieldEnabled=bEnabled; CurrentFocalDistance=FocalDistance; CurrentAperture=Aperture; }
void UCameraSystem::SetDutchAngle(float Degrees) { CurrentDutchAngle=FMath::Clamp(Degrees,-45.f,45.f); }
void UCameraSystem::Shake(float Intensity, float Duration) { bShaking=true; ShakeIntensity=Intensity; ShakeDuration=Duration; ShakeTime=0.f; }
void UCameraSystem::ExplosionShake(FVector Origin, float Radius)
{
    if(!CameraTarget) return;
    float Distance=FVector::Dist(CameraTarget->GetActorLocation(),Origin);
    if(Distance<Radius) { float Intensity=(1.f-Distance/Radius)*2.f; Shake(Intensity,0.5f); }
}
void UCameraSystem::SetSlowMotion(float Factor, float Duration) { SlowMotionFactor=FMath::Clamp(Factor,0.01f,1.f); SlowMotionTime=Duration; }
void UCameraSystem::ExecuteShotContract(const FShotContract& Shot)
{
    switch(Shot.ShotSize)
    {
        case EShotSize::ExtremeWide: CurrentFocalLength=14.f; break;
        case EShotSize::Wide: CurrentFocalLength=24.f; break;
        case EShotSize::MediumWide: CurrentFocalLength=28.f; break;
        case EShotSize::Medium: CurrentFocalLength=35.f; break;
        case EShotSize::MediumCloseUp: CurrentFocalLength=50.f; break;
        case EShotSize::CloseUp: CurrentFocalLength=85.f; break;
        case EShotSize::ExtremeCloseUp: CurrentFocalLength=135.f; break;
        case EShotSize::Macro: CurrentFocalLength=100.f; break;
        case EShotSize::OverShoulder: CurrentFocalLength=50.f; break;
    }
    CurrentAperture=Shot.Aperture; CurrentDutchAngle=Shot.DutchAngle;
    switch(Shot.Movement)
    {
        case ECameraMovement::PushIn: DollyPushIn(200.f,Shot.Duration*0.8f,Shot.Ending); break;
        case ECameraMovement::PullBack: DollyPullBack(300.f,Shot.Duration*0.8f,Shot.Ending); break;
        case ECameraMovement::LateralTrack: LateralTrack(200.f,Shot.Duration*0.8f,true); break;
        case ECameraMovement::Orbit: Orbit(300.f,90.f,Shot.Duration*0.8f,true); break;
        case ECameraMovement::Pan: Pan(30.f,Shot.Duration*0.8f); break;
        case ECameraMovement::Tilt: Tilt(15.f,Shot.Duration*0.8f); break;
        case ECameraMovement::Pedestal: Pedestal(200.f,Shot.Duration*0.8f); break;
        case ECameraMovement::Crane: CraneReveal(500.f,300.f,Shot.Duration*0.8f); break;
        case ECameraMovement::Handheld: SetHandheld(true,0.5f); break;
        case ECameraMovement::RackFocus: RackFocus(500.f,2000.f,Shot.Duration*0.6f); break;
        case ECameraMovement::WhipPan: WhipPan(60.f,0.2f); break;
        case ECameraMovement::DollyZoom: DollyZoom(150.f,Shot.Duration*0.8f); break;
        case ECameraMovement::Breathing: SetBreathing(true,0.3f); break;
        case ECameraMovement::LockedOff: default: break;
    }
    if(Shot.bCameraShake) Shake(Shot.ShakeIntensity,Shot.Duration*0.3f);
    if(Shot.SlowMotionFactor<1.f) SetSlowMotion(Shot.SlowMotionFactor,Shot.Duration);
}
void UCameraSystem::Tick(float DeltaTime)
{
    UpdateAnimation(DeltaTime); UpdateShake(DeltaTime); UpdateHandheld(DeltaTime); UpdateBreathing(DeltaTime);
    if(SlowMotionTime>0) { SlowMotionTime-=DeltaTime; if(SlowMotionTime<=0) SlowMotionFactor=1.f; }
}
void UCameraSystem::UpdateAnimation(float DeltaTime)
{
    if(!bIsAnimating) return;
    AnimateTime+=DeltaTime;
    float Alpha=FMath::Clamp(AnimateTime/AnimateDuration,0.f,1.f);
    Alpha=FMath::InterpEaseInOut(0.f,1.f,Alpha,2.f);
    if(bOrbiting&&CameraTarget)
    {
        float CurrentAngle=FMath::Lerp(OrbitStartAngle,OrbitEndAngle,Alpha);
        FVector Offset(FMath::Cos(FMath::DegreesToRadians(CurrentAngle))*OrbitRadius,FMath::Sin(FMath::DegreesToRadians(CurrentAngle))*OrbitRadius,OrbitHeight);
    }
    if(bDollyZooming) CurrentFocalLength=FMath::Lerp(DollyZoomStartFocal,DollyZoomEndFocal,Alpha);
    if(Alpha>=1.f) { bIsAnimating=false; bOrbiting=false; bDollyZooming=false; bIsTransitioning=false; }
}
void UCameraSystem::UpdateShake(float DeltaTime)
{
    if(!bShaking) return;
    ShakeTime+=DeltaTime;
    if(ShakeTime>=ShakeDuration) { bShaking=false; return; }
}
void UCameraSystem::UpdateHandheld(float DeltaTime)
{
    if(!bHandheldEnabled) return;
    float Time=GetWorld()?GetWorld()->GetTimeSeconds():0.f;
    float OffsetX=FMath::Sin(Time*3.7f)*HandheldIntensity*2.f;
    float OffsetY=FMath::Cos(Time*4.3f)*HandheldIntensity*1.5f;
}
void UCameraSystem::UpdateBreathing(float DeltaTime)
{
    if(!bBreathingEnabled) return;
    BreathingPhase+=DeltaTime*0.5f;
    float BreathOffset=FMath::Sin(BreathingPhase)*0.3f;
    CurrentFocalLength+=BreathOffset*0.1f;
}
FVector UCameraSystem::GetCurrentCameraLocation() const { return FVector::ZeroVector; }
FRotator UCameraSystem::GetCurrentCameraRotation() const { return FRotator::ZeroRotator; }
