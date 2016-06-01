include(../common/common.pri)

QT += core network
symbian: QT -= gui

TEMPLATE = app
TARGET = situationsserver
DESTDIR = ../bin
CONFIG += console

INCLUDEPATH += $$PWD/../common
DEPENDPATH += $$PWD/../common

HEADERS += \
    server.h \
    confirmationquery.h \
    callmonitor.h

symbian:HEADERS += globalquerysymbian.h
symbian:HEADERS += callmonitorsymbian.h

SOURCES += \
    main.cpp \
    server.cpp \
    confirmationquery.cpp \
    callmonitor.cpp

symbian:SOURCES += globalquerysymbian.cpp
symbian:SOURCES += callmonitorsymbian.cpp

RESOURCES += server.qrc

symbian {
    LIBS += -lavkon
    LIBS += -laknnotify
    LIBS += -letel3rdparty
    LIBS += -lfeatdiscovery

    TARGET.UID3 = 0x20065CC2
    TARGET.EPOCHEAPSIZE = 0x20000 0x2000000

    RSS_RULES += "hidden = KAppIsHidden;"

    autoStartBlock = \
    "SOURCEPATH ." \
    "START RESOURCE autostart.rss" \
    "END"

    MMP_RULES += autoStartBlock

    deployRscFile = "\"$${EPOCROOT}epoc32/data/autostart.rsc\" - \"C:/private/101f875a/import/[20065CC2].rsc\""
    deployFiles.pkg_postrules += deployRscFile
    DEPLOYMENT += deployFiles
}

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../bin/ -lsituationscommon
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../bin/ -lsituationscommon
else:symbian: LIBS += -lsituationscommon
else:unix: LIBS += -L$$OUT_PWD/../bin/ -lsituationscommon

