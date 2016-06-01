include(../../../common/common.pri)

QT -= gui
QT += declarative

TARGET = situationsactionprofile
DESTDIR = ../../../bin
TEMPLATE = lib
CONFIG += plugin
CONFIG += mobility
MOBILITY += systeminfo

INCLUDEPATH += $$PWD/../../../common
DEPENDPATH += $$PWD/../../../common

HEADERS += \
    profileplugin.h \
    profileaction.h

symbianui:HEADERS += profilelistmodel.h

SOURCES += \
    profileplugin.cpp \
    profileaction.cpp

symbianui:SOURCES += profilelistmodel.cpp

RESOURCES += profile.qrc

OTHER_FILES += \
    qml/profile/Action.qml \
    qml/profile/Editor.qml \
    qml/profile/Settings.qml

symbian {
    LIBS += -lProfileEngine

    load(data_caging_paths)
    TARGET.UID3 = 0x20065CC9
    TARGET.EPOCALLOWDLLDATA = 1
    addFiles.sources = situationsactionprofile.dll
    addFiles.path = $$QT_PLUGINS_BASE_DIR
    DEPLOYMENT += addFiles
    target.path += $$[QT_INSTALL_PLUGINS]
    INSTALLS += target
}

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../../bin/ -lsituationscommon
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../../bin/ -lsituationscommon
else:symbian: LIBS += -lsituationscommon
else:unix: LIBS += -L$$OUT_PWD/../../../bin/ -lsituationscommon

