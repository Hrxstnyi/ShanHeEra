#pragma once
#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "ObjectPoolSubsystem.generated.h"

class AActor;
class UUserWidget;

/**
 * 对象池子系统 - 基于 ue5-performance-packaging 方法论
 * 复用频繁生成/销毁的对象，减少动态分配开销和GC压力
 * 支持 Actor 池和 Widget 池
 */
UCLASS()
class SHANHEERA_API UObjectPoolSubsystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;

    // === Actor 对象池 ===

    /** 从池中获取或创建一个 Actor */
    UFUNCTION(BlueprintCallable, Category="山河纪元|对象池")
    AActor* AcquireActor(TSubclassOf<AActor> ActorClass, FVector Location = FVector::ZeroVector,
                         FRotator Rotation = FRotator::ZeroRotator);

    /** 将 Actor 归还到池中 */
    UFUNCTION(BlueprintCallable, Category="山河纪元|对象池")
    void ReleaseActor(AActor* Actor);

    /** 预生成指定数量的 Actor 到池中 */
    UFUNCTION(BlueprintCallable, Category="山河纪元|对象池")
    void PreWarmActorPool(TSubclassOf<AActor> ActorClass, int32 Count);

    // === Widget 对象池 ===

    /** 从池中获取或创建一个 Widget */
    UFUNCTION(BlueprintCallable, Category="山河纪元|对象池")
    UUserWidget* AcquireWidget(TSubclassOf<UUserWidget> WidgetClass, UObject* WorldContextObject);

    /** 将 Widget 归还到池中 */
    UFUNCTION(BlueprintCallable, Category="山河纪元|对象池")
    void ReleaseWidget(UUserWidget* Widget);

    /** 预生成指定数量的 Widget 到池中 */
    UFUNCTION(BlueprintCallable, Category="山河纪元|对象池")
    void PreWarmWidgetPool(TSubclassOf<UUserWidget> WidgetClass, int32 Count, UObject* WorldContextObject);

    // === 池管理 ===

    /** 清空所有池，销毁所有对象 */
    UFUNCTION(BlueprintCallable, Category="山河纪元|对象池")
    void ClearAllPools();

    /** 获取池统计信息 */
    UFUNCTION(BlueprintCallable, Category="山河纪元|对象池")
    FString GetPoolStats() const;

    /** 设置池最大容量 */
    UFUNCTION(BlueprintCallable, Category="山河纪元|对象池")
    void SetMaxPoolSize(int32 NewMaxSize) { MaxPoolSize = NewMaxSize; }

private:
    // Actor 池：类 -> 可用对象栈
    TMap<UClass*, TArray<AActor*>> ActorPool;

    // Widget 池：类 -> 可用对象栈
    TMap<UClass*, TArray<UUserWidget*>> WidgetPool;

    // 正在使用的 Actor（用于验证归还合法性）
    TSet<AActor*> ActiveActors;

    // 正在使用的 Widget
    TSet<UUserWidget*> ActiveWidgets;

    // 池最大容量
    int32 MaxPoolSize = 100;

    // 统计
    int32 TotalAcquired = 0;
    int32 TotalReleased = 0;
    int32 TotalCreated = 0;

    void DestroyPooledActor(AActor* Actor);
};
