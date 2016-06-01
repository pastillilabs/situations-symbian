#include "bluetoothaction.h"
#include <identifiers.h>
#include <QTimerEvent>

static const int DEFAULT_DELAY(2000);
int BluetoothAction::mRestoreTimerId(0);
QVariant BluetoothAction::mRestoreValue;

BluetoothAction::BluetoothAction(QObject* parent)
    : Action(parent)
{
    setStartDelay(DEFAULT_DELAY);
}

BluetoothAction::~BluetoothAction()
{
    doCancelRestore();
}

const QVariant BluetoothAction::currentValue() const
{
    const bool current(mBluetoothLocalDevice.hostMode() != QBluetoothLocalDevice::HostPoweredOff);
    return current;
}

void BluetoothAction::doExecute(const QVariant& value)
{
    const bool current(currentValue().toBool());
    const bool next(value.toBool());

    if(restore() && !mRestoreValue.isValid())
    {
        mRestoreValue = current;
    }

    if(next != current)
    {
        next ? mBluetoothLocalDevice.powerOn() : mBluetoothLocalDevice.setHostMode(QBluetoothLocalDevice::HostPoweredOff);
    }
}

void BluetoothAction::doRestore()
{
    if(!mRestoreTimerId) mRestoreTimerId = startTimer(0);
}

void BluetoothAction::doCancelRestore()
{
    if(mRestoreTimerId) killTimer(mRestoreTimerId);
    mRestoreTimerId = 0;

    if(!restore()) mRestoreValue.clear();
}

void BluetoothAction::timerEvent(QTimerEvent* event)
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

