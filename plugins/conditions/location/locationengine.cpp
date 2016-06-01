#include "locationengine.h"
#include <QVariant>

static const int FALLBACKINTERVAL(1000*60*60); // 60 min

LocationEngine::LocationEngine(QObject* parent)
    : QObject(parent)
    , mLocationSource(0)
{
}

LocationEngine::~LocationEngine()
{
    delete mLocationSource;
}

void LocationEngine::start(QObject& client)
{
    if(!mClients.contains(&client))
    {
        mClients.append(&client);
        connect(this, SIGNAL(currentPositionChanged(QGeoPositionInfo)), &client, SLOT(currentPositionChanged(QGeoPositionInfo)));

        if(!mLocationSource)
        {
            mLocationSource = new LocationSource(this);
            connect(mLocationSource, SIGNAL(positionChanged(QGeoPositionInfo)), this, SLOT(positionChanged(QGeoPositionInfo)));
        }

        refresh();
    }
}

void LocationEngine::stop(QObject& client)
{
    mClients.removeAll(&client);
    disconnect(this, SIGNAL(currentPositionChanged(QGeoPositionInfo)), &client, SLOT(currentPositionChanged(QGeoPositionInfo)));

    if(mClients.isEmpty())
    {
        if(mLocationSource) mLocationSource->stop();
        delete mLocationSource;
        mLocationSource = 0;
    }
    else
    {
        refresh();
    }
}

void LocationEngine::refresh()
{
    if(mLocationSource)
    {
        updateInterval();
        mLocationSource->start();
    }
}

const QGeoPositionInfo LocationEngine::currentPosition() const
{    
    QGeoPositionInfo currentPosition;
    if(mLocationSource) currentPosition = mLocationSource->position();
    return currentPosition;
}

void LocationEngine::updateInterval()
{
    // Refresh update interval
    int updateInterval(FALLBACKINTERVAL);
    bool gps(false);
    foreach(const QObject* client, mClients)
    {
        const int current(client->property("updateInterval").toInt());
        if(current > 0 && current < updateInterval) updateInterval = current;
        if(client->property("gps").toBool()) gps = true;
    }

    mLocationSource->setGps(gps);
    mLocationSource->setUpdateInterval(updateInterval);
}

void LocationEngine::positionChanged(const QGeoPositionInfo& position)
{
    if(position.isValid())
    {
        emit currentPositionChanged(position);
        updateInterval();
    }
}

