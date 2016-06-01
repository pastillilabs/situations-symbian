#ifndef LAUNCHPLUGIN_H
#define LAUNCHPLUGIN_H

#include <actionplugin.h>

/**
  * LaunchPlugin
  */
class LaunchPlugin
    : public QObject
    , public ActionPlugin
{
private:
    Q_OBJECT
    Q_INTERFACES(ActionPlugin)

public:
    LaunchPlugin();

private: // From ActionPlugin
    Action* createInstance(QObject* parent);
    void qmlRegisterTypes();
    const QVariant property(const char* name) const;
};

#endif // LAUNCHPLUGIN_H

