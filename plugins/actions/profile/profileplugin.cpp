#include "profileplugin.h"
#include "profileaction.h"
#include "profilelistmodel.h"
#include <identifiers.h>
#include <qdeclarative.h>

static const char* pluginName("profile");

ProfilePlugin::ProfilePlugin()
    : QObject()
{
    setProperty(Identifiers::groupId, 100);
    setProperty(Identifiers::groupName, pluginName);
    setProperty(Identifiers::groupIcon, Identifiers::templateGroupIcon.arg(pluginName).arg("profiles"));
    setProperty(Identifiers::groupLocId, "action_profile");
    setProperty(Identifiers::groupItem, Identifiers::templateActionItem.arg(pluginName));
    setProperty(Identifiers::groupEditor, Identifiers::templateGroupEditor.arg(pluginName));
    setProperty(Identifiers::groupSettings, Identifiers::templateGroupSettings.arg(pluginName));
    setProperty(Identifiers::groupMaxCount, 1);
    setProperty(Identifiers::groupInitToDefault, true);
}

Action* ProfilePlugin::createInstance(QObject* parent)
{
    return new ProfileAction(parent);
}

void ProfilePlugin::qmlRegisterTypes()
{
    // @uri com.pastillilabs.situations.action.profile
    qmlRegisterType<ProfileAction>(Identifiers::templateActionUri.arg(pluginName).toLatin1(), 1, 0, "ProfileAction");
#ifdef Q_WS_SYMBIAN_UI
    qmlRegisterType<ProfileListModel>(Identifiers::templateActionUri.arg(pluginName).toLatin1(), 1, 0, "ProfileListModel");
#endif
}

const QVariant ProfilePlugin::property(const char* name) const
{
    return QObject::property(name);
}

Q_EXPORT_PLUGIN2(situationsactionprofile, ProfilePlugin)

