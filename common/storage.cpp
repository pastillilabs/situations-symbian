#include "storage.h"
#include "identifiers.h"
#include <QDir>
#include <QFile>

static const QString TEMP_POSTFIX(".tmp");

Storage::Storage(const QString& name, QObject* parent)
    : QObject(parent)
    , mName(name)
{
}

bool Storage::read(QVariantMap& data)
{
    bool success(false);

    QDir dir(QDir::homePath());
    dir.mkdir(Identifiers::situationsDir);
    dir.cd(Identifiers::situationsDir);

    QFile file(dir.filePath(mName));
    if(file.open(QIODevice::ReadOnly))
    {
        QDataStream in(&file);
        in.setVersion(QDataStream::Qt_4_7);
        in >> data;
        success = true;
        file.close();
    }

    return success;
}

void Storage::write(const QVariantMap& data)
{
    QDir dir(QDir::homePath());
    dir.mkdir(Identifiers::situationsDir);
    dir.cd(Identifiers::situationsDir);

    // Write first to temp file
    QFile file(dir.filePath(mName + TEMP_POSTFIX));
    if(file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Unbuffered))
    {
        QDataStream out(&file);
        out.setVersion(QDataStream::Qt_4_7);
        out << data;
        file.flush();
        file.close();

        // Replace old file with new
        QFile::remove(dir.filePath(mName));
        file.rename(dir.filePath(mName));
    }
}

