# 山河纪元 · 技术架构文档

> 版本：v1.1
> 引擎：Unreal Engine 5.3+
> 语言：C++20

---

## 一、整体架构

### 三层游戏模式

```
┌──────────────────────────────────────────────────────────┐
│                    GameMode (全局调度)                     │
│  ┌────────────────────────────────────────────────────┐  │
│  │          WorldSimulationSubsystem (C++核心)         │  │
│  │  ┌──────────┐ ┌──────────┐ ┌──────────┐           │  │
│  │  │ 时间推进  │ │ NPC调度  │ │ 经济模拟 │           │  │
│  │  └──────────┘ └──────────┘ └──────────┘           │  │
│  │  ┌──────────┐ ┌──────────┐ ┌──────────┐           │  │
│  │  │ 事件系统  │ │ 信息传播 │ │ 存档系统 │           │  │
│  │  └──────────┘ └──────────┘ └──────────┘           │  │
│  └────────────────────────────────────────────────────┘  │
│                                                            │
│  ┌─────────────┐  ┌─────────────┐  ┌─────────────┐      │
│  │  LifeLayer  │  │StrategyLayer│  │ BattleLayer │      │
│  │  (太吾式)   │  │  (CK3式)    │  │  (骑砍式)   │      │
│  │  个人人生   │  │  大地图战略  │  │  战场战术   │      │
│  └─────────────┘  └─────────────┘  └─────────────┘      │
└──────────────────────────────────────────────────────────┘
```

### 核心设计原则

1. **世界不围绕玩家运行**：所有模拟在Subsystem中独立运行，玩家只是观察者+参与者
2. **信息差**：三层之间信息不互通，通过信息传播系统间接传递
3. **数据驱动**：所有游戏数据用DataAsset/数据表配置，不硬编码
4. **C++核心 + 蓝图表现**：模拟逻辑用C++，UI/剧情/特效用蓝图

---

## 二、目录结构与模块划分

```
Source/ShanHeEra/
├── Core/                    # 核心框架
│   ├── ShanHeGameMode       # 游戏模式，时间推进主循环
│   ├── ShanHeGameState      # 全局世界状态
│   ├── ShanHePlayerController
│   ├── ShanHePlayerState
│   ├── ShanHeCharacter      # 角色基类
│   ├── ShanHeEnums          # 全局枚举
│   ├── ShanHeStructs        # 全局结构体
│   ├── GameLayerManager     # 三层模式切换+信息差
│   ├── InformationSystem    # 信息传播系统
│   └── SaveSystem           # 二进制存档
├── World/                   # 世界模拟
│   ├── WorldSimulationSubsystem  # 世界模拟总调度
│   ├── RegionManager        # 7大区域管理
│   └── CityManager          # 城池管理
├── Character/               # NPC与AI
│   ├── NPCManager           # NPC数据管理
│   ├── NPCAIController      # AI控制器
│   └── NPCFragment          # MassEntity纯数据片段
├── Dynasty/                 # 朝廷政治
│   └── CourtSystem          # 皇帝/官员/派系/科举
├── Economy/                 # 经济
│   └── EconomySystem        # 农业/商业/货币/贸易
├── Military/                # 军事
│   └── WarSystem            # 军队/战争/围城
├── Family/                  # 家族
│   └── FamilySystem         # 婚姻/子女/传承
├── Society/                 # 社会组织
│   └── SocietySystem        # 宗族/行会/商帮/书院/镖局
├── History/                 # 历史演化
│   └── HistorySystem        # 朝代/事件/天灾/彩蛋
├── Culture/                 # 文化
│   └── CultureSystem        # 节日/民俗
├── Audio/                   # 音频
│   ├── AudioManager         # 自适应音乐管理
│   └── CGManager            # CG动画管理
├── Story/                   # 剧情
│   └── StoryManager         # 角色/任务/彩蛋
└── UI/                      # 界面
    ├── MainMenuWidget
    └── HUDWidget
```

---

## 三、关键系统实现

### 3.1 世界模拟主循环

```cpp
// WorldSimulationSubsystem.cpp
void UWorldSimulationSubsystem::SimulateTick(int32 Days)
{
    TotalSimulatedDays += Days;
    // 各系统并行模拟（实际可用TaskGraph并行）
    SimulateEconomy(Days);      // 经济：粮价、贸易、税收
    SimulatePolitics(Days);     // 政治：官员升迁、派系斗争
    SimulateMilitary(Days);     // 军事：战争、军队
    SimulateSociety(Days);      // 社会：组织、江湖
    SimulateDisasters(Days);    // 天灾：随机灾害
    CheckHistoricalEvents();    // 历史事件触发
}
```

**性能策略**：
- 经济/政治/社会用纯数据计算，不Spawn Actor
- 只有玩家附近的NPC才Spawn可视Actor
- 远处的NPC用MassEntity批量处理

### 3.2 三层模式与信息差

```cpp
// GameLayerManager.h
enum class EGameLayer : uint8
{
    Life,        // 个人人生层：只能看到身边的事
    Strategy,    // 大地图战略层：能看到天下大势
    Battle       // 战场战术层：能看到战区信息
};

// 信息差规则：Life与Strategy之间不能直接访问
// 必须通过InformationSystem传播（可能延迟、夸大、变成谣言）
bool CanAccessInformation(EGameLayer From, EGameLayer To) const;
```

