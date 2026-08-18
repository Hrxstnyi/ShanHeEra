#include "Society/EntertainmentSystem.h"
#include "Core/ShanHeLog.h"

FGamblingResult UEntertainmentSystem::PlayDice(int32 BetAmount, int32 LuckAttribute)
{
    FGamblingResult Result;
    Result.BetAmount = BetAmount;
    // 福源影响胜率，基础50%，每点福源+1%
    Result.WinChance = FMath::Clamp(0.5f + LuckAttribute * 0.01f, 0.1f, 0.9f);
    Result.bWon = FMath::RandRange(0.0f, 1.0f) < Result.WinChance;
    if (Result.bWon)
    {
        Result.Winnings = BetAmount; // 1:1赔率
        TotalGamblingWins++;
        NetProfit += BetAmount;
    }
    else
    {
        Result.Winnings = -BetAmount;
        TotalGamblingLosses++;
        NetProfit -= BetAmount;
    }
    UE_LOG(LogShanHe, Log, TEXT("掷骰子：%s，%s %d"),
        Result.bWon ? TEXT("赢") : TEXT("输"),
        Result.bWon ? TEXT("获得") : TEXT("输掉"),
        BetAmount);
    return Result;
}

FGamblingResult UEntertainmentSystem::PlayPaiGow(int32 BetAmount, int32 Intelligence)
{
    FGamblingResult Result;
    Result.BetAmount = BetAmount;
    // 悟性影响牌九胜率
    Result.WinChance = FMath::Clamp(0.45f + Intelligence * 0.015f, 0.1f, 0.85f);
    Result.bWon = FMath::RandRange(0.0f, 1.0f) < Result.WinChance;
    Result.Winnings = Result.bWon ? BetAmount * 2 : -BetAmount; // 1:2赔率
    if (Result.bWon) { TotalGamblingWins++; NetProfit += BetAmount * 2; }
    else { TotalGamblingLosses++; NetProfit -= BetAmount; }
    return Result;
}

void UEntertainmentSystem::WatchStorytelling(FName StoryID)
{
    UE_LOG(LogShanHe, Log, TEXT("听说书：%s"), *StoryID.ToString());
}

void UEntertainmentSystem::WatchOpera(FName OperaID)
{
    UE_LOG(LogShanHe, Log, TEXT("看戏：%s"), *OperaID.ToString());
}

int32 UEntertainmentSystem::PlayChess(int32 PlayerSkill, int32 OpponentSkill)
{
    int32 Diff = PlayerSkill - OpponentSkill;
    float WinChance = FMath::Clamp(0.5f + Diff * 0.02f, 0.05f, 0.95f);
    return FMath::RandRange(0.0f, 1.0f) < WinChance ? 1 : 0; // 1=胜, 0=负
}
