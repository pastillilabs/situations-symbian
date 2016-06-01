include(../../../common/common.pri)

QT -= gui
QT += declarative

TARGET = situationsactionflightmode
DESTDIR = ../../../bin
TEMPLATE = lib
CONFIG += plugin

INCLUDEPATH += $$PWD/../../../common
DEPENDPATH += $$PWD/../../../common

HEADERS += \
    flightmodeplugin.h \
    flightmodeaction.h

SOURCES += \
    flightmodeplugin.cpp \
    flightmodeaction.cpp

RESOURCES += flightmode.qrc

OTHER_FILES += \
    qml/flightmode/Action.qml \
    qml/flightmode/Settings.qml

symbian {
    load(data_caging_paths)
#    TARGET.UID3 = 0xEA20E5F2 // TODO
    TARGET.EPOCALLOWDLLDATA = 1
    addFiles.sources = situationsactionflightmode.dll
    addFiles.path = $$QT_PLUGINS_BASE_DIR
    DEPLOYMENT += addFiles
    target.path += $$[QT_INSTALL_PLUGINS]
    INSTALLS += target
}

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../../bin/ -lsituationscommon
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../../bin/ -lsituationscommon
else:symbian: LIBS += -lsituationscommon
else:unix: LIBS += -L$$OUT_PWD/../../../bin/ -lsituationscommon

