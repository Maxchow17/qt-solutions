infile(config.pri, SOLUTIONS_LIBRARY, yes): CONFIG += qtcolortriangle-uselib
TEMPLATE += fakelib
QTCOLORTRIANGLE_LIBNAME = $$qtLibraryTarget(QtSolutions_ColorTriangle-2.6)
TEMPLATE -= fakelib
QTCOLORTRIANGLE_LIBDIR = $$PWD/lib
unix:qtcolortriangle-uselib:!qtcolortriangle-buildlib:QMAKE_RPATHDIR += $$QTCOLORTRIANGLE_LIBDIR
