#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ShanHeInventoryWidget.generated.h"

class UUniformGridPanel;
class UTextBlock;
class UImage;
class UButton;
class UInventorySlotWidget;

UCLASS()
class SHANHEERA_API UInventoryWidget : public UUserWidget
{
    GENERATED_BODY()
public:
    UPROPERTY(meta = (BindWidget)) UUniformGridPanel* ItemGrid;
    UPROPERTY(meta = (BindWidget)) UTextBlock* GoldText;
    UPROPERTY(meta = (BindWidget)) UTextBlock* WeightText;
    UPROPERTY(meta = (BindWidget)) UTextBlock* ItemNameText;
    UPROPERTY(meta = (BindWidget)) UTextBlock* ItemDescText;
    UPROPERTY(meta = (BindWidget)) UButton* UseButton;
    UPROPERTY(meta = (BindWidget)) UButton* DropButton;
    UPROPERTY(meta = (BindWidget)) UButton* SortButton;

    UPROPERTY(EditAnywhere) TSubclassOf<UInventorySlotWidget> SlotWidgetClass;
    UPROPERTY(EditAnywhere) int32 Columns = 8;
    UPROPERTY(EditAnywhere) int32 Rows = 6;

    UFUNCTION(BlueprintCallable, Category="山河纪元|UI")
    void RefreshInventory();

    UFUNCTION(BlueprintCallable, Category="山河纪元|UI")
    void SelectItem(int32 SlotIndex);

    UFUNCTION(BlueprintCallable, Category="山河纪元|UI")
    void UseSelectedItem();

    UFUNCTION(BlueprintCallable, Category="山河纪元|UI")
    void DropSelectedItem();

    UFUNCTION(BlueprintCallable, Category="山河纪元|UI")
    void SortItems();

protected:
    virtual void NativeConstruct() override;
    int32 SelectedSlot = -1;
};
