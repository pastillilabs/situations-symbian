#ifndef TIMEPLUGIN_H
#define TIMEPLUGIN_H

#include <conditionplugin.h>

/**
  * TimePlugin
  */
class TimePlugin
    : public QObject
    , public ConditionPlugin
{
private:
    Q_OBJECT
    Q_INTERFACES(ConditionPlugin)

public:
    TimePlugin();

private: // From ConditionPlugin
    Condition* createInstance(QObject* parent);
    void qmlRegisterTypes();
    const QVariant property(const char* name) const;
};

#endif // TIMEPLUGIN_H

