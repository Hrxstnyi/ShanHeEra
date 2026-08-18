#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ShanHeAttributePanelWidget.generated.h"

class UTextBlock;
class UProgressBar;
class UImage;

UCLASS()
class SHANHEERA_API UAttributePanelWidget : public UUserWidget
{
    GENERATED_BODY()
public:
    // 基础信息
    UPROPERTY(meta = (BindWidget)) UTextBlock* NameText;
    UPROPERTY(meta = (BindWidget)) UTextBlock* AgeText;
    UPROPERTY(meta = (BindWidget)) UTextBlock* ClassText;
    UPROPERTY(meta = (BindWidget)) UTextBlock* TitleText;

    // 六维属性
    UPROPERTY(meta = (BindWidget)) UTextBlock* StrengthText;
    UPROPERTY(meta = (BindWidget)) UTextBlock* IntelligenceText;
    UPROPERTY(meta = (BindWidget)) UTextBlock* CourageText;
    UPROPERTY(meta = (BindWidget)) UTextBlock* VitalityText;
    UPROPERTY(meta = (BindWidget)) UTextBlock* AgilityText;
    UPROPERTY(meta = (BindWidget)) UTextBlock* LuckText;

    // 状态条
    UPROPERTY(meta = (BindWidget)) UProgressBar* HealthBar;
    UPROPERTY(meta = (BindWidget)) UProgressBar* StaminaBar;
    UPROPERTY(meta = (BindWidget)) UProgressBar* SatietyBar;
    UPROPERTY(meta = (BindWidget)) UProgressBar* MoodBar;

    // 技能列表
    UPROPERTY(meta = (BindWidget)) UTextBlock* SkillsText;

    UFUNCTION(BlueprintCallable, Category="山河纪元|UI")
    void RefreshAttributes();

protected:
    virtual void NativeConstruct() override;
};
