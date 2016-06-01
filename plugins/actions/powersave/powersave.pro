include(../../../common/common.pri)

QT -= gui
QT += declarative

TARGET = situationsactionpowersave
DESTDIR = ../../../bin
TEMPLATE = lib
CONFIG += plugin

INCLUDEPATH += $$PWD/../../../common
DEPENDPATH += $$PWD/../../../common

HEADERS += \
    powersaveaction.h \
    powersaveplugin.h

SOURCES += \
    powersaveaction.cpp \
    powersaveplugin.cpp

RESOURCES += powersave.qrc

OTHER_FILES += \
    qml/powersave/Action.qml \
    qml/powersave/Settings.qml

symbian {
    LIBS += -lpsmclient

    load(data_caging_paths)
    TARGET.UID3 = 0x20065DB4
    TARGET.EPOCALLOWDLLDATA = 1
    addFiles.sources = situationsactionpowersave.dll
    addFiles.path = $$QT_PLUGINS_BASE_DIR
    DEPLOYMENT += addFiles
    target.path += $$[QT_INSTALL_PLUGINS]
    INSTALLS += target
}

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../../bin/ -lsituationscommon
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../../bin/ -lsituationscommon
else:symbian: LIBS += -lsituationscommon
else:unix: LIBS += -L$$OUT_PWD/../../../bin/ -lsituationscommon

