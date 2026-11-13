# 如何开启新对话并恢复进度

## 1. 开启新对话
在 Cursor 编辑器中：
1. 按 `Ctrl + L`（或点击侧边栏的 Chat 图标）
2. 点击右上角的 **"+"** 号或 **"New Chat"**
3. 新对话会清空上下文，重新开始计数 token

## 2. 恢复进度模板（复制粘贴用）

### 方式一：简洁版（推荐日常开发）
```
你好，我是 ChessDefense 项目的开发者，这是一个 Godot 4.x + C++ GDExtension 的 3D 塔防游戏。

项目路径：D:\Program\Cpp\Project\ChessDefense

**当前进度**：
- 关卡 1、2 已完成（MainLevel_1.tscn / MainLevel_2.tscn）
- 阻挡、子弹、点选查看、地形、音效接口已实现
- 选关跳转和结算面板正常工作

**坐标约定**：
我描述布局时用 1-based 行列坐标，左上角 (1,1)。
代码内部用 0-based，左上角 (0,0)。
详见 COORDINATE_CONVENTION.md。

**本次任务**：
[在这里填写你要做的事，例如：]
- 完成关卡 3 的设计和实现
- 或：优化第二关的波次难度
- 或：增加新的近战卡牌

需要了解哪些文件或系统？
```

### 方式二：完整版（适合长期中断后恢复）
```
你好，我是 ChessDefense 项目的开发者，需要恢复之前的开发进度。

**项目基本信息**：
- 名称：ChessDefense
- 类型：3D 单机 PVE 卡牌塔防
- 技术栈：Godot 4.x + C++ GDExtension
- 路径：D:\Program\Cpp\Project\ChessDefense

**核心文件结构**：
```
src/                   # C++ GDExtension
  ├── chess_board.*    # 棋盘（可配置尺寸/地形）
  ├── chess_piece.*    # 棋子（阻挡/攻击/移动）
  ├── wave_manager.*   # 单路径波次管理器
  └── battle_manager.* # 战斗/经济管理
game/
  ├── MainLevel_1.tscn # 关卡 1（5x5 单路径）
  ├── MainLevel_2.tscn # 关卡 2（5x6 双路径）
  ├── level_config_*.gd# 地形配置
  └── wave_manager_2.gd# 关卡 2 波次控制
```

**当前完成内容**：
- 阶段一：第一关体验完成（5 波、阻挡、子弹、点选、地形、音效）
- 阶段二：关卡 1、2 已实现，选关跳转正常

**坐标系统约定（重要）**：
- 我描述布局时：1-based 行列坐标，左上 (1,1)
- 代码实现时：0-based，左上 (0,0)
- 详细规则见 COORDINATE_CONVENTION.md

**待完成任务**（按优先级）：
1. 关卡 3 设计与实现
2. 新手教程
3. 卡牌仓库/碎片系统
4. 关卡编辑器
5. 美术替换

**本次具体任务**：
[在这里详细描述本次要做什么]

**需要你先做的**：
请先阅读以下文档确认项目状态：
- CONTEXT_FOR_NEW_SESSION.md（项目概览）
- COORDINATE_CONVENTION.md（坐标约定）
- ROADMAP.md（开发计划）

然后告诉我你理解了哪些关键点，我们再开始本次任务。
```

## 3. 实战示例

### 示例 A：做关卡 3
```
你好，ChessDefense 项目恢复开发。

项目路径：D:\Program\Cpp\Project\ChessDefense
已完成：关卡 1、2 + 核心机制

**本次任务：设计并实现关卡 3**
- 棋盘尺寸：6x6
- 特色机制：[待讨论，例如多波同时出怪 / BOSS 波]
- 出怪点、路径：[待设计]

我会先描述关卡布局（用 1-based 坐标），你负责：
1. 创建 MainLevel_3.tscn
2. 写 level_config_3.gd 配置地形
3. 写波次控制逻辑（单路径用 WaveManager，多路径写 GDScript）
4. 更新 level_select.gd 的跳转映射

准备好了告诉我。
```

### 示例 B：优化现有关卡
```
ChessDefense 项目，继续优化。

路径：D:\Program\Cpp\Project\ChessDefense
当前：关卡 1、2 可玩

**本次任务：优化第二关难度和体验**
1. 调整波次数量和间隔
2. 增加第 3 波开始出现的"治疗怪"（设计中）
3. 测试通关需要的最低卡牌配置

需要你：
1. 读取 wave_manager_2.gd 当前配置
2. 根据我的反馈调整波次参数
3. 如果要新增怪物，指导我建 enemy_healer.tscn

从读取当前波次配置开始。
```

### 示例 C：增加新卡牌
```
ChessDefense 项目，新增卡牌。

路径：D:\Program\Cpp\Project\ChessDefense

**本次任务：添加"骑士"卡牌**
- 类型：近战
- 阻挡：3
- 攻击范围：前方 2x2
- 费用：20
- 属性：高血高防低攻

需要你：
1. 创建 knight.tscn（参考 operator.tscn）
2. 配置 CardStats（阻挡、攻速、范围）
3. 更新 MainLevel_1.tscn 的卡槽（加第三张卡）
4. 指导我测试并调整数值

从创建 knight.tscn 开始。
```

## 4. 注意事项
- **首次消息控制在 500 字内**：AI 会根据后续对话逐步深入
- **明确本次目标**：别一次提多个任务，专注一个大目标
- **附上关键约定**：坐标系统、编译流程这些每次都要提醒
- **分段确认**：完成阶段性工作后，测试再继续

## 5. 常用文档快速索引
需要时让 AI 读取：
- `CONTEXT_FOR_NEW_SESSION.md`：项目全局概览
- `COORDINATE_CONVENTION.md`：坐标系统约定
- `ROADMAP.md`：开发计划和优先级
- `PROJECT_DESIGN.md`：项目设计文档
- `src/chess_board.h`：棋盘 API
- `src/chess_piece.h`：棋子 API
- `game/MainLevel_1.tscn`：关卡 1 场景结构

## 6. 编译提醒模板
```
编译流程：
1. 关闭 Godot 编辑器
2. 运行 build.bat
3. 编译成功后重启 Godot
4. 刷新文件系统面板

如果报错 DLL 拒绝访问：确认 Godot 已关闭
如果报错 PDB 冲突：已设置 -j 1 单线程编译
```

---

**总结**：
- 开新对话：`Ctrl + L` → 点 "+" 号
- 恢复进度：复制上面模板，填写本次任务，发送
- 关键文档：CONTEXT_FOR_NEW_SESSION.md + COORDINATE_CONVENTION.md
- 每次明确一个目标，分段确认测试

祝开发顺利！有问题随时新开对话问我。
