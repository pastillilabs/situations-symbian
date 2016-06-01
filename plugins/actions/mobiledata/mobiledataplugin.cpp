#include "mobiledataplugin.h"
#include "mobiledataaction.h"
#include <identifiers.h>
#include <qdeclarative.h>

static const char* pluginName("mobiledata");

MobileDataPlugin::MobileDataPlugin()
    : QObject()
{
    setProperty(Identifiers::groupId, 200);
    setProperty(Identifiers::groupName, pluginName);
    setProperty(Identifiers::groupIcon, Identifiers::templateGroupIcon.arg(pluginName).arg("connectivity"));
    setProperty(Identifiers::groupLocId, Identifiers::templateActionLocId.arg(pluginName));
    setProperty(Identifiers::groupItem, Identifiers::templateActionItem.arg(pluginName));
    setProperty(Identifiers::groupSettings, Identifiers::templateGroupSettings.arg(pluginName));
    setProperty(Identifiers::groupMaxCount, 1);
    setProperty(Identifiers::groupInitToDefault, true);
}

Action* MobileDataPlugin::createInstance(QObject* parent)
{
    return new MobileDataAction(parent);
}

void MobileDataPlugin::qmlRegisterTypes()
{
}

const QVariant MobileDataPlugin::property(const char* name) const
{
    return QObject::property(name);
}

Q_EXPORT_PLUGIN2(situationsactionmobiledata, MobileDataPlugin)

