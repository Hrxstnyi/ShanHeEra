#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WorldMapWidget.generated.h"

class UImage;
class UTextBlock;
class UButton;
class UCanvasPanel;

UCLASS()
class SHANHEERA_API UWorldMapWidget : public UUserWidget
{
    GENERATED_BODY()
public:
    UPROPERTY(meta = (BindWidget)) UCanvasPanel* MapCanvas;
    UPROPERTY(meta = (BindWidget)) UImage* MapBackground;
    UPROPERTY(meta = (BindWidget)) UImage* PlayerMarker;
    UPROPERTY(meta = (BindWidget)) UTextBlock* LocationNameText;
    UPROPERTY(meta = (BindWidget)) UTextBlock* LocationInfoText;
    UPROPERTY(meta = (BindWidget)) UButton* TravelButton;
    UPROPERTY(meta = (BindWidget)) UButton* ZoomInButton;
    UPROPERTY(meta = (BindWidget)) UButton* ZoomOutButton;
    UPROPERTY(meta = (BindWidget)) UButton* FilterButton;

    UPROPERTY(EditAnywhere) float ZoomLevel = 1.0f;
    UPROPERTY(EditAnywhere) float MinZoom = 0.5f;
    UPROPERTY(EditAnywhere) float MaxZoom = 3.0f;

    UFUNCTION(BlueprintCallable, Category="山河纪元|UI")
    void RefreshMap();

    UFUNCTION(BlueprintCallable, Category="山河纪元|UI")
    void SelectLocation(FName LocationID);

    UFUNCTION(BlueprintCallable, Category="山河纪元|UI")
    void TravelToSelected();

    UFUNCTION(BlueprintCallable, Category="山河纪元|UI")
    void ZoomIn();

    UFUNCTION(BlueprintCallable, Category="山河纪元|UI")
    void ZoomOut();

    UFUNCTION(BlueprintCallable, Category="山河纪元|UI")
    void ToggleFilter();

protected:
    virtual void NativeConstruct() override;
    FName SelectedLocation;
    bool bShowCities = true;
    bool bShowVillages = true;
    bool bShowDangers = false;
};
