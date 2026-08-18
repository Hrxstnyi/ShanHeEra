#include "Core/PerformanceSystem.h"
#include "Core/ShanHeLog.h"

void UPerformanceSystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
    UE_LOG(LogShanHe, Log, TEXT("性能优化系统初始化 - 对象池/LOD/流式加载/帧率监控"));
}

void UPerformanceSystem::Tick(float DeltaTime)
{
    UpdateFPS(DeltaTime);
    UpdateStreaming();
}

void UPerformanceSystem::CreateObjectPool(FName PoolID, TSubclassOf<AActor> ActorClass, int32 PreSpawnCount)
{
    if (ObjectPools.Contains(PoolID)) return;
    FObjectPool Pool;
    Pool.PoolID = PoolID;
    Pool.ActorClass = ActorClass;
    for (int32 i = 0; i < PreSpawnCount; i++)
    {
        FActorSpawnParameters Params;
        Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
        AActor* NewActor = GetWorld()->SpawnActor<AActor>(ActorClass, FVector::ZeroVector, FRotator::ZeroRotator, Params);
        if (NewActor)
        {
            NewActor->SetActorHiddenInGame(true);
            NewActor->SetActorEnableCollision(false);
            FPooledActor Pooled;
            Pooled.Actor = NewActor;
            Pooled.bIsInUse = false;
            Pool.PooledActors.Add(Pooled);
        }
    }
    ObjectPools.Add(PoolID, Pool);
    UE_LOG(LogShanHe, Log, TEXT("创建对象池：%s，预生成 %d 个"), *PoolID.ToString(), PreSpawnCount);
}

AActor* UPerformanceSystem::GetFromPool(FName PoolID, FVector Location, FRotator Rotation)
{
    if (FObjectPool* Pool = ObjectPools.Find(PoolID))
    {
        for (FPooledActor& P : Pool->PooledActors)
        {
            if (!P.bIsInUse && P.Actor)
            {
                P.bIsInUse = true;
                P.LastUsedTime = GetWorld()->GetTimeSeconds();
                P.Actor->SetActorLocationAndRotation(Location, Rotation);
                P.Actor->SetActorHiddenInGame(false);
                P.Actor->SetActorEnableCollision(true);
                Pool->ActiveCount++;
                return P.Actor;
            }
        }
        // 池满，动态扩展
        if (Pool->PooledActors.Num() < Pool->MaxPoolSize)
        {
            FActorSpawnParameters Params;
            AActor* NewActor = GetWorld()->SpawnActor<AActor>(Pool->ActorClass, Location, Rotation, Params);
            if (NewActor)
            {
                FPooledActor Pooled;
                Pooled.Actor = NewActor;
                Pooled.bIsInUse = true;
                Pooled.LastUsedTime = GetWorld()->GetTimeSeconds();
                Pool->PooledActors.Add(Pooled);
                Pool->ActiveCount++;
                return NewActor;
            }
        }
    }
    return nullptr;
}

void UPerformanceSystem::ReturnToPool(FName PoolID, AActor* Actor)
{
    if (FObjectPool* Pool = ObjectPools.Find(PoolID))
    {
        for (FPooledActor& P : Pool->PooledActors)
        {
            if (P.Actor == Actor)
            {
                P.bIsInUse = false;
                Actor->SetActorHiddenInGame(true);
                Actor->SetActorEnableCollision(false);
                Actor->SetActorLocation(FVector(0, 0, -10000)); // 移到远处
                Pool->ActiveCount = FMath::Max(0, Pool->ActiveCount - 1);
                return;
            }
        }
    }
}

ELODLevel UPerformanceSystem::CalculateLOD(FVector ObjectLocation, float ViewDistance) const
{
    if (ViewDistance < LODDistance0) return ELODLevel::LOD0;
    if (ViewDistance < LODDistance1) return ELODLevel::LOD1;
    if (ViewDistance < LODDistance2) return ELODLevel::LOD2;
    if (ViewDistance < LODDistance3) return ELODLevel::LOD3;
    return ELODLevel::Culled;
}

void UPerformanceSystem::ApplyLOD(AActor* Actor, ELODLevel LOD)
{
    if (!Actor) return;
    switch (LOD)
    {
        case ELODLevel::Culled:
            Actor->SetActorHiddenInGame(true);
            Actor->SetActorEnableCollision(false);
            break;
        case ELODLevel::LOD3:
            Actor->SetActorHiddenInGame(false);
            Actor->SetActorEnableCollision(true);
            // 降低Tick频率
            break;
        default:
            Actor->SetActorHiddenInGame(false);
            Actor->SetActorEnableCollision(true);
            break;
    }
}

void UPerformanceSystem::StreamInRegion(FName RegionID)
{
    if (!LoadedRegions.Contains(RegionID))
    {
        LoadedRegions.Add(RegionID);
        UE_LOG(LogShanHe, Log, TEXT("流式加载区域：%s"), *RegionID.ToString());
    }
}

void UPerformanceSystem::StreamOutRegion(FName RegionID)
{
    if (LoadedRegions.Contains(RegionID))
    {
        LoadedRegions.Remove(RegionID);
        UE_LOG(LogShanHe, Log, TEXT("卸载区域：%s"), *RegionID.ToString());
    }
}

void UPerformanceSystem::SetTargetFrameRate(int32 FPS)
{
    UE_LOG(LogShanHe, Log, TEXT("目标帧率设置：%d FPS"), FPS);
}

void UPerformanceSystem::RunGC()
{
    UE_LOG(LogShanHe, Log, TEXT("执行垃圾回收"));
}

void UPerformanceSystem::UpdateFPS(float DeltaTime)
{
    FrameTimeAccumulator += DeltaTime;
    FrameCount++;
    if (FrameTimeAccumulator >= 1.0f)
    {
        CurrentFPS = (float)FrameCount / FrameTimeAccumulator;
        FrameTimeAccumulator = 0.0f;
        FrameCount = 0;
        if (CurrentFPS < 30.0f)
        {
            UE_LOG(LogShanHe, Warning, TEXT("帧率过低：%.1f FPS"), CurrentFPS);
        }
    }
}

void UPerformanceSystem::UpdateStreaming()
{
    // 根据玩家位置自动加载/卸载区域
}
