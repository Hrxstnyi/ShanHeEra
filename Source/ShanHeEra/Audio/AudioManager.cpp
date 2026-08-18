#include "Audio/AudioManager.h"
#include "Core/ShanHeLog.h"
UAudioManager::UAudioManager()
{
    for(uint8 i=0; i<(uint8)EMusicLayer::Percussion+1; i++) { LayerVolumes.Add((EMusicLayer)i, i==0?1.f:0.f); TargetLayerVolumes.Add((EMusicLayer)i, i==0?1.f:0.f); }
}
void UAudioManager::InitializeAudio()
{
    struct FTrackDef { FName ID; const TCHAR* Path; EMusicLayer Layer; float BPM; float Vol; };
    FTrackDef Tracks[] = {
        {TEXT("Peaceful_Base"), TEXT("/Game/Audio/Music/Peaceful_Base"), EMusicLayer::Base, 80.f, 0.7f},
        {TEXT("Peaceful_Ambient"), TEXT("/Game/Audio/Music/Peaceful_Ambient"), EMusicLayer::Base, 80.f, 0.5f},
        {TEXT("Exploration_Base"), TEXT("/Game/Audio/Music/Exploration_Base"), EMusicLayer::Base, 100.f, 0.7f},
        {TEXT("Exploration_Perc"), TEXT("/Game/Audio/Music/Exploration_Perc"), EMusicLayer::Percussion, 100.f, 0.6f},
        {TEXT("Battle_Base"), TEXT("/Game/Audio/Music/Battle_Base"), EMusicLayer::Base, 140.f, 0.8f},
        {TEXT("Battle_Tension"), TEXT("/Game/Audio/Music/Battle_Tension"), EMusicLayer::Tension, 140.f, 0.7f},
        {TEXT("Battle_Epic"), TEXT("/Game/Audio/Music/Battle_Epic"), EMusicLayer::Epic, 140.f, 0.9f},
        {TEXT("Battle_Perc"), TEXT("/Game/Audio/Music/Battle_Perc"), EMusicLayer::Percussion, 140.f, 0.8f},
        {TEXT("Diplomacy_Base"), TEXT("/Game/Audio/Music/Diplomacy_Base"), EMusicLayer::Base, 90.f, 0.6f},
        {TEXT("Diplomacy_Tension"), TEXT("/Game/Audio/Music/Diplomacy_Tension"), EMusicLayer::Tension, 90.f, 0.5f},
        {TEXT("Ceremony_Base"), TEXT("/Game/Audio/Music/Ceremony_Base"), EMusicLayer::Base, 72.f, 0.8f},
        {TEXT("Ceremony_Epic"), TEXT("/Game/Audio/Music/Ceremony_Epic"), EMusicLayer::Epic, 72.f, 0.9f},
        {TEXT("Sorrow_Base"), TEXT("/Game/Audio/Music/Sorrow_Base"), EMusicLayer::Base, 60.f, 0.6f},
        {TEXT("Sorrow_Sorrow"), TEXT("/Game/Audio/Music/Sorrow_Sorrow"), EMusicLayer::Sorrow, 60.f, 0.7f},
        {TEXT("Victory_Epic"), TEXT("/Game/Audio/Music/Victory_Epic"), EMusicLayer::Epic, 120.f, 1.0f},
    };
    for(const auto& T : Tracks)
    {
        FMusicTrack Track; Track.TrackID=T.ID; Track.SoundPath=T.Path; Track.Layer=T.Layer; Track.BPM=T.BPM; Track.Volume=T.Vol; Track.bIsLooping=true;
        MusicLibrary.Add(T.ID, Track);
    }
    struct FStingerDef { EStingerType Type; const TCHAR* Path; float Vol; };
    FStingerDef Stingers[] = {
        {EStingerType::Impact, TEXT("/Game/Audio/Stingers/Impact"), 1.0f},
        {EStingerType::Reveal, TEXT("/Game/Audio/Stingers/Reveal"), 0.9f},
        {EStingerType::Victory, TEXT("/Game/Audio/Stingers/Victory"), 1.0f},
        {EStingerType::Sorrow, TEXT("/Game/Audio/Stingers/Sorrow"), 0.8f},
        {EStingerType::Suspense, TEXT("/Game/Audio/Stingers/Suspense"), 0.7f},
        {EStingerType::Transition, TEXT("/Game/Audio/Stingers/Transition"), 0.8f},
        {EStingerType::Horn, TEXT("/Game/Audio/Stingers/Horn"), 0.9f},
    };
    for(const auto& S : Stingers)
    {
        FStinger St; St.Type=S.Type; St.SoundPath=S.Path; St.Volume=S.Vol;
        StingerLibrary.Add(S.Type, St);
    }
}
void UAudioManager::PlayMusic(FName TrackID, float FadeInTime)
{
    if(MusicLibrary.Find(TrackID)) { CurrentBPM = MusicLibrary[TrackID].BPM; BeatPhase = 0.f; }
}
void UAudioManager::StopMusic(float FadeOutTime) {}
void UAudioManager::SetMusicVolume(float Volume) { MusicVolume=FMath::Clamp(Volume,0.f,1.f); }
void UAudioManager::SetMusicMood(EMusicMood NewMood, float TransitionTime)
{
    if(NewMood==CurrentMood) return;
    TargetMood=NewMood; MoodTransition=0.f; MoodTransitionDuration=TransitionTime;
    switch(NewMood)
    {
        case EMusicMood::Peaceful:
            TargetLayerVolumes[EMusicLayer::Base]=1.f; TargetLayerVolumes[EMusicLayer::Tension]=0.f;
            TargetLayerVolumes[EMusicLayer::Epic]=0.f; TargetLayerVolumes[EMusicLayer::Sorrow]=0.f; TargetLayerVolumes[EMusicLayer::Percussion]=0.f; break;
        case EMusicMood::Exploration:
            TargetLayerVolumes[EMusicLayer::Base]=1.f; TargetLayerVolumes[EMusicLayer::Tension]=0.f;
            TargetLayerVolumes[EMusicLayer::Epic]=0.f; TargetLayerVolumes[EMusicLayer::Sorrow]=0.f; TargetLayerVolumes[EMusicLayer::Percussion]=0.5f; break;
        case EMusicMood::Tension:
            TargetLayerVolumes[EMusicLayer::Base]=0.8f; TargetLayerVolumes[EMusicLayer::Tension]=1.f;
            TargetLayerVolumes[EMusicLayer::Epic]=0.f; TargetLayerVolumes[EMusicLayer::Sorrow]=0.f; TargetLayerVolumes[EMusicLayer::Percussion]=0.7f; break;
        case EMusicMood::Battle:
            TargetLayerVolumes[EMusicLayer::Base]=0.7f; TargetLayerVolumes[EMusicLayer::Tension]=0.8f;
            TargetLayerVolumes[EMusicLayer::Epic]=1.f; TargetLayerVolumes[EMusicLayer::Sorrow]=0.f; TargetLayerVolumes[EMusicLayer::Percussion]=1.f; break;
        case EMusicMood::Epic:
            TargetLayerVolumes[EMusicLayer::Base]=0.8f; TargetLayerVolumes[EMusicLayer::Tension]=0.3f;
            TargetLayerVolumes[EMusicLayer::Epic]=1.f; TargetLayerVolumes[EMusicLayer::Sorrow]=0.f; TargetLayerVolumes[EMusicLayer::Percussion]=0.8f; break;
        case EMusicMood::Sorrow:
            TargetLayerVolumes[EMusicLayer::Base]=0.8f; TargetLayerVolumes[EMusicLayer::Tension]=0.f;
            TargetLayerVolumes[EMusicLayer::Epic]=0.f; TargetLayerVolumes[EMusicLayer::Sorrow]=1.f; TargetLayerVolumes[EMusicLayer::Percussion]=0.f; break;
        case EMusicMood::Ceremony:
            TargetLayerVolumes[EMusicLayer::Base]=0.9f; TargetLayerVolumes[EMusicLayer::Tension]=0.f;
            TargetLayerVolumes[EMusicLayer::Epic]=1.f; TargetLayerVolumes[EMusicLayer::Sorrow]=0.f; TargetLayerVolumes[EMusicLayer::Percussion]=0.5f; break;
        case EMusicMood::Victory:
            TargetLayerVolumes[EMusicLayer::Base]=0.7f; TargetLayerVolumes[EMusicLayer::Tension]=0.f;
            TargetLayerVolumes[EMusicLayer::Epic]=1.f; TargetLayerVolumes[EMusicLayer::Sorrow]=0.f; TargetLayerVolumes[EMusicLayer::Percussion]=0.9f; break;
    }
}
void UAudioManager::SetLayerVolume(EMusicLayer Layer, float Volume, float FadeTime) { TargetLayerVolumes[Layer]=FMath::Clamp(Volume,0.f,1.f); }
void UAudioManager::FadeInLayer(EMusicLayer Layer, float FadeTime) { TargetLayerVolumes[Layer]=1.f; }
void UAudioManager::FadeOutLayer(EMusicLayer Layer, float FadeTime) { TargetLayerVolumes[Layer]=0.f; }
void UAudioManager::TriggerTension(float Intensity) { TensionIntensity=FMath::Clamp(Intensity,0.f,1.f); TargetLayerVolumes[EMusicLayer::Tension]=TensionIntensity; }
void UAudioManager::ReleaseTension(float FadeTime) { TensionIntensity=0.f; TargetLayerVolumes[EMusicLayer::Tension]=0.f; }
float UAudioManager::GetBeatProgress() const { return BeatPhase; }
bool UAudioManager::IsOnBeat(float Tolerance) const { return BeatPhase < Tolerance || BeatPhase > (1.f-Tolerance); }
void UAudioManager::PlayStinger(EStingerType Type) {}
void UAudioManager::PlaySFX(FName SFXID, FVector Location) {}
void UAudioManager::PlayUISound(FName SoundID) {}
void UAudioManager::StartDialogueDuck(float DuckAmt, float FadeTime) { bDialogueDucking=true; DuckAmount=DuckAmt; }
void UAudioManager::EndDialogueDuck(float FadeTime) { bDialogueDucking=false; }
void UAudioManager::ExecuteShotAudio(const FShotContract& Shot)
{
    if(Shot.MusicMood!=EMusicMood::Peaceful || Shot.MusicMood!=CurrentMood) SetMusicMood(Shot.MusicMood, Shot.Duration*0.3f);
    if(Shot.Stinger!=EStingerType::Impact) PlayStinger(Shot.Stinger);
    if(Shot.bDialogueDuck) StartDialogueDuck(0.4f, 0.3f);
    if(Shot.SlowMotionFactor<1.f) { TargetLayerVolumes[EMusicLayer::Percussion]=0.3f; }
}
void UAudioManager::Tick(float DeltaTime) { UpdateMoodTransition(DeltaTime); UpdateLayerVolumes(DeltaTime); UpdateBeat(DeltaTime); UpdateDialogueDuck(DeltaTime); }
void UAudioManager::UpdateMoodTransition(float DeltaTime)
{
    if(TargetMood==CurrentMood) return;
    MoodTransition+=DeltaTime;
    if(MoodTransition>=MoodTransitionDuration) CurrentMood=TargetMood;
}
void UAudioManager::UpdateLayerVolumes(float DeltaTime)
{
    for(auto& Pair : LayerVolumes)
    {
        float Target = TargetLayerVolumes.Contains(Pair.Key) ? TargetLayerVolumes[Pair.Key] : 0.f;
        Pair.Value = FMath::FInterpTo(Pair.Value, Target, DeltaTime, 2.f);
    }
}
void UAudioManager::UpdateBeat(float DeltaTime)
{
    if(CurrentBPM<=0.f) return;
    float BeatDuration = 60.f/CurrentBPM;
    BeatPhase += DeltaTime/BeatDuration;
    if(BeatPhase>=1.f) BeatPhase=FMath::Fractional(BeatPhase);
}
void UAudioManager::UpdateDialogueDuck(float DeltaTime)
{
    float TargetDuck = bDialogueDucking ? DuckAmount : 0.f;
    CurrentDuck = FMath::FInterpTo(CurrentDuck, TargetDuck, DeltaTime, 5.f);
}
