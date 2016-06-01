#ifndef CALENDARCONDITION_H
#define CALENDARCONDITION_H

#include "calendarengine.h"
#include <condition.h>

/**
  * CalendarCondition
  */
class CalendarCondition
    : public Condition
{
private:
    Q_OBJECT
    Q_PROPERTY(bool noAllDay READ noAllDay WRITE setNoAllDay NOTIFY noAllDayChanged)
    Q_PROPERTY(bool alarm READ alarm WRITE setAlarm NOTIFY alarmChanged)
    Q_PROPERTY(bool recurring READ recurring WRITE setRecurring NOTIFY recurringChanged)
    Q_PROPERTY(QString collectionId READ collectionId WRITE setCollectionId NOTIFY collectionIdChanged)
    Q_PROPERTY(QString keyword READ keyword WRITE setKeyword NOTIFY keywordChanged)

public:
    CalendarCondition(QObject* parent = 0);
    ~CalendarCondition();

    bool noAllDay() const;
    void setNoAllDay(bool noAllDay);
    bool alarm() const;
    void setAlarm(bool alarm);
    bool recurring() const;
    void setRecurring(bool recurring);
    QString collectionId() const;
    void setCollectionId(const QString& collectionId);
    QString keyword() const;
    void setKeyword(const QString& keyword);

signals:
    void noAllDayChanged(bool allDay);
    void alarmChanged(bool alarm);
    void recurringChanged(bool recurring);
    void collectionIdChanged(QString collectionId);
    void keywordChanged(QString keyword);

private:
    void resolveActive();

private: // From Condition
    void doStart();
    void doStop();
    void doStore(QVariantMap& data) const;
    void doRestore(const QVariantMap& data);

private slots:
    void handleItems(const QList<QOrganizerItem>& items);

private:
    bool mNoAllDay;
    bool mAlarm;
    bool mRecurring;
    QString mCollectionId;
    QString mKeyword;

    static CalendarEngine mEngine;
};

#endif // CALENDARCONDITION_H

