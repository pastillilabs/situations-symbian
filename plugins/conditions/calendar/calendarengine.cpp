#include "calendarengine.h"
#include <QOrganizerEventTime>

static const int MSECSADAY(86400000);
static const int TIMERDELAY(200);

CalendarEngine::CalendarEngine(QObject* parent)
    : QObject(parent)
    , mOrganizerManager(0)
    , mNextTimeout(0)
{
    connect(&mTimer, SIGNAL(timeout()), this, SLOT(dataChanged()));
}

CalendarEngine::~CalendarEngine()
{
    delete mOrganizerManager;
}

void CalendarEngine::start(QObject& client)
{
    if(!mOrganizerManager)
    {
        mOrganizerManager = new QOrganizerManager(this);
        connect(mOrganizerManager, SIGNAL(dataChanged()), this, SLOT(dataChanged()));
        connect(mOrganizerManager, SIGNAL(itemsAdded(QList<QOrganizerItemId>)), this, SLOT(dataChanged()));
        connect(mOrganizerManager, SIGNAL(itemsChanged(QList<QOrganizerItemId>)), this, SLOT(dataChanged()));
        connect(mOrganizerManager, SIGNAL(itemsRemoved(QList<QOrganizerItemId>)), this, SLOT(dataChanged()));

        dataChanged();
    }

    if(!mClients.contains(&client))
    {
        mClients.append(&client);
        connect(this, SIGNAL(handleItems(QList<QOrganizerItem>)), &client, SLOT(handleItems(QList<QOrganizerItem>)));
    }
}

void CalendarEngine::stop(QObject& client)
{
    mClients.removeAll(&client);
    disconnect(this, SIGNAL(handleItems(QList<QOrganizerItem>)), &client, SLOT(handleItems(QList<QOrganizerItem>)));

    if(mClients.isEmpty())
    {
        delete mOrganizerManager;
        mOrganizerManager = 0;
        mTimer.stop();
    }
}

const QList<QOrganizerItem> CalendarEngine::activeItems()
{
    const QDateTime current(QDateTime::currentDateTime());
    mNextTimeout = current.time().msecsTo(QTime(0, 0)) + MSECSADAY;

    // Get list of active items and update timeout
    QList<QOrganizerItem> activeItems;
    if(mOrganizerManager)
    {
        const QList<QOrganizerItem> allItems(mOrganizerManager->items(current.addDays(-1), current.addDays(1)));
        foreach(const QOrganizerItem& item, allItems)
        {
            const QOrganizerEventTime eventTime(item.detail<QOrganizerEventTime>());
            const QDateTime eventStart(eventTime.startDateTime());
            const QDateTime eventEnd(eventTime.endDateTime());

            // Append to active items, if event is ongoing
            if(!eventTime.isEmpty() && eventStart <= current && eventEnd >= current) activeItems.append(item);

            // If event starts today, set next timeout to event start (if earlier than current)
            if(current.date() == eventStart.date())
            {
                const int timeout(current.time().msecsTo(eventStart.time()));
                if(timeout > 0 && timeout < mNextTimeout) mNextTimeout = timeout;
            }

            // If event ends today, set timeout to event end (if earlier than current)
            if(current.date() == eventEnd.date())
            {
                const int timeout(current.time().msecsTo(eventEnd.time()));
                if(timeout > 0 && timeout < mNextTimeout) mNextTimeout = timeout;
            }
        }
    }

    return activeItems;
}

void CalendarEngine::dataChanged()
{
    // Signal active items to clients for further inspection
    const QList<QOrganizerItem> items(activeItems());
    emit handleItems(items);

    // Start timer with some delay at the end to avoid situation jumping
    const int delay(items.count() ? TIMERDELAY : 0);
    mTimer.start(mNextTimeout + delay);
}

