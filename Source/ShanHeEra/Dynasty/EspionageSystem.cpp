#include "Dynasty/EspionageSystem.h"
#include "Core/ShanHeLog.h"

void UEspionageSystem::InitializeEspionage()
{
    struct FAgentDef { FName ID; const TCHAR* Name; int32 Stealth; int32 Charm; int32 Combat; };
    FAgentDef Agents[] = {
        {TEXT("spy_01"), TEXT("影子"), 85, 60, 70},
        {TEXT("spy_02"), TEXT("夜莺"), 70, 90, 40},
        {TEXT("spy_03"), TEXT("无痕"), 90, 50, 80},
        {TEXT("spy_04"), TEXT("媚娘"), 50, 95, 30},
        {TEXT("spy_05"), TEXT("墨者"), 75, 65, 75},
    };

    for (const auto& A : Agents)
    {
        FSpyAgent Ag;
        Ag.AgentID = A.ID; Ag.AgentName = FText::FromString(UTF8_TO_TCHAR(A.Name));
        Ag.Stealth = A.Stealth; Ag.Charm = A.Charm; Ag.Combat = A.Combat;
        AgentRoster.Add(A.ID, Ag);
    }

    UE_LOG(LogShanHe, Log, TEXT("情报系统初始化 - %d 名间谍"), AgentRoster.Num());
}

bool UEspionageSystem::DeployAgent(FName AgentID, FName TargetFaction)
{
    if (FSpyAgent* A = AgentRoster.Find(AgentID))
    {
        if (A->bIsActive) return false;
        A->bIsActive = true;
        A->TargetFaction = TargetFaction;
        A->CoverLevel = 10;
        UE_LOG(LogShanHe, Log, TEXT("部署间谍 %s 到 %s"), *AgentID.ToString(), *TargetFaction.ToString());
        return true;
    }
    return false;
}

FIntelligence UEspionageSystem::GatherIntelligence(FName TargetFaction)
{
    FIntelligence Intel;
    Intel.IntelID = FName(*FString::Printf(TEXT("intel_%d"), FMath::Rand()));
    Intel.SourceFaction = TargetFaction;
    Intel.Reliability = FMath::RandRange(30, 90);
    Intel.ObtainedDate = FDateTime::Now();

    // 根据情报网等级和目标难度生成内容
    UE_LOG(LogShanHe, Log, TEXT("收集情报：%s, 可信度%d"), *TargetFaction.ToString(), Intel.Reliability);
    IntelligenceCache.Add(Intel);
    return Intel;
}

bool UEspionageSystem::AssassinateTarget(FName TargetID)
{
    // 成功率取决于间谍战斗+潜行，目标护卫等级
    int32 SuccessRate = 30 + SpyNetworkLevel * 5;
    int32 Roll = FMath::RandRange(1, 100);
    if (Roll <= SuccessRate)
    {
        UE_LOG(LogShanHe, Log, TEXT("刺杀成功：%s"), *TargetID.ToString());
        return true;
    }
    UE_LOG(LogShanHe, Log, TEXT("刺杀失败：%s (roll=%d, rate=%d)"), *TargetID.ToString(), Roll, SuccessRate);
    return false;
}

bool UEspionageSystem::CounterIntelligence(FName TargetFaction)
{
    // 反间计：让目标内部产生猜忌
    int32 SuccessRate = 40 + SpyNetworkLevel * 3;
    int32 Roll = FMath::RandRange(1, 100);
    if (Roll <= SuccessRate)
    {
        UE_LOG(LogShanHe, Log, TEXT("反间计成功：%s 内部产生猜忌"), *TargetFaction.ToString());
        return true;
    }
    UE_LOG(LogShanHe, Log, TEXT("反间计失败"));
    return false;
}

bool UEspionageSystem::HoneyTrap(FName AgentID, FName TargetID)
{
    if (const FSpyAgent* A = AgentRoster.Find(AgentID))
    {
        int32 SuccessRate = A->Charm;
        int32 Roll = FMath::RandRange(1, 100);
        if (Roll <= SuccessRate)
        {
            UE_LOG(LogShanHe, Log, TEXT("美人计成功：%s 迷惑 %s"), *AgentID.ToString(), *TargetID.ToString());
            return true;
        }
    }
    UE_LOG(LogShanHe, Log, TEXT("美人计失败"));
    return false;
}

bool UEspionageSystem::BribeOfficial(FName TargetID, int32 Amount)
{
    // 贿赂成功率取决于金额
    int32 SuccessRate = FMath::Min(90, Amount / 100);
    int32 Roll = FMath::RandRange(1, 100);
    if (Roll <= SuccessRate)
    {
        UE_LOG(LogShanHe, Log, TEXT("贿赂成功：%s, 花费%d"), *TargetID.ToString(), Amount);
        return true;
    }
    UE_LOG(LogShanHe, Log, TEXT("贿赂失败：%s"), *TargetID.ToString());
    return false;
}

void UEspionageSystem::RecallAgent(FName AgentID)
{
    if (FSpyAgent* A = AgentRoster.Find(AgentID))
    {
        A->bIsActive = false;
        A->TargetFaction = NAME_None;
        A->CoverLevel = 0;
        UE_LOG(LogShanHe, Log, TEXT("召回间谍：%s"), *AgentID.ToString());
    }
}
