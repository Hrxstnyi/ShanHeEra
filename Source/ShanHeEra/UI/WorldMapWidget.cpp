#include "UI/WorldMapWidget.h"
#include "Core/ShanHeLog.h"

void UWorldMapWidget::NativeConstruct()
{
    Super::NativeConstruct();
    RefreshMap();
    if (TravelButton) TravelButton->OnClicked.AddDynamic(this, &UWorldMapWidget::TravelToSelected);
    if (ZoomInButton) ZoomInButton->OnClicked.AddDynamic(this, &UWorldMapWidget::ZoomIn);
    if (ZoomOutButton) ZoomOutButton->OnClicked.AddDynamic(this, &UWorldMapWidget::ZoomOut);
    if (FilterButton) FilterButton->OnClicked.AddDynamic(this, &UWorldMapWidget::ToggleFilter);
}

void UWorldMapWidget::RefreshMap()
{
    UE_LOG(LogShanHe, Log, TEXT("大地图刷新 - 缩放:%.1f, 显示城市:%d 村庄:%d 危险:%d"),
        ZoomLevel, bShowCities, bShowVillages, bShowDangers);
}

void UWorldMapWidget::SelectLocation(FName LocationID)
{
    SelectedLocation = LocationID;
    UE_LOG(LogShanHe, Log, TEXT("选中地点：%s"), *LocationID.ToString());
}

void UWorldMapWidget::TravelToSelected()
{
    if (!SelectedLocation.IsNone())
        UE_LOG(LogShanHe, Log, TEXT("前往：%s"), *SelectedLocation.ToString());
}

void UWorldMapWidget::ZoomIn()
{
    ZoomLevel = FMath::Min(MaxZoom, ZoomLevel + 0.25f);
    RefreshMap();
}

void UWorldMapWidget::ZoomOut()
{
    ZoomLevel = FMath::Max(MinZoom, ZoomLevel - 0.25f);
    RefreshMap();
}

void UWorldMapWidget::ToggleFilter()
{
    bShowDangers = !bShowDangers;
    RefreshMap();
    UE_LOG(LogShanHe, Log, TEXT("切换过滤器 - 显示危险点:%d"), bShowDangers);
}
