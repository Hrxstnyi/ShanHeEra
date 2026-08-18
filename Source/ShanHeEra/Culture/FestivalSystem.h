#pragma once
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ShanHeFestivalSystem.generated.h"

UENUM(BlueprintType)
enum class EFestivalType : uint8
{
    SpringFestival  UMETA(DisplayName="春节"),
    Lantern         UMETA(DisplayName="元宵"),
    Qingming        UMETA(DisplayName="清明"),
    DragonBoat      UMETA(DisplayName="端午"),
    Qixi            UMETA(DisplayName="七夕"),
    MidAutumn        UMETA(DisplayName="中秋"),
    DoubleNinth     UMETA(DisplayName="重阳"),
    WinterSolstice  UMETA(DisplayName="冬至"),
    Laba            UMETA(DisplayName="腊八")
};

USTRUCT(BlueprintType)
struct FFestivalData
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite) EFestivalType Type;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FText Name;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 LunarMonth = 1;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 LunarDay = 1;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FText Description;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FText Customs;      // 习俗
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FText SpecialFood;  // 特色食物
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 HappinessBonus = 10;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) bool bHasSpecialEvent = false;
};

/**
 * 节日系统 - 传统节日活动、限定剧情、特殊奖励
 * 增强文化沉浸感
 */
UCLASS()
class SHANHEERA_API UFestivalSystem : public UObject
{
    GENERATED_BODY()
public:
    UFUNCTION(BlueprintCallable, Category="山河纪元|节日")
    void InitializeFestivals();

    UFUNCTION(BlueprintCallable, Category="山河纪元|节日")
    FFestivalData GetFestivalForDate(int32 LunarMonth, int32 LunarDay) const;

    UFUNCTION(BlueprintCallable, Category="山河纪元|节日")
    bool IsFestivalDay(int32 LunarMonth, int32 LunarDay) const;

    UFUNCTION(BlueprintCallable, Category="山河纪元|节日")
    void TriggerFestivalEvent(EFestivalType Type);

    UFUNCTION(BlueprintCallable, Category="山河纪元|节日")
    TArray<FFestivalData> GetAllFestivals() const { return Festivals; }

    UPROPERTY(BlueprintReadOnly, Category="山河纪元|节日")
    TArray<FFestivalData> Festivals;

    UPROPERTY(BlueprintReadOnly, Category="山河纪元|节日")
    EFestivalType CurrentFestival;

    UPROPERTY(BlueprintReadOnly, Category="山河纪元|节日")
    bool bIsFestivalActive = false;
};
