#include "Core/PerformanceMonitorSubsystem.h"
#include "Engine/World.h"
#include "Engine/Engine.h"
#include "GameFramework/Actor.h"
#include "HAL/PlatformMemory.h"
#include "Misc/DateTime.h"

void UPerformanceMonitorSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
    FrameTimeHistory.Reserve(MaxHistoryFrames);
    UE_LOG(LogTemp, Log, TEXT("[PerformanceMonitor] 子系统初始化"));
}

void UPerformanceMonitorSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
    Super::OnWorldBeginPlay(InWorld);
}

void UPerformanceMonitorSubsystem::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    UpdateFPS(DeltaTime);

    if (bAutoAdjust)
    {
        AutoAdjustQuality(DeltaTime);
    }

    if (bBenchmarkRunning)
    {
        BenchmarkTime += DeltaTime;
        BenchmarkFrameCount++;
        BenchmarkTotalFPS += CurrentFPS;
        BenchmarkMinFPS = FMath::Min(BenchmarkMinFPS, CurrentFPS);
        BenchmarkMaxFPS = FMath::Max(BenchmarkMaxFPS, CurrentFPS);

        if (BenchmarkTime >= 10.0f) // 默认10秒
        {
            UE_LOG(LogTemp, Log, TEXT("[PerformanceMonitor] 基准测试自动结束"));
        }
    }
}

bool UPerformanceMonitorSubsystem::DoesSupportWorldType(const EWorldType::Type WorldType) const
{
    return WorldType == EWorldType::Game || WorldType == EWorldType::PIE;
}

void UPerformanceMonitorSubsystem::UpdateFPS(float DeltaTime)
{
    CurrentFrameTime = DeltaTime;
    CurrentFPS = (DeltaTime > 0.0f) ? (1.0f / DeltaTime) : 0.0f;

    FrameTimeHistory.Add(DeltaTime);
    if (FrameTimeHistory.Num() > MaxHistoryFrames)
    {
        FrameTimeHistory.RemoveAt(0);
    }

    if (FrameTimeHistory.Num() > 0)
    {
        float AvgFrameTime = 0.0f;
        for (float FT : FrameTimeHistory)
        {
            AvgFrameTime += FT;
        }
        AvgFrameTime /= FrameTimeHistory.Num();
        AverageFPS = (AvgFrameTime > 0.0f) ? (1.0f / AvgFrameTime) : 0.0f;
    }
}

void UPerformanceMonitorSubsystem::AutoAdjustQuality(float DeltaTime)
{
    AdjustCooldown -= DeltaTime;
    if (AdjustCooldown > 0.0f) return;

    // 连续低帧率 -> 降低画质
    if (AverageFPS < TargetFPS * 0.7f && PerformanceLevel > 0)
    {
        PerformanceLevel--;
        ApplyPerformanceLevel(PerformanceLevel);
        AdjustCooldown = 5.0f; // 5秒内不再调整
        UE_LOG(LogTemp, Log, TEXT("[PerformanceMonitor] 帧率过低(%.1f), 降低画质到等级%d"), AverageFPS, PerformanceLevel);
    }
    // 连续高帧率 -> 提升画质
    else if (AverageFPS > TargetFPS * 1.2f && PerformanceLevel < 3)
    {
        PerformanceLevel++;
        ApplyPerformanceLevel(PerformanceLevel);
        AdjustCooldown = 5.0f;
        UE_LOG(LogTemp, Log, TEXT("[PerformanceMonitor] 帧率充足(%.1f), 提升画质到等级%d"), AverageFPS, PerformanceLevel);
    }
}

void UPerformanceMonitorSubsystem::ApplyPerformanceLevel(int32 Level)
{
    if (!GEngine) return;

    // 这里可以调用 UGameUserSettings 来调整实际画质
    // Level 0: 最低, 1: 低, 2: 中, 3: 高
    switch (Level)
    {
    case 0:
        // 最低画质：关闭后处理，降低分辨率
        break;
    case 1:
        // 低画质
        break;
    case 2:
        // 中等画质
        break;
    case 3:
        // 高画质
        break;
    }
}

