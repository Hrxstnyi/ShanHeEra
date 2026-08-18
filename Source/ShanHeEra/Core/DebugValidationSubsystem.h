#pragma once
#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Core/ShanHeLog.h"
#include "DebugValidationSubsystem.generated.h"

class UWorld;

/**
 * 调试验证子系统 - 基于 ue5-debug-validation 方法论
 * 提供结构化日志、运行时验证、资产检查、性能标记
 */

UENUM(BlueprintType)
enum class EValidationLevel : uint8
{
    Info,
    Warning,
    Error,
    Critical
};

USTRUCT(BlueprintType)
struct FValidationResult
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly) EValidationLevel Level = EValidationLevel::Info;
    UPROPERTY(BlueprintReadOnly) FString System;
    UPROPERTY(BlueprintReadOnly) FString Message;
    UPROPERTY(BlueprintReadOnly) FString Context;
    UPROPERTY(BlueprintReadOnly) FDateTime Timestamp;
};

UCLASS()
class SHANHEERA_API UDebugValidationSubsystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;

    // === 结构化日志 ===

    /** 记录信息日志 */
    UFUNCTION(BlueprintCallable, Category="山河纪元|调试")
    void LogInfo(const FString& System, const FString& Message);

    /** 记录警告日志 */
    UFUNCTION(BlueprintCallable, Category="山河纪元|调试")
    void LogWarning(const FString& System, const FString& Message);

    /** 记录错误日志 */
    UFUNCTION(BlueprintCallable, Category="山河纪元|调试")
    void LogError(const FString& System, const FString& Message);

    /** 记录关键错误 */
    UFUNCTION(BlueprintCallable, Category="山河纪元|调试")
    void LogCritical(const FString& System, const FString& Message);

    // === 运行时验证 ===

    /** 验证资产是否存在 */
    UFUNCTION(BlueprintCallable, Category="山河纪元|调试")
    bool ValidateAsset(const FString& AssetPath, const FString& Context = TEXT(""));

    /** 验证世界状态一致性 */
    UFUNCTION(BlueprintCallable, Category="山河纪元|调试")
    bool ValidateWorldState();

    /** 验证存档完整性 */
    UFUNCTION(BlueprintCallable, Category="山河纪元|调试")
    bool ValidateSaveGame(class UShanHeSaveGame* SaveData);

    // === 性能标记 ===

    /** 开始性能标记 */
    UFUNCTION(BlueprintCallable, Category="山河纪元|调试")
    void BeginPerformanceScope(const FString& ScopeName);

    /** 结束性能标记 */
    UFUNCTION(BlueprintCallable, Category="山河纪元|调试")
    void EndPerformanceScope(const FString& ScopeName);

    // === 调试工具 ===

    /** 切换调试模式 */
    UFUNCTION(BlueprintCallable, Category="山河纪元|调试")
    void SetDebugMode(bool bEnabled) { bDebugMode = bEnabled; }

    /** 是否调试模式 */
    UFUNCTION(BlueprintPure, Category="山河纪元|调试")
    bool IsDebugMode() const { return bDebugMode; }

    /** 获取验证报告 */
    UFUNCTION(BlueprintCallable, Category="山河纪元|调试")
    FString GetValidationReport() const;

    /** 导出日志到文件 */
    UFUNCTION(BlueprintCallable, Category="山河纪元|调试")
    void ExportLogsToFile(const FString& Filename);

    /** 屏幕调试消息 */
    UFUNCTION(BlueprintCallable, Category="山河纪元|调试")
    void ScreenMessage(const FString& Message, FColor Color = FColor::White, float Duration = 5.0f);

private:
    bool bDebugMode = false;
    TArray<FValidationResult> ValidationHistory;
    TMap<FString, double> PerformanceScopes;
    TMap<FString, float> PerformanceTotals;
    TMap<FString, int32> PerformanceCounts;

    void AddValidationResult(EValidationLevel Level, const FString& System, const FString& Message);
};
