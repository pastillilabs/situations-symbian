#ifndef CALENDARENGINE_H
#define CALENDARENGINE_H

#include <QObject>
#include <QOrganizerManager>
#include <QOrganizerItem>
#include <QTimer>

QTM_USE_NAMESPACE

/**
  * CalendarEngine
  */
class CalendarEngine
    : public QObject
{
private:
    Q_OBJECT

public:
    explicit CalendarEngine(QObject* parent = 0);
    ~CalendarEngine();

    void start(QObject& client);
    void stop(QObject& client);

    const QList<QOrganizerItem> activeItems();

signals:
    void handleItems(const QList<QOrganizerItem>& items);

private slots:
    void dataChanged();

private:
    QList<QObject*> mClients;

    QOrganizerManager* mOrganizerManager;
    QTimer mTimer;
    int mNextTimeout;
};

#endif // CALENDARENGINE_H

