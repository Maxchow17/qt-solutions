TEMPLATE = app
INCLUDEPATH += .

include(../../src/qtthumbwheel.pri)

# Input
SOURCES += main.cpp
RESOURCES = xform.qrc

unix {
    MOC_DIR=.moc
    OBJECTS_DIR=.obj
}
