#ifndef LOCATIONENGINE_H
#define LOCATIONENGINE_H

#include "locationsource.h"
#include <QObject>
#include <QGeoPositionInfo>

QTM_USE_NAMESPACE

/**
  * LocationEngine
  */
class LocationEngine
    : public QObject
{
private:
    Q_OBJECT

public:
    explicit LocationEngine(QObject* parent = 0);
    ~LocationEngine();

    void start(QObject& client);
    void stop(QObject& client);
    void refresh();

    const QGeoPositionInfo currentPosition() const;

private:
    void updateInterval();

signals:
    void currentPositionChanged(const QGeoPositionInfo& currentPosition);

private slots:
    void positionChanged(const QGeoPositionInfo& position);

private:
    LocationSource* mLocationSource;
    QList<QObject*> mClients;
};

#endif // LOCATIONENGINE_H

