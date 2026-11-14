# SmartGround 模块开发状态跟踪

> **更新时间**: 2026-11-14 - 画板交互修复轮 + 第 4 批联动
> **当前进度**: 37 个唯一可运行模块（38 个下拉条目） | 0 个头文件骨架
> **核心变化**: 画板十项交互问题一次性修完——①**笔迹重采样 + 中心线描边**，快划不断裂、自交不再出现白色空洞（BUG-117）；②**形状拖动期间实时预览**（BUG-118）；③**马赛克改为真像素化**，基于底层场景快照降采样再块状放大（BUG-119）；④**文本框 / 笔记插入后回落选择工具并进入行内编辑，双击不再新建**（BUG-120）；⑤**LaTeX 改为非模态实时预览编辑器**（BUG-121）；⑥**图标规范化**为铅笔 / 橡皮 / 方框 T（BUG-122）；⑦**全工具两段式交互**（首次选工具、再点展开该工具属性）；⑧**Ctrl+X/C/V 剪贴板**可在鼠标位置粘贴；⑨**模块切换批注按模块归档**，笔记公式不再随切模块丢失（第 4 批联动）；⑩Markdown 代码块与 LaTeX 预览样式细化。全量 47 个测试套件串行全绿

## 2026-11-14 画板交互修复轮 + 第 4 批联动

| 项目 | 涉及文件 | 实现要点 |
|------|----------|----------|
| 笔迹重采样与中心线描边 | `src/canvas/StrokeItem.{h,cpp}` | `resampledPoints()` 按 2.5px 线性重采样原始采样点，消除稀疏采样导致的样条过冲与断裂；样条改为逐段三次贝塞尔（每段 8 步）；`paint()` 改用 `QPen(RoundCap/RoundJoin)` 直接描中心线，彻底放弃自交 ribbon 多边形，重叠处不再挖出白色空洞（BUG-117） |
| 形状实时预览 | `src/canvas/ShapeItem.{h,cpp}` | `setGeometry()` / `setMosaicCells()` / `setKind()` 统一调用 `notifyContentChanged()`，经 `contentChanged()` 信号驱动宿主 `viewport()->update()`，拖动期间即时可见（BUG-118） |
| 真像素化马赛克 | `CanvasView::captureSceneRegion()` + `ToolController::captureMosaic()` + `ShapeItem` | 底层算法场景渲染为快照 → 降采样为 `cols×rows` 网格 → 块状放大得到像素化位图；`ShapeItem::setMosaicImage()` 优先渲染位图，缺失时回退网格多边形；位图以 base64 PNG 序列化（BUG-119） |
| 文本框 / 笔记行内编辑 | `src/canvas/ToolController.cpp` | `createItemForTool()` 完成后立即 `setTool(AnnotationSelect)`，文本框与笔记调用 `beginInlineEdit()`；`eventFilter()` 新增 `MouseButtonDblClick` 分支，双击已有元素只切换行内编辑态，不新建（BUG-120） |
| LaTeX 实时编辑器 | `src/canvas/ToolController.cpp` + `src/canvas/LatexItem.cpp` | 插入与双击均改为非模态 `AnnotationEditor`（`setLiveMode(true)`），左侧源码右侧 `QSvgRenderer` 实时渲染，源码改动即时写回卡片（BUG-121） |
| 图标规范化 | `src/ui/widgets/ToolIconButton.{h,cpp}` | 画笔 → 铅笔（笔身 + 笔尖 + 笔芯）；橡皮 → 传统斜置橡皮块（带分隔带）；文本框 → 方框内 "T"；马赛克 → 带白边棋盘格；按钮尺寸 34×34 → 30×30（BUG-122） |
| 两段式工具交互 | `src/ui/widgets/CanvasToolBar.{h,cpp}` | 15 个工具全部进入主条；按钮点击时若已是当前工具则切换 `m_toolPanel`（`QStackedWidget`）对应属性页，否则仅切工具并收起浮层。属性页覆盖：画笔 / 橡皮 / 形状 / 马赛克 / 表格 / 文本 / 笔记 / LaTeX |
| 批注剪贴板 | `src/canvas/ToolController.{h,cpp}` | `copySelection()` / `cutSelection()` / `pasteAt()` / `pasteAtCursor()`，依赖各 `AnnotationItem::cloneItem()` 虚函数；粘贴按当前鼠标场景坐标落位并注册 undo 命令 |
| 模块切换批注归档 | `src/canvas/AnnotationScene.{h,cpp}` + `src/ui/MainWindow.{h,cpp}` | `AnnotationScene::stashTo()/restoreFrom()` 以 `QHash<QString, QJsonObject>` 按模块名归档；`MainWindow::archiveAnnotations()/restoreAnnotations()` 嵌入 `onModuleChanged()`，切模块前归档、切回后恢复（第 4 批联动） |
| Markdown 代码块样式 | `src/canvas/MarkdownItem.cpp` + `src/ui/widgets/AnnotationEditor.{h,cpp}` | 卡片与编辑器预览统一补齐 `pre` / `code` / `blockquote` / `em` / `hr` 规则，代码块使用 Cascadia Mono + 浅蓝底，编辑器预览新增 `applyMarkdownPreviewStyle()` |
| 新增测试 | `tests/CanvasPolishTest.cpp`、`tests/CanvasEditingTest.cpp` | 前者 19 项断言（快划零断裂列、自交零白洞、形状实时预览、真马赛克像素、剪贴板增删与撤销重做、两段式点击）；后者 16 项断言（插入回落、行内编辑、归档 / 恢复、双击不新建） |

## 2026-11-14 画笔手感升级轮（ZoomIt 风格）

| 项目 | 涉及文件 | 实现要点 |
|------|----------|----------|
| 笔迹样条平滑 | `src/canvas/StrokeItem.{h,cpp}` | `smoothPoints()` 用 Catmull-Rom 转三次贝塞尔插值（控制点 `c1 = p1 + (p2-p0)/6`、`c2 = p2 - (p3-p1)/6`，每段细分 12 步）；`paint()` 沿平滑路径求切线法向、左右各偏移半笔宽生成闭合丝带多边形填充，笔画边缘随曲线自然过渡（BUG-114） |
| 采样降噪 | `StrokeItem::addPoint()` | 最小采样间距 `kMinSampleDistance = 0.9`，剔除抖动点，避免笔迹毛刺 |
| 批注层挂载 view | `src/canvas/AnnotationScene.{h,cpp}` | 新增 `setHostView()` / `hostView()`；`addItem` / `removeItem` / `destroyItem` 变更后立即 `m_hostView->viewport()->update()`；`CanvasView::setAnnotationScene()` 自动完成挂接（BUG-115） |
| 逐点实时重绘 | `src/canvas/AnnotationItem.{h,cpp}` | 新增 `contentChanged()` 信号与 `notifyContentChanged()`；`AnnotationScene::addItem()` 为每个图元连接该信号，笔画**绘制过程中每加一点即通知重绘**，不再等到松手 |
| 工具层显式刷新 | `src/canvas/ToolController.cpp` | `updateStroke` / `finishStroke` / `updateShape` / `finishShape` / `updateEraseDrag` / `finishEraseDrag` 均调用 `m_view->refreshOverlay()` |
| 橡皮拖动擦除 | `src/canvas/ToolController.{h,cpp}` | 新增 `beginEraseDrag()` / `updateEraseDrag()` / `finishEraseDrag()`；拖动时按 `eraserRadius * 0.4` 步长沿路径插值逐段擦除（BUG-116） |
| 命中判定统一 | `src/canvas/AnnotationItem.{h,cpp}` + `StrokeItem.{h,cpp}` | 新增虚函数 `hitTest(scenePos, radius)`；`StrokeItem` 覆写为「样条轮廓命中 **或** 距任一原始采样点 ≤ 半径 + 半笔宽」，兼顾平滑偏差与细笔画 |
| 橡皮光标环 | `src/canvas/CanvasView.cpp` | `drawForeground()` 在橡皮激活时绘制红色描边 + 半透明填充圆环，实时跟随鼠标，直观显示擦除范围 |
| 荧光笔 | `src/canvas/StrokeItem.{h,cpp}` + `ToolController` | 新增 `StrokeItem::Mode { Ink, Highlighter }`；高亮笔 alpha 0.38、宽度为普通笔 2.4 倍、`FlatCap + RoundJoin` 马克笔笔头，叠加处不加深；工具枚举新增 `Highlighter`，快捷键 `H` |
| 滚轮调笔宽 | `src/canvas/ToolController.cpp` | 画笔 / 荧光笔激活时滚轮增减笔宽（0.5px 步进，1~40px）；橡皮激活时滚轮增减橡皮半径（2px 步进）——ZoomIt 标志性手感 |
| 数字键换色 | `src/canvas/ToolController.cpp` + `MainWindow.cpp` | `1`~`7` 切换 红 / 绿 / 蓝 / 黄 / 橙 / 粉 / 深灰；`Alt + R/G/B/Y/O/P/K` 为备用组合（因 B/R/G/O 已占用为工具快捷键） |
| 橡皮尺寸滑块 | `src/ui/widgets/CanvasToolBar.cpp` | 「样式」浮层新增「橡皮」滑块（4~60px），与笔宽 / 不透明度并列 |
| 荧光笔图标 | `src/ui/widgets/ToolIconButton.{h,cpp}` | 新增 `Highlighter` 矢量图标（斜体笔身 + 高亮条），图标总数 17 → 18 |
| 新增测试 | `tests/BrushQualityTest.cpp`、`tests/AppBrushTest.cpp` | 前者 25 项断言（实时可见、实时增长、擦除即时生效、撤销复原、荧光笔半透明、笔迹恒宽、橡皮拖动、滚轮调宽、数字键换色）；后者 7 项断言 + 3 张真实窗口截图（`app_brush_stroke.png` / `app_brush_highlighter.png` / `app_brush_eraser.png`） |

