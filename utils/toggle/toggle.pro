include(../../common/common.pri)

TARGET = situationstoggle
DESTDIR = ../../bin

QT -= gui

TEMPLATE = app

CONFIG += console

INCLUDEPATH += $$PWD/../../common
DEPENDPATH += $$PWD/../../common

SOURCES += main.cpp

symbian {
    LIBS += -lcellulardatastate
    LIBS += -letel
    LIBS += -letelmm
    LIBS += -lcustomapi

    load(data_caging_paths)
    TARGET.UID3 = 0x2006F084
    TARGET.CAPABILITY += ALL -TCB -DRM -AllFiles
}

