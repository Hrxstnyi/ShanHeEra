#include "UI/MapSystem.h"
#include "Core/ShanHeLog.h"

void UMapSystem::InitializeMap()
{
    // 初始化主要地点标记
    struct FMarkerDef { FName ID; EMapIconType Type; const TCHAR* Label; float X; float Y; FName Region; };
    FMarkerDef Markers[] = {
        {TEXT("Beijing"), EMapIconType::City, NSLOCTEXT("Map","Beijing","京城"), 0, 0, TEXT("NorthPlain")},
        {TEXT("Nanjing"), EMapIconType::City, NSLOCTEXT("Map","Nanjing","金陵"), 500, -200, TEXT("Jiangnan")},
        {TEXT("Hangzhou"), EMapIconType::City, NSLOCTEXT("Map","Hangzhou","杭州"), 600, -300, TEXT("Jiangnan")},
        {TEXT("Quanzhou"), EMapIconType::City, NSLOCTEXT("Map","Quanzhou","泉州"), 550, -500, TEXT("Coast")},
        {TEXT("TaiwanFu"), EMapIconType::City, NSLOCTEXT("Map","Taiwan","台湾府"), 700, -600, TEXT("Taiwan")},
        {TEXT("Xian"), EMapIconType::City, NSLOCTEXT("Map","Xian","长安"), -300, -100, TEXT("Northwest")},
        {TEXT("Chengdu"), EMapIconType::City, NSLOCTEXT("Map","Chengdu","成都"), -400, -300, TEXT("Southwest")},
        {TEXT("Shaolin"), EMapIconType::Temple, NSLOCTEXT("Map","Shaolin","少林寺"), -100, 50, TEXT("NorthPlain")},
        {TEXT("Wudang"), EMapIconType::Temple, NSLOCTEXT("Map","Wudang","武当山"), -200, -150, TEXT("Inland")},
    };

    for (const auto& M : Markers)
    {
        FMapMarker Marker;
        Marker.MarkerID = M.ID;
        Marker.IconType = M.Type;
        Marker.Label = M.Label;
        Marker.WorldPosition = FVector2D(M.X, M.Y);
        Marker.RegionID = M.Region;
        Marker.bIsDiscovered = true; // 主要城市默认已发现
        AllMarkers.Add(Marker);
    }

    UE_LOG(LogShanHe, Log, TEXT("地图系统初始化 - %d 个地点标记"), AllMarkers.Num());
}

void UMapSystem::SwitchMapMode(EMapMode NewMode)
{
    CurrentMode = NewMode;
    UE_LOG(LogShanHe, Log, TEXT("地图模式切换：%d"), (int32)NewMode);
}

void UMapSystem::AddMarker(FMapMarker Marker)
{
    AllMarkers.Add(Marker);
    UE_LOG(LogShanHe, Log, TEXT("添加地图标记：%s"), *Marker.Label.ToString());
}

void UMapSystem::RemoveMarker(FName MarkerID)
{
    for (int32 i = 0; i < AllMarkers.Num(); i++)
    {
        if (AllMarkers[i].MarkerID == MarkerID)
        {
            AllMarkers.RemoveAt(i);
            return;
        }
    }
}

void UMapSystem::DiscoverLocation(FName LocationID)
{
    DiscoveredLocations.Add(LocationID);
    for (FMapMarker& M : AllMarkers)
    {
        if (M.MarkerID == LocationID)
        {
            M.bIsDiscovered = true;
            UE_LOG(LogShanHe, Log, TEXT("发现新地点：%s"), *M.Label.ToString());
            return;
        }
    }
}

FNavigationPath UMapSystem::CalculatePath(FVector2D From, FVector2D To)
{
    FNavigationPath Path;
    Path.PathID = FName(*FString::Printf(TEXT("Path_%d"), FDateTime::Now().GetTicks()));
    Path.Waypoints = {From, To};
    Path.TotalDistance = (int32)FVector2D::Distance(From, To);
    Path.EstimatedDays = FMath::Max(1, Path.TotalDistance / 100); // 假设每天走100单位
    Path.DangerLevel = FMath::RandRange(0.1f, 0.5f);

    UE_LOG(LogShanHe, Log, TEXT("路径计算完成：距离 %d，预计 %d 天，危险度 %.0f%%"),
        Path.TotalDistance, Path.EstimatedDays, Path.DangerLevel * 100);
    return Path;
}

void UMapSystem::SetNavigationTarget(FName MarkerID)
{
    for (FMapMarker& M : AllMarkers)
    {
        M.bIsNavTarget = (M.MarkerID == MarkerID);
        if (M.MarkerID == MarkerID)
        {
            UE_LOG(LogShanHe, Log, TEXT("导航目标：%s"), *M.Label.ToString());
        }
    }
}

void UMapSystem::ClearNavigation()
{
    for (FMapMarker& M : AllMarkers)
        M.bIsNavTarget = false;
    CurrentPath = FNavigationPath();
    UE_LOG(LogShanHe, Log, TEXT("清除导航"));
}

TArray<FMapMarker> UMapSystem::GetVisibleMarkers() const
{
    TArray<FMapMarker> Result;
    for (const FMapMarker& M : AllMarkers)
    {
        if (M.bIsVisible && (!bShowFogOfWar || M.bIsDiscovered))
            Result.Add(M);
    }
    return Result;
}

TArray<FMapMarker> UMapSystem::GetMarkersByType(EMapIconType Type) const
{
    TArray<FMapMarker> Result;
    for (const FMapMarker& M : AllMarkers)
        if (M.IconType == Type) Result.Add(M);
    return Result;
}
