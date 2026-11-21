# SmartGround 模块开发状态跟踪

> **更新时间**: 2026-11-21 - 第 33 批（新增「自由结构 - 笛卡尔坐标系」模块，GeoGebra 对标；已过两轮实测回归）
> **当前进度**: 63 个唯一可运行模块（63 个下拉条目） | 0 个头文件骨架
> **核心变化**：本批新增**笛卡尔坐标系模块**——理论上无限延伸的网格画布（视口裁剪），滚轮缩放驱动**分度尺阶梯**（1.0 / 0.5 / 0.2 / 0.1 / 0.05 / 0.02 / 0.01），画布工具栏**扩展第二排**（画点 / 连线 / 函数），左侧新增**对象列表面板**（点坐标管理 + 样式设置 + `f(x) = 表达式` 函数录入），一次 / 二次 / 三次函数**按可视范围自适应绘制**，平移出视口即消失、移回即恢复。两轮实测回归后补齐：对象列表实时刷新、描点**接近吸附**（可落 `(0.5, 0.8)` 等自由坐标）、工具按钮**图标与文字同一命中区**、画点 / 连线模式**右键拖动平移画布**、网格**真无限**（`exposedRect` 视口驱动）、双击对象弹**右侧检视面板**。

## 2026-11-21 第 32 批：笛卡尔坐标系新模块（GeoGebra 对标）

| 项目 | 涉及文件 | 实现要点 |
|---|---|---|
| 表达式解析器 | `src/core/CartesianExpression.{h,cpp}`（新建） | 递归下降解析器，支持**一次 / 二次 / 三次**多项式；**隐式乘法**（`2x`、`3x^2`、`(x+1)(x-1)`）由 `startsFactor()` + `multiplyTerms()` 支持；`parse()` 返回 `Result{c3,c2,c1,c0,error}`；`evaluate()` 求值；`describe()` 输出 `y = 3x^2 - 8x + 7` 形式的规范式；`describeSegment()` 输出线段一次方程 `y = kx + b`（含水平 / 垂直退化情形） |
| 坐标引擎 | `src/render/CartesianElements.{h,cpp}`（新建） | `CartesianEngine` 负责数学坐标 ↔ 场景坐标换算（`scenePos = (originX + x·scale, originY - y·scale)`）、**分度尺阶梯** `stepLadder()`（100 → 0.01 共 13 级）、`snapMath()` / `snapScene()` 网格吸附、`zoomBy()` 锚点缩放（缩放时保持鼠标下数学坐标不变）、`updateViewport()` 驱动曲线可视范围 |
| 网格渲染 | `CartesianGridItem` | 主/次网格线 + 坐标轴 + 刻度数字，全部按当前视口裁剪绘制；`setViewport()` 记录可视矩形并预留 400px 余量；`boundingRect()` 跟随视口，避免超大 `boundingRect` 拖慢渲染 |
| 点对象 | `CartesianPointItem` | 原生 `QGraphicsObject`（**非动画属性**，拖动零延迟）；`hoverEnterEvent` 切换 `Qt::PointingHandCursor`（食指向上）；形状支持圆 / 方 / 菱 / 叉四种；名字标签白底圆角；`nameHidden` 支持隐藏名字；`setMathPos()` 统一入口保证吸附与位置一致 |
| 线段对象 | `CartesianSegmentItem` | 两端点连线；`refreshGeometry()` 跟随端点位移；`hitTest()` 用点到线段距离做命中；双击显示一次方程标签 |
| 函数曲线 | `CartesianCurveItem` | **多段折线 `m_runs`** 结构：出界的部分只**切断当前段**而不清空整条曲线（修复「二次曲线整条消失」缺陷）；`setVisibleRange(minX,maxX,minY,maxY)` 按视口数学范围自适应重绘，平移出视口则无可见段、移回即恢复 |
| 模块封装 | `src/modules/CartesianModule.{h,cpp}`（新建） | 继承 `FreeStructureBase`，模块名 `自由结构 - 笛卡尔坐标系`；`isCartesian()` 返回 true 供 UI 切换第二排工具栏；`moduleSettingsFields()` 提供「显示网格 / 显示刻度数字 / 描点时显示名字 / 显示方程」四个开关；`buildLayout()` 挂载引擎到场景；`releaseSceneItems()` **同时 `setEnabled(false)`**（修复切走后引擎仍启用） |
| 画布工具栏 | `src/ui/widgets/CanvasToolBar.{h,cpp}`、`src/ui/widgets/ToolIconButton.{h,cpp}` | 新增 `CartPoint` / `CartLine` / `CartFunction` 三个图标；`setCartesianMode()` 控制**第二排工具行**显隐；画点工具的**二次点击展开设置卡**（描点是否带名字）；图标绘制：画点 = 鼠标尖带圆点、连线 = 直线两端带小圆点、函数 = 抛物线 |
| 左侧面板 | `src/ui/widgets/CartesianPanel.{h,cpp}`（新建） | 对象列表（每行：颜色圆点 + 名字 + 坐标 + ⋮ 菜单）；⋮ 菜单 →「编辑名称与坐标」「设置」「删除」；**样式卡**用现有 `MyColorPicker`（无极变色）+ `MySlider`（大小）+ `MySegmentedControl`（形状）+ `MyToggleSwitch`（隐藏名字），**不含「高级 / 脚本」子菜单**；函数区输入 `f(x) = 表达式` 并实时校验；`CartesianPointEditor` 模态对话框同时编辑**名字与坐标**并带「隐藏名字」开关 |
| 交互实现 | `src/canvas/ToolController.{h,cpp}` | 新增 `CartPoint` / `CartLine` 工具枚举与 `setCartesianEngine()`；`beginCartesianPoint()` 单击落点并吸附到当前分度尺粗线交点，长按拖动时 `updateCartesianPoint()` **逐帧跟手**（无 Qt 属性动画，60fps 无卡顿）；`beginCartesianLink()` 奇数次选点高亮起点、偶数次选点连成线段；`cartesianDoubleClick()` 统一分发点 / 线段 / 曲线双击 |
| 缩放与平移 | `src/canvas/CanvasView.{h,cpp}` | 笛卡尔模式下滚轮不再缩放 `QGraphicsView` 变换，而是调用 `CartesianEngine::zoomBy()` **改变分度尺**；中键平移改为移动 `originScene()`；`resizeEvent()` 同步视口 |
| 面板联动 | `src/ui/MainWindow.{h,cpp}` | `onModuleChanged()` 中根据 `isCartesian()` 切换第二排工具栏与左侧面板显隐，并把引擎注入 `ToolController` 与 `CanvasView`；新增 `onCartesianToolChanged` / `onCartesianPointNamesChanged` / `onCartesianPointEdited` / `onCartesianPointStyleEdited` / `onCartesianFunctionStyleEdited` / `onCartesianFunctionAdded` / `onCartesianFunctionRemoved` / `refreshCartesianViewport`；析构时先解绑引擎指针再释放模块（修复退出崩溃） |
| 新增测试 | `tests/CartesianModuleTest.cpp`（新建，11 组断言）、`tests/CartesianVisualTest.cpp`（新建，8 项断言 + 9 张渲染图）、`tests/CartesianPixelTest.cpp`（新建，15 项像素级断言）、`tests/CartesianUiTest.cpp`（新建，18 项 UI 集成断言） | 覆盖：表达式解析（一次 / 二次 / 三次 / 隐式乘法 / 非法输入）、方程描述、坐标换算往返、网格吸附、分度尺阶梯随缩放变化、点 / 线段 / 函数增删改查、像素级验证坐标轴 / 网格 / 点标记 / 名字标签 / 线段中点 / 曲线着色、面板与工具栏显隐联动、切走模块后引擎禁用 |
| 全量回归 | `tests/build/` | **79 个测试可执行文件全部零失败**；`SmartGroundGroupAUi` 模块计数更新为 63 并新增笛卡尔存在性断言；`SmartGround.exe` Release 构建成功 |

## 2026-11-21 第 32 批补二：笛卡尔坐标系七项实测回归修复（第二轮）

针对用户第二轮实测提出的七个问题（列表不刷新 / 线段直接显示方程 / 描点被硬量化 / 按钮文字不可点 / 右键不能平移 / 坐标系非无限 / 双击无右侧信息框）完成一轮修复。

| 项目 | 涉及文件 | 实现要点 |
|---|---|---|
| 列表实时刷新 | `src/ui/MainWindow.{h,cpp}`、`src/render/CartesianElements.{h,cpp}` | 根因是 `setupCanvasTools()` 早于模块创建，信号连接从未建立。抽出 `setupCartesianWiring()` 在模块注册后调用；`CartesianPointItem::setMathPos()` 补发 `pointMoved` 信号，拖动时列表坐标同步 |
| 线段方程语义 | `src/render/CartesianElements.{h,cpp}` | `Settings::showEquation` 默认改 `false`，线段不再自带方程标签；新增 `equationText()` / `segmentSummary()` 只读接口，方程改由检视面板按需索取（用户仍可在设置里勾选画回图上） |
| 接近吸附语义 | `src/render/CartesianElements.{h,cpp}` | `snapMath()` 的硬量化拆为 `snappedMath()`：距离最近交点 ≤ `m_snapTolerance`（默认 0.28 分度尺）才吸附，否则**保留自由坐标**；新增 `isSnapped()` 区分吸附点与自由点；`setStep()` / `setSnapTolerance()` / `setSnapEnabled()` 供设置与测试使用 |
| 按钮判定区域 | `src/ui/widgets/ToolIconButton.{h,cpp}`、`src/ui/widgets/CanvasToolBar.cpp` | `ToolIconButton` 新增 `setCaption()`，文字**画在按钮内部**并参与尺寸计算（30 + 5 + 文字宽 + 4）；移除独立 `QLabel`，图标与文字成为同一块可点击区域，选中态整体高亮 |
| 右键平移 | `src/canvas/ToolController.{h,cpp}` | 新增 `m_cartesianPanning` / `m_cartesianPanLast`；坐标系启用且处于画点 / 连线时右键进入平移态，按增量调用 `setOriginScene()` 移动原点并 `syncCartesianViewport()` |
| 无限画布 | `src/core/ViewportFitter.{h,cpp}`、`src/render/CartesianElements.cpp` | `ViewportFitter` 新增 `setInfiniteCanvas()` / `infiniteCanvas()` 与 ±1e7 场景矩形；`CartesianGridItem::boundingRect()` 改大方框，`paint()` 改用 `option->exposedRect` **按当前视口动态生成网格线**，与「只处理当前页面内显示哪些坐标」的思路一致 |
| 右侧检视面板 | `src/ui/widgets/CartesianInfoPanel.{h,cpp}`（新建）、`src/modules/CartesianModule.{h,cpp}`、`src/ui/MainWindow.{h,cpp}` | 新增自绘圆角白卡浮层（标题 + 等宽正文 + 提示行），锚在画布右侧中部且不出界；`CartesianModule` 拆分「检视」(`pointInspected` / `segmentInspected` / `curveInspected`) 与「编辑」两组信号；双击点显示点名 / 坐标 / 是否吸附，双击线段显示一次方程与长度，双击曲线显示函数名与表达式 |
| 连带崩溃修复 | `src/render/GraphElements.{h,cpp}` | `PointerBlock` 的 `Qt::UniqueConnection` + lambda 组合在 Qt 6.11 触发致命断言（26 个测试进程 fail-fast）。新增 `m_anchorConn` 与 `disconnectAnchor()`，改用普通 lambda 连接并显式断开 |
| 新增测试 | `tests/CartesianRound2Test.cpp`（新建，32 条断言）、`tests/CartesianRound2Shot.cpp`（新建，截图存档）、`tests/CartesianModuleTest.cpp`（改写吸附断言） | 覆盖七项诉求逐条断言；旧「`snapScene` 必落在交点」断言与新语义冲突，改写为「远处点击保持自由坐标 / 靠近交点才吸附」 |
| 全量回归 | `tests/build/` | **90 个测试可执行文件全部零失败**（修复 `PointerBlock` 崩溃前有 26 个失败）；`SmartGround.exe` Release 构建成功 |

## 2026-11-21 第 33 批补：笛卡尔坐标系交互行为的**真实事件链**验收（第三轮）

第二轮修复本身是有效的，但**验证方法存在盲区**：上一轮测试直接调用引擎 API（`engine->addPoint()` / `engine->pointDoubleClicked()`）断言行为，**完全绕过了真实的鼠标事件链路**（`QGraphicsView` → `viewport` 事件过滤器 → `ToolController` → 引擎）。这意味着「画布单击是否真的落点」「双击是否真的弹检视面板」「点击文字标签是否真的选中工具」这些**用户实际动作**从未被验证过 —— 而事件未被消费、工具未激活、过滤器顺序错误这类缺陷**恰恰只在这条链路上暴露**。

| 项目 | 涉及文件 | 实现要点 |
|---|---|---|
| 真实交互测试 | `tests/CartesianRound3Test.cpp`（新建）、`tests/CMakeLists.txt` | 新增 `SmartGroundCartesianRound3`。全部断言改用**真实鼠标事件**驱动：向 `view->viewport()` 投递 `QMouseEvent` press / release / dblClick、向按钮投递真实点击。覆盖：单击落点 → 左侧列表新增**可见**行；在 `(0.5, 0.8)` 真实单击 → 落点保持自由坐标且 `isSnapped()` 为 false；在交点 `(5, 4)` 真实单击 → 精确吸附且 `isSnapped()` 为 true；点击按钮**文字区域**（`width()-6`）→ 工具真正切换且按钮进入 active 态；真实右键按下+移动+释放 → 原点位移；真实点击两点再切连线工具点击两点 → 生成线段且**画布上不绘制方程**；真实双击点 / 线段 → 检视面板弹出、坐标与方程正确、面板**锚定在画布右半区且不出界** |
| 无限性判据升级 | `tests/CartesianRound3Test.cpp` | 由绝对阈值（「远处网格像素 > 500」）改为**相对判据**：原点处网格像素 63327、平移 126000 单位后 61716，密度一致才算通过。绝对阈值会因视口尺寸与分度尺档位变化而误报 |
| 构建树治理 | `build/`、`build/tests/` | 仓库存在**两套构建树**：`build/`（应用）与 `build/tests/`（独立的测试工程，有自己的 `CMakeLists.txt`）。新增测试文件后必须先 `cmake -S tests -B build/tests` 重新生成目标，再从 `build/tests` 构建；发布前**两棵树都要重建**，避免残留陈旧二进制 |
| 实测结论 | — | **七项全部通过**，证实第二轮修复真实有效；全量回归 **90 个测试可执行文件零失败**（新增 1 个） |

## 2026-11-21 第 32 批补：笛卡尔坐标系可用性回归修复

针对用户实测提出的五个问题（描点工具不可见、左侧算法输入冗余、对齐网格冲突、函数画不出、不会描点）完成一轮修复。

| 项目 | 涉及文件 | 实现要点 |
|---|---|---|
| 工具栏第二行定位 | `src/ui/widgets/CanvasToolBar.{h,cpp}` | `reposition()` 改为**绝对定位两行**：主行 `(0, 0, barW, mainH)`、笛卡尔行 `(0, mainH, barW, cartH)`，工具栏总高 `mainH + cartH`，`barW` 取两行 `sizeHint()` 较大值；三个按钮加中文标签「画点 / 连线 / 函数」；新增 `syncCartesianButtons(int)` 双向同步高亮态 |
| 算法控件裁剪 | `src/ui/MainWindow.{h,cpp}` | 新增 `applyCartesianLayoutMode(bool)`，笛卡尔模式隐藏 `labelInput` / `textInput` / `btnBuild` / `btnStep` / `btnStepBack` / `btnAuto` / `checkScriptText` / `checkGrid`；点与线统一由 `CartesianPanel` 的「对象列表」管理 |
| 对齐网格关闭 | `src/ui/MainWindow.cpp` | 进入笛卡尔模式时 `checkGrid` 取消勾选并调用 `onGridToggled(false)`，`SceneGrid::Config` 的 `enabled` 与 `showGuides` 一并置 `false`，与坐标系 60px 网格彻底解耦 |
| 曲线裁剪修正 | `src/render/CartesianElements.{h,cpp}` | `rebuildPolyline()` 按**真实 Y 窗口** `[m_minY - padY, m_maxY + padY]` 裁剪（`padY = (m_maxY - m_minY) * 0.5 + 8.0`），并以 `maxJump` 侦测不连续段；`boundingRect()` 改为按 `visibleBounds()` 动态计算（原固定 `±400000` 方框会被场景剔除整条曲线） |
| 函数录入增强 | `src/core/CartesianExpression.cpp`、`src/ui/widgets/CartesianPanel.{h,cpp}` | `parse()` 支持 **`f(x) =` / `y =` / `f =` 前缀剥离**；输入框接 `editingFinished`（回车绘制）与 `textChanged` 实时绿/红解析反馈；按钮文案由 `"+"` 改为「绘图」（避开与算法「绘制可视化效果」按钮的 `findButton` 文本冲突）；新增 `focusFunctionInput()` |
| 描点可发现性 | `src/canvas/ToolController.cpp`、`src/ui/MainWindow.{h,cpp}`、`src/ui/widgets/CartesianPanel.{h,cpp}` | `isDrawingTool()` 补入 `CartPoint` / `CartLine`（此前这两个工具**不消费鼠标事件**，描点根本触发不了）；`keyPressEvent` 实现 `P` / `S` 快捷键（仅笛卡尔模式生效）；进入模块自动切到画点工具；`CartesianPanel` 顶部新增 `GuideCard` 四步引导卡；状态栏常驻操作提示 |

## 2026-11-20 第 31 批：算法动画深化 + 欧拉筛新模块

