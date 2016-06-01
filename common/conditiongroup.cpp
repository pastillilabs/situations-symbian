#include "conditiongroup.h"
#include "condition.h"
#include "identifiers.h"

ConditionGroup::ConditionGroup(ConditionPlugin& plugin, QObject* parent)
    : QAbstractListModel(parent)
    , mPlugin(plugin)
    , mRunning(false)
    , mActive(true)
    , mNegative(false)
{
    QHash<int, QByteArray> roles;
    roles[ConditionRole] = "condition";
    setRoleNames(roles);
}

ConditionGroup::~ConditionGroup()
{
}

void ConditionGroup::operator<<(const QVariantMap& in)
{
    const int id(in[Identifiers::keyId].toInt());
    if(id == mPlugin.property(Identifiers::groupId))
    {
        setNegative(in[Identifiers::keyNegative].toBool());

        const QVariantList instances(in[Identifiers::keyInstances].toList());
        foreach(const QVariant& data, instances) addCondition(data.toMap());
    }
}

void ConditionGroup::operator>>(QVariantMap& out) const
{
    QVariantList instances;
    foreach(const Condition* condition, mConditions)
    {
        QVariantMap data(condition->data());
        instances.append(data);
    }

    out[Identifiers::keyId] = mPlugin.property(Identifiers::groupId).toInt();
    out[Identifiers::keyNegative] = mNegative;
    out[Identifiers::keyInstances] = instances;
}

void ConditionGroup::start()
{
    if(!mRunning)
    {
        mRunning = true;
        emit runningChanged(mRunning);

        // Start all conditions
        QList<Condition*>::const_iterator i(mConditions.begin());
        while(i != mConditions.end())
        {
            (*i)->start();
            ++i;
        }

        evaluate();
    }
}

void ConditionGroup::stop()
{
    if(mRunning)
    {
        setActive(false);

        // Stop all conditions
        QList<Condition*>::const_iterator i(mConditions.begin());
        while(i != mConditions.end())
        {
            (*i)->stop();
            ++i;
        }

        mRunning = false;
        emit runningChanged(mRunning);
    }
}

bool ConditionGroup::running() const
{
    return mRunning;
}

bool ConditionGroup::active() const
{
    return mActive;
}

bool ConditionGroup::empty() const
{
    return mConditions.empty();
}

bool ConditionGroup::negative() const
{
    return mNegative;
}

void ConditionGroup::setNegative(bool negative)
{
    if(negative != mNegative)
    {
        mNegative = negative;
        emit negativeChanged(negative);

        if(mRunning) evaluate();
    }
}

Condition* ConditionGroup::addCondition(const QVariantMap& data)
{
    Condition* condition(mPlugin.createInstance(this));
    addCondition(*condition);
    condition->setData(data);
    connect(condition, SIGNAL(activeChanged(bool)), this, SLOT(conditionActiveChanged(bool)));

    if(mRunning)
    {
        condition->start();
        evaluate();
    }
    else
    {
        setActive(false);
    }

    return condition;
}

void ConditionGroup::rmvCondition(const int condition)
{
    if(condition >= 0 && condition < mConditions.count())
    {
        beginRemoveRows(QModelIndex(), condition, condition);
        Condition* c(mConditions.takeAt(condition));
        endRemoveRows();

        delete c;
        if(mRunning) evaluate();
    }
}

void ConditionGroup::setConditionData(const int condition, const QVariantMap& data)
{
    if(condition >= 0 && condition < mConditions.count())
    {
        Condition* c(mConditions.at(condition));
        c->setData(data);

        QModelIndex i(index(condition));
        emit dataChanged(i, i);
    }
}

const QVariantMap ConditionGroup::getConditionData(const int condition)
{
    if(condition >= 0 && condition < mConditions.count())
    {
        return mConditions.at(condition)->data();
    }

    return QVariantMap();
}

ConditionPlugin& ConditionGroup::plugin() const
{
    return mPlugin;
}

const QVariant ConditionGroup::property(const char* name) const
{
    return mPlugin.property(name);
}

void ConditionGroup::addCondition(Condition& condition)
{
    beginInsertRows(QModelIndex(), conditions().count(), conditions().count());
    mConditions.append(&condition);
    endInsertRows();
}

const QList<Condition*>& ConditionGroup::conditions() const
{
    return mConditions;
}

void ConditionGroup::evaluate()
{
    bool active(false);

    QList<Condition*>::const_iterator i(mConditions.begin());
    while(i != mConditions.end() && !active)
    {
        active = (*i)->active();
        ++i;
    }

    if(mNegative) active = !active;
    if(empty()) active = true;

    setActive(active);
}

void ConditionGroup::setActive(bool active)
{
    if(active != mActive)
    {
        mActive = active;
        emit activeChanged(mActive);
    }
}

void ConditionGroup::conditionActiveChanged(bool /*active*/)
{
    Q_ASSERT(mRunning);
    evaluate();
}

int ConditionGroup::rowCount(const QModelIndex& /*parent*/) const
{
    return mConditions.count();
}

QVariant ConditionGroup::data(const QModelIndex& index, int role) const
{
    QVariant data;
    if(index.row() >= 0 && index.row() < mConditions.size())
    {
        switch(role)
        {
        case ConditionRole:
            data = QVariant::fromValue(qobject_cast<QObject*>(mConditions.at(index.row())));
            break;
        default:
            Q_ASSERT(false);
        }
    }

    return data;
}

