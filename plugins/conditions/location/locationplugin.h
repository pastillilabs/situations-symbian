#ifndef LOCATIONPLUGIN_H
#define LOCATIONPLUGIN_H

#include <conditionplugin.h>

/**
  * LocationPlugin
  */
class LocationPlugin
    : public QObject
    , public ConditionPlugin
{
private:
    Q_OBJECT
    Q_INTERFACES(ConditionPlugin)

public:
    LocationPlugin();

private: // From ConditionPlugin
    Condition* createInstance(QObject* parent);
    void qmlRegisterTypes();
    const QVariant property(const char* name) const;
};

#endif // LOCATIONPLUGIN_H

