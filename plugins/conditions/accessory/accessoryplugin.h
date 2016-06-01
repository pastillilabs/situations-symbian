#ifndef ACCESSORYPLUGIN_H
#define ACCESSORYPLUGIN_H

#include <conditionplugin.h>

/**
  * AccessoryPlugin
  */
class AccessoryPlugin
    : public QObject
    , public ConditionPlugin
{
private:
    Q_OBJECT
    Q_INTERFACES(ConditionPlugin)

public:
    AccessoryPlugin();

private: // From ConditionPlugin
    Condition* createInstance(QObject* parent);
    void qmlRegisterTypes();
    const QVariant property(const char* name) const;
};

#endif // ACCESSORYPLUGIN_H

