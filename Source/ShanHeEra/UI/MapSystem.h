#pragma once
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MapSystem.generated.h"

UENUM(BlueprintType)
enum class EMapMode : uint8
{
    Mini        UMETA(DisplayName="小地图"),
    Full        UMETA(DisplayName="大地图"),
    World       UMETA(DisplayName="世界地图"),
    Region      UMETA(DisplayName="区域地图")
};

UENUM(BlueprintType)
enum class EMapIconType : uint8
{
    Player      UMETA(DisplayName="玩家"),
    City        UMETA(DisplayName="城市"),
    Village     UMETA(DisplayName="村庄"),
    Temple      UMETA(DisplayName="寺庙"),
    Market      UMETA(DisplayName="市集"),
    Danger      UMETA(DisplayName="危险"),
    Quest       UMETA(DisplayName="任务"),
    NPC         UMETA(DisplayName="NPC"),
    Custom      UMETA(DisplayName="自定义")
};

USTRUCT(BlueprintType)
struct FMapMarker
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FName MarkerID;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) EMapIconType IconType;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FText Label;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FVector2D WorldPosition; // 世界坐标
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FName RegionID;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) bool bIsDiscovered = false;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) bool bIsVisible = true;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) bool bIsNavTarget = false;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FLinearColor IconColor = FLinearColor::White;
};

USTRUCT(BlueprintType)
struct FShanHeMapPath
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FName PathID;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) TArray<FVector2D> Waypoints;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 TotalDistance = 0;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 EstimatedDays = 0;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) float DangerLevel = 0.0f; // 0-1
    UPROPERTY(EditAnywhere, BlueprintReadWrite) TArray<FName> PassingCities;
};

/**
 * 小地图系统 - 大地图/小地图切换、地点标记、导航路径
 * 支持迷雾战争（未探索区域不可见）
 */
UCLASS()
class SHANHEERA_API UMapSystem : public UObject
{
    GENERATED_BODY()
public:
    UFUNCTION(BlueprintCallable, Category="山河纪元|地图")
    void InitializeMap();

    UFUNCTION(BlueprintCallable, Category="山河纪元|地图")
    void SwitchMapMode(EMapMode NewMode);

    UFUNCTION(BlueprintCallable, Category="山河纪元|地图")
    void AddMarker(FMapMarker Marker);

    UFUNCTION(BlueprintCallable, Category="山河纪元|地图")
    void RemoveMarker(FName MarkerID);

    UFUNCTION(BlueprintCallable, Category="山河纪元|地图")
    void DiscoverLocation(FName LocationID);

    UFUNCTION(BlueprintCallable, Category="山河纪元|地图")
    FShanHeMapPath CalculatePath(FVector2D From, FVector2D To);

    UFUNCTION(BlueprintCallable, Category="山河纪元|地图")
    void SetNavigationTarget(FName MarkerID);

    UFUNCTION(BlueprintCallable, Category="山河纪元|地图")
    void ClearNavigation();

    UFUNCTION(BlueprintCallable, Category="山河纪元|地图")
    TArray<FMapMarker> GetVisibleMarkers() const;

    UFUNCTION(BlueprintCallable, Category="山河纪元|地图")
    TArray<FMapMarker> GetMarkersByType(EMapIconType Type) const;

    UPROPERTY(BlueprintReadOnly, Category="山河纪元|地图")
    EMapMode CurrentMode = EMapMode::Mini;

    UPROPERTY(BlueprintReadOnly, Category="山河纪元|地图")
    TArray<FMapMarker> AllMarkers;

    UPROPERTY(BlueprintReadOnly, Category="山河纪元|地图")
    FShanHeMapPath CurrentPath;

    UPROPERTY(BlueprintReadOnly, Category="山河纪元|地图")
    TSet<FName> DiscoveredLocations;

    UPROPERTY(EditAnywhere, Category="山河纪元|地图")
    float MiniMapSize = 200.0f;

    UPROPERTY(EditAnywhere, Category="山河纪元|地图")
    float MiniMapRange = 5000.0f; // 小地图显示范围

    UPROPERTY(EditAnywhere, Category="山河纪元|地图")
    bool bShowFogOfWar = true;
};
