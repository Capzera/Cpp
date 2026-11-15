# SmartGround Bug Log

**更新时间：2026-11-14**
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
| BUG-090 | 线段树-建树作为第一节课内容过于复杂：同时带乘法懒标记 `mul[]`、区间加懒标记 `lz[]` 与取模 `% p`，学生第一遍就接触不下 | 已修复 | `SegmentTreeBuildModule` 精简为纯区间求和版：只保留 `tr[]`（区间和）与 `lz[]`（区间加懒标记，建树阶段恒为 0）两行，删除 `mul[]` 行与全部 `% p` 运算；`push_up` 简化为 `tr[node] = tr[ls] + tr[rs]`，叶子简化为 `tr[node] = a[l]`；`SegmentTreeCommon::parseInput()` 改为只解析 `n` 与 `a[1..n]`，同时兼容旧版两段式表头；新增 `ActionType::SEG_INIT_LZ` 帧承载「本节点懒标记清零」语义 |
| BUG-091 | 树状数组可视化过于简陋：只有上下两排色块，看不出 `t[i]` 覆盖了 `a` 的哪一段 | 已修复 | `FenwickTreeModule` 移除原先的覆盖区间色块，改用真实几何连线：对每个 `t[i]`，以其格心为顶点、其覆盖区间 `[i-lowbit(i)+1, i]` 对应的 `a` 格为底边，绘制一个半透明填充三角形（`QGraphicsPolygonItem`）并向区间内每个 `a[j]` 拉出扇形辐射线；`n=8` 时形成「一个大三角里套多个小三角」的经典树状数组结构图 |
| BUG-092 | 五个自由结构沙盒模块在画布上铺满说明文字、标题与公式注释，遮蔽了数据本身 | 已修复 | `FreeStructureBase::showHint()` 改为空实现，`m_mathText` 全部不再创建；`SandboxArrayModule` / `SandboxMatrixModule` / `SandboxTreeModule` / `SandboxGraphModule` / `SandboxLatexModule` 移除各自的标题、提示与公式题注，画布只保留纯数据结构图元；`SandboxLatexModule` 改用 `LatexBlock::setTargetHeight()` 按目标高度排布公式矢量图，不再依赖失真的 `scaleFactor` |
| BUG-093 | 自由结构-图结构每行只支持 2 个元素，且图元自带默认权值，与老师预期的「每行 u v w」不符 | 已修复 | `SandboxGraphModule` 解析每行 2 或 3 个元素，写 3 个时该边带权 w，写 2 个时该边不带权（`has_weight=false`，画图不显示徽标），彻底取消任何默认权值注入；默认值更新为 `5\n1 2 3\n1 3 2\n2 4 5\n3 4 1\n4 5 4` |
| BUG-094 | 只有数组格支持双击修改，图结点与边权不能像数组一样就地编辑 | 已修复 | `IModule::enableLiveEditing()` 为未登记的 `GraphNode`（行 `"v"`、下标 = 结点 id）与 `GraphEdge`（行 `"w"`、下标 = `edge->key()`）自动补登记槽位并接入就地编辑器；`GraphEdge::beginEdit()` 放宽原先的 `has_weight` 前置条件，无权边双击后自动补上权值；`GraphNode::commitEdit()` 改写 `customValueText` 使新值直接显示在圆内，并抑制重复的 `v:N` 标签；`refreshLiveSlots()` 同步回填，保证重建后修改不丢失 |
| BUG-095 | 右下角帧进度条旋钮在最左端（x=0）被「吃掉」一半 | 已修复 | 根因是旋钮圆心落在控件边界上，左半圆与外圈辉光（半径 6px + 6px halo）超出 `QWidget::paintEvent` 的裁剪范围。`MySlider` 引入左右对称内边距 `kSidePadding = 14.0`，`trackRect()` 起点右移 14px、`handleCenterX(t)` 在 `[14, width-14]` 区间内插值、`valueAtX()` 同步按新行程换算；辉光绘制再按 `maxGlow` 逐环裁剪，杜绝任何越界像素 |
| BUG-096 | 算法模块下拉框塞进 37 个长条目，查找成本高、不符合人机交互原则 | 已修复 | `MyComboBox` 新增 `addGroup()` 分组模式与 `ComboTile` 按钮瓦片 / `ComboSectionHeader` 分节标题控件：点击下拉后展开为按「基础结构 / 数学 / 数据结构 / 算法」四类分组的 3 列按钮网格，每类内部按课程进度排序（早出现的算法在前）；浮层宽度下限提升到 520px、视口高度上限提升到 430px；`MainWindow` 依据分组重建 `registeredModules` 顺序，保证下拉索引与模块数组严格对齐 |
| BUG-097 | 堆结构动画逻辑混乱：结点圆内不显示值、序号与值混在一起；交换时只变色不真的换值；下方缺少完全二叉树数组与父子关系提示 | 已修复 | `HeapTreeBase` 重构：`layoutHeap()` 把结点值写入 `customValueText`（画在圆内）、把 1-based 序号写入 `subLabel`（画在圆外上方）；新增 `applyHeapValues()` / `swapHeapValues()` 在 `HG_SWAP` / `HS_SWAP` 帧真正互换 `ArrayBlock` 与 `GraphNode` 的值；新增 `highlightRelation()` 在比较帧高亮 `i/2`、`2i`、`2i+1` 三组父子关系（当前节点与对手为强调色，父与孩子为浅蓝），并在数组行下方实时显示关系说明文字 |
| BUG-098 | 模块下拉网格分组浮层的右侧与下侧残留一根"未闭合"的竖线/横线，像边框忘记关闭 | 已修复 | 根因是浮层阴影绘制在 `ComboPopupFrame` 上但被浮层自身矩形裁剪：多层圆角阴影的右下溢出部分被硬切，形成一条直角暗带。新增 `ComboPopupFrame::paintEvent()` 用「奇偶填充的外环-内环差集路径」逐层绘制柔和阴影（12 环、每环 alpha 递增、整体向下偏移 4px），阴影完全落在浮层内；同时把浮层外边距从 `12/10/12/12` 放宽到 `18/18/18/22`、宽度补偿从 +24 提到 +36，保证阴影有足够绘制空间；`ComboCard` 边框改为半透明 `QColor(214,222,236,200)` 圆角卡片，避免硬边 |
| BUG-099 | 树状数组仍是「上下两排 + 覆盖三角形」，看不出 `floor(log2(n))+1` 层的二进制分层结构 | 已修复 | `FenwickTreeModule` 重写为二叉分层格阵：底层是完整数组 `a[1..n]`，其上按 `ctz(i)`（即 `log2(lowbit(i))`）分层，第 `k` 层的 `t[i]` 块宽度 = `lowbit(i) * cellW`、水平跨度覆盖 `[i-lowbit(i)+1, i]`；每层左侧标注 `L0..Lk` 与 `lowbit` 说明，右侧标注该层覆盖区间；构建阶段改用自底向上的二叉合并：`t[i] = t[i-2^(k-1)] + a[i-2^(k-1)+1] + ... + a[i]`，新增 `ActionType::FT_MERGE` 帧承载「从兄弟块与底层数组格同时飞入并合成 `t[i]`」的动作 |
| BUG-100 | 模块下拉网格的按钮瓦片与分节标题观感陈旧，不够科技感；中文字体未统一 | 已修复 | `MyComboBox` 全面重绘：`ComboOption::paintTile()` 改为冷调渐变瓦片（浅蓝灰底 + 细边框 + hover 高亮发光边框 + 左侧强调条 + 选中态顶部高光条）；`ComboSectionHeader::paintEvent()` 改为彩色竖条 + 渐变文字 + 尾部渐隐短横线，去掉原先横贯整卡的分割线；全部字体改用 `UiTheme::uiFont()`（Microsoft YaHei UI） |
| BUG-101 | 模块显示名过长（如「二分图匹配 - 匈牙利算法」「线段树 - 区间乘 / 区间加（懒标记）」），下拉瓦片排版拥挤 | 已修复 | 统一精简 16 个模块的 `getModuleName()`：「匈牙利算法」「线段树 (lz标记)」「线段树 (建树)」「线段树 (区间查询)」「KMP (pi数组)」「KMP (匹配)」「Dijkstra 最短路」「完全背包」「01 背包」「堆调整 up/down」「LIS 最长递增子序列」「Tarjan 强连通分量」「并查集」「BFS 广度优先搜索」；同步更新 `tests/GroupAUiTest.cpp` 的目标名断言 |
| BUG-102 | 分组过粗，同一算法无法在多个分类下出现；模块下拉条目与代码结构耦合，无法跨分支复用 | 已修复 | `MainWindow` 引入 `std::vector<int> comboToModule`，把下拉条目序号映射到 `registeredModules` 的唯一实例下标，使同一模块可在多个分组中重复出现而不重复 new；分组细化为「自由结构 / 数学基础 / 线性结构 / 经典排序 / 堆结构 / 并查集 / 树状数组 / 线段树 / 图论算法 / 匹配与动态规划 / 字符串算法」；Kruskal 同时挂在「并查集」与「图论 · 最小生成树」两个分支下 |
| BUG-103 | 前缀和与差分两个模块存在大段重复样板（解析、布局、渲染、动画组），维护成本高 | 已修复 | 抽取公共基类 `src/modules/TwoRowArrayModule.h/.cpp`，把输入解析、双行布局构建、`PD_*` 帧渲染与「多源数字飞入目标格」的 `QParallelAnimationGroup` 动画收敛到基类；`PrefixSumModule` / `DifferenceModule` 只保留各自的查询语义、帧序列与实时编辑回调，代码量减少约 45% |
| BUG-104 | 全项目存在已废弃的死文件与未使用的公共辅助函数，编译体积与阅读成本虚高 | 已修复 | 删除 `src/modules/ArrayModule.h/.cpp`（无任何派生类引用）与 `src/modules/IModuleHelpers.h`；清理 `IModule` 中已无调用方的 `playFadeItem`/`createEdge`/`drawLabel`/`drawCaption`/`applyNodeHighlight`/`clearNodeHighlights`/`attachEdge`/`detachAllEdges`；清理 `GraphRenderHelper.h` 中未使用的 `addAxisLabel`/`addNote`；清理 `HeapTreeBase.h` 中从未写入的 `m_relationLabels` 成员；同步更新 `CMakeLists.txt` 与 `tests/CMakeLists.txt` |
| BUG-105 | 隐形对齐网格参考线从未真正绘制：画布上完全看不到 56px 点线网格，勾选「对齐网格」也无任何视觉反馈 | 已修复 | 根因是 `GridGuideLayer` 构造函数只调用了 `setZValue` 等标志位，**从未把自身加入场景**——`QGraphicsItem` 的构造函数不接受 `QGraphicsScene*` 参数（已用独立程序验证：该签名不存在），必须显式 `scene->addItem(this)`。但直接补 `addItem` 会引入**栈溢出崩溃**：该图元的 `boundingRect()` 返回 `scene()->itemsBoundingRect().adjusted(...)`，而 `itemsBoundingRect()` 会遍历所有图元并调用各自的 `boundingRect()`，形成自引用递归（独立程序复现，进程以 `-1073741571` 退出）。最终改为**场景级背景绘制**方案：新增 `GridGuideScene`（继承 `QGraphicsScene`，覆写 `drawBackground`）在背景层画网格点线，`MainWindow` 直接以 `GridGuideScene` 取代普通 `QGraphicsScene`；网格不再是场景图元，因此既不进入 `itemsBoundingRect()`（不影响 `ViewportFitter` 居中与缩放），也不会被 `clearScene()` 的 `scene->clear()` 清掉，更不存在递归风险。`SceneGrid::drawGuides()` 签名由 `(painter, scene, config)` 改为 `(painter, bounds, config)`，直接按传入矩形绘制；`SceneGrid::attach()` 与 `GridGuideLayer` 整个类被删除，`setConfig()` 改为直接刷新 `GridGuideScene` 视口 |
| BUG-106 | 算法模块下拉网格列表里的分类标题文字完全看不见（或与背景同色），分节信息丢失 | 已修复 | 根因是 `ComboSectionHeader::paintEvent()` 用 `QLinearGradient` 从 `m_accent.lighter(128)` 渐变到 `m_accent.lighter(150)`，在浅色浮层底上对比度不足，文字几乎融进背景。改为高对比深色渐变 `m_accent.darker(140)` → `m_accent.darker(110)`，同时保留彩色竖条与尾部渐隐短横线的视觉语言。`SmartGroundComboShot` 截图复核确认「基础结构 / 数学 / 排序 / 数据结构」等标题清晰可读 |
| BUG-107 | Release 版 exe 打包后运行弹出黑色控制台窗口，并漏出 `QPropertyAnimation: you're trying to animate a non-existing property ratio of your QObject` 与 `QString::arg: Argument missing: "失配：沿已知前缀回退", 2` 等内部告警 | 已修复 | 三处根因分别处理：① `MyCollapsible` 补上 `Q_PROPERTY(qreal ratio READ ratio WRITE setRatio)`，此前 `QPropertyAnimation(this, "ratio")` 指向了未声明的属性，Qt 直接判定为「不存在」并打印告警；② `KMPNextModule` / `KMPMatchModule` / `HeapSortModule` 中若干文案字符串没有 `%1` 占位符却仍调用 `.arg(...)`，Qt 会打印 `Argument missing`——已移除这些多余的 `.arg()` 调用，并把 `HungarianModule` 中「重复使用 `%1` 又传三个参数」的写法改成显式编号占位符（`%1/%2/%3`）；③ `CMakeLists.txt` 为 `SmartGround` 目标设置 `WIN32_EXECUTABLE TRUE`，Release 产物以 Windows GUI 子系统链接，不再附带控制台窗口 |
| BUG-108 | 双击修改 Dijkstra 图结点后，新值没有顶替旧值，而是在结点下方叠出一大坨 `v : 3` 重复文字 | 已修复 | 根因是 `GraphNode::paint()` 里同时存在两条绘制路径：`customValueText` 与 `valueText` 之一非空时绘制自定义文本，而 `value > 0` 的兜底分支又独立绘制 `v:%1`；`commitEdit()` 写入的正是 `customValueText`，于是旧值兜底与新值同时出现。修复为让兜底分支只在 `valueText.isEmpty() && customValueText.isEmpty()` 时才生效，并把 `commitEdit()` 统一走 `setValueText()`。`SmartGroundNodeLiveEdit` 回归断言「结点下方只有一行文本」已通过 |
| BUG-109 | 批注层与算法层分离后，批注图形在画布上完全不显示 | 已修复 | 根因是 `CanvasView::drawForeground()` 里 `painter->setClipRect(viewport()->rect())`——`drawForeground` 收到的 painter 已带视图变换，裁剪矩形用的是视口像素坐标，而批注场景坐标为负（场景原点在中心），两者坐标系不一致导致所有内容被裁掉。同时误用了 `render()` 的 source/target 语义。修复：去掉多余的 `setClipRect`，直接以场景坐标 `render(painter, src, src, Qt::IgnoreAspectRatio)`，由 `QGraphicsScene::render()` 自行完成视口映射；选框矩形改用 `painter` 现有变换绘制。`SmartGroundCanvasVisual` 像素级断言已覆盖 |
| BUG-110 | 批注层撤销/重做后，被擦除的图形莫名消失，撤销栈与图形池状态不一致 | 已修复 | 根因是 `ToolController::pushCommand()` 在清空重做栈时会把「Remove 类命令」持有的图元直接 `delete`，但这些图元此时可能已经通过撤销被重新挂回场景，导致悬垂指针与提前析构。改为由 `AnnotationScene` 统一持有全部批注图元所有权（`m_owned` 列表 + 析构时统一释放），`ToolController` 只负责在场景中挂载/卸载，不再持有或销毁任何图元；`AnnotationScene::removeItem(item, destroy)` 的 `destroy=false` 语义表示「从场景卸载但保留所有权」 |
| BUG-111 | 批注文本框与 Markdown 卡片的文字互相重叠、撑不开 | 已修复 | `QGraphicsTextItem` 未设置换行宽度时按内容自然排版，多行文本会横向溢出。`TextBoxItem::rebuildDocument()` 与 `MarkdownItem::rebuildDocument()` 补上 `setTextWidth(280.0 / 320.0)` + `adjustSize()`，卡片宽度固定、行高自适应，`QGraphicsTextItem` 随文档内容重新排版 |
| BUG-112 | 算法模块下拉浮层里的**分类标题文字完全看不见**（只看到一条彩色竖条） | 已修复 | 根因是 `ComboSectionHeader::paintEvent()` 用 `painter.setBrush(textGrad)` 设置渐变后再 `painter.setPen(Qt::NoPen)`，随后调用 `drawText()`——而 `QPainter::drawText()` **只使用画笔（pen）绘制文字，完全忽略画刷（brush）**，于是分类标题被以 `NoPen` 绘制，等于隐形。截图逐像素采样证实：标题行存在 `(59,125,221)` 的强调色竖条像素，但**深色文字像素数为 0**。修复：改为用 `painter.setPen(深色标题色)` 绘制文字，竖条与尾部装饰线继续用画刷绘制；若强调色过浅则回退到 `UiTheme::textPrimary()` 保证对比度。`SmartGroundToolSuite` 现以「强调色像素 > 40 且深色文字像素 > 200」双重断言守护 |
| BUG-113 | 画布工具条把 14 个文字按钮 + 5 个滑块 + 色块平铺进主窗口布局，应用宽度被撑到 1920px 屏外 | 已修复 | 根因是 `CanvasToolBar` 被 `rightLayout->addWidget(...)` 放进窗口主布局，`FlowLayout` 按自然宽度一行铺开全部控件。重构为**浮动轻套件**：`CanvasToolBar` 改为以 `CanvasView` 为父级的透明覆盖层（不再占用布局尺寸），主条只保留 10 个 34px 矢量图标按钮 + 样式/更多两个入口，总宽 451px、高 50px，顶端居中悬浮；颜色/粗细/不透明度/文字样式归入「样式」浮层，文本框/笔记/公式/表格/表格行列/撤销重做/批注显隐锁定/导出清空归入「更多」浮层，二者互斥展开、按宿主尺寸自动避让边界。矢量图标由新增 `src/ui/widgets/ToolIconButton.cpp` 手工绘制（不依赖 Segoe UI Symbol 字形，避免缺字方框）。窗口自然宽度由 >1920px 回落到 **694px**（`minimumSizeHint` 558px） |

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
  - `SmartGroundGroupAUi`：14 项断言，在真实 `MainWindow` 中验证 A 组六个模块全部出现在模块下拉框中、逐个可被选中并正常切换，且模块总数为 38（37 个唯一模块 + Kruskal 在「图论 · 最小生成树」分支下的跨分支别名）。
  - `SmartGroundGroupA`（更新）：树状数组断言改为校验 `FT_MERGE` 分层合并帧：合并帧覆盖全部 5 个节点，且 `t[2] = c1 + a2 = 4`、`t[4] = c3 + a3 + a4 = 10`。
  - `SmartGroundVerifyShot`：24 项断言，覆盖本轮八项调整。BUG-090：线段树-建树同时渲染 `tr` 与 `lz` 两行，且帧序列含 `SEG_LEAF` 与 `SEG_PUSHUP`、公式文本中无 `%` 取模残留；BUG-091：树状数组场景中存在三角形多边形与辐射连线；BUG-092：五个自由结构沙盒模块画布上的 `QGraphicsTextItem` 数量为 0；BUG-093：3 元素行产生 5 条带权边、2 元素行产生 0 条带权边（无默认权值注入）；BUG-097：堆的 5 个结点值在圆内、5 个序号在圆外、下方完整二叉树数组块数量为 5，且交换帧后结点与数组块的值确实互换；BUG-095：滑块在最左端时旋钮完整绘出（暗色像素计数达标），点击最左端映射到最小值。
  - `SmartGroundFenwickShot`：把树状数组分层格阵渲染成 `fenwick_lattice.png`，供人工核对底层数组与上层 `L0..Lk` 各层的对位关系。
  - `SmartGroundComboGroupShot`：把带分组的模块下拉浮层渲染成 `combo_groups.png`，供人工核对瓦片渐变、彩色标题与浮层阴影边缘。
  - `SmartGroundGridGuideTest`：9 项断言，覆盖 BUG-105（隐形对齐网格从未绘制）。验证 `GridGuideScene` 启用网格后画布上出现网格像素（黑底上非黑像素 4864 > 300）、关闭后像素归零（4864 → 0）、网格**不进入场景图元集合**（`scene->items()` 为空，故不影响 `ViewportFitter` 的 `itemsBoundingRect()` 居中计算）、`scene->clear()` 后网格依然存活（重建场景不丢网格）、以及真实 `MainWindow` 中画布确实呈现网格参考线。
  - `SmartGroundCanvasTools`：36 项断言，覆盖批注层核心能力。画笔/直线/方框/圆形/箭头/三角/马赛克七种工具各自创建出对应图元类型；橡皮只擦除上层批注且撤销栈同步入栈；撤销→重做→再撤销三态往返后图元数量精确复原（覆盖 BUG-110）；文本框、Markdown、LaTeX、表格四种卡片的创建、尺寸、内容与默认值（表格单元格默认为空字符串）；JSON 序列化往返后图元数量与类型完全一致；批量显隐与批量锁定覆盖全部图元；框选多选、全选、整体缩放、删除与删除后撤销均行为正确。
  - `SmartGroundCanvasVisual`：6 项断言 + 截图 `canvas_visual.png`，像素级验证批注层真的绘制到画布上（BUG-109）：十种图元同时可见，红色彩笔画、矩形、圆形、箭头、三角形、马赛克、文本框、Markdown 卡片（含代码块）与表格网格均正确渲染。
  - `SmartGroundToolSuite`：11 项断言 + 4 张截图（`tool_suite_combo.png` / `tool_suite_canvas.png` / `tool_suite_style.png` / `tool_suite_more.png`），覆盖 BUG-112 与 BUG-113。BUG-112：下拉浮层同时存在强调色竖条像素与深色标题文字像素（修复前深色文字像素为 0）。BUG-113：浮动工具套件主条宽度 451px、单行高度 50px（均低于阈值 520px / 80px），样式浮层与更多浮层可正常开合。
  - `SmartGroundAppWidth`：6 项断言 + 3 张截图（`app_window.png` / `app_style_open.png` / `app_more_open.png`），覆盖 BUG-113。真实 `MainWindow` 自然宽度 694px、最小宽度 558px，均远低于 1920px 目标；工具套件确认为 `CanvasView` 的子级（真正浮在画布之上）；两个浮层在真实窗口中可正常开合。
  - `SmartGroundAppCombo`：在真实 `MainWindow` 中打开模块下拉浮层并截图 `app_combo_popup.png`，统计标题带的深色文字像素（14276 > 300），确认 11 个分类标题（自由结构 / 数学基础 / 线性结构 / 经典排序 / 堆结构 / 并查集 / 树状数组 / 线段树 / 图论算法 / 匹配与动态规划 / 字符串算法）在真实应用中全部可见（BUG-112 端到端回归）。