| 项目 | 涉及文件 | 实现要点 |
|---|---|---|
| 帧类型扩展 | `src/engine/AnimFrame.h` | 新增 24 个动作类型：归并 `MS_SPLIT/MS_REC/MS_MERGE_OPEN/MS_CMP/MS_PICK/MS_APPEND/MS_COPY/MS_DONE`、背包 `KN_COMPARE`、并查集 `DP_NO_COMPRESS/DP_HEIGHT`、欧拉筛 `ES_RESET/ES_OUTER/ES_INNER/ES_CROSS/ES_PRIME_ADD/ES_PRIME_PICK/ES_BREAK/ES_DONE` |
| 归并排序分镜独立 | `src/modules/MergeSortModule.{h,cpp}` | **不再复用快排的 `AR_*` 帧族**，改用专属 `MS_*`；布局新增 `temp[]` 合并输出缓冲行（y=400）与底部讲解行（y=620）；合并过程为「两段队首对比高亮 → 胜者从原格飞入 temp → temp 整体写回原区间」；左段染蓝、右段染紫，已排好序的区间染绿，整体有序后染深绿 |
| 01 背包先比较后跳 | `src/modules/Knapsack01Module.cpp` | 新增 `KN_COMPARE` 帧：`skip` 与 `take` 两个候选**同时**高亮 + 涟漪对比，随后**只有胜者**通过 `playValueFlight` 飞向目标格；`KN_CANDIDATE` 帧退化为「只高亮来源格、不再抢先飞行」；容量不足时只发 `KN_COMPARE`（`take = -1`）并给出「只能不选」的公式 |
| 并查集双模式 | `src/modules/DSUPathCompressionModule.{h,cpp}` | 新增 `moduleSettingsFields()`（`makeSegment`「有路径压缩 / 无路径压缩」）与 `applyModuleSetting()`；无压缩模式改发 `DP_NO_COMPRESS` 帧（路径染红、明确标注「父指针保持原样」）；两模式都发 `DP_HEIGHT` 帧，底部常驻**树高标尺**；默认数据改为 `9\n0 0 1 2 3 4 0 6 0`（树高 5 → 压缩后 1） |
| 新增欧拉筛 | `src/modules/EulerSieveModule.{h,cpp}`（新建） | 复用埃氏筛的 10 列矩阵布局（行距 92px 留出指针空间）；右侧独立**质数库 `prime[]`** 区域（5 列网格）；`i` 指针（橙，向下指当前外层数）、`p` 指针（紫，向上指当前质数）；`ES_INNER` 高亮 `i*p` 目标格，`ES_CROSS` 划除并注明「最小质因子 = p」，`ES_BREAK` 把 `i` 所在格染红说明「`i % p == 0` → 后面的 p 不用试了」 |
| 边原地改指向 | `src/render/GraphElements.{h,cpp}` | `GraphEdge` 新增 `repointDest(GraphNode*)`，路径压缩时把已有父边**原地改指向**根结点（自动从旧 dest 解绑、向新 dest 绑定），无需重建边对象 |
| 新增测试 | `tests/Batch31Test.cpp`（新建，30 项断言）、`tests/Batch31Visual.cpp`（新建，5 张渲染图） | 断言覆盖：归并不复用 `AR_*` 帧、最终数组有序、合并次数 = n-1；背包每个 `KN_DECIDE` 前必有 `KN_COMPARE`；并查集两模式的帧类型互斥且树高 5 → 1；欧拉筛**每个合数只被划一次**（42 个合数无重复）、每次 `break` 都满足 `i % p == 0`、最终 42 划除 / 17 保留；埃氏筛行为**未被改动** |
| 全量回归 | `tests/build/` | **77 个测试可执行文件全部零失败**；`SmartGroundStrictAudit` 63 模块 `TOTAL OVERLAP CONFLICTS = 0` / `TOTAL TIP FAILURES = 0` / `TOTAL AXIS FAILURES = 0`；`SmartGround.exe` Release 构建成功 |

## 2026-11-20 第 30 批：视觉缺陷修复 + 美学宪法建立

| 项目 | 涉及文件 | 实现要点 |
|---|---|---|
| 美学宪法 | `docs/AnimationAesthetics.md`（新建） | 8 大章节：三条底线 / 色彩语义表 / 运动语义表（四种边运动 + 五种节点运动 + 指针运动 + 轨迹 + 时长基准）/ 布局分层规范 / 控件语义规范 / 状态完整性条款 / 文案规范 / 帧设计规范 / 新模块自检清单（10 条） |
| 规则接入 | `.cursorrules` / `AGENTS.md` / `.cursor/rules/smartground-project.mdc` / `Cursor_Workflow.md` | 四处均加入「动笔前必须先读 `docs/AnimationAesthetics.md`」的强制条款；`Cursor_Workflow.md` 第 2 节补入颜色语义、运动方向、状态收尾、控件语义四条宪法 |
| 开关语义修正 | `src/ui/widgets/MyToggleSwitch.cpp` | **统一为「滑块在右 = 该状态开启」**：左侧画 `m_offText`、右侧画 `m_onText`，高亮跟随滑块位置；`trackRect()` 改用 `m_offText` 宽度计算左侧留白。影响全项目所有 toggle（图的类型 / 高亮延迟边 / 显示反例对照 / 右键擦除） |
| 边运动方向 | `src/render/GraphElements.{h,cpp}` | `GraphEdge` 新增 `m_beamReversed` 属性；`paint()` 反向时用 `quadSubPath(p0, c, p1, 1.0 - head, 1.0 - tail)`，光束头部与拖尾一并反向 |
| 边运动 API | `src/modules/IModule.{h,cpp}` | 新增 `playEdgeTravelFrom(edge, color, fromNodeId, ms)` / `playEdgeTraceFrom(edge, color, fromNodeId, ms)` / `startBeamFrom(...)`；内部比对 `edge->dest->id == fromNodeId` 自动判定反向 |
| 二分图队列 | `src/modules/BipartiteModule.{h,cpp}` | 新增 `BP_QUEUE_CLEAR` 帧类型（BFS 结束、结论之前清空整行队列并隐藏双指针）；修正 `BP_DEQUEUE` 的 `val1` 取值时机（改为 `++head` 之前）；队列行下移至 y=560，双指针改 `PointerBlock::Up` + `belowCell` 锚在槽位下方；`col[]` 行 → y=470，结论文本 → y=630，奇环文本 → y=666 |
| 欧拉延迟边 | `src/modules/EulerModule.cpp` | `EU_EDGE` / `EU_BRIDGE` 改用带起点 id 的运动 API；`EU_BRIDGE` 帧的 `u`/`v` 改为「回溯出发点 → 边的另一端」，延迟边高亮方向与实际回溯方向一致 |
| 匈牙利增广边 | `src/modules/HungarianModule.cpp` | `HN_TRY` / `HN_OCCUPIED` 改用 `playEdgeTravelFrom` 并传入起点 id，增广路的试探方向与递归让位方向一致 |
| 新增测试 | `tests/VisualFixTest.cpp`（新建） | 5 项断言：队列行最终无残留值 / tail 指针不越界 / 指针标签不越出画布 / 欧拉桥接帧产出 / 开关两态快照渲染；`tests/CMakeLists.txt` 注册 `SmartGroundVisualFix` |
| 全量回归 | `tests/build/` | **76 个测试可执行文件全部零失败**；`SmartGroundStrictAudit` 62 模块 × 2 轮 `TOTAL OVERLAP CONFLICTS = 0` / `TOTAL TIP FAILURES = 0` / `TOTAL AXIS FAILURES = 0`；`SmartGround.exe` Release 构建成功 |

## 2026-11-20 第 29 批：四个已有模块的动画深化（分镜落地）

| 项目 | 涉及文件 | 实现要点 |
|---|---|---|
| 帧类型扩展 | `src/engine/AnimFrame.h` | 新增 20 个动作类型：`TR_QUERY_RESET/CHAR/HIT/MISS/FOUND`、`TR_ORDER_START/NODE/DONE`、`IV_REJECT_BAND/PHASE_RESET/COUNTER_COMPARE/COUNTER_PICK/COUNTER_DONE`、`EU_DEADEND/BACKTRACK/SPLICE/TRAIL/BRIDGE`、`BP_ENQUEUE/DEQUEUE/ODD_CYCLE` |
| Trie 三模式 | `src/modules/TrieModule.{h,cpp}` | 新增 `moduleSettingsFields()`（`makeSegment` 三模式 + `makeText` 查询词）与 `applyModuleSetting()`；`runAlgorithm()` 拆成 `runInsertMode()` / `runQueryMode()` / `runOrderMode()` 三条独立路径。查询模式新增 `buildStaticTrie()` 先建静态树再逐字符下降；字典序模式用显式栈做「字符升序 DFS」，`TR_ORDER_NODE` 的 `v` 字段区分「进入子树」与「输出单词」，`val1` 携带输出序号 |
| Trie 指针 | `src/modules/TrieModule.cpp` | 保留字符行指针 `i`（`PointerGeometry::aboveCell`，锚定当前字符格）与游标指针 `p`（锚定当前树结点）；查询/建树两种模式下 `i` 随字符推进逐格右移 |
| 区间调度反证 | `src/modules/IntervalSchedModule.{h,cpp}` | 新增 `m_showCounterExample` 开关与 `IV_REJECT_BAND` / `IV_COUNTER_*` 帧；冲突时用 `QGraphicsRectItem`（红虚线 + 70/255 半透明填充）在时间轴上画出**重叠区间带**；反证阶段改按开始时间排序重跑，错误解排在独立一行（`m_naiveLabels`），与贪心解（`m_greedyLabels`）上下对照 |
| 二分图奇环 | `src/modules/BipartiteModule.{h,cpp}` | 新增 `m_queueBlocks`（队列数组行）+ `m_queueHeadPtr` / `m_queueTailPtr`（双指针锚定槽位）；新增 `extractOddCycle(u, v)`：沿 `m_parent[]` 父指针链求两条链的公共祖先，拼接出完整的环；`BP_ODD_CYCLE` 帧用 `PathTrail` 描出整条奇环并把环上结点染红 |
| 欧拉回溯拼接 | `src/modules/EulerModule.{h,cpp}` | 新增 `m_stackBlocks` / `m_pathBlocks`（纵向栈列）、`m_resultBlocks`（底部答案行）、`m_trail`（`PathTrail` 轨迹）、`m_spliceLine`（`LinkLine` 橙色虚线箭头）；新增「高亮延迟边」开关；`EU_DEADEND` 死路结点灰化收缩、`EU_BRIDGE` 延迟边紫色描边、`EU_SPLICE` 拼接箭头 + 涟漪、`EU_PATH` 答案行落值 + 轨迹描边 |
| 布局重排 | `src/modules/EulerModule.cpp` / `IntervalSchedModule.cpp` | 欧拉：`deg[]` 行起始 x 44 → 170、行距 54 → 76，`stk[]`/`path[]` 列 x 470 / 600，答案行下移至 y = 618；区间调度：标题与扫描行分离（`greedyY = timelineBottom + 58`），结果/反证文字各占独立行 |
| 奇环抽取修正 | `src/modules/BipartiteModule.cpp` | `extractOddCycle()` 原先用「双指针同步上跳找相遇点」，在两链长度不同时会漏掉公共祖先；改为「分别取完整祖先链 → 从尾部对齐比较公共后缀 → 拼成 `u→…→LCA←…←v` 的环」 |
| 新增测试 | `tests/AnimUpgradeTest.cpp`（新增） | 9 项断言覆盖三个新模式的帧产出（查询命中 / 查询失败 / 字典序完成 / 奇环抽取 / 死路回溯 / 拼接 / 冲突带 / 反证结论），并输出 5 张可视化截图供人工复核 |
| 注册测试 | `tests/CMakeLists.txt` | 新增 `add_sg_test(SmartGroundAnimUpgrade AnimUpgradeTest.cpp)` |
| 全量回归 | `tests/build/` | **73 个测试可执行文件全部零失败**；`SmartGroundStrictAudit` 62 模块 × 2 轮逐帧审计 `TOTAL OVERLAP CONFLICTS = 0` / `TOTAL TIP FAILURES = 0` / `TOTAL AXIS FAILURES = 0`；`SmartGround.exe` Release 构建成功 |

## 2026-11-20 第 28 批：数组下标归位 + 指针轴向对齐 + 图形空心实心语义修正
> **核心变化**：**① 数组下标归位**：上一轮为规避重叠告警把下标从单元格**上方**挪进了**格子内部左上角**，且把值区改成扣除下标高度后的剩余区，导致值偏离格心 —— 这是一次擅自决断。本批 `ArrayBlock::paint()` 恢复为下标绘制在 `QRectF(0, -kIndexGap - kIndexHeight, m_cellW, kIndexHeight)`（`AlignHCenter | AlignBottom`，与顶边框保持 6px 净距），值恢复在整个窗格内 `Qt::AlignCenter` 双向居中；新增 `kIndexGap`/`kIndexHeight` 常量与 `indexRect()`，`boundingRect()` 上边界相应上移。**② 指针轴向对齐**：`layoutPointerSlots()` 原先 `tips[idx] += QPointF(offset, 0)` 会**横向平移箭头尖端**（偏离格心），`resolveVisualCollisions()` 原先以横向平移标签为主（标签脱离自身轴线，`i`/`j`/`L`/`R` 与指针对不上号）。现改为**轴向优先**：垂直指针沿轴线由近及远堆叠（`m_labelRow`），水平指针沿轴向让位，横向平移仅作水平指针兜底；`separatePointerLabels()` 垂直指针改纵向分离。**③ 水平箭头方向修正**：`Right` 的箭头尖此前与 `Left` 同朝左，语义错误，现 `Right` 朝右、`Left` 朝左，标签偏移同步。**④ 图形空心/实心语义修正**：`ShapeItem::paint()` 原先把「空心」画成**虚线边框 + `#EEF0F4` 灰填充**（把图标语言误用到画布结果上），现改为笔一律实线、空心 `Qt::NoBrush`（内部全透明）、实心 `QBrush(m_penColor)`（边框色即填充色）；`MySegmentedControl` 新增 `ShapeIcon` 与 `setShapeIcon()`，**图标**语义为「空心=虚线边框无填充、实心=实线边框同色实心」，绘制逻辑抽为公共静态 `buildIconPath()` / `paintShapeIcon()` 供 UI 与测试共用。**⑤ 链式前向星**：手工文本下标改为 `ArrayBlock` 内建下标，`k` 指针标签改 `setLabelAtTail()` 落轴线，行距 96 → 116。

## 2026-11-20 第 28 批：数组下标归位 + 指针轴向对齐 + 图形空心实心语义修正

| 项目 | 涉及文件 | 实现要点 |
|---|---|---|
| 数组下标归位 | `src/render/GraphElements.{h,cpp}` | `ArrayBlock::paint()` 下标改绘制于 `QRectF(0, -kIndexGap - kIndexHeight, m_cellW, kIndexHeight)`（`Qt::AlignHCenter \| Qt::AlignBottom`），与顶边框保持 6px 净距；值改在整格 `QRectF(0, 0, m_cellW, m_cellH)` 内 `Qt::AlignCenter`；新增 `kIndexGap = 6.0` / `kIndexHeight = 16.0` 与 `indexRect()`；`boundingRect()` 上边界上移至 `-(kIndexGap + kIndexHeight)` |
| 指针尖端不偏移 | `src/modules/IModule.cpp`（`layoutPointerSlots`） | 删除同格多指针时的 `tips[idx] += QPointF(offset, 0.0)`，改为沿轴线分行（`setLabelRow`），箭头尖端恒落在单元格宽度中心 / 竖直中点 |
| 指针标签轴向优先 | `src/modules/IModule.cpp`（`resolveVisualCollisions` / `separatePointerLabels`） | 重写为轴向求解：垂直指针（Up/Down）沿轴线由近及远堆叠最多 8 行（步长 22px），水平指针（Left/Right）沿轴向左右让位；横向平移仅作水平指针兜底，垂直指针标签恒在自身轴线上；`separatePointerLabels()` 垂直指针改纵向分离 22px |
| 水平箭头方向修正 | `src/render/GraphElements.cpp`（`PointerBlock::paint`） | `Right` 箭头尖由朝左改为朝右（`drawLine(0, 0, 9 * sign, ±7)`），`Left` 朝左；`defaultLabelOffset()` / `tailLabelOffset()` / `setLabelToRight()` 同步 —— `Left` 标签在杆右外侧、`Right` 在杆左外侧 |
| 画布空心 / 实心绘制 | `src/canvas/ShapeItem.cpp`（`paint`） | 笔一律 `Qt::SolidLine`；空心 `setBrush(Qt::NoBrush)`（内部完全透明）；实心 `setBrush(QBrush(m_penColor))`（边框色即填充色），填充透明度沿用 `m_fillOpacity` |
| 分段控件形状图标 | `src/ui/widgets/MySegmentedControl.{h,cpp}` | 新增 `ShapeIcon` 枚举（`RectIcon`/`CircleIcon`/`TriangleIcon`）、`setShapeIcon()` / `setShapeIcons()`；图标与文字并排居中；**空心段画虚线边框无填充、实心段画实线边框同色实心**；绘制逻辑抽为公共静态 `buildIconPath()` / `paintShapeIcon()`，UI 与测试共用同一份代码 |
| 工具栏接入图标 | `src/ui/widgets/CanvasToolBar.cpp` | 按当前工具（矩形 / 圆 / 三角形）给 `canvasToolShapeFill` 设置对应形状图标 |
| 链式前向星下标 | `src/modules/ForwardStarModule.cpp` | 手工 `QGraphicsTextItem` 下标（`rowY - 32`，包围盒高 26）改为直接使用 `ArrayBlock` 内建下标；`k` 指针标签由 `setLabelOffset(-38, 0)` 改为 `setLabelAtTail(8.0)` 落轴线；`kFsRowGap` 96 → 116 让开相邻行下标 |
| 审计增强 | `tests/StrictAudit.cpp` | 新增 `[AXIS-FAIL]` 断言：标签中心必须与指针轴线对齐、尖端必须落在格心 / 节点圆心延长线上；`ArrayBlock` 下标包围盒改用 `indexRect()` |
| 审计包围盒同步 | `tests/OverlapAuditTest.cpp` | 下标包围盒由写死的 `QRectF(0, -25, w, 20)` 改为 `b->indexRect()`，与实际绘制一致 |
| 画板断言扩展 | `tests/CanvasFeaturesTest.cpp` | 新增 feature 4（画布空心内部像素全透明 / 实心内部完全填充）与 feature 5（图标：空心有边框且内部透明、实心内部填充且墨量更大），共 9 项新断言 |
| 全量回归 | `build/tests/` | **97 个测试可执行文件全部零失败**；`SmartGroundStrictAudit` 报 `TOTAL OVERLAP CONFLICTS = 0` / `TOTAL TIP FAILURES = 0` / `TOTAL AXIS FAILURES = 0`；`SmartGroundOverlapAudit` / `SmartGroundLayoutFit` 均 `ALL PASS`；`SmartGround.exe` Release 构建成功 |

## 2026-11-20 第 27 批：激光光束贴合曲线 + 编辑器析构崩溃根治 + Tarjan 数组重排版

| 项目 | 涉及文件 | 实现要点 |
|---|---|---|
| 光束沿曲线绘制 | `src/render/GraphElements.{h,cpp}` | 新增 `GraphEdge::splitQuad()`（de Casteljau 二次曲线二分）与 `GraphEdge::quadSubPath()`（按 `from..to` 抽取子曲线生成 `QPainterPath`）；`paint()` 的光束段由 `drawLine` 弦线改为 `drawPath` 子曲线，辉光 9.0px + 内核 3.4px 两层均用沿路径首尾的 `QLinearGradient`，端点亮点取 `beamPath.pointAtPercent(1.0)` |
| 光束几何回归测试 | `tests/BeamCurveTest.cpp`（新增） | 光束端点与原贝塞尔曲线偏差 **0.00000000 px** |
| 光束渲染回归测试 | `tests/BeamRenderTest.cpp`（新增） | 19 个进度档位 × 61 采样点：弧线上 **1159 采样点零断裂**，超出光束头部 **351 采样点零残留** |
| 编辑器析构安全 | `src/render/GraphElements.{h,cpp}` | 新增 `EditorTextItem`（`QGraphicsTextItem` 子类，析构时置位共享 `EditorGuard::tearingDown`）；`EditorWatcher::eventFilter()` 收到 `FocusOut` 时若标志已置位则直接吞掉，绝不回调 `commitEdit()`；`GraphNode` / `GraphEdge` / `ArrayBlock` 析构函数均先置位标志再 `setParentItem(nullptr)` + `delete` 编辑器子项 |
| Tarjan 数组排版 | `src/modules/TarjanCutVertexBridgeModule.cpp` | `buildLayout()` 中 `dfnY` 500 → 545、新增 `rowGap = 100`、`dfnX` 120 → 170；`dfn[]` / `low[]` 标签由 `(114, 500)` / `(114, 530)` 移到 `(48, 553)` / `(48, 653)`，与首列窗格保持 49px 净距 |
| 全量回归 | `build/tests/` | 全部测试可执行文件零失败；`SmartGroundStrictAudit` 58 模块 × 2 轮逐帧审计 `TOTAL OVERLAP CONFLICTS = 0` / `TOTAL TIP FAILURES = 0`；`SmartGroundGraphInteraction` 连跑 30 次零失败；`SmartGround.exe` Release 构建成功 |

