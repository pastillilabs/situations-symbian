#include "locationcondition.h"
#include <QVariant>
#include <QTimerEvent>

static const int MINUPDATEINTERVAL(1000*60); // 30 s
static const int MAXUPDATEINTERVAL(1000*60*60); // 60 min

static const char* KEY_LATITUDE("latitude");
static const char* KEY_LONGITUDE("longitude");
static const char* KEY_RADIUS("radius");
static const char* KEY_ZOOMLEVEL("zoomLevel");
static const char* KEY_GPS("gps");

LocationEngine LocationCondition::mEngine;

LocationCondition::LocationCondition(QObject* parent)
    : Condition(parent)
    , mLatitude(60.1676)
    , mLongitude(24.9532)
    , mRadius(100)
    , mZoomLevel(15)
    , mGps(true)
{
#ifdef Q_OS_SYMBIAN
    mGps = false;
#endif
}

LocationCondition::~LocationCondition()
{
    doStop();
}

double LocationCondition::latitude() const
{
    return mLatitude;
}

void LocationCondition::setLatitude(double latitude)
{
    if(latitude != mLatitude)
    {
        mLatitude = latitude;
        emit(latitudeChanged(latitude));
    }
}

double LocationCondition::longitude() const
{
    return mLongitude;
}

void LocationCondition::setLongitude(double longitude)
{
    if(longitude != mLongitude)
    {
        mLongitude = longitude;
        emit(longitudeChanged(longitude));
    }
}

qreal LocationCondition::radius() const
{
    return mRadius;
}

void LocationCondition::setRadius(qreal radius)
{
    if(radius != mRadius)
    {
        mRadius = radius;
        emit(radiusChanged(radius));
    }
}

double LocationCondition::zoomLevel() const
{
    return mZoomLevel;
}

void LocationCondition::setZoomLevel(double zoomLevel)
{
    if(zoomLevel != mZoomLevel)
    {
        mZoomLevel = zoomLevel;
        emit(zoomLevelChanged(zoomLevel));
    }
}

bool LocationCondition::gps() const
{
   return mGps;
}

void LocationCondition::setGps(bool gps)
{
    if(gps != mGps)
    {
        mGps = gps;
        emit(gpsChanged(gps));
        setProperty("gps", QVariant(gps));
        mEngine.refresh();
    }
}

void LocationCondition::resolveActive(const QGeoPositionInfo& currentPosition)
{
    // Get distance to target border
    const QGeoCoordinate current(currentPosition.coordinate());
    const QGeoCoordinate target(mLatitude, mLongitude);
    const qreal distance(current.distanceTo(target) - mRadius);

    // Set update interval based on current distance to target
    int updateInterval = distance * 10;
    if(updateInterval < 0) updateInterval = -updateInterval;
    if(updateInterval < MINUPDATEINTERVAL) updateInterval = MINUPDATEINTERVAL;
    if(updateInterval > MAXUPDATEINTERVAL) updateInterval = MAXUPDATEINTERVAL;

    setProperty("updateInterval", QVariant(updateInterval));
    setActive(distance <= 0);
}

void LocationCondition::currentPositionChanged(const QGeoPositionInfo& currentPosition)
{
    resolveActive(currentPosition);
}

void LocationCondition::doStart()
{
    setProperty("updateInterval", QVariant(MINUPDATEINTERVAL));
    mEngine.start(*this);
    resolveActive(mEngine.currentPosition());
}

void LocationCondition::doStop()
{
    mEngine.stop(*this);
    setActive(false);
}

void LocationCondition::doStore(QVariantMap& data) const
{
    data[KEY_LATITUDE] = mLatitude;
    data[KEY_LONGITUDE] = mLongitude;
    data[KEY_RADIUS] = mRadius;
    data[KEY_ZOOMLEVEL] = mZoomLevel;
    data[KEY_GPS] = mGps;
}

void LocationCondition::doRestore(const QVariantMap& data)
{
    setLatitude(data[KEY_LATITUDE].toDouble());
    setLongitude(data[KEY_LONGITUDE].toDouble());
    setRadius(data[KEY_RADIUS].toDouble());
    setZoomLevel(data[KEY_ZOOMLEVEL].toInt());
    setGps(data[KEY_GPS].toBool());
}

