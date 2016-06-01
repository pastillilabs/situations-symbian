#include "flightmodeplugin.h"
#include "flightmodeaction.h"
#include <identifiers.h>
#include <qdeclarative.h>

static const char* pluginName("flightmode");

FlightmodePlugin::FlightmodePlugin()
    : QObject()
{
    setProperty(Identifiers::groupId, 300);
    setProperty(Identifiers::groupName, pluginName);
    setProperty(Identifiers::groupIcon, Identifiers::templateGroupIcon.arg(pluginName).arg("no_connection"));
    setProperty(Identifiers::groupLocId, "action_flightmode");
    setProperty(Identifiers::groupItem, Identifiers::templateActionItem.arg(pluginName));
    setProperty(Identifiers::groupSettings, Identifiers::templateGroupSettings.arg(pluginName));
    setProperty(Identifiers::groupMaxCount, 1);
    setProperty(Identifiers::groupInitToDefault, true);
}

Action* FlightmodePlugin::createInstance(QObject* parent)
{
    return new FlightmodeAction(parent);
}

void FlightmodePlugin::qmlRegisterTypes()
{
}

const QVariant FlightmodePlugin::property(const char* name) const
{
    return QObject::property(name);
}

Q_EXPORT_PLUGIN2(situationsactionflightmode, FlightmodePlugin)

