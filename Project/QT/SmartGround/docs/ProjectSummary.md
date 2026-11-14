# SmartGround 项目接手摘要

**更新时间：2026-11-14**

## 项目概况

SmartGround 是面向 CSP-J/S 算法教学的 Qt6 Widgets 桌面程序，使用 C++17 和 CMake。用户选择模块、输入数据后，模块先生成 AnimFrame 序列，界面再逐帧解释和绘制这些动作。

## 当前代码状态

- 菜单中注册了 37 个唯一可运行模块（32 个算法模块 + 5 个自由结构沙盒模块），因跨分支别名下拉条目共 38 个。
- 已无待实现的头文件骨架。
- 本轮改动已通过 Qt 6.11.1 / MinGW 13.1 Debug 构建，42 个测试套件在 `offscreen` 平台下串行全绿。
- 项目已纳入 `D:\Program\Cpp` 的 Git 仓库（SmartGround 位于 `Project/QT/SmartGround` 子路径），可查看提交历史与工作树状态。
- 根目录 main.cpp 是辅助入口；CMake 实际编译 src/main.cpp。
- Release 产物以 Windows GUI 子系统链接（`WIN32_EXECUTABLE TRUE`），`windeployqt` 打包后不再弹出黑色控制台窗口。

### 已注册模块

- 排序：BubbleSortModule、SelectionSortModule、InsertionSortModule、QuickSortModule、MergeSortModule
- 数据结构：StackModule、QueueModule、MonotonicStackModule、PrefixSumModule、DifferenceModule、HeapBasicsModule、HeapAdjustModule（up/down）、HeapSortModule、FenwickTreeModule（树状数组 BIT）
- 搜索：BinarySearchClosedModule、BinarySearchHalfOpenModule
- 图论：DSUBasicModule、TarjanModule、TarjanCutVertexBridgeModule、TopologicalSortModule、DijkstraNaiveModule、KruskalModule、BFSModule、HungarianModule（二分图匹配）
- 动态规划：Knapsack01Module、KnapsackCompleteModule、LISModule
- 字符串：KMPNextModule（已改造为 `pi[]` 1-indexed + `#` 哨兵）、KMPMatchModule（匹配过程）
- 线段树：SegmentTreeBuildModule（建树）、SegmentTreeTagModule（区间乘/加懒标记）、SegmentTreeQueryModule（区间查询）
- 自由结构沙盒（不含算法，仅作讲题草稿）：SandboxArrayModule、SandboxMatrixModule、SandboxTreeModule、SandboxGraphModule、SandboxBlankModule（空白画布，供老师叠加批注）

## 核心架构

- src/engine/AnimFrame.h 定义统一动作类型和帧数据。
- src/modules/IModule.h 提供模块接口、基类场景指针和公共动画辅助函数，并预置了面向未来的空实现接口（分类、复杂度、教学提示、步长代价、输入校验、画布双击回调、主题应用）。
- src/modules/GraphModuleUtils.h 提供共用的图输入解析（按行判定是否带权）与布局辅助。
- src/modules/GraphRenderHelper.h 提供共用的建点、建边、查边与标签函数，消除图模块重复布局代码。
- src/modules/SegmentTreeCommon.h 提供线段树共用的 `ls`/`rs` 移位约定、输入解析、树形拓扑生成（节点坐标由递归二分展开）。
- src/modules/SegmentTreeEditBase.h 为懒标记与查询两模块的共享基类，统一树布局、`tr`/`add`/`mul` 三条物理数组、节点高亮与指针移动逻辑。
- src/modules/HeapTreeBase.h 为堆类模块的共享基类，统一层序数组排布、完全二叉树拓扑构建与数组值到树节点的同步。
- src/modules/TwoRowArrayModule.h 为前缀和 / 差分等「双行数组」模块的共享基类，统一输入解析、双行布局构建、`PD_*` 帧渲染与多源数字飞入动画。
- src/render/GraphElements.h 与 cpp 定义数组、指针、栈、图节点和可交互图边等图元；`GraphNode` 另支持子标签（区间 `[l,r]`）、值文本（`tr=值`）与懒标记徽章（`tagText` + `tagColor`）；`GraphEdge` 支持自定义线宽（`setPenWidth()`），供割点桥模块把桥加粗高亮。
- src/render/SceneGrid.h 提供隐形对齐网格（磁吸对齐 + 单元格吸附 + Ctrl 自由拖动）。
- src/core/ViewportFitter.h 统一画布居中与自适应缩放。
- src/canvas/ 为画布批注层（双层架构，详见下方「画布批注层」章节）：AnnotationScene、AnnotationItem 及其子类 StrokeItem / ShapeItem / TextBoxItem / MarkdownItem / LatexItem / CanvasTableItem，ToolController 工具状态机，CanvasView 叠加渲染，LatexService Node 渲染助手。
- src/ui/UiTheme.h 为全项目设计令牌（颜色、圆角、间距、字体）。
- src/ui/widgets/ 为自研控件库：MyButton、MyToggleSwitch、MyRadioButton、MyCheckBox、MyComboBox、MyTextEdit、MyCollapsible、ToolIconButton。MyComboBox 为真正的 `Qt::Popup` 浮层（自绘圆角卡片 + 多层柔和投影 + 内嵌滚动区），长列表可滚动；支持 `addGroup()` 分组模式，展开为 11 类分组的 3 列按钮瓦片网格（冷调渐变瓦片 + hover 发光边框 + 彩色分节标题），类内按课程进度排序。ToolIconButton 以 `QPainterPath` 手工绘制矢量图标，供浮动轻套件使用。
- src/ui/MainWindow.cpp 管理模块注册、输入、构建、录制、播放、撤销和循环播放，并渲染图结构选项面板；模块按 11 个分类分组注册，`comboToModule` 把下拉条目序号映射到唯一模块实例下标，使同一模块可跨分支出现（如 Kruskal 同时挂在「并查集」与「图论 · 最小生成树」）。

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

代码验证以当前 Qt/CMake 配置为准。本轮 Release 与 Debug 构建均通过；`tests/` 无头自动化测试覆盖极端输入、动画中断清理、边权跟随、箭头几何、UI 状态机、单元格单线边框、就地编辑崩溃回归、画布批注层工具与渲染、快划笔画连续性、自交无白洞、形状实时预览、真像素化马赛克、批注剪贴板、两段式工具交互、插入后工具回落与双击行内编辑、批注按模块归档 / 恢复，全部通过（47 个测试套件全绿）。GUI 交互和算法结果测试尚未执行。

## 工程整洁度（2026-11-14）

- 项目根目录的 38 张调试截图（约 469 KB）与重复的 `main.cpp` 已清除；根目录现只保留 `.cursorrules` / `AGENTS.md` / `CMakeLists.txt` / `Cursor_Workflow.md` / `README.md` / `package.json` / `package-lock.json` / `.gitignore`。
- `.gitignore` 新增 `/*.png`，防止测试截图再次污染根目录；`tools/sg.ps1 -Target clean` 同时清理根目录与 `build/tests` 下的 PNG。
- 测试构建从「每个测试独立编译全部模块源码」改为共享静态库（`SgCore` / `SgWidgets` / `SgUi`），build 目录由 **7539 MB 降至 233 MB**，单个测试的编译目录由约 180 MB 降至约 1 MB。
- `StartupPanelTest` 不再依赖 `UiShot` 事先生成的截图文件，改为自行构造 `MainWindow` 抓取画面做像素分析，消除测试间的隐式顺序依赖。
