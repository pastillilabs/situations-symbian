#ifndef STORAGEMANAGER_H
#define STORAGEMANAGER_H

#include "common_global.h"
#include <QObject>
#include <QThread>
#include <QVariantMap>

class Storage;

/**
  * StorageManager
  */
class COMMONSHARED_EXPORT StorageManager
    : public QObject
{
private:
    Q_OBJECT

public:
    explicit StorageManager(QObject *parent = 0);
    ~StorageManager();

    void addStorage(const QString& name);

    bool read(const QString& name, QVariantMap& data);
    void write(const QString& name, const QVariantMap& data);

private:
    QThread mThread;

    QMap<QString, Storage*> mStorageMap;
};

#endif // STORAGEMANAGER_H

