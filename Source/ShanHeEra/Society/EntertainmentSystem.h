#pragma once
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "EntertainmentSystem.generated.h"

UENUM(BlueprintType)
enum class EGameType : uint8
{
    Dice        UMETA(DisplayName="骰子"),
    PaiGow      UMETA(DisplayName="牌九"),
    Mahjong     UMETA(DisplayName="麻将"),
    CockFight   UMETA(DisplayName="斗鸡"),
    Cricket     UMETA(DisplayName="斗蟋蟀"),
    Storytelling UMETA(DisplayName="听说书"),
    Opera       UMETA(DisplayName="看戏"),
    Chess       UMETA(DisplayName="下棋")
};

USTRUCT(BlueprintType)
struct FGamblingResult
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite) bool bWon = false;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 Winnings = 0;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 BetAmount = 0;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) float WinChance = 0.5f;
};

/**
 * 市井娱乐系统 - 骰子/牌九/斗鸡/说书/戏曲/下棋
 * 赌场玩法，影响金钱和快乐度
 */
UCLASS()
class SHANHEERA_API UEntertainmentSystem : public UObject
{
    GENERATED_BODY()
public:
    UFUNCTION(BlueprintCallable, Category="山河纪元|娱乐")
    FGamblingResult PlayDice(int32 BetAmount, int32 LuckAttribute);

    UFUNCTION(BlueprintCallable, Category="山河纪元|娱乐")
    FGamblingResult PlayPaiGow(int32 BetAmount, int32 Intelligence);

    UFUNCTION(BlueprintCallable, Category="山河纪元|娱乐")
    void WatchStorytelling(FName StoryID);

    UFUNCTION(BlueprintCallable, Category="山河纪元|娱乐")
    void WatchOpera(FName OperaID);

    UFUNCTION(BlueprintCallable, Category="山河纪元|娱乐")
    int32 PlayChess(int32 PlayerSkill, int32 OpponentSkill);

    UPROPERTY(BlueprintReadOnly, Category="山河纪元|娱乐")
    int32 TotalGamblingWins = 0;

    UPROPERTY(BlueprintReadOnly, Category="山河纪元|娱乐")
    int32 TotalGamblingLosses = 0;

    UPROPERTY(BlueprintReadOnly, Category="山河纪元|娱乐")
    int32 NetProfit = 0;
};
