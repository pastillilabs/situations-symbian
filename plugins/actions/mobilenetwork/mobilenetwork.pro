include(../../../common/common.pri)

QT -= gui
QT += declarative

TARGET = situationsactionmobilenetwork
DESTDIR = ../../../bin
TEMPLATE = lib
CONFIG += plugin
CONFIG += cellular-qt

INCLUDEPATH += $$PWD/../../../common
DEPENDPATH += $$PWD/../../../common

HEADERS += \
    mobilenetworkplugin.h \
    mobilenetworkaction.h

SOURCES += \
    mobilenetworkplugin.cpp \
    mobilenetworkaction.cpp

RESOURCES += mobilenetwork.qrc

OTHER_FILES += \
    qml/mobilenetwork/Action.qml \
    qml/mobilenetwork/Editor.qml \
    qml/mobilenetwork/Settings.qml

symbian {
    load(data_caging_paths)
    TARGET.UID3 = 0x2006F085
    TARGET.EPOCALLOWDLLDATA = 1
    addFiles.sources = situationsactionmobilenetwork.dll
    addFiles.path = $$QT_PLUGINS_BASE_DIR
    DEPLOYMENT += addFiles
    target.path += $$[QT_INSTALL_PLUGINS]
    INSTALLS += target
}

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../../bin/ -lsituationscommon
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../../bin/ -lsituationscommon
else:symbian: LIBS += -lsituationscommon
else:unix: LIBS += -L$$OUT_PWD/../../../bin/ -lsituationscommon

