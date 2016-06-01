#ifndef LOCATIONSOURCE_H
#define LOCATIONSOURCE_H

#include <QObject>
#include <QGeoPositionInfoSource>
#include <QGeoPositionInfo>

QTM_USE_NAMESPACE

/**
  * LocationSource
  */
class LocationSource
    : public QObject
{
private:
    Q_OBJECT
    Q_PROPERTY(double latitude READ latitude NOTIFY latitudeChanged)
    Q_PROPERTY(double longitude READ longitude NOTIFY longitudeChanged)
    Q_PROPERTY(double accuracy READ accuracy NOTIFY accuracyChanged)
    Q_PROPERTY(bool valid READ valid NOTIFY validChanged)
    Q_PROPERTY(bool gps READ gps WRITE setGps NOTIFY gpsChanged)
    Q_PROPERTY(int updateInterval READ updateInterval WRITE setUpdateInterval NOTIFY updateIntervalChanged)

public:
    explicit LocationSource(QObject* parent = 0);
    ~LocationSource();

    double latitude() const;
    double longitude() const;
    double accuracy() const;
    bool valid() const;
    bool gps() const;
    int updateInterval() const;

    void setGps(bool gps);
    void setUpdateInterval(int udpdateInterval);

    Q_INVOKABLE void start();
    Q_INVOKABLE void stop();

    const QGeoPositionInfo position() const;

signals:
    void latitudeChanged(double latitude);
    void longitudeChanged(double latitude);
    void accuracyChanged(double accuracy);
    void validChanged(bool valid);
    void gpsChanged(bool gps);
    void updateIntervalChanged(int updateInterval);

    void positionChanged(const QGeoPositionInfo& position);

private: // From QObject
    void timerEvent(QTimerEvent* event);

private slots:
    void positionUpdated(const QGeoPositionInfo& update);
    void updateTimeout();

private:
    QGeoPositionInfoSource* mGeoPositionInfoSource;
    bool mGps;
    int mUpdateInterval;
    int mUpdateTimer;
};

#endif // LOCATIONSOURCE_H
