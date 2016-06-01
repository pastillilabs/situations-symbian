#include "connectivityplugin.h"
#include "connectivitycondition.h"
#include "wlanscanner.h"
#include "btscanner.h"
#include <identifiers.h>
#include <qdeclarative.h>

static const char* pluginName("connectivity");

ConnectivityPlugin::ConnectivityPlugin()
    : QObject()
    , ConditionPlugin()
{
    setProperty(Identifiers::groupId, 300);
    setProperty(Identifiers::groupName, pluginName);
    setProperty(Identifiers::groupIcon, Identifiers::templateGroupIcon.arg(pluginName).arg("connectivity"));
    setProperty(Identifiers::groupLocId, Identifiers::templateConditionLocId.arg(pluginName));
    setProperty(Identifiers::groupItem, Identifiers::templateConditionItem.arg(pluginName));
    setProperty(Identifiers::groupEditor, Identifiers::templateGroupEditor.arg(pluginName));
    setProperty(Identifiers::groupMaxCount, 10);
}

Condition* ConnectivityPlugin::createInstance(QObject* parent)
{
    return new ConnectivityCondition(parent);
}

void ConnectivityPlugin::qmlRegisterTypes()
{
    // @uri com.pastillilabs.situations.condition.connectivity
    qmlRegisterType<ConnectivityCondition>(Identifiers::templateConditionUri.arg(pluginName).toLatin1(), 1, 0, "ConnectivityCondition");
    qmlRegisterType<WlanScanner>(Identifiers::templateConditionUri.arg(pluginName).toLatin1(), 1, 0, "WlanScanner");
    qmlRegisterType<BtScanner>(Identifiers::templateConditionUri.arg(pluginName).toLatin1(), 1, 0, "BtScanner");
}

const QVariant ConnectivityPlugin::property(const char* name) const
{
    return QObject::property(name);
}

Q_EXPORT_PLUGIN2(situationsconditionconnectivity, ConnectivityPlugin)

