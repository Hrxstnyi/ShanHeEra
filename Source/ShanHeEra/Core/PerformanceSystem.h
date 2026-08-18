#pragma once
#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "ShanHePerformanceSystem.generated.h"

UENUM(BlueprintType)
enum class ELODLevel : uint8
{
    LOD0    UMETA(DisplayName="最高精度"),
    LOD1    UMETA(DisplayName="高精度"),
    LOD2    UMETA(DisplayName="中精度"),
    LOD3    UMETA(DisplayName="低精度"),
    Culled  UMETA(DisplayName="剔除")
};

USTRUCT(BlueprintType)
struct FPooledActor
{
    GENERATED_BODY()
    UPROPERTY() AActor* Actor = nullptr;
    UPROPERTY() bool bIsInUse = false;
    UPROPERTY() float LastUsedTime = 0.0f;
};

USTRUCT(BlueprintType)
struct FObjectPool
{
    GENERATED_BODY()
    UPROPERTY() FName PoolID;
    UPROPERTY() TSubclassOf<AActor> ActorClass;
    UPROPERTY() TArray<FPooledActor> PooledActors;
    UPROPERTY() int32 MaxPoolSize = 50;
    UPROPERTY() int32 ActiveCount = 0;
};

/**
 * 性能优化系统 - 对象池、LOD、流式加载、帧率优化
 */
UCLASS()
class SHANHEERA_API UPerformanceSystem : public UWorldSubsystem
{
    GENERATED_BODY()
public:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Tick(float DeltaTime) override;

    // 对象池
    UFUNCTION(BlueprintCallable, Category="山河纪元|性能|对象池")
    void CreateObjectPool(FName PoolID, TSubclassOf<AActor> ActorClass, int32 PreSpawnCount);

    UFUNCTION(BlueprintCallable, Category="山河纪元|性能|对象池")
    AActor* GetFromPool(FName PoolID, FVector Location, FRotator Rotation);

    UFUNCTION(BlueprintCallable, Category="山河纪元|性能|对象池")
    void ReturnToPool(FName PoolID, AActor* Actor);

    // LOD
    UFUNCTION(BlueprintCallable, Category="山河纪元|性能|LOD")
    ELODLevel CalculateLOD(FVector ObjectLocation, float ViewDistance) const;

    UFUNCTION(BlueprintCallable, Category="山河纪元|性能|LOD")
    void ApplyLOD(AActor* Actor, ELODLevel LOD);

    // 流式加载
    UFUNCTION(BlueprintCallable, Category="山河纪元|性能|流式加载")
    void StreamInRegion(FName RegionID);

    UFUNCTION(BlueprintCallable, Category="山河纪元|性能|流式加载")
    void StreamOutRegion(FName RegionID);

    // 帧率优化
    UFUNCTION(BlueprintCallable, Category="山河纪元|性能")
    void SetTargetFrameRate(int32 FPS);

    UFUNCTION(BlueprintCallable, Category="山河纪元|性能")
    float GetCurrentFPS() const { return CurrentFPS; }

    UFUNCTION(BlueprintCallable, Category="山河纪元|性能")
    void RunGC();

    UPROPERTY(BlueprintReadOnly, Category="山河纪元|性能")
    TMap<FName, FObjectPool> ObjectPools;

    UPROPERTY(BlueprintReadOnly, Category="山河纪元|性能")
    TSet<FName> LoadedRegions;

    UPROPERTY(BlueprintReadOnly, Category="山河纪元|性能")
    float CurrentFPS = 60.0f;

    UPROPERTY(EditAnywhere, Category="山河纪元|性能")
    float LODDistance0 = 500.0f;   // LOD0范围
    UPROPERTY(EditAnywhere, Category="山河纪元|性能")
    float LODDistance1 = 1500.0f;  // LOD1范围
    UPROPERTY(EditAnywhere, Category="山河纪元|性能")
    float LODDistance2 = 3000.0f;  // LOD2范围
    UPROPERTY(EditAnywhere, Category="山河纪元|性能")
    float LODDistance3 = 5000.0f;  // LOD3范围，超出剔除

private:
    float FrameTimeAccumulator = 0.0f;
    int32 FrameCount = 0;
    void UpdateFPS(float DeltaTime);
    void UpdateStreaming();
};
