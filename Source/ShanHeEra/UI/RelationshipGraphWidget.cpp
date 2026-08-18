#include "UI/RelationshipGraphWidget.h"
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

void URelationshipGraphWidget::NativeConstruct()
{
    Super::NativeConstruct();
    if (RefreshButton) RefreshButton->OnClicked.AddDynamic(this, &URelationshipGraphWidget::RefreshGraph);
    if (FilterAll) FilterAll->OnClicked.AddDynamic(this, &URelationshipGraphWidget::ShowAllRelations);
    RefreshGraph();
}

void URelationshipGraphWidget::RefreshGraph()
{
    if (GraphCanvas) GraphCanvas->ClearChildren();
    // 从RelationshipSystem获取所有NPC关系
    // 节点按关系类型着色，连线粗细表示关系强度
    UE_LOG(LogShanHe, Log, TEXT("关系图谱刷新 - %d个节点, %d条连线"), Nodes.Num(), Edges.Num());
}

void URelationshipGraphWidget::SelectNPC(FName NPCID)
{
    SelectedNPCID = NPCID;
    if (const FRelationNode* N = Nodes.Find(NPCID))
    {
        if (NPCNameText) NPCNameText->SetText(N->Name);
        if (RelationValueText) RelationValueText->SetText(FText::FromString(FString::Printf(TEXT("好感度: %d"), N->RelationValue)));
        UE_LOG(LogShanHe, Log, TEXT("选中NPC：%s, 好感度%d"), *NPCID.ToString(), N->RelationValue);
    }
}

void URelationshipGraphWidget::SetFilter(ERelationLineType FilterType)
{
    CurrentFilter = FilterType;
    bFilterActive = true;
    RefreshGraph();
    UE_LOG(LogShanHe, Log, TEXT("关系过滤：%d"), (int32)FilterType);
}

void URelationshipGraphWidget::ShowAllRelations()
{
    bFilterActive = false;
    RefreshGraph();
    UE_LOG(LogShanHe, Log, TEXT("显示全部关系"));
}

TArray<FRelationEdge> URelationshipGraphWidget::GetRelationsOfNPC(FName NPCID) const
{
    TArray<FRelationEdge> Result;
    for (const FRelationEdge& E : Edges)
    {
        if (E.FromID == NPCID || E.ToID == NPCID)
            Result.Add(E);
    }
    return Result;
}
