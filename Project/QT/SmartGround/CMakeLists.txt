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
    src/ui/MainWindow.h
    src/main.cpp
    src/ui/MainWindow.cpp
    src/render/GraphElements.cpp
    src/render/MatrixBlock.h
    src/render/MatrixBlock.cpp
    src/render/LatexBlock.h
    src/render/LatexBlock.cpp
    src/modules/IModule.cpp
    src/modules/TarjanModule.cpp
    src/modules/ArrayModule.cpp
    src/modules/BubbleSortModule.cpp
    src/modules/SelectionSortModule.cpp
    src/modules/InsertionSortModule.cpp
    src/modules/QuickSortModule.cpp
    src/modules/MergeSortModule.cpp
    src/modules/HeapBasicsModule.cpp
    src/modules/MonotonicStackModule.cpp
    src/modules/DSUBasicModule.cpp
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
    src/modules/SandboxLatexModule.h
    src/modules/SandboxLatexModule.cpp
    src/resources.qrc
)
target_link_libraries(SmartGround Qt6::Widgets Qt6::Gui Qt6::Core Qt6::Svg)
target_include_directories(SmartGround PRIVATE src)



