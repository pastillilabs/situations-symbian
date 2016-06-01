#include "timeplugin.h"
#include "timecondition.h"
#include <identifiers.h>
#include <qdeclarative.h>

static const char* pluginName("time");

TimePlugin::TimePlugin()
    : QObject()
{
    setProperty(Identifiers::groupId, 100);
    setProperty(Identifiers::groupName, pluginName);
    setProperty(Identifiers::groupIcon, Identifiers::templateGroupIcon.arg(pluginName).arg("clock"));
    setProperty(Identifiers::groupLocId, Identifiers::templateConditionLocId.arg(pluginName));
    setProperty(Identifiers::groupItem, Identifiers::templateConditionItem.arg(pluginName));
    setProperty(Identifiers::groupEditor, Identifiers::templateGroupEditor.arg(pluginName));
    setProperty(Identifiers::groupMaxCount, 10);
}

Condition* TimePlugin::createInstance(QObject* parent)
{
    return new TimeCondition(parent);
}

void TimePlugin::qmlRegisterTypes()
{
    // @uri com.pastillilabs.situations.condition.time
    qmlRegisterType<TimeCondition>(Identifiers::templateConditionUri.arg(pluginName).toLatin1(), 1, 0, "TimeCondition");
}

const QVariant TimePlugin::property(const char* name) const
{
    return QObject::property(name);
}

Q_EXPORT_PLUGIN2(situationsconditiontime, TimePlugin)

