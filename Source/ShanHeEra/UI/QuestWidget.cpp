#include "UI/QuestWidget.h"
#include "Core/ShanHeLog.h"

void UQuestWidget::NativeConstruct()
{
    Super::NativeConstruct();
    RefreshQuests();
    if (TrackButton) TrackButton->OnClicked.AddDynamic(this, &UQuestWidget::TrackQuest);
    if (AbandonButton) AbandonButton->OnClicked.AddDynamic(this, &UQuestWidget::AbandonQuest);
}

void UQuestWidget::RefreshQuests()
{
    if (ActiveQuestList) ActiveQuestList->ClearChildren();
    if (CompletedQuestList) CompletedQuestList->ClearChildren();
    UE_LOG(LogShanHe, Log, TEXT("任务界面刷新 - 进行中/已完成"));
}

void UQuestWidget::SelectQuest(FName QuestID)
{
    SelectedQuestID = QuestID;
    UE_LOG(LogShanHe, Log, TEXT("选中任务：%s"), *QuestID.ToString());
}

void UQuestWidget::TrackQuest()
{
    if (!SelectedQuestID.IsNone())
        UE_LOG(LogShanHe, Log, TEXT("追踪任务：%s"), *SelectedQuestID.ToString());
}

void UQuestWidget::AbandonQuest()
{
    if (!SelectedQuestID.IsNone())
    {
        UE_LOG(LogShanHe, Log, TEXT("放弃任务：%s"), *SelectedQuestID.ToString());
        RefreshQuests();
    }
}
