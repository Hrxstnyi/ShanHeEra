#pragma once
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DialogueSystem.generated.h"

USTRUCT(BlueprintType)
struct FDialogueOption
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FText Text;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FName NextNodeID;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 RequiredReputation = 0;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FName RequiredItem;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 OpinionChange = 0;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FName TriggerQuestID;
};

USTRUCT(BlueprintType)
struct FDialogueNode
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FName NodeID;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FText SpeakerName;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FText DialogueText;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) TArray<FDialogueOption> Options;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) bool bIsEnd = false;
};

USTRUCT(BlueprintType)
struct FQuest
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FName QuestID;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FText QuestTitle;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FText QuestDescription;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FGuid GiverID;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) bool bIsActive = false;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) bool bIsComplete = false;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 RewardSilver = 0;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 RewardReputation = 0;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FName RewardItem;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) TArray<FName> Objectives;
};

/**
 * NPC对话任务系统 - 对话树、好感度、支线任务框架
 */
UCLASS()
class SHANHEERA_API UDialogueSystem : public UObject
{
    GENERATED_BODY()
public:
    UFUNCTION(BlueprintCallable, Category="山河纪元|对话")
    void StartDialogue(FGuid NPCID, FName StartNodeID);

    UFUNCTION(BlueprintCallable, Category="山河纪元|对话")
    FDialogueNode GetNode(FName NodeID) const;

    UFUNCTION(BlueprintCallable, Category="山河纪元|对话")
    void SelectOption(FName NodeID, int32 OptionIndex);

    UFUNCTION(BlueprintCallable, Category="山河纪元|对话")
    void AcceptQuest(FName QuestID);

    UFUNCTION(BlueprintCallable, Category="山河纪元|对话")
    void CompleteQuest(FName QuestID);

    UFUNCTION(BlueprintCallable, Category="山河纪元|对话")
    TArray<FQuest> GetActiveQuests() const;

    UPROPERTY(BlueprintReadOnly, Category="山河纪元|对话")
    TMap<FName, FDialogueNode> DialogueTree;

    UPROPERTY(BlueprintReadOnly, Category="山河纪元|对话")
    TArray<FQuest> AllQuests;

    UPROPERTY(BlueprintReadOnly, Category="山河纪元|对话")
    TMap<FGuid, int32> NPCRelations; // NPC好感度
};
