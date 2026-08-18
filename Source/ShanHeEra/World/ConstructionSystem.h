#pragma once
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ConstructionSystem.generated.h"

UENUM(BlueprintType)
enum class EBuildingType : uint8
{
    House       UMETA(DisplayName="民居"),
    Market      UMETA(DisplayName="市集"),
    Temple      UMETA(DisplayName="寺庙"),
    School      UMETA(DisplayName="书院"),
    Barracks    UMETA(DisplayName="兵营"),
    Wall        UMETA(DisplayName="城墙"),
    Granary     UMETA(DisplayName="粮仓"),
    Palace      UMETA(DisplayName="宫殿"),
    Canal       UMETA(DisplayName="运河"),
    Bridge      UMETA(DisplayName="桥梁"),
    Port        UMETA(DisplayName="港口")
};

USTRUCT(BlueprintType)
struct FBuildingData
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FName BuildingID;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FText BuildingName;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) EBuildingType Type;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FName CityID;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 ConstructionDays = 30;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 DaysRemaining = 0;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 SilverCost = 1000;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 WorkersRequired = 50;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) bool bIsComplete = false;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) TMap<FName, float> CityEffects; // 对城市的影响
};

USTRUCT(BlueprintType)
struct FConstructionProject
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FName ProjectID;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FBuildingData Building;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 WorkersAssigned = 0;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 SilverSpent = 0;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) float Progress = 0.0f;
};

/**
 * 建筑工程系统 - 城市建设、水利、城墙、宫殿建造
 * 大工程触发历史CG（如大运河、长城）
 */
UCLASS()
class SHANHEERA_API UConstructionSystem : public UObject
{
    GENERATED_BODY()
public:
    UFUNCTION(BlueprintCallable, Category="山河纪元|建筑")
    void StartConstruction(FName BuildingType, FName CityID, int32 Workers);

    UFUNCTION(BlueprintCallable, Category="山河纪元|建筑")
    void SimulateConstruction(int32 Days);

    UFUNCTION(BlueprintCallable, Category="山河纪元|建筑")
    TArray<FConstructionProject> GetActiveProjects(FName CityID) const;

    UFUNCTION(BlueprintCallable, Category="山河纪元|建筑")
    TArray<FBuildingData> GetCompletedBuildings(FName CityID) const;

    UPROPERTY(BlueprintReadOnly, Category="山河纪元|建筑")
    TArray<FConstructionProject> ActiveProjects;

    UPROPERTY(BlueprintReadOnly, Category="山河纪元|建筑")
    TArray<FBuildingData> CompletedBuildings;
};
