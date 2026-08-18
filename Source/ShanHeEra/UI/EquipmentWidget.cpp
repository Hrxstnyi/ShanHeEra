#include "UI/EquipmentWidget.h"
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
    UE_LOG(LogShanHe, Log, TEXT("选中装备槽：%d"), (int32)InSlot);
}

void UEquipmentWidget::UnequipSelected()
{
    UE_LOG(LogShanHe, Log, TEXT("卸下装备：%d"), (int32)SelectedSlot);
    RefreshEquipment();
}
