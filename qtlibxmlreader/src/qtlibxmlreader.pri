# If libxml2 is not found in a system directory, uncomment the
# following lines and enter the location of libxml2 headers and
# libraries: 
# LIBXML2_INCLUDEPATH = c:/some/path
# LIBXML2_LIBPATH = c:/some/other/path

include(../common.pri)
INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

qtlibxmlreader-uselib:!qtlibxmlreader-buildlib {
    LIBS += -L$$QTLIBXMLREADER_LIBDIR -l$$QTLIBXMLREADER_LIBNAME
} else {
    INCLUDEPATH += $$LIBXML2_INCLUDEPATH
    !isEmpty(LIBXML2_LIBPATH):LIBS += -l$$LIBXML2_LIBPATH
    SOURCES += $$PWD/qtlibxmlreader.cpp
    HEADERS += $$PWD/qtlibxmlreader.h    
}
LIBS += -lxml2

win32 {
    contains(TEMPLATE, lib):contains(CONFIG, shared):DEFINES += QT_QTLIBXMLREADER_EXPORT
    else:qtlibxmlreader-uselib:DEFINES += QT_QTLIBXMLREADER_IMPORT
}
