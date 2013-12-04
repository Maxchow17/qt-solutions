TEMPLATE=subdirs
CONFIG += ordered
include(common.pri)
qtspellcheckingtextedit-uselib:SUBDIRS=buildlib
SUBDIRS+=example
!mac {
    error( "This component is only supported on Mac OS X. No makefile generated." )
}
