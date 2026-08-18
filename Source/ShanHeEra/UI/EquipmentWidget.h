#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EquipmentWidget.generated.h"

class UImage;
class UTextBlock;
class UButton;

UENUM(BlueprintType)
enum class EEquipSlot : uint8
{
    Head, Necklace, Body, Ring, Weapon, OffHand, Gloves, Belt, Legs, Boots
};

UCLASS()
class SHANHEERA_API UEquipmentWidget : public UUserWidget
{
    GENERATED_BODY()
public:
    UPROPERTY(meta = (BindWidget)) UImage* HeadSlot;
    UPROPERTY(meta = (BindWidget)) UImage* NecklaceSlot;
    UPROPERTY(meta = (BindWidget)) UImage* BodySlot;
    UPROPERTY(meta = (BindWidget)) UImage* RingSlot;
    UPROPERTY(meta = (BindWidget)) UImage* WeaponSlot;
    UPROPERTY(meta = (BindWidget)) UImage* OffHandSlot;
    UPROPERTY(meta = (BindWidget)) UImage* GlovesSlot;
    UPROPERTY(meta = (BindWidget)) UImage* BeltSlot;
    UPROPERTY(meta = (BindWidget)) UImage* LegsSlot;
    UPROPERTY(meta = (BindWidget)) UImage* BootsSlot;

    UPROPERTY(meta = (BindWidget)) UTextBlock* EquipNameText;
    UPROPERTY(meta = (BindWidget)) UTextBlock* EquipStatsText;
    UPROPERTY(meta = (BindWidget)) UButton* UnequipButton;

    UFUNCTION(BlueprintCallable, Category="山河纪元|UI")
    void RefreshEquipment();

    UFUNCTION(BlueprintCallable, Category="山河纪元|UI")
    void SelectEquipSlot(EEquipSlot InSlot);

    UFUNCTION(BlueprintCallable, Category="山河纪元|UI")
    void UnequipSelected();

protected:
    virtual void NativeConstruct() override;
    EEquipSlot SelectedSlot = EEquipSlot::Weapon;
};
