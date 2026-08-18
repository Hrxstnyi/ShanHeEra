#include "Military/WeaponComponent.h"
#include "GameFramework/Character.h"
#include "Core/ShanHeLog.h"

UWeaponComponent::UWeaponComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UWeaponComponent::EquipWeapon(const FWeaponData& NewWeapon)
{
    CurrentWeapon = NewWeapon;
    UE_LOG(LogShanHe, Log, TEXT("装备武器: %s"), *NewWeapon.WeaponName.ToString());
}

void UWeaponComponent::UnequipWeapon()
{
    CurrentWeapon = FWeaponData();
    bIsAttacking = false;
    bIsBlocking = false;
}

float UWeaponComponent::CalculateDamage(AActor* Target, float AttackDirection)
{
    if (!CurrentWeapon.WeaponID.IsValid()) return BaseDamage;

    float Damage = CurrentWeapon.Damage + BaseDamage;

    // 方向加成：不同方向有不同的伤害修正（骑砍式）
    // 左侧攻击对右侧防御弱的目标伤害更高
    ACharacter* OwnerChar = Cast<ACharacter>(GetOwner());
    if (OwnerChar && Target)
    {
        float TargetFacing = Target->GetActorRotation().Yaw;
        Damage *= GetDirectionalBonus(AttackDirection, TargetFacing);
    }

    // 暴击判定
    if (FMath::RandRange(0.0f, 1.0f) < CurrentWeapon.CritChance)
    {
        Damage *= CurrentWeapon.CritMultiplier;
        UE_LOG(LogShanHe, Log, TEXT("暴击！伤害: %.1f"), Damage);
    }

    // 武器耐久损耗
    CurrentWeapon.Durability = FMath::Max(0, CurrentWeapon.Durability - 1);

    return Damage;
}

bool UWeaponComponent::CanBlock(float AttackDirection) const
{
    if (!bIsBlocking) return false;
    // 简化：如果攻击方向在格挡角度内，则格挡成功
    // 实际项目中需要计算攻击者和防御者的相对角度
    return FMath::Abs(AttackDirection - 0.0f) < BlockAngle / 2.0f;
}

void UWeaponComponent::OnAttackHit(AActor* Target, float Damage)
{
    UE_LOG(LogShanHe, Log, TEXT("攻击命中 %s，伤害 %.1f"), *Target->GetName(), Damage);
    // 实际项目中调用Target的TakeDamage
}

float UWeaponComponent::GetDirectionalBonus(float AttackDirection, float TargetFacing) const
{
    // 方向攻击系统：4个方向，每个方向有不同的伤害加成
    // 0=左砍, 1=右砍, 2=上劈, 3=下刺
    float Bonuses[] = {1.1f, 1.0f, 1.2f, 0.9f};
    int32 Dir = FMath::Clamp((int32)AttackDirection, 0, 3);
    return Bonuses[Dir];
}
