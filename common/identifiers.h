#ifndef IDENTIFIERS_H
#define IDENTIFIERS_H

#include "common_global.h"

/**
 * Identifiers
 */
class COMMONSHARED_EXPORT Identifiers
{
public:
    Identifiers();

public:
    static const QString SERVERNAME;

    static const char* group;
    static const char* groupId;
    static const char* groupName;
    static const char* groupLocId;
    static const char* groupIcon;
    static const char* groupItem;
    static const char* groupEditor;
    static const char* groupSettings;
    static const char* groupMaxCount;
    static const char* groupInitToDefault;

    static const char* keySituations;
    static const char* keyConditions;
    static const char* keyActions;

    static const char* keyManual;
    static const char* keyEnabled;
    static const char* keyCurrent;
    static const char* keyProposal;
    static const char* keyNotifications;

    static const char* keyConditionGroups;
    static const char* keyActionGroups;
    static const char* keyLocId;
    static const char* keyName;
    static const char* keyType;

    static const char* keyId;
    static const char* keyNegative;
    static const char* keyInstances;

    static const char* keyStartDelay;
    static const char* keyRestore;
    static const char* keyValue;
    static const char* keySituationEnd;

    // The rest are not really ids but values or value templates
    static const char* situationsDir;
    static const char* locDir;

    static const QString templateGroupIcon;
    static const QString templateGroupEditor;
    static const QString templateGroupSettings;
    static const QString templateConditionLocId;
    static const QString templateActionLocId;
    static const QString templateConditionItem;
    static const QString templateActionItem;
    static const QString templateConditionUri;
    static const QString templateActionUri;
};

#endif // IDENTIFIERS_H

