#pragma once
#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "ShanHeAudioManager.generated.h"

class UAudioComponent;
class USoundBase;

UENUM(BlueprintType)
enum class EMusicMood : uint8
{
    Peaceful    UMETA(DisplayName="太平"),
    Tense       UMETA(DisplayName="紧张"),
    War         UMETA(DisplayName="战争"),
    Epic        UMETA(DisplayName="史诗"),
    Sorrowful   UMETA(DisplayName="悲壮"),
    Triumphant  UMETA(DisplayName="凯旋"),
    Festival    UMETA(DisplayName="节庆"),
    Nostalgic   UMETA(DisplayName="怀古")
};

UCLASS()
class SHANHEERA_API UAudioManager : public UGameInstanceSubsystem
{
    GENERATED_BODY()
public:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;

    UFUNCTION(BlueprintCallable, Category="山河纪元|音频")
    void PlayBackgroundMusic(FName TrackName);

    UFUNCTION(BlueprintCallable, Category="山河纪元|音频")
    void SetMusicMood(EMusicMood NewMood);

    UFUNCTION(BlueprintCallable, Category="山河纪元|音频")
    void PlayCGMusic(FName CGName);

    UFUNCTION(BlueprintCallable, Category="山河纪元|音频")
    void StopMusic(float FadeTime = 2.0f);

    UFUNCTION(BlueprintCallable, Category="山河纪元|音频")
    void SetMasterVolume(float Volume);
    UFUNCTION(BlueprintCallable, Category="山河纪元|音频")
    void SetMusicVolume(float Volume);
    UFUNCTION(BlueprintCallable, Category="山河纪元|音频")
    void SetSFXVolume(float Volume);

    UPROPERTY(BlueprintReadOnly, Category="山河纪元|音频")
    EMusicMood CurrentMood = EMusicMood::Peaceful;

    UPROPERTY(BlueprintReadOnly, Category="山河纪元|音频")
    float MasterVolume = 1.0f;
    UPROPERTY(BlueprintReadOnly, Category="山河纪元|音频")
    float MusicVolume = 0.8f;
    UPROPERTY(BlueprintReadOnly, Category="山河纪元|音频")
    float SFXVolume = 1.0f;

private:
    UAudioComponent* MusicComponent = nullptr;
    TMap<FName, USoundBase*> MusicLibrary;
    TMap<FName, USoundBase*> CGMusicLibrary;
};
