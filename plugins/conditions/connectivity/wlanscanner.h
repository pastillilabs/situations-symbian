#ifndef WLANSCANNER_H
#define WLANSCANNER_H

#include <QObject>
#include <QStringList>

#ifdef Q_OS_SYMBIAN
#include "wlanscanner_symbian.h"
class CWlanState;
#endif

/**
  * WlanScanner
  */
class WlanScanner
    : public QObject
#ifdef Q_OS_SYMBIAN
    , private CWlanScanner::Observer
#endif
{
private:
    Q_OBJECT
    Q_PROPERTY(QStringList connections READ connections NOTIFY connectionsChanged)
    Q_PROPERTY(int count READ count NOTIFY countChanged)
    Q_PROPERTY(bool scanning READ scanning NOTIFY scanningChanged)

public:
    explicit WlanScanner(QObject* parent = NULL);
    ~WlanScanner();

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

#ifdef Q_OS_SYMBIAN
private: // From CWlanScanner::Observer
    void WlanScanComplete(const RArray<TWlanSsid>& wlanArray);
    void NotifyError(const TInt aError);
#endif

private:
    QStringList mConnections;
    bool mScanning;

#ifdef Q_OS_SYMBIAN
    CWlanState* mWlanState;
    CWlanScanner* mWlanScanner;
#endif
};

#endif // WLANSCANNER_H

