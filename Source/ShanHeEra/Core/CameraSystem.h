#pragma once
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ShanHeCameraSystem.generated.h"

UENUM(BlueprintType)
enum class ECameraMode : uint8
{
    ThirdPerson  UMETA(DisplayName="第三人称"),   // 人生模式
    Strategy     UMETA(DisplayName="战略俯瞰"),   // 大地图模式
    Battle       UMETA(DisplayName="战斗动作"),   // 战斗模式
    Cinematic    UMETA(DisplayName="过场动画"),   // CG模式
    Free         UMETA(DisplayName="自由相机")    // 调试/观察
};

/**
 * 相机系统 - 三层模式无缝切换
 * 战略层俯瞰/人生层第三人称/战斗层动作相机
 */
UCLASS()
class SHANHEERA_API UCameraSystem : public UObject
{
    GENERATED_BODY()
public:
    UFUNCTION(BlueprintCallable, Category="山河纪元|相机")
    void SwitchCameraMode(ECameraMode NewMode);

    UFUNCTION(BlueprintCallable, Category="山河纪元|相机")
    ECameraMode GetCurrentMode() const { return CurrentMode; }

    UFUNCTION(BlueprintCallable, Category="山河纪元|相机")
    void SetCameraTarget(AActor* Target);

    UFUNCTION(BlueprintCallable, Category="山河纪元|相机")
    void Zoom(float Amount);

    UFUNCTION(BlueprintCallable, Category="山河纪元|相机")
    void Rotate(float YawInput, float PitchInput);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="山河纪元|相机")
    float ThirdPersonDistance = 300.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="山河纪元|相机")
    float StrategyHeight = 2000.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="山河纪元|相机")
    float BattleDistance = 200.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="山河纪元|相机")
    float TransitionSpeed = 2.0f;

    UPROPERTY(BlueprintReadOnly, Category="山河纪元|相机")
    ECameraMode CurrentMode = ECameraMode::ThirdPerson;

    UPROPERTY(BlueprintReadOnly, Category="山河纪元|相机")
    bool bIsTransitioning = false;
};
