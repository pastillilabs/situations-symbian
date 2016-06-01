#include "mobilenetworkaction.h"
#include <identifiers.h>
#include <QTimerEvent>

#ifdef Q_OS_SYMBIAN
#include <QProcess>

static const int DEFAULT_DELAY(2000);
static const QString TOGGLE("situationstoggle");
static const QString MOBILENETWORK(" A847C5F0843D4e9eBC3A1FC433FE178B");
static const QString GET(" get");
static const QString SET(" set");
#endif

int MobileNetworkAction::mRestoreTimerId(0);
QVariant MobileNetworkAction::mRestoreValue;

MobileNetworkAction::MobileNetworkAction(QObject* parent)
    : Action(parent)
{
#ifdef Q_OS_SYMBIAN
    setStartDelay(DEFAULT_DELAY);
#endif
}

MobileNetworkAction::~MobileNetworkAction()
{
    doCancelRestore();
}

QString MobileNetworkAction::nameById(const int id) const
{
    QString name;

#ifdef Q_OS_SYMBIAN
    switch(id)
    {
    case 0:
        name = qtTrId("action_mobilenetwork_dual");
        break;
    case 1:
        name = qtTrId("action_mobilenetwork_3g");
        break;
    case 2:
        name = qtTrId("action_mobilenetwork_2g");
        break;
    default:
        break;
    }
#endif

    return name;
}

const QVariant MobileNetworkAction::currentValue() const
{
#ifdef Q_OS_SYMBIAN
    const int current(QProcess::execute(TOGGLE + MOBILENETWORK + GET));
    return current;
#endif

#ifdef Q_WS_SIMULATOR
    return 0;
#endif
}

void MobileNetworkAction::doExecute(const QVariant& value)
{
#ifdef Q_OS_SYMBIAN
    const int current(currentValue().toInt());
    const int next(value.toInt());

    if(restore() && !mRestoreValue.isValid())
    {
        mRestoreValue = current;
    }

    if(next != current)
    {
        QProcess::startDetached(TOGGLE + MOBILENETWORK + SET + QString(" %1").arg(next));
    }
#endif

#ifdef Q_WS_SIMULATOR
    Q_UNUSED(value);
#endif
}

void MobileNetworkAction::doRestore()
{
    if(!mRestoreTimerId) mRestoreTimerId = startTimer(0);
}

void MobileNetworkAction::doCancelRestore()
{
    if(mRestoreTimerId) killTimer(mRestoreTimerId);
    mRestoreTimerId = 0;

    if(!restore()) mRestoreValue.clear();
}

void MobileNetworkAction::timerEvent(QTimerEvent* event)
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

