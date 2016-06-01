include(../../../common/common.pri)

QT -= gui
QT += declarative

TARGET = situationsactionbrightness
DESTDIR = ../../../bin
TEMPLATE = lib
CONFIG += plugin

INCLUDEPATH += $$PWD/../../../common
DEPENDPATH += $$PWD/../../../common

HEADERS += \
    brightnessaction.h \
    brightnessplugin.h

SOURCES += \
    brightnessaction.cpp \
    brightnessplugin.cpp

RESOURCES += brightness.qrc

OTHER_FILES += \
    qml/brightness/Action.qml \
    qml/brightness/Settings.qml

symbian {
    LIBS += -lcentralrepository

    load(data_caging_paths)
    TARGET.UID3 = 0x2006F082
    TARGET.EPOCALLOWDLLDATA = 1
    addFiles.sources = situationsactionbrightness.dll
    addFiles.path = $$QT_PLUGINS_BASE_DIR
    DEPLOYMENT += addFiles
    target.path += $$[QT_INSTALL_PLUGINS]
    INSTALLS += target
}

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../../bin/ -lsituationscommon
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../../bin/ -lsituationscommon
else:symbian: LIBS += -lsituationscommon
else:unix: LIBS += -L$$OUT_PWD/../../../bin/ -lsituationscommon

