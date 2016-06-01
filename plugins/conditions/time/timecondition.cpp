#include "timecondition.h"
#include <QTime>
#include <QStringList>

static const int MSECSADAY(86400000);
static const int TIMERDELAY(200);

static const char* KEY_START_HOUR("startHour");
static const char* KEY_START_MINUTE("startMinute");
static const char* KEY_END_HOUR("endHour");
static const char* KEY_END_MINUTE("endMinute");
static const char* KEY_DAYS_OF_WEEK("daysOfWeek");

TimeCondition::TimeCondition(QObject *parent)
    : Condition(parent)
    , mStartHour(8)
    , mStartMinute(0)
    , mEndHour(16)
    , mEndMinute(0)
    , mDaysOfWeek(31)
{
    connect(&mTimer, SIGNAL(timeout()), this, SLOT(timeout()));
}

TimeCondition::~TimeCondition()
{
    doStop();
}

int TimeCondition::startHour() const
{
    return mStartHour;
}

int TimeCondition::startMinute() const
{
    return mStartMinute;
}

int TimeCondition::endHour() const
{
    return mEndHour;
}

int TimeCondition::endMinute() const
{
    return mEndMinute;
}

bool TimeCondition::monday() const
{
    return mDaysOfWeek & 1;
}

bool TimeCondition::tuesday() const
{
    return mDaysOfWeek & 2;
}

bool TimeCondition::wednesday() const
{
    return mDaysOfWeek & 4;
}

bool TimeCondition::thursday() const
{
    return mDaysOfWeek & 8;
}

bool TimeCondition::friday() const
{
    return mDaysOfWeek & 16;
}

bool TimeCondition::saturday() const
{
    return mDaysOfWeek & 32;
}

bool TimeCondition::sunday() const
{
    return mDaysOfWeek & 64;
}

void TimeCondition::setStartHour(const int startHour)
{
    if(startHour != mStartHour)
    {
        mStartHour = startHour;
        emit(startHourChanged(startHour));
    }
}

void TimeCondition::setStartMinute(const int startMinute)
{
    if(startMinute != mStartMinute)
    {
        mStartMinute = startMinute;
        emit(startMinuteChanged(startMinute));
    }
}

void TimeCondition::setEndHour(const int endHour)
{
    if(endHour != mEndHour)
    {
        mEndHour = endHour;
        emit(endHourChanged(endHour));
    }
}

void TimeCondition::setEndMinute(const int endMinute)
{
    if(endMinute != mEndMinute)
    {
        mEndMinute = endMinute;
        emit(endMinuteChanged(endMinute));
    }
}

void TimeCondition::setMonday(bool selected)
{
    if(selected != monday())
    {
        mDaysOfWeek ^= 1;
        emit(mondayChanged(selected));
    }
}

void TimeCondition::setTuesday(bool selected)
{
    if(selected != tuesday())
    {
        mDaysOfWeek ^= 2;
        emit(tuesdayChanged(selected));
    }
}

void TimeCondition::setWednesday(bool selected)
{
    if(selected != wednesday())
    {
        mDaysOfWeek ^= 4;
        emit(wednesdayChanged(selected));
    }
}

void TimeCondition::setThursday(bool selected)
{
    if(selected != thursday())
    {
        mDaysOfWeek ^= 8;
        emit(thursdayChanged(selected));
    }
}

void TimeCondition::setFriday(bool selected)
{
    if(selected != friday())
    {
        mDaysOfWeek ^= 16;
        emit(fridayChanged(selected));
    }
}

void TimeCondition::setSaturday(bool selected)
{
    if(selected != saturday())
    {
        mDaysOfWeek ^= 32;
        emit(saturdayChanged(selected));
    }
}

void TimeCondition::setSunday(bool selected)
{
    if(selected != sunday())
    {
        mDaysOfWeek ^= 64;
        emit(sundayChanged(selected));
    }
}

void TimeCondition::resolveActive()
{
    bool active(false);

    // Get current date & time
    const QDate date(QDate::currentDate());
    const QTime time(QTime::currentTime());

    // Check if we are between start & end time
    const QTime startTime(mStartHour, mStartMinute);
    const QTime endTime(mEndHour, mEndMinute);
    active = time >= startTime && time < endTime;
    if(startTime >= endTime) active = time >= startTime || time < endTime;

    // Check if current day is selected or no days selected at all
    if(active && mDaysOfWeek != 0)
    {
        const int dayOfWeek(date.dayOfWeek());
        int dayBit(1);
        for(int i(1); i < dayOfWeek; ++i) dayBit *= 2;
        active = mDaysOfWeek & dayBit;
    }

    setActive(active);
}

void TimeCondition::timeout()
{
    doStart();
}

void TimeCondition::doStart()
{
    // Update active status
    resolveActive();

    // Resolve new target time for timer
    const QTime currentTime(QTime::currentTime());
    const QTime targetTime(active() ? QTime(mEndHour, mEndMinute) : QTime(mStartHour, mStartMinute));

    // Get milliseconds to target time or next mid night
    int msecs(currentTime.msecsTo(targetTime));
    if(msecs < 0) msecs = currentTime.msecsTo(QTime(0, 0)) + MSECSADAY;

    // Start the timer
    const int delay(active() ? TIMERDELAY : 0); // Add some delay at the end to avoid situation jumping
    mTimer.start(msecs + delay);
}

void TimeCondition::doStop()
{
    mTimer.stop();
    setActive(false);
}

void TimeCondition::doStore(QVariantMap& data) const
{
    data[KEY_START_HOUR] = mStartHour;
    data[KEY_START_MINUTE] = mStartMinute;
    data[KEY_END_HOUR] = mEndHour;
    data[KEY_END_MINUTE] = mEndMinute;
    data[KEY_DAYS_OF_WEEK] = mDaysOfWeek;
}

void TimeCondition::doRestore(const QVariantMap& data)
{
    setStartHour(data[KEY_START_HOUR].toInt());
    setStartMinute(data[KEY_START_MINUTE].toInt());
    setEndHour(data[KEY_END_HOUR].toInt());
    setEndMinute(data[KEY_END_MINUTE].toInt());

    const int daysOfWeek(data[KEY_DAYS_OF_WEEK].toInt());
    setMonday(daysOfWeek & 1);
    setTuesday(daysOfWeek & 2);
    setWednesday(daysOfWeek & 4);
    setThursday(daysOfWeek & 8);
    setFriday(daysOfWeek & 16);
    setSaturday(daysOfWeek & 32);
    setSunday(daysOfWeek & 64);
}

