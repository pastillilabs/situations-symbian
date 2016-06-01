include(../../../common/common.pri)

QT -= gui
QT += declarative

TARGET = situationsactionwlan
DESTDIR = ../../../bin
TEMPLATE = lib
CONFIG += plugin

INCLUDEPATH += $$PWD/../../../common
DEPENDPATH += $$PWD/../../../common

HEADERS += \
    wlanplugin.h \
    wlanaction.h

SOURCES += \
    wlanplugin.cpp \
    wlanaction.cpp

RESOURCES += \
    wlan.qrc

OTHER_FILES += \
    qml/wlan/Action.qml \
    qml/wlan/Settings.qml

symbian {
    LIBS += -lwlanstate

    load(data_caging_paths)
    TARGET.UID3 = 0x20065CCB
    TARGET.EPOCALLOWDLLDATA = 1
    addFiles.sources = situationsactionwlan.dll
    addFiles.path = $$QT_PLUGINS_BASE_DIR
    DEPLOYMENT += addFiles
    target.path += $$[QT_INSTALL_PLUGINS]
    INSTALLS += target
}

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../../bin/ -lsituationscommon
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../../bin/ -lsituationscommon
else:symbian: LIBS += -lsituationscommon
else:unix: LIBS += -L$$OUT_PWD/../../../bin/ -lsituationscommon

