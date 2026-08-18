#include "Core/CameraSystem.h"
#include "Core/ShanHeLog.h"

void UCameraSystem::SwitchCameraMode(ECameraMode NewMode)
{
    if (NewMode == CurrentMode) return;
    CurrentMode = NewMode;
    bIsTransitioning = true;
    UE_LOG(LogShanHe, Log, TEXT("相机模式切换：%d"), (int32)NewMode);
}

void UCameraSystem::SetCameraTarget(AActor* Target)
{
    UE_LOG(LogShanHe, Log, TEXT("相机目标设置"));
}

void UCameraSystem::Zoom(float Amount)
{
    switch (CurrentMode)
    {
        case ECameraMode::ThirdPerson:
            ThirdPersonDistance = FMath::Clamp(ThirdPersonDistance + Amount * 10, 150.0f, 600.0f);
            break;
        case ECameraMode::Strategy:
            StrategyHeight = FMath::Clamp(StrategyHeight + Amount * 50, 500.0f, 5000.0f);
            break;
        case ECameraMode::Battle:
            BattleDistance = FMath::Clamp(BattleDistance + Amount * 5, 100.0f, 400.0f);
            break;
        default: break;
    }
}

void UCameraSystem::Rotate(float YawInput, float PitchInput)
{
    // 相机旋转逻辑
}
