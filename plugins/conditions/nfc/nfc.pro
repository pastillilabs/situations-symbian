include(../../../common/common.pri)

QT -= gui
QT += declarative

TARGET = situationsconditionnfc
DESTDIR = ../../../bin
TEMPLATE = lib
CONFIG += plugin
CONFIG += mobility
MOBILITY += connectivity

INCLUDEPATH += $$PWD/../../../common
DEPENDPATH += $$PWD/../../../common

HEADERS += \
    nfcplugin.h \
    nfccondition.h \
    nfcscanner.h \
    nfcengine.h

SOURCES += \
    nfcplugin.cpp \
    nfccondition.cpp \
    nfcscanner.cpp \
    nfcengine.cpp

RESOURCES += nfc.qrc

OTHER_FILES += \
    qml/nfc/Condition.qml \
    qml/nfc/Editor.qml

symbian {
    LIBS += -lcone

    load(data_caging_paths)
    TARGET.UID3 = 0x2006F086
    TARGET.EPOCALLOWDLLDATA = 1
    addFiles.sources = situationsconditionnfc.dll
    addFiles.path = $$QT_PLUGINS_BASE_DIR
    DEPLOYMENT += addFiles
    target.path += $$[QT_INSTALL_PLUGINS]
    INSTALLS += target
}

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../../bin/ -lsituationscommon
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../../bin/ -lsituationscommon
else:symbian: LIBS += -lsituationscommon
else:unix: LIBS += -L$$OUT_PWD/../../../bin/ -lsituationscommon

