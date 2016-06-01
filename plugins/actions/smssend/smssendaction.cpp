#include "smssendaction.h"
#include "smssender.h"
#include <identifiers.h>
#include <QContactManager>
#include <QContactName>

QTM_USE_NAMESPACE

static const char* KEY_CONTACT_ID("contactId");
static const char* KEY_CONTACT_NUMBER("contactNumber");

SmsSendAction::SmsSendAction(QObject* parent)
    : Action(parent)
    , mContactId(0)
    , mSituationEnd(false)
{
    setValue(qtTrId("action_sms_send_default_message"));
}

SmsSendAction::~SmsSendAction()
{
}

void SmsSendAction::setContactId(int contactId)
{
    if(contactId != mContactId)
    {
        mContactId = contactId;
        emit contactIdChanged(contactId);
    }
}

int SmsSendAction::contactId() const
{
    return mContactId;
}

void SmsSendAction::setContactNumber(const QString contactNumber)
{
    if(contactNumber != mContactNumber)
    {
        mContactNumber = contactNumber;
        emit contactNumberChanged(contactNumber);
    }
}

const QString SmsSendAction::contactNumber() const
{
    return mContactNumber;
}

void SmsSendAction::setSituationEnd(bool situationEnd)
{
    if(situationEnd != mSituationEnd)
    {
        mSituationEnd = situationEnd;
        emit situationEndChanged(situationEnd);
    }
}

bool SmsSendAction::situationEnd() const
{
    return mSituationEnd;
}

QString SmsSendAction::nameById(const int id) const
{
    QString name;

    // Get name for contact id
    if(id)
    {
        QContactManager contactManager;
        QContactName contactName(contactManager.contact(id).detail<QContactName>());
        if(contactName.lastName().isEmpty() || contactName.firstName().isEmpty())
        {
            name = contactName.firstName() + contactName.lastName();
        }
        else
        {
            name = qtTrId("action_sms_send_recipient").arg(contactName.firstName()).arg(contactName.lastName());
        }
    }

    return name;
}

void SmsSendAction::doStart(bool running)
{
    if(!mSituationEnd && !running)
    {
        SmsSender* sender(new SmsSender());
        sender->send(value().toString(), mContactId, mContactNumber);
    }
}

void SmsSendAction::doStop(bool running)
{
    if(mSituationEnd && !running)
    {
        SmsSender* sender(new SmsSender());
        sender->send(value().toString(), mContactId, mContactNumber);
    }
}

void SmsSendAction::doGetData(QVariantMap& data) const
{
    data[KEY_CONTACT_ID] = mContactId;
    data[KEY_CONTACT_NUMBER] = mContactNumber;
    data[Identifiers::keySituationEnd] = mSituationEnd;
}

void SmsSendAction::doSetData(const QVariantMap& data)
{
    setContactId(data[KEY_CONTACT_ID].toInt());
    setContactNumber(data[KEY_CONTACT_NUMBER].toString());
    setSituationEnd(data[Identifiers::keySituationEnd].toBool());
}

