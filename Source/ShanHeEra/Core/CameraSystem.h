#pragma once
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Core/CinematicDirector.h"
#include "ShanHeCameraSystem.generated.h"
class UCameraComponent;
class USpringArmComponent;
class AActor;
UCLASS()
class SHANHEERA_API UCameraSystem : public UObject
{
    GENERATED_BODY()
public:
    UCameraSystem();
    UFUNCTION(BlueprintCallable, Category="山河纪元|相机")
    void SwitchCameraMode(ECameraMode NewMode);
    UFUNCTION(BlueprintCallable, Category="山河纪元|相机")
    ECameraMode GetCurrentMode() const { return CurrentMode; }
    UFUNCTION(BlueprintCallable, Category="山河纪元|相机")
    void SetCameraTarget(AActor* Target);
    UFUNCTION(BlueprintCallable, Category="山河纪元|相机")
    void Zoom(float Amount);
    UFUNCTION(BlueprintCallable, Category="山河纪元|相机")
    void Rotate(float YawInput, float PitchInput);
    UFUNCTION(BlueprintCallable, Category="山河纪元|相机|电影运镜")
    void DollyPushIn(float Distance, float Duration, EEndingProfile Ending = EEndingProfile::Resolve);
    UFUNCTION(BlueprintCallable, Category="山河纪元|相机|电影运镜")
    void DollyPullBack(float Distance, float Duration, EEndingProfile Ending = EEndingProfile::Resolve);
    UFUNCTION(BlueprintCallable, Category="山河纪元|相机|电影运镜")
    void LateralTrack(float Distance, float Duration, bool bRight = true);
    UFUNCTION(BlueprintCallable, Category="山河纪元|相机|电影运镜")
    void Orbit(float Radius, float Degrees, float Duration, bool bClockwise = true);
    UFUNCTION(BlueprintCallable, Category="山河纪元|相机|电影运镜")
    void Pan(float Degrees, float Duration);
    UFUNCTION(BlueprintCallable, Category="山河纪元|相机|电影运镜")
    void Tilt(float Degrees, float Duration);
    UFUNCTION(BlueprintCallable, Category="山河纪元|相机|电影运镜")
    void Pedestal(float Height, float Duration);
    UFUNCTION(BlueprintCallable, Category="山河纪元|相机|电影运镜")
    void CraneReveal(float Height, float Distance, float Duration);
    UFUNCTION(BlueprintCallable, Category="山河纪元|相机|电影运镜")
    void RackFocus(float FromFocalDistance, float ToFocalDistance, float Duration);
    UFUNCTION(BlueprintCallable, Category="山河纪元|相机|电影运镜")
    void DollyZoom(float Distance, float Duration);
    UFUNCTION(BlueprintCallable, Category="山河纪元|相机|电影运镜")
    void SetHandheld(bool bEnabled, float Intensity = 0.5f);
    UFUNCTION(BlueprintCallable, Category="山河纪元|相机|电影运镜")
    void WhipPan(float Degrees, float Duration = 0.2f);
    UFUNCTION(BlueprintCallable, Category="山河纪元|相机|电影运镜")
    void SetBreathing(bool bEnabled, float Intensity = 0.3f);
    UFUNCTION(BlueprintCallable, Category="山河纪元|相机|镜头参数")
    void SetFocalLength(float NewFocalLength);
    UFUNCTION(BlueprintCallable, Category="山河纪元|相机|镜头参数")
    void SetAperture(float NewAperture);
    UFUNCTION(BlueprintCallable, Category="山河纪元|相机|镜头参数")
    void SetDepthOfField(bool bEnabled, float FocalDistance = 1000.0f, float Aperture = 2.8f);
    UFUNCTION(BlueprintCallable, Category="山河纪元|相机|镜头参数")
    void SetDutchAngle(float Degrees);
    UFUNCTION(BlueprintCallable, Category="山河纪元|相机|震动")
    void Shake(float Intensity, float Duration);
    UFUNCTION(BlueprintCallable, Category="山河纪元|相机|震动")
    void ExplosionShake(FVector Origin, float Radius = 1000.0f);
    UFUNCTION(BlueprintCallable, Category="山河纪元|相机|时间")
    void SetSlowMotion(float Factor, float Duration);
    UFUNCTION(BlueprintCallable, Category="山河纪元|相机|镜头契约")
    void ExecuteShotContract(const FShotContract& Shot);
    void Tick(float DeltaTime);
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="山河纪元|相机")
    float ThirdPersonDistance = 300.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="山河纪元|相机")
    float StrategyHeight = 2000.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="山河纪元|相机")
    float BattleDistance = 200.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="山河纪元|相机")
    float TransitionSpeed = 2.0f;
    UPROPERTY(BlueprintReadOnly, Category="山河纪元|相机")
    ECameraMode CurrentMode = ECameraMode::ThirdPerson;
    UPROPERTY(BlueprintReadOnly, Category="山河纪元|相机")
    bool bIsTransitioning = false;
    UPROPERTY(BlueprintReadOnly, Category="山河纪元|相机|电影")
    float CurrentFocalLength = 35.0f;
    UPROPERTY(BlueprintReadOnly, Category="山河纪元|相机|电影")
    float CurrentAperture = 2.8f;
    UPROPERTY(BlueprintReadOnly, Category="山河纪元|相机|电影")
    bool bDepthOfFieldEnabled = false;
    UPROPERTY(BlueprintReadOnly, Category="山河纪元|相机|电影")
    float CurrentFocalDistance = 1000.0f;
    UPROPERTY(BlueprintReadOnly, Category="山河纪元|相机|电影")
    float CurrentDutchAngle = 0.0f;
    UPROPERTY(BlueprintReadOnly, Category="山河纪元|相机|电影")
    bool bHandheldEnabled = false;
    UPROPERTY(BlueprintReadOnly, Category="山河纪元|相机|电影")
    float HandheldIntensity = 0.5f;
    UPROPERTY(BlueprintReadOnly, Category="山河纪元|相机|电影")
    bool bBreathingEnabled = false;
private:
    bool bIsAnimating = false;
    FVector AnimateStartLocation;
    FVector AnimateEndLocation;
    FRotator AnimateStartRotation;
    FRotator AnimateEndRotation;
    float AnimateTime = 0.0f;
    float AnimateDuration = 1.0f;
    EEndingProfile CurrentEndingProfile = EEndingProfile::Resolve;
    bool bShaking = false;
    float ShakeIntensity = 0.0f;
    float ShakeTime = 0.0f;
    float ShakeDuration = 0.0f;
    float SlowMotionFactor = 1.0f;
    float SlowMotionTime = 0.0f;
    bool bOrbiting = false;
    FVector OrbitCenter;
    float OrbitRadius = 0.0f;
    float OrbitStartAngle = 0.0f;
    float OrbitEndAngle = 0.0f;
    float OrbitHeight = 0.0f;
    bool bDollyZooming = false;
    float DollyZoomStartFocal = 35.0f;
    float DollyZoomEndFocal = 35.0f;
    float BreathingPhase = 0.0f;
    AActor* CameraTarget = nullptr;
    void UpdateAnimation(float DeltaTime);
    void UpdateShake(float DeltaTime);
    void UpdateHandheld(float DeltaTime);
    void UpdateBreathing(float DeltaTime);
    FVector GetCurrentCameraLocation() const;
    FRotator GetCurrentCameraRotation() const;
};
