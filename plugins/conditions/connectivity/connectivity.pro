include(../../../common/common.pri)

QT -= gui
QT += declarative network

TARGET = situationsconditionconnectivity
DESTDIR = ../../../bin
TEMPLATE = lib
CONFIG += plugin
CONFIG += mobility
MOBILITY += connectivity

INCLUDEPATH += $$PWD/../../../common
DEPENDPATH += $$PWD/../../../common

SOURCES += \
    connectivityplugin.cpp \
    connectivitycondition.cpp \
    connectivityengine.cpp \
    wlanscanner.cpp \
    btscanner.cpp

symbian:SOURCES += wlanscanner_symbian.cpp

HEADERS += \
    connectivityplugin.h \
    connectivitycondition.h \
    connectivityengine.h \
    wlanscanner.h \
    btscanner.h

symbian:HEADERS += wlanscanner_symbian.h

RESOURCES += connectivity.qrc

OTHER_FILES += \
    qml/connectivity/Condition.qml \
    qml/connectivity/Editor.qml

OTHER_FILES +=

symbian {
    LIBS += -lecom
    LIBS += -lcommdb
    LIBS += -lwlanstate
    LIBS += -lbluetooth
    LIBS += -lesock

    load(data_caging_paths)
    TARGET.UID3 = 0x20065CC7
    TARGET.EPOCALLOWDLLDATA = 1
    addFiles.sources = situationsconditionconnectivity.dll
    addFiles.path = $$QT_PLUGINS_BASE_DIR
    DEPLOYMENT += addFiles
    target.path += $$[QT_INSTALL_PLUGINS]
    INSTALLS += target
}

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../../bin/ -lsituationscommon
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../../bin/ -lsituationscommon
else:symbian: LIBS += -lsituationscommon
else:unix: LIBS += -L$$OUT_PWD/../../../bin/ -lsituationscommon

