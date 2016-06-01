#include "application.h"
#include "version.h"
#include <conditionplugin.h>
#include <actionplugin.h>
#include <identifiers.h>
#include <QDeclarativeContext>
#include <QtDeclarative>

static const char* APPLICATION_FILE = "application";
static const char* KEY_ADVANCED_MODE = "advancedMode";

Application::Application(int& argc, char** argv)
    : QApplication(argc, argv)
    , mVersionString(VERSION_STRING)
    , mAdvancedMode(false)
{
    mStorageManager.addStorage(APPLICATION_FILE);

    mTranslationManager.addComponent("common");
    mTranslationManager.addComponent("application");
    mTranslationManager.addComponent("localisation");

    connect(&mClient.pluginManager(), SIGNAL(addCondition(ConditionPlugin&,int,QString)), this, SLOT(addConditionPlugin(ConditionPlugin&,int,QString)));
    connect(&mClient.pluginManager(), SIGNAL(addAction(ActionPlugin&,int,QString)), this, SLOT(addActionPlugin(ActionPlugin&,int,QString)));
    connect(&mClient.pluginManager(), SIGNAL(rmvCondition(ConditionPlugin&,QString)), this, SLOT(rmvConditionPlugin(ConditionPlugin&,QString)));
    connect(&mClient.pluginManager(), SIGNAL(rmvAction(ActionPlugin&,QString)), this, SLOT(rmvActionPlugin(ActionPlugin&,QString)));
}

Application::~Application()
{
}

int Application::exec()
{
    restore();

    // Set root context properties
    mViewer.rootContext()->setContextProperty("application", this);
    mViewer.rootContext()->setContextProperty("client", &mClient);
    mViewer.rootContext()->setContextProperty("situations", &mClient.situationManager());
    mViewer.rootContext()->setContextProperty("translationUpdater", &mTranslationUpdater);
    mViewer.rootContext()->setContextProperty("versionString", mVersionString);

    // Set QML source & show UI
    mViewer.setSource(QUrl("qrc:/qml/application/main.qml"));
    mViewer.showExpanded();

    return QApplication::exec();
}

void Application::setAdvancedMode(bool advancedMode)
{
    if(advancedMode != mAdvancedMode)
    {
        mAdvancedMode = advancedMode;
        emit advancedModeChanged(advancedMode);
        store();
    }
}

bool Application::advancedMode() const
{
    return mAdvancedMode;
}

void Application::addConditionPlugin(ConditionPlugin& plugin, int /*index*/, const QString& name)
{
    mTranslationManager.addComponent(name);
    plugin.qmlRegisterTypes();
}

void Application::addActionPlugin(ActionPlugin& plugin, int /*index*/, const QString& name)
{
    mTranslationManager.addComponent(name);
    plugin.qmlRegisterTypes();
}

void Application::rmvConditionPlugin(ConditionPlugin& /*plugin*/, const QString& name)
{
    mTranslationManager.rmvComponent(name);
}

void Application::rmvActionPlugin(ActionPlugin& /*plugin*/, const QString& name)
{
    mTranslationManager.rmvComponent(name);
}

void Application::store()
{
    QVariantMap data;
    data[KEY_ADVANCED_MODE] = mAdvancedMode;

    mStorageManager.write(APPLICATION_FILE, data);
}

void Application::restore()
{
    QVariantMap data;

    if(mStorageManager.read(APPLICATION_FILE, data))
    {
        setAdvancedMode(data.value(KEY_ADVANCED_MODE).toBool());
    }
}

