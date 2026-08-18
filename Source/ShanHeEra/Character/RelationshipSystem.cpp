#include "Character/RelationshipSystem.h"
#include "Core/ShanHeLog.h"

void URelationshipSystem::InitializeRelations()
{
    UE_LOG(LogShanHe, Log, TEXT("关系系统初始化 - 好感/婚姻/结拜/师徒/世仇"));
}

void URelationshipSystem::ChangeOpinion(FGuid A, FGuid B, int32 Amount)
{
    FRelationData* Rel = FindRelation(A, B);
    if (!Rel)
    {
        FRelationData NewRel;
        NewRel.NPCA = A; NewRel.NPCB = B;
        NewRel.Opinion = Amount;
        Relations.Add(NewRel);
        Rel = &Relations.Last();
    }
    else
    {
        Rel->Opinion = FMath::Clamp(Rel->Opinion + Amount, -100, 100);
    }
    UpdateRelationType(*Rel);
}

bool URelationshipSystem::ProposeMarriage(FGuid A, FGuid B)
{
    FRelationData* Rel = FindRelation(A, B);
    if (!Rel || Rel->Opinion < 70 || Rel->Romance < 50) return false;
    Rel->bIsMarried = true;
    Rel->Type = ERelationType::Spouse;
    UE_LOG(LogShanHe, Log, TEXT("成婚！%s 与 %s"), *A.ToString(), *B.ToString());
    return true;
}

bool URelationshipSystem::ProposeSwornBrotherhood(FGuid A, FGuid B)
{
    FRelationData* Rel = FindRelation(A, B);
    if (!Rel || Rel->Opinion < 60 || Rel->Trust < 50) return false;
    Rel->Type = ERelationType::Sworn;
    UE_LOG(LogShanHe, Log, TEXT("结拜！%s 与 %s"), *A.ToString(), *B.ToString());
    return true;
}

bool URelationshipSystem::ProposeMasterDisciple(FGuid Master, FGuid Disciple)
{
    FRelationData* Rel = FindRelation(Master, Disciple);
    if (!Rel || Rel->Opinion < 40) return false;
    Rel->Type = ERelationType::Master;
    UE_LOG(LogShanHe, Log, TEXT("拜师！%s 收 %s 为徒"), *Master.ToString(), *Disciple.ToString());
    return true;
}

void URelationshipSystem::DeclareEnemy(FGuid A, FGuid B)
{
    FRelationData* Rel = FindRelation(A, B);
    if (!Rel)
    {
        FRelationData NewRel;
        NewRel.NPCA = A; NewRel.NPCB = B;
        NewRel.Opinion = -80;
        NewRel.Type = ERelationType::Enemy;
        Relations.Add(NewRel);
    }
    else
    {
        Rel->Opinion = FMath::Max(-100, Rel->Opinion - 50);
        Rel->Type = ERelationType::Enemy;
    }
    UE_LOG(LogShanHe, Warning, TEXT("结仇！%s 与 %s"), *A.ToString(), *B.ToString());
}

ERelationType URelationshipSystem::GetRelation(FGuid A, FGuid B) const
{
    for (const FRelationData& R : Relations)
        if ((R.NPCA == A && R.NPCB == B) || (R.NPCA == B && R.NPCB == A))
            return R.Type;
    return ERelationType::Stranger;
}

int32 URelationshipSystem::GetOpinion(FGuid A, FGuid B) const
{
    for (const FRelationData& R : Relations)
        if ((R.NPCA == A && R.NPCB == B) || (R.NPCA == B && R.NPCB == A))
            return R.Opinion;
    return 0;
}

TArray<FGuid> URelationshipSystem::GetFamily(FGuid NPC) const
{
    TArray<FGuid> Result;
    for (const FRelationData& R : Relations)
    {
        if (R.Type == ERelationType::Family || R.Type == ERelationType::Spouse)
        {
            if (R.NPCA == NPC) Result.Add(R.NPCB);
            else if (R.NPCB == NPC) Result.Add(R.NPCA);
        }
    }
    return Result;
}

TArray<FGuid> URelationshipSystem::GetFriends(FGuid NPC, int32 MinOpinion) const
{
    TArray<FGuid> Result;
    for (const FRelationData& R : Relations)
    {
        if (R.Opinion >= MinOpinion && R.Type != ERelationType::Enemy)
        {
            if (R.NPCA == NPC) Result.Add(R.NPCB);
            else if (R.NPCB == NPC) Result.Add(R.NPCA);
        }
    }
    return Result;
}

TArray<FGuid> URelationshipSystem::GetEnemies(FGuid NPC) const
{
    TArray<FGuid> Result;
    for (const FRelationData& R : Relations)
    {
        if (R.Type == ERelationType::Enemy || R.Type == ERelationType::Nemesis || R.Opinion < -50)
        {
            if (R.NPCA == NPC) Result.Add(R.NPCB);
            else if (R.NPCB == NPC) Result.Add(R.NPCA);
        }
    }
    return Result;
}

FRelationData* URelationshipSystem::FindRelation(FGuid A, FGuid B)
{
    for (FRelationData& R : Relations)
        if ((R.NPCA == A && R.NPCB == B) || (R.NPCA == B && R.NPCB == A))
            return &R;
    return nullptr;
}

void URelationshipSystem::UpdateRelationType(FRelationData& Rel)
{
    if (Rel.bIsMarried) { Rel.Type = ERelationType::Spouse; return; }
    if (Rel.Opinion >= 80 && Rel.Romance >= 50) { Rel.Type = ERelationType::Lover; return; }
    if (Rel.Opinion >= 70) { Rel.Type = ERelationType::CloseFriend; return; }
    if (Rel.Opinion >= 40) { Rel.Type = ERelationType::Friend; return; }
    if (Rel.Opinion >= 10) { Rel.Type = ERelationType::Acquaintance; return; }
    if (Rel.Opinion <= -80) { Rel.Type = ERelationType::Nemesis; return; }
    if (Rel.Opinion <= -40) { Rel.Type = ERelationType::Enemy; return; }
    if (Rel.Opinion <= -10) { Rel.Type = ERelationType::Rival; return; }
    Rel.Type = ERelationType::Stranger;
}
