infile(config.pri, SOLUTIONS_LIBRARY, yes): CONFIG += qtcopydialog-uselib
TEMPLATE += fakelib
QTCOPYDIALOG_LIBNAME = $$qtLibraryTarget(QtSolutions_CopyDialog-2.2)
TEMPLATE -= fakelib
QTCOPYDIALOG_LIBDIR = $$PWD/lib
unix:qtcopydialog-uselib:!qtcopydialog-buildlib:QMAKE_RPATHDIR += $$QTCOPYDIALOG_LIBDIR
