#pragma once
#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "ShanHeHUD.generated.h"

class UHUDWidget;

/**
 * 山河纪元 HUD - 负责游戏内界面的显示和管理
 */
UCLASS()
class SHANHEERA_API AShanHeHUD : public AHUD
{
    GENERATED_BODY()
public:
    AShanHeHUD();

    virtual void BeginPlay() override;

    /** 获取HUD Widget */
    UFUNCTION(BlueprintPure, Category="山河纪元|UI")
    UHUDWidget* GetHUDWidget() const { return HUDWidget; }

    /** 显示/隐藏HUD */
    UFUNCTION(BlueprintCallable, Category="山河纪元|UI")
    void SetHUDVisible(bool bVisible);

protected:
    UPROPERTY(EditDefaultsOnly, Category="山河纪元|UI")
    TSubclassOf<UHUDWidget> HUDWidgetClass;

    UPROPERTY()
    UHUDWidget* HUDWidget = nullptr;
};