- 本轮全量回归：42 个测试套件串行执行完毕，全部零失败、零崩溃、零断言告警；`offscreen`（无头）平台下全绿。
- 截图辅助程序：`SmartGroundArrowShot`、`SmartGroundDsuShot`、`SmartGroundUiShot`、`SmartGroundToggleShot`、`SmartGroundFenwickShot`、`SmartGroundComboGroupShot`，用于人工核对箭头、权值、滑动开关两种状态、树状数组分层格阵与下拉分组网格观感。
- 项目目录没有 Git 元数据，无法附上提交或工作树引用。

> 说明：2026-11-14 起项目已纳入 `D:\Program\Cpp` 的 Git 仓库，SmartGround 目录位于 `Project/QT/SmartGround` 子路径下，提交与工作树引用可正常提供。

## BUG-114 画笔笔刷锯齿严重（折线未做样条平滑）

- **现象**：画笔笔迹呈明显阶梯状折线，快速移动鼠标时棱角尖锐，手感粗糙。
- **根因**：`StrokeItem::buildPath()` 直接用 `QPainterPath::lineTo()` 连接原始鼠标采样点，得到的是折线而非平滑曲线；采样点越稀疏（鼠标移动越快），棱角越明显。
- **修复**：
  - `StrokeItem::smoothPoints()` 改为 **Catmull-Rom → 三次贝塞尔** 插值：每段以 `p0..p3` 求控制点 `c1 = p1 + (p2-p0)/6`、`c2 = p2 - (p3-p1)/6`，每段细分 12 步采样，彻底消除折角。
  - `StrokeItem::paint()` 改为**变宽丝带（ribbon）渲染**：沿平滑路径逐点求切线法向，向两侧偏移半个笔宽生成左右轮廓，再合拢成闭合多边形填充；笔画边缘随曲线走向自然过渡，不再有锯齿。
  - 最小采样间距 `kMinSampleDistance = 0.9`，剔除抖动采样点。
  - 荧光笔分支保留 `FlatCap + RoundJoin` 的马克笔笔头形状，宽度为普通笔的 2.4 倍。
