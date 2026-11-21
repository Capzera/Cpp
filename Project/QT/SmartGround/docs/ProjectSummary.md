# SmartGround 项目接手摘要

**更新时间：2026-11-21**

## 第 33 批：笛卡尔坐标系新模块（GeoGebra 对标）（2026-11-21，已完成）

PM 决定本批只做一个模块：**笛卡尔坐标系**，功能对标 GeoGebra Web，并给出 12 张参考截图与 7 项明确要求（无限网格、滚轮缩放改变分度尺、第二排画笔工具、画点工具、连线工具、左侧对象列表、函数绘制），另加一条硬指标：**延迟要低，不能只有 20–30 帧**。

### ① 新增四个源文件

| 文件 | 职责 |
|---|---|
| `src/core/CartesianExpression.{h,cpp}` | 递归下降表达式解析器，支持一次 / 二次 / 三次多项式与**隐式乘法**（`2x`、`3x^2`、`(x+1)(x-1)`）；输出 `y = 3x^2 - 8x + 7` 规范式与线段一次方程 `y = kx + b` |
| `src/render/CartesianElements.{h,cpp}` | `CartesianEngine`（坐标换算 / 分度尺阶梯 / 网格吸附 / 锚点缩放）+ 四个原生 `QGraphicsObject` 图元（网格 / 点 / 线段 / 曲线） |
| `src/modules/CartesianModule.{h,cpp}` | 继承 `FreeStructureBase`，模块名 `自由结构 - 笛卡尔坐标系`，`isCartesian()` 供 UI 切换 |
| `src/ui/widgets/CartesianPanel.{h,cpp}` | 左侧对象列表面板：点坐标管理、⋮ 菜单、样式设置卡、`f(x) = 表达式` 录入；含 `CartesianPointEditor` 模态对话框 |

### ② 坐标系内核

- **无限网格 + 视口裁剪**：网格是理论上的无限延伸，只按当前视口绘制；`CartesianGridItem::boundingRect()` 跟随视口并预留 400px 余量，避免超大包围盒拖慢渲染。
- **分度尺阶梯**：滚轮缩放改变 `m_scale`，`syncStepToScale()` 在 `100 / 50 / 20 / 10 / 5 / 2 / 1 / 0.5 / 0.2 / 0.1 / 0.05 / 0.02 / 0.01` 共 13 级阶梯中选出「像素间距 ≤ 62px」的档位。默认 `scale=60` → 分度尺 **1.0**；放大到 `scale=6000` → 分度尺 **0.01**。
- **坐标换算**：`scenePos = (originX + x·scale, originY - y·scale)`；`snapMath()` 把坐标四舍五入到当前分度尺的整数倍，保证描点总是落在**粗线交点**上。
- **锚点缩放**：`zoomBy(factor, anchor)` 保持鼠标下的数学坐标不变 —— 先算锚点数学坐标，改完 `scale` 后反解新原点。
- **平移**：中键拖动改的是 `originScene()` 而非 `QGraphicsView` 滚动条。

### ③ 工具与交互

| 工具 | 图标 | 行为 |
|---|---|---|
| 画点 | 鼠标尖带圆点 | 单击落点并吸附网格交点；**长按拖动时圆点逐帧跟手**（原生事件，非 Qt 属性动画 → 零延迟）；悬停已有点变 `Qt::PointingHandCursor`；双击弹出编辑对话框改**名字 + 坐标**并带「隐藏名字」开关；工具按钮二次点击展开设置卡可切换「描点是否带名字」 |
| 连线 | 直线两端带小圆点 | 奇数次选点高亮起点，偶数次选点连成线段；双击线段显示一次方程 |
| 函数 | 抛物线 | 左侧输入 `f(x) = 表达式`（一次 / 二次 / 三次）；曲线**按可视数学范围自适应绘制**，平移出视口即消失、移回即恢复；支持多条曲线并存 |

### ④ 左侧面板（对应 PM 的图 5–8）

- 每行显示：颜色圆点 + 点名字 + 坐标 + ⋮ 菜单。
- ⋮ 菜单：**编辑名称与坐标** / **设置** / **删除**（**不含「高级」「脚本」**，与 PM 要求一致）。
- 「设置」样式卡：颜色用项目现有的 `MyColorPicker`（**无极变色框架**）、大小用 `MySlider`、形状用 `MySegmentedControl`（圆 / 方 / 菱 / 叉）、隐藏名字用 `MyToggleSwitch`。
- 函数区：`f(x) =` 输入框 + `+` 按钮，输入即校验并给出中文错误提示。

### ⑤ 低延迟保障

所有笛卡尔图形对象（点 / 线段 / 曲线 / 网格）都是**原生 `QGraphicsObject`**，直接接管 `mousePressEvent` / `mouseMoveEvent` / `hoverEnterEvent`，**完全不走 Qt 属性动画**。描点拖动时每帧只做一次 `setMathPos()`（改坐标 + `setPos()` + `update()`），实测拖动与函数曲线重绘均保持流畅。

### ⑥ 过程中修掉的 5 个真实缺陷

| 缺陷 | 根因 | 修复 |
|---|---|---|
| 二次函数**整条曲线消失** | `rebuildPolyline()` 出界时 `m_polyline.clear()` 把已累积顶点全清空 | 改为 `QVector<QPolygonF> m_runs` 多段结构，出界只切断当前段 |
| `2x + 7` / `3x^2 - 8x + 7` **解析失败** | `parseProduct()` 只认显式 `*` | 新增 `startsFactor()` + `multiplyTerms()` 支持隐式乘法 |
| 切走模块后**引擎仍启用 / 退出崩溃** | `releaseSceneItems()` 漏 `setEnabled(false)`；`onModuleChanged()` 里 `currentModule = nullptr` 写在 `clearScene()` **之前** | 补 `setEnabled(false)`；调整两行顺序；析构时先解绑引擎裸指针 |
| `shape()` 返回类型冲突 | 形状编号与 `QGraphicsItem::shape()` 同名 | 形状编号改 `pointShape()`；`shape()` 恢复标准 `QPainterPath` 覆写 |
| 四处接口对不上 Qt | `paint()` 签名 / `const` 方法改成员 / `QLayout::addStretch` / 连接受保护槽位 | 逐项改为 Qt 标准写法 |

### 验证

- 新增 4 个测试可执行文件共 **52 项断言全部通过**：`SmartGroundCartesian`（11 组）、`SmartGroundCartesianVisual`（8 项 + 9 张渲染图）、`SmartGroundCartesianPixel`（15 项像素级）、`SmartGroundCartesianUi`（18 项 UI 集成）。
- **79 个测试可执行文件零失败**；`SmartGroundGroupAUi` 模块计数更新为 63 并新增笛卡尔存在性断言；`SmartGround.exe` Release 构建成功。

---

## 第 32 批：归并重构 + 并查集合并 + 欧拉筛提速 + 四模块深化（2026-11-20，已完成）

PM 提交四条硬性指令：① 归并排序必须让数组**真的裂开**，分组内排好后再**拼接**成整体，未排序段与已排序段分色；② 并查集删掉重复的双模块，保留**一个**模块，并按课堂 `dsu` 模板（`pa[i]=i` / `sz[i]=1` 起步 → 父亲慢慢改 → `sz` 变动）；③ 欧拉筛指针要**平移**不能瞬移，且 `n/2` 之后要像埃氏筛一样**加速播放**；④ 把第 31 批剩余待办（拓扑排序、朴素 Dijkstra、Kruskal、堆调整）全部解决。

### ① 归并排序重构：真实的「裂开 — 拼接」（BUG-233）

- **新增帧族**：`MS_SPLIT` / `MS_REC` / `MS_MERGE_OPEN` / `MS_CMP` / `MS_PICK` / `MS_APPEND` / `MS_COPY` / `MS_DONE`。
- **裂开**：模块用 `std::set<int> m_openBounds` 记录当前所有活跃的「分裂缝」。`MS_SPLIT` 把 `mid` 插入集合，`applyLayout()` 据此为每格重算 x 坐标并用 `playMoveTo` 平移——**数组在画面上真的被撑开成两段**。
- **拼接**：`MS_COPY`（合并完成、写回 temp）把 `mid` 从集合里移除，缝被合上，两段重新贴成一个整体，同时整段染成已排序色。
- **分色**：未排序段淡白 `kMsIdle` → 分裂后左段浅蓝 `kMsLeft` / 右段浅紫 `kMsRight` → 归并完成整段转绿 `kMsDone`。
- **temp 缓冲区**：底部新增 `temp[]` 行，比较帧把两段队首染琥珀并脉冲，取出时数值用 `playValueFlight` 从原格飞入 temp 格。

### ② 并查集合并为单一模块（BUG-232）

- **删除** `src/modules/DSUBasicModule.{h,cpp}`，`MainWindow` 与 `tests/CMakeLists.txt` 同步摘除，模块总数由 63 调整为 **62**。
- **`DSUPathCompressionModule` 更名为「并查集」**，语义严格对齐 PM 给出的课堂模板：`n = _n + 1`；`pa = sz = vector<int>(n, 1)` 后 `iota(pa.begin(), pa.end(), 0)`；`find(x)` 带路径压缩；`merge(u,v)` 先 `find` 两端，**按大小合并**（`sz[u] < sz[v]` 则 swap），然后 `pa[v] = u; sz[u] += sz[v];`；`pd(u,v)` 判同集合。
- **新增帧族**：`DSU_INIT` / `DSU_FIND_START` / `DSU_FIND_STEP` / `DSU_FIND_ROOT` / `DSU_COMPRESS_STEP` / `DSU_MERGE_START` / `DSU_MERGE_LINK` / `DSU_MERGE_SIZE` / `DSU_PREDICATE` / `DSU_DONE`。
- **画面**：森林图上每个结点挂 `pa=N` 子标签；右侧 `pa[]` / `sz[]` 两行数组**实时同步**（`pa[]` 初始全为自身下标、`sz[]` 初始全为 1）；路径压缩时用 `GraphEdge::repointDest()` 现场改接父边并画紫色飞线。

### ③ 欧拉筛：指针平移 + n/2 之后加速（BUG-231）

- **指针瞬移根治**：`IModule::syncPointerTips()` 原先在 `finalizeFrame()` 里无条件 `stopAnimFor(ptr)` + `setPos(tip)`，把刚启动的 `pos` 平移动画**当场掐断**，指针看起来是瞬移。现在先检查 `m_animRegistry` 里是否有正在运行、且终点就是目标 tip 的 `pos` 动画 —— 有则**跳过吸附**，让缓动曲线跑完。
- **加速播放**：新增 `ES_FAST_START` / `ES_FAST_BATCH` / `ES_FAST_DONE`。`i > n/2` 时内层循环必然越界，改为**批量收录素数**（每批 8 个），逐批淡入质数库并染绿对应格子。

### ④ 四模块深化（第 31 批剩余待办）

| 模块 | 深化内容 |
|------|----------|
| 拓扑排序 | 删边时该边染灰并打 **✕  -1** 徽标（删边 ⇒ 终点入度 -1 合并成同一个动作）；入度表与图结点同序对齐；新增 `TS_EDGE_DELETE` / `TS_NEW_ZERO` / `TS_CYCLE`；成环时剩余结点染红并画红色闭环；底部加图例条 |
| 朴素 Dijkstra | 右侧新增**候选尺**：未确定点按 `dist` 升序排成刻度尺，`min` 指针指向最小者，已确定点划掉沉底；新增 `DJ_CANDIDATE` / `DJ_CHOOSE` / `DJ_RELAX_KEEP`，把「**为什么选它**」显式化；松弛不更优时红线划掉候选值 |
| Kruskal | `find` 改为**逐跳上溯**可视化（`KR_FIND_START` / `KR_FIND_STEP` / `KR_FIND_ROOT`），沿途结点染紫、父链逐段点亮；同根时 `KR_CYCLE` 把该边染朱红并沿树上路径画出**红色闭环**反证「为什么必须舍弃」 |
| 堆调整 | 树下方常驻**公式条**「左孩子 = 2k　右孩子 = 2k+1　父 = ⌊k/2⌋」；新增 `HG_INDEX` 帧核对当前结点下标（父 = ⌊k/2⌋、孩子 = 2k / 2k+1）；数组格与树结点之间加**极细引导线**建立映射；交换时数值沿**树边**弧线飞行，与光束路径重合 |

### ⑤ 顺带根治：`playMoveTo` 野指针（BUG-230）

`IModule::playMoveTo()` 把 `std::vector<QGraphicsItem*>` **裸指针**捕获进动画 lambda。画布中途 `clear()` 时这些 item 已被销毁，动画继续写值 → **SIGSEGV**（`SmartGroundAnimStress` 在 MergeSort 处崩溃即由此而来）。修复：签名改为 `std::vector<QGraphicsObject*>`（`ArrayBlock` / `GraphNode` 均继承自 `QGraphicsObject`），动画内改用 `std::vector<QPointer<QGraphicsObject>>`，item 一旦析构指针自动置空。

### 验证

- `SmartGroundStrictAudit`：62 模块 × 2 轮逐帧审计 → `TOTAL OVERLAP CONFLICTS = 0` / `TOTAL TIP FAILURES = 0` / `TOTAL AXIS FAILURES = 0`。
- **全部测试可执行文件零失败**（`TOTAL FAILED: 0`）；`SmartGround.exe` Release 构建成功。
- 同步更新 `tests/Batch31Test.cpp`（并查集断言改为统一模块语义）、`tests/Batch31Visual.cpp`（去掉双模式视觉块）、`tests/Batch8ModulesTest.cpp`、`tests/GroupAUiTest.cpp`（模块数 63 → 62、模块名「并查集 路径压缩」→「并查集」）、`tests/OverlapAuditTest.cpp`、`tests/EdgeRenderTest.cpp`、`tests/StressTest.cpp`。

---

## 第 31 批：现有模块动画深化落地（2026-11-20，已完成）

PM 复查 `TrieModule` / `IntervalSchedModule` / `EulerModule` / `BipartiteModule` 后明确策略转向：**不开新篇，先把现有算法做到尽善尽美**。随后 PM 逐条评审 10 条分镜方案，裁定为：**快排不动底层逻辑、归并单独调整、01 背包先比较再跳、KMP 与堆调整维持现状（教学优先，画面简洁）、并查集拆成双模式、埃氏筛保留「一次干掉一大堆」的爽感并另开欧拉筛**。本批即按该裁定落地。

### 已落地的 4 项改造 + 1 个新模块

| 项目 | 涉及文件 | 实现要点 |
|---|---|---|
| 归并排序分镜独立 | `src/modules/MergeSortModule.{h,cpp}` | 新建专属 `MS_*` 帧族（`MS_SPLIT` / `MS_REC` / `MS_MERGE_OPEN` / `MS_CMP` / `MS_PICK` / `MS_APPEND` / `MS_COPY` / `MS_DONE`），**与快排的 `AR_*` 完全隔离**；新增 `temp[]` 合并输出缓冲行；合并时「两段队首对比高亮 → 胜者飞入 temp → temp 整体写回原区间」；左段蓝 / 右段紫 / 已排序段绿 |
| 01 背包先比较后跳 | `src/modules/Knapsack01Module.cpp` | 新增 `KN_COMPARE` 帧：两个候选**并排高亮对比**后，**只有胜者**飞向目标格；`KN_CANDIDATE` 退化为「只高亮来源格」；容量不足时给出「只能不选」公式 |
| 并查集双模式 | `src/modules/DSUPathCompressionModule.{h,cpp}` | 新增「有路径压缩 / 无路径压缩」分段设置；无压缩模式发 `DP_NO_COMPRESS`（路径染红，明确说明「父指针保持原样」）；两模式都发 `DP_HEIGHT`，底部常驻**树高标尺**；默认数据换成深链 `9\n0 0 1 2 3 4 0 6 0`（树高 **5 → 1**，差别一眼可见） |
| 新增欧拉筛模块 | `src/modules/EulerSieveModule.{h,cpp}`（新建，已注册到「数学」分组） | 复用埃氏筛的 10 列矩阵；右侧独立**质数库 `prime[]`** 区域 + `p` 指针；`i` 指针指当前外层数；筛 `i*p` 后立刻演示 `i % p == 0 → break`（`i` 所在格染红说明「后面的 p 不用试了」）；文案全程强调「每个合数只被它的最小质因子筛掉一次」 |
| 边原地改指向 | `src/render/GraphElements.{h,cpp}` | `GraphEdge::repointDest(GraphNode*)` 让路径压缩把已有父边原地改指向根结点，无需重建边对象 |

### 帧类型扩展

`src/engine/AnimFrame.h` 新增 24 个动作类型：归并 8 个（`MS_*`）、背包 1 个（`KN_COMPARE`）、并查集 2 个（`DP_NO_COMPRESS` / `DP_HEIGHT`）、欧拉筛 9 个（`ES_*`）。

### 验证

- 新增 `tests/Batch31Test.cpp`（30 项断言）+ `tests/Batch31Visual.cpp`（5 张渲染图），`tests/CMakeLists.txt` 注册 `SmartGroundBatch31` / `SmartGroundBatch31Visual`。
- 关键断言：归并最终数组有序且合并次数 = n-1；背包每个 `KN_DECIDE` 前必有 `KN_COMPARE`；并查集两模式帧类型互斥、树高 5 → 1；欧拉筛 **42 个合数无重复划除**、每次 `break` 均满足 `i % p == 0`、最终 42 划除 / 17 保留；**埃氏筛行为未被改动**。
- `SmartGroundStrictAudit` 63 模块：`TOTAL OVERLAP CONFLICTS = 0` / `TOTAL TIP FAILURES = 0` / `TOTAL AXIS FAILURES = 0`。
- **77 个测试可执行文件零失败**；`SmartGround.exe` Release 构建成功。

### 评审记录（PM 第 7 轮裁定，存档备查）

| 序 | 模块 | PM 裁定 |
|---|---|---|
| 1 | `QuickSortModule` | 底层动画逻辑不动，只做分镜表层调整 |
| 2 | `MergeSortModule` | 单独调整，与快排区分开（→ 已落地 `MS_*` 帧族） |
| 3 | `HeapAdjustModule` | 通过：新增「选择当前最小值」指示 |
| 4 | `Knapsack01Module` | 通过：应先比较最大值，再往目标格跳（→ 已落地 `KN_COMPARE`） |
| 5 | `KMPNextModule` | 维持现状：教学目的是老师边讲边演示，过多元素反而干扰效果 |
| 6 | `TopologicalSortModule` | 通过 |
| 7 | `DijkstraNaiveModule` | 通过：新增「选择当前最小值」有必要 |
| 8 | `KruskalModule` | 通过 |
| 9 | `DSUPathCompressionModule` | 通过：不要同时演示有/无压缩，用滑块切换成两种模式，默认数据要能体现差别且偏链式（→ 已落地双模式 + 深链默认数据） |
| 10 | `SieveOfEratosthenesModule` | 埃氏筛不动（「一次干掉一大堆才是帅的」），把「一个一个归档」方案**转用于欧拉筛**，新增欧拉筛模块并配独立质数库与 `p` 指针（→ 已落地 `EulerSieveModule`） |

### 交付物
- 新增 **`docs/Batch31_Storyboards.md`** —— 10 个现有模块的动画深化分镜，逐个给出**「旧方案现状（已核对源码）」vs「新方案分镜」**对照。

### 十个深化对象与各自的核心缺口（历史方案存档）

| 序 | 模块 | 旧方案的核心缺口 |
|---|---|---|
| 1 | `QuickSortModule` | 无可视递归栈（depth 只在脚本文本）；pivot 是脱离数组的浮空格子 |
| 2 | `MergeSortModule` | **与快排共用同一套帧类型**，两算法视觉上无法区分 |
| 3 | `HeapAdjustModule` | 上下标与 `2k / 2k+1` 公式只在脚本文本；光束与值迁移路径不重合 |
| 4 | `Knapsack01Module` | 两个候选被拆成两帧先后覆盖，看不到并排比较 |
| 5 | `KMPNextModule` + `KMPMatchModule` | `pi[]` 只是数字表，「最长真前后缀」完全不可见 |
| 6 | `TopologicalSortModule` | 边被"删除"无删除标记；入度表与图中结点无对齐 |
| 7 | `DijkstraNaiveModule` | **「为什么选它」完全不可见**（核心教学点缺失） |
| 8 | `KruskalModule` | `find` 的逐跳路径压缩被跳过；成环边无红环反证 |
| 9 | `DSUPathCompressionModule` | 无压缩前后对照；旧链无残影；`sz[]` 是摆设 |
| 10 | `SieveOfEratosthenesModule` | 「从 `i*i` 开始」的剪枝理由只在脚本文本 |

### 新方案共用的视觉契约
统一视觉契约（颜色语义表、指针与光束方向规则、五段分层布局、收尾完整性）见 `docs/AnimationAesthetics.md`；十个分镜在同一基线上展开，新增的关键可视化结构包括：递归层带、基准值徽标、候选最小值尺、并查集森林树形图、树高标尺、`pi[]` 跨格括线、拓扑入度表对齐、`i*i` 紫色区间标记。

### 实施分批（已完成）
**31-A**（快排 / 归并 / 堆）→ **31-B**（背包 / 埃氏筛 / 拓扑）→ **31-C**（KMP / Dijkstra / Kruskal / 并查集）→ **31-D**（线段树合并版）。

---

## 第 30 批：视觉缺陷修复 + 动画美学宪法（2026-11-20）

PM 亲自复查后指出三个视觉缺陷，本批全部修复；同时把「什么样的动画才算好」这件事从口头共识升级为**可执行的书面宪法**。

### ① 开关控件的左右语义反了
- **现象**：图的类型开关滑块在**右**时显示的却是「有向图」高亮；模块设置里的「高亮延迟边」开关也是同样的反逻辑。
- **根因**（`src/ui/widgets/MyToggleSwitch.cpp`）：
  1. `paintEvent()` 左侧画 `m_onText`、右侧画 `m_offText`，把「开」放左、「关」放右，而高亮逻辑却跟着滑块走 —— 滑块靠右时高亮的是左边的「开」，视觉上等于「滑块在右 = 关」；
  2. `trackRect()` 用 `m_onText` 的宽度算左侧留白，轨道位置与文字不匹配。
- **修复**：统一为**「滑块在右 = 该状态开启」**。左侧画 `m_offText`、右侧画 `m_onText`，高亮跟随滑块位置（靠右 → 右侧 `textPrimary` 深色；靠左 → 左侧深色）。`trackRect()` 改用 `m_offText` 宽度。全项目 toggle 一次性对齐。

### ② 二分图判定的队列没清空，且 tail 指针飞出队列行
- **根因三处**：
  1. `BP_FINISH` 分支**完全没有清空队列**，残留值留到最后一帧；
  2. `BP_DEQUEUE` 帧的 `val1` 传的是 `head - 1`（**已自增过的值**），清的是错格子，真正出队的格子永远脏；
  3. 双指针都用 `PointerBlock::Down` 锚在槽位**上方**且杆长相同，标签被避让算法推到 y=434，**飞出队列行**。
