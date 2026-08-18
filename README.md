# 山河纪元 · ShanHe Era

> 华夏古代历史文明沙盘 RPG | Historical Sandbox RPG
> 
> *"历史不是背景，历史就是世界本身。"*

---

## 项目简介

**山河纪元**是一款融合《骑马与砍杀》战场战术、《十字军之王3》大地图战略、《太吾绘卷》个人人生模拟的动态沙盘RPG/历史模拟器。

玩家不是天命之人，只是时代中的一个生命。世界不围绕玩家运行，AI NPC 自主决策，历史在玩家介入或旁观中自然演进。

**引擎**：Unreal Engine 5.8.1 | **语言**：C++20 + 蓝图 | **当前版本**：v2.5.5

---

## 核心特色

### 三层玩法融合

| 层级 | 参考作品 | 玩法 |
|------|---------|------|
| **大地图战略层** | 十字军之王3 | 角色肖像+关系图谱，AI领主自主决策，朝廷派系斗争 |
| **战场战术层** | 骑马与砍杀 | 第三人称指挥+亲自上阵，百人级同屏战斗 |
| **个人人生层** | 太吾绘卷 | 村庄/县城/职业/家族/江湖，完整人生模拟 |

### 信息不对称设计

- 农民玩家看不到朝廷派系斗争全貌，只能通过粮价、征兵、谣言感知
- 高官玩家看不到村庄具体旱灾细节，只能通过奏折了解
- 信息传播系统驱动谣言、情报、舆论的自然扩散

### 爱国情怀与历史传承

- **历史英雄彩蛋**：乱世护民、边疆驻守、灾年赈灾、王朝改革——触发精神传承者剧情
- **东海列岛支线**：以古称暗喻台湾，展现自古以来的文化同源与统一大势
- **人民团结**：灾年村民自发互助、普通人的英雄故事

---

## 技术架构

### 模块划分（14个子系统目录）

```
Source/ShanHeEra/
├── Core/          # 核心：GameMode/存档/性能/调试/数据配置/导演/相机/特效
├── World/         # 世界：城市/区域/建筑/昼夜/天气/导航/旅行事件
├── Character/     # 角色：玩家/NPC/NPC片段/属性
├── Dynasty/       # 王朝：朝廷/科举/宫廷斗争
├── Economy/       # 经济：贸易/物价/平衡
├── Military/      # 军事：战争/战斗/兵种
├── Family/        # 家族：族谱/传承/婚姻
├── Society/       # 社会：江湖/武功/社交
├── History/       # 历史：事件/天灾/剧本
├── Culture/       # 文化：科技/文学/医学/宗教/烹饪/节日
├── Audio/         # 音频：音乐/音效/CG/空间音频
├── Story/         # 剧情：任务/故事/对话
└── UI/            # 界面：12个UMG界面+基类
```

### 关键技术

- **对象池系统**：Actor/Widget对象池，减少频繁Spawn开销
- **性能监控**：实时FPS/内存/Actor统计，自动画质调整
- **版本化存档**：带版本号的SaveGame，支持存档迁移
- **调试验证**：结构化日志、运行时资产检查、性能标记
- **电影导演系统**：基于Shot Contract的分镜化演出，13种运镜，9种色彩分级
- **自适应音乐**：5层动态分层配乐，7种Stinger，情绪平滑过渡
- **数据驱动**：DataTable/JSON配置，支持Mod和平衡调整

---

## 系统清单（70+）

### 核心系统
GameMode | GameState | PlayerController | Character | HUD | SaveGame | ObjectPool | PerformanceMonitor | PerformanceSystem | DebugValidation | DataConfig | Achievement | Camera | Effect | CinematicDirector | Tutorial | DebugConsole | Information | Item | Scenario

### 世界系统
CityManager | RegionManager | Construction | DayNight | Weather | Navigation | TravelEvent | Map

### 角色系统
PlayerCharacter | NPCManager | NPCFragment | NPCMassProcessor

### 王朝/经济/军事
Court | Examination | Economy | EconomyBalance | War | Combat

### 家族/社会/历史
FamilyTree | Jianghu | MartialArts | Society | History | Disaster

### 文化系统
Technology | Literature | Medical | Religion | Cooking | Festival | Culture

