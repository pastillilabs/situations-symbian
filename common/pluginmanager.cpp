#include "pluginmanager.h"
#include "translationmanager.h"
#include "conditionplugin.h"
#include "actionplugin.h"
#include "identifiers.h"
#include <QDir>
#include <QPluginLoader>
#include <QCoreApplication>
#include <QLibraryInfo>
#include <QVariant>

static const QString CONDITION("situationscondition%1");
static const QString ACTION("situationsaction%1");

PluginManager::PluginManager(QObject* parent)
    : QObject(parent)
{
}

void PluginManager::operator<<(const QVariantMap& in)
{
    const QVariantList conditions(in[Identifiers::keyConditions].toList());
    const QVariantList actions(in[Identifiers::keyActions].toList());

    foreach(const QVariant& condition, conditions) addCondition(condition.toString());
    foreach(const QVariant& action, actions) addAction(action.toString());
}

void PluginManager::operator>>(QVariantMap& out) const
{
    QVariantList conditions;
    QVariantList actions;
    foreach(const ConditionPlugin* plugin, mConditionPlugins) conditions.append(plugin->property(Identifiers::groupName).toString());
    foreach(const ActionPlugin* plugin, mActionPlugins) actions.append(plugin->property(Identifiers::groupName).toString());

    out[Identifiers::keyConditions] = conditions;
    out[Identifiers::keyActions] = actions;
}

int PluginManager::addCondition(const QString& name)
{
    int index(conditionIndex(name));
    if(index == -1)
    {
        ConditionPlugin* plugin(loadCondition(name));
        if(plugin)
        {
            index = 0;

            // Get correct index for the plugin
            const int conditionId(plugin->property(Identifiers::groupId).toInt());
            while(index < mConditionPlugins.count() && mConditionPlugins.at(index)->property(Identifiers::groupId).toInt() <= conditionId) ++index;
            mConditionPlugins.insert(index, plugin);
            emit addCondition(*plugin, index, name);
        }
    }

    return index;
}

int PluginManager::addAction(const QString& name)
{
    int index(actionIndex(name));
    if(index == -1)
    {
        ActionPlugin* plugin(loadAction(name));
        if(plugin)
        {
            index = 0;

            // Get correct index for the plugin
            const int actionId(plugin->property(Identifiers::groupId).toInt());
            while(index < mActionPlugins.count() && mActionPlugins.at(index)->property(Identifiers::groupId).toInt() <= actionId) ++index;
            mActionPlugins.insert(index, plugin);
            emit addAction(*plugin, index, name);
        }
    }

    return index;
}

int PluginManager::rmvCondition(const QString& name)
{
    const int index(conditionIndex(name));
    if(index > -1)
    {
        ConditionPlugin* plugin(mConditionPlugins.takeAt(index));
        emit rmvCondition(*plugin, name);

        delete plugin;
    }

    return index;
}

int PluginManager::rmvAction(const QString& name)
{
    const int index(actionIndex(name));
    if(index > -1)
    {
        ActionPlugin* plugin(mActionPlugins.takeAt(index));
        emit rmvAction(*plugin, name);

        delete plugin;
    }

    return index;
}

const QStringList PluginManager::conditions() const
{
    QStringList conditions;
    foreach(const ConditionPlugin* plugin, mConditionPlugins) conditions.append(plugin->property(Identifiers::groupName).toString());
    return conditions;
}

const QStringList PluginManager::actions() const
{
    QStringList actions;
    foreach(const ActionPlugin* plugin, mActionPlugins) actions.append(plugin->property(Identifiers::groupName).toString());
    return actions;
}

const QList<ConditionPlugin*>& PluginManager::conditionPlugins() const
{
    return mConditionPlugins;
}

const QList<ActionPlugin*>& PluginManager::actionPlugins() const
{
    return mActionPlugins;
}

ConditionPlugin* PluginManager::loadCondition(const QString& name)
{
    ConditionPlugin* plugin(qobject_cast<ConditionPlugin*>(loadPlugin(CONDITION.arg(name))));
    return plugin;
}

ActionPlugin* PluginManager::loadAction(const QString& name)
{
    ActionPlugin* plugin(qobject_cast<ActionPlugin*>(loadPlugin(ACTION.arg(name))));
    return plugin;
}

QObject* PluginManager::loadPlugin(const QString& name)
{
#ifdef Q_OS_SYMBIAN
    QDir pluginsDir(QLibraryInfo::location(QLibraryInfo::PluginsPath));
    QPluginLoader loader(pluginsDir.absoluteFilePath(QString("%1.dll").arg(name)));
#endif

#ifdef Q_WS_SIMULATOR
    QDir pluginsDir(QCoreApplication::applicationDirPath());
    QPluginLoader loader(pluginsDir.absoluteFilePath(QString("%1.dll").arg(name)));
#endif

    return loader.instance();
}

int PluginManager::conditionIndex(const QString& name) const
{
    int index(-1);
    for(int i(0); i < mConditionPlugins.count() && index == -1; ++i)
    {
        if(mConditionPlugins.at(i)->property(Identifiers::groupName).toString() == name) index = i;
    }

    return index;
}

int PluginManager::actionIndex(const QString& name) const
{
    int index(-1);
    for(int i(0); i < mActionPlugins.count() && index == -1; ++i)
    {
        if(mActionPlugins.at(i)->property(Identifiers::groupName).toString() == name) index = i;
    }

    return index;
}

