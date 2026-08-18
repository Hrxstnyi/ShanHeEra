#include "Core/SettingsSystem.h"
#include "Core/ShanHeLog.h"

void USettingsSystem::LoadSettings()
{
    // 从配置文件加载，这里用默认值
    UE_LOG(LogShanHe, Log, TEXT("设置系统 - 加载配置"));
}

void USettingsSystem::SaveSettings()
{
    UE_LOG(LogShanHe, Log, TEXT("设置系统 - 保存配置"));
}

void USettingsSystem::ApplyGraphicsSettings()
{
    UE_LOG(LogShanHe, Log, TEXT("应用画面设置 - 质量:%d, 分辨率:%dx%d, 全屏:%d"),
        (int32)CurrentSettings.GraphicsQuality,
        CurrentSettings.ResolutionX, CurrentSettings.ResolutionY,
        CurrentSettings.bFullscreen);
}

void USettingsSystem::ApplyAudioSettings()
{
    UE_LOG(LogShanHe, Log, TEXT("应用音频设置 - 主音量:%.0f%%, 音乐:%.0f%%, 音效:%.0f%%"),
        CurrentSettings.MasterVolume * 100,
        CurrentSettings.MusicVolume * 100,
        CurrentSettings.SFXVolume * 100);
}

void USettingsSystem::SetLanguage(EGameLanguage NewLanguage)
{
    CurrentSettings.Language = NewLanguage;
    UE_LOG(LogShanHe, Log, TEXT("切换语言：%d"), (int32)NewLanguage);
}

void USettingsSystem::SetDifficulty(EGameDifficulty NewDifficulty)
{
    CurrentSettings.Difficulty = NewDifficulty;
    UE_LOG(LogShanHe, Log, TEXT("切换难度：%d"), (int32)NewDifficulty);
}

void USettingsSystem::ResetToDefaults()
{
    CurrentSettings = FGameSettings();
    UE_LOG(LogShanHe, Log, TEXT("恢复默认设置"));
}
