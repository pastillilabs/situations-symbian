#ifndef TIMECONDITION_H
#define TIMECONDITION_H

#include <condition.h>
#include <QTimer>

/**
  * TimeCondition
  */
class TimeCondition
    : public Condition
{
private:
    Q_OBJECT

    Q_PROPERTY(int startHour READ startHour WRITE setStartHour NOTIFY startHourChanged)
    Q_PROPERTY(int startMinute READ startMinute WRITE setStartMinute NOTIFY startMinuteChanged)
    Q_PROPERTY(int endHour READ endHour WRITE setEndHour NOTIFY endHourChanged)
    Q_PROPERTY(int endMinute READ endMinute WRITE setEndMinute NOTIFY endMinuteChanged)

    Q_PROPERTY(bool monday READ monday WRITE setMonday NOTIFY mondayChanged)
    Q_PROPERTY(bool tuesday READ tuesday WRITE setTuesday NOTIFY tuesdayChanged)
    Q_PROPERTY(bool wednesday READ wednesday WRITE setWednesday NOTIFY wednesdayChanged)
    Q_PROPERTY(bool thursday READ thursday WRITE setThursday NOTIFY thursdayChanged)
    Q_PROPERTY(bool friday READ friday WRITE setFriday NOTIFY fridayChanged)
    Q_PROPERTY(bool saturday READ saturday WRITE setSaturday NOTIFY saturdayChanged)
    Q_PROPERTY(bool sunday READ sunday WRITE setSunday NOTIFY sundayChanged)

public:
    TimeCondition(QObject* parent = 0);
    ~TimeCondition();

    int startHour() const;
    int startMinute() const;
    int endHour() const;
    int endMinute() const;

    bool monday() const;
    bool tuesday() const;
    bool wednesday() const;
    bool thursday() const;
    bool friday() const;
    bool saturday() const;
    bool sunday() const;

    void setStartHour(const int startHour);
    void setStartMinute(const int startMinute);
    void setEndHour(const int endHour);
    void setEndMinute(const int endMinute);

    void setMonday(bool selected);
    void setTuesday(bool selected);
    void setWednesday(bool selected);
    void setThursday(bool selected);
    void setFriday(bool selected);
    void setSaturday(bool selected);
    void setSunday(bool selected);

signals:
    void startHourChanged(int startHour);
    void startMinuteChanged(int startMinute);
    void endHourChanged(int endHour);
    void endMinuteChanged(int endMinute);

    void mondayChanged(bool selected);
    void tuesdayChanged(bool selected);
    void wednesdayChanged(bool selected);
    void thursdayChanged(bool selected);
    void fridayChanged(bool selected);
    void saturdayChanged(bool selected);
    void sundayChanged(bool selected);

private:
    void resolveActive();

private slots:
    void timeout();

private: // From Condition
    void doStart();
    void doStop();

    void doStore(QVariantMap& data) const;
    void doRestore(const QVariantMap& data);

private:
    int mStartHour;
    int mStartMinute;
    int mEndHour;
    int mEndMinute;
    int mDaysOfWeek;

    QTimer mTimer;
};

#endif // TIMECONDITION_H

