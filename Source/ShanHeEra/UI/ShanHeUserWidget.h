#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ShanHeUserWidget.generated.h"

class UTexture2D;

/**
 * 山河纪元 UI 基类 - 基于 ue5-ui-umg-slate 方法论
 * 提供统一的生命周期管理、数据绑定、输入模式和清理路径
 * 所有游戏内 UI 应继承此类
 */
UCLASS(Abstract)
class SHANHEERA_API UShanHeUserWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UShanHeUserWidget(const FObjectInitializer& ObjectInitializer);

    // === 生命周期 ===

    virtual void NativeConstruct() override;
    virtual void NativeDestruct() override;
    virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

    // === UI 管理 ===

    /** 显示到视口 */
    UFUNCTION(BlueprintCallable, Category="山河纪元|UI")
    virtual void Show(int32 ZOrder = 0);

    /** 从视口隐藏 */
    UFUNCTION(BlueprintCallable, Category="山河纪元|UI")
    virtual void Hide();

    /** 是否可见 */
    UFUNCTION(BlueprintPure, Category="山河纪元|UI")
    bool IsShown() const { return bIsShown; }

    // === 输入模式 ===

    /** 设置此 UI 打开时的输入模式 */
    UFUNCTION(BlueprintCallable, Category="山河纪元|UI")
    void SetInputModeOnShow(bool bUIOnly = false, bool bShowMouse = true);

    /** 关闭时恢复输入模式 */
    UFUNCTION(BlueprintCallable, Category="山河纪元|UI")
    void RestoreInputModeOnHide();

    // === 数据绑定 ===

    /** 刷新 UI 数据（子类重写） */
    UFUNCTION(BlueprintCallable, Category="山河纪元|UI")
    virtual void RefreshData();

    /** 订阅数据变化事件 */
    UFUNCTION(BlueprintCallable, Category="山河纪元|UI")
    virtual void BindDataEvents();

    /** 取消订阅数据变化事件 */
    UFUNCTION(BlueprintCallable, Category="山河纪元|UI")
    virtual void UnbindDataEvents();

    // === 定时器管理 ===

    /** 添加受生命周期管理的定时器 */
    UFUNCTION(BlueprintCallable, Category="山河纪元|UI")
    void AddManagedTimer(FTimerHandle Handle);

    /** 清除所有受管理的定时器 */
    UFUNCTION(BlueprintCallable, Category="山河纪元|UI")
    void ClearAllManagedTimers();

    // === 工具函数 ===

    /** 设置背景模糊（毛玻璃效果） */
    UFUNCTION(BlueprintCallable, Category="山河纪元|UI")
    void SetBackgroundBlur(float Strength = 5.0f, FLinearColor Tint = FLinearColor::Black);

    /** 播放 UI 出现动画 */
    UFUNCTION(BlueprintImplementableEvent, Category="山河纪元|UI")
    void OnShowAnimation();

    /** 播放 UI 消失动画 */
    UFUNCTION(BlueprintImplementableEvent, Category="山河纪元|UI")
    void OnHideAnimation();

protected:
    // 子类可重写的生命周期钩子
    virtual void OnShow() {}
    virtual void OnHide() {}
    virtual void OnRefreshData() {}

    // 是否已经显示
    bool bIsShown = false;

    // 输入模式设置
    bool bUseCustomInputMode = false;
    bool bInputModeUIOnly = false;
    bool bShowMouseCursor = true;

    // 受管理的定时器句柄
    TArray<FTimerHandle> ManagedTimers;

    // 之前的焦点控件（用于恢复）
    UPROPERTY()
    TObjectPtr<UWidget> PreviousFocusWidget = nullptr;

private:
    // 内部清理
    void Cleanup();
};
