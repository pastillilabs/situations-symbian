#ifndef CONDITIONPLUGIN_H
#define CONDITIONPLUGIN_H

#include <QtPlugin>

class Condition;

/**
  * ConditionPlugin
  */
class ConditionPlugin
{
public:
    virtual ~ConditionPlugin() {}

    virtual Condition* createInstance(QObject* parent) = 0;
    virtual void qmlRegisterTypes() = 0;
    virtual const QVariant property(const char* name) const = 0;
};

Q_DECLARE_INTERFACE(ConditionPlugin, "com.pastillilabs.situations.condition/1.0")

#endif // CONDITIONPLUGIN_H

