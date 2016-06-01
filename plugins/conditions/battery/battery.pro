include(../../../common/common.pri)

QT -= gui
QT += declarative

TARGET = situationsconditionbattery
DESTDIR = ../../../bin
TEMPLATE = lib
CONFIG += plugin
CONFIG += mobility
MOBILITY += systeminfo

INCLUDEPATH += $$PWD/../../../common
DEPENDPATH += $$PWD/../../../common

HEADERS += \
    batteryplugin.h \
    batterycondition.h

SOURCES += \
    batteryplugin.cpp \
    batterycondition.cpp

RESOURCES += battery.qrc

OTHER_FILES += \
    qml/battery/Condition.qml \
    qml/battery/Editor.qml

OTHER_FILES +=

symbian {
    load(data_caging_paths)
    TARGET.UID3 = 0x20065DB9
    TARGET.EPOCALLOWDLLDATA = 1
    addFiles.sources = situationsconditionbattery.dll
    addFiles.path = $$QT_PLUGINS_BASE_DIR
    DEPLOYMENT += addFiles
    target.path += $$[QT_INSTALL_PLUGINS]
    INSTALLS += target
}

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../../bin/ -lsituationscommon
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../../bin/ -lsituationscommon
else:symbian: LIBS += -lsituationscommon
else:unix: LIBS += -L$$OUT_PWD/../../../bin/ -lsituationscommon

