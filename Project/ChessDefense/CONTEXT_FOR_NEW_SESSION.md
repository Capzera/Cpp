# 项目上下文总结（供新对话使用）

## 项目信息
- **名称**：ChessDefense
- **类型**：3D 单机 PVE 卡牌塔防
- **技术栈**：Godot 4.x + C++ GDExtension
- **路径**：`D:\Program\Cpp\Project\ChessDefense`

## 坐标系统约定（重要）
**棋盘坐标从左上角开始**：
- 左上角 = `(0, 0)`
- 右下角 = `(columns-1, rows-1)`
- 例：5x6 格子，左上 `(0,0)`，右下 `(4,5)`

## 当前进度
- **阶段一**：第一关体验已完成（5 波、阻挡、子弹、点选查看、地形、音效接口）
- **阶段二**：关卡 1、2 已完成
  - `MainLevel_1.tscn`：5x5，单路径
  - `MainLevel_2.tscn`：6x5，双路径
  - 选关跳转映射表已更新
- **阶段三**：新手教程已完成
  - `tutorial.tscn`：4x3 教程地图，1 波怪物
  - `tutorial_guide.gd`：分步引导系统（欢迎、部署、朝向、查看、阻挡、开战、胜利）
  - `SaveManager`：增加 `tutorial_completed` 字段
  - 首次游戏自动跳转教程
- **阶段四**：卡牌仓库与手牌配置系统已完成
  - **卡牌仓库** (`card_depot.tscn` + `card_depot.gd`)：图鉴/合成/详情页，攻击范围可视化
  - **手牌配置** (`deck_config.tscn` + `deck_config.gd`)：植物大战僵尸风格选卡界面，飞行动画
  - **SaveManager 扩展**：卡牌拥有状态、等级、星级、碎片存储
- **阶段五**：关卡编辑器已完成
  - `level_editor.tscn` + `level_editor.gd`：游戏内关卡编辑器
  - 四种编辑模式：地形/路径/出怪点/基地，通过标签页切换
  - 波次配置：表格编辑每波怪物类型、数量、出怪间隔、波次间隔
  - JSON 导出/导入：保存地图尺寸、地形、路径、出怪点、基地（支持多基地）、波次
  - `level_loader.gd`：通用 JSON 加载器，应用到 ChessBoard
  - 关卡迁移：已生成 `game/levels/level_1.json` 和 `level_2.json`
  - 预览模式：路径高亮闪烁 + 实时地图信息统计面板
  - **UI 优化**：
    - 笔刷逻辑：鼠标按下立即涂抹起始格，拖拽涂抹经过的格子
    - 格子颜色：5 种地形明显可区分（草地绿、冰蓝白、高台黄、深绿低谷、近黑障碍）
    - 布局优化：左侧面板整体可滚动，标题和右侧网格固定不跳动
- **阶段五点五**：开发者工具已完成
  - `options_menu.tscn` + `options_menu.gd`：选项菜单（开发者模式开关、清除存档）
  - SaveManager 新增：`delete_save(slot)`、`get_dev_mode()`、`set_dev_mode(enabled)`
  - 非开发者模式下关卡编辑器按钮自动隐藏

