#include "locationplugin.h"
#include "locationcondition.h"
#include "locationsource.h"
#include <identifiers.h>
#include <qdeclarative.h>

static const char* pluginName("location");

LocationPlugin::LocationPlugin()
    : QObject()
{
    setProperty(Identifiers::groupId, 400);
    setProperty(Identifiers::groupName, pluginName);
    setProperty(Identifiers::groupIcon, Identifiers::templateGroupIcon.arg(pluginName).arg("map"));
    setProperty(Identifiers::groupLocId, Identifiers::templateConditionLocId.arg(pluginName));
    setProperty(Identifiers::groupItem, Identifiers::templateConditionItem.arg(pluginName));
    setProperty(Identifiers::groupEditor, Identifiers::templateGroupEditor.arg(pluginName));
    setProperty(Identifiers::groupMaxCount, 10);
}

Condition* LocationPlugin::createInstance(QObject* parent)
{
    return new LocationCondition(parent);
}

void LocationPlugin::qmlRegisterTypes()
{
    // @uri com.pastillilabs.situations.condition.location
    qmlRegisterType<LocationCondition>(Identifiers::templateConditionUri.arg(pluginName).toLatin1(), 1, 0, "LocationCondition");
    qmlRegisterType<LocationSource>(Identifiers::templateConditionUri.arg(pluginName).toLatin1(), 1, 0, "LocationSource");
}

const QVariant LocationPlugin::property(const char* name) const
{
    return QObject::property(name);
}

Q_EXPORT_PLUGIN2(situationsconditionlocation, LocationPlugin)

