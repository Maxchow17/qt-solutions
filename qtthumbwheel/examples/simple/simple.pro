TEMPLATE = app
INCLUDEPATH += .

include(../../src/qtthumbwheel.pri)

# Input
SOURCES += main.cpp
unix {
    OBJECTS_DIR=.obj
    MOC_DIR = .moc
}
