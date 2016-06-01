#include "smssender.h"
#include <QContactDetailFilter>
#include <QContactManager>
#include <QContactPhoneNumber>
#include <QContactName>
#include <QMessage>

SmsSender::SmsSender(QObject* parent)
    : QObject(parent)
    , mContactId(0)
{
}

void SmsSender::send(const QString& message, const int contactId, const QString contactNumber)
{
    mMessage = message;
    mContactId = contactId;
    mContactNumber = contactNumber;

    started();
}

void SmsSender::started()
{
    if(mMessage.size())
    {
        if(mContactNumber.isEmpty())
        {
            // Get number for contact id
            QContactManager contactManager;
            const QContact contact(contactManager.contact(mContactId));
            const QList<QContactPhoneNumber> numbers(contact.details<QContactPhoneNumber>());

            QStringList mobileNumbers;
            foreach(const QContactPhoneNumber& number, numbers)
            {
                if(number.subTypes().contains(QContactPhoneNumber::SubTypeMobile)) mobileNumbers.append(number.number());
            }

            // Pick first mobile number
            if(mobileNumbers.count()) mContactNumber = mobileNumbers.at(0);
        }

        // Create message
        QMessage message;
        message.setType(QMessage::Sms);
        message.setTo(QMessageAddress(QMessageAddress::Phone, mContactNumber));
        message.setBody(mMessage);

        // Send it
        mMessageService = new QMessageService(this);
        connect(mMessageService, SIGNAL(stateChanged(QMessageService::State)), this, SLOT(stateChanged(QMessageService::State)));
        mMessageService->send(message);
    }
}

void SmsSender::finished()
{
    delete this;
}

void SmsSender::stateChanged(QMessageService::State state)
{
    if(state == QMessageService::FinishedState)
    {
        deleteLater();
    }
}

