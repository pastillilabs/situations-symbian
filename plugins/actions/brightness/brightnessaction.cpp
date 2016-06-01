#include "brightnessaction.h"
#include <identifiers.h>
#include <QTimerEvent>

#ifdef Q_OS_SYMBIAN
#include <hwrmlightdomaincrkeys.h>
#include <centralrepository.h>
#endif

static const int DEFAULT_DELAY(2000);
int BrightnessAction::mRestoreTimerId(0);
QVariant BrightnessAction::mRestoreValue;

BrightnessAction::BrightnessAction(QObject* parent)
    : Action(parent)
{
    setStartDelay(DEFAULT_DELAY);
}

BrightnessAction::~BrightnessAction()
{
    doCancelRestore();
}

int BrightnessAction::minBrightness() const
{
#ifdef Q_OS_SYMBIAN
    return 0;
#endif

#ifdef Q_WS_SIMULATOR
    return 1;
#endif
}

int BrightnessAction::maxBrightness() const
{
#ifdef Q_OS_SYMBIAN
    return 100;
#endif

#ifdef Q_WS_SIMULATOR
    return 5;
#endif
}

const QVariant BrightnessAction::currentValue() const
{
#ifdef Q_OS_SYMBIAN
    TInt current(0);
    CRepository* repository(0);
    TRAPD(err, repository = CRepository::NewL(KCRUidLightSettings));
    if(err == KErrNone)
    {
        repository->Get(KLightSensorSensitivity, current);
        delete repository;
    }
    return current;
#endif

#ifdef Q_WS_SIMULATOR
    return maxBrightness();
#endif
}

void BrightnessAction::doExecute(const QVariant& value)
{
    const int current(currentValue().toInt());
    const int next(value.toInt());

    if(restore() && !mRestoreValue.isValid())
    {
        mRestoreValue = current;
    }

    if(next != current)
    {
#ifdef Q_OS_SYMBIAN
        CRepository* repository(0);
        TRAPD(err, repository = CRepository::NewL(KCRUidLightSettings));
        if(err == KErrNone)
        {
            repository->Set(KLightSensorSensitivity, next);
            delete repository;
        }
#endif
    }
}

void BrightnessAction::doRestore()
{
    if(!mRestoreTimerId) mRestoreTimerId = startTimer(0);
}

void BrightnessAction::doCancelRestore()
{
    if(mRestoreTimerId) killTimer(mRestoreTimerId);
    mRestoreTimerId = 0;

    if(!restore()) mRestoreValue.clear();
}

void BrightnessAction::timerEvent(QTimerEvent* event)
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

