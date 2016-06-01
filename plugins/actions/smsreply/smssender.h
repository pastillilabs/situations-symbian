#ifndef SMSSENDER_H
#define SMSSENDER_H

#include <QObject>
#include <QThread>
#include <QMessageService>

QTM_USE_NAMESPACE

/**
  * SmsSender
  */
class SmsSender
    : public QObject
{
private:
    Q_OBJECT

public:
    explicit SmsSender(QObject* parent = 0);

    void send(const QString& message, const QString& number);

private slots:
    void started();
    void finished();
    void stateChanged(QMessageService::State);

private:
    QThread mThread;

    QString mMessage;
    QString mNumber;
    QMessageService* mMessageService;
};

#endif // SMSSENDER_H

