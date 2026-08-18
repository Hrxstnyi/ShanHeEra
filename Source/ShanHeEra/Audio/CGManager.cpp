#include "Audio/CGManager.h"
#include "Core/CinematicDirector.h"
#include "Core/ShanHeLog.h"

UCGManager::UCGManager() {}

void UCGManager::InitializeCG()
{
    FCGSequence Seq1;
    Seq1.CGID = TEXT("CG_Founding");
    Seq1.MediaPath = TEXT("/Game/CG/Founding");
    Seq1.Title = FText::FromString(TEXT("开国大典"));
    Seq1.Duration = 16.5f;
    Seq1.bCanSkip = true;
    Seq1.CinematicSequenceID = ECinematicSequenceID::FoundingCeremony;
    CGLibrary.Add(Seq1.CGID, Seq1);

    FCGSequence Seq2;
    Seq2.CGID = TEXT("CG_War");
    Seq2.MediaPath = TEXT("/Game/CG/War");
    Seq2.Title = FText::FromString(TEXT("烽火连天"));
    Seq2.Duration = 15.0f;
    Seq2.bCanSkip = true;
    Seq2.CinematicSequenceID = ECinematicSequenceID::FlamesOfWar;
    CGLibrary.Add(Seq2.CGID, Seq2);

    FCGSequence Seq3;
    Seq3.CGID = TEXT("CG_Reunification");
    Seq3.MediaPath = TEXT("/Game/CG/Reunification");
    Seq3.Title = FText::FromString(TEXT("九州一统"));
    Seq3.Duration = 15.5f;
    Seq3.bCanSkip = true;
    Seq3.CinematicSequenceID = ECinematicSequenceID::Unification;
    CGLibrary.Add(Seq3.CGID, Seq3);

    FCGSequence Seq4;
    Seq4.CGID = TEXT("CG_TaiwanReturn");
    Seq4.MediaPath = TEXT("/Game/CG/TaiwanReturn");
    Seq4.Title = FText::FromString(TEXT("宝岛归心"));
    Seq4.Duration = 18.5f;
    Seq4.bCanSkip = true;
    Seq4.CinematicSequenceID = ECinematicSequenceID::IslandReturn;
    CGLibrary.Add(Seq4.CGID, Seq4);
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

    if(UWorld* World = GetWorld())
    {
        if(UCinematicDirector* Director = World->GetGameInstance()->GetSubsystem<UCinematicDirector>())
        {
            Director->PlaySequence(CGID);
        }
    }
}
void UCGManager::StopCG() { bIsPlaying=false; bSubtitleVisible=false; CurrentSubtitleIndex=-1; }
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

void UCGManager::SkipCG()
{
    if (bIsPlaying)
    {
        UE_LOG(LogShanHe, Log, TEXT("跳过CG: %s"), *CurrentCGID.ToString());
        StopCG();
    }
}
