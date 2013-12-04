infile(config.pri, SOLUTIONS_LIBRARY, yes): CONFIG += qtthumbwheel-uselib
TEMPLATE += fakelib
QTTHUMBWHEEL_LIBNAME = $$qtLibraryTarget(QtSolutions_ThumbWheel-2.4)
TEMPLATE -= fakelib
QTTHUMBWHEEL_LIBDIR = $$PWD/lib
unix:qtthumbwheel-uselib:!qtthumbwheel-buildlib:QMAKE_RPATHDIR += $$QTTHUMBWHEEL_LIBDIR
