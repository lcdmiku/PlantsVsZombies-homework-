# Plants vs. Zombies (Qt/C++ 课程设计) —— Scary大人指导版

一个基于 **Qt6** 框架、使用 **C++17** 开发的植物大战僵尸游戏复刻项目，采用 CMake 构建。项目实现了完整的游戏循环、多种植物与僵尸类型、关卡系统、动画系统、音效管理以及商店/阳光/小推车等核心机制，适合作为 Qt 图形界面编程的学习参考。

本版本在经典玩法基础上，参考了 B 站 UP 主“头牛o”的《植物大战僵尸指导版》，并加入了原创指导角色 **Scary大人**、新事件系统与新 Boss，是一个带有二次创作性质的课程设计作品。

---

## 设计理念与特色

### 设计理念

- **理性上**：此前有塔防游戏开发经验，本选题更加得心应手。
- **感性上**：喜爱《植物大战僵尸》这款游戏，受网上各式改版鼓动，决定写一个自己的简陋改版。

### 设计特色

- **参考**：B 站 UP 主“头牛o”的《植物大战僵尸指导版》。
- **区别**：
  - 新的指导角色：**Scary大人**（胆小菇拟人版），由队友推荐，性格有趣，且本就是植僵大家族的一员。
  - 新的事件系统：献祭植物、护眼模式、选项对 Boss 的影响等。
  - 新的 Boss：由冰车僵尸“超进化”而来（原定巨人僵尸，因资源不足改为冰车僵尸），根据前面献祭的植物拥有不同数值，具备召唤僵尸的特殊技能（低概率自我复制），并继承原版碾压植物的特点。
- **代码规模**：约 4k~5k 行。为了层层封装，部分函数在多个类中都有实现（通过调用最初实现的函数），略显重复，但能更好地实现继承和代码复用。

---

## 功能特性

- **多关卡系统**：内置多个关卡数据，支持关卡选择界面。
- **植物类型**：豌豆射手、向日葵、坚果墙、樱桃炸弹、土豆雷、寒冰射手、胆小菇等，均继承自 `Plant` 基类。
- **僵尸类型**：普通僵尸、路障僵尸、铁桶僵尸、撑杆跳僵尸、橄榄球僵尸、冰车僵尸等，继承自 `Zombie` 基类。
- **战斗机制**：子弹碰撞检测、僵尸受伤与死亡动画、小推车保底机制、阳光生成与消耗。
- **动画系统**：自定义 `Animate` 类，支持位移、缩放、透明度、缓动曲线、动画完成回调等。
- **音效管理**：`SoundManager` 统一管理背景音乐与短音效播放。
- **场景流转**：开始界面 → 关卡选择 → 游戏场景 → 胜利/失败结算，支持返回开始界面。
- **商店与卡牌**：通过商店购买植物卡牌，拖拽种植，铲子移除植物。
- **指导版特色**：Scary大人对话与事件系统、献祭机制、Boss 特殊技能等。

---

## 技术栈

| 项目 | 说明 |
|------|------|
| 语言 | C++17 |
| 框架 | Qt6（Widgets、Multimedia） |
| 构建工具 | CMake ≥ 3.16 |
| 图形系统 | QGraphicsScene / QGraphicsObject |
| 动画 | QPropertyAnimation + 自定义 Animate 封装 |
| 音效 | QMediaPlayer + QAudioOutput |

---

## 项目结构

```
PlantsVsZombies-homework-/
├── CMakeLists.txt              # CMake 构建配置
├── main.cpp                    # 程序入口（MainScene）
├── mainscene.h / .cpp / .ui    # 主场景（开始界面）
├── gamescene.h / .cpp          # 游戏核心场景（QGraphicsScene 子类）
├── myobj.h / .cpp              # 所有游戏对象的基类（QGraphicsObject 子类）
├── plant.h / .cpp              # 植物基类
├── peashooter.h / .cpp         # 豌豆射手
├── sunflower.h / .cpp          # 向日葵
├── wallnut.h / .cpp            # 坚果墙
├── snowpeashooter.h / .cpp     # 寒冰射手
├── cherrybomb.h / .cpp         # 樱桃炸弹
├── potatomine.h / .cpp         # 土豆雷
├── scaredyshroom.h / .cpp      # 胆小菇
├── zombie.h / .cpp             # 僵尸基类
├── nomalzombie.h / .cpp        # 普通僵尸
├── conezombie.h / .cpp         # 路障僵尸
├── bucketzombie.h / .cpp       # 铁桶僵尸
├── screenzombie.h / .cpp       # 撑杆跳僵尸
├── footballzombie.h / .cpp     # 橄榄球僵尸
├── zomboni.h / .cpp            # 冰车僵尸
├── bullet.h / .cpp             # 子弹类
├── plantarea.h / .cpp          # 种植区域（网格格）
├── mower.h / .cpp              # 小推车
├── shop.h / .cpp               # 商店
├── sunlight.h / .cpp           # 阳光
├── card.h / .cpp               # 植物卡牌
├── cardscontainer.h / .cpp     # 卡牌容器
├── selectplant.h / .cpp        # 选择植物面板
├── shovel.h / .cpp             # 铲子
├── animate.h / .cpp            # 动画封装类
├── soundmanager.h / .cpp       # 音效管理器
├── gameleveldata.h / .cpp      # 关卡数据
├── selectlevelwidget.h / .cpp  # 关卡选择界面
├── startwidget.h / .cpp        # 开始控件
├── settingsmenu.h / .cpp       # 设置菜单
├── dominator.h / .cpp          # 游戏进程控制器（Scary大人实现）
├── varydominator.h / .cpp      # 可变控制器（更多关卡实现）
├── flagmeter.h / .cpp          # 波次进度条
├── coordinate.h / .cpp         # 坐标辅助
├── dialogbox.h / .cpp          # 对话框（Scary大人对话框）
├── trophy.h / .cpp             # 奖杯（胜利结算）
├── res.qrc                     # Qt 资源文件
└── GameRes/                    # 游戏资源（图片、音频等）
```

