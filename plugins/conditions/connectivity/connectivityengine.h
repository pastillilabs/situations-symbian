#ifndef CONNECTIVITYENGINE_H
#define CONNECTIVITYENGINE_H

#include <QObject>

class WlanScanner;
class BtScanner;

/**
 * ConnectivityEngine
 */
class ConnectivityEngine
    : public QObject
{
private:
    Q_OBJECT

public:
    explicit ConnectivityEngine(QObject *parent = 0);
    ~ConnectivityEngine();

    void startWlan(QObject& client);
    void stopWlan(QObject& client);
    void doneWlan();
    bool scanningWlan() const;

    void startBt(QObject& client);
    void stopBt(QObject& client);
    void doneBt();
    bool scanningBt() const;

    const QStringList wlanConnections() const;
    const QStringList btConnections() const;

private: // From QObject
    void timerEvent(QTimerEvent* event);

private:
    QList<QObject*> mWlanClients;
    QList<QObject*> mBtClients;

    WlanScanner* mWlanScanner;
    BtScanner* mBtScanner;

    int mWlanDoneCounter;
    int mBtDoneCounter;

    int mWlanTimerId;
    int mBtTimerId;
};

#endif // CONNECTIVITYENGINE_H