- **验证**：`SmartGroundBrushQuality` 断言「水平笔画跨越 192 列」「笔画垂直厚度恒定在 3~12px 之间」。

## BUG-115 画笔/橡皮批注不实时刷新（架构性缺陷）

- **现象**：画完一笔、擦除一块后，画布内容**不立刻出现**，必须缩放、缩小或平移视图才刷新。
- **根因**：批注层是独立的 `AnnotationScene`，**从未挂接到任何 `QGraphicsView`**。因此 `StrokeItem::update()`、`AnnotationScene::removeItem()` 触发的都是「无人观看的场景」自身的重绘调度，`CanvasView::drawForeground()` 根本不会被调用。只有缩放/平移会触发 `CanvasView` 整体重绘，所以画面才「姗姗来迟」。
- **修复**：
  - `AnnotationScene` 新增 `setHostView()` / `hostView()`，记录承载它的画布视图。
  - `AnnotationScene::addItem()` / `removeItem()` / `destroyItem()` 在变更后立即调用 `m_hostView->viewport()->update()`。
  - `AnnotationItem` 新增 `contentChanged()` 信号；`AnnotationScene::addItem()` 为每个图元连接该信号，**笔画在绘制过程中每加一个点就实时通知画布重绘**（此前只在松手时才更新）。
  - `CanvasView::setAnnotationScene()` 自动调用 `m_annotationScene->setHostView(this)` 完成挂接。
  - `ToolController` 的 `updateStroke` / `finishStroke` / `updateShape` / `finishShape` / `updateEraseDrag` / `finishEraseDrag` 均显式调用 `m_view->refreshOverlay()`。
