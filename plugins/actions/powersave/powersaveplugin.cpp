#include "powersaveplugin.h"
#include "powersaveaction.h"
#include <identifiers.h>
#include <qdeclarative.h>

static const char* pluginName("powersave");

PowersavePlugin::PowersavePlugin()
    : QObject()
{
    setProperty(Identifiers::groupId, 500);
    setProperty(Identifiers::groupName, pluginName);
    setProperty(Identifiers::groupIcon, Identifiers::templateGroupIcon.arg(pluginName).arg("battery_full"));
    setProperty(Identifiers::groupLocId, Identifiers::templateActionLocId.arg(pluginName));
    setProperty(Identifiers::groupItem, Identifiers::templateActionItem.arg(pluginName));
    setProperty(Identifiers::groupSettings, Identifiers::templateGroupSettings.arg(pluginName));
    setProperty(Identifiers::groupMaxCount, 1);
    setProperty(Identifiers::groupInitToDefault, true);
}

Action* PowersavePlugin::createInstance(QObject* parent)
{
    return new PowersaveAction(parent);
}

void PowersavePlugin::qmlRegisterTypes()
{
}

const QVariant PowersavePlugin::property(const char* name) const
{
    return QObject::property(name);
}

Q_EXPORT_PLUGIN2(situationsactionpowersave, PowersavePlugin)

