# SmartGround Bug Log

**更新时间：2026-11-11**
| ID | 问题 | 状态 | 修复记录 |
|---|---|---|---|
| BUG-001 | TarjanModule::runAlgorithm 调用 initGraph 清空了 buildLayout 录入的 head/to/nxt 邻接表，图被当成无边图处理 | 已修复 | 抽出 resetTraversalState，只重置 dfn、low、ins、stk、scc 和计数器；算法开始时保留邻接表 |
| BUG-002 | 多个派生模块重复声明 m_scene，遮蔽 IModule 的基类场景指针 | 已修复 | 删除 modules 下派生模块头文件的同名成员，并移除 HeapBasicsModule 旧构造函数初始化项，所有 buildLayout 赋值现在指向基类成员 |
| BUG-003 | 撤销重建布局后才关闭动画锁，布局动画和帧重放动画可能继续运行 | 已修复 | MainWindow 在重建和帧重放期间关闭 m_animated；Tarjan 布局初始化也遵守动画锁 |
| BUG-004 | 完整播放后只将帧索引拨回 0，没有恢复场景；不同输入可能与旧帧混用 | 已修复 | MainWindow 保存录制输入，循环播放前按录制输入重建场景 |
| BUG-005 | 切换模块或重新绘制后仍保留旧帧，可能在新场景上渲染旧算法动作 | 已修复 | 模块切换与重新绘制时清空帧队列并复位帧索引；输入变更后运行会重建当前布局 |
| BUG-006 | 项目总结、README 和模块状态把完成模块数记为 17，菜单实际只有 15 个 | 已修复 | 按 MainWindow 注册清单统一改为 15 个可运行模块，并单独统计 8 个骨架 |
| BUG-007 | 算法大纲将已实现的选择/插入/快速/归并排序、堆映射、单调栈和并查集标记为待开发 | 已修复 | 将大纲状态与源代码和 MainWindow 注册项对齐 |
| BUG-008 | Cursor_Workflow 仍要求依赖帧 0 重置，和 MainWindow 按录制输入重建场景的实现冲突 | 已修复 | 更新播放重置说明，明确撤销/重播重建场景及无动画锁要求 |
| BUG-009 | 01 背包比较历史状态时，既有移动动画会隐藏源格，破坏 DP 表已算出的状态 | 已修复 | 在 IModule 增加 playValueFlight，以保留源状态的弧线飞行呈现图节点入队和候选值转移；动画锁关闭时同步更新，并忽略过期飞行动画回调 |
| BUG-010 | 新图模块输入异常可能造成静态数组越界；负权输入会使 Dijkstra 结果失效 | 已修复 | 增加共用图输入解析和范围校验；Dijkstra 拒绝负权，图规模限制在可视化布局范围内 |
| BUG-011 | 图遍历过程的当前节点/边高亮会残留，掩盖队列层级或最终连通分量状态 | 已修复 | BFS、朴素 Dijkstra、Kruskal 和拓扑排序渲染时复位瞬态高亮并保留永久状态色 |
| BUG-012 | KMPNextModule 在 renderFrame 中重新计算 `430 - n * 30` 作为图案起点，模式串长度 ≥ 10 时指针与方格错位 | 已修复 | 起点改为成员变量 `m_startX`，在 buildLayout 中计算一次并复用 |
| BUG-013 | `IModule::playSwapAnim` 在关闭动画锁的分支里没有恢复 `showValue`，撤销后交换格子数字消失 | 已修复 | 非动画分支补上 `b1->showValue = true` 与 `b2->showValue = true` |
| BUG-014 | 图模块的有向/无向由代码硬编码，教师无法在课堂上切换 | 已修复 | IModule 新增 `supportsGraphOptions/supportsDirected/supportsBidirectional/getGraphOptions/setGraphOptions`；MainWindow 新增"图结构选项"分组（单选有向/无向 + 双向箭头复选框），按模块能力自动启用/禁用；BFS、拓扑排序、朴素 Dijkstra 支持有向/无向切换，Kruskal 强制无向，Tarjan 强制有向 |
| BUG-015 | 边权标签是独立的静态 `QGraphicsTextItem`，在画布上拖动节点或弯曲边时权值不跟随 | 已修复 | 权值徽标改为在 `GraphEdge::paint()` 内沿曲线法向偏移绘制，节点拖动与边弯曲时权值自动跟随 |
| BUG-016 | 动画结束回调捕获裸指针，`scene->clear()` 后回调访问已释放的 `ArrayBlock/FloatingText/StackBlock/GraphEdge`，导致闪退或 double free | 已修复 | 全项目动画回调统一改用 `QPointer` 包装并在使用前判空；`GraphEdge` 基类由 `QGraphicsItem` 改为 `QGraphicsObject` 以支持 `QPointer` |
| BUG-017 | `scene->clear()` 删除节点的顺序不定，`GraphEdge` 持有裸 `GraphNode*`，节点先于边释放时 `paint()` 访问悬垂指针 | 已修复 | `GraphNode`/`GraphEdge` 增加析构互相解绑（`removeEdge`/`detach`），任一先释放都不会留下悬垂边引用 |
| BUG-018 | 有向图中同时存在 u→v 与 v→u 时，渲染层把两条反向边错误合并成一条 | 已修复 | `GraphRenderHelper::buildOneEdge` 在有向模式下只按 (u,v) 精确查重；`findEdge` 增加 `directed` 参数 |
| BUG-019 | 图结构选项的单选框在点击后被 `updateGraphOptionPanel()` 用旧选项覆盖，切换无效 | 已修复 | 改为先 `applyGraphOptions()` 再刷新面板；面板只做禁用态与显示态同步，不再回写勾选状态 |
| BUG-020 | 多个模块输入越界：`DSUBasicModule` 父节点下标越界、`HeapBasicsModule` 元素数超过 n、`Queue/StackModule` 容量失控、前缀和/差分查询区间越界 | 已修复 | 统一裁剪 n 与容量上限，校验父节点下标，查询区间钳制到 [1, n]；队列/栈满时提示并忽略入队 |
| BUG-021 | `DSUBasicModule::find` 递归查找在父数组成环时无限递归导致栈溢出 | 已修复 | 改为迭代式路径压缩并带访问计数上限保护 |
| BUG-022 | `BubbleSortModule::runAlgorithm` 中 `swapped` 未初始化，`n <= 1` 时读取未定义值 | 已修复 | 初始化为 `false` |
| BUG-023 | `MonotonicStackModule` 使用 `std::stack` 演示底层结构，与项目 OI 竞赛风格不一致，且 `m_stackBlocks` 成员为空壳 | 已修复 | 改用静态数组 `int stk[N]` + `top` 模拟栈，删除未使用的 `m_stackBlocks` |
| BUG-024 | 图模块存在已废弃的 `m_weightLabels`、`m_degreeLabels`、`m_weightedEdgeList` 成员，属于逻辑不统一的残留 | 已修复 | 权值改为附着在 `GraphEdge` 上后，清理所有冗余成员及其 reset 清理代码 |
| BUG-025 | 并查集模块建边时未调用 `node->addEdge(edge)`，节点在画布上拖动时边线不跟随，表现为图像撕裂 | 已修复 | `DSUBasicModule::buildLayout()` 建边后双向注册到源点与父节点；`HeapBasicsModule::buildTree()` 同样补齐父子节点注册 |
| BUG-026 | 有向图的默认箭头不可见：箭头尖端绘制在目标节点圆心，被节点白色填充完全遮蔽 | 已修复 | `GraphEdge::paint()` 新增 `trimmedStart()/trimmedEnd()`，曲线两端按节点半径 22px 裁剪，箭头尖端落在节点圆外侧；`shape()`/`boundingRect()` 同步裁剪 |
| BUG-027 | 箭头朝向反了：方向向量取自「裁剪端点 - 控制点」，在弯曲边下指向错误一侧 | 已修复 | 改用「节点中心 - 裁剪端点」作为方向向量，有向边箭头始终指向目标节点 |
| BUG-028 | 无向图勾选「无向边加双向箭头」后没有任何视觉变化 | 已修复 | `is_bidirectional` 时在首端补画第二个箭头尖，两端各一个箭头 |
| BUG-029 | 边权徽标的白色圆角矩形底框遮挡视野 | 已修复 | 去掉底框与描边，改为纯文本绘制；同时修正权值锚点误减 `pos()` 导致的整体坐标偏移 |
| BUG-030 | 切换有向/无向图会整块重建画布，老师手工排布的节点位置被重置 | 已修复 | `IModule` 新增 `captureNodePositions()/restoreNodePositions()`，图模块经 `GraphRenderHelper` 统一存取；切换选项时先捕获坐标、重建后回写 |
| BUG-031 | 自动播放使用固定 600ms 定时器，与文字长度和动画复杂度无关，观感过快且易与动画重叠 | 已修复 | 改为单次触发链路，间隔按帧文字长度计算 `520 + 长度 * 28` 毫秒，钳制在 900 - 2800ms |
| BUG-032 | 自动播放无暂停态，老师无法中断；且暂停时若动画帧未播完就推进下一帧，会出现画面撕裂 | 已修复 | 新增 `m_playing`/`m_pendingPause` 状态机与 `m_settleTimer`；暂停先置标志，等当前帧动画（最长 800ms）播完后由 820ms 定时器确认再停止推进 |
| BUG-033 | 暂停后手动调整上一帧/下一帧，恢复自动播放仍从原暂停位置继续 | 已修复 | 播放位置统一由 `m_frameIndex` 驱动，手动步进即更新索引，恢复播放从当前索引往后继续 |
| BUG-034 | 切换模块时崩溃：`onModuleChanged()` 先赋值 `currentModule` 再清场，旧模块的 `reset()` 对已被 `scene->clear()` 删除的图元解引用，访问悬垂指针 | 已修复 | 新增 `IModule::releaseSceneItems()` 虚接口（只清指针不做状态重置），`MainWindow::clearScene()` 先释放指针再清场；`onModuleChanged()` 改为先清场再切换模块 |
| BUG-035 | `BubbleSortModule::reset()` 遍历 `m_blocks` 无判空保护，配合清场顺序会解引用已释放图元 | 已修复 | 循环内增加空指针检查 |
| BUG-036 | 非图论模块仍显示「图结构选项」分组，下方留下大块空白 | 已修复 | 面板改为动态插入布局（`insertWidget`）并 `setVisible(false)`，输入框与按钮自动上推 |
| BUG-037 | 教师清空默认数据后没有任何提示，难以回忆 OI 题目的输入格式 | 已修复 | 每个模块实现 `getInputHint()` 返回 OI 输入格式说明，`QTextEdit::setPlaceholderText()` 显示灰色占位提示 |
| BUG-038 | 图结构选项的分组框、单选框、复选框与项目其余控件风格割裂 | 已修复 | 新增全局 `applyStyleSheet()`，统一主色、圆角、边框与交互态 |
| BUG-039 | 启动软件时左上角残留「图结构选项」标签与蓝色底纹块：`setupUI()` 中面板默认可见，只有 `onModuleChanged()` 才会隐藏，而初始索引 0 不触发该槽 | 已修复 | 面板容器 `graphPanelHost` 在 `setupUI()` 末尾即 `setVisible(false)`，并由 `updateGraphOptionPanel()` 统一按模块能力显隐 |
| BUG-040 | 图形绘制后不居中：仅调用 `centerOn()` 未做缩放适配，图偏在一侧且大小不适配视口 | 已修复 | 新增全局 `src/core/ViewportFitter.h`，统一执行 `fitInView(KeepAspectRatio)` + 缩放上下限（0.25 - 2.4）+ `centerOn`，`MainWindow::adjustSceneBoundary()` 改为委托该工具类 |
| BUG-041 | 有向图三角形箭头过小过尖、几乎不可见 | 已修复 | `GraphEdge::paint()` 箭头改为等腰实心三角（长 15px、半宽 7px），并加白色描边提亮，与 LaTeX `\rightarrow` 观感一致 |
| BUG-042 | 沙盒节点完全自由拖动，老师难以把节点对齐到同一水平线/垂直线 | 已修复 | 新增 `src/render/SceneGrid.h`：`magneticAlign()` 在 18px 半径内磁吸对齐到邻近节点的 X/Y 轴，`snapToCell()` 做 56px 单元格吸附；按住 `Ctrl` 临时关闭吸附自由拖动 |
| BUG-043 | 原生 Qt 控件（按钮/单选/复选/下拉/文本域）观感生硬、文字锐化过度，与产品质感不匹配 | 已修复 | 新建自研控件库 `src/ui/widgets/`（`MyButton`/`MyToggleSwitch`/`MyRadioButton`/`MyCheckBox`/`MyComboBox`/`MyTextEdit`/`MyCollapsible`）与设计令牌 `src/ui/UiTheme.h`，全部改为自绘抗锯齿渲染 |
| BUG-044 | 有向/无向使用原生单选框，无法实现 iOS 风格滑动开关与子选项流线展开 | 已修复 | 用 `MyToggleSwitch`（灰色 OFF=有向，iOS 绿 ON=无向）替换单选框；「无向边加双向箭头」随开关做 `ratio` 透明度 + 高度动画，选择有向时向上流动隐藏 |
| BUG-045 | 算法模块下拉框点不开：浮层被直接绘制在 `MyComboBox::paintEvent()` 里，超出控件矩形的部分被裁剪掉，且没有任何弹出窗口承载列表 | 已修复 | 重写 `MyComboBox`：列表项抽为独立 `ComboRow` 控件，浮层改为真正的 `Qt::Popup` 顶层窗口（带白色圆角卡片 + 描边），点击选项即选中并关闭；点击外部或按 Esc 自动关闭；上下键可切换 |
| BUG-046 | 下拉浮层一次性铺开 25 项，观感差；浮层四周一圈黑色边框（QSS `border` 被 `FramelessWindowHint` 与透明背景放大成黑边） | 已修复 | 浮层内嵌 `QScrollArea`，视口高度上限 268px，超出即出现自定义细滚动条（8px 宽、半透明圆角把手）；卡片改为自绘 `ComboCard`（`paintEvent` 画白底 + 1px 浅灰描边 + 12px 圆角），彻底去掉 QSS 边框，并加 `QGraphicsDropShadowEffect` 投影 |
| BUG-047 | 01 背包 DP 画布文字全部粘连：DP 表在每一行重复绘制列标签 `j=0..W`，行间距仅 58px，标签溢出到相邻行 | 已修复 | DP 表改为教科书布局——列下标只在第 0 行绘制，行下标 `0..n` 独立排在左侧；行间距 58→74px，网格整体下移至 y=340，标题/坐标轴 `i`/`j` 独立标注；`ArrayBlock` 新增 `showLabel` 开关用于只保留表头标签 |
| BUG-048 | 数组框格头写 `j = 0` / `d[1]` / `vis[1]` 等冗余前缀，与冒泡排序的纯下标风格不一致 | 已修复 | 全量统一为纯下标：`ArrayBlock` 标签改为 `0,1,2...`，数组含义改由左侧行标题（queue / dist / parent / indegree 等）承担；冒泡排序等原本即用 `id` 作为下标的模块保持不变 |
| BUG-049 | 广度优先搜索下方的 `dist` / `parent` 两个数组始终没有数值：`ArrayBlock::showValue=false`，数值靠独立的 `QGraphicsTextItem` 叠在方块上，刷新时被方块自身重绘覆盖 | 已修复 | `ArrayBlock` 新增 `customValue` 字段与 `setCustomValue()` 接口，允许直接写 `∞` / `—` 等非数值内容；`BFSModule` 与 `DijkstraNaiveModule` 移除冗余的 `m_distanceLabels` / `m_parentLabels` 文本项映射，统一走方块自身绘制 |
| BUG-050 | 拓扑排序与广搜的队头/队尾指针写全称 `head` / `tail`，喧宾夺主且两个指针在同一格时完全重叠 | 已修复 | 改为单字母 `H` / `T`；新增全局 `IModule::playPointerArrange(ptrs, anchors)` 统一处理「同格多指针互相推搡」：按锚点分组，同格指针以 16/21px 步长左右分开；`TopologicalSortModule`、`BFSModule`、`QueueModule` 全部接入 |
| BUG-051 | Dijkstra 的 `u` / `v` 指针没有对准顶点中心，且老师拖动节点后指针原地不动、与顶点脱钩 | 已修复 | `GraphNode` 新增 `pointerList` 锚点注册表，`PointerBlock` 新增 `setAnchorNode()` / `syncToAnchor()`；`GraphNode::itemChange()` 在位置变化时同步所有绑定指针；`DijkstraNaiveModule` 把 `u` / `v` 绑定到对应 `GraphNode`，与权值徽标一样随节点拖动 |
| BUG-052 | 有向/无向滑动开关又长又丑，文字与滑块挤在同一条轨道上 | 已修复 | `MyToggleSwitch` 轨道宽度固定 50px、高 24px；文字移出轨道：滑块关闭时「有向图」显示在右侧，滑块打开时「无向图」显示在左侧，当前态深色、另一态浅灰 |
| BUG-053 | 数据输入框边框为通用灰色，教师难以一眼定位到哪里录入数据 | 已修复 | `UiTheme` 新增 `borderSoft()`（#C4D8F3 淡蓝），`MyTextEdit` 在视口上自绘淡蓝圆角边框，聚焦时切换为 `borderFocus()` 深蓝 |
| BUG-054 | 二分查找的 `L` / `R` / `mid` 三个指针使用固定像素偏移，同格时互相压字；LIS、KMP、队列的指针同样缺少统一的挤占机制 | 已修复 | 三个模块统一接入 `IModule::playPointerArrange()`；二分查找用 `slot` 属性记录各指针当前所在格，逐帧重排全部指针，保证任何时刻都不重叠 |
| BUG-055 | 文本框被清空后灰色占位提示不出现 | 已修复 | `MyTextEdit` 不再依赖 Qt 原生占位符（其受 `QTextEdit::placeholder` 样式子控件限制，渲染不可控），改为自持 `m_hint` 并在 `paintEvent` 中手动逐行绘制灰色 OI 输入格式说明 |
| BUG-056 | 从任意模块切回「请选择...」时画布残留上一个模块的图形，输入框与状态栏也不复位 | 已修复 | `MainWindow::onModuleChanged()` 增加 index 0 分支：清空画布、清空输入框、恢复默认占位提示、隐藏图结构选项面板、复位状态栏文案与缩放变换 |
| BUG-057 | 自动播放播完最后一帧后从头循环回放，老师无法停在终态讲解 | 已修复 | `MainWindow::advanceFrame()` 播完末帧后不再排下一帧，改由 `m_settleTimer` 收尾并调用 `finishAutoPlay()`，状态栏显示「播放完毕。」；`onFrameSettled()` 同步判定播放结束 |
| BUG-058 | 所有算法的数据输入栏全部显示为空白，且光标无法进入、无法键入 | 已修复 | `MyTextEdit::renderFrame()` 原先在 `viewport()` 上以不透明白色 `UiTheme::surface()` 填充整个视口，而 `QTextEdit::paintEvent()` 已把文档文字绘制在同一视口层且顺序在前，导致文字、光标、选区被每帧的白底彻底覆盖。重构绘制管线为四步且全部落在视口层：`renderBackground()`（不透明圆角底色）→ `QTextEdit::paintEvent()`（原生文字与光标）→ `renderPlaceholder()`（灰色提示）→ `renderBorder()`（只描边不填充），彻底消除覆盖关系 |
| BUG-059 | 输入框为空时不显示灰色默认提示文字（OI 输入格式说明） | 已修复 | 沿用 BUG-058 的分层绘制管线，`renderPlaceholder()` 在 `toPlainText()` 为空时以 `UiTheme::textMuted()` 逐行绘制多行提示文字；`setPlaceholderText()` 同步维护 `m_hint` 并触发重绘；清空文本后提示立即重现 |
| BUG-060 | `MainWindow` 全局样式表中的 `QTextEdit { background:... }` 会覆盖控件自身样式，导致输入框边框与底色失控 | 已修复 | 从 `MainWindow::applyStyleSheet()` 移除全局 `QTextEdit` 规则，改由 `MyTextEdit` 自身通过 `objectName` 作用域样式表精确控制，避免样式表优先级互相覆盖 |
| BUG-061 | 线段树建树模块首次实现时 `NodeInfo::item` 从未赋值，连边时用空指针构造 `GraphEdge`，导致切换模块瞬间崩溃 | 已修复 | 建边时统一改用 `m_treeNodes[node]` 查表取节点，并在取不到时 `continue` 跳过；`SegmentTreeEditBase` 同步修正 |
| BUG-062 | 线段树建树的 `push_up` 合并顺序错误，导致父节点的 `tr` 在孩子赋值前就被计算 | 已修复 | 递归时记录 `visitOrder`，建树全部结束后按 `visitOrder` 逆序做 `push_up`，保证后序合并 |
| BUG-063 | `AnimFrame` 字段顺序为 `type, u, v, val1, val2, group`，新模块误把数据写进 `val2` 导致区间查询答案显示为 0 | 已修复 | 统一约定：`u`=节点或下标、`v`=区间或次下标、`val1`=主值、`val2`=次值；`SEG_QUERY_DONE` 的答案放 `val1`；`SEG_DESCEND` 的左右孩子取 `val1`/`val2` |
| BUG-064 | KMP 匹配模块的模式串行 `p` 不会随匹配进度移动，`p[j+1]` 与 `s[i]` 完全不对齐 | 已修复 | 新增 `syncPatternRow(i, j)`，按 `offset = (i - (j+1)) * cellW` 整体平移模式串行与其标题，使 `p[j+1]` 恒定落在 `s[i]` 正下方 |
| BUG-065 | KMP 匹配模块中「构建 pi 数组」与「在主串上匹配」两个阶段复用同一批帧动作，导致构建阶段也去平移模式串行 | 已修复 | 引入阶段标记：构建阶段帧的 `group[0] = -1`，渲染时据此走「固定模式串行 + 指针指向 p 行」分支；匹配阶段 `group[0] = 0` 走「整体平移 + 指针分列 s/p 两行」分支；另新增 `KMPM_PI_SET` 动作专门回填 `pi[]` 数值 |
| BUG-066 | KMP 匹配模块的 `KMPM_RESET` 帧位于 pi 构建之后，会把已算出的 `pi[]` 数值清空 | 已修复 | `KMPM_RESET` 不再重置 `pi[]` 槽位，仅复位模式串行位置与高亮 |
| BUG-067 | `SEG_PUSHUP` 帧把右孩子的**数值**写进 `val2`，三个线段树模块却按节点 ID 去查 `tr` 槽位，查不到时 `std::map::operator[]` 插入空指针，随后在 `SEG_DONE` 遍历中解引用导致闪退 | 已修复 | `SEG_PUSHUP` 帧的 `group` 携带 `{左孩子ID, 右孩子ID}`，渲染端改用 `group[1]` 取右孩子；三个线段树模块统一改用 `trBlock()/arrayBlock()` 等安全查找（`find` + 判空），杜绝 `operator[]` 插入空指针 |
| BUG-068 | 所有 `QGraphicsObject` 子类共用默认 `type()`，`qgraphicsitem_cast<ArrayBlock*>` 把 `GraphNode` 误判成 `ArrayBlock`，读取到野指针并崩溃 | 已修复 | `GraphNode` / `GraphEdge` / `StackBlock` / `ArrayBlock` / `FloatingText` / `PointerBlock` / `MatrixBlock` / `LatexBlock` 各自分配唯一 `UserType + N` 并重写 `type()`，使 `qgraphicsitem_cast` 语义可靠 |
| BUG-069 | 线段树建树时 `node` 指针停在最后一行叶子节点会与下方 `tr` 数组重叠 | 已修复 | 新增 `SegmentTreeCommon::computeLayout(n, rowCount)`，按 `treeDepth(n)` 动态推算树高，数组首行下移到最深树层之下 `treeBottom + 130`；`m_trY` / `m_mulY` 由布局结果赋值，不再硬编码 |
| BUG-070 | 线段树按教学惯例开 `4n` 槽位，演示时尾部大量空槽观感极差 | 已修复 | `collectNodeIds(1, 1, n)` 精确收集真实存在的节点 ID，数组只渲染 `$2n-1$` 个有效槽位；`m_slotCount` 取最大真实 ID，三个线段树模块统一生效 |
| BUG-071 | 多个模块各自 `new QPropertyAnimation` 直接驱动同一个图元的 `"pos"` / `"scale"`，并发帧同时写入造成画面撕裂 | 已修复 | `IModule` 建立集中动画注册表 `m_animRegistry`（键为 `target + property`），`acquireAnim()` 复用同一动画并从当前插值位置重定向；新增 `playScalePulse()` / `playRowShift()` 公共封装，二分、堆、并查集、单调栈、Tarjan、排序、队列、栈等模块改为调用基类接口 |
| BUG-072 | 线段树节点的区间标签宽度固定，节点 ID 与标签互相遮盖、标签被截断 | 已修复 | `GraphNode::paint()` 用 `QFontMetrics::horizontalAdvance()` 按实际文本宽度自适应绘制 `subLabel` / `valueText` / `tagText`；徽标移到圆外 `(24, -10, tw, 20)`，`boundingRect()` 扩大为 `(-84, -48, 172, 108)` 容纳全部附属文本 |
| BUG-073 | 底部状态栏没有帧定位手段，长动画（如快速排序上百帧）无法快速回退到任意帧 | 已修复 | 新增自研 `MySlider`（`src/ui/widgets/`）接入状态栏右侧，自绘轨道 + 圆形滑块 + 径向散光辉光，尺寸与既有控件协调；`MainWindow::seekToFrame()` 重建布局后以非动画模式快进到目标帧，右侧帧徽标显示 `x / total` |
| BUG-074 | 两个 KMP 模块的字符串数组窗格之间留有 6px 缝隙，视觉上被切成独立小格 | 已修复 | 单元格矩形由 `QRectF(0, 0, m_cellW - 6, 42)` 改为 `QRectF(0, 0, m_cellW, 42)`，相邻窗格边框严丝合缝拼接 |
| BUG-075 | KMP 匹配过程模式串行 `p` 右推是瞬移，没有平移过程 | 已修复 | 新增 `IModule::playRowShift()`（`QVariantAnimation` + `InOutCubic`）驱动整行窗格与字符同步平移，`syncPatternRow()` 统一计算偏移；注册表保证重叠平移不会互相覆写 |
| BUG-076 | `latex_formulas.json` 依赖运行目录相对路径，测试环境与发布目录都读不到公式库 | 已修复 | 开启 `CMAKE_AUTORCC`，`src/resources.qrc` 将公式库编入二进制资源；`LatexBlock` 优先从 `:/render/latex_formulas.json` 读取，文件路径作为回退 |
| BUG-077 | 缺少与算法无关的自由结构沙盒模块，老师无法用纯数据结构打草稿 | 已修复 | 新增 `SandboxArrayModule` / `SandboxMatrixModule` / `SandboxTreeModule` / `SandboxGraphModule` / `SandboxLatexModule`，统一继承 `FreeStructureBase`；`ArrayBlock` 支持双击就地编辑数值（`setEditable` / `beginEdit` / `commitEdit`） |
| BUG-078 | 线段树三部曲的 `node` 指针移动时留下残影 | 已修复 | 根因是 `PointerBlock::boundingRect()` 用 `max(28, 文本宽+8)` 而 `paint()` 用 `max(40, 文本宽+8)`，超出包围盒的像素绘制了却从不参与脏区失效，移动后旧像素永久残留。统一为同一宽度公式并把包围盒扩到 `(−w/2, 0, w, 47)`；同时 `GraphNode` / `ArrayBlock` / `FloatingText` 的包围盒改为按 `QFontMetrics` 动态计算，`setSubLabel` / `setValueText` / `setTag` / `setCustomValue` 补 `prepareGeometryChange()`；`IModule` 新增 `purgeTransientItems()`，把飞行数字等瞬态图元登记为 `m_transientItems`，在帧边界统一清理，避免动画中断留下孤儿文字 |
| BUG-079 | 双击数组格输入新值时，新数字与旧数字重叠 | 已修复 | `ArrayBlock::paint()` 在 `m_editor != nullptr` 时不再绘制自身文字，改由子 `QGraphicsTextItem` 独占显示；`GraphNode` / `GraphEdge` 补齐对称的就地编辑器（双击结点改值、双击边改权值），并在编辑态抑制本体的 `customValueText` / `id` / 权值绘制 |
| BUG-080 | 就地编辑器没有提交入口，双击后输入的数字无法生效 | 已修复 | 新增 `EditorWatcher` 事件过滤器统一接管：回车/回车键提交、Esc 取消、焦点丢失自动提交；`GraphNode` 新增 `valueEdited(int, QString)` 信号，`GraphEdge` 的 `weightEdited(int)` 改为携带真实边权而非权重文本 |
| BUG-081 | 「动画帧计算」按钮与进度条功能重复 | 已修复 | 移除 `btnRun` 与 `MainWindow::onBtnRunClicked()`；点击「绘制可视化效果」后 `onBtnBuildClicked()` 自动调用 `computeFrames()` 完成后台录制，状态栏直接回报录制帧数 |
| BUG-082 | 数组 / 结点 / 边权全部是静态显示产物，老师改不动；即便改動也只是自慰修改，算法仍按原始输入计算 | 已修复 | 新增 `src/render/EditableModel.h` 存放按 `scope|row#index` 定位的实时覆盖值；`IModule` 增加 `registerArraySlot/registerNodeSlot/registerEdgeSlot` 槽位登记、`editableRowFor` 行映射、`liveValue` 读取与 `applyLiveEdit` 写回；`MainWindow::rebuildCurrentLayout()` 在建图后自动 `enableLiveEditing()`，编辑事件通过 `setLiveEditCallback` 触发 `applyLiveEditsAndReplay()`：以非动画模式重建场景 → 重跑 `runAlgorithm()` → 快进到原帧位置，后续动画按老师改后的数值继续计算 |
| BUG-083 | 01 背包 DP 表的数组块两两重叠（49 块产生 140 对重叠） | 已修复 | `ArrayBlock::boundingRect()` 原为 `QRectF(-10, -30, 80, 80)`，宽 80px 却按 60px 列距排布，相邻格上下左右全部侵入。改为固定 `QRectF(0, -27, 60, 68)`，宽度严格等于单元格节距；`drawRect` 内缩 1px，保证 2px 描边不越界 |
| BUG-084 | 二分查找 `L` / `R` / `mid` 三指针在末帧互相压字 | 已修复 | 排布引擎原来只在「同一锚点」内部推开指针，相邻格上的指针仍会互相侵入。新增 `IModule::layoutPointerSlots()`：所有指针按 x 排序后反复把侵入的相邻对各推一半，直到互不重叠；`playPointerArrange` 与 `playPointersSync` 共用同一套消解逻辑 |
| BUG-085 | 无头测试环境（`QT_QPA_PLATFORM=offscreen`）字体库为空，`QFontMetrics` 全部返回 0，导致指针宽度塌缩成 20px 最小值并互相重叠 | 已修复 | `PointerBlock::labelWidth()` 在 `horizontalAdvance()` 返回 0 时按「字符数 × 9px」兜底估算，保证任何平台下宽度合理；同时 `SandboxVisualTest` 的 KMP 边框扫描行由 141 改为 124，避开字形笔画干扰（原先仅在有字体的 Windows 平台失败） |
| BUG-086 | 所有数组结构相邻单元格之间出现明显双线分隔，无法形成 Excel 式连续表格 | 已修复 | 根因是 `ArrayBlock::paint()` 用 `drawRect(1, 1, W-2, H-2)` 把描边整体内缩 1px，相邻格的右描边与左描边中心相差 2px，中间还夹一段白缝，交界处形成 4px 暗带 + 白缝（像素实测：外边界 2px，内部交界 4px）。改为 `drawRect(0, 0, W, H)` 让相邻格描边中心精确重合于同一坐标，交界处只剩 1 条 2px 竖线；`boundingRect()` 同步扩为 `(-1, -27, W+2, H+28)` 容纳描边外扩 1px；新增 `setCellSize(w, h)` / `bodyRect()` 接口，使 `SandboxArrayModule`(62px)、`SegmentTreeBuildModule`(58/54px)、`SegmentTreeEditBase`(58/54px)、`KMPNextModule`(56px)、`KMPMatchModule`(52px) 的格宽严格等于列距，描边完全重合 |
| BUG-087 | 双击数组格就地修改时容易闪退 | 已修复 | 根因有二：其一，`EditorWatcher::eventFilter` 处理 `FocusOut` 时调用 `onDone(true)` → `commitEdit()` → 同步 `delete editor`，销毁了事件过滤器正在处理的 `receiver`，Qt 在 `sendThroughObjectEventFilters` 返回后访问已释放对象造成 use-after-free；其二，`deleteLater()` 延迟销毁期间若用户双击另一格，旧 editor 的 watcher 回调会把新 editor 提前提交。修复：新增公共 `destroyEditor()` 统一走 `setParentItem(nullptr)` → `removeItem` → `deleteLater()`，绝不在事件过滤器回调内同步销毁；`FocusOut` 分支处理完立即 `return true` 消费事件，不再回落到 `QObject::eventFilter(obj, event)`；`m_editor` 改为 `QPointer` 并配合 `QPointer<Self>` 守卫宿主对象；watcher 回调增加 `m_editor.data() != editor` 身份校验，丢弃过期 editor 的回调 |
| BUG-088 | 测试构建目录膨胀到 7.5 GB，且测试之间存在隐式的截图顺序依赖 | 已修复 | 根因一：`tests/CMakeLists.txt` 中每个测试都独立编译一遍全部模块源码（28 个测试 × 50 个 .obj × Debug `-g` 符号），造成 5.4 GB 冗余。改为提取三个共享静态库 `SgCore`（模块与渲染层）/ `SgWidgets`（自研控件）/ `SgUi`（MainWindow），测试只编译自身 `main.cpp` 后链接对应库，build 目录从 7539 MB 降至 233 MB。根因二：`StartupPanelTest` 直接读取 `ui_start.png` / `ui_graph_panel.png`，依赖 `UiShot` 先运行，清理构建目录后必然失败；改为该测试自行构造 `MainWindow` 并抓取当前画面做像素分析，彻底消除跨测试文件依赖。同时 `tools/sg.ps1` 显式指定 MinGW 工具链与生成器，避免缓存被外部工具改写后配置失败 |
| BUG-089 | Git 仓库把 `node_modules` 下 3806 个文件全部纳入版本管理，占跟踪文件的 96%，仓库体积虚高且每次依赖变动都会污染提交历史 | 已修复 | 在仓库根 `.gitignore` 增加 `node_modules/` 与 `**/node_modules/`，并用 `git rm -r --cached node_modules` 把已跟踪的 3806 个文件从索引中移除（保留磁盘上的实际依赖目录）。跟踪文件数从 3962 降到 156，全部为源码、测试、文档与构建脚本 |

