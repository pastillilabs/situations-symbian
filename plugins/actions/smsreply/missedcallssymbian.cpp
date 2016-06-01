#include "missedcallssymbian.h"
#include <logcli.h>
#include <logview.h>
#include <logwraplimits.h>
#include <logfilterandeventconstants.hrh>
#include <limits.h>

#include <trace.h>

static const TTimeIntervalMicroSeconds32 NOTIFYDELAY(1000000);
static const int INITIALCOUNT(INT_MAX);

MissedCalls::MissedCalls(Observer& observer)
    : CActive(CActive::EPriorityStandard)
    , mObserver(observer)
    , mState(Uninitialized)
    , mLogClient(0)
    , mLogViewRecent(0)
    , mLogViewDuplicate(0)
    , mCount(INITIALCOUNT)
    , mDuplicates(INITIALCOUNT)
{
    CActiveScheduler::Add(this);
}

MissedCalls::~MissedCalls()
{
    stop();
}

void MissedCalls::start()
{
    TRACE;
    if(mState == Uninitialized) TRAP_IGNORE(ConstructL());
}

void MissedCalls::stop()
{
    Cancel();

    delete mLogViewDuplicate;
    mLogViewDuplicate = 0;
    delete mLogViewRecent;
    mLogViewRecent = 0;
    delete mLogClient;
    mLogClient = 0;

    mFs.Close();

    mState = Uninitialized;
}

void MissedCalls::ConstructL()
{
    User::LeaveIfError(mFs.Connect());

    mLogClient = CLogClient::NewL(mFs);
    mLogViewRecent = CLogViewRecent::NewL(*mLogClient);
    mLogViewDuplicate = CLogViewDuplicate::NewL(*mLogClient);

    // Handle initial change to initialize count & duplicateCount
    HandleChangeL();
}

void MissedCalls::RequestNotifyChange()
{
    mState = WaitingChange;
    mLogClient->NotifyChange(NOTIFYDELAY, iStatus);
    SetActive();
}

void MissedCalls::HandleChangeL()
{
    mState = ReadingLog;
    if(mLogViewRecent->SetRecentListL(KLogRecentMissedCalls,iStatus))
    {
        SetActive();
    }
    else
    {
        mCount = 0;
        RequestNotifyChange();
    }
}

void MissedCalls::HandleRecentListL()
{
    mState = ReadingLogItem;
    if(mLogViewRecent->FirstL(iStatus))
    {
        SetActive();
    }
    else
    {
        mCount = 0;
        RequestNotifyChange();
    }
}

void MissedCalls::HandleRecentListItemL()
{
    bool newItem(mLogViewRecent->CountL() > mCount);
    mCount = mLogViewRecent->CountL();

    if(newItem)
    {
        // Reset duplicate count for new item
        mDuplicates = 0;
        RequestNotifyChange();

        // Handle new log item
        const CLogEvent& event(mLogViewRecent->Event());
        if(!(event.Flags() & KLogEventRead))
        {
            mObserver.handleMissedCall(QString::fromUtf16(event.Number().Ptr(), event.Number().Length()));
        }
    }
    else
    {
        // Check for new duplicates
        mState = ReadingDuplicates;
        if(mLogViewRecent->DuplicatesL(*mLogViewDuplicate, iStatus))
        {
            SetActive();
        }
        else
        {
            mDuplicates = 0;
            RequestNotifyChange();
        }
    }
}

void MissedCalls::HandleDuplicatesL()
{
    bool newDuplicate(mLogViewDuplicate->CountL() > mDuplicates);
    mDuplicates = mLogViewDuplicate->CountL();
    RequestNotifyChange();

    if(newDuplicate)
    {
        // Handle new log duplicate
        const CLogEvent& event(mLogViewRecent->Event());
        if(!(event.Flags() & KLogEventRead))
        {
            mObserver.handleMissedCall(QString::fromUtf16(event.Number().Ptr(), event.Number().Length()));
        }
    }
}

void MissedCalls::DoCancel()
{
    if(mLogViewDuplicate) mLogViewDuplicate->Cancel();
    if(mLogViewRecent) mLogViewRecent->Cancel();
    if(mLogClient)
    {
        mLogClient->NotifyChangeCancel();
        mLogClient->Cancel();
    }
}

void MissedCalls::RunL()
{
    if(iStatus != KErrCancel)
    {
        switch(mState)
        {
        case WaitingChange:
            HandleChangeL();
            break;
        case ReadingLog:
            HandleRecentListL();
            break;
        case ReadingLogItem:
            HandleRecentListItemL();
            break;
        case ReadingDuplicates:
            HandleDuplicatesL();
            break;
        default:
            Q_ASSERT(false);
            break;
        }
    }
}

