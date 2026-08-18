#include "UI/ShanHeUserWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "TimerManager.h"

UShanHeUserWidget::UShanHeUserWidget(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
}

void UShanHeUserWidget::NativeConstruct()
{
    Super::NativeConstruct();
    BindDataEvents();
}

void UShanHeUserWidget::NativeDestruct()
{
    Cleanup();
    Super::NativeDestruct();
}

void UShanHeUserWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);
}

void UShanHeUserWidget::Show(int32 ZOrder)
{
    if (bIsShown) return;

    AddToViewport(ZOrder);
    bIsShown = true;

    if (bUseCustomInputMode)
    {
        APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
        if (PC)
        {
            if (bInputModeUIOnly)
            {
                UWidgetBlueprintLibrary::SetInputMode_UIOnlyEx(PC, this, bShowMouseCursor ? EMouseLockMode::DoNotLock : EMouseLockMode::LockAlways);
            }
            else
            {
                UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(PC, this, EMouseLockMode::DoNotLock, false);
            }
            PC->bShowMouseCursor = bShowMouseCursor;
        }
    }

    OnShow();
    OnShowAnimation();
}

void UShanHeUserWidget::Hide()
{
    if (!bIsShown) return;

    OnHideAnimation();
    OnHide();

    if (bUseCustomInputMode)
    {
        RestoreInputModeOnHide();
    }

    RemoveFromParent();
    bIsShown = false;
}

void UShanHeUserWidget::SetInputModeOnShow(bool bUIOnly, bool bShowMouse)
{
    bUseCustomInputMode = true;
    bInputModeUIOnly = bUIOnly;
    bShowMouseCursor = bShowMouse;
}

void UShanHeUserWidget::RestoreInputModeOnHide()
{
    APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    if (PC)
    {
        UWidgetBlueprintLibrary::SetInputMode_GameOnly(PC);
        PC->bShowMouseCursor = false;
    }
}

void UShanHeUserWidget::RefreshData()
{
    OnRefreshData();
}

void UShanHeUserWidget::BindDataEvents()
{
    // 子类重写以订阅事件
}

void UShanHeUserWidget::UnbindDataEvents()
{
    // 子类重写以取消订阅
}

void UShanHeUserWidget::AddManagedTimer(FTimerHandle Handle)
{
    if (Handle.IsValid())
    {
        ManagedTimers.Add(Handle);
    }
}

void UShanHeUserWidget::ClearAllManagedTimers()
{
    UWorld* World = GetWorld();
    if (!World) return;

    FTimerManager& TimerManager = World->GetTimerManager();
    for (FTimerHandle& Handle : ManagedTimers)
    {
        if (Handle.IsValid())
        {
            TimerManager.ClearTimer(Handle);
        }
    }
    ManagedTimers.Empty();
}

void UShanHeUserWidget::SetBackgroundBlur(float Strength, FLinearColor Tint)
{
    // 蓝图中通过 BackgroundBlur 控件实现，这里提供接口
}

void UShanHeUserWidget::Cleanup()
{
    ClearAllManagedTimers();
    UnbindDataEvents();

    if (bIsShown)
    {
        RemoveFromParent();
        bIsShown = false;
    }
}
