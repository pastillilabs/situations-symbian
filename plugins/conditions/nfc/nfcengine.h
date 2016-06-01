#ifndef NFCENGINE_H
#define NFCENGINE_H

#include <QObject>

class NfcScanner;

/**
  * NfcEngine
  */
class NfcEngine
    : public QObject
{
private:
    Q_OBJECT

public:
    explicit NfcEngine(QObject* parent = 0);
    ~NfcEngine();

    void start(QObject& client);
    void stop(QObject& client);

    const QVariant uid() const;

private:
    QList<QObject*> mClients;

    NfcScanner* mScanner;
};

#endif // NFCENGINE_H

