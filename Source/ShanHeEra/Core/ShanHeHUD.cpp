#include "Core/ShanHeHUD.h"
#include "UI/HUDWidget.h"
#include "Core/ShanHeLog.h"

AShanHeHUD::AShanHeHUD()
{
}

void AShanHeHUD::BeginPlay()
{
    Super::BeginPlay();

    if (HUDWidgetClass)
    {
        HUDWidget = CreateWidget<UHUDWidget>(GetWorld(), HUDWidgetClass);
        if (HUDWidget)
        {
            HUDWidget->AddToViewport();
            UE_LOG(LogShanHe, Log, TEXT("HUD已创建"));
        }
    }
    else
    {
        UE_LOG(LogShanHe, Warning, TEXT("HUDWidgetClass未设置，请在蓝图中配置"));
    }
}

void AShanHeHUD::SetHUDVisible(bool bVisible)
{
    if (HUDWidget)
    {
        HUDWidget->SetVisibility(bVisible ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
    }
}
