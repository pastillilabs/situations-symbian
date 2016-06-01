#ifndef LOCATIONCONDITION_H
#define LOCATIONCONDITION_H

#include "locationengine.h"
#include <condition.h>
#include <QTimer>

/**
  * LocationCondition
  */
class LocationCondition
    : public Condition
{
private:
    Q_OBJECT

    Q_PROPERTY(double latitude READ latitude WRITE setLatitude NOTIFY latitudeChanged)
    Q_PROPERTY(double longitude READ longitude WRITE setLongitude NOTIFY longitudeChanged)
    Q_PROPERTY(qreal radius READ radius WRITE setRadius NOTIFY radiusChanged)
    Q_PROPERTY(double zoomLevel READ zoomLevel WRITE setZoomLevel NOTIFY zoomLevelChanged)
    Q_PROPERTY(bool gps READ gps WRITE setGps NOTIFY gpsChanged)

public:
    LocationCondition(QObject* parent = 0);
    ~LocationCondition();

    double latitude() const;
    void setLatitude(double latitude);
    double longitude() const;
    void setLongitude(double longitude);
    qreal radius() const;
    void setRadius(qreal radius);
    double zoomLevel() const;
    void setZoomLevel(double zoomLevel);
    bool gps() const;
    void setGps(bool gps);

signals:
    void latitudeChanged(double latitude);
    void longitudeChanged(double longitude);
    void radiusChanged(qreal radius);
    void zoomLevelChanged(double zoomLevel);
    void gpsChanged(bool gps);

private:
    void resolveActive(const QGeoPositionInfo& currentPosition);

private slots:
    void currentPositionChanged(const QGeoPositionInfo& currentPosition);

private: // From Condition
    void doStart();
    void doStop();
    void doStore(QVariantMap& data) const;
    void doRestore(const QVariantMap& data);

private:
    double mLatitude;
    double mLongitude;
    qreal mRadius;
    double mZoomLevel;
    bool mGps;

    static LocationEngine mEngine;
};

#endif // LOCATIONCONDITION_H

