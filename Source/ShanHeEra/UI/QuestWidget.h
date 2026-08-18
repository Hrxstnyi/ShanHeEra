#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ShanHeQuestWidget.generated.h"

class UVerticalBox;
class UTextBlock;
class UButton;

USTRUCT(BlueprintType)
struct FQuestDisplay
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FName QuestID;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FText Title;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FText Description;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) bool bIsComplete = false;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 Progress = 0;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 Total = 1;
};

UCLASS()
class SHANHEERA_API UQuestWidget : public UUserWidget
{
    GENERATED_BODY()
public:
    UPROPERTY(meta = (BindWidget)) UVerticalBox* ActiveQuestList;
    UPROPERTY(meta = (BindWidget)) UVerticalBox* CompletedQuestList;
    UPROPERTY(meta = (BindWidget)) UTextBlock* QuestTitleText;
    UPROPERTY(meta = (BindWidget)) UTextBlock* QuestDescText;
    UPROPERTY(meta = (BindWidget)) UTextBlock* QuestProgressText;
    UPROPERTY(meta = (BindWidget)) UTextBlock* QuestRewardText;
    UPROPERTY(meta = (BindWidget)) UButton* TrackButton;
    UPROPERTY(meta = (BindWidget)) UButton* AbandonButton;

    UFUNCTION(BlueprintCallable, Category="山河纪元|UI")
    void RefreshQuests();

    UFUNCTION(BlueprintCallable, Category="山河纪元|UI")
    void SelectQuest(FName QuestID);

    UFUNCTION(BlueprintCallable, Category="山河纪元|UI")
    void TrackQuest();

    UFUNCTION(BlueprintCallable, Category="山河纪元|UI")
    void AbandonQuest();

protected:
    virtual void NativeConstruct() override;
    FName SelectedQuestID;
};