## 2026-11-14 浮动轻套件重构轮

| 项目 | 涉及文件 | 实现要点 |
|------|----------|----------|
| 浮动工具套件 | `src/ui/widgets/CanvasToolBar.{h,cpp}` | 由 `rightLayout` 内的布局控件改为 `CanvasView` 的透明覆盖层子级，不参与任何布局计算，对窗口尺寸零贡献；主条 451 × 50px，顶边居中悬浮 |
| 矢量图标控件 | 新增 `src/ui/widgets/ToolIconButton.{h,cpp}` | 17 种图标全部用 `QPainterPath` 手工绘制，不依赖 Segoe UI Symbol 字形，杜绝缺字方框；自带 hover / active 双态与工具提示 |
| 互斥浮层 | `CanvasToolBar::toggleStylePanel()` / `toggleMorePanel()` | 「样式」浮层装颜色 / 粗细 / 不透明度 / 文字样式；「更多」浮层装文本 / 笔记 / 公式 / 表格插入、表格行列、撤销重做删除、批注显隐锁定、导出清空；两者互斥并按宿主边界自动避让 |
| 宿主跟随 | `CanvasToolBar::eventFilter()` | 监听 `CanvasView` 的 `Resize` 事件重新定位主条与浮层，画布缩放 / 改尺寸时套件始终贴顶居中 |
| 动作转交 | `MainWindow::setupCanvasTools()` | 导出 / 清空改由 `exportRequested` / `clearRequested` 信号转交 `MainWindow`，避免与浮层内的同名按钮形成两套控件 |
| 下拉分类标题 | `src/ui/widgets/MyComboBox.cpp` | `ComboSectionHeader::paintEvent()` 原用 `setBrush(渐变)` + `setPen(NoPen)` 后 `drawText()`；因 `drawText` 只用画笔绘制，标题被隐形。改为 `setPen(深色)` 绘制标题，竖条与装饰线继续用画刷；强调色过浅时回退 `textPrimary()` |
| 新增测试 | `tests/ToolSuiteTest.cpp`、`tests/AppWidthTest.cpp` | 前者 11 项断言（分类标题深色像素 > 200、套件宽度 < 520px、两浮层开合）；后者 6 项断言（真实 `MainWindow` 自然宽度 694px、最小宽度 558px、套件确为 `CanvasView` 子级、两浮层在真实窗口可开合） |

## 2026-11-14 隐形对齐网格修复轮

| 项目 | 涉及文件 | 实现要点 |
|------|----------|----------|
| 网格从不显示 | `src/render/SceneGrid.h/.cpp`、`src/ui/MainWindow.cpp` | BUG-105：`GridGuideLayer` 从未 `scene->addItem(this)`，网格实际不可见；且其 `boundingRect()` 依赖 `itemsBoundingRect()` 会自引用递归（实测崩溃码 `-1073741571`）。改为新增 `GridGuideScene`（继承 `QGraphicsScene`，覆写 `drawBackground()`）在背景层绘制 56px 淡色点线，`MainWindow` 改用 `GridGuideScene` 作为画布场景；删除 `SceneGrid::attach()` 与 `GridGuideLayer` 整个类，`drawGuides()` 签名改为 `(painter, bounds, config)` |
| 网格回归测试 | `tests/GridGuideTest.cpp` | 9 项断言：启用后有网格像素、关闭后归零、网格不进入 `scene->items()`（不影响居中）、`scene->clear()` 后仍存活、真实 `MainWindow` 画布呈现网格 |

## 2026-11-14 分层格阵 / 科技感下拉 / 命名精简 / 代码瘦身轮

| 能力 | 覆盖范围 | 实现要点 |
|------|----------|----------|
| 树状数组二叉分层格阵 | `FenwickTreeModule` | 底层为完整数组 `a[1..n]`，其上按 `ctz(i)`（`log2(lowbit(i))`）分 `floor(log2(n))+1` 层；第 `k` 层 `t[i]` 块宽 = `lowbit(i) * cellW`，水平覆盖 `[i-lowbit(i)+1, i]`；每层左侧标 `L0..Lk` 与 `lowbit`，右侧标覆盖区间；层间以半透明梯形引导线连接（BUG-099） |
| 树状数组自底向上合并 | `FenwickTreeModule` + `AnimFrame.h` | 新增 `ActionType::FT_MERGE`；构建序列改为 `t[i] = t[i-2^(k-1)] + a[i-2^(k-1)+1] + ... + a[i]`，渲染时兄弟块 `t[i-2^(k-1)]` 与底层 `a[]` 格的数字**同时飞入** `t[i]` 合成新值（BUG-099） |
| 多源数字飞入 | `IModule` | 新增 `playMultiValueFlight(sources, values, target, total, color)`：任意数量来源的 `FloatingText` 沿各自弧线同时飞向目标格并求和落格，供树状数组合并与后续同类场景复用 |
| 下拉浮层边缘暗线修复 | `MyComboBox` | `ComboPopupFrame::paintEvent()` 改用「外环 - 内环」奇偶填充差集路径逐层绘制柔和阴影（12 环、向下偏移 4px），避免溢出被浮层矩形硬切产生直角暗带；浮层外边距放宽至 `18/18/18/22`、宽度补偿 +36；卡片边框改半透明（BUG-098） |
| 下拉科技感瓦片 | `MyComboBox` | `ComboOption::paintTile()` 冷调渐变瓦片 + 细边框 + hover 发光边框 + 左侧强调条 + 选中态顶部高光；`ComboSectionHeader` 彩色竖条 + 渐变标题文字 + 尾部渐隐短横线（原横贯分割线已移除）；字体统一 `UiTheme::uiFont()`（Microsoft YaHei UI）（BUG-100） |
| 分组细化与跨分支 | `MainWindow` | 分组细化为「自由结构 / 数学基础 / 线性结构 / 经典排序 / 堆结构 / 并查集 / 树状数组 / 线段树 / 图论算法 / 匹配与动态规划 / 字符串算法」11 类；新增 `comboToModule` 把下拉序号映射到唯一模块实例下标，Kruskal 同时出现在「并查集」与「图论 · 最小生成树」（BUG-102） |
| 模块命名精简 | 16 个模块 | 「匈牙利算法」「线段树 (lz标记)」「线段树 (建树)」「线段树 (区间查询)」「KMP (pi数组)」「KMP (匹配)」「Dijkstra 最短路」「完全背包」「01 背包」「堆调整 up/down」「LIS 最长递增子序列」「Tarjan 强连通分量」「并查集」「BFS 广度优先搜索」（BUG-101） |
| 双行数组公共基类 | 新增 `TwoRowArrayModule.h/.cpp` | 收敛输入解析、双行布局构建、`PD_*` 帧渲染与多源飞入动画；`PrefixSumModule` / `DifferenceModule` 只保留各自语义与帧序列，代码量减少约 45%（BUG-103） |
| 死代码清理 | 全项目 | 删除 `ArrayModule.h/.cpp`（无派生类）与 `IModuleHelpers.h`；清理 `IModule` 中 8 个无调用方辅助函数、`GraphRenderHelper.h` 中 2 个未用函数、`HeapTreeBase.h` 中从未写入的 `m_relationLabels`；同步更新两份 `CMakeLists.txt`（BUG-104） |
| 新增截图工具 | `tests/FenwickShot.cpp` / `tests/ComboGroupShot.cpp` | 输出 `fenwick_lattice.png`（分层格阵）与 `combo_groups.png`（分组瓦片网格），供人工核对 |