### 音频/剧情/UI
AudioManager | SpatialAudio | CGManager | StoryManager | HUD | MainMenu | Inventory | Equipment | Quest | WorldMap | SkillTree | FamilyTreeUI | RelationshipGraph | Sect | Codex

---

## 角色阵容（25+）

### 核心伙伴
- **老顽童匠师** — 70岁爱吃糖，技艺通神
- **冷面热心女镖师** — 左脸有疤，外冷内热
- **迂腐可爱落第书生** — 大是大非面前异常坚定
- **深不可测茶馆老板** — 右手缺小指，什么都知道
- **神医** — 江湖奇人，医术通神
- **歌姬** — 才貌双全，情报枢纽
- **富商** — 亦正亦邪，经济线关键

### 宫廷角色
皇帝 | 太子 | 公主 | 太监总管 | 妃嫔

### 外族首领
匈奴 | 突厥 | 蒙古

---

## 历史剧本（4大开局）

| 剧本 | 时代 | 特点 |
|------|------|------|
| 三国 | 东汉末年 | 群雄割据，英雄辈出 |
| 盛唐 | 唐朝 | 开放繁荣，万国来朝 |
| 弱宋 | 宋朝 | 文化鼎盛，军事积弱 |
| 明末 | 明朝末年 | 内忧外患，王朝更迭 |

---

## 语言支持（10种）

简体中文 | 繁體中文 | 文言文 | English | 日本語 | 한국어 | Русский | Français | Deutsch | Español

---

## 音乐与CG

### 15首核心曲目
主主题 | 战场 | 朝堂 | 田园 | 江湖 | 灾年 | 盛世 | 乱世 | 边疆 | 海岛 | 科举 | 婚礼 | 葬礼 | 大一统 | 宝岛归心

### 8段CG动画
开国大典 | 烽火连天 | 九州一统 | 宝岛归心 | 王朝覆灭 | 变法图强 | 民不聊生 | 山河破碎

---

## 快速开始

### 环境要求
- Unreal Engine 5.8.1+
- Visual Studio 2022/2026 (C++ Desktop Development)
- Windows 10/11 x64

### 编译步骤

```bash
# 1. 克隆仓库
git clone https://github.com/Hrxstnyi/ShanHeEra.git

# 2. 右键 ShanHeEra.uproject → Generate Visual Studio project files

# 3. 用VS打开 ShanHeEra.sln，选择 Development Editor + Win64，编译

# 4. 或使用命令行编译
"C:\Epic Games\虚幻引擎5\UE_5.8\Engine\Binaries\ThirdParty\DotNet\10.0\win-x64\dotnet.exe" \
  "C:\Epic Games\虚幻引擎5\UE_5.8\Engine\Binaries\DotNET\UnrealBuildTool\UnrealBuildTool.dll" \
  ShanHeEraEditor Win64 Development \
  "-project=路径\ShanHeEra.uproject" -waitmutex -NoUBA
```

### 启动器

```bash
cd Launcher
pip install -r requirements.txt
python launcher.py
```

启动器支持10种语言、游戏设置、更新检查，可通过UE编辑器开发模式启动。

---

## 项目状态

- ✅ C++框架编译通过（UE5.8.1）
- ✅ 70+系统骨架完成
- ✅ 10种语言本地化框架
- ✅ 启动器完成
- ✅ 设计文档完整（18份）
- ⏳ 资产填充（角色/场景/UI/音频）
- ⏳ 蓝图绑定（GameMode/PlayerController/HUD/UI）
- ⏳ 基础关卡创建
- ⏳ 垂直切片Demo

---

## 开发路线图

### Phase 1：垂直切片（当前）
- 一个朝代、一个地区、一条职业线
- 基础玩法循环验证
- 可玩Demo

### Phase 2：核心完整版
- 3个朝代、全地图
- 九大文明支柱系统
- 家族传承/多世代
- EA发售

### Phase 3：内容扩展
- 更多朝代剧本
- 联机模式
- Mod支持
- 多语言配音

---

## 贡献

欢迎提交Issue和Pull Request。

---

## 许可证

Copyright (c) 2026 山河纪元工作室. All Rights Reserved.

---

*山河纪元工作室 | 用游戏讲述华夏故事*