- **验证**：`SmartGroundBrushQuality` 断言「按下鼠标尚未松开即已可见」（629 像素）、「拖动时笔画实时增长」（629 → 1819）、「擦除无需缩放即刻生效」（3149 → 128）、「撤销后笔画实时复原」。

## BUG-116 橡皮只响应单击，拖动无效且无光标指示

- **现象**：橡皮必须精确点中笔画才能擦除，鼠标按住拖动毫无反应；也没有任何光标提示，擦除范围不可见。
- **根因**：`ToolController::onMousePress()` 里橡皮直接调用 `eraseAt(pos)` 一次性判定；`onMouseMove()` 中橡皮分支缺失，拖动被完全忽略。且 `eraseAt()` 的命中判定使用 `shape().contains()`，而平滑后的曲线偏离原始采样点，命中容差为零。
- **修复**：
  - 新增 `beginEraseDrag()` / `updateEraseDrag()` / `finishEraseDrag()`，橡皮改为**按住拖动连续擦除**；拖动时按 `eraserRadius * 0.4` 步长沿路径插值采样，逐段擦除。
  - `AnnotationItem::hitTest(scenePos, radius)` 统一命中判定；`StrokeItem` 覆写为「样条轮廓命中 **或** 与任一原始采样点的距离 ≤ 半径 + 半笔宽」，兼顾平滑偏差与细笔画。
  - 橡皮尺寸独立为 `m_eraserRadius`（默认 14），样式浮层新增「橡皮」滑块。
  - `CanvasView::drawForeground()` 绘制**橡皮光标圆环**（红色描边 + 半透明填充），实时跟随鼠标，直观显示擦除范围。
