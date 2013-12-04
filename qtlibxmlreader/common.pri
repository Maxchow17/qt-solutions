infile(config.pri, SOLUTIONS_LIBRARY, yes): CONFIG += qtlibxmlreader-uselib
TEMPLATE += fakelib
QTLIBXMLREADER_LIBNAME = $$qtLibraryTarget(QtSolutions_libxml2reader-2.1)
TEMPLATE -= fakelib
QTLIBXMLREADER_LIBDIR = $$PWD/lib
unix:qtlibxmlreader-uselib:!qtlibxmlreader-buildlib:QMAKE_RPATHDIR += $$QTLIBXMLREADER_LIBDIR
