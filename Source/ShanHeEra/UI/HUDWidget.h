#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ShanHeHUDWidget.generated.h"

class UTextBlock;
class UProgressBar;
class UImage;

UCLASS()
class SHANHEERA_API UHUDWidget : public UUserWidget
{
    GENERATED_BODY()
public:
    UPROPERTY(meta = (BindWidget)) UTextBlock* DateText;
    UPROPERTY(meta = (BindWidget)) UTextBlock* LocationText;
    UPROPERTY(meta = (BindWidget)) UTextBlock* GoldText;
    UPROPERTY(meta = (BindWidget)) UTextBlock* ReputationText;
    UPROPERTY(meta = (BindWidget)) UProgressBar* TimeScaleBar;
    UPROPERTY(meta = (BindWidget)) UTextBlock* EventNotification;

    UFUNCTION(BlueprintCallable, Category="山河纪元|HUD")
    void UpdateDate(const FString& DateStr);
    UFUNCTION(BlueprintCallable, Category="山河纪元|HUD")
    void ShowNotification(const FString& Message, float Duration = 5.0f);
};