- **验证**：`SmartGroundBrushQuality` 断言「拖动橡皮可整笔擦除」。

## 画笔 ZoomIt 风格增强

参照 Sysinternals ZoomIt 的交互范式，为批注层补齐以下能力：

- **荧光笔（Highlighter）**：新增工具，快捷键 `H`；半透明（alpha 0.38）马克笔笔头，叠加处不加深，与 ZoomIt 的高亮笔行为一致。
- **滚轮调笔宽**：画笔 / 荧光笔激活时，鼠标滚轮实时增减笔宽（0.5px 步进，范围 1~40px）；橡皮激活时滚轮调整橡皮半径（2px 步进）。这是 ZoomIt 最具辨识度的操作手感。
- **数字键换色**：`1`~`7` 快速切换 红 / 绿 / 蓝 / 黄 / 橙 / 粉 / 深灰（与 ZoomIt 的颜色快捷键一致）；`Alt + R/G/B/Y/O/P/K` 为备用组合。
- **实时重绘**：见 BUG-115，笔画绘制过程中即时上屏。

## BUG-117 笔刷快速移动断裂 + 笔迹自交处被抹成白色

- **现象**：
  1. 鼠标移动过快时，笔画中间出现明显断裂（多个独立小段）。
  2. 笔迹回绕或与自身路径重叠时，重叠区域变成白色，画布上出现"空洞"。
