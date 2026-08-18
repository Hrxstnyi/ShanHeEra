#include "Core/GameLayerManager.h"
#include "Core/ShanHeLog.h"

void UGameLayerManager::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
    // 信息差配置：数值越大，能获取的信息范围越广
    InformationAccessRange.Add(EGameLayer::Life, 0.2f);      // 个人层：只能看到身边的事
    InformationAccessRange.Add(EGameLayer::Strategy, 0.8f);  // 战略层：能看到天下大势，但看不到细节
    InformationAccessRange.Add(EGameLayer::Battle, 0.5f);    // 战场层：能看到战区信息
    UE_LOG(LogShanHe, Log, TEXT("游戏层级管理器初始化 - 三层信息差系统就绪"));
}

void UGameLayerManager::SwitchLayer(EGameLayer NewLayer)
{
    if (NewLayer == CurrentLayer) return;
    EGameLayer OldLayer = CurrentLayer;
    CurrentLayer = NewLayer;
    ApplyLayerEffects(NewLayer);
    OnLayerChanged.Broadcast(NewLayer, OldLayer);
    UE_LOG(LogShanHe, Log, TEXT("游戏层级切换: %d -> %d"), (int32)OldLayer, (int32)NewLayer);
}

bool UGameLayerManager::CanAccessInformation(EGameLayer FromLayer, EGameLayer ToLayer) const
{
    // 信息差规则：
    // Life层只能感知Life层的信息，通过谣言/粮价/征兵间接感知Strategy层
    // Strategy层能看到Strategy层，通过奏折/报告间接感知Life层
    // Battle层介于两者之间
    if (FromLayer == ToLayer) return true;
    if (FromLayer == EGameLayer::Strategy && ToLayer == EGameLayer::Battle) return true;
    if (FromLayer == EGameLayer::Battle && ToLayer == EGameLayer::Strategy) return true;
    return false; // Life与Strategy之间不能直接访问，必须通过信息传播系统
}

void UGameLayerManager::ApplyLayerEffects(EGameLayer NewLayer)
{
    switch (NewLayer)
    {
        case EGameLayer::Life:
            // 进入个人层：暂停大地图模拟，聚焦玩家周围
            break;
        case EGameLayer::Strategy:
            // 进入战略层：加速时间，显示全国地图
            break;
        case EGameLayer::Battle:
            // 进入战场：切换战斗HUD，生成战场Actor
            break;
        default:
            break;
    }
}
