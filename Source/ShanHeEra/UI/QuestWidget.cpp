#include "UI/QuestWidget.h"
#include "Components/SizeBox.h"
#include "Components/Overlay.h"
#include "Components/Border.h"
#include "Components/Slider.h"
#include "Components/ComboBoxString.h"
#include "Components/ScrollBox.h"
#include "Components/HorizontalBox.h"
#include "Components/VerticalBox.h"
#include "Components/CanvasPanel.h"
#include "Components/GridPanel.h"
#include "Components/ProgressBar.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
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