## 2026-11-14 八项视觉与交互调整轮

| 能力 | 覆盖范围 | 实现要点 |
|------|----------|----------|
| 线段树-建树简化 | `SegmentTreeBuildModule` / `SegmentTreeCommon` | 只保留 `tr[]`（区间和）与 `lz[]`（区间加懒标记，建树阶段恒 0）两行；删除 `mul[]` 行与全部 `% p` 取模；`push_up` = `tr[ls] + tr[rs]`，叶子 = `a[l]`；`parseInput()` 只解析 `n` 与 `a[1..n]`（兼容旧表头）；新增 `ActionType::SEG_INIT_LZ`（BUG-090） |
| 树状数组嵌套三角 | `FenwickTreeModule` | 上排 `t`、下排 `a`；每个 `t[i]` 以格心为顶点、其覆盖区间 `[i-lowbit(i)+1, i]` 对应 `a` 格为底边绘制半透明填充三角，并向区间内每个 `a[j]` 拉出辐射线，形成「大三角套小三角」结构图（BUG-091） |
| 沙盒清空画布文字 | 五个 `Sandbox*Module` + `FreeStructureBase` | `showHint()` 空实现、`m_mathText` 不再创建；移除标题/提示/公式题注，画布只留纯数据结构图元；`LatexBlock::setTargetHeight()` 按目标高度排布公式矢量图（BUG-092） |
| 沙盒图结构 u v w | `SandboxGraphModule` | 每行支持 2 或 3 个元素；3 元素时该边带权 w，2 元素时 `has_weight=false`，彻底取消默认权值注入；默认输入改为 5 条带权边（BUG-093） |
| 图元全局双击编辑 | `IModule` / `GraphElements` | `enableLiveEditing()` 为未登记的 `GraphNode`（行 `"v"`）与 `GraphEdge`（行 `"w"`）自动补登记槽位；`GraphEdge::beginEdit()` 放宽 `has_weight` 前置条件，无权边双击后自动补权值；`GraphNode::commitEdit()` 写 `customValueText` 使新值落在圆内并抑制重复 `v:N`；`refreshLiveSlots()` 同步回填（BUG-094） |
| 进度条旋钮修复 | `MySlider` | 引入左右对称内边距 `kSidePadding = 14.0`，`handleCenterX(t)` 在 `[14, width-14]` 区间插值，`valueAtX()` 同步换算；辉光按 `maxGlow` 逐环裁剪，杜绝越界像素（BUG-095） |
| 模块下拉分组网格 | `MyComboBox` / `MainWindow` | 新增 `addGroup()` 与 `ComboTile` 瓦片 / `ComboSectionHeader` 分节标题；下拉展开为「基础结构 / 数学 / 数据结构 / 算法」四类、每类 3 列按钮网格、类内按课程进度排序；浮层宽度下限 520px、视口高度上限 430px；`MainWindow` 依分组重建 `registeredModules` 顺序以保证索引对齐（BUG-096） |
| 堆结构动画重构 | `HeapTreeBase` / `HeapAdjustModule` / `HeapSortModule` | 结点值写入 `customValueText`（圆内）、1-based 序号写入 `subLabel`（圆外上方）；新增 `applyHeapValues()` / `swapHeapValues()` 在 `HG_SWAP` / `HS_SWAP` 真正互换数组块与结点值；新增 `highlightRelation()` 高亮 `i/2`、`2i`、`2i+1` 父子关系并在数组行下方给出关系说明（BUG-097） |

## 2026-11-14 A 组经典算法模块轮

| 能力 | 覆盖范围 | 实现要点 |
|------|----------|----------|
| 树状数组 BIT | 新模块 `FenwickTreeModule` | 上排原数组 `a`、下排树状数组 `t` 双排联动；半透明覆盖带直接标出 `t[i]` 维护的区间 `[i-lowbit(i)+1, i]`；`add` 沿 `i += lowbit(i)` 向右上跳跃累加，`sum` 沿 `i -= lowbit(i)` 向左上跳跃收集；每步弹出 `lowbit` 计算式 |
| 完全背包 DP | 新模块 `KnapsackCompleteModule` | 与 01 背包同款二维网格排版；候选值分「不选」（上一行同列，蓝色）与「再选一次」（同一行左侧，橙色）两色，转移箭头来自**同行左侧**，直观体现物品可重复使用 |
| Tarjan 割点与桥 | 新模块 `TarjanCutVertexBridgeModule` | 复用 `GraphRenderHelper` 圆环布局；节点下方挂 `dfn[]` 与 `low[]` 两条状态数组；割点染红并脉冲放大，桥染成深红且线宽加粗；判定条件分别标注 `low[v] >= dfn[u]` 与 `low[v] > dfn[u]` |
| 二分图匹配 | 新模块 `HungarianModule` | 左右两列节点（左侧蓝、右侧橙）；增广路过程按「尝试匹配 → 目标已占 → 递归让位 → 回溯改配」四段着色，成功匹配转绿并加粗连线，失败路径标红 |
| 堆 up / down | 新模块 `HeapAdjustModule` | 复用 `HeapTreeBase` 的数组↔树双视图；`up()` 演示与父节点比较上浮，`down()` 演示与孩子比较下沉，交换时数组格用弧线交换动画、树节点同步变色 |
| 堆排序 | 新模块 `HeapSortModule` | 先自底向上建堆（对 `n/2..1` 逐个 `down()`），再反复取堆顶与堆尾交换、缩小堆区间并重新下沉；已归位的位置染绿区分有序区 |
| 双视图基类 | 新增 `HeapTreeBase.h` | 抽出堆类模块共用的层序数组排布 + 完全二叉树拓扑构建 + 数组值同步到树节点的逻辑，避免两个堆模块重复实现 |
| 边线宽可配置 | 全局 `GraphEdge` | 新增 `setPenWidth()/penWidth()`，`GraphEdge::paint()` 使用该线宽绘制曲线，割点桥模块据此把桥加粗到 4px |
| 动作类型扩展 | 全局 `AnimFrame.h` | 新增 24 个动作类型：`FT_*`（树状数组 6 个）、`KC_*`（完全背包 3 个）、`TC_*`（Tarjan 6 个）、`HN_*`（匈牙利 7 个）、`HG_*`（堆调整 4 个）、`HS_*`（堆排序 4 个） |
| 仓库治理 | Git | 根 `.gitignore` 增加 `node_modules/`，并把已跟踪的 3806 个依赖文件移出索引；跟踪文件从 3962 降到 156（BUG-089） |
| 新增测试 | `tests/GroupATest.cpp` | 32 项断言覆盖 A 组五个模块的帧序、答案正确性与渲染非空，含匈牙利增广路场景与 BIT 非法输入降级 |

## 2026-11-13 单元格单线边框与编辑稳定性轮

