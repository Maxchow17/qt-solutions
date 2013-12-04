infile(config.pri, SOLUTIONS_LIBRARY, yes): CONFIG += qtspellcheckingtextedit-uselib
TEMPLATE += fakelib
QTSPELLCHECKINGTEXTEDIT_LIBNAME = $$qtLibraryTarget(QtSolutions_SpellcheckingTextEdit-2.1)
TEMPLATE -= fakelib
QTSPELLCHECKINGTEXTEDIT_LIBDIR = $$PWD/lib
unix:qtspellcheckingtextedit-uselib:!qtspellcheckingtextedit-buildlib:QMAKE_RPATHDIR += $$QTSPELLCHECKINGTEXTEDIT_LIBDIR
