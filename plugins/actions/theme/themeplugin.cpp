#include "themeplugin.h"
#include "themeaction.h"
#include "themelistmodel.h"
#include <identifiers.h>
#include <qdeclarative.h>

static const char* pluginName("theme");

ThemePlugin::ThemePlugin()
    : QObject()
{
    setProperty(Identifiers::groupId, 150);
    setProperty(Identifiers::groupName, pluginName);
    setProperty(Identifiers::groupIcon, Identifiers::templateGroupIcon.arg(pluginName).arg("personalise"));
    setProperty(Identifiers::groupLocId, Identifiers::templateActionLocId.arg(pluginName));
    setProperty(Identifiers::groupItem, Identifiers::templateActionItem.arg(pluginName));
    setProperty(Identifiers::groupEditor, Identifiers::templateGroupEditor.arg(pluginName));
    setProperty(Identifiers::groupSettings, Identifiers::templateGroupSettings.arg(pluginName));
    setProperty(Identifiers::groupMaxCount, 1);
    setProperty(Identifiers::groupInitToDefault, true);
}

Action* ThemePlugin::createInstance(QObject* parent)
{
    return new ThemeAction(parent);
}

void ThemePlugin::qmlRegisterTypes()
{
    // @uri com.pastillilabs.situations.action.theme
    qmlRegisterType<ThemeAction>(Identifiers::templateActionUri.arg(pluginName).toLatin1(), 1, 0, "ThemeAction");
    qmlRegisterType<ThemeListModel>(Identifiers::templateActionUri.arg(pluginName).toLatin1(), 1, 0, "ThemeListModel");
}

const QVariant ThemePlugin::property(const char* name) const
{
    return QObject::property(name);
}

Q_EXPORT_PLUGIN2(situationsactiontheme, ThemePlugin)

