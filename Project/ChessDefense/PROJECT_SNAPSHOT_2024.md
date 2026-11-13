# ChessDefense 项目快照 - 2024

## 项目概况
- **类型**：3D 单机 PVE 卡牌塔防游戏
- **技术栈**：Godot 4.x + C++ GDExtension
- **路径**：`D:\Program\Cpp\Project\ChessDefense`
- **当前版本**：阶段五点五完成

## 核心系统实现状态

### ✅ 已完成系统

#### 基础战斗系统
- 3D 棋盘网格（可配置尺寸：3-20 格）
- 8 种地形类型（普通/雪地/高台/低谷/障碍等）
- 阻挡机制（敌人碰撞干员，超出阻挡数继续前进）
- 攻击系统（近战快速黄条/远程慢速蓝球）
- 点选查看（慢动作 + 属性面板）
- 波次管理（路径预览 → 刷怪 → 间隔）

#### 关卡系统
- **关卡 1**：5x5 单路径（教学关卡）
- **关卡 2**：6x5 双路径（进阶挑战）
- **教程关卡**：4x3 强制引导（7 步分步教学）
- **选关界面**：关卡跳转 + 开发者模式入口

#### 卡牌系统
- **卡牌仓库**：
  - 图鉴展示（等阶边框着色）
  - 碎片合成（6 等阶，真实抽卡池）
  - 卡牌详情（立绘 + 攻击范围可视化 + 属性面板）
  - 升级/升星系统
  - 碎片折兑（高→低等阶按比例）
- **手牌配置**：
  - 植物大战僵尸风格选卡界面
  - 飞行动画（点击卡牌飞到槽位，点击槽位飞回）
  - 支持重复选同一张卡
  - 7 槽位（后期可扩展到 16）
- **存档系统**：卡牌拥有状态、等级、星级、碎片数量存储

#### 关卡编辑器（阶段五核心成果）
- **地形编辑**：
  - 5 种地形明显可区分（草地绿、冰蓝白、高台黄、深绿低谷、近黑障碍）
  - 人性化笔刷：点击立即涂抹起始格，拖拽涂抹路径
  - 避免重复涂抹（drag_painted 数组记录）
- **路径绘制**：
  - 支持多条路径
  - 拖拽连续绘制
  - 节点序号显示（P1-1, P2-3 等）
- **出怪点/基地标记**：
  - 点击切换标记/取消
  - 支持多个出怪点和多个基地
  - 列表实时显示坐标
- **波次配置**：
  - 表格编辑怪物类型（普通/快跑/坦克）
  - 配置数量、出怪间隔、波次间隔
  - 选中波次即时编辑，保存后列表刷新
- **导出/导入**：
  - JSON 格式保存完整关卡数据
  - 支持地图尺寸、地形、路径、出怪点、基地（多基地）、波次
  - 向后兼容旧格式（单基地 → 多基地自动转换）
- **预览模式**：
  - 路径高亮闪烁动画
  - 实时地图信息统计（尺寸/路径数/出怪点/波次/总敌人数）
- **UI 优化**：
  - 左侧面板整体可滚动（LeftScrollContainer 包裹）
  - 标题和右侧网格固定位置，不受标签页内容影响
  - TabContainer 高度自然扩展，不撑开父容器
- **已生成关卡数据**：
  - `game/levels/level_1.json`
  - `game/levels/level_2.json`

#### 开发者工具（阶段五点五）
- **选项菜单**：`options_menu.tscn`
  - 开发者模式开关（存储在 `user://settings.cfg`）
  - 清除存档功能（二次确认对话框）
- **权限控制**：非开发者模式下自动隐藏关卡编辑器按钮
- **SaveManager 扩展**（C++）：
  - `delete_save(slot)` - 删除指定存档
  - `get_dev_mode()` / `set_dev_mode(enabled)` - 开发者模式管理

#### 通用工具
- **LevelLoader**（`level_loader.gd`）：
  - 静态类方法 `load_level_from_json(json_path, board)`
  - 自动应用地形、出怪点到 ChessBoard
  - 返回路径和波次数据供 WaveManager 使用

### 🔄 待优化系统

#### 近期可做
- 美术资源替换（模型、特效、UI）
- 数值平衡（费用/血量/攻击）
- 卡牌多样性扩展（目标 10-15 张）
- 关卡 3 设计

