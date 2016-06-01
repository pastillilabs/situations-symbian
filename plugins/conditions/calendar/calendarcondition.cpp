#include "calendarcondition.h"
#include <QOrganizerEventTime>
#include <QOrganizerItemReminder>
#include <QOrganizerItemRecurrence>
#include <QOrganizerItemComment>
#include <QOrganizerItemDescription>
#include <QOrganizerItemDisplayLabel>

static const char* KEY_NO_ALL_DAY("noAllDay");
static const char* KEY_ALARM("alarm");
static const char* KEY_RECURRING("recurring");
static const char* KEY_COLLECTION("collection");
static const char* KEY_KEYWORD("keyword");

CalendarEngine CalendarCondition::mEngine;

CalendarCondition::CalendarCondition(QObject* parent)
    : Condition(parent)
    , mNoAllDay(true)
    , mAlarm(true)
    , mRecurring(false)
{
}

CalendarCondition::~CalendarCondition()
{
    doStop();
}

bool CalendarCondition::noAllDay() const
{
    return mNoAllDay;
}

void CalendarCondition::setNoAllDay(bool noAllDay)
{
    if(noAllDay != mNoAllDay)
    {
        mNoAllDay = noAllDay;
        emit noAllDayChanged(noAllDay);
    }
}

bool CalendarCondition::alarm() const
{
    return mAlarm;
}

void CalendarCondition::setAlarm(bool alarm)
{
    if(alarm != mAlarm)
    {
        mAlarm = alarm;
        emit alarmChanged(alarm);
    }
}

bool CalendarCondition::recurring() const
{
    return mRecurring;
}

void CalendarCondition::setRecurring(bool recurring)
{
    if(recurring != mRecurring)
    {
        mRecurring = recurring;
        emit recurringChanged(recurring);
    }
}

QString CalendarCondition::keyword() const
{
    return mKeyword;
}

void CalendarCondition::setKeyword(const QString& keyword)
{
    if(keyword != mKeyword)
    {
        mKeyword = keyword;
        emit keywordChanged(keyword);
    }
}

QString CalendarCondition::collectionId() const
{
    return mCollectionId;
}

void CalendarCondition::setCollectionId(const QString& collectionId)
{
    if(collectionId != mCollectionId)
    {
        mCollectionId = collectionId;
        emit collectionIdChanged(collectionId);
    }
}

void CalendarCondition::doStart()
{
    mEngine.start(*this);
    handleItems(mEngine.activeItems());
}

void CalendarCondition::doStop()
{
    mEngine.stop(*this);
    setActive(false);
}

void CalendarCondition::doStore(QVariantMap& data) const
{
    data[KEY_NO_ALL_DAY] = mNoAllDay;
    data[KEY_ALARM] = mAlarm;
    data[KEY_RECURRING] = mRecurring;
    data[KEY_COLLECTION] = mCollectionId;
    data[KEY_KEYWORD] = mKeyword;
}

void CalendarCondition::doRestore(const QVariantMap& data)
{
    setNoAllDay(data[KEY_NO_ALL_DAY].toBool());
    setAlarm(data[KEY_ALARM].toBool());
    setRecurring(data[KEY_RECURRING].toBool());
    setCollectionId(data[KEY_COLLECTION].toString());
    setKeyword(data[KEY_KEYWORD].toString());
}

void CalendarCondition::handleItems(const QList<QOrganizerItem>& items)
{
    bool active(false);

    foreach(const QOrganizerItem& item, items)
    {
        active = true;

        // Check all-day constraint
        if(active && mNoAllDay)
        {
            const QOrganizerEventTime eventTime(item.detail<QOrganizerEventTime>());
            const QDateTime eventStart(eventTime.startDateTime());
            const QDateTime eventEnd(eventTime.endDateTime());

            // Get also time based all-day status
            const QDateTime startOfDay(QDateTime::currentDateTime().date(), QTime(0,0));
            const QDateTime endOfDay(startOfDay.addDays(1));
            const bool allTime(eventStart <= startOfDay && eventEnd >= endOfDay);

            if(eventTime.isAllDay() || allTime) active = false;
        }

        // Check alarm constraint
        if(active && mAlarm)
        {
            const QList<QOrganizerItemReminder> reminders(item.details<QOrganizerItemReminder>());
            if(!reminders.count()) active = false;
        }

        // Check recurring item constraint
        if(active && mRecurring)
        {
            active = false;

            const QList<QOrganizerItemType> types(item.details<QOrganizerItemType>());
            for(int i(0); i < types.count() && !active; ++i)
            {
                const QOrganizerItemType& type(types.at(i));
                if(type.type() == QOrganizerItemType::TypeEventOccurrence) active = true;
            }
        }

        // Check collection constraint
        if(active && !mCollectionId.isEmpty())
        {
            if(item.collectionId().toString() != mCollectionId) active = false;
        }

        // Check keyword constraint
        if(active && !mKeyword.isEmpty())
        {
            active = false;

            // Search from comments
            const QList<QOrganizerItemComment> comments(item.details<QOrganizerItemComment>());
            for(int c(0); c < comments.count() && !active; ++c)
            {
                const QOrganizerItemComment& comment(comments.at(c));
                active = comment.comment().indexOf(mKeyword) != -1;
            }

            // Search from descriptions
            const QList<QOrganizerItemDescription> descriptions(item.details<QOrganizerItemDescription>());
            for(int d(0); d < descriptions.count() && !active; ++d)
            {
                const QOrganizerItemDescription& description(descriptions.at(d));
                active = description.description().indexOf(mKeyword) != -1;
            }

            // Search from display labels
            const QList<QOrganizerItemDisplayLabel> labels(item.details<QOrganizerItemDisplayLabel>());
            for(int l(0); l < labels.count() && !active; ++l)
            {
                const QOrganizerItemDisplayLabel& label(labels.at(l));
                active = label.label().indexOf(mKeyword) != -1;
            }
        }

        if(active) break;
    }

    // Update active status
    setActive(active);
}

