TEMPLATE=lib
CONFIG += qt dll qtcolortriangle-buildlib
mac:CONFIG += absolute_library_soname
win32|mac:!wince*:!win32-msvc:!macx-xcode:CONFIG += debug_and_release build_all
include(../src/qtcolortriangle.pri)
TARGET = $$QTCOLORTRIANGLE_LIBNAME
DESTDIR = $$QTCOLORTRIANGLE_LIBDIR
win32 {
    DLLDESTDIR = $$[QT_INSTALL_BINS]
    QMAKE_DISTCLEAN += $$[QT_INSTALL_BINS]\\$${QTCOLORTRIANGLE_LIBNAME}.dll
}
target.path = $$DESTDIR
INSTALLS += target
