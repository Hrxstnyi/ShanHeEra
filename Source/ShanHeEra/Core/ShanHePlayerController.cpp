#include "Core/ShanHePlayerController.h"
#include "Core/ShanHeCharacter.h"
#include "Core/ShanHeLog.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

AShanHePlayerController::AShanHePlayerController()
{
    bShowMouseCursor = true;
    bEnableClickEvents = true;
    bEnableMouseOverEvents = true;
}

void AShanHePlayerController::BeginPlay()
{
    Super::BeginPlay();
    if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
        ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
    {
        if (DefaultMappingContext)
            Subsystem->AddMappingContext(DefaultMappingContext, 0);
    }
    UE_LOG(LogShanHe, Log, TEXT("玩家控制器初始化完成"));
}

void AShanHePlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();
    if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(InputComponent))
    {
        if (MoveAction) EIC->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AShanHePlayerController::OnMove);
        if (LookAction) EIC->BindAction(LookAction, ETriggerEvent::Triggered, this, &AShanHePlayerController::OnLook);
        if (InteractAction) EIC->BindAction(InteractAction, ETriggerEvent::Started, this, &AShanHePlayerController::OnInteract);
        if (OpenMenuAction) EIC->BindAction(OpenMenuAction, ETriggerEvent::Started, this, &AShanHePlayerController::OnOpenMenu);
        if (TimeScaleAction) EIC->BindAction(TimeScaleAction, ETriggerEvent::Started, this, &AShanHePlayerController::OnTimeScale);
    }
}

void AShanHePlayerController::OnMove(const FInputActionValue& Value)
{
    FVector2D MovementVector = Value.Get<FVector2D>();
    if (AShanHeCharacter* PlayerChar = Cast<AShanHeCharacter>(GetPawn()))
    {
        const FRotator Rotation = GetControlRotation();
        const FRotator YawRotation(0, Rotation.Yaw, 0);
        const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
        const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
        PlayerChar->AddMovementInput(ForwardDirection, MovementVector.Y);
        PlayerChar->AddMovementInput(RightDirection, MovementVector.X);
    }
}

void AShanHePlayerController::OnLook(const FInputActionValue& Value)
{
    FVector2D LookAxisVector = Value.Get<FVector2D>();
    AddYawInput(LookAxisVector.X);
    AddPitchInput(LookAxisVector.Y);
}

void AShanHePlayerController::OnInteract(const FInputActionValue& Value)
{
    UE_LOG(LogShanHe, Log, TEXT("玩家交互"));
    // 交互逻辑：射线检测前方可交互物体
    FHitResult HitResult;
    FVector Start = PlayerCameraManager->GetCameraLocation();
    FVector End = Start + PlayerCameraManager->GetCameraRotation().Vector() * 300.0f;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(GetPawn());
    if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, Params))
    {
        UE_LOG(LogShanHe, Log, TEXT("交互目标: %s"), *HitResult.GetActor()->GetName());
    }
}

void AShanHePlayerController::OnOpenMenu(const FInputActionValue& Value)
{
    UE_LOG(LogShanHe, Log, TEXT("打开菜单"));
    // 菜单切换逻辑待UI完善后实现
}

void AShanHePlayerController::OnTimeScale(const FInputActionValue& Value)
{
    UE_LOG(LogShanHe, Log, TEXT("调整时间流速"));
    // 时间缩放逻辑待GameState完善后实现
}
