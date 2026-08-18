#pragma once
#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "ShanHeAchievementSystem.generated.h"

UENUM(BlueprintType)
enum class EAchievementCategory : uint8
{
    Life        UMETA(DisplayName="人生"),
    Official    UMETA(DisplayName="官场"),
    Commerce    UMETA(DisplayName="商业"),
    Military    UMETA(DisplayName="军事"),
    Culture     UMETA(DisplayName="文化"),
    Jianghu     UMETA(DisplayName="江湖"),
    Family      UMETA(DisplayName="家族"),
    Hidden      UMETA(DisplayName="隐藏"),
    Patriotic   UMETA(DisplayName="爱国彩蛋")
};

USTRUCT(BlueprintType)
struct FAchievement
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FName AchievementID;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FText Title;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FText Description;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) EAchievementCategory Category;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 Points = 10;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) bool bIsHidden = false;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) bool bIsUnlocked = false;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FText RewardDescription;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAchievementUnlocked, FAchievement, Achievement);

/**
 * 成就系统 - 记录玩家在游戏中的各种里程碑
 * 8大类：人生/官场/商业/军事/文化/江湖/家族/隐藏
 * 特别分类：爱国彩蛋成就
 */
UCLASS()
class SHANHEERA_API UAchievementSystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()
public:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;

    UFUNCTION(BlueprintCallable, Category="山河纪元|成就")
    void UnlockAchievement(FName AchievementID);

    UFUNCTION(BlueprintCallable, Category="山河纪元|成就")
    bool IsUnlocked(FName AchievementID) const;

    UFUNCTION(BlueprintCallable, Category="山河纪元|成就")
    TArray<FAchievement> GetAllAchievements() const { return Achievements; }

    UFUNCTION(BlueprintCallable, Category="山河纪元|成就")
    TArray<FAchievement> GetUnlockedAchievements() const;

    UFUNCTION(BlueprintCallable, Category="山河纪元|成就")
    int32 GetTotalPoints() const;

    UFUNCTION(BlueprintCallable, Category="山河纪元|成就")
    float GetCompletionRate() const;

    UPROPERTY(BlueprintAssignable, Category="山河纪元|成就")
    FOnAchievementUnlocked OnAchievementUnlocked;

    UPROPERTY(BlueprintReadOnly, Category="山河纪元|成就")
    TArray<FAchievement> Achievements;

private:
    void RegisterAchievements();
};
