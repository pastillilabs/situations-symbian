#include "smssendplugin.h"
#include "smssendaction.h"
#include "contactlistmodel.h"
#include <identifiers.h>
#include <qdeclarative.h>

static const char* pluginName("smssend");

SmsSendPlugin::SmsSendPlugin()
    : QObject()
{
    setProperty(Identifiers::groupId, 650);
    setProperty(Identifiers::groupName, pluginName);
    setProperty(Identifiers::groupIcon, Identifiers::templateGroupIcon.arg(pluginName).arg("create_message"));
    setProperty(Identifiers::groupLocId, "action_sms_send_name");
    setProperty(Identifiers::groupItem, Identifiers::templateActionItem.arg(pluginName));
    setProperty(Identifiers::groupEditor, Identifiers::templateGroupEditor.arg(pluginName));
    setProperty(Identifiers::groupSettings, Identifiers::templateGroupSettings.arg(pluginName));
    setProperty(Identifiers::groupMaxCount, 10);
}

Action* SmsSendPlugin::createInstance(QObject* parent)
{
    return new SmsSendAction(parent);
}

void SmsSendPlugin::qmlRegisterTypes()
{
    // @uri com.pastillilabs.situations.action.smssend
    qmlRegisterType<SmsSendAction>(Identifiers::templateActionUri.arg(pluginName).toLatin1(), 1, 0, "SmsSendAction");
    qmlRegisterType<ContactListModel>(Identifiers::templateActionUri.arg(pluginName).toLatin1(), 1, 0, "ContactListModel");
}

const QVariant SmsSendPlugin::property(const char* name) const
{
    return QObject::property(name);
}

Q_EXPORT_PLUGIN2(situationsactionsmssend, SmsSendPlugin)

