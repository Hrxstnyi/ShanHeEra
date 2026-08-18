#include "Audio/AudioManager.h"
#include "Core/ShanHeLog.h"

void UAudioManager::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
    UE_LOG(LogShanHe, Log, TEXT("音频管理器初始化 - 背景音乐与历史CG音乐系统就绪"));
}

void UAudioManager::PlayBackgroundMusic(FName TrackName)
{
    UE_LOG(LogShanHe, Log, TEXT("播放背景音乐：%s"), *TrackName.ToString());
    // 实际项目中从MusicLibrary加载并播放
}

void UAudioManager::SetMusicMood(EMusicMood NewMood)
{
    CurrentMood = NewMood;
    // 根据情绪切换背景音乐
    switch (NewMood)
    {
        case EMusicMood::War: PlayBackgroundMusic(TEXT("WarTheme")); break;
        case EMusicMood::Epic: PlayBackgroundMusic(TEXT("EpicTheme")); break;
        case EMusicMood::Triumphant: PlayBackgroundMusic(TEXT("VictoryTheme")); break;
        case EMusicMood::Sorrowful: PlayBackgroundMusic(TEXT("SorrowTheme")); break;
        default: PlayBackgroundMusic(TEXT("PeacefulTheme")); break;
    }
}

void UAudioManager::PlayCGMusic(FName CGName)
{
    UE_LOG(LogShanHe, Log, TEXT("播放CG动画音乐：%s - 调动情绪的历史时刻"), *CGName.ToString());
}

void UAudioManager::StopMusic(float FadeTime) {}
void UAudioManager::SetMasterVolume(float Volume) { MasterVolume = Volume; }
void UAudioManager::SetMusicVolume(float Volume) { MusicVolume = Volume; }
void UAudioManager::SetSFXVolume(float Volume) { SFXVolume = Volume; }
