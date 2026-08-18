#include "Military/BattleManager.h"
#include "Core/ShanHeLog.h"
#include "Audio/AudioManager.h"

void UBattleManager::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
    UE_LOG(LogShanHe, Log, TEXT("战斗管理器初始化 - 骑砍式战场战术层就绪"));
}

void UBattleManager::StartBattle(TArray<UArmyUnit*> Attackers, TArray<UArmyUnit*> Defenders, FName Location)
{
    AttackerUnits = Attackers;
    DefenderUnits = Defenders;
    BattleLocation = Location;
    CurrentPhase = EBattlePhase::Engagement;
    BattleTime = 0.0f;

    // 切换到战争音乐
    if (UGameInstance* GI = GetWorld()->GetGameInstance())
    {
        if (UAudioManager* AM = GI->GetSubsystem<UAudioManager>())
            AM->SetMusicMood(EMusicMood::War);
    }

    UE_LOG(LogShanHe, Warning, TEXT("战斗开始！地点: %s，攻击方 %d 部，防守方 %d 部"),
        *Location.ToString(), Attackers.Num(), Defenders.Num());
}

void UBattleManager::SimulateBattleTick(float DeltaTime)
{
    if (CurrentPhase == EBattlePhase::Ended || CurrentPhase == EBattlePhase::PreBattle) return;

    BattleTime += DeltaTime;

    // 阶段推进
    if (BattleTime > 30.0f && CurrentPhase == EBattlePhase::Engagement)
        CurrentPhase = EBattlePhase::Melee;
    if (BattleTime > 120.0f && CurrentPhase == EBattlePhase::Melee)
        CurrentPhase = EBattlePhase::Climax;

    ResolveEngagement();
    CalculateMoraleChanges();
    CheckBattleEnd();
}

void UBattleManager::ResolveEngagement()
{
    // 简化的战斗解算：双方各部队互相造成伤亡
    // 实际项目中应该用空间位置+距离+兵种克制来计算
    for (UArmyUnit* Atk : AttackerUnits)
    {
        if (!Atk || Atk->Data.bIsRouting || Atk->Data.Count <= 0) continue;
        for (UArmyUnit* Def : DefenderUnits)
        {
            if (!Def || Def->Data.bIsRouting || Def->Data.Count <= 0) continue;
            // 攻击方造成伤害
            int32 Damage = (int32)(Atk->CalculatePower() * 0.01f);
            Def->TakeCasualties(Damage);
            // 反击
            int32 CounterDamage = (int32)(Def->CalculatePower() * 0.008f);
            Atk->TakeCasualties(CounterDamage);
        }
    }
}

void UBattleManager::CalculateMoraleChanges()
{
    // 侧翼被击溃会影响全军士气
    // 将领阵亡/受伤会大幅降低士气
    // 这里简化处理
}

void UBattleManager::CheckBattleEnd()
{
    int32 AtkAlive = 0, DefAlive = 0;
    for (UArmyUnit* U : AttackerUnits) if (U && U->Data.Count > 0 && !U->Data.bIsRouting) AtkAlive++;
    for (UArmyUnit* U : DefenderUnits) if (U && U->Data.Count > 0 && !U->Data.bIsRouting) DefAlive++;

    if (AtkAlive == 0 || DefAlive == 0)
    {
        EndBattle();
    }
}

void UBattleManager::OrderCharge(FName UnitID)
{
    for (UArmyUnit* U : AttackerUnits)
    {
        if (U && U->Data.UnitID == UnitID)
        {
            U->Data.ChargeBonus = 20.0f;
            UE_LOG(LogShanHe, Log, TEXT("部队 %s 发起冲锋！"), *U->Data.UnitName.ToString());
        }
    }
}

void UBattleManager::OrderRetreat(FName UnitID)
{
    for (UArmyUnit* U : AttackerUnits)
    {
        if (U && U->Data.UnitID == UnitID)
        {
            U->Data.bIsRouting = true;
            UE_LOG(LogShanHe, Log, TEXT("部队 %s 撤退"), *U->Data.UnitName.ToString());
        }
    }
}

void UBattleManager::OrderRally(FName UnitID)
{
    for (UArmyUnit* U : AttackerUnits)
    {
        if (U && U->Data.UnitID == UnitID) U->Rally();
    }
}

void UBattleManager::EndBattle()
{
    CurrentPhase = EBattlePhase::Ended;
    FBattleResult Result = GenerateResult();
    OnBattleEnded.Broadcast(Result);

    // 切换音乐
    if (UGameInstance* GI = GetWorld()->GetGameInstance())
    {
        if (UAudioManager* AM = GI->GetSubsystem<UAudioManager>())
            AM->SetMusicMood(Result.Result == EWarResult::Victory ? EMusicMood::Triumphant : EMusicMood::Sorrowful);
    }

    UE_LOG(LogShanHe, Log, TEXT("战斗结束！结果: %d，攻击方损失 %d，防守方损失 %d"),
        (int32)Result.Result, Result.AttackerLosses, Result.DefenderLosses);
}

FBattleResult UBattleManager::GenerateResult()
{
    FBattleResult Result;
    int32 AtkTotal = 0, DefTotal = 0;
    int32 AtkAlive = 0, DefAlive = 0;
    for (UArmyUnit* U : AttackerUnits) { if (U) { AtkTotal += U->Data.MaxCount; if (U->Data.Count > 0) AtkAlive += U->Data.Count; else Result.AttackerLosses += U->Data.MaxCount; } }
    for (UArmyUnit* U : DefenderUnits) { if (U) { DefTotal += U->Data.MaxCount; if (U->Data.Count > 0) DefAlive += U->Data.Count; else Result.DefenderLosses += U->Data.MaxCount; } }

    Result.AttackerLosses = AtkTotal - AtkAlive;
    Result.DefenderLosses = DefTotal - DefAlive;
    Result.Result = (AtkAlive > DefAlive) ? EWarResult::Victory : EWarResult::Defeat;
    Result.DurationDays = BattleTime / 60.0f;
    return Result;
}
