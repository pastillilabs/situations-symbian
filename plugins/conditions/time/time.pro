include(../../../common/common.pri)

QT -= gui
QT += declarative

TARGET = situationsconditiontime
DESTDIR = ../../../bin
TEMPLATE = lib
CONFIG += plugin

INCLUDEPATH += $$PWD/../../../common
DEPENDPATH += $$PWD/../../../common

HEADERS += \
    timeplugin.h \
    timecondition.h

SOURCES += \
    timeplugin.cpp \
    timecondition.cpp

RESOURCES += time.qrc

OTHER_FILES += \
    qml/time/Condition.qml \
    qml/time/Editor.qml \
    qml/time/Picker.qml \
    qml/time/TimePicker.qml \
    qml/time/TimePeriodPicker.qml

symbian {
    load(data_caging_paths)
    TARGET.UID3 = 0x20065CC5
    TARGET.EPOCALLOWDLLDATA = 1
    addFiles.sources = situationsconditiontime.dll
    addFiles.path = $$QT_PLUGINS_BASE_DIR
    DEPLOYMENT += addFiles
    target.path += $$[QT_INSTALL_PLUGINS]
    INSTALLS += target
}

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../../bin/ -lsituationscommon
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../../bin/ -lsituationscommon
else:symbian: LIBS += -lsituationscommon
else:unix: LIBS += -L$$OUT_PWD/../../../bin/ -lsituationscommon

