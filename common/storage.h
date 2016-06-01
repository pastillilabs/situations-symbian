#ifndef STORAGE_H
#define STORAGE_H

#include "common_global.h"
#include <QObject>
#include <QVariantMap>

/**
  * Storage
  */
class COMMONSHARED_EXPORT Storage
    : public QObject
{
private:
    Q_OBJECT

public:
    Storage(const QString& name, QObject* parent = 0);

    bool read(QVariantMap& data);

public slots:
    void write(const QVariantMap& data);

private:
    QString mName;
};

#endif // STORAGE_H

