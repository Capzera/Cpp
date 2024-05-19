#-------------------------------------------------
#
# Project created by QtCreator 2023-12-29T16:04:40
#
#-------------------------------------------------

QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
TARGET = CursorPos
TEMPLATE = app
DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    globalmouseevent_win.cpp
HEADERS += \
    mainwindow.h \
    globalmouseevent.h

FORMS += \
    mainwindow.ui

win32 {
LIBS+= -luser32    # 使用WindowsAPI需要链接库
}

DISTFILES += \
    logo.rc
    RC_FILE += logo.rc
