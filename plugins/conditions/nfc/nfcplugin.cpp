#include "nfcplugin.h"
#include "nfccondition.h"
#include "nfcscanner.h"
#include <identifiers.h>
#include <qdeclarative.h>

static const char* pluginName("nfc");

NfcPlugin::NfcPlugin()
    : QObject()
{
    setProperty(Identifiers::groupId, 275);
    setProperty(Identifiers::groupName, pluginName);
    setProperty(Identifiers::groupIcon, Identifiers::templateGroupIcon.arg(pluginName).arg("tag"));
    setProperty(Identifiers::groupLocId, Identifiers::templateConditionLocId.arg(pluginName));
    setProperty(Identifiers::groupItem, Identifiers::templateConditionItem.arg(pluginName));
    setProperty(Identifiers::groupEditor, Identifiers::templateGroupEditor.arg(pluginName));
    setProperty(Identifiers::groupMaxCount, 10);
}

Condition* NfcPlugin::createInstance(QObject* parent)
{
    return new NfcCondition(parent);
}

void NfcPlugin::qmlRegisterTypes()
{
    // @uri com.pastillilabs.situations.condition.nfc
    qmlRegisterType<NfcCondition>(Identifiers::templateConditionUri.arg(pluginName).toLatin1(), 1, 0, "NfcCondition");
    qmlRegisterType<NfcScanner>(Identifiers::templateConditionUri.arg(pluginName).toLatin1(), 1, 0, "NfcScanner");
}

const QVariant NfcPlugin::property(const char* name) const
{
    return QObject::property(name);
}

Q_EXPORT_PLUGIN2(situationsconditionnfc, NfcPlugin)

