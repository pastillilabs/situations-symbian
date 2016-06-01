#ifndef GLOBALQUERYSYMBIAN_H
#define GLOBALQUERYSYMBIAN_H

#include <QString>
#include <e32base.h>

class CAknGlobalNote;

/**
  * GlobalQuerySymbian
  */
class GlobalQuerySymbian
    : public CActive
{
public:
    /**
      * Observer
      */
    class Observer
    {
    public:
        virtual void onGlobalQueryDone(const int result) = 0;
    };

public:
    GlobalQuerySymbian(Observer& observer);
    ~GlobalQuerySymbian();

    void ShowL(const QString& message);

private: // From CActive
    void DoCancel();
    void RunL();

private:
    Observer& mObserver;
    CAknGlobalNote* mGlobalNote;
    TInt mNoteId;
};

#endif

