#include "Story/DialogueSystem.h"
#include "Core/ShanHeLog.h"

void UDialogueSystem::StartDialogue(FGuid NPCID, FName StartNodeID)
{
    UE_LOG(LogShanHe, Log, TEXT("开始对话：NPC %s，节点 %s"), *NPCID.ToString(), *StartNodeID.ToString());
}

FDialogueNode UDialogueSystem::GetNode(FName NodeID) const
{
    if (const FDialogueNode* Found = DialogueTree.Find(NodeID))
        return *Found;
    return FDialogueNode();
}

void UDialogueSystem::SelectOption(FName NodeID, int32 OptionIndex)
{
    if (const FDialogueNode* Node = DialogueTree.Find(NodeID))
    {
        if (OptionIndex < Node->Options.Num())
        {
            const FDialogueOption& Opt = Node->Options[OptionIndex];
            if (Opt.OpinionChange != 0)
            {
                UE_LOG(LogShanHe, Log, TEXT("选择对话选项，好感变化：%d"), Opt.OpinionChange);
            }
            if (!Opt.TriggerQuestID.IsNone())
            {
                AcceptQuest(Opt.TriggerQuestID);
            }
        }
    }
}

void UDialogueSystem::AcceptQuest(FName QuestID)
{
    for (FQuest& Q : AllQuests)
    {
        if (Q.QuestID == QuestID && !Q.bIsActive)
        {
            Q.bIsActive = true;
            UE_LOG(LogShanHe, Log, TEXT("接受任务：%s"), *Q.QuestTitle.ToString());
            return;
        }
    }
}

void UDialogueSystem::CompleteQuest(FName QuestID)
{
    for (FQuest& Q : AllQuests)
    {
        if (Q.QuestID == QuestID && Q.bIsActive && !Q.bIsComplete)
        {
            Q.bIsComplete = true;
            Q.bIsActive = false;
            UE_LOG(LogShanHe, Log, TEXT("任务完成：%s，奖励：%d银两 +%d声望"),
                *Q.QuestTitle.ToString(), Q.RewardSilver, Q.RewardReputation);
            return;
        }
    }
}

TArray<FQuest> UDialogueSystem::GetActiveQuests() const
{
    TArray<FQuest> Result;
    for (const FQuest& Q : AllQuests)
        if (Q.bIsActive) Result.Add(Q);
    return Result;
}
