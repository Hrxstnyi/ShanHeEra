#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Core/ShanHeStructs.h"
#include "Core/ShanHeEnums.h"
#include "ShanHeCharacter.generated.h"

UCLASS()
class SHANHEERA_API AShanHeCharacter : public ACharacter
{
    GENERATED_BODY()
public:
    AShanHeCharacter();

    virtual void BeginPlay() override;

    // === 基本信息 ===
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="山河纪元|角色|信息")
    FPersonName CharName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="山河纪元|角色|信息")
    FCharacterStats Stats;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="山河纪元|角色|信息")
    ESocialClass SocialClass = ESocialClass::Farmer;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="山河纪元|角色|信息")
    TArray<ECharacterTrait> Traits;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="山河纪元|角色|信息")
    int32 Age = 30;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="山河纪元|角色|信息")
    bool bIsAlive = true;

    // === 生命值与体力 ===
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="山河纪元|角色|状态")
    float Health = 100.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="山河纪元|角色|状态")
    float MaxHealth = 100.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="山河纪元|角色|状态")
    float Stamina = 100.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="山河纪元|角色|状态")
    float MaxStamina = 100.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="山河纪元|角色|状态")
    float Hunger = 100.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="山河纪元|角色|状态")
    float Money = 0.0f;

    // === 方法 ===
    UFUNCTION(BlueprintCallable, Category="山河纪元|角色")
    bool HasTrait(ECharacterTrait Trait) const { return Traits.Contains(Trait); }

    UFUNCTION(BlueprintPure, Category="山河纪元|角色")
    FString GetFullName() const { return CharName.Surname + CharName.GivenName; }

    UFUNCTION(BlueprintCallable, Category="山河纪元|角色|状态")
    void ApplyDamage(float Damage);

    UFUNCTION(BlueprintCallable, Category="山河纪元|角色|状态")
    void Heal(float Amount);

    UFUNCTION(BlueprintCallable, Category="山河纪元|角色|状态")
    void ConsumeStamina(float Amount);

    UFUNCTION(BlueprintCallable, Category="山河纪元|角色|状态")
    void RestoreStamina(float Amount);

    UFUNCTION(BlueprintCallable, Category="山河纪元|角色|状态")
    bool IsAlive() const { return bIsAlive && Health > 0.0f; }

    UFUNCTION(BlueprintCallable, Category="山河纪元|角色|交互")
    void Interact();

protected:
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
