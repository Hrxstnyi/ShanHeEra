#include "UI/HUDWidget.h"
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
