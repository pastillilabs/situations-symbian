#ifndef SMSREPLYACTION_H
#define SMSREPLYACTION_H

#include <action.h>

#ifdef Q_OS_SYMBIAN
#include "missedcallssymbian.h"
#endif

/**
  * SmsReplyAction
  */
class SmsReplyAction
    : public Action
#ifdef Q_OS_SYMBIAN
    , private MissedCalls::Observer
#endif
{
private:
    Q_OBJECT
    Q_PROPERTY(bool onlyContacts READ onlyContacts WRITE setOnlyContacts NOTIFY onlyContactsChanged)

public:
    explicit SmsReplyAction(QObject* parent = 0);
    ~SmsReplyAction();

    bool onlyContacts() const;
    void setOnlyContacts(bool onlyContacts);

signals:
    void onlyContactsChanged(bool onlyContacts);

private:
    void reply(const QString& number);

private: // From Action
    void doStart(bool running);
    void doStop(bool running);

    void doGetData(QVariantMap& data) const;
    void doSetData(const QVariantMap& data);

#ifdef Q_OS_SYMBIAN
private: // From MissedCalls::Observer
    void handleMissedCall(const QString number);
#endif

private:
    bool mOnlyContacts;

#ifdef Q_OS_SYMBIAN
    MissedCalls mMissedCalls;
#endif
};

#endif // SMSREPLYACTION_H

