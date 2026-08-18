#pragma once
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ShanHeCodexSystem.generated.h"

UENUM(BlueprintType)
enum class ECodexCategory : uint8
{
    Item        UMETA(DisplayName="物品图鉴"),
    Character   UMETA(DisplayName="人物图鉴"),
    Location    UMETA(DisplayName="地点图鉴"),
    MartialArt  UMETA(DisplayName="武功图鉴"),
    Beast       UMETA(DisplayName="异兽图鉴"),
    Plant       UMETA(DisplayName="草木图鉴"),
    History     UMETA(DisplayName="历史图鉴")
};

USTRUCT(BlueprintType)
struct FCodexEntry
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FName EntryID;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) ECodexCategory Category;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FText Name;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FText Description;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) bool bDiscovered = false;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FDateTime DiscoveredDate;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 Rarity = 1; // 1-5
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCodexUnlocked, FCodexEntry, Entry);

/**
 * 图鉴收藏系统 - 物品/人物/地点/武功/异兽图鉴，收集解锁奖励
 */
UCLASS()
class SHANHEERA_API UCodexSystem : public UObject
{
    GENERATED_BODY()
public:
    UFUNCTION(BlueprintCallable, Category="山河纪元|图鉴")
    void InitializeCodex();

    UFUNCTION(BlueprintCallable, Category="山河纪元|图鉴")
    bool UnlockEntry(FName EntryID);

    UFUNCTION(BlueprintCallable, Category="山河纪元|图鉴")
    TArray<FCodexEntry> GetEntriesByCategory(ECodexCategory Category) const;

    UFUNCTION(BlueprintCallable, Category="山河纪元|图鉴")
    int32 GetDiscoveryProgress(ECodexCategory Category) const;

    UFUNCTION(BlueprintCallable, Category="山河纪元|图鉴")
    int32 GetTotalProgress() const;

    UFUNCTION(BlueprintCallable, Category="山河纪元|图鉴")
    bool CheckCollectionReward(); // 检查是否达成收集奖励

    UPROPERTY(BlueprintAssignable, Category="山河纪元|图鉴")
    FOnCodexUnlocked OnEntryUnlocked;

    UPROPERTY(BlueprintReadOnly, Category="山河纪元|图鉴")
    TMap<FName, FCodexEntry> CodexDatabase;

    UPROPERTY(BlueprintReadOnly, Category="山河纪元|图鉴")
    int32 TotalEntries = 0;

    UPROPERTY(BlueprintReadOnly, Category="山河纪元|图鉴")
    int32 DiscoveredEntries = 0;
};
