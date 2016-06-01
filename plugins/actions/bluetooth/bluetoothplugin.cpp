#include "bluetoothplugin.h"
#include "bluetoothaction.h"
#include <identifiers.h>
#include <qdeclarative.h>

static const char* pluginName("bluetooth");

BluetoothPlugin::BluetoothPlugin()
    : QObject()
{
    setProperty(Identifiers::groupId, 400);
    setProperty(Identifiers::groupName, pluginName);
    setProperty(Identifiers::groupIcon, Identifiers::templateGroupIcon.arg(pluginName).arg("bluetooth"));
    setProperty(Identifiers::groupLocId, Identifiers::templateActionLocId.arg(pluginName));
    setProperty(Identifiers::groupItem, Identifiers::templateActionItem.arg(pluginName));
    setProperty(Identifiers::groupSettings, Identifiers::templateGroupSettings.arg(pluginName));
    setProperty(Identifiers::groupMaxCount, 1);
    setProperty(Identifiers::groupInitToDefault, true);
}

Action* BluetoothPlugin::createInstance(QObject* parent)
{
    return new BluetoothAction(parent);
}

void BluetoothPlugin::qmlRegisterTypes()
{
}

const QVariant BluetoothPlugin::property(const char* name) const
{
    return QObject::property(name);
}

Q_EXPORT_PLUGIN2(situationsactionbluetooth, BluetoothPlugin)

