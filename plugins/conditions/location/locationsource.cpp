#include "locationsource.h"

static const int MIN_INTERVAL(1000);

LocationSource::LocationSource(QObject* parent)
    : QObject(parent)
    , mGeoPositionInfoSource(0)
    , mGps(true)
    , mUpdateInterval(MIN_INTERVAL)
    , mUpdateTimer(0)
{
#ifdef Q_OS_SYMBIAN
    mGps = false;
#endif
}

LocationSource::~LocationSource()
{
    stop();
}

double LocationSource::latitude() const
{
    double latitude(60.1676);
    if(mGeoPositionInfoSource) latitude = mGeoPositionInfoSource->lastKnownPosition().coordinate().latitude();
    return latitude;
}

double LocationSource::longitude() const
{
    double longitude(24.9532);
    if(mGeoPositionInfoSource) longitude = mGeoPositionInfoSource->lastKnownPosition().coordinate().longitude();
    return longitude;
}

double LocationSource::accuracy() const
{
    double accuracy(0);
    if(mGeoPositionInfoSource) accuracy = mGeoPositionInfoSource->lastKnownPosition().attribute(QGeoPositionInfo::HorizontalAccuracy);
    return accuracy;
}

bool LocationSource::valid() const
{
    bool valid(false);
    if(mGeoPositionInfoSource) valid = mGeoPositionInfoSource->lastKnownPosition().isValid();
    return valid;
}

bool LocationSource::gps() const
{
    return mGps;
}

int LocationSource::updateInterval() const
{
    return mUpdateInterval;
}

void LocationSource::setGps(bool gps)
{
    if(gps != mGps)
    {
        if(mUpdateTimer)
        {
            if(mGeoPositionInfoSource) mGeoPositionInfoSource->stopUpdates();
            delete mGeoPositionInfoSource;
            mGeoPositionInfoSource = 0;

            killTimer(mUpdateTimer);
            mUpdateTimer = startTimer(MIN_INTERVAL);
        }

        mGps = gps;
        emit gpsChanged(gps);
    }
}

void LocationSource::setUpdateInterval(int updateInterval)
{
    if(updateInterval != mUpdateInterval)
    {
        mUpdateInterval = updateInterval;
        emit updateIntervalChanged(updateInterval);

        if(mUpdateTimer)
        {
            killTimer(mUpdateTimer);
            mUpdateTimer = startTimer(updateInterval);
        }
    }
}

void LocationSource::start()
{
    if(!mGeoPositionInfoSource)
    {
        mGeoPositionInfoSource = QGeoPositionInfoSource::createDefaultSource(this);
        connect(mGeoPositionInfoSource, SIGNAL(positionUpdated(QGeoPositionInfo)), this, SLOT(positionUpdated(QGeoPositionInfo)));
        connect(mGeoPositionInfoSource, SIGNAL(updateTimeout()), this, SLOT(updateTimeout()));
        mGeoPositionInfoSource->setPreferredPositioningMethods(mGps ? QGeoPositionInfoSource::AllPositioningMethods : QGeoPositionInfoSource::NonSatellitePositioningMethods);
    }

    mGeoPositionInfoSource->requestUpdate();

    if(mUpdateTimer) killTimer(mUpdateTimer);
    mUpdateTimer = startTimer(mUpdateInterval);
}

void LocationSource::stop()
{
    if(mGeoPositionInfoSource) mGeoPositionInfoSource->stopUpdates();
    delete mGeoPositionInfoSource;
    mGeoPositionInfoSource = 0;

    if(mUpdateTimer) killTimer(mUpdateTimer);
    mUpdateTimer = 0;
}

const QGeoPositionInfo LocationSource::position() const
{
    QGeoPositionInfo position;
    if(mGeoPositionInfoSource) position = mGeoPositionInfoSource->lastKnownPosition();
    return position;
}

void LocationSource::timerEvent(QTimerEvent* /*event*/)
{
    start();
}

void LocationSource::positionUpdated(const QGeoPositionInfo& update)
{
    if(mUpdateInterval > MIN_INTERVAL)
    {
        mGeoPositionInfoSource->stopUpdates();
#ifdef Q_OS_SYMBIAN
        delete mGeoPositionInfoSource;
        mGeoPositionInfoSource = 0;
#endif
    }

    emit latitudeChanged(update.coordinate().latitude());
    emit longitudeChanged(update.coordinate().longitude());
    emit accuracyChanged(update.attribute(QGeoPositionInfo::HorizontalAccuracy));
    emit validChanged(update.isValid());
    emit positionChanged(update);
}

void LocationSource::updateTimeout()
{
    mGeoPositionInfoSource->requestUpdate();
}

