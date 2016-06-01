#include "wlanaction.h"
#include <identifiers.h>
#include <QTimerEvent>

#ifdef Q_OS_SYMBIAN
#include <wlanstate.h>
#endif

static const int DEFAULT_DELAY(4000);
int WlanAction::mRestoreTimerId(0);
QVariant WlanAction::mRestoreValue;

WlanAction::WlanAction(QObject* parent)
    : Action(parent)
#ifdef Q_OS_SYMBIAN
    , mWlanState(0)
#endif
{
    setStartDelay(DEFAULT_DELAY);

#ifdef Q_OS_SYMBIAN
    QT_TRAP_THROWING(mWlanState = CWlanState::NewL(NULL, CWlanState::EDialogOptGlobal));
#endif
}

WlanAction::~WlanAction()
{
#ifdef Q_OS_SYMBIAN
    delete mWlanState;
#endif

    doCancelRestore();
}

const QVariant WlanAction::currentValue() const
{
#ifdef Q_OS_SYMBIAN
    bool current(mWlanState->GetState() != CWlanState::EWlanStateOff);
    return current;
#endif

#ifdef Q_WS_SIMULATOR
    return true;
#endif
}

void WlanAction::doExecute(const QVariant& value)
{
#ifdef Q_OS_SYMBIAN
    const bool current(currentValue().toBool());
    const bool next(value.toBool());

    if(restore() && !mRestoreValue.isValid())
    {
        mRestoreValue = current;
    }

    if(next != current)
    {
        TRAP_IGNORE(mWlanState->SetStateL(next ? CWlanState::EWlanStateOn : CWlanState::EWlanStateOff));
    }
#endif

#ifdef Q_WS_SIMULATOR
    Q_UNUSED(value);
#endif
}

void WlanAction::doRestore()
{
    if(!mRestoreTimerId) mRestoreTimerId = startTimer(0);
}

void WlanAction::doCancelRestore()
{
    if(mRestoreTimerId) killTimer(mRestoreTimerId);
    mRestoreTimerId = 0;

    if(!restore()) mRestoreValue.clear();
}

void WlanAction::timerEvent(QTimerEvent* event)
{
    const int timerId(event->timerId());
    if(timerId == mRestoreTimerId)
    {
        killTimer(mRestoreTimerId);
        mRestoreTimerId = 0;
        if(mRestoreValue.isValid()) doExecute(mRestoreValue);
        mRestoreValue.clear();
    }

    Action::timerEvent(event);
}

