QT += widgets

CONFIG += c++17

SOURCES += \
    main.cpp \
    widget.cpp

HEADERS += \
    widget.h

FORMS += \
    widget.ui

LIBS += -luser32 -lwinmm

msvc {
    QMAKE_LFLAGS += /MANIFESTUAC:"level='requireAdministrator' uiAccess='false'"
}

!msvc {
    RC_FILE = admin.rc
}

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target