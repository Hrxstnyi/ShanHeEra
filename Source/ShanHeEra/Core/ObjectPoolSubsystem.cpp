#include "Core/ObjectPoolSubsystem.h"
#include "GameFramework/Actor.h"
#include "Blueprint/UserWidget.h"
#include "Engine/World.h"
#include "Engine/GameInstance.h"

void UObjectPoolSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
    UE_LOG(LogTemp, Log, TEXT("[ObjectPool] 子系统初始化完成"));
}

void UObjectPoolSubsystem::Deinitialize()
{
    ClearAllPools();
    Super::Deinitialize();
}

AActor* UObjectPoolSubsystem::AcquireActor(TSubclassOf<AActor> ActorClass, FVector Location, FRotator Rotation)
{
    if (!ActorClass)
    {
        UE_LOG(LogTemp, Warning, TEXT("[ObjectPool] AcquireActor: ActorClass为空"));
        return nullptr;
    }

    UClass* Key = ActorClass.Get();
    TArray<AActor*>& Pool = ActorPool.FindOrAdd(Key);

    AActor* Acquired = nullptr;

    // 从池中获取
    while (Pool.Num() > 0)
    {
        Acquired = Pool.Pop();
        if (IsValid(Acquired))
        {
            break;
        }
        Acquired = nullptr;
    }

    // 池为空，创建新对象
    if (!Acquired)
    {
        UWorld* World = GetWorld();
        if (!World) return nullptr;

        FActorSpawnParameters Params;
        Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
        Acquired = World->SpawnActor<AActor>(ActorClass, Location, Rotation, Params);
        TotalCreated++;
    }
    else
    {
        // 重置位置和状态
        Acquired->SetActorLocationAndRotation(Location, Rotation);
        Acquired->SetActorHiddenInGame(false);
        Acquired->SetActorEnableCollision(true);
        if (Acquired->GetRootComponent())
        {
            Acquired->GetRootComponent()->SetVisibility(true);
        }
    }

    if (Acquired)
    {
        ActiveActors.Add(Acquired);
        TotalAcquired++;
    }

    return Acquired;
}

void UObjectPoolSubsystem::ReleaseActor(AActor* Actor)
{
    if (!IsValid(Actor)) return;
    if (!ActiveActors.Contains(Actor)) return;

    UClass* Key = Actor->GetClass();
    TArray<AActor*>& Pool = ActorPool.FindOrAdd(Key);

    // 池已满，销毁对象
    if (Pool.Num() >= MaxPoolSize)
    {
        DestroyPooledActor(Actor);
        ActiveActors.Remove(Actor);
        return;
    }

    // 隐藏并禁用
    Actor->SetActorHiddenInGame(true);
    Actor->SetActorEnableCollision(false);
    Actor->SetActorTickEnabled(false);
    if (Actor->GetRootComponent())
    {
        Actor->GetRootComponent()->SetVisibility(false);
    }

    Pool.Add(Actor);
    ActiveActors.Remove(Actor);
    TotalReleased++;
}

void UObjectPoolSubsystem::PreWarmActorPool(TSubclassOf<AActor> ActorClass, int32 Count)
{
    if (!ActorClass || Count <= 0) return;

    UWorld* World = GetWorld();
    if (!World) return;

    UClass* Key = ActorClass.Get();
    TArray<AActor*>& Pool = ActorPool.FindOrAdd(Key);

    for (int32 i = 0; i < Count && Pool.Num() < MaxPoolSize; i++)
    {
        FActorSpawnParameters Params;
        Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
        AActor* NewActor = World->SpawnActor<AActor>(ActorClass, FVector::ZeroVector, FRotator::ZeroRotator, Params);
        if (NewActor)
        {
            NewActor->SetActorHiddenInGame(true);
            NewActor->SetActorEnableCollision(false);
            NewActor->SetActorTickEnabled(false);
            Pool.Add(NewActor);
            TotalCreated++;
        }
    }

    UE_LOG(LogTemp, Log, TEXT("[ObjectPool] 预热Actor池 %s: %d个"), *ActorClass->GetName(), Count);
}

