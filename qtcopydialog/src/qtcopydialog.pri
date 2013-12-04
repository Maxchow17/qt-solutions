include(../common.pri)
INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

qtcopydialog-uselib:!qtcopydialog-buildlib {
    LIBS += -L$$QTCOPYDIALOG_LIBDIR -l$$QTCOPYDIALOG_LIBNAME
} else {
    SOURCES += $$PWD/qtcopydialog.cpp \
               $$PWD/qtfilecopier.cpp
    HEADERS += $$PWD/qtcopydialog.h \
               $$PWD/qtfilecopier.h
    FORMS   += $$PWD/qtcopydialog.ui \
               $$PWD/qtoverwritedialog.ui \
               $$PWD/qtotherdialog.ui
}

win32 {
    contains(TEMPLATE, lib):contains(CONFIG, shared):DEFINES += QT_QTCOPYDIALOG_EXPORT
    else:qtcopydialog-uselib:DEFINES += QT_QTCOPYDIALOG_IMPORT
}
