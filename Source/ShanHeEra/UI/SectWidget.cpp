#include "UI/SectWidget.h"
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

void USectWidget::NativeConstruct()
{
    Super::NativeConstruct();
    if (AcceptMissionButton) AcceptMissionButton->OnClicked.AddDynamic(this, &USectWidget::AcceptMission);
    if (DeclareWarButton) DeclareWarButton->OnClicked.AddDynamic(this, &USectWidget::DeclareWar);
    if (ProposeAllyButton) ProposeAllyButton->OnClicked.AddDynamic(this, &USectWidget::ProposeAlliance);
    RefreshSect();
}

void USectWidget::RefreshSect()
{
    // 从SectSystem获取门派数据
    UE_LOG(LogShanHe, Log, TEXT("帮派界面刷新 - 当前页:%d"), CurrentTab);
}

void USectWidget::SelectMember(FName MemberID)
{
    SelectedMemberID = MemberID;
    UE_LOG(LogShanHe, Log, TEXT("选中门派成员：%s"), *MemberID.ToString());
}

void USectWidget::SelectMission(FName MissionID)
{
    SelectedMissionID = MissionID;
    UE_LOG(LogShanHe, Log, TEXT("选中门派任务：%s"), *MissionID.ToString());
}

void USectWidget::AcceptMission()
{
    if (!SelectedMissionID.IsNone())
    {
        UE_LOG(LogShanHe, Log, TEXT("接取门派任务：%s"), *SelectedMissionID.ToString());
        RefreshSect();
    }
}

void USectWidget::DeclareWar()
{
    UE_LOG(LogShanHe, Log, TEXT("宣战"));
    RefreshSect();
}

void USectWidget::ProposeAlliance()
{
    UE_LOG(LogShanHe, Log, TEXT("提议结盟"));
    RefreshSect();
}

void USectWidget::SwitchTab(int32 TabIndex)
{
    CurrentTab = FMath::Clamp(TabIndex, 0, 3);
    RefreshSect();
    UE_LOG(LogShanHe, Log, TEXT("切换帮派页面：%d"), CurrentTab);
}
