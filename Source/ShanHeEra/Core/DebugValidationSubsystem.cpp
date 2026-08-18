#include "Core/DebugValidationSubsystem.h"
#include "Core/ShanHeSaveGame.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"

DEFINE_LOG_CATEGORY(LogShanHe);

void UDebugValidationSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
    UE_LOG(LogShanHe, Log, TEXT("[DebugValidation] 子系统初始化"));
}

void UDebugValidationSubsystem::Deinitialize()
{
    UE_LOG(LogShanHe, Log, TEXT("[DebugValidation] 子系统关闭"));
    Super::Deinitialize();
}

void UDebugValidationSubsystem::LogInfo(const FString& System, const FString& Message)
{
    UE_LOG(LogShanHe, Log, TEXT("[%s] %s"), *System, *Message);
    AddValidationResult(EValidationLevel::Info, System, Message);
}

void UDebugValidationSubsystem::LogWarning(const FString& System, const FString& Message)
{
    UE_LOG(LogShanHe, Warning, TEXT("[%s] %s"), *System, *Message);
    AddValidationResult(EValidationLevel::Warning, System, Message);
}

void UDebugValidationSubsystem::LogError(const FString& System, const FString& Message)
{
    UE_LOG(LogShanHe, Error, TEXT("[%s] %s"), *System, *Message);
    AddValidationResult(EValidationLevel::Error, System, Message);
}

void UDebugValidationSubsystem::LogCritical(const FString& System, const FString& Message)
{
    UE_LOG(LogShanHe, Fatal, TEXT("[%s] %s"), *System, *Message);
    AddValidationResult(EValidationLevel::Critical, System, Message);
}

bool UDebugValidationSubsystem::ValidateAsset(const FString& AssetPath, const FString& Context)
{
    if (AssetPath.IsEmpty())
    {
        LogWarning(TEXT("AssetValidation"), FString::Printf(TEXT("空资产路径 [%s]"), *Context));
        return false;
    }

    // 简单验证：检查路径格式
    if (!AssetPath.Contains(TEXT("/")))
    {
        LogWarning(TEXT("AssetValidation"), FString::Printf(TEXT("无效资产路径格式: %s [%s]"), *AssetPath, *Context));
        return false;
    }

    return true;
}

bool UDebugValidationSubsystem::ValidateWorldState()
{
    UWorld* World = GetWorld();
    if (!World)
    {
        LogError(TEXT("WorldValidation"), TEXT("World为空"));
        return false;
    }

    int32 ActorCount = World->GetCurrentLevel()->Actors.Num();
    if (ActorCount == 0)
    {
        LogWarning(TEXT("WorldValidation"), TEXT("关卡中没有Actor"));
    }

    LogInfo(TEXT("WorldValidation"), FString::Printf(TEXT("世界状态正常: %d个Actor"), ActorCount));
    return true;
}

bool UDebugValidationSubsystem::ValidateSaveGame(UShanHeSaveGame* SaveData)
{
    if (!SaveData)
    {
        LogError(TEXT("SaveValidation"), TEXT("存档对象为空"));
        return false;
    }

    if (!SaveData->IsValid())
    {
        LogError(TEXT("SaveValidation"), TEXT("存档数据无效"));
        return false;
    }

    if (SaveData->NeedsMigration())
    {
        LogWarning(TEXT("SaveValidation"), FString::Printf(TEXT("存档需要迁移: 版本%d"), SaveData->SaveVersion));
    }

    LogInfo(TEXT("SaveValidation"), FString::Printf(TEXT("存档验证通过: %s"), *SaveData->GetSummary()));
    return true;
}

void UDebugValidationSubsystem::BeginPerformanceScope(const FString& ScopeName)
{
    PerformanceScopes.Add(ScopeName, FPlatformTime::Seconds());
}

void UDebugValidationSubsystem::EndPerformanceScope(const FString& ScopeName)
{
    double* StartTime = PerformanceScopes.Find(ScopeName);
    if (!StartTime) return;

    double Elapsed = FPlatformTime::Seconds() - *StartTime;
    float& Total = PerformanceTotals.FindOrAdd(ScopeName);
    Total += (float)Elapsed;
    int32& Count = PerformanceCounts.FindOrAdd(ScopeName);
    Count++;

    PerformanceScopes.Remove(ScopeName);

    if (bDebugMode)
    {
        UE_LOG(LogShanHe, Log, TEXT("[Perf] %s: %.3fms (avg %.3fms, %d次)"),
            *ScopeName, Elapsed * 1000.0f,
            Count > 0 ? Total / Count * 1000.0f : 0.0f, Count);
    }
}

FString UDebugValidationSubsystem::GetValidationReport() const
{
    int32 InfoCount = 0, WarningCount = 0, ErrorCount = 0, CriticalCount = 0;
    for (const FValidationResult& Result : ValidationHistory)
    {
        switch (Result.Level)
        {
        case EValidationLevel::Info: InfoCount++; break;
        case EValidationLevel::Warning: WarningCount++; break;
        case EValidationLevel::Error: ErrorCount++; break;
        case EValidationLevel::Critical: CriticalCount++; break;
        }
    }

    FString Report = FString::Printf(
        TEXT("=== 验证报告 ===\n")
        TEXT("总记录: %d\n")
        TEXT("信息: %d, 警告: %d, 错误: %d, 关键: %d\n"),
        ValidationHistory.Num(), InfoCount, WarningCount, ErrorCount, CriticalCount
    );

    if (PerformanceTotals.Num() > 0)
    {
        Report += TEXT("\n--- 性能统计 ---\n");
        for (const auto& Pair : PerformanceTotals)
        {
            int32 Count = PerformanceCounts.FindRef(Pair.Key);
            float Avg = Count > 0 ? Pair.Value / Count * 1000.0f : 0.0f;
            Report += FString::Printf(TEXT("  %s: 总%.2fms, %d次, 平均%.3fms\n"),
                *Pair.Key, Pair.Value * 1000.0f, Count, Avg);
        }
    }

    return Report;
}

void UDebugValidationSubsystem::ExportLogsToFile(const FString& Filename)
{
    FString FullPath = FPaths::ProjectSavedDir() / Filename;
    FString Content = GetValidationReport();

    Content += TEXT("\n--- 详细日志 ---\n");
    for (const FValidationResult& Result : ValidationHistory)
    {
        Content += FString::Printf(TEXT("[%s] [%s] %s: %s\n"),
            *Result.Timestamp.ToString(),
            *Result.System,
            *UEnum::GetValueAsString(Result.Level),
            *Result.Message);
    }

    FFileHelper::SaveStringToFile(Content, *FullPath);
    UE_LOG(LogShanHe, Log, TEXT("[DebugValidation] 日志已导出到: %s"), *FullPath);
}

void UDebugValidationSubsystem::ScreenMessage(const FString& Message, FColor Color, float Duration)
{
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, Duration, Color, Message);
    }
}

void UDebugValidationSubsystem::AddValidationResult(EValidationLevel Level, const FString& System, const FString& Message)
{
    FValidationResult Result;
    Result.Level = Level;
    Result.System = System;
    Result.Message = Message;
    Result.Timestamp = FDateTime::Now();
    ValidationHistory.Add(Result);

    // 限制历史记录数量
    if (ValidationHistory.Num() > 1000)
    {
        ValidationHistory.RemoveAt(0, ValidationHistory.Num() - 1000);
    }
}
