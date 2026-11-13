cmake_minimum_required(VERSION 3.16)
project(TarjanVisualizer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 COMPONENTS Widgets Gui Core REQUIRED)

add_executable(TarjanVisualizer main.cpp)

target_link_libraries(TarjanVisualizer Qt6::Widgets Qt6::Gui Qt6::Core)