> **核心变化**：**① 全局零重叠三道防线**（PM 要求「不再出现文字/控件/数组窗格重叠、指针未指向结点或数组中心、指针瞬移、平移结构拖影、动画落点偏移」）：`IModule` 新增 `resolveScriptTextOverlaps()`（讲课脚本文字与 `ArrayBlock`/`GraphNode`/矩形实体求交后沿候选偏移避让）、`resolveVisualCollisions()`（指针标签按 `m_labelRow` 堆叠错位）、`syncPointerTips()`（每帧末按 `pointerTipFor()` 复算并强制归位），三者由 `finalizeFrame()` 统一串联，播放/单步/撤销/拖进度条四条路径全部接入。**② 激光拖影根治**（PM 反馈「所有图论算法边像激光射过去时路径上都有碎片残留波纹」）：`GraphEdge::paint()` 的 26 段短线叠加改为 `QLinearGradient` 沿路径一次性连续绘制；`IModule::playEdgeTravel()` 改走新增的 `startBeam()`，用 `m_beamAnims` 注册表按边去重，`purgeTransientItems()` 与析构时统一 `stop()` + `clearBeam()`。**③ 指针精确锚定 + 不瞬移**：新增 `GraphNode::kRadius = 20.0`（尖端精确落圆周）、`retargetPointerAcrossCells()`（同侧换格走行外弧线不穿格）、`retargetPointerWithDirection()`（方向翻转延后到位生效，先绕行再切向），`CircularQueueModule` 已接入。**④ 数组高亮改矩形涟漪**：`RectRipple` 取代圆形涟漪用于数组结构。**⑤ 生命周期安全**：`GraphEdge` / `GraphNode` / `PathTrail` 全线 `QPointer` 化，场景重建期 use-after-free 消除。**⑥ 画板三项新功能**：左侧竖向笔刷滑块 `CanvasBrushSlider`（笔/荧光笔/橡皮可见，与飞出面板、滚轮双向同步）、画笔/荧光笔「左写右擦」（橡皮面板 `MyToggleSwitch` 可关且 `QSettings` 记忆，默认开启）、矩形/圆形/三角形「空心 / 实心」分段切换（虚线框 vs `#EEF0F4` 实心填充）。**⑦ 修掉一个真缺陷**：偏好读写原用无参 `QSettings`，因 `main.cpp` 未设 org/app 名被判 `InvalidScope` 而**根本没落盘**；现补 `setOrganizationName/setApplicationName` 且改用显式 `IniFormat + UserScope` 作用域 + `sync()`。全量测试**零失败**，`SmartGroundStrictAudit` 报告 **0 处重叠冲突 / 0 处指针失锚**（58 模块 × 2 轮全帧审计）。

## 2026-11-20 第 26 批：全局零重叠审计 + 激光拖影根治 + 画板三项新功能

| 项目 | 涉及文件 | 实现要点 |
|---|---|---|
| 三道防线入口 | `src/modules/IModule.{h,cpp}` | `finalizeFrame()` = `resolveVisualCollisions()` → `resolveScriptTextOverlaps()` → `syncPointerTips()`；`resolveScriptTextOverlaps()` 收集带 `kScriptTextMarker` 的文本项，与 `ArrayBlock` 单元框、`GraphNode` 圆框 + `subLabel`/`valueText`、`QGraphicsRectItem` 求交，冲突时沿 $(\pm 34k)$ 候选偏移逐档移动至无冲突 |
| 指针尖端锁定 | `src/modules/IModule.{h,cpp}` | `syncPointerTips()` 每帧末按 `pointerTipFor(ptr, anchor)` 复算尖端，`stopAnimFor(ptr)` 后 `setPos(tip)` + `setTipTarget(anchor, tip)`，杜绝动画结束后尖端残留偏移 |
| 激光光束连续绘制 | `src/render/GraphElements.cpp` | `GraphEdge::paint()` 光束段由 26 段短线叠加改为 `QLinearGradient` 沿路径一次绘制（外层 9px 渐变 + 内层 3.4px 亮芯 + 端点亮点），消除「串珠 / 波纹」接缝 |
| 光束动画去重 | `src/modules/IModule.{h,cpp}` | 新增 `startBeam(edge, color, duration)` + `std::map<GraphEdge*, QPointer<QVariantAnimation>> m_beamAnims`；`purgeTransientItems()` 与析构统一 `stop()` + `clearBeam()`，杜绝同边多动画互相打断留下的残段 |
| 指针锚点精确 | `src/render/GraphElements.{h,cpp}` | `GraphNode::kRadius = 20.0`；`IModule::pointerTipFor()` 与 `PointerBlock::anchoredPos()` 统一用 `kRadius`，结点指针尖端精确落圆周（`PointerAlignTest` 校验 dy == 20.0） |
| 指针不瞬移 / 不穿格 | `src/modules/IModule.{h,cpp}` | 新增 `retargetPointerAcrossCells()`（同侧换格走行外弧线）与 `retargetPointerWithDirection()`（方向翻转延后到位生效：先按原方向绕行 → 到位后切向并锁定终点）；`pointerTipFor()` 新增显式方向重载 |
| 循环队列指针 | `src/modules/CircularQueueModule.cpp` | `aimQueuePointers()` 改用 `retargetPointerWithDirection()`；head==tail 时 head 停格上边缘、tail 停格下边缘，不再方向瞬翻或杆体穿格 |
| 数组矩形涟漪 | `src/render/GraphElements.{h,cpp}` + `src/modules/IModule.cpp` | 新增 `RectRipple`（矩形双层描边随 `setGrow()` 外扩）；`playCellRipple()` / `playCellRippleAt()` 改用它，`playNodeRipple()` 检测到 `ArrayBlock` 锚点自动走矩形波纹，结点仍保留圆形涟漪 |
| 生命周期安全 | `src/render/GraphElements.{h,cpp}` + `src/modules/*` | `GraphEdge::source/dest`、`GraphNode::edgeList/pointerList`、`PathTrail` 指针全线改 `QPointer`；`PointerBlock::setAnchorNode/setAnchorItem` 监听 `destroyed` 自动置空；`playEdgeTrace()` 的 `edge` 捕获改 `QPointer` 守卫 |
| 画板：笔刷滑块 | `src/ui/widgets/CanvasBrushSlider.{h,cpp}`（新增） | 贴靠画布左缘的圆角竖向滑块；仅笔 / 荧光笔 / 橡皮显示；与工具飞出面板、滚轮双向同步（`ToolController::styleChanged` 驱动）；笔调粗细、橡皮调半径 |
| 画板：左写右擦 | `src/canvas/ToolController.{h,cpp}` + `src/ui/widgets/CanvasToolBar.cpp` | 新增 `m_rightClickErase` 与 `beginRightErase/updateRightErase/finishRightErase`；橡皮面板新增 `MyToggleSwitch`（objectName = `canvasToolRightClickErase`），默认开启，关闭后右键回归与左键一致的绘制行为 |
| 画板：空心 / 实心 | `src/ui/widgets/CanvasToolBar.cpp` + `src/canvas/ShapeItem.cpp` | 矩形 / 圆形 / 三角形面板顶部新增 `MySegmentedControl`（objectName = `canvasToolShapeFill`，选项「空心 / 实心」）；**第 28 批已修正语义** —— 空心 = 只有完整边框、内部无填充（`Qt::NoBrush`），实心 = 边框与填充同色（`QBrush(m_penColor)`）；「虚线 / 实线」仅作为分段控件**图标**语言 |
| 偏好持久化修复 | `src/main.cpp` + `src/canvas/ToolController.cpp` | 补 `QCoreApplication::setOrganizationName/setApplicationName`；偏好读写改用 `QSettings(QSettings::IniFormat, QSettings::UserScope, "SmartGround", "SmartGround")` + `sync()`，修复原「无 org/app 名 → `InvalidScope` → 根本没落盘」 |
| 全局审计工具 | `tests/StrictAudit.cpp` + `tests/StrictAuditModules.h`（新增） | 58 个算法模块 × 2 轮（讲课脚本开 / 关）逐帧采集全部 `ArrayBlock`/`GraphNode`/`PointerBlock`/`QGraphicsTextItem` 包围盒两两求交，并独立校验指针尖端是否仍锚在目标边界；结论 `TOTAL OVERLAP CONFLICTS = 0` / `TOTAL TIP FAILURES = 0` |
| 画板功能测试 | `tests/CanvasFeaturesTest.cpp`（新增） | 21 项断言覆盖：滑块工具可见性、滚轮双向同步、右键擦除默认值与跨实例记忆、toggle 与控制器联动、空心/实心分段控件在矩形/圆形/三角形三页均存在且写入工具样式 |
| 全量回归 | `build/tests/` | 全部测试可执行文件零失败，含 `SmartGroundStrictAudit` / `SmartGroundCanvasFeatures` / `SmartGroundPointerAlign` / `SmartGroundBatch9Visual` / `SmartGroundPointerMotion`；主程序 `SmartGround.exe` Release 构建成功 |

## 2026-11-20 第 25 批：三个模块深度重构（LCS 连续回溯路径 / 匈牙利分栏与增广路 / 树堆下沉轨迹）
> **核心变化**：**① 三个模块深度重构**（PM 抉择后落地）：`src/render/GraphElements` 新增 `PathTrail`（可渐进绘制的多段折线轨迹 + 行进头），`IModule` 新增 `playPathTrail` / `playPathTrailAppend` 两个编排助手。**LCS** 回溯改为一条沿对角线持续延伸、头亮点跟随回溯位置的**连续路径**（`LC_TRACE` 帧携带完整走位点序列）；**匈牙利算法** 新增左右两条虚线分栏带（明确二分图两部）+ `PathTrail` 作**增广路主角**（起笔 / 追加右部点 / 占用转红 / 配对转绿 / 终帧冻结）；**树与堆** 新增逐层**下沉轨迹**（`HeapTreeBase` 与 `SegmentTreeEditBase` 各提供共享实现，下降帧同时播放边行进与轨迹延伸）。**② 全场动画美学升级**（以哈夫曼树为基准）：`src/render/GraphElements` 新增 `TracerDot`（光晕+内核移动光点），`IModule` 新增 8 个共享编排助手 `playEdgeTravel` / `playDiscover` / `playEdgeDiscover` / `playNodeWake` / `playFocusRing` / `playValueArcFlight` / `playCellSweep` / `playCellLanding`，按族批量落地到 **69 个模块文件**：图算法（BFS / DFS / Dijkstra×2 / 拓扑 / Prim / Kruskal / Tarjan×2 / 二分图 / 欧拉 / 匈牙利）、树堆（堆排序 / 堆调整 / Trie / 哈希表 / 前向星 / 线段树×3）、排序（快排 / 希尔 / 冒泡 / 选择 / 插入 / 归并 / 计数 / 基数 / 桶）、查与算（二分×2 / 快速幂 / 树状数组 / LIS / 区间调度 / GCD / 单调队列 / 单调栈 / 并查集×2 / 树直径 / 树子树 / 背包×2 / 字符串哈希 / 筛法）、矩阵数论（LCS / 扩欧）、数据结构（栈 / 队列 / 循环队列 / KMP pi）。语义统一为 idle / scanning / active / matched / terminal 五态色。**③ 画笔变色与菜单自关闭**：画笔换色即时应用到当前笔迹与选中对象；画笔飞出菜单在点击画布等非菜单区域时自动收起。**④ 三角形去背景**：三角形绘图项的半透明填充底色彻底移除，改为纯描边。**⑤ 进度条同步自检**：修复 `MainWindow::m_seeking` 被鼠标滚轮误置为 true 后永久锁死的问题（仅在 `sliderPressed` 置位、`sliderReleased` 清零），`tests/TmpSliderAudit.cpp` 审计全部 70 个模块的滑块值与帧号逐帧一致，`ALL PASS fail=0`。全量 **79 个测试可执行文件零失败**。

## 2026-11-20 第 25 批：三个模块深度重构（LCS 连续回溯路径 / 匈牙利分栏与增广路 / 树堆下沉轨迹）

| 项目 | 涉及文件 | 实现要点 |
|---|---|---|
| 路径轨迹图形元素 | `src/render/GraphElements.{h,cpp}` | 新增 `PathTrail`（`UserType + 11`）：持有 `QVector<QPointF>` 折线点集；`setProgress(t)` 按**总段数 × t** 逐段插值绘制已走过部分；`paint()` 叠三层 —— 低透明度加粗辉光（`width × 2.4`）+ 实色主线（`width`）+ 末端光晕 / 内核 / 白环「行进头」；`setPoints` / `appendPoint` / `clearPoints` / `setTrailWidth` / `setHeadRadius` 均可直接调用 |
| 轨迹编排助手 | `src/modules/IModule.{h,cpp}` | 新增 `playPathTrail(trail, points, color, duration)`：`setPoints` 后以 `InOutCubic` 从 0 补间到 1，`m_animated == false` 时直接置 1.0；新增 `playPathTrailAppend(trail, point, color, duration)`：追加一点并从 `(n-2)/(n-1)` 续画到 1.0，实现「一条路径持续生长」 |
| LCS 连续回溯路径 | `src/modules/LCSModule.{h,cpp}` | `runAlgorithm()` 回溯段新增 `walk` 记录**每一步退让的落点**（含非匹配步）；`LC_TRACE` 帧的 `group` 携带已走过的路径点序列；`renderFrame` 取出各点对应的 dp 格中心连成折线交给 `PathTrail`，形成一条沿对角线持续延伸、行进头跟随回溯位置的连续路径；匹配格另走 `playCellLanding` + 涟漪，退让步只延伸路径 |
| 匈牙利分栏与增广路 | `src/modules/HungarianModule.{h,cpp}` | `buildLayout()` 新增左右两条虚线分栏带（左浅蓝 / 右浅橙，半透明填充，`Z = -2`）明确二分图左右两部；新增 `PathTrail` 作增广路主角：`HN_START` 起笔、`HN_TRY` 追加右部顶点、`HN_OCCUPIED` 追加持有者并转红、`HN_ASSIGN` 追加配对点并转绿、`HN_DONE` 冻结为绿并对全部匹配顶点收束缩放；`m_matchedEdges` 记录最终匹配边供终帧回放 |
| 树堆下沉轨迹 | `src/modules/HeapTreeBase.h` + `HeapSortModule` / `HeapAdjustModule` | `HeapTreeBase` 新增 `ensureDescentTrail` / `clearDescentTrail` / `appendDescent` / `freezeDescentTrail` 四个助手与 `m_descentTrail` / `m_descentPoints` 成员；比较与交换帧把当前结点位置追加进轨迹，形成逐层下沉的可见路径，终帧冻结为有序色 |
| 线段树下沉轨迹 | `src/modules/SegmentTreeEditBase.{h,cpp}` + `SegmentTreeBuildModule` / `SegmentTreeQueryModule` / `SegmentTreeTagModule` | `SegmentTreeEditBase` 提供同款共享实现（`SegmentTreeBuildModule` 不继承该基类，自带一份等价实现）；下降帧同时播放 `playEdgeTravel` 边行进与轨迹延伸，形成「树往下走」的连贯主线；`m_treeEdges` 索引新增于 `SegmentTreeEditBase` 与 `SegmentTreeBuildModule` |
| 全量回归 | `build/tests/` | 79 个测试可执行文件全部零失败，含 `SmartGroundTmpSliderAudit`（进度条同步）/ `SmartGroundTmpBrushDiag`（画笔换色 + 菜单自关闭 + 三角形无背景）/ `SmartGroundBatch9Visual` / `SmartGroundOverlapAudit` / `SmartGroundLayoutFit` |

## 2026-11-20 第 24 批：全场动画美学对齐哈夫曼树 + 画笔/三角形/进度条三项修复

