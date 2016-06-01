#include "calendarplugin.h"
#include "calendarcondition.h"
#include "calendarcollections.h"
#include <identifiers.h>
#include <qdeclarative.h>

static const char* pluginName("calendar");

CalendarPlugin::CalendarPlugin()
    : QObject()
    , ConditionPlugin()
{
    setProperty(Identifiers::groupId, 200);
    setProperty(Identifiers::groupName, pluginName);
    setProperty(Identifiers::groupIcon, Identifiers::templateGroupIcon.arg(pluginName).arg("calendar"));
    setProperty(Identifiers::groupLocId, Identifiers::templateConditionLocId.arg(pluginName));
    setProperty(Identifiers::groupItem, Identifiers::templateConditionItem.arg(pluginName));
    setProperty(Identifiers::groupEditor, Identifiers::templateGroupEditor.arg(pluginName));
    setProperty(Identifiers::groupMaxCount, 10);
}

Condition* CalendarPlugin::createInstance(QObject* parent)
{
    return new CalendarCondition(parent);
}

void CalendarPlugin::qmlRegisterTypes()
{
    // @uri com.pastillilabs.situations.condition.calendar
    qmlRegisterType<CalendarCondition>(Identifiers::templateConditionUri.arg(pluginName).toLatin1(), 1, 0, "CalendarCondition");
    qmlRegisterType<CalendarCollections>(Identifiers::templateConditionUri.arg(pluginName).toLatin1(), 1, 0, "CalendarCollections");
}

const QVariant CalendarPlugin::property(const char* name) const
{
    return QObject::property(name);
}

Q_EXPORT_PLUGIN2(situationsconditioncalendar, CalendarPlugin)

