#include "UI/SkillTreeWidget.h"
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

void USkillTreeWidget::NativeConstruct()
{
    Super::NativeConstruct();
    if (LearnButton) LearnButton->OnClicked.AddDynamic(this, &USkillTreeWidget::LearnSelectedSkill);
    if (UpgradeButton) UpgradeButton->OnClicked.AddDynamic(this, &USkillTreeWidget::UpgradeSelectedSkill);
    RefreshSkillTree();
}

void USkillTreeWidget::RefreshSkillTree()
{
    if (TreeCanvas) TreeCanvas->ClearChildren();
    // 根据CurrentTab加载对应技能树数据
    // 实际项目中从MartialArtsSystem/AttributeSystem获取
    UE_LOG(LogShanHe, Log, TEXT("技能树刷新 - 当前分类:%d, 技能点:%d"), (int32)CurrentTab, AvailableSkillPoints);
}

void USkillTreeWidget::SelectSkill(FName SkillID)
{
    SelectedSkillID = SkillID;
    if (const FSkillNode* Node = SkillNodes.Find(SkillID))
    {
        if (SkillNameText) SkillNameText->SetText(Node->SkillName);
        if (SkillDescText) SkillDescText->SetText(Node->Description);
        if (SkillLevelText) SkillLevelText->SetText(FText::FromString(FString::Printf(TEXT("%d/%d"), Node->Level, Node->MaxLevel)));
        UE_LOG(LogShanHe, Log, TEXT("选中技能：%s"), *SkillID.ToString());
    }
}

void USkillTreeWidget::LearnSelectedSkill()
{
    if (SelectedSkillID.IsNone()) return;
    if (!CanLearnSkill(SelectedSkillID))
    {
        UE_LOG(LogShanHe, Warning, TEXT("无法学习：前置技能未满足或技能点不足"));
        return;
    }
    if (FSkillNode* Node = SkillNodes.Find(SelectedSkillID))
    {
        Node->bUnlocked = true;
        Node->Level = 1;
        AvailableSkillPoints--;
        UE_LOG(LogShanHe, Log, TEXT("学习技能：%s"), *SelectedSkillID.ToString());
        RefreshSkillTree();
    }
}

void USkillTreeWidget::UpgradeSelectedSkill()
{
    if (SelectedSkillID.IsNone()) return;
    if (FSkillNode* Node = SkillNodes.Find(SelectedSkillID))
    {
        if (Node->Level >= Node->MaxLevel) return;
        if (AvailableSkillPoints <= 0) return;
        Node->Level++;
        AvailableSkillPoints--;
        UE_LOG(LogShanHe, Log, TEXT("升级技能：%s → %d级"), *SelectedSkillID.ToString(), Node->Level);
        RefreshSkillTree();
    }
}

void USkillTreeWidget::SwitchTab(ESkillTreeTab NewTab)
{
    CurrentTab = NewTab;
    SelectedSkillID = NAME_None;
    RefreshSkillTree();
    UE_LOG(LogShanHe, Log, TEXT("切换技能分类：%d"), (int32)NewTab);
}

bool USkillTreeWidget::CanLearnSkill(FName SkillID) const
{
    if (AvailableSkillPoints <= 0) return false;
    if (const FSkillNode* Node = SkillNodes.Find(SkillID))
    {
        if (Node->bUnlocked) return false;
        for (FName Prereq : Node->Prerequisites)
        {
            if (const FSkillNode* PrereqNode = SkillNodes.Find(Prereq))
                if (!PrereqNode->bUnlocked) return false;
        }
        return true;
    }
    return false;
}
