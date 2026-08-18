#pragma once
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ShanHeArmyUnit.generated.h"

UENUM(BlueprintType)
enum class EUnitType : uint8
{
    Infantry    UMETA(DisplayName="步兵"),
    Cavalry     UMETA(DisplayName="骑兵"),
    Archer      UMETA(DisplayName="弓兵"),
    Crossbowman UMETA(DisplayName="弩兵"),
    Spearman    UMETA(DisplayName="枪兵"),
    ShieldInfantry UMETA(DisplayName="盾兵"),
    HeavyInfantry UMETA(DisplayName="重步兵"),
    LightCavalry UMETA(DisplayName="轻骑兵"),
    HeavyCavalry UMETA(DisplayName="重骑兵"),
    Sailor      UMETA(DisplayName="水兵"),
    Canoneer    UMETA(DisplayName="炮兵")
};

USTRUCT(BlueprintType)
struct FUnitData
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FName UnitID;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FText UnitName;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) EUnitType Type;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 Count = 100;       // 兵力
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 MaxCount = 100;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 Attack = 30;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 Defense = 20;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 Morale = 80;       // 士气 0-100
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 Training = 50;     // 训练度 0-100
    UPROPERTY(EditAnywhere, BlueprintReadWrite) float MoveSpeed = 100.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) float ChargeBonus = 0.0f; // 冲锋加成
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 UpkeepCost = 1;    // 每日维护费(银两)
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FName CommanderID;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) bool bIsRouting = false; // 是否溃逃
};

/**
 * 兵种单位 - 战场战术层的基本单位
 * 每个Unit代表一支百人级部队，有独立的士气、训练、状态
 * 战斗时用MassEntity或简单的Group AI控制
 */
UCLASS()
class SHANHEERA_API UArmyUnit : public UObject
{
    GENERATED_BODY()
public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="山河纪元|兵种")
    FUnitData Data;

    UFUNCTION(BlueprintCallable, Category="山河纪元|兵种")
    float CalculatePower() const; // 综合战力

    UFUNCTION(BlueprintCallable, Category="山河纪元|兵种")
    void TakeCasualties(int32 Amount);

    UFUNCTION(BlueprintCallable, Category="山河纪元|兵种")
    void ChangeMorale(int32 Amount);

    UFUNCTION(BlueprintCallable, Category="山河纪元|兵种")
    bool CheckRouting() const; // 检查是否溃逃

    UFUNCTION(BlueprintCallable, Category="山河纪元|兵种")
    void Rally(); // 重整旗鼓
};