| 能力 | 覆盖范围 | 实现要点 |
|------|----------|----------|
| Excel 式连续表格 | 全局 `ArrayBlock::paint()` | 描边由 `drawRect(1,1,W-2,H-2)` 改为 `drawRect(0,0,W,H)`，相邻格的右/左描边中心精确重合于同一坐标，交界处只剩 1 条 2px 竖线，不再出现「双线 + 白缝」（BUG-086）；`boundingRect()` 扩为 `(-1,-27,W+2,H+28)` 容纳描边外扩 |
| 格宽可配置 | 全局 `ArrayBlock` | 新增 `setCellSize(w,h)` / `cellWidth()` / `cellHeight()` / `bodyRect()`；已接入 `SandboxArrayModule`(62px)、`SegmentTreeBuildModule`(58/54px)、`SegmentTreeEditBase`(58/54px)、`KMPNextModule`(56px)、`KMPMatchModule`(52px)，格宽严格等于列距，描边完全重合 |
| 编辑器延迟销毁 | 全局 `GraphElements` | 新增公共 `destroyEditor()`：`setParentItem(nullptr)` → `removeItem()` → `deleteLater()`，绝不在事件过滤器回调内同步销毁被过滤对象（BUG-087） |
| 事件消费 | 全局 `EditorWatcher` | `FocusOut` 分支处理完立即 `return true`，不再回落到 `QObject::eventFilter(obj, event)` 触碰可能已释放的 `receiver` |
| 指针化编辑器 | 全局 `ArrayBlock` / `GraphNode` / `GraphEdge` | `m_editor` 由裸指针改为 `QPointer`；watcher 回调用 `QPointer<Self>` 守卫宿主对象，并加 `m_editor.data() != editor` 身份校验，丢弃过期回调 |
| 编辑器自适应居中 | 全局 `ArrayBlock::beginEdit()` | 编辑器定位改用 `m_cellW / m_cellH`，不再硬编码 `30.0 / 20.0`，非 60px 宽模块同样居中 |
| 边框回归测试 | `tests/BorderDiagTest.cpp` | 像素级扫描单元格所在行的暗色游程宽度：边框恒 2px、数字笔画 1px，禁止出现 >3px 的双线暗带；另校验同行相邻格列距严格等于格宽 |
| 编辑崩溃回归测试 | `tests/EditCrashTest.cpp` | 17 项断言覆盖「开始编辑 → 焦点丢失提交 → 回车提交 → Esc 取消 → 编辑中清空场景 → 连续编辑两格时旧回调不得关闭新编辑器」 |
| 双平台回归 | `tests/` 全量 | 27 个测试套件在 `offscreen` 与 `windows` 两种平台下各跑一遍，全部零失败 |
| 构建体积治理 | `tests/CMakeLists.txt` | 提取共享静态库 `SgCore` / `SgWidgets` / `SgUi`，测试不再各自重复编译全部模块源码；build 目录从 7539 MB 降至 233 MB（BUG-088） |
| 工具链固化 | `tools/sg.ps1` | 显式指定 MinGW 编译器、make 程序与 Qt 前缀路径，避免构建缓存被外部工具改写导致配置失败 |

## 2026-11-12 包围盒与指针消解轮

| 能力 | 覆盖范围 | 实现要点 |
|------|----------|----------|
| 固定格身包围盒 | 全局 `ArrayBlock` | `boundingRect()` 由动态计算改为固定尺寸：宽度严格等于该行单元格节距，彻底消除 01 背包 DP 表 140 对重叠（BUG-083）。注：该轮采用的「`drawRect` 内缩 1px」做法在后续 BUG-086 中已被推翻，现改为描边绘制在格身边界上，相邻格描边中心重合为同一条 2px 线 |
| 跨组指针消解 | 全局 `IModule::layoutPointerSlots()` | 所有指针按 x 排序后反复把侵入的相邻对各推一半，直到互不重叠；相邻格上的指针（如二分查找 `mid` 与 `R`）不再互相压字（BUG-084） |
| 排布逻辑收敛 | `playPointerArrange` / `playPointersSync` | 两个入口共用同一套消解逻辑，避免规则漂移 |
| 无字体环境兜底 | 全局 `PointerBlock::labelWidth()` | `horizontalAdvance()` 返回 0 时按「字符数 × 9px」估算宽度，避免无头测试环境指针宽度塌缩（BUG-085） |
| 双平台回归 | `tests/` 全量 | 25 个测试套件在 `offscreen` 与 `windows` 两种平台下各跑一遍，全部零失败 |

## 2026-11-11 实时可编辑数据与残影修复轮

| 能力 | 覆盖范围 | 实现要点 |
|------|----------|----------|
| 实时覆盖值存储 | 全局 `src/render/EditableModel.h` | 按 `scope|row#index` 存放老师改过的数值；`scope` 取模块名，不同模块互不干扰；重新绘制时按 scope 整体清空 |
| 槽位登记机制 | 全局 `IModule` | `registerArraySlot(block,row,index)` / `registerNodeSlot(id,row,index)` / `registerEdgeSlot(key,row,index)` 把画布图元映射到数据行与下标；`liveValue(row,index,computed)` 在算法计算时优先取覆盖值，缺失时回落到算法算出的值 |
| 自动开启编辑 | 全局 `MainWindow` | `rebuildCurrentLayout()` 建图后自动 `enableLiveEditing()`，无需模块逐个接线 |
| 修改真实重算 | 全局 `MainWindow` | 编辑事件经 `setLiveEditCallback` → `applyLiveEditsAndReplay()`：非动画重建场景 → 重跑 `runAlgorithm()` → 快进到原帧位置；下游状态随改后的数值重新推导（如改 `S[2]` 后 `S[3]`、`S[4]` 同步重算） |
| 数组行可编辑 | 前缀和、差分、冒泡/选择/插入/快速/归并排序、单调栈、LIS、三个线段树模块 | 原数组行 `a`、前缀和行 `s`、差分行 `d`、线段树 `a`/`tr`/`add`/`mul` 全部登记为可编辑槽位 |
| 结点与边权可编辑 | 朴素 Dijkstra、Kruskal、BFS、拓扑排序 | 结点值 `v`、边权 `w`、Dijkstra 的 `dis`/`vis`/`pa` 数组登记为可编辑槽位；算法读取覆盖值重新推导 |
| 就地编辑器 | `ArrayBlock` / `GraphNode` / `GraphEdge` | 双击进入编辑，编辑态抑制本体文字绘制避免新旧数字重叠；`EditorWatcher` 事件过滤器统一处理回车提交、Esc 取消、焦点丢失提交 |
| 指针残影根治 | 全局 `GraphElements` | `PointerBlock` 包围盒宽度公式与 `paint()` 统一为 `labelWidth()`，杜绝绘制超出脏区导致旧像素残留；`GraphNode`/`FloatingText` 包围盒改用 `QFontMetrics` 动态计算；文本变化补 `prepareGeometryChange()` |
| 瞬态图元清理 | 全局 `IModule` | `m_transientItems` 登记飞行数字等临时文字，`purgeTransientItems()` 在帧边界统一清理，动画中断不留孤儿 |
| 移除「动画帧计算」 | 全局 `MainWindow` | `btnRun` 与 `onBtnRunClicked()` 移除；点击「绘制可视化效果」后自动 `computeFrames()` 完成后台录制 |
| 新增测试 | `SmartGroundLiveEdit` | 8 项断言，验证 `s[2]` 改值后 `S[3]`/`S[4]` 真实重算、冒泡排序改 `a[0]` 后帧内容同步更新 |

## 2026-11-14 画布批注层三轮（Batch 1 / 2 / 3）

