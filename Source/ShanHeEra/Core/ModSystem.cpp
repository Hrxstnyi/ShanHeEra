#include "Core/ModSystem.h"
#include "Core/ShanHeLog.h"

void UModSystem::ScanMods()
{
    // 扫描Mod目录下的mod.json配置文件
    UE_LOG(LogShanHe, Log, TEXT("Mod系统 - 扫描Mod目录"));
}

bool UModSystem::EnableMod(FName ModID)
{
    for (FModInfo& M : InstalledMods)
    {
        if (M.ModID == ModID)
        {
            M.bIsEnabled = true;
            UE_LOG(LogShanHe, Log, TEXT("启用Mod：%s"), *M.ModName.ToString());
            return true;
        }
    }
    return false;
}

void UModSystem::DisableMod(FName ModID)
{
    for (FModInfo& M : InstalledMods)
    {
        if (M.ModID == ModID)
        {
            M.bIsEnabled = false;
            M.bIsLoaded = false;
            UE_LOG(LogShanHe, Log, TEXT("禁用Mod：%s"), *M.ModName.ToString());
            return;
        }
    }
}

void UModSystem::LoadAllEnabledMods()
{
    // 按LoadOrder排序后加载
    InstalledMods.Sort([](const FModInfo& A, const FModInfo& B) {
        return A.LoadOrder < B.LoadOrder;
    });
    for (FModInfo& M : InstalledMods)
    {
        if (M.bIsEnabled && !M.bIsLoaded)
        {
            M.bIsLoaded = true;
            UE_LOG(LogShanHe, Log, TEXT("加载Mod：%s v%s"), *M.ModName.ToString(), *M.Version.ToString());
        }
    }
}

void UModSystem::UnloadAllMods()
{
    for (FModInfo& M : InstalledMods)
        M.bIsLoaded = false;
    UE_LOG(LogShanHe, Log, TEXT("卸载所有Mod"));
}

bool UModSystem::ExecuteModScript(FName ModID, const FString& FunctionName)
{
    for (const FModInfo& M : InstalledMods)
    {
        if (M.ModID == ModID && M.bIsLoaded)
        {
            UE_LOG(LogShanHe, Log, TEXT("执行Mod脚本：%s:%s"), *ModID.ToString(), *FunctionName);
            return true;
        }
    }
    return false;
}
