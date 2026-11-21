# SmartGround Bug Log

**更新时间：2026-11-21**
| ID | 问题 | 状态 | 修复记录 |
|---|---|---|---|
| BUG-241 | **笛卡尔坐标系第三轮九项实测问题**：① 拖动已有点仍被硬量化到格点（描点可以落非格点，拖点却不行）；② 右键平移方向上下相反（鼠标往下拖画面反而往下走）；③ 拖动有连线的点时线段留下残影；④ 左侧对象列表字号过小且无法双击编辑坐标；⑤ `Ctrl+A` / `Ctrl+C` / `Backspace` 快捷键全部失效；⑥ 删除点后新点命名不复用被释放的名字；⑦ 线段删除后右侧信息卡一直挂着，点击任意空白也不消失；⑧ 函数输入框仍占据左侧面板、且「函数」工具按钮实际只是「瞄点」；⑨ 对象过多时列表没有滚动条 | 已修复 | ① **拖点吸附**：`CartesianEngine::snapMath()` 的硬量化只在**描点落地**时使用；拖动路径 `ToolController::updateCartesianPoint()` 改走 `snappedMath()`（接近吸附），容差 `snapTolerance() = m_snapRatio * m_step`（`m_snapRatio = 0.28`）**相对分度尺**计算，因此任意缩放级别下都能在格子内部自由落点，靠近格点（≤ 0.28 分度尺）仍会吸附到格点。② **平移方向**：`ToolController` 与 `CanvasView` 的右键/中键平移统一改为 `originScene() - delta`（鼠标往下 → `originY` 减小 → 画面向上）。③ **残影**：`CartesianPointItem::setMathPos()` 内补调 `m_engine->refreshSegmentsFor(this)`，对两端含该点的线段执行 `prepareGeometryChange()` + 重算路径 + `update()`，拖动过程中线段逐帧跟随。④ **列表字号 + 双击编辑**：新增设计令牌 `cartesianObjectName`（13px / 600 字重）与 `cartesianObjectValue`（12px 等宽字体），替代原先被全局 `hintLabel`（9px）样式压制的字号；`CartesianPanel::editPointCoordinates()` 在双击点行时弹出坐标编辑框，回车后发 `pointEdited` → 点位与所连线段立即按新坐标重绘。⑤ **快捷键**：`MainWindow::keyPressEvent()` 在 `m_cartesianActive` 时把 `Ctrl+A` / `Ctrl+C` / `Ctrl+X` / `Ctrl+V` / `Backspace` / `Delete` 路由到 `ToolController` 的 `cartesianSelectAll` / `cartesianCopySelection` / `cartesianCutSelection` / `cartesianPasteAtCursor` / `cartesianDeleteSelection`。⑥ **点名回收**：`CartesianEngine::pointNamePool()` 生成 `A..Z` → `Aa..Zz` 的**有序**候选池（不再 `sort()`，否则 `Aa` 会排到 `B` 前面），逐个跳过已占用名，天然取到**字典序最小的被释放单字母**（保留 `A B E G` 时新点依次为 `C D F`）。⑦ **信息卡消失**：`ToolController::cartesianBackgroundClicked` 与 `CartesianEngine::objectsChanged` 分别接到 `MainWindow::dismissCartesianInfo()` / `syncCartesianInfoTarget()`，点击画布空白或删除对象即刻收起右侧检视卡。**附带修掉一个索引漂移缺陷**：原实现按**下标**判断被检视对象是否存活，删除**兄弟**线段后下标整体前移，会把「仍然存在」的线段误判为已删除而误收卡片 —— 改为用 `QPointer<QObject> m_cartesianInfoObject` **按对象身份**跟踪，存活判据改为「该指针仍在 `engine->points()/segments()/curves()` 中」。⑧ **函数上移**：`CanvasToolBar::createCartesianPage(CartFunction)` 内嵌 `MyTextInput`（objectName = `canvasCartFunctionInput`）+ 解析反馈标签；点击「函数」按钮只是展开该下拉页，**不切换绘图工具**；左侧 `CartesianPanel` 移除函数输入段，整块空间让给对象列表。⑨ **滚动条**：`CartesianPanel` 新增 `m_listScroll`（`QScrollArea`）承载全部对象行，对象过多时出现垂直滚动条 |
| BUG-240 | **笛卡尔坐标系第二轮七项问题的验证方法存在盲区**：上一轮的测试用例直接调用引擎 API（`engine->addPoint()` / `engine->pointDoubleClicked()`）来断言行为，**绕过了真实的鼠标事件链路**（`QGraphicsView` → `viewport` 事件过滤器 → `ToolController` → 引擎）。这意味着「描点是否真的响应画布点击」「双击是否真的弹检视面板」「点击文字标签是否真的选中工具」这些**用户实际动作**从未被验证过 | 已修复 | **方法论缺陷，不是功能缺陷** —— 补写 `tests/CartesianRound3Test.cpp`（`SmartGroundCartesianRound3`），全部改用**真实鼠标事件**驱动：① 用 `QMouseEvent` 向 `view->viewport()` 投递 press/release，验证「画布单击 → 落点 → 左侧列表新增一行且该行确实可见」；② 在数学坐标 `(0.5, 0.8)` 处真实单击，验证落点坐标**保持自由值**且 `isSnapped()` 为 false；③ 在交点 `(5, 4)` 处真实单击，验证落点**精确吸附**且 `isSnapped()` 为 true；④ 向按钮**文字区域**（`width()-6` 处）投递真实点击，验证工具被真正切换；⑤ 真实右键按下 + 移动 + 释放，验证原点位移；⑥ 真实点击两个点后再切连线工具真实点击两点，验证**生成线段且画布上不绘制方程**；⑦ 真实双击点 / 线段，验证**右侧检视面板弹出、坐标与方程正确、且面板锚定在画布右半区且不出界**。实测七项全部通过 —— 证实上轮修复真实有效。**无限性判据同时升级为相对比较**：原点处网格像素 63327、平移 126000 单位后 61716（密度一致），而非原来的绝对阈值 |
| BUG-239 | **笛卡尔坐标系第二轮七项实测问题**：① 描点后左侧列表不更新；② 连线后直接显示一次函数；③ 描点只能落在坐标交点上（无法落 `(0.5, 0.8)`）；④ 工具按钮只有图标可点、文字点不动；⑤ 描点模式下右键不能拖动画布；⑥ 坐标系延伸到 60 就结束、不是无限；⑦ 双击描点后右侧不弹出信息框 | 已修复 | ① **初始化顺序**：`setupUI()` 里的 `setupCanvasTools()` 早于模块 new 信号连接从未建立 —— 抽出 `MainWindow::setupCartesianWiring()` 在模块注册后调用；`CartesianPointItem::setMathPos()` 补发 `pointMoved` 让拖点也刷新列表。② `Settings::showEquation` 默认改 `false`，方程改由 `CartesianSegmentItem::equationText()` 在检视面板按需索取。③ `snapMath()` 的硬量化拆成 `snappedMath()` **接近吸附**（距离 ≤ 0.28 分度尺才吸附，否则保留自由坐标）+ `isSnapped()`。④ `ToolIconButton` 增加 `setCaption()`，文字**画在按钮内部**并参与尺寸计算，图标与文字成为同一块可点击区域。⑤ `ToolController` 新增 `m_cartesianPanning`，画点 / 连线模式下右键拖动调用 `setOriginScene()` 平移原点。⑥ `ViewportFitter` 新增 `setInfiniteCanvas()`（±1e7 场景矩形），`CartesianGridItem::boundingRect()` 改大方框、`paint()` 改用 `option->exposedRect` **按当前视口动态画网格**。⑦ 新增 `src/ui/widgets/CartesianInfoPanel.{h,cpp}` 右侧浮层检视卡，`CartesianModule` 拆分「检视」与「编辑」两组信号 |
| BUG-238 | **`PointerBlock` 用 `Qt::UniqueConnection` 连接 lambda 导致进程崩溃**：26 个测试可执行文件以退出码 `-1073740286`（fail-fast）挂掉 | 已修复 | `Qt::UniqueConnection` **只支持指向成员函数的槽**，传 lambda 会触发 `QObject::connect` 断言「Unique connection requires the slot to be a pointer to a member function of a QObject subclass」，Qt 6.11 将其升级为致命断言。修复：`PointerBlock` 新增 `m_anchorConn`（`QMetaObject::Connection`）与 `disconnectAnchor()`，改用不带 `UniqueConnection` 的 lambda 连接并在重新锚定时显式断开，避免连接泄漏与重复触发 |
| BUG-234 | **二次函数 `f(x) = 3x^2 - 8x + 7` 整条曲线消失**：曲线路径一旦有任何一段超出范围，就把整条折线 `clear()` 掉，导致本该可见的部分也被抹掉 | 已修复 | 根因在 `CartesianCurveItem::rebuildPolyline()`：出界分支执行 `m_polyline.clear()`，把**已经累积的全部顶点**清空，而抛物线在可视区间内必然先下探后回升（顶点低于视口下沿），于是整条曲线被判定为不可见。修复：把单条 `QPolygonF m_polyline` 改为 **`QVector<QPolygonF> m_runs` 多段结构** —— 出界时只 `flush()` 当前段（切断）而**不清空历史段**，恢复后另起新段；`paint()` 遍历绘制所有段。同步把可视范围扩展为 `setVisibleRange(minX, maxX, minY, maxY)`（由 `updateViewport()` 传入视口的数学 y 范围），`hitTest()` 改为遍历所有段 |
| BUG-235 | **表达式解析器不认隐式乘法**：`2x + 7`、`3x^2 - 8x + 7`、`(x+1)(x-1)` 全部解析失败，一次 / 二次 / 三次函数**一个都画不出来** | 已修复 | 根因在 `src/core/CartesianExpression.cpp`：`parseProduct()` 只在显式 `*` 处做乘法，遇到 `2x` 时解析出数字 `2` 后立刻返回，把 `x` 留在记号流里导致「表达式结尾有多余内容」。修复：① 新增 `startsFactor(token)` 判断下一个记号是否为隐式乘数的开头（`x`、`(` 或数字）；② `parseProduct()` 在无显式 `*` 时若 `startsFactor(lexer.peek())` 成立则**继续解析一个因子并相乘**；③ 抽出 `multiplyTerms()` 统一做项相乘并检查次数不超过 3。修复后 `2x + 7` → `c1=2, c0=7`；`3x^2 - 8x + 7` → `c2=3, c1=-8, c0=7`；`(x+1)(x-1)` → `c2=1, c0=-1` |
| BUG-236 | **切走笛卡尔模块后引擎仍然启用**，网格仍被绘制，且窗口退出时崩溃（`-1073741819`） | 已修复 | 两处根因：① `CartesianModule::releaseSceneItems()` 只调 `clearObjects()` + `detachScene()`，**没有 `setEnabled(false)`**，网格项的 `setVisible(m_enabled && showGrid)` 仍为 true；② `MainWindow::onModuleChanged()` 的 else 分支写成 `currentModule = nullptr; clearScene();` —— **`clearScene()` 在 `currentModule` 已被置空之后才调用**，于是 `clearScene()` 里的 `if (currentModule) currentModule->releaseSceneItems();` 永不执行，旧模块的场景资源根本没被释放。修复：① `releaseSceneItems()` 补上 `m_engine->setEnabled(false)`；② else 分支**调整顺序**为 `clearScene(); currentModule = nullptr;`（与 if 分支一致）。另在 `MainWindow` 析构中先 `setCartesianEngine(nullptr)` 解绑 `ToolController` / `CanvasView` / `CartesianPanel` 三处引擎裸指针，再释放模块，杜绝悬垂指针 |
| BUG-237 | **`CartesianPointItem` 的 `shape()` 与 `QGraphicsItem::shape()` 返回类型冲突**，`CartesianPanel` 里 `point->shape()` 取到的却是 `QPainterPath` 而不是形状序号 | 已修复 | 根因：`CartesianPointItem` 把「点的形状编号（圆/方/菱/叉）」也命名为 `shape()`，与 `QGraphicsItem::shape()` 的 `QPainterPath` 签名撞车，编译器报「conflicting return type」。修复：形状编号访问器更名为 `pointShape()` / `setShape(int)` 不变，`QGraphicsItem::shape()` 恢复为标准 `QPainterPath` 覆写（返回以点为心、半径 `m_size + 4` 的椭圆），既满足 Qt 命中测试契约又不丢语义；`CartesianPanel` 相应改用 `point->pointShape()` |
| BUG-238 | **笛卡尔坐标系的三处「坐标系参数」被写错**：`paint()` 覆写签名用 `const QRectF&` 而非 `const QStyleOptionGraphicsItem*`（Qt 不认，编译失败）；`nextPointName()` 在 `const` 方法里改写 `m_nextPointSerial`（编译失败）；`CartesianPanel` 用 `QLayout` 调用 `addStretch()`（`QLayout` 无此方法） | 已修复 | 三处均为新模块初版的接口对齐问题。① 四个图元类的 `paint()` 签名统一改为 Qt 标准的 `paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*)` 并 `override`；② `nextPointName()` 去掉 `const` 限定（它需要推进名字序号）；③ `CartesianPanel::rebuildList()` 改用 `qobject_cast<QVBoxLayout*>` 取布局再调 `addStretch()`；④ `CartesianPanel` 的菜单项改用 `QWidgetAction` + `setDefaultWidget(card)` 承载自定义样式卡；⑤ 面板内的 `MenuButton` 改用 `std::function` 回调（原先试图 `connect(menu, &QWidget::mousePressEvent, ...)` 连接受保护槽位，编译不通过） |
| MAINT-030 | **第 32 批新增笛卡尔坐标系模块**（对齐 GeoGebra Web）：无限网格画布、滚轮缩放驱动分度尺、第二排画笔工具、左侧对象列表、一次 / 二次 / 三次函数绘制 | 已修复 | 本批新建 `src/core/CartesianExpression.{h,cpp}`、`src/render/CartesianElements.{h,cpp}`、`src/modules/CartesianModule.{h,cpp}`、`src/ui/widgets/CartesianPanel.{h,cpp}` 四个源文件，并扩展 `src/ui/widgets/CanvasToolBar.{h,cpp}`（第二排工具 + 画点设置飞出卡）、`src/ui/widgets/ToolIconButton.{h,cpp}`（三个新图标）、`src/canvas/ToolController.{h,cpp}`（`CartPoint` / `CartLine` 工具与逐帧跟手的拖点）、`src/canvas/CanvasView.{h,cpp}`（滚轮改分度尺、中键平移改移原点）、`src/ui/MainWindow.{h,cpp}`（模块切换联动与八个槽函数）。所有图形对象使用**原生 `QGraphicsObject` 直接事件**（不走 Qt 属性动画），保证描点拖动零延迟、不掉帧。验证：新增 `SmartGroundCartesian` / `SmartGroundCartesianVisual` / `SmartGroundCartesianPixel` / `SmartGroundCartesianUi` 四个测试可执行文件共 52 项断言全部通过；**79 个测试可执行文件零失败**；`SmartGround.exe` Release 构建成功 |
| BUG-231 | **欧拉筛的指针是瞬移过去的，不是平移的**；且 `i` 超过 `n/2` 之后动画极慢，「放一个动画得半节课」 | 已修复 | ① **指针瞬移根因**：`IModule::syncPointerTips()` 在每次 `finalizeFrame()` 时**无条件**执行 `stopAnimFor(ptr)` + `ptr->setPos(tip)`，把 `playPointerMove()` 刚启动的 `pos` 缓动动画**当场掐断**并把指针硬吸附到目标点 —— 于是所有指针移动看起来都是瞬移。修复：改为先遍历 `m_animRegistry`，若存在**正在运行**且**终点等于**目标 tip 的 `pos` 动画，则**跳过吸附**，让缓动曲线自然跑完；只有确实没有匹配动画时才吸附。② **提速**：`i > n/2` 时内层循环必然 `i*p > n` 越界、筛不到任何数，改为**批量收录素数**（新增 `ES_FAST_START` / `ES_FAST_BATCH` / `ES_FAST_DONE` 三个帧类型，每批 8 个素数一次性淡入质数库并把对应格子染绿），把后半段的逐帧演示压成几帧 |
| BUG-232 | **并查集被拆成了两个重复模块**（`DSUBasicModule` + `DSUPathCompressionModule`），学生看到的是两份近乎相同的内容；且现有实现与课堂 `dsu` 模板不符：`pa[]` / `sz[]` 的初始化语义、`merge` 的按大小合并、`sz` 的累加过程都没有被演示出来 | 已修复 | **删除** `src/modules/DSUBasicModule.{h,cpp}`，从 `MainWindow` 注册表与 `tests/CMakeLists.txt` 一并摘除，模块总数由 63 降为 62。保留的 `DSUPathCompressionModule` 更名为「并查集」，语义**严格对齐 PM 提供的课堂模板**：<br>`n = _n + 1; pa = sz = vector<int>(n, 1); iota(pa.begin(), pa.end(), 0);` → `find(x)` 带路径压缩 → `merge(u,v)` 先 `find` 两端、按大小合并（`if (sz[u] < sz[v]) swap(u, v);`）、`pa[v] = u; sz[u] += sz[v];` → `pd(u,v)` 判同集合。<br>**新增帧族**：`DSU_INIT` / `DSU_FIND_START` / `DSU_FIND_STEP` / `DSU_FIND_ROOT` / `DSU_COMPRESS_STEP` / `DSU_MERGE_START` / `DSU_MERGE_LINK` / `DSU_MERGE_SIZE` / `DSU_PREDICATE` / `DSU_DONE`。<br>**画面**：森林图上每个结点挂 `pa=N` 子标签；右侧 `pa[]` / `sz[]` 两行数组**实时同步**（`pa[]` 初始全为自身下标、`sz[]` 初始全为 1）；`merge` 帧先染子根（橙）再染父根（绿）并把父边改接为绿色；`DSU_MERGE_SIZE` 帧把 `sz[parent]` 更新为新值并落位脉冲；路径压缩时用 `GraphEdge::repointDest()` **现场改接**父边并画紫色飞线 |
| BUG-233 | **归并排序不会让数组真的裂开**：无论递归到多深，画面上始终是一条完整数组；合并完成后也没有「拼回一起」的动作，学生看不到「怎么递、怎么归」；且「未排序区域」与「已排好序区域」没有颜色区分 | 已修复 | `MergeSortModule` 做**重构级**改造：① **裂开** —— 新增 `std::set<int> m_openBounds` 记录当前所有活跃的「分裂缝」，`cellX(index)` 据此为每格计算 x 坐标（每条缝额外让出 `kMsGap = 20px`）；`MS_SPLIT` 帧把 `mid` 插入集合并调用 `applyLayout()` 用 `playMoveTo` 把两段**平移分开**，数组在画面上真的被撑开成两段。② **拼接** —— `MS_COPY`（合并完成、temp 写回原区间）把 `mid` 从集合移除，缝被合上，两段重新贴成一个整体，同时整段染成已排序色 `kMsDone` 并播放落位脉冲。③ **分色** —— 未排序段淡白 `kMsIdle`；分裂后左段浅蓝 `kMsLeft`、右段浅紫 `kMsRight`；单元素区间（递归基）转 `kMsSorted`；整段合并完成转 `kMsDone`。④ **temp 缓冲行** —— 底部新增 `temp[]` 行，`MS_CMP` 把两段队首染琥珀并脉冲，`MS_PICK` / `MS_APPEND` 把取出的数值用 `playValueFlight` 从原格**飞入** temp 格，原格转灰表示已取走 |
| MAINT-029 | **第 32 批重构与深化**：归并排序不会让数组真的裂开、合并后也没有拼接成整体；并查集被拆成两个重复模块，且语义与课堂 `dsu` 模板不符（看不到 `pa` 慢慢改、`sz` 累加）；欧拉筛指针瞬移且 `n/2` 之后拖满半节课；拓扑排序 / 朴素 Dijkstra / Kruskal / 堆调整四项待办未做 | 已修复 | 本批落地五组改造：① **归并排序重构**（`MergeSortModule`）：用 `std::set<int> m_openBounds` 记录活跃分裂缝，`MS_SPLIT` 插缝、`MS_COPY` 合缝，`applyLayout()` 据此重算 x 坐标并 `playMoveTo` 平移 —— 数组**真的裂开**成两段，合并完成后再**拼接**回整体；新增底部 `temp[]` 缓冲行；未排序段 / 左段 / 右段 / 已排序段四色区分。② **并查集合并为单一模块**：删除 `DSUBasicModule.{h,cpp}`，`DSUPathCompressionModule` 更名「并查集」，严格对齐课堂模板（`pa[i]=i`、`sz[i]=1` 起步 → `find` 带路径压缩 → `merge` 按大小合并 `pa[v]=u; sz[u]+=sz[v]`）；新增 `DSU_*` 共 10 个帧类型；画面上结点挂 `pa=N` 标签，右侧 `pa[]` / `sz[]` 两行实时同步，路径压缩用 `GraphEdge::repointDest()` 现场改接父边。③ **欧拉筛提速**：`i > n/2` 时改为批量收录素数（`ES_FAST_START` / `ES_FAST_BATCH` / `ES_FAST_DONE`，每批 8 个）。④ **四模块深化**：拓扑排序删边打 `✕  -1` 徽标 + 入度表对齐 + 成环红环；朴素 Dijkstra 新增右侧**候选尺**显式展示「为什么选它」+ 松弛失败红线划掉；Kruskal `find` 逐跳上溯可视化 + 成环边朱红闭环反证；堆调整常驻**公式条** + `HG_INDEX` 下标核对 + 数组↔树**引导线** + 数值沿树边弧线飞行。⑤ **顺带根治 `playMoveTo` 野指针**（见 BUG-230）。验证：`SmartGroundStrictAudit` 62 模块 × 2 轮 `TOTAL OVERLAP CONFLICTS = 0` / `TOTAL TIP FAILURES = 0` / `TOTAL AXIS FAILURES = 0`；**全部测试可执行文件零失败** |
| BUG-230 | **画布中途清空后动画继续写已销毁对象导致 SIGSEGV**：`SmartGroundAnimStress` 在 MergeSort 处必崩（退出码 `-1073741819`），`SmartGroundBatch31` / `SmartGroundBatch31Visual` 同样崩溃 | 已修复 | 根因：`IModule::playMoveTo()` 把 `std::vector<QGraphicsItem*>` **裸指针**按值捕获进 `QVariantAnimation::valueChanged` 的 lambda。测试用例在渲染若干帧后调用 `scene.clear()`，这些 item 已被 `delete`，但动画仍在跑并对已释放内存调用 `setPos()` → 访问违例。修复：① 函数签名由 `std::vector<QGraphicsItem*>` 改为 `std::vector<QGraphicsObject*>`（`ArrayBlock` 与 `GraphNode` 均继承自 `QGraphicsObject`，而 `QGraphicsItem` 在 Qt6 中并非 `QObject`）；② 动画 lambda 内改用 `std::vector<QPointer<QGraphicsObject>>`，item 一旦析构指针自动置空，跳过写入；③ `finished` 回调同样改用 `QPointer` 保护后再操作 `m_rowShiftAnims`。调用方 `MergeSortModule::applyLayout()` 与 `HuffmanTreeModule`（`applyTreeRows` / `applyQueueOrder` / 三处 `playMoveTo({chip}, ...)`）同步改为 `QGraphicsObject*` |
| MAINT-028 | **第 31 批动画深化**：归并排序与快速排序共用同一套 `AR_*` 帧类型导致两种算法的视觉语言完全一致（老师无法从画面区分「分治」与「归并」）；01 背包的两个候选值**同时**飞向目标格、缺少「先比较再跳」的因果；并查集只能演示有路径压缩，看不到压缩前后的差别；埃氏筛「一次干掉一大堆」虽好看但讲不清「每个合数只被筛一次」 | 已修复 | 本批落地 4 项改造 + 1 个新模块：① **归并排序**新建独立帧族 `MS_SPLIT` / `MS_REC` / `MS_MERGE_OPEN` / `MS_CMP` / `MS_PICK` / `MS_APPEND` / `MS_COPY` / `MS_DONE`，与快排的 `AR_*` 完全隔离；新增 `temp[]` 合并输出缓冲行，合并时数值从两段队首**飞入 temp** 再整体**写回**原区间；② **01 背包**新增 `KN_COMPARE` 帧：先把 `skip` 与 `take` 两个候选**同时高亮对比**，再由**胜者**单独飞向目标格，`KN_DECIDE` 落位；③ **并查集**新增「有路径压缩 / 无路径压缩」双模式设置（`ModuleField::makeSegment`），新增 `DP_NO_COMPRESS` 与 `DP_HEIGHT` 帧，底部常驻**树高标尺**实时显示森林最大深度，默认数据改为 `9\n0 0 1 2 3 4 0 6 0` 的**深链结构**（树高 5 → 压缩后 1，差别极明显）；④ **新增欧拉筛模块** `EulerSieveModule`：复用埃氏筛的矩阵布局，右侧增设**质数库 `prime[]`** 区域，`p` 指针指向当前质数，筛除 `i*p` 后立刻判断 `i % p == 0` 并 `break`，用 `ES_BREAK` 帧高亮 `i` 所在格说明「为什么可以停」；⑤ 新增 `GraphEdge::repointDest()`，让路径压缩能把已有父边**原地改指向**根结点，无需重建边。验证：`SmartGroundStrictAudit` 63 模块 `TOTAL OVERLAP CONFLICTS = 0` / `TOTAL TIP FAILURES = 0` / `TOTAL AXIS FAILURES = 0`；新增 `SmartGroundBatch31` 30 项断言全通过；**77 个测试可执行文件零失败** |
| BUG-228 | **开关控件的左右语义反了**：所有 `MyToggleSwitch` 的文字标签与滑块位置不匹配——「有向图 / 无向图」开关滑块在**右**时却显示「有向图」高亮；模块设置里的「高亮延迟边」开关显示成「开 / 关」但激活方向与直觉相反 | 已修复 | 根因在 `src/ui/widgets/MyToggleSwitch.cpp` 两处：① `paintEvent()` 里左侧画 `m_onText`、右侧画 `m_offText`，把「开」放左、「关」放右，而滑块靠右时高亮的却是左边的「开」；② `trackRect()` 用 `m_onText` 的宽度计算左侧留白，导致轨道位置与文字错配。修复：**统一为「滑块在右 = 该状态开启」**——`paintEvent()` 改为左侧画 `m_offText`、右侧画 `m_onText`，且高亮跟随滑块位置（滑块靠右 → 右侧「开/无向图」用 `textPrimary` 深色，左侧用 `textMuted` 浅色；滑块靠左 → 左侧「关/有向图」深色）；`trackRect()` 改用 `m_offText` 宽度计算左侧留白。全项目 toggle（图的类型、高亮延迟边、显示反例对照、右键擦除）语义一次性对齐。**新增 `docs/AnimationAesthetics.md` 第 4.1 节把这条固化为硬性条款** |
| BUG-227 | **二分图判定的队列最终没有清空，且 `tail` 指针有一帧飞出队列行** | 已修复 | 根因有三：① `renderFrame()` 的 `BP_FINISH` 分支**完全没有清空队列**，队列行里的残留值一直留到最后一帧；② `BP_DEQUEUE` 帧的 `val1` 传的是 `head - 1`（**已经自增过的值**），导致出队帧指向的是**上一个**槽位，`setQueueSlot(headIndex, value, false)` 清的是错格子，真正出队的那个格子永远是脏的；③ 队列行的 `head` / `tail` 指针都用 `PointerBlock::Down` + `PointerGeometry::aboveCell` 锚定在槽位**上方**，两个指针杆长相同、标签挤在同一行，`tail` 标签被 `resolveVisualCollisions()` 推到 y=434 处**飞出队列行**。修复：① 新增 `BP_QUEUE_CLEAR` 帧类型，在 BFS 结束、结论之前专门清空整行队列并隐藏两个指针；② `BP_DEQUEUE` 改为在 `++head` **之前**取值，`val1` 传自增前的 `head`；③ 队列行下移到 y=560，两个指针改为 `PointerBlock::Up` + `PointerGeometry::belowCell` 锚定在槽位**下方**，并用**不同杆长**（head=28 / tail=28，但标签分离）分层。布局重排：`col[]` 行移到 y=470，结论文本移到 y=630，奇环文本移到 y=666 |
| BUG-226 | **无向边的光束方向错误**：同一条边被第二次访问时，光束仍然从第一次的起点扫向终点（欧拉路径的延迟边、匈牙利算法的增广边、二分图的检查边都有此问题） | 已修复 | 根因：`GraphEdge` 的光束（`m_beamProgress`）只支持**正向**绘制，`paint()` 里固定用 `quadSubPath(p0, c, p1, tail, head)` 从 `source` 扫向 `dest`；而 `IModule::playEdgeTravel(edge, color, ms)` **不接受起点参数**，模块只能按「边自身的 source→dest」方向播放，于是「从 v 访问 u」时画出来的还是 u→v。修复：① `GraphEdge` 新增 `m_beamReversed` 属性（`Q_PROPERTY(bool beamReversed)`），`paint()` 中反向时用 `quadSubPath(p0, c, p1, 1.0 - head, 1.0 - tail)` 绘制，光束头部与拖尾也随之反向；② `IModule` 新增 `playEdgeTravelFrom(edge, color, fromNodeId, ms)`、`playEdgeTraceFrom(edge, color, fromNodeId, ms)`、`startBeamFrom(...)`，内部比对 `edge->dest->id == fromNodeId` 判定是否需要反向；③ 欧拉路径 `EU_EDGE` / `EU_BRIDGE`、匈牙利 `HN_TRY` / `HN_OCCUPIED`、二分图 `BP_CHECK` / `BP_CONFLICT` 全部改用带起点 id 的版本。**该语义已写入 `docs/AnimationAesthetics.md` 第 2.1 节作为强制条款** |
| MAINT-027 | **四个已有模块的动画过于静态**：Trie 只能建树、区间调度只有贪心结论、二分图只有染色结果、欧拉路径只给最终路径，缺少「为什么这样选」的因果过程 | 已修复 | 本批把 4 个分镜方案落地为真实动画（详见 `docs/ModuleStatus.md` 第 29 批）：① **Trie** 新增「插入建树 / 查询单词 / 字典序遍历」三模式设置与查询词输入框，查询模式逐字符下降并区分「命中 / 无分支 / 仅前缀」三种结局，字典序模式按字符升序 DFS 并逐词落到输出行；② **区间调度** 新增冲突**重叠带**（红色虚线半透明矩形画在时间轴上）与**反证对照阶段**（改用「按开始时间最早选」的错误准则重跑，上下两行对照贪心解与错误解）；③ **二分图** 新增显式 BFS 队列行（`q[]` + `head`/`tail` 双指针锚槽位），冲突时沿 BFS 父指针回溯**抽出那条奇环**并用 `PathTrail` 描出，环上结点染红；④ **欧拉路径** 新增 `stk[]` 栈列、`path[]` 答案列、底部结果行与轨迹描边，并把「死路 → 压栈 → 回溯出栈 → 延迟边 → 拼接」五段语义拆成独立帧类型。验证：`SmartGroundStrictAudit` 62 模块 × 2 轮 `TOTAL OVERLAP CONFLICTS = 0` / `TOTAL TIP FAILURES = 0` / `TOTAL AXIS FAILURES = 0`；新增 `SmartGroundAnimUpgrade` 9 项断言全通过；**73 个测试可执行文件零失败** |
| BUG-226a | **二分图的奇环抽取算错**：冲突发生时回溯不出奇环，「同色相邻」只停留在结论层面，讲不出「为什么一定存在奇环」 | 已修复 | 根因：原 `extractOddCycle(u, v)` 用「双指针同步上跳找相遇点」求公共祖先，当 $u$ 与 $v$ 到 LCA 的距离不同时会跳过相遇点直接返回空数组。修复：改为「分别取 $u$ 与 $v$ 的**完整** BFS 父指针祖先链 → 从两链**尾部对齐**比较公共后缀求出 LCA → 按 `u→…→LCA←…←v` 拼成完整环」，环长必为奇数（首尾两点同色、中间每步异色）。新增 `BP_ODD_CYCLE` 帧把环交给 `PathTrail` 描出并把环上结点染红 |
| BUG-225 | **画布「右键擦除」开关不生效且偏好无法记忆**：橡皮面板里的 toggle 虽能切换，但重启软件后设置丢失 | 已修复 | 根因：`ToolController::rightClickErasePreference()` 用的是**无参** `QSettings`，而 `main.cpp` 从未调用 `setOrganizationName()` / `setApplicationName()`，Qt 因缺少 org/app 名而把 `QSettings` 判定为 `InvalidScope`，读写全部落到空操作，偏好**根本没有落盘**。修复：① `src/main.cpp` 增加 `QCoreApplication::setOrganizationName("SmartGround")` 与 `setApplicationName("SmartGround")`；② `ToolController` 的偏好读写改为**显式作用域** `QSettings(QSettings::IniFormat, QSettings::UserScope, "SmartGround", "SmartGround")`，并在写入后调用 `sync()`，即使宿主未设置 org/app 名也能可靠持久化。关闭开关后画笔/荧光笔的右键恢复为与左笔完全相同的绘制行为 |
| BUG-224 | **画板新增功能三项**：① 画布最左侧缺少竖向笔刷粗细滑块；② 画笔/荧光笔缺「左写右擦」与可记忆的关闭开关；③ 矩形/圆形/三角形缺空心/实心切换 | 已修复 | ① 新增 `src/ui/widgets/CanvasBrushSlider.{h,cpp}`：贴靠画布左缘的圆角竖向滑块，仅在笔/荧光笔/橡皮三种工具下显示，与工具飞出面板、滚轮**双向同步**（`ToolController::styleChanged` 信号驱动）；② `ToolController` 新增 `m_rightClickErase` 与 `beginRightErase/updateRightErase/finishRightErase`，右键按下即进入橡皮拖拽态；橡皮面板新增 `MyToggleSwitch`（objectName = `canvasToolRightClickErase`）控制该行为；③ 矩形/圆形/三角形面板顶部新增 `MySegmentedControl`（objectName = `canvasToolShapeFill`，选项「空心 / 实心」），`ShapeItem` 以虚线框 + 透明填充表示空心、实线 + `#EEF0F4` 填充表示实心 |
| BUG-223 | **图论算法的「激光边」在射向目标结点时沿途留下碎片残留波纹**（所有图论模块均复现）：边像激光一样从 u 射向 v 的过程中，路径上出现一串断断续续的残影 | 已修复 | 根因有两处：① `GraphEdge::paint()` 用 **26 段短线**叠加绘制光束，每段各自带 `RoundCap` 与不同的 alpha 与线宽，段与段之间在曲线内侧留出可见缝隙，形成「串珠/波纹」；② `IModule::playEdgeTravel()` 每次都 `new QPropertyAnimation(edge, "beamProgress")` 而**不入动画注册表**，同一条边被多次触发时会有多个动画实例同时写 `beamProgress`，互相打断后留下停在半途的残段。修复：① 光束改用 `QLinearGradient` 沿路径**一次性连续绘制**（外层 9px 宽渐变 + 内层 3.4px 亮芯 + 端点亮点），彻底消除分段接缝；② 新增 `IModule::startBeam(GraphEdge*, QColor, int)` 统一管理光束，`m_beamAnims` 注册表按边去重（同一时刻同一条边只有一条光束），并在 `purgeTransientItems()`（每帧 `beginFrame` 触发）与析构时统一 `stop()` + `clearBeam()`，杜绝残留 |
| BUG-222 | **指针移动过程中的「瞬移」与「穿过单元格」**：指针换锚点时箭头方向瞬间翻转、位置瞬移；数组指针从一侧换到另一侧时箭头杆体横穿整排单元格 | 已修复 | 新增 `IModule::retargetPointerWithDirection(ptr, anchor, offset, dir)`：方向翻转时**不在翻转瞬间改方向**，而是先按原方向走「绕行路径」（先退到单元格行外 → 沿行外平移 → 从目标侧进入），到位后才切换方向并锁定终点；新增 `IModule::retargetPointerAcrossCells()` 让同侧换格也走行外弧线，不再直线穿过单元格。`CircularQueueModule::aimQueuePointers()` 改用新接口，`head`/`tail` 在 head==tail 时分别停在格子上下边缘 |
| BUG-221 | **指针锚点偏移 2px**：图结点指针的箭头尖端停在圆周外 2px 而非圆周上，「指向结点」看起来差一点点 | 已修复 | `GraphNode` 新增 `static constexpr double kRadius = 20.0`，`IModule::pointerTipFor()` 与 `PointerBlock::anchoredPos()` 统一改用 `kRadius`，结点指针尖端精确落在圆周上（`PointerAlignTest` 校验 dy == 20.0） |
| BUG-220 | 哈夫曼树「向上飞入」的落点算错：结点 16 飞进来后落在叶子 5/9 所在的第 0 行（而不是与合并伙伴 14 同排），下一帧又自动「跳」到正确位置；结点 20 同样先落错行再跳 | 已修复 | 根因：`HF_PROMOTE` 用 `treePosOf(id, step.before)` 计算飞入目标，而 `step.before` 是**上移之前**的森林行号，被提升的子树此时还停在更低的行，导致目标行算低一级。修复：改用 `step.afterLift`（上移对齐后的森林）计算目标 `treePosOf(id, step.afterLift)`，飞入即直接落到对齐行，无需下一帧二次跳位。同时 `HF_PROMOTE` 只处理**原始叶子**的队列筹码（内部结点由 `HF_LIFT` 帧负责树内位移），避免同一结点在树区与队列区各留一份造成重影 |
| BUG-219 | 哈夫曼树合并出新父结点时，父结点的队列副本**从屏幕上方 (0,0) 飞下来**，中间拖出一条穿越全画面的长轨迹 | 已修复 | 根因：`HF_MERGE` 新建队列筹码 `GraphNode(9000 + parentId)` 后未设置初始位置，`GraphNode` 默认 `pos()` 为原点。修复：新建筹码时先 `chip->setPos(treePosOf(parentId, step.after))`（父结点在树中的真实坐标），再 `playMoveTo` 到 `queueSlotPos`，父结点即从树里「落」回候选队列 |
| BUG-218 | 哈夫曼树根结点向上调整完成后，**旧的那个点没有及时删除**，在候选队列里留下一个多余的根结点副本 | 已修复 | 根因：最后一次合并 `HF_MERGE` 仍把根结点的队列筹码送到候选队列（`queueIndexOf` 返回 0），随后 `HF_FINAL` 只染色树内根结点，没有回收队列筹码。修复：`HF_MERGE` 增加 `isRoot` 判定，最后一次合并只为根结点**创建但立刻隐藏**队列筹码（保持「每个结点一份队列副本」的节点计数不变），不再把它排入队列；`HF_FINAL` 再次兜底隐藏根结点的队列筹码，双重保险消除重影 |
| BUG-217 | 哈夫曼树合并出父结点时，两条子边**瞬间出现**，动画缺少「从孩子长到父亲」的流线感 | 新增需求（已实现） | `GraphEdge` 新增可动画属性 `Q_PROPERTY(qreal drawProgress READ drawProgress WRITE setDrawProgress)`，默认 1.0（完全绘制）；`GraphEdge::paint()` 按 `drawProgress` 对二次贝塞尔曲线做**部分求值**（de Casteljau 分割出 `[0, t]` 段），画出从孩子端生长到父亲端的渐进曲线；`IModule` 新增共享辅助 `playEdgeGrow(GraphEdge* edge, int duration = 200)`（`OutCubic` 缓动）。`HF_JELLY` 帧中父结点「果冻」出现的同时，把两条子边的 `drawProgress` 置 0 后在 **200ms** 内生长到 1.0；`HF_PICK` / `HF_JELLY` / `HF_MERGE` 三帧入口调用 `completeEdgeGrowth()` 兜底，保证快速播放时边不会停留在半画状态。实测 t=45ms 时 progress≈0.42、t=90ms 时≈0.75、t=135ms 时已 1.00 |
| BUG-216 | 哈夫曼树整棵树「乱完了」：根结点被画在叶子下方、同一层的结点错行、整棵树完全不成结构 | 已修复 | 根因在 `HuffmanTreeModule::simulateStates()`：把「子树整体上移」的行差算成了 **结点 id 之差**（`const int higher = (aRow < bRow) ? b : a; const int delta = higher - lower;`），而不是**行号之差**，导致 delta 一路放大到 9、11，树高失控、层次错乱。修复后 delta 改为 `higherRow - lowerRow`（`aRow` / `bRow` 本身），行高回到理论值；最终树为「根在最上、叶子在最下、每个父结点严格高于其孩子」的规范结构，最小结点间距 56.08px，无交叉线 |
| BUG-215 | 哈夫曼树的边带有箭头，树是**无向**结构，箭头是错的 | 已修复 | `HuffmanTreeModule::buildLayout()` 中两条子边改用 `new GraphEdge(child, parent, false)`（`is_directed = false`、`is_bidirectional = false`），`GraphEdge::paint()` 不再绘制箭头端点。0/1 路径标签由 `pathLabel` 徽标机制单独绘制，不受影响 |
| BUG-214 | 哈夫曼树的频率数组**整格是空的**，看不到任何原始权值 | 已修复 | 根因：`buildLayout()` 按 `m_nodes.size()`（2n-1，含内部结点）创建数组格子，导致 `HF_SORT_DROP` 的 `perm.size() != m_arrayCells.size()` 守卫**永远成立**而提前 `return`，数值从未写入；且初始态 `showValue = false` 本就全空。修复：数组格子数改为 `m_leafCount`（n 个），初始态即写入原始权值并置 `showValue = true`（`kHfLeaf` 底色），`HF_RESET` 同样恢复原始权值；排序阶段改为 `HF_SORT_LIFT`（该格飞空）→ `HF_SORT_DROP`（数值流星般落回）成对推进，每帧携带当前置换数组以保证回放/拖进度条可复现 |
| BUG-213 | 扩展欧几里得的公式飞入时**位置太偏左**，落点明显偏在目标位置左侧 | 已修复 | 根因有二：① `IModule::playTextFlight()` 把终点当**中心**处理（`end.x - halfW`），而调用方传的是目标文本的**左上角**，导致落点偏左半个字宽；② 回代片段的前缀宽度用**硬编码 7.6px/字符**估算，与 mono(13) 实际约 17px/字符严重不符，片段被甩到行首左侧。修复：① 新增 `IModule::playTextFlightAligned(text, start, endLeft, ...)` —— 起点终点均为**左对齐**语义，`ExgcdModule` 的四处飞行全部改用它；② `ExgcdBackStep` 新增 `prefixText` 字段记录片段前的真实前缀文本，落点改用 `QFontMetrics(target->font()).horizontalAdvance("1 = " + prefixText)` 实测宽度（末行 425px，原估算仅约 170px）。回代片段现精确落进括号位置 |
| BUG-217a | 扩展欧几里得的推导文字**互相重叠**：回代各行、辗转相除左右两栏、章节标题与正文全部压在一起 | 已修复 | 根因：行距（`kExRowStep = 26` / 回代 24）小于 mono(13) 的实际行高（28px）。修复后重排为**单栏左对齐**结构：等式段 `kExEqY = 80`（步距 34）、辗转相除段标题 `kExGcdTitleY = 190` / 栏头 `kExHeadY = 222` / 行首 `kExRowY = 252`（步距 32，左右两栏分置 `x = 100` 与 `x = 500`，互不侵占）、回代段标题与正文独立成行（步距 32）；章节标题与栏头全部错开行位。文本框两两相交检测由 15 处降为 **0 处** |
| BUG-212 | 画布上大量讲课脚本文字（`m_noteText` / `m_mathText` / `m_resultText` / `m_scanText` / `m_heapText` / `m_windowText` / `m_probeText` / `m_stateText` 等）直接常驻显示，遮挡结构标注；软件定位是「老师免备课的讲课助手」，这些文字应由老师在口头讲解时自行发挥，而不是印在画布上 | 已修复 | `IModule` 新增**全局讲课脚本文字门控**：`static bool s_scriptTextVisible` + `setScriptTextVisible(bool)` / `scriptTextVisible()` / `createScriptText(scene, text)` / `setScriptText(item, text[, color])` / `clearScriptText(item)` / `createStatusText(scene, text)` / `setStatusText(item, text)`。`createScriptText` 在关闭状态下创建**空且不可见**的文本项，`setScriptText` 在关闭状态下清空内容并隐藏。59 个算法模块的讲课脚本类文字成员共 **263 处**改为经由门控创建与写入。**结构性标注全部保留**：`频率数组 a[]`、`a[] 原数组`、下标行、`桶 0`、树名、`第一步：…` / `阶段一：…` 章节标题、`a[i] / 编码 / 码长` 表头仍由 `scene->addText()` 直接创建并常驻。界面新增「显示讲课脚本文字」开关（`MainWindow`，`objectName = "scriptTextToggle"`），**默认关闭**，勾选后所有讲课脚本文字恢复显示并即时重绘画布 |
| BUG-211 | 扩展欧几里得的回代公式片段飞入后与最终静止位置存在像素偏差，飞入终点与文字最终落点不重合 | 已修复 | `IModule::playTextFlightTo` 的飞行副本目标点直接取 `target->pos()`，落地时 `safeTarget->setPos(dst)` 保证与静止位置**完全重合**；`playTextFlightFragment` 额外用 `QFontMetrics(target->font()).horizontalAdvance(prefix)` 计算前缀宽度，片段落在前缀右端而非整行左端，两者均无像素差 |
| BUG-210 | 哈夫曼树森林中出现多棵组件树时，各树之间位置移动混乱、结点大量重叠（多个示例均复现），判定为坐标计算错误 | 已修复 | 布局引擎重写为 `subtreeLayout` + `layoutComponentSym`：递归计算每棵子树占用的宽度后，组件树按 `m_compRank` **稳定排序**并依次分配水平区间，区间内再按对称规则放置结点；各组件树**互不交换位置**且**不再重叠** |
| BUG-209 | 哈夫曼树左右两棵子树在不合适时机交换位置；交换后有结点重合；从第三层开始，新造结点并不在它两个孩子的垂直平分线上（坐标计算错误） | 已修复 | 布局改用**对称递归** `layoutComponentSym`：对每个内部结点强制 $x_L = x_P - d$、$x_R = x_P + d$，其中 $d$ 为子树半宽，从而严格满足 $x_P = (x_L + x_R)/2$；左右孩子的次序由合并时的子结点次序**固定**，不再随布局重新洗牌 |
| BUG-208 | 哈夫曼树阶段三「下方选点并出现新的点」时，新结点从屏幕上方 (0,0) 飞入并拖出一条特别长的边线，逻辑很糟 | 已修复 | 新父结点在 `HF_PICK` 帧起即在两子结点中点处 `setOpacity(0)` **原地预置**，`HF_JELLY` 帧只播放 `playNodeAppear` 果冻生长 + `playRipple` 涟漪扩散，不再从屏幕上方飞入，超长边线消失 |
| BUG-207 | 哈夫曼树「选用的结点」是闪现上去的，看不到从下方候选队列**向上飞入树区域框内**的过程 | 已修复 | `HF_PICK` 帧中两个被选候选结点由底部队列位置**向上飞入树区域框内**（`playMoveTo` 逐项独立目标点补间），到位后再进入 `HF_PROMOTE` / `HF_JELLY`，不再是原地闪现 |
| BUG-206 | 哈夫曼树阶段一数字飞入频率数组格子时**位置偏下**，没有落在格子正中 | 已修复 | `HF_SORT_DROP` 的落点由「格子上边缘」改为**格子中心**：水平方向按格子宽度居中、垂直方向按格子高度居中，数字精确落在单元格几何中心 |
| BUG-202 | 哈夫曼树 WPL 累加结束帧显示的总数是 0（`HF_WPL_DONE` 帧在累加循环结束前就读取了尚未写入的 `m_wplTotal`），导致最终结论与逐项累加数值不一致 | 已修复 | `runAlgorithm()` 的 WPL 段改为先在循环外累加变量 `running` 中求和，循环结束后再执行 `m_wplTotal = running;` 然后才构造 `HF_WPL_DONE` 帧；`HF_WPL` 帧本身仍把 `running` 实时写入 `m_wplSumCell`，逐项高亮与累加过程不变。默认数据 `45 13 20 16 9 12 5` 的 WPL = 45×2 + 13×3 + 20×2 + 16×3 + 9×4 + 12×3 + 5×4 = **309**（等于全部内部结点权值之和 14+25+30+45+75+120，符合哈夫曼最优性） |
| BUG-203 | 哈夫曼树的建树阶段不满足「参考草图」的 choreography：候选队列行长一行没有按权值升序排好；合并时新结点没有在两子结点中点「果冻」般长出；父结点生成后没有复制一份落回底部候选队列；合并后队列成员也没有左右让位重排 | 已修复 | `HuffmanTreeModule` 整模块重写为六阶段：**阶段一**顶部数组初始为空，数字先悬停在格子上方（`kHoldY`），再按升序一个一个飞入正确格子（`HF_SORT_LIFT` → `HF_SORT_DROP` × n → `HF_SORT_DONE`）；**阶段二**全部数字飞入底部候选队列圆形结点（`HF_QUEUE_DROP`），按 `(权值, 是否叶子, id)` 三级比较排好；**阶段三**每次合并走 `HF_PICK`（取两最小，高亮 + 缩放脉冲）→ `HF_PROMOTE`（子树整体上移到同一行，树高自动对齐）→ `HF_JELLY`（在中点长出新结点，`playNodeAppear` + `playRipple` 果冻特效，边线由橙色高亮）→ `HF_MERGE`（父结点副本从树中飞下落到候选队列，被取走的两个结点淡出，其余成员左右让位重排）；**阶段四**`HF_FINAL` 根结点染成专用根色并缩放强调；**阶段五**`HF_CODE` 沿根到叶路径逐段贴出 0/1 路径标签并填写右侧编码表；**阶段六**`HF_WPL` 逐项高亮 `a[i]×码长` 并累加到 WPL 汇总格。树布局改用「子树宽度」递归算法（`subtreeWidth` / `layoutComponent`），父结点严格居中于两子结点中点，子树过宽时自动左右分开保证间距均匀 |
| BUG-204 | 哈夫曼树的候选队列在权值相同时无法区分叶子与内部结点，导致取数顺序与「优先取叶子」的课堂约定冲突（例如 45 同时是叶子和内部结点时先取了内部结点） | 已修复 | `computeTree()` / `simulateStates()` 的排序比较器改为三级比较：先比权值，权值相同再比「是否原始叶子」（叶子优先），最后按 id 稳定排序；即 `(weight, isOriginal ? 0 : 1, id)`，与课堂「同权值优先取叶子」的规则一致 |
| BUG-205 | 哈夫曼树的 `HF_CODE` / `HF_WPL` 帧文案里编码与码长恒为空：帧文本在 `runAlgorithm()` 里就按 `m_codeRows[r].code` / `codeLength` 拼装，但这两个字段要到播放阶段的 `HF_CODE` 帧才被 `setLeafCode()` 回填，导致讲解文字变成「编码为 ，码长 0」 | 已修复 | `computeCodes()` 在求出 `m_codeMap` 后立即回填 `m_codeRows` 的 `code` 与 `codeLength`；`buildLayout()` 中的调用顺序改为 `computeTree() → simulateStates() → buildCodeTable() → computeCodes()`，保证表格行先建、编码后填 |
| BUG-199 | 扩展欧几里得模块只是抽象递归演示，没有可视化「同余式 → 不定方程 → 辗转相除 → 递归回代」的完整推导链，缺少左侧取余 / 右侧整除式的双栏对照，也没有任何公式飞入展开的效果 | 已修复 | `ExgcdModule` 整模块重写为三阶段：**第一步**在画布上方依次呈现 `11914·x ≡ 1 (mod 146415)` → `11914·x = 146415·y + 1` → `11914·x - 146415·y = 1` 三行变形（逐行高亮）；**第二步**双栏对照表，左栏 `146415 % 11914 = 3447`、右栏 `146415 = 12 × 11914 + 3447`，每轮由「行」指针锚定当前行并高亮左右两式；**第三步**递归回代逐层展开，回代式由右侧整除式行飞入并展开为 `1 = 10 × 1 - 1 × 9` → `1 = 10 - (29 - 2 × 10) = 3 × 10 - 29 × 1` → … → `1 = 15079 × 11914 - 1227 × 146415`，末尾给出「11914 在模 146415 意义下的乘法逆元是 15079」的绿色结论 |
| BUG-200 | 扩展欧几里得的输入上限为 `b <= 100000`，导致 `146415` 这类合法数据被直接判为非法 | 已修复 | `ExgcdModule::buildLayout()` 的取值上限放宽到 `1000000`，并在 `getInputHint()` 中同步说明 |
| BUG-201 | LCS 模块的文字与数组下标重合：「串 a[]」/「串 b[]」标签压在字符串单元格上方，矩阵列下标标签与矩阵首行重叠 | 已修复 | `LCSModule` 的 `kLcStrY` 由 62 调到 92、`kLcGridY` 由 206 调到 236；字符串标签上移到 `kLcStrY - 26`，矩阵列下标移到 `m_gridY - 22`，行下标移到 `m_gridX - 34`，三者各占独立行不再重叠 |
| BUG-188 | 哈夫曼树完全没有动画：整棵树在建树前就被一次性画好，用户看不到「排序 → 建树」的流线过程，也没有「元素飞出数组、像飞星一样落位」的效果 | 已修复 | `HuffmanTreeModule` 整模块重写为两阶段动画：**阶段一**先把频率数组升序排序（不在应在位置的元素先飞出数组并高亮，再像流星一样逐个飞回正确位置）；**阶段二**每次从森林中取两个最小权值，用 `HF_PICK`（取最小）→ `HF_LIFT`（飞到一起 / 子树上移到同一行）→ `HF_MERGE`（连线生成父节点）三帧逐步建树。树不是预先画好的：所有结点与边 `setOpacity(0)`，随合并逐个淡入，配合 `IModule::playNodeAppear()` 的 `OutBack` 生长动画 |
| BUG-189 | 哈夫曼树建树时树高与树宽瞬间跳变，没有过渡；且树根与叶子行的上下关系颠倒，根被画在底部 | 已修复 | `simulateLayout()` 预先模拟每次合并后的森林形态，记录每一帧的 `rows` / `cols` 快照；`computePositions()` 采用「叶子等距铺开 + 内部结点取左右孩子中点」的对称布局；每次合并先补间到「上移对齐」位姿（300ms），再补间到「合并完成」位姿（460ms）。`IModule` 新增共享助手 `playMoveTo(items, targets, duration)` 支持逐项独立目标点的补间 |
| BUG-190 | 哈夫曼树模块内嵌结构体导致 moc 编译失败（`Meta object features not supported for nested classes` / `Missing access specifier for slots`） | 已修复 | 把 `HuffNode` / `MergeStep` 从类内嵌套结构体提升为文件级结构体 `HuffmanNodeData` / `HuffmanMergeStep`；成员 `slots` 与 Qt 关键字 `slots` 冲突，统一改名为 `cols` |
| BUG-191 | 桶排序与基数排序的竖直桶数组单元格之间存在空行，相邻单元格之间出现两条横线 | 已修复 | `BucketSortModule` / `RadixSortModule` 的桶槽位改用 `cellHeight == spacing` 的无缝竖向排布（相邻单元格之间只保留 1 条线） |
| BUG-192 | 桶排序的章节说明文字与 Bucket 0 的标题重叠；基数排序的「按当前位分的 10 个桶」压住数字 0 的标题 | 已修复 | 桶排序的章节说明文字上移到 `(40, 180)`、桶头单元格下移到 `y = 250`；基数排序的「按当前位分的 10 个桶」上移到 `(40, 154)`，并把数字标题整合进桶头单元格 |
| BUG-193 | 循环队列的 `head` / `tail` 指针从上方插入数组内部，箭头杆体穿过单元格；数组上方多出一行冗余标题 `q[N], N = 6`；右下角有「队尾走到末尾后回到队头」的提示文字；左下角的「操作日志」方框内容全部渲染在框格下方，语义不明 | 已修复 | `CircularQueueModule` 的两个指针改为 `Direction::Up` + `PointerGeometry::belowCell()`，从数组下方指上去且箭头尖端停在单元格外；删除 `q[N], N = 6` 标题行、右下角回绕提示文字、以及整套「操作日志」方框与标题（含 `m_logBlocks` / `logTitle` 成员） |
| BUG-194 | 循环队列被归类在「数据结构 · 进阶」，实际属于基础数据结构 | 已修复 | `MainWindow` 的模块分类表把 `CircularQueueModule` 从「数据结构 · 进阶」移到「基础数据结构」 |
| BUG-195 | 字符串哈希的演示与课堂模板完全不一致 | 已修复 | `StringHashModule` 按课堂模板重写：`p1 = 131`、`m1 = 998244353`；`p[i] = p[i-1] * p1 % mod`；`h1[i] = (h1[i-1] * p1 + s1[i-1]) % mod`；`h2[i]` 同理；从 `len = min(n1, n2)` 递减枚举，比较 `getHash(h1,1,len) == getHash(h2,n2-len+1,n2)` 或 `getHash(h2,1,len) == getHash(h1,n1-len+1,n1)`，首个匹配即为答案 |
| BUG-196 | 字符串哈希缺少字符游标指针 `i`，无法看出当前在算哪个字符；且 `p[]` 的计算与 `h[]` 的计算混用同一个动作类型，测试无法区分 | 已修复 | 新增 `STH_POW` 动作类型专门记录 `p[]` 幂次计算；新增指针 `i`（s1 行，`Up` 方向）与 `j`（s2 行，`Up` 方向），锚定字符单元格并用 `PointerGeometry::belowCell()` 保证箭头尖端不进入格子 |
| BUG-197 | LCS 左下角文字重叠、矩阵上方的数组单元格不并拢、左上角多出 `s = "abcbdab"` 一行 | 已修复 | `LCSModule` 删除左上角的 `s = "abcbdab"`；矩阵上方横排两个无缝数组（字符串 a / 串 b，中间留 40px 间隙）；指针 `i` 自上而下指向 a 串元素、指针 `j` 自下而上指向 b 串元素；底部轨迹区重新排布为 追踪标签(624) / 追踪格(650) / 说明(712) / 结论(746)，彻底消除压字 |
| BUG-198 | 哈夫曼树的阶段二没有把「被取走的两个最小值」从数组里移除，数组始终显示全部原始权值 | 已修复 | 每完成一次合并，数组已用槽位计数 `m_usedCount` 递增，剩余槽位置灰为 `kHfEmpty`，直观体现「森林中还剩哪些候选」 |
| BUG-187 | 8 个新增算法模块的指针偏移沿用了旧的「落进格子中心」写法，箭头尖端与杆体会压在单元格内部，与项目「指针必须在单元格外」的统一规范冲突 | 已修复 | 新增 `tests/Batch9VisualTest.cpp` 逐帧审计全部数组指针：只要箭头尖端越出其锚定单元格的外框即判失败。桶排序 / 基数排序 / 循环队列 / 单调队列 / 字符串哈希 / 最长公共子序列的指针统一改用 `PointerGeometry::aboveCell()`；LCS 的行指针 `i` 改 `PointerGeometry::leftOfCell()` 且方向为 `Right`（横向从左侧指入），列指针 `j` 用 `aboveCell()` |
| BUG-186 | 哈希表探针指针 `k` 的箭头体与箭头尖扎进槽位单元格内部 | 已修复 | `HashTableModule` 的 `m_probePointer` 锚点偏移由 `(cellW / 2, 19)`（落在格子中心）改为 `PointerGeometry::aboveCell(cellW, cellH)`，尖端停在槽位上边缘上方 2px、杆体与 `k` 标签全部在单元格之外；补齐此前缺失的 `cellHeight()` 访问器（`renderFrame` 引用了未声明的 `cellHeight`）；`tests/PointerAlignTest.cpp` 与 `tests/PointerMotionTest.cpp` 的期望坐标同步改为 `(cellW / 2, -2)` 与 `y = 116.0` |
| BUG-185 | Trie 字典树的游标指针 `i` 整体下移，箭头体与箭头尖都跑进数组格子内部 | 已修复 | `TrieModule` 的 `m_indexPointer` 锚点偏移由 `(27, 19)`（格子中心）改为 `PointerGeometry::aboveCell(54.0, 38.0)`，箭头尖端停在格子上边缘上方 2px，杆体向上、标签 `i` 落在杆尾之外，整根指针完全位于数组外部 |
| BUG-184 | 行列式模块中「矩阵」形态的数学方括号 `[` `]` 没有把矩阵包住，括号上下与矩阵外边框齐平甚至更靠内，视觉上像是浮在矩阵边上 | 已修复 | `MatrixBlock::paint()` 的 `PlainMatrix` 分支：括号折线上下各外扩 `over = 9.0`、左右各外扩 `3.0`，臂长 `arm = 14.0`、唇长 `lip = 7.0`；`sideMargin()` 由 20 放宽到 26 以完整容纳括号臂；`boundingRect()` 按形态引入 `vPad`（矩阵 13.0，其余形态 4.0），保证外扩出的括号部分不被场景包围盒裁剪 |
| BUG-183 | 全项目字体未统一：结点数值、堆数组标题、Tarjan 的 `ins / stk` 标签仍直接用 `QFont("Arial", ...)` / `QFont("Consolas", ...)`，绕开了 BUG-151 建立的 `UiFonts` 字体引擎 | 已修复 | `GraphElements.cpp` 的 `GraphNode::paint()`（中心值 / `v:` / `dfn: low:`）、`StackBlock::paint()`、`ArrayBlock::paint()` 全部改走 `UiFonts::mono()`；`HeapBasicsModule` 的两个英文标题改 `UiFonts::cjk()`；`TarjanModule` 的 `ins / stk` 改 `UiFonts::mono()`；`GraphRenderHelper::addLabel()` 的 `QFont("Microsoft YaHei", ...)` 改 `UiFonts::cjk()`。至此 `src/` 下不再有任何直接 `QFont(...)` 字面量（`ToolIconButton` 的 `Segoe UI` 符号字体探测除外） |
| BUG-182 | 矩阵 / 行列式 / 表格三种形态互相切换时，新形态渲染出来了、旧形态的残影却留在画布上，必须拖动右侧画布才会被清掉 | 已修复 | 根因：`QGraphicsScene::clear()` 只清场景项，`QGraphicsView` 的缓存层（背景层 + 物品层）仍保留旧像素。修复：`MainWindow` 新增 `invalidateCanvas()`，对场景的所有视图调用 `invalidateScene(view->rect(), BackgroundLayer)` 与 `invalidateScene(view->rect(), ItemLayer)` 并强制 `viewport()->update()`；`clearScene()` 与 `rebuildCurrentLayout()` 末尾各调用一次 |
| BUG-181 | 矩阵形态被画成行列式的两根竖线，与数学中的方括号矩阵写法不符；表格形态反而带上了矩阵外框 | 已修复 | `MatrixBlock::paint()` 三形态分流：`PlainMatrix` 用多段折线绘制数学方括号 `[` `]`（`arm = 13` / `lip = 6` 的上下小唇）；`Determinant` 保持左右单根竖线加上下小衬线；`BracketedTable` 完全不画外框，`sideMargin()` 由 20 收到 4 |
| BUG-180 | 矩阵设置只能改单元格宽度，老师无法画长方形窗格；标签「单元格」也不够醒目 | 已修复 | `ModuleField` 新增 `SizeInput` 复合控件与 `makeSize(key, label, w, wMin, wMax, h, hMin, hMax)` 工厂；`SandboxMatrixModule` 的设置项由 `makeText("cell", "单元格", ...)` 改为 `makeSize("cell", "单元格大小", ...)`，宽度 34~120px、高度 26~120px 双向可调；`MatrixBlock` 新增 `cellHeight` 属性与 `setCellHeight()` / `cellHeightValue()`，绘制、`cellAtPoint()` 与编辑框定位全部改用矩形单元格 |
| BUG-179 | 哈希表指针 `k` 悬在表格上方的虚空里，指不到槽位；且所有指针都是瞬间跳到目标，没有平移动画 | 已修复 | `HashTableModule` 的 `m_probePointer` 改 `Down` 方向、`shaftLength = 26`、锚点偏移 `(cellW / 2, 19)`，尖端正好落在槽位中心；标签用 `setLabelAtTail(10.0)` 放到杆尾；`HT_HASH` / `HT_PROBE` 的定位改用 `IModule::retargetPointerTo()` 平移过去 |
| BUG-178 | Trie 字典树的指针 `i` 没有指到格子正中间；向下指时标签 `i` 与箭头杆身重叠；新建结点突然「啪」地冒出来，很突兀 | 已修复 | `m_indexPointer` 锚点偏移改为 `(27, 19)`（格子水平中心 + 竖直中心），`setLabelAtTail(10.0)` 把标签推到杆尾之外；新增 `IModule::playNodeAppear()`（`QEasingCurve::OutBack` 从 0.15 缩放弹到 1.14 再回落 1.0，叠加透明度淡入）与 `playFadeIn()`，`TR_CREATE` 帧新建结点时播放生长动画 |
| BUG-177 | 指针会瞬间闪现并跳到新位置，不像「移动过去」；且指针在未被使用前停在原点 `(0,0)`，把 `itemsBoundingRect()` 撑大、连带整个画布渲染错位 | 已修复 | `PointerBlock` 新增 `m_placed` 标记（通过 `xChanged` / `yChanged` 信号捕获）与 `isPlaced()`；`IModule::playPointerMove()` 在指针尚未落位时直接定位、落位之后一律走 `QPropertyAnimation` 平移；`PointerBlock::boundingRect()` 在 `opacity() < 0.02` 时返回空矩形并监听 `opacityChanged` 触发 `prepareGeometryChange()`，隐藏指针不再污染场景包围盒 |
| BUG-176 | 二分图的 `col[]` 数组紧贴在图下方，视觉上挤成一团；数组默认初值显示为 `—` 而不是 `-1` | 已修复 | `BipartiteModule` 图心由 `(470, 210)` 下移到 `(470, 210)` 并加大节点间距到 115/135，`col[]` 标签下移到 `y = 470`、数组行下移到 `y = 487`、结论文字下移到 `y = 610`；`col[]` 初始自定义值由 `"—"` 改为 `"-1"`（与 `m_color` 数组的 `-1` 未染色语义一致） |
| BUG-175 | 下拉里 Kruskal 与 Prim 分散在两个「最小生成树」分类里 | 已修复 | `MainWindow` 分类表合并为单一分组「图论 · 最小生成树」，内含 `KruskalModule` 与 `PrimModule`；去重后唯一模块数由 56 降为 54，`tests/GroupAUiTest.cpp` 的断言同步由 56 改为 54 |
| BUG-174 | Prim 演示时指针 `u` 没有正对顶点圆，偏了一截 | 已修复 | `PrimModule` 的 `m_pickPointer` 改 `Up` 方向、`anchorOffset = (0, 20)`、`labelOffset = (0, 30)`、`shaftLength = 24`，尖端落在半径 20 的顶点圆边缘；定位改用 `retargetPointerToNode()` 平移 |
| BUG-173a | `EulerModule.cpp` 中文字符串 `统计每个节点的入度与出度` 存在损坏字符 | 已修复 | 修正为 `统计每个节点的入度与出度`（原字节序列非 UTF-8，编译期被转义为乱码） |
| BUG-173 | 折叠面板内容不可见：「矩阵设置」与「图结构选项」面板展开后一片空白，控件全部看不见 | 已修复 | 根因：`MyCollapsible::m_contentHeight` 只在 `setContentWidget()` 里、且在布局尚未建立（`sizeHint()` 返回 0）时计算一次，导致内容区高度被钉死为 0。修复：新增 `refreshContentHeight()` 重新测量并同步 `min/maxHeight`；`eventFilter()` 拦截内容控件的 `QEvent::LayoutRequest` 自动重测；`setContentWidget()` 改走 `updateGeometryAnimated()` 统一路径。`MainWindow::rebuildModulePanel()` 保持「矩阵设置」仅在矩阵模块下渲染、切到其他模块自动隐藏 |
| BUG-172 | 图结构选项面板在图模块下同样看不到任何控件（与 BUG-173 同根因），连带欧拉路径无法切换有向/无向导致入度不显示 | 已修复 | 同 BUG-173 的 `MyCollapsible` 高度重测修复；`EulerModule` 另补有向图开关与 `in[]` / `out[]` 双数组入出度渲染，无向图时回落 `deg[]` |
| BUG-171 | 哈希表与 Trie 字典树的数组行单元格之间存在空隙，与项目内其他数组视觉不一致；哈希表默认数据缺乏代表性，看不到冲突 | 已修复 | `HashTableModule` / `TrieModule` 数组改用 `cellWidth == spacing` 的无缝排布（单元格起点 = `startX + i * cellWidth`）；`HashTableModule` 默认数据改为 `5\n3 10 17 24 31`（对 7 取模后 `3 3 3 3 3` 全部冲突，完整演示线性探测） |
| BUG-170 | Trie 字典树是「先建好整棵树再先序遍历一遍」，不是真正的动态建树教学 | 已修复 | `TrieModule` 重写为动态构造：起始只有根节点、游标 `p` 默认指向根；字符串以数组呈现并由指针 `i` 右移逐字符推进；每步检测当前子节点是否已存在，命中复用（`HIT`）、未命中新建（`CREATE`）；下方用一整条无缝数组模拟树形结构的内存槽位 |
| BUG-169 | 二分图判定 BFS 染色时，父节点的色块被子节点染色过程覆写，最终 6 个点出现 4 橙 + 2 蓝，与二分图应有的 3 蓝 + 3 橙不符 | 已修复 | `BipartiteModule` 把颜色写入与颜色块刷新彻底隔离：染色只写目标结点的 `centerText` / 状态色，不再遍历修改父结点的色块；图内结点背景色与数组中的颜色标记同源刷新 |
| BUG-168 | 计数排序的前缀和阶段是逐位静态赋值，没有体现「原数组与前缀和 `s[i-1]` 同时飞出合并成和」的效果 | 已修复 | `CountingSortModule` 前缀和阶段改用 `IModule::playMultiValueFlight`，把 `prefix[i-1]` 与 `cnt[i]` 两个来源数字同时沿弧线飞入 `prefix[i]` 合并落位 |
| BUG-167 | Prim 最小生成树只演示了第一个点便提前结束，直接报「存在孤立分量，不存在 MST」，全程仅 3 帧，逻辑炸裂 | 已修复 | `PrimModule::runAlgorithm()` 在主循环开始前先执行一次 `relaxFrom(m_src)`，把起点的出边松弛进候选集，否则第一轮候选集为空即被误判为不连通 |
| BUG-166 | GCD 辗转相除画面堆叠了多组重复的 `ValueBar` 槽位，特效杂乱、空间重复，看不出「取模 + 小的替换大的」 | 已修复 | `GcdModule` 整模块重写：画面只保留两个大号数字框 `a` / `b` 与一个余数框 `r`，中央直书 `a % b = r` 的算式；每轮把较小者用 `a % b` 替换较大者，历史轮次变灰留存 |
| BUG-165 | DFS 显式栈的 `stk` 指针标签在右边缘被裁掉一半，看不全 `stk` 字样 | 已修复 | 根因：`PointerBlock::labelWidth()` 的测量字体与 `paint()` 的绘制字体不一致导致宽度估计偏窄。修复：`labelWidth()` 与 `paint()` 统一使用 `UiFonts::mono(14, true)`，宽度改为 `qMax(28.0, adv + 14.0)` 留足内边距；`PointerBlock` 新增 `setAnchorItem(QGraphicsItem*)` 支持锚定任意图元（不再局限于 `GraphNode*`） |
| BUG-165a | 部分较新的算法模块动画速度固定，未随当前解说文字长度变化 | 已修复 | 把自适应延时下沉到核心基层：`IModule::frameDelayMs(const AnimFrame&)` 与 `IModule::frameDelayMsForText(const QString&)` 统一按「解说 + 公式」文本长度计算（下限 900ms、上限 3000ms），`MainWindow::advanceFrame()` 改用该基类实现，新模块自动继承 |
| BUG-164 | 9 个新模块的画布存在多处元素压盖：标题文字压住图结点、结点锚定指针压住被指向的圆、指针压住数组下标行、行标签压住单元格 | 已修复 | `PointerBlock` 新增 `setLabelToRight()`（标签移到杆右侧）与 `setAnchorOffset()`；`DFSModule` 图心下移到 y=205、`vis`/`ord` 两行下移 30px、`u` 指针改 `Left` 方向且锚点偏移到圆右侧；`PrimModule` 图心下移到 y=200、`u` 指针锚点偏移到 (30,30)；`EulerModule` 图心下移到 y=200；`CountingSortModule` 四行整体下移 20px、行标签与单元格起点间距由 140px 放宽到 180px。`tests/OverlapAuditTest.cpp` 扩测至 9 个新模块（并把 `GraphNode` 审计框由宽松的 `boundingRect()` 改为真实圆框 + 单独采集 `subLabel`/`valueText`），最终 `conflicts=0` |
| BUG-162 | 行列式功能未落实：「矩阵设置」面板缺少形态单选、行/列默认 3×3；数据输入区仍强制 `rows=` / `cols=` / `type=` 键；面板在其他模块下依然显示 | 已修复 | `SandboxMatrixModule` 重构：`moduleSettingsFields()` 追加 `Segment` 形态字段（矩阵 / 行列式 / 表格，默认索引 2 即「表格」）、`IntInput` 行数与列数（默认 3×3）；`buildLayout` 移除全部 `rows=` / `cols=` / `type=` / `cell=` 键解析，数据输入区只按行优先顺序收集纯数值填入 `MatrixBlock`，缺额自动补 0；`moduleSettingsTitle()` 返回「矩阵设置」，面板仅在选中本模块时由 `MainWindow::rebuildModulePanel()` 渲染，切到其他模块自动隐藏。测试新增 `tests/NewModuleBatchTest.cpp` 与 `ModuleSettingsPanelTest.cpp` 的面板显隐断言 |
| BUG-163 | 新增 9 个算法模块后，多处编译失败：头文件缺少 `supportsBidirectional()` 声明、`QString::arg()` 空参调用、`const int n` 重复定义、`GraphNode*` 与 `QGraphicsItem*` 类型不匹配、`SegmentBar` 未加入链接 | 已修复 | `PrimModule.h` / `BipartiteModule.h` / `EulerModule.h` 补齐 `supportsBidirectional()` 声明；`PrimModule.cpp` 移除多余 `.arg()`；`CountingSortModule.cpp` 将重复的 `const int n` 合并为 `const int count`；`TrieModule.cpp` 节点指针改用 `static_cast<GraphNode*>`；新增 `src/render/SegmentBar.h/.cpp` 并同步登记到 `CMakeLists.txt` 与 `tests/CMakeLists.txt`；`HashTableModule.cpp` 删除无声明的 `buildLabels()` |
| BUG-159 | 打开算法模块下拉列表时，弹层窗口**最右侧与最下方**各有一条贯穿整个弹层高度的深灰硬竖线（RGB 140,140,141），位于卡片外侧与窗口边界之间的空白区 | 已修复 | 根因不在自绘阴影，而在 **Windows 给 `Qt::Popup` 顶层窗口绘制的原生 DWM 边框**：`Qt::FramelessWindowHint` 只去掉标题栏，不会去掉 DWM 的 1px 描边。修复：`ComboPopupFrame` 构造时与 `MyComboBox::openPopup()` 中均追加 `Qt::NoDropShadowWindowHint`（0x40000000），关闭系统阴影与原生描边；阴影完全由 `ComboPopupFrame::paintEvent` 自绘。同时候 `ComboPopupFrame::paintEvent` 的多层同心圆角矩形扩散半径由 `i*2.2`（最外层 26.4px）收敛为 `maxSpreadX=11 / maxSpreadY=6`，避免阴影被窗口边界裁切成折线；弹层外边距由 (27,18,27,20) 调整为 (18,12,18,14)。像素实测：修复前窗口右边界 x=1220 为 `140,140,141` 硬线，修复后该列变为纯净 `251,252,254`，右/下边缘均为平滑渐变 |
| BUG-160 | 切换算法模块时侧栏崩溃（SIGSEGV，`MainWindow::updateGraphOptionPanel` → `QObject::blockSignals` 空指针） | 已修复 | 根因：`MainWindow::rebuildModulePanel()` 清空 `modulePanelContent` 时对每个子控件调用 `deleteLater()`，其中包含**跨模块复用的成员控件** `toggleDirected` / `checkBidirectional`；随后 `rebuildModulePanel()` 又把这两个已析构的成员指针重新加入新面板，造成悬空指针。修复：清空前先 `setParent(modulePanelHost)` 把复用控件移出待删列表，并跳过对二者的 `deleteLater()`；非图模块下显式 `setVisible(false)` 隐藏二者。另修复 `updateGraphOptionPanel()` 与 `animateSubOptionVisibility()` 中因语句粘连（`if (!supported) return;` 与下一语句同行）导致的不可达代码 |
| BUG-161 | `SmartGroundUiBehaviorTest` 断言图面板（`graphPanel`）在图模块下可见，与 BUG-158 合并后的「特化设置」单面板设计冲突 | 已修复 | 测试改为按标题查找 `MyCollapsible`（新增 `headerText()` 访问器），定位到「特化设置」面板再断言可见性；断言文案同步由「graph panel」改为「settings panel」 |
| BUG-153 | 算法模块下拉弹层外侧残留一圈包裹整个结构的硬边阴影线（右侧与下侧尤为明显，与卡片之间有明显空隙） | 已修复 | 移除 `ComboCard` 上的 `QGraphicsDropShadowEffect`（该控件会把阴影裁切到自身矩形，形成硬边）；`ComboPopupFrame::paintEvent` 改为 12 层由外到内的同心圆角矩形叠加绘制柔和渐隐阴影，边缘不再出现折线；弹层外边距由 (20,18,20,20) 调整为 (27,18,27,20) 使阴影四周对称 |
| BUG-154 | 图结点权值双击修改后，新值仍显示在圆形结点下方（`valueText` 位置 y=22）而非圆内 | 已修复 | `GraphNode::commitEdit()` 统一走 `setCenterText()`（绘制于 `QRectF(-20,-20,40,40)`）并清除残留的 `valueText`；`IModule::refreshLiveSlots()` 同步改为写入圆内文本；`GraphNode::beginEdit()` 预填圆内/自定义/下方文本优先级；新增 `GraphNode::belowText()` 访问器与 `editorItem()` 便于测试 |
| BUG-155 | 行列式 / 矩阵 / 表格模块仅有行列式形态可改，其余功能残缺；形态切换与行列数调整散落、无法在统一面板内操作 | 已修复 | `MatrixBlock` 重写：新增 `sideMargin()` / `setCellSize()`，行列式与表格形态使用真正的左右竖线括号（`barColor` 独立配色），单元格改为 `mono(13)` 且支持双击就地改值；`SandboxMatrixModule` 支持 `rows=` / `cols=` / `type=` / `cell=` 输入键，三种形态（矩阵 / 行列式 / 表格）可切换，行列数与单元格尺寸可在「特化设置」面板中调整并保留已有数据；`buildLayout` 保留用户已覆盖的形态与尺寸设置，避免重建时回退 |
| BUG-156 | 左侧存在三个下拉视觉风格（Vue 扁平 / 柔光玻璃 / 极简描边），风格切换器冗余 | 已修复 | `ComboVisualStyle` 仅保留 `VueLight`；删除 `paintGlassTile` / `paintMinimalTile` 与 `ComboCard` / `ComboPopupFrame` 中的对应分支；移除 `MainWindow` 侧栏的风格切换器（`comboStylePicker`）；`MyComboBox::styleNames()` 只返回 `{"Vue 扁平"}` |
| BUG-157 | 算法模块下拉内的文字偏小，可读性不足 | 已修复 | 选项字号由 9pt 提升至 10pt（`kOptionFontSize`），分组标题 9pt → 10pt（`kHeaderFontSize`），按钮文字 10pt → 11pt（`kButtonFontSize`）；选项内边距由 34 → 40，列宽由 `qBound(120, ..., 240)` 放宽为 `qBound(140, ..., 300)`，长名称（如「自由结构 - 矩阵 / 行列式」）不再截断 |
| BUG-158 | 图结构选项与模块设置分成两块独立面板，割裂且占用侧栏空间 | 已修复 | 合并为唯一的「特化设置」折叠面板：`MainWindow::rebuildModulePanel()` 在图模块下自动插入「图的类型」开关、「无向边加双向箭头」复选框与边权提示，随后追加模块自身的 `ModuleField`；`updateGraphOptionPanel()` 不再单独显示 `graphPanelHost`；分段控件独占整行，数值输入框固定 72px 宽 |
| BUG-141 | 树状数组指针箭头与数组下标行重合，多个算法的指针 / 文字 / 单元格互相压盖 | 已修复 | `PointerBlock` 重构：新增 `setShaftLength()` / `setLabelOffset()` / `setLabelAbove()`，箭头尖端统一落在原点，标签位置与杆长解耦，`boundingRect()` 改为「杆身 ∪ 标签」并集；树状数组下标行改为独立文本置于方块上方，指针 `i` 自下而上指向目标块底边、标签落在层间空隙；新增 `tests/OverlapAuditTest.cpp` 做全模块几何级自动审计（BUG-141） |
| BUG-142 | 链式前向星四组数组竖排并置，指针 k 与 head[u] 压住单元格与下标行 | 已修复 | 四组数组重构为「四行横排」（`head/to/nxt/weight` 各占一行），指针 `k` 与 `head[u]` 均自下向上指，标题移入左侧独立列，图节点右移避开数组区（BUG-142） |
| BUG-143 | 堆调整 / 堆排序的数组标题压住完全二叉树根节点，树结构标题压住根节点 | 已修复 | `HeapTreeBase::layoutHeap()` 中数组标题上移到 `arrY - 54`、树标题上移到 `treeTop - 92`，与节点包围盒彻底分离（BUG-143） |
| BUG-144 | 线段树指针 node 锚定在节点包围盒内部 | 已修复 | 锚点偏移由 `0,46` 调整为 `0,80`，指针杆身与标签完全脱离节点包围盒（BUG-144） |
| BUG-145 | 拓扑排序 H/T 指针压住队列单元格与下标行，结点层与标题重叠 | 已修复 | 结点层布局由 `80 + j*90` 下移到 `150 + j*90`；输出序列行由 `degreeY+200` 下移到 `degreeY+270`；`playPointerArrange` 偏移改为 40（BUG-145） |
| BUG-146 | 双击数组单元格改值时数字键 1-7 被画布快捷键抢占，无法输入 | 已修复 | 新增 `src/core/TextEditGuard.h`：检测 `QApplication::focusWidget()` 或 `QGraphicsScene::focusItem()` 处于文本编辑态时，`CanvasView` 与 `MainWindow` 跳过快捷键拦截（BUG-146） |
| BUG-147 | 表格被关闭或删除后，上方调整工具栏仍悬浮在画布上 | 已修复 | `TableToolBar` 增加表格对象有效性校验，表格删除 / 取消选中即刻隐藏（BUG-147） |
| BUG-148 | 框选多个元素删除后仍有残留批注与活动表格态 | 已修复 | `ToolController::deleteSelected()` 同步清空 `m_activeTable`、发 `tableActivated(nullptr)` 并取消全部选中（BUG-148） |
| BUG-149 | 箭头笔尖不在直线最尖端，鼠标停点与箭头位置有偏差 | 已修复 | `ShapeItem` 箭头多边形顶点严格落在 `m_end`，杆身截至 `m_end - (ux,uy) * head`，不再穿出箭头（BUG-149） |
| BUG-150 | 文本框与 Markdown 输入后必须点击别处才刷新预览 | 已修复 | `TextBoxItem::notifyContentChanged()` 每次按键即触发重绘；`MarkdownItem` 拆分为 `m_editor` + `m_preview` 双文档，输入时实时重排预览（BUG-150） |
| BUG-151 | 全项目中文与西文字体混杂，中文未使用微软雅黑、西文未使用 Consolas | 已修复 | 新增 `src/ui/UiFonts.h` 字体引擎：`cjk()` / `mono()` / `mixed()` 三档，全项目改用该引擎（BUG-151） |
| BUG-152 | 算法模块下拉弹层瓦片廉价生硬，缺少悬停 / 点击动效 | 已修复 | `ComboOption` 重做为圆角 11px 渐变瓦片 + hover 柔光扫掠 + 按下收缩，新增 `hoverProgress` / `pressProgress` 属性驱动 `QPropertyAnimation`；分组瓦片继承分类主色；弹层开合加透明度过渡（BUG-152） |
| BUG-131 | 并查集路径压缩模块 pa 数组下标越界导致崩溃 | 已修复 | pa/sz 改 1-based；liveValue 兜底值 -1 先校验再写入；无覆盖值时回落到 buildLayout 解析的 m_parents（BUG-131） |
| BUG-132 | 树的直径第二轮 DFS 只沿子节点方向遍历，直径算短 | 已修复 | 两轮 DFS 统一走无向邻接表；第二轮记录 parent2[] 回溯真实路径；TD_PATH 帧区分汇总与逐段点亮（BUG-132） |
| BUG-133 | 快速幂模块 qmul 内层循环未展开，竖式乘法面板信息缺失 | 已修复 | qmul 展开为逐帧录制，新增 FP_QMUL_INIT/ROW/ACC/DONE 四个动作类型（BUG-133） |
| BUG-134 | 埃氏筛把被划掉的数字整格清空，学生看不到原值 | 已修复 | ArrayBlock 新增 `m_struck` 划除标记；`applyCross()` 保留数字、置灰底色、叠加删除线并缩放至 0.82（BUG-134） |
| BUG-135 | 自动播放时右下角帧滑块与帧计数徽标不跟随动画前进 | 已修复 | `MainWindow::advanceFrame()` 增加 `syncFrameSlider()` 调用，播放中实时同步滑块位置与 "N / M" 徽标（BUG-135） |
| BUG-136 | 埃氏筛每个倍数单独占一帧，帧数爆炸且缺少流动感 | 已修复 | 新增 `SV_CROSS_BATCH` 动作；同一素数的全部倍数合并为一帧，用 `QVariantAnimation` 按顺序流动划掉并逐格发出涟漪（BUG-136） |
| BUG-137 | 快速幂字与下标重叠、含 qmul 面板、帧数过多，且 `a` 在精确模式下整型溢出显示为 0 | 已修复 | 删除 qmul 与 `FP_QMUL_*`/`FP_SQUARE`/`FP_SHIFT` 动作；新增 `AnimFrame::big1~big4` 承载 64 位数值；二进制位条右端为第 0 位，每位合并为一帧；精确值超出 64 位范围时提示改用取模分支（BUG-137） |
| BUG-138 | 树的直径节点标签 `dep=...` 过长，被相邻节点遮挡 | 已修复 | `layoutTreeScene()` 叶子间距由 104.0 放宽到 178.0，节点标签互不重叠（BUG-138） |
| BUG-139 | 堆优化 Dijkstra 左侧画二叉堆树，元素无法完整显示，且入堆没有推移过程 | 已修复 | 删除堆树节点与连线，改为数组化 `pq (堆)`；单元格加宽到 98.0 并用 11 号字显示 `pair(dis, u)`；入堆时 `k` 之后元素整体右移一格（BUG-139） |
| BUG-140 | 链式前向星指针 k 与 to[] 单元格重叠，且染色瞬间完成无反馈 | 已修复 | 四列 X 间距放宽到 150；指针 k 改用 `PointerBlock::Right` 从 to 列左侧指向；`highlightCell()` 增加 `playScalePulse` 高亮脉冲（BUG-140） |
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