| 能力 | 覆盖范围 | 实现要点 |
|------|----------|----------|
| 双层画布架构 | `src/canvas/AnnotationScene` + `CanvasView` | 下层 `GridGuideScene` 承载算法图元（$Z\in[-3,10]$），上层 `AnnotationScene` 独立承载批注图元；上层不进入下层 `itemsBoundingRect()`，因此 `ViewportFitter` 的居中与缩放不会被批注污染。`CanvasView::drawForeground()` 把上层场景按视口变换叠加绘制，选框矩形与上层共用同一套变换 |
| 工具状态机 | `src/canvas/ToolController` | 14 种工具：指针 / 选中 / 画笔 / 橡皮 / 直线 / 方框 / 圆形 / 箭头 / 三角 / 马赛克 / 文本框 / Markdown / LaTeX / 表格。`eventFilter` 从视口拦截原生 `QMouseEvent` 并经 `view->mapToScene()` 换算场景坐标，避免合成 `QGraphicsSceneMouseEvent` 触发的 `QEventPoint::position()` 崩溃 |
| Shift 几何约束 | `ToolController::constrainPoint()` | 直线与箭头吸附到 45° 步进；方框与圆形锁定为正方形 / 正圆；三角形按等边三角形（高 = 边长 × √3/2）定位 |
| 橡皮作用域 | `ToolController::eraseAt()` | 只命中上层批注图元（`m_scene->annotationItems()`），并跳过锁定图元；对下层算法结构零影响 |
| 空白白板模块 | 新模块 `SandboxBlankModule` | 取代原 `SandboxLatexModule`（按新理解删除纯 LaTeX 模块）；下层不放任何算法结构，专门供老师在任意模块上叠加批注 |
| 上层框选与整体变换 | `ToolController` 的 `AnnotationSelect` | 框选矩形与选区命中测试都在上层完成；`move` 只改批注图元 `pos()`，`scaleBy()` 以选区中心为轴缩放，绝不改动下层结构；`Ctrl` / `Shift` 多选 |
| 撤销 / 重做栈 | `ToolController::Command` | 区分 `Add` / `Remove` 两类命令；`AnnotationScene` 统一持有批注图元所有权（`m_owned`），控制器只挂载 / 卸载不销毁，避免清空重做栈时误删已复活的图元（BUG-110） |
| 笔刷样式条 | `src/ui/widgets/CanvasToolBar` | 颜色取色盘（`MyColorSwatch`）、粗细滑块、不透明度滑块、撤销 / 重做、显隐切换、锁定切换、表格行列滑块、文本字号滑块、加粗 / 斜体 / 下划线。**注：此平铺形态已在 BUG-113 中重构为浮动轻套件的「样式 / 更多」两个互斥浮层** |
| Markdown 卡片 | `src/canvas/MarkdownItem` | `QTextDocument::setMarkdown(..., MarkdownDialectGitHub)` + GitHub 风格样式表；卡片固定宽度自动换行；双击经 `AnnotationEditor` 打开编辑器，左编辑右实时预览 |
| LaTeX 公式卡片 | `src/canvas/LatexItem` + `LatexService` | 常驻 Node 助手 `tools/latex/serve.js`（`mathjax-full`）以 JSON 行协议渲染 SVG，`QSvgRenderer` 贴进卡片；双击 `AnnotationEditor` 改源码后实时重渲染 |
| 富文本框 | `src/canvas/TextBoxItem` | 字体族 / 字号 / 加粗 / 斜体 / 下划线 / 文字颜色可调；双击经 `AnnotationEditor` 编辑正文 |
| 可编辑画布表格 | `src/canvas/CanvasTableItem` | 行列数可配，双击单元格就地编辑；与「自由结构 - 行列式」的区别是单元格默认为空字符串而非 0 |
| 编辑器对话框 | `src/ui/widgets/AnnotationEditor` | 三种模式：纯文本 / Markdown（左源码右预览）/ LaTeX（源码 + 预览）；自绘样式，不引入原生输入控件 |
| 画布 PDF 导出 | `MainWindow::exportCanvasToPdf()` | 自动取下层与上层 `itemsBoundingRect()` 的并集作为画布范围，加 40px 边距后按页面尺寸等比缩放，`QPdfWriter` 输出单页 PDF |
| 键盘快捷键 | `MainWindow::keyPressEvent()` | `V` 指针 / `A` 选中 / `B` 画笔 / `E` 橡皮 / `L` 直线 / `R` 方框 / `O` 圆形 / `D` 箭头 / `T` 三角 / `M` 马赛克 / `N` 文本 / `G` 笔记 / `X` 公式 / `I` 表格；`Ctrl+Z` / `Ctrl+Y` / `Ctrl+Shift+Z` 撤销重做 |

## 2026-11-11 沙盒结构与渲染健壮性轮

| 能力 | 覆盖范围 | 实现要点 |
|------|----------|----------|
| 沙盒 - 数组 | 新模块 `SandboxArrayModule` | 纯数据结构草稿，无算法；`ArrayBlock` 双击进入就地编辑，输入即改写格子数值并广播 `valueEdited` |
| 沙盒 - 矩阵 / 行列式 | 新模块 `SandboxMatrixModule` | 行列数可配置，默认 3×3；`MatrixBlock` 负责网格绘制，行列式以方括号包裹呈现 |
| 沙盒 - 树形结构 | 新模块 `SandboxTreeModule` | 复用 `GraphNode` / `GraphEdge`，节点可拖动，边随动 |
| 沙盒 - 图结构 | 新模块 `SandboxGraphModule` | 有向 / 无向 / 双向三态切换，复用图选项管线 |
| 沙盒 - 空白画布 | 新模块 `SandboxBlankModule` | 取代原 `SandboxLatexModule`；下层不放任何算法结构，专门供老师在画布上叠加批注层 |
| 集中动画注册表 | `IModule` | `m_animRegistry` 以 `target + property` 为键复用动画，从当前插值位置重定向，杜绝并发帧同时写入同一 `"pos"` 造成撕裂；新增 `playScalePulse()` / `playRowShift()` |
| 帧滑条 | `MySlider` + `MainWindow` | 自绘轨道 / 滑块 / 散光辉光，状态栏右侧接入；`seekToFrame()` 重建布局后非动画快进；右侧帧徽标显示 `x / total` |
| 线段树动态布局 | 三个线段树模块 | `SegmentTreeCommon::computeLayout()` 按树高把数组首行下移到最深树层之下；`collectNodeIds()` 只渲染真实节点槽位（$2n-1$），不再留 `4n` 尾部空位 |
| 节点标签自适应 | `GraphNode` | `QFontMetrics` 驱动 `subLabel` / `valueText` / `tagText` 宽度；徽标移出圆外，`boundingRect()` 扩大容纳附属文本 |
| 图元类型唯一化 | `GraphElements.h` 等 | 八个 `QGraphicsObject` 子类各自分配 `UserType + N`，`qgraphicsitem_cast` 不再误判 |
| KMP 窗格与平移 | `KMPMatchModule` / `KMPNextModule` | 窗格无缝拼接；模式串行经 `playRowShift()` 以 `InOutCubic` 平移，无瞬移 |
| 新增测试 | `SmartGroundSandboxVisual` / `SmartGroundFrameSlider` | 沙盒可视化 17 项断言（布局间距、紧凑槽位、窗格无缝、标签不重叠、公式库加载）+ 帧滑条 8 项断言（存在性、禁用态、紧凑宽度、范围、跳转） |

## 2026-11-10 线段树与 KMP 匹配轮（依据课堂模板对齐）

| 能力 | 覆盖范围 | 实现要点 |
|------|----------|----------|
| 线段树 - 建树 | 新模块 `SegmentTreeBuildModule` | 严格对齐课堂第一节课模板：`build(node, l, r, a)` 单函数建树，只维护 `tr[]`（区间和）与 `lz[]`（区间加懒标记，建树阶段恒 0）；叶子 `tr[node] = a[l]`，`push_up` 用 `tr[ls] + tr[rs]`，全程无取模；`ls=node<<1` / `rs=node<<1\|1` 移位关系打在分裂瞬间；`node` 编号与 `tr[]` 槽位一一对应 |
| 线段树 - 区间乘/区间加（懒标记） | 新模块 `SegmentTreeTagModule` | `changeMul` / `changeAdd` / `push_down` 完整流程；懒标记以徽章形式挂在节点右上角（紫 `×k` / 蓝 `+k`），下放时徽章沿弧线坠向左右孩子；父节点标记清空后 `mul=1, add=0` |
| 线段树 - 区间查询 | 新模块 `SegmentTreeQueryModule` | `querySum` 递归 + `ans` 暂存方块，逐段命中值飞入累加；查询前先 `push_down` 结算欠账 |
| KMP 匹配过程 | 新模块 `KMPMatchModule` | 1-indexed + `#` 哨兵；`p[j+1]` 严格对齐 `s[i]` 正下方；`i` 永不回退、`j` 回退时模式串整体右跳；命中起点 `i-j+1` 追加到结果列表 |
| KMP pi 数组改造 | 改造 `KMPNextModule` | `next[]` → `pi[]`，0-indexed → 1-indexed，首格加 `#` 哨兵，公式文本改用 `p[i]` / `p[j+1]` / `pi[j]` 写法 |
| `GraphNode` 扩展 | 全局渲染 | 新增 `subLabel`（区间 `[l,r]`）、`valueText`（`tr=值`）、`customValueText`、`tagText`+`tagColor`（懒标记徽章）；`boundingRect()` 相应放大 |
| 公共动画扩展 | `IModule` | 新增 `playDualValueFlight()`（双值同时飞入同一目标，供 `push_up` 使用）与 `playTagDrop()`（标记徽章下坠动画） |
| 帧动作扩展 | `AnimFrame.h` | 追加 `SEG_RESET` / `SEG_INIT_MUL` / `SEG_DESCEND` / `SEG_LEAF` / `SEG_PUSHUP` / `SEG_DONE` / `SEG_OP_START` / `SEG_COVER_MUL` / `SEG_COVER_ADD` / `SEG_PARTIAL` / `SEG_PUSH_DOWN` / `SEG_TAG_CLEAR` / `SEG_QUERY_COVER` / `SEG_QUERY_ACC` / `SEG_QUERY_DONE` / `KMPM_RESET` / `KMPM_COMPARE` / `KMPM_FALLBACK` / `KMPM_ADVANCE` / `KMPM_HIT` / `KMPM_PI_SET` / `KMPM_DONE` |
| 共享基类 | `SegmentTreeEditBase` | 懒标记与查询两模块共用树布局、`tr`/`add`/`mul` 三条物理数组、节点高亮与指针移动逻辑，避免重复代码 |
| 新增测试 | `SmartGroundNewModule` | 24 项断言覆盖建树帧序、懒标记三动作、查询答案 9、KMP 命中位置 1、非法输入降级，并输出 5 张渲染截图供人工复核 |

