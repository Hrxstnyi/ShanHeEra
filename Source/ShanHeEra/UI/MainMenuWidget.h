#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Core/ShanHeEnums.h"
#include "ShanHeMainMenuWidget.generated.h"

class UButton;
class UTextBlock;
class UComboBoxString;

UCLASS()
class SHANHEERA_API UMainMenuWidget : public UUserWidget
{
    GENERATED_BODY()
protected:
    virtual bool Initialize() override;

    UPROPERTY(meta = (BindWidget)) UButton* NewGameButton;
    UPROPERTY(meta = (BindWidget)) UButton* ContinueButton;
    UPROPERTY(meta = (BindWidget)) UButton* SettingsButton;
    UPROPERTY(meta = (BindWidget)) UButton* ExitButton;
    UPROPERTY(meta = (BindWidget)) UComboBoxString* LanguageSelect;
    UPROPERTY(meta = (BindWidget)) UTextBlock* TitleText;
    UPROPERTY(meta = (BindWidget)) UTextBlock* SubtitleText;

    UFUNCTION() void OnNewGame();
    UFUNCTION() void OnContinue();
    UFUNCTION() void OnSettings();
    UFUNCTION() void OnExit();
    UFUNCTION() void OnLanguageChanged(FString SelectedItem, ESelectInfo::Type SelectionType);

public:
    UFUNCTION(BlueprintCallable, Category="山河纪元|UI")
    void SetLanguage(ELanguage NewLang);
};