- **修复**：新增 `BP_QUEUE_CLEAR` 帧类型专门收尾；`val1` 改为 `++head` 之前取值；队列行下移到 y=560，双指针改 `Up` + `belowCell` 锚在**下方**并用不同杆长分层。布局重排：`col[]` → y=470、结论 → y=630、奇环文本 → y=666。

### ③ 无向边第二次访问的光束方向不变
- **根因**：`GraphEdge` 的光束只支持正向绘制，`playEdgeTravel()` 也不接受起点参数，于是「从 v 访问 u」画出来的还是 u→v。
- **修复**：
  - `GraphEdge` 新增 `m_beamReversed` 属性，反向时 `quadSubPath(p0, c, p1, 1.0 - head, 1.0 - tail)`，光束头尾一并反向；
  - `IModule` 新增 `playEdgeTravelFrom` / `playEdgeTraceFrom` / `startBeamFrom`，内部比对 `edge->dest->id == fromNodeId` 自动判定；
  - 欧拉 `EU_EDGE`/`EU_BRIDGE`、匈牙利 `HN_TRY`/`HN_OCCUPIED`、二分图 `BP_CHECK`/`BP_CONFLICT` 全部改用带起点 id 的版本。

### ④ 新建《动画与视觉美学规范》
- 新增 **`docs/AnimationAesthetics.md`**，把下列内容固化为**强制条款**：
  - **三条底线**：语义先于装饰、一处实现全局复用、状态必须收尾；
  - **色彩语义表**：颜色 = 算法事实（蓝=0/偶数层，橙=1/奇数层，绿=最终成立，黄=正在扫描，红=真冲突，紫=延迟边，灰=已淘汰，靛=反例解），同时活跃强调色 ≤ 3 种；
  - **运动语义表**：四种边运动（行进 / **反向行进** / 生长 / 描迹）、五种节点运动、指针运动规则、`PathTrail` 用法、时长基准表；
  - **布局规范**：画布五段分层（标题 / 主体 / 状态数组 / 辅助结构 / 结论）、净距要求、数组下标在格上、值居中、**队列三段式画法**；
  - **控件语义规范**：**滑块在右 = 该状态开启**，互斥多态选择必须用 `makeSegment`；
  - **状态完整性**：临时结构复位、指针归位、结论落地、重放幂等、撤销兼容；
  - **文案规范**、**帧设计规范**、**新模块 10 条自检清单**。
- 同步接入 `.cursorrules`、`AGENTS.md`、`.cursor/rules/smartground-project.mdc`、`Cursor_Workflow.md` 第 2 节，保证每个新会话都带着这套美学记忆开工。

### 验证
- 新增 `tests/VisualFixTest.cpp`（`SmartGroundVisualFix`）：5 项断言覆盖队列清空、指针不越界、指针标签不出画布、桥接帧产出、开关两态快照。
- `SmartGroundStrictAudit` 62 模块 × 2 轮 → `TOTAL OVERLAP CONFLICTS = 0` / `TOTAL TIP FAILURES = 0` / `TOTAL AXIS FAILURES = 0`。
- **76 个测试可执行文件零失败**；`SmartGround.exe` Release 构建成功。

## 第 29 批：四个已有模块的动画深化（2026-11-20）

本批**不新增模块**，只把上一轮提出的 4 个分镜方案落地到已有模块，把「静态示意图」升级为「有教学推进感的分步动画」。新增 20 个 `ActionType`，4 个模块各自补齐「为什么这样选」的因果链。

### ① Trie 字典树：插入 / 查询 / 字典序三模式
- **新增** `moduleSettingsFields()`：`makeSegment` 提供「插入建树 / 查询单词 / 字典序遍历」三模式切换，`makeText` 提供查询词输入框；`applyModuleSetting()` 分派到 `runInsertMode()` / `runQueryMode()` / `runOrderMode()`。
- **查询模式**：先用 `buildStaticTrie()` 建好静态树，再逐字符沿路径下降（`TR_QUERY_CHAR`）。命中分支边变紫并弹跳（`TR_QUERY_HIT`）；走到底且 `end = 1` 则绿脉冲表示**命中**（`TR_QUERY_FOUND`）；中途无分支（`TR_QUERY_MISS`）或仅走到前缀结点则红闪**判失败**。
- **字典序模式**：按字符升序做 DFS 前序遍历，`TR_ORDER_START` / `TR_ORDER_NODE`（`v` 字段区分「进入子树」与「输出单词」）/ `TR_ORDER_DONE`；命中的单词逐个落到底部输出行 `m_orderItems`，顺序严格递增。

### ② 区间调度贪心：冲突重叠带 + 反证对照
- **冲突带**：`IV_REJECT_BAND` 帧在时间轴上用 `QGraphicsRectItem`（红色虚线边框 + 70/255 半透明填充）画出**重叠区间带**，直观呈现「为什么这两个活动不能同时选」。
- **反证阶段**：`IV_PHASE_RESET` 重置画面后，改用「按**开始时间**最早选」的错误准则重跑（`IV_COUNTER_COMPARE` / `IV_COUNTER_PICK` / `IV_COUNTER_DONE`），错误解单独排在下一行（`m_naiveLabels`），与贪心解（`m_greedyLabels`）上下对照，直接给出「贪心 2 个 vs 错误 1 个」的结论。

### ③ 二分图染色判定：BFS 队列行 + 奇环抽取
- **队列可视化**：新增 `m_queueBlocks`（`q[]` 数组行）+ `m_queueHeadPtr` / `m_queueTailPtr` 双指针锚定槽位，`BP_ENQUEUE` / `BP_DEQUEUE` 把「染色」与「同色冲突」的因果链显式化。
- **奇环抽取**：`extractOddCycle(u, v)` 沿 BFS 父指针链求两条链的公共祖先，拼出完整环；`BP_ODD_CYCLE` 帧用 `PathTrail` 描出整条奇环并把环上结点染红，解释「同色相邻」的本质原因是**奇数长度回路**。
- **修正**（BUG-226）：原实现用「双指针同步上跳找相遇点」，两链长度不同时会跳过相遇点返回空数组；改为「取完整祖先链 → 尾部对齐比较公共后缀求 LCA → 按 `u→…→LCA←…←v` 拼环」。

### ④ 欧拉路径 Hierholzer：栈列 / 回溯 / 拼接
- **栈与答案列**：`stk[]` 栈列与 `path[]` 答案列改为**纵向堆叠**（显示栈底 → 栈顶），底部新增「欧拉路径」结果行与 `PathTrail` 描边轨迹。
- **五段语义拆帧**：`EU_DEADEND` 死路结点灰化收缩 → 压栈 → `EU_BACKTRACK` 回溯出栈 → `EU_BRIDGE` 延迟边紫色描边（可开关）→ `EU_SPLICE` 拼接（橙色虚线箭头 + 涟漪）→ `EU_TRAIL` 最终一笔描出有序轨迹。
- **布局重排**：`deg[]` 行起始 x 44 → 170、行距 54 → 76；`stk[]` / `path[]` 列 x 470 / 600（标签水平相隔 120px）；答案行下移至 y = 618。

### 验证
- 新增 `tests/AnimUpgradeTest.cpp`（`SmartGroundAnimUpgrade`）：9 项断言覆盖三模式帧产出、奇环抽取、死路回溯、拼接、冲突带与反证结论。
- `SmartGroundStrictAudit`：62 模块 × 2 轮逐帧审计 → `TOTAL OVERLAP CONFLICTS = 0` / `TOTAL TIP FAILURES = 0` / `TOTAL AXIS FAILURES = 0`。
- **73 个测试可执行文件零失败**；`SmartGround.exe` Release 构建成功。

## 第 28 批：数组下标归位 / 指针轴向对齐 / 图形空心实心语义修正（2026-11-20）

PM 三图复查，指出上一轮改动**违背既定要求**，共四条。本批全部修正并补上自动化断言。

### ① 数组下标必须回到单元格上方，值必须双向居中

- **现象**：所有数组的下标被从单元格**上方**挪进了**格子内部左上角**；格内的值随之整体上移、偏离中心。
- **根因**：上一轮为规避「下标与窗格重叠」的审计告警，`ArrayBlock::paint()` 把下标改成 `drawText(QRectF(3, 0, m_cellW - 6, indexH), Qt::AlignLeft | Qt::AlignVCenter, ...)`（绘制在窗格矩形内部左上角），值区也改成 `QRectF(0, indexH, m_cellW, m_cellH - indexH)`。这是一次**擅自决断**，违背了「下标在单元格上方、值水平垂直居中」的既定要求。
- **修复**（`src/render/GraphElements.{h,cpp}`）：
  - 下标绘制矩形改为 `QRectF(0, -kIndexGap - kIndexHeight, m_cellW, kIndexHeight)`，用 `Qt::AlignHCenter | Qt::AlignBottom` 水平居中并贴底，与顶边框保持 `kIndexGap = 6.0` 净距，不重合；
  - 值恢复为在**整个窗格** `QRectF(0, 0, m_cellW, m_cellH)` 内 `Qt::AlignCenter` 绘制；
  - 新增 `kIndexGap` / `kIndexHeight` 常量与 `indexRect()` 访问器；`boundingRect()` 上边界上移到 `-(kIndexGap + kIndexHeight)`，下标不被裁剪。

### ② 指针尖端必须精确落在格心 / 节点圆心延长线上

- **现象**：二分查找等模块指针不再指向目标格子，提示变量 `i` / `j` / `L` / `R` 与对应指针对不上号。
- **根因（两处）**：
  1. `IModule::layoutPointerSlots()` 在同格挂多个指针时执行 `tips[idx] += QPointF(offset, 0.0)`，**直接横向平移了箭头尖端**，尖端偏离单元格宽度中心；
  2. `IModule::resolveVisualCollisions()` 的避让策略以**横向平移标签**为主（`bestShift = QPointF(dx, 0.0)`），标签被推离自己的指针轴线，`i` / `j` / `L` / `R` 与指针的对应关系随之错乱。
- **修复**：
  - `layoutPointerSlots()` 删除尖端横向偏移，同格多指针改为**沿轴线分行**；
  - `resolveVisualCollisions()` 重写为**轴向优先**求解：垂直指针候选位置沿轴线由近及远堆叠（最多 8 行，步长 22px），水平指针沿轴向左右让位；横向平移只作为水平指针的兜底；
  - `separatePointerLabels()` 垂直指针改为**纵向分离**，不再横向平移。
- **顺带修正水平箭头方向**：`PointerBlock::paint()` 中 `Right` 的箭头尖此前与 `Left` **同朝左**（「从左往右指向单元格」的语义是错的）。改为 `Right` 朝右（`drawLine(0, 0, 9 * sign, ±7)`）、`Left` 朝左；`defaultLabelOffset()` / `tailLabelOffset()` / `setLabelToRight()` 同步修正 —— `Left` 标签放杆的右外侧、`Right` 放杆的左外侧。

### ③ 图形空心 / 实心：画布绘制与图标语义分离

- **现象**：画板矩形 / 圆 / 三角形在「空心」态画成**虚线边框 + 灰色填充**，与需求完全相反。
- **根因**：把「空心 / 实心」误解成了「虚线 / 实线」，并把这个**图标语言**误用到了**画布绘制结果**上。
- **修复**：
  - `src/canvas/ShapeItem.cpp`：笔一律 `Qt::SolidLine`；空心 → `setBrush(Qt::NoBrush)`（内部完全透明）；实心 → `setBrush(QBrush(m_penColor))`（边框色即填充色，整体实心）；
  - `MySegmentedControl` 新增 `ShapeIcon`（`RectIcon` / `CircleIcon` / `TriangleIcon`）与 `setShapeIcon()`，图标与文字并排居中；**图标语义**：第 0 段「空心」画**虚线边框 + 无填充**，第 1 段「实心」画**实线边框 + 同色实心**；
  - 绘制逻辑抽为公共静态 `MySegmentedControl::buildIconPath()` / `paintShapeIcon()`，UI 与测试共用同一份代码；
  - `CanvasToolBar` 按当前工具给 `canvasToolShapeFill` 切换对应形状图标。

### ④ 连带修复：链式前向星

该模块用手工 `QGraphicsTextItem` 画下标（`rowY - 32`，字号 11 但包围盒高 26），位置不准且与 `k` 指针标签打架。改为直接使用 `ArrayBlock` 内建下标，`k` 指针标签改用 `setLabelAtTail()` 落在轴线上，行距 `kFsRowGap` 由 96 放宽到 116 让开相邻行下标。

### ⑤ 回归验证

| 项目 | 结果 |
|------|------|
| `SmartGroundStrictAudit`（58 模块 × 2 轮逐帧） | `TOTAL OVERLAP CONFLICTS = 0`、`TOTAL TIP FAILURES = 0`、**`TOTAL AXIS FAILURES = 0`** |
| `SmartGroundOverlapAudit` | `ALL PASS`（下标包围盒改用 `ArrayBlock::indexRect()`） |
| `SmartGroundLayoutFit` | `ALL PASS` |
| `SmartGroundCanvasFeatures` | 新增 9 项断言（图标语义 + 画布空心/实心像素级验证）全绿 |
| `build/tests/` 全部可执行文件 | **97 个零失败** |
| `SmartGround.exe`（Release） | 构建成功 |

> `SmartGroundStrictAudit` 本批新增 `[AXIS-FAIL]` 断言：标签中心必须与指针轴线对齐、尖端必须落在格心 / 节点圆心延长线上。

---

## 第 27 批：光束几何根治 + 编辑器崩溃根治 + Tarjan 排版（2026-11-20）

PM 二次复查上一轮修复，逐条复核并追加新问题。本批定位到**上一轮"激光根治"并未真正解决几何问题**，另发现一个长期潜伏的析构崩溃与一处排版重叠。

### ① 激光光束几何根治：光束画的是弦，不是曲线

上一轮把 26 段短线换成 `QLinearGradient`，只解决了「串珠」观感，但**几何错误仍在**：`GraphEdge::paint()` 用 `drawLine(beamStart, beamEnd)` 连接曲线上参数 `tail` 与 `head` 的两点 —— 这是一条**弦**。弦与贝塞尔曲线之间最大偏离可达数十像素，且每帧切换 `head` 时弦随之跳变，于是：
- 光束**不贴合**边的路径，看起来像直线穿过弯曲的边；
- 相邻帧的弦不共线，屏幕上留下一串**断裂碎片**；
- 弦端点与曲线终点不重合，亮点位置有肉眼可见的偏移。

**修复**：`src/render/GraphElements.{h,cpp}` 新增
- `GraphEdge::splitQuad(p0, p1, p2, s, ...)` —— de Casteljau 二次曲线二分，把曲线在参数 `s` 处切成左右两段；
- `GraphEdge::quadSubPath(p0, p1, p2, from, to, out)` —— 按参数区间 `from..to` 抽取子曲线并生成 `QPainterPath`。

光束改为**沿子曲线 `drawPath` 描边**：外层 9.0px 辉光与内层 3.4px 亮芯都使用「沿路径首尾」的 `QLinearGradient`，端点亮点取 `beamPath.pointAtPercent(1.0)`，精确落在曲线终点。

**几何回归测试**：
- `tests/BeamCurveTest.cpp`（`SmartGroundBeamCurve`）：光束端点与原曲线偏差 **0.00000000 px**；
- `tests/BeamRenderTest.cpp`（`SmartGroundBeamRender`）：19 个进度档位 × 61 采样点，弧线上 **1159 采样点零断裂**，超出光束头部的 **351 采样点零残留**。

> 修复过程中还发现 `splitQuad` 右半段控制点的写法有误（应为 `l2` 而非 `l2 + (p2 - l2) * s`），导致偏差 16.92px；由 `BeamCurveTest` 精确定位后修正。

### ② 编辑器析构崩溃根治（`SmartGroundGraphInteraction` 间歇性段错误）

- **现象**：该测试 30 次连跑失败 5 次（退出码 `0xC0000005`）。
- **根因**：`GraphEdge` / `GraphNode` / `ArrayBlock` 析构时，其子项编辑器 `QGraphicsTextItem` 随之析构；Qt 在 `QGraphicsItem` 基类析构中发出 `FocusOut`，被 `EditorWatcher` 拦截后回调 `commitEdit()`，而此时 `QTextDocument` 已进入释放流程 —— `toPlainText()` 访问已释放内存。gdb 栈回溯精确落在 `QTextDocument::toPlainText() ← GraphEdge::commitEdit() ← EditorWatcher::eventFilter() ← ~QGraphicsItem`。
- **修复**：
  - 新增 `EditorTextItem`（`QGraphicsTextItem` 子类），析构时置位共享的 `EditorGuard::tearingDown` 标志；
  - `EditorWatcher::eventFilter()` 收到 `FocusOut` 时若该标志已置位，**直接吞掉事件**，绝不回调 `commitEdit()`；
  - 三个可编辑图元的析构函数统一「先置位标志 → `setParentItem(nullptr)` → `delete` 编辑器子项」，从根上杜绝析构期回调。
- **验证**：`SmartGroundGraphInteraction` 连跑 **30 次零失败**（修复前 30 次失败 5 次）。

### ③ Tarjan 割点与桥：数组与文字标签重叠

- **现象**：`dfn[]` / `low[]` 两组数组与文字标签明显重叠。
- **根因**：原布局 `dfnX = 120`、`dfnY = 500`，而 `low[]` 标签被放在 `(114, 530)` —— 恰好压在 `dfn[]` 数组行（y 500~540）的窗格上，重叠面积约 **620px²**。
- **修复**：`TarjanCutVertexBridgeModule::buildLayout()` 重排版式 —— 数组行下移至 `dfnY = 545`、行距 `rowGap = 100`、起点 `dfnX = 170`；`dfn[]` / `low[]` 标签左移到 `x = 48` 的独立列，与首列窗格保持 **49px** 净距。

### ④ 回归验证

| 项目 | 结果 |
|------|------|
| `SmartGroundStrictAudit`（58 模块 × 2 轮逐帧） | `TOTAL OVERLAP CONFLICTS = 0`、`TOTAL TIP FAILURES = 0` |
| `build/tests/` 全部可执行文件 | **零失败** |
| `SmartGroundGraphInteraction` 连跑 30 次 | **零失败** |
| `SmartGround.exe`（Release） | 构建成功 |

---

## 第 26 批：全局零重叠审计 + 激光拖影根治 + 画板三项新功能（2026-11-20）

PM 复查上一轮修复，指出：**所有图论算法**的边「像激光一样射过去」时路径上都会产生**碎片残留波纹**；要求彻底排查全部算法的①文字/控件/数组窗格重叠 ②指针未指向结点或数组中心 ③指针瞬移 ④平移结构拖影 ⑤动画落点偏移 ⑥数组结构用圆形涟漪不美观；并追加画板三项新功能。

### ① 全局零重叠机制（三道防线）

| 防线 | 入口 | 作用 |
|------|------|------|
| 讲课脚本文字避让 | `IModule::resolveScriptTextOverlaps()` | 扫描带 `kScriptTextMarker` 标记的文本项，与 `ArrayBlock` / `GraphNode` / `QGraphicsRectItem` 等实体求交，冲突时沿 $(\pm dy)$ 候选偏移逐档下移，直到完全不压任何结构 |
| 指针标签消解 | `IModule::resolveVisualCollisions()` | **轴向优先**求解：垂直指针（Up / Down）沿轴线由近及远堆叠（`m_labelRow`），水平指针沿轴向左右让位；横向平移只作水平指针兜底，**不移动箭头尖端、不脱离自身轴线**（第 28 批修正） |
| 指针尖端锁定 | `IModule::syncPointerTips()` | 每帧末尾按 `pointerTipFor()` 复算尖端并强制归位，杜绝动画结束后残留偏移 |

`IModule::finalizeFrame()` 统一串联三道防线：`resolveVisualCollisions() → resolveScriptTextOverlaps() → syncPointerTips()`。`MainWindow` 的播放 / 单步 / 撤销 / 拖动进度条四条路径全部调用 `finalizeFrame()`。

**审计工具**：`tests/StrictAudit.cpp` + `tests/StrictAuditModules.h`（`SmartGroundStrictAudit`）对 **58 个算法模块**跑两轮（讲课脚本开 / 关），逐帧采集全部 `ArrayBlock` / `GraphNode` / `PointerBlock` / `QGraphicsTextItem` 的场景包围盒两两求交，并独立校验每个指针的尖端是否仍锚在目标边界。结论：**0 处重叠冲突、0 处指针失锚**。

### ② 激光拖影与残留波纹根治（BUG-223）

- **波纹来源**：`GraphEdge::paint()` 用 **26 段短线**叠加画光束，每段独立 `RoundCap` 且 alpha/线宽各异，曲线内侧留出可见缝隙 → 视觉上是一串「串珠」。改为 `QLinearGradient` 沿路径**一次性连续绘制**（外层 9px 宽渐变 + 内层 3.4px 亮芯 + 端点亮点）。
- **残留来源**：`IModule::playEdgeTravel()` 每次都 `new QPropertyAnimation(edge, "beamProgress")` 且**不入动画注册表**，同一条边被多次触发时多个动画实例同时写 `beamProgress`，互相打断后留下停在半途的残段。新增 `IModule::startBeam()` 与 `m_beamAnims` 注册表（按边去重，同一时刻同一条边只有一条光束），在 `purgeTransientItems()`（每帧 `beginFrame` 触发）与析构时统一 `stop()` + `clearBeam()`。

### ③ 指针：锚点精确 + 不瞬移 + 不穿格（BUG-221 / BUG-222）

- `GraphNode` 新增 `static constexpr double kRadius = 20.0`，`IModule::pointerTipFor()` 与 `PointerBlock::anchoredPos()` 统一改用 `kRadius`，结点指针尖端精确落在**圆周上**（不再外偏 2px）。
- 新增 `IModule::retargetPointerAcrossCells()`：同侧换格走行外弧线，不再直线穿过单元格。
- 新增 `IModule::retargetPointerWithDirection()`：**方向翻转不在翻转瞬间生效**，而是先按原方向走「绕行路径」（退到行外 → 行外平移 → 从目标侧进入），到位后才切换方向并锁定终点，彻底消除「方向瞬间翻转 + 位置瞬移」。`CircularQueueModule::aimQueuePointers()` 改用该接口。
- `IModule::pointerTipFor()` 新增显式方向重载，便于在不改方向的前提下预演目标落点。

### ④ 数组高亮改矩形涟漪（PM 第 6 点）

`GraphElements` 新增 `RectRipple`（矩形边框双层描边，随 `setGrow()` 外扩），`IModule::playCellRipple()` / `playCellRippleAt()` 改用它；`playNodeRipple()` 检测到锚点是 `ArrayBlock` 时**自动走矩形波纹**，结点仍保留圆形涟漪。

