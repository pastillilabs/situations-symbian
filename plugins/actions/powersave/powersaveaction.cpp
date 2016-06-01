#include "powersaveaction.h"
#include <identifiers.h>
#include <QTimerEvent>

#ifdef Q_OS_SYMBIAN
#include <psmclient.h>
#include <psmtypes.h>
#include <psmsettings.h>
#include <psmclientobserver.h>
#endif

#ifdef Q_OS_SYMBIAN
class PsmClientObserver
    : public MPsmClientObserver
{
public: // From MPsmClientObserver
    void PowerSaveModeChanged(const TPsmsrvMode /*aMode*/) {};
    void PowerSaveModeChangeError(const TInt /*aError*/) {};
};
#endif

int PowersaveAction::mRestoreTimerId(0);
QVariant PowersaveAction::mRestoreValue;

PowersaveAction::PowersaveAction(QObject* parent)
    : Action(parent)
#ifdef Q_OS_SYMBIAN
    , mPsmClientObserver(0)
    , mPsmClient(0)
#endif
{
#ifdef Q_OS_SYMBIAN
    mPsmClientObserver = new PsmClientObserver();
    QT_TRAP_THROWING(mPsmClient = CPsmClient::NewL(*mPsmClientObserver));
#endif
}

PowersaveAction::~PowersaveAction()
{
    doCancelRestore();
#ifdef Q_OS_SYMBIAN
    delete mPsmClient;
    delete mPsmClientObserver;
#endif
}

const QVariant PowersaveAction::currentValue() const
{
#ifdef Q_OS_SYMBIAN
    TInt mode(0);
    mPsmClient->PsmSettings().GetCurrentMode(mode);
    const bool current(mode != EPsmsrvModeNormal);
    return current;
#endif

#ifdef Q_WS_SIMULATOR
    return false;
#endif
}

void PowersaveAction::doExecute(const QVariant& value)
{
    const bool current(currentValue().toBool());
    const bool next(value.toBool());

    if(restore() && !mRestoreValue.isValid())
    {
        mRestoreValue = current;
    }

    if(next != current)
    {
#ifdef Q_OS_SYMBIAN
        mPsmClient->ChangePowerSaveMode(next ? EPsmsrvModePowerSave : EPsmsrvModeNormal);
#endif
    }
}

void PowersaveAction::doRestore()
{
    if(!mRestoreTimerId) mRestoreTimerId = startTimer(0);
}

void PowersaveAction::doCancelRestore()
{
    if(mRestoreTimerId) killTimer(mRestoreTimerId);
    mRestoreTimerId = 0;

    if(!restore()) mRestoreValue.clear();
}

void PowersaveAction::timerEvent(QTimerEvent* event)
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

