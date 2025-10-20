# 🐍 Gluttonous Snake (贪吃蛇游戏)

![License](https://img.shields.io/badge/license-MIT-blue.svg)
![Platform](https://img.shields.io/badge/platform-Windows-lightgrey.svg)
![Language](https://img.shields.io/badge/language-C%2B%2B-orange.svg)

## 项目简介

一个基于 C++ 和 EasyX 图形库开发的贪吃蛇小游戏，采用面向对象设计。<br>
实现了多种游戏模式（单人模式、人机对战、双人对战）、A* 寻路算法、复杂地形（加速区、减速区、软硬墙壁）、食物机制（四种食物类型影响生命、体能和分数）以及完整的游戏系统（生命值、体能值、速度控制、排行榜记录），可以查找与显示游戏的结果记录，界面提供实时UI，显示蛇的长度、剩余生命数、当前分数、历史最高分、游戏时间等内容。<br>
（1）区分软墙壁和硬墙壁，各自造成不同伤害。<br>
（2）地图划分加速区和减速区。<br>
（3）设置体能槽以及体能计算规则。<br>
（4）食物随机出现“精灵果”和“恶果”，精灵果具有正面效果，恶果具有负面效果。<br>
（5）扩大地图边界，屏幕只显示一小块地图，屏幕外有隐藏区域，镜头可以跟随蛇移动。<br>
（6）实现人机对战，编写基于 A* 算法的决策逻辑。<br>
（7）实现双人对战，可以同时采用键盘控制两条蛇。<br>
（8）使用 EasyX 的接口编程图形程序实现图形化。<br>

---

## 快速开始

### 系统要求
- **操作系统**：Windows 7 及以上
- **编译器**：Visual Studio 2015 或更高版本（支持 C++11）
- **图形库**：EasyX Graphics Library

### 编译运行

1. **克隆项目**
   ```bash
   git clone https://github.com/GalxyX/Gluttonous-Snake.git
   cd Gluttonous-Snake
   ```

2. **安装 EasyX**：访问 [EasyX 官网](https://easyx.cn/) 下载并安装图形库

3. **编译游戏**：使用 Visual Studio 打开 `贪吃蛇.sln`，编译

4. **运行游戏**：启动游戏，确保 `image` 文件夹与可执行文件在同一目录

## 游戏玩法与规则

| 模式         | 玩法           | 说明                                           |
| ------------ | -------------- | ---------------------------------------------- |
| **单人模式** | 经典贪吃蛇玩法 | 可选地图大小、蛇的颜色，三种难度级别，成绩记录 |
| **人机对战** | 与 AI 竞技     | AI 采用 A* 算法智能寻路                        |
| **双人对战** | 本地同屏对战   | 玩家1(WASD) vs 玩家2(方向键)                   |

### 游戏元素

**地形系统**
- 普通地面：标准速度移动
- 快速区域：移动速度提升
- 缓慢区域：移动速度降低

**障碍物**
- 软墙壁：碰撞扣除 2 点生命值，-2 分
- 硬墙壁：碰撞扣除一半生命值，-3 分

**食物类型**
- 坏食物：-1 生命，-4 分
- 普通食物：+2 分，+50 体能
- 好食物：+4 分，+80 体能
- 极品食物：+6 分，+100 体能，+1 生命

### 游戏系统
- 生命值系统：不同难度初始生命不同，耗尽游戏结束
- 体能值系统：初始 400 点，移动消耗，吃食物恢复
- 得分机制：根据食物类型和游戏表现计分
- 排行榜：记录各难度最高分，持久化存储

### 基本操作
- 移动控制：方向键 ↑↓←→ 或 WASD 键
- 暂停/继续：空格键或点击暂停按钮
- 菜单选择：鼠标点击

## 项目结构

```
Gluttonous-Snake/
│
├── 贪吃蛇.sln                      # Visual Studio 解决方案
├── LICENSE                        # MIT 许可证
├── README.md                      # 项目文档
└── Gluttonous_Snake/              # 源代码目录
    │
    ├── 头文件
    │   ├── Parameter.h            # 全局参数和常量定义
    │   ├── snake_main.h           # 主程序接口
    │   └── snake_class.h          # 核心类声明
    │
    ├── 实现文件
    │   ├── snake_main.cpp         # 程序入口和主循环
    │   ├── snake_menu.cpp         # 菜单系统（主菜单、难度选择、排行榜）
    │   ├── snake_class.cpp        # 核心类实现（Map、Snake、Food）
    │   ├── snake_localgame.cpp    # 游戏逻辑（单人、双人、人机）
    │   └── snake_AIbrain.cpp      # AI 算法（A* 寻路）
    │
    └── 资源文件 (image/)
        ├── 界面：menu_bk.png、help.png、info_bk.png
        ├── 蛇头：snake_head[1-3]_[up/down/left/right].png
        ├── 蛇身：snake_body[1-3].png
        ├── 食物：bad/simple/good/excellent_food.png
        ├── 墙体：soft/hard_wall.png
        └── 地形：fast/slow_region.png
```

### 核心文件说明

| 文件                  | 功能描述                                                        |
| --------------------- | --------------------------------------------------------------- |
| `Parameter.h`         | 定义窗口大小、颜色常量、游戏模式等全局参数                      |
| `snake_class.h/.cpp`  | 实现 Map（地图）、Snake（蛇）、Food（食物）、BasicImg（图像）类 |
| `snake_main.cpp`      | 程序入口，游戏主循环，模式切换                                  |
| `snake_menu.cpp`      | 主菜单、难度选择、排行榜、帮助界面                              |
| `snake_localgame.cpp` | 单人/双人/人机模式的游戏逻辑和暂停功能                          |
| `snake_AIbrain.cpp`   | A* 寻路算法，食物价值评估，AI 决策                              |
