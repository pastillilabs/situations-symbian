#include "actiongrouplistmodel.h"
#include "actiongroup.h"
#include "identifiers.h"

ActionGroupListModel::ActionGroupListModel(QObject* parent)
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

void ActionGroupListModel::operator<<(const QVariantList& in)
{
    foreach(const QVariant& data, in)
    {
        // Try to input data to all groups (only matching group should accept it)
        foreach(ActionGroup* group, mActionGroups) (*group) << data.toMap();
    }
}

void ActionGroupListModel::operator>>(QVariantList& out) const
{
    foreach(const ActionGroup* group, mActionGroups)
    {
        QVariantMap map;
        (*group) >> map;
        out.append(map);
    }
}

const QList<ActionGroup*>& ActionGroupListModel::list() const
{
    return mActionGroups;
}

void ActionGroupListModel::addActionGroup(ActionGroup& actionGroup, int index)
{
    beginInsertRows(QModelIndex(), index, index);
    mActionGroups.insert(index, &actionGroup);
    endInsertRows();
}

ActionGroup* ActionGroupListModel::rmvActionGroup(int index)
{
    ActionGroup* group(0);
    beginRemoveRows(QModelIndex(), index, index);
    group = mActionGroups.takeAt(index);
    endRemoveRows();
    return group;
}

void ActionGroupListModel::addAction(const int group, const QVariantMap& data)
{
    ActionGroup* c(mActionGroups.at(group));
    if(c) c->addAction(data);
}

void ActionGroupListModel::rmvAction(const int group, const int action)
{
    ActionGroup* c(mActionGroups.at(group));
    if(c) c->rmvAction(action);
}

void ActionGroupListModel::setActionData(const int group, const int action, const QVariantMap& data)
{
    ActionGroup* c(mActionGroups.at(group));
    if(c) c->setActionData(action, data);
}

void ActionGroupListModel::start()
{
    QList<ActionGroup*>::const_iterator c(mActionGroups.constBegin());
    while(c != mActionGroups.constEnd())
    {
        (*c)->start();
        ++c;
    }
}

void ActionGroupListModel::stop()
{
    QList<ActionGroup*>::const_iterator c(mActionGroups.constBegin());
    while(c != mActionGroups.constEnd())
    {
        (*c)->stop();
        ++c;
    }
}

int ActionGroupListModel::rowCount(const QModelIndex& /*parent*/) const
{
    return mActionGroups.count();
}

QVariant ActionGroupListModel::data(const QModelIndex& index, int role) const
{
    QVariant data;
    if(index.row() >= 0 && index.row() < mActionGroups.count())
    {
        ActionGroup* group(mActionGroups.at(index.row()));
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

