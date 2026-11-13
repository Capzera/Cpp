# SmartGround 开发者快速上手指南

## 项目概览
SmartGround 是一个面向 CSP-J/S 和 NOIP 教学的 C++ Qt6 算法可视化沙盒系统，采用"录制-播放"分离的 MVC 架构。

## 核心架构

### 1. 运行机制（三阶段）
**录制阶段（Recording）**
- 用户点击"计算"按钮
- 调用 `IModule::runAlgorithm()`
- 算法在竞赛风格代码中瞬间执行完毕
- 每一步状态变化被封装为 `AnimFrame` 压入 `m_frames` 队列

**播放阶段（Playback）**
- 用户点击"单步执行"或"自动播放"
- UI 调度器从 `m_frames` 取出当前帧
- 调用 `IModule::renderFrame(const AnimFrame& frame)` 解析并播放动画
- 通过 Qt 动画组实现平滑视觉效果

**撤销阶段（Undo）**
- 用户点击"上一步"
- 清空画布，关闭动画锁 `m_animated = false`
- 用 for 循环瞬间重绘 0 到 i-1 帧
- 重新开启动画锁，实现 PPT 级无感回退

### 2. 核心数据结构

#### AnimFrame（动画帧）
```cpp
struct AnimFrame {
    ActionType type;           // 动作类型枚举
    int u, v;                  // 参与动作的目标图元 ID
    int val1, val2;            // 附加数值（权值/状态值）
    std::vector<int> group;    // 批量操作的 ID 集合
    QString explanation;       // UI 底部状态栏文字
    QString mathText;          // 画布中央显示的公式/推导
};
```

#### ActionType（动作类型）
采用模块前缀分类：
- `GN_*` 图论相关（HIGHLIGHT_NODE, TRAVERSE_EDGE, UPDATE_LOW, PUSH_STACK, POP_STACK, CONDENSE, MARK_CUT）
- `AR_*` 数组相关（HIGHLIGHT, SWAP, RESET, DONE）
- `PD_*` 前缀和/差分（HIGHLIGHT, UPDATE_VAL, RESET, MATH_TEXT）
- `SQ_*` 栈/队列（POINTER_MOVE, UPDATE_VAL, CLEAR_VAL）
- `BS_*` 二分查找（POINTER_MOVE, HIGHLIGHT_MID, DISCARD_RANGE, FOUND, NOT_FOUND）

### 3. 核心视觉组件（GraphElements）

**图论组件**
- `GraphNode` & `GraphEdge`: 用于图论、树、线段树、字典树
- `StackBlock`: 用于 Tarjan 算法的栈可视化

**数组组件**
- `ArrayBlock`: 60x40 长方形块，包含数值与底部下标
- `PointerBlock`: 带箭头的悬浮指针（如 L, R, mid, top, head）

**特效组件**
- `FloatingText`: 数据转移时的悬浮抛物线飞入效果

`IModule::playValueFlight` 用于复制式值传递：源图元保留原值，数值飞入目标数组格。队列入队和 DP 候选状态应复用该公共动画；动画关闭时直接更新目标。

### 4. IModule 接口规范

所有算法模块必须实现以下接口：

```cpp
class IModule {
public:
    bool m_animated = true;  // 动画锁
    
    virtual QString getModuleName() const = 0;
    virtual QString getDefaultInput() const = 0;
    virtual void buildLayout(QGraphicsScene* scene, const QString& inputData) = 0;
    virtual std::vector<AnimFrame> runAlgorithm() = 0;
    virtual void renderFrame(const AnimFrame& frame) = 0;
    virtual void reset() = 0;
};
```

## 模块开发流程

### 步骤 1：提出并确认动画分镜
- 说明游标、辅助图元、标签、颜色语义和关键帧。
- 获得用户确认后，再编写模块代码。

### 步骤 2：创建模块骨架
```cpp
// XxxModule.h
#ifndef XXXMODULE_H
#define XXXMODULE_H
#include "IModule.h"
#include "render/GraphElements.h"
#include <vector>
#include <QObject>

class XxxModule : public QObject, public IModule {
    Q_OBJECT
public:
    XxxModule();
    QString getModuleName() const override;
    QString getDefaultInput() const override;
    void buildLayout(QGraphicsScene* scene, const QString& inputData) override;
    std::vector<AnimFrame> runAlgorithm() override;
    void renderFrame(const AnimFrame& frame) override;
    void reset() override;
private:
    std::vector<AnimFrame> m_frames;
    // 模块特定成员变量
};
#endif
```

