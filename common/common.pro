include(common.pri)

TEMPLATE = lib
TARGET = situationscommon
DESTDIR = ../bin

DEFINES += COMMON_LIBRARY

HEADERS += \
    common_global.h \
    situationmanager.h \
    situation.h \
    pluginmanager.h \
    messageids.h \
    conditiongrouplistmodel.h \
    conditiongroup.h \
    condition.h \
    action.h \
    actionplugin.h \
    conditionplugin.h \
    actiongroup.h \
    actiongrouplistmodel.h \
    trace.h \
    translationmanager.h \
    identifiers.h \
    storage.h \
    storagemanager.h

SOURCES += \
    situationmanager.cpp \
    situation.cpp \
    pluginmanager.cpp \
    conditiongrouplistmodel.cpp \
    conditiongroup.cpp \
    condition.cpp \
    action.cpp \
    actiongroup.cpp \
    actiongrouplistmodel.cpp \
    translationmanager.cpp \
    identifiers.cpp \
    storage.cpp \
    storagemanager.cpp

RESOURCES += common.qrc

symbian {
    TARGET.UID3 = 0x20065CC3
    TARGET.EPOCALLOWDLLDATA = 1
    addFiles.sources = situationscommon.dll
    addFiles.path = !:/sys/bin
    DEPLOYMENT += addFiles
}

