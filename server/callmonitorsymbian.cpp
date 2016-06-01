#include "callmonitorsymbian.h"
#include <qglobal.h>

CallMonitorSymbian::CallMonitorSymbian(Observer& observer)
    : CActive(EPriorityStandard)
    , mObserver(observer)
    , mCurrentStatusPckg(mCurrentStatus)
    , mTelephony(0)
{
    QT_TRAP_THROWING(mTelephony = CTelephony::NewL());
    CActiveScheduler::Add(this);

    startListening();
}

CallMonitorSymbian::~CallMonitorSymbian()
{
    Cancel();
    delete mTelephony;
}

void CallMonitorSymbian::startListening()
{
    Cancel();

    mCurrentStatus.iStatus = CTelephony::EStatusUnknown;
    mTelephony->NotifyChange(iStatus, CTelephony::EVoiceLineStatusChange, mCurrentStatusPckg);
    SetActive();
}

void CallMonitorSymbian::DoCancel()
{
    mTelephony->CancelAsync(CTelephony::EVoiceLineStatusChangeCancel);
}


void CallMonitorSymbian::RunL()
{
    mObserver.callStatusChanged(mCurrentStatus.iStatus);
    if(iStatus != KErrCancel) startListening();
}

