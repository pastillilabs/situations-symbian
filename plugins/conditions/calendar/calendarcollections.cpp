#include "calendarcollections.h"
#include <QOrganizerManager>

CalendarCollections::CalendarCollections(QObject* parent)
    : QObject(parent)
{
    refresh();
}

void CalendarCollections::refresh()
{
    QOrganizerManager organizerManager;
    mCollections = organizerManager.collections();
}

int CalendarCollections::count() const
{
    return mCollections.count() + 1;
}

QStringList CalendarCollections::names() const
{
    QStringList names;
    names.append(qtTrId("calendar_any"));
    foreach(const QOrganizerCollection& collection, mCollections)
    {
        QString name(collection.metaData(QOrganizerCollection::KeyName).toString());

        names.append(name);
    }

    return names;
}

QString CalendarCollections::nameByIndex(const int index) const
{
    QString name(qtTrId("calendar_any"));
    if(index > 0 && index < count())
    {
        const QOrganizerCollection collection(mCollections.at(index - 1));
        name = collection.metaData(QOrganizerCollection::KeyName).toString();
    }

    return name;
}

QString CalendarCollections::idByIndex(const int index) const
{
    QString id;
    if(index > 0 && index < count())
    {
        const QOrganizerCollection collection(mCollections.at(index - 1));
        id = collection.id().toString();
    }

    return id;
}

QString CalendarCollections::nameById(const QString id) const
{
    QString name(qtTrId("calendar_any"));
    if(!id.isEmpty())
    {
        name = qtTrId("calendar_unknown");
        foreach(const QOrganizerCollection& collection, mCollections)
        {
            if(id == collection.id().toString())
            {
                name = collection.metaData(QOrganizerCollection::KeyName).toString();
                break;
            }
        }
    }

    return name;
}

int CalendarCollections::indexById(const QString id) const
{
    int index(0);
    if(!id.isEmpty())
    {
        foreach(const QOrganizerCollection& collection, mCollections)
        {
            if(id == collection.id().toString()) break;
            ++index;
        }
    }

    return index;
}