- **根因**：
  1. 鼠标采样点稀疏且不均匀，直接送入均匀 Catmull-Rom 样条会产生巨大的切向量，导致曲线过冲、翻折甚至自交。
  2. 笔画以「左边界正向 + 右边界反向」的闭合多边形（ribbon）填充绘制，而 `QPainterPath` 默认 `Qt::OddEvenFill`。笔迹自交处两侧被判定为内部重叠，奇偶校验为偶数，于是被挖成空洞，透出白色背景。
- **修复**：
  - `StrokeItem::resampledPoints()`：先按 2.5px 间距对原始采样点做**线性重采样**，保证控制点均匀分布。
  - 样条求值改为逐段求三次贝塞尔（每段 8 步细分），再拼成中心线路径。
  - 绘制改用 `QPen` 直接描中心线（`RoundCap` + `RoundJoin` + 抗锯齿），彻底放弃自交 ribbon 多边形，从根本上消除白色空洞。
- **验证**：`SmartGroundCanvasPolish` 断言「1000px 快速划线后断裂列数为 0」、「自交笔画核心 ±3px 内白色像素为 0」。

## BUG-118 形状拖动期间无实时预览

- **现象**：画直线 / 矩形 / 圆时，鼠标未松开前画布上看不到任何预览，必须松开鼠标，或者缩放 / 平移窗口后形状才突然显示。
- **根因**：`ShapeItem` 的 `setGeometry()` / `setMosaicCells()` / `setKind()` 只修改内部数据并 `update()`。但 `AnnotationScene` 是挂在 `CanvasView` 上的**分离 overlay 场景**（基础 `GridGuideScene` 才是真正 setScene 的对象），对 overlay item 调用 `update()` 只会弄脏 `AnnotationScene` 本身，不会触发 `CanvasView::viewport()->update()`。
- **修复**：
  - `ShapeItem` 所有几何/内容 setter 统一调用 `notifyContentChanged()`，由 `AnnotationItem` 发出 `contentChanged()` 信号，`AnnotationScene` 转发至宿主视图 `m_hostView->viewport()->update()`。
  - `ToolController` 的形状拖动过程中调用 `m_view->refreshOverlay()`，保证拖动期间实时可见。
- **验证**：`SmartGroundCanvasPolish` 断言「鼠标未松开时画布已出现 >12000 个形状像素」。

## BUG-119 马赛克只呈现为几条竖线

- **现象**：马赛克工具画出来是几条深色竖线，完全没有"像素化"效果。
- **根因**：旧实现用多边形网格模拟马赛克，网格线绘制成了实心竖条，且没有采样底层画布的真实颜色。
- **修复**：
  - `CanvasView::captureSceneRegion()`：把底层算法场景（不含批注层）渲染到 `QPixmap` 快照。
  - `ToolController::captureMosaic()`：将快照降采样为 `cols × rows` 网格（`cell` 像素一块），再以块状方式放大，得到真正的像素化马赛克。
  - `ShapeItem` 新增 `setMosaicImage()`，马赛克绘制时优先渲染像素化位图，位图缺失时回退到网格多边形。
  - 序列化：马赛克位图以 base64 PNG（`mosaicPng`）写入 JSON，并记录 `mosaicRx/mosaicRy/mosaicRw/mosaicRh`，撤销 / 重做 / 归档后仍可还原。
