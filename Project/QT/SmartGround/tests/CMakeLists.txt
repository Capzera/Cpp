cmake_minimum_required(VERSION 3.16)
project(SmartGroundTests LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
find_package(Qt6 COMPONENTS Widgets Gui Core Svg REQUIRED)

set(SG_MODULE_SOURCES
    ../src/render/GraphElements.cpp
    ../src/render/SceneGrid.cpp
    ../src/render/MatrixBlock.cpp
    ../src/render/LatexBlock.cpp
    ../src/core/ViewportFitter.cpp
    ../src/modules/IModule.cpp
    ../src/modules/TarjanModule.cpp
    ../src/modules/BubbleSortModule.cpp
    ../src/modules/SelectionSortModule.cpp
    ../src/modules/InsertionSortModule.cpp
    ../src/modules/QuickSortModule.cpp
    ../src/modules/MergeSortModule.cpp
    ../src/modules/HeapBasicsModule.cpp
    ../src/modules/MonotonicStackModule.cpp
    ../src/modules/DSUBasicModule.cpp
    ../src/modules/TwoRowArrayModule.cpp
    ../src/modules/PrefixSumModule.cpp
    ../src/modules/DifferenceModule.cpp
    ../src/modules/StackModule.cpp
    ../src/modules/QueueModule.cpp
    ../src/modules/BinarySearchClosedModule.cpp
    ../src/modules/BinarySearchHalfOpenModule.cpp
    ../src/modules/TopologicalSortModule.cpp
    ../src/modules/DijkstraNaiveModule.cpp
    ../src/modules/KruskalModule.cpp
    ../src/modules/BFSModule.cpp
    ../src/modules/Knapsack01Module.cpp
    ../src/modules/LISModule.cpp
    ../src/modules/KMPNextModule.cpp
    ../src/modules/SegmentTreeBuildModule.cpp
    ../src/modules/SegmentTreeEditBase.cpp
    ../src/modules/SegmentTreeTagModule.cpp
    ../src/modules/SegmentTreeQueryModule.cpp
    ../src/modules/KMPMatchModule.cpp
    ../src/modules/FenwickTreeModule.cpp
    ../src/modules/KnapsackCompleteModule.cpp
    ../src/modules/TarjanCutVertexBridgeModule.cpp
    ../src/modules/HungarianModule.cpp
    ../src/modules/HeapAdjustModule.cpp
    ../src/modules/HeapSortModule.cpp
    ../src/modules/FreeStructureBase.cpp
    ../src/modules/SandboxArrayModule.cpp
    ../src/modules/SandboxMatrixModule.cpp
    ../src/modules/SandboxTreeModule.cpp
    ../src/modules/SandboxGraphModule.cpp
    ../src/modules/SandboxBlankModule.cpp
)

set(SG_MODULE_HEADERS
    ../src/engine/AnimFrame.h
    ../src/render/GraphElements.h
    ../src/render/SceneGrid.h
    ../src/render/MatrixBlock.h
    ../src/render/LatexBlock.h
    ../src/core/ViewportFitter.h
    ../src/ui/UiTheme.h
    ../src/modules/IModule.h
    ../src/modules/GraphModuleUtils.h
    ../src/modules/GraphRenderHelper.h
    ../src/modules/TopologicalSortModule.h
    ../src/modules/DijkstraNaiveModule.h
    ../src/modules/KruskalModule.h
    ../src/modules/BFSModule.h
    ../src/modules/Knapsack01Module.h
    ../src/modules/LISModule.h
    ../src/modules/KMPNextModule.h
    ../src/modules/SegmentTreeBuildModule.h
    ../src/modules/SegmentTreeCommon.h
    ../src/modules/SegmentTreeEditBase.h
    ../src/modules/SegmentTreeTagModule.h
    ../src/modules/SegmentTreeQueryModule.h
    ../src/modules/KMPMatchModule.h
    ../src/modules/FenwickTreeModule.h
    ../src/modules/KnapsackCompleteModule.h
    ../src/modules/TarjanCutVertexBridgeModule.h
    ../src/modules/HungarianModule.h
    ../src/modules/HeapAdjustModule.h
    ../src/modules/HeapSortModule.h
    ../src/modules/HeapTreeBase.h
    ../src/modules/TwoRowArrayModule.h
    ../src/modules/FreeStructureBase.h
    ../src/modules/SandboxArrayModule.h
    ../src/modules/SandboxMatrixModule.h
    ../src/modules/SandboxTreeModule.h
    ../src/modules/SandboxGraphModule.h
    ../src/modules/SandboxBlankModule.h
    ../src/modules/TarjanModule.h
    ../src/modules/BubbleSortModule.h
    ../src/modules/SelectionSortModule.h
    ../src/modules/InsertionSortModule.h
    ../src/modules/QuickSortModule.h
    ../src/modules/MergeSortModule.h
    ../src/modules/HeapBasicsModule.h
    ../src/modules/MonotonicStackModule.h
    ../src/modules/DSUBasicModule.h
    ../src/modules/PrefixSumModule.h
    ../src/modules/DifferenceModule.h
    ../src/modules/StackModule.h
    ../src/modules/QueueModule.h
    ../src/modules/BinarySearchClosedModule.h
    ../src/modules/BinarySearchHalfOpenModule.h
)

set(SG_WIDGET_SOURCES
    ../src/ui/widgets/MyButton.cpp
    ../src/ui/widgets/MyToggleSwitch.cpp
    ../src/ui/widgets/MyRadioButton.cpp
    ../src/ui/widgets/MyCheckBox.cpp
    ../src/ui/widgets/MyComboBox.cpp
    ../src/ui/widgets/MyTextEdit.cpp
    ../src/ui/widgets/MyCollapsible.cpp
    ../src/ui/widgets/MySlider.cpp
    ../src/ui/widgets/MyColorSwatch.cpp
    ../src/ui/widgets/FlowLayout.cpp
    ../src/ui/widgets/AnnotationEditor.cpp
    ../src/canvas/LatexService.cpp
)

set(SG_CANVAS_SOURCES
    ../src/canvas/AnnotationItem.cpp
    ../src/canvas/StrokeItem.cpp
    ../src/canvas/ShapeItem.cpp
    ../src/canvas/TextBoxItem.cpp
    ../src/canvas/MarkdownItem.cpp
    ../src/canvas/LatexItem.cpp
    ../src/canvas/CanvasTableItem.cpp
    ../src/canvas/AnnotationScene.cpp
    ../src/canvas/ToolController.cpp
    ../src/canvas/CanvasView.cpp
    ../src/ui/widgets/CanvasToolBar.cpp
    ../src/ui/widgets/ToolIconButton.cpp
)

add_library(SgCore STATIC ${SG_MODULE_SOURCES} ${SG_MODULE_HEADERS})
target_link_libraries(SgCore PUBLIC Qt6::Widgets Qt6::Gui Qt6::Core Qt6::Svg)
target_include_directories(SgCore PUBLIC ../src)

add_library(SgWidgets STATIC ${SG_WIDGET_SOURCES})
target_link_libraries(SgWidgets PUBLIC SgCore Qt6::Widgets Qt6::Gui Qt6::Core Qt6::Svg)
target_include_directories(SgWidgets PUBLIC ../src)

add_library(SgCanvas STATIC ${SG_CANVAS_SOURCES})
target_link_libraries(SgCanvas PUBLIC SgWidgets SgCore Qt6::Widgets Qt6::Gui Qt6::Core Qt6::Svg)
target_include_directories(SgCanvas PUBLIC ../src)

add_library(SgUi STATIC ../src/ui/MainWindow.cpp ../src/ui/MainWindow.h)
target_link_libraries(SgUi PUBLIC SgWidgets SgCanvas SgCore Qt6::Widgets Qt6::Gui Qt6::Core Qt6::Svg)
target_include_directories(SgUi PUBLIC ../src)

function(sg_link name lib)
    add_executable(${name} ${ARGN} ../src/resources.qrc)
    target_link_libraries(${name} PRIVATE ${lib} Qt6::Widgets Qt6::Gui Qt6::Core Qt6::Svg)
    target_include_directories(${name} PRIVATE ../src)
endfunction()

function(add_sg_test name source)
    sg_link(${name} SgCore ${source})
endfunction()

function(add_sg_canvas_test name source)
    sg_link(${name} SgCanvas ${source})
endfunction()

function(add_sg_ui_test name source)
    sg_link(${name} SgUi ${source})
endfunction()

function(add_sg_widget_test name source)
    sg_link(${name} SgWidgets ${source})
endfunction()

add_sg_test(SmartGroundStress StressTest.cpp)
add_sg_test(SmartGroundAnimStress AnimStressTest.cpp)
add_sg_test(SmartGroundWeightTest WeightFollowTest.cpp)
add_sg_test(SmartGroundEdgeRenderTest EdgeRenderTest.cpp)
add_sg_test(SmartGroundArrowPixelTest ArrowPixelTest.cpp)
add_sg_test(SmartGroundArrowShot ArrowShot.cpp)
add_sg_ui_test(SmartGroundUiBehaviorTest UiBehaviorTest.cpp)
add_sg_test(SmartGroundDsuShot DsuShot.cpp)
add_sg_ui_test(SmartGroundUiShot UiShot.cpp)
add_sg_ui_test(SmartGroundWidgetPixelTest WidgetPixelTest.cpp)
add_sg_ui_test(SmartGroundStartupPanelTest StartupPanelTest.cpp)
add_sg_widget_test(SmartGroundComboPopupTest ComboPopupTest.cpp)
add_sg_widget_test(SmartGroundComboShot ComboShot.cpp)
add_sg_widget_test(SmartGroundUiPolish UiPolishTest.cpp)
add_sg_widget_test(SmartGroundUiPolish2 UiPolish2Test.cpp)
add_sg_ui_test(SmartGroundModuleReset ModuleResetTest.cpp)
add_sg_widget_test(SmartGroundToggleShot ToggleShot.cpp)
add_sg_ui_test(SmartGroundAutoPlayEnd AutoPlayEndTest.cpp)
add_sg_test(SmartGroundFinalShot FinalShot.cpp)
add_sg_test(SmartGroundNewModule NewModuleTest.cpp)
add_sg_test(SmartGroundSandboxVisual SandboxVisualTest.cpp)
add_sg_ui_test(SmartGroundFrameSlider FrameSliderTest.cpp)
add_sg_widget_test(SmartGroundTextEditRegression TextEditRegressionTest.cpp)
add_sg_ui_test(SmartGroundTextEditUiShot TextEditUiShot.cpp)
add_sg_test(SmartGroundLiveEdit LiveEditTest.cpp)
add_sg_test(SmartGroundEditCrash EditCrashTest.cpp)
add_sg_test(SmartGroundBorderDiag BorderDiagTest.cpp)
add_sg_test(SmartGroundGroupA GroupATest.cpp)
add_sg_ui_test(SmartGroundGroupAUi GroupAUiTest.cpp)
add_sg_test(SmartGroundNodeLiveEdit NodeLiveEditTest.cpp)
add_sg_test(SmartGroundOverlayRenderProbe OverlayRenderProbe.cpp)
add_sg_canvas_test(SmartGroundCanvasTools CanvasToolsTest.cpp)
add_sg_canvas_test(SmartGroundCanvasVisual CanvasVisualTest.cpp)
add_sg_canvas_test(SmartGroundToolSuite ToolSuiteTest.cpp)
add_sg_canvas_test(SmartGroundBrushQuality BrushQualityTest.cpp)
add_sg_canvas_test(SmartGroundCanvasPolish CanvasPolishTest.cpp)
add_sg_canvas_test(SmartGroundCanvasEditing CanvasEditingTest.cpp)

add_sg_ui_test(SmartGroundAppWidth AppWidthTest.cpp)
add_sg_ui_test(SmartGroundAppCombo AppComboTest.cpp)
add_sg_ui_test(SmartGroundAppBrush AppBrushTest.cpp)









