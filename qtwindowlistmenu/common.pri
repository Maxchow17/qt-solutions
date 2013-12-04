infile(config.pri, SOLUTIONS_LIBRARY, yes): CONFIG += qtwindowlistmenu-uselib
TEMPLATE += fakelib
QTWINDOWLISTMENU_LIBNAME = $$qtLibraryTarget(QtSolutions_WindowMenu-2.2)
TEMPLATE -= fakelib
QTWINDOWLISTMENU_LIBDIR = $$PWD/lib
unix:qtwindowlistmenu-uselib:!qtwindowlistmenu-buildlib:QMAKE_RPATHDIR += $$QTWINDOWLISTMENU_LIBDIR
