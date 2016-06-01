#include "flightmodeaction.h"
#include <identifiers.h>
#include <QTimerEvent>

int FlightmodeAction::mRestoreTimerId(0);
QVariant FlightmodeAction::mRestoreValue;

FlightmodeAction::FlightmodeAction(QObject* parent)
    : Action(parent)
{
}

FlightmodeAction::~FlightmodeAction()
{
    doCancelRestore();
}

const QVariant FlightmodeAction::currentValue() const
{
#ifdef Q_WS_SIMULATOR
    return false;
#endif
}

void FlightmodeAction::doExecute(const QVariant& value)
{
#ifdef Q_WS_SIMULATOR
    Q_UNUSED(value);
#endif
}

void FlightmodeAction::doRestore()
{
    if(!mRestoreTimerId) mRestoreTimerId = startTimer(0);
}

void FlightmodeAction::doCancelRestore()
{
    if(mRestoreTimerId) killTimer(mRestoreTimerId);
    mRestoreTimerId = 0;

    if(!restore()) mRestoreValue.clear();
}

void FlightmodeAction::timerEvent(QTimerEvent* event)
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

