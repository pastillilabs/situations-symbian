#include "cellplugin.h"
#include "cellcondition.h"
#include <identifiers.h>
#include <qdeclarative.h>

static const char* pluginName("cell");

CellPlugin::CellPlugin()
    : QObject()
{
    setProperty(Identifiers::groupId, 250);
    setProperty(Identifiers::groupName, pluginName);
    setProperty(Identifiers::groupIcon, Identifiers::templateGroupIcon.arg(pluginName).arg("reception"));
    setProperty(Identifiers::groupLocId, Identifiers::templateConditionLocId.arg(pluginName));
    setProperty(Identifiers::groupItem, Identifiers::templateConditionItem.arg(pluginName));
    setProperty(Identifiers::groupEditor, Identifiers::templateGroupEditor.arg(pluginName));
    setProperty(Identifiers::groupMaxCount, 10);
}

Condition* CellPlugin::createInstance(QObject* parent)
{
    return new CellCondition(parent);
}

void CellPlugin::qmlRegisterTypes()
{
    // @uri com.pastillilabs.situations.condition.cell
    qmlRegisterType<CellCondition>(Identifiers::templateConditionUri.arg(pluginName).toLatin1(), 1, 0, "CellCondition");
}

const QVariant CellPlugin::property(const char* name) const
{
    return QObject::property(name);
}

Q_EXPORT_PLUGIN2(situationsconditioncell, CellPlugin)

