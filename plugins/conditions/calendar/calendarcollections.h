#ifndef CALENDARCOLLECTIONS_H
#define CALENDARCOLLECTIONS_H

#include <QObject>
#include <QStringList>
#include <QOrganizerCollection>

QTM_USE_NAMESPACE

/**
  * CalendarCollections
  */
class CalendarCollections
    : public QObject
{
private:
    Q_OBJECT

public:
    explicit CalendarCollections(QObject* parent = 0);

    Q_INVOKABLE void refresh();
    Q_INVOKABLE int count() const;
    Q_INVOKABLE QStringList names() const;

    Q_INVOKABLE QString nameByIndex(const int index) const;
    Q_INVOKABLE QString idByIndex(const int index) const;
    Q_INVOKABLE QString nameById(const QString id) const;
    Q_INVOKABLE int indexById(const QString id) const;

private:
    QList<QOrganizerCollection> mCollections;
};

#endif // CALENDARCOLLECTIONS_H