- 派生类不得声明 m_scene；在 buildLayout 中将传入的 scene 赋值给 IModule 基类指针。

### 步骤 3：实现 buildLayout
- 解析输入数据
- 创建并定位图元到 scene
- 初始化模块状态

### 步骤 4：实现 runAlgorithm（核心）
- 使用竞赛风格代码实现算法逻辑
- 在关键状态变化处插入 `m_frames.push_back({...})`
- 返回完整帧序列

### 步骤 5：实现 renderFrame
- 根据 `frame.type` 分支处理
- 当 `m_animated == true` 时创建 QPropertyAnimation
- 当 `m_animated == false` 时直接修改属性（用于撤销重绘）

### 步骤 6：实现 reset
- 清空所有容器
- 重置指针为 nullptr
- 准备下一次 buildLayout

### 步骤 7：注册模块
在 `MainWindow.cpp` 构造函数中：
```cpp
#include "modules/XxxModule.h"
// ...
registeredModules.push_back(new XxxModule());
```

在 `CMakeLists.txt` 中：
```cmake
add_executable(SmartGround
    # ...
    src/modules/XxxModule.cpp
)
```

## 代码规范强调

### 竞赛风格模板
```cpp
// 正确：静态数组模拟链表
int head[N], to[N], nxt[N], idx;
void add(int u, int v) {
    to[idx] = v; nxt[idx] = head[u]; head[u] = idx++;
}

// 避免：STL容器（除非 map/set 这种不可替代的场景）
std::vector<int> adj[N];  // 非竞赛风格
```

### 无注释 + 极度紧凑
```cpp
void tarjan(int u) {
    dfn[u] = low[u] = ++timestamp;
    stk[++top] = u; ins[u] = true;
    m_frames.push_back({ActionType::GN_HIGHLIGHT_NODE, u, 0, dfn[u], low[u], {}, QString("访问节点%1").arg(u)});
    for (int i = head[u]; i; i = nxt[i]) {
        int v = to[i];
        if (!dfn[v]) {
            tarjan(v);
            low[u] = std::min(low[u], low[v]);
        } else if (ins[v]) {
            low[u] = std::min(low[u], dfn[v]);
        }
    }
}
```

### 动画锁机制
```cpp
void XxxModule::renderFrame(const AnimFrame& step) {
    if (step.type == ActionType::AR_SWAP) {
        ArrayBlock* b1 = m_blocks[step.u];
        ArrayBlock* b2 = m_blocks[step.v];
        if (m_animated) {
            // 创建动画对象
            QPropertyAnimation* anim = new QPropertyAnimation(b1, "pos");
            anim->setDuration(400);
            anim->start(QAbstractAnimation::DeleteWhenStopped);
        } else {
            // 直接修改属性（撤销重绘）
            b1->setPos(targetPos);
        }
    }
}
```

## 调试技巧

### 1. 验证帧序列
在 `runAlgorithm()` 返回前打印 `m_frames.size()`

### 2. 单帧调试
在 `renderFrame()` 入口打印 `frame.type` 和 `frame.explanation`

### 3. 动画锁测试
- 正常播放：检查动画是否流畅
- 点击"上一步"：检查是否瞬间重绘无延迟

## 常见陷阱

### 陷阱 1：忘记处理 m_animated = false
导致撤销时动画仍然播放，画面卡顿

### 陷阱 2：ActionType 前缀冲突
不同模块使用了相同的枚举值，导致渲染混乱

### 陷阱 3：静态数组未清零
在 `reset()` 或 `buildLayout()` 中忘记 `memset` 或重新初始化

### 陷阱 4：scene 指针管理
图元添加到 scene 后，scene 负责内存管理，不要手动 delete

## 每次接手前
阅读 docs/ProjectSummary.md、Cursor_Workflow.md、docs/DevGuide.md、docs/ModuleStatus.md、docs/outlines.md 和 docs/BugLog.md。Codex 与 Cursor 还应读取根目录 AGENTS.md 和 .cursor/rules/smartground-project.mdc。
