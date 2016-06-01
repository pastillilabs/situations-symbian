#include "situation.h"
#include "pluginmanager.h"
#include "conditionplugin.h"
#include "conditiongroup.h"
#include "actionplugin.h"
#include "actiongroup.h"
#include "identifiers.h"

Situation::Situation(QObject* parent, const QString& locId, const QString& name)
    : QObject(parent)
    , mLocId(locId)
    , mName(name)
    , mEnabled(true)
    , mActive(false)
    , mRunning(false)
{
}

void Situation::initialize(const PluginManager& pluginManager)
{
    connect(&pluginManager, SIGNAL(addCondition(ConditionPlugin&,int,QString)), this, SLOT(addConditionPlugin(ConditionPlugin&,int,QString)));
    connect(&pluginManager, SIGNAL(addAction(ActionPlugin&,int,QString)), this, SLOT(addActionPlugin(ActionPlugin&,int,QString)));
    connect(&pluginManager, SIGNAL(rmvCondition(ConditionPlugin&,QString)), this, SLOT(rmvConditionPlugin(ConditionPlugin&,QString)));
    connect(&pluginManager, SIGNAL(rmvAction(ActionPlugin&,QString)), this, SLOT(rmvActionPlugin(ActionPlugin&,QString)));

    // Iterate through condition plugins
    const QList<ConditionPlugin*>& conditions(pluginManager.conditionPlugins());
    for(int c(0); c < conditions.count(); ++c)
    {
        addConditionPlugin(*(conditions.at(c)), c, "");
    }

    // Iterate through action plugins
    const QList<ActionPlugin*>& actions(pluginManager.actionPlugins());
    for(int a(0); a < actions.count(); ++a)
    {
        addActionPlugin(*(actions.at(a)), a, "");
    }
}

void Situation::operator<<(const QVariantMap& in)
{
    mConditionGroups << in[Identifiers::keyConditionGroups].toList();
    mActionGroups << in[Identifiers::keyActionGroups].toList();
    setLocId(in[Identifiers::keyLocId].toString());
    setName(in[Identifiers::keyName].toString());
    setEnabled(in[Identifiers::keyEnabled].toBool());
}

void Situation::operator>>(QVariantMap& out) const
{
    QVariantList conditionGroups;
    QVariantList actionGroups;
    mConditionGroups >> conditionGroups;
    mActionGroups >> actionGroups;

    out[Identifiers::keyConditionGroups] = conditionGroups;
    out[Identifiers::keyActionGroups] = actionGroups;
    out[Identifiers::keyLocId] = mLocId;
    out[Identifiers::keyName] = mName;
    out[Identifiers::keyEnabled] = mEnabled;
}

void Situation::setLocId(const QString& locId)
{
    if(locId != mLocId)
    {
        mLocId = locId;
        emit locIdChanged(locId);
        emit nameChanged(name());
    }
}

void Situation::setName(const QString& name)
{
    if(name != mName)
    {
        mName = name;
        emit nameChanged(name);
    }
}

void Situation::setEnabled(bool enabled)
{
    if(enabled != mEnabled)
    {
        mEnabled = enabled;
        emit enabledChanged(enabled);
    }

    if(mRunning) mEnabled ? startConditions() : stopConditions();
}

void Situation::setActive(bool active)
{
    if(active != mActive)
    {
        mActive = active;
        emit activeChanged(active);
    }
}

const QString& Situation::locId() const
{
    return mLocId;
}

const QString Situation::name() const
{
    QString name(mName);
    if(name.isEmpty()) name = qtTrId(mLocId.toLatin1());
    return name;
}

bool Situation::enabled() const
{
    return mEnabled;
}

bool Situation::active() const
{
    return mActive;
}

void Situation::start()
{
    if(!mRunning)
    {
        mRunning = true;
        if(mEnabled)
        {
            startConditions();
            evaluate();
        }
    }
}

void Situation::stop()
{
    if(mRunning)
    {
        mRunning = false;
        if(mEnabled) stopConditions();
    }
}

void Situation::execute()
{
    mActionGroups.start();
}

void Situation::finish()
{
    mActionGroups.stop();
}

Condition* Situation::addCondition(const int group, const QVariantMap& data)
{
    Condition* condition(0);
    if(group >= 0 && group < mConditionGroups.list().count())
    {
        condition = mConditionGroups.list().at(group)->addCondition(data);
        if(mRunning && mEnabled) evaluate();
    }

    return condition;
}

void Situation::rmvCondition(const int group, const int condition)
{
    if(group >= 0 && group < mConditionGroups.list().count())
    {
        mConditionGroups.list().at(group)->rmvCondition(condition);
        if(mRunning && mEnabled)
        {
            startConditions();
            evaluate();
        }
    }
}

