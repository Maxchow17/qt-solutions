TEMPLATE=lib
CONFIG += qt dll qtcopydialog-buildlib
mac:CONFIG += absolute_library_soname
win32|mac:!wince*:!win32-msvc:!macx-xcode:CONFIG += debug_and_release build_all
include(../src/qtcopydialog.pri)
TARGET = $$QTCOPYDIALOG_LIBNAME
DESTDIR = $$QTCOPYDIALOG_LIBDIR
win32 {
    DLLDESTDIR = $$[QT_INSTALL_BINS]
    QMAKE_DISTCLEAN += $$[QT_INSTALL_BINS]\\$${QTCOPYDIALOG_LIBNAME}.dll
}
target.path = $$DESTDIR
INSTALLS += target
