#include "Core/DataConfigSystem.h"
#include "Core/ShanHeLog.h"

void UDataConfigSystem::LoadAllConfigs()
{
    // 从默认路径加载所有配置表
    LoadItemConfig(TEXT("/Game/Config/Items"));
    LoadNPCConfig(TEXT("/Game/Config/NPCs"));
    LoadRegionConfig(TEXT("/Game/Config/Regions"));
    bConfigsLoaded = true;
    UE_LOG(LogShanHe, Log, TEXT("数据驱动配置系统 - 所有配置表加载完成"));
}

bool UDataConfigSystem::LoadItemConfig(const FString& TablePath)
{
    // 实际项目中用LoadObject<UDataTable>加载
    UE_LOG(LogShanHe, Log, TEXT("加载物品配置表：%s"), *TablePath);
    return true;
}

bool UDataConfigSystem::LoadNPCConfig(const FString& TablePath)
{
    UE_LOG(LogShanHe, Log, TEXT("加载NPC配置表：%s"), *TablePath);
    return true;
}

bool UDataConfigSystem::LoadRegionConfig(const FString& TablePath)
{
    UE_LOG(LogShanHe, Log, TEXT("加载区域配置表：%s"), *TablePath);
    return true;
}

FItemConfigRow UDataConfigSystem::GetItemConfig(FName ItemID) const
{
    if (ItemConfigTable)
    {
        static const FString Context = TEXT("ItemConfig");
        if (FItemConfigRow* Row = ItemConfigTable->FindRow<FItemConfigRow>(ItemID, Context))
            return *Row;
    }
    return FItemConfigRow();
}

FNPCConfigRow UDataConfigSystem::GetNPCConfig(FName NPCID) const
{
    if (NPCConfigTable)
    {
        static const FString Context = TEXT("NPCConfig");
        if (FNPCConfigRow* Row = NPCConfigTable->FindRow<FNPCConfigRow>(NPCID, Context))
            return *Row;
    }
    return FNPCConfigRow();
}

FRegionConfigRow UDataConfigSystem::GetRegionConfig(FName RegionID) const
{
    if (RegionConfigTable)
    {
        static const FString Context = TEXT("RegionConfig");
        if (FRegionConfigRow* Row = RegionConfigTable->FindRow<FRegionConfigRow>(RegionID, Context))
            return *Row;
    }
    return FRegionConfigRow();
}

TArray<FItemConfigRow> UDataConfigSystem::GetAllItems() const
{
    TArray<FItemConfigRow> Result;
    if (ItemConfigTable)
    {
        static const FString Context = TEXT("AllItems");
        TArray<FItemConfigRow*> Rows;
        ItemConfigTable->GetAllRows<FItemConfigRow>(Context, Rows);
        for (FItemConfigRow* Row : Rows)
        {
            Result.Add(*Row);
        }
    }
    return Result;
}

bool UDataConfigSystem::ExportConfigToJSON(const FString& ExportPath)
{
    UE_LOG(LogShanHe, Log, TEXT("导出配置到JSON：%s"), *ExportPath);
    return true;
}

bool UDataConfigSystem::ImportConfigFromJSON(const FString& ImportPath)
{
    UE_LOG(LogShanHe, Log, TEXT("从JSON导入配置：%s"), *ImportPath);
    bConfigsLoaded = false;
    LoadAllConfigs();
    return true;
}
