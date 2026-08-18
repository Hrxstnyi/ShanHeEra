#include "UI/SectWidget.h"
#include "Core/ShanHeLog.h"

void USectWidget::NativeConstruct()
{
    Super::NativeConstruct();
    if (AcceptMissionButton) AcceptMissionButton->OnClicked.AddDynamic(this, &USectWidget::AcceptMission);
    if (DeclareWarButton) DeclareWarButton->OnClicked.AddDynamic(this, &USectWidget::DeclareWar, NAME_None);
    if (ProposeAllyButton) ProposeAllyButton->OnClicked.AddDynamic(this, &USectWidget::ProposeAlliance, NAME_None);
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

void USectWidget::DeclareWar(FName EnemySectID)
{
    if (!EnemySectID.IsNone())
    {
        UE_LOG(LogShanHe, Log, TEXT("向 %s 宣战"), *EnemySectID.ToString());
        RefreshSect();
    }
}

void USectWidget::ProposeAlliance(FName AllySectID)
{
    if (!AllySectID.IsNone())
    {
        UE_LOG(LogShanHe, Log, TEXT("向 %s 提议结盟"), *AllySectID.ToString());
        RefreshSect();
    }
}

void USectWidget::SwitchTab(int32 TabIndex)
{
    CurrentTab = FMath::Clamp(TabIndex, 0, 3);
    RefreshSect();
    UE_LOG(LogShanHe, Log, TEXT("切换帮派页面：%d"), CurrentTab);
}
