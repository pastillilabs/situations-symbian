#ifndef SITUATION_H
#define SITUATION_H

#include "common_global.h"
#include "conditiongrouplistmodel.h"
#include "actiongrouplistmodel.h"
#include <QObject>

class PluginManager;
class ConditionPlugin;
class ActionPlugin;
class Condition;
class Action;

/**
  * Situation
  */
class COMMONSHARED_EXPORT Situation
    : public QObject
{
private:
    Q_OBJECT
    Q_DISABLE_COPY(Situation)

    Q_PROPERTY(QString locId READ locId NOTIFY locIdChanged)
    Q_PROPERTY(QString name READ name NOTIFY nameChanged)
    Q_PROPERTY(bool enabled READ enabled NOTIFY enabledChanged)
    Q_PROPERTY(bool active READ active NOTIFY activeChanged)

public:
    Situation(QObject* parent, const QString& locId, const QString& name);
    void initialize(const PluginManager& pluginManager);

    void operator<<(const QVariantMap& in);
    void operator>>(QVariantMap& out) const;

    void setLocId(const QString& locId);
    void setName(const QString& name);
    void setEnabled(bool enabled);
    void setActive(bool active);

    const QString& locId() const;
    const QString name() const;
    bool enabled() const;
    bool active() const;

    void start();
    void stop();
    void execute();
    void finish();

    Condition* addCondition(const int group, const QVariantMap& data);
    void rmvCondition(const int group, const int condition);
    void setConditionData(const int group, const int condition, const QVariantMap& data);

    Action* addAction(const int group, const QVariantMap& data);
    void rmvAction(const int group, const int action);
    void setActionData(const int group, const int action, const QVariantMap& data);

    void setConditionGroupNegative(const int group, const bool negative);

    ConditionGroupListModel& conditions();
    ActionGroupListModel& actions();

signals:
    void locIdChanged(const QString& locId);
    void nameChanged(const QString& name);
    void enabledChanged(bool enabled);
    void activeChanged(bool active);

private:
    void evaluate();
    void startConditions();
    void stopConditions();

private slots:
    void conditionGroupActiveChanged(bool active);

    void addConditionPlugin(ConditionPlugin& plugin, int index, const QString& name);
    void addActionPlugin(ActionPlugin& plugin, int index, const QString& name);
    void rmvConditionPlugin(ConditionPlugin& plugin, const QString& name);
    void rmvActionPlugin(ActionPlugin& plugin, const QString& name);

private:
    QString mLocId;
    QString mName;
    bool mEnabled;
    bool mActive;
    bool mRunning;

    ConditionGroupListModel mConditionGroups;
    ActionGroupListModel mActionGroups;
};

#endif // SITUATION_H