## BUG-127 矩形选中后拖动时元素在原地疯狂震动

- **现象**：用矩形框选一批元素后，按住左键拖动，元素不跟随鼠标，而是在原地高频来回抖动。
- **根因**：`ToolController::updateMoveSelection()` 把**增量位移**与**按下时的初始位置**混用——`delta = pos - m_moveLastPos` 是相邻两次鼠标事件之间的步长，却直接加到按下瞬间记录的 `m_moveOrigins`（绝对初始坐标）上。元素因此只会停在「初始位置 + 最后一步步长」这个固定点附近，反复移动鼠标就在该点周围抖动，永远跟不上下游标。
- **修复**：`beginMoveSelection()` 额外记录按下点 `m_movePressPos`；`updateMoveSelection()` 改为按**累计位移** `total = pos - m_movePressPos` 计算目标位置 `origin + total`，位置只与光标相对按下点的偏移有关，与事件步长无关。鼠标未移动（步长为零）时直接跳过重绘，避免无效刷新。
- **验证**：新增 `SmartGroundSmartCanvas` 用例「拖动累计位移等于整段光标行程」，断言 9 段移动后总偏移精确等于 `(90, 45)`。

## BUG-128 编辑态下全局快捷键未隔离，数字键触发画笔颜色

- **现象**：落下文本框 / Markdown 笔记后开始输入，按数字键 `1`–`7`，左下角状态栏仍显示「画笔颜色 #…」，按字母键还会切换工具。
- **根因**：两层问题叠加。
  1. 画布的键盘焦点在**算法下拉框**（`MyComboBox`）上，事件沿 `MyComboBox → QWidget → MainWindow` 冒泡，`MainWindow::keyPressEvent()` 无条件执行 `applyColorShortcut()` 与工具切换分支，完全不知道画布正处于行内编辑状态。
  2. 画布 `viewport()` 上虽装了 `ToolController` 事件过滤器，但焦点在 view 上时按键根本不会送到 viewport，过滤器形同虚设。
