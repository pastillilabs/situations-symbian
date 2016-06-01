#include "conditiongrouplistmodel.h"
#include "conditiongroup.h"
#include "identifiers.h"

ConditionGroupListModel::ConditionGroupListModel(QObject* parent)
    : QAbstractListModel(parent)
{
    QHash<int, QByteArray> roles;
    roles[Group] = Identifiers::group;
    roles[GroupLocId] = Identifiers::groupLocId;
    roles[GroupIcon] = Identifiers::groupIcon;
    roles[GroupItem] = Identifiers::groupItem;
    roles[GroupEditor] = Identifiers::groupEditor;
    roles[GroupSettings] = Identifiers::groupSettings;
    roles[GroupMaxCount] = Identifiers::groupMaxCount;
    setRoleNames(roles);
}

void ConditionGroupListModel::operator<<(const QVariantList& in)
{
    foreach(const QVariant& data, in)
    {
        // Try to input data to all groups (only matching group should accept it)
        foreach(ConditionGroup* group, mConditionGroups) (*group) << data.toMap();
    }
}

void ConditionGroupListModel::operator>>(QVariantList& out) const
{
    foreach(const ConditionGroup* group, mConditionGroups)
    {
        QVariantMap map;
        (*group) >> map;
        out.append(map);
    }
}

const QList<ConditionGroup*>& ConditionGroupListModel::list() const
{
    return mConditionGroups;
}

void ConditionGroupListModel::addConditionGroup(ConditionGroup& conditionGroup, int index)
{
    beginInsertRows(QModelIndex(), index, index);
    mConditionGroups.insert(index, &conditionGroup);
    endInsertRows();
}

ConditionGroup* ConditionGroupListModel::rmvConditionGroup(int index)
{
    ConditionGroup* group(0);
    beginRemoveRows(QModelIndex(), index, index);
    group = mConditionGroups.takeAt(index);
    endRemoveRows();
    return group;
}

void ConditionGroupListModel::addCondition(const int group, const QVariantMap& data)
{
    ConditionGroup* c(mConditionGroups.at(group));
    if(c) c->addCondition(data);
}

void ConditionGroupListModel::rmvCondition(const int group, const int condition)
{
    ConditionGroup* c(mConditionGroups.at(group));
    if(c) c->rmvCondition(condition);
}

void ConditionGroupListModel::setConditionData(const int group, const int condition, const QVariantMap& data)
{
    ConditionGroup* c(mConditionGroups.at(group));
    if(c) c->setConditionData(condition, data);
}

int ConditionGroupListModel::rowCount(const QModelIndex& /*parent*/) const
{
    return mConditionGroups.count();
}

QVariant ConditionGroupListModel::data(const QModelIndex& index, int role) const
{
    QVariant data;
    if(index.row() >= 0 && index.row() < mConditionGroups.count())
    {
        ConditionGroup* group(mConditionGroups.at(index.row()));
        switch(role)
        {
        case Group:
            data = QVariant::fromValue(qobject_cast<QObject*>(group));
            break;
        case GroupLocId:
            data = group->property(Identifiers::groupLocId);
            break;
        case GroupIcon:
            data = group->property(Identifiers::groupIcon);
            break;
        case GroupItem:
            data = group->property(Identifiers::groupItem);
            break;
        case GroupEditor:
            data = group->property(Identifiers::groupEditor);
            break;
        case GroupSettings:
            data = group->property(Identifiers::groupSettings);
            break;
        case GroupMaxCount:
            data = group->property(Identifiers::groupMaxCount);
            break;
        default:
            Q_ASSERT(false);
        }
    }

    return data;
}

