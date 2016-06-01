#ifndef BTSCANNER_H
#define BTSCANNER_H

#include <QObject>
#include <QStringList>
#include <QBluetoothDeviceDiscoveryAgent>

#ifdef Q_OS_SYMBIAN
#include <bt_sock.h>
#endif

QTM_USE_NAMESPACE

/**
 * BtScanner
 */
class BtScanner
    : public QObject
#ifdef Q_OS_SYMBIAN
    , private MBluetoothPhysicalLinksNotifier
#endif
{
private:
    Q_OBJECT
    Q_PROPERTY(QStringList connections READ connections NOTIFY connectionsChanged)
    Q_PROPERTY(int count READ count NOTIFY countChanged)
    Q_PROPERTY(bool scanning READ scanning NOTIFY scanningChanged)

public:
    explicit BtScanner(QObject *parent = 0);
    ~BtScanner();

    const QStringList& connections() const;
    int count() const;
    bool scanning() const;

    Q_INVOKABLE void start();
    Q_INVOKABLE void stop();
    Q_INVOKABLE void clear();

    Q_INVOKABLE bool power() const;
    Q_INVOKABLE void powerOn();

signals:
    void connectionsChanged(const QStringList& connections);
    void countChanged(const int count);
    void scanningChanged(bool scanning);

private:
    void setScanning(bool scanning);

private slots:
    void deviceDiscovered(const QBluetoothDeviceInfo& info);
    void finished();
    void error(QBluetoothDeviceDiscoveryAgent::Error);

#ifdef Q_OS_SYMBIAN
private: // From MBluetoothPhysicalLinksNotifier
    void HandleCreateConnectionCompleteL(TInt aErr);
    void HandleDisconnectCompleteL(TInt aErr);
    void HandleDisconnectAllCompleteL(TInt aErr);
#endif

private:
    QStringList mConnections;
    bool mScanning;

    QBluetoothDeviceDiscoveryAgent* mBtDiscoveryAgent;
};

#endif // BTSCANNER_H

