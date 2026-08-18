#include "Society/JianghuSystem.h"
#include "Core/ShanHeLog.h"

void UJianghuSystem::InitializeJianghu()
{
    // 初始化江湖势力
    FJianghuFaction Shaolin;
    Shaolin.FactionID = TEXT("Shaolin");
    Shaolin.FactionName = NSLOCTEXT("Jianghu","Shaolin","少林寺");
    Shaolin.Type = EJianghuFactionType::Temple;
    Shaolin.BaseCity = TEXT("Luoyang");
    Shaolin.Members = 500;
    Shaolin.Influence = 80;
    Shaolin.Reputation = 90;
    Shaolin.Description = NSLOCTEXT("Jianghu","ShaolinDesc","天下武功出少林，佛门圣地，正道领袖");
    Factions.Add(Shaolin);

    FJianghuFaction Wudang;
    Wudang.FactionID = TEXT("Wudang");
    Wudang.FactionName = NSLOCTEXT("Jianghu","Wudang","武当派");
    Wudang.Type = EJianghuFactionType::Sect;
    Wudang.BaseCity = TEXT("Xiangyang");
    Wudang.Members = 300;
    Wudang.Influence = 75;
    Wudang.Reputation = 85;
    Wudang.Description = NSLOCTEXT("Jianghu","WudangDesc","以柔克刚，道法自然，内家拳发源地");
    Factions.Add(Wudang);

    FJianghuFaction BiaoJu;
    BiaoJu.FactionID = TEXT("TianXiaBiaoJu");
    BiaoJu.FactionName = NSLOCTEXT("Jianghu","BiaoJu","天下镖局");
    BiaoJu.Type = EJianghuFactionType::Escort;
    BiaoJu.BaseCity = TEXT("Beijing");
    BiaoJu.Members = 800;
    BiaoJu.Influence = 60;
    BiaoJu.Reputation = 70;
    BiaoJu.Description = NSLOCTEXT("Jianghu","BiaoJuDesc","北方最大镖局，信誉卓著，走南闯北");
    Factions.Add(BiaoJu);

    FJianghuFaction GreenGang;
    GreenGang.FactionID = TEXT("QingBang");
    GreenGang.FactionName = NSLOCTEXT("Jianghu","GreenGang","青帮");
    GreenGang.Type = EJianghuFactionType::Gang;
    GreenGang.BaseCity = TEXT("Hangzhou");
    GreenGang.Members = 2000;
    GreenGang.Influence = 70;
    GreenGang.Reputation = 30;
    GreenGang.Description = NSLOCTEXT("Jianghu","GreenGangDesc","漕运起家，势力遍布运河两岸，亦正亦邪");
    Factions.Add(GreenGang);

    UE_LOG(LogShanHe, Log, TEXT("江湖系统初始化 - %d 股势力，江湖不是幻想世界，是官方之外的人际网络"), Factions.Num());
}

void UJianghuSystem::Simulate(int32 Days)
{
    SimulateFactions(Days);
    SimulateConflicts(Days);
    if (FMath::RandRange(0, 100) < 5) RandomJianghuEvents();
}

void UJianghuSystem::ChangeReputation(FName PersonID, int32 Amount)
{
    if (int32* Rep = PersonReputations.Find(PersonID))
    {
        *Rep = FMath::Clamp(*Rep + Amount, -100, 100);
    }
    else
    {
        PersonReputations.Add(PersonID, FMath::Clamp(Amount, -100, 100));
    }
}

EJianghuRank UJianghuSystem::GetJianghuRank(int32 Reputation) const
{
    if (Reputation >= 90) return EJianghuRank::Legend;
    if (Reputation >= 70) return EJianghuRank::Grandmaster;
    if (Reputation >= 50) return EJianghuRank::Master;
    if (Reputation >= 30) return EJianghuRank::Expert;
    if (Reputation >= 10) return EJianghuRank::Adept;
    if (Reputation >= 1) return EJianghuRank::Novice;
    return EJianghuRank::Unknown;
}

void UJianghuSystem::LearnMartialArt(FName PersonID, const FMartialArt& Art)
{
    TArray<FMartialArt>& Arts = PersonMartialArts.FindOrAdd(PersonID);
    // 检查是否已学
    for (FMartialArt& A : Arts)
    {
        if (A.SkillID == Art.SkillID) return;
    }
    Arts.Add(Art);
    UE_LOG(LogShanHe, Log, TEXT("%s 习得武功: %s"), *PersonID.ToString(), *Art.SkillName.ToString());
}

void UJianghuSystem::PracticeMartialArt(FName PersonID, FName SkillID, int32 Days)
{
    if (TArray<FMartialArt>* Arts = PersonMartialArts.Find(PersonID))
    {
        for (FMartialArt& A : *Arts)
        {
            if (A.SkillID == SkillID && A.Level < A.MaxLevel)
            {
                A.Level = FMath::Min(A.MaxLevel, A.Level + Days / 30); // 每月升1级
                break;
            }
        }
    }
}

void UJianghuSystem::TriggerJianghuEvent(FName EventID, FName PersonID)
{
    for (FJianghuEvent& E : Events)
    {
        if (E.EventID == EventID && !E.bIsTriggered)
        {
            E.bIsTriggered = true;
            ChangeReputation(PersonID, E.ReputationChange);
            UE_LOG(LogShanHe, Log, TEXT("江湖事件: %s，声望变化 %d"), *E.Title.ToString(), E.ReputationChange);
            break;
        }
    }
}

void UJianghuSystem::AddEnemy(FName PersonA, FName PersonB, int32 Severity)
{
    TMap<FName, int32>& RelsA = PersonRelations.FindOrAdd(PersonA);
    RelsA.Add(PersonB, FMath::Max(-100, (RelsA.FindRef(PersonB) - Severity)));
    TMap<FName, int32>& RelsB = PersonRelations.FindOrAdd(PersonB);
    RelsB.Add(PersonA, FMath::Max(-100, (RelsB.FindRef(PersonA) - Severity)));
}

int32 UJianghuSystem::GetRelation(FName PersonA, FName PersonB) const
{
    if (const TMap<FName, int32>* Rels = PersonRelations.Find(PersonA))
        return Rels->FindRef(PersonB);
    return 0;
}

void UJianghuSystem::SimulateFactions(int32 Days)
{
    for (FJianghuFaction& F : Factions)
    {
        F.Influence += FMath::RandRange(-1, 1) * (Days / 30);
        F.Wealth += FMath::RandRange(-50, 100) * (Days / 30);
    }
}

void UJianghuSystem::SimulateConflicts(int32 Days)
{
    // 江湖冲突：门派争斗、劫镖、寻仇
    if (FMath::RandRange(0, 1000) < 1)
    {
        UE_LOG(LogShanHe, Log, TEXT("江湖冲突：两派发生争斗"));
    }
}

void UJianghuSystem::RandomJianghuEvents()
{
    // 随机江湖事件：比武大会、侠客行侠、恶霸横行等
    UE_LOG(LogShanHe, Log, TEXT("江湖传闻：有大事发生"));
}
