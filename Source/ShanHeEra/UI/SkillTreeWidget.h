#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SkillTreeWidget.generated.h"

class UCanvasPanel;
class UImage;
class UTextBlock;
class UButton;
class UVerticalBox;

UENUM(BlueprintType)
enum class ESkillTreeTab : uint8
{
    MartialArts UMETA(DisplayName="武功"),
    Internal    UMETA(DisplayName="内功"),
    Lightness   UMETA(DisplayName="轻功"),
    Hidden      UMETA(DisplayName="暗器"),
    Craft       UMETA(DisplayName="手艺"),
    Scholar     UMETA(DisplayName="文才")
};

USTRUCT(BlueprintType)
struct FSkillNode
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FName SkillID;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FText SkillName;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FVector2D Position; // 树状图位置
    UPROPERTY(EditAnywhere, BlueprintReadWrite) TArray<FName> Prerequisites; // 前置技能
    UPROPERTY(EditAnywhere, BlueprintReadWrite) bool bUnlocked = false;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 Level = 0;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 MaxLevel = 5;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FText Description;
};

/**
 * 技能树界面 - 武功/技能可视化树状图，学习路径
 */
UCLASS()
class SHANHEERA_API USkillTreeWidget : public UUserWidget
{
    GENERATED_BODY()
public:
    UPROPERTY(meta = (BindWidget)) UCanvasPanel* TreeCanvas;
    UPROPERTY(meta = (BindWidget)) UTextBlock* SkillNameText;
    UPROPERTY(meta = (BindWidget)) UTextBlock* SkillDescText;
    UPROPERTY(meta = (BindWidget)) UTextBlock* SkillLevelText;
    UPROPERTY(meta = (BindWidget)) UTextBlock* SkillPrereqText;
    UPROPERTY(meta = (BindWidget)) UButton* LearnButton;
    UPROPERTY(meta = (BindWidget)) UButton* UpgradeButton;
    UPROPERTY(meta = (BindWidget)) UButton* TabMartial;
    UPROPERTY(meta = (BindWidget)) UButton* TabInternal;
    UPROPERTY(meta = (BindWidget)) UButton* TabLightness;
    UPROPERTY(meta = (BindWidget)) UButton* TabHidden;
    UPROPERTY(meta = (BindWidget)) UButton* TabCraft;
    UPROPERTY(meta = (BindWidget)) UButton* TabScholar;
    UPROPERTY(meta = (BindWidget)) UTextBlock* SkillPointsText;

    UPROPERTY(EditAnywhere) float NodeSpacingX = 150.0f;
    UPROPERTY(EditAnywhere) float NodeSpacingY = 100.0f;

    UFUNCTION(BlueprintCallable, Category="山河纪元|UI")
    void RefreshSkillTree();

    UFUNCTION(BlueprintCallable, Category="山河纪元|UI")
    void SelectSkill(FName SkillID);

    UFUNCTION(BlueprintCallable, Category="山河纪元|UI")
    void LearnSelectedSkill();

    UFUNCTION(BlueprintCallable, Category="山河纪元|UI")
    void UpgradeSelectedSkill();

    UFUNCTION(BlueprintCallable, Category="山河纪元|UI")
    void SwitchTab(ESkillTreeTab NewTab);

    UFUNCTION(BlueprintCallable, Category="山河纪元|UI")
    bool CanLearnSkill(FName SkillID) const;

protected:
    virtual void NativeConstruct() override;
    ESkillTreeTab CurrentTab = ESkillTreeTab::MartialArts;
    FName SelectedSkillID;
    TMap<FName, FSkillNode> SkillNodes;
    int32 AvailableSkillPoints = 0;
};
