#include "Audio/CGManager.h"
#include "Core/ShanHeLog.h"
#include "Core/CinematicDirector.h"
UCGManager::UCGManager() {}
void UCGManager::InitializeCG()
{
    struct FCGDef { FName ID; const TCHAR* Path; const TCHAR* Title; float Dur; bool Skip; ECinematicSequenceID SeqID; };
    FCGDef CGs[] = {
        {TEXT("CG_Founding"), TEXT("/Game/CG/Founding"), TEXT("开国大典"), 16.5f, true, ECinematicSequenceID::CINE_Founding},
        {TEXT("CG_War"), TEXT("/Game/CG/War"), TEXT("烽火连天"), 15.0f, true, ECinematicSequenceID::CINE_War},
        {TEXT("CG_Reunification"), TEXT("/Game/CG/Reunification"), TEXT("九州一统"), 15.5f, true, ECinematicSequenceID::CINE_Reunification},
        {TEXT("CG_TaiwanReturn"), TEXT("/Game/CG/TaiwanReturn"), TEXT("宝岛归心"), 18.5f, true, ECinematicSequenceID::CINE_TaiwanReturn},
    };
    for(const auto& C : CGs)
    {
        FCGSequence Seq; Seq.CGID=C.ID; Seq.MediaPath=C.Path; Seq.Title=FText::FromString(C.Title);
        Seq.Duration=C.Dur; Seq.bCanSkip=C.Skip; Seq.CinematicSequenceID=C.SeqID;
        CGLibrary.Add(C.ID, Seq);
    }
}
void UCGManager::PlayCG(FName CGID)
{
    if(!CGLibrary.Find(CGID)) return;
    CurrentCGID=CGID; bIsPlaying=true; PlaybackTime=0.f; CurrentSubtitleIndex=-1; bSubtitleVisible=false;
}
void UCGManager::PlayCGWithCinematic(FName CGID)
{
    if(!CGLibrary.Find(CGID)) return;
    PlayCG(CGID);
    FCinematicSequence Cinematic = GetCinematicForCG(CGID);
    if(Cinematic.SequenceID != ECinematicSequenceID::None)
    {
        if(UWorld* World = GetWorld())
        {
            if(UCinematicDirector* Director = World->GetGameInstance()->GetSubsystem<UCinematicDirector>())
            {
                Director->PlaySequence(Cinematic.SequenceID);
            }
        }
    }
}
void UCGManager::StopCG() { bIsPlaying=false; bSubtitleVisible=false; CurrentSubtitleIndex=-1; }
void UCGManager::SkipCG() { StopCG(); }
float UCGManager::GetPlaybackTime() const { return PlaybackTime; }
FCinematicSequence UCGManager::GetCinematicForCG(FName CGID)
{
    FCinematicSequence Result;
    if(FCGSequence* CG = CGLibrary.Find(CGID))
    {
        if(UWorld* World = GetWorld())
        {
            if(UCinematicDirector* Director = World->GetGameInstance()->GetSubsystem<UCinematicDirector>())
            {
                Result = Director->GetSequence(CG->CinematicSequenceID);
            }
        }
    }
    return Result;
}
void UCGManager::ShowSubtitle(const FSubtitleEntry& Subtitle)
{
    CurrentSubtitle = Subtitle.Text; bSubtitleVisible = true;
}
void UCGManager::HideSubtitle() { bSubtitleVisible = false; CurrentSubtitle = FText::GetEmpty(); }
void UCGManager::UpdateSubtitles(float DeltaTime)
{
    if(!bIsPlaying) return;
    PlaybackTime += DeltaTime;
    if(FCGSequence* CG = CGLibrary.Find(CurrentCGID))
    {
        for(int32 i=0; i<CG->Subtitles.Num(); i++)
        {
            const FSubtitleEntry& Sub = CG->Subtitles[i];
            if(PlaybackTime >= Sub.StartTime && PlaybackTime < Sub.StartTime + Sub.Duration)
            {
                if(i != CurrentSubtitleIndex) { CurrentSubtitleIndex = i; ShowSubtitle(Sub); }
                return;
            }
        }
        if(bSubtitleVisible) { HideSubtitle(); CurrentSubtitleIndex = -1; }
        if(PlaybackTime >= CG->Duration) { StopCG(); }
    }
}