**信息传播流程**：
1. 事件发生在Strategy层（如朝廷改革）
2. InformationSystem创建信息片段，从京城开始传播
3. 信息每天向相邻城市扩散，可信度逐渐下降
4. 玩家在Life层只能通过"听说"获取延迟且可能不准确的信息
5. 玩家身份越高（官员/士人），信息越准确

### 3.3 数千NPC同存方案（MassEntity）

```cpp
// NPCFragment.h - 纯数据片段，不继承UObject
struct FNPCIdentityFragment : FMassFragment
{
    FGuid NPCID;
    FPersonName Name;
    int32 Age;
    ESocialClass SocialClass;
    bool bIsAlive;
};

struct FNPCNeedFragment : FMassFragment
{
    float Hunger;     // 饱腹度
    float Happiness;  // 幸福度
    float Health;     // 健康度
};

// Processor批量处理数千NPC
void UNPCMassProcessor::ProcessNeeds(float DeltaTime, TArray<FNPCNeedFragment>& Needs)
{
    for (FNPCNeedFragment& Need : Needs)
    {
        Need.Hunger -= 0.1f * DeltaTime;  // 每小时衰减
        Need.Happiness += FMath::RandRange(-0.05f, 0.05f);
    }
}
```

**Actor生成策略**：
- 玩家周围500米内的NPC → Spawn ACharacter（可视、可交互）
- 500米外的NPC → 仅存在于MassEntity数据中，不渲染
- 玩家离开后 → Actor销毁，数据保留

### 3.4 存档系统（二进制序列化）

```cpp
// SaveSystem.h
struct FWorldSaveData
{
    FDate CurrentDate;
    int32 TotalPopulation;
    float WorldStability;
    // 玩家状态
    FGuid PlayerNPCID;
    FCharacterStats PlayerStats;
    // 经济/政治状态
    float NationalTreasury;
    float ImperialAuthority;
    // 区域/城市数据（用TArray序列化）
    TArray<FName> RegionIDs;
    TArray<int32> RegionPopulations;
    // 已触发事件
    TArray<FName> TriggeredEvents;
};

// 二进制序列化，比JSON快10倍，体积小5倍
bool SerializeData(FArchive& Ar, FWorldSaveData& Data)
{
    Ar << Data.CurrentDate.Year;
    Ar << Data.CurrentDate.Month;
    // ...
}
```

**存档格式**：`.shsave`（自定义二进制格式）
- 包含版本号，支持升级迁移
- 关键NPC全量保存，普通NPC用种子+偏移量生成
- 支持QuickSave/QuickLoad

### 3.5 自适应音乐系统

```cpp
// AudioManager.h
enum class EMusicMood : uint8
{
    Peaceful, Tense, War, Epic,
    Sorrowful, Triumphant, Festival, Nostalgic
};

void UAudioManager::SetMusicMood(EMusicMood NewMood)
{
    // 通过MetaSounds参数切换情绪层
    // 地域氛围层持续播放，情绪层淡入淡出
}
```

详见 [AudioDesign.md](AudioDesign.md)

---

## 四、C++与蓝图分工

| 层级 | C++ | 蓝图 |
|------|-----|------|
| **世界模拟** | 全部核心逻辑、数据结构 | 无 |
| **NPC AI** | MassEntity数据、StateTree逻辑 | 行为树装饰器、交互对话 |
| **UI** | Widget基类、数据绑定 | 界面布局、动画、交互 |
| **剧情** | 任务系统框架、数据结构 | 对话树、过场序列、CG触发 |
| **战斗** | 战力计算、战争模拟 | 战斗HUD、技能表现、特效 |
| **存档** | 二进制序列化 | 存档UI |
| **音频** | MetaSounds参数控制 | 音效触发 |

---

## 五、性能优化策略

### 5.1 NPC优化
- 数千NPC用MassEntity纯数据，不Spawn Actor
- 玩家附近才生成可视Actor，使用对象池
- AI决策用StateTree，比行为树快3倍
- 远处NPC的AI每N帧才更新一次

### 5.2 世界优化
- 大地图用World Partition，按区域流式加载
- 经济/政治模拟在后台线程运行（TaskGraph）
- 时间尺度可调节：暂停/1x/3x/快速推进

### 5.3 内存优化
- 音乐流式加载，常驻内存<50MB
- 贴图用Virtual Texture
- 存档用二进制，单存档<5MB

---

## 六、网络同步（预留）

如果未来做联机：
- 世界模拟在服务器端运行
- 客户端只同步玩家附近的NPC和事件
- 用自定义序列化，不用Replication（数据量太大）
- 支持最多4人联机（各自扮演不同角色，在同一世界中）

---

## 七、版本更新记录

### v1.1 (当前)
- 新增 GameLayerManager 三层模式切换+信息差系统
- 新增 MassEntity NPC数据片段（数千NPC同存方案）
- 新增 SaveSystem 二进制存档系统
- 新增 InformationSystem 信息传播系统
- 新增 音乐系统设计文档
- 新增 苏忘机（顽童匠师）完整人设
- 完善 技术架构文档

### v1.0
- 初始版本：核心框架+九大系统+三语本地化+启动器
