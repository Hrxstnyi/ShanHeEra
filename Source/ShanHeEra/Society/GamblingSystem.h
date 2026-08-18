#pragma once
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GamblingSystem.generated.h"

UENUM(BlueprintType)
enum class EGambleType : uint8
{
    Dice        UMETA(DisplayName="骰子"),
    PaiGow      UMETA(DisplayName="牌九"),
    Cockfight   UMETA(DisplayName="斗鸡"),
    Cricket     UMETA(DisplayName="斗蛐蛐"),
    Mahjong     UMETA(DisplayName="麻将"),
    Lottery     UMETA(DisplayName="抽签")
};

USTRUCT(BlueprintType)
struct FGambleRecord
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite) EGambleType Type;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 BetAmount = 0;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 WinAmount = 0;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) bool bWon = false;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FDateTime Time;
};

/**
 * 赌博系统 - 骰子/牌九/斗鸡/蛐蛐，赌徒人生线
 */
UCLASS()
class SHANHEERA_API UGamblingSystem : public UObject
{
    GENERATED_BODY()
public:
    UFUNCTION(BlueprintCallable, Category="山河纪元|赌博")
    void InitializeGambling();

    UFUNCTION(BlueprintCallable, Category="山河纪元|赌博")
    int32 PlayDice(int32 BetAmount, int32 Luck, int32 GamblingSkill);

    UFUNCTION(BlueprintCallable, Category="山河纪元|赌博")
    int32 PlayPaiGow(int32 BetAmount, int32 Intelligence, int32 GamblingSkill);

    UFUNCTION(BlueprintCallable, Category="山河纪元|赌博")
    int32 PlayCockfight(int32 BetAmount, int32 CockQuality);

    UFUNCTION(BlueprintCallable, Category="山河纪元|赌博")
    int32 PlayCricket(int32 BetAmount, int32 CricketQuality);

    UFUNCTION(BlueprintCallable, Category="山河纪元|赌博")
    int32 GetTotalWinnings() const;

    UFUNCTION(BlueprintCallable, Category="山河纪元|赌博")
    int32 GetGamblingAddiction() const; // 赌瘾 0-100

    UFUNCTION(BlueprintCallable, Category="山河纪元|赌博")
    bool IsGamblingDebt() const; // 是否负债

    UPROPERTY(BlueprintReadOnly, Category="山河纪元|赌博")
    TArray<FGambleRecord> History;

    UPROPERTY(BlueprintReadOnly, Category="山河纪元|赌博")
    int32 TotalWon = 0;

    UPROPERTY(BlueprintReadOnly, Category="山河纪元|赌博")
    int32 TotalLost = 0;

    UPROPERTY(BlueprintReadOnly, Category="山河纪元|赌博")
    int32 Addiction = 0; // 赌瘾

    UPROPERTY(EditAnywhere, Category="山河纪元|赌博")
    int32 MaxDebt = 10000; // 最大负债
};