float UPerformanceMonitorSubsystem::GetMemoryUsageMB() const
{
    FPlatformMemoryStats Stats = FPlatformMemory::GetStats();
    return (Stats.UsedPhysical / 1024.0f / 1024.0f);
}

int32 UPerformanceMonitorSubsystem::GetActorCount() const
{
    UWorld* World = GetWorld();
    if (!World) return 0;
    return World->GetCurrentLevel()->Actors.Num();
}

void UPerformanceMonitorSubsystem::StartBenchmark(float DurationSeconds)
{
    bBenchmarkRunning = true;
    BenchmarkTime = 0.0f;
    BenchmarkMinFPS = 9999.0f;
    BenchmarkMaxFPS = 0.0f;
    BenchmarkTotalFPS = 0.0f;
    BenchmarkFrameCount = 0;
    UE_LOG(LogTemp, Log, TEXT("[PerformanceMonitor] 基准测试开始，时长%.1f秒"), DurationSeconds);
}

FString UPerformanceMonitorSubsystem::EndBenchmark()
{
    bBenchmarkRunning = false;

    float AvgFPS = (BenchmarkFrameCount > 0) ? (BenchmarkTotalFPS / BenchmarkFrameCount) : 0.0f;

    FString Result = FString::Printf(
        TEXT("=== 性能基准测试结果 ===\n")
        TEXT("测试时长: %.1f秒\n")
        TEXT("总帧数: %d\n")
        TEXT("平均FPS: %.1f\n")
        TEXT("最低FPS: %.1f\n")
        TEXT("最高FPS: %.1f\n")
        TEXT("内存使用: %.1f MB\n")
        TEXT("Actor数量: %d"),
        BenchmarkTime, BenchmarkFrameCount, AvgFPS,
        BenchmarkMinFPS, BenchmarkMaxFPS,
        GetMemoryUsageMB(), GetActorCount()
    );

    UE_LOG(LogTemp, Log, TEXT("%s"), *Result);
    return Result;
}

void UPerformanceMonitorSubsystem::BeginPerformanceEvent(const FString& EventName)
{
    EventStartTimes.Add(EventName, FPlatformTime::Seconds());
}

void UPerformanceMonitorSubsystem::EndPerformanceEvent(const FString& EventName)
{
    double* StartTime = EventStartTimes.Find(EventName);
    if (!StartTime) return;

    double Elapsed = FPlatformTime::Seconds() - *StartTime;
    float& Total = EventTotalTimes.FindOrAdd(EventName);
    Total += (float)Elapsed;
    int32& Count = EventCallCounts.FindOrAdd(EventName);
    Count++;

    EventStartTimes.Remove(EventName);
}

FString UPerformanceMonitorSubsystem::GetPerformanceReport() const
{
    FString Report = FString::Printf(
        TEXT("=== 性能报告 ===\n")
        TEXT("当前FPS: %.1f\n")
        TEXT("平均FPS: %.1f\n")
        TEXT("帧时间: %.2f ms\n")
        TEXT("内存: %.1f MB\n")
        TEXT("Actor: %d\n")
        TEXT("性能等级: %d/3\n"),
        CurrentFPS, AverageFPS, CurrentFrameTime * 1000.0f,
        GetMemoryUsageMB(), GetActorCount(), PerformanceLevel
    );

    if (EventTotalTimes.Num() > 0)
    {
        Report += TEXT("\n--- 性能事件 ---\n");
        for (const auto& Pair : EventTotalTimes)
        {
            int32 Count = EventCallCounts.FindRef(Pair.Key);
            float Avg = (Count > 0) ? (Pair.Value / Count * 1000.0f) : 0.0f;
            Report += FString::Printf(TEXT("  %s: 总%.2fms, %d次, 平均%.3fms\n"),
                *Pair.Key, Pair.Value * 1000.0f, Count, Avg);
        }
    }

    return Report;
}
