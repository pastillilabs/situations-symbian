#include "profileaction.h"
#include <identifiers.h>
#include <QTimerEvent>

#ifdef Q_OS_SYMBIAN
#include <QSystemDeviceInfo>
#include <mproengengine.h>
#include <mproengprofile.h>
#include <mproengprofilename.h>
#include <proengfactory.h>

QTM_USE_NAMESPACE
#endif

int ProfileAction::mRestoreTimerId(0);
QVariant ProfileAction::mRestoreValue;

ProfileAction::ProfileAction(QObject* parent)
    : Action(parent)
#ifdef Q_OS_SYMBIAN
    , mEngine(0)
#endif
{
#ifdef Q_OS_SYMBIAN
    QT_TRAP_THROWING(mEngine = ProEngFactory::NewEngineL());
#endif
}

ProfileAction::~ProfileAction()
{
    doCancelRestore();

#ifdef Q_OS_SYMBIAN
    mEngine->Release();
#endif
}

QString ProfileAction::nameById(const int id) const
{
    QString name;

#ifdef Q_OS_SYMBIAN
    MProEngProfile* profile(0);
    TRAPD(err, profile = mEngine->ProfileL(id));
    if(err == KErrNone)
    {
        const MProEngProfileName& profileName(profile->ProfileName());
        name = QString::fromUtf16(profileName.Name().Ptr(), profileName.Name().Length());
    }
#endif

#ifdef Q_WS_SIMULATOR
    name = QString("Profile %1").arg(id);
#endif

    return name;
}

const QVariant ProfileAction::currentValue() const
{
#ifdef Q_OS_SYMBIAN
    const int current(mEngine->ActiveProfileId());
    return current;
#endif

#ifdef Q_WS_SIMULATOR
    return 0;
#endif
}

void ProfileAction::doExecute(const QVariant& value)
{
#ifdef Q_OS_SYMBIAN
    QSystemDeviceInfo systemDeviceInfo;
    if(systemDeviceInfo.simStatus() != QSystemDeviceInfo::SimNotAvailable)
    {
        const int current(currentValue().toInt());
        const int next(value.toInt());

        if(restore() && !mRestoreValue.isValid())
        {
            mRestoreValue = current;
        }

        if(next != current)
        {
            TRAP_IGNORE(mEngine->SetActiveProfileL(next));
        }
    }
#endif

#ifdef Q_WS_SIMULATOR
    Q_UNUSED(value);
#endif
}

void ProfileAction::doRestore()
{
    if(!mRestoreTimerId) mRestoreTimerId = startTimer(0);
}

void ProfileAction::doCancelRestore()
{
    if(mRestoreTimerId) killTimer(mRestoreTimerId);
    mRestoreTimerId = 0;

    if(!restore()) mRestoreValue.clear();
}

void ProfileAction::timerEvent(QTimerEvent* event)
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