| 项目 | 涉及文件 | 实现要点 |
|---|---|---|
| 边行进光点 | `src/render/GraphElements.{h,cpp}` | 新增 `TracerDot`：`QGraphicsObject`，`setZValue(28)`、`acceptedMouseButtons = NoButton`；`paint()` 画两层——外圈低透明度光晕（`radius * 2.1`）与内核高亮圆点（`radius`），内核再叠一圈白色细环，构成「光点沿边滑行」的观感 |
| 共享编排助手 | `src/modules/IModule.{h,cpp}` | 新增 8 个助手：`playEdgeTravel(edge, color, duration)`（沿边 `scenePointAt(t)` 行进的光点，半径与透明度按 `sin(t·π)` 起伏，`InOutCubic` 缓动，结束后自动移除）、`playDiscover(node, color, maxRadius, duration)`（结点发现涟漪）、`playEdgeDiscover(edge, node, color, duration)`（边先到、结点后醒）、`playNodeWake(node, color, maxRadius, duration)`（结点苏醒脉冲）、`playFocusRing(node, color, duration)`、`playValueArcFlight(start, end, value, color, duration)`（数值弧线飞行）、`playCellSweep(cells, color, stagger, duration)`（逐格错峰扫描）、`playCellLanding(cell, color, duration)`（落位脉冲）。全部遵守 `m_animated == false` 时瞬时生效、不创建 Qt 动画的约束 |
| 图算法族 | `BFSModule` / `DfsModule` / `DijkstraNaiveModule` / `DijkstraHeapModule` / `TopologicalSortModule` / `PrimModule` / `KruskalModule` / `TarjanModule` / `TarjanCutVertexBridgeModule` / `BipartiteModule` / `EulerModule` / `HungarianModule` | 扫描与松弛时播放 `playEdgeTravel` 边行进；首次访问结点播放 `playDiscover` / `playNodeWake`；松弛成功 / 匹配成功播放结点涟漪与缩放；割点与桥加冲突脉冲；匈牙利匹配加占用冲突脉冲 |
| 树堆族 | `HeapSortModule` / `HeapAdjustModule` / `HeapBasicsModule` / `TrieModule` / `HashTableModule` / `ForwardStarModule` / `SegmentTreeBuildModule` / `SegmentTreeQueryModule` / `SegmentTreeTagModule` | 堆的比较 / 交换播放树边行进（`m_treeEdges` 索引 `min/max` 对齐）；线段树下降播放 `playEdgeTravel` + 子结点涟漪；前向星建边播放结点发现涟漪与落位 |
| 排序族 | `BubbleSortModule` / `SelectionSortModule` / `InsertionSortModule` / `MergeSortModule` / `QuickSortModule` / `ShellSortModule` / `CountingSortModule` / `RadixSortModule` / `BucketSortModule` | 交换与归位播放涟漪 / 落位脉冲；快排递归区间播放 `playCellSweep` 错峰扫描；希尔分组播放组色涟漪；计数排序计数格播放脉冲 + 涟漪；**归并排序第 32 批重构**：`m_openBounds` 记录分裂缝，`MS_SPLIT` 用 `playMoveTo` 把数组**真的撑开**，`MS_COPY` 合缝**拼接**并整段染 `kMsDone`，底部 `temp[]` 缓冲行承接 `playValueFlight` |
| 查与算族 | `BinarySearchClosedModule` / `BinarySearchHalfOpenModule` / `FastPowModule` / `FenwickTreeModule` / `LISModule` / `IntervalSchedModule` / `GcdModule` / `MonotonicQueueModule` / `MonotonicStackModule` / `DSUPathCompressionModule` / `TreeDiameterModule` / `TreeSubtreeSizeModule` / `Knapsack01Module` / `KnapsackCompleteModule` / `StringHashModule` / `SieveOfEratosthenesModule` / `EulerSieveModule` | 二分命中与丢弃播放涟漪；快速幂 / 树状数组播放数值弧线飞行；区间调度选中与冲突分别播放绿 / 红双层涟漪；筛法合数与质数分别播放红 / 绿涟漪；**并查集第 32 批统一**：`pa[]` / `sz[]` 双行实时同步，`merge` 按大小合并、`sz` 累加逐项落位，路径压缩 `repointDest()` 现场改接父边；**欧拉筛第 32 批提速**：`i > n/2` 段改为 `ES_FAST_BATCH` 批量收录素数 |
| 矩阵数论族 | `LCSModule` / `ExgcdModule` | LCS 候选比较播放 `playValueArcFlight`（上方 / 左方候选值弧线飞入目标格）+ 目标格涟漪；确定值播放 `playCellLanding`；回溯路径播放落位脉冲；扩欧每轮整除式飞入时播放行首涟漪 |
| 数据结构族 | `StackModule` / `QueueModule` / `CircularQueueModule` / `KMPNextModule` | 入栈 / 入队播放 `playCellLanding`，出栈 / 出队播放收缩脉冲 + 涟漪；循环队列入队 / 出队 / 绕回分别播放落位 / 涟漪 / 落位；KMP pi 比较播放双格涟漪、失配播放蓝涟漪、确定 pi 播放落位脉冲 + 涟漪 |
| 画笔换色与菜单自关闭 | `src/ui/` 画板相关 | 画笔颜色变更即时写入当前笔迹与已选中对象的渲染色；应用级 `eventFilter` 挂在 `qApp` 上，画笔飞出菜单打开时，点击菜单以外的区域（画布等）自动收起菜单 |
| 三角形去背景 | `src/render/` 图形项绘制 | 三角形绘图项移除半透明填充底色，仅保留描边，背景完全透明 |
| 进度条同步自检 | `src/ui/MainWindow.cpp` + `tests/TmpSliderAudit.cpp` | `m_seeking` 只在 `MySlider::sliderPressed` 置为 true、在 `sliderReleased` 清零；此前鼠标滚轮划过滑块会把它永久置真，导致帧号继续递增而滑块冻结。新增审计工具遍历全部 70 个模块，逐帧比对滑块值与 `probeFrameIndex()`，结果 `ALL PASS fail=0` |
| 全量回归 | `build/tests/` | 79 个测试可执行文件全部零失败，含 `SmartGroundTmpSliderAudit`（进度条同步）/ `SmartGroundBatch9Visual` / `SmartGroundOverlapAudit` / `SmartGroundLayoutFit` / `SmartGroundHuffmanLayoutDump` |

## 2026-11-19 第 23 批：哈夫曼树飞入落点/父结点来源/根结点残留修正 + 边渐进生长动画

| 项目 | 涉及文件 | 实现要点 |
|---|---|---|
| 哈夫曼树飞入落点 | `src/modules/HuffmanTreeModule.cpp`（`HF_PROMOTE`） | 目标坐标由 `treePosOf(id, step.before)` 改为 `treePosOf(id, step.afterLift)`：用**上移对齐后**的森林行号计算落点，结点 16 / 20 一次飞入即到达与合并伙伴同排的行，不再出现「先落错行、下一帧跳位」（BUG-220）。同时本帧只处理原始叶子的队列筹码，内部结点的树内位移交由 `HF_LIFT` 帧，避免重影 |
| 哈夫曼父结点来源 | 同上（`HF_MERGE`） | 新建父结点队列筹码时先 `chip->setPos(treePosOf(parentId, step.after))`，父结点从树中的真实坐标「落」回候选队列，不再从 (0,0) 飞下（BUG-219） |
| 哈夫曼根结点清理 | 同上（`HF_MERGE` / `HF_FINAL`） | `HF_MERGE` 增加 `isRoot` 判定：最后一次合并为根结点**创建但立刻隐藏**队列筹码（保持「每结点一份队列副本」的节点计数），不再把它排入候选队列；`HF_FINAL` 再次兜底隐藏根结点的队列筹码（BUG-218） |
| 边渐进生长 | `src/render/GraphElements.{h,cpp}` | `GraphEdge` 新增 `Q_PROPERTY(qreal drawProgress READ drawProgress WRITE setDrawProgress)`，默认 `1.0`；`paint()` 按 `drawProgress` 对二次贝塞尔曲线做 de Casteljau 分割求值，只绘制 `[0, t]` 段，形成「从孩子端生长到父亲端」的展开效果；`progress < 1.0` 时不绘制箭头端点，避免箭头先于曲线出现（BUG-217） |
| 共享动画助手 | `src/modules/IModule.{h,cpp}` | 新增 `playEdgeGrow(GraphEdge* edge, int duration = 200)`：`acquireAnim(edge, "drawProgress", duration, OutCubic)`，`m_animated == false` 时直接置 1.0（不创建任何 Qt 动画） |
| 哈夫曼边生长编排 | `src/modules/HuffmanTreeModule.{h,cpp}` | `HF_JELLY` 帧在父结点果冻出现的同时把两条子边的 `drawProgress` 置 0 并在 200ms 内生长到 1.0；新增 `completeEdgeGrowth()`，在 `HF_PICK` / `HF_JELLY` / `HF_MERGE` 三帧入口调用，保证快速播放时边不会停在半画状态 |
| 测试辅助 | `tests/HuffmanFrameDump.cpp` + `tests/CMakeLists.txt` | 新增 `SmartGroundHuffmanFrameDump` 逐帧诊断工具：可按帧号打印所有可见结点的坐标、并在 `HF_JELLY` 帧采样各边的 `drawProgress`，用于定位落点与残影问题 |
| 全量回归 | `build/tests/` | 79 个测试可执行文件全部零失败，含 `SmartGroundBatch9Visual`（哈夫曼树 11 结点 / 队列 11 副本 / 10 条边断言全绿）/ `SmartGroundBatch9Modules` / `SmartGroundOverlapAudit` / `SmartGroundLayoutFit` / `SmartGroundHuffmanLayoutDump` |

## 2026-11-19 第 22 批：哈夫曼树行列/无向边/频率数组修正 + 扩展欧几里得文字去重叠与飞入对齐

| 项目 | 涉及文件 | 实现要点 |
|---|---|---|
| 哈夫曼树行高修正 | `src/modules/HuffmanTreeModule.cpp`（`simulateStates`） | 子树上移的行差由「结点 id 之差」改为「行号之差」：`const int delta = higherRow - lowerRow;`（原误用 `higher - lower` 两个 id）。行高不再随 id 膨胀（delta 由 9/11 回到 1~2），树恢复「根在最上、叶子在最下」的规范层次（BUG-216） |
| 哈夫曼树无向边 | 同上（`buildLayout`） | 两条子边改用 `new GraphEdge(child, parent, false)`，`is_directed = false`，`GraphEdge::paint()` 不再绘制箭头端点；0/1 路径标签仍由 `pathLabel` 徽标单独绘制（BUG-215） |
| 哈夫曼频率数组 | 同上（`buildLayout` / `renderFrame` / `runAlgorithm`） | 数组格子数由 `m_nodes.size()`（2n-1，含内部结点）改为 `m_leafCount`（n）；初始态即 `setCustomValue(原始权值)` 且 `showValue = true`、`kHfLeaf` 底色，`HF_RESET` 同样恢复原始权值；排序帧改为 `HF_SORT_LIFT`（该格清空并飞出数值）/ `HF_SORT_DROP`（数值流星般落回）成对推进，帧内携带当前置换数组以保证回放与拖动进度条可复现（BUG-214） |
| 扩欧版面重排 | `src/modules/ExgcdModule.cpp`（`buildLayoutBlocks`） | 新增版面常量：等式段 `kExEqY = 80`（步距 34）、辗转相除段标题 `kExGcdTitleY = 190` / 栏头 `kExHeadY = 222` / 行首 `kExRowY = 252`（步距 32）、左右两栏分置 `kExLeftX = 100` 与 `kExRightX = 500` 互不侵占、回代段步距 `kExBackStep = 32`；回代片段前缀宽度改用 `QFontMetrics::horizontalAdvance` 实测（末行 425px，原硬编码估算约 170px）（BUG-213 / BUG-217） |
| 共享飞行助手 | `src/modules/IModule.{h,cpp}` | 新增 `playTextFlightAligned(text, start, endLeft, fontSize, color, duration, onArrive)`：起点终点均为**左对齐**语义（`playTextFlight` 为居中语义），供扩欧四处飞行使用；新增 `playValueFlyOut(start, value, color, duration)`：数值自格子中心向上飞出，供哈夫曼排序阶段使用 |
| 回代项渲染修正 | `src/modules/ExgcdModule.cpp`（`renderTerms`） | 改为单遍拼接，同时记录片段的真实起始偏移 `fragStart`（含符号与系数前缀），供飞行落点计算 |
| 测试同步 | `tests/Batch9ModulesTest.cpp` | 新增断言：哈夫曼频率数组恰好 n 格且每格有值、根结点位于最上行、每个父结点严格高于其孩子、树边无箭头；扩欧文本框两两无相交 |
| 全量回归 | `build/tests/` | 78 个测试可执行文件全部零失败，含 `SmartGroundBatch9Modules`（新增断言全绿）/ `SmartGroundBatch9Visual` / `SmartGroundOverlapAudit` / `SmartGroundLayoutFit` / `SmartGroundHuffmanLayoutDump`（中点违例 0、最小结点间距 56.08px） |

## 2026-11-19 第 21 批：指针首现飞入修正 + 三模块排版去重叠 + 全量回归

| 项目 | 涉及文件 | 实现要点 |
|---|---|---|
| 指针首现瞬移 | `src/modules/IModule.cpp`（`playPointerMove`） | 指针在 `buildLayout()` 中创建后从未定位，其 `pos()` 为默认 (0,0)。首次调用 `playPointerMove()` 时若 `from.isNull()` 则直接 `setPos(targetPos)`，不启动动画；其余情况照旧补间（BUG-162） |
| 树状数组指针落点 | `src/modules/FenwickTreeModule.cpp`（`movePointerTo`） | 落点由 `t->pos() + QPointF(t->cellWidth()/2, t->pos().y()+t->cellHeight())`（纵坐标被当相对偏移再加一次绝对值）改为 `t->pos() + QPointF(cellX, t->cellHeight())`；水平中心按所属 level 区间换算 `cellX = (index - slot.left) * m_cellW + m_cellW/2`；指针方向统一 `Up`、标签偏移 `(-30, 0)`；`a[]` 标题左移到 `m_startX - 66`（BUG-163） |
| 链式前向星排版 | `src/modules/ForwardStarModule.cpp`（`layoutArrays` / `buildLayout`） | 行标题 `titleX` 由 `m_arraysX - 104` 改为 `m_arraysX - 178`；下标行由 `rowY - 22` 改为 `rowY - 32`，并对四类方块统一 `setShowLabel(false)` 消除重复下标；图区中心 x 由 1010 改为 1120；`k` 指针箭杆长度 26 → 22、标签改用 `setLabelOffset(-38, 0)`（BUG-164） |
| 堆模块排版 | `src/modules/HeapTreeBase.h`（`layoutHeap`） | `heap[]（数组存储）` 由数组左侧改放到数组行下方 `arrY + 48`；关系文本下移到 `arrY + 92`；树结构标题由 `treeTop - 54` 上移到 `treeTop - 96`（BUG-165） |
| 测试同步 | `tests/LayoutFitTest.cpp` + `tests/GroupAUiTest.cpp` | `LayoutFitTest` 中写死的 to 行 y=248 改为按行序动态定位（不再随布局常量漂移）；`GroupAUiTest` 断言同步新模块名「完全背包 DP」「二分图匹配 - 匈牙利算法」「堆调整 up / down」（BUG-166） |
| 全量回归 | `build/tests/` | 75 个测试可执行文件全部零失败，含 `SmartGroundBatch9Visual` / `SmartGroundBatch9Modules` / `SmartGroundOverlapAudit` / `SmartGroundLayoutFit` / `SmartGroundHuffmanLayoutDump`（中点违例 0、最小结点间距 56.08px） |

## 2026-11-18 第 20 批：哈夫曼树坐标/对称布局修正 + 扩展欧几里得飞入对齐 + 全局讲课脚本文字门控

| 项目 | 涉及文件 | 实现要点 |
|---|---|---|
| 全局讲课脚本文字门控 | `src/modules/IModule.{h,cpp}` + 59 个算法模块 `.cpp` | `IModule` 新增静态开关 `s_scriptTextVisible` 与一组 API：`setScriptTextVisible(bool)` / `scriptTextVisible()` / `createScriptText(scene, text)` / `setScriptText(item, text[, color])` / `clearScriptText(item)` / `createStatusText(scene, text)` / `setStatusText(item, text)`。`createScriptText` 在关闭状态下创建**空且不可见**的文本项；`setScriptText` 在关闭状态下清空内容并隐藏。所有讲课脚本类文字成员统一改用这两个 API，共改造 **263 处**（`m_noteText` 51 处 / `m_mathText` 59 处 / `m_resultText` 14 处 / `m_scanText` 9 处 / `m_heapText` 6 处 / `m_windowText` 3 处 / `m_probeText` 4 处 / `m_stateText` 5 处 / `m_relationText` 5 处 / `m_depthText` 5 处 / `m_verdictText` 5 处 / `m_accText` 3 处 / `m_stepText` 3 处 / `m_wordText` 2 处 / `m_gapText` 2 处）。**结构性标注未受影响**：`频率数组 a[]`、`a[] 原数组`、下标行、`桶 0`、树名、`第一步：…` / `阶段一：…` 章节标题、`a[i] / 编码 / 码长` 表头等仍由 `scene->addText()` 直接创建并常驻（BUG-212） |
| 讲课脚本文字开关 | `src/ui/MainWindow.{h,cpp}` | 界面新增 `MyCheckBox` 开关「显示讲课脚本文字」，`objectName = "scriptTextToggle"`，**默认未勾选**；勾选/取消时调用 `IModule::setScriptTextVisible(on)` 并 `rebuildCurrentLayout()` 重绘当前画布，状态栏同步提示「已显示 / 已隐藏讲课脚本文字：画布只保留数组名、下标、树名等结构性标注」 |
| 扩展欧几里得飞入对齐 | `src/modules/IModule.cpp`（`playTextFlightTo` / `playTextFlightFragment`） | 飞行副本的目标点直接取 `target->pos()`，落位后 `safeTarget->setPos(dst)` 保证与静止位置**完全重合**（像素差 0）；内联回代片段额外按 `QFontMetrics(target->font()).horizontalAdvance(prefix)` 计算前缀宽度，片段落在前缀右端而非整行左端（BUG-211） |
| 哈夫曼树排序落位居中 | `src/modules/HuffmanTreeModule.cpp`（`HF_SORT_DROP`） | 阶段一数字飞入频率数组格子时，落点由「格子上边缘」改为**格子中心**（水平按格子宽度居中、垂直按格子高度居中），不再偏下（BUG-206） |
| 哈夫曼树原地生成父结点 | `src/modules/HuffmanTreeModule.cpp`（`HF_JELLY`） | 新父结点由 `HF_PICK` 帧起就在两子结点中点处 `setOpacity(0)` 预置，`HF_JELLY` 帧只做 `playNodeAppear` 果冻生长 + `playRipple` 涟漪，**不再从屏幕上方 (0,0) 飞入**，杜绝超长边线（BUG-207） |
| 哈夫曼树候选结点上飞 | `src/modules/HuffmanTreeModule.cpp`（`HF_PICK` / `HF_PROMOTE`） | 被选中的两个候选结点从底部队列**向上飞入树区域框内**后再合并（`playMoveTo`），不再是原地闪现（BUG-208） |
| 哈夫曼树对称布局与中点规则 | `src/modules/HuffmanTreeModule.cpp`（`subtreeLayout` / `layoutComponentSym`） | 布局改用对称递归：子结点强制对称于父结点（$x_L=x_P-d$、$x_R=x_P+d$），保证 $x_P=(x_L+x_R)/2$；子树**不左右互换**（左右顺序按合并时的子结点次序固定）；森林中多棵组件树按 `m_compRank` 稳定排序并统一居中排布，间距按子树宽度分配，**不再出现结点重叠**（BUG-209） |
| 测试同步 | `tests/` 全量 | `SmartGround.exe` 与 `tests/` 下全部测试目标重新编译通过；`SmartGroundAnimStress` / `SmartGroundMatrixShape` / `SmartGroundMatrixRebuild` / `SmartGroundNewModuleBatch` 等逐帧与形态测试全部通过 |

## 2026-11-18 第 19 批：哈夫曼树六阶段重写 + WPL 修复

