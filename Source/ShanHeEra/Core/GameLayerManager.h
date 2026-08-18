#pragma once
#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "ShanHeGameLayerManager.generated.h"

UENUM(BlueprintType)
enum class EGameLayer : uint8
{
    None        UMETA(DisplayName="无"),
    Life        UMETA(DisplayName="个人人生层 (太吾式)"),
    Strategy    UMETA(DisplayName="大地图战略层 (CK3式)"),
    Battle      UMETA(DisplayName="战场战术层 (骑砍式)"),
    CG          UMETA(DisplayName="CG过场")
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnLayerChanged, EGameLayer, NewLayer, EGameLayer, OldLayer);

/**
 * 三层游戏模式管理器
 * 负责在 个人人生层 / 大地图战略层 / 战场战术层 之间切换
 * 核心设计：三层之间存在"信息差"——农民看不到朝廷全貌，高官看不到村庄细节
 */
UCLASS()
class SHANHEERA_API UGameLayerManager : public UGameInstanceSubsystem
{
    GENERATED_BODY()
public:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;

    UFUNCTION(BlueprintCallable, Category="山河纪元|层级")
    void SwitchLayer(EGameLayer NewLayer);

    UFUNCTION(BlueprintCallable, Category="山河纪元|层级")
    EGameLayer GetCurrentLayer() const { return CurrentLayer; }

    UFUNCTION(BlueprintCallable, Category="山河纪元|层级")
    bool CanAccessInformation(EGameLayer FromLayer, EGameLayer ToLayer) const;

    UPROPERTY(BlueprintAssignable, Category="山河纪元|层级")
    FOnLayerChanged OnLayerChanged;

    UPROPERTY(BlueprintReadOnly, Category="山河纪元|层级")
    EGameLayer CurrentLayer = EGameLayer::Life;

    UPROPERTY(EditAnywhere, Category="山河纪元|层级|信息差")
    TMap<EGameLayer, float> InformationAccessRange;

private:
    void ApplyLayerEffects(EGameLayer NewLayer);
};
