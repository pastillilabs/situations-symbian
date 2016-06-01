#include "connectivityengine.h"
#include "wlanscanner.h"
#include "btscanner.h"
#include <QDateTime>
#include <QTimerEvent>

static const int SECONDSINMINUTE(60);
static const int MSINSECOND(1000);
static const int BTTIMEROFFSETSECS(30);

ConnectivityEngine::ConnectivityEngine(QObject* parent)
    : QObject(parent)
    , mWlanScanner(0)
    , mBtScanner(0)
    , mWlanDoneCounter(0)
    , mBtDoneCounter(0)
    , mWlanTimerId(0)
    , mBtTimerId(0)
{
}

ConnectivityEngine::~ConnectivityEngine()
{
}

void ConnectivityEngine::startWlan(QObject& client)
{
    if(!mWlanClients.contains(&client))
    {
        mWlanClients.append(&client);

        if(!mWlanScanner)
        {
            mWlanScanner = new WlanScanner(this);
            mWlanScanner->start();

            const QDateTime current(QDateTime::currentDateTime());
            mWlanTimerId = startTimer((SECONDSINMINUTE - current.time().second()) * MSINSECOND);
        }

        connect(mWlanScanner, SIGNAL(connectionsChanged(QStringList)), &client, SLOT(connectionsChanged(QStringList)));
    }
}

void ConnectivityEngine::stopWlan(QObject& client)
{
    mWlanClients.removeAll(&client);
    if(mWlanScanner) disconnect(mWlanScanner, SIGNAL(connectionsChanged(QStringList)), &client, SLOT(connectionsChanged(QStringList)));

    if(mWlanClients.isEmpty())
    {
        if(mWlanTimerId) killTimer(mWlanTimerId);
        if(mWlanScanner) mWlanScanner->stop();
        delete mWlanScanner;
        mWlanScanner = 0;
    }
}

void ConnectivityEngine::doneWlan()
{
    if(mWlanScanner && mWlanScanner->scanning())
    {
        ++mWlanDoneCounter;
        if(mWlanDoneCounter == mWlanClients.count()) mWlanScanner->stop();
    }
}

bool ConnectivityEngine::scanningWlan() const
{
    return mWlanScanner ? mWlanScanner->scanning() : false;
}

void ConnectivityEngine::startBt(QObject& client)
{
    if(!mBtClients.contains(&client))
    {
        mBtClients.append(&client);

        if(!mBtScanner)
        {
            mBtScanner = new BtScanner(this);
            mBtScanner->start();

            const QDateTime current(QDateTime::currentDateTime());
            mBtTimerId = startTimer((SECONDSINMINUTE - current.time().second() + BTTIMEROFFSETSECS) * MSINSECOND);
        }

        connect(mBtScanner, SIGNAL(connectionsChanged(QStringList)), &client, SLOT(connectionsChanged(QStringList)));
    }
}

void ConnectivityEngine::stopBt(QObject& client)
{
    mBtClients.removeAll(&client);
    if(mBtScanner) disconnect(mBtScanner, SIGNAL(connectionsChanged(QStringList)), &client, SLOT(connectionsChanged(QStringList)));

    if(mBtClients.isEmpty())
    {
        if(mBtTimerId) killTimer(mBtTimerId);
        if(mBtScanner) mBtScanner->stop();
        delete mBtScanner;
        mBtScanner = 0;
    }
}

void ConnectivityEngine::doneBt()
{
    if(mBtScanner && mBtScanner->scanning())
    {
        ++mBtDoneCounter;
        if(mBtDoneCounter == mBtClients.count()) mBtScanner->stop();
    }
}

bool ConnectivityEngine::scanningBt() const
{
    return mBtScanner ? mBtScanner->scanning() : false;
}

const QStringList ConnectivityEngine::wlanConnections() const
{
    if(mWlanScanner) return mWlanScanner->connections();
    return QStringList();
}

const QStringList ConnectivityEngine::btConnections() const
{
    if(mBtScanner) return mBtScanner->connections();
    return QStringList();
}

void ConnectivityEngine::timerEvent(QTimerEvent* event)
{
    const int timerId(event->timerId());
    killTimer(timerId);
    const QDateTime current(QDateTime::currentDateTime());

    if(timerId == mWlanTimerId)
    {
        mWlanDoneCounter = 0;
        mWlanScanner->start();
        mWlanTimerId = startTimer((SECONDSINMINUTE - current.time().second()) * MSINSECOND);
    }

    if(timerId == mBtTimerId)
    {
        mBtDoneCounter = 0;
        mBtScanner->start();
        mBtTimerId = startTimer((SECONDSINMINUTE - current.time().second() + BTTIMEROFFSETSECS) * MSINSECOND);
    }
}

