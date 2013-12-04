TEMPLATE = app
TARGET = testxml
CONFIG += console
QT += xml

include(../src/qtlibxmlreader.pri)

SOURCES += main.cpp parser.cpp
HEADERS += parser.h
