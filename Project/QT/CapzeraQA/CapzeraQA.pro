QT += core gui network charts widgets webenginewidgets webchannel
CONFIG += c++17
CONFIG -= qtquickcompiler

SOURCES += \
    AnalysisTestWidget.cpp \
    AnalysisProblemWidget.cpp \
    CapzeraButton.cpp \
    CapzeraComboBox.cpp \
    CapzeraDialog.cpp \
    CapzeraToggle.cpp \
    ClassDashboardWidget.cpp \
    DateTimePickerWidget.cpp \
    NetworkManager.cpp \
    main.cpp \
    mainwindow.cpp \
    StudentPage.cpp \
    TeacherPage.cpp \
    ScoreChartWidget.cpp \
    StudentHistoryWidget.cpp \
    StudentDashboardWidget.cpp \
    StudentExamWidget.cpp \
    StudentSettingWidget.cpp \
    TeacherSettingPage.cpp \
    TeacherStudentPage.cpp \
    TeacherClassPage.cpp \
    TeacherGradingPage.cpp \
    TeacherProblemPage.cpp \
    TeacherExamPage.cpp

HEADERS += \
    AnalysisTestWidget.h \
    AnalysisProblemWidget.h \\
    CapzeraButton.h \
    CapzeraComboBox.h \
    CapzeraDialog.h \
    CapzeraToggle.h \
    ClassDashboardWidget.h \
    DateTimePickerWidget.h \
    NetworkManager.h \
    StudentPage.h \
    TeacherPage.h \
    StudentHistoryWidget.h \
    StudentDashboardWidget.h \
    StudentExamWidget.h \
    StudentSettingWidget.h \
    mainwindow.h \
    CommonTypes.h \
    ScoreChartWidget.h \
    TeacherSettingPage.h \
    TeacherStudentPage.h \
    TeacherClassPage.h \
    TeacherGradingPage.h \
    TeacherProblemPage.h \
    TeacherExamPage.h

FORMS += \
    mainwindow.ui

TRANSLATIONS += \
    CapzeraQA_zh_CN.ts
CONFIG += lrelease
CONFIG += embed_translations

RC_ICONS = logo.ico
RESOURCES += assets.qrc