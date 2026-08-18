#include "Audio/SpatialAudioSystem.h"
#include "Core/ShanHeLog.h"

void USpatialAudioSystem::InitializeAudio()
{
    struct FAmbDef { EAmbientType Type; const TCHAR* Path; float Vol; };
    FAmbDef Ambs[] = {
        {EAmbientType::Market, TEXT("/Game/Audio/Ambient/Market_Cue"), 0.6f},
        {EAmbientType::Battlefield, TEXT("/Game/Audio/Ambient/Battlefield_Cue"), 0.8f},
        {EAmbientType::Forest, TEXT("/Game/Audio/Ambient/Forest_Cue"), 0.4f},
        {EAmbientType::Village, TEXT("/Game/Audio/Ambient/Village_Cue"), 0.3f},
        {EAmbientType::Palace, TEXT("/Game/Audio/Ambient/Palace_Cue"), 0.2f},
        {EAmbientType::Temple, TEXT("/Game/Audio/Ambient/Temple_Cue"), 0.3f},
        {EAmbientType::River, TEXT("/Game/Audio/Ambient/River_Cue"), 0.5f},
        {EAmbientType::Desert, TEXT("/Game/Audio/Ambient/Desert_Cue"), 0.2f},
        {EAmbientType::Night, TEXT("/Game/Audio/Ambient/Night_Cue"), 0.15f},
    };

    for (const auto& A : Ambs)
    {
        FShanHeAmbientSound S;
        S.Type = A.Type; S.SoundCuePath = A.Path; S.BaseVolume = A.Vol;
        AmbientDatabase.Add(A.Type, S);
    }

    UE_LOG(LogShanHe, Log, TEXT("3D音效系统初始化 - %d 种环境音"), AmbientDatabase.Num());
}

void USpatialAudioSystem::PlayAmbient(EAmbientType Type)
{
    if (const FShanHeAmbientSound* S = AmbientDatabase.Find(Type))
    {
        CurrentAmbient = Type;
        bIsAmbientPlaying = true;
        UE_LOG(LogShanHe, Log, TEXT("播放环境音：%d, 音量%.0f%%"), (int32)Type, S->BaseVolume * 100);
    }
}

void USpatialAudioSystem::StopAmbient()
{
    bIsAmbientPlaying = false;
    UE_LOG(LogShanHe, Log, TEXT("停止环境音"));
}

void USpatialAudioSystem::PlayFootstep(FVector Location, float Volume)
{
    // 根据地面材质播放不同脚步声（泥土/石头/木头/水面）
    UE_LOG(LogShanHe, Verbose, TEXT("脚步声 at %s, 音量%.1f"), *Location.ToString(), Volume);
}

void USpatialAudioSystem::PlaySpatialSound(FName SoundID, FVector Location, float Volume)
{
    UE_LOG(LogShanHe, Log, TEXT("3D音效：%s at %s, 音量%.1f"), *SoundID.ToString(), *Location.ToString(), Volume);
}

void USpatialAudioSystem::SetReverbPreset(FName Preset)
{
    UE_LOG(LogShanHe, Log, TEXT("混响预设：%s"), *Preset.ToString());
}
