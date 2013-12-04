HEADERS       = mainwindow.h \
                mdichild.h \
                menueditor.h
SOURCES       = main.cpp \
                mainwindow.cpp \
                mdichild.cpp \
                menueditor.cpp
RESOURCES     = editor.qrc
FORMS         = mainwindow.ui \
                menueditor.ui

include(../../src/qtwindowlistmenu.pri)
