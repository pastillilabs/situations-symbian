#include "wlanplugin.h"
#include "wlanaction.h"
#include <identifiers.h>
#include <qdeclarative.h>

static const char* pluginName("wlan");

WlanPlugin::WlanPlugin()
    : QObject()
{
    setProperty(Identifiers::groupId, 300);
    setProperty(Identifiers::groupName, pluginName);
    setProperty(Identifiers::groupIcon, Identifiers::templateGroupIcon.arg(pluginName).arg("wlan_on"));
    setProperty(Identifiers::groupLocId, Identifiers::templateActionLocId.arg(pluginName));
    setProperty(Identifiers::groupItem, Identifiers::templateActionItem.arg(pluginName));
    setProperty(Identifiers::groupSettings, Identifiers::templateGroupSettings.arg(pluginName));
    setProperty(Identifiers::groupMaxCount, 1);
    setProperty(Identifiers::groupInitToDefault, true);
}

Action* WlanPlugin::createInstance(QObject* parent)
{
    return new WlanAction(parent);
}

void WlanPlugin::qmlRegisterTypes()
{
}

const QVariant WlanPlugin::property(const char* name) const
{
    return QObject::property(name);
}

Q_EXPORT_PLUGIN2(situationsactionwlan, WlanPlugin)

