#include "identifiers.h"
#include <QString>

static const QString SITUATIONSURI("com.pastillilabs.situations");

const QString Identifiers::SERVERNAME(SITUATIONSURI + ".server");

const char* Identifiers::group("group");
const char* Identifiers::groupId("groupId");
const char* Identifiers::groupName("groupName");
const char* Identifiers::groupLocId("groupLocId");
const char* Identifiers::groupIcon("groupIcon");
const char* Identifiers::groupItem("groupItem");
const char* Identifiers::groupEditor("groupEditor");
const char* Identifiers::groupSettings("groupSettings");
const char* Identifiers::groupMaxCount("groupMaxCount");
const char* Identifiers::groupInitToDefault("pluginInitToDefault");

const char* Identifiers::keySituations("situations");
const char* Identifiers::keyConditions("conditions");
const char* Identifiers::keyActions("actions");

const char* Identifiers::keyManual("manualMode");
const char* Identifiers::keyEnabled("enabled");
const char* Identifiers::keyCurrent("current");
const char* Identifiers::keyProposal("proposal");
const char* Identifiers::keyNotifications("notifications");

const char* Identifiers::keyConditionGroups("conditionGroups");
const char* Identifiers::keyActionGroups("actionGroups");
const char* Identifiers::keyLocId("locId");
const char* Identifiers::keyName("name");
const char* Identifiers::keyType("type");

const char* Identifiers::keyId("id");
const char* Identifiers::keyNegative("negative");
const char* Identifiers::keyInstances("instances");

const char* Identifiers::keyStartDelay("startDelay");
const char* Identifiers::keyRestore("restore");
const char* Identifiers::keyValue("value");
const char* Identifiers::keySituationEnd("situationEnd");

const char* Identifiers::situationsDir("situations");
const char* Identifiers::locDir("loc");

const QString Identifiers::templateGroupIcon("qrc:/gfx/%1/%2.svg");
const QString Identifiers::templateGroupEditor("qrc:/qml/%1/Editor.qml");
const QString Identifiers::templateGroupSettings("qrc:/qml/%1/Settings.qml");
const QString Identifiers::templateConditionLocId("condition_%1_name");
const QString Identifiers::templateActionLocId("action_%1_name");
const QString Identifiers::templateConditionItem("qrc:/qml/%1/Condition.qml");
const QString Identifiers::templateActionItem("qrc:/qml/%1/Action.qml");
const QString Identifiers::templateConditionUri(SITUATIONSURI + ".condition.%1");
const QString Identifiers::templateActionUri(SITUATIONSURI + ".action.%1");

Identifiers::Identifiers()
{
}