- **验证**：`SmartGroundCanvasPolish` 断言「马赛克位图非空」且「块内像素颜色与底层场景主色一致」。

## BUG-120 双击文本框会新建第二个文本框

- **现象**：插入文本框后工具仍停留在文本框模式，双击已有文本框会再叠一个新文本框，非常难用。
- **根因**：`ToolController::createItemForTool()` 创建完 item 后没有复位工具；且鼠标双击事件被 `onMousePress()` 当作普通按下处理。
- **修复**：
  - `createItemForTool()` 创建完成后立即 `setTool(AnnotationSelect)`，并对文本框 / 笔记调用 `beginInlineEdit()` 进入**画布内直接编辑**。
  - `ToolController::eventFilter()` 新增 `QEvent::MouseButtonDblClick` 分支：命中已存在的文本框 / 笔记时切换其行内编辑态，**不再新建元素**。
  - LaTeX 双击改为非模态 `AnnotationEditor`（`setLiveMode(true)`），避免模态对话框在自动化测试中死锁。
- **验证**：`SmartGroundCanvasEditing` 断言「插入后工具回落为选择」、「双击不新增元素且切换行内编辑」。

## BUG-121 LaTeX 公式无法编辑 / 工具属性被藏在右侧

- **现象**：插入 LaTeX 只能得到固定的 `E = mc^2`，没有编辑框；笔刷颜色 / 宽度等属性只能去右侧浮层找，交互反直觉。
- **根因**：`LatexItem` 插入时写死默认公式；工具栏所有按钮一视同仁，点一下直接切工具，没有"再点一次展开详细设置"的两段式交互。
- **修复**：
  - `ToolController::openLatexEditor()`：插入 LaTeX 时打开**非模态** `AnnotationEditor`，左侧源码编辑、右侧 `QSvgRenderer` 实时渲染预览，源码改动即时写回画布卡片。
  - `CanvasToolBar` 改为两段式交互：按钮点击时若该工具已是当前工具，则切换 `m_toolPanel`（`QStackedWidget`）中对应工具的独立属性页；否则仅切换工具并收起浮层。
  - 全部 15 个工具统一进入主工具条，属性页覆盖：画笔 / 橡皮 / 形状 / 马赛克 / 表格 / 文本 / 笔记 / LaTeX。
- **验证**：`SmartGroundToolSuite` 断言「首次点击切工具不弹浮层」、「再次点击弹出该工具属性页」。

## 图标规范化（BUG-122）

- **现象**：画笔 / 橡皮 / 文本框图标抽象难辨。
- **修复**：`ToolIconButton` 重绘为通用 UI 符号：
  - 画笔 → **铅笔**（斜置笔身 + 笔尖 + 笔芯）。
  - 橡皮 → **传统斜置橡皮块**（带分隔带的矩形）。
  - 文本框 → **方框内一个 "T"**。
  - 马赛克 → **带白边棋盘格色块**（不再与竖线混淆）。
  - 按钮尺寸由 34×34 收紧至 30×30，15 个工具一行排布不超出 1920 屏幕。

## 批注剪贴板（Ctrl+X / Ctrl+C / Ctrl+V）

- `ToolController::copySelection()` / `cutSelection()` / `pasteAt()` / `pasteAtCursor()`：依赖各 `AnnotationItem` 子类的 `cloneItem()` 虚函数复制。
- 粘贴时按当前鼠标的**场景坐标**落位，并注册 undo 命令，`Ctrl+Z` / `Ctrl+Y` 可回退。
- **验证**：`SmartGroundCanvasPolish` 断言「复制后粘贴元素数量 +1」「剪切后数量 -1」「撤销 / 重做数量恢复正确」。

## 模块切换批注归档（第 4 批联动）

- **问题**：切换算法模块会清空画布，批注层的笔记、公式、手绘内容一并丢失。
- **修复**：
  - `AnnotationScene::stashTo(bucket)` / `restoreFrom(bucket)`：以 `QHash<QString, QJsonObject>` 按模块名归档批注 JSON。
  - `MainWindow::archiveAnnotations()` / `restoreAnnotations()`：在 `onModuleChanged()` 中于切模块前归档当前模块批注，切模块后恢复目标模块批注。
- **验证**：`SmartGroundCanvasEditing` 断言「归档后画布清空」、「恢复后元素数量一致」、「恢复不存在的模块为空操作」。

## BUG-123 文本框 / Markdown 无法编辑、背景全白

- **现象**：插入文本框后光标没有激活，键盘输入无效；双击已有文本框也无法改字；文本框与 Markdown 笔记都有一块不透明的白底卡片。
- **根因**：`AnnotationScene` 是**游离场景**（detached scene），没有挂到 `QGraphicsView` 上，`QGraphicsTextItem` 拿不到原生窗口/场景焦点，因此键盘事件根本不会落到文档里；`TextBoxItem::paint()` / `MarkdownItem::paint()` 额外画了不透明底色。
- **修复**：
  - 键盘事件改为在 `ToolController::eventFilter` 中拦截 `CanvasView::viewport()` 的 `QEvent::KeyPress`，通过 `QTextCursor` 直接改文档并手动 `update()`，彻底绕开焦点问题。
  - `TextBoxItem` / `MarkdownItem` 移除任何不透明底色绘制，改为只画虚线框；编辑态自绘光标（`QTextLayout::lineForTextPosition` + `cursorToX` 定位），不依赖原生文本光标。
  - `ToolController::createItemForTool()` 修正顺序：先 `setTool(AnnotationSelect)` 再 `beginInlineEdit(item)`，避免 `setTool` 内部的 `endInlineEdit(true)` 把刚进入的编辑态关掉。
  - `ToolController::onKeyPress()` 在行内编辑态下优先把按键交给 `m_editingItem->handleEditKey()`。