### ⑤ 生命周期安全

`GraphEdge::source/dest`、`GraphNode::edgeList/pointerList`、`PathTrail` 指针全部改用 `QPointer`；`PointerBlock::setAnchorNode/setAnchorItem` 监听目标 `destroyed` 信号自动置空锚点；`playEdgeTrace()` 的 `edge` 捕获改用 `QPointer` 守卫，消除场景重建期的 use-after-free。

### ⑥ 画板三项新功能（BUG-224 / BUG-225）

| 功能 | 实现 |
|------|------|
| **左侧竖向笔刷滑块** | 新增 `src/ui/widgets/CanvasBrushSlider.{h,cpp}`：贴靠画布左缘的圆角竖向滑块，仅在笔 / 荧光笔 / 橡皮下显示；与工具飞出面板、滚轮**双向同步**（`ToolController::styleChanged` 驱动）；笔调粗细、橡皮调半径 |
| **左写右擦** | `ToolController` 新增 `m_rightClickErase` 与 `beginRightErase/updateRightErase/finishRightErase`，右键按下即进入橡皮拖拽态；橡皮面板新增 `MyToggleSwitch`（objectName = `canvasToolRightClickErase`）控制该行为，默认开启 |
| **空心 / 实心切换** | 矩形 / 圆形 / 三角形面板顶部新增 `MySegmentedControl`（objectName = `canvasToolShapeFill`，选项「空心 / 实心」）；`ShapeItem` 以虚线框 + 透明填充表示空心、实线 + `#EEF0F4` 填充表示实心 |

**关键缺陷修复**：`ToolController::rightClickErasePreference()` 原本用无参 `QSettings`，而 `main.cpp` 从未设置 org/app 名，Qt 判定 `InvalidScope` 导致偏好**根本没有落盘**。修复：`main.cpp` 补 `setOrganizationName` / `setApplicationName`，且偏好读写改为**显式作用域** `QSettings(IniFormat, UserScope, "SmartGround", "SmartGround")` + `sync()`。

### 涉及文件

| 文件 | 改动 |
|------|------|
| `src/modules/IModule.{h,cpp}` | 三道防线的入口与实现、`startBeam` / `m_beamAnims`、`retargetPointerAcrossCells` / `retargetPointerWithDirection`、`pointerTipFor` 方向重载、`syncPointerTips` |
| `src/render/GraphElements.{h,cpp}` | `GraphNode::kRadius`、光束改 `QLinearGradient`、`PointerBlock::anchoredPos()` 归一、`QPointer` 化、`RectRipple` |
| `src/modules/CircularQueueModule.cpp` | `aimQueuePointers()` 改用方向感知重定向 |
| `src/ui/widgets/CanvasBrushSlider.{h,cpp}` | 新增：左侧竖向笔刷滑块 |
| `src/canvas/ToolController.{h,cpp}` | 右键擦除状态机、`QSettings` 显式作用域修复 |
| `src/ui/widgets/CanvasToolBar.cpp` | 右键擦除 toggle、空心/实心分段控件 |
| `src/main.cpp` | 补 `QCoreApplication` org/app 名（`QSettings` 持久化前提） |
| `tests/StrictAudit.cpp` + `tests/StrictAuditModules.h` | 58 模块 × 2 轮全帧重叠与指针锚点审计 |
| `tests/CanvasFeaturesTest.cpp` | 画板三项新功能的端到端验证 |

**验证**：`build/tests/` 下全部测试可执行文件**零失败**；`SmartGroundStrictAudit` 报告 `TOTAL OVERLAP CONFLICTS = 0` / `TOTAL TIP FAILURES = 0`；`SmartGroundCanvasFeatures` 21 项断言全绿；`SmartGroundPointerAlign`、`SmartGroundBatch9Visual`、`SmartGroundPointerMotion` 全绿；主程序 `SmartGround.exe` Release 构建成功。

## 第 25 批：三个模块深度重构 —— LCS 连续回溯路径 / 匈牙利分栏与增广路 / 树堆下沉轨迹（2026-11-20）

PM 在第 24 批后抉择：LCS、匈牙利算法、树与堆三类模块需要深度重构（其余保持现状）。

### 共同基础：路径轨迹主角

`src/render/GraphElements.{h,cpp}` 新增 `PathTrail`：

```cpp
class PathTrail : public QGraphicsObject {
    void setPoints(const QVector<QPointF>& pts);
    void appendPoint(const QPointF& p);
    void setProgress(qreal t);      // 沿折线渐进绘制
    void setTrailWidth(qreal w);
    void setHeadRadius(qreal r);
};
```

`paint()` 叠三层：低透明度加粗辉光 → 实色主线 → 末端「行进头」（光晕 + 内核 + 白环）。

`src/modules/IModule.{h,cpp}` 新增两个编排助手：

```cpp
void playPathTrail(QObject* trail, QVector<QPointF> points, QColor color, int duration = 560);
void playPathTrailAppend(QObject* trail, QPointF point, QColor color, int duration = 420);
```

`playPathTrailAppend` 从上次进度续画（`(n-2)/(n-1) → 1.0`），实现「一条路径持续生长」而非每帧重画。

### ① LCS：沿对角线的连续回溯路径

- `runAlgorithm()` 回溯段新增 `walk`，记录**每一步退让的落点**（不只记录匹配格）；
- `LC_TRACE` 帧的 `group` 携带已走过的路径点序列；
- `renderFrame` 把这些点对应的 dp 格中心连成折线交给 `PathTrail` —— 回溯时一条线持续沿对角线延伸，行进头跟着当前回溯位置走；
- 匹配到的格子另走 `playCellLanding` + 涟漪；退让步只延伸路径、不惊动格子。

### ② 匈牙利算法：左右分栏 + 增广路主角

- `buildLayout()` 新增左右两条虚线分栏带（左浅蓝 / 右浅橙，半透明填充，`Z = -2`），把两列结点明确分区；
- `PathTrail` 作增广路主角：
  - `HN_START` 从当前左部顶点起笔；
  - `HN_TRY` 追加右部顶点（黄）；
  - `HN_OCCUPIED` 追加被占用的持有者（红）；
  - `HN_ASSIGN` 追加最终配对点（绿）；
  - `HN_DONE` 冻结为绿色并对全部匹配顶点做收束缩放。
- `m_matchedEdges` 记录最终匹配边，供终帧回放。

### ③ 树与堆：逐层下沉轨迹

- `HeapTreeBase` 新增 `ensureDescentTrail` / `clearDescentTrail` / `appendDescent` / `freezeDescentTrail` 四个助手；
- `HeapSortModule` / `HeapAdjustModule` 在比较与交换帧把当前结点位置追加进轨迹，形成**逐层下沉**的可见路径，终帧冻结为有序色；
- `SegmentTreeEditBase` 提供同款共享实现（`SegmentTreeBuildModule` 不继承该基类，自带等价实现），下降帧同时播放 `playEdgeTravel` 边行进与轨迹延伸，形成「树往下走」的连贯主线。

**验证**：主程序零编译错误；79 个测试可执行文件全部零失败。

## 第 24 批：全场动画美学对齐哈夫曼树 + 画笔/三角形/进度条三项修复（2026-11-20）

PM 提交四项任务：① 画笔工具换色不生效，且打开画笔菜单后点击非菜单区域不会自动关闭；② 三角形绘图带有背景需清除；③ 部分动画播放时右下角进度条不随播放前进（只有数字在变），要求全量自检；④ 以哈夫曼树为动画美学基准，对全场算法做一次观感升级。

### ① 画笔换色与菜单自关闭

- 画笔颜色变更即时写入当前笔迹与已选中对象的渲染色，不再只改工具栏色块；
- 应用级 `eventFilter` 挂在 `qApp` 上：画笔飞出菜单打开时，点击菜单以外的区域（画布等）自动收起菜单。

### ② 三角形绘图去背景

`src/render/` 图形项绘制：三角形移除半透明填充底色，只保留描边，背景完全透明。

### ③ 进度条同步自检

根因在 `src/ui/MainWindow.cpp` 的 `m_seeking`：此前只要在滑块上滚动鼠标滚轮就会把它置为 `true` 且永不复原，于是帧号继续递增、滑块却冻结。

修复：`m_seeking` 只在 `MySlider::sliderPressed` 置位、`sliderReleased` 清零。

新增审计工具 `tests/TmpSliderAudit.cpp`（`SmartGroundTmpSliderAudit`）：遍历全部 70 个模块，模拟滚轮后自动播放，逐帧比对滑块值与 `probeFrameIndex()`，结果 **`ALL PASS fail=0`**。

### ④ 全场动画美学对齐哈夫曼树

**共享词汇表**（`src/render/GraphElements.{h,cpp}` + `src/modules/IModule.{h,cpp}`）

| 元素 | 作用 |
|---|---|
| `TracerDot` | 带光晕与内核的移动光点，作为「边行进」轨迹的视觉主体 |
| `playEdgeTravel` | 光点沿边曲线滑行，半径与透明度按 `sin(t·π)` 起伏，`InOutCubic` 缓动 |
| `playDiscover` | 结点首次被发现时的扩散涟漪 |
| `playEdgeDiscover` | 边先到、结点后醒的组合动作 |
| `playNodeWake` | 结点苏醒脉冲 |
| `playFocusRing` | 焦点环 |
| `playValueArcFlight` | 数值沿弧线飞入目标格 |
| `playCellSweep` | 一组格子错峰扫描 |
| `playCellLanding` | 值落位时的脉冲 |

全部助手遵守 `m_animated == false` 时瞬时生效、不创建 Qt 动画的统一约束。

**批量落地范围（69 个模块文件）**

- 图算法：BFS / DFS / Dijkstra×2 / 拓扑排序 / Prim / Kruskal / Tarjan×2 / 二分图 / 欧拉 / 匈牙利
- 树与堆：堆排序 / 堆调整 / 堆基础 / Trie / 哈希表 / 链式前向星 / 线段树×3
- 排序：冒泡 / 选择 / 插入 / 归并 / 快排 / 希尔 / 计数 / 基数 / 桶
- 查与算：二分×2 / 快速幂 / 树状数组 / LIS / 区间调度 / GCD / 单调队列 / 单调栈 / 并查集×2 / 树直径 / 树子树大小 / 背包×2 / 字符串哈希 / 筛法
- 矩阵与数论：LCS / 扩展欧几里得
- 数据结构：栈 / 队列 / 循环队列 / KMP pi 数组

语义统一为 idle / scanning / active / matched / terminal 五态色；扫描与匹配一律带涟漪或落位脉冲，终态一律带收束缩放。

**验证**：`SmartGround` 主程序零编译错误；`build/tests/` 下 **79 个测试可执行文件全部零失败**。

### 需要整体重构、待 PM 抉择的模块

以下模块的现状是「动作有了，但缺少统一的视觉主角与空间编排」，若要达到哈夫曼树的水准，需要重构版式与分镜，已准备好分镜方案待选：

1. **LCS** — 矩阵 trace-back 需要一条沿对角线的连续路径主角，而不是逐格高亮；**✅ 已于第 25 批落地**
2. **扩展欧几里得** — 双栏公式需要「式子变形」的连续主角（等式逐行变形 + 回代式逐层展开）；**未选中，暂缓**
3. **匈牙利算法** — 二分图布局需要左右分栏 + 匹配边的持续高亮带；**✅ 已于第 25 批落地**
4. **线段树 / 树堆** — 需要树的「逐层下沉」主角，与数组槽位的联动主线；**✅ 已于第 25 批落地**

> PM 已于第 25 批选择 LCS / 匈牙利 / 树堆三项落地，扩展欧几里得暂缓。

## 第 23 批：哈夫曼树飞入落点/父结点来源/根结点残留修正 + 边渐进生长动画（2026-11-19）

PM 提交 5 张截图，指出哈夫曼树「16 飞入落错行 / 下一帧自动跳位 / 父结点从天上飞下 / 20 初始位置不对 / 根结点向上调整后旧点没删」，并追加需求「边要有从孩子到父亲的展开过程，0.2s 内完成」。

### ① 飞入落点算错，下一帧才跳到正确位置（BUG-220）

`src/modules/HuffmanTreeModule.cpp` — `HF_PROMOTE`

目标坐标用了 `step.before`（**上移之前**的森林行号），被提升的子树此时还停在更低的行，目标行因此算低一级：

```cpp
const QPointF target = treePosOf(id, step.before);   // 错：算的是上移前的行
```

改为使用 `step.afterLift`（上移对齐后的森林），飞入即直达对齐行，无需下一帧二次跳位：

```cpp
const QPointF target = treePosOf(id, step.afterLift);
```

同时本帧只处理**原始叶子**的队列筹码（内部结点的树内位移交由 `HF_LIFT` 帧负责），避免同一结点在树区与队列区各留一份造成重影。

### ② 合并出的父结点从天上飞下来（BUG-219）

`src/modules/HuffmanTreeModule.cpp` — `HF_MERGE`

新建队列筹码 `GraphNode(9000 + parentId)` 后没有设置初始位置，`GraphNode` 的默认 `pos()` 是原点，于是父结点从 (0,0) 飞下并拖出一条穿越全画面的长轨迹。修复：

```cpp
chip->setPos(treePosOf(parentId, step.after));   // 从树里的真实坐标「落」回候选队列
```

### ③ 根结点向上调整后旧点残留（BUG-218）

`src/modules/HuffmanTreeModule.cpp` — `HF_MERGE` / `HF_FINAL`

最后一次合并仍把根结点的队列筹码排入候选队列，`HF_FINAL` 只给树内根结点染色而没回收队列筹码，于是候选队列里多出一个根结点副本。修复：`HF_MERGE` 增加 `isRoot` 判定，最后一次合并为根结点**创建但立刻隐藏**队列筹码（保持「每个结点一份队列副本」的节点计数不变），不再把它排入队列；`HF_FINAL` 再兜底隐藏一次。

### ④ 边的渐进生长（BUG-217，新增需求）

`src/render/GraphElements.{h,cpp}` — `GraphEdge`

新增可动画属性与曲线部分求值：

```cpp
Q_PROPERTY(qreal drawProgress READ drawProgress WRITE setDrawProgress)
```

`paint()` 按 `drawProgress` 对二次贝塞尔曲线做 de Casteljau 分割，只绘制 `[0, t]` 段，形成「从孩子端生长到父亲端」的展开效果；`progress < 1.0` 时不绘制箭头端点，避免箭头先于曲线出现。默认值为 `1.0`（完全绘制），对其他模块零影响。

`src/modules/IModule.{h,cpp}` — 新增共享助手：

```cpp
void playEdgeGrow(GraphEdge* edge, int duration = 200);
```

`src/modules/HuffmanTreeModule.cpp` — `HF_JELLY` 帧在父结点果冻出现的同时把两条子边的 `drawProgress` 置 0 并在 200ms 内生长到 1.0；新增 `completeEdgeGrowth()`，在 `HF_PICK` / `HF_JELLY` / `HF_MERGE` 三帧入口调用，保证快速播放时边不会停在半画状态。

实测采样：t=45ms 时 progress≈0.42，t=90ms 时≈0.75，t=135ms 时已 1.00。

### 验证

- 新增 `tests/HuffmanFrameDump.cpp`（`SmartGroundHuffmanFrameDump`）：按帧号打印所有可见结点的坐标、并在 `HF_JELLY` 帧采样各边的 `drawProgress`，用于定位落点与残影问题。
- 全量 **79 个测试可执行文件零失败**，含 `SmartGroundBatch9Visual`（哈夫曼树 11 结点 / 队列 11 副本 / 10 条边断言全绿）。

## 第 22 批：哈夫曼树行列/无向边/频率数组修正 + 扩展欧几里得文字去重叠与飞入对齐（2026-11-19）

PM 提交 4 张截图，指出哈夫曼树「整棵树乱完了 / 树的边不需要箭头 / 频率数组全空」与扩展欧几里得「文字重叠 / 公式飞入太偏左」五个问题。

### ① 哈夫曼树整棵树层次错乱（BUG-216）

`src/modules/HuffmanTreeModule.cpp` — `simulateStates()`

把「矮子树整体上移对齐」所需的**行差**错算成了**结点 id 之差**：

```cpp
const int lower = (aRow < bRow) ? a : b;
const int higher = (aRow < bRow) ? b : a;
const int delta = higher - lower;   // 这是 id 之差，不是行号之差
```

id 随结点编号增长，delta 一路放大到 9、11，树高彻底失控 —— 根结点（120）被画到 y=382 而叶子（5）落在 y=574，根跑到叶子下方。修复后 delta 取 `higherRow - lowerRow`，行高回到理论值，最终树为「根在最上、叶子在最下、每个父结点严格高于其孩子」的规范结构，最小结点间距 56.08px，无交叉线。

### ② 树的边不需要箭头（BUG-215）

`src/modules/HuffmanTreeModule.cpp` — `buildLayout()`

两条子边由 `new GraphEdge(child, parent, true)` 改为 `false`（`is_directed = false`），`GraphEdge::paint()` 不再绘制箭头端点。0/1 路径徽标由 `pathLabel` 机制单独绘制，不受影响。

### ③ 频率数组全空（BUG-214）

`src/modules/HuffmanTreeModule.cpp` — `buildLayout()` / `renderFrame()` / `runAlgorithm()`

根因：数组格子按 `m_nodes.size()`（2n-1，**含内部结点**）创建，而排序帧携带的置换数组长度是 n（叶子数），于是 `HF_SORT_DROP` 里的 `perm.size() != m_arrayCells.size()` 守卫**永远成立**、提前 `return`，数值一个都没写进去；叠加初始态 `showValue = false`，数组自然是全空。修复：格子数改为 `m_leafCount`；初始态与 `HF_RESET` 都直接写入原始权值；排序阶段改为「`HF_SORT_LIFT` 该格清空并飞出数值 → `HF_SORT_DROP` 数值流星般落回」成对推进，帧内携带当前置换数组，保证回放与拖动进度条可复现。

### ④ 扩欧文字重叠（BUG-217）

`src/modules/ExgcdModule.cpp` — `buildLayoutBlocks()`

行距（`kExRowStep = 26`、回代 24）小于 mono(13) 的实际行高（28px），回代各行、辗转相除左右两栏、章节标题与正文全部压在一起。版面重排为三段式：等式段 `kExEqY = 80`（步距 34）、辗转相除段标题 `kExGcdTitleY = 190` / 栏头 `kExHeadY = 222` / 行首 `kExRowY = 252`（步距 32）、左右两栏分置 `x = 100` 与 `x = 500` 互不侵占、回代段步距 32。文本框两两相交检测由 **15 处降为 0 处**。

### ⑤ 公式飞入太偏左（BUG-213）

`src/modules/IModule.{h,cpp}` + `src/modules/ExgcdModule.cpp`

两处根因：① `playTextFlight()` 把终点当**中心**处理（`end.x - halfW`），而调用方传的是目标文本的**左上角**，落点因此偏左半个字宽；② 回代片段的前缀宽度用**硬编码 7.6px/字符**估算，与 mono(13) 实际约 17px/字符严重不符，片段被甩到行首左侧。修复：新增共享助手 `playTextFlightAligned(text, start, endLeft, ...)`（起点终点均为左对齐语义），扩欧四处飞行全部改用；`ExgcdBackStep` 新增 `prefixText` 记录片段前的真实前缀文本，落点改用 `QFontMetrics(target->font()).horizontalAdvance("1 = " + prefixText)` 实测宽度（末行 425px，原估算约 170px）。回代片段现精确落进括号位置。

### ⑥ 配套改动

| 位置 | 新增 |
|------|------|
| `src/modules/IModule.{h,cpp}` | `playTextFlightAligned()`（左对齐语义的文本飞行）、`playValueFlyOut()`（数值自格子中心向上飞出） |
| `src/modules/ExgcdModule.{h,cpp}` | `ExgcdBackStep::prefixText` 字段；`renderTerms()` 改为单遍拼接并记录片段真实起始偏移 |
| `tests/Batch9ModulesTest.cpp` | 新增断言：哈夫曼频率数组恰好 n 格且每格有值、根在最上行、每个父结点严格高于其孩子、树边无箭头；扩欧文本框两两无相交 |

**验证**：`build/tests/` 下 **78 个测试可执行文件全部零失败**。关键项：`SmartGroundBatch9Modules`（新增断言全绿）、`SmartGroundBatch9Visual`、`SmartGroundOverlapAudit`、`SmartGroundLayoutFit`、`SmartGroundHuffmanLayoutDump`（中点违例 0、最小结点间距 56.08px）。

## 第 21 批：指针首现飞入修正 + 三模块排版去重叠 + 全量回归（2026-11-19）

收尾上一批动画修正后剩余的指针审计失败与排版冲突，并跑完全量回归。

### ① 指针首现不再从原点飞入（BUG-162）
`src/modules/IModule.cpp` — `playPointerMove()`

循环队列 `head`/`tail`、字符串哈希 `i`、LCS `i`/`j` 的箭头第一次出现时，会从画布左上角 (0,0) 一路飞到目标格子（视觉上像被斜线"拽"过来）。根因是这些指针在 `buildLayout()` 里创建后从未定位，`pos()` 仍是默认 (0,0)，而 `playPointerMove()` 直接拿 `ptr->pos()` 当动画起点。修复后：起点为 `QPointF()` 时直接 `setPos(targetPos)` 瞬移到位，不启动动画。

### ② 树状数组指针 i 落点修正（BUG-163）
`src/modules/FenwickTreeModule.cpp` — `movePointerTo()`

落点原本写成 `t->pos() + QPointF(t->cellWidth()/2, t->pos().y() + t->cellHeight())`，把"格顶+格高"的**绝对**纵坐标当作**相对**偏移又加了一次 `t->pos().y()`，导致箭头插进格子内部并压住相邻格；水平方向也用了整块 `t` 的宽度求中点，而 `t[i]` 只占该宽度的一部分。现改为落点 `t->pos() + QPointF(cellX, t->cellHeight())`，水平中心按所属 level 区间换算 `cellX = (index - slot.left) * m_cellW + m_cellW/2`。

### ③ 链式前向星排版去重叠（BUG-164）
`src/modules/ForwardStarModule.cpp` — `layoutArrays()` / `buildLayout()`

四列行标题、下标数字、右侧图区三者互相压字：行标题右边界越过第一列方块（`weight[k]` 本身就有约 110px 宽）；下标行紧贴方格上沿，文字实际高 26px 而非 22px，底边越界 4px；图区中心 x=1010 距最右数组列仅 32px；`k` 指针尾部标签正落在 `nxt` 行下标带内。修复：标题左移到 `m_arraysX - 178`、下标行上移到 `rowY - 32` 并关闭方块自带标签、图区中心右移到 1120、`k` 指针标签改放箭头左侧。

### ④ 堆模块排版去重叠（BUG-165）
`src/modules/HeapTreeBase.h` — `layoutHeap()`

