# Plants vs. Zombies (Qt/C++ 课程设计)

一个基于 **Qt6** 框架、使用 **C++17** 开发的植物大战僵尸游戏复刻项目，采用 CMake 构建。项目实现了完整的游戏循环、多种植物与僵尸类型、关卡系统、动画系统、音效管理以及商店/阳光/小推车等核心机制，适合作为 Qt 图形界面编程的学习参考。

## 功能特性

- **多关卡系统**：内置多个关卡数据，支持关卡选择界面。
- **植物类型**：豌豆射手、向日葵、坚果墙、樱桃炸弹、土豆雷、寒冰射手等，均继承自 `Plant` 基类。
- **僵尸类型**：普通僵尸、路障僵尸、铁桶僵尸、撑杆跳僵尸、橄榄球僵尸、冰车僵尸等，继承自 `Zombie` 基类。
- **战斗机制**：子弹碰撞检测、僵尸受伤与死亡动画、小推车保底机制、阳光生成与消耗。
- **动画系统**：自定义 `Animate` 类，支持位移、缩放、透明度、缓动曲线、动画完成回调等。
- **音效管理**：`SoundManager` 统一管理背景音乐与短音效播放。
- **场景流转**：开始界面 → 关卡选择 → 游戏场景 → 胜利/失败结算，支持返回开始界面。
- **商店与卡牌**：通过商店购买植物卡牌，拖拽种植，铲子移除植物。

## 技术栈

| 项目 | 说明 |
|------|------|
| 语言 | C++17 |
| 框架 | Qt6（Widgets、Multimedia） |
| 构建工具 | CMake ≥ 3.16 |
| 图形系统 | QGraphicsScene / QGraphicsObject |
| 动画 | QPropertyAnimation + 自定义 Animate 封装 |
| 音效 | QMediaPlayer + QAudioOutput |

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
├── dominator.h / .cpp          # 游戏进程控制器
├── varydominator.h / .cpp      # 可变控制器
├── flagmeter.h / .cpp          # 波次进度条
├── coordinate.h / .cpp         # 坐标辅助
├── dialogbox.h / .cpp          # 对话框
├── trophy.h / .cpp             # 奖杯（胜利结算）
├── res.qrc                     # Qt 资源文件
└── GameRes/                    # 游戏资源（图片、音频等）
```

## 构建要求

- **CMake** ≥ 3.16
- **Qt6**（或 Qt5），需包含以下模块：
  - `Qt6::Widgets`
  - `Qt6::Core`
  - `Qt6::Multimedia`
- 支持 C++17 的编译器（GCC / Clang / MSVC / MinGW）

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

## 已知问题与注意事项

- 部分类（如 `animate.h` 中的 `Animate`）存在未完全解决的 bug，但基本功能可用。
- `soundManager` 相关提交显示音效管理仍在迭代中。
- 文本对话功能曾尝试实现但存在遗留问题。
- 项目为课程设计作业，代码风格与架构可能一般


## 作者

- **lcdmiku** — [GitHub 主页](https://github.com/lcdmiku)

