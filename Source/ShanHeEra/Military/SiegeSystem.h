#pragma once
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ShanHeSiegeSystem.generated.h"

UENUM(BlueprintType)
enum class ESiegePhase : uint8
{
    Blockade    UMETA(DisplayName="封锁"),
    Bombardment UMETA(DisplayName="炮击"),
    Assault     UMETA(DisplayName="强攻"),
    Breach      UMETA(DisplayName="破城"),
    StreetFight UMETA(DisplayName="巷战"),
    Surrender   UMETA(DisplayName="投降"),
    Lifted      UMETA(DisplayName="解围")
};

USTRUCT(BlueprintType)
struct FSiegeData
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FName CityID;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) ESiegePhase Phase;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 BesiegerStrength = 0;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 DefenderStrength = 0;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) float WallIntegrity = 100.0f; // 城墙完整度
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 CityFood = 100;         // 城中存粮(天)
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 CityMorale = 80;        // 城中士气
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 SiegeDays = 0;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) bool bHasSiegeWeapons = false;
};

/**
 * 攻城系统 - 城池攻防战
 * 支持：封锁围城、炮击、强攻、破城、巷战、投降
 * 攻城方需要考虑粮草、攻城武器、士气
 * 守城方需要考虑城墙、存粮、援军
 */
UCLASS()
class SHANHEERA_API USiegeSystem : public UObject
{
    GENERATED_BODY()
public:
    UFUNCTION(BlueprintCallable, Category="山河纪元|攻城")
    void StartSiege(FName CityID, int32 BesiegerStrength);

    UFUNCTION(BlueprintCallable, Category="山河纪元|攻城")
    void SimulateSiege(int32 Days);

    UFUNCTION(BlueprintCallable, Category="山河纪元|攻城")
    void OrderAssault(); // 强攻

    UFUNCTION(BlueprintCallable, Category="山河纪元|攻城")
    void OrderBombardment(); // 炮击

    UFUNCTION(BlueprintCallable, Category="山河纪元|攻城")
    void LiftSiege(); // 解围

    UFUNCTION(BlueprintCallable, Category="山河纪元|攻城")
    bool CheckSurrender() const;

    UPROPERTY(BlueprintReadOnly, Category="山河纪元|攻城")
    FSiegeData CurrentSiege;

    UPROPERTY(BlueprintReadOnly, Category="山河纪元|攻城")
    bool bIsSiegeActive = false;
};
