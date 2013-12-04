include(../common.pri)
INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

qtwindowlistmenu-uselib:!qtwindowlistmenu-buildlib {
    LIBS += -L$$QTWINDOWLISTMENU_LIBDIR -l$$QTWINDOWLISTMENU_LIBNAME
} else {
    SOURCES += $$PWD/qtwindowlistmenu.cpp
    HEADERS += $$PWD/qtwindowlistmenu.h
}

win32 {
    contains(TEMPLATE, lib):contains(CONFIG, shared):DEFINES += QT_QTWINDOWLISTMENU_EXPORT
    else:qtwindowlistmenu-uselib:DEFINES += QT_QTWINDOWLISTMENU_IMPORT
}