`heap[]（数组存储）` 紧贴第一列左侧压住下标数字，树结构标题正落在根结点 subLabel 带内。现分别下移到数组行下方（`arrY + 48`）与上移到 `treeTop - 96`。

### ⑤ 测试同步与全量回归（BUG-166）
`tests/LayoutFitTest.cpp` 的 to 行 y=248 写死值改为按行序动态定位；`tests/GroupAUiTest.cpp` 断言同步新模块名「完全背包 DP」「二分图匹配 - 匈牙利算法」「堆调整 up / down」。

**验证**：`build/tests/` 下 **75 个测试可执行文件全部零失败**。关键项：`SmartGroundBatch9Visual`（指针审计 8 组全绿）、`SmartGroundOverlapAudit`（19 个模块零冲突）、`SmartGroundLayoutFit`、`SmartGroundHuffmanLayoutDump`（中点违例 0、最小结点间距 56.08px）、`SmartGroundBatch9Modules`。

## 第 19 批：哈夫曼树六阶段重写 + WPL 修复（2026-11-18）

PM 提交 8 张参考草图并要求哈夫曼树严格按草图重做 choreography，同时修正 WPL 总数。

### ① 哈夫曼树整模块重写（BUG-203 / BUG-204）

`src/modules/HuffmanTreeModule.{h,cpp}` 整模块重写为六阶段：

- **阶段一 排序**：顶部数组初始为空，7 个数字（`45 13 20 16 9 12 5`）先悬停在各自格子正上方（`kHoldY = 86`），再按升序**一个一个**飞入正确格子（`5 9 12 13 16 20 45`）。`HF_SORT_LIFT` → `HF_SORT_DROP` × n → `HF_SORT_DONE`。
- **阶段二 候选队列**：全部数字从数组飞到底部候选队列的 7 个圆形结点（`kQueueY = 664`），按 `(权值, 是否叶子, id)` 排好。同权值时**优先取叶子**（BUG-204，与课堂约定一致）。
- **阶段三 合并 choreography**：每次合并四帧 —— `HF_PICK`（取两个最小值并高亮）→ `HF_PROMOTE`（树高不同时矮的那棵子树整体上移对齐）→ `HF_JELLY`（在两子结点**中点**果冻般长出新结点，`OutBack` 生长 + 涟漪扩散 + 橙色新边）→ `HF_MERGE`（父结点副本从树中飞下落回候选队列，被取走的两成员淡出，其余**左右让位**重排）。6 次合并全部按草图逐帧对齐。
- **阶段四 根结点**：`HF_FINAL` 根结点染专用根色并缩放强调。
- **阶段五 编码**：沿根到叶路径逐段贴出左 0 右 1 徽标，填写右侧 `n × 3` 编码表（`a[i]` / 编码 / 码长）。
- **阶段六 WPL**：底部逐项高亮 `a[i]×码长` 并累加到汇总格。

**树布局引擎**：新增 `subtreeWidth()` / `layoutComponent()` / `componentRoots()`，递归计算子树占宽，父结点严格居中于两子结点中点；森林中多棵子树按宽度依次排布并统一居中，新结点插入后子树自动左右分开保持间距均匀。

**② WPL 总数修复（BUG-202）**

`runAlgorithm()` 的 WPL 段在累加循环**结束之后**才执行 `m_wplTotal = running;` 并构造 `HF_WPL_DONE` 帧；此前该帧读取的是尚未写入的旧值。修正后默认数据 WPL = 45×2 + 13×3 + 20×2 + 16×3 + 9×4 + 12×3 + 5×4 = **309**，等于全部内部结点权值之和（14+25+30+45+75+120），符合哈夫曼最优性。

### ③ 编码回填修复（BUG-205）

`HF_CODE` / `HF_WPL` 帧文案在 `runAlgorithm()` 阶段就按 `m_codeRows[r].code` / `codeLength` 拼装，但这两个字段原先要到播放阶段的 `HF_CODE` 帧才被回填，导致讲解文字显示「编码为 ，码长 0」。现改为 `computeCodes()` 求出 `m_codeMap` 后立即回填，并把 `buildLayout()` 的调用顺序调整为 `computeTree() → simulateStates() → buildCodeTable() → computeCodes()`。

### ④ 配套改动

| 文件 | 说明 |
|---|---|
| `src/render/GraphElements.{h,cpp}` | `GraphNode` 新增 `Q_PROPERTY(qreal scale READ scale WRITE setScale)` 与 `m_scale`，`paint()` 内 `painter->scale(m_scale, m_scale)`，使 `playNodeAppear` / `playScalePulse` 的果冻缩放生效 |
| `src/engine/AnimFrame.h` | 新增 `HF_SORT_LIFT` / `HF_SORT_DROP` / `HF_SORT_DONE` / `HF_QUEUE_DROP` / `HF_JELLY` / `HF_PROMOTE` / `HF_FINAL` |
| `tests/HuffmanShot.cpp` + `tests/CMakeLists.txt` | 新增 `SmartGroundHuffmanShot` 逐帧截图生成器，覆盖六阶段共 55 帧 |
| `tests/Batch9VisualTest.cpp` | 哈夫曼专项断言按六阶段模型重写：候选队列落位帧、果冻帧与合并帧一一对应、编码帧与 WPL 项帧各等于叶子数、树结点 11 个 / 队列副本 11 个 / 边 10 条 |

## 项目概况

SmartGround 是面向 CSP-J/S 算法教学的 Qt6 Widgets 桌面程序，使用 C++17 和 CMake。用户选择模块、输入数据后，模块先生成 AnimFrame 序列，界面再逐帧解释和绘制这些动作。

## 当前代码状态

- 菜单中注册了 63 个唯一可运行模块（57 个算法模块 + 6 个自由结构沙盒模块），下拉条目与唯一模块一一对应。
- 已无待实现的头文件骨架。
- 本轮改动已通过 Qt 6.11.1 / MinGW 13.1 构建，全部测试套件在 `offscreen` 平台下串行全绿。
- 项目已纳入 `D:\Program\Cpp` 的 Git 仓库（SmartGround 位于 `Project/QT/SmartGround` 子路径），可查看提交历史与工作树状态。
- 根目录 main.cpp 是辅助入口；CMake 实际编译 src/main.cpp。
- Release 产物以 Windows GUI 子系统链接（`WIN32_EXECUTABLE TRUE`），`windeployqt` 打包后不再弹出黑色控制台窗口。
- 状态栏右下角显示版本徽标 `0.7.<build stamp>`（`src/core/AppVersion.h`），构建序号由 CMake 自动取 Git 提交数并写回 `AppVersionGenerated.h`，工作区有未提交改动时 +1。

### 已注册模块

- 排序：BubbleSortModule、SelectionSortModule、InsertionSortModule、ShellSortModule（希尔排序）、QuickSortModule、MergeSortModule（归并排序：真实「裂开—归并」重构，二分分裂时数组插缝、合并完成时格子拼接回整段，未排序段与已排序段分色）、CountingSortModule（计数排序）、BucketSortModule（桶排序）、RadixSortModule（基数排序）
- 数据结构：StackModule、QueueModule、CircularQueueModule（循环队列）、MonotonicStackModule、PrefixSumModule、DifferenceModule、HeapBasicsModule、HeapAdjustModule（up/down）、HeapSortModule、FenwickTreeModule（树状数组 BIT）、HashTableModule（哈希表 开放寻址）
- 搜索：BinarySearchClosedModule、BinarySearchHalfOpenModule
- 图论：DSUPathCompressionModule（并查集：统一模块，`pa[i]=i` / `sz[i]=1` 初始化，`merge(u,v)` 按大小合并 + 路径压缩，右侧 `pa[]` / `sz[]` 两行实时同步）、TarjanModule、TarjanCutVertexBridgeModule、TopologicalSortModule（拓扑排序：删边打 ✕ 徽标 + `-1` 入度递减、入度表与图结点同序对齐、成环染红）、DijkstraNaiveModule（朴素 Dijkstra：右侧「候选尺」按 `dist` 升序显式展示「为什么选它」，松弛失败用红线划掉）、DijkstraHeapModule（堆优化）、ForwardStarModule（链式前向星）、KruskalModule（Kruskal：`find` 逐跳上溯 + 路径压缩可视化，成环边染朱红并画出红色闭环）、PrimModule（Prim 最小生成树）、BFSModule、DfsModule（DFS 深度优先搜索）、BipartiteModule（二分图判定）、EulerModule（欧拉路径）、HungarianModule（二分图匹配）
- 动态规划：Knapsack01Module、KnapsackCompleteModule、LISModule、LCSModule（最长公共子序列：标签与数组下标分列，消除压字）
- 字符串：KMPNextModule（已改造为 `pi[]` 1-indexed + `#` 哨兵）、KMPMatchModule（匹配过程）、TrieModule（Trie 字典树）、StringHashModule（字符串哈希）
- 贪心：IntervalSchedModule（区间调度）、MonotonicQueueModule（单调队列）、HuffmanTreeModule（哈夫曼树：频率数组初始显示原始权值并逐个飞入排序、底部候选队列、果冻合并、无向树边、0/1 路径标注、右侧编码表、底部 WPL 累加）
- 线段树：SegmentTreeBuildModule（建树）、SegmentTreeTagModule（区间乘/加懒标记）、SegmentTreeQueryModule（区间查询）
- 数论与树形：SieveOfEratosthenesModule（埃氏筛）、EulerSieveModule（欧拉筛：独立质数库 `prime[]` + `p` 指针，`i*p` 划除后立刻 `i % p == 0 → break`）、FastPowModule（快速幂）、GcdModule（GCD 辗转相除）、ExgcdModule（扩展欧几里得：同余式变形 → 辗转相除双栏 → 递归回代逐层展开，公式左对齐飞入落位，给出模逆元）、TreeSubtreeSizeModule（子树大小统计）、TreeDiameterModule（树的直径）
- 自由结构沙盒（不含算法，仅作讲题草稿）：SandboxArrayModule、SandboxMatrixModule（矩阵 / 行列式 / 表格，带「矩阵设置」特化面板）、SandboxTreeModule、SandboxGraphModule、SandboxBlankModule（空白画布，供老师叠加批注）

## 核心架构

- src/engine/AnimFrame.h 定义统一动作类型和帧数据。
- src/modules/IModule.h 提供模块接口、基类场景指针和公共动画辅助函数，并预置了面向未来的空实现接口（分类、复杂度、教学提示、步长代价、输入校验、画布双击回调、主题应用）。
- src/modules/GraphModuleUtils.h 提供共用的图输入解析（按行判定是否带权）与布局辅助。
- src/modules/GraphRenderHelper.h 提供共用的建点、建边、查边与标签函数，消除图模块重复布局代码。
- src/modules/SegmentTreeCommon.h 提供线段树共用的 `ls`/`rs` 移位约定、输入解析、树形拓扑生成（节点坐标由递归二分展开）。
- src/modules/SegmentTreeEditBase.h 为懒标记与查询两模块的共享基类，统一树布局、`tr`/`add`/`mul` 三条物理数组、节点高亮与指针移动逻辑。
- src/modules/HeapTreeBase.h 为堆类模块的共享基类，统一层序数组排布、完全二叉树拓扑构建与数组值到树节点的同步。
- src/modules/TwoRowArrayModule.h 为前缀和 / 差分等「双行数组」模块的共享基类，统一输入解析、双行布局构建、`PD_*` 帧渲染与多源数字飞入动画。
- src/modules/TreeDpBase.h 为树形 DP 类模块的共享基类，统一「每行一个父节点 + 子节点列表」的输入解析、叶子对齐的树布局与节点徽章设置。
- src/render/GraphElements.h 与 cpp 定义数组、指针、栈、图节点和可交互图边等图元；`GraphNode` 另支持子标签（区间 `[l,r]`）、值文本（`tr=值`）与懒标记徽章（`tagText` + `tagColor`）；`GraphEdge` 支持自定义线宽（`setPenWidth()`），供割点桥模块把桥加粗高亮；`LinkLine` 提供贝塞尔飞线（实线 / 虚线 / 箭头 / 弯曲量）供前向星与希尔排序模块使用；`Ripple` 提供扩散波纹供树直径与埃氏筛模块使用；`ArrayBlock` 支持划除标记（`setStruck()`，保留数字并叠加删除线）、独立值字号（`setValueFontSize()`）；`PointerBlock` 支持四个方向（`setDirection(Direction)`），供前向星模块把指针 k 从 to 列左侧横向指向，并新增 `setLabelToRight()`（标签移到杆右侧）与 `setAnchorOffset()`（结点锚点偏移），供新模块避免指针压住被指向的圆。
- src/render/ValueBar.h 与 cpp 提供比例数值条（条长 = 数值大小），GCD 模块用它表达 `a` / `b` / 余数 `r` 的量级关系。
- src/render/SegmentBar.h 与 cpp 提供时间区间条（支持 muted 半透明态），区间调度模块用它表达活动区间在时间轴上的占位与冲突淘汰。
- src/render/SceneGrid.h 提供隐形对齐网格（磁吸对齐 + 单元格吸附 + Ctrl 自由拖动）。
- src/core/ViewportFitter.h 统一画布居中与自适应缩放。
- src/canvas/ 为画布批注层（双层架构，详见下方「画布批注层」章节）：AnnotationScene、AnnotationItem 及其子类 StrokeItem / ShapeItem / TextBoxItem / MarkdownItem / LatexItem / CanvasTableItem，ToolController 工具状态机，CanvasView 叠加渲染，LatexService Node 渲染助手。
- src/ui/UiTheme.h 为全项目设计令牌（颜色、圆角、间距、字体）。
- src/ui/widgets/ 为自研控件库：MyButton、MyToggleSwitch、MyRadioButton、MyCheckBox、MyComboBox、MyTextEdit、MyCollapsible、MySegmentedControl、ToolIconButton。MyComboBox 为真正的 `Qt::Popup` 浮层（自绘圆角卡片 + 内嵌滚动区），长列表可滚动；支持 `addGroup()` 分组模式，展开为 11 类分组的 3 列按钮瓦片网格；瓦片样式仅保留 `ComboVisualStyle::VueLight`（Vue 扁平流线）：悬停仅淡入强调色、选中态柔和底 + 3px 圆角指示条，分组标题为圆点 + 渐隐细线；弹层阴影由 `ComboPopupFrame` 内 12 层同心圆角矩形叠加绘制柔和渐隐（不再使用会把阴影裁切成硬边的 `QGraphicsDropShadowEffect`）；选项字号 10pt、分组标题 10pt、按钮 11pt。ToolIconButton 以 `QPainterPath` 手工绘制矢量图标，供浮动轻套件使用。
- src/ui/MainWindow.cpp 管理模块注册、输入、构建、录制、播放、撤销和循环播放，并渲染统一的「特化设置」折叠面板（图结构选项与模块自身设置合并在同一区域）；模块按 14 个分类分组注册，`comboToModule` 把下拉条目序号映射到唯一模块实例下标（当前每个模块只出现在一个分组中，无跨分支别名）。

## 第 18 批：扩展欧几里得推导链重写 + 哈夫曼编码表与 WPL + LCS 坐标修正（2026-11-17）

PM 一次性提交 3 组需求：哈夫曼树（6 项：空格子弹跳落位、树自上而下生长并逐行对称居中、边中点标左 0 右 1、叶子生成哈夫曼编码、右侧 n×3 编码表、底部 WPL 逐项累加）、LCS（文字与数组下标重合）、扩展欧几里得（按 `11914x ≡ 1 (mod 146415)` 的课堂讲授流程完整重写演示）。

### ① 扩展欧几里得推导链重写（BUG-199 / BUG-200）

`src/modules/ExgcdModule.{h,cpp}` 整模块重写，默认输入改为 `11914 146415`，画布自上而下分为三个阶段：

**第一步 · 同余式变形**：三行等式逐行高亮 —— `11914·x ≡ 1 (mod 146415)` → `11914·x = 146415·y + 1` → `11914·x - 146415·y = 1`（`EX_EQUATION` 帧）。

**第二步 · 辗转相除双栏对照**：左栏为取余式 `146415 % 11914 = 3447`，右栏为整除式 `146415 = 12 × 11914 + 3447`，共 9 轮，与课堂板书逐行一致；「行」指针（`Direction::Left`，锚点偏移 -26px，保证箭头体完全在文字之外）锚定当前行，左右两式同时高亮。

**第三步 · 递归回代逐层展开**：`buildBackSteps()` 用「项表（index, coef）」递推。从 `1 × gcd` 出发，每轮取最高次项按 `r_k = r_{k-2} - q·r_{k-1}` 展开后合并同类项；回代式先由右侧整除式行**飞入**画布，再展开为化简后的整式：

```
1 = 10 × 1 - 1 × 9
1 = 10 - (29 - 2 × 10) = 3 × 10 - 29 × 1
1 = 3(68 - 2 × 29) - 29 = 3 × 68 - 7 × 29
1 = 31(1573 - 5 × 301) - 7 × 301 = 31 × 1573 - 162 × 301
...
1 = 15079 × 11914 - 1227 × 146415
```

终帧给出绿色结论：**11914 在模 146415 意义下的乘法逆元是 15079**。

同时把输入上限由 `100000` 放宽到 `1000000`（BUG-200），否则 `146415` 这类合法数据会被判非法。

### ② 哈夫曼树深化（编码表 / 路径标注 / WPL）

- **空格子弹跳**：`ArrayBlock` 初始 `showValue = false` 且着色为空格态，数值通过 `IModule::playValueDropInto()` 从空中落入**格子中心**（`OutCubic` 缓动，动画结束才提交值）。
- **树自上而下生长**：根节点位于顶部、叶子位于底部；每次合并前兄弟子树先旋转/上移对齐到同一行再连线；`computeXPositions()` 以「叶子等距铺开 + 内部结点取左右孩子中点」计算位置，随后按行统计 `levelMin/levelMax` 做整体平移，使**每一行都居中、整棵树对称**。
- **路径标注与编码**：`GraphEdge` 新增 `pathLabel` / `pathLabelColor` / `setPathLabel()` / `pathLabelAnchor()`，在边中点绘制圆角徽标显示 **左 0 右 1**；`HF_CODE` 帧沿根到叶逐条揭示路径标签并写入编码表。
- **右侧 n×3 编码表**：第 1 列 `a[i]` 为叶子频率且**按初始输入顺序**（非排序后顺序），第 2 列为哈夫曼编码，第 3 列为码长。
- **底部 WPL 逐项累加**：`n` 个格子逐项显示 `a[i]×码长`，右侧 `WPL` 累加格逐项刷新并脉冲高亮，`HF_WPL_DONE` 给出总 WPL。

### ③ LCS 坐标修正（BUG-201）

`kLcStrY` 由 62 调到 92、`kLcGridY` 由 206 调到 236；「串 a[]」/「串 b[]」标签上移到 `kLcStrY - 26`，矩阵列下标移到 `m_gridY - 22`，行下标移到 `m_gridX - 34`，标签与下标各占独立行，彻底消除文字与数组下标重合。

### ④ 共享能力与图形元素扩展

| 位置 | 新增 |
|------|------|
| `src/modules/IModule.{h,cpp}` | `playValueDropInto(start, target, value, color, duration)`（数值落入格中心）、`playTextFlight(text, start, end, fontSize, color, duration)`（任意文本飞行，哈夫曼与扩欧共用） |
| `src/render/GraphElements.{h,cpp}` | `ArrayBlock::centerPos()`、`FloatingText::fontSize` / `textColor`、`GraphEdge` 路径标签徽标 |
| `src/engine/AnimFrame.h` | `EX_EQUATION`、`HF_CODE`、`HF_WPL`、`HF_WPL_DONE` |

### ⑤ 测试与验证

- `tests/Batch9ModulesTest.cpp`：扩欧用例改用 `11914 146415`，新增断言 —— 逆元 `x == 15079`、`11914×15079 - 146415×1227 == 1`、最终回代式同时含 `15079` 与 `1227`、`EX_EQUATION` 帧数为 3。
- 全量回归：`build/tests` 下全部测试可执行文件在 `offscreen` 平台串行全绿（**TOTAL FAILURES: 0**）。

## 第 17 批：哈夫曼树动画重构 + 排版修复 + 字符串哈希对标课堂模板（2026-11-17）

PM 一次性提交 8 项需求（桶排序竖排压线、基数排序同上、循环队列归类与冗余文案、循环队列指针方向与操作日志、哈夫曼树两阶段动画、字符串哈希对标课堂模板、LCS 布局统一），其中扩欧动画逻辑按 PM 要求本批不做、留待单独确认。

### ① 哈夫曼树动画重构（BUG-188 / BUG-189 / BUG-198）

`src/modules/HuffmanTreeModule.{h,cpp}` 整模块重写，拆成两个教学阶段：

**阶段一 · 排序**：顶部展示初始频率数组。乱位元素先「飞出数组」（`HF_SORT_LIFT`，向上飞出并高亮），随后像流星一样逐个飞回自己排序后应在的位置（`HF_SORT_DROP`，帧内携带完整置换数组，保证回放/拖动进度条时状态可复现）；`HF_SORT_DONE` 宣布排序完成。

**阶段二 · 建树**：维护一个「森林」，每次取权值最小的两个结点：

| 情形 | 动画 |
|------|------|
| 两个都是原始数据 | 直接作叶子，连到新生成的父结点 |
| 一个已带子树 | 另一个结点飞过去与之相连，父结点生成 |
| 两个都带子树 | 先把一方上移对齐到同一行，再连线求和生成父结点 |

配套机制：

- **树不是预先摆在画布里的**：所有 2n-1 个结点与 2n-2 条边在 `buildLayout()` 里一次性创建，但初始 `opacity = 0`，随每次合并逐个 `playNodeAppear()` 生长淡入。
- **位姿补间**：`simulateLayout()` 先离线模拟每次合并后的森林形态，记录每一步的 `rows` / `cols` 快照；`computePositions()` 采用「叶子等距铺开 + 内部结点取左右孩子中点」的对称布局。合并时先补间到「上移对齐」位姿（300ms），再补间到「合并完成」位姿（460ms），树高与树宽的变化都有过渡，不瞬移。
- **左小右大**：同一层的结点按排序后的权值排名从左到右升序排列。

### ② 共享动画助手 `playMoveTo`（BUG-189）

`IModule` 新增 `playMoveTo(std::vector<QGraphicsItem*> items, std::vector<QPointF> targets, int duration = 440)`，支持逐项独立目标点的位置补间（`m_animated == false` 时瞬时落位）。哈夫曼树的「多个元素各自飞往不同目标」正是复用此助手。

### ③ 排版与文案修复

