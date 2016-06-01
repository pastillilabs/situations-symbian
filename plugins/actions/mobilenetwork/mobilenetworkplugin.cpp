#include "mobilenetworkplugin.h"
#include "mobilenetworkaction.h"
#include <identifiers.h>
#include <qdeclarative.h>

static const char* pluginName("mobilenetwork");

MobileNetworkPlugin::MobileNetworkPlugin()
    : QObject()
{
    setProperty(Identifiers::groupId, 150);
#ifdef Q_WS_SYMBIAN_UI
    setProperty(Identifiers::groupId, 175);
#endif
    setProperty(Identifiers::groupName, pluginName);
    setProperty(Identifiers::groupIcon, Identifiers::templateGroupIcon.arg(pluginName).arg("reception"));
    setProperty(Identifiers::groupLocId, Identifiers::templateActionLocId.arg(pluginName));
    setProperty(Identifiers::groupItem, Identifiers::templateActionItem.arg(pluginName));
    setProperty(Identifiers::groupEditor, Identifiers::templateGroupEditor.arg(pluginName));
    setProperty(Identifiers::groupSettings, Identifiers::templateGroupSettings.arg(pluginName));
    setProperty(Identifiers::groupMaxCount, 1);
    setProperty(Identifiers::groupInitToDefault, true);
}

Action* MobileNetworkPlugin::createInstance(QObject* parent)
{
    return new MobileNetworkAction(parent);
}

void MobileNetworkPlugin::qmlRegisterTypes()
{
    // @uri com.pastillilabs.situations.action.mobilenetwork
    qmlRegisterType<MobileNetworkAction>(Identifiers::templateActionUri.arg(pluginName).toLatin1(), 1, 0, "MobileNetworkAction");
}

const QVariant MobileNetworkPlugin::property(const char* name) const
{
    return QObject::property(name);
}

Q_EXPORT_PLUGIN2(situationsactionmobilenetwork, MobileNetworkPlugin)

