#ifndef CONDITIONGROUP_H
#define CONDITIONGROUP_H

#include "common_global.h"
#include "conditionplugin.h"
#include <QAbstractListModel>

class Condition;

/**
  * ConditionGroup
  */
class COMMONSHARED_EXPORT ConditionGroup
    : public QAbstractListModel
{
private:
    Q_OBJECT
    Q_DISABLE_COPY(ConditionGroup)
    Q_PROPERTY(bool negative READ negative WRITE setNegative NOTIFY negativeChanged)

    enum Roles {
        ConditionRole = Qt::UserRole + 1
    };

public:
    ConditionGroup(ConditionPlugin& plugin, QObject* parent = 0);
    virtual ~ConditionGroup();

    void operator<<(const QVariantMap& in);
    void operator>>(QVariantMap& out) const;

    void start();
    void stop();

    bool running() const;
    bool active() const;
    bool empty() const;

    bool negative() const;
    void setNegative(bool negative);

    Condition* addCondition(const QVariantMap& data);
    void rmvCondition(const int condition);

    void setConditionData(const int condition, const QVariantMap& data);
    Q_INVOKABLE const QVariantMap getConditionData(const int condition);

    ConditionPlugin& plugin() const;
    const QVariant property(const char* name) const;

signals:
    void runningChanged(bool running);
    void activeChanged(bool active);
    void negativeChanged(bool negative);

private:
    void addCondition(Condition& condition);
    const QList<Condition*>& conditions() const;

    void evaluate();
    void setActive(bool active);

private slots:
    void conditionActiveChanged(bool active);

private: // From QAbstractListModel
    int rowCount(const QModelIndex& parent = QModelIndex()) const;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;

private:
    ConditionPlugin& mPlugin;
    bool mRunning;
    bool mActive;
    bool mNegative;

    QList<Condition*> mConditions;
};

#endif // CONDITIONGROUP_H

