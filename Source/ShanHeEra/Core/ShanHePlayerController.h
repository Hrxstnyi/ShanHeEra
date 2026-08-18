#pragma once
#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ShanHePlayerController.generated.h"

class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

UCLASS()
class SHANHEERA_API AShanHePlayerController : public APlayerController
{
    GENERATED_BODY()
public:
    AShanHePlayerController();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="山河纪元|输入")
    UInputMappingContext* DefaultMappingContext;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="山河纪元|输入")
    UInputAction* MoveAction;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="山河纪元|输入")
    UInputAction* LookAction;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="山河纪元|输入")
    UInputAction* InteractAction;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="山河纪元|输入")
    UInputAction* OpenMenuAction;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="山河纪元|输入")
    UInputAction* TimeScaleAction;

protected:
    virtual void SetupInputComponent() override;
    virtual void BeginPlay() override;

    void OnMove(const FInputActionValue& Value);
    void OnLook(const FInputActionValue& Value);
    void OnInteract(const FInputActionValue& Value);
    void OnOpenMenu(const FInputActionValue& Value);
    void OnTimeScale(const FInputActionValue& Value);
};
