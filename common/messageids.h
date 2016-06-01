#ifndef MESSAGEIDS_H
#define MESSAGEIDS_H

// Client requests
enum ClientMessage
{
    ReqVersion = 0,
    ReqPlugins,
    ReqSituations,
    ReqAddPlugin,
    ReqRmvPlugin,
    ReqSetEnabled,
    ReqSetCurrent,
    ReqSetNotifications,
    ReqAddSituation,
    ReqRmvSituation,
    ReqMoveSituation,
    ReqSetSituationName,
    ReqSetSituationEnabled,
    ReqAddCondition,
    ReqRmvCondition,
    ReqSetConditionData,
    ReqAddAction,
    ReqRmvAction,
    ReqSetActionData,
    ReqSetConditionGroupNegative,
    ReqUpdateTranslation
};

// Server responses & indications
enum ServerMessage
{
    ResVersion = 0,
    ResPlugins,
    ResSituations,
    IndAddPlugin,
    IndRmvPlugin,
    IndSetEnabled,
    IndSetProposal,
    IndSetCurrent,
    IndSetNotifications,
    IndAddSituation,
    IndRmvSituation,
    IndMoveSituation,
    IndSetSituationName,
    IndSetSituationEnabled,
    IndSetSituationActive,
    IndAddCondition,
    IndRmvCondition,
    IndSetConditionData,
    IndAddAction,
    IndRmvAction,
    IndSetActionData,
    IndSetConditionGroupNegative
};

#endif // MESSAGEIDS_H