| 项目 | 涉及文件 | 实现要点 |
|---|---|---|
| 哈夫曼六阶段建树 choreography | `src/modules/HuffmanTreeModule.{h,cpp}`（整模块重写） | 默认数据 `45 13 20 16 9 12 5`（初始顺序即乱序，展示排序过程）。**阶段一**：`HF_SORT_LIFT` 逐个点亮悬停数字（悬停行 `kHoldY = 86`），`HF_SORT_DROP` 按升序一个一个飞入正确格子（`playValueDropInto`），`HF_SORT_DONE` 收尾。**阶段二**：`HF_QUEUE_DROP` 把全部数字从数组飞到底部候选队列圆形结点（`kQueueY = 664`），按升序排好。**阶段三**：每个合并四帧 —— `HF_PICK`（两个最小值染成挑选色并缩放脉冲）→ `HF_PROMOTE`（若两者树高不同，矮的那棵子树**整体上移**对齐）→ `HF_JELLY`（中点长出新结点，`playNodeAppear` 果冻生长 + `playRipple` 涟漪 + 橙色新边）→ `HF_MERGE`（父结点副本飞下落到候选队列，被取走的两队列成员淡出，其余**左右分开让位**）。**阶段四**：`HF_FINAL` 根结点专用根色 + 缩放。**阶段五**：`HF_CODE` 沿路径贴 0/1 徽标并填表。**阶段六**：`HF_WPL` 逐项高亮并累加（BUG-203） |
| 哈夫曼队列同权值取数规则 | 同上 | 排序比较器改为 `(weight, isOriginal ? 0 : 1, id)`：先比权值，同权值时**优先取叶子**，与课堂约定一致（BUG-204） |
| 哈夫曼树布局引擎 | 同上 | 新增 `subtreeWidth()` / `layoutComponent()` / `componentRoots()`：递归计算子树占宽，父结点严格位于两子结点中点；森林中多棵子树按宽度依次排布并统一居中，新结点插入后子树自动左右分开保持间距均匀 |
| 哈夫曼 WPL 总数修复 | 同上 | `runAlgorithm()` 的 WPL 段先累加到 `running`，循环结束后再 `m_wplTotal = running;` 才构造 `HF_WPL_DONE` 帧；修正后默认数据 WPL = 45×2 + 13×3 + 20×2 + 16×3 + 9×4 + 12×3 + 5×4 = **309**（等于全部内部结点权值之和 14+25+30+45+75+120，符合哈夫曼最优性）（BUG-202） |
| 哈夫曼编码回填 | 同上 | `computeCodes()` 求出 `m_codeMap` 后立即回填 `m_codeRows` 的 `code` / `codeLength`；`buildLayout()` 调用顺序改为 `computeTree() → simulateStates() → buildCodeTable() → computeCodes()`，修复 `HF_CODE` / `HF_WPL` 帧文案中编码与码长恒为空的问题（BUG-205） |
| 图形元素扩展 | `src/render/GraphElements.{h,cpp}` | `GraphNode` 新增 `Q_PROPERTY(qreal scale READ scale WRITE setScale)` 与 `m_scale`，`paint()` 内 `painter->scale(m_scale, m_scale)`，使 `playNodeAppear` / `playScalePulse` 的果冻缩放效果生效 |
| 动作码扩展 | `src/engine/AnimFrame.h` | 新增 `HF_SORT_LIFT` / `HF_SORT_DROP` / `HF_SORT_DONE` / `HF_QUEUE_DROP` / `HF_JELLY` / `HF_PROMOTE` / `HF_FINAL` |
| 测试同步 | `tests/HuffmanShot.cpp` + `tests/CMakeLists.txt` | 新增 `SmartGroundHuffmanShot` 逐帧截图生成器，覆盖排序悬停 / 落位 / 候选队列 / 逐次合并 / 最终树 / 编码表 / WPL 六阶段共 41 帧 |

## 2026-11-17 第 18 批：扩展欧几里得推导链重写 + 哈夫曼编码表/WPL + LCS 坐标修正

| 项目 | 涉及文件 | 实现要点 |
|---|---|---|
| 扩展欧几里得三阶段推导链 | `src/modules/ExgcdModule.{h,cpp}`（整模块重写） | 默认数据改为 `11914 146415`（求 11914 在模 146415 下的乘法逆元）。**第一步**画布上方三行同余式变形：`11914·x ≡ 1 (mod 146415)` → `11914·x = 146415·y + 1` → `11914·x - 146415·y = 1`，`EX_EQUATION` 逐行高亮。**第二步**双栏对照表：左栏取余 `146415 % 11914 = 3447`、右栏整除式 `146415 = 12 × 11914 + 3447`，共 9 轮；「行」指针（`Direction::Left` + 锚点偏移 -26px）锚定当前行并左右两式同时高亮。**第三步**递归回代：`buildBackSteps()` 用「项表（index, coef）」递推，从 `1 × gcd` 起每轮把最高次项按 `r_k = r_{k-2} - q·r_{k-1}` 展开并合并同类项，回代式由右侧整除式行飞入并展开为 `1 = 10 × 1 - 1 × 9` → `1 = 10 - (29 - 2 × 10) = 3 × 10 - 29 × 1` → … → `1 = 15079 × 11914 - 1227 × 146415`；终帧绿色结论「11914 在模 146415 意义下的乘法逆元是 15079」（BUG-199） |
| 扩展欧几里得取值上限 | 同上 | 输入上限由 `100000` 放宽到 `1000000`，`getInputHint()` 同步说明（BUG-200） |
| 哈夫曼编码表 + WPL | `src/modules/HuffmanTreeModule.{h,cpp}` | 树右侧新增 `n × 3` 编码表：第 1 列 `a[i]`（**初始输入顺序**，非排序后顺序）、第 2 列哈夫曼编码（左 0 右 1）、第 3 列码长；`HF_CODE` 帧沿根到叶逐条揭示路径上的 0/1 标签并写入表格。底部新增 WPL 行：`n` 个格子逐项显示 `a[i]×码长`，右侧 `WPL` 累加格逐项刷新并脉冲高亮，`HF_WPL_DONE` 给出总 WPL |
| 哈夫曼建树向上生长 | 同上 | 树自上而下：根在顶部、叶子在底部；每次合并后兄弟子树先旋转/上移对齐到同一行再合并，`computeXPositions()` 让每行居中、`levelMin/levelMax` 逐行整体平移使各行对称，最终根节点居于树内正中 |
| 哈夫曼路径标注与编码 | 同上 + `src/render/GraphElements.{h,cpp}` | `GraphEdge` 新增 `pathLabel` / `pathLabelColor` / `setPathLabel()` / `pathLabelAnchor()`，在边中点绘制圆角徽标显示左 0 右 1 |
| LCS 坐标修正 | `src/modules/LCSModule.cpp` | `kLcStrY` 62 → 92、`kLcGridY` 206 → 236；「串 a[]」/「串 b[]」标签上移到 `kLcStrY - 26`，矩阵列下标移到 `m_gridY - 22`，行下标移到 `m_gridX - 34`，标签与下标各占独立行彻底消除压字（BUG-201） |
| 共享动画助手 | `src/modules/IModule.{h,cpp}` | 新增 `playValueDropInto(start, target, value, color, duration)`：数值从空中落入数组格中心（`OutCubic`）并在动画结束时提交值；新增 `playTextFlight(text, start, end, fontSize, color, duration)`：任意文本飞行（哈夫曼路径标注与扩欧回代式飞入共用） |
| 图形元素扩展 | `src/render/GraphElements.{h,cpp}` | `ArrayBlock` 新增 `centerPos()`（格子中心坐标）；`FloatingText` 新增 `fontSize` / `textColor`；`GraphEdge` 新增路径标签徽标 |
| 动作码扩展 | `src/engine/AnimFrame.h` | 新增 `EX_EQUATION`、`HF_CODE`、`HF_WPL`、`HF_WPL_DONE` |
| 测试同步 | `tests/Batch9ModulesTest.cpp` | 扩展欧几里得用例改用 `11914 146415`，新增断言：逆元 `x == 15079`、`11914×15079 - 146415×1227 == 1`、最终回代式同时含 `15079` 与 `1227`、`EX_EQUATION` 帧数为 3 |

## 2026-11-17 第 17 批：哈夫曼树动画重构 + 多模块排版修复

| 项目 | 涉及文件 | 实现要点 |
|---|---|---|
| 哈夫曼树动画重构 | `src/modules/HuffmanTreeModule.{h,cpp}`（整模块重写） | 输入 `n` + `n` 个权值。**阶段一**（排序）：不在应有位置的元素先飞出数组并高亮（`HF_SORT_LIFT`），再逐个像流星一样飞回排序后的位置（`HF_SORT_DROP`，帧内携带完整置换数组保证可重放）；`HF_SORT_DONE` 宣布排序完成。**阶段二**（建树）：每次取森林中两个最小权值，`HF_PICK`（取最小并高亮）→ `HF_LIFT`（飞到一起 / 带子树的一方上移到同一行）→ `HF_MERGE`（连线生成父节点）；三种情况分别对应「都是原始数据 → 作叶子」「一个带子树 → 另一个飞过去相连」「两个都带子树 → 先对齐树高再相连」。树与边初始 `opacity = 0`，随合并逐个 `playNodeAppear()` 生长淡入（BUG-188 / BUG-198） |
| 哈夫曼树位姿补间 | 同上 | `simulateLayout()` 预先模拟每次合并后的森林形态，记录每步的 `rows` / `cols` 快照；`computePositions()` 采用「叶子等距铺开 + 内部结点取左右孩子中点」的对称布局；每次合并先补间到「上移对齐」位姿（300ms），再补间到「合并完成」位姿（460ms），树高与树宽变化均有过渡（BUG-189） |
| 共享动画助手 | `src/modules/IModule.{h,cpp}` | 新增 `playMoveTo(std::vector<QGraphicsItem*> items, std::vector<QPointF> targets, int duration = 440)`：逐项独立目标点的位置补间，`m_animated == false` 时瞬时落位，供哈夫曼树等需要「每个元素各自去往不同目标」的场景复用（BUG-189） |
| 桶排序排版 | `src/modules/BucketSortModule.{h,cpp}` | 桶内竖直数组改为 `cellHeight == spacing`（40px）无缝排布，相邻单元格之间只保留 1 条横线；章节说明文字上移到 `(40, 180)`、桶头单元格下移到 `y = 250`，Bucket 0 标题不再被压住（BUG-191 / BUG-192） |
| 基数排序排版 | `src/modules/RadixSortModule.{h,cpp}` | 10 个数字桶同样改为 `cellHeight == spacing`（38px）无缝排布；「按当前位分的 10 个桶」上移到 `(40, 154)`，数字标题整合进桶头单元格（BUG-191 / BUG-192） |
| 循环队列清理 | `src/modules/CircularQueueModule.{h,cpp}` | `head` / `tail` 改 `Direction::Up` + `PointerGeometry::belowCell()`，从数组下方指上去且箭头尖端停在单元格外（`head == tail` 时额外偏移避免箭头重叠）；删除 `q[N], N = 6` 标题、右下角「队尾走到末尾后回到队头」提示、以及整套「操作日志」方框与标题（BUG-193） |
| 循环队列归类 | `src/ui/MainWindow.cpp` | `CircularQueueModule` 由「数据结构 · 进阶」移入「基础数据结构」（BUG-194） |
| 字符串哈希对标模板 | `src/modules/StringHashModule.{h,cpp}` | `p1 = 131`、`m1 = 998244353`；`p[i] = p[i-1]*p1 % mod`；`h1[i] = (h1[i-1]*p1 + s1[i-1]) % mod`；`h2[i]` 同理；从 `len = min(n1,n2)` 递减枚举，判断 `getHash(h1,1,len) == getHash(h2,n2-len+1,n2)` 或 `getHash(h2,1,len) == getHash(h1,n1-len+1,n1)`，首个匹配即答案；画布重排为 s1 / h1 / s2 / h2 / p 五行（BUG-195） |
| 字符串哈希指针与动作码 | 同上 + `src/engine/AnimFrame.h` | 新增 `STH_POW` 动作类型专门记录幂表计算（原与 `STH_PREP` 混用）；新增字符游标指针 `i`（s1 行）与 `j`（s2 行），均 `Up` 方向 + `PointerGeometry::belowCell()`（BUG-196） |
| 最长公共子序列布局 | `src/modules/LCSModule.{h,cpp}` | 删除左上角 `s = "abcbdab"`；矩阵上方横排两个无缝字符串数组（a 串 / b 串，中间留 40px 间隙）；指针 `i` 自上而下指向 a 串元素、指针 `j` 自下而上指向 b 串元素；底部轨迹区重排为 追踪标签(624) / 追踪格(650) / 说明(712) / 结论(746)（BUG-197） |
| 动作码扩展 | `src/engine/AnimFrame.h` | 新增 `HF_SORT_DONE`；`STH_POW` 与 `STH_PREP` 拆分（BUG-196） |
| 测试同步 | `tests/Batch9ModulesTest.cpp` + `tests/Batch9VisualTest.cpp` | 字符串哈希测试输入由 `abcdefg / cde`（无公共前后缀，答案 0）改为 `abcdefg / cdefg`（答案 5），并新增答案断言；`Batch9VisualTest` 新增哈夫曼树专项用例：断言排序阶段的抬升 / 落位帧、排序先于建树、6 个叶子合并 5 次、终态 11 结点 10 边 |

## 2026-11-17 第 16 批：8 个新算法模块 + 矩阵方括号外扩 + 指针外置化

| 项目 | 涉及文件 | 实现要点 |
|------|----------|----------|
| 矩阵方括号包裹矩阵 | `src/render/MatrixBlock.{h,cpp}` | `PlainMatrix` 分支的折线括号改为上下各外扩 `over = 9.0`、左右各外扩 `3.0`，`arm = 14.0` / `lip = 7.0`；`sideMargin()` 20 → 26 以容纳括号臂；`boundingRect()` 按形态引入 `vPad`（矩阵 13.0，其余 4.0），保证外扩部分不被裁剪（BUG-184） |
| Trie 游标 `i` 外置 | `src/modules/TrieModule.cpp` | `i` 指针锚点偏移由 `(cellW/2, 19)` 改为 `PointerGeometry::aboveCell(54.0, 38.0)`，尖端落在格子上边缘上方 2px，杆体与标签全部位于单元格外部（BUG-185） |
| 哈希表探针 `k` 外置 | `src/modules/HashTableModule.{h,cpp}` | `k` 指针同样改用 `PointerGeometry::aboveCell()`；补齐缺失的 `cellHeight()` 访问器（此前 `renderFrame` 编译不过）（BUG-186） |
| 桶排序 | 新增 `src/modules/BucketSortModule.{h,cpp}` | 输入 `n` + `n` 个 0~99 整数；5 个桶（每桶容量 4），`b = v / 20` 分桶 → 桶内插入排序 → 按桶序回填 `out[]`；数值以飞行轨迹落入输出格；动作码 `BK_RESET / BK_BUCKET / BK_SORT / BK_COLLECT / BK_DONE` |
| 基数排序 | 新增 `src/modules/RadixSortModule.{h,cpp}` | 输入 `n` + `n` 个三位数；LSD 三趟（个 / 十 / 百位），每趟 10 个数字桶，桶内顺序收集后回写源数组；动作码 `RD_RESET / RD_DIGIT / RD_BUCKET / RD_COLLECT / RD_DONE` |
| 循环队列 | 新增 `src/modules/CircularQueueModule.{h,cpp}` | 输入容量 `N` + `PUSH x` / `POP` 指令流；`head = (head + 1) % N`、`tail = (tail + 1) % N`，队尾走到末尾绕回下标 0 并高亮提示；`head` / `tail` 指针恒在单元格外部；动作码 `CQ_RESET / CQ_ENQUEUE / CQ_DEQUEUE / CQ_WRAP / CQ_FULL / CQ_EMPTY / CQ_DONE` |
| 单调队列 | 新增 `src/modules/MonotonicQueueModule.{h,cpp}` | 输入 `n` + `n` 个整数 + 窗口大小 `k`；`q[]` 存下标且对应数值单调递减，队头即窗口最大值；`MQ_EXPIRE` 淘汰滑出窗口者、`MQ_POP_BACK` 踢出队尾较小者；动作码 `MQ_RESET / MQ_PUSH / MQ_POP_BACK / MQ_EXPIRE / MQ_MAX / MQ_DONE` |
| 扩展欧几里得 | 新增 `src/modules/ExgcdModule.{h,cpp}` | 输入 `a b`；递归调用栈逐层可视化（`EX_RECURSE` / `EX_BASE`），回溯时逐层写回 `x = y'`、`y = x' - ⌊a/b⌋ y'`；动作码 `EX_RESET / EX_RECURSE / EX_BASE / EX_BACKTRACE / EX_DONE` |
| 哈夫曼树 | 新增 `src/modules/HuffmanTreeModule.{h,cpp}` | 输入 `n` + `n` 个权值；每次取权值最小的两个结点合并，新父结点动态创建并重新布局树深度；动作码 `HF_RESET / HF_PICK / HF_MERGE / HF_DONE` |
| 字符串哈希 | 新增 `src/modules/StringHashModule.{h,cpp}` | 输入 `s` + 待查询子串；前缀哈希 `h[i] = (h[i-1] · p + code) mod M` 与幂表 `p^k` 预计算，子串哈希 `h[r] - h[l-1] · p^(r-l+1) mod M` 做到 O(1) 查询；动作码 `STH_RESET / STH_PREP / STH_POW / STH_QUERY / STH_MATCH / STH_DONE` |
| 最长公共子序列 | 新增 `src/modules/LCSModule.{h,cpp}` | 输入两行字符串 `s` / `t`；`dp[i][j]` 二维网格，字符相同取左上角 +1，不同取左 / 上两者最大值并高亮候选来源；最后沿对角线回溯出 LCS 并填入回溯路径条；动作码 `LC_RESET / LC_CANDIDATE / LC_DECIDE / LC_TRACE / LC_DONE` |
| 动作码扩展 | `src/engine/AnimFrame.h` | 新增 39 个 `ActionType` 枚举值，按模块前缀（`BK_` / `RD_` / `CQ_` / `MQ_` / `EX_` / `HF_` / `STH_` / `LC_`）隔离 |
| 模块注册 | `src/ui/MainWindow.cpp` | 桶排序 / 基数排序 → 排序；循环队列 → 数据结构 · 进阶；单调队列 / 哈夫曼树 → 贪心；字符串哈希 → 字符串；扩展欧几里得 → 数学；最长公共子序列 → 动态规划 |
| 构建注册 | `CMakeLists.txt` + `tests/CMakeLists.txt` | 8 个模块的 `.h` / `.cpp` 同时加入主工程与测试工程源文件列表 |
| 新增测试 | `tests/Batch9ModulesTest.cpp`（新增） + `tests/Batch9VisualTest.cpp`（新增） + `tests/GroupAUiTest.cpp` + `tests/PointerAlignTest.cpp` + `tests/PointerMotionTest.cpp` | `Batch9ModulesTest` 逐模块验证帧数、动作码计数与数值正确性（如桶排序输出 `8 11 13 22 26 30 37 45`、基数排序末趟升序、LCS 长度 4）；`Batch9VisualTest` 逐帧审计所有数组指针的箭头尖端是否越出单元格外框，并断言矩阵方括号的外扩量；`GroupAUiTest` 模块数 54 → 62 并逐个断言新模块名；`PointerAlignTest` / `PointerMotionTest` 的期望坐标改为外置后的 `-2.0` / `116.0` |


## 2026-11-16 第 15 批：矩阵形态与尺寸 + 指针动画体系 + 分类合并

