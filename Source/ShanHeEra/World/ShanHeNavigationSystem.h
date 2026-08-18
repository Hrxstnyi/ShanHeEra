#pragma once
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ShanHeNavigationSystem.generated.h"

USTRUCT(BlueprintType)
struct FSeaRoute
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FName RouteID;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FName FromPort;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FName ToPort;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 Distance = 0;      // 航行天数
    UPROPERTY(EditAnywhere, BlueprintReadWrite) float Danger = 0.3f;    // 危险度 0-1
    UPROPERTY(EditAnywhere, BlueprintReadWrite) float Profit = 0.3f;    // 利润率
    UPROPERTY(EditAnywhere, BlueprintReadWrite) bool bIsDiscovered = false;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) TArray<FName> RequiredTechs;
};

USTRUCT(BlueprintType)
struct FShipData
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FName ShipID;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FText ShipName;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 Capacity = 100;    // 载货量
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 Crew = 20;        // 船员
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 Speed = 5;        // 航行速度
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 Durability = 100; // 耐久
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FName ShipType;         // 福船/宝船/沙船
};

USTRUCT(BlueprintType)
struct FVoyage
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FName VoyageID;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FName ShipID;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FName RouteID;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 DaysRemaining = 0;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 CargoValue = 0;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) bool bIsInDanger = false;
};

/**
 * 航海海洋系统 - 造船、航海、海贸、海盗、海外探索
 * 关联郑和下西洋彩蛋和台湾暗喻线
 */
UCLASS()
class SHANHEERA_API UShanHeNavigationSystem : public UObject
{
    GENERATED_BODY()
public:
    UFUNCTION(BlueprintCallable, Category="山河纪元|航海")
    void InitializeNavigation();

    UFUNCTION(BlueprintCallable, Category="山河纪元|航海")
    void Simulate(int32 Days);

    UFUNCTION(BlueprintCallable, Category="山河纪元|航海")
    FShipData BuildShip(FName ShipType, FText ShipName);

    UFUNCTION(BlueprintCallable, Category="山河纪元|航海")
    bool StartVoyage(FName ShipID, FName RouteID, int32 CargoValue);

    UFUNCTION(BlueprintCallable, Category="山河纪元|航海")
    void DiscoverRoute(FName RouteID);

    UFUNCTION(BlueprintCallable, Category="山河纪元|航海")
    TArray<FSeaRoute> GetDiscoveredRoutes() const;

    UPROPERTY(BlueprintReadOnly, Category="山河纪元|航海")
    TArray<FSeaRoute> SeaRoutes;

    UPROPERTY(BlueprintReadOnly, Category="山河纪元|航海")
    TArray<FShipData> Ships;

    UPROPERTY(BlueprintReadOnly, Category="山河纪元|航海")
    TArray<FVoyage> ActiveVoyages;

    UPROPERTY(BlueprintReadOnly, Category="山河纪元|航海")
    int32 TotalVoyages = 0;

private:
    void UpdateVoyages(int32 Days);
    void CheckPirateAttacks();
    void CheckStorms();
};
