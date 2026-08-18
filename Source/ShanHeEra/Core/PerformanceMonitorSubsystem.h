#pragma once
#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "PerformanceMonitorSubsystem.generated.h"

/**
 * 性能监控子系统 - 基于 ue5-performance-packaging 方法论
 * 实时监控帧率、内存、Actor数量，自动调整画质
 * 支持性能基准测试和热点分析
 */
UCLASS()
class SHANHEERA_API UPerformanceMonitorSubsystem : public UWorldSubsystem
{
    GENERATED_BODY()

public:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void OnWorldBeginPlay(UWorld& InWorld) override;
    virtual void Tick(float DeltaTime) override;
    virtual bool DoesSupportWorldType(const EWorldType::Type WorldType) const override;

    // === 性能指标 ===

    /** 获取当前FPS */
    UFUNCTION(BlueprintPure, Category="山河纪元|性能")
    float GetCurrentFPS() const { return CurrentFPS; }

    /** 获取平均FPS（最近60帧） */
    UFUNCTION(BlueprintPure, Category="山河纪元|性能")
    float GetAverageFPS() const { return AverageFPS; }

    /** 获取帧时间(ms) */
    UFUNCTION(BlueprintPure, Category="山河纪元|性能")
    float GetFrameTimeMs() const { return CurrentFrameTime * 1000.0f; }

    /** 获取内存使用量(MB) */
    UFUNCTION(BlueprintPure, Category="山河纪元|性能")
    float GetMemoryUsageMB() const;

    /** 获取当前Actor数量 */
    UFUNCTION(BlueprintPure, Category="山河纪元|性能")
    int32 GetActorCount() const;

    // === 自动调整 ===

    /** 启用/禁用自动画质调整 */
    UFUNCTION(BlueprintCallable, Category="山河纪元|性能")
    void SetAutoAdjustEnabled(bool bEnabled) { bAutoAdjust = bEnabled; }

    /** 设置目标FPS */
    UFUNCTION(BlueprintCallable, Category="山河纪元|性能")
    void SetTargetFPS(float NewTarget) { TargetFPS = NewTarget; }

    /** 获取性能等级 (0=最低, 3=最高) */
    UFUNCTION(BlueprintPure, Category="山河纪元|性能")
    int32 GetPerformanceLevel() const { return PerformanceLevel; }

    // === 基准测试 ===

    /** 开始性能基准测试 */
    UFUNCTION(BlueprintCallable, Category="山河纪元|性能")
    void StartBenchmark(float DurationSeconds = 10.0f);

    /** 结束基准测试并返回结果 */
    UFUNCTION(BlueprintCallable, Category="山河纪元|性能")
    FString EndBenchmark();

    /** 是否正在基准测试 */
    UFUNCTION(BlueprintPure, Category="山河纪元|性能")
    bool IsBenchmarkRunning() const { return bBenchmarkRunning; }

    // === 性能事件标记 ===

    /** 开始一个CPU性能事件（用于分析热点） */
    UFUNCTION(BlueprintCallable, Category="山河纪元|性能")
    void BeginPerformanceEvent(const FString& EventName);

    /** 结束CPU性能事件 */
    UFUNCTION(BlueprintCallable, Category="山河纪元|性能")
    void EndPerformanceEvent(const FString& EventName);

    /** 获取性能报告 */
    UFUNCTION(BlueprintCallable, Category="山河纪元|性能")
    FString GetPerformanceReport() const;

private:
    // 帧率统计
    float CurrentFPS = 60.0f;
    float AverageFPS = 60.0f;
    float CurrentFrameTime = 0.016f;
    TArray<float> FrameTimeHistory;
    static const int32 MaxHistoryFrames = 60;

    // 自动调整
    bool bAutoAdjust = true;
    float TargetFPS = 60.0f;
    int32 PerformanceLevel = 3; // 0=最低, 1=低, 2=中, 3=高
    float AdjustCooldown = 0.0f;

    // 基准测试
    bool bBenchmarkRunning = false;
    float BenchmarkTime = 0.0f;
    float BenchmarkMinFPS = 9999.0f;
    float BenchmarkMaxFPS = 0.0f;
    float BenchmarkTotalFPS = 0.0f;
    int32 BenchmarkFrameCount = 0;

    // 性能事件
    TMap<FString, double> EventStartTimes;
    TMap<FString, float> EventTotalTimes;
    TMap<FString, int32> EventCallCounts;

    void UpdateFPS(float DeltaTime);
    void AutoAdjustQuality(float DeltaTime);
    void ApplyPerformanceLevel(int32 Level);
};