- **修复**：
  - `ToolController` 新增 `isInlineEditing()`，行内编辑期间 `onKeyPress()` 只把按键交给 `m_editingItem->handleEditKey()` 并吞掉事件，不落入任何全局快捷键分支；`applyColorShortcut()` 在编辑态直接返回 `false`。
  - `MainWindow::keyPressEvent()` 在 `Ctrl` 组合键与普通键两段入口都先判断 `isInlineEditing()`，命中即 `accept()` 并返回。
  - `ToolController::beginInlineEdit()` 主动把焦点交给 `m_view->viewport()`，使按键稳定落在画布而非下拉框。
  - 新增 `ToolController::handleKeyPress()`（返回是否处理）与 `CanvasView::keyPressEvent()`，让落在 view 上的按键也能进入控制器；同时 `onKeyPress()` 改为返回布尔值，未处理的普通按键仍交回 `QGraphicsView`，不吞掉空格平移等既有交互。
- **验证**：`SmartGroundSmartCanvas` 断言「编辑态下数字快捷键不改变画笔颜色」「字母快捷键不切换工具」「输入字符仍能进入文本框」。

## BUG-129 颜色选择器顶部预览只显示黑色，取色条不连续

- **现象**：画笔 / 荧光笔设置面板顶部的预览框始终是一片黑色；「渐变预设」标签语义不准；色相条两端不是纯黑到纯白，取色无法连续调节。
- **根因**：`MyColorPicker` 用一个普通 `QWidget` 当预览容器并给它写了不透明样式 `background:#1f2733`，该子控件完全覆盖了父控件 `paintEvent()` 里绘制的渐变条，父控件的绘制根本看不见；同时色相条用 `QColor::fromHsv(360, …)` 作渐变端点，而 360 不是合法色相（合法范围 0–359），端点退化成了黑色。
- **修复**：重写 `MyColorPicker`。
  - 预览改为自绘 `ColorPreview`，纯色填充 + 圆角描边，颜色跟随当前取值。
  - 新增自绘 `ColorField`：上半部分是**饱和度 × 明度**二维取色区（白→纯色横向渐变叠加黑色纵向渐变），下半部分是**连续色相条**（0–359° 循环渐变 + 可拖拽指示圈）。两处都支持按下即定位与拖动实时取色。
  - 色相渐变端点改用 359°，并对无彩色（灰色）单独做色相归一化，避免 `QColor::hue()` 返回 -1 造成取色跳变。
  - 「渐变预设」标签改为「预设」；色相滑条量程改为 0–359。
  - 色值、RGB 数字、滑条与预览通过 `syncInputs()` 双向同步，并加 `m_syncing` 标志防止信号回环。
