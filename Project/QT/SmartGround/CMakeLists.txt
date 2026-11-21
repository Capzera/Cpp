cmake_minimum_required(VERSION 3.16)
project(SmartGround LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
find_package(Qt6 COMPONENTS Widgets Gui Core Svg REQUIRED)
add_executable(SmartGround
    src/engine/AnimFrame.h
    src/render/GraphElements.h
    src/render/SceneGrid.h
    src/render/SceneGrid.cpp
    src/core/ViewportFitter.h
    src/core/ViewportFitter.cpp
    src/ui/UiTheme.h
    src/ui/widgets/MyButton.h
    src/ui/widgets/MyButton.cpp
    src/ui/widgets/MyToggleSwitch.h
    src/ui/widgets/MyToggleSwitch.cpp
    src/ui/widgets/MyRadioButton.h
    src/ui/widgets/MyRadioButton.cpp
    src/ui/widgets/MyCheckBox.h
    src/ui/widgets/MyCheckBox.cpp
    src/ui/widgets/MyComboBox.h
    src/ui/widgets/MyComboBox.cpp
    src/ui/widgets/MyTextEdit.h
    src/ui/widgets/MyTextEdit.cpp
    src/ui/widgets/MyCollapsible.h
    src/ui/widgets/MyCollapsible.cpp
    src/ui/widgets/MySlider.h
    src/ui/widgets/MySlider.cpp
    src/ui/widgets/MyColorSwatch.h
    src/ui/widgets/MyColorSwatch.cpp
    src/ui/widgets/MyColorPicker.h
    src/ui/widgets/MyColorPicker.cpp
    src/ui/widgets/MySegmentedControl.h
    src/ui/widgets/MySegmentedControl.cpp
    src/ui/widgets/MyTextInput.h
    src/ui/widgets/MyTextInput.cpp
    src/ui/widgets/TableToolBar.h
    src/ui/widgets/TableToolBar.cpp
    src/ui/widgets/TableIconButton.h
    src/ui/widgets/TableIconButton.cpp
    src/ui/widgets/FlowLayout.h
    src/ui/widgets/FlowLayout.cpp
    src/ui/widgets/CanvasToolBar.h
    src/ui/widgets/CanvasToolBar.cpp
    src/ui/widgets/CanvasBrushSlider.h
    src/ui/widgets/CanvasBrushSlider.cpp
    src/ui/widgets/ToolIconButton.h
    src/ui/widgets/ToolIconButton.cpp
    src/ui/widgets/AnnotationEditor.h
    src/ui/widgets/AnnotationEditor.cpp
    src/canvas/AnnotationItem.h
    src/canvas/AnnotationItem.cpp
    src/canvas/StrokeItem.h
    src/canvas/StrokeItem.cpp
    src/canvas/ShapeItem.h
    src/canvas/ShapeItem.cpp
    src/canvas/TextBoxItem.h
    src/canvas/TextBoxItem.cpp
    src/canvas/MarkdownItem.h
    src/canvas/MarkdownItem.cpp
    src/canvas/LatexItem.h
    src/canvas/LatexItem.cpp
    src/canvas/LatexService.h
    src/canvas/LatexService.cpp
    src/canvas/CanvasTableItem.h
    src/canvas/CanvasTableItem.cpp
    src/canvas/AnnotationScene.h
    src/canvas/AnnotationScene.cpp
    src/canvas/InlineTextEdit.h
    src/canvas/InlineTextEdit.cpp
    src/canvas/SelectionOverlay.h
    src/canvas/SelectionOverlay.cpp
    src/canvas/ToolController.h
    src/canvas/ToolController.cpp
    src/canvas/CanvasView.h
    src/canvas/CanvasView.cpp
    src/modules/IModule.h
    src/modules/GraphModuleUtils.h
    src/modules/GraphRenderHelper.h
    src/modules/TopologicalSortModule.h
    src/modules/DijkstraNaiveModule.h
    src/modules/KruskalModule.h
    src/modules/BFSModule.h
    src/modules/Knapsack01Module.h
    src/modules/LISModule.h
    src/modules/KMPNextModule.h
    src/modules/KMPMatchModule.h
    src/modules/SegmentTreeCommon.h
    src/modules/SegmentTreeBuildModule.h
    src/modules/SegmentTreeEditBase.h
    src/modules/SegmentTreeTagModule.h
    src/modules/SegmentTreeQueryModule.h
    src/modules/FenwickTreeModule.h
    src/modules/KnapsackCompleteModule.h
    src/modules/TarjanCutVertexBridgeModule.h
    src/modules/HungarianModule.h
    src/modules/HeapAdjustModule.h
    src/modules/HeapSortModule.h
    src/modules/HeapTreeBase.h
    src/modules/TwoRowArrayModule.h
    src/ui/MainWindow.h
    src/main.cpp
    src/ui/MainWindow.cpp
    src/render/GraphElements.cpp
    src/render/MatrixBlock.h
    src/render/MatrixBlock.cpp
    src/render/LatexBlock.h
    src/render/LatexBlock.cpp
    src/modules/IModule.cpp
    src/modules/ModuleFields.h
    src/modules/ModuleFields.cpp
    src/modules/TarjanModule.cpp
    src/modules/BubbleSortModule.cpp
    src/modules/SelectionSortModule.cpp
    src/modules/InsertionSortModule.cpp
    src/modules/QuickSortModule.cpp
    src/modules/MergeSortModule.cpp
    src/modules/HeapBasicsModule.cpp
    src/modules/MonotonicStackModule.cpp
    src/modules/TwoRowArrayModule.cpp
    src/modules/PrefixSumModule.cpp
    src/modules/DifferenceModule.cpp
    src/modules/StackModule.cpp
    src/modules/QueueModule.cpp
    src/modules/BinarySearchClosedModule.cpp
    src/modules/BinarySearchHalfOpenModule.cpp
    src/modules/TopologicalSortModule.cpp
    src/modules/DijkstraNaiveModule.cpp
    src/modules/KruskalModule.cpp
    src/modules/BFSModule.cpp
    src/modules/Knapsack01Module.cpp
    src/modules/LISModule.cpp
    src/modules/KMPNextModule.cpp
    src/modules/KMPMatchModule.cpp
    src/modules/SegmentTreeBuildModule.cpp
    src/modules/SegmentTreeEditBase.cpp
    src/modules/SegmentTreeTagModule.cpp
    src/modules/SegmentTreeQueryModule.cpp
    src/modules/FenwickTreeModule.cpp
    src/modules/KnapsackCompleteModule.cpp
    src/modules/TarjanCutVertexBridgeModule.cpp
    src/modules/HungarianModule.cpp
    src/modules/HeapAdjustModule.cpp
    src/modules/HeapSortModule.cpp
    src/modules/FreeStructureBase.h
    src/modules/FreeStructureBase.cpp
    src/modules/SandboxArrayModule.h
    src/modules/SandboxArrayModule.cpp
    src/modules/SandboxMatrixModule.h
    src/modules/SandboxMatrixModule.cpp
    src/modules/SandboxTreeModule.h
    src/modules/SandboxTreeModule.cpp
    src/modules/SandboxGraphModule.h
    src/modules/SandboxGraphModule.cpp
    src/modules/SandboxBlankModule.h
    src/modules/SandboxBlankModule.cpp
    src/modules/ShellSortModule.h
    src/modules/ShellSortModule.cpp
    src/modules/FastPowModule.h
    src/modules/FastPowModule.cpp
    src/modules/SieveOfEratosthenesModule.h
    src/modules/SieveOfEratosthenesModule.cpp
    src/modules/EulerSieveModule.h
    src/modules/EulerSieveModule.cpp
    src/modules/ForwardStarModule.h
    src/modules/ForwardStarModule.cpp
    src/modules/DSUPathCompressionModule.h
    src/modules/DSUPathCompressionModule.cpp
    src/modules/TreeDpBase.h
    src/modules/TreeSubtreeSizeModule.h
    src/modules/TreeSubtreeSizeModule.cpp
    src/modules/TreeDiameterModule.h
    src/modules/TreeDiameterModule.cpp
    src/modules/DijkstraHeapModule.h
    src/modules/DijkstraHeapModule.cpp
    src/modules/GcdModule.h
    src/modules/GcdModule.cpp
    src/modules/DfsModule.h
    src/modules/DfsModule.cpp
    src/modules/PrimModule.h
    src/modules/PrimModule.cpp
    src/modules/CountingSortModule.h
    src/modules/CountingSortModule.cpp
    src/modules/BipartiteModule.h
    src/modules/BipartiteModule.cpp
    src/modules/EulerModule.h
    src/modules/EulerModule.cpp
    src/modules/IntervalSchedModule.h
    src/modules/IntervalSchedModule.cpp
    src/modules/TrieModule.h
    src/modules/TrieModule.cpp
    src/modules/HashTableModule.h
    src/modules/HashTableModule.cpp
    src/modules/BucketSortModule.h
    src/modules/BucketSortModule.cpp
    src/modules/RadixSortModule.h
    src/modules/RadixSortModule.cpp
    src/modules/CircularQueueModule.h
    src/modules/CircularQueueModule.cpp
    src/modules/MonotonicQueueModule.h
    src/modules/MonotonicQueueModule.cpp
    src/modules/ExgcdModule.h
    src/modules/ExgcdModule.cpp
    src/modules/HuffmanTreeModule.h
    src/modules/HuffmanTreeModule.cpp
    src/modules/StringHashModule.h
    src/modules/StringHashModule.cpp
    src/modules/LCSModule.h
    src/modules/LCSModule.cpp
    src/render/ValueBar.h
    src/render/ValueBar.cpp
    src/render/SegmentBar.h
    src/render/SegmentBar.cpp
    src/core/CartesianExpression.h
    src/core/CartesianExpression.cpp
    src/render/CartesianElements.h
    src/render/CartesianElements.cpp
    src/modules/CartesianModule.h
    src/modules/CartesianModule.cpp
    src/ui/widgets/CartesianPanel.h
    src/ui/widgets/CartesianPanel.cpp
    src/resources.qrc
)
target_link_libraries(SmartGround Qt6::Widgets Qt6::Gui Qt6::Core Qt6::Svg)
target_include_directories(SmartGround PRIVATE src)
set_target_properties(SmartGround PROPERTIES WIN32_EXECUTABLE TRUE)

set(SMARTGROUND_VERSION_FILE ${CMAKE_CURRENT_BINARY_DIR}/generated/AppVersionGenerated.h)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/generated)
set(SMARTGROUND_BUILD_STAMP "0")
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/.versionstamp)
    file(READ ${CMAKE_CURRENT_SOURCE_DIR}/.versionstamp SMARTGROUND_BUILD_STAMP)
    string(STRIP "${SMARTGROUND_BUILD_STAMP}" SMARTGROUND_BUILD_STAMP)
