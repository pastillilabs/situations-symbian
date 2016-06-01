include(../../../common/common.pri)

QT -= gui
QT += declarative

TARGET = situationsconditioncalendar
DESTDIR = ../../../bin
TEMPLATE = lib
CONFIG += plugin
CONFIG += mobility
MOBILITY += organizer

INCLUDEPATH += $$PWD/../../../common
DEPENDPATH += $$PWD/../../../common

SOURCES += \
    calendarplugin.cpp \
    calendarcondition.cpp \
    calendarengine.cpp \
    calendarcollections.cpp

HEADERS += \
    calendarplugin.h \
    calendarcondition.h \
    calendarengine.h \
    calendarcollections.h

RESOURCES += calendar.qrc

OTHER_FILES += \
    qml/calendar/Condition.qml \
    qml/calendar/Editor.qml

OTHER_FILES +=

symbian {
    load(data_caging_paths)
    TARGET.UID3 = 0x20065CC6
    TARGET.EPOCALLOWDLLDATA = 1
    addFiles.sources = situationsconditioncalendar.dll
    addFiles.path = $$QT_PLUGINS_BASE_DIR
    DEPLOYMENT += addFiles
    target.path += $$[QT_INSTALL_PLUGINS]
    INSTALLS += target
}

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../../bin/ -lsituationscommon
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../../bin/ -lsituationscommon
else:symbian: LIBS += -lsituationscommon
else:unix: LIBS += -L$$OUT_PWD/../../../bin/ -lsituationscommon

