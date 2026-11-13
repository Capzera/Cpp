cmake_minimum_required(VERSION 3.5)
project(TrachtenbergTrainer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 COMPONENTS Widgets REQUIRED)

add_executable(TrachtenbergTrainer
    main.cpp
    TrainerWidget.h
    TrainerWidget.cpp
    MainWindow.h
    MainWindow.cpp
    ChallengeWidget.h
    ChallengeWidget.cpp
    ExtremeWidget.h
    ExtremeWidget.cpp
    LeaderboardWidget.h
    LeaderboardWidget.cpp
    SettingsWidget.h
    SettingsWidget.cpp
    CustomUI.h
    CustomUI.cpp

)
target_link_libraries(TrachtenbergTrainer PRIVATE Qt6::Widgets)