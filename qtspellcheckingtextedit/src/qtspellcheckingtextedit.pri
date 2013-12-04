include(../common.pri)
INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

qtspellcheckingtextedit-uselib:!qtspellcheckingtextedit-buildlib {
    LIBS += -L$$QTSPELLCHECKINGTEXTEDIT_LIBDIR -l$$QTSPELLCHECKINGTEXTEDIT_LIBNAME
} else {
    HEADERS += $$PWD/qtspellcheckingtextedit.h $$PWD/privateqt_mac.h
    OBJECTIVE_SOURCES += $$PWD/qtspellcheckingtextedit.cpp $$PWD/qtspellcheckingtextedit_mac.mm $$PWD/privateqt_mac.mm
}