- **验证**：新增 `SmartGroundColorPickerTest`，逐像素采样色相条断言「左端偏红 / 中段偏绿 / 经过品红 / 右端回绕到红」且不是纯色；断言预览显示当前色而非黑色；断言「预设」标签存在、「渐变预设」已移除；模拟拖动色相条断言确实选出了新颜色。

## BUG-130 表格插入多列时行列数与数据长度不同步，导致渲染越界闪退

- **现象**：在表格上下文工具栏执行「插入 2 列」后再删除行列，画布重绘时崩溃（`QList::operator[]: index out of range`，栈在 `CanvasTableItem::paint()`）。
- **根因**：`CanvasTableItem::insertColumns(at, count)` 的循环体只插入**一列**（`m_data[i].insert(at, …)` 写在外层 for 内层没有按 `count` 展开），却把 `m_cols` 直接加了 `count`。数据行长度从此少于 `m_cols`，`paint()` 遍历 `j < m_cols` 时访问 `m_data[i][j]` 越界。
- **修复**：`insertColumns()` 改为按 `count` 循环插入；`insertRows()` / `removeRows()` / `removeColumns()` 统一走 `rebuildCells()` 归一化数据维度；`paint()`、`cellAt()`、`setCell()`、`setCellAlign()`、`cellAlign()` 全部补齐越界保护。
- **验证**：`SmartGroundSmartCanvas` 既有用例覆盖插入 / 删除行列后尺寸回落；本轮全量 54 个测试目标全部通过。

