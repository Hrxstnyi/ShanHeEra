#include "UI/InventoryWidget.h"
#include "Core/ShanHeLog.h"

void UInventoryWidget::NativeConstruct()
{
    Super::NativeConstruct();
    RefreshInventory();
    if (UseButton) UseButton->OnClicked.AddDynamic(this, &UInventoryWidget::UseSelectedItem);
    if (DropButton) DropButton->OnClicked.AddDynamic(this, &UInventoryWidget::DropSelectedItem);
    if (SortButton) SortButton->OnClicked.AddDynamic(this, &UInventoryWidget::SortItems);
}

void UInventoryWidget::RefreshInventory()
{
    if (!ItemGrid) return;
    ItemGrid->ClearChildren();
    // 生成48个格子
    for (int32 i = 0; i < Columns * Rows; i++)
    {
        // 实际项目中从玩家背包获取物品数据
        int32 Col = i % Columns;
        int32 Row = i / Columns;
        // 创建SlotWidget并添加到Grid
    }
    UE_LOG(LogShanHe, Log, TEXT("背包界面刷新 - %d格"), Columns * Rows);
}

void UInventoryWidget::SelectItem(int32 SlotIndex)
{
    SelectedSlot = SlotIndex;
    UE_LOG(LogShanHe, Log, TEXT("选中物品格：%d"), SlotIndex);
}

void UInventoryWidget::UseSelectedItem()
{
    if (SelectedSlot >= 0)
        UE_LOG(LogShanHe, Log, TEXT("使用物品：%d"), SelectedSlot);
}

void UInventoryWidget::DropSelectedItem()
{
    if (SelectedSlot >= 0)
        UE_LOG(LogShanHe, Log, TEXT("丢弃物品：%d"), SelectedSlot);
}

void UInventoryWidget::SortItems()
{
    UE_LOG(LogShanHe, Log, TEXT("整理背包"));
    RefreshInventory();
}
