#ifndef ACTIONPLUGIN_H
#define ACTIONPLUGIN_H

#include <QtPlugin>

class Action;

/**
  * ActionPlugin
  */
class ActionPlugin
{
public:
    virtual ~ActionPlugin() {}

    virtual Action* createInstance(QObject* parent) = 0;
    virtual void qmlRegisterTypes() = 0;
    virtual const QVariant property(const char* name) const = 0;
};

Q_DECLARE_INTERFACE(ActionPlugin, "com.pastillilabs.situations.action/1.0")

#endif // ACTIONPLUGIN_H