#### 中期计划
- 技能树系统（长线成长、全局 Buff）
- 更多敌人类型
- 音效和音乐完善

## 技术架构

### C++ GDExtension 模块
```
src/
├── chess_board.h/cpp       # 可配置尺寸/地形棋盘
├── chess_piece.h/cpp       # 阻挡/攻击/移动
├── card_stats.h/cpp        # 卡牌属性数据
├── wave_manager.h/cpp      # 单路径波次管理
├── battle_manager.h/cpp    # 战斗/经济管理
├── player_controller.h/cpp # 相机/部署/点选
├── projectile.h/cpp        # 子弹
├── save_manager.h/cpp      # 存档管理（含开发者模式）
└── register_types.cpp      # GDExtension 注册
```

### GDScript 场景
```
game/
├── MainLevel_1.tscn        # 关卡 1
├── MainLevel_2.tscn        # 关卡 2
├── tutorial.tscn           # 新手教程
├── level_editor.tscn/gd    # 关卡编辑器（核心工具）
├── level_loader.gd         # JSON 加载器
├── options_menu.tscn/gd    # 选项菜单
├── deck_config.tscn/gd     # 手牌配置
├── card_depot.tscn/gd      # 卡牌仓库
├── level_init.gd           # 动态手牌卡槽生成
├── tutorial_guide.gd       # 分步引导系统
└── ...（其他配置脚本）
```

### 数据格式

#### 关卡 JSON 格式
```json
{
  "size": [宽, 高],
  "terrain": [[地形类型矩阵]],
  "paths": [[[路径1坐标]], [[路径2坐标]]],
  "spawn_points": [[出怪点坐标]],
  "base": [兼容单基地],
  "bases": [[多基地坐标]],
  "waves": [
    {
      "enemy_type": 0/1/2,
      "count": 数量,
      "spawn_interval": 出怪间隔,
      "wave_interval": 波次间隔
    }
  ]
}
```

#### 存档格式（user://save_1.cfg）
```ini
[player]
level_unlocked = 3
tutorial_completed = true
owned = ["operator", "queen"]
level_operator = 1
shards_operator = 50
stars_operator = 0
universal_shards_white = 1000
...（其他碎片和卡牌数据）

[settings]  # user://settings.cfg
dev_mode = true
```

## 关键设计决策

### 坐标系统
- **左上角为原点** `(0, 0)`
- **右下角为最大值** `(columns-1, rows-1)`
- 例：5x6 地图，左上 `(0,0)`，右下 `(4,5)`

### 笔刷逻辑（关卡编辑器）
1. 鼠标按下 `button_down`：记录起始格子，立即涂抹
2. 鼠标拖拽 `mouse_entered`：涂抹经过的格子（避免重复）
3. 鼠标松开 `button_up`：清除拖拽状态

### 布局方案（关卡编辑器）
```
MainContainer (HBoxContainer)
├── LeftScrollContainer (整体滚动)
│   └── LeftPanel (VBoxContainer, size_flags_vertical=0)
│       ├── 标题
│       ├── 地图尺寸
│       ├── TabContainer (size_flags_vertical=0, 自然扩展)
│       ├── 操作按钮
│       └── 预览信息
└── GridScrollContainer (右侧网格，完全独立)
```

## 编译与测试

### 编译流程
```cmd
cd /d d:\Program\Cpp\Project\ChessDefense
build.bat
```
注意：编译前关闭 Godot 编辑器

### 测试流程
1. 运行游戏（F5）
2. 主菜单 → 选项 → 开启开发者模式
3. 返回 → 开始游戏 → 选关
4. 点击"关卡编辑器"按钮进入

## 文档索引
- `CONTEXT_FOR_NEW_SESSION.md` - 项目总览和当前进度
- `ROADMAP.md` - 开发计划和优先级
- `PROJECT_DESIGN.md` - 核心设计文档
- `NEW_SESSION_START.md` - 新对话启动指令

## 下一步建议

### 高优先级
1. 美术资源替换（立绘、模型、特效）
2. 数值平衡调优
3. 关卡 3 设计（新机制引入）

### 中优先级
4. 卡牌扩展（10-15 张，形成流派）
5. 技能树系统（长线成长）
6. 音效和音乐完善

### 低优先级
7. 成就系统
8. 剧情/世界观补充
9. 多语言支持

---

**项目状态**：阶段五、五点五全部完成，核心工具链完备，可进入内容扩展和美术替换阶段。
