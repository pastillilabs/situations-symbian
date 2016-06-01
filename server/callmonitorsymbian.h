#ifndef CALLMONITORSYMBIAN_H
#define CALLMONITORSYMBIAN_H

#include <Etel3rdParty.h>

/**
  * CallMonitorSymbian
  */
class CallMonitorSymbian
    : public CActive
{
public:
    /**
      * Observer
      */
    class Observer
    {
    public:
        virtual void callStatusChanged(CTelephony::TCallStatus& status) = 0;
    };

public:
    CallMonitorSymbian(Observer& observer);
    ~CallMonitorSymbian();

private:
    void startListening();

private: // From CActive
    void DoCancel();
    void RunL();

private:
    Observer& mObserver;
    CTelephony::TCallStatusV1 mCurrentStatus;
    CTelephony::TCallStatusV1Pckg mCurrentStatusPckg;
    CTelephony* mTelephony;
};

#endif // CALLMONITORSYMBIAN_H

