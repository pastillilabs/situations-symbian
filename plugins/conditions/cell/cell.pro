include(../../../common/common.pri)

QT -= gui
QT += declarative

TARGET = situationsconditioncell
DESTDIR = ../../../bin
TEMPLATE = lib
CONFIG += plugin
CONFIG += mobility
MOBILITY += systeminfo

INCLUDEPATH += $$PWD/../../../common
DEPENDPATH += $$PWD/../../../common

HEADERS += \
    cellplugin.h \
    cellcondition.h

SOURCES += \
    cellplugin.cpp \
    cellcondition.cpp

RESOURCES += cell.qrc

OTHER_FILES += \
    qml/cell/Condition.qml \
    qml/cell/Editor.qml

OTHER_FILES +=

symbian {
    load(data_caging_paths)
    TARGET.UID3 = 0x20065DB8
    TARGET.EPOCALLOWDLLDATA = 1
    addFiles.sources = situationsconditioncell.dll
    addFiles.path = $$QT_PLUGINS_BASE_DIR
    DEPLOYMENT += addFiles
    target.path += $$[QT_INSTALL_PLUGINS]
    INSTALLS += target
}

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../../bin/ -lsituationscommon
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../../bin/ -lsituationscommon
else:symbian: LIBS += -lsituationscommon
else:unix: LIBS += -L$$OUT_PWD/../../../bin/ -lsituationscommon