## BUG-131 并查集路径压缩模块的 pa 数组下标越界导致崩溃

- **现象**：`并查集 路径压缩` 模块一构建就崩溃（`0xC0000005`），无头测试直接触发访问违例。
- **根因**：`DSUPathCompressionModule::runAlgorithm()` 用 `liveValue("pa", i + 1, -1)` 读取父节点覆盖值，而 `liveValue()` 在**没有覆盖值**时返回传入的兜底值 `-1`。代码把这个 `-1` 直接写进 `pa[x - 1]`，随后 `while (pa[x - 1] != x)` 循环用 `-1` 下标访问数组，越界读；同时 `pa` 数组本身按 0-based 分配却用 1-based 结点编号访问，整体错位一格。
- **修复**：`pa` / `sz` 改为 1-based（`std::vector<int> pa(m_n + 1)`）；`liveValue` 的兜底值改用 `-1` 判断后再做边界校验才写入；未提供覆盖值时回落到 `buildLayout()` 解析出的 `m_parents`（同样做范围钳制）。
- **验证**：`SmartGroundBatch8` 新增 `并查集 路径压缩` 用例，断言 `DP_FIND` / `DP_COMPRESS` / `DP_DONE` 帧存在、场景非空、非法输入降级为空帧。

## BUG-132 树的直径第二轮 DFS 只沿子节点方向遍历，导致直径算短

