include(../common/common.pri)

# Add more folders to ship with the application, here
#folder_01.source = qml/application
#folder_01.target = qml
#DEPLOYMENTFOLDERS = folder_01

# Additional import path used to resolve QML modules in Creator's code model
#QML_IMPORT_PATH =

QT += network

TARGET = situationsapplication
DESTDIR = ../bin

symbian {
    LIBS += -lezip -lbafl -lefsrv -leuser

    TARGET.UID3 = 0x20065CC4
    TARGET.EPOCHEAPSIZE = 0x20000 0x2000000

    DEPLOYMENT.display_name = "Situations"
    ICON = gfx/application/situations_symbian.svg
}

# Smart Installer package's UID
# This UID is from the protected range and therefore the package will
# fail to install if self-signed. By default qmake uses the unprotected
# range value if unprotected UID is defined for the application and
# 0x2002CCCF value if protected UID is given to the application
#symbian:DEPLOYMENT.installer_header = 0x2002CCCF

# If your application uses the Qt Mobility libraries, uncomment the following
# lines and add the respective components to the MOBILITY variable.
CONFIG += mobility

# Add dependency to Symbian components
CONFIG += qt-components
CONFIG += localize_deployment

INCLUDEPATH += $$PWD/../common
DEPENDPATH += $$PWD/../common

HEADERS += \
    application.h \
    translationupdater.h \
    version.h \
    client.h

SOURCES += \
    main.cpp \
    application.cpp \
    translationupdater.cpp \
    client.cpp

RESOURCES += application.qrc

TRANSLATIONS += \
    ts/Application/application_en.ts \
    ts/Application/application_fi.ts

OTHER_FILES += \
    qml/application/main.qml \
    qml/application/MainHeading.qml \
    qml/application/SettingsHeading.qml \
    qml/application/SituationListItem.qml \
    qml/application/SituationListItemEdit.qml \
    qml/application/MainPage.qml \
    qml/application/MainMenu.qml \
    qml/application/SettingsPage.qml \
    qml/application/SituationSettingsDialog.qml \
    qml/application/ConditionGroup.qml \
    qml/application/ActionGroup.qml \
    qml/application/GroupFooter.qml \
    qml/application/GroupHeader.qml \
    qml/application/TabL.qml \
    qml/application/TabR.qml \
    qml/application/AboutDialog.qml \
    qml/application/NewSituationDialog.qml \
    qml/application/TranslationUpdateDialog.qml \
    qml/application/PluginItem.qml \
    qml/application/PluginSwitchItem.qml \
    qml/application/PluginTextItem.qml \
    qml/application/DialogLoader.qml \
    qml/application/ActionSettings.qml \
    qml/application/ActionsPage.qml \
    qml/application/ConditionsPage.qml


# Please do not modify the following two lines. Required for deployment.
include(qmlapplicationviewer/qmlapplicationviewer.pri)
qtcAddDeployment()

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../bin/ -lsituationscommon
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../bin/ -lsituationscommon
else:symbian: LIBS += -lsituationscommon
else:unix: LIBS += -L$$OUT_PWD/../bin/ -lsituationscommon