else()
    find_package(Git QUIET)
    if(GIT_FOUND)
        execute_process(COMMAND ${GIT_EXECUTABLE} rev-list --count HEAD -- .
                        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                        OUTPUT_VARIABLE SMARTGROUND_GIT_COUNT
                        OUTPUT_STRIP_TRAILING_WHITESPACE
                        ERROR_QUIET)
        execute_process(COMMAND ${GIT_EXECUTABLE} status --porcelain -- .
                        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                        OUTPUT_VARIABLE SMARTGROUND_GIT_STATUS
                        OUTPUT_STRIP_TRAILING_WHITESPACE
                        ERROR_QUIET)
        if(SMARTGROUND_GIT_COUNT)
            set(SMARTGROUND_BUILD_STAMP "${SMARTGROUND_GIT_COUNT}")
        endif()
        if(SMARTGROUND_GIT_STATUS)
            math(EXPR SMARTGROUND_BUILD_STAMP "${SMARTGROUND_BUILD_STAMP} + 1")
        endif()
    endif()
endif()
file(WRITE ${SMARTGROUND_VERSION_FILE}
"#ifndef APPVERSIONGENERATED_H\n#define APPVERSIONGENERATED_H\nnamespace AppVersionBuild {\ninline const char* kStamp = \"${SMARTGROUND_BUILD_STAMP}\";\n}\n#endif\n")
target_include_directories(SmartGround PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/generated)




