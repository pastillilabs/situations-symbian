#ifndef CONDITIONGROUPLISTMODEL_H
#define CONDITIONGROUPLISTMODEL_H

#include "common_global.h"
#include <QAbstractListModel>

class ConditionGroup;

/**
  * ConditionGroupListModel
  */
class COMMONSHARED_EXPORT ConditionGroupListModel
    : public QAbstractListModel
{
private:
    Q_OBJECT
    Q_DISABLE_COPY(ConditionGroupListModel)

    enum Roles {
        Group = Qt::UserRole + 1,
        GroupLocId,
        GroupIcon,
        GroupItem,
        GroupSettings,
        GroupEditor,
        GroupMaxCount
    };

public:
    explicit ConditionGroupListModel(QObject* parent = 0);

    void operator<<(const QVariantList& in);
    void operator>>(QVariantList& out) const;

    const QList<ConditionGroup*>& list() const;

    void addConditionGroup(ConditionGroup& conditionGroup, int index);
    ConditionGroup* rmvConditionGroup(int index);

    void addCondition(const int group, const QVariantMap& data);
    void rmvCondition(const int group, const int condition);
    void setConditionData(const int group, const int condition, const QVariantMap& data);

private: // From QAbstractListModel
    int rowCount(const QModelIndex& parent = QModelIndex()) const;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;

private:
    QList<ConditionGroup*> mConditionGroups;
};

#endif // CONDITIONGROUPLISTMODEL_H

