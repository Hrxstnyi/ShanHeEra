#include "Core/ShanHeCharacter.h"
#include "Core/ShanHeLog.h"

AShanHeCharacter::AShanHeCharacter()
{
    PrimaryActorTick.bCanEverTick = true;
    Health = MaxHealth;
    Stamina = MaxStamina;
}

void AShanHeCharacter::BeginPlay()
{
    Super::BeginPlay();
    UE_LOG(LogShanHe, Log, TEXT("角色 %s 进入世界 - 身份: %s"), *GetFullName(),
        *UEnum::GetValueAsString(SocialClass));
}

void AShanHeCharacter::ApplyDamage(float Damage)
{
    if (!bIsAlive) return;
    Health = FMath::Clamp(Health - Damage, 0.0f, MaxHealth);
    UE_LOG(LogShanHe, Log, TEXT("%s 受到 %.1f 点伤害，剩余生命: %.1f"), *GetFullName(), Damage, Health);
    if (Health <= 0.0f)
    {
        bIsAlive = false;
        UE_LOG(LogShanHe, Warning, TEXT("%s 已死亡"), *GetFullName());
    }
}

void AShanHeCharacter::Heal(float Amount)
{
    if (!bIsAlive) return;
    Health = FMath::Clamp(Health + Amount, 0.0f, MaxHealth);
}

void AShanHeCharacter::ConsumeStamina(float Amount)
{
    Stamina = FMath::Clamp(Stamina - Amount, 0.0f, MaxStamina);
}

void AShanHeCharacter::RestoreStamina(float Amount)
{
    Stamina = FMath::Clamp(Stamina + Amount, 0.0f, MaxStamina);
}

void AShanHeCharacter::Interact()
{
    UE_LOG(LogShanHe, Log, TEXT("%s 尝试交互"), *GetFullName());
    // 射线检测前方可交互物体
    FHitResult HitResult;
    FVector Start = GetActorLocation() + FVector(0, 0, 60.0f);
    FVector End = Start + GetActorForwardVector() * 200.0f;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(this);
    if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, Params))
    {
        UE_LOG(LogShanHe, Log, TEXT("交互目标: %s"), *HitResult.GetActor()->GetName());
    }
}

void AShanHeCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
}
