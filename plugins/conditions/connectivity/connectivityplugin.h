#ifndef CONNECTIVITYPLUGIN_H
#define CONNECTIVITYPLUGIN_H

#include <conditionplugin.h>

/**
  * ConnectivityPlugin
  */
class ConnectivityPlugin
    : public QObject
    , public ConditionPlugin
{
private:
    Q_OBJECT
    Q_INTERFACES(ConditionPlugin)

public:
    ConnectivityPlugin();

private: // From ConditionPlugin
    Condition* createInstance(QObject* parent);
    void qmlRegisterTypes();
    const QVariant property(const char* name) const;
};

#endif // CONNECTIVITYPLUGIN_H

