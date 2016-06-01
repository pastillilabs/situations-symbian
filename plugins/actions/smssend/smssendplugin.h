#ifndef SMSSENDPLUGIN_H
#define SMSSENDPLUGIN_H

#include <actionplugin.h>

/**
  * SmsSendPlugin
  */
class SmsSendPlugin
    : public QObject
    , public ActionPlugin
{
private:
    Q_OBJECT
    Q_INTERFACES(ActionPlugin)

public:
    SmsSendPlugin();

private: // From ActionPlugin
    Action* createInstance(QObject* parent);
    void qmlRegisterTypes();
    const QVariant property(const char* name) const;
};

#endif // SMSSENDPLUGIN_H

