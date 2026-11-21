cmake_minimum_required(VERSION 3.16)
project(SmartGroundTests LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
find_package(Qt6 COMPONENTS Widgets Gui Core Svg REQUIRED)

set(SG_GENERATED_DIR ${CMAKE_CURRENT_BINARY_DIR}/generated)
file(MAKE_DIRECTORY ${SG_GENERATED_DIR})
set(SG_BUILD_STAMP "0")
find_package(Git QUIET)
if(GIT_FOUND)
    execute_process(COMMAND ${GIT_EXECUTABLE} rev-list --count HEAD -- .
                    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/..
                    OUTPUT_VARIABLE SG_GIT_COUNT
                    OUTPUT_STRIP_TRAILING_WHITESPACE
                    ERROR_QUIET)
    if(SG_GIT_COUNT)
        set(SG_BUILD_STAMP "${SG_GIT_COUNT}")
    endif()
endif()
file(WRITE ${SG_GENERATED_DIR}/AppVersionGenerated.h
"#ifndef APPVERSIONGENERATED_H\n#define APPVERSIONGENERATED_H\nnamespace AppVersionBuild {\ninline const char* kStamp = \"${SG_BUILD_STAMP}\";\n}\n#endif\n")

set(SG_MODULE_SOURCES
    ../src/render/GraphElements.cpp
    ../src/render/SceneGrid.cpp
    ../src/render/MatrixBlock.cpp
    ../src/modules/ModuleFields.cpp
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
    ../src/modules/ShellSortModule.cpp
    ../src/modules/FastPowModule.cpp
    ../src/modules/SieveOfEratosthenesModule.cpp
    ../src/modules/EulerSieveModule.cpp
    ../src/modules/ForwardStarModule.cpp
    ../src/modules/DSUPathCompressionModule.cpp
    ../src/modules/TreeSubtreeSizeModule.cpp
    ../src/modules/TreeDiameterModule.cpp
    ../src/modules/DijkstraHeapModule.cpp
    ../src/modules/GcdModule.cpp
    ../src/modules/DfsModule.cpp
    ../src/modules/PrimModule.cpp
    ../src/modules/CountingSortModule.cpp
    ../src/modules/BipartiteModule.cpp
    ../src/modules/EulerModule.cpp
    ../src/modules/IntervalSchedModule.cpp
    ../src/modules/TrieModule.cpp
    ../src/modules/HashTableModule.cpp
    ../src/modules/BucketSortModule.cpp
    ../src/modules/RadixSortModule.cpp
    ../src/modules/CircularQueueModule.cpp
    ../src/modules/MonotonicQueueModule.cpp
    ../src/modules/ExgcdModule.cpp
    ../src/modules/HuffmanTreeModule.cpp
    ../src/modules/StringHashModule.cpp
    ../src/modules/LCSModule.cpp
    ../src/render/ValueBar.cpp
    ../src/render/SegmentBar.cpp
    ../src/core/CartesianExpression.cpp
    ../src/render/CartesianElements.cpp
    ../src/modules/CartesianModule.cpp
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
    ../src/modules/ShellSortModule.h
    ../src/modules/FastPowModule.h
    ../src/modules/SieveOfEratosthenesModule.h
    ../src/modules/EulerSieveModule.h
    ../src/modules/ForwardStarModule.h
    ../src/modules/DSUPathCompressionModule.h
    ../src/modules/TreeDpBase.h
    ../src/modules/TreeSubtreeSizeModule.h
    ../src/modules/TreeDiameterModule.h
    ../src/modules/DijkstraHeapModule.h
    ../src/modules/GcdModule.h
    ../src/modules/DfsModule.h
    ../src/modules/PrimModule.h
    ../src/modules/CountingSortModule.h
    ../src/modules/BipartiteModule.h
    ../src/modules/EulerModule.h
    ../src/modules/IntervalSchedModule.h
    ../src/modules/TrieModule.h
    ../src/modules/HashTableModule.h
    ../src/modules/BucketSortModule.h
    ../src/modules/RadixSortModule.h
    ../src/modules/CircularQueueModule.h
    ../src/modules/MonotonicQueueModule.h
    ../src/modules/ExgcdModule.h
    ../src/modules/HuffmanTreeModule.h
    ../src/modules/StringHashModule.h
    ../src/modules/LCSModule.h
    ../src/render/ValueBar.h
    ../src/render/SegmentBar.h
    ../src/core/CartesianExpression.h
    ../src/render/CartesianElements.h
    ../src/modules/CartesianModule.h
    ../src/modules/TarjanModule.h
    ../src/modules/BubbleSortModule.h
    ../src/modules/SelectionSortModule.h
    ../src/modules/InsertionSortModule.h
    ../src/modules/QuickSortModule.h
    ../src/modules/MergeSortModule.h
    ../src/modules/HeapBasicsModule.h
    ../src/modules/MonotonicStackModule.h
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
    ../src/ui/widgets/MyColorPicker.cpp
    ../src/ui/widgets/MySegmentedControl.cpp
    ../src/ui/widgets/MyTextInput.cpp
    ../src/ui/widgets/CartesianPanel.cpp
    ../src/ui/widgets/CartesianInfoPanel.cpp
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
    ../src/canvas/InlineTextEdit.cpp
    ../src/canvas/SelectionOverlay.cpp
    ../src/canvas/ToolController.cpp
    ../src/canvas/CanvasView.cpp
    ../src/ui/widgets/CanvasToolBar.cpp
    ../src/ui/widgets/CanvasBrushSlider.cpp
    ../src/ui/widgets/TableToolBar.cpp
    ../src/ui/widgets/TableIconButton.cpp
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
target_include_directories(SgUi PUBLIC ../src ${SG_GENERATED_DIR})

function(sg_link name lib)
    add_executable(${name} ${ARGN} ../src/resources.qrc)
    target_link_libraries(${name} PRIVATE ${lib} Qt6::Widgets Qt6::Gui Qt6::Core Qt6::Svg)
    target_include_directories(${name} PRIVATE ../src ${SG_GENERATED_DIR})
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

add_sg_test(SmartGroundBatch8 Batch8ModulesTest.cpp)
add_sg_test(SmartGroundBatch8Visual Batch8VisualTest.cpp)
add_sg_test(SmartGroundLayoutFit LayoutFitTest.cpp)
add_sg_test(SmartGroundOverlapAudit OverlapAuditTest.cpp)


add_sg_test(SmartGroundStress StressTest.cpp)
add_sg_test(SmartGroundAnimStress AnimStressTest.cpp)
add_sg_test(SmartGroundWeightTest WeightFollowTest.cpp)
add_sg_test(SmartGroundEdgeRenderTest EdgeRenderTest.cpp)
add_sg_test(SmartGroundArrowPixelTest ArrowPixelTest.cpp)
add_sg_ui_test(SmartGroundUiBehaviorTest UiBehaviorTest.cpp)
add_sg_ui_test(SmartGroundWidgetPixelTest WidgetPixelTest.cpp)
add_sg_ui_test(SmartGroundStartupPanelTest StartupPanelTest.cpp)
add_sg_widget_test(SmartGroundComboPopupTest ComboPopupTest.cpp)
add_sg_widget_test(SmartGroundUiPolish UiPolishTest.cpp)
add_sg_widget_test(SmartGroundUiPolish2 UiPolish2Test.cpp)
add_sg_ui_test(SmartGroundModuleReset ModuleResetTest.cpp)
add_sg_ui_test(SmartGroundAutoPlayEnd AutoPlayEndTest.cpp)
add_sg_test(SmartGroundNewModule NewModuleTest.cpp)
add_sg_test(SmartGroundSandboxVisual SandboxVisualTest.cpp)
add_sg_ui_test(SmartGroundFrameSlider FrameSliderTest.cpp)
add_sg_widget_test(SmartGroundTextEditRegression TextEditRegressionTest.cpp)
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
add_sg_canvas_test(SmartGroundEraserPanel EraserPanelTest.cpp)
add_sg_canvas_test(SmartGroundLivePreview LivePreviewTest.cpp)
add_sg_canvas_test(SmartGroundSmartCanvas SmartCanvasTest.cpp)
add_sg_widget_test(SmartGroundColorPickerTest ColorPickerTest.cpp)

add_sg_ui_test(SmartGroundStylePicker StylePickerTest.cpp)
add_sg_ui_test(SmartGroundAppWidth AppWidthTest.cpp)
add_sg_ui_test(SmartGroundAppCombo AppComboTest.cpp)
add_sg_ui_test(SmartGroundAppBrush AppBrushTest.cpp)
add_sg_ui_test(SmartGroundModuleSettingsPanel ModuleSettingsPanelTest.cpp)
add_sg_test(SmartGroundGraphInlineEdit GraphInlineEditTest.cpp)
add_sg_canvas_test(SmartGroundCanvasCaret CanvasCaretTest.cpp)
add_sg_test(SmartGroundSieveStrike SieveStrikeTest.cpp)
add_sg_test(SmartGroundMatrixShape MatrixShapeTest.cpp)
add_sg_test(SmartGroundMatrixRebuild MatrixRebuildTest.cpp)
add_sg_test(SmartGroundMatrixGhost MatrixGhostTest.cpp)
add_sg_ui_test(SmartGroundMatrixGhostApp MatrixGhostAppTest.cpp)
add_sg_test(SmartGroundPointerVisual PointerVisualTest.cpp)
add_sg_test(SmartGroundPointerAlign PointerAlignTest.cpp)
add_sg_test(SmartGroundPointerDebug PointerDebugTest.cpp)
add_sg_ui_test(SmartGroundMstGroup MstGroupTest.cpp)
add_sg_ui_test(SmartGroundPopupEdgeVerify PopupEdgeVerify.cpp)
add_sg_test(SmartGroundGraphInteraction GraphInteractionTest.cpp)
add_sg_test(SmartGroundNewModuleBatch NewModuleBatchTest.cpp)
add_sg_test(SmartGroundRound10Fix Round10FixTest.cpp)
add_sg_test(SmartGroundRound10Visual Round10VisualTest.cpp)
add_sg_test(SmartGroundRound10RowDump Round10RowDump.cpp)
add_sg_ui_test(SmartGroundVersionBadge VersionBadgeTest.cpp)








add_sg_test(SmartGroundPointerMotion PointerMotionTest.cpp)
add_sg_test(SmartGroundBatch9Modules Batch9ModulesTest.cpp)
add_sg_test(SmartGroundBatch9Visual Batch9VisualTest.cpp)
add_sg_test(SmartGroundExgcdDebug ExgcdDebug.cpp)
add_sg_test(SmartGroundHuffmanShot HuffmanShot.cpp)
add_sg_test(SmartGroundHuffmanLayoutDump HuffmanLayoutDump.cpp)
add_sg_test(SmartGroundStrictAudit StrictAudit.cpp)
add_sg_test(SmartGroundAnimUpgrade AnimUpgradeTest.cpp)
add_sg_test(SmartGroundBeamCurve BeamCurveTest.cpp)
add_sg_test(SmartGroundBeamRender BeamRenderTest.cpp)
add_sg_canvas_test(SmartGroundCanvasFeatures CanvasFeaturesTest.cpp)

add_sg_test(SmartGroundBatch31 Batch31Test.cpp)
add_sg_test(SmartGroundBatch31Visual Batch31Visual.cpp)
add_sg_widget_test(SmartGroundVisualFix VisualFixTest.cpp)
add_sg_ui_test(SmartGroundCartesian CartesianModuleTest.cpp)
add_sg_ui_test(SmartGroundCartesianVisual CartesianVisualTest.cpp)
add_sg_ui_test(SmartGroundCartesianPixel CartesianPixelTest.cpp)
add_sg_ui_test(SmartGroundCartesianUi CartesianUiTest.cpp)
add_sg_ui_test(SmartGroundCartesianDiag CartesianDiagTest.cpp)
add_sg_test(SmartGroundCartesianProbe CartesianProbeTest.cpp)
add_sg_test(SmartGroundCartesianCurveIso CartesianCurveIsoTest.cpp)
add_sg_ui_test(SmartGroundCartesianAppView CartesianAppViewTest.cpp)
add_sg_ui_test(SmartGroundCartesianShot CartesianShotTest.cpp)
add_sg_ui_test(SmartGroundCartesianUx CartesianUxTest.cpp)
add_sg_ui_test(SmartGroundCartesianRound2 CartesianRound2Test.cpp)


add_sg_ui_test(SmartGroundCartesianRound2Shot CartesianRound2Shot.cpp)

add_sg_ui_test(SmartGroundCartesianGridExtent CartesianGridExtentTest.cpp)

add_sg_ui_test(SmartGroundCartesianRound3 CartesianRound3Test.cpp)

add_sg_ui_test(SmartGroundCartesianRound4 CartesianRound4Test.cpp)
