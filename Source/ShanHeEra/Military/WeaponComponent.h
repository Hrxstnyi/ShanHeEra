#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WeaponComponent.generated.h"

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
    Unarmed     UMETA(DisplayName="徒手"),
    Sword       UMETA(DisplayName="剑"),
    Saber       UMETA(DisplayName="刀"),
    Spear       UMETA(DisplayName="枪"),
    Staff       UMETA(DisplayName="棍"),
    Bow         UMETA(DisplayName="弓"),
    Crossbow    UMETA(DisplayName="弩"),
    Polearm     UMETA(DisplayName="长柄"),
    Shield      UMETA(DisplayName="盾"),
    Firearm     UMETA(DisplayName="火器")
};

USTRUCT(BlueprintType)
struct FWeaponData
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FName WeaponID;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FText WeaponName;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) EWeaponType Type;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 Damage = 10;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) float AttackSpeed = 1.0f; // 攻击/秒
    UPROPERTY(EditAnywhere, BlueprintReadWrite) float Range = 150.0f;    // 攻击范围(cm)
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 ArmorPenetration = 0;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) float CritChance = 0.05f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) float CritMultiplier = 1.5f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 Durability = 100;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 MaxDurability = 100;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) bool bIsRanged = false;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 RequiredStrength = 0;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) TArray<FName> SpecialAbilities;
};

/**
 * 武器组件 - 挂载在角色上，管理武器装备和战斗计算
 * 支持骑砍式的实时战斗：方向攻击、格挡、闪避、暴击
 */
UCLASS(ClassGroup=(Combat), meta=(BlueprintSpawnableComponent))
class SHANHEERA_API UWeaponComponent : public UActorComponent
{
    GENERATED_BODY()
public:
    UWeaponComponent();

    UFUNCTION(BlueprintCallable, Category="山河纪元|战斗")
    void EquipWeapon(const FWeaponData& NewWeapon);

    UFUNCTION(BlueprintCallable, Category="山河纪元|战斗")
    void UnequipWeapon();

    UFUNCTION(BlueprintCallable, Category="山河纪元|战斗")
    float CalculateDamage(AActor* Target, float AttackDirection); // 方向: 0=左,1=右,2=上,3=下

    UFUNCTION(BlueprintCallable, Category="山河纪元|战斗")
    bool CanBlock(float AttackDirection) const;

    UFUNCTION(BlueprintCallable, Category="山河纪元|战斗")
    void OnAttackHit(AActor* Target, float Damage);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="山河纪元|战斗")
    FWeaponData CurrentWeapon;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="山河纪元|战斗")
    float BaseDamage = 5.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="山河纪元|战斗")
    float BlockAngle = 90.0f; // 格挡角度(度)

    UPROPERTY(BlueprintReadOnly, Category="山河纪元|战斗")
    bool bIsAttacking = false;

    UPROPERTY(BlueprintReadOnly, Category="山河纪元|战斗")
    bool bIsBlocking = false;

    UPROPERTY(BlueprintReadOnly, Category="山河纪元|战斗")
    float CurrentAttackDirection = 0.0f;

private:
    float GetDirectionalBonus(float AttackDirection, float TargetFacing) const;
};
