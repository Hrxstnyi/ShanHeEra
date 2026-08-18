#include "Military/WarSystem.h"
#include "Core/ShanHeLog.h"

void UWarSystem::InitializeMilitary()
{
    FArmyData ImperialGuard;
    ImperialGuard.ArmyID = TEXT("ImperialGuard");
    ImperialGuard.ArmyName = FText::FromString(TEXT("禁军"));
    ImperialGuard.Infantry = 50000;
    ImperialGuard.Cavalry = 10000;
    ImperialGuard.Archers = 20000;
    ImperialGuard.Morale = 90;
    ImperialGuard.Training = 85;
    ImperialGuard.StationedCity = TEXT("Yingtian");
    Armies.Add(ImperialGuard);

    FArmyData Navy;
    Navy.ArmyID = TEXT("ImperialNavy");
    Navy.ArmyName = FText::FromString(TEXT("水师"));
    Navy.Navy = 30000;
    Navy.Infantry = 10000;
    Navy.StationedCity = TEXT("Quanzhou");
    Armies.Add(Navy);

    TotalSoldiers = 120000;
    UE_LOG(LogShanHe, Log, TEXT("军事系统初始化 - 战争不是个人英雄战斗，依靠粮食、兵员、训练、将领、地形、情报"));
}

void UWarSystem::Simulate(int32 Days)
{
    for (FWarData& War : ActiveWars)
    {
        if (!War.bIsActive) continue;
        War.DurationDays += Days;
        // 战争消耗
    }
}

void UWarSystem::DeclareWar(FName Attacker, FName Defender, FText WarName)
{
    FWarData NewWar;
    NewWar.WarID = FName(*FString::Printf(TEXT("War_%d"), FDateTime::Now().GetTicks()));
    NewWar.WarName = WarName;
    NewWar.Attacker = Attacker;
    NewWar.Defender = Defender;
    NewWar.bIsActive = true;
    ActiveWars.Add(NewWar);
    UE_LOG(LogShanHe, Warning, TEXT("战争爆发：%s"), *WarName.ToString());
}

EWarResult UWarSystem::ResolveBattle(FArmyData& Attacker, FArmyData& Defender, FName City)
{
    // 综合战力计算：兵力*训练*士气*将领*地形
    float AtkPower = (Attacker.Infantry + Attacker.Cavalry * 1.5f + Attacker.Archers * 1.2f)
        * (Attacker.Training / 50.0f) * (Attacker.Morale / 80.0f);
    float DefPower = (Defender.Infantry + Defender.Cavalry * 1.2f + Defender.Archers * 1.5f)
        * (Defender.Training / 50.0f) * (Defender.Morale / 80.0f) * 1.3f; // 防守加成

    if (AtkPower > DefPower * 1.2f) return EWarResult::Victory;
    if (AtkPower < DefPower * 0.8f) return EWarResult::Defeat;
    return EWarResult::Stalemate;
}

void UWarSystem::SiegeCity(FName CityID, FArmyData& Besieger)
{
    UE_LOG(LogShanHe, Log, TEXT("围城战开始：%s"), *CityID.ToString());
}
