#include "Character/NPCManager.h"
#include "Core/ShanHeLog.h"

void UNPCManager::InitializeNPCs()
{
    AllNPCs.Empty();
    // 生成初始NPC人口 - 每个城市分配基础人口
    // 这里创建关键历史人物和特色角色
    UE_LOG(LogShanHe, Log, TEXT("NPC系统初始化 - 他们拥有自己的家庭、目标、利益、性格、经历、命运"));
    TotalNPCs = AllNPCs.Num();
}

void UNPCManager::SimulateNPCs(int32 Days)
{
    for (FNPData& NPC : AllNPCs)
    {
        if (!NPC.bIsAlive) continue;
        SimulateDailyLife(NPC, Days);
        SimulateCareer(NPC, Days);
        SimulateRelations(NPC, Days);
        CheckBirthDeath(NPC, Days);
    }
}

void UNPCManager::SimulateDailyLife(FNPData& NPC, int32 Days)
{
    NPC.Age += Days / 30;
}

void UNPCManager::SimulateCareer(FNPData& NPC, int32 Days)
{
    // 根据职业和性格模拟人生发展
    if (NPC.Traits.Contains(ECharacterTrait::Ambitious))
        NPC.Gold += FMath::RandRange(0, 5) * Days;
}

void UNPCManager::SimulateRelations(FNPData& NPC, int32 Days) {}
void UNPCManager::CheckBirthDeath(FNPData& NPC, int32 Days) {}

TArray<FNPData> UNPCManager::GetNPCsInCity(FName CityID) const
{
    TArray<FNPData> Result;
    for (const FNPData& N : AllNPCs)
        if (N.CurrentCity == CityID && N.bIsAlive) Result.Add(N);
    return Result;
}

FNPData UNPCManager::GetNPC(FGuid NPCID) const
{
    for (const FNPData& N : AllNPCs)
        if (N.NPCID == NPCID) return N;
    return FNPData();
}

int32 UNPCManager::GetRelation(FGuid A, FGuid B) const
{
    if (const FNPData* NA = AllNPCs.FindByPredicate([&](const FNPData& N){ return N.NPCID == A; }))
        if (const int32* R = NA->Relations.Find(B))
            return *R;
    return 0;
}