| 模块 | 修复 |
|------|------|
| 桶排序 | 桶内竖直数组改为 `cellHeight == spacing`（40px）无缝排布，相邻单元格只保留 1 条线；说明文字上移，Bucket 0 标题不再被压（BUG-191 / BUG-192） |
| 基数排序 | 10 个数字桶改为 `cellHeight == spacing`（38px）无缝排布；「按当前位分的 10 个桶」上移（BUG-191 / BUG-192） |
| 循环队列 | `head` / `tail` 改 `Up` + `PointerGeometry::belowCell()` 自数组下方指上去；删除 `q[N], N = 6`、右下角回绕提示、整套「操作日志」方框（BUG-193） |
| 循环队列归类 | 从「数据结构 · 进阶」移入「基础数据结构」（BUG-194） |
| 字符串哈希 | `p1 = 131`、`m1 = 998244353`；预计算 `p[]` / `h1[]` / `h2[]`；从 `len = min(n1,n2)` 递减枚举，用 `getHash()` 判断前缀-后缀重叠匹配，首个匹配即答案（BUG-195） |
| 字符串哈希指针 | 新增 `STH_POW` 动作码（幂表计算独立成帧）；新增字符游标指针 `i`（s1 行）/ `j`（s2 行），各自行内水平移动（BUG-196） |
| 最长公共子序列 | 删除左上角 `s = "abcbdab"`；矩阵上方横排两个无缝字符串数组（a 串 / b 串，间隔 40px）；指针 `i` 自上而下、`j` 自下而上；底部轨迹区重排消除压字（BUG-197） |

### ④ 测试与验证

- `tests/Batch9ModulesTest.cpp`：字符串哈希输入由 `abcdefg / cde`（答案 0）改为 `abcdefg / cdefg`（答案 5），并新增答案断言。
- `tests/Batch9VisualTest.cpp`：新增哈夫曼树专项 —— 断言排序阶段的抬升/落位帧存在、排序先于建树、6 个叶子合并 5 次、终态 11 结点 10 边。
- 全量回归：`build/tests` 下 69 个测试可执行文件在 `offscreen` 平台串行全绿（`count=69, ALL SUITES PASS`）。

## 第 15 批：矩阵形态与尺寸 + 指针平移动画体系 + 分类合并（2026-11-16）

PM 一次性提交 9 项缺陷（矩阵残影、矩阵/行列式/表格语义反转、单元格只能设宽、Prim 指针偏移、最小生成树分类重复、二分图数组过近与初值、Trie 指针与结点突兀、哈希表指针悬空与指针瞬移、全局代码审计），并要求「这版发给其他老师测别的 bug」。

### ① 矩阵形态语义反转（BUG-181 / BUG-180）

`MatrixBlock::paint()` 改为三形态分流，语义与 PM 要求一致：

| 形态 | 外框 | `sideMargin()` |
|------|------|---------------|
| `PlainMatrix` 矩阵 | 数学方括号 `[` `]`（多段折线，`arm = 13` / `lip = 6` 的上下小唇） | 20.0 |
| `Determinant` 行列式 | 左右单根竖线 + 上下小衬线 | 18.0 |
| `BracketedTable` 表格 | 无任何外框，只有网格线 | 4.0 |

单元格尺寸改为可设宽高：`ModuleField` 新增 `SizeInput` 类型与 `makeSize(key, label, w, wMin, wMax, h, hMin, hMax)` 工厂，面板渲染为「宽 × 高」双数值输入；`MatrixBlock` 新增 `cellHeight`（默认 52）与 `setCellHeight()` / `cellHeightValue()`，绘制、`cellAtPoint()`、就地编辑框定位全部改用矩形单元格，老师可以直接画长方形窗格。设置项标签由「单元格」改为「单元格大小」。

### ② 画布残影清理（BUG-182）

`QGraphicsScene::clear()` 只清场景项，`QGraphicsView` 的背景层与物品层缓存仍留着旧像素，所以形态切换后残影要拖动画布才消失。`MainWindow` 新增 `invalidateCanvas()`，对场景所有视图调用 `invalidateScene(view->rect(), BackgroundLayer)` + `invalidateScene(view->rect(), ItemLayer)`，再强制 `viewport()->update()`；`clearScene()` 与 `rebuildCurrentLayout()` 末尾各调用一次。

### ③ 指针平移动画体系（BUG-177 / BUG-178 / BUG-179 / BUG-174）

这是本批影响面最大的一项。核心问题有两个：

1. **瞬移**：各模块直接 `setPos()` 硬切指针。修复为 `PointerBlock` 新增 `m_placed` 标记与 `isPlaced()`（经 `xChanged` / `yChanged` 信号捕获落位），`IModule::playPointerMove()` 在指针**首次落位**时直接定位（避免从画面原点飞入），**落位之后**一律走 `QPropertyAnimation` 平移过渡；并提供 `retargetPointer()` / `retargetPointerToNode()` / `retargetPointerTo()` 三个语义化重定向入口。
2. **原点污染**：未使用的指针停在 `(0, 0)`，把 `itemsBoundingRect()` 撑大、连带整个画布渲染错位（KMP 快照测试的边框行因此偏移 22px 而失败）。修复：`PointerBlock::boundingRect()` 在 `opacity() < 0.02` 时返回空矩形，并监听 `opacityChanged` 触发 `prepareGeometryChange()`，隐藏指针不再污染场景包围盒。

配套的落位与标签归位：

| 模块 | 改动 |
|------|------|
| Trie | `i` 锚点偏移 `(27, 19)` 精确落在格子中心；标签用 `setLabelAtTail(10.0)` 推到杆尾之外，不再与箭头杆身重叠；`TR_CREATE` 帧新建结点播放 `playNodeAppear()`（`QEasingCurve::OutBack`，0.15 → 1.14 → 1.0 缩放弹入 + 透明度淡入），不再突兀冒点 |
| 哈希表 | `k` 改 `Down` 方向、`shaftLength = 26`、锚点偏移 `(cellW / 2, 19)`，尖端正落槽位中心，标签置于杆尾 |
| Prim | `u` 改 `Up` 方向、锚点偏移 `(0, 20)`、`labelOffset = (0, 30)`、`shaftLength = 24`，尖端落在半径 20 的顶点圆边缘 |

### ④ 分类合并与二分图布局（BUG-175 / BUG-176）

Kruskal 与 Prim 合并入单一分组「图论 · 最小生成树」，去重后唯一模块数 56 → 54，`tests/GroupAUiTest.cpp` 的期望值同步更新。二分图的 `col[]` 数组整体下移（标签 `y = 470`、数组行 `y = 487`、结论 `y = 610`），初值由 `"—"` 改为 `"-1"`，与 `m_color` 的未染色语义一致。

### ⑤ 全局审计与修复（BUG-183 / BUG-173a）

- **字体统一（BUG-183）**：全项目审计发现 `src/` 下仍残留绕过 `UiFonts` 引擎的 `QFont(...)` 字面量 —— `GraphNode::paint()` 的中心值 / `v:` / `dfn: low:`、`StackBlock::paint()`、`ArrayBlock::paint()`、`HeapBasicsModule` 的两个英文标题、`TarjanModule` 的 `ins / stk`、`GraphRenderHelper::addLabel()`。全部改走 `UiFonts::mono()` / `UiFonts::cjk()`，落实 BUG-151 的字体规范。
- `EulerModule.cpp` 中 `统计每个节点的入度与出度` 的损坏字节修正为合法 UTF-8；同时对 `src/` 下全部 `.cpp` / `.h` 做严格 UTF-8 校验，0 个文件存在非法字节序列。
- `tests/OverlapAuditTest.cpp` 采集 `PointerBlock::anchorItem()`，指针与其锚定目标之间的**有意压叠**不再计为冲突（此前 Trie 的 `i` 与哈希表的 `k` 会被误报）。
- 新增 `tests/PointerMotionTest.cpp`：断言指针落位状态、终态坐标、以及 Prim 的 `u` 尖端落在顶点圆边缘。
- 指针动画入口收敛：`IModule::playPointersSync()` / `layoutPointerSlots()` / `StackModule` / `ForwardStarModule` 四处各自手写的 `acquireAnim(ptr, "pos", ...)` 统一改调 `playPointerMove()`，全项目指针移动只有一条动画路径，杜绝漏改导致的瞬移。

### ⑥ 测试与验证

- `build/tests` 下 69 个测试可执行文件在 `offscreen` 平台串行全绿（`SmartGroundOverlapAudit` 0 冲突、`SmartGroundPointerMotion` 全绿、`SmartGroundGroupAUi` 54/54）。
- 主程序 `SmartGround.exe` 编译通过，无警告级错误。

## 第 14 批：10 项缺陷整改 + OI-Wiki 分类重构 + 版本徽标（2026-11-16）

PM 一次性提交 10 项缺陷（面板不可见、GCD 乱版、DFS 指针裁切、Prim 逻辑炸裂、计数排序前缀和、二分图染色、欧拉路径入度、Trie 静态遍历、哈希表空隙与冲突缺失）以及三项架构要求（文本自适应延时下沉到核心层、分类按 OI-Wiki 重排、版本计数器）。

### ① 折叠面板内容不可见（BUG-173 / BUG-172）

根因在 `MyCollapsible`：`m_contentHeight` 只在 `setContentWidget()` 里计算一次，且当时内容控件的布局尚未建立、`sizeHint()` 返回 0，导致内容区高度被钉死为 0 —— 面板标题可见、内部控件全部不可见，「矩阵设置」与「图结构选项」同时中招。

修复：`refreshContentHeight()` 做 `updateGeometry()` → `adjustSize()` → `sizeHint()` 三级回退测量并同步 `setMaximumHeight`/`setMinimumHeight`；`eventFilter()` 拦截内容控件的 `QEvent::LayoutRequest` 自动重测；`setContentWidget()` 统一走 `updateGeometryAnimated()`。

### ② 九个新模块逐项整改

| 模块 | 整改 |
|------|------|
| GCD | 整模块重写：删除堆叠的 `ValueBar` 槽位，画面只留 `a` / `b` 两个大号数字框与余数框 `r`，中央直书 `a % b = r`，每轮较小者被 `a % b` 替换较大者，历史轮次变灰（BUG-166） |
| DFS | `PointerBlock::labelWidth()` 与 `paint()` 统一 `UiFonts::mono(14, true)`，宽度改 `qMax(28.0, adv + 14.0)`；新增 `setAnchorItem(QGraphicsItem*)` 支持锚定任意图元（BUG-165） |
| Prim | `runAlgorithm()` 主循环前先 `relaxFrom(m_src)` 播种起点出边候选集，不再因第一轮候选集为空而误报「存在孤立分量」（BUG-167） |
| 计数排序 | 前缀和阶段改用 `IModule::playMultiValueFlight`，`prefix[i-1]` 与 `cnt[i]` 同时飞出合并落入 `prefix[i]`（BUG-168） |
| 二分图 | 染色写入与色块刷新彻底隔离，子节点染色不再覆写父节点色块；图内结点背景色与数组颜色标记同源刷新（BUG-169） |
| 欧拉路径 | 新增有向图开关；有向时分别统计并渲染 `in[]` / `out[]` 双数组，无向时回落 `deg[]`；新增 `EU_DEGREE` 动作码（BUG-172） |
| Trie | 改为真正动态建树：起始只有根节点、游标 `p` 指向根，字符串以数组呈现并由指针 `i` 逐字符右移，命中复用 / 未命中新建；下方用一条无缝数组模拟树形结构内存槽位（BUG-170） |
| 哈希表 | 数组改 `cellWidth == spacing` 无缝排布；默认数据改 `5\n3 10 17 24 31`（模 7 后全冲突）完整演示线性探测；新增 `HT_PROBE` 逐步探测动画（BUG-171） |

### ③ 核心层文本自适应延时（BUG-165a）

把「解说文字越长、等待越久」下沉到核心基类，不再由各模块各自为政：`IModule::frameDelayMs(const AnimFrame&)` 与 `IModule::frameDelayMsForText(const QString&)` 统一按「解说 + 公式」文本长度计算，下限 900ms、上限 3000ms；`MainWindow::advanceFrame()` 改调基类实现。此后新增模块自动继承该能力。

### ④ OI-Wiki 分类重构

下拉分组调整为：自由结构 / 基础数据结构 / 排序 / 树与堆 / 数据结构 · 进阶（并查集、Kruskal、树状数组、线段树、**哈希表**）/ 图论 / 匹配 / 动态规划 / 字符串 / 数学 / **贪心**（区间调度）/ 树形 DP，另设「图论 · 最小生成树」跨分支组（Kruskal + Prim）。哈希表回归数据结构大类，贪心独立成组。

### ⑤ 版本徽标

新增 `src/core/AppVersion.h`：`AppVersion::display()` 返回 `0.7.<build stamp>`。构建序号由 CMake 配置阶段自动取 `git rev-list --count HEAD`（工作区有未提交改动时 +1），写回 `generated/AppVersionGenerated.h`；`AppVersion.h` 用 `__has_include` 探测该生成头，缺失时回落到 `0.7.0`，保证任何构建方式都能编译。`MainWindow` 状态栏右下角新增 `labelVersion`（objectName `versionBadge`，弱化配色 + tooltip `SmartGround v0.7.<stamp>`），不占标题也不做标签页。当前显示 `0.7.6`。

### ⑥ 测试与验证

- `tests/Round10FixTest.cpp`：覆盖九项整改的行为断言，无缝行探针坐标随新布局更新（Trie 字符串行 y=500、槽位行 y=596；哈希表键行 y=222）。
- `tests/AutoPlayEndTest.cpp`：自适应延时使冒泡排序全流程超过原 72s 等待预算，等待循环由 600×120ms 放宽到 4000×120ms。
- `tests/UiBehaviorTest.cpp`：延时上限断言由 3200ms 改为 3000ms，与基类实现对齐。
- `tests/VersionBadgeTest.cpp`（新增）：断言版本号格式为 `主.次.构建`、状态栏徽标存在、徽标文本与 `AppVersion::display()` 一致且带 tooltip。
- 全量回归：`build/tests` 下 59 个测试可执行文件在 `offscreen` 平台串行全绿（`SmartGroundOverlapAudit` 0 冲突、`SmartGroundRound10Fix` 40/40 通过）。

## 第 13 批：矩阵设置落实 + 9 个新算法模块（2026-11-16）

### ① 矩阵设置面板（`SandboxMatrixModule`）

PM 完整描述了行列式功能的需求，本批按描述逐条落实：

| 需求 | 落实位置 |
|------|----------|
| 单选形态（矩阵 / 行列式 / 表格），**默认表格** | `moduleSettingsFields()` 返回 `ModuleField::makeSegment("shape", "形态", {"矩阵", "行列式", "表格"}, 2)`，`makeSegment` 走自研 `MySegmentedControl` 渲染为整行分段控件 |
| 行、列**默认 3 × 3** | `ModuleField::makeInt("rows", "行数", 3, 1, 8)` 与 `makeInt("cols", "列数", 3, 1, 8)`；`reset()` 把 `m_rows = m_cols = 3` |
| 数据输入只填矩阵内容值，**按行优先存储** | `buildLayout()` 删除全部 `rows=` / `cols=` / `type=` / `cell=` 键解析，改为把输入区所有 token 按出现顺序（即行优先）填入 `MatrixBlock`，不足处补 `0`；`fillFromTokens()` 按 `for r → for c` 顺序写入 |
| **矩阵模块未打开时「矩阵设置」自动隐藏** | `moduleSettingsTitle()` 返回「矩阵设置」；`MainWindow::rebuildModulePanel()` 在 `fields.isEmpty() && !graphSupported` 时隐藏 `modulePanelHost`，因此切到任何其他模块时该面板整体消失 |

形态三态渲染由 `MatrixBlock::Shape` 承担：`PlainMatrix`（无竖线）/ `Determinant`（米黄底 + 棕色竖线 + 顶部延伸短线 + 标题「行列式」）/ `BracketedTable`（白底 + 深蓝竖线 + 标题「表格」）。

### ② 新增 9 个算法模块

| 模块 | 核心可视化结构 | 关键动作序列 |
|------|----------------|--------------|
| GCD 辗转相除 | `ValueBar` 比例数值条（几何长度 = 数值大小）+ 历史轮次槽位 | 求余 → 余数条脉冲 → 交换 → 历史轮次变灰 → 终值变绿 |
| DFS 深度优先搜索 | `stk` 栈数组 / `vis` 访问标记 / `ord` 访问序 三行数组 + 指针 | 入栈点亮 → 走边 → 标记访问 → 出栈回溯 → 访问序追加 |
| Prim 最小生成树 | `dis[]` 距离数组 + 图边 | 重置 → 选最小候选 → 松弛邻点（飞数值）→ 取边入树 → 收敛判定 |
| 计数排序 | 原数组 → 计数桶 → 前缀和 → 输出数组 | 计数 → 前缀和累加 → 数值飞行落入桶位 → 输出 |
| 二分图判定 | 图节点 + 两侧配色 | 交替染色 → 冲突边警示 → 终止 |
| 欧拉路径 | 度数统计 + 栈 + 路径 | 度数判定 → 压栈 → 走边 → 弹栈 → 路径点亮 |
| 区间调度 | `SegmentBar` 时间轴条 + 时间刻度 | 按结束时间排序 → 逐条扫描 → 选中加粗 / 冲突灰化 |
| Trie 字典树 | 层级树 + `end` 标记数组 | 逐字符下行 → 建新节点 / 复用旧分支 → 末节点标记 |
| 哈希表（开放寻址） | 槽位行 + 关键字行 | 计算哈希 → 冲突线性探测 → 落位插入 |

配套新增渲染件：`src/render/ValueBar.h/.cpp`（比例数值条）、`src/render/SegmentBar.h/.cpp`（时间区间条，支持 muted 半透明态）。`src/engine/AnimFrame.h` 追加 34 个按模块前缀隔离的 `ActionType` 动作码。

### ④ 首轮几何重叠修正（BUG-164）

首轮实现后由 `tests/OverlapAuditTest.cpp` 自动审计出 9 个新模块共 14 处元素压盖，已全部修正：

| 现象 | 修正 |
|------|------|
| 顶部标题文字压住图结点 1 | DFS 图心 y=170→205、Prim y=165→200、Euler y=165→200，标题带与圆环彻底分离 |
| 结点锚定指针 `u` 压住被指向的圆 | `PointerBlock` 新增 `setLabelToRight()`；DFS 的 `u` 改 `Left` 方向 + 锚点偏移 (20,0)；Prim 的 `u` 锚点偏移 (30,30)，标签一律落在杆右侧 |
| 指针压住数组下标行 | DFS 的 `vis` / `ord` 两行整体下移 30px，指针标签移到杆右侧 |
| 计数排序行标签压住单元格 | 四行整体下移 20px；行标签与单元格起点间距由 140px 放宽到 180px（`srcX` / `bucketX` / `outX` 起点由 170 → 210） |

同时把审计器的 `GraphNode` 采样框由宽松的 `boundingRect()` 收紧为真实圆框 `QRectF(-20,-20,40,40)`，并把 `subLabel` / `valueText` 拆成独立标签盒单独参与求交，避免漏报与误报。最终 19 个受测模块 `conflicts=0`。

### ③ 测试与验证

- 新增 `tests/NewModuleBatchTest.cpp`：断言 9 个模块均能产生帧序列、渲染非空画面，并验证矩阵默认形态 / 3×3 尺寸 / 行优先填值 / 缺额补 0 / 形态切换生效。
- `tests/ModuleSettingsPanelTest.cpp`：新增断言「矩阵设置」面板在矩阵模块可见、切到 01 背包自动隐藏、切回恢复；为此在 `MainWindow` 增加 `probeModulePanelVisible()` / `probeModulePanelTitle()`。
- `tests/GroupAUiTest.cpp`：模块数由 46 改为 56，并逐个断言 9 个新模块名可被选中。
- `tests/MatrixShapeTest.cpp`：输入改为纯行优先数值，默认断言 3×3 与补 0 行为。

## 第 12 批：下拉弹层竖线根除 + 侧栏崩溃修复（2026-11-15）

PM 用截图标注了「打开算法模块列表就能看到」的那条竖线，要求只处理这一件事。

**根因不在自绘阴影，而在 Windows DWM 给 `Qt::Popup` 顶层窗口绘制的原生 1px 描边。** `Qt::FramelessWindowHint` 只移除标题栏与系统按钮，不影响 DWM 窗口描边，因此即使 `WA_TranslucentBackground` 全透明渲染，弹层最外一圈仍会出现深灰硬边。

像素实测（3840×2160 物理像素，缩放 1.5）：弹层逻辑位置 (136,174)、尺寸 677×394，窗口右边界在物理 x=1220，该列恒为 `140,140,141` 并贯穿全部 591 行；底边 y=852 同理。卡片自身右边缘在 x=1191（`226,232,242`），二者相距 29px —— 这正是用户红框标注的位置。

**处置**：

- `ComboPopupFrame` 构造时与 `MyComboBox::openPopup()` 中均追加 `Qt::NoDropShadowWindowHint`（0x40000000），关闭系统阴影与 DWM 原生描边；阴影完全交由 `paintEvent` 自绘；
- 自绘阴影的同心圆角矩形扩散半径由 `i * 2.2`（最外层 26.4px）收敛为 `maxSpreadX = 11` / `maxSpreadY = 6` 并改线性 alpha 衰减，确保阴影层完全落在窗口边界内、不被裁切成折角；
- 弹层外边距 (27,18,27,20) → (18,12,18,14)，窗口宽度补偿 +54 → +36；
- 修复后窗口右边界 x=1220 变为纯净 `251,252,254`，右/下边缘均为平滑渐变；A/B 对照确认移除该 flag 后硬线立刻复现。

**顺带修掉的侧栏崩溃**：验证过程中发现切换算法模块会 SIGSEGV（栈顶 `QObject::blockSignals` ← `MainWindow::updateGraphOptionPanel`）。根因是 `rebuildModulePanel()` 清空面板时把跨模块复用的成员控件 `toggleDirected` / `checkBidirectional` 一并 `deleteLater()`，重建时又重新加入已析构指针，形成悬空指针。修复为清空前先 `setParent(modulePanelHost)` 移出待删集合并跳过删除，非图模块下显式隐藏；同时拆开 `updateGraphOptionPanel()` 与 `animateSubOptionVisibility()` 中因语句粘连导致的不可达代码。

配套新增 `tests/PopupEdgeVerify.cpp`（真实屏幕像素级回归，`offscreen` 平台自动跳过）与 `MyCollapsible::headerText()` 访问器（供测试按标题定位「特化设置」面板，修正 BUG-161 的断言错位）。

## 第 11 批：PM 四点反馈收口（2026-11-15）

PM 指出四项未完工：① 下拉弹层外侧仍有一圈包裹整个结构的硬边阴影线（右侧与下侧尤为明显）；② 图结点权值双击改值后新值仍落在圆下方而非圆内；③ 行列式 / 矩阵 / 表格结构基本没按要求做、除行列式外功能残缺；④ 三套下拉风格只保留 Vue 扁平，其余两套删除，且算法模块内文字放大 8%~10%。

### ① 弹层硬边阴影清除

