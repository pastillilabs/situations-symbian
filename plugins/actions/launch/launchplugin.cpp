#include "launchplugin.h"
#include "launchaction.h"
#include "applicationlistmodel.h"
#include "bookmarklistmodel.h"
#include <identifiers.h>
#include <qdeclarative.h>

static const char* pluginName("launch");

LaunchPlugin::LaunchPlugin()
    : QObject()
{
    setProperty(Identifiers::groupId, 700);
    setProperty(Identifiers::groupName, pluginName);
    setProperty(Identifiers::groupIcon, Identifiers::templateGroupIcon.arg(pluginName).arg("applications"));
    setProperty(Identifiers::groupLocId, Identifiers::templateActionLocId.arg(pluginName));
    setProperty(Identifiers::groupItem, Identifiers::templateActionItem.arg(pluginName));
    setProperty(Identifiers::groupEditor, Identifiers::templateGroupEditor.arg(pluginName));
    setProperty(Identifiers::groupSettings, Identifiers::templateGroupSettings.arg(pluginName));
    setProperty(Identifiers::groupMaxCount, 10);
}

Action* LaunchPlugin::createInstance(QObject* parent)
{
    return new LaunchAction(parent);
}

void LaunchPlugin::qmlRegisterTypes()
{
    // @uri com.pastillilabs.situations.action.launch
    qmlRegisterType<LaunchAction>(Identifiers::templateActionUri.arg(pluginName).toLatin1(), 1, 0, "LaunchAction");
    qmlRegisterType<ApplicationListModel>(Identifiers::templateActionUri.arg(pluginName).toLatin1(), 1, 0, "ApplicationListModel");
    qmlRegisterType<BookmarkListModel>(Identifiers::templateActionUri.arg(pluginName).toLatin1(), 1, 0, "BookmarkListModel");
}

const QVariant LaunchPlugin::property(const char* name) const
{
    return QObject::property(name);
}

Q_EXPORT_PLUGIN2(situationsactionlaunch, LaunchPlugin)

