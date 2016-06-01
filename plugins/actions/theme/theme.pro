include(../../../common/common.pri)

QT -= gui
QT += declarative

TARGET = situationsactiontheme
DESTDIR = ../../../bin
TEMPLATE = lib
CONFIG += plugin

INCLUDEPATH += $$PWD/../../../common
DEPENDPATH += $$PWD/../../../common

HEADERS += \
    themeplugin.h \
    themeaction.h \
    themelistmodel.h

SOURCES += \
    themeplugin.cpp \
    themeaction.cpp \
    themelistmodel.cpp

RESOURCES += \
    theme.qrc

OTHER_FILES += \
    qml/theme/Action.qml \
    qml/theme/Editor.qml \
    qml/theme/Settings.qml

symbian {
    LIBS += -laknskinsrv
    LIBS += -lcentralrepository

    load(data_caging_paths)
    TARGET.UID3 = 0x20065DBA
    TARGET.EPOCALLOWDLLDATA = 1
    addFiles.sources = situationsactiontheme.dll
    addFiles.path = $$QT_PLUGINS_BASE_DIR
    DEPLOYMENT += addFiles
    target.path += $$[QT_INSTALL_PLUGINS]
    INSTALLS += target
}

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../../bin/ -lsituationscommon
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../../bin/ -lsituationscommon
else:symbian: LIBS += -lsituationscommon
else:unix: LIBS += -L$$OUT_PWD/../../../bin/ -lsituationscommon

