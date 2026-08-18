#pragma once
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SettingsSystem.generated.h"

UENUM(BlueprintType)
enum class EGraphicsQuality : uint8
{
    Low     UMETA(DisplayName="低"),
    Medium  UMETA(DisplayName="中"),
    High    UMETA(DisplayName="高"),
    Ultra   UMETA(DisplayName="极高")
};

UENUM(BlueprintType)
enum class EGameDifficulty : uint8
{
    Easy    UMETA(DisplayName="简单"),
    Normal  UMETA(DisplayName="普通"),
    Hard    UMETA(DisplayName="困难"),
    Ironman UMETA(DisplayName="铁人") // 不能手动存档
};

UENUM(BlueprintType)
enum class EGameLanguage : uint8
{
    ChineseSimplified  UMETA(DisplayName="简体中文"),
    ChineseTraditional UMETA(DisplayName="繁體中文"),
    ClassicalChinese   UMETA(DisplayName="文言文"),
    English            UMETA(DisplayName="English"),
    Japanese           UMETA(DisplayName="日本語"),
    Korean             UMETA(DisplayName="한국어"),
    Russian            UMETA(DisplayName="Русский"),
    French             UMETA(DisplayName="Français"),
    German             UMETA(DisplayName="Deutsch"),
    Spanish            UMETA(DisplayName="Español")
};

USTRUCT(BlueprintType)
struct FGameSettings
{
    GENERATED_BODY()
    // 画面
    UPROPERTY(EditAnywhere, BlueprintReadWrite) EGraphicsQuality GraphicsQuality = EGraphicsQuality::High;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 ResolutionX = 1920;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 ResolutionY = 1080;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) bool bFullscreen = true;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) bool bVSync = true;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 FrameRateLimit = 60;
    // 音量
    UPROPERTY(EditAnywhere, BlueprintReadWrite) float MasterVolume = 0.8f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) float MusicVolume = 0.6f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) float SFXVolume = 0.7f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) float VoiceVolume = 1.0f;
    // 游戏
    UPROPERTY(EditAnywhere, BlueprintReadWrite) EGameDifficulty Difficulty = EGameDifficulty::Normal;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) EGameLanguage Language = EGameLanguage::ChineseSimplified;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) float TimeScale = 1.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) bool bAutoSave = true;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 AutoSaveInterval = 5; // 分钟
    UPROPERTY(EditAnywhere, BlueprintReadWrite) bool bShowTutorial = true;
    // 显示
    UPROPERTY(EditAnywhere, BlueprintReadWrite) bool bShowFPS = false;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) bool bSubtitles = true;
};

/**
 * 设置系统 - 画质/音量/语言/难度/键位/存档管理
 */
UCLASS()
class SHANHEERA_API USettingsSystem : public UObject
{
    GENERATED_BODY()
public:
    UFUNCTION(BlueprintCallable, Category="山河纪元|设置")
    void LoadSettings();

    UFUNCTION(BlueprintCallable, Category="山河纪元|设置")
    void SaveSettings();

    UFUNCTION(BlueprintCallable, Category="山河纪元|设置")
    void ApplyGraphicsSettings();

    UFUNCTION(BlueprintCallable, Category="山河纪元|设置")
    void ApplyAudioSettings();

    UFUNCTION(BlueprintCallable, Category="山河纪元|设置")
    void SetLanguage(EGameLanguage NewLanguage);

    UFUNCTION(BlueprintCallable, Category="山河纪元|设置")
    void SetDifficulty(EGameDifficulty NewDifficulty);

    UFUNCTION(BlueprintCallable, Category="山河纪元|设置")
    void ResetToDefaults();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="山河纪元|设置")
    FGameSettings CurrentSettings;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="山河纪元|设置")
    TMap<FName, FKey> KeyBindings;
};
