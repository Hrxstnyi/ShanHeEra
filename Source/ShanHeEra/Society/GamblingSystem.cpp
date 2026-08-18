#include "Society/GamblingSystem.h"
#include "Core/ShanHeLog.h"

void UGamblingSystem::InitializeGambling()
{
    UE_LOG(LogShanHe, Log, TEXT("赌博系统初始化 - 6种赌博方式"));
}

int32 UGamblingSystem::PlayDice(int32 BetAmount, int32 Luck, int32 GamblingSkill)
{
    // 骰子：纯运气为主，技能影响小
    int32 PlayerRoll = FMath::RandRange(1, 6) + FMath::RandRange(1, 6) + FMath::RandRange(1, 6);
    int32 DealerRoll = FMath::RandRange(1, 6) + FMath::RandRange(1, 6) + FMath::RandRange(1, 6);
    PlayerRoll += Luck / 5 + GamblingSkill / 10;

    bool bWon = PlayerRoll > DealerRoll;
    int32 Result = bWon ? BetAmount : -BetAmount;

    FGambleRecord Rec;
    Rec.Type = EGambleType::Dice; Rec.BetAmount = BetAmount;
    Rec.WinAmount = Result; Rec.bWon = bWon; Rec.Time = FDateTime::Now();
    History.Add(Rec);

    if (bWon) TotalWon += BetAmount; else TotalLost += BetAmount;
    Addiction = FMath::Min(100, Addiction + (bWon ? 3 : 5));

    UE_LOG(LogShanHe, Log, TEXT("骰子：玩家%d 庄家%d，%s，净赚%d"), PlayerRoll, DealerRoll, bWon ? TEXT("赢") : TEXT("输"), Result);
    return Result;
}

int32 UGamblingSystem::PlayPaiGow(int32 BetAmount, int32 Intelligence, int32 GamblingSkill)
{
    // 牌九：技术+运气
    int32 Score = Intelligence + GamblingSkill * 2 + FMath::RandRange(0, 50);
    int32 DealerScore = 30 + FMath::RandRange(0, 50);
    bool bWon = Score > DealerScore;
    int32 Result = bWon ? BetAmount : -BetAmount;

    if (bWon) TotalWon += BetAmount; else TotalLost += BetAmount;
    Addiction = FMath::Min(100, Addiction + (bWon ? 2 : 4));

    UE_LOG(LogShanHe, Log, TEXT("牌九：%s，净赚%d"), bWon ? TEXT("赢") : TEXT("输"), Result);
    return Result;
}

int32 UGamblingSystem::PlayCockfight(int32 BetAmount, int32 CockQuality)
{
    int32 PlayerScore = CockQuality + FMath::RandRange(0, 30);
    int32 OpponentScore = 50 + FMath::RandRange(0, 30);
    bool bWon = PlayerScore > OpponentScore;
    int32 Result = bWon ? BetAmount : -BetAmount;

    if (bWon) TotalWon += BetAmount; else TotalLost += BetAmount;
    Addiction = FMath::Min(100, Addiction + 2);

    UE_LOG(LogShanHe, Log, TEXT("斗鸡：%s，净赚%d"), bWon ? TEXT("赢") : TEXT("输"), Result);
    return Result;
}

int32 UGamblingSystem::PlayCricket(int32 BetAmount, int32 CricketQuality)
{
    int32 PlayerScore = CricketQuality + FMath::RandRange(0, 20);
    int32 OpponentScore = 50 + FMath::RandRange(0, 20);
    bool bWon = PlayerScore > OpponentScore;
    int32 Result = bWon ? BetAmount * 2 : -BetAmount; // 蛐蛐赔率高

    if (bWon) TotalWon += BetAmount * 2; else TotalLost += BetAmount;
    Addiction = FMath::Min(100, Addiction + 3);

    UE_LOG(LogShanHe, Log, TEXT("斗蛐蛐：%s，净赚%d"), bWon ? TEXT("赢") : TEXT("输"), Result);
    return Result;
}

int32 UGamblingSystem::GetTotalWinnings() const
{
    return TotalWon - TotalLost;
}

int32 UGamblingSystem::GetGamblingAddiction() const
{
    return Addiction;
}

bool UGamblingSystem::IsGamblingDebt() const
{
    return (TotalLost - TotalWon) > MaxDebt;
}