## 验证

- 构建验证：本轮 Qt 6.11.1 / MinGW 13.1 Release 与 Debug 目标均通过 CMake 构建并成功链接。
- 自动化测试：`tests/` 目录，包含七个无头测试可执行程序：
  - `SmartGroundStress`：对 16 个模块跑极端输入（空图、单点、超容量、越界区间、负权、成环并查集等），全部通过。
  - `SmartGroundAnimStress`：在动画播放中途反复 `scene->clear()` + 重建，模拟撤销/切换模块的崩溃场景，Debug 构建下无异常退出。
  - `SmartGroundWeightTest`：验证边权徽标跟随节点拖动与边弯曲、无向双向标志、有向反向边不被合并。
  - `SmartGroundEdgeRenderTest`：验证有向/双向标志语义与并查集节点位置存取。
  - `SmartGroundArrowPixelTest`：像素级验证有向箭头只出现在目标端、双向箭头两端都有、无向图两端都无。
  - `SmartGroundUiBehaviorTest`：37 项断言，覆盖自研控件发现、图面板启动隐藏（BUG-039）、占位提示、自动播放按钮状态机、暂停时序、节点位置保持、自适应延迟、滑动开关子选项展开收起、画布居中与缩放钳制（BUG-040），并含 21 轮模块/选项切换崩溃回归。
  - `SmartGroundWidgetPixelTest`：10 项像素级断言，验证 iOS 开关灰→绿切换、子选项淡出、实心按钮主色、LaTeX 三角箭头的实心填充与白色描边、56px 单元格吸附。
  - `SmartGroundStartupPanelTest`：截图级断言，启动态左栏图面板像素数为 0，选中图模块后恢复为完整面板。
  - `SmartGroundComboPopupTest`：15 项断言，验证下拉浮层能真正弹出（`Qt::Popup` 顶层窗口可见、尺寸合理）、选项文字可见、选中行高亮、键盘上下键切换、Esc 关闭、点击外部关闭、二次点击切换开关。
  - `SmartGroundComboShot`：下拉浮层截图与像素断言，确认浮层实际渲染出条目文字与选中高亮。
  - `SmartGroundUiPolish`：下拉滚动条断言（`QScrollArea` 存在、策略为 `AsNeeded`、内容溢出时可滚动、视口高度受限）+ 01 背包布局断言（场景文本项两两不重叠、数组块两两不重叠）。
  - `SmartGroundUiPolish2`：13 项断言，覆盖 BUG-049（广搜 `dis` / `pa` 数组每个格子都有值）、BUG-050（`H` / `T` 短标签且拓扑双指针矩形不重叠）、BUG-051（Dijkstra 指针绑定到 `GraphNode`，拖动节点后指针位移量与节点一致）、BUG-052（滑动开关尺寸紧凑）、BUG-053（输入框存在淡蓝边框像素）、BUG-054（二分查找三指针不重叠）。
  - `SmartGroundModuleReset`：11 项断言，覆盖 BUG-055（清空输入后占位提示仍在）与 BUG-056（切回「请选择...」画布清空、输入框复位、占位提示恢复）。
  - `SmartGroundAutoPlayEnd`：4 项断言，覆盖 BUG-057（自动播放播完后按钮回到「自动播放」态且状态栏显示「播放完毕。」而非从头回播）。
  - `SmartGroundTextEditRegression`：8 项断言，覆盖 BUG-058 / BUG-059 / BUG-060（空态显示灰色提示、多行提示可读、有文本时不显示提示、清空后提示回归、聚焦后可键入、宿主窗口整体非空白、提示文字可实时替换）。以 `textMuted()` 灰色像素计数作为占位符可见性的判定依据，避免受控件高度差异干扰。
  - `SmartGroundTextEditUiShot`：在真实 `MainWindow` 中走完「启动 → 切换广搜 → 清空 → 键入 → 切换冒泡」全流程，逐帧统计输入框灰色提示像素并截图，验证 `MainWindow` 装配层同样无回归。
  - `SmartGroundNewModule`：24 项断言，覆盖线段树建树的帧序（`SEG_LEAF` / `SEG_PUSHUP` 均出现）、懒标记模块的三个关键动作（`SEG_COVER_MUL` / `SEG_COVER_ADD` / `SEG_PUSH_DOWN`）、区间查询答案 `2+3+4=9`、KMP 匹配命中数为 1 且起点为 1、KMP `pi[7]=2`（对照 `ABABCABAA`）、以及两个模块的非法输入降级（不崩溃、帧序列为空）；同时输出 `seg_build.png` / `seg_tag.png` / `seg_query.png` / `kmp_match.png` / `kmp_next.png` 五张渲染截图供人工复核。
  - `SmartGroundBorderDiag`：像素级验证 BUG-086（Excel 式单线边框）。在前缀和、冒泡排序、线段树三个模块上扫描单元格所在行，按「暗色游程的高度」区分边框与字形笔画（边框贯穿整格高、字形笔画只有十几像素），要求所有分隔线宽度恒为 2px、不出现宽度 > 3px 的双线暗带，且分隔线数量与格数严格对应；同时校验同行相邻格的列距严格等于格宽（无缝隙、无重叠）。该断言不依赖字体环境，`offscreen` 与 `windows` 平台结果一致。
  - `SmartGroundEditCrash`：17 项断言，覆盖 BUG-087（就地编辑闪退）。走完「双击开始编辑 → 焦点丢失提交 → 回车提交 → Esc 取消 → 编辑中清空场景 → 连续编辑两格时旧编辑器回调不得关闭新编辑器」全流程，全程零崩溃零 double free。
  - `SmartGroundGroupA`：32 项断言，覆盖 A 组五个新模块。树状数组验证 `FT_INSERT`/`FT_JUMP_UP`/`FT_ACC`/`FT_RESULT` 四类帧齐全、`add(2,5)` 后 `sum(5)==21`、非法输入降级为空帧序列；完全背包验证 `dp[3][8]==12`（对比 01 背包的 `dp[3][8]==10`，证明完全背包可重复取物）；Tarjan 割点与桥验证 `TC_VISIT`/`TC_CUT`/`TC_BRIDGE` 帧齐全且能定位割点；匈牙利算法验证最大匹配数、在完全图上取到 3、在 `L1-R1, L1-R2, L2-R1` 增广路场景上取到 2 并出现 `HN_OCCUPIED` 递归帧；堆调整与堆排序验证比较帧与交换帧齐全。
- 本轮全量回归：28 个测试套件串行执行完毕，全部零失败、零崩溃、零断言告警；并分别在 `offscreen`（无头）与 `windows`（真实字体）两种平台下各跑一遍，结果一致全绿。
- 截图辅助程序：`SmartGroundArrowShot`、`SmartGroundDsuShot`、`SmartGroundUiShot`、`SmartGroundToggleShot`，用于人工核对箭头、权值、滑动开关两种状态与整体 UI 观感。
- 项目目录没有 Git 元数据，无法附上提交或工作树引用。

> 说明：2026-11-14 起项目已纳入 `D:\Program\Cpp` 的 Git 仓库，SmartGround 目录位于 `Project/QT/SmartGround` 子路径下，提交与工作树引用可正常提供。

## 测试运行方式

```powershell
$env:Path = "D:\QT\Tools\mingw1310_64\bin;D:\QT\6.11.1\mingw_64\bin;$env:Path"
cmake -S tests -B build/tests -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Debug
cmake --build build/tests -j 8
$env:QT_QPA_PLATFORM = "offscreen"
.\build\tests\SmartGroundStress.exe
.\build\tests\SmartGroundAnimStress.exe
.\build\tests\SmartGroundWeightTest.exe
```
