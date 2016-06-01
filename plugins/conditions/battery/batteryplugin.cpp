#include "batteryplugin.h"
#include "batterycondition.h"
#include <identifiers.h>
#include <qdeclarative.h>

static const char* pluginName("battery");

BatteryPlugin::BatteryPlugin()
    : QObject()
{
    setProperty(Identifiers::groupId, 230);
    setProperty(Identifiers::groupName, pluginName);
    setProperty(Identifiers::groupIcon, Identifiers::templateGroupIcon.arg(pluginName).arg("battery_medium"));
    setProperty(Identifiers::groupLocId, Identifiers::templateConditionLocId.arg(pluginName));
    setProperty(Identifiers::groupItem, Identifiers::templateConditionItem.arg(pluginName));
    setProperty(Identifiers::groupEditor, Identifiers::templateGroupEditor.arg(pluginName));
    setProperty(Identifiers::groupMaxCount, 2);
}

Condition* BatteryPlugin::createInstance(QObject* parent)
{
    return new BatteryCondition(parent);
}

void BatteryPlugin::qmlRegisterTypes()
{
    // @uri com.pastillilabs.situations.condition.battery
    qmlRegisterType<BatteryCondition>(Identifiers::templateConditionUri.arg(pluginName).toLatin1(), 1, 0, "BatteryCondition");
}

const QVariant BatteryPlugin::property(const char* name) const
{
    return QObject::property(name);
}

Q_EXPORT_PLUGIN2(situationsconditionbattery, BatteryPlugin)

