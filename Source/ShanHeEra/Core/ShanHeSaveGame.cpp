#include "Core/ShanHeSaveGame.h"

void UShanHeSaveGame::MigrateToLatest()
{
    if (SaveVersion >= 2) return;

    UE_LOG(LogTemp, Log, TEXT("[SaveGame] 正在迁移存档从版本%d到版本2"), SaveVersion);

    // v1 -> v2 迁移：添加新字段
    if (SaveVersion < 2)
    {
        // 确保WorldData有CurrentEra字段
        if (WorldData.CurrentEra.IsNone())
        {
            WorldData.CurrentEra = TEXT("盛唐"); // 默认值
        }
    }

    SaveVersion = 2;
    UE_LOG(LogTemp, Log, TEXT("[SaveGame] 存档迁移完成"));
}

bool UShanHeSaveGame::IsValid() const
{
    // 基本验证
    if (SaveVersion <= 0) return false;
    if (PlayerData.CharacterID.IsNone()) return false;
    if (WorldData.Year <= 0) return false;

    return true;
}

FString UShanHeSaveGame::GetSummary() const
{
    return FString::Printf(
        TEXT("%s | %s | %d年%d月%d日 | 游戏时长: %.1f小时 | 版本: v%d"),
        *PlayerData.PlayerName,
        *WorldData.CurrentEra.ToString(),
        WorldData.Year, WorldData.Month, WorldData.Day,
        PlayTimeSeconds / 3600.0f,
        SaveVersion
    );
}