- **现象**：`树的直径` 模块算出的直径长度恒小于真实值（测试树 `1 2 3 / 2 4 5 / 3 6 / 4 7` 期望 5，实测 4）。
- **根因**：两轮 DFS 都只遍历 `m_children[u]`（父→子方向）。第一轮从根出发尚可，但第二轮**从 A 出发**时，A 往往是叶子节点，其出边方向在 `m_children` 中不存在，导致搜索无法展开。
- **修复**：两轮 DFS 统一改为遍历**无向邻接表**（由 `m_children` 双向展开得到）；第二轮额外记录 `parent2[]` 以便回溯出真实路径；`TD_PATH` 帧改用 `group[0]` 区分「汇总帧」与「逐段点亮帧」，逐段帧携带前一结点编号用于精准点亮对应树边。
- **验证**：`SmartGroundBatch8` 断言直径长度为 5、端点为 7 与 6。

## BUG-133 快速幂模块 qmul 内层循环未展开，竖式乘法面板信息缺失

- **现象**：`快速幂` 模块右侧 `qmul` 面板只有标题没有过程，学员看不出竖式乘法的逐位推进。
- **根因**：`runAlgorithm()` 里的 `qmul` lambda 直接用算术表达式完成乘法，没有把 `while (b) { if (b & 1) res += a; b >>= 1; a += a; }` 的每一步录成动画帧。
- **修复**：把 `qmul` 展开为逐帧录制：新增 `FP_QMUL_INIT` / `FP_QMUL_ROW` / `FP_QMUL_ACC` / `FP_QMUL_DONE` 四个动作类型，面板实时显示「当前底数 a / 乘数 b / 累加 res」以及 `b >>= 1`、`a = (a + a) mod p` 的公式；`qpow` 外层的取位、平方、移位也各自独立成帧。
- **验证**：`SmartGroundBatch8` 断言 `3^13 mod 1000000007 = 1594323` 且 `FP_QMUL_ROW` 帧数大于 10。

## BUG-134 橡皮工具面板沿用画笔面板高度，导致设置卡片过高且无法选择橡皮形状

- **现象**：点开橡皮工具后弹出面板高度与画笔 / 荧光笔面板几乎一致（因为 `QStackedWidget` 的 `sizeHint()` 取所有页面最大值），实际橡皮只有一行大小滑块却撑出大片空白；同时橡皮只能圆形擦除，无法切换方块。
- **根因**：`CanvasToolBar::buildToolPanel()` 用 `QStackedWidget` 承载全部工具页，`QStackedLayout` 的 `sizeHint` 会返回所有页面的最大尺寸，于是矮页面（橡皮、马赛克）也被迫按最高页（画笔 + 取色器）定高；`ToolStyle` 里也没有记录橡皮形状的字段。
- **修复**：新增 `CanvasToolBar::applyToolPageSize()`，在切换 / 重建工具页时按当前页 `sizeHint` 固定 `m_toolPages` 尺寸，使每个工具面板高度独立贴合内容；`ToolStyle` 增加 `eraserSquare`，`ToolController` 增加 `eraserSquare()` / `setEraserSquare()`；`eraseAt()` 依形状分别走圆形 `hitTest` 与方形区域求交；画布橡皮光标同步绘制圆形或方框。
- **验证**：新增 `SmartGroundEraserPanel`，断言橡皮面板高度 103px（< 130px）且明显低于画笔面板；`eraser_panel.png` 可见「圆形 / 方形」分段选择器。

## BUG-135 算法模块下拉气泡线条杂乱、配色发白且带锯齿状阴影边框，观感廉价

- **现象**：算法模块下拉弹层的每组瓦片都带左侧色条 + 顶部高光 + 斜向扫光三层叠加，弹层外圈还套了 16 层同心圆环模拟阴影，形成明显的椭圆锯齿边框，整体偏灰偏白、棱角过多，与画笔弹出面板的现代风格割裂。
- **根因**：`ComboOption::paintTile()` 同时绘制渐变底、白色高光遮罩、左色条与斜扫光四种叠加；`ComboPopupFrame::paintEvent()` 用 16 层递增圆角矩形手工堆阴影，边缘必然出现台阶状锯齿。
- **修复**：`MyComboBox` 引入 `ComboVisualStyle`（`VueLight` / `SoftGlass` / `MinimalLine`）三套风格；`VueLight` 改为 Vue 式扁平瓦片（无色条、无扫光、悬停时仅 160ms 强调色淡入、选中项用强调色 34 透明底 + 左侧 3px 圆角指示条）；弹层阴影改用 `QGraphicsDropShadowEffect` 真投影；弹层卡片改 16px 圆角 + 极淡描边；分组标题由「色条 + 渐变尾线」简化为「6px 圆点 + 文字 + 渐隐细线」；下拉箭头加入开合旋转动画；下拉框文字按中西文自动切换 Consolas / 微软雅黑。
- **验证**：新增 `SmartGroundComboStyleShot` 生成 `style_vue_light.png` / `style_soft_glass.png` / `style_minimal_line.png` 三张对照图；`SmartGroundAppCombo` 断言真实主窗口下拉弹层仍渲染出分类标题（dark pixels = 16368）。

## BUG-136 左侧新增风格切换器后，启动面板像素审计误报「左栏存在强调色块」

- **现象**：`SmartGroundStartupPanelTest` 报 `FAIL: no accent-tinted block in left column at startup`（accent=1570）。
- **根因**：该测试用固定像素带 `y ∈ [110, 300)` 扫描左栏有无「图结构选项」面板残留；风格切换器 `MySegmentedControl` 的选中指示条正好使用强调色渐变（`m_accent.lighter(118) → m_accent`），落在该像素带内被误计入。断言本身没错，但扫描区域与新增控件重叠了。
- **修复**：测试改为按控件几何动态定位扫描带 —— 取 `comboStylePicker` 底边 +6px 作为起始，再向下取 190px 作为图表面板区，避免与风格指示器重叠。
- **验证**：修正后启动态 `accent=0`、选中图论模块后 `panelSurface=33730`，两条断言均通过；全量 62 个测试目标全绿。

## BUG-137 下拉气泡缺少全局风格切换入口，用户选定风格后无法运行时切换

- **现象**：`ComboVisualStyle` 三套风格虽已在 `MyComboBox` 内实现，但没有对外的切换入口，风格写死在 `MainWindow` 构造里。
- **根因**：`setVisualStyle()` 只作用于单个实例，且缺少全局状态同步机制。
- **修复**：`MyComboBox` 新增静态实例表 `g_instances` 与 `setGlobalVisualStyle()`，切换时同步刷新所有已存在实例；新增 `styleNames()` / `styleName()` / `styleIndex()` 供 UI 绑定；`MainWindow` 左侧面板在「算法模块」下方加入「界面风格」标签 + `MySegmentedControl`（Vue 扁平 / 柔光玻璃 / 极简描边），切换即时生效。
- **验证**：新增 `SmartGroundStylePicker`，断言三段选项存在、默认为 VueLight、三次切换分别落到 SoftGlass / MinimalLine / VueLight 且切回后弹层仍能正常打开。

## BUG-159 下拉弹层窗口右/下边缘残留贯穿式深灰硬线

- **现象**：打开算法模块下拉列表时，弹层**最右侧**与**最下方**各有一条贯穿整个弹层高度的深灰竖线（用户截图红框内），位于卡片外侧的空白区，与卡片之间有明显间隔。
- **像素实测（修复前，3840×2160 物理像素）**：弹层逻辑位置 (136,174)，尺寸 677×394，缩放 1.5 后窗口右边界在物理 x=1220。该列像素恒为 `140,140,141`（深灰），向下贯穿全部 591 行；底边 y=852 处同理 `140,140,141`。而卡片自身右边缘在 x=1191（`226,232,242`，浅蓝灰），二者相距 29px。
- **根因**：**Windows 桌面窗口管理器（DWM）给 `Qt::Popup` 顶层窗口绘制的原生 1px 描边**。`Qt::FramelessWindowHint` 只移除标题栏与系统按钮，并不影响 DWM 的窗口描边，因此即使 `WA_TranslucentBackground` 全透明渲染，窗口最外一圈仍会出现一圈深色硬边。在浅色画布上这条边非常显眼。
  - 自绘阴影并非元凶：`ComboPopupFrame::paintEvent` 原用 `spread = i * 2.2`（最外层 26.4px）而右边距仅 27px，理论上会被窗口边界裁切成折线，但 A/B 对照显示裁切处落在窗口**内部**（x=1191 卡片边框处），并非用户看到的 x=1220。
- **修复**：
  1. `ComboPopupFrame` 构造时追加 `setWindowFlags(windowFlags() | Qt::NoDropShadowWindowHint)`，`MyComboBox::openPopup()` 中改用 `frame->setWindowFlags(Qt::Popup | Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint)`，关闭系统阴影与 DWM 原生描边；阴影完全交由 `paintEvent` 自绘。
  2. `ComboPopupFrame::paintEvent` 的同心圆角矩形扩散半径由 `i * 2.2` 收敛为 `maxSpreadX = 11` / `maxSpreadY = 6`，并改为线性衰减 alpha，确保阴影层完全落在窗口边界内、不会产生任何折角。
  3. 弹层外边距由 (27,18,27,20) 调整为 (18,12,18,14)，窗口宽度由 `popupWidth + 54` 收窄为 `popupWidth + 36`。
- **验证**：新增回归测试 `SmartGroundPopupEdgeVerify`（真实屏幕像素级）。修复后窗口右边界 x=1220 列变为纯净 `251,252,254`，右/下边缘均为平滑渐变；A/B 对照确认移除 `Qt::NoDropShadowWindowHint` 后硬线立刻复现。

## BUG-160 切换算法模块时侧栏崩溃（悬空指针）

- **现象**：`SmartGroundUiBehaviorTest` 在切换到图论模块时 SIGSEGV 崩溃，栈顶为 `QObject::blockSignals(bool)` ← `MainWindow::updateGraphOptionPanel`（`MainWindow.cpp:738`）← `MainWindow::onModuleChanged`。
- **根因**：`MainWindow::rebuildModulePanel()` 清空 `modulePanelContent` 时对每个子控件调用 `deleteLater()`，其中包含**跨模块复用的成员控件** `toggleDirected`（`MyToggleSwitch`）与 `checkBidirectional`（`MyCheckBox`）；重建时又把这两个**已被析构**的成员指针重新加入新面板，形成悬空指针。同时 `updateGraphOptionPanel()` 与 `animateSubOptionVisibility()` 中因语句粘连（`if (!supported) return;` 与下一语句写在同一行）造成后续语句不可达。
- **修复**：
  1. `rebuildModulePanel()` 清空前先 `toggleDirected->setParent(modulePanelHost)` / `checkBidirectional->setParent(modulePanelHost)` 把复用控件移出待删集合，并在 `deleteLater()` 判断中显式跳过二者。
  2. 非图模块分支显式 `setVisible(false)` 隐藏这两个复用控件，图模块分支恢复 `setVisible(true)`。
  3. 拆开粘连语句：`updateGraphOptionPanel()` 的 `if (!supported) { ...; return; }` 独立成块；`animateSubOptionVisibility()` 的函数体首行独立成行。
- **验证**：`SmartGroundUiBehaviorTest` 恢复为 0 失败，21 轮模块/选项切换崩溃回归通过。

## BUG-161 UiBehavior 测试断言与「特化设置」单面板设计冲突

- **现象**：`SmartGroundUiBehaviorTest` 报 2 条失败：`graph panel VISIBLE for graph module` 与 `graph panel VISIBLE again after switching back`。
- **根因**：BUG-158 已把图结构选项合并进唯一的「特化设置」面板（`modulePanel`），`graphPanel`（`MyCollapsible`，标题「图结构选项」）成为不再显示的历史控件；而测试用 `findChild<MyCollapsible*>()` 取到的是**第一个**折叠面板，恰好是历史 `graphPanel`，断言对象错位。
- **修复**：测试改为按标题查找 —— `MyCollapsible` 新增 `headerText()` 访问器，测试遍历 `findChildren<MyCollapsible*>()` 匹配标题「特化设置」后再断言可见性；断言文案由「graph panel」改为「settings panel」。
- **验证**：`SmartGroundUiBehaviorTest` 全绿；全量 54 个测试套件零失败。

## BUG-162 指针首次出现时从原点 (0,0) 飞入，产生贯穿全图的连线

- **现象**：循环队列 `head`/`tail`、字符串哈希 `i`、LCS `i`/`j` 等模块在指针首次出现时，箭头从画布左上角 (0,0) 一路飞到目标单元格，视觉上像是被一根斜线"拽"过来。
- **根因**：`IModule::playPointerMove()` 直接以 `ptr->pos()` 作为动画起点，而这些指针在 `buildLayout()` 中创建后从未设置过位置，其 `pos()` 仍为默认的 (0,0)。首次调用时动画便从 (0,0) 开始插值。
- **修复**：`playPointerMove()` 增加起点判定 —— 若 `ptr->pos()` 为 `QPointF()`（默认未放置），则直接 `setPos(targetPos)` 瞬移到位，不启动动画；其余情况照常播放移动动画。
- **验证**：`SmartGroundBatch9Visual` 的指针审计项（cq / stringhash / lcs / bucket / radix / mq / trie / hash）全部转绿，`fail=0`。

## BUG-163 树状数组指针 i 落在被指向单元格内部并压住相邻格子

- **现象**：树状数组模块中红色指针 `i` 的箭头尖端插进目标 `t[i]` 方格内部，并与左右相邻的方格重叠；箭头方向也从水平（Right）中途变为竖直（Up），视觉上像是"躺"在格子里。
- **根因**：`FenwickTreeModule::movePointerTo()` 计算落点时用了 `t->pos() + QPointF(t->cellWidth() / 2.0, originY)`，其中 `originY` 已经是"格顶 + 格高"的绝对纵坐标，却又作为**相对偏移**加到 `t->pos().y()` 上，导致落点被整体下移一个 `t->pos().y()`；同时水平方向用了整块 `t` 的宽度求中点，而 `t[i]` 的方格实际只占该宽度的一部分（`t` 块按 lowbit 区间合并渲染），所以水平中心也偏了。
- **修复**：
  1. 落点改为 `t->pos() + QPointF(cellX, t->cellHeight())`，纵坐标使用**相对格高**的偏移，箭头尖端恰好落在方格下沿之外。
  2. 水平位置按所属 level 的区间换算：`cellX = (index - slot.left) * m_cellW + m_cellW / 2.0`，即落在 `t[i]` 实际占据的那一格正中。
  3. 指针方向统一为 `Up`（箭头从下方指向方格），标签偏移设为 `(-30, 0)` 避免压住相邻格。
- **验证**：`SmartGroundOverlapAudit` 的 `fenwick: no text/pointer overlap` 由 3 处冲突降为 0。

## BUG-164 链式前向星四列数组的行标题、下标与图区相互重叠

- **现象**：`head[u]` / `to[k]` / `nxt[k]` / `weight[k]` 四个行标题与第一列方格叠在一起；下标数字与相邻行的方格压边；右侧图区结点的下标数字与图结点叠在一起；`k` 指针的尾部标签压住 `nxt` 行的下标。
- **根因**：行标题放在 `m_arraysX - 104`，而 `weight[k]` 标签本身宽约 110px，实际右边界已经越过第一列方格（x=250）；下标行紧贴方格上沿（`rowY - 22`），文字实际高度 26px，底边越界 4px 压进方格；图区中心 x=1010、半径 130，与最右侧数组列（x≈978）仅差 32px，圆形布局的外圈结点正好落在下标数字上；`k` 指针用默认的"尾部标签"布局，标签落在箭头正下方 26+8px 处，正处于 `nxt` 行下标带内。
- **修复**：
  1. 行标题整体左移到 `m_arraysX - 178`，与第一列方格保持 48px 净距。
  2. 下标行上移到 `rowY - 32`，与自身方格留 10px、与上一行方格留 28px；各方块统一 `setShowLabel(false)`，避免与独立下标行重复。
  3. 图区中心右移到 x=1120，与数组区拉开距离。
  4. `k` 指针改用 `setLabelOffset(-38, 0)` 把标签放到箭头左侧，箭杆长度收短到 22。
- **验证**：`SmartGroundOverlapAudit` 的 `forwardstar: no text/pointer overlap` 由 16 处冲突降为 0；`SmartGroundLayoutFit` 的 `k pointer sits at the to row bottom edge` 由失败转通过（同时把该断言中写死的行号 248 改为按行序动态定位，不再随布局常量漂移）。

