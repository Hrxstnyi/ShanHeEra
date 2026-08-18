#pragma once
#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "ShanHeSaveGame.generated.h"

/**
 * 山河纪元存档对象 - 基于 ue5-save-load-replication 方法论
 * - 显式版本号，支持迁移
 * - 稳定ID而非索引顺序
 * - 分离持久化状态和运行时状态
 * - 支持异步存档/读档
 */

USTRUCT(BlueprintType)
struct FPlayerSaveData
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite) FString PlayerName;
    UPROPERTY(BlueprintReadWrite) FName CharacterID;
    UPROPERTY(BlueprintReadWrite) int32 Age = 16;
    UPROPERTY(BlueprintReadWrite) FName CurrentRegion;
    UPROPERTY(BlueprintReadWrite) FName CurrentCity;
    UPROPERTY(BlueprintReadWrite) TMap<FName, float> Attributes;
    UPROPERTY(BlueprintReadWrite) TMap<FName, int32> Skills;
    UPROPERTY(BlueprintReadWrite) TArray<FName> InventoryItemIDs;
    UPROPERTY(BlueprintReadWrite) TArray<int32> InventoryItemCounts;
    UPROPERTY(BlueprintReadWrite) int32 Gold = 0;
    UPROPERTY(BlueprintReadWrite) int32 Reputation = 0;
};

USTRUCT(BlueprintType)
struct FShanHeWorldSaveData
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite) int32 Year = 1;
    UPROPERTY(BlueprintReadWrite) int32 Month = 1;
    UPROPERTY(BlueprintReadWrite) int32 Day = 1;
    UPROPERTY(BlueprintReadWrite) int32 Hour = 6;
    UPROPERTY(BlueprintReadWrite) FName CurrentEra; // 三国/盛唐/弱宋/明末
    UPROPERTY(BlueprintReadWrite) TMap<FName, int32> CityPopulations;
    UPROPERTY(BlueprintReadWrite) TMap<FName, float> CityWealth;
    UPROPERTY(BlueprintReadWrite) TMap<FName, int32> FactionRelations;
    UPROPERTY(BlueprintReadWrite) TArray<FName> ActiveEvents;
};

USTRUCT(BlueprintType)
struct FFamilySaveData
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite) FName FamilyName;
    UPROPERTY(BlueprintReadWrite) int32 Generation = 1;
    UPROPERTY(BlueprintReadWrite) TArray<FName> MemberIDs;
    UPROPERTY(BlueprintReadWrite) TMap<FName, FName> ParentRelations;
    UPROPERTY(BlueprintReadWrite) int32 FamilyReputation = 0;
};

UCLASS()
class SHANHEERA_API UShanHeSaveGame : public USaveGame
{
    GENERATED_BODY()

public:
    // === 存档元数据 ===

    /** 存档版本号 - 用于迁移检测 */
    UPROPERTY(BlueprintReadOnly) int32 SaveVersion = 2;

    /** 游戏版本 */
    UPROPERTY(BlueprintReadOnly) FString GameVersion = TEXT("2.5.8");

    /** 存档创建时间 */
    UPROPERTY(BlueprintReadOnly) FString CreateTime;

    /** 最后保存时间 */
    UPROPERTY(BlueprintReadOnly) FString LastSaveTime;

    /** 游戏时长(秒) */
    UPROPERTY(BlueprintReadOnly) float PlayTimeSeconds = 0.0f;

    /** 存档槽位索引 */
    UPROPERTY(BlueprintReadOnly) int32 SlotIndex = 0;

    // === 游戏状态 ===

    UPROPERTY(BlueprintReadWrite) FPlayerSaveData PlayerData;
    UPROPERTY(BlueprintReadWrite) FShanHeWorldSaveData WorldData;
    UPROPERTY(BlueprintReadWrite) TArray<FFamilySaveData> Families;

    // === 扩展数据（Mod支持） ===

    UPROPERTY(BlueprintReadWrite) TMap<FName, FString> ModData;

    // === 方法 ===

    /** 检查存档版本是否需要迁移 */
    UFUNCTION(BlueprintPure, Category="山河纪元|存档")
    bool NeedsMigration() const { return SaveVersion < 2; }

    /** 执行存档迁移 */
    UFUNCTION(BlueprintCallable, Category="山河纪元|存档")
    void MigrateToLatest();

    /** 验证存档完整性 */
    UFUNCTION(BlueprintPure, Category="山河纪元|存档")
    bool IsValid() const;

    /** 获取存档摘要（用于存档列表显示） */
    UFUNCTION(BlueprintPure, Category="山河纪元|存档")
    FString GetSummary() const;
};