| 问题 | 根因 | 处置 |
|------|------|------|
| 弹层外侧包裹式硬边 | `ComboCard` 上的 `QGraphicsDropShadowEffect` 会把阴影裁切到控件自身矩形，右侧与下侧形成直角折线；弹层外边距左右不等加剧了错位感 | 删除该图形效果；`ComboPopupFrame::paintEvent()` 改为 12 层同心圆角矩形由外到内叠加、alpha 递减、圆角随扩散半径同步放大；外边距改为 (27,18,27,20) 使四周对称（BUG-153） |

### ② 图结点权值入圆

| 问题 | 根因 | 处置 |
|------|------|------|
| 改值后落在圆下方 | `GraphNode::commitEdit()` 的兜底分支调用 `setValueText()`，而 `valueText` 绘制于 `QRectF(-w/2, 22, w, 18)`，即圆外下方 | 统一改为 `setCenterText()`（绘制于圆内 `QRectF(-20,-20,40,40)`）并清空残留 `valueText`；`IModule::refreshLiveSlots()` 同步改为圆内写入；`beginEdit()` 预填优先级改为 圆内 > 自定义 > 下方 > `value`（BUG-154） |

### ③ 矩阵 / 行列式 / 表格重构

| 能力 | 实现要点 |
|------|----------|
| 三形态渲染 | `MatrixBlock` 重写：新增 `sideMargin()`（行列式 18 / 表格 16）与 `barColor` 独立括号配色；行列式与表格改用**真正的左右竖线括号**并带顶部延伸短线；普通矩阵无括号；单元格改 `mono(13)`，双击可就地改值（BUG-155） |
| 面板内调参 | `SandboxMatrixModule` 暴露 `shape`（分段：矩阵 / 行列式 / 表格）、`rows` / `cols`（数值输入，1~8 钳制）、`cell`（单元格尺寸）四个 `ModuleField`；输入格式支持 `rows=` / `cols=` / `type=` / `cell=` 键 |
| 数据保留 | `resizeGrid()` 保留已有单元格；`buildLayout` 在重建时保留用户已覆盖的形态与尺寸，不被输入文本回退 |
| 正确释放 | 新增 `releaseSceneItems()`，配合 `MainWindow::clearScene()` 正确摘除并释放图元 |

### ④ 风格收敛与字号放大

| 项目 | 处置 |
|------|------|
| 风格唯一化 | `ComboVisualStyle` 仅保留 `VueLight`；删除 `paintGlassTile` / `paintMinimalTile` 与 `ComboCard` / `ComboPopupFrame` 中对应分支；`styleNames()` 只返回 `{"Vue 扁平"}`；移除 `MainWindow` 侧栏「界面风格」标签与 `comboStylePicker` 段控件（BUG-156） |
| 字号放大 | 新增常量 `kOptionFontSize = 10` / `kHeaderFontSize = 10` / `kButtonFontSize = 11`；选项内边距 34 → 40；列宽 `qBound(120,...,240)` → `qBound(140,...,300)`，长名称（如「自由结构 - 矩阵 / 行列式」）不再截断（BUG-157） |

### ⑤ 特化设置面板统一

图结构选项与模块设置原本分成两块独立面板，现合并为唯一的「特化设置」折叠面板：`MainWindow::rebuildModulePanel()` 在图模块下自动插入「图的类型」开关、「无向边加双向箭头」复选框与边权提示，随后追加模块自身的 `ModuleField`；`updateGraphOptionPanel()` 不再单独显示 `graphPanelHost`；分段控件独占整行，数值输入框固定 72px 宽（BUG-158）。

新增测试：`tests/MatrixShapeTest.cpp`、`tests/ModuleSettingsPanelTest.cpp`、`tests/StylePickerTest.cpp`（重写）、`tests/GraphInlineEditTest.cpp`（扩充）、`tests/PanelEdgeScanTest.cpp`。

## 第 10 批：橡皮面板紧凑化 + 下拉气泡风格重做（2026-11-15）

PM 指出两处体验问题：橡皮工具点开后沿用画笔面板的高宽显得臃肿，且橡皮形状不可调；算法模块下拉气泡线条杂乱、配色发白、弹层外圈还有一圈椭圆状锯齿阴影，观感廉价且与画笔弹出面板割裂。

### 橡皮面板

| 问题 | 根因 | 处置 |
|------|------|------|
| 面板过高 | `QStackedWidget` 的 `sizeHint` 取所有页面最大值，矮页面被最高页（画笔 + 取色器）撑高 | 新增 `CanvasToolBar::applyToolPageSize()`，按当前页 `sizeHint` 固定尺寸。橡皮面板 477px → **103px** |
| 形状不可调 | `ToolStyle` 无形状字段 | 增 `eraserSquare`；`eraseAt()` 圆形走 `hitTest`、方形走场景路径与矩形求交；画布橡皮光标同步画圆 / 画方；面板加自绘 `MySegmentedControl`「圆形 / 方形」 |

### 下拉气泡三套风格（第 10 批，已于第 11 批收敛为仅 Vue 扁平）

`MyComboBox` 引入 `ComboVisualStyle` 体系，可整体切换：

- **`VueLight`（保留）**：Vue 式扁平瓦片，无左色条、无斜扫光；悬停 160ms 强调色淡入；选中态用强调色 34 透明底 + 左侧 3px 圆角指示条；文字按中西文自动切 Consolas / 微软雅黑。
- ~~**`SoftGlass`**~~：毛玻璃质感，半透明白底 + 内高光边缘 + 强调色渐层。**第 11 批按 PM 要求删除。**
- ~~**`MinimalLine`**~~：极简描边，选中态仅用圆点标记，几乎无线条。**第 11 批按 PM 要求删除。**

弹层阴影在本批曾改用 `QGraphicsDropShadowEffect` 真投影，**第 11 批已因该控件会把阴影裁切成硬边而回退**，改为 `ComboPopupFrame` 内 12 层同心圆角矩形叠加绘制柔和渐隐；卡片保持 16px 圆角 + 极淡描边；分组标题为「6px 圆点 + 文字 + 渐隐细线」；下拉箭头带开合旋转动画。

### 风格切换入口（第 10 批，已于第 11 批移除）

第 10 批 PM 定稿「保留三套并提供运行时切换」，实现为 `MyComboBox::setGlobalVisualStyle()` + 侧栏 `MySegmentedControl`。**第 11 批 PM 要求只保留 Vue 扁平，该切换器与对应 API 入口一并移除**，侧栏「界面风格」标签与 `comboStylePicker` 不再存在（`tests/StartupPanelTest.cpp` 已同步断言其不存在）。

## 第 9 批十四项需求轮（2026-11-15，PM 反馈轮）

PM 一次性提出 14 项跨画板 / 全局 / 算法可视化的改进，已分四簇批量落地。

### 簇一：全局底层规范

| 需求 | 落地做法 |
|------|----------|
| 8. 中文微软雅黑 + 西文 Consolas | 新增 `src/ui/UiFonts.h` 统一字体引擎：`UiFonts::cjk()`（Microsoft YaHei）、`UiFonts::mono()`（Consolas）、`UiFonts::mixed()`（`setFamilies({"Consolas","Microsoft YaHei"})`）按字形族自动回退。全项目文本改用该引擎。 |

### 簇二：画板交互与工具栏

| 需求 | 落地做法 |
|------|----------|
| 1. 元素须能直接拖边框移动 | `AnnotationItem::borderHitTest(pos, tol)` + `ToolController::borderItemAt(pos)`：命中轮廓环即直接进入 move 模式，无需先选中。 |
| 2. 表格工具栏改 Word 式符号图标 | 新增 `src/ui/widgets/TableIconButton.h/cpp`，以 `QPainterPath` 手工绘制对齐 / 选区 / 增删行列矢量图标，替换原中文按钮。 |
| 3. 表格关闭后工具栏残留 | `TableToolBar` 校验表格对象有效性，表格被删除或取消选中即刻隐藏。 |
| 4. 框选删除后残留批注 | `ToolController::deleteSelected()` 同步清空 `m_activeTable`、发 `tableActivated(nullptr)`、取消全部选中，清除幽灵态。 |
| 5. 箭头笔尖对齐落点 | `ShapeItem` 箭头多边形顶点严格落在 `m_end`，直线杆身截至 `m_end - (ux,uy) * head`，不再穿出。 |
| 14. 文本框 / Markdown 实时预览 | `TextBoxItem::notifyContentChanged()` 每次按键即触发视口重绘；`MarkdownItem` 拆为 `m_editor`（源码）+ `m_preview`（GitHub 方言渲染）双文档，输入时实时重排预览。 |

### 簇三：算法可视化专一性（文字 / 指针 / 控件互不重叠）

新增 `tests/OverlapAuditTest.cpp` 做几何级自动审计：对每个模块渲染全部帧后，采集 `ArrayBlock` 单元、其下标行、`PointerBlock`、`GraphNode`（真实圆框 + `subLabel` / `valueText`）与所有 `QGraphicsTextItem` 的场景包围盒，两两求交，输出冲突清单。

- **`PointerBlock` 语义重构**：新增 `setShaftLength()` / `setLabelOffset()` / `setLabelAbove()`；箭头尖端统一落在原点，标签位置与杆长解耦，横向指针可把标签甩到杆尾或上方；`boundingRect()` 改为「杆身 ∪ 标签」的并集，与实际绘制严格一致。
- **树状数组**：`t[]` 与 `a[]` 下标行改为独立文本并置于方块上方；指针 `i` 改为自下而上指向目标块底边，标签落在层间空隙；`L0/L1/...` 层级标记与说明文字分列左右两侧。
- **链式前向星**：四组数组由「四列并排」重构为「四行横排」（`head/to/nxt/weight` 各占一行），指针 `k` 与 `head[u]` 均自下方指向上方对应单元格，标签落在行间空隙；标题移入左侧独立列；图节点右移避开数组区。
- **堆调整 / 堆排序**：数组标题与树结构标题上移，避开完全二叉树根节点与下标行。
- **线段树**：指针 `node` 锚点偏移 `0,46` → `0,80`，脱离节点包围盒。
- **拓扑排序**：结点层布局下移避让标题；输出序列行下移到 `degreeY+270`，为 `H/T` 指针标签让出空隙。
- 审计基线：冒泡排序、前缀和、01 背包、线段树、并查集等均零冲突，最终 10 个受测模块全部 `conflicts=0`。

### 簇四：动效与数据

| 需求 | 落地做法 |
|------|----------|
| 9. 下拉气泡质感 | `MyComboBox` 的 `ComboOption` 重做：圆角 11px、双段线性渐变、hover 时渐变插值 + 顶部柔光扫掠、按下时半径收缩与文字微沉；新增 `hoverProgress` / `pressProgress` 两个 `Q_PROPERTY` 驱动 `QPropertyAnimation`；分组瓦片继承该分类的主色；弹层开合加入透明度过渡与多层柔化投影。 |
| 10. 堆节点值同步飞出 | `IModule` 新增 `playNodeSwapAnim` / `playDualSwapAnim`，`HeapAdjustModule` 与 `HeapSortModule` 在数组值交换时同步播放树节点数值弧线飞出。 |
| 11. 匈牙利默认数据 | 换成经典「分手重配」样例：`3 3 / 1 1 / 1 2 / 2 1 / 3 1 / 3 2 / 3 3`，并补充换配关系说明文本。 |

### 簇五：规划登记（问题 13，暂不实现）

在 `docs/outlines.md` 新增「十六、几何与坐标系模拟器」章节，登记 GeoGebra 风格笛卡尔坐标系模拟器的完整需求与设计约束：

- 动态比例尺（默认分划 `1`，逐级 `0.5 / 0.25 / 0.125`，缩小反向 `2 / 4 / 8`）；
- 点的插入、顺序命名、重命名、坐标就地编辑；
- 两点定线并显示直线方程；
- 表达式求值器与函数曲线绘制（`y = x^2`、`sin(x)` 等）；
- 约束：不引入外部依赖、不套用 `IModule` 帧体系、坐标变换单一入口、与算法模块联动仅做接口预留；
- 分四期推进：底板 → 点与线 → 函数 → 联动。

## 第 8 批八模块（2026-11-15，按课堂模板对齐）

PM 提供了课堂统一代码模板，本批 8 个模块**严格按其风格实现**，不自行发明算法：

| 模块 | 模板要点 | 教学呈现 |
|------|----------|----------|
| 希尔排序 | `gap = n/2` 折半，同余下标分组 | 弧线连成一组（6 色轮转）+ 组内插入排序 |
| 快速幂 | `qpow` 外层位分解 | 指数二进制位条（最右为第 0 位）+ 底数/指数/结果三面板，每位合并为一帧 |
| 埃氏筛 | 只输入 `n`，从 `i*i` 起按步长 `i` 划除 | 右侧 10 列矩阵滚动定位，被划格**保留数字**并置灰打叉；同一素数的倍数一帧流动划完 |
| 链式前向星 | `head/nxt/to/weight` 头插 `add(u,v,w)` | 四列数组（列距 150）+ 右侧图，指针 k 从 to 列左侧指向，染色有高亮脉冲 |
| 并查集路径压缩 | `find` + `merge`（`sz` 按大小合并） | 路径亮橙后**整树压扁成星**，`pa[]`/`sz[]` 实时同步 |
| 子树大小 | `sz[u] = 1 + Σ sz[v]` 后序遍历 | sz 气泡沿弧线汇入父节点相加 + 底部后序序列条 |
| 树的直径 | `dep[]` 维护最深点 `c` | 两轮**无向** DFS + 波纹扩散，A→B 逐段点亮成粗红线 |
| 堆优化 Dijkstra | **隐去 `vis`**，`if (d > dis[u]) continue` 懒删除 | 只画 `dis[]`，过期条目标暗红丢弃；优先队列为**数组化 `pq (堆)`**（元素是 `pair(dis, u)`，入堆时后续元素右移一格） |

配套扩展：`LinkLine`（贝塞尔飞线）、`Ripple`（扩散波纹）、`playRipple()` / `playLinkRedirect()` / `playBubbleFlight()` 三个共享动画；`AnimFrame.h` 新增 40 个动作类型。

## 第 8 批七项可视化改进（2026-11-15，PM 反馈轮）

PM 对八模块逐一点评后提出七项改进，逐一落地（BUG-134 ~ BUG-140）：

| # | 反馈 | 落地做法 |
|---|------|----------|
| 1 | 质数筛被筛去的数字不该消失 | `ArrayBlock` 新增 `m_struck` 划除标记：保留数字、底色转灰、叠加删除线、缩放至 0.82 |
| 2 | 帧滑块应随播放自动同步 | `MainWindow::advanceFrame()` 调 `syncFrameSlider()`，滑块与 "N / M" 徽标实时跟随 |
| 3 | 质数筛倍数应一帧流动删除 | 新增 `SV_CROSS_BATCH`：同一素数的全部倍数合并为一帧，`QVariantAnimation` 按 28ms/格 顺序划除并逐格涟漪 |
| 4 | 快速幂太劣质 | 删除 qmul 面板与 `FP_QMUL_*`/`FP_SQUARE`/`FP_SHIFT`；2 输入不取模、3 输入取模；二进制位条右端为第 0 位，每位合并为一帧 |
| 5 | 树的直径节点宽度要拉大 | `layoutTreeScene()` 叶子间距 104.0 → 178.0，`dep=...` 标签不再被遮挡 |
| 6 | Dijkstra 左侧应显示数组化优先队列 | 删除二叉堆树，改为 `pq (堆)` 数组；单元格 98 宽 + 11 号字容纳 `pair(dis, u)`；入堆时 k 之后元素整体右移一格 |
| 7 | 前向星指针 k 与格子重叠、染色太突兀 | `PointerBlock` 新增 `Direction` 枚举；四列间距放宽到 150；指针 k 用 `Right` 方向从 to 列左侧指向；染色加 `playScalePulse` 脉冲 |

顺带修掉两个由重构暴露的真实缺陷：

- **快速幂精确模式 64 位溢出**：`a` 在 `2^20` 时达到 4294967296，`int` 截断后显示为 0。`AnimFrame` 新增 `big1 ~ big4` 承载 64 位数值，`FastPowModule` 改用它们传参（BUG-137）。
- **`QString::arg` 的 `%%` 不转义**：原先写作 `"b %% 2 = %1"`，Qt 不解释 `%%`，界面直接显示 `b %% 2 = 0`。改为 `"b % 2 = %1"`。
- **精确值超出 64 位**：新增 `FastPowModule::fitsExactPower()` 前置校验，超范围时提示改用取模分支，避免 `long long` 乘法溢出。

同时新增 `ArrayBlock::setValueFontSize()` 以便不同模块用不同字号渲染单元格值，并新增 `tests/LayoutFitTest.cpp` 做几何级校验（pq 文本不溢出、树标签不重叠、指针净空）与 `tests/FrameSliderTest.cpp` 的自动播放同步断言。全量 **59 个测试套件**串行全绿。

## 画布批注层（双层架构，2026-11-14 三轮落地）

### 架构

- **双层画布**：下层 `GridGuideScene` 承载算法图元，上层 `AnnotationScene` 独立承载批注图元。上层不进入下层的 `itemsBoundingRect()`，因此 `ViewportFitter` 的居中与缩放永远只由算法内容决定，批注不会把画布顶偏。
- **叠加渲染**：`CanvasView::drawForeground()` 用 `QGraphicsScene::render()` 把上层场景按视口变换贴到算法层之上。绘制器在该回调中已带视图变换，因此**不能再叠加 `setTransform(transform())`**（会二次变换），也**不能用视口像素坐标裁剪**（`drawForeground` 的 painter 处于场景坐标系，负坐标内容会被整体裁掉，见 BUG-109）。
- **所有权**：`AnnotationScene` 用 `m_owned` 列表统一持有全部批注图元，`ToolController` 只负责挂载 / 卸载，不持有也不销毁任何图元。这样清空重做栈时不会误删已经通过撤销复活的图元（BUG-110）。

### 工具（Batch 1）

- 14 种工具：指针 / 选中 / 画笔 / 荧光笔 / 橡皮 / 直线 / 方框 / 圆形 / 箭头 / 三角 / 马赛克 / 文本框 / Markdown / LaTeX / 表格。
- `ToolController` 作为视口事件过滤器拦截原生 `QMouseEvent`，再经 `view->mapToScene()` 换算场景坐标。**不要**向 `viewport()` 投递合成的 `QGraphicsSceneMouseEvent`：`QApplication::notify` 会把它当作 `QEventPoint` 处理并访问 `QEventPoint::position()`，直接崩溃。
- Shift 约束：直线 / 箭头吸附 45°；方框 / 圆形锁定正方形 / 正圆；三角形按等边三角形（高 = 边长 × √3/2）定位。
- 橡皮只命中上层批注图元并跳过锁定项，对下层算法结构零影响。

### 选中与历史（Batch 2）

- 框选矩形与选区命中测试都在上层完成，移动只改批注图元 `pos()`，缩放以选区中心为轴，**绝不改动下层结构**。
- 撤销 / 重做栈区分 `Add` / `Remove` 两类命令；`Ctrl+Z` / `Ctrl+Y` / `Ctrl+Shift+Z` 三向快捷键同时挂在视口与主窗口上。
- 样式条：颜色取色盘、粗细滑块、不透明度滑块、撤销 / 重做、显隐切换、锁定切换、表格行列滑块、文本字号滑块、加粗 / 斜体 / 下划线。（Batch 2 的样式条为平铺形态，已在 BUG-113 中重构为浮动轻套件的「样式 / 更多」两个互斥浮层。）

### 卡片（Batch 3）

- **Markdown 卡片**：`QTextDocument::setMarkdown(..., MarkdownDialectGitHub)` + GitHub 风格样式表，固定宽度自动换行；双击经 `AnnotationEditor` 左编辑右实时预览。
- **LaTeX 公式卡片**：常驻 Node 助手 `tools/latex/serve.js`（`mathjax-full`）以 JSON 行协议渲染 SVG，`QSvgRenderer` 贴进卡片；双击改源码后实时重渲染。
- **富文本框**：字体族 / 字号 / 加粗 / 斜体 / 下划线 / 文字颜色可调。
- **可编辑表格**：行列数可配，双击单元格就地编辑；与「自由结构 - 行列式」的区别是**单元格默认为空字符串而非 0**。
- **编辑器对话框** `src/ui/widgets/AnnotationEditor`：纯文本 / Markdown / LaTeX 三种模式，全部自绘，不引入原生输入控件。

### 导出与快捷键

- `MainWindow::exportCanvasToPdf()` 自动取两层 `itemsBoundingRect()` 的并集作为画布范围，加 40px 边距后按页面尺寸等比缩放，`QPdfWriter` 输出单页 PDF。
- 工具快捷键：`V` 指针 / `A` 选中 / `B` 画笔 / `H` 荧光笔 / `E` 橡皮 / `L` 直线 / `R` 方框 / `O` 圆形 / `D` 箭头 / `T` 三角 / `M` 马赛克 / `N` 文本 / `G` 笔记 / `X` 公式 / `I` 表格。
- 颜色快捷键：`1`~`7` 红 / 绿 / 蓝 / 黄 / 橙 / 粉 / 深灰；`Alt + R/G/B/Y/O/P/K` 备用。

## 画板十项需求轮（2026-11-15，BUG-123 ~ 126）

### 笔刷设置跟随每个工具且具备记忆（BUG-124 关联）

- `ToolController::ToolStyle` 结构体承载 color / width / opacity / eraserRadius / mosaicCell / tableRows / tableCols / textSize，`QHash<int, ToolStyle> m_styles` 按工具枚举索引**独立保存**。
- 画笔与荧光笔的颜色、宽度、透明度完全互不干扰；切换工具后再切回来，属性原样保留；算法模块切换时 `ToolController` 生命周期长于模块，记忆自然延续。
- `CanvasToolBar::placeToolPanelUnderButton()` 把属性浮层**精确对齐到当前工具按钮正下方**（浮层中心对齐按钮中心，边界自动避让画布四边），切换工具时实时重定位。

### 笔迹平滑与 0 延迟跟随（BUG-124）

- **向心 Catmull-Rom**（`alpha = 0.5`）：节点间距按 `‖P(i+1)-P(i)‖^0.5` 加权，急转弯不再产生 cusp（尖点）与自交，均匀 Catmull-Rom 的过冲问题消失。
- 采样间距 3.0 → 2.6、最小采样距离 0.4 → 0.35，曲率大的地方自动加密。
- `CanvasView::mapToSceneF()` 用 `viewportTransform().inverted()` 保留亚像素精度；`ToolController` 全部坐标换算改走该接口，消除 `toPoint()` 整像素截断造成的台阶顿挫。
- `StrokeItem` 增量重建：`m_resampled` 缓存采样点，`rebuildTail()` 只重建末段，落笔后每加一点只重算一小段。

### 无极取色（需求 3）

- `MyColorPicker` 自绘连续色相条（拖动直接取色）+ 饱和 / 明度滑块 + 9 枚渐变预设芯片（`ColorChip` 自绘圆角色块）+ Hex 输入 + R / G / B 数字输入，全部为自研控件，未引入原生输入框。

