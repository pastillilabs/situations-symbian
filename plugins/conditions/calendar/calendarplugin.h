#ifndef CALENDARPLUGIN_H
#define CALENDARPLUGIN_H

#include <conditionplugin.h>

/**
  * CalendarPlugin
  */
class CalendarPlugin
    : public QObject
    , public ConditionPlugin
{
private:
    Q_OBJECT
    Q_INTERFACES(ConditionPlugin)

public:
    CalendarPlugin();

private: // From ConditionPlugin
    Condition* createInstance(QObject* parent);
    void qmlRegisterTypes();
    const QVariant property(const char* name) const;
};

#endif // CALENDARPLUGIN_H