| 项目 | 涉及文件 | 实现要点 |
|------|----------|----------|
| 矩阵形态语义反转 | `src/render/MatrixBlock.{h,cpp}` | `paint()` 三形态分流：`PlainMatrix` 绘制数学方括号 `[` `]`（多段折线，`arm = 13` / `lip = 6`）；`Determinant` 保留左右单竖线 + 上下小衬线；`BracketedTable` 不画任何外框，`sideMargin()` 收到 4.0（BUG-181） |
| 单元格大小宽高可调 | `src/modules/ModuleFields.{h,cpp}` + `src/modules/SandboxMatrixModule.{h,cpp}` + `src/render/MatrixBlock.{h,cpp}` | `ModuleField` 新增 `SizeInput` 类型与 `makeSize(key, label, w, wMin, wMax, h, hMin, hMax)`；面板渲染为「宽 × 高」双数值输入；`MatrixBlock` 新增 `cellHeight`（默认 52）与 `setCellHeight()` / `cellHeightValue()`，绘制、`cellAtPoint()`、就地编辑框定位全部改用矩形单元格（BUG-180） |
| 画布残影清理 | `src/ui/MainWindow.{h,cpp}` | 新增 `invalidateCanvas()`：对场景所有视图调用 `invalidateScene(rect, BackgroundLayer)` + `invalidateScene(rect, ItemLayer)`，再 `viewport()->update()`；`clearScene()` 与 `rebuildCurrentLayout()` 末尾各调用一次（BUG-182） |
| 指针平移动画体系 | `src/render/GraphElements.{h,cpp}` + `src/modules/IModule.{h,cpp}` | `PointerBlock` 新增 `m_placed` 标记与 `isPlaced()`（经 `xChanged` / `yChanged` 信号捕获）、`anchoredPos()`、`setLabelAtTail(gap)`；`boundingRect()` 在 `opacity() < 0.02` 时返回空矩形并监听 `opacityChanged`；`IModule` 新增 `retargetPointer()` / `retargetPointerToNode()` / `retargetPointerTo()`，`playPointerMove()` 在指针首次落位时直接定位、之后一律走 `QPropertyAnimation` 平移（BUG-177） |
| 指针落位与标签归位 | `src/modules/TrieModule.cpp` + `src/modules/HashTableModule.{h,cpp}` + `src/modules/PrimModule.cpp` | Trie 的 `i` 锚点偏移 `(27, 19)` 精确落在格子中心、标签用 `setLabelAtTail(10.0)` 推到杆尾；哈希表的 `k` 下移至槽位行（`Down` 方向、`shaftLength = 26`、偏移 `(cellW / 2, 19)`）；Prim 的 `u` 改 `Up` 方向、偏移 `(0, 20)`、`shaftLength = 24`，尖端落在顶点圆边缘（BUG-178 / BUG-179 / BUG-174） |
| 结点生成动画 | `src/modules/IModule.{h,cpp}` + `src/modules/TrieModule.cpp` | 新增 `playNodeAppear(item, duration)`（`QEasingCurve::OutBack`，0.15 → 1.14 → 1.0 缩放弹入 + 透明度淡入）与 `playFadeIn(item, duration)`；`TrieModule` 的 `TR_CREATE` 帧新建结点时播放生长动画，不再突兀冒点（BUG-178） |
| 二分图数组下移 | `src/modules/BipartiteModule.cpp` | 图心 `(470, 210)`、节点间距放宽到 115/135；`col[]` 标签下移至 `y = 470`、数组行下移至 `y = 487`、结论文字下移至 `y = 610`；数组初值由 `"—"` 改为 `"-1"`（BUG-176） |
| 最小生成树分类合并 | `src/ui/MainWindow.cpp` | Kruskal 与 Prim 合并入单一分组「图论 · 最小生成树」，去重后唯一模块数 56 → 54（BUG-175） |
| 字符损坏修复 | `src/modules/EulerModule.cpp` | 修正 `统计每个节点的入度与出度` 中的损坏字节（BUG-173a） |
| 字体统一收尾 | `src/render/GraphElements.cpp` + `src/modules/HeapBasicsModule.cpp` + `src/modules/TarjanModule.cpp` + `src/modules/GraphRenderHelper.h` | `GraphNode::paint()`（中心值 / `v:` / `dfn: low:`）、`StackBlock::paint()`、`ArrayBlock::paint()`、`HeapBasicsModule` 英文标题、`TarjanModule` 的 `ins / stk`、`GraphRenderHelper::addLabel()` 全部改走 `UiFonts::mono()` / `UiFonts::cjk()`；`src/` 下不再有绕过字体引擎的 `QFont(...)` 字面量（BUG-183） |
| 指针动画路径收敛 | `src/modules/IModule.cpp` + `src/modules/StackModule.cpp` + `src/modules/ForwardStarModule.cpp` | `playPointersSync()` / `layoutPointerSlots()` / `StackModule` / `ForwardStarModule` 四处各自手写的 `acquireAnim(ptr, "pos", ...)` 统一改调 `playPointerMove()`，全项目指针移动只有一条动画路径（BUG-177） |
| 新增 / 修订测试 | `tests/GroupAUiTest.cpp` + `tests/OverlapAuditTest.cpp` + `tests/PointerMotionTest.cpp`（新增） + `tests/SandboxVisualTest.cpp` | `GroupAUiTest` 期望模块数 56 → 54；`OverlapAuditTest` 采集 `PointerBlock::anchorItem()`，指针与其锚定目标的有意压叠不再计为冲突；`PointerMotionTest` 断言指针落位状态、终态坐标与顶点圆边缘贴合 |

## 2026-11-16 第 14 批：10 项缺陷整改 + 分类重构 + 版本徽标

| 项目 | 涉及文件 | 实现要点 |
|------|----------|----------|
| 折叠面板内容不可见 | `src/ui/widgets/MyCollapsible.{h,cpp}` | `m_contentHeight` 原先只在 `setContentWidget()` 且布局未建立时算一次，高度被钉为 0。新增 `refreshContentHeight()`（`updateGeometry()` + `adjustSize()` + `sizeHint()` 三级回退测量）并同步 `setMaximumHeight`/`setMinimumHeight`；`eventFilter()` 拦截内容控件 `QEvent::LayoutRequest` 自动重测；`setContentWidget()` 统一走 `updateGeometryAnimated()`（BUG-173 / BUG-172） |
| GCD 整模块重写 | `src/modules/GcdModule.{h,cpp}` | 删除堆叠的 `ValueBar` 槽位，画面只留 `a` / `b` 两个大号数字框与余数框 `r`，中央直书 `a % b = r`；每轮较小者被 `a % b` 替换较大者，历史轮次变灰（BUG-166） |
| DFS 指针标签裁切 | `src/render/GraphElements.{h,cpp}` | `PointerBlock::labelWidth()` 与 `paint()` 统一 `UiFonts::mono(14, true)`，宽度改 `qMax(28.0, adv + 14.0)`；新增 `setAnchorItem(QGraphicsItem*)`，指针可锚定任意图元（`ArrayBlock` / `GraphNode` / 其它）（BUG-165） |
| Prim 提前终止 | `src/modules/PrimModule.cpp` | `runAlgorithm()` 主循环前先 `relaxFrom(m_src)` 播种起点的出边候选集，避免第一轮候选集为空被误判为「存在孤立分量」（BUG-167） |
| 计数排序前缀和 | `src/modules/CountingSortModule.cpp` | 前缀和阶段改用 `IModule::playMultiValueFlight`，`prefix[i-1]` 与 `cnt[i]` 同时飞出合并落入 `prefix[i]`（BUG-168） |
| 二分图染色错乱 | `src/modules/BipartiteModule.{h,cpp}` | 染色写入与色块刷新彻底隔离，子节点染色不再覆写父节点色块；图内结点背景色与数组颜色标记同源刷新（BUG-169） |
| 欧拉路径有向 / 无向 | `src/modules/EulerModule.{h,cpp}` | 新增有向图开关；有向时分别统计并渲染 `in[]` / `out[]` 双数组，无向时回落 `deg[]`；新增 `EU_DEGREE` 动作码（BUG-172） |
| Trie 动态建树 | `src/modules/TrieModule.{h,cpp}` | 起始只有根节点、游标 `p` 指向根；字符串以数组呈现，指针 `i` 逐字符右移；逐步检测子节点命中复用（`TR_HIT`）/ 未命中新建（`TR_CREATE`）；下方用一条无缝数组模拟树形结构内存槽位（BUG-170） |
| 哈希表无缝 + 探测 | `src/modules/HashTableModule.{h,cpp}` | 数组改 `cellWidth == spacing` 无缝排布；默认数据改 `5\n3 10 17 24 31`（模 7 后全冲突）完整演示线性探测；新增 `HT_PROBE` 逐步探测动画（BUG-171） |
| 核心层自适应延时 | `src/modules/IModule.{h,cpp}` | 新增 `static int frameDelayMs(const AnimFrame&)` 与 `static int frameDelayMsForText(const QString&)`，按「解说 + 公式」长度计算：下限 900ms、上限 3000ms；`MainWindow::advanceFrame()` 改调基类实现（BUG-165a） |
| 分类按 OI-Wiki 重排 | `src/ui/MainWindow.cpp` | 下拉分组调整为：自由结构 / 基础数据结构 / 排序 / 树与堆 / 数据结构 · 进阶（并查集、Kruskal、树状数组、线段树、**哈希表**）/ 图论 / 匹配 / 动态规划 / 字符串 / 数学 / **贪心**（区间调度）/ 树形 DP +「图论 · 最小生成树」跨分支组；哈希表归入数据结构，贪心独立成组 |
| 版本徽标 | 新增 `src/core/AppVersion.h` + `src/ui/MainWindow.cpp` + `CMakeLists.txt` + `tests/CMakeLists.txt` | `AppVersion::display()` 返回 `0.7.<stamp>`；`MainWindow` 状态栏右下角新增 `labelVersion`（objectName `versionBadge`，弱化配色 + tooltip `SmartGround v0.7.<stamp>`）；CMake 配置阶段自动取 `git rev-list --count HEAD` 作为构建序号并写回 `generated/AppVersionGenerated.h`（工作区有未提交改动时 +1）；`AppVersion.h` 用 `__has_include` 探测生成头，缺失时回落到 `0.7.0`；测试工程同样生成该头并加入 `SgUi` / 目标的 include 路径 |
| 新增测试 | `tests/Round10FixTest.cpp` + `tests/AutoPlayEndTest.cpp` + `tests/UiBehaviorTest.cpp` + `tests/VersionBadgeTest.cpp` | `Round10FixTest` 覆盖九项整改的行为断言（无缝行探针坐标随布局更新）；`AutoPlayEndTest` 的等待预算由 600×120ms 放宽到 4000×120ms（自适应延时使冒泡排序全流程超过原 72s 预算）；`UiBehaviorTest` 的延时上限断言改为 3000ms；`VersionBadgeTest` 断言版本号格式为 `主.次.构建`、徽标存在且文本与 `AppVersion::display()` 一致 |

## 2026-11-16 第 13 批：矩阵设置落实 + 9 个新算法模块

| 项目 | 涉及文件 | 实现要点 |
|------|----------|----------|
| 矩阵设置面板 | `src/modules/SandboxMatrixModule.{h,cpp}` | `moduleSettingsFields()` 返回 `Segment("shape", 形态, {矩阵, 行列式, 表格}, 2)` + `IntInput("rows", 行数, 3, 1, 8)` + `IntInput("cols", 列数, 3, 1, 8)` + `TextInput("cell", 单元格)`；`moduleSettingsTitle()` 返回「矩阵设置」；`buildLayout` 删去全部 `rows=` / `cols=` / `type=` / `cell=` 键解析，改为把输入区所有 token 按行优先填入 `MatrixBlock`，不足处补 `0`；`applyShape()` 切换三种形态并同步配色（行列式=米黄底/棕竖线、表格=白底/深蓝竖线、矩阵=无竖线）；`applyDimensions()` 调整行列数时保留已有数据（BUG-162） |
| 面板自动隐藏 | `src/ui/MainWindow.cpp` + `src/ui/MainWindow.h` | `rebuildModulePanel()` 在 `fields.isEmpty() && !graphSupported` 时隐藏 `modulePanelHost`，因此「矩阵设置」只在矩阵模块下出现；新增 `probeModulePanelVisible()` / `probeModulePanelTitle()` 供测试断言 |
| GCD 辗转相除 | 新增 `src/modules/GcdModule.{h,cpp}` + `src/render/ValueBar.{h,cpp}` | 输入 `a b`；按 `a = a % b` 迭代，`ValueBar` 比例条表达数值大小，历史轮次逐条变灰，右下角余数条脉冲高亮；动作码 `GC_RESET / GC_DIVIDE / GC_SWAP / GC_DONE` |
| DFS 深度优先搜索 | 新增 `src/modules/DfsModule.{h,cpp}` | 输入 `n m src` + 边表；显式栈 `stk` / 访问标记 `vis` / 访问序 `ord` 三组数组，指针随栈顶移动，节点三色态（待访问 / 栈中 / 已完成）；动作码 `DFS_RESET / PUSH / EDGE / VISIT / POP / DONE` |
| Prim 最小生成树 | 新增 `src/modules/PrimModule.{h,cpp}` | 输入 `n m src` + 带权边；`dis[]` 数组记录到生成树的距离，每轮高亮最小候选边并松弛邻点；动作码 `PR_RESET / SELECT / RELAX / TAKE / FINISH` |
| 计数排序 | 新增 `src/modules/CountingSortModule.{h,cpp}` | 输入 `n` + 数组；三阶段：计数桶 → 前缀和 → 从后往前回填，数值以飞行轨迹落入桶位；动作码 `CS_RESET / COUNT / PREFIX / PLACE / DONE` |
| 二分图判定 | 新增 `src/modules/BipartiteModule.{h,cpp}` | 输入 `n m` + 边表；BFS 交替染色，冲突边以警示色高亮并终止；动作码 `BP_RESET / COLOR / CONFLICT / FINISH` |
| 欧拉路径 | 新增 `src/modules/EulerModule.{h,cpp}` | 输入 `n m` + 边表；先算度数并判定欧拉通路存在性，再用 Hierholzer 栈式算法回溯生成路径，路径边逐条点亮；动作码 `EU_RESET / PUSH / EDGE / POP / PATH / DONE` |
| 区间调度 | 新增 `src/modules/IntervalSchedModule.{h,cpp}` + `src/render/SegmentBar.{h,cpp}` | 输入 `n` + `n` 行 `l r`；按结束时间排序后逐条判定是否与已选活动冲突，冲突条灰化半透明，选中条加粗高亮；动作码 `IV_RESET / SORT / SCAN / PICK / REJECT / DONE` |
| Trie 字典树 | 新增 `src/modules/TrieModule.{h,cpp}` | 输入 `n` + `n` 个单词；按字符逐层下行，已有分支复用、新分支才建节点，末节点 `end` 标记为 1；动作码 `TR_RESET / CREATE / HIT / END / DONE` |
| 哈希表（开放寻址） | 新增 `src/modules/HashTableModule.{h,cpp}` | 输入 `n` + `n` 个关键字；`h(k) = k % 7`，冲突时线性探测下一空槽，槽位行显示哈希值、关键字行显示落位元素；动作码 `HT_RESET / HASH / PROBE / INSERT / DONE` |
| 动作码扩展 | `src/engine/AnimFrame.h` | 新增 34 个 `ActionType` 枚举值，按模块前缀（`GC_` / `DFS_` / `PR_` / `CS_` / `BP_` / `EU_` / `IV_` / `TR_` / `HT_`）隔离，避免与既有模块动作码冲突 |
| 模块注册 | `src/ui/MainWindow.cpp` | 9 个新模块分别挂到「经典排序」（计数排序）、「图论算法」（DFS、二分图、欧拉路径）、「字符串算法」（Trie）、「数论与树形」（GCD）、「贪心与哈希」（区间调度、Prim、哈希表）分组 |
| 新增测试 | `tests/NewModuleBatchTest.cpp` + `tests/GroupAUiTest.cpp` + `tests/ModuleSettingsPanelTest.cpp` + `tests/MatrixShapeTest.cpp` + `tests/OverlapAuditTest.cpp` | 前者验证 9 个模块均能产生帧并渲染非空画面，并断言矩阵默认形态/尺寸/行优先填值；`GroupAUiTest` 模块数由 46 改为 56 并逐个断言新模块名；`ModuleSettingsPanelTest` 断言「矩阵设置」面板在矩阵模块可见、切到 01 背包自动隐藏、切回恢复；`MatrixShapeTest` 改为纯行优先输入；`OverlapAuditTest` 扩测 9 个新模块并把 `GraphNode` 审计框收紧为真实圆框（BUG-164） |


## 2026-11-15 第 12 批：弹层 DWM 原生边框清除 + 侧栏悬空指针修复

| 项目 | 涉及文件 | 实现要点 |
|------|----------|----------|
| 弹层右/下边缘竖线 | `src/ui/widgets/MyComboBox.{h,cpp}` | `ComboPopupFrame` 构造追加 `setWindowFlags(windowFlags() | Qt::NoDropShadowWindowHint)`；`openPopup()` 改用 `setWindowFlags(Qt::Popup | Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint)`，关闭 DWM 原生描边与系统阴影；`ComboPopupFrame::paintEvent` 的 14 层同心圆角矩形扩散半径由 `i * 2.2` 收敛为 `maxSpreadX = 11` / `maxSpreadY = 6` 并改线性 alpha 衰减，阴影层完全落在窗口边界内不产生折角；外边距 (27,18,27,20) → (18,12,18,14)，窗口宽度 `popupWidth + 54` → `popupWidth + 36`（BUG-159） |
| 侧栏切换模块崩溃 | `src/ui/MainWindow.cpp` | `rebuildModulePanel()` 清空 `modulePanelContent` 前先 `toggleDirected->setParent(modulePanelHost)` / `checkBidirectional->setParent(modulePanelHost)` 把复用控件移出待删集合，并在 `deleteLater()` 判断中跳过二者；非图模块分支显式 `setVisible(false)`、图模块分支恢复 `setVisible(true)`；拆开 `updateGraphOptionPanel()` 与 `animateSubOptionVisibility()` 中粘连的语句（BUG-160） |
| 折叠面板标题访问器 | `src/ui/widgets/MyCollapsible.h` | 新增 `QString headerText() const` 访问器，供测试按标题定位「特化设置」面板（BUG-161） |
| 新增测试 | `tests/PopupEdgeVerify.cpp` | 真实屏幕像素级回归：断言弹层带 `NoDropShadowWindowHint` / 无边框 / 半透明，并在真实屏幕抓图中扫描卡片右外侧与下外侧的空白区，确认不存在贯穿式硬线；`offscreen` 平台自动跳过 |

## 2026-11-15 第 11 批：下拉收口 + 图结点权值入圆 + 矩阵族重构