- **验证**：`SmartGroundSmartCanvas` 断言「插入后立刻进入行内编辑」「输入的 `Hi` 落到文本框」「`paint()` 输出无白色不透明像素」「Markdown 输入落到笔记」。

## BUG-124 画笔拐弯有棱角、鼠标跟随有延迟

- **现象**：画笔 / 荧光笔急转弯时出现明显折角；快速划动时笔迹"追不上"鼠标。
- **根因**：路径用**均匀** Catmull-Rom 样条，控制点间距不均或转角 > 90° 时会产生尖点（cusp）与过冲；同时鼠标坐标被 `event->position().toPoint()` 截断到整像素，丢掉了亚像素信息。
- **修复**：
  - `StrokeItem` 改用**向心 Catmull-Rom**（`alpha = 0.5`）三次贝塞尔转换，节点间距按 `‖P(i+1)-P(i)‖^0.5` 加权，急转弯不再产生尖点与自交。
  - 采样间距由 3.0 收紧到 2.6，最小采样距离由 0.4 收紧到 0.35，曲率大的地方自动加密。
  - 新增 `CanvasView::mapToSceneF()`：用 `viewportTransform().inverted()` 保留亚像素精度，`ToolController` 全部鼠标坐标改走该接口，消除整像素台阶造成的"顿挫感"。
  - `StrokeItem` 改为增量重建（`m_resampled` 缓存 + `rebuildTail()`），落笔后每次只追加末段，避免整条路径反复重算。
- **验证**：`SmartGroundSmartCanvas` 断言「笔迹路径元素数 > 5（确为三次样条而非折线）」。

## BUG-125 框选后无法拖动 / 无法缩放

- **现象**：矩形框选元素后按左键拖不动；选框上也没有缩放箭头，无法拉伸。
- **根因**：`AnnotationItem::scaleAbout()` 依赖 `QGraphicsItem::setScale()`，该接口只接受**单个标量**，非等比缩放根本无法表达（源码里写成了 `setScale(sx, sy)`，直接编译不过）；缩放过程中每帧都以当前状态为基准再次累乘，导致拖拽越缩越飘。
- **修复**：
  - 新增 `AnnotationFrame`（origin + basisX + basisY）抽象，把位置与仿射基向量分开保存，`AnnotationItem::captureFrame()` / `applyFrame()` / `scaleFrameAbout()` 基于它做**以起始帧为基准**的绝对缩放，杜绝累乘漂移。
  - 等比缩放时走 `setScale(uniform)`，保持 `QGraphicsItem::scale()` 语义正确（既有测试依赖该属性）。
  - 新增 `SelectionOverlay`：8 个方向手柄（4 角 + 4 边中点），命中容差按视口缩放反向换算 `tol = 9.0 / max(0.2, zoom)`；拖动时按手柄类型计算相对起点的 `sx/sy` 与对角锚点。
  - `ToolController::onMousePress()` 支持在**选框内部空白处**按下即整体拖动（`pointInsideSelection()`），不再要求必须点中元素本体。
  - 手柄命中放宽到多选场景（原来仅 `m_selected.size() == 1` 才生效）。
  - `TopLeft` / `BottomLeft` / `MiddleLeft` 的 `sx` 计算方向此前写反（导致向左拖动反而缩小），已修正为 `(b.right() - scenePos.x()) / b.width()`。
- **验证**：`SmartGroundSmartCanvas` 断言「拖动后元素位置变化」「角手柄可命中」「拖动角手柄后选框宽度增大」；`SmartGroundCanvasTools` 断言「`scaleSelected()` 后 `item->scale()` 变大」。

## BUG-126 表格缺少上下文工具栏与增删行列

- **现象**：插入表格后没有悬浮工具栏，无法居中 / 左中 / 右中 / 顶端对齐，也无法选中单元格、行、列，更不能增删行列。
- **根因**：表格只实现了数据模型，没有任何 UI 入口；`ToolController::handleTableClick()` 又依赖 `m_activeTable` 预先赋值，导致点击其它表格时无响应。
- **修复**：
  - 新增 `TableToolBar`（自绘 `TableCard`，非原生控件）：居中 / 左中 / 右中 / 顶端对齐、选中单元格 / 行 / 列、增行 / 列、删行 / 列。
  - `MainWindow::positionTableToolBar()` 把工具栏浮动定位在表格上方，表格移动 / 改尺寸后自动重定位。
  - `ToolController::handleTableClick()` 改为按点击位置动态解析目标表格并 `emit tableActivated()`；单元格命中改用 `headerHeight()` 偏移，标题行不再误判为第 0 行。
  - 右键表格弹出 `tableContextRequested` → `TableToolBar::showContextMenu()`，提供上 / 下 / 左 / 右插入行 / 列、删除当前行 / 列。
  - 「插入 k 行 / k 列」使用 `MyTextInput` 输入数量（自绘控件，未引入原生 `QTextEdit`）。
- **验证**：`SmartGroundSmartCanvas` 断言「单元格对齐被记录」「插入行 / 列后行列数增长」「删除行 / 列生效」「尺寸回落正确」。

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