---

## 核心架构说明

### 对象模型

所有游戏实体均继承自 `MyObject`（`QGraphicsObject` 子类），通过 `Type` 枚举区分植物、僵尸、子弹、小推车等类别。`MyObject` 内置 `QMovie` 用于 GIF 动画播放，并通过 `CheckMeet()` 实现碰撞检测。

### 场景管理

`GameScene` 继承自 `QGraphicsScene`，统筹管理所有游戏元素：

- 植物区域（`QList<PlantArea*>` 和 `QList<QList<PlantArea*>>` 二维网格）
- 僵尸集合（按行分组的 `QMap`）
- 商店、铲子、阳光生成、小推车、波次计时等
- 音效播放（`QMediaPlayer` + `SoundManager`）

### 动画系统

`Animate` 类对 `QPropertyAnimation` 进行了链式封装，支持：

```cpp
Animate(target).move(QPointF(x, y)).duration(AnimationType::Move, 500).finish(AnimationType::Move, [](bool){ /* 回调 */ });
```

### 关卡数据

`GameLevelData` 基类提供关卡配置接口，`GameLevelData_1` 等子类定义具体关卡的僵尸波次、植物可用性等。

### 重要类简介

| 类名 | 说明 |
|------|------|
| `GameScene` | 对 Qt 场景类的封装，处理大部分元素的交互 |
| `SelectLevelWidget` | 选择关卡的页面封装类，继承 `QWidget` |
| `MyObject` | 对 `QObject` 的封装，是后面大部分元素的基类 |
| `Plant` | 植物基类 |
| `Zombie` | 僵尸基类 |
| `Bullet` | 子弹基类 |
| `Card` | 卡片类，用来封装植物卡牌 |
| `CardContainer` | 卡牌容器基类，`Shop`、`SelectPlant` 继承自它 |
| `Shop` | 游戏内选择植物的框 |
| `SelectPlant` | 游戏开始前选择可选植物的选择板 |
| `PlantArea` | 种植地类，可以当作“花盆” |
| `Dominator` | Scary大人的实现 |
| `DialogBox` | Scary大人的对话框 |
| `VaryDominator` | 由于 `Dominator` 类作为基类需要实现事件，导致单文件代码很长，于是将更多关卡的 `Dominator` 实现转移到 `VaryDominator`，方便代码管理。不同 `Dominator` 只需实现基类的两个虚函数即可 |
| `FlagMeter` | 游戏进度条 |
| `Zomboni` | 冰车僵尸，也就是 Boss（目前就一个 Boss） |
| `Animate` | 用来实现 `MyObject` 的链式动画 |
| `Coordinate` | 用来实现行列情况到坐标的映射变换，方便定位 |
| `GameLevelData` | 用来存储关卡信息，实例化作为参数传给 `GameScene` 的构造函数 |
| `SoundManager` | 处理音效的类 |

---

## 构建要求

- **CMake** ≥ 3.16
- **Qt6**（或 Qt5），需包含以下模块：
  - `Qt6::Widgets`
  - `Qt6::Core`
  - `Qt6::Multimedia`
- 支持 C++17 的编译器（GCC / Clang / MSVC / MinGW）

---

## 构建与运行

### 1. 克隆仓库

```bash
git clone https://github.com/lcdmiku/PlantsVsZombies-homework-.git
cd PlantsVsZombies-homework-
```

### 2. 使用 CMake 构建

```bash
mkdir build && cd build
cmake ..
cmake --build . --config Release
```

### 3. 运行

构建完成后，可执行文件 `Game`（Linux/macOS）或 `Game.exe`（Windows）会生成在 `build/` 目录下：

```bash
./Game
```

> Windows 下若提示缺少 Qt 动态库，请将 Qt 的 `bin` 目录加入 `PATH`，或使用 `windeployqt` 自动部署。

---

## 已知问题与注意事项

- 部分类（如 `animate.h` 中的 `Animate`）存在未完全解决的 bug，但基本功能可用。
- `soundManager` 相关提交显示音效管理仍在迭代中。
- 文本对话功能曾尝试实现但存在遗留问题。
- 项目为课程设计作业，代码风格与架构可能一般。

---

## 作者

- **lcdmiku** — [GitHub 主页](https://github.com/lcdmiku)

## 致谢

- 参考 B 站 UP 主“头牛o”的《植物大战僵尸指导版》，感谢其创意启发。
- 感谢队友在角色设定、关卡设计等方面的协作与支持。
- 感谢 Qt 社区提供的优秀框架与文档。
