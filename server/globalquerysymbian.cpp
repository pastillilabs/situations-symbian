#include "globalquerysymbian.h"
#include <aknglobalnote.h>
#include <avkon.rsg>
#include <avkon.mbg>
#include <eikon.hrh>

GlobalQuerySymbian::GlobalQuerySymbian(Observer& observer)
    : CActive(CActive::EPriorityStandard)
    , mObserver(observer)
    , mGlobalNote(0)
    , mNoteId(0)
{
    CActiveScheduler::Add(this);
}

GlobalQuerySymbian::~GlobalQuerySymbian()
{
    delete mGlobalNote;
}

void GlobalQuerySymbian::ShowL(const QString& message)
{
    delete mGlobalNote;
    mGlobalNote = CAknGlobalNote::NewL();
    mGlobalNote->SetSoftkeys(R_AVKON_SOFTKEYS_OK_CANCEL__OK);
    mGlobalNote->SetGraphic(EMbmAvkonQgn_note_query, EMbmAvkonQgn_note_query_mask);
    iStatus = KRequestPending;
    SetActive();

    TPtrC string(reinterpret_cast<const TText*>(message.constData()),message.length());
    mNoteId = mGlobalNote->ShowNoteL(iStatus, EAknGlobalConfirmationNote, string);
}

void GlobalQuerySymbian::DoCancel()
{
    if(mGlobalNote) mGlobalNote->CancelNoteL(mNoteId);
    delete mGlobalNote;
}

void GlobalQuerySymbian::RunL()
{
    int result(0);
    if(iStatus.Int() == EEikBidOk) result = 1;
    mObserver.onGlobalQueryDone(result);
}

