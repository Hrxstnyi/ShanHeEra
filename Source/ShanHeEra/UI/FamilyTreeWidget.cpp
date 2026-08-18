#include "UI/FamilyTreeWidget.h"
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

void UFamilyTreeWidget::NativeConstruct()
{
    Super::NativeConstruct();
    if (ZoomInButton) ZoomInButton->OnClicked.AddDynamic(this, &UFamilyTreeWidget::ZoomIn);
    if (ZoomOutButton) ZoomOutButton->OnClicked.AddDynamic(this, &UFamilyTreeWidget::ZoomOut);
    if (CenterOnPlayerButton) CenterOnPlayerButton->OnClicked.AddDynamic(this, &UFamilyTreeWidget::CenterOnPlayer);
    RefreshFamilyTree();
}

void UFamilyTreeWidget::RefreshFamilyTree()
{
    if (TreeCanvas) TreeCanvas->ClearChildren();
    // 从FamilySystem获取家族数据，按代排列
    // 第一代在最上方，后代依次向下，连线表示父母-子女关系
    int32 GenCount = GetGenerationCount();
    if (GenerationText) GenerationText->SetText(FText::FromString(FString::Printf(TEXT("第 %d 代"), GenCount)));
    UE_LOG(LogShanHe, Log, TEXT("家族树刷新 - %d代 %d人, 缩放:%.1f"), GenCount, GetTotalMembers(), ZoomLevel);
}

void UFamilyTreeWidget::SelectMember(FName MemberID)
{
    SelectedMemberID = MemberID;
    if (const FFamilyMemberNode* M = FamilyMembers.Find(MemberID))
    {
        if (MemberNameText) MemberNameText->SetText(M->Name);
        if (MemberTitleText) MemberTitleText->SetText(M->Title);
        FString Bio = FString::Printf(TEXT("%d年生"), M->BirthYear);
        if (!M->bIsAlive) Bio += FString::Printf(TEXT(", %d年卒"), M->DeathYear);
        if (MemberBioText) MemberBioText->SetText(FText::FromString(Bio));
        UE_LOG(LogShanHe, Log, TEXT("选中家族成员：%s (第%d代)"), *M->Name.ToString(), M->Generation);
    }
}

void UFamilyTreeWidget::ZoomIn()
{
    ZoomLevel = FMath::Min(MaxZoom, ZoomLevel + 0.25f);
    RefreshFamilyTree();
}

void UFamilyTreeWidget::ZoomOut()
{
    ZoomLevel = FMath::Max(MinZoom, ZoomLevel - 0.25f);
    RefreshFamilyTree();
}

void UFamilyTreeWidget::CenterOnPlayer()
{
    // 将视图居中到玩家角色
    UE_LOG(LogShanHe, Log, TEXT("居中到玩家角色"));
}

int32 UFamilyTreeWidget::GetGenerationCount() const
{
    int32 MaxGen = 0;
    for (const auto& Pair : FamilyMembers)
        MaxGen = FMath::Max(MaxGen, Pair.Value.Generation);
    return MaxGen;
}

int32 UFamilyTreeWidget::GetTotalMembers() const
{
    return FamilyMembers.Num();
}
