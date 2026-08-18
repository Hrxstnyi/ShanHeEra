#include "Core/SaveSystem.h"
#include "Core/ShanHeLog.h"
#include "HAL/FileManager.h"
#include "Misc/Paths.h"

void USaveSystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
    UE_LOG(LogShanHe, Log, TEXT("存档系统初始化 - 二进制序列化"));
}

FString USaveSystem::GetSaveDirectory() const
{
    return FPaths::ProjectSavedDir() + TEXT("SaveGames/");
}

FString USaveSystem::GetSaveFilePath(const FString& SlotName) const
{
    return GetSaveDirectory() + SlotName + TEXT(".shsave");
}

bool USaveSystem::SaveGame(const FString& SlotName, const FWorldSaveData& Data)
{
    FString Dir = GetSaveDirectory();
    IFileManager::Get().MakeDirectory(*Dir, true);

    FString FilePath = GetSaveFilePath(SlotName);
    TArray<uint8> Buffer;
    FMemoryWriter Writer(Buffer);

    FWorldSaveData MutableData = Data;
    MutableData.SaveTime = FDateTime::Now();

    if (!SerializeData(Writer, MutableData))
    {
        UE_LOG(LogShanHe, Error, TEXT("存档序列化失败: %s"), *SlotName);
        return false;
    }

    if (!FFileHelper::SaveArrayToFile(Buffer, *FilePath))
    {
        UE_LOG(LogShanHe, Error, TEXT("存档写入失败: %s"), *FilePath);
        return false;
    }

    UE_LOG(LogShanHe, Log, TEXT("存档成功: %s (%d bytes)"), *SlotName, Buffer.Num());
    return true;
}

bool USaveSystem::LoadGame(const FString& SlotName, FWorldSaveData& OutData)
{
    FString FilePath = GetSaveFilePath(SlotName);
    TArray<uint8> Buffer;

    if (!FFileHelper::LoadFileToArray(Buffer, *FilePath))
    {
        UE_LOG(LogShanHe, Error, TEXT("读档失败: %s"), *FilePath);
        return false;
    }

    FMemoryReader Reader(Buffer);
    if (!SerializeData(Reader, OutData))
    {
        UE_LOG(LogShanHe, Error, TEXT("读档反序列化失败: %s"), *SlotName);
        return false;
    }

    UE_LOG(LogShanHe, Log, TEXT("读档成功: %s"), *SlotName);
    return true;
}

bool USaveSystem::DeleteSave(const FString& SlotName)
{
    FString FilePath = GetSaveFilePath(SlotName);
    return IFileManager::Get().Delete(*FilePath);
}

TArray<FString> USaveSystem::GetAllSaveSlots() const
{
    TArray<FString> Slots;
    TArray<FString> Files;
    IFileManager::Get().FindFiles(Files, *(GetSaveDirectory() + TEXT("*.shsave")), true, false);
    for (const FString& File : Files)
    {
        Slots.Add(FPaths::GetBaseFilename(File));
    }
    return Slots;
}

void USaveSystem::QuickSave()
{
    FWorldSaveData Data;
    // 从GameState和各子系统收集数据...
    SaveGame(TEXT("QuickSave"), Data);
}

void USaveSystem::QuickLoad()
{
    FWorldSaveData Data;
    if (LoadGame(TEXT("QuickSave"), Data))
    {
        // 恢复世界状态...
    }
}

bool USaveSystem::SerializeData(FArchive& Ar, FWorldSaveData& Data)
{
    Ar << Data.SaveName;
    Ar << Data.SaveTime;
    Ar << Data.PlayTimeSeconds;
    Ar << Data.Version;
    Ar << Data.CurrentDate.Year;
    Ar << Data.CurrentDate.Month;
    Ar << Data.CurrentDate.Day;
    Ar << Data.TotalPopulation;
    Ar << Data.WorldStability;
    Ar << Data.TotalSimulatedDays;
    Ar << Data.NationalTreasury;
    Ar << Data.InflationRate;
    Ar << Data.ImperialAuthority;
    Ar << Data.CourtStability;
    Ar << Data.ReignYears;
    Ar << Data.PlayerAge;
    Ar << Data.PlayerLocation;
    // ... 更多字段
    return true;
}
