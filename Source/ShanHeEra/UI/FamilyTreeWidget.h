#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ShanHeFamilyTreeWidget.generated.h"

class UCanvasPanel;
class UImage;
class UTextBlock;
class UButton;
class UScrollBox;

USTRUCT(BlueprintType)
struct FFamilyMemberNode
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FName MemberID;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FText Name;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 Generation = 1; // 第几代
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FVector2D Position;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) TArray<FName> Parents;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) TArray<FName> Spouses;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) TArray<FName> Children;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) bool bIsAlive = true;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FText Title; // 身份/官职
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 BirthYear = 0;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 DeathYear = 0;
};

/**
 * 家族树界面 - 多代家族关系可视化，传承脉络
 */
UCLASS()
class SHANHEERA_API UFamilyTreeWidget : public UUserWidget
{
    GENERATED_BODY()
public:
    UPROPERTY(meta = (BindWidget)) UScrollBox* TreeScroll;
    UPROPERTY(meta = (BindWidget)) UCanvasPanel* TreeCanvas;
    UPROPERTY(meta = (BindWidget)) UTextBlock* MemberNameText;
    UPROPERTY(meta = (BindWidget)) UTextBlock* MemberTitleText;
    UPROPERTY(meta = (BindWidget)) UTextBlock* MemberBioText;
    UPROPERTY(meta = (BindWidget)) UTextBlock* MemberRelationsText;
    UPROPERTY(meta = (BindWidget)) UTextBlock* GenerationText;
    UPROPERTY(meta = (BindWidget)) UButton* ZoomInButton;
    UPROPERTY(meta = (BindWidget)) UButton* ZoomOutButton;
    UPROPERTY(meta = (BindWidget)) UButton* CenterOnPlayerButton;

    UPROPERTY(EditAnywhere) float ZoomLevel = 1.0f;
    UPROPERTY(EditAnywhere) float MinZoom = 0.5f;
    UPROPERTY(EditAnywhere) float MaxZoom = 2.0f;

    UFUNCTION(BlueprintCallable, Category="山河纪元|UI")
    void RefreshFamilyTree();

    UFUNCTION(BlueprintCallable, Category="山河纪元|UI")
    void SelectMember(FName MemberID);

    UFUNCTION(BlueprintCallable, Category="山河纪元|UI")
    void ZoomIn();

    UFUNCTION(BlueprintCallable, Category="山河纪元|UI")
    void ZoomOut();

    UFUNCTION(BlueprintCallable, Category="山河纪元|UI")
    void CenterOnPlayer();

    UFUNCTION(BlueprintCallable, Category="山河纪元|UI")
    int32 GetGenerationCount() const;

    UFUNCTION(BlueprintCallable, Category="山河纪元|UI")
    int32 GetTotalMembers() const;

protected:
    virtual void NativeConstruct() override;
    TMap<FName, FFamilyMemberNode> FamilyMembers;
    FName SelectedMemberID;
};
