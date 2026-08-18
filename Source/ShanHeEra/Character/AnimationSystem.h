#pragma once
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AnimationSystem.generated.h"

UENUM(BlueprintType)
enum class ECharacterAction : uint8
{
    Idle        UMETA(DisplayName="待机"),
    Walk        UMETA(DisplayName="行走"),
    Run         UMETA(DisplayName="奔跑"),
    Jump        UMETA(DisplayName="跳跃"),
    Attack      UMETA(DisplayName="攻击"),
    Block       UMETA(DisplayName="格挡"),
    Hit         UMETA(DisplayName="受击"),
    Death       UMETA(DisplayName="死亡"),
    Interact    UMETA(DisplayName="交互"),
    Sit         UMETA(DisplayName="坐下"),
    Sleep       UMETA(DisplayName="睡觉"),
    Craft       UMETA(DisplayName="制作")
};

USTRUCT(BlueprintType)
struct FShanHeAnimationState
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite) ECharacterAction CurrentAction = ECharacterAction::Idle;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) float Speed = 0.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) float Direction = 0.0f; // -180~180
    UPROPERTY(EditAnywhere, BlueprintReadWrite) bool bIsInAir = false;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) bool bIsCrouching = false;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) float HealthPercent = 1.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) float StaminaPercent = 1.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 ComboCount = 0;
};

/**
 * 动画系统 - 角色动画蓝图、走跑跳/攻击/受击状态机、面部动画
 */
UCLASS()
class SHANHEERA_API UAnimationSystem : public UObject
{
    GENERATED_BODY()
public:
    UFUNCTION(BlueprintCallable, Category="山河纪元|动画")
    void SetAction(ECharacterAction NewAction);

    UFUNCTION(BlueprintCallable, Category="山河纪元|动画")
    void UpdateLocomotion(float Speed, float Direction);

    UFUNCTION(BlueprintCallable, Category="山河纪元|动画")
    void PlayAttackMontage(int32 ComboIndex);

    UFUNCTION(BlueprintCallable, Category="山河纪元|动画")
    void PlayHitReact();

    UFUNCTION(BlueprintCallable, Category="山河纪元|动画")
    void PlayDeath();

    UFUNCTION(BlueprintCallable, Category="山河纪元|动画")
    void SetFacialExpression(FName Expression); // 喜/怒/哀/惧/惊/思

    UFUNCTION(BlueprintCallable, Category="山河纪元|动画")
    FShanHeAnimationState GetState() const { return State; }

    UPROPERTY(BlueprintReadOnly, Category="山河纪元|动画")
    FShanHeAnimationState State;

    UPROPERTY(EditAnywhere, Category="山河纪元|动画")
    TMap<FName, float> FacialBlendshapes; // 面部混合形变

private:
    ECharacterAction LastAction = ECharacterAction::Idle;
};
