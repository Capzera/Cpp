# SmartGround 架构与动画引擎

## 运行机制

### 录制

用户点击「绘制可视化效果」后，MainWindow 先重建布局，随即自动调用当前模块的 runAlgorithm 完成后台录制（不再需要独立的「动画帧计算」按钮）。模块执行算法并生成 AnimFrame 序列，帧数据包含动作类型、索引或节点 ID、附加值、批量目标、说明文字和公式文字。

### 播放

单步或定时器播放取出下一帧并调用 renderFrame。算法状态变化由帧记录表达，渲染模块根据动作类型更新图元并在允许时播放 Qt 动画。

### 撤销

“上一步”将帧索引回退一格，以录制时的输入重建布局，再关闭 m_animated 并重放目标位置前的所有帧。场景重建和帧回放期间不创建或启动属性动画。

### 完整重播

播放到末尾后再次自动播放时，MainWindow 使用录制输入重建布局并从帧 0 开始播放。新建布局、切换算法或修改输入后，旧帧序列会失效，避免不同输入的数据混播。

## 实时可编辑数据

老师可以在播放过程中直接双击画布上的数组格、结点或边权标签修改数值，修改是真实生效的，不是装饰。

- src/render/EditableModel.h：按 `scope|row#index` 存放实时覆盖值，`scope` 取模块名，不同模块互不干扰。
- IModule 提供槽位登记接口 `registerArraySlot(block, row, index)` / `registerNodeSlot(id, row, index)` / `registerEdgeSlot(key, row, index)`，把画布图元映射到具体的数据行与下标；`liveValue(row, index, computed)` 在算法计算时优先读取覆盖值，缺失时回落到算法算出的值。
- `MainWindow::rebuildCurrentLayout()` 在建图后自动调用 `enableLiveEditing()`，为已登记的图元开启就地编辑并接上编辑事件。
- 编辑发生时 `applyLiveEditsAndReplay()` 接管：以非动画模式重建场景 → 重新执行 `runAlgorithm()` → 快进到原帧位置。于是后续所有动画帧都按老师改后的数值继续计算，下游状态（如前缀和 `S[i+1]`）会跟着重新推导。
- 重新点击「绘制可视化效果」会清空当前模块的全部覆盖值，回到老师原始输入。

## 核心类型

AnimFrame 位于 src/engine/AnimFrame.h，统一使用 ActionType 描述图论、数组、前缀和/差分、栈队列、二分查找和堆操作。

IModule 位于 src/modules/IModule.h，定义模块名称、默认输入、构建布局、录制帧、渲染帧和重置接口。基类保存唯一的 protected QGraphicsScene 指针。派生模块不应再次声明 m_scene。

IModule 同时预置了一批面向未来的空实现钩子，供新模块选择性覆盖：`getCategoryName()`、`getComplexityNote()`、`getTeachingTip()`、`getStepCost()`、`validateInput()`、`onCanvasDoubleClick()`、`onCanvasBackgroundClick()`、`applyTheme()`。开发阶段允许保留这些骨架，项目完工后再统一瘦身。

## 公共动画辅助函数

IModule 提供 playSwapAnim、playShiftAnim、playExtractAnim、playPointerMove、playScaleAnim、playPointersSync 和 playValueFlight。playValueFlight 保留源图元状态，将数值沿弧线飞入目标数组格，可用于图节点入队和 DP 候选状态。

IModule 另提供绘制类辅助：drawLabel、drawCaption（统一字体与配色）、applyNodeHighlight、clearNodeHighlights（统一高亮复位）、attachEdge、detachAllEdges（统一边注册与解绑，防止画布撕裂）。

新模块优先调用已有辅助函数；发现新的重复视觉效果时先抽取公共接口，再由模块调用。

m_animated=false 时必须直接设置最终图元状态，不得创建或启动 QPropertyAnimation。布局初始化也应尊重此状态，确保撤销与重播快速完成。

## 图元

- GraphNode、GraphEdge：图论节点和有向/无向边
- ArrayBlock：数值格和可选语义标签
- PointerBlock：带标签的移动指针
- StackBlock：Tarjan 栈元素
- FloatingText：用于数值移动过程的临时文字

