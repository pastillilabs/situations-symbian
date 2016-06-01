#include "brightnessplugin.h"
#include "brightnessaction.h"
#include <identifiers.h>
#include <qdeclarative.h>

static const char* pluginName("brightness");

BrightnessPlugin::BrightnessPlugin()
    : QObject()
{
    setProperty(Identifiers::groupId, 550);
    setProperty(Identifiers::groupName, pluginName);
    setProperty(Identifiers::groupIcon, Identifiers::templateGroupIcon.arg(pluginName).arg(pluginName));
    setProperty(Identifiers::groupLocId, Identifiers::templateActionLocId.arg(pluginName));
    setProperty(Identifiers::groupItem, Identifiers::templateActionItem.arg(pluginName));
    setProperty(Identifiers::groupSettings, Identifiers::templateGroupSettings.arg(pluginName));
    setProperty(Identifiers::groupMaxCount, 1);
    setProperty(Identifiers::groupInitToDefault, true);
}

Action* BrightnessPlugin::createInstance(QObject* parent)
{
    return new BrightnessAction(parent);
}

void BrightnessPlugin::qmlRegisterTypes()
{
}

const QVariant BrightnessPlugin::property(const char* name) const
{
    return QObject::property(name);
}

Q_EXPORT_PLUGIN2(situationsactionbrightness, BrightnessPlugin)

