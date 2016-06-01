#ifndef WLANPLUGIN_H
#define WLANPLUGIN_H

#include <actionplugin.h>

/**
  * WlanPlugin
  */
class WlanPlugin
    : public QObject
    , public ActionPlugin
{
private:
    Q_OBJECT
    Q_INTERFACES(ActionPlugin)

public:
    WlanPlugin();

private: // From ActionPlugin
    Action* createInstance(QObject* parent);
    void qmlRegisterTypes();
    const QVariant property(const char* name) const;
};

#endif // WLANPLUGIN_H

