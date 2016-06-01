include(../../../common/common.pri)

QT -= gui
QT += declarative

TARGET = situationsactionbluetooth
DESTDIR = ../../../bin
TEMPLATE = lib
CONFIG += plugin
CONFIG += mobility
MOBILITY += connectivity

INCLUDEPATH += $$PWD/../../../common
DEPENDPATH += $$PWD/../../../common

HEADERS += \
    bluetoothaction.h \
    bluetoothplugin.h

SOURCES += \
    bluetoothaction.cpp \
    bluetoothplugin.cpp

RESOURCES += bluetooth.qrc

OTHER_FILES += \
    qml/bluetooth/Action.qml \
    qml/bluetooth/Settings.qml

symbian {
    load(data_caging_paths)
    TARGET.UID3 = 0x20065DB3
    TARGET.EPOCALLOWDLLDATA = 1
    addFiles.sources = situationsactionbluetooth.dll
    addFiles.path = $$QT_PLUGINS_BASE_DIR
    DEPLOYMENT += addFiles
    target.path += $$[QT_INSTALL_PLUGINS]
    INSTALLS += target
}

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../../bin/ -lsituationscommon
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../../bin/ -lsituationscommon
else:symbian: LIBS += -lsituationscommon
else:unix: LIBS += -L$$OUT_PWD/../../../bin/ -lsituationscommon

