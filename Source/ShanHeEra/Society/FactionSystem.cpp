#include "Society/FactionSystem.h"
#include "Core/ShanHeLog.h"

bool UFactionSystem::CreateFaction(FText Name, EFactionType Type, FName HQCity)
{
    if (bHasFaction) return false;
    PlayerFaction.FactionID = FName(*FString::Printf(TEXT("Faction_%d"), FDateTime::Now().GetTicks()));
    PlayerFaction.FactionName = Name;
    PlayerFaction.Type = Type;
    PlayerFaction.Headquarters = HQCity;
    PlayerFaction.Morale = 50;
    PlayerFaction.Treasury = 0;
    bHasFaction = true;
    UE_LOG(LogShanHe, Warning, TEXT("创建势力：%s，类型：%d，总部：%s"),
        *Name.ToString(), (int32)Type, *HQCity.ToString());
    return true;
}

bool UFactionSystem::RecruitMember(FGuid NPCID)
{
    if (!bHasFaction) return false;
    if (PlayerFaction.Members.Contains(NPCID)) return false;
    PlayerFaction.Members.Add(NPCID);
    PlayerFaction.Morale = FMath::Min(100, PlayerFaction.Morale + 2);
    UE_LOG(LogShanHe, Log, TEXT("招募成员：%s，势力人数：%d"), *NPCID.ToString(), PlayerFaction.Members.Num());
    return true;
}

void UFactionSystem::ExpelMember(FGuid NPCID)
{
    if (PlayerFaction.Members.Remove(NPCID) > 0)
    {
        PlayerFaction.Morale = FMath::Max(0, PlayerFaction.Morale - 5);
        UE_LOG(LogShanHe, Log, TEXT("逐出成员：%s"), *NPCID.ToString());
    }
}

void UFactionSystem::CollectTreasury(int32 Amount)
{
    PlayerFaction.Treasury += Amount;
    UE_LOG(LogShanHe, Log, TEXT("势力金库：+%d，总计：%d"), Amount, PlayerFaction.Treasury);
}

void UFactionSystem::ExpandToCity(FName CityID)
{
    if (!bHasFaction) return;
    int32& Count = PlayerFaction.Holdings.FindOrAdd(CityID);
    Count++;
    PlayerFaction.Influence += 10;
    UE_LOG(LogShanHe, Log, TEXT("势力扩展到：%s"), *CityID.ToString());
}

void UFactionSystem::SimulateFaction(int32 Days)
{
    if (!bHasFaction) return;
    // 每日收入
    int32 DailyIncome = PlayerFaction.Members.Num() * PlayerFaction.Influence / 10;
    PlayerFaction.Treasury += DailyIncome * Days;
    // 士气变化
    if (PlayerFaction.Treasury < 0)
        PlayerFaction.Morale = FMath::Max(0, PlayerFaction.Morale - Days);
    else
        PlayerFaction.Morale = FMath::Min(100, PlayerFaction.Morale + Days / 10);
}