图元坐标、绘制边界和指针中心偏移以 src/render/GraphElements.cpp 为准。

绘制边界与实际绘制范围必须严格一致：所有 `boundingRect()` 都用 `QFontMetrics::horizontalAdvance()` 动态计算文本宽度，宽度公式与 `paint()` 中的绘制公式共用同一个常量（例如 `PointerBlock` 统一使用 `max(40, 文本宽+8)`）。一旦绘制超出包围盒，Qt 的脏区失效就会漏掉那些像素，指针移动后会留下永久残影。文本内容变化时必须调用 `prepareGeometryChange()`。

`ArrayBlock` / `GraphNode` / `GraphEdge` 均支持双击就地编辑，编辑期间本体的文字绘制被抑制（`m_editor != nullptr` 守卫），由子 `QGraphicsTextItem` 独占显示，避免新旧数字重叠。回车提交、Esc 取消、焦点丢失自动提交，由 `EditorWatcher` 事件过滤器统一处理。

## UI 层（自研控件，零第三方依赖）

- src/ui/UiTheme.h：全项目设计令牌。所有颜色、圆角、间距、字体必须取自此处，禁止在业务代码写死色值。
- src/ui/widgets/：自绘控件库。
  - MyButton：Solid / Soft / Ghost 三种变体，含悬停、按下、聚焦态。
  - MyToggleSwitch：iOS 风格滑动开关，灰色 OFF、绿色 ON，滑块位移带缓动。
  - MyRadioButton、MyCheckBox：自绘指示器，CheckBox 支持 `ratio` 属性驱动透明度与可用态。
  - MyComboBox：自绘下拉浮层。列表项为独立 `ComboRow` 控件，浮层是真正的 `Qt::Popup` 顶层窗口（自绘 `ComboCard` 白底圆角卡片 + 投影），内嵌 `QScrollArea` 使长列表可滚动（视口上限 268px，自定义细滚动条）；选中行用 `accentSoft` 高亮；点击外部或 Esc 自动关闭。
  - MyTextEdit：圆角边框、聚焦高亮、细滚动条、OI 格式占位提示。
  - MyCollapsible：可折叠面板，用 maximumHeight 动画做流线展开/收起。
  - MyColorPicker：自绘无极取色器。顶部 `ColorPreview` 为纯色预览卡；中部 `ColorField` 上半是饱和度 × 明度二维取色区（白→纯色横向渐变叠加黑色纵向渐变），下半是 0–359° 连续色相条，两者均支持按下即定位与拖动实时取色；下方是色相 / 饱和 / 明度滑块、Hex 输入与 R / G / B 数字输入，全部通过 `syncInputs()` 双向同步（`m_syncing` 标志防信号回环）。
- 规范：不得再向用户暴露原生 QPushButton / QRadioButton / QCheckBox / QComboBox / QTextEdit。

## 画布与网格

- src/core/ViewportFitter.h：统一画布居中与自适应缩放。`fitToItems()` 执行 `fitInView(KeepAspectRatio)`，把缩放钳制在 0.25 - 2.4，再 `centerOn` 内容中心。禁止在 MainWindow 手写变换矩阵。
- src/render/SceneGrid.h：隐形对齐网格。`magneticAlign()` 在 magnetRange 半径内把节点吸附到邻近节点的横纵坐标轴；`snapToCell()` 按 cell 尺寸吸附到隐形晶格；按住 Ctrl 临时关闭吸附进行自由摆放。
- 画布左下角提供「启用对齐网格」开关，开启时显示淡色点线参考网格。

## 关键文件

- src/ui/MainWindow.cpp：界面和帧播放状态管理
- src/ui/UiTheme.h：设计令牌
- src/ui/widgets/：自研控件库
- src/core/ViewportFitter.h：视口居中与缩放
- src/render/SceneGrid.h：对齐网格
- src/modules/IModule.cpp：共享动画实现
- src/modules/TarjanModule.cpp：图论示例模块
- CMakeLists.txt：唯一正式应用目标与源文件清单

已知问题与修复记录见 docs/BugLog.md。
