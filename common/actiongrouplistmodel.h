#ifndef ACTIONGROUPLISTMODEL_H
#define ACTIONGROUPLISTMODEL_H

#include "common_global.h"
#include <QAbstractListModel>

class ActionGroup;

/**
  * ActionGroupListModel
  */
class COMMONSHARED_EXPORT ActionGroupListModel
    : public QAbstractListModel
{
private:
    Q_OBJECT
    Q_DISABLE_COPY(ActionGroupListModel)

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
    explicit ActionGroupListModel(QObject* parent = 0);

    void operator<<(const QVariantList& in);
    void operator>>(QVariantList& out) const;

    const QList<ActionGroup*>& list() const;

    void addActionGroup(ActionGroup& actionGroup, int index);
    ActionGroup* rmvActionGroup(int index);

    void addAction(const int group, const QVariantMap& data);
    void rmvAction(const int group, const int action);
    void setActionData(const int group, const int action, const QVariantMap& data);

    void start();
    void stop();

private: // From QAbstractListModel
    int rowCount(const QModelIndex& parent = QModelIndex()) const;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;

private:
    QList<ActionGroup*> mActionGroups;
};

#endif // ACTIONGROUPLISTMODEL_H