| 项目 | 涉及文件 | 实现要点 |
|------|----------|----------|
| 弹层硬边阴影清除 | `src/ui/widgets/MyComboBox.cpp` | 删除 `ComboCard` 上的 `QGraphicsDropShadowEffect`（该控件把阴影裁到自身矩形，右侧与下侧留下包裹结构的硬边）；`ComboPopupFrame::paintEvent()` 改为 12 层同心圆角矩形由外到内叠加，alpha 递减、圆角随扩散半径同步放大，边缘无折线；外边距 (20,18,20,20) → (27,18,27,20)，宽度补偿 +54（BUG-153） |
| 风格收敛为 Vue 扁平 | `src/ui/widgets/MyComboBox.{h,cpp}` + `src/ui/MainWindow.{h,cpp}` | `ComboVisualStyle` 删去 `SoftGlass` / `MinimalLine`；移除 `paintGlassTile` / `paintMinimalTile`、`ComboCard::paintEvent` 的分支、`ComboPopupFrame` 的阴影分支；`styleNames()` 只返回 `{"Vue 扁平"}`，`styleIndex()` 恒为 0；侧栏「界面风格」标签 + `comboStylePicker` 段控件整体删除（BUG-156） |
| 下拉字号放大 8~10% | `src/ui/widgets/MyComboBox.cpp` | 新增常量 `kOptionFontSize = 10` / `kHeaderFontSize = 10` / `kButtonFontSize = 11`；`ComboOption` 构造字号 9→10、`preferredWidth()` 内边距 34→40、`computeColumnWidth()` 列宽 `qBound(120,...,240)` → `qBound(140,...,300)`，弹层宽度补偿 +32→+44（BUG-157） |
| 图结点权值入圆 | `src/render/GraphElements.{h,cpp}` + `src/modules/IModule.cpp` | `commitEdit()` 统一写 `setCenterText(shown)`（圆内 `QRectF(-20,-20,40,40)`）并清空残留 `valueText`；`refreshLiveSlots()` 同步改为圆内写入；`beginEdit()` 预填优先级改为 圆内 > 自定义 > 下方 > `value`；新增 `belowText()` / `editorItem()` 访问器（BUG-154） |
| 矩阵族重构 | `src/render/MatrixBlock.{h,cpp}` + `src/modules/SandboxMatrixModule.{h,cpp}` | `MatrixBlock` 重写：新增 `sideMargin()`（行列式 18 / 表格 16）、`setCellSize()`、`barColor` 独立括号配色；行列式与表格改用真正竖线括号并带顶部延伸短线；单元格改 `mono(13)`，双击就地改值保留；`SandboxMatrixModule` 支持 `rows=` / `cols=` / `type=` / `cell=` 键，形态（矩阵 / 行列式 / 表格）可切换，行列数与单元格尺寸可调且保留已有数据，`buildLayout` 保留用户已覆盖的设置不被重建回退；新增 `releaseSceneItems()` 正确释放图元（BUG-155） |
| 特化设置面板统一 | `src/ui/MainWindow.cpp` | `rebuildModulePanel()` 把图结构选项（图类型开关 / 双向箭头复选框 / 边权提示）与模块自身 `ModuleField` 合并进唯一的「特化设置」折叠面板；`updateGraphOptionPanel()` 不再单独显示 `graphPanelHost`；分段控件独占整行，数值输入框固定 72px 宽（BUG-158） |
| 新增测试 | `tests/MatrixShapeTest.cpp`、`tests/ModuleSettingsPanelTest.cpp`、`tests/StylePickerTest.cpp`、`tests/GraphInlineEditTest.cpp`、`tests/PanelEdgeScanTest.cpp` | 分别验证三形态渲染与输入解析、面板字段类型与数值钳制与重建后保留、风格唯一性与弹层可开、权值提交后落圆内且圆下无文字、侧栏像素扫描无残留强调色块 |

## 2026-11-15 第 10 批：橡皮面板紧凑化 + 下拉气泡风格重做

| 项目 | 涉及文件 | 实现要点 |
|------|----------|----------|
| 橡皮面板贴合内容 | `src/ui/widgets/CanvasToolBar.{h,cpp}` | 新增 `applyToolPageSize()`：切换 / 重建工具页时按当前页 `sizeHint` 固定 `m_toolPages` 尺寸，绕开 `QStackedWidget` 取最大页高度的通病，橡皮面板由 477px 收敛到 103px（BUG-134） |
| 橡皮形状可选 | 新增 `src/ui/widgets/MySegmentedControl.{h,cpp}` + `ToolController.{h,cpp}` + `CanvasView.cpp` | `ToolStyle` 增 `eraserSquare`；`eraseAt()` 圆形走 `hitTest`、方形走场景路径与矩形求交；画布橡皮光标同步画圆 / 画方；面板加「圆形 / 方形」分段选择器 |
| 下拉气泡三套风格 | `src/ui/widgets/MyComboBox.{h,cpp}` | 引入 `ComboVisualStyle`（`VueLight` / `SoftGlass` / `MinimalLine`）：`VueLight` 为 Vue 式扁平瓦片（无左色条、无扫光、悬停 160ms 强调色淡入、选中用 34 透明底 + 3px 圆角指示条）；弹层阴影改 `QGraphicsDropShadowEffect` 真投影；卡片 16px 圆角极淡描边；分组标题改「6px 圆点 + 文字 + 渐隐细线」；下拉箭头开合旋转动画；中西文自动切字体（BUG-135） |
| 新增测试 | `tests/EraserPanelTest.cpp` + `tests/ComboStyleShot.cpp` + `tests/StylePickerTest.cpp` | 前者断言橡皮面板 103px 且矮于画笔面板；后者生成 `style_vue_light.png` / `style_soft_glass.png` / `style_minimal_line.png` 三张对照图；`StylePickerTest` 断言三段风格切换器存在、默认 VueLight、三次切换分别落到 SoftGlass / MinimalLine / VueLight 且弹层仍可打开 |

## 2026-11-15 第 10 批补记：风格切换入口

PM 定稿：**VueLight 为默认风格**，**保留三套并提供运行时切换入口**。

| 项目 | 涉及文件 | 实现要点 |
|------|----------|----------|
| 全局风格状态 | `src/ui/widgets/MyComboBox.{h,cpp}` | 新增静态实例表 `g_instances` 与 `setGlobalVisualStyle()`，切换时同步刷新所有 `MyComboBox` 实例；新增 `styleNames()` / `styleName()` / `styleIndex()` 供 UI 绑定 |
| 风格切换器 | `src/ui/MainWindow.{h,cpp}` | 左侧面板「算法模块」下方新增「界面风格」标签 + `MySegmentedControl`（Vue 扁平 / 柔光玻璃 / 极简描边），切换即时生效 |
| 审计断言修正 | `tests/StartupPanelTest.cpp` | 扫描带改为按 `comboStylePicker` 几何动态定位，避免风格指示条被误判为「左栏残留强调色块」（BUG-136） |

## 2026-11-15 第 9 批十四项需求轮

| 项目 | 涉及文件 | 实现要点 |
|------|----------|----------|
| 字体引擎 | 新增 `src/ui/UiFonts.h` | `UiFonts::cjk()`（Microsoft YaHei）/ `mono()`（Consolas）/ `mixed()`（`setFamilies({"Consolas","Microsoft YaHei"})` 按字形族回退）；全项目文本改用该引擎（BUG-151） |
| 编辑态快捷键隔离 | 新增 `src/core/TextEditGuard.h` + `src/canvas/CanvasView.cpp` + `src/ui/MainWindow.cpp` | 检测 `QApplication::focusWidget()` 或 `QGraphicsScene::focusItem()` 处于 `Qt::TextEditorInteraction` 时跳过画布快捷键拦截，双击单元格可正常输入数字键 1-7（BUG-146） |
| 指针几何重构 | `src/render/GraphElements.{h,cpp}` | `PointerBlock` 新增 `setShaftLength()` / `setLabelOffset()` / `setLabelAbove()`；四个方向的箭头尖端统一落在原点；标签偏移与杆长解耦，可甩到杆尾或上方；`boundingRect()` 改为「杆身 ∪ 标签」并集，与绘制严格一致（BUG-141） |
| 树状数组布局 | `src/modules/FenwickTreeModule.{h,cpp}` | `t[]` / `a[]` 下标行改为独立文本并置于方块上方；指针 `i` 自下而上指向目标块底边、标签落在层间空隙；`L0/L1/...` 层级标记移至左列，说明文字移至右列（BUG-141） |
| 链式前向星布局 | `src/modules/ForwardStarModule.{h,cpp}` | 四组数组由四列并排重构为**四行横排**；指针 `k` 与 `head[u]` 均自下向上指；标题移入左侧独立列；图节点中心右移到 1480 避开数组区（BUG-142） |
| 堆类模块布局 | `src/modules/HeapTreeBase.h` | 数组标题上移到 `arrY - 54`、树结构标题上移到 `treeTop - 92`，与根节点包围盒及下标行彻底分离（BUG-143） |
| 线段树指针 | `src/modules/SegmentTreeEditBase.cpp` + `src/modules/SegmentTreeBuildModule.cpp` | 指针 `node` 锚点偏移 `0,46` → `0,80`（BUG-144） |
| 拓扑排序布局 | `src/modules/TopologicalSortModule.cpp` | 结点层下移到 `150 + j*90`；输出序列行下移到 `degreeY+270`；`playPointerArrange` 偏移改 40（BUG-145） |
| 重叠自动审计 | 新增 `tests/OverlapAuditTest.cpp` | 渲染全帧后采集单元 / 下标行 / 指针 / 节点 / 文本的场景包围盒两两求交；区分「同一单元格的附属下标」与真实冲突；受测 10 个模块最终 `conflicts=0`（BUG-141） |
| 堆节点飞出 | `src/modules/IModule.{h,cpp}` + `HeapAdjustModule.cpp` + `HeapSortModule.cpp` | 新增 `playNodeSwapAnim()` / `playDualSwapAnim()`，数组值交换时树节点数值同步弧线飞出（问题 10） |
| 匈牙利默认数据 | `src/modules/HungarianModule.{h,cpp}` | 换成经典「分手重配」样例 `3 3 / 1 1 / 1 2 / 2 1 / 3 1 / 3 2 / 3 3`，补充换配关系说明文本（问题 11） |
| 下拉瓦片动效 | `src/ui/widgets/MyComboBox.{h,cpp}` | `ComboOption` 重做为圆角 11px 渐变瓦片；新增 `hoverProgress` / `pressProgress` 两个 `Q_PROPERTY` 驱动 `QPropertyAnimation`；hover 顶部柔光扫掠、按下半径收缩文字微沉；分组瓦片继承分类主色；弹层开合加透明度过渡与多层柔化投影（BUG-152） |
| 画板边框直拖 | `src/canvas/AnnotationItem.cpp` + `src/canvas/ToolController.cpp` | `borderHitTest(pos, tol)` + `borderItemAt(pos)`：命中轮廓环即进入 move 模式，无需先选中（问题 1） |
| 表格图标化 | 新增 `src/ui/widgets/TableIconButton.{h,cpp}` + `src/ui/widgets/TableToolBar.cpp` | 以 `QPainterPath` 手工绘制对齐 / 选区 / 增删行列矢量图标，替换中文按钮（问题 2） |
| 表格工具栏隐藏 | `src/ui/widgets/TableToolBar.cpp` | 校验表格对象有效性，表格删除 / 取消选中即刻隐藏（BUG-147） |
| 框选删除清理 | `src/canvas/ToolController.cpp` | `deleteSelected()` 同步清空 `m_activeTable`、发 `tableActivated(nullptr)`、取消全部选中（BUG-148） |
| 箭头笔尖对齐 | `src/canvas/ShapeItem.cpp` | 箭头多边形顶点严格落在 `m_end`，杆身截至 `m_end - (ux,uy) * head`（BUG-149） |
| 实时预览 | `src/canvas/TextBoxItem.cpp` + `src/canvas/MarkdownItem.{h,cpp}` | `notifyContentChanged()` 每次按键即重绘；`MarkdownItem` 拆为 `m_editor` + `m_preview` 双文档，输入时实时重排预览（BUG-150） |
| 坐标系规划 | `docs/outlines.md` | 新增「十六、几何与坐标系模拟器」章节：动态比例尺（`1 / 0.5 / 0.25 / 0.125`）、点插入命名、两点定线、表达式求值与曲线绘制；约束不引外部依赖、不套 `IModule` 帧体系；分四期推进（问题 13） |

## 2026-11-15 第 8 批八模块轮 · 七项可视化改进

| 项目 | 涉及文件 | 实现要点 |
|------|----------|----------|
| 埃氏筛保留数字 | `src/render/GraphElements.{h,cpp}` + `src/modules/SieveOfEratosthenesModule.cpp` | `ArrayBlock` 新增 `m_struck` 划除标记；被划掉的格子**保留数字**，底色转灰、叠加删除线、缩放至 0.82（BUG-134） |
| 帧滑块自动同步 | `src/ui/MainWindow.cpp` | `advanceFrame()` 调用 `syncFrameSlider()`，自动播放时右下角滑块与 "N / M" 徽标实时跟随帧索引（BUG-135） |
| 质数筛倍数批量流动 | `src/modules/SieveOfEratosthenesModule.{h,cpp}` + `src/engine/AnimFrame.h` | 新增 `SV_CROSS_BATCH` 动作；同一素数的全部倍数合并为**一帧**，`QVariantAnimation` 按时长 28ms/格 顺序划除并逐格发出涟漪（BUG-136） |
| 快速幂重做 | `src/modules/FastPowModule.{h,cpp}` + `src/engine/AnimFrame.h` | 删除 qmul 面板与 `FP_QMUL_*`/`FP_SQUARE`/`FP_SHIFT` 动作；2 个输入不取模、3 个输入按 p 取模；新增 `AnimFrame::big1~big4` 承载 64 位数值（修复精确模式 `a` 溢出显示 0）；二进制位条右端为第 0 位，每位合并为一帧，2^20 从 14 帧降到 7 帧（BUG-137） |
| 树直径节点加宽 | `src/modules/TreeDiameterModule.cpp` | `layoutTreeScene()` 叶子间距由 104.0 放宽到 178.0，`dep=...` 标签不再被相邻节点遮挡（BUG-138） |
| Dijkstra 数组化优先队列 | `src/modules/DijkstraHeapModule.{h,cpp}` + `src/render/GraphElements.{h,cpp}` | 删除二叉堆树视图，改为 `pq (堆)` 数组；`ArrayBlock` 新增 `setValueFontSize()`；单元格加宽到 98.0 并用 11 号字显示 `pair(dis, u)`；入堆时位置 k 之后的元素经 `playRowShift` 整体右移一格，新元素落入 k（BUG-139） |
| 前向星指针与染色 | `src/modules/ForwardStarModule.cpp` + `src/render/GraphElements.{h,cpp}` | `PointerBlock` 新增 `Direction { Up, Down, Left, Right }`；四列 X 间距放宽到 150（head 40 / to 215 / nxt 365 / weight 515）；指针 k 改用 `PointerBlock::Right` 从 to 列左侧 30px 处指向；`highlightCell()` 增加 `playScalePulse(1.18)` 染色高亮脉冲（BUG-140） |
| 新增测试 | `tests/LayoutFitTest.cpp` + `tests/FrameSliderTest.cpp` | 前者 3 项断言（Dijkstra pq 文本不溢出单元格、树直径标签不重叠、指针 k 与 to 列保持 30px 净空）；后者新增 4 项断言（自动播放时滑块前进且未跳到末尾、徽标文本跟随帧索引） |

## 2026-11-15 第 8 批八模块轮

| 项目 | 涉及文件 | 实现要点 |
|------|----------|----------|
| 希尔排序 | 新增 `src/modules/ShellSortModule.{h,cpp}` | 按 `gap = n/2 → 1` 折半；同余下标元素用 `LinkLine` 弧线连成一组（6 色轮转）；组内做插入排序，`i` / `j` 双指针（橙 / 蓝）标注待插入与回退比较位；后移走 `playShiftAnim`，落位染翠绿 |
| 快速幂（qpow） | 新增 `src/modules/FastPowModule.{h,cpp}` | 只展示 `qpow`：底数 a / 指数 b / 结果 res 三块面板 + 底部二进制位条（最右为第 0 位）；每帧完成「b%2 取位 → 写二进制位 → 必要时 res×a → a=a×a → b=b>>1」完整一步；支持 2 输入（精确值）与 3 输入（mod p）；`AnimFrame::big1~big4` 承载 64 位数值 |
| 埃氏筛 | 新增 `src/modules/SieveOfEratosthenesModule.{h,cpp}` | 只输入 n；右侧 `ceil(n/10)` 行 × **10 列**矩阵逐行排列 1..n；扫描线从左往右从上往下定位当前数字；确认素数后从 **i*i** 起按步长 i **一帧流动划掉全部倍数**（划除后保留数字、置灰打叉并缩至 0.82）；右侧 `prime[]` 表逐个淡入 |
| 链式前向星 | 新增 `src/modules/ForwardStarModule.{h,cpp}` | 左侧四列静态数组 `head[u]` / `to[k]` / `nxt[k]` / `weight[k]`（列距 150，指针 k 从 to 列左侧指向），右侧逻辑图；`add(u,v,w)` 头插全过程逐帧（`to[++tot]=v` → `nxt[tot]=head[u]` → `head[u]=tot`）；`to[k]` 连亮青实线箭头到图节点、`nxt[k]` 连紫虚线箭头到 `head[u]`，链表链一目了然；末尾 `for(i=head[u]; i; i=nxt[i])` 逐跳遍历 |
| 并查集 路径压缩 | 新增 `src/modules/DSUPathCompressionModule.{h,cpp}` | 森林图 + 右侧 `pa[]` / `sz[]` 两行数组；find 沿父指针上行（路径亮橙），回溯时路径上所有非根节点父指针一次性重定向到根，**整树压扁成星**；`x` 指针绑定结点随动（BUG-131） |
| 子树大小统计 | 新增 `src/modules/TreeSubtreeSizeModule.{h,cpp}` | 树 + 每结点 `sz=值` 徽章 + 底部后序序列条；子结点 sz 气泡脱落在树边弧线上滑向父节点（`playBubbleFlight`），碰撞相加数字跳动变大；父结点所有孩子到位才结算；底部序列逐个点亮（BUG-132 同类修复） |
| 树的直径 | 新增 `src/modules/TreeDiameterModule.{h,cpp}` | 两轮**无向** DFS：第一轮暖色维护 `dep[]` 并随时更新最深点 c，钉住 A（金框）；第二轮冷色从 A 出发找 B；`dep` 标注挂在结点上（叶子间距放宽到 178.0，标签不重叠）；A→B 路径逐段点亮成粗红线（BUG-132） |
| 堆优化 Dijkstra | 新增 `src/modules/DijkstraHeapModule.{h,cpp}` | 严格对齐「隐去 vis」的懒删除模板：只保留 `dis[]`（**不画 vis[]**），`if (d > dis[u]) continue` 过期条目标暗红直接丢弃；优先队列为**数组化 `pq (堆)`**（元素是 `pair(dis, u)`，单元格 98 宽 / 11 号字），入堆时插入位置 k 之后的元素整体右移一格，弹顶后剩余元素左移 |
| 共享图元扩展 | `src/render/GraphElements.{h,cpp}` | 新增 `LinkLine`（二次贝塞尔飞线，支持实线 / 虚线 / 箭头 / 弯曲量）与 `Ripple`（扩散波纹），均为自绘 `QGraphicsObject` |
| 共享动画扩展 | `src/modules/IModule.{h,cpp}` | 新增 `playRipple()`（波纹扩散）、`playLinkRedirect()`（飞线端点迁移）、`playBubbleFlight()`（数值气泡沿弧线飞行）；`m_transientItems` 拆为 `FloatingText` 与 `Ripple` 两个强类型向量（`QPointer` 仅支持 `QObject` 派生，`QGraphicsItem` 不能直接包 `QPointer`） |
| 动作类型扩展 | `src/engine/AnimFrame.h` | 新增 40 个动作类型：`SH_*`（6）、`FP_*`（10）、`SV_*`（6）、`FS_*`（6）、`DP_*`（6）、`TSZ_*`（6）、`TD_*`（6）、`DH_*`（6） |
| 新增测试 | `tests/Batch8ModulesTest.cpp` + `tests/GroupAUiTest.cpp` | 前者 40 项断言覆盖八模块帧序、答案正确性（`3^13 mod 1e9+7 = 1594323`、120 以内 30 个素数、直径长度 5、`sz[1]=8`）与非法输入降级；后者新增 9 项断言 + 14 个模块下拉可达性 |

## 2026-11-15 画板四项缺陷修复轮

