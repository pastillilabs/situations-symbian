#include "storagemanager.h"
#include "storage.h"

static const int EXIT_WAIT(10000); // 10 seconds

StorageManager::StorageManager(QObject* parent)
    : QObject(parent)
{
    mThread.start();
}

StorageManager::~StorageManager()
{
    mThread.quit();
    mThread.wait(EXIT_WAIT);
}

void StorageManager::addStorage(const QString& name)
{
    Storage* storage(new Storage(name));
    mStorageMap.insert(name, storage);

    storage->moveToThread(&mThread);
}

bool StorageManager::read(const QString& name, QVariantMap& data)
{
    bool success(false);
    Storage* storage(mStorageMap.value(name));
    if(storage) success = storage->read(data);

    return success;
}

void StorageManager::write(const QString& name, const QVariantMap& data)
{
    Storage* storage(mStorageMap.value(name));
    if(storage) QMetaObject::invokeMethod(storage, "write", Q_ARG(QVariantMap, data));
}

