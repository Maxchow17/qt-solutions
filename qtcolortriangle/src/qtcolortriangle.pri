include(../common.pri)
INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

qtcolortriangle-uselib:!qtcolortriangle-buildlib {
    LIBS += -L$$QTCOLORTRIANGLE_LIBDIR -l$$QTCOLORTRIANGLE_LIBNAME
} else {
    SOURCES += $$PWD/qtcolortriangle.cpp
    HEADERS += $$PWD/qtcolortriangle.h
}

win32 {
    contains(TEMPLATE, lib):contains(CONFIG, shared):DEFINES += QT_QTCOLORTRIANGLE_EXPORT
    else:qtcolortriangle-uselib:DEFINES += QT_QTCOLORTRIANGLE_IMPORT
}
