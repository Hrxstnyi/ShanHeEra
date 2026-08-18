#include "UI/EquipmentWidget.h"
#include "Core/ShanHeLog.h"

void UEquipmentWidget::NativeConstruct()
{
    Super::NativeConstruct();
    RefreshEquipment();
    if (UnequipButton) UnequipButton->OnClicked.AddDynamic(this, &UEquipmentWidget::UnequipSelected);
}

void UEquipmentWidget::RefreshEquipment()
{
    UE_LOG(LogShanHe, Log, TEXT("装备界面刷新 - 10个装备槽"));
}

void UEquipmentWidget::SelectEquipSlot(EEquipSlot InSlot)
{
    SelectedSlot = InSlot;
    UE_LOG(LogShanHe, Log, TEXT("选中装备槽：%d"), (int32)Slot);
}

void UEquipmentWidget::UnequipSelected()
{
    UE_LOG(LogShanHe, Log, TEXT("卸下装备：%d"), (int32)SelectedSlot);
    RefreshEquipment();
}
