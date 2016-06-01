#ifndef BLUETOOTHACTION_H
#define BLUETOOTHACTION_H

#include <action.h>
#include <QBluetoothLocalDevice>

QTM_USE_NAMESPACE

/**
  * BluetoothAction
  */
class BluetoothAction
    : public Action
{
private:
    Q_OBJECT

public:
    explicit BluetoothAction(QObject* parent = 0);
    ~BluetoothAction();

private: // From Action
    const QVariant currentValue() const;
    void doExecute(const QVariant& value);
    void doRestore();
    void doCancelRestore();

private: // From QObject
    void timerEvent(QTimerEvent* event);

private:
    QBluetoothLocalDevice mBluetoothLocalDevice;

    static int mRestoreTimerId;
    static QVariant mRestoreValue;
};

#endif // BLUETOOTHACTION_H

