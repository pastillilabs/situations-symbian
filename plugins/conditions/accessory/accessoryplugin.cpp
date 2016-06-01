#include "accessoryplugin.h"
#include "accessorycondition.h"
#include <identifiers.h>
#include <qdeclarative.h>

static const char* pluginName("accessory");

AccessoryPlugin::AccessoryPlugin()
    : QObject()
{
    setProperty(Identifiers::groupId, 220);
    setProperty(Identifiers::groupName, pluginName);
    setProperty(Identifiers::groupIcon, Identifiers::templateGroupIcon.arg(pluginName).arg("headphones"));
    setProperty(Identifiers::groupLocId, Identifiers::templateConditionLocId.arg(pluginName));
    setProperty(Identifiers::groupItem, Identifiers::templateConditionItem.arg(pluginName));
    setProperty(Identifiers::groupEditor, Identifiers::templateGroupEditor.arg(pluginName));
    setProperty(Identifiers::groupMaxCount, 4);
}

Condition* AccessoryPlugin::createInstance(QObject* parent)
{
    return new AccessoryCondition(parent);
}

void AccessoryPlugin::qmlRegisterTypes()
{
    // @uri com.pastillilabs.situations.condition.accessory
    qmlRegisterType<AccessoryCondition>(Identifiers::templateConditionUri.arg(pluginName).toLatin1(), 1, 0, "AccessoryCondition");
}

const QVariant AccessoryPlugin::property(const char* name) const
{
    return QObject::property(name);
}

Q_EXPORT_PLUGIN2(situationsconditionaccessory, AccessoryPlugin)