### 纯快捷键化（需求 4 / 5）

- 工具栏移除复制 / 粘贴 / 剪切 / 删除 / 撤销 / 重做 / 全选专用按钮，「更多」浮层只保留显隐 / 锁定 / 导出 PDF / 清空批注并附快捷键说明。
- `Backspace` 与 `Delete` 双绑定删除选区内全部批注元素，`Ctrl+Z` / `Ctrl+Y` 撤销重做，`Ctrl+C/X/V/A` 剪贴板与全选。

### 框选拖动与缩放手柄（BUG-125）

- 选框内部空白处按下即可整体拖动（`pointInsideSelection()`），不必精确点中元素本体。
- `SelectionOverlay` 提供 8 个方向手柄（4 角 + 4 边中点），命中容差按视口缩放反向换算 `tol = 9.0 / max(0.2, zoom)`。
- 新增 `AnnotationFrame`（origin + basisX + basisY）仿射抽象：缩放以**起始帧为绝对基准**，杜绝逐帧累乘漂移；等比缩放走 `setScale(uniform)` 保持 `QGraphicsItem::scale()` 语义正确。

### 透明原地编辑（BUG-123）

- 文本框与 Markdown 笔记移除一切不透明底色，只保留虚线框。
- `AnnotationScene` 是**游离场景**，从未挂到 `QGraphicsView`，`QGraphicsTextItem` 拿不到原生焦点，键盘事件根本进不来；改为在 `ToolController::eventFilter` 拦截视口 `KeyPress`，用 `QTextCursor` 直接改文档并手动 `update()`。
- 编辑态自绘光标（`QTextLayout::lineForTextPosition` + `cursorToX`），不依赖原生文本光标。
- `createItemForTool()` 修正顺序：先 `setTool(AnnotationSelect)` 再 `beginInlineEdit(item)`，否则 `setTool` 内部的 `endInlineEdit(true)` 会把刚进入的编辑态关掉。

### 表格上下文工具栏（BUG-126）

- 新增自绘 `TableToolBar` 浮层：居中 / 左中 / 右中 / 顶端对齐、选中单元格 / 行 / 列、上 / 下 / 左 / 右插入行列、删除当前行 / 列、「插入 k 行 / k 列」用 `MyTextInput` 输入数量。
- `MainWindow::positionTableToolBar()` 把浮层定位在表格上方并随表格移动 / 改尺寸自动重定位。
- 右键表格弹出上下文菜单（`tableContextRequested` → `showContextMenu()`），同样提供四向插入与删除。
- `handleTableClick()` 改为按点击位置动态解析目标表格，单元格命中用 `headerHeight()` 偏移，标题行不再误判为第 0 行。

## 画板交互修复轮 + 第 4 批联动（2026-11-14，BUG-117 ~ 122）

### 笔迹：快划断裂与自交白洞（BUG-117）

- **断裂根因**：鼠标采样点稀疏且间距不均，直接送入均匀 Catmull-Rom 样条会产生巨大切向量，导致曲线过冲、翻折甚至自交，视觉上断成多段。
- **白洞根因**：笔画原以「左边界正向 + 右边界反向」的闭合 ribbon 多边形填充，而 `QPainterPath` 默认 `Qt::OddEvenFill`；笔迹自交处两侧被判定为内部重叠，奇偶校验为偶数，于是被挖成空洞，透出白色背景。
- **处置**：`StrokeItem::resampledPoints()` 先按 2.5px 线性重采样保证控制点均匀；样条改为逐段三次贝塞尔（每段 8 步）；绘制改用 `QPen(RoundCap/RoundJoin)` 直接描中心线，**彻底放弃自交 ribbon 多边形**。

### 形状实时预览（BUG-118）

- **根因**：`AnnotationScene` 是挂在 `CanvasView` 上的**分离 overlay 场景**（真正 `setScene` 的是下层 `GridGuideScene`）。对 overlay item 调用 `update()` 只弄脏 `AnnotationScene` 本身，不会触发 `CanvasView::viewport()->update()`。
- **处置**：`ShapeItem` 的 `setGeometry()` / `setMosaicCells()` / `setKind()` 统一调用 `notifyContentChanged()`，经 `contentChanged()` 信号驱动宿主视口重绘。

### 真像素化马赛克（BUG-119）

- `CanvasView::captureSceneRegion()` 把**底层算法场景**（不含批注层）渲染为快照。
- `ToolController::captureMosaic()` 将快照降采样为 `cols × rows` 网格，再块状放大，得到真正的像素化马赛克。
- `ShapeItem::setMosaicImage()` 优先渲染位图，缺失时回退网格多边形；位图以 base64 PNG 序列化，撤销 / 重做 / 归档后仍可还原。

### 文本框与笔记（BUG-120）

- `createItemForTool()` 完成后立即 `setTool(AnnotationSelect)`，文本框 / 笔记直接 `beginInlineEdit()` 进入**画布内编辑**。
- `ToolController::eventFilter()` 新增 `QEvent::MouseButtonDblClick` 分支：双击已有元素只切换行内编辑态，**不再新建**。

### LaTeX 实时编辑器（BUG-121）

- 插入与双击均改为**非模态** `AnnotationEditor`（`setLiveMode(true)`）：左侧源码、右侧 `QSvgRenderer` 实时渲染，源码改动即时写回画布卡片。模态 `exec()` 会造成事件循环重入，自动化测试中会死锁。

### 两段式工具交互与图标（BUG-122）

- 15 个工具全部进入主条；点击按钮时若该工具已是当前工具，则切换 `m_toolPanel`（`QStackedWidget`）中对应工具的独立属性页；否则仅切工具并收起浮层。属性页覆盖：画笔 / 橡皮 / 形状 / 马赛克 / 表格 / 文本 / 笔记 / LaTeX。
- 图标改为通用符号：画笔 → 铅笔，橡皮 → 传统斜置橡皮块，文本框 → 方框内 "T"，马赛克 → 带白边棋盘格。

### 批注剪贴板

- `copySelection()` / `cutSelection()` / `pasteAt()` / `pasteAtCursor()`，依赖各 `AnnotationItem::cloneItem()` 虚函数；粘贴按当前鼠标的**场景坐标**落位并注册 undo 命令。

### 批注按模块归档（第 4 批联动）

- `AnnotationScene::stashTo(bucket)` / `restoreFrom(bucket)` 以 `QHash<QString, QJsonObject>` 按模块名归档批注 JSON。
- `MainWindow::archiveAnnotations()` / `restoreAnnotations()` 嵌入 `onModuleChanged()`：切模块前归档当前模块批注，切回后恢复目标模块批注，笔记 / 公式 / 手绘不再随切模块丢失。

## 画笔手感升级（2026-11-14，ZoomIt 风格，BUG-114 / 115 / 116）

### 笔迹锯齿（BUG-114）

- **根因**：`StrokeItem::buildPath()` 用 `QPainterPath::lineTo()` 直接连**原始鼠标采样点**，得到的是折线而非曲线；鼠标移动越快采样越稀，阶梯状棱角越明显。
- **处置**：`smoothPoints()` 改为 **Catmull-Rom 转三次贝塞尔**插值——每段取 `p0..p3`，控制点 `c1 = p1 + (p2-p0)/6`、`c2 = p2 - (p3-p1)/6`，每段细分 12 步采样，彻底消除折角。
- **渲染**：`paint()` 不再描边，而是沿平滑路径逐点求切线法向、左右各偏移**半个笔宽**生成左右轮廓，合拢为闭合**丝带多边形**后填充。笔画边缘随曲线走向自然过渡，宽度恒定且无锯齿。
- **降噪**：最小采样间距 `kMinSampleDistance = 0.9`，剔除鼠标抖动点。

### 不实时刷新（BUG-115，架构性缺陷）

- **根因**：批注层是独立的 `AnnotationScene`，**从未挂接到任何 `QGraphicsView`**。因此 `StrokeItem::update()` 与 `AnnotationScene::removeItem()` 触发的只是「无人观看的场景」自身的重绘调度，`CanvasView::drawForeground()` 根本不会被调用。缩放 / 平移之所以能「刷新」，是因为那会触发 `CanvasView` 整体重绘。
- **处置**：
  - `AnnotationScene` 新增 `setHostView()` / `hostView()`；`CanvasView::setAnnotationScene()` 自动完成挂接。
  - `AnnotationScene::addItem()` / `removeItem()` / `destroyItem()` 变更后立即 `m_hostView->viewport()->update()`。
  - `AnnotationItem` 新增 `contentChanged()` 信号与 `notifyContentChanged()`；`AnnotationScene::addItem()` 为每个图元连接该信号，**笔画绘制过程中每加一个采样点即通知画布重绘**，不再等到松手才出现。
  - `ToolController` 的 `updateStroke` / `finishStroke` / `updateShape` / `finishShape` / `updateEraseDrag` / `finishEraseDrag` 均显式调用 `m_view->refreshOverlay()`。
- **实测**：按下鼠标尚未松开即已可见 629 像素；拖动时笔画实时增长 629 → 1819；擦除即刻生效 3149 → 128；撤销后实时复原。

### 橡皮（BUG-116）

- **根因**：橡皮在 `onMousePress()` 里一次性调用 `eraseAt(pos)`，`onMouseMove()` 中橡皮分支缺失，**按住拖动完全无效**；且命中判定用 `shape().contains()`，而平滑后的曲线偏离原始采样点，命中容差为零。
- **处置**：新增 `beginEraseDrag()` / `updateEraseDrag()` / `finishEraseDrag()`，按住拖动连续擦除，路径上按 `eraserRadius * 0.4` 步长插值采样逐段擦除。
- **命中统一**：`AnnotationItem::hitTest(scenePos, radius)` 虚函数；`StrokeItem` 覆写为「样条轮廓命中 **或** 距任一原始采样点 ≤ 半径 + 半笔宽」，兼顾平滑偏差与细笔画。
- **可视化**：`CanvasView::drawForeground()` 在橡皮激活时绘制**红色描边 + 半透明填充圆环**，实时跟随鼠标，直观显示擦除范围。

### ZoomIt 风格增强

- **荧光笔**：新增 `StrokeItem::Mode { Ink, Highlighter }`；alpha 0.38、宽度为普通笔 2.4 倍、`FlatCap + RoundJoin` 马克笔笔头，**叠加处不加深**；快捷键 `H`。
- **滚轮调笔宽**：画笔 / 荧光笔激活时滚轮增减笔宽（0.5px 步进，1~40px）；橡皮激活时滚轮增减橡皮半径（2px 步进）——ZoomIt 最具辨识度的操作手感。
- **数字键换色**：`1`~`7` 切换 红 / 绿 / 蓝 / 黄 / 橙 / 粉 / 深灰；`Alt + R/G/B/Y/O/P/K` 为备用组合（`B`/`R`/`G`/`O` 已占用为工具快捷键，故不用裸字母）。
- **橡皮尺寸**：样式浮层新增「橡皮」滑块（4~60px）。

## 浮动轻套件（2026-11-14 重构，BUG-113）

### 布局回归的根因与处置

- **根因**：`CanvasToolBar` 此前被 `rightLayout->addWidget(...)` 塞进主窗口主布局，内部用 `FlowLayout` 按自然宽度一行铺开 14 个文字按钮 + 5 个滑块 + 色块 + 8 个动作按钮，把窗口自然宽度撑到 1920px 屏外。
- **处置**：工具条改为**浮动覆盖层**，不再参与任何布局计算，因而对窗口尺寸零贡献。

### 结构

- `CanvasToolBar` 以 `CanvasView` 为父级的透明覆盖层（`WA_TranslucentBackground`），顶边居中悬浮，不吃鼠标事件（画布其余区域照常绘制与交互）。
- **主条**（451 × 50px）：10 个 34px 矢量图标按钮（指针 / 选中 / 画笔 / 橡皮 / 直线 / 方框 / 圆形 / 箭头 / 三角 / 马赛克）+ 分隔线 + 样式入口 + 更多入口。
- **样式浮层**：颜色取色盘、粗细滑块、不透明度滑块、加粗 / 斜体 / 下划线、字号滑块。
- **更多浮层**：文本框 / Markdown / LaTeX / 表格四个插入图标、表格行列滑块、撤销 / 重做 / 删除、批注显隐 / 锁定、导出 PDF / 清空批注。
- 两个浮层**互斥展开**，按宿主尺寸自动避让左右与上下边界；宿主 `Resize` 时经 `eventFilter` 重新定位。
- 矢量图标由新增 `src/ui/widgets/ToolIconButton.cpp` 用 `QPainterPath` 手工绘制，**不依赖 Segoe UI Symbol 字形**，从根本上杜绝缺字方框。

### 收益

- 窗口自然宽度 **694px**、`minimumSizeHint` **558px**，1080p（1920 × 1080）下宽裕。
- `CanvasToolBar` 不再拥有 `MyButton` 导出 / 清空实例，改由 `exportRequested` / `clearRequested` 信号转交 `MainWindow`，避免同一动作出现两套控件。

### 下拉分类标题修复（BUG-112）

- **根因**：`ComboSectionHeader::paintEvent()` 用 `painter.setBrush(渐变)` + `painter.setPen(Qt::NoPen)` 后调用 `drawText()`。`QPainter::drawText()` **只使用画笔绘制文字、忽略画刷**，于是分类标题被以 `NoPen` 绘制 = 完全隐形。逐像素采样截图证实：标题行只有 `(59,125,221)` 强调色竖条，深色文字像素为 **0**。
- **处置**：标题改用 `painter.setPen(深色)` 绘制；竖条与尾部装饰线继续用画刷。强调色过浅时回退 `UiTheme::textPrimary()` 保证对比度。
- 该缺陷此前的两次修复（`lighter(128)` → `darker(140/110)`）都只是调色，未触及「`drawText` 忽略画刷」这一根因，故未能奏效。

## 本轮三项缺陷修复（2026-11-14）

1. **下拉分类标题不可见**：真实根因见上方「下拉分类标题修复（BUG-112）」——`drawText` 只用画笔绘制，代码却把它设成了 `NoPen`。早先的调色修复方向错误，现已改为画笔绘制并加对比度兜底。
2. **Release 黑框与内部告警**：`MyCollapsible` 补 `Q_PROPERTY(qreal ratio ...)`（此前 `QPropertyAnimation` 指向未声明属性）；清理 KMP / Heap 中无占位符却调用 `.arg()` 的文案，并把 `HungarianModule` 的重复占位符改成显式编号；CMake 为 `SmartGround` 设置 `WIN32_EXECUTABLE TRUE`，Release 产物不再附带控制台（BUG-107）。
3. **Dijkstra 结点改值叠字**：`GraphNode::paint()` 里 `customValueText` 与 `value > 0` 兜底分支同时生效，导致新值与旧值叠在一起。兜底分支改为仅在 `valueText` 与 `customValueText` 皆空时生效，`commitEdit()` 统一走 `setValueText()`（BUG-108）。

## 本轮调整（2026-11-14，分层格阵 / 科技感下拉 / 命名精简 / 代码瘦身）

1. **下拉浮层边缘暗线修复**：原先浮层阴影绘制在 `ComboPopupFrame` 上但被浮层矩形裁剪，多层圆角阴影的右下溢出被硬切成直角暗带。现改用「外环 - 内环」奇偶填充差集路径逐层绘制柔和投影（12 环、向下偏移 4px），浮层外边距放宽至 18/18/18/22、宽度补偿 +36，卡片边框改半透明。
2. **树状数组二叉分层格阵**：底层是完整数组 `a[1..n]`，其上按 `ctz(i)`（`log2(lowbit(i))`）分 `floor(log2(n))+1` 层；第 `k` 层的 `t[i]` 块宽度 = `lowbit(i) * cellW`、水平覆盖 `[i-lowbit(i)+1, i]`，每层标注 `L0..Lk`、`lowbit` 与覆盖区间，层间以半透明梯形引导线连接。
3. **树状数组合并动画**：新增 `ActionType::FT_MERGE`，构建序列改为自底向上的二叉合并 `t[i] = t[i-2^(k-1)] + a[i-2^(k-1)+1] + ... + a[i]`；渲染时兄弟块与底层 `a[]` 格的数字**同时飞入** `t[i]` 合成新值（例：`c2 = c1 + a2`）。为此在 `IModule` 新增 `playMultiValueFlight()`，支持任意数量来源的弧线飞入。
4. **下拉科技感升级**：瓦片改为冷调渐变 + 细边框 + hover 发光边框 + 左侧强调条 + 选中态顶部高光条；分节标题改为彩色竖条 + 渐变标题文字 + 尾部渐隐短横线（原横贯分割线已移除）；全部字体统一 `UiTheme::uiFont()`（Microsoft YaHei UI）。
5. **分组细化与跨分支**：分组由 4 类细化为 11 类（自由结构 / 数学基础 / 线性结构 / 经典排序 / 堆结构 / 并查集 / 树状数组 / 线段树 / 图论算法 / 匹配与动态规划 / 字符串算法），Kruskal 可同时出现在「并查集」与「图论 · 最小生成树」。
6. **命名精简**：16 个模块显示名改为短标签（匈牙利算法、线段树 (lz标记)、KMP (pi数组)、Dijkstra 最短路、01 背包、堆调整 up/down 等）。
7. **代码瘦身**：新增 `TwoRowArrayModule` 公共基类收敛前缀和 / 差分的重复样板（约 -45%）；删除死文件 `ArrayModule.h/.cpp` 与 `IModuleHelpers.h`；清理 `IModule` 8 个无调用方辅助函数、`GraphRenderHelper.h` 2 个未用函数、`HeapTreeBase.h` 从未写入的 `m_relationLabels`；同步更新两份 `CMakeLists.txt`。

## 上一轮八项调整（2026-11-14）

1. **线段树-建树** 精简为第一节课形态：只留 `tr[]` 与 `lz[]` 两行，删除 `mul[]` 与全部 `% p` 取模；`push_up` = `tr[ls] + tr[rs]`，叶子 = `a[l]`；输入只需 `n` 与 `a[1..n]`。
2. **树状数组** 改用嵌套三角几何：`t[i]` 为顶点、其覆盖区间对应的 `a` 格为底边，绘制半透明填充三角并向区间内每格拉辐射线，形成「大三角套小三角」的经典结构图。（该实现已被本轮的二叉分层格阵取代，见上文第 2、3 条）
3. **五个自由结构沙盒** 清空画布文字（`FreeStructureBase::showHint()` 空实现、不再创建 `m_mathText`），只保留纯数据结构图元。
4. **沙盒-图结构** 每行支持 `u v` 或 `u v w`；写 3 个数该边带权，写 2 个数不带权，无任何默认权值注入。
5. **图结点与边权** 全面支持双击就地编辑：`IModule::enableLiveEditing()` 为未登记的结点（行 `"v"`）与边（行 `"w"`）自动补槽位；无权边双击后自动补上权值；结点新值直接显示在圆内。
6. **右下角进度条** 旋钮不再被裁切：`MySlider` 引入左右对称内边距 14px，旋钮圆心始终落在 `[14, width-14]` 内，辉光按可用半径逐环裁剪。
7. **算法模块下拉** 改为分组按钮网格：`MyComboBox::addGroup()` + `ComboTile` / `ComboSectionHeader`，四个分类、每类 3 列瓦片、类内按课程进度排序。
8. **堆结构动画** 重构：结点值写在圆内（`customValueText`）、1-based 序号写在圆外上方（`subLabel`）；交换帧真正互换数组块与结点值；下方完整二叉树数组行实时高亮 `i/2`、`2i`、`2i+1` 父子关系并给出关系说明文字。
- docs/outlines.md 记录算法教学需求；docs/ModuleStatus.md 记录实现状态；docs/BugLog.md 记录缺陷和修复。

## 实时可编辑数据（老师中途改值真实生效）

- 画布上的数组格、图结点、边权标签全部支持双击就地编辑，不再是静态显示产物。
- `src/render/EditableModel.h` 按 `scope|row#index` 存放老师改过的数值，`scope` 取模块名，不同模块互不干扰；重新点击「绘制可视化效果」会按 scope 整体清空，回到原始输入。
- `IModule` 提供槽位登记接口（`registerArraySlot` / `registerNodeSlot` / `registerEdgeSlot`）把画布图元映射到具体数据行与下标，算法侧通过 `liveValue(row, index, computed)` 优先取覆盖值。
- `MainWindow::rebuildCurrentLayout()` 建图后自动 `enableLiveEditing()`；编辑发生后 `applyLiveEditsAndReplay()` 以非动画模式重建场景、重跑 `runAlgorithm()` 并快进到原帧位置，于是后续动画帧全部按改后的数值计算，下游状态一并重新推导。
- 已接入：前缀和（`a` / `s`）、差分（`a` / `d`）、五种排序（`a`）、单调栈（`a`）、LIS（`a`）、三个线段树模块（`a` / `tr` / `add` / `mul`）、树状数组 BIT（`a` / `t`）、堆 up/down 与堆排序（`heap`）、朴素 Dijkstra（`w` / `dis` / `vis` / `pa`）、Kruskal（`w`）、BFS 与拓扑排序（结点值 `v`）。

## 渲染残影根治

- 残影根因是图元绘制范围超出 `boundingRect()`：超出部分被绘制却从不参与 Qt 脏区失效，指针移动后旧像素永久残留。`PointerBlock` 现统一用 `labelWidth()`（文本宽 + 8，下限 20px）作为包围盒与绘制宽度公式，`GraphNode` / `FloatingText` 的包围盒改用 `QFontMetrics` 动态计算。
- `ArrayBlock::boundingRect()` 现为 `QRectF(-1, -27, W+2, H+28)`：正好框住格身、格头下标与描边向外溢出的一半（2px 描边中心落在边界上，需各留 1px）。格宽 `W` 严格等于该行的单元格节距，相邻格不再互相侵入（01 背包 DP 表按 60px 列距排布，旧版 80px 宽包围盒会导致上下左右全部重叠）。描边绘制在格身边界上（`drawRect(0, 0, W, H)`），相邻格描边中心重合为同一条线，详见下方「数组单元格单线边框」。

## 数组单元格单线边框（Excel 式表格）

- 根因：旧版 `ArrayBlock::paint()` 用 `drawRect(1, 1, W-2, H-2)` 把描边整体内缩 1px。相邻格的右描边中心落在 `X+W-1`，左描边中心落在 `X+W+1`，两者相距 2px，中间还夹着一段白缝，交界处形成 4px 暗带 + 白缝，肉眼看到的就是「两条竖线 + 一道缝」。像素实测证实：外边界 2px，内部交界 4px。
- 修复：改为 `drawRect(0, 0, W, H)`，让相邻格的描边中心精确重合于同一坐标，交界处只剩 1 条 2px 竖线，形成连续表格。`boundingRect()` 同步扩为 `(-1, -27, W+2, H+28)` 以容纳描边外扩的 1px。
- `ArrayBlock` 新增 `setCellSize(w, h)` / `cellWidth()` / `cellHeight()` / `bodyRect()` 接口，让格宽可按各模块的实际列距配置。已接入 `SandboxArrayModule`(62px)、`SegmentTreeBuildModule`(58px 数组 / 54px 槽位)、`SegmentTreeEditBase`(58px / 54px)、`KMPNextModule`(56px)、`KMPMatchModule`(52px)，使这些模块的描边边界也完全重合。
- 像素实测（修复后）：单元格所在行的暗色游程宽度序列为 `2 1 1 2 1 1`，即边框恒为 2px、数字笔画 1px，全程无双线暗带。

