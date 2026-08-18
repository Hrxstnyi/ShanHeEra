#include "Military/SiegeSystem.h"
#include "Core/ShanHeLog.h"

void USiegeSystem::StartSiege(FName CityID, int32 BesiegerStrength)
{
    CurrentSiege = FSiegeData();
    CurrentSiege.CityID = CityID;
    CurrentSiege.Phase = ESiegePhase::Blockade;
    CurrentSiege.BesiegerStrength = BesiegerStrength;
    CurrentSiege.DefenderStrength = 5000; // 基础守军
    CurrentSiege.WallIntegrity = 100.0f;
    CurrentSiege.CityFood = 180; // 默认半年存粮
    CurrentSiege.CityMorale = 80;
    bIsSiegeActive = true;
    UE_LOG(LogShanHe, Warning, TEXT("围城开始！城市: %s，攻城方兵力: %d"), *CityID.ToString(), BesiegerStrength);
}

void USiegeSystem::SimulateSiege(int32 Days)
{
    if (!bIsSiegeActive) return;
    CurrentSiege.SiegeDays += Days;

    // 消耗存粮
    CurrentSiege.CityFood = FMath::Max(0, CurrentSiege.CityFood - Days);

    // 存粮耗尽，士气下降
    if (CurrentSiege.CityFood <= 0)
    {
        CurrentSiege.CityMorale = FMath::Max(0, CurrentSiege.CityMorale - 5 * Days);
    }

    // 封锁阶段：双方消耗
    if (CurrentSiege.Phase == ESiegePhase::Blockade)
    {
        CurrentSiege.BesiegerStrength = FMath::Max(0, CurrentSiege.BesiegerStrength - Days * 5); // 疾病/逃兵
        if (CheckSurrender())
        {
            CurrentSiege.Phase = ESiegePhase::Surrender;
            bIsSiegeActive = false;
            UE_LOG(LogShanHe, Log, TEXT("%s 守军投降！围城 %d 天"), *CurrentSiege.CityID.ToString(), CurrentSiege.SiegeDays);
        }
    }

    // 强攻阶段：城墙受损，双方伤亡
    if (CurrentSiege.Phase == ESiegePhase::Assault)
    {
        CurrentSiege.WallIntegrity -= 2.0f * Days;
        CurrentSiege.BesiegerStrength -= 100 * Days;
        CurrentSiege.DefenderStrength -= 50 * Days;
        if (CurrentSiege.WallIntegrity <= 0)
        {
            CurrentSiege.Phase = ESiegePhase::Breach;
            UE_LOG(LogShanHe, Warning, TEXT("城墙被攻破！进入巷战阶段"));
        }
    }
}

void USiegeSystem::OrderAssault()
{
    if (bIsSiegeActive && CurrentSiege.Phase == ESiegePhase::Blockade)
    {
        CurrentSiege.Phase = ESiegePhase::Assault;
        UE_LOG(LogShanHe, Log, TEXT("下令强攻！"));
    }
}

void USiegeSystem::OrderBombardment()
{
    if (bIsSiegeActive && CurrentSiege.bHasSiegeWeapons)
    {
        CurrentSiege.Phase = ESiegePhase::Bombardment;
        CurrentSiege.WallIntegrity -= 5.0f; // 炮击伤害更高
        UE_LOG(LogShanHe, Log, TEXT("开始炮击！城墙完整度: %.1f%%"), CurrentSiege.WallIntegrity);
    }
}

void USiegeSystem::LiftSiege()
{
    bIsSiegeActive = false;
    CurrentSiege.Phase = ESiegePhase::Lifted;
    UE_LOG(LogShanHe, Log, TEXT("围城解除"));
}

bool USiegeSystem::CheckSurrender() const
{
    // 士气低于20或存粮耗尽且士气低于40，则投降
    return CurrentSiege.CityMorale < 20 ||
           (CurrentSiege.CityFood <= 0 && CurrentSiege.CityMorale < 40);
}
