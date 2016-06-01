#ifndef NFCSCANNER_H
#define NFCSCANNER_H

#include <QObject>
#include <QVariant>
#include <QNearFieldManager>

QTM_USE_NAMESPACE

/**
  * NfcScanner
  */
class NfcScanner
    : public QObject
{
private:
    Q_OBJECT
    Q_PROPERTY(QVariant uid READ uid NOTIFY uidChanged)

public:
    explicit NfcScanner(QObject *parent = 0);
    ~NfcScanner();

    const QVariant uid() const;

    Q_INVOKABLE bool power() const;
    Q_INVOKABLE void powerOn();

signals:
    void uidChanged(const QVariant uid);

private slots:
    void targetDetected(QNearFieldTarget* target);
    void targetLost(QNearFieldTarget* target);

private:
    QNearFieldManager mNearFieldManager;
    QByteArray mUid;
};

#endif // NFCSCANNER_H