## 就地编辑稳定性

- 根因：`EditorWatcher::eventFilter` 处理 `FocusOut` 时调用 `onDone(true)` → `commitEdit()` → 同步 `delete editor`，销毁了事件过滤器正在处理的 `receiver`。Qt 在 `sendThroughObjectEventFilters` 返回后仍会访问 `receiver->d_func()->extraData`，造成 use-after-free 闪退。
- 修复要点：
  - 新增公共 `destroyEditor()`，统一走 `setParentItem(nullptr)` → `removeItem()` → `deleteLater()`，绝不在事件过滤器回调内部同步销毁对象。
  - `FocusOut` 分支处理完立即 `return true` 消费事件，不再回落到 `QObject::eventFilter(obj, event)` 触碰可能已释放的对象。
  - `m_editor` 成员由裸指针改为 `QPointer`，并用 `QPointer<Self>` 守卫宿主对象，宿主析构后回调自动失效。
  - watcher 回调增加 `m_editor.data() != editor` 身份校验：`deleteLater()` 延迟销毁窗口内若用户双击另一格，旧 editor 的过期回调会被丢弃，不会误提交或误关闭新编辑器。
- `ArrayBlock::beginEdit()` 的编辑器定位改用 `m_cellW / m_cellH`，不再硬编码 `30.0 / 20.0`，非 60px 宽的模块同样居中。
- 文本内容变化时补 `prepareGeometryChange()`，保证几何变化被正确通知；数组格、图结点、边权的值写入一律走 `setCustomValue` / `setValueText` / `setWeight`，不再直写公有成员绕过脏区失效。
- `IModule::purgeTransientItems()` 登记并清理飞行数字等瞬态图元，动画被中断时不留孤儿文字。

## UI 全面自研（零第三方依赖）

- 项目不再向用户暴露任何原生 Qt 输入控件，全部由 `src/ui/widgets/` 自绘，配合 `src/ui/UiTheme.h` 令牌实现抗锯齿圆角、悬停/按下态与统一字体策略。
- 图类型切换改用 iOS 风格 `MyToggleSwitch`：灰色 OFF = 有向图，iOS 绿 ON = 无向图，滑块位移带缓动动画。
- 「无向边加双向箭头」为 `MyCheckBox`，通过 `ratio` 属性驱动透明度与可用态；选择有向图时该子选项向上流动隐藏，选择无向图时向下展开。
- 画布背景使用 `UiTheme::canvasBg()`，网格参考线为淡色点线，不干扰主体内容。

## 画布居中与隐形对齐网格

- 每次构建图形后统一走 `ViewportFitter::fitToItems()`：先 `fitInView(KeepAspectRatio)`，再钳制缩放区间（0.25 - 2.4），最后 `centerOn` 内容中心，保证图形永远居中且大小合适。
- 节点拖动时启用 `SceneGrid::magneticAlign()`：18px 半径内自动吸附到邻近节点的横纵坐标轴，其余情况按 56px 单元格吸附；按住 `Ctrl` 可临时关闭吸附进行自由摆放。
- 网格参考线走**场景背景层**而非场景图元：`GridGuideScene`（继承 `QGraphicsScene`）覆写 `drawBackground()` 绘制淡色点线，`MainWindow` 直接以它作为画布场景。这样网格既不进入 `itemsBoundingRect()`（不影响居中与缩放适配），也不会被 `clearScene()` 的 `scene->clear()` 清掉。（详见 BUG-105：旧实现因图元未加入场景且 `boundingRect()` 自引用递归，网格其实从未绘制过。）

## 数组下标与表格排版规范

- 所有数组型图元的框格头只显示纯下标（`0,1,2...`），不再出现 `j = 0`、`d[1]`、`vis[1]` 这类冗余前缀；数组语义改由左侧行标题承担。
- 左侧行标题统一采用 OI 短名（中文教学命名）：拓扑排序 `ind` / `q` / `out`，Dijkstra `dis` / `vis` / `pa`，Kruskal `e[]` / `pa`，广搜 `q` / `dis` / `pa`，LIS `a[]` / `dp` / `g`。
- `ArrayBlock::showLabel` 可单独关闭某个框格的标签，用于二维 DP 表只在首行绘制列下标的教科书排版。
- `ArrayBlock::customValue` 让方块直接承载 `∞` / `—` / `0` / `1` 等非数值内容，不再依赖叠加的独立文本项。
- 01 背包 DP 表布局：列下标仅首行绘制，行下标 `0..n` 独立排在左侧，行间距 74px，`i` / `j` 坐标轴独立标注，标题与网格之间留 60px 以上间距，杜绝文字粘连。

## 指针挤占与顶点锚定

- `IModule::playPointerArrange(ptrs, anchors)` 是全局唯一的指针排布入口：按锚点分组，同一格内的多个指针按各自 `labelWidth()` 依次排开（间距 6px），保证任何时刻都不压字。
- 排布完成后统一交给 `IModule::layoutPointerSlots()` 做**跨组消解**：把所有指针按 x 排序，反复把相互侵入的相邻对各自推开一半，直到互不重叠。这样相邻格上的指针（例如二分查找中 `mid` 与 `R` 相邻）也不会互相压字。
- `IModule::playPointersSync()` 复用同一套消解逻辑，避免两处规则漂移。
- 二分查找（闭区间 / 左闭右开）、LIS、KMP、队列、拓扑排序、广搜全部接入该引擎；二分查找用 `slot` 属性记录各指针当前所在格，每帧重排全部指针。
- `GraphNode` 新增 `pointerList` 锚点注册表，`PointerBlock::setAnchorNode()` 把指针绑定到具体顶点；`GraphNode::itemChange()` 在位置变化时同步所有绑定指针，因此 Dijkstra 的 `u` / `v` 会像权值徽标一样跟随老师拖动节点。
- 队列类指针统一使用单字母标签 `H` / `T`，取代冗长的 `head` / `tail`。

## 控件质感与画布复位

- `MyToggleSwitch` 轨道压缩到 50×24px，文字移出轨道：滑块关闭时「有向图」在右侧、滑块打开时「无向图」在左侧，当前态深色、另一态浅灰。
- 数据输入框使用 `UiTheme::borderSoft()`（#C4D8F3）淡蓝边框，聚焦切换 `borderFocus()`，教师一眼可辨录入位置。
- `MyTextEdit` 采用严格四层绘制管线，全部落在 viewport 层：圆角底色 → 原生文字与光标 → 灰色占位提示 → 只描边不填充的边框。彻底避免自绘白底覆盖原生文字导致的「输入框全白、光标进不去、无法键入」。
- 占位提示由 `MyTextEdit` 自持 `m_hint` 并逐行绘制（`UiTheme::textMuted()`），不依赖 Qt 原生占位符；`toPlainText()` 为空时立即出现，清空文本后立刻重现，与「请输入密码」这类登录框提示一致。
- `MainWindow` 全局样式表不再包含 `QTextEdit` 规则，避免样式表优先级互相覆盖输入框外观。
- 从任意模块切回「请选择...」立即清空画布、清空输入框、恢复默认占位提示、隐藏图结构选项面板、复位状态栏与缩放变换。
- 自动播放播完最后一帧即停，状态栏提示「播放完毕。」，不再从头循环回放。

## 协作模式

- 用户担任项目经理角色，只审查最终效果。代理承担完整技术决策，全自动完成「评估 → 编码 → 编译 → 测试 → 文档同步」，中途不打断。
- 多条建议按主题分组合并落地，避免串行逐条交付。
- 优先改源码；仅当确需引入外部库时才出文字方案供遴选。当前 UI 层保持零第三方依赖。

## 图结构选项与权值跟随

- 教师可在左侧"图结构选项"分组中切换有向图/无向图（iOS 风格滑动开关：灰色=有向，绿色=无向），并对无向图勾选"无向边加双向箭头"。
- 面板按模块能力自动启用/禁用：Tarjan 强制有向，Kruskal 强制无向，BFS、拓扑排序、朴素 Dijkstra 两者皆可。
- 面板容器仅在选中支持图论选项的模块时显示，启动时默认隐藏，不会在左上角留下残留色块。
- 权值由输入数据自动判定：只要有一行边含 3 个元素，整体按带权图处理，否则按无权图处理。
- 边权徽标直接绘制在 `GraphEdge::paint()` 内，沿曲线中点的法向偏移；教师在画布上拖动节点或拖拽弯曲边时，权值始终贴合在边上。
- 图边本身是可交互图元：二次贝塞尔曲线、`shape()` 命中检测、拖拽弯曲、悬停高亮。
- 有向边箭头为等腰实心三角（15px 长 / 7px 半宽）+ 白色描边，视觉等同 LaTeX `\rightarrow`。

## 本轮接手修复与七模块实现

- TarjanModule 在算法启动时只清空遍历状态，保留 buildLayout 建立的邻接表。
- Tarjan 布局动画遵守 m_animated 锁，撤销重建时即时布局。
- 所有模块头文件移除对子类 m_scene 的重复声明，统一使用 IModule::m_scene。
- MainWindow 记录布局输入和算法录制输入；构建、切换模块时清除旧帧。
- 运行时输入与现有布局不一致时会重建布局。
- 撤销时用录制输入重建场景，并关闭动画后重放到目标帧。
- 自动播放到末尾后停在末帧，不再重建场景从头回放。

- README、模块进度、算法大纲、工作流和架构说明统一为当前代码事实。
- 新建 Codex 与 Cursor 项目级规则文件，使项目约束在后续会话中可持续读取。
- 完成拓扑排序：入度递减、队列入队/出队、输出序列和有向环残留提示。
- 完成朴素 Dijkstra：非负权输入校验、O(N²) 选点、松弛、前驱与不可达展示。
- 完成 Kruskal：排序边扫描、并查集合并、成环舍弃和不连通提示。
- 完成 BFS：层次着色、数组队列、距离与父节点记录。
- 完成 01 背包二维 DP、LIS 双解法对照和 KMP 前缀函数 Next 数组。
- IModule 新增保留源值的弧线飞入动画，供图节点入队和 DP 候选状态复用；关闭动画锁时同步更新。

## 本轮稳定性加固

- 所有动画结束回调改用 `QPointer` 包装图元，`scene->clear()` 后判空再操作，消除撤销/切换模块时的闪退与 double free。
- `GraphEdge` 由 `QGraphicsItem` 升级为 `QGraphicsObject` 以支持 `QPointer`；节点与边析构时互相解绑，杜绝悬垂引用。
- 修复有向图中 u→v 与 v→u 被错误合并成一条边的问题。
- 修复图结构选项单选框点击后被面板回写覆盖、切换无效的问题。
- 统一裁剪各模块输入规模：并查集父节点下标、堆元素个数、栈/队列容量、前缀和与差分查询区间。
- 并查集 find 改为带上限的迭代路径压缩，避免父节点成环时栈溢出。
- 单调栈改用静态数组 `int stk[N]` + `top` 模拟，与项目 OI 竞赛风格统一。
- 清理图模块中已废弃的 `m_weightLabels`、`m_degreeLabels`、`m_weightedEdgeList` 成员。

## 2026-11-08 交互与视觉打磨轮（九项修复）

### 底层渲染修复

- `GraphEdge::paint()` 重写箭头几何：新增 `trimmedStart()` / `trimmedEnd()`，曲线两端按节点半径（22px）裁剪，箭头尖端落在节点圆外侧而非圆心；箭头朝向改用「节点中心 - 裁剪端点」向量，彻底解决有向图箭头被节点白底遮蔽、以及方向取反的问题。
- 无向图双向箭头现在两端各绘制一个箭头尖（`is_bidirectional` 时首端补画），此前只在末端绘制导致"勾选无效"的观感。
- 边权徽标去掉白色圆角矩形底框，改为纯文本绘制；同时修正权值锚点的坐标系（此前误减 `pos()` 导致整体偏移）。
- `GraphEdge::shape()` 与 `boundingRect()` 同步改用裁剪端点，保证命中检测与绘制一致。

### 撕裂与悬垂指针修复

- `DSUBasicModule::buildLayout()` 建边后补上 `addEdge()` 注册，节点拖动时边线跟随，彻底解决并查集图像撕裂。
- `HeapBasicsModule::buildTree()` 同样补齐父子节点的 `addEdge()` 注册。
- 新增 `IModule::releaseSceneItems()` 虚接口：每个模块实现「只清空图元指针、不做状态重置」，`MainWindow::clearScene()` 先调用它再 `graphicsScene->clear()`。
- 修复悬垂指针崩溃根因：原 `onModuleChanged()` 先赋值 `currentModule` 再清场，导致旧模块的 `reset()` 对已被删除的图元解引用；现改为先清场再切换模块。
- `BubbleSortModule::reset()` 原会遍历 `m_blocks` 解引用，改为判空保护。

### UI 与输入体验

- 全局 QSS 统一风格：主色 `#2f6fbd`，圆角 6-8px，`QGroupBox` / `QRadioButton` / `QCheckBox` / `QPushButton` / `QComboBox` / `QTextEdit` 视觉一致，分组框、标签、状态栏统一对象名样式。
- 非图论模块自动隐藏"图结构选项"分组（`setVisible(false)` + 从布局中移除），输入框与按钮自动上推。
- 每个模块实现 `getInputHint()`，返回 OI 题目输入格式说明；`QTextEdit::setPlaceholderText()` 在数据被清空时显示灰色提示。
- 图表选项面板改为 `insertWidget(2, ...)` 动态插入/隐藏，避免固定占位留下空白。

### 节点位置保持

- `IModule` 新增 `captureNodePositions()` / `restoreNodePositions()`，图模块通过 `GraphRenderHelper` 统一存取。
- 切换有向/无向、双向箭头时先捕获当前节点坐标，重建布局后回写，老师手工排布的节点位置不再被重置。

### 自动播放状态机

- 固定 600ms 定时器改为单次触发的自适应链路，间隔按帧文字长度计算：`520 + 文字长度 * 28` 毫秒，钳制在 900 - 2800ms。
- 「自动播放」按钮点击后变为淡蓝色底色的「暂停」按钮（`pauseButton` 对象名 + QSS），暂停中显示「暂停中...」。
- 暂停请求不立即中断：置 `m_pendingPause` 标志，当前帧动画播完后由 `m_settleTimer`（820ms，覆盖最长 800ms 动画）确认，再停止推进。
- 暂停期间手动「上一步/下一步」会更新 `m_frameIndex`，恢复播放从当前帧往后继续，而非从原暂停位置继续。
- 播放完最后一帧后停在末帧并回到「自动播放」态，状态栏提示「播放完毕。」，不再重建场景从头回播。

### 新增测试

- `tests/EdgeRenderTest.cpp`：箭头/双向标志语义 + DSU 位置存取。
- `tests/ArrowPixelTest.cpp`：像素级验证有向箭头只出现在目标端、双向箭头两端都有、无向图两端都无。
- `tests/UiBehaviorTest.cpp`：33 项断言，覆盖图面板显隐、占位提示、按钮状态机、暂停时序、位置保持、自适应延迟，外加 21 轮模块/选项切换崩溃回归。
- `tests/TextEditRegressionTest.cpp`：8 项断言，覆盖输入框空态提示、多行提示、有文本时隐藏提示、清空后提示回归、聚焦键入、宿主渲染非空白、提示实时替换。
- `tests/TextEditUiShot.cpp`：在真实 `MainWindow` 中走完「启动 → 切模块 → 清空 → 键入 → 切模块」全流程，逐帧统计灰色提示像素并输出截图。

## 当前开发优先级

1. ~~**批注层深化**~~：LaTeX 实时 SVG 预览已搬进 `AnnotationEditor`（`setLiveMode(true)`），Markdown 预览已支持 `pre` / `code` / `blockquote` 语法高亮样式 → **本轮已完成**。
2. ~~**批注与算法联动**~~：批注层已随模块切换自动归档 / 恢复（`AnnotationScene::stashTo/restoreFrom` + `MainWindow::archiveAnnotations/restoreAnnotations`）→ **本轮已完成**。
3. **线段树建树（SegmentTreeBuildModule）**：补齐最后一个已有头文件骨架，闭合项目「一课一模块」的骨架清单。
4. **KMP 匹配过程**：与已完成的 KMP Next 预处理构成完整闭环，是大纲中最自然的下一步。
5. **完全背包 DP**：复用 01 背包的二维网格渲染管线，改动量最小、复用度最高。
6. **线段树单点修改与区间查询**：接在建树之后，共享同一套树形布局。

新算法模块先提交动画分镜，包括游标数量、暂存结构、标签、颜色语义和帧序列，再等用户确认后实现。

## 维护约束

所有任务遵循根目录 .cursorrules 与 Cursor_Workflow.md。Codex 使用 AGENTS.md 获取项目级约束，Cursor 使用 .cursor/rules/smartground-project.mdc。修复缺陷或改变模块状态时，同步更新 BugLog.md、ModuleStatus.md 和本摘要。

代码验证以当前 Qt/CMake 配置为准。本轮 Release 与 Debug 构建均通过；`tests/` 无头自动化测试覆盖极端输入、动画中断清理、边权跟随、箭头几何、UI 状态机、单元格单线边框、就地编辑崩溃回归、画布批注层工具与渲染、快划笔画连续性、自交无白洞、形状实时预览、真像素化马赛克、批注剪贴板、两段式工具交互、插入后工具回落与双击行内编辑、批注按模块归档 / 恢复、笔刷分工具记忆、向心样条平滑、无极取色器、Backspace/Delete 删除、框选拖动与 8 向缩放手柄、透明原地行内编辑、LaTeX/笔记移动、表格上下文工具栏增删行列，以及本轮新增的「框选拖拽累计位移」「编辑态全局快捷键隔离」「色相条逐像素连续渐变与回绕」「预设标签改名」「表格增删行列不越界」，全部通过（54 个测试套件全绿）。GUI 交互和算法结果测试尚未执行。

## 工程整洁度（2026-11-14）

- 项目根目录的 38 张调试截图（约 469 KB）与重复的 `main.cpp` 已清除；根目录现只保留 `.cursorrules` / `AGENTS.md` / `CMakeLists.txt` / `Cursor_Workflow.md` / `README.md` / `package.json` / `package-lock.json` / `.gitignore`。
- `.gitignore` 新增 `/*.png`，防止测试截图再次污染根目录；`tools/sg.ps1 -Target clean` 同时清理根目录与 `build/tests` 下的 PNG。
- 测试构建从「每个测试独立编译全部模块源码」改为共享静态库（`SgCore` / `SgWidgets` / `SgUi`），build 目录由 **7539 MB 降至 233 MB**，单个测试的编译目录由约 180 MB 降至约 1 MB。
- `StartupPanelTest` 不再依赖 `UiShot` 事先生成的截图文件，改为自行构造 `MainWindow` 抓取画面做像素分析，消除测试间的隐式顺序依赖。

## 工程整洁度（2026-11-20，大清理）

- 项目总占用由 **1149.9 MB 降至 78.0 MB**（释放约 1072 MB，降幅 93%）。
- 清理内容：
  - **根目录 197 张调试截图**（`*.png`，约 6 MB）全部删除。
  - **`shots/` 目录整体删除**（43 个截图 + 3 个一次性裁剪脚本 `crop.py` / `grid.py` / `zoom.py`，共 6.4 MB）。
  - **`tools/` 一次性补丁与恢复脚本全部清除**：110 个 Python 脚本（`add_helpers.py` / `build_imodule.py` / `recover_modules*.py` / `dedupe_*.py` / `hf_replay*.py` / `patch_replay*.py` 等）、59 个 `_` 前缀中间产物（`_edits.json` 3.4 MB / `_events.json` 6.0 MB / `_recover.jsonl` 0.8 MB / `_im_cur.cpp` / `_hf_backup.cpp` 等）、49 个 `tools/_recovered/` 恢复副本。
  - **`tools/__pycache__/`**（`.pyc` 字节码）删除；**`.qtcreator/`** 本地残留删除。
  - **`build/tests/` 全量删除**（97 个测试可执行文件 701.8 MB + `CMakeFiles` 191.9 MB + 207 张测试截图 4.7 MB），这是纯编译产物，可由 `cmake -S tests -B build/tests` 完整重建。
  - **`build/Desktop_Qt_6_11_1_MinGW_64_bit-Release/`**（Qt Creator 影子构建目录，45.9 MB，含 `.qtc_clangd` 索引 8.7 MB）删除。
  - `build/Testing/`、`build/.qt/`、`build/*.pdf` 临时产物删除。
- **保留项**：`tools/latex/`（`serve.js` / `render.js` / `formulas.tex`）与 `node_modules/`（40.4 MB）是 LaTeX 公式渲染服务的**运行时依赖**，`LatexService.cpp` 会按 `appDir/tools/latex/serve.js` 路径查找，删除将导致画布公式预览不可用，故予以保留；`tools/clean.sh` 与 `tools/sg.ps1` 为长期维护脚本，保留。
- **构建体系瘦身**：主程序 `build/CMakeFiles` 中 `compiler_depend.make`(34.8 MB) / `compiler_depend.internal`(33.4 MB) / `ParseCache.txt`(2.9 MB) 三份 MinGW 依赖扫描缓存删除后，由 `cmake -S . -B build` 重新 configure 自动再生，`build` 目录由 103.4 MB 降至 35.2 MB。
- **`.gitignore` 扩充**：新增 `/shots/`、`/tools/_recovered/`、`/tools/__pycache__/`、`/tools/_*.{py,cpp,h,json,jsonl,txt}`、`/tools/*.py`、`/.qtc_clangd/`、`*.pdf` 共 12 条规则，防止同类垃圾再次入库。
- **验证**：清理后 `cmake -S . -B build` + `cmake --build build` 重新编译 `SmartGround.exe` 成功（100%，0 error）；`cmake -S tests -B build/tests` 配置成功并单独构建 `SmartGroundStrictAudit` 套件，运行结果 `TOTAL OVERLAP CONFLICTS = 0` / `TOTAL TIP FAILURES = 0` / `TOTAL AXIS FAILURES = 0`，退出码 0，确认源码与测试资产完好无损。
- `build/` 已被 `.gitignore` 的 `/build*/` 覆盖，故本次清理不影响版本库内容（`git status` 仅显示 194 项删除与 `.gitignore` 一处修改，均为预期的垃圾文件移除）。
