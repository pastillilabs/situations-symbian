include(../../../common/common.pri)

QT += declarative

TARGET = situationsactionclose
DESTDIR = ../../../bin
TEMPLATE = lib
CONFIG += plugin

INCLUDEPATH += $$PWD/../../../common
DEPENDPATH += $$PWD/../../../common

HEADERS += \
    closeaction.h \
    closeplugin.h \
    applicationlistmodel.h

SOURCES += \
    closeaction.cpp \
    closeplugin.cpp \
    applicationlistmodel.cpp

RESOURCES += close.qrc

OTHER_FILES += \
    qml/close/Action.qml \
    qml/close/Editor.qml \
    qml/close/Settings.qml

symbian {
    LIBS += -lapgrfx
    LIBS += -lapparc
    LIBS += -lws32

    load(data_caging_paths)
    TARGET.UID3 = 0x20065DBC
    TARGET.EPOCALLOWDLLDATA = 1
    addFiles.sources = situationsactionclose.dll
    addFiles.path = $$QT_PLUGINS_BASE_DIR
    DEPLOYMENT += addFiles
    target.path += $$[QT_INSTALL_PLUGINS]
    INSTALLS += target
}

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../../bin/ -lsituationscommon
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../../bin/ -lsituationscommon
else:symbian: LIBS += -lsituationscommon
else:unix: LIBS += -L$$OUT_PWD/../../../bin/ -lsituationscommon

