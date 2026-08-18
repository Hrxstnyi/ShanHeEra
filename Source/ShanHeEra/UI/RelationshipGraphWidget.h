#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RelationshipGraphWidget.generated.h"

class UCanvasPanel;
class UImage;
class UTextBlock;
class UButton;

UENUM(BlueprintType)
enum class ERelationLineType : uint8
{
    Family      UMETA(DisplayName="血缘"),
    Marriage    UMETA(DisplayName="婚姻"),
    Friendship  UMETA(DisplayName="友情"),
    Master      UMETA(DisplayName="师徒"),
    Sworn       UMETA(DisplayName="结义"),
    Enemy       UMETA(DisplayName="仇敌"),
    Romance     UMETA(DisplayName="暧昧"),
    Rival       UMETA(DisplayName="对手")
};

USTRUCT(BlueprintType)
struct FRelationNode
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FName NPCID;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FText Name;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FVector2D Position;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FLinearColor NodeColor;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 RelationValue = 0; // -100~100
};

USTRUCT(BlueprintType)
struct FRelationEdge
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FName FromID;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FName ToID;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) ERelationLineType Type;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 Strength = 50; // 关系强度
};

/**
 * 关系图谱界面 - NPC关系网，爱恨情仇一目了然
 */
UCLASS()
class SHANHEERA_API URelationshipGraphWidget : public UUserWidget
{
    GENERATED_BODY()
public:
    UPROPERTY(meta = (BindWidget)) UCanvasPanel* GraphCanvas;
    UPROPERTY(meta = (BindWidget)) UTextBlock* NPCNameText;
    UPROPERTY(meta = (BindWidget)) UTextBlock* NPCDescText;
    UPROPERTY(meta = (BindWidget)) UTextBlock* RelationValueText;
    UPROPERTY(meta = (BindWidget)) UTextBlock* RelationTypeText;
    UPROPERTY(meta = (BindWidget)) UButton* FilterFamily;
    UPROPERTY(meta = (BindWidget)) UButton* FilterFriend;
    UPROPERTY(meta = (BindWidget)) UButton* FilterEnemy;
    UPROPERTY(meta = (BindWidget)) UButton* FilterAll;
    UPROPERTY(meta = (BindWidget)) UButton* RefreshButton;

    UFUNCTION(BlueprintCallable, Category="山河纪元|UI")
    void RefreshGraph();

    UFUNCTION(BlueprintCallable, Category="山河纪元|UI")
    void SelectNPC(FName NPCID);

    UFUNCTION(BlueprintCallable, Category="山河纪元|UI")
    void SetFilter(ERelationLineType FilterType);

    UFUNCTION(BlueprintCallable, Category="山河纪元|UI")
    void ShowAllRelations();

    UFUNCTION(BlueprintCallable, Category="山河纪元|UI")
    TArray<FRelationEdge> GetRelationsOfNPC(FName NPCID) const;

protected:
    virtual void NativeConstruct() override;
    TMap<FName, FRelationNode> Nodes;
    TArray<FRelationEdge> Edges;
    FName SelectedNPCID;
    ERelationLineType CurrentFilter;
    bool bFilterActive = false;
};
