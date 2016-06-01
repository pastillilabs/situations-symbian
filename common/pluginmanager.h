#ifndef PLUGINMANAGER_H
#define PLUGINMANAGER_H

#include "common_global.h"
#include <QObject>
#include <QStringList>
#include <QVariantMap>

class ConditionPlugin;
class ActionPlugin;
class TranslationManager;

/**
  * PluginManager
  */
class COMMONSHARED_EXPORT PluginManager
    : public QObject
{
private:
    Q_OBJECT
    Q_DISABLE_COPY(PluginManager)

public:
    explicit PluginManager(QObject* parent = 0);

    void operator<<(const QVariantMap& in);
    void operator>>(QVariantMap& out) const;

    int addCondition(const QString& name);
    int addAction(const QString& name);

    int rmvCondition(const QString& name);
    int rmvAction(const QString& name);

    const QStringList conditions() const;
    const QStringList actions() const;

    const QList<ConditionPlugin*>& conditionPlugins() const;
    const QList<ActionPlugin*>& actionPlugins() const;

signals:
    void addCondition(ConditionPlugin& plugin, int index, const QString& name);
    void addAction(ActionPlugin& plugin, int index, const QString& name);
    void rmvCondition(ConditionPlugin& plugin, const QString& name);
    void rmvAction(ActionPlugin& plugin, const QString& name);

private:
    ConditionPlugin* loadCondition(const QString& name);
    ActionPlugin* loadAction(const QString& name);
    QObject* loadPlugin(const QString& name);

    int conditionIndex(const QString& name) const;
    int actionIndex(const QString& name) const;

private:
    QList<ConditionPlugin*> mConditionPlugins;
    QList<ActionPlugin*> mActionPlugins;
};

#endif // PLUGINMANAGER_H

