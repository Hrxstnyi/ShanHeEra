#include "Core/TutorialSystem.h"
#include "Core/ShanHeLog.h"

void UTutorialSystem::InitializeTutorials()
{
    // 第一章：基础操作
    FTutorialChapter Ch1;
    Ch1.ChapterID = TEXT("BasicControls");
    Ch1.ChapterName = NSLOCTEXT("Tutorial","Ch1","基础操作");
    Ch1.bIsUnlocked = true;

    FTutorialStep S1;
    S1.StepID = TEXT("Move"); S1.Title = NSLOCTEXT("Tutorial","Move","移动");
    S1.Description = NSLOCTEXT("Tutorial","MoveDesc","使用WASD移动角色");
    S1.Hint = NSLOCTEXT("Tutorial","MoveHint","按 W 键向前移动");
    Ch1.Steps.Add(S1);

    FTutorialStep S2;
    S2.StepID = TEXT("Interact"); S2.Title = NSLOCTEXT("Tutorial","Interact","交互");
    S2.Description = NSLOCTEXT("Tutorial","InteractDesc","靠近NPC或物品按E交互");
    S2.Hint = NSLOCTEXT("Tutorial","InteractHint","按 E 键与附近的人对话");
    Ch1.Steps.Add(S2);

    Chapters.Add(Ch1);

    // 第二章：人生模式
    FTutorialChapter Ch2;
    Ch2.ChapterID = TEXT("LifeMode");
    Ch2.ChapterName = NSLOCTEXT("Tutorial","Ch2","人生模式");
    FTutorialStep S3;
    S3.StepID = TEXT("OpenMenu"); S3.Title = NSLOCTEXT("Tutorial","Menu","打开菜单");
    S3.Description = NSLOCTEXT("Tutorial","MenuDesc","按Tab打开角色菜单查看属性");
    Ch2.Steps.Add(S3);
    Chapters.Add(Ch2);

    UE_LOG(LogShanHe, Log, TEXT("新手引导系统初始化 - %d 章教程"), Chapters.Num());
}

void UTutorialSystem::StartTutorial(FName ChapterID)
{
    for (int32 i = 0; i < Chapters.Num(); i++)
    {
        if (Chapters[i].ChapterID == ChapterID)
        {
            CurrentChapterIndex = i;
            CurrentStepIndex = 0;
            bIsActive = true;
            Chapters[i].bIsUnlocked = true;
            UE_LOG(LogShanHe, Log, TEXT("开始教程：%s"), *Chapters[i].ChapterName.ToString());
            return;
        }
    }
}

void UTutorialSystem::CompleteStep(FName StepID)
{
    if (!bIsActive) return;
    FTutorialChapter& Ch = Chapters[CurrentChapterIndex];
    if (CurrentStepIndex < Ch.Steps.Num())
    {
        Ch.Steps[CurrentStepIndex].bIsCompleted = true;
        CurrentStepIndex++;
        if (CurrentStepIndex >= Ch.Steps.Num())
        {
            UE_LOG(LogShanHe, Log, TEXT("教程章节完成：%s"), *Ch.ChapterName.ToString());
            bIsActive = false;
        }
    }
}

void UTutorialSystem::SkipTutorial()
{
    bIsActive = false;
    UE_LOG(LogShanHe, Log, TEXT("跳过教程"));
}

FTutorialStep UTutorialSystem::GetCurrentStep() const
{
    if (bIsActive && CurrentChapterIndex < Chapters.Num())
    {
        const FTutorialChapter& Ch = Chapters[CurrentChapterIndex];
        if (CurrentStepIndex < Ch.Steps.Num())
            return Ch.Steps[CurrentStepIndex];
    }
    return FTutorialStep();
}
