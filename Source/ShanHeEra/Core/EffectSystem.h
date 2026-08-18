#pragma once
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Core/CinematicDirector.h"
#include "ShanHeEffectSystem.generated.h"
class UNiagaraSystem;
class UNiagaraComponent;
class UMaterialInstanceDynamic;
class UPostProcessComponent;
UENUM(BlueprintType)
enum class EEffectType : uint8
{
    Skill       UMETA(DisplayName="技能特效"),
    Weather     UMETA(DisplayName="天气特效"),
    Fire        UMETA(DisplayName="火焰"),
    Water       UMETA(DisplayName="水流"),
    Smoke       UMETA(DisplayName="烟尘"),
    Explosion   UMETA(DisplayName="爆炸"),
    Blood       UMETA(DisplayName="血雾"),
    Magic       UMETA(DisplayName="法术"),
    UI          UMETA(DisplayName="UI特效")
};
UENUM(BlueprintType)
enum class EPostEffectType : uint8
{
    None            UMETA(DisplayName="无"),
    DepthOfField    UMETA(DisplayName="景深"),
    MotionBlur      UMETA(DisplayName="运动模糊"),
    Bloom           UMETA(DisplayName="辉光"),
    ChromaticAberration UMETA(DisplayName="色差"),
    Vignette        UMETA(DisplayName="暗角"),
    FilmGrain       UMETA(DisplayName="胶片颗粒"),
    ColorGradeLUT   UMETA(DisplayName="色彩分级LUT"),
    SlowMotion      UMETA(DisplayName="慢动作"),
    BulletTime      UMETA(DisplayName="子弹时间"),
    FlashWhite      UMETA(DisplayName="白闪"),
    FlashRed        UMETA(DisplayName="红闪"),
    ScreenShake     UMETA(DisplayName="屏幕震动"),
    IrisWipe        UMETA(DisplayName="圈入圈出")
};
USTRUCT(BlueprintType)
struct FEffectData
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FName EffectID;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) EEffectType Type;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FString NiagaraSystemPath;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) float Duration = 1.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) float Scale = 1.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) bool bIsLooping = false;
};
USTRUCT(BlueprintType)
struct FPostEffectData
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite) EPostEffectType Type;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) float Intensity = 1.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) float Duration = 1.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) float FadeInTime = 0.1f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) float FadeOutTime = 0.3f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) EColorGrade ColorGrade = EColorGrade::None;
};
UCLASS()
class SHANHEERA_API UEffectSystem : public UObject
{
    GENERATED_BODY()
public:
    UEffectSystem();
    UFUNCTION(BlueprintCallable, Category="山河纪元|特效")
    void InitializeEffects();
    UFUNCTION(BlueprintCallable, Category="山河纪元|特效")
    void SpawnEffect(FName EffectID, FVector Location, FRotator Rotation, float Scale = 1.0f);
    UFUNCTION(BlueprintCallable, Category="山河纪元|特效")
    void SpawnWeatherEffect(EEffectType WeatherType, FVector Location);
    UFUNCTION(BlueprintCallable, Category="山河纪元|特效")
    void StopAllEffects();
    UFUNCTION(BlueprintCallable, Category="山河纪元|特效|后处理")
    void ApplyPostEffect(EPostEffectType Type, float Intensity = 1.0f, float Duration = 1.0f);
    UFUNCTION(BlueprintCallable, Category="山河纪元|特效|后处理")
    void SetDepthOfField(bool bEnabled, float FocalDistance = 1000.0f, float Aperture = 2.8f);
    UFUNCTION(BlueprintCallable, Category="山河纪元|特效|后处理")
    void SetMotionBlur(float Amount);
    UFUNCTION(BlueprintCallable, Category="山河纪元|特效|后处理")
    void SetBloom(float Intensity);
    UFUNCTION(BlueprintCallable, Category="山河纪元|特效|后处理")
    void SetChromaticAberration(float Intensity);
    UFUNCTION(BlueprintCallable, Category="山河纪元|特效|后处理")
    void SetVignette(float Intensity);
    UFUNCTION(BlueprintCallable, Category="山河纪元|特效|后处理")
    void SetFilmGrain(float Intensity);
    UFUNCTION(BlueprintCallable, Category="山河纪元|特效|色彩分级")
    void ApplyColorGrade(EColorGrade Grade, float TransitionTime = 1.0f);
    UFUNCTION(BlueprintCallable, Category="山河纪元|特效|色彩分级")
    EColorGrade GetCurrentColorGrade() const { return CurrentColorGrade; }
    UFUNCTION(BlueprintCallable, Category="山河纪元|特效|色彩分级")
    void SetSaturation(float Saturation);
    UFUNCTION(BlueprintCallable, Category="山河纪元|特效|色彩分级")
    void SetColorTemperature(float Temp);
    UFUNCTION(BlueprintCallable, Category="山河纪元|特效|色彩分级")
    void SetContrast(float Contrast);
    UFUNCTION(BlueprintCallable, Category="山河纪元|特效|镜头")
    void ScreenShake(float Intensity, float Duration);
    UFUNCTION(BlueprintCallable, Category="山河纪元|特效|镜头")
    void FlashWhite(float Intensity = 1.0f, float Duration = 0.2f);
    UFUNCTION(BlueprintCallable, Category="山河纪元|特效|镜头")
    void FlashRed(float Intensity = 0.5f, float Duration = 0.3f);
    UFUNCTION(BlueprintCallable, Category="山河纪元|特效|镜头")
    void SetSlowMotion(float Factor, float Duration);
    UFUNCTION(BlueprintCallable, Category="山河纪元|特效|镜头")
    void BulletTime(float Duration = 3.0f);
    UFUNCTION(BlueprintCallable, Category="山河纪元|特效|镜头")
    void IrisWipe(bool bOpen, float Duration = 1.0f);
    UFUNCTION(BlueprintCallable, Category="山河纪元|特效|镜头契约")
    void ExecuteShotEffects(const FShotContract& Shot);
    void Tick(float DeltaTime);
    UPROPERTY(BlueprintReadOnly, Category="山河纪元|特效")
    TMap<FName, FEffectData> EffectDatabase;
    UPROPERTY(BlueprintReadOnly, Category="山河纪元|特效")
    int32 ActiveEffects = 0;
    UPROPERTY(BlueprintReadOnly, Category="山河纪元|特效|后处理")
    EColorGrade CurrentColorGrade = EColorGrade::None;
    UPROPERTY(BlueprintReadOnly, Category="山河纪元|特效|后处理")
    float CurrentSaturation = 0.0f;
    UPROPERTY(BlueprintReadOnly, Category="山河纪元|特效|后处理")
    float CurrentColorTemp = 0.0f;
    UPROPERTY(BlueprintReadOnly, Category="山河纪元|特效|后处理")
    float CurrentContrast = 0.0f;
    UPROPERTY(BlueprintReadOnly, Category="山河纪元|特效|后处理")
    float CurrentMotionBlur = 0.5f;
    UPROPERTY(BlueprintReadOnly, Category="山河纪元|特效|后处理")
    float CurrentBloom = 0.5f;
    UPROPERTY(BlueprintReadOnly, Category="山河纪元|特效|镜头")
    float SlowMotionFactor = 1.0f;
private:
    bool bDOFEnabled = false;
    float DOFFocalDistance = 1000.0f;
    float DOFAperture = 2.8f;
    float ChromaticAberration = 0.0f;
    float VignetteIntensity = 0.3f;
    float FilmGrainIntensity = 0.0f;
    TArray<FPostEffectData> ActivePostEffects;
    float FlashIntensity = 0.0f;
    FLinearColor FlashColor = FLinearColor::White;
    float IrisValue = 1.0f;
    bool bIrisOpening = false;
    EColorGrade TargetColorGrade = EColorGrade::None;
    float ColorGradeTransition = 0.0f;
    float ColorGradeTransitionDuration = 1.0f;
    float SlowMotionTime = 0.0f;
    void UpdatePostEffects(float DeltaTime);
    void UpdateColorGrade(float DeltaTime);
    void UpdateFlash(float DeltaTime);
    void UpdateSlowMotion(float DeltaTime);
};
