#include "smsreplyplugin.h"
#include "smsreplyaction.h"
#include <identifiers.h>
#include <qdeclarative.h>

static const char* pluginName("smsreply");

SmsReplyPlugin::SmsReplyPlugin()
    : QObject()
{
    setProperty(Identifiers::groupId, 600);
    setProperty(Identifiers::groupName, pluginName);
    setProperty(Identifiers::groupIcon, Identifiers::templateGroupIcon.arg(pluginName).arg("reply_message"));
    setProperty(Identifiers::groupLocId, Identifiers::templateActionLocId.arg(pluginName));
    setProperty(Identifiers::groupItem, Identifiers::templateActionItem.arg(pluginName));
    setProperty(Identifiers::groupEditor, Identifiers::templateGroupEditor.arg(pluginName));
    setProperty(Identifiers::groupMaxCount, 1);
}

Action* SmsReplyPlugin::createInstance(QObject* parent)
{
    return new SmsReplyAction(parent);
}

void SmsReplyPlugin::qmlRegisterTypes()
{
    // @uri com.pastillilabs.situations.action.smsreply
    qmlRegisterType<SmsReplyAction>(Identifiers::templateActionUri.arg(pluginName).toLatin1(), 1, 0, "SmsReplyAction");
}

const QVariant SmsReplyPlugin::property(const char* name) const
{
    return QObject::property(name);
}

Q_EXPORT_PLUGIN2(situationsactionsmsreply, SmsReplyPlugin)

