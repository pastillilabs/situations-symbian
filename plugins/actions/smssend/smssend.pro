include(../../../common/common.pri)

QT -= gui
QT += declarative

TARGET = situationsactionsmssend
DESTDIR = ../../../bin
TEMPLATE = lib
CONFIG += plugin
CONFIG += mobility
MOBILITY += contacts messaging

INCLUDEPATH += $$PWD/../../../common
DEPENDPATH += $$PWD/../../../common

HEADERS += \
    smssendplugin.h \
    smssendaction.h \
    contactlistmodel.h \
    smssender.h

SOURCES += \
    smssendplugin.cpp \
    smssendaction.cpp \
    contactlistmodel.cpp \
    smssender.cpp

RESOURCES += smssend.qrc

OTHER_FILES += \
    qml/smssend/Action.qml \
    qml/smssend/Editor.qml \
    qml/smssend/Settings.qml \
    qml/smssend/ContactDialog.qml

symbian {
    load(data_caging_paths)
    TARGET.UID3 = 0x20065DB7
    TARGET.EPOCALLOWDLLDATA = 1
    addFiles.sources = situationsactionsmssend.dll
    addFiles.path = $$QT_PLUGINS_BASE_DIR
    DEPLOYMENT += addFiles
    target.path += $$[QT_INSTALL_PLUGINS]
    INSTALLS += target
}

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../../bin/ -lsituationscommon
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../../bin/ -lsituationscommon
else:symbian: LIBS += -lsituationscommon
else:unix: LIBS += -L$$OUT_PWD/../../../bin/ -lsituationscommon

