include(../../../common/common.pri)

QT -= gui
QT += declarative

TARGET = situationsconditionlocation
DESTDIR = ../../../bin
TEMPLATE = lib
CONFIG += plugin
CONFIG += mobility
MOBILITY += location

INCLUDEPATH += $$PWD/../../../common
DEPENDPATH += $$PWD/../../../common

HEADERS += \
    locationplugin.h \
    locationcondition.h \
    locationengine.h \
    locationsource.h

SOURCES += \
    locationplugin.cpp \
    locationcondition.cpp \
    locationengine.cpp \
    locationsource.cpp

RESOURCES += location.qrc

OTHER_FILES += \
    qml/location/Condition.qml \
    qml/location/Editor.qml

OTHER_FILES +=

symbian {
    load(data_caging_paths)
    TARGET.UID3 = 0x20065CC8
    TARGET.EPOCALLOWDLLDATA = 1
    addFiles.sources = situationsconditionlocation.dll
    addFiles.path = $$QT_PLUGINS_BASE_DIR
    DEPLOYMENT += addFiles
    target.path += $$[QT_INSTALL_PLUGINS]
    INSTALLS += target
}

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../../bin/ -lsituationscommon
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../../bin/ -lsituationscommon
else:symbian: LIBS += -lsituationscommon
else:unix: LIBS += -L$$OUT_PWD/../../../bin/ -lsituationscommon

