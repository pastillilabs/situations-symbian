#include "smsreplyaction.h"
#include "smssender.h"
#include <identifiers.h>
#include <QContactManager>
#include <QContactDetailFilter>
#include <QContactPhoneNumber>

static const char* KEY_ONLY_CONTACTS("onlyContacts");

SmsReplyAction::SmsReplyAction(QObject* parent)
    : Action(parent)
    , mOnlyContacts(true)
#ifdef Q_OS_SYMBIAN
    , mMissedCalls(*this)
#endif
{
    setValue(qtTrId("smsreply_default_answer"));
}

SmsReplyAction::~SmsReplyAction()
{
}

bool SmsReplyAction::onlyContacts() const
{
    return mOnlyContacts;
}

void SmsReplyAction::setOnlyContacts(bool onlyContacts)
{
    if(onlyContacts != mOnlyContacts)
    {
        mOnlyContacts = onlyContacts;
        emit(onlyContactsChanged(onlyContacts));
    }
}

void SmsReplyAction::reply(const QString& number)
{
    bool sendMessage(true);

    if(mOnlyContacts)
    {
        QContactManager cm;

        // Specify search filter
        QContactDetailFilter filter;
        filter.setDetailDefinitionName(QContactPhoneNumber::DefinitionName, QContactPhoneNumber::FieldNumber);
        filter.setMatchFlags(QContactFilter::MatchPhoneNumber);
        filter.setValue(number);

        // Search for phone number matches in contacts
        QList<QContact> matches(cm.contacts(filter));
        if(matches.count()) sendMessage = true;
        else sendMessage = false;
    }

    if(sendMessage)
    {
        SmsSender* sender(new SmsSender());
        sender->send(value().toString(), number);
    }
}

void SmsReplyAction::doStart(bool /*running*/)
{
#ifdef Q_OS_SYMBIAN
    mMissedCalls.start();
#endif
}

void SmsReplyAction::doStop(bool /*running*/)
{
#ifdef Q_OS_SYMBIAN
    mMissedCalls.stop();
#endif
}

void SmsReplyAction::doGetData(QVariantMap& data) const
{
    data[KEY_ONLY_CONTACTS] = mOnlyContacts;
}

void SmsReplyAction::doSetData(const QVariantMap& data)
{
    setOnlyContacts(data[KEY_ONLY_CONTACTS].toBool());
}

#ifdef Q_OS_SYMBIAN
void SmsReplyAction::handleMissedCall(QString number)
{
    reply(number);
}
#endif

