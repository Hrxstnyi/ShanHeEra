#include "UI/HUDWidget.h"
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
#include "Components/Button.h"
#include "Components/TextBlock.h"

void UHUDWidget::UpdateDate(const FString& DateStr)
{
    if (DateText) DateText->SetText(FText::FromString(DateStr));
}

void UHUDWidget::ShowNotification(const FString& Message, float Duration)
{
    if (EventNotification)
    {
        EventNotification->SetText(FText::FromString(Message));
        EventNotification->SetVisibility(ESlateVisibility::Visible);
    }
}
