#include "closeplugin.h"
#include "closeaction.h"
#include "applicationlistmodel.h"
#include <identifiers.h>
#include <qdeclarative.h>

static const char* pluginName("close");

ClosePlugin::ClosePlugin()
    : QObject()
{
    setProperty(Identifiers::groupId, 800);
    setProperty(Identifiers::groupName, pluginName);
    setProperty(Identifiers::groupIcon, Identifiers::templateGroupIcon.arg(pluginName).arg("close_stop"));
    setProperty(Identifiers::groupLocId, Identifiers::templateActionLocId.arg(pluginName));
    setProperty(Identifiers::groupItem, Identifiers::templateActionItem.arg(pluginName));
    setProperty(Identifiers::groupEditor, Identifiers::templateGroupEditor.arg(pluginName));
    setProperty(Identifiers::groupSettings, Identifiers::templateGroupSettings.arg(pluginName));
    setProperty(Identifiers::groupMaxCount, 10);
}

Action* ClosePlugin::createInstance(QObject* parent)
{
    return new CloseAction(parent);
}

void ClosePlugin::qmlRegisterTypes()
{
    // @uri com.pastillilabs.situations.action.close
    qmlRegisterType<CloseAction>(Identifiers::templateActionUri.arg(pluginName).toLatin1(), 1, 0, "CloseAction");
    qmlRegisterType<ApplicationListModel>(Identifiers::templateActionUri.arg(pluginName).toLatin1(), 1, 0, "ApplicationListModel");
}

const QVariant ClosePlugin::property(const char* name) const
{
    return QObject::property(name);
}

Q_EXPORT_PLUGIN2(situationsactionclose, ClosePlugin)

