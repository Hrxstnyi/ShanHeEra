#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CodexWidget.generated.h"

class UVerticalBox;
class UGridPanel;
class UTextBlock;
class UButton;
class UImage;
class UProgressBar;

UENUM(BlueprintType)
enum class ECodexTab : uint8
{
    Item        UMETA(DisplayName="物品"),
    Character   UMETA(DisplayName="人物"),
    Location    UMETA(DisplayName="地点"),
    MartialArt  UMETA(DisplayName="武功"),
    Beast       UMETA(DisplayName="异兽"),
    Plant       UMETA(DisplayName="草木"),
    History     UMETA(DisplayName="历史")
};

/**
 * 图鉴界面 - 7大类图鉴的浏览界面
 */
UCLASS()
class SHANHEERA_API UCodexWidget : public UUserWidget
{
    GENERATED_BODY()
public:
    UPROPERTY(meta = (BindWidget)) UGridPanel* EntryGrid;
    UPROPERTY(meta = (BindWidget)) UTextBlock* EntryNameText;
    UPROPERTY(meta = (BindWidget)) UTextBlock* EntryDescText;
    UPROPERTY(meta = (BindWidget)) UTextBlock* EntryRarityText;
    UPROPERTY(meta = (BindWidget)) UTextBlock* EntryDateText;
    UPROPERTY(meta = (BindWidget)) UImage* EntryImage;
    UPROPERTY(meta = (BindWidget)) UProgressBar* CategoryProgress;
    UPROPERTY(meta = (BindWidget)) UTextBlock* ProgressText;
    UPROPERTY(meta = (BindWidget)) UTextBlock* TotalProgressText;

    // 分类标签
    UPROPERTY(meta = (BindWidget)) UButton* TabItem;
    UPROPERTY(meta = (BindWidget)) UButton* TabCharacter;
    UPROPERTY(meta = (BindWidget)) UButton* TabLocation;
    UPROPERTY(meta = (BindWidget)) UButton* TabMartial;
    UPROPERTY(meta = (BindWidget)) UButton* TabBeast;
    UPROPERTY(meta = (BindWidget)) UButton* TabPlant;
    UPROPERTY(meta = (BindWidget)) UButton* TabHistory;

    UPROPERTY(EditAnywhere) int32 GridColumns = 5;

    UFUNCTION(BlueprintCallable, Category="山河纪元|UI")
    void RefreshCodex();

    UFUNCTION(BlueprintCallable, Category="山河纪元|UI")
    void SelectEntry(FName EntryID);

    UFUNCTION(BlueprintCallable, Category="山河纪元|UI")
    void SwitchCategory(ECodexTab NewTab);

    UFUNCTION(BlueprintCallable, Category="山河纪元|UI")
    int32 GetCategoryProgress() const;

    UFUNCTION(BlueprintCallable, Category="山河纪元|UI")
    int32 GetTotalProgress() const;

protected:
    virtual void NativeConstruct() override;
    ECodexTab CurrentCategory = ECodexTab::Item;
    FName SelectedEntryID;
};
