include(../common.pri)
INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

qtthumbwheel-uselib:!qtthumbwheel-buildlib {
    LIBS += -L$$QTTHUMBWHEEL_LIBDIR -l$$QTTHUMBWHEEL_LIBNAME
} else {
    SOURCES += $$PWD/qtthumbwheel.cpp
    HEADERS += $$PWD/qtthumbwheel.h
}

win32 {
    contains(TEMPLATE, lib):contains(CONFIG, shared):DEFINES += QT_QTTHUMBWHEEL_EXPORT
    else:qtthumbwheel-uselib:DEFINES += QT_QTTHUMBWHEEL_IMPORT
}