## BUG-165 堆模块的 heap[] 标题与下标行、二叉树标题与结点标签重叠

- **现象**：`heap[]（数组存储）` 标题压在数组第一列的下标数字上；`完全二叉树结构（...）` 标题压在根结点的下标标签上。
- **根因**：两个标题分别放在 `centerX - (n*cellW)/2 - 10`（紧贴第一列左侧）与 `treeTop - 54`（正好落在根结点 subLabel 的 y 带内）。
- **修复**：`heap[]（数组存储）` 下移到数组行下方（`arrY + 48`），关系文本同步下移到 `arrY + 92`；树结构标题上移到 `treeTop - 96`，与根结点标签带保持 26px 净距。
- **验证**：`SmartGroundOverlapAudit` 的 `heapadjust` 与 `heapsort` 两项各由 5 处冲突降为 0。

## BUG-166 GroupAUi 测试断言使用了过时的模块名

- **现象**：`SmartGroundGroupAUi` 报 6 条失败：`has complete knapsack`、`has hungarian`、`has heap adjust` 以及对应的 3 条 `selectable:` 断言。
- **根因**：模块在本轮升级中已更名为「完全背包 DP」「二分图匹配 - 匈牙利算法」「堆调整 up / down」，但 `tests/GroupAUiTest.cpp` 仍按旧名断言，导致 `QStringList::contains()` 全部落空。
- **修复**：同步测试断言与 `targets` 选择列表为新模块名。
- **验证**：`SmartGroundGroupAUi` 恢复 0 失败。

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

## 2026-11-20 三个模块的深度重构（LCS / 匈牙利 / 树堆）

- **现象**：这三类模块的动作已具备，但缺少「视觉主角与空间编排」——LCS 的回溯是逐格闪现而非一条连续路径；匈牙利缺少左右分栏的版式与一条贯穿的增广路；树与堆的下沉过程没有一条可跟随的轨迹线。
- **根因**：`IModule` 缺少「多段折线轨迹」这一表达能力，`GraphElements` 也没有对应的图形元素。
- **修复**：
  - `src/render/GraphElements.{h,cpp}` 新增 `PathTrail`：可设置任意折线点集，按 `progress` 沿路径**渐进绘制**（de Casteljau 风格的逐段插值），末端带光晕 + 内核 + 白环的「行进头」，并叠一层低透明度加粗辉光；`setPoints` / `appendPoint` / `clearPoints` / `setProgress` / `setTrailWidth` / `setHeadRadius` 全部可动画；
  - `src/modules/IModule.{h,cpp}` 新增 `playPathTrail(trail, points, color, duration)`（整条路径一次性绘制）与 `playPathTrailAppend(trail, point, color, duration)`（追加一段并从上次进度续画），均遵守 `m_animated == false` 的瞬时生效约束；
  - **LCS**：`runAlgorithm()` 的回溯段改为记录**完整走位** `walk`（含每一步退让的落点），`LC_TRACE` 帧携带已走过的路径点序列；`renderFrame` 把这些点连成折线交给 `PathTrail`，形成一条沿对角线不断延伸、头亮点跟着回溯位置走的**连续路径**；匹配到的格子另走 `playCellLanding` 与涟漪，未匹配的退让步只更新路径不惊动格子；
  - **匈牙利算法**：`buildLayout()` 新增左右两条虚线分栏带（左部浅蓝 / 右部浅橙，`Z = -2`）把两列结点明确分区；新增 `PathTrail` 作为**增广路主角**——`HN_START` 从当前左部顶点起笔，`HN_TRY` 追加右部顶点，`HN_OCCUPIED` 追加被占用的持有者（转红），`HN_ASSIGN` 追加最终配对点（转绿），`HN_DONE` 冻结为绿色并对全部匹配顶点做收束缩放；
  - **树与堆**：`HeapTreeBase` 新增 `ensureDescentTrail` / `clearDescentTrail` / `appendDescent` / `freezeDescentTrail` 四个助手与 `m_descentTrail` 成员；`HeapSortModule` / `HeapAdjustModule` 在比较与交换帧把当前结点位置追加进轨迹，形成**逐层下沉**的可见路径，终帧冻结为有序色；`SegmentTreeBuildModule` / `SegmentTreeQueryModule` / `SegmentTreeTagModule` 同样接入（`SegmentTreeEditBase` 提供共享实现），下降帧同时播放 `playEdgeTravel` 边行进与轨迹延伸，形成「树往下走」的连贯主线。
- **验证**：`SmartGround` 主程序零编译错误；`build/tests/` 下 **79 个测试可执行文件全部零失败**。

## 2026-11-20 全局动画美学升级（以哈夫曼树为基准）

- **现象**：哈夫曼树的动画成为项目美学基准后，全场模块的动效密度、色彩语义与「路径感」不统一；部分模块只有静态换色，没有飞入、涟漪、边行进等可读的动作链。
- **根因**：`IModule` 早期只提供 `playSwapAnim` / `playScalePulse` / `playValueFlight` 等少量基础助手，各模块各自零散实现，缺少统一的「边行进 / 发现涟漪 / 落位脉冲」词汇表。
- **修复**：
  - `src/render/GraphElements.h` / `.cpp` 新增 `TracerDot`（带光晕与内核的移动光点，用于边行进轨迹）；
  - `src/modules/IModule.h` / `.cpp` 新增共享编排助手：`playEdgeTravel`（沿边飞行的 tracer）、`playDiscover`（结点发现涟漪）、`playEdgeDiscover`（边先到、结点后醒）、`playNodeWake`（结点苏醒脉冲）、`playFocusRing`（焦点环）、`playValueArcFlight`（数值弧线飞行）、`playCellSweep`（逐格错峰扫描）、`playCellLanding`（落位脉冲）；
  - 按族批量落地：图算法（BFS / DFS / Dijkstra×2 / 拓扑排序 / Prim / Kruskal / Tarjan×2 / 二分图 / 欧拉 / 匈牙利）、树与堆（堆排序 / 堆调整 / Trie / 哈希表 / 链式前向星 / 线段树×3）、排序（快排 / 希尔 / 冒泡 / 选择 / 插入 / 归并 / 计数 / 基数 / 桶）、查与算（二分×2 / 快速幂 / 树状数组 / LIS / 区间调度 / GCD / 单调队列 / 单调栈 / 并查集×2 / 树直径 / 树子树大小 / 背包×2 / 字符串哈希 / 筛法）、矩阵与数论（LCS / 扩展欧几里得）、数据结构（栈 / 队列 / 循环队列 / KMP pi 数组）；
  - 语义统一为 idle / scanning / active / matched / terminal 五态色，扫描与匹配一律带涟漪或落位脉冲，终态一律带收束缩放。
- **验证**：`SmartGround` 主程序零编译错误；`tests/TmpSliderAudit.cpp` 进度条同步审计 `ALL PASS fail=0`（滑块值与帧号逐帧一致）。

## 2026-11-20 画笔换色不生效 / 画笔菜单不自关闭

- **现象**：在画笔菜单里改颜色，颜色块变了但画笔实际画出的颜色不变；同时打开画笔菜单后点击画布，菜单不会自动收起。
- **根因（换色）**：`CanvasToolBar` 的取色回调先调 `ToolController::setStyleFor(tool, s)`（已把新颜色写进 `m_styles`），再调 `ToolController::setPenColor(c)`；而 `setPenColor` 的实现是「若当前样式颜色已等于新颜色则直接 return」，于是它**总是提前返回**，`applySelectionStyle()` 从未执行，已选中元素不会被重新上色。更根本的是，正在绘制中的笔迹（`m_activeStroke`）与正在拖拽中的图形（`m_activeShape`）根本不在这条更新链路里，换色对它们完全无效。
- **修复**：
  - `src/canvas/ToolController.cpp` 新增 `applyActiveStyle()`：把当前笔迹的 `m_activeStroke` 与当前图形的 `m_activeShape` 的 `setPenColor` / `setPenWidth` / `setFillOpacity` 一并刷新；
  - `setPenColor` / `setPenWidth` / `setOpacity` 改为「样式写入」与「样式应用」两段分离：写入部分只在值真的变化时才执行并 `emit styleChanged()`，**应用部分无条件执行** `applySelectionStyle()` + `applyActiveStyle()`；原先的提前 return 一并移除；
  - `setStyleFor` 在 `tool == m_tool` 时同样补上 `applyActiveStyle()`，保证从工具面板改样式与从取色器改颜色两条路径行为一致。
- **根因（菜单）**：画笔飞出面板原本没有「点击面板外自动收起」的机制。修复：`CanvasToolBar` 挂 `qApp->installEventFilter(this)`，在 `eventFilter` 中对 `QEvent::MouseButtonPress` 判定按下点是否落在工具面板 / 更多面板 / 工具条本身之内；落在之外则调用 `closeFlyouts()` 收起全部浮层。
- **验证**：新增 `tests/TmpBrushDiag.cpp`（`SmartGroundTmpBrushDiag`）三段断言全绿 —— 取色器改色后笔迹使用新颜色、已选中元素随换色重绘、飞出菜单点击画布后收起且点击面板内部不收起、三角形内部像素采样 **0 / 625 不透明**（确认无背景填充）。

## 2026-11-20 图论激光拖影 / 碎片残留 / 编辑器崩溃 / Tarjan 数组重叠 四项修复

- **现象（激光拖影与碎片波纹）**：所有图论算法在 `u -> v` 的边延伸动画中，画面上出现多条残留的动画碎片、扇贝状波纹与卡死的光束。
- **根因（几何）**：`GraphEdge::paint` 把光束当作**直线**绘制 —— 用 `drawLine(beamStart, beamEnd)` 连接二次贝塞尔曲线上参数 `tail` 与 `head` 两点。弦与曲线之间存在最大可达数十像素的偏离，直线段因此既不贴合路径，又在每个动画帧切换时留下断裂的短线段碎片。此外原实现用 26 段离散折线近似曲线，制造出「串珠 / 波纹」观感。
- **修复（几何）**：`src/render/GraphElements.{h,cpp}` 新增 `GraphEdge::splitQuad`（de Casteljau 二次曲线二分）与 `GraphEdge::quadSubPath`（按参数区间 `from..to` 抽取子曲线生成 `QPainterPath`），光束改为**沿子曲线 `drawPath` 描边**；辉光层（9.0px）与内核层（3.4px）均使用沿路径首尾的 `QLinearGradient`，末端亮点用 `beamPath.pointAtPercent(1.0)` 精确落在曲线终点。
- **修复（残留）**：`IModule::startBeam` 用 `m_beamAnims` 注册表保证**每条边同一时刻只有一条活跃光束**；`purgeTransientItems()` 与析构统一 `stop()` + `clearBeam()`；光束动画 `finished` 时主动 `clearBeam()`。
- **现象（编辑器崩溃）**：`SmartGroundGraphInteraction` 间歇性段错误（退出码 `0xC0000005`），30 次连跑失败 5 次。
- **根因（崩溃）**：`GraphEdge` / `GraphNode` / `ArrayBlock` 析构时，其子项编辑器 `QGraphicsTextItem` 随之析构；Qt 在 `QGraphicsItem` 基类析构中发出 `FocusOut` 事件，被 `EditorWatcher` 事件过滤器拦截后回调 `commitEdit()`，而此时 `QTextDocument` 已开始释放 —— `toPlainText()` 访问已释放内存导致崩溃。栈回溯确认于 `QTextDocument::toPlainText() <- GraphEdge::commitEdit() <- EditorWatcher::eventFilter() <- ~QGraphicsItem`。
- **修复（崩溃）**：`src/render/GraphElements.{h,cpp}` 新增 `EditorTextItem`（`QGraphicsTextItem` 子类）与共享 `EditorGuard` 标志位：析构时置位 `tearingDown`，`EditorWatcher` 收到 `FocusOut` 时若标志已置位则直接吞掉事件、绝不回调 `commitEdit()`；三个可编辑图元的析构函数均先置位标志再主动 `delete` 编辑器子项，从根上杜绝析构期回调。
- **现象（Tarjan 数组重叠）**：`Tarjan 割点与桥` 的 `dfn[]` / `low[]` 两组数组与文字标签明显重叠。
- **根因（重叠）**：原始布局 `dfnX = 120`、`dfnY = 500`，而 `low[]` 标签被放在 `(114, 530)` —— 恰好压在 `dfn[]` 数组行（y 500~540）的窗格上，重叠面积约 620px²。
- **修复（重叠）**：`src/modules/TarjanCutVertexBridgeModule.cpp` 的 `buildLayout()` 重排版式 —— 数组行下移至 `dfnY = 545`、行距 `rowGap = 100`、起点 `dfnX = 170`，`dfn[]` / `low[]` 标签左移到 `x = 48` 的独立列，与首列窗格保持 49px 净距。
- **验证**：
  - 新增 `tests/BeamCurveTest.cpp`（`SmartGroundBeamCurve`）：光束端点与原曲线偏差 **0.00000000 px**；
  - 新增 `tests/BeamRenderTest.cpp`（`SmartGroundBeamRender`）：19 个进度档位共 **1159 个弧线采样点零断裂**、**351 个超出光束头部的采样点零残留**；
  - `SmartGroundStrictAudit` 覆盖 58 个模块的每一动画帧（脚本文字可见 / 隐藏两轮）：`TOTAL OVERLAP CONFLICTS = 0`、`TOTAL TIP FAILURES = 0`；
  - `SmartGroundGraphInteraction` 连跑 **30 次零失败**（修复前 30 次失败 5 次）；
  - `build/tests/` 下全部测试可执行文件零失败；`SmartGround.exe` Release 构建成功。

## 2026-11-20 数组下标位置 / 指针指向 / 图形空心实心 三项回归修复

- **现象（数组下标）**：所有数组的下标被从单元格**上方**挪进了**格子内部左上角**，且格内的值随之整体上移、偏离单元格中心。
- **根因（下标）**：`src/render/GraphElements.cpp` 的 `ArrayBlock::paint()` 在上一轮为规避「下标与窗格重叠」的审计告警，把下标改成 `drawText(QRectF(3, 0, m_cellW - 6, indexH), Qt::AlignLeft | Qt::AlignVCenter, topText)` —— 绘制在窗格矩形内部左上角；同时把值区改成 `QRectF(0, indexH, m_cellW, m_cellH - indexH)`，导致值在剩余区域居中而非整格居中。这是一次擅自决断，违背了「下标必须在单元格上方、值必须水平垂直居中」的既定要求。
- **修复（下标）**：
  - `ArrayBlock::paint()` 恢复为**下标在格子正上方**：绘制矩形 `QRectF(0, -kIndexGap - kIndexHeight, m_cellW, kIndexHeight)`，用 `Qt::AlignHCenter | Qt::AlignBottom` 水平居中并贴住底边，与顶边框保持 `kIndexGap = 6.0` 的净距，不重合；
  - 单元格值恢复为在**整个窗格** `QRectF(0, 0, m_cellW, m_cellH)` 内 `Qt::AlignCenter` 绘制，水平垂直双向居中；
  - `ArrayBlock` 新增 `kIndexGap` / `kIndexHeight` 常量与 `indexRect()` 访问器；`boundingRect()` 上边界相应上移到 `-(kIndexGap + kIndexHeight)`，保证下标不被裁剪。
- **现象（指针指向）**：二分查找等模块的指针不再指向目标格子，提示变量（`i` / `j` / `L` / `R`）与对应指针也对不上号。
- **根因（指针）**：`IModule::layoutPointerSlots()` 在同一格挂多个指针时执行 `tips[idx] += QPointF(offset, 0.0)`，**直接横向平移了箭头尖端**，使尖端偏离单元格宽度中心；同时 `resolveVisualCollisions()` 的避让策略以**横向平移标签**为主（`bestShift = QPointF(dx, 0.0)`），标签被推离自己的指针轴线，`i` / `j` / `L` / `R` 与指针的对应关系随之错乱。
- **修复（指针）**：
  - `layoutPointerSlots()` 删除尖端横向偏移，同格多指针改为**沿轴线分行**（`setLabelRow`），箭头尖端始终精确落在单元格宽度中心 / 竖直中点；
  - `resolveVisualCollisions()` 重写为**轴向优先**求解：垂直指针（Up / Down）候选位置沿轴线由近及远堆叠（最多 8 行，步长 22px），水平指针（Left / Right）沿轴向左右让位；横向平移只作为水平指针的兜底，不再用于垂直指针，因此垂直指针的标签始终落在自己的轴线上；
  - `separatePointerLabels()` 垂直指针改为**纵向分离**（沿轴线推远 22px），不再横向平移；
  - `PointerBlock::paint()` 修正水平箭头方向：`Right` 的箭头尖由原来的朝左改为**朝右**（`drawLine(0, 0, 9 * sign, ±7)`），`Left` 朝左 —— 此前两者箭头尖同朝左，「从左往右指向单元格」的语义是错的；
  - `PointerBlock::defaultLabelOffset()` / `tailLabelOffset()` / `setLabelToRight()` 同步修正：`Left` 的标签放在杆的右外侧、`Right` 放在杆的左外侧，与修正后的箭头方向一致。
- **现象（图形空心 / 实心）**：画板上的矩形 / 圆 / 三角形在「空心」态画成**虚线边框 + 灰色填充**，与需求相反。
- **根因（图形）**：`src/canvas/ShapeItem.cpp` 把「空心」实现为 `Qt::DashLine` 虚线笔 + `#EEF0F4` 灰填充，把「实心」实现为实线笔 + 同色灰填充；同时把「虚线 / 实线」这个**图标语言**误用到了**画布绘制结果**上。需求是：空心 = 只有完整边框、内部无填充；实心 = 边框与填充同色、整体实心；虚线 / 实线**只用于工具栏图标**。
- **修复（图形）**：
  - `ShapeItem::paint()` 画布绘制改为：笔一律 `Qt::SolidLine`；空心时 `setBrush(Qt::NoBrush)`（内部完全透明），实心时 `setBrush(QBrush(m_penColor))`（边框色即填充色，整体实心），填充透明度沿用 `m_fillOpacity`；
  - `MySegmentedControl` 新增 `ShapeIcon`（`RectIcon` / `CircleIcon` / `TriangleIcon`）与 `setShapeIcon()` / `setShapeIcons()`，绘制时按当前形状画小图标，并与文字并排居中；
  - 图标语义：第 0 段「空心」画**虚线边框 + 无填充**图标，第 1 段「实心」画**实线边框 + 同色实心**图标；
  - `CanvasToolBar` 按当前工具（矩形 / 圆 / 三角形）给 `canvasToolShapeFill` 切换对应形状图标。
