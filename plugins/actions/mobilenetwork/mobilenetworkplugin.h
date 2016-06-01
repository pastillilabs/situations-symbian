#ifndef MOBILENETWORKPLUGIN_H
#define MOBILENETWORKPLUGIN_H

#include <actionplugin.h>

/**
  * MobileNetworkPlugin
  */
class MobileNetworkPlugin
    : public QObject
    , public ActionPlugin
{
private:
    Q_OBJECT
    Q_INTERFACES(ActionPlugin)

public:
    MobileNetworkPlugin();

private: // From ActionPlugin
    Action* createInstance(QObject* parent);
    void qmlRegisterTypes();
    const QVariant property(const char* name) const;
};

#endif // MOBILENETWORKPLUGIN_H

