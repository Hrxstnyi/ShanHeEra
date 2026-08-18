#include "UI/CodexWidget.h"
#include "Core/ShanHeLog.h"

void UCodexWidget::NativeConstruct()
{
    Super::NativeConstruct();
    RefreshCodex();
}

void UCodexWidget::RefreshCodex()
{
    if (EntryGrid) EntryGrid->ClearChildren();
    // 从CodexSystem获取当前分类的图鉴条目
    // 已发现的显示正常，未发现的显示问号
    int32 CatProgress = GetCategoryProgress();
    if (CategoryProgress) CategoryProgress->SetPercent(CatProgress / 100.0f);
    if (ProgressText) ProgressText->SetText(FText::FromString(FString::Printf(TEXT("%d%%"), CatProgress)));
    if (TotalProgressText) TotalProgressText->SetText(FText::FromString(FString::Printf(TEXT("总进度: %d%%"), GetTotalProgress())));
    UE_LOG(LogShanHe, Log, TEXT("图鉴界面刷新 - 分类:%d, 进度:%d%%"), (int32)CurrentCategory, CatProgress);
}

void UCodexWidget::SelectEntry(FName EntryID)
{
    SelectedEntryID = EntryID;
    UE_LOG(LogShanHe, Log, TEXT("选中图鉴条目：%s"), *EntryID.ToString());
}

void UCodexWidget::SwitchCategory(ECodexTab NewTab)
{
    CurrentCategory = NewTab;
    SelectedEntryID = NAME_None;
    RefreshCodex();
    UE_LOG(LogShanHe, Log, TEXT("切换图鉴分类：%d"), (int32)NewTab);
}

int32 UCodexWidget::GetCategoryProgress() const
{
    // 从CodexSystem获取当前分类进度
    return 0;
}

int32 UCodexWidget::GetTotalProgress() const
{
    // 从CodexSystem获取总进度
    return 0;
}
