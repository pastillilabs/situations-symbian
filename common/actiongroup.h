#ifndef ACTIONGROUP_H
#define ACTIONGROUP_H

#include "common_global.h"
#include "actionplugin.h"
#include <QAbstractListModel>

class Action;

/**
  * ActionGroup
  */
class COMMONSHARED_EXPORT ActionGroup
    : public QAbstractListModel
{
private:
    Q_OBJECT
    Q_DISABLE_COPY(ActionGroup)

    enum Roles {
        ActionRole = Qt::UserRole + 1
    };

public:
    ActionGroup(ActionPlugin& plugin, QObject* parent = 0);
    virtual ~ActionGroup();

    void operator<<(const QVariantMap& in);
    void operator>>(QVariantMap& out) const;

    void start();
    void stop();

    Action* addAction(const QVariantMap& data);
    void rmvAction(const int action);

    void setActionData(const int action, const QVariantMap& data);
    Q_INVOKABLE const QVariantMap getActionData(const int action);

    ActionPlugin& plugin() const;
    const QVariant property(const char* name) const;

protected:
    void addAction(Action& action);
    const QList<Action*>& actions() const;

private: // From QAbstractListModel
    int rowCount(const QModelIndex& parent = QModelIndex()) const;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;

private:
    ActionPlugin& mPlugin;

    QList<Action*> mActions;
    bool mRunning;
};

#endif // ACTIONGROUP_H

