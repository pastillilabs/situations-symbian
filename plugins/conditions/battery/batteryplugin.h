#ifndef BATTERYPLUGIN_H
#define BATTERYPLUGIN_H

#include <conditionplugin.h>

/**
  * BatteryPlugin
  */
class BatteryPlugin
    : public QObject
    , public ConditionPlugin
{
private:
    Q_OBJECT
    Q_INTERFACES(ConditionPlugin)

public:
    BatteryPlugin();

private: // From ConditionPlugin
    Condition* createInstance(QObject* parent);
    void qmlRegisterTypes();
    const QVariant property(const char* name) const;
};

#endif // BATTERYPLUGIN_H

