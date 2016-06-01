#include "actiongroup.h"
#include "action.h"
#include "identifiers.h"

ActionGroup::ActionGroup(ActionPlugin& plugin, QObject* parent)
    : QAbstractListModel(parent)
    , mPlugin(plugin)
    , mRunning(false)
{
    QHash<int, QByteArray> roles;
    roles[ActionRole] = "action";
    setRoleNames(roles);
}

ActionGroup::~ActionGroup()
{
}

void ActionGroup::operator<<(const QVariantMap& in)
{
    const int id(in[Identifiers::keyId].toInt());
    if(id == mPlugin.property(Identifiers::groupId))
    {
        const QVariantList instances(in[Identifiers::keyInstances].toList());
        foreach(const QVariant& data, instances) addAction(data.toMap());
    }
}

void ActionGroup::operator>>(QVariantMap& out) const
{
    QVariantList instances;
    foreach(const Action* action, mActions)
    {
        instances.append(action->data());
    }

    out[Identifiers::keyId] = mPlugin.property(Identifiers::groupId).toInt();
    out[Identifiers::keyInstances] = instances;
}

void ActionGroup::start()
{
    mRunning = true;

    // Start all actions
    QList<Action*>::const_iterator i(mActions.begin());
    while(i != mActions.end())
    {
        (*i)->start(false);
        ++i;
    }
}

void ActionGroup::stop()
{
    mRunning = false;

    // Stop all actions
    QList<Action*>::const_iterator i(mActions.begin());
    while(i != mActions.end())
    {
        (*i)->stop(false);
        ++i;
    }
}

Action* ActionGroup::addAction(const QVariantMap& data)
{
    Action* action(mPlugin.createInstance(this));
    addAction(*action);
    if(data.isEmpty()) action->initialize();
    else action->setData(data);

    if(mRunning) action->start(true);
    return action;
}

void ActionGroup::rmvAction(const int action)
{
    if(action >= 0 && action < mActions.count())
    {
        beginRemoveRows(QModelIndex(), action, action);
        Action* c(mActions.takeAt(action));
        if(mRunning) c->stop(true);
        delete c;
        endRemoveRows();
    }
}

void ActionGroup::setActionData(const int action, const QVariantMap& data)
{
    if(action >= 0 && action < mActions.count())
    {
        Action* c(mActions.at(action));
        c->setData(data);

        QModelIndex i(index(action));
        emit dataChanged(i, i);
    }
}

const QVariantMap ActionGroup::getActionData(const int action)
{
    if(action >= 0 && action < mActions.count())
    {
        return mActions.at(action)->data();
    }

    return QVariantMap();
}

ActionPlugin& ActionGroup::plugin() const
{
    return mPlugin;
}

const QVariant ActionGroup::property(const char* name) const
{
    return mPlugin.property(name);
}

void ActionGroup::addAction(Action& action)
{
    beginInsertRows(QModelIndex(), actions().count(), actions().count());
    mActions.append(&action);
    endInsertRows();
}

const QList<Action*>& ActionGroup::actions() const
{
    return mActions;
}

int ActionGroup::rowCount(const QModelIndex& /*parent*/) const
{
    return mActions.count();
}

QVariant ActionGroup::data(const QModelIndex& index, int role) const
{
    QVariant data;
    if(index.row() >= 0 && index.row() < mActions.size())
    {
        switch(role)
        {
        case ActionRole:
            data = QVariant::fromValue(qobject_cast<QObject*>(mActions.at(index.row())));
            break;
        default:
            Q_ASSERT(false);
        }
    }

    return data;
}

