#include "mobiledataaction.h"
#include <identifiers.h>
#include <QTimerEvent>

#ifdef Q_OS_SYMBIAN
#include <QProcess>

static const QString TOGGLE("situationstoggle");
static const QString MOBILEDATA(" FBDC70D3571343f2A4B25026CC7943E0");
static const QString GET(" get");
static const QString SET(" set");
static const QString ON(" on");
static const QString OFF(" off");
#endif

static const int DEFAULT_DELAY(1000);
int MobileDataAction::mRestoreTimerId(0);
QVariant MobileDataAction::mRestoreValue;

MobileDataAction::MobileDataAction(QObject* parent)
    : Action(parent)
{
    setStartDelay(DEFAULT_DELAY);
}

MobileDataAction::~MobileDataAction()
{
    doCancelRestore();
}

const QVariant MobileDataAction::currentValue() const
{
#ifdef Q_OS_SYMBIAN
    const bool current(QProcess::execute(TOGGLE + MOBILEDATA + GET));
    return current;
#endif

#ifdef Q_WS_SIMULATOR
    return true;
#endif
}

void MobileDataAction::doExecute(const QVariant& value)
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
        QProcess::startDetached(TOGGLE + MOBILEDATA + SET + (next ? ON : OFF));
    }
#endif

#ifdef Q_WS_SIMULATOR
    Q_UNUSED(value);
#endif
}

void MobileDataAction::doRestore()
{
    if(!mRestoreTimerId) mRestoreTimerId = startTimer(0);
}

void MobileDataAction::doCancelRestore()
{
    if(mRestoreTimerId) killTimer(mRestoreTimerId);
    mRestoreTimerId = 0;

    if(!restore()) mRestoreValue.clear();
}

void MobileDataAction::timerEvent(QTimerEvent* event)
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

