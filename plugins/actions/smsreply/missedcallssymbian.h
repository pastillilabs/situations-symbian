#ifndef MISSEDCALLSSYMBIAN_H
#define MISSEDCALLSSYMBIAN_H

#include <QString>
#include <f32file.h>

class CLogClient;
class CLogViewRecent;
class CLogViewDuplicate;

/**
  * MissedCalls
  */
class MissedCalls
    : public CActive
{
public:
    /**
      * Observer
      */
    class Observer
    {
    public:
        virtual void handleMissedCall(const QString number) = 0;
    };

private:
    enum MonitorState
    {
        Uninitialized,
        WaitingChange,
        ReadingLog,
        ReadingLogItem,
        ReadingDuplicates
    };

public:
    MissedCalls(Observer& observer);
    ~MissedCalls();

    void start();
    void stop();

private:
    void ConstructL();
    void RequestNotifyChange();
    void HandleChangeL();
    void HandleRecentListL();
    void HandleRecentListItemL();
    void HandleDuplicatesL();

private: // From CActive
    void DoCancel();
    void RunL();

private:
    Observer& mObserver;
    MonitorState mState;

    RFs mFs;
    CLogClient* mLogClient;
    CLogViewRecent* mLogViewRecent;
    CLogViewDuplicate* mLogViewDuplicate;
    int mCount;
    int mDuplicates;
};

#endif // MISSEDCALLSSYMBIAN_H
