include(../../../common/common.pri)

QT += declarative

TARGET = situationsactionlaunch
DESTDIR = ../../../bin
TEMPLATE = lib
CONFIG += plugin

INCLUDEPATH += $$PWD/../../../common
DEPENDPATH += $$PWD/../../../common

HEADERS += \
    launchaction.h \
    launchplugin.h \
    applicationlistmodel.h \
    bookmarklistmodel.h

SOURCES += \
    launchaction.cpp \
    launchplugin.cpp \
    applicationlistmodel.cpp \
    bookmarklistmodel.cpp

RESOURCES += launch.qrc

OTHER_FILES += \
    qml/launch/Action.qml \
    qml/launch/Editor.qml \
    qml/launch/Settings.qml

symbian {
    LIBS += -lapgrfx
    LIBS += -lapparc
    LIBS += -lws32
    LIBS += -lfavouritesengine

    load(data_caging_paths)
    TARGET.UID3 = 0x20065DB6
    TARGET.EPOCALLOWDLLDATA = 1
    addFiles.sources = situationsactionlaunch.dll
    addFiles.path = $$QT_PLUGINS_BASE_DIR
    DEPLOYMENT += addFiles
    target.path += $$[QT_INSTALL_PLUGINS]
    INSTALLS += target
}

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../../bin/ -lsituationscommon
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../../bin/ -lsituationscommon
else:symbian: LIBS += -lsituationscommon
else:unix: LIBS += -L$$OUT_PWD/../../../bin/ -lsituationscommon

