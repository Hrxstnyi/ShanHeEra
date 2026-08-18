#pragma once
#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Core/CinematicDirector.h"
#include "AudioManager.generated.h"

class USoundBase;
class UAudioComponent;
UENUM(BlueprintType)
enum class EMusicLayer : uint8
{
    Base        UMETA(DisplayName="基础层"),
    Tension     UMETA(DisplayName="紧张层"),
    Epic        UMETA(DisplayName="史诗层"),
    Sorrow      UMETA(DisplayName="悲壮层"),
    Percussion  UMETA(DisplayName="打击层")
};
UENUM(BlueprintType)
enum class EMusicMood : uint8
{
    Peaceful    UMETA(DisplayName="宁静"),
    Exploration UMETA(DisplayName="探索"),
    Tension     UMETA(DisplayName="紧张"),
    Battle      UMETA(DisplayName="战斗"),
    Epic        UMETA(DisplayName="史诗"),
    Sorrow      UMETA(DisplayName="悲壮"),
    Ceremony    UMETA(DisplayName="典礼"),
    Victory     UMETA(DisplayName="胜利"),
    Joyful      UMETA(DisplayName="欢快"),
    Mysterious  UMETA(DisplayName="神秘"),
    Festival    UMETA(DisplayName="节庆")
};
UENUM(BlueprintType)
enum class EStingerType : uint8
{
    Impact      UMETA(DisplayName="冲击"),
    Reveal      UMETA(DisplayName="揭示"),
    Victory     UMETA(DisplayName="胜利"),
    Sorrow      UMETA(DisplayName="悲伤"),
    Suspense    UMETA(DisplayName="悬念"),
    Transition  UMETA(DisplayName="转场"),
    Horn        UMETA(DisplayName="号角")
};
USTRUCT(BlueprintType)
struct FMusicTrack
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FName TrackID;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FString SoundPath;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) EMusicLayer Layer = EMusicLayer::Base;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) float BPM = 120.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) float Volume = 0.8f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) bool bIsLooping = true;
};
USTRUCT(BlueprintType)
struct FStinger
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite) EStingerType Type;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FString SoundPath;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) float Volume = 1.0f;
};
UCLASS()
class SHANHEERA_API UAudioManager : public UGameInstanceSubsystem
{
    GENERATED_BODY()
public:
    UAudioManager();
    UFUNCTION(BlueprintCallable, Category="山河纪元|音频")
    void InitializeAudio();
    UFUNCTION(BlueprintCallable, Category="山河纪元|音频|音乐")
    void PlayMusic(FName TrackID, float FadeInTime = 1.0f);
    UFUNCTION(BlueprintCallable, Category="山河纪元|音频|音乐")
    void StopMusic(float FadeOutTime = 1.0f);
    UFUNCTION(BlueprintCallable, Category="山河纪元|音频|音乐")
    void SetMusicVolume(float Volume);
    UFUNCTION(BlueprintCallable, Category="山河纪元|音频|音乐")
    void SetMusicMood(EMusicMood NewMood, float TransitionTime = 2.0f);
    UFUNCTION(BlueprintCallable, Category="山河纪元|音频|音乐")
    EMusicMood GetCurrentMood() const { return CurrentMood; }
    UFUNCTION(BlueprintCallable, Category="山河纪元|音频|音乐|分层")
    void SetLayerVolume(EMusicLayer Layer, float Volume, float FadeTime = 1.0f);
    UFUNCTION(BlueprintCallable, Category="山河纪元|音频|音乐|分层")
    void FadeInLayer(EMusicLayer Layer, float FadeTime = 1.0f);
    UFUNCTION(BlueprintCallable, Category="山河纪元|音频|音乐|分层")
    void FadeOutLayer(EMusicLayer Layer, float FadeTime = 1.0f);
    UFUNCTION(BlueprintCallable, Category="山河纪元|音频|音乐|分层")
    void TriggerTension(float Intensity = 1.0f);
    UFUNCTION(BlueprintCallable, Category="山河纪元|音频|音乐|分层")
    void ReleaseTension(float FadeTime = 2.0f);
    UFUNCTION(BlueprintCallable, Category="山河纪元|音频|音乐|节拍")
    float GetBeatProgress() const;
    UFUNCTION(BlueprintCallable, Category="山河纪元|音频|音乐|节拍")
    bool IsOnBeat(float Tolerance = 0.1f) const;
    UFUNCTION(BlueprintCallable, Category="山河纪元|音频|音乐|Stinger")
    void PlayStinger(EStingerType Type);
    UFUNCTION(BlueprintCallable, Category="山河纪元|音频|音效")
    void PlaySFX(FName SFXID, FVector Location = FVector::ZeroVector);
    UFUNCTION(BlueprintCallable, Category="山河纪元|音频|音效")
    void PlayUISound(FName SoundID);
    UFUNCTION(BlueprintCallable, Category="山河纪元|音频|对话")
    void StartDialogueDuck(float DuckAmount = 0.4f, float FadeTime = 0.3f);
    UFUNCTION(BlueprintCallable, Category="山河纪元|音频|对话")
    void EndDialogueDuck(float FadeTime = 0.5f);
    UFUNCTION(BlueprintCallable, Category="山河纪元|音频|镜头契约")
    void ExecuteShotAudio(const FShotContract& Shot);
    void Tick(float DeltaTime);
    UPROPERTY(BlueprintReadOnly, Category="山河纪元|音频")
    TMap<FName, FMusicTrack> MusicLibrary;
    UPROPERTY(BlueprintReadOnly, Category="山河纪元|音频")
    TMap<EStingerType, FStinger> StingerLibrary;
    UPROPERTY(BlueprintReadOnly, Category="山河纪元|音频")
    EMusicMood CurrentMood = EMusicMood::Peaceful;
    UPROPERTY(BlueprintReadOnly, Category="山河纪元|音频")
    float MasterVolume = 1.0f;
    UPROPERTY(BlueprintReadOnly, Category="山河纪元|音频")
    float MusicVolume = 0.8f;
    UPROPERTY(BlueprintReadOnly, Category="山河纪元|音频")
    float SFXVolume = 1.0f;
    UPROPERTY(BlueprintReadOnly, Category="山河纪元|音频")
    float CurrentBPM = 120.0f;
private:
    EMusicMood TargetMood = EMusicMood::Peaceful;
    float MoodTransition = 0.0f;
    float MoodTransitionDuration = 2.0f;
    TMap<EMusicLayer, float> LayerVolumes;
    TMap<EMusicLayer, float> TargetLayerVolumes;
    float TensionIntensity = 0.0f;
    float BeatPhase = 0.0f;
    bool bDialogueDucking = false;
    float DuckAmount = 0.0f;
    float CurrentDuck = 0.0f;
    void UpdateMoodTransition(float DeltaTime);
    void UpdateLayerVolumes(float DeltaTime);
    void UpdateBeat(float DeltaTime);
    void UpdateDialogueDuck(float DeltaTime);
};
