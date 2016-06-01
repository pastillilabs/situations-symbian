include(../../../common/common.pri)

QT -= gui
QT += declarative

TARGET = situationsactionsmsreply
DESTDIR = ../../../bin
TEMPLATE = lib
CONFIG += plugin
CONFIG += mobility
MOBILITY += contacts messaging

INCLUDEPATH += $$PWD/../../../common
DEPENDPATH += $$PWD/../../../common

HEADERS += \
    smsreplyplugin.h \
    smsreplyaction.h \
    smssender.h

symbian: HEADERS += missedcallssymbian.h

SOURCES += \
    smsreplyplugin.cpp \
    smsreplyaction.cpp \
    smssender.cpp

symbian: SOURCES += missedcallssymbian.cpp

RESOURCES += smsreply.qrc

OTHER_FILES += \
    qml/smsreply/Action.qml \
    qml/smsreply/Editor.qml

symbian {
    LIBS += -lefsrv
    LIBS += -llogcli

    load(data_caging_paths)
    TARGET.UID3 = 0x20065DB5
    TARGET.EPOCALLOWDLLDATA = 1
    addFiles.sources = situationsactionsmsreply.dll
    addFiles.path = $$QT_PLUGINS_BASE_DIR
    DEPLOYMENT += addFiles
    target.path += $$[QT_INSTALL_PLUGINS]
    INSTALLS += target
}

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../../bin/ -lsituationscommon
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../../bin/ -lsituationscommon
else:symbian: LIBS += -lsituationscommon
else:unix: LIBS += -L$$OUT_PWD/../../../bin/ -lsituationscommon

