CONFIG += symbianui # Define manually to get Symbian UI in simulator

symbian {
    INCLUDEPATH += $$EPOCROOT/epoc32/include/platform/
    INCLUDEPATH += $$EPOCROOT/epoc32/include/platform/mw/
    INCLUDEPATH += $$EPOCROOT/epoc32/include/mw/
    INCLUDEPATH += $$EPOCROOT/epoc32/include/app/
    INCLUDEPATH += $$EPOCROOT/epoc32/include/gcc_mingw/

    CONFIG += symbianui
    TARGET.CAPABILITY += ALL -TCB -DRM -AllFiles -CommDD -DiskAdmin -MultimediaDD -NetworkControl
}

symbianui {
    DEFINES += Q_WS_SYMBIAN_UI
}

