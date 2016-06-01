include(../../../common/common.pri)

QT -= gui
QT += declarative

TARGET = situationsactionmobiledata
DESTDIR = ../../../bin
TEMPLATE = lib
CONFIG += plugin

INCLUDEPATH += $$PWD/../../../common
DEPENDPATH += $$PWD/../../../common

HEADERS += \
    mobiledataplugin.h \
    mobiledataaction.h

SOURCES += \
    mobiledataplugin.cpp \
    mobiledataaction.cpp

RESOURCES += \
    mobiledata.qrc

OTHER_FILES += \
    qml/mobiledata/Action.qml \
    qml/mobiledata/Settings.qml

symbian {
    load(data_caging_paths)
    TARGET.UID3 = 0x20065CCA
    TARGET.EPOCALLOWDLLDATA = 1
    addFiles.sources = situationsactionmobiledata.dll
    addFiles.path = $$QT_PLUGINS_BASE_DIR
    DEPLOYMENT += addFiles
    target.path += $$[QT_INSTALL_PLUGINS]
    INSTALLS += target
}

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../../bin/ -lsituationscommon
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../../bin/ -lsituationscommon
else:symbian: LIBS += -lsituationscommon
else:unix: LIBS += -L$$OUT_PWD/../../../bin/ -lsituationscommon