| 项目 | 涉及文件 | 实现要点 |
|------|----------|----------|
| 拖拽震动根治 | `src/canvas/ToolController.{h,cpp}` | `beginMoveSelection()` 记录按下点 `m_movePressPos`；`updateMoveSelection()` 改用**累计位移** `total = pos - m_movePressPos`，目标位置 `origin + total`；步长为零时跳过重绘；`finishMoveSelection()` 复位按下点与步长（BUG-127） |
| 编辑态快捷键隔离 | `src/canvas/ToolController.{h,cpp}` + `src/canvas/CanvasView.{h,cpp}` + `src/ui/MainWindow.cpp` | 新增 `ToolController::isInlineEditing()` 与 `handleKeyPress()`；`onKeyPress()` 改为返回布尔值，行内编辑时只交给 `m_editingItem->handleEditKey()` 并吞掉事件；`applyColorShortcut()` 编辑态返回 `false`；`MainWindow::keyPressEvent()` 的 Ctrl 段与普通段均先判编辑态；`beginInlineEdit()` 调 `viewport()->setFocus()` 夺取焦点；`CanvasView::keyPressEvent()` 让落在 view 上的按键也进入控制器，未处理的普通按键仍交回 `QGraphicsView`（BUG-128） |
| 三图标重绘 | `src/ui/widgets/ToolIconButton.cpp` | `drawPencil()` 改为经典铅笔：笔身 + 金属箍（两道横线）+ 木质锥尖 + 实心笔芯；`drawHighlighter()` 改为马克笔：笔帽 + 荧光色带 + 笔身 + 楔形笔头 + 底部高亮横线；`drawEraser()` 改为斜置橡皮块：顶面 + 正面 + 分隔带 + 底部划痕线 |
| 无极取色器重构 | `src/ui/widgets/MyColorPicker.{h,cpp}` | 新增自绘 `ColorField`（饱和度×明度二维区 + 连续色相条，按下即定位、拖动实时取色）与自绘 `ColorPreview`（纯色卡）；「渐变预设」→「预设」；色相滑条量程 0–359；`normalizedHue()` 处理灰色无色相；`m_syncing` 防信号回环，色值 / RGB / 滑条 / 预览实时同步（BUG-129） |
| 表格越界崩溃修复 | `src/canvas/CanvasTableItem.cpp` | `insertColumns()` 按 `count` 循环插入；`insertRows()` / `removeRows()` / `removeColumns()` 统一走 `rebuildCells()` 归一化维度；`paint()` / `cellAt()` / `setCell()` / `setCellAlign()` / `cellAlign()` 补齐越界保护（BUG-130） |
| 新增测试 | `tests/ColorPickerTest.cpp` + `tests/SmartCanvasTest.cpp` | 前者 11 项断言（逐像素采样色相条验证连续渐变与回绕、预览非黑、标签改名、拖动取色生效）；后者新增 6 项断言（拖拽累计位移精确等于整段光标行程、编辑态数字快捷键不改画笔颜色、字母快捷键不切换工具、输入字符仍进文本框） |

## 2026-11-15 画板十项需求轮

| 项目 | 涉及文件 | 实现要点 |
|------|----------|----------|
| 笔刷设置独立 + 记忆 | `src/canvas/ToolController.{h,cpp}` | 新增 `ToolStyle` 结构（color / width / opacity / eraserRadius / mosaicCell / tableRows / tableCols / textSize）与 `QHash<int, ToolStyle> m_styles`，按工具枚举索引独立保存；`styleFor()` / `setStyleFor()` 供 UI 读写；工具切换不互相污染，算法模块切换时 `ToolController` 生命周期长于模块，记忆自然保留 |
| 两段式属性浮层 | `src/ui/widgets/CanvasToolBar.{h,cpp}` | 属性页仍用 `QStackedWidget` 按工具分页；新增 `placeToolPanelUnderButton()`，浮层**精确对齐到当前工具按钮正下方**（居中于按钮，边界避让画布边缘），切换工具时实时重定位 |
| 向心 Catmull-Rom 笔迹 | `src/canvas/StrokeItem.{h,cpp}` | `kCentripetalAlpha = 0.5`，节点间距按 `‖Δ‖^0.5` 加权，急转弯不产生 cusp 与自交；采样间距 3.0 → 2.6，最小采样距离 0.4 → 0.35；`m_resampled` 增量缓存 + `rebuildTail()` 只重建末段，落笔跟手（BUG-124） |
| 亚像素鼠标跟随 | `src/canvas/CanvasView.{h,cpp}` | 新增 `CanvasView::mapToSceneF()`（`viewportTransform().inverted()` 保留小数），`ToolController` 全部坐标换算改走该接口，消除整像素截断造成的台阶顿挫 |
| 无极取色器 | `src/ui/widgets/MyColorPicker.{h,cpp}` | 自绘连续色相条（拖动取色）+ 饱和 / 明度滑块 + 9 枚渐变预设芯片（`ColorChip` 自绘）+ Hex 输入 + R / G / B 数字输入，全部自绘控件，未引入原生输入框 |
| 精简工具栏 | `src/ui/widgets/CanvasToolBar.cpp` | 删除复制 / 粘贴 / 剪切 / 删除 / 撤销 / 重做 / 全选专用按钮；「更多」浮层保留显隐 / 锁定 / 导出 PDF / 清空批注，并附快捷键说明文字 |
| 删除双键绑定 | `src/canvas/ToolController.cpp` | `onKeyPress()` 中 `Backspace` 与 `Delete` 均触发 `deleteSelected()`，删除选区内全部批注元素并注册 undo 命令 |
| 框选拖动 | `src/canvas/ToolController.{h,cpp}` | 新增 `pointInsideSelection()`：在选框内部空白处按下即进入整体拖动，不必精确点中元素本体 |
| 选框缩放手柄 | `src/canvas/SelectionOverlay.{h,cpp}` + `AnnotationItem.{h,cpp}` | 8 个方向手柄（4 角 + 4 边中点），容差 `tol = 9.0 / max(0.2, zoom)`；新增 `AnnotationFrame`（origin + basisX + basisY）抽象，缩放以**起始帧为绝对基准**，杜绝每帧累乘漂移；等比缩放走 `setScale(uniform)` 保持 `QGraphicsItem::scale()` 语义（BUG-125） |
| 透明行内编辑 | `src/canvas/TextBoxItem.{h,cpp}` + `MarkdownItem.{h,cpp}` | 移除不透明底色绘制，只保留虚线框；游离 `AnnotationScene` 无原生焦点，键盘改由 `ToolController::eventFilter` 拦截视口 `KeyPress` 后用 `QTextCursor` 直接改文档；编辑态自绘光标（`QTextLayout::lineForTextPosition` + `cursorToX`）；`createItemForTool()` 修正为先切工具再 `beginInlineEdit()`（BUG-123） |
| LaTeX / 笔记可移动 | `src/canvas/LatexItem.cpp` + `ToolController` | `LatexItem::applyStyle()` 接受 `Qt::LeftButton`，公式卡片进入选择态后可拖动；笔记与文本框同理走统一 `setPos()` 拖动管线 |
| 表格上下文工具栏 | `src/ui/widgets/TableToolBar.{h,cpp}` | 新增自绘 `TableCard` 浮层：居中 / 左中 / 右中 / 顶端对齐、选中单元格 / 行 / 列、上 / 下 / 左 / 右插入行列、删除当前行 / 列、「插入 k 行 / k 列」用 `MyTextInput` 输入数量；`MainWindow::positionTableToolBar()` 浮动定位在表格上方并随表格移动重定位；右键表格弹 `tableContextRequested` → `showContextMenu()`（BUG-126） |
| 新增测试 | `tests/SmartCanvasTest.cpp` | 40 项断言，覆盖 10 项需求：工具样式独立性、样条平滑、取色器存在、无冗余按钮、Backspace/Delete 删除与撤销、拖动移动、透明与行内输入、LaTeX/笔记移动、缩放手柄、表格对齐与增删行列、两段式点击 |

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
| 希尔排序 | ✅ | ShellSortModule | Cursor | 同余分组弧线 + 组内插入排序，gap 从 n/2 折半到 1 |
| 快速排序 | 🔄 | QuickSortModule | Kiro | 已使用 playPointersSync |
| 归并排序 | ✅ | MergeSortModule | Kiro | 辅助数组双视图已实现 |
| 计数排序 | ⏳ | CountingSortModule | - | 需频次数组 + 前缀和 |
| 桶排序 | ✅ | BucketSortModule | Cursor | 5 桶可视化 + 桶内插入排序 + 按桶序回填 out[] |
| 基数排序 | ✅ | RadixSortModule | Cursor | LSD 三趟 × 10 数字桶，桶内顺序收集 |

---

## 二、线性数据结构 (8 模块)
| 模块名 | 状态 | 文件名 | 负责人 | 备注 |
|--------|------|--------|--------|------|
| 前缀和 | ✅ | PrefixSumModule | Gemini | 已完成 |
| 差分 | ✅ | DifferenceModule | Gemini | 已完成 |
| 栈模拟 | ✅ | StackModule | Gemini | 已完成 |
| 队列模拟 | ✅ | QueueModule | Gemini | 已完成 |
| 循环队列 | ✅ | CircularQueueModule | Cursor | 取模折返可视化 + head/tail 指针自下方外置（归类：基础数据结构） |
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
| 子树大小统计 | ✅ | TreeSubtreeSizeModule | Cursor | 后序遍历自底向上，sz 气泡沿弧线汇入父节点相加 |
| 树的重心 | ⏳ | TreeCentroidModule | - | 切断连通块计算 |
| 树的直径 | ✅ | TreeDiameterModule | Cursor | 两轮无向 DFS + 波纹扩散，A→B 路径逐段点亮成粗红线 |
| LCA 暴力向上爬 | ⏳ | LCANaiveModule | - | 双指针碰撞 |
| LCA 倍增法 | ⏳ | LCADoublingModule | - | 2^k 跳跃 |

---

## 六、进阶数据结构 (10 模块)
| 模块名 | 状态 | 文件名 | 负责人 | 备注 |
|--------|------|--------|--------|------|
| 单调栈 | ✅ | MonotonicStackModule | Kiro | 下一个更大元素已实现 |
| 单调队列 | ✅ | MonotonicQueueModule | Cursor | 滑动窗口最大值，q[] 存下标单调递减 |
| 并查集 | ✅ | DSUPathCompressionModule | Cursor | **第 32 批统一为单一模块**（删除 `DSUBasicModule`）：严格对齐课堂 `dsu` 模板 —— `pa[i]=i` / `sz[i]=1` 起步，`find` 带路径压缩，`merge` 按大小合并 `pa[v]=u; sz[u]+=sz[v]`；右侧 `pa[]` / `sz[]` 两行实时同步，路径压缩用 `GraphEdge::repointDest()` 现场改接父边 |
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
| 链式前向星存图 | ✅ | ForwardStarModule | Cursor | head/to/nxt/weight 四列静态数组，to 亮青实线连图、nxt 紫虚线连 head，末尾逐跳遍历 |
| 拓扑排序 | ✅ | TopologicalSortModule | Codex | 入度队列、输出序列与有向环提示；有向/无向可切换 |
| 朴素 Dijkstra | ✅ | DijkstraNaiveModule | Codex | O(N²) 选点、非负权松弛与前驱数组；有向/无向可切换 |
| 堆优化 Dijkstra | ✅ | DijkstraHeapModule | Cursor | O(M log N)，隐去 vis 的懒删除模板，优先队列数组行 + 完全二叉树双视图 |
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
| 字符串哈希 | ✅ | StringHashModule | Cursor | 对标课堂模板：p=131 / mod=998244353，预计算 p[]/h1[]/h2[]，前后缀重叠匹配 |

---

## 十、动态规划 (9 模块)
| 模块名 | 状态 | 文件名 | 负责人 | 备注 |
|--------|------|--------|--------|------|
| 01 背包 | ✅ | Knapsack01Module | Codex | 二维 DP、选/不选候选状态飞入；教科书式表格排版（首行列下标 + 左侧行下标） |
| 完全背包 | ✅ | KnapsackCompleteModule | Cursor | 同行左侧转移，重复取物双色候选 |
| 最长递增子序列 | ✅ | LISModule | Codex | O(N²) DP 与 O(N log N) tails 对照 |
| 最长公共子序列 | ✅ | LCSModule | Cursor | 二维网格 dp[i][j]，上方横排 a/b 字符串数组 + 上下对指指针，标签与下标各占独立行，候选来源分色 + 对角线回溯 |
| 区间 DP 石子合并 | ⏳ | IntervalDPModule | - | 三角矩阵逐层 |
| 树形 DP 没有上司 | ⏳ | TreeDPModule | - | dp[u][0/1] |
| 状压 DP 蒙德里安 | ⏳ | StateBitmaskDPModule | - | 二进制兼容判定 |
| 数位 DP | ⏳ | DigitDPModule | - | 记忆化搜索 |
| 记忆化搜索对比 | ⏳ | MemoizationVsDPModule | - | 递归树剪枝 |

---

## 十一、数论与基础数学 (5 模块)
| 模块名 | 状态 | 文件名 | 负责人 | 备注 |
|--------|------|--------|--------|------|
| 埃氏筛 | ✅ | SieveOfEratosthenesModule | Cursor | 只输入 n，右侧 10 列矩阵滚动定位，从 i*i 起划除并打叉下沉 |
| 线性筛/欧拉筛 | ⏳ | LinearSieveModule | - | 最小质因子精准击杀 |
| 辗转相除 GCD | ⏳ | GCDModule | - | 几何矩形缩短 |
| 快速幂 | ✅ | FastPowModule | Cursor | qpow + qmul 双层模板，竖式乘法面板逐帧展开 |
| 扩展欧几里得 | ✅ | ExgcdModule | Cursor | 三阶段推导链：同余式变形 → 辗转相除双栏对照 → 递归回代逐层展开（公式飞入并展开），终帧给出模逆元 |

---

## 十二、贪心算法 (2 模块)
| 模块名 | 状态 | 文件名 | 负责人 | 备注 |
|--------|------|--------|--------|------|
| 区间调度 | ⏳ | IntervalSchedulingModule | - | 扫描线淘汰 |
| 哈夫曼树 | ✅ | HuffmanTreeModule | Cursor | 频率数组初始即显示原始权值（n 格），排序阶段数值逐个飞出再流星般落回；树自上而下生长、根在最上、每行居中对称、无交叉线；树边为**无向**线段（无箭头），边中点标左 0 右 1；右侧 n×3 编码表（a[i]/编码/码长）；底部 WPL 逐项累加 |

---

## 十三、哈希与基础匹配 (3 模块)
| 模块名 | 状态 | 文件名 | 负责人 | 备注 |
|--------|------|--------|--------|------|
| 哈希表拉链法 | ⏳ | HashTableChainingModule | - | 链表头插 |
| 哈希表开放寻址 | ⏳ | HashTableOpenAddressingModule | - | 探测空槽 |
| 字符串哈希 | ✅ | StringHashModule | Cursor | 与 KMP 对比 |

---

## 开发优先级建议

### P0 (已完成 ✅)
1. ✅ **选择排序** - 最简单的排序，已验证数组模块基类，已重构使用动画引擎
2. ✅ **插入排序** - 为希尔排序打基础，已重构使用 playExtractAnim 等新特性
3. ✅ **堆基础映射** - 为后续堆模块打地基

### P1 (已完成 ✅)
1. ✅ **快速排序** - 高频算法，递归可视化技术已突破，已迁移 playPointersSync
2. ✅ **归并排序** - **第 32 批重构**：真实「裂开—拼接」动画，辅助数组技术已实现
3. ✅ **单调栈** - 中等难度，已完成实现
4. ✅ **并查集** - **第 32 批统一为单一模块**，路径压缩前置能力已内置

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
- **2026-11-20 大清理**：`build/tests/` 已整体删除（纯编译产物，可由 `cmake -S tests -B build/tests` 完整重建），`build/Desktop_Qt_6_11_1_MinGW_64_bit-Release/` 影子构建目录、根目录 197 张调试截图、`shots/` 目录、`tools/` 下 169 个一次性脚本与中间产物、`tools/_recovered/` 恢复副本均已清除；项目总占用由 **1149.9 MB 降至 78.0 MB**。保留 `tools/latex/` 与 `node_modules/`（LaTeX 公式渲染服务运行时依赖）。清理后重新构建 `SmartGround.exe` 成功，`SmartGroundStrictAudit` 套件重建运行三项计数全为 0。

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

### 下一批开发方向（2026-11-20 排定）

PM 已确认策略转向：**先做深现有模块，不开新篇**。批 30 四个模块（`TrieModule` / `IntervalSchedModule` / `EulerModule` / `BipartiteModule`）深化完成并验收通过。

**批 31 —— 十个现有模块的动画深化**，完整分镜（旧方案 vs 新方案对照）见 `docs/Batch31_Storyboards.md`：

| 序 | 模块 | 旧方案核心缺口 | 新方案关键新增 |
|---|---|---|---|
| 1 | `QuickSortModule` | 无可视递归栈；pivot 是脱离数组的浮空格子 | 递归层带 + 基准值徽标 + 锁定语义 |
| 2 | `MergeSortModule` | 与快排**共用同一套帧类型**，两算法视觉上无法区分 | 独立的 `MS_*` 帧族：两段汇入单一输出 |
| 3 | `HeapAdjustModule` | 上下标与 `2k/2k+1` 公式只存在于脚本文本；光束与值迁移路径不重合 | 结点内写下标 + 常驻公式条 + 值沿树边真正迁移 |
| 4 | `Knapsack01Module` | 两个候选被拆成两帧先后覆盖，看不到并排比较 | 两条候选路径并排 + 胜者飞行落位 + 转移来源注记 |
| 5 | `KMPNextModule` + `KMPMatchModule` | `pi[]` 只是数字表，前后缀含义完全不可见 | 跨格紫色括线 + 长度编码匹配条 |
| 6 | `TopologicalSortModule` | 边被"删除"无删除标记；入度与图结点无对齐 | 删边打 ✕ + 徽标 `-1` + 入度表与结点同序对齐 |
| 7 | `DijkstraNaiveModule` | **「为什么选它」完全不可见**（核心教学点缺失） | 候选最小值尺 + 松弛新旧值对比 |
| 8 | `KruskalModule` | `find` 的逐跳路径压缩被跳过；成环边无反证 | 逐跳 find + 路径压缩改写飞线 + 红色成环 |
| 9 | `DSUPathCompressionModule` | 无压缩前后对照；旧链无残影；`sz[]` 是摆设 | 树高标尺 + 旧链虚线残影 + 按秩合并可视化 |
| 10 | `SieveOfEratosthenesModule` | 「从 `i*i` 开始」的剪枝理由只在脚本文本 | 紫色区间标记 + 扫描指针轨 + 质数逐个归档 |

实施分批：**31-A**（快排/归并/堆）→ **31-B**（背包/埃氏筛/拓扑）→ **31-C**（KMP/Dijkstra/Kruskal/并查集）→ **31-D**（线段树合并版）。

后续备选（批 32+）：线段树单点修改与区间查询合并版、Bellman-Ford/SPFA 负权松弛、Floyd 三重循环矩阵松弛、区间 DP 石子合并、树上 DP（没有上司的舞会）、状压 DP、线性筛/欧拉筛、堆综合操作与 `ph/hp` 映射、双向链表静态数组模拟、笛卡尔坐标系底板（GeoGebra 风格）。
