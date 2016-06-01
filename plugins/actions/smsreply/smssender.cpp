#include "smssender.h"
#include <QMessage>

SmsSender::SmsSender(QObject* parent)
    : QObject(parent)
{
}

void SmsSender::send(const QString& message, const QString& number)
{
    mMessage = message;
    mNumber = number;

    started();
}

void SmsSender::started()
{
    if(mMessage.size())
    {
        // Create message
        QMessage message;
        message.setType(QMessage::Sms);
        message.setTo(QMessageAddress(QMessageAddress::Phone, mNumber));
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