## 核心文件结构
```
ChessDefense/
├── src/                        # C++ GDExtension
│   ├── chess_board.h/cpp       # 棋盘（可配置尺寸/地形）
│   ├── chess_piece.h/cpp       # 棋子（阻挡/攻击/移动）
│   ├── card_stats.h/cpp        # 卡牌数据
│   ├── wave_manager.h/cpp      # 单路径波次管理器
│   ├── battle_manager.h/cpp    # 战斗/经济管理
│   ├── player_controller.h/cpp # 相机/部署/点选
│   ├── projectile.h/cpp        # 子弹
│   ├── save_manager.h/cpp      # 存档管理（含 session_deck）
│   └── register_types.cpp      # GDExtension 注册
├── game/
│   ├── MainLevel_1.tscn        # 关卡 1
│   ├── MainLevel_2.tscn        # 关卡 2
│   ├── tutorial.tscn           # 新手教程关卡（4x3，1波）
│   ├── level_editor.tscn/gd    # 关卡编辑器（地形/路径/波次/导出导入）
│   ├── level_loader.gd         # 通用 JSON 关卡加载器（static class）
│   ├── options_menu.tscn/gd    # 选项菜单（开发者模式/清除存档）
│   ├── deck_config.tscn/gd     # 手牌配置界面（植物大战僵尸风格）
│   ├── card_depot.tscn/gd      # 卡牌仓库（图鉴/合成/详情）
│   ├── level_init.gd           # 动态生成手牌卡槽
│   ├── level_select.gd         # 选关跳转（关卡 2+ 先进选卡界面）
│   ├── level_config_1.gd       # 关卡 1 地形配置
│   ├── level_config_2.gd       # 关卡 2 地形配置
│   ├── tutorial_config.gd      # 教程关卡地形配置
│   ├── wave_manager_2.gd       # 关卡 2 双路径波次（GDScript）
│   ├── wave_manager_tutorial.gd# 教程波次管理（1波，等引导解锁）
│   ├── tutorial_guide.gd       # 分步引导系统（对话框+遮罩）
│   ├── hud.gd                  # HUD / 音效
│   ├── settlement_panel.gd     # 结算面板
│   ├── operator.tscn           # 士兵卡（近战，阻挡 2）
│   ├── queen.tscn              # 皇后卡（远程，阻挡 1）
│   ├── enemy.tscn              # 普通敌人
│   ├── enemy_runner.tscn       # 快跑敌人
│   └── enemy_tank.tscn         # 坦克敌人
├── PROJECT_DESIGN.md           # 项目设计文档
├── ROADMAP.md                  # 开发计划
└── build.bat                   # 编译脚本
```

## 编译流程
1. 关闭 Godot 编辑器
2. 运行 `cmd /c "cd /d d:\Program\Cpp\Project\ChessDefense && build.bat"`
3. 编译成功后重启 Godot
4. 刷新文件系统面板

## 关键机制
- **阻挡**：敌人走进干员格子会被挡住，超出阻挡数继续走
- **子弹**：近战黄条快速、远程蓝球慢速
- **点选查看**：左键点棋子进入慢动作，显示属性面板，点空白/右键关闭
- **地形**：雪地减速 ×0.8，高台/低谷视觉区分，障碍不可放置
- **波次**：路径预览 → 刷怪 → 间隔，可在检查器调参数
- **手牌配置**：关卡 1 固定士兵卡，关卡 2+ 从 `deck_config.tscn` 配置手牌，通过 `get_tree().get_meta("session_deck")` 传递

## 常见问题与解决方案
- **GDScript 调用 C++ 方法报错**：检查 `ClassDB::bind_method` 是否注册，确认 DLL 已重新编译
- **DLL 拒绝访问**：关闭 Godot 再编译
- **地形配置不生效**：确认 `build_visuals()` 在设置地形后调用
- **路径不正确**：检查坐标是否左上角 `(0,0)` 起算
- **选卡界面空白**：确保从主菜单正常流程进入（按 F5 运行完整游戏），`main_menu.gd` 会调用 `load_save(1)` 加载存档
- **点击卡牌无响应**：检查子节点 `mouse_filter` 是否设为 `MOUSE_FILTER_IGNORE`

## 待完成任务（按优先级）
1. **当前状态测试**：手牌配置系统功能验证，关卡 2 通关流程
2. **关卡 3**：设计布局、路径、特色机制
3. **关卡编辑器**：地形/路径/波次可视化配置
4. **美术替换**：模型、特效、UI
5. **技能树/长线成长**

## 开发方法论（重要）
- **读代码再改代码**：每次改动前只读必要的函数，不读整个文件
- **不绕路**：避免临时方案叠加，优先修复根本问题
- **改动最小化**：需要加什么就加什么，不顺手重构不相关的部分
- **先问再动**：设计有分歧时先确认，不要猜测需求
- **更新文档**：阶段完成后更新本文件和 `ROADMAP.md`
