#ifndef SMSREPLYPLUGIN_H
#define SMSREPLYPLUGIN_H

#include <actionplugin.h>

/**
  * SmsReplyPlugin
  */
class SmsReplyPlugin
    : public QObject
    , public ActionPlugin
{
private:
    Q_OBJECT
    Q_INTERFACES(ActionPlugin)

public:
    SmsReplyPlugin();

private: // From ActionPlugin
    Action* createInstance(QObject* parent);
    void qmlRegisterTypes();
    const QVariant property(const char* name) const;
};

#endif // SMSREPLYPLUGIN_H