void Situation::setConditionData(const int group, const int condition, const QVariantMap& data)
{
    if(group >= 0 && group < mConditionGroups.list().count())
    {
        mConditionGroups.list().at(group)->setConditionData(condition, data);
    }
}

Action* Situation::addAction(const int group, const QVariantMap& data)
{
    Action* action(0);
    if(group >= 0 && group < mActionGroups.list().count())
    {
        action = mActionGroups.list().at(group)->addAction(data);
    }

    return action;
}

void Situation::rmvAction(const int group, const int action)
{
    if(group >= 0 && group < mActionGroups.list().count())
    {
        mActionGroups.list().at(group)->rmvAction(action);
    }
}

void Situation::setActionData(const int group, const int action, const QVariantMap& data)
{
    if(group >= 0 && group < mActionGroups.list().count())
    {
        mActionGroups.list().at(group)->setActionData(action, data);
    }
}

void Situation::setConditionGroupNegative(const int group, const bool negative)
{
    if(group >= 0 && group < mConditionGroups.list().count())
    {
        mConditionGroups.list().at(group)->setNegative(negative);
    }
}

ConditionGroupListModel& Situation::conditions()
{
    return mConditionGroups;
}

ActionGroupListModel& Situation::actions()
{
    return mActionGroups;
}

void Situation::evaluate()
{
    int active(0);
    int groups(0);
    const QList<ConditionGroup*>& groupList(mConditionGroups.list());

    QList<ConditionGroup*>::const_iterator i(groupList.constBegin());
    while(i != groupList.constEnd() && active == groups)
    {
        ConditionGroup* group(*i);
        if(!group->empty())
        {
            ++groups;
            if(group->active()) ++active;
        }
        ++i;
    }

    setActive(active && active == groups);
}

void Situation::startConditions()
{
    // Start condition groups until first non-running, non-active group
    const QList<ConditionGroup*>& groupList(mConditionGroups.list());
    QList<ConditionGroup*>::const_iterator i(groupList.constBegin());
    while(i != groupList.constEnd())
    {
        ConditionGroup* group(*i);
        if(!group->running())
        {
            group->start();
            if(!group->active()) break; // Stop at first non-active group
        }

        ++i;
    }
}

void Situation::stopConditions()
{
    // Stop all condition groups
    const QList<ConditionGroup*>& groupList(mConditionGroups.list());
    QList<ConditionGroup*>::const_iterator i(groupList.constBegin());
    while(i != groupList.constEnd())
    {
        ConditionGroup* group(*i);
        group->stop();
        ++i;
    }
}

void Situation::conditionGroupActiveChanged(bool active)
{
    ConditionGroup* group(qobject_cast<ConditionGroup*>(sender()));
    const QList<ConditionGroup*>& groupList(mConditionGroups.list());

    // Start/stop following condition groups
    const bool starting(active && !group->empty());
    const int nextIndex(groupList.indexOf(group) + 1);
    for(int i(nextIndex); i < groupList.count(); ++i)
    {
        ConditionGroup* next(groupList.at(i));
        if(starting)
        {
            next->start();
            if(!next->active()) break; // Stop at next non-active group
        }
        else
        {
            next->stop();
        }
    }

    evaluate();
}

void Situation::addConditionPlugin(ConditionPlugin& plugin, int index, const QString& /*name*/)
{
    ConditionGroup* group(new ConditionGroup(plugin, &mConditionGroups));
    connect(group, SIGNAL(activeChanged(bool)), this, SLOT(conditionGroupActiveChanged(bool)));
    mConditionGroups.addConditionGroup(*group, index);
    if(mRunning && mEnabled) startConditions();
}

void Situation::addActionPlugin(ActionPlugin& plugin, int index, const QString& /*name*/)
{
    ActionGroup* group(new ActionGroup(plugin, &mActionGroups));
    mActionGroups.addActionGroup(*group, index);
}

void Situation::rmvConditionPlugin(ConditionPlugin& plugin, const QString& /*name*/)
{
    const QList<ConditionGroup*> list(mConditionGroups.list());
    for(int i(0); i < list.count(); ++i)
    {
        ConditionGroup* group(list.at(i));
        if(&(group->plugin()) == &plugin)
        {
            mConditionGroups.rmvConditionGroup(i);
            delete group;
            if(mRunning && mEnabled) evaluate();
            break;
        }
    }
}

void Situation::rmvActionPlugin(ActionPlugin& plugin, const QString& /*name*/)
{
    const QList<ActionGroup*> list(mActionGroups.list());
    for(int i(0); i < list.count(); ++i)
    {
        ActionGroup* group(list.at(i));
        if(&(group->plugin()) == &plugin)
        {
            mActionGroups.rmvActionGroup(i);
            delete group;
            break;
        }
    }
}

