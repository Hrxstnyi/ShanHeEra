#include "Core/ShanHePlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

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

void AShanHePlayerController::OnMove(const FInputActionValue& Value) {}
void AShanHePlayerController::OnLook(const FInputActionValue& Value) {}
void AShanHePlayerController::OnInteract(const FInputActionValue& Value) {}
void AShanHePlayerController::OnOpenMenu(const FInputActionValue& Value) {}
void AShanHePlayerController::OnTimeScale(const FInputActionValue& Value) {}
