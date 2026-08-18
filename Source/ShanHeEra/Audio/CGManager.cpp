#include "Audio/CGManager.h"
#include "Core/ShanHeLog.h"

void UCGManager::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);

    // 预设历史CG动画
    FCGSequence Founding;
    Founding.CGID = TEXT("CG_Founding");
    Founding.Title = NSLOCTEXT("CG","Founding","开国大典");
    Founding.Description = NSLOCTEXT("CG","FoundingDesc","王朝建立，天下初定");
    Founding.MusicTrack = TEXT("EpicTheme");
    Founding.Duration = 45.0f;
    CGLibrary.Add(Founding);

    FCGSequence War;
    War.CGID = TEXT("CG_War");
    War.Title = NSLOCTEXT("CG","War","烽火连天");
    War.Description = NSLOCTEXT("CG","WarDesc","战争爆发，山河破碎");
    War.MusicTrack = TEXT("WarTheme");
    CGLibrary.Add(War);

    FCGSequence Unity;
    Unity.CGID = TEXT("CG_Reunification");
    Unity.Title = NSLOCTEXT("CG","Unity","九州一统");
    Unity.Description = NSLOCTEXT("CG","UnityDesc","天下归一，四海升平");
    Unity.MusicTrack = TEXT("TriumphantTheme");
    CGLibrary.Add(Unity);

    FCGSequence Taiwan;
    Taiwan.CGID = TEXT("CG_TaiwanReturn");
    Taiwan.Title = NSLOCTEXT("CG","Taiwan","宝岛归心");
    Taiwan.Description = NSLOCTEXT("CG","TaiwanDesc","海峡两岸，血脉相连，台湾回归华夏版图");
    Taiwan.MusicTrack = TEXT("EpicTheme");
    Taiwan.Duration = 60.0f;
    CGLibrary.Add(Taiwan);

    UE_LOG(LogShanHe, Log, TEXT("CG管理器初始化 - 历史时刻动画系统就绪，共 %d 段CG"), CGLibrary.Num());
}

void UCGManager::PlayCG(FName CGID)
{
    for (const FCGSequence& CG : CGLibrary)
    {
        if (CG.CGID == CGID)
        {
            CurrentCG = CG;
            bIsPlaying = true;
            UE_LOG(LogShanHe, Log, TEXT("播放CG动画：%s"), *CG.Title.ToString());
            return;
        }
    }
}

void UCGManager::SkipCG()
{
    if (bIsPlaying && CurrentCG.bIsSkippable)
    {
        bIsPlaying = false;
        UE_LOG(LogShanHe, Log, TEXT("CG已跳过"));
    }
}