## 2026-11-08 输入框绘制管线重构轮（全局能力，非单模块）

| 能力 | 覆盖范围 | 实现要点 |
|------|----------|----------|
| 输入框四层绘制管线 | 全局 `MyTextEdit` | 严格顺序：`renderBackground()`（不透明圆角底色）→ `QTextEdit::paintEvent()`（原生文字与光标）→ `renderPlaceholder()`（灰色多行提示）→ `renderBorder()`（只描边不填充）。全部落在 viewport 层，彻底解决「文字被自绘白底覆盖」导致的输入框全白、光标不可见、无法键入（BUG-058） |
| 空态灰色提示 | 全局 `MyTextEdit` | `toPlainText()` 为空时以 `UiTheme::textMuted()` 逐行绘制 OI 输入格式说明；`setPlaceholderText()` 维护 `m_hint` 并触发重绘；清空后提示立即重现（BUG-059） |
| 样式表作用域隔离 | 全局 | 移除 `MainWindow::applyStyleSheet()` 中的全局 `QTextEdit` 规则，改由控件自身按 `objectName` 作用域控制，避免样式表优先级互相覆盖（BUG-060） |
| 输入框回归测试 | 全局 | 新增 `SmartGroundTextEditRegression`（控件级 8 项断言）与 `SmartGroundTextEditUiShot`（`MainWindow` 全流程 5 态截图），以 `textMuted()` 灰色像素计数判定占位符可见性 |

## 2026-11-08 指针挤占与画布质感轮（全局能力，非单模块）