- **连带修复（链式前向星）**：该模块用手工 `QGraphicsTextItem` 画下标（`rowY - 32`，字号 11 但包围盒高 26），位置不准且与 `k` 指针标签打架。改为直接使用 `ArrayBlock` 内建下标（自动居中于格子上方、留 6px 净距），`k` 指针标签改用 `setLabelAtTail()` 落在轴线上，行距 `kFsRowGap` 由 96 放宽到 116 让开相邻行下标。
- **验证**：
  - `SmartGroundStrictAudit` 新增 `[AXIS-FAIL]` 断言（标签中心必须与指针轴线对齐、尖端必须落在格心 / 节点圆心延长线上），最终 `TOTAL OVERLAP CONFLICTS = 0`、`TOTAL TIP FAILURES = 0`、`TOTAL AXIS FAILURES = 0`；
  - `SmartGroundOverlapAudit` 的下标包围盒改用 `ArrayBlock::indexRect()`，与实际绘制一致；`forwardstar` 用例归零，全套 `ALL PASS`；
  - `SmartGroundLayoutFit` 的 `forwardstar: k pointer sits at the to row bottom edge` 恢复通过；
  - `build/tests/` 下 **97 个测试可执行文件全部零失败**；`SmartGround.exe` Release 构建成功。

## 2026-11-21 笛卡尔坐标系五项可用性回归修复

用户对「自由结构 - 笛卡尔坐标系」模块实测后提出五个问题：描点工具找不到、左侧算法输入冗余、对齐网格与坐标网格冲突、函数画不出来、不知道如何描点。逐条定位与修复如下。

- **现象（描点工具不可见）**：浮动工具栏第二行（笛卡尔专属行）整体跑到了工具栏右边界之外，与主行重叠，用户完全看不到「画点 / 连线 / 函数」三个按钮。
- **根因（布局）**：`src/ui/widgets/CanvasToolBar.cpp` 的 `reposition()` 用 `m_cartesianRow->mapTo(m_cartesianRow, QPoint(0, 0))` 做自参照定位 —— 该映射恒等于 `(0, 0)`，但由于 `m_cartesianRow` 是 `CanvasToolBar` 的子控件，`setGeometry` 传入的 `barW` 被裁剪到工具栏实际宽度，而行本身仍按 `sizeHint().width()` 排版，导致按钮被挤到可视区外；行高也未计入工具栏总高度。
- **修复（布局）**：`reposition()` 改为**绝对定位两行** —— 主行 `setGeometry(0, 0, barW, mainH)`，笛卡尔行 `setGeometry(0, mainH, barW, cartH)`，工具栏总高 `mainH + cartH`，`barW` 取两行 `sizeHint()` 的较大值；三个按钮各加中文标签「画点 / 连线 / 函数」，并新增 `CanvasToolBar::syncCartesianButtons(int)` 让工具状态与笛卡尔按钮的高亮双向同步。
- **现象（左侧算法输入冗余）**：笛卡尔模式仍显示算法输入框、构建 / 单步 / 自动播放按钮与脚本文字开关，与「自由结构」语义冲突。
- **根因（布局）**：`MainWindow::onModuleChanged` 未对笛卡尔模式做布局裁剪，算法控件一直挂在 `leftLayout` 上。
- **修复（布局）**：新增 `MainWindow::applyCartesianLayoutMode(bool)`，在笛卡尔模式下隐藏 `labelInput` / `textInput` / `btnBuild` / `btnStep` / `btnStepBack` / `btnAuto` / `checkScriptText` / `checkGrid`；点与线的管理统一交给 `CartesianPanel` 的「对象列表」与「函数绘制」两段。
- **现象（对齐网格冲突）**：笛卡尔坐标系自带 60px 数学网格，画布的 `SceneGrid` 56px 虚线对齐辅助线叠加在其上，视觉噪声极大。
- **根因（配置）**：`SceneGrid::Config::enabled` 与 `showGuides` 是**全局静态状态**，切换模块时无人重置，默认 `enabled = true`。
- **修复（配置）**：`applyCartesianLayoutMode(true)` 内在 `checkGrid` 处于勾选态时主动 `setChecked(false)` 并调用 `onGridToggled(false)`，同时把 `cfg.enabled` 与 `cfg.showGuides` 一并置 `false`；离开笛卡尔模式时恢复控件可见性。
- **现象（函数画不出来）**：输入 `3x^2 - 8x + 7` 后画布上没有任何曲线。
- **根因（裁剪）**：`CartesianCurveItem::rebuildPolyline()` 用 `yLimit = span * 2.0 + 40.0` 这一**只看 x 跨度**的经验值裁剪采样窗口，完全无视 `m_minY` / `m_maxY`；抛物线在 `x = -9.27` 处 `y = 338.96`，远超经验上限，曲线整条被裁掉；此外 `boundingRect()` 返回固定的 `±400000` 方框，`QGraphicsScene` 依此做视口剔除时会把曲线整条裁出可视区。
- **修复（裁剪）**：`rebuildPolyline()` 重写为**按真实 Y 窗口裁剪** —— `padY = (m_maxY - m_minY) * 0.5 + 8.0`，采样窗口 `[m_minY - padY, m_maxY + padY]`，并以 `maxJump = (highY - lowY) * 2.0 + 16.0` 侦测不连续段做 `flush()`；`boundingRect()` 改为按 `visibleBounds()` 动态计算并留 40px 余量。
- **修复（输入）**：`CartesianPanel` 的函数输入框接上 `editingFinished`（回车即绘制）与 `textChanged` 实时解析反馈（绿字「将绘制：…」/ 红字「无法解析：…」），按钮文案由 `"+"` 改为「绘制」；`CartesianExpression::parse()` 新增 **`f(x) =` / `y =` / `f =` 前缀剥离**，允许用户直接照抄教科书写法。
- **现象（不会描点）**：用户找不到描点入口，也不知道工具怎么用。
- **根因（可发现性）**：进入笛卡尔模式后工具仍停在「指针」，用户必须先在工具栏里认出那个没有文字的图标；工具提示里承诺的 `(P)` / `(S)` 快捷键**从未实现**；面板上没有任何操作指引。
- **修复（可发现性）**：
  - `ToolController::isDrawingTool()` 补上 `CartPoint` / `CartLine` —— 此前这两个工具**不消费鼠标事件**，`eventFilter` 返回 `false`，画布把点击吞掉，描点根本不会触发；
  - `MainWindow::keyPressEvent` 实现 `P`（画点）/ `S`（连线）快捷键，仅在 `m_cartesianActive` 时生效；
  - 进入笛卡尔模式时自动切到 `CartPoint` 工具并同步引擎 `ActiveTool::Point`；
  - `CartesianPanel` 顶部新增 `GuideCard` 四步引导卡（画点 / 连线 / 函数 / 管理），用序号徽章 + 蓝底圆角卡片呈现；
  - 状态栏 `labelStatus` 常驻提示「描点：单击画布落点 · 连线：按 S 切到连线工具 · 函数：在左侧「函数绘制」输入 f(x)= 表达式」；
  - 「函数」按钮点击后自动 `focusFunctionInput()` 并全选，光标直接落到表达式输入框。
- **验证**：
  - 新增 `tests/CartesianUxTest.cpp`（`SmartGroundCartesianUx`）：覆盖五项诉求共 25 条断言，**全部 PASS**（含「第二行与主行不重叠」「进入即选中画点工具」「算法控件全部隐藏」「`SceneGrid` enabled/showGuides 均为 false」「曲线在应用视口内可见像素 1323」「`f(x) = ` 前缀可解析」「`S` / `P` 快捷键生效」「引导卡存在」）；
  - 新增 `tests/CartesianAppViewTest.cpp`（`SmartGroundCartesianAppView`）：走**真实 UI 路径**（输入框 → 绘制按钮 → 视口渲染）验证曲线可见；
  - 新增 `tests/CartesianCurveIsoTest.cpp`（`SmartGroundCartesianCurveIso`）：引擎级曲线渲染隔离验证；
  - 新增 `tests/CartesianShotTest.cpp`（`SmartGroundCartesianShot`）：整窗 UI 截图存档；
  - `SmartGroundCartesianUi`、`SmartGroundCartesianDiag`、`SmartGroundCartesianProbe` 回归通过；`SmartGround.exe` Release 构建成功。
- **连带缺陷 1（工具栏宽度退化）**：`CanvasToolBar::buildMainBar()` 原先有一个 `QVBoxLayout` 根布局包裹 `mainRow`，本轮为了绝对定位两行而移除它；`mainBarWidth()` 返回 `sizeHint().width()`，失去根布局后该值退化为 `-1`，`SmartGroundToolSuite` 的「工具栏宽度 < 700px」断言失败。修复：`mainBarWidth()` 改为返回 `mainRowWidth()`（即 `mainRow->sizeHint().width()`），语义仍是「主行宽度」。
- **连带缺陷 2（按钮文本撞车）**：函数按钮文案由 `"+"` 改为「绘制」后，`UiBehaviorTest` 的 `findButton(&w, "绘制")` 会先命中 `CartesianPanel` 里这个按钮（它排在 `leftLayout` 更靠前），而不是算法的「绘制可视化效果」按钮，导致 `btnBuild->click()` 落空、后续 14 项断言连环失败。修复：函数按钮最终定名「绘图」，与算法侧按钮文本彻底分离。
- **最终验证**：`build/tests/` 下 **85 个测试可执行文件全部零失败**；`SmartGround.exe` Release 构建成功。

## 2026-11-21 笛卡尔坐标系七项实测回归修复（第二轮）

用户对「自由结构 - 笛卡尔坐标系」模块第二轮实测后提出七个问题。逐条定位与修复如下。

### 问题 1：描点后左侧对象列表不更新

- **根因（初始化顺序）**：`MainWindow` 构造函数里 `setupUI()` 先执行并在其中调用 `setupCanvasTools()`，而**笛卡尔模块是在 `setupUI()` 返回之后才被 new 出来**的。`setupCanvasTools()` 里的 `if (m_cartesianModule) { ... }` 判断发生在 `m_cartesianModule` 还是 `nullptr` 的时刻，整段 `objectsChanged` / `statusMessage` / `requestPointEditor` / `pointInspected` 等**信号连接从未建立**，面板因此永远收不到刷新通知。
- **修复（信号重排）**：把笛卡尔模块的全部信号连接抽成独立方法 `MainWindow::setupCartesianWiring()`，在**模块注册完成之后**（`m_cartesianModule` 赋值语句之后）调用一次，并用 `m_cartesianWired` 标志防重复连接。
- **修复（拖点同步）**：`CartesianPointItem::setMathPos()` 此前只在 `update()` 后静默返回，拖动过程中坐标变化**不会通知任何人**，左侧列表的坐标读数会停在旧值。改为在真正发生位移时 `emit m_engine->pointMoved(m_index, x, y)`，并在 `MainWindow` 中把 `CartesianEngine::pointMoved` 也接到 `CartesianPanel::refresh()`。

### 问题 2：连线后不应直接显示一次函数

- **根因（默认开关）**：`CartesianEngine::Settings::showEquation` 默认值为 `true`，`updateSettings()` 会把每一根线段的 `m_equationVisible` 置真，于是 `CartesianSegmentItem::paint()` 在线段中点画出一张 `y = kx + b` 白底小卡。
- **修复（语义反转）**：默认改为 `false`，线段不再自带方程标签；新增 `CartesianSegmentItem::equationText()` 与 `segmentSummary()` 只读接口，方程文本改为**由检视面板按需索取**。用户在「坐标系设置」里仍可勾选「显示线段与函数方程」把它画回图上。

### 问题 3：描点只能落在坐标交点上

- **根因（量化语义）**：`CartesianEngine::snapMath()` 无条件执行 `std::round(x / step) * step`，**任何**坐标都被硬量化到分度尺的整数倍，因此 `(0.5, 0.8)` 这样的自由坐标根本无法落点。
- **修复（接近吸附）**：把「吸附」与「量化」拆成两件事 —— 新增 `snappedMath(const QPointF& math)` 做**接近判定**：先算到最近交点的距离 $\Delta$，只有 $\Delta \le$ `m_snapTolerance`（默认 0.28 个分度尺单位）才吸附，否则**原样保留自由坐标**；新增 `isSnapped()` 让调用方能区分「吸附点」与「自由点」。`snapScene()` 改为 `toScene(snappedMath(toMath(scene)))`，`addPoint()` 内部亦改用 `snappedMath()`（不再无条件量化）。新增 `setStep()` / `setSnapTolerance()` / `setSnapEnabled()` 供设置面板与测试使用。

### 问题 4：工具按钮的点击判定区域太苛刻

- **根因（控件拆分）**：第二排的每个工具原本是「一个 30×30 的 `ToolIconButton` 图标 + 一个独立的 `QLabel` 文字」，**只有图标响应点击**，点文字等于点空气。
- **修复（图标内嵌文字）**：给 `ToolIconButton` 增加 `setCaption()` 能力 —— 文字直接**画在按钮内部**并参与尺寸计算（宽度 = 30 + 5 + 文字宽 + 4），按钮宽度随文字自适应；`CanvasToolBar::buildCartesianRow()` 移除独立 `QLabel`，改用 `button->setCaption("画点")`。现在图标与文字是**同一个控件的同一块可点击区域**，选中态（accent 圆角底 + 反白文字）也整体高亮。

### 问题 5：描点功能下按住右键应拖动画布

- **根因（事件吞噬）**：`ToolController::eventFilter()` 的右键分支只处理「右键擦除」与「表格右键菜单」，在 `CartPoint` / `CartLine` 工具下**右键既不拖动也不做任何事**。
- **修复（右键平移）**：在 `ToolController` 中新增 `m_cartesianPanning` / `m_cartesianPanLast` 两个状态：坐标系启用且处于 `CartPoint` / `CartLine` 时，右键按下即进入平移态（光标切 `ClosedHandCursor`），`onMouseMove()` 按增量调用 `CartesianEngine::setOriginScene()` 移动坐标原点并 `CanvasView::syncCartesianViewport()` 同步网格与曲线可视范围，右键松开恢复十字光标。

### 问题 6：坐标系延伸到哪里就结束，不是无限

- **根因（场景矩形）**：`ViewportFitter::expandSceneRect()` 用 `scene->itemsBoundingRect()` 加 800px 余量设定 `sceneRect`，坐标系只有几个点时包围盒极小，**画布平移范围被死死卡住**；同时 `CartesianGridItem::boundingRect()` 返回的是**缓存的视口矩形**，平移出该矩形后网格直接不再绘制。
- **修复（无限画布）**：① `ViewportFitter` 新增 `setInfiniteCanvas()` / `infiniteCanvas()` 与 ±1e7 的场景矩形，笛卡尔模式下由 `MainWindow::onModuleChanged()` 打开，离开时关闭，`expandSceneRect()` 在无限模式下直接维持大场景矩形；② `CartesianGridItem::boundingRect()` 改为返回 ±1e7 的大方框，`paint()` 改用 `option->exposedRect`（**当前真正暴露在屏幕上的区域**）计算网格线的起止，网格线数量由视口动态决定而非预生成数组 —— 与「只处理当前页面内显示哪些坐标」的思路一致。

### 问题 7：双击描点后右侧不弹出信息框

- **根因（语义错位 + 无控件）**：画布双击点被转发到 `CartesianModule::requestPointEditor`，弹出的是**模态编辑对话框**（改名字 / 改坐标），既遮挡画布也与「查看信息」的诉求不符；项目里根本不存在右侧信息面板。
- **修复（新增检视面板）**：新增 `src/ui/widgets/CartesianInfoPanel.{h,cpp}` —— 自绘圆角白卡（标题 + 等宽正文 + 提示行），以**浮层**形式锚在画布右侧中部（`MainWindow::positionCartesianInfo()` 负责定位，永远落在画布右半区且不出界）。`CartesianModule` 新增 `pointInspected` / `segmentInspected` / `curveInspected` 三个「检视」信号，与「编辑」信号彻底分离；双击点 → 右侧卡片显示**点名 / x / y / 是否已吸附**；双击线段 → 卡片显示**一次方程 y = kx + b 与线段长度**；双击曲线 → 卡片显示**函数名与表达式**。

### 连带缺陷：`PointerBlock` 的 `Qt::UniqueConnection` 导致进程崩溃

- **现象**：`SmartGroundNewModule` / `SmartGroundBatch8` / `SmartGroundAnimStress` 等 26 个测试可执行文件在渲染若干帧后以退出码 `-1073740286`（`0xC0000602`，fail-fast）崩溃，与本轮改动无关但被回归暴露。
- **根因（Qt 6.11 契约）**：`src/render/GraphElements.cpp` 的 `PointerBlock::setAnchorNode()` / `setAnchorItem()` 用 `QObject::connect(sender, &QObject::destroyed, this, [lambda]{...}, Qt::UniqueConnection)` 连接。**`Qt::UniqueConnection` 只支持「指向成员函数的槽」**，传 lambda 会触发 `QObject::connect` 的断言：`Unique connection requires the slot to be a pointer to a member function of a QObject subclass`，Qt 6.11 把该断言升级为 `qt_assert_x` 致命错误。
- **修复（连接句柄显式管理）**：新增 `PointerBlock::m_anchorConn`（`QMetaObject::Connection`）与 `disconnectAnchor()`，改用**不带 `UniqueConnection`** 的 lambda 连接并在重新锚定时显式 `QObject::disconnect(m_anchorConn)`，避免连接泄漏与重复触发。`QPointer` 成员本就负责节点销毁后的置空。

### 验证

- 新增 `tests/CartesianRound2Test.cpp`（`SmartGroundCartesianRound2`）：覆盖七项诉求共 **32 条断言，全部 PASS**（含「描点后列表多一行」「`(0.5, 0.8)` 保持自由坐标」「靠近原点才吸附到 `(0, 0)`」「线段默认不画方程」「按钮带文字且宽度大于纯图标」「右键拖动改变原点」「无限画布标志已开且场景矩形大于 10 万」「信息框可见且落在画布右半区」「信息框正文含 `x =` / `y =`」「线段信息框正文含方程」）。
- 更新 `tests/CartesianModuleTest.cpp`：旧断言「`snapScene` 必定落在网格交点上」与新语义冲突，改写为「远处点击保持自由坐标 / 靠近交点的点击被吸附」。
- 新增 `tests/CartesianRound2Shot.cpp`（`SmartGroundCartesianRound2Shot`）：整窗截图存档，覆盖总览 / 双击信息框 / 右键平移 / 远距离平移四态。
- **最终验证**：`build/tests/` 下 **87 个测试可执行文件全部零失败**（修复前 26 个因 `PointerBlock` 崩溃而失败）；`SmartGround.exe` Release 构建成功。