UUserWidget* UObjectPoolSubsystem::AcquireWidget(TSubclassOf<UUserWidget> WidgetClass, UObject* WorldContextObject)
{
    if (!WidgetClass) return nullptr;

    UClass* Key = WidgetClass.Get();
    TArray<UUserWidget*>& Pool = WidgetPool.FindOrAdd(Key);

    UUserWidget* Acquired = nullptr;

    while (Pool.Num() > 0)
    {
        Acquired = Pool.Pop();
        if (IsValid(Acquired))
        {
            break;
        }
        Acquired = nullptr;
    }

    if (!Acquired)
    {
        Acquired = CreateWidget<UUserWidget>(WorldContextObject, WidgetClass);
        TotalCreated++;
    }

    if (Acquired)
    {
        Acquired->SetVisibility(ESlateVisibility::Visible);
        ActiveWidgets.Add(Acquired);
        TotalAcquired++;
    }

    return Acquired;
}

void UObjectPoolSubsystem::ReleaseWidget(UUserWidget* Widget)
{
    if (!IsValid(Widget)) return;
    if (!ActiveWidgets.Contains(Widget)) return;

    UClass* Key = Widget->GetClass();
    TArray<UUserWidget*>& Pool = WidgetPool.FindOrAdd(Key);

    if (Pool.Num() >= MaxPoolSize)
    {
        Widget->RemoveFromParent();
        Widget->MarkAsGarbage();
        ActiveWidgets.Remove(Widget);
        return;
    }

    Widget->RemoveFromParent();
    Widget->SetVisibility(ESlateVisibility::Collapsed);
    Pool.Add(Widget);
    ActiveWidgets.Remove(Widget);
    TotalReleased++;
}

void UObjectPoolSubsystem::PreWarmWidgetPool(TSubclassOf<UUserWidget> WidgetClass, int32 Count, UObject* WorldContextObject)
{
    if (!WidgetClass || Count <= 0) return;

    UClass* Key = WidgetClass.Get();
    TArray<UUserWidget*>& Pool = WidgetPool.FindOrAdd(Key);

    for (int32 i = 0; i < Count && Pool.Num() < MaxPoolSize; i++)
    {
        UUserWidget* NewWidget = CreateWidget<UUserWidget>(WorldContextObject, WidgetClass);
        if (NewWidget)
        {
            NewWidget->SetVisibility(ESlateVisibility::Collapsed);
            Pool.Add(NewWidget);
            TotalCreated++;
        }
    }
}

void UObjectPoolSubsystem::ClearAllPools()
{
    // 销毁所有池中的Actor
    for (auto& Pair : ActorPool)
    {
        for (AActor* Actor : Pair.Value)
        {
            DestroyPooledActor(Actor);
        }
    }
    ActorPool.Empty();

    // 销毁所有池中的Widget
    for (auto& Pair : WidgetPool)
    {
        for (UUserWidget* Widget : Pair.Value)
        {
            if (IsValid(Widget))
            {
                Widget->RemoveFromParent();
                Widget->MarkAsGarbage();
            }
        }
    }
    WidgetPool.Empty();

    ActiveActors.Empty();
    ActiveWidgets.Empty();

    UE_LOG(LogTemp, Log, TEXT("[ObjectPool] 所有池已清空"));
}

FString UObjectPoolSubsystem::GetPoolStats() const
{
    int32 TotalPooled = 0;
    for (const auto& Pair : ActorPool) TotalPooled += Pair.Value.Num();
    for (const auto& Pair : WidgetPool) TotalPooled += Pair.Value.Num();

    return FString::Printf(TEXT("池统计: 池内=%d, 使用中=%d, 总创建=%d, 总获取=%d, 总归还=%d"),
        TotalPooled, ActiveActors.Num() + ActiveWidgets.Num(),
        TotalCreated, TotalAcquired, TotalReleased);
}

void UObjectPoolSubsystem::DestroyPooledActor(AActor* Actor)
{
    if (IsValid(Actor))
    {
        Actor->Destroy();
    }
}