| 能力 | 覆盖范围 | 实现要点 |
|------|----------|----------|
| 全局指针挤占 `IModule::playPointerArrange()` | 二分查找×2、LIS、KMP、队列、拓扑排序、广搜 | 按锚点分组；同格多指针按各自文本宽度依次排开（间距 6px）；随后由 `layoutPointerSlots()` 跨组消解，保证相邻格指针也不压字 |
| 顶点指针锚点绑定 | 朴素 Dijkstra | `GraphNode::pointerList` + `PointerBlock::setAnchorNode()/syncToAnchor()`；`GraphNode::itemChange()` 位置变化时同步指针，`u` / `v` 如权值徽标般随节点拖动 |
| `ArrayBlock::customValue` | 广搜、Dijkstra | 方块直接承载 `∞` / `—` / `0` / `1` 等非数值内容，取代冗余的独立文本项叠加层 |
| 紧凑型 iOS 滑动开关 | 全局 | 轨道 50×24px；「有向图」在滑块右侧（OFF 态）、「无向图」在左侧（ON 态），当前态深色、另一态浅灰 |
| 淡蓝输入框边框 | 全局 | `UiTheme::borderSoft()` (#C4D8F3)，聚焦切换 `borderFocus()` |
| 手写占位提示 | 全局 | `MyTextEdit` 自持 `m_hint` 并逐行绘制灰色 OI 输入格式，摆脱 Qt 原生占位符的样式限制；提示绘制在文字层之上、边框层之下，清空文本即刻重现 |
| 画布复位 | 全局 | 切回「请选择...」立即清空画布、清空输入框、隐藏图面板、复位状态栏与缩放 |
| 末帧暂停 | 全局 | 自动播放播完末帧即停，状态栏提示「播放完毕。」，不再循环回播 |
| 短数组行标题 | 拓扑排序 / Dijkstra / Kruskal / 广搜 / LIS | `ind` `q` `out` / `dis` `vis` `pa` / `e[]` `pa` / `q` `dis` `pa` / `a[]` `dp` `g` |
| 短指针标签 | 拓扑排序、广搜、队列 | 队头队尾指针由 `head` / `tail` 改为 `H` / `T` |

## 2026-11-08 UI 全面自研轮（全局能力，非单模块）

| 能力 | 覆盖范围 | 实现要点 |
|------|----------|----------|
| 自研控件库 `src/ui/widgets/` | 全局 UI | `MyButton`（Solid/Soft/Ghost）、`MyToggleSwitch`、`MyRadioButton`、`MyCheckBox`、`MyComboBox`（自绘浮层）、`MyTextEdit`、`MyCollapsible`（可折叠 + 高度动画） |
| 设计令牌 `src/ui/UiTheme.h` | 全局 UI | 颜色、圆角、间距、`uiFont()`/`monoFont()` 字体策略，禁止业务代码写死色值 |
| iOS 风格图类型开关 | BFS / 拓扑排序 / 朴素 Dijkstra | 灰色 OFF = 有向图，iOS 绿 ON = 无向图；子选项「无向边加双向箭头」随开关做 `ratio` 透明度 + 高度动画 |
| 画布居中与自适应缩放 | 全局 | `src/core/ViewportFitter.h` 统一 `fitInView(KeepAspectRatio)` + 缩放钳制（0.25 - 2.4）+ `centerOn` |
| 隐形对齐网格 | 所有图模块 | `src/render/SceneGrid.h`，18px 磁吸对齐 + 56px 单元格吸附，`Ctrl` 临时自由拖动 |
| LaTeX 三角箭头 | 所有图模块 | `GraphEdge::paint()` 等腰实心三角（15px / 半宽 7px）+ 白色描边 |
| 启动态图面板隐藏 | 全局 | `graphPanelHost` 在 `setupUI()` 即隐藏，杜绝左上角残留色块 |
| 前瞻性全局接口 | `IModule` | 新增 `getCategoryName()` / `getComplexityNote()` / `getTeachingTip()` / `getStepCost()` / `validateInput()` / `onCanvasDoubleClick()` / `applyTheme()` 空实现钩子 |

## 图例说明
- ✅ **已完成**: 代码实现、动画锁、渲染逻辑均已就绪
- 🔄 **已重构**: 已迁移到 IModule 动画引擎（DRY 原则）
- 🚧 **开发中**: 正在实现
- ⏳ **待开发**: 尚未开始
- 🏗️ **骨架存在**: 仅有头文件，待实现

---

## 图结构选项支持矩阵

| 模块 | 有向/无向可切换 | 双向箭头 | 权值 | 说明 |
|------|------------------|----------|------|------|
| BFS | ✅ | ✅ | 自动判定 | 无向时双向入邻接表 |
| 拓扑排序 | ✅（默认有向） | ✅ | 自动判定 | 有向环检测依赖方向语义 |
| 朴素 Dijkstra | ✅ | ✅ | 自动判定 | 拒绝负权 |
| Kruskal | ❌ 强制无向 | ✅ | 必需 | 生成树定义在无向图 |
| Tarjan | ❌ 强制有向 | ❌ | 自动判定 | SCC 依赖有向语义 |

> 权值判定规则：输入中只要有一行边为 3 个元素即整体视为带权图；否则为无权图。

---

## 2026-11-08 全模块通用增强

| 能力 | 覆盖模块 | 实现要点 |
|------|----------|----------|
| `getInputHint()` 输入格式占位提示 | 全部 22 个模块 | `MyTextEdit::setPlaceholderText()` 自持 `m_hint`，空态时以 `textMuted()` 逐行绘制灰色 OI 输入格式说明 |
| `releaseSceneItems()` 图元指针释放 | 全部 22 个模块 | 清场前先释放裸指针，杜绝 `scene->clear()` 后的悬垂访问 |
| `captureNodePositions()/restoreNodePositions()` | BFS、拓扑排序、朴素 Dijkstra、Kruskal、Tarjan、并查集 | 切换图选项时保持教师手工排布的节点坐标 |
| 边跟随拖动 | 并查集、堆 | 建边时补齐 `addEdge()` 注册 |
| 箭头几何重做 | 全部图模块 | 端点按节点半径裁剪，箭头尖端落在节点外侧 |
| 纯文本边权 | 全部图模块 | 去掉白色圆角底框 |

---

## 一、排序算法 (9 模块)
| 模块名 | 状态 | 文件名 | 负责人 | 备注 |
|--------|------|--------|--------|------|
| 冒泡排序 | ✅ | BubbleSortModule | Gemini → Kiro | 已从 ArrayModule 独立 |
| 选择排序 | 🔄 | SelectionSortModule | Kiro | 已迁移到动画引擎 |
| 插入排序 | 🔄 | InsertionSortModule | Kiro | 已迁移到动画引擎 |
| 希尔排序 | ⏳ | ShellSortModule | - | 需多组颜色区分 gap |
| 快速排序 | 🔄 | QuickSortModule | Kiro | 已使用 playPointersSync |
| 归并排序 | ✅ | MergeSortModule | Kiro | 辅助数组双视图已实现 |
| 计数排序 | ⏳ | CountingSortModule | - | 需频次数组 + 前缀和 |
| 桶排序 | ⏳ | BucketSortModule | - | 需桶容器可视化 |
| 基数排序 | ⏳ | RadixSortModule | - | 需 10 个队列可视化 |

---

## 二、线性数据结构 (8 模块)
| 模块名 | 状态 | 文件名 | 负责人 | 备注 |
|--------|------|--------|--------|------|
| 前缀和 | ✅ | PrefixSumModule | Gemini | 已完成 |
| 差分 | ✅ | DifferenceModule | Gemini | 已完成 |
| 栈模拟 | ✅ | StackModule | Gemini | 已完成 |
| 队列模拟 | ✅ | QueueModule | Gemini | 已完成 |
| 循环队列 | ⏳ | CircularQueueModule | - | 需取模折返特效 |
| 静态数组基础 | ⏳ | StaticArrayModule | - | 插入/删除整体移动 |
| 单向链表 | ⏳ | SinglyLinkedListModule | - | 静态数组模拟 |
| 双向链表 | ⏳ | DoublyLinkedListModule | - | 四步指针重连 |

---

## 三、堆与优先队列 (6 模块)
| 模块名 | 状态 | 文件名 | 负责人 | 备注 |
|--------|------|--------|--------|------|
| 堆基础映射 | ✅ | HeapBasicsModule | Kiro | 数组与树双视图已完成 |
| 自底向上建堆 | 🔄 | HeapSortModule | Cursor | 已并入堆排序的建堆阶段（`n/2..1` 逐个 `down()`） |
| 自顶向下建堆 | ⏳ | HeapBuildTopDownModule | - | O(N log N) |
| 堆 up/down | ✅ | HeapAdjustModule | Cursor | up() 上浮与 down() 下沉，数组↔树双视图同步，值在圆内、序号在圆外 |
| 堆综合操作 | ⏳ | HeapOperationsModule | - | Insert/Pop/Modify |
| 堆排序 | ✅ | HeapSortModule | Cursor | 建堆 + 取堆顶交换 + 缩小堆区间，有序区染绿，交换时值真正互换 |

---

## 四、基础算法与搜索 (5 模块)
| 模块名 | 状态 | 文件名 | 负责人 | 备注 |
|--------|------|--------|--------|------|
| 二分查找(闭区间) | ✅ | BinarySearchClosedModule | Gemini | 已完成 |
| 二分查找(左闭右开) | ✅ | BinarySearchHalfOpenModule | Gemini | 已完成 |
| 双指针 | ⏳ | TwoPointersModule | - | 滑动窗口 |
| 深度优先搜索 | ⏳ | DFSModule | - | 递归树 + 回溯 |
| 广度优先搜索 | ✅ | BFSModule | Codex | 数组队列、距离、父节点和层级着色；支持有向/无向切换 |

---

## 五、树形结构与树上 DP (6 模块)
| 模块名 | 状态 | 文件名 | 负责人 | 备注 |
|--------|------|--------|--------|------|
| 树的深度与层级 | ⏳ | TreeDepthModule | - | DFS/BFS 深度传递 |
| 子树大小统计 | ⏳ | TreeSubtreeSizeModule | - | 树形 DP 入门 |
| 树的重心 | ⏳ | TreeCentroidModule | - | 切断连通块计算 |
| 树的直径 | ⏳ | TreeDiameterModule | - | 两次 DFS/BFS |
| LCA 暴力向上爬 | ⏳ | LCANaiveModule | - | 双指针碰撞 |
| LCA 倍增法 | ⏳ | LCADoublingModule | - | 2^k 跳跃 |

---

## 六、进阶数据结构 (10 模块)
| 模块名 | 状态 | 文件名 | 负责人 | 备注 |
|--------|------|--------|--------|------|
| 单调栈 | ✅ | MonotonicStackModule | Kiro | 下一个更大元素已实现 |
| 单调队列 | ⏳ | MonotonicQueueModule | - | 滑动窗口 |
| 并查集基础 | ✅ | DSUBasicModule | Kiro | Find & Union 已实现 |
| 并查集路径压缩 | ⏳ | DSUPathCompressionModule | - | 压扁树形 |
| 树状数组 | ✅ | FenwickTreeModule | Cursor | 二叉分层格阵（`floor(log2 n)+1` 层），`FT_MERGE` 自底向上合并，兄弟块与底层 `a[]` 数字同时飞入合成 `t[i]`，add/sum 双操作 |
| 线段树建树 | ✅ | SegmentTreeBuildModule | - | 纯区间求和版，无取模、无乘法懒标记 |
| 线段树单点修改 | ⏳ | SegmentTreePointUpdateModule | - | 链式回溯更新 |
| 线段树区间查询 | ⏳ | SegmentTreeRangeQueryModule | - | 拼凑目标区间 |
| 线段树延迟标记 | ⏳ | SegmentTreeLazyTagModule | - | pushdown 下坠 |
| 字典树 | ⏳ | TrieModule | - | 多叉树字符流 |

---

## 七、经典图论模板 (9 模块)
| 模块名 | 状态 | 文件名 | 负责人 | 备注 |
|--------|------|--------|--------|------|
| 链式前向星存图 | ⏳ | ForwardStarModule | - | 静态数组存图 |
| 拓扑排序 | ✅ | TopologicalSortModule | Codex | 入度队列、输出序列与有向环提示；有向/无向可切换 |
| 朴素 Dijkstra | ✅ | DijkstraNaiveModule | Codex | O(N²) 选点、非负权松弛与前驱数组；有向/无向可切换 |
| 堆优化 Dijkstra | ⏳ | DijkstraHeapModule | - | O(M log N) |
| Bellman-Ford/SPFA | ⏳ | BellmanFordModule | - | 负权边松弛 |
| Floyd 多源最短路 | ⏳ | FloydModule | - | 3D DP 矩阵 |
| Kruskal 最小生成树 | ✅ | KruskalModule | Codex | 排序边、并查集合并与成环舍弃；强制无向图 |
| Prim 最小生成树 | ⏳ | PrimModule | - | 类 Dijkstra 扩展 |
| Tarjan 强连通分量 | ✅ | TarjanModule | Gemini | 已完成 |

---

## 八、高级图论与二分图 (6 模块)
| 模块名 | 状态 | 文件名 | 负责人 | 备注 |
|--------|------|--------|--------|------|
| Tarjan 割点与桥 | ✅ | TarjanCutVertexBridgeModule | Cursor | dfn/low 判定，割点染红、桥加粗爆红 |
| Tarjan 双连通分量 | ⏳ | TarjanBCCModule | - | v-BCC/e-BCC |
| 二分图染色判定 | ⏳ | BipartiteColoringModule | - | 黑白交替染色 |
| 匈牙利算法 | ✅ | HungarianModule | Cursor | 增广路推倒重来，递归让位与回溯改配分色 |
| 欧拉路径/回路 | ⏳ | EulerianPathModule | - | Hierholzer 算法 |
| 网络流 (选修) | ⏳ | MaxFlowModule | - | Dinic/EK 算法 |

---

## 九、字符串算法 (4 模块)
| 模块名 | 状态 | 文件名 | 负责人 | 备注 |
|--------|------|--------|--------|------|
| 字符串暴力匹配 | ⏳ | BruteForceMatchModule | - | KMP 前置对比 |
| KMP Next 数组 | ✅ | KMPNextModule | Codex | 前缀函数、双指针回退 |
| KMP 匹配过程 | ⏳ | KMPMatchModule | - | 模式串滑动 |
| 字符串哈希 | ⏳ | StringHashModule | - | 前缀哈希 O(1) |

---

## 十、动态规划 (9 模块)
| 模块名 | 状态 | 文件名 | 负责人 | 备注 |
|--------|------|--------|--------|------|
| 01 背包 | ✅ | Knapsack01Module | Codex | 二维 DP、选/不选候选状态飞入；教科书式表格排版（首行列下标 + 左侧行下标） |
| 完全背包 | ✅ | KnapsackCompleteModule | Cursor | 同行左侧转移，重复取物双色候选 |
| 最长递增子序列 | ✅ | LISModule | Codex | O(N²) DP 与 O(N log N) tails 对照 |
| 最长公共子序列 | ⏳ | LCSModule | - | 二维网格字符匹配 |
| 区间 DP 石子合并 | ⏳ | IntervalDPModule | - | 三角矩阵逐层 |
| 树形 DP 没有上司 | ⏳ | TreeDPModule | - | dp[u][0/1] |
| 状压 DP 蒙德里安 | ⏳ | StateBitmaskDPModule | - | 二进制兼容判定 |
| 数位 DP | ⏳ | DigitDPModule | - | 记忆化搜索 |
| 记忆化搜索对比 | ⏳ | MemoizationVsDPModule | - | 递归树剪枝 |

---

## 十一、数论与基础数学 (5 模块)
| 模块名 | 状态 | 文件名 | 负责人 | 备注 |
|--------|------|--------|--------|------|
| 埃氏筛 | ⏳ | SieveOfEratosthenesModule | - | 倍数扫荡 |
| 线性筛/欧拉筛 | ⏳ | LinearSieveModule | - | 最小质因子精准击杀 |
| 辗转相除 GCD | ⏳ | GCDModule | - | 几何矩形缩短 |
| 快速幂 | ⏳ | FastPowModule | - | 二进制拆分 |
| 扩展欧几里得 | ⏳ | ExgcdModule | - | 递归栈推演 |

---

## 十二、贪心算法 (2 模块)
| 模块名 | 状态 | 文件名 | 负责人 | 备注 |
|--------|------|--------|--------|------|
| 区间调度 | ⏳ | IntervalSchedulingModule | - | 扫描线淘汰 |
| 哈夫曼树 | ⏳ | HuffmanTreeModule | - | 森林合并 |

---

## 十三、哈希与基础匹配 (3 模块)
| 模块名 | 状态 | 文件名 | 负责人 | 备注 |
|--------|------|--------|--------|------|
| 哈希表拉链法 | ⏳ | HashTableChainingModule | - | 链表头插 |
| 哈希表开放寻址 | ⏳ | HashTableOpenAddressingModule | - | 探测空槽 |
| 字符串哈希 | ⏳ | StringHashModule | - | 与 KMP 对比 |

---

## 开发优先级建议

### P0 (已完成 ✅)
1. ✅ **选择排序** - 最简单的排序，已验证数组模块基类，已重构使用动画引擎
2. ✅ **插入排序** - 为希尔排序打基础，已重构使用 playExtractAnim 等新特性
3. ✅ **堆基础映射** - 为后续堆模块打地基

### P1 (已完成 ✅)
1. ✅ **快速排序** - 高频算法，递归可视化技术已突破，已迁移 playPointersSync
2. ✅ **归并排序** - 辅助数组技术已实现
3. ✅ **单调栈** - 中等难度，已完成实现
4. ✅ **并查集基础** - 路径压缩前置模块

### P2 (本批已完成 ✅)
1. ✅ **拓扑排序** - 入度队列和有向环检测
2. ✅ **朴素 Dijkstra** - O(N²) 选点与松弛
3. ✅ **Kruskal** - 排序边与并查集合并
4. ✅ **BFS** - 队列遍历和层次状态

### P3 (进行中)
1. **线段树系列** - 线段树建树骨架待实现
2. **KMP 匹配过程** - Next 数组已完成，匹配过程待实现
3. **完全背包** - 01 背包已完成，继续演示同一行转移

### P4 (长期规划)
1. **高级图论（割点/桥/双连通）** - 难度高但视觉效果好
2. **状压 DP** - 需要位运算可视化创新
3. **网络流** - 选修内容

---

## 维护说明
- 每完成一个模块，将状态从 ⏳ 改为 ✅
- 迁移到动画引擎后，将状态从 ✅ 改为 🔄
- 在备注栏记录重要技术难点或依赖关系
- 定期同步到主 README.md

## 当前基线与维护记录

- 菜单注册 37 个唯一可运行模块（因跨分支别名，下拉条目共 38 个）；无头文件骨架。
- 图模块统一接入"图结构选项"面板：IModule 提供 `supportsGraphOptions/supportsDirected/supportsBidirectional/getGraphOptions/setGraphOptions`，MainWindow 渲染单选与复选控件并按模块能力禁用。
- 边权徽标绘制在 `GraphEdge::paint()` 内，沿曲线中点法向偏移，节点拖动与边弯曲时权值同步跟随。
- 图边改为可交互 `GraphEdge`（二次贝塞尔曲线 + shape() 命中检测 + 拖拽弯曲），供教师在课堂上直接拖动演示。
- 全项目动画结束回调统一使用 `QPointer` 包装图元，避免 `scene->clear()` 后的悬垂指针访问。
- `GraphNode` 与 `GraphEdge` 析构时互相解绑，防止节点先于边释放造成崩溃。
- SelectionSortModule、InsertionSortModule、QuickSortModule 已使用 IModule 动画辅助函数。
- TopologicalSortModule 与 BFSModule 共用 playValueFlight 实现图节点入队；DP 候选值也使用该公共动画。
- 其余已完成模块仍有可逐步迁移的重复渲染动画。
- 派生模块统一使用 IModule::m_scene；禁止重新声明同名场景指针。
- 自动重播和撤销按录制输入重建布局，并在快速重放期间关闭动画锁。
- Tarjan 算法启动时只重置遍历数组，不清除 buildLayout 生成的邻接表。
- 新模块顺序：先确认动画分镜，再实现算法帧、渲染、输入和重置流程。
- Bug 状态与修复验证记录在 docs/BugLog.md。
- `tests/` 提供 32 个无头测试可执行程序（共享静态库构建，build 目录约 233 MB）：覆盖极端输入压力测试、动画中断清理测试、边权跟随测试、像素级边框/箭头/控件渲染断言、UI 状态机、就地编辑崩溃回归、A 组新模块行为与菜单可达性断言、树状数组合并帧正确性断言；用 `cmake -S tests -B build/tests` 单独构建，不影响主程序。

## 下一步优先级

> 以下 1–3 项均已完成并注册，当前清单已全部完成，无头文件骨架。后续候选见 `docs/outlines.md` 各章节的 ⏳ 条目。

1. ~~SegmentTreeBuildModule：区间递归拆分与自底向上 pushup。~~ ✅ 已完成
2. ~~KMP 匹配过程：主串/模式串游标与失配滑动。~~ ✅ 已完成
3. ~~完全背包、线段树操作和堆系列。~~ ✅ 已完成（线段树三部曲、堆 up/down 与堆排序齐备）

### A 组完成后的候选

- **树状数组进阶**：区间修改 + 区间查询（双树状数组 / 差分 BIT）
- **堆综合操作**：Insert / Pop / Modify 三种操作与 `ph[k]`、`hp[k]` 映射数组
- **Tarjan 双连通分量**：v-BCC 与 e-BCC 的划分演示
- **二分图染色判定**：黑白交替染色与冲突警告（匈牙利算法的前置模块）
- **高级 DP**：区间 DP 石子合并、树形 DP、状压 DP
- **数论**：埃氏筛、线性筛、快速幂、辗转相除
- **贪心**：区间调度、哈夫曼树
- **哈希**：哈希表拉链法与开放寻址
