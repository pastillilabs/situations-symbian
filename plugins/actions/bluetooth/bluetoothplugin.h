#ifndef BLUETOOTHPLUGIN_H
#define BLUETOOTHPLUGIN_H

#include <actionplugin.h>

/**
  * BluetoothPlugin
  */
class BluetoothPlugin
    : public QObject
    , public ActionPlugin
{
private:
    Q_OBJECT
    Q_INTERFACES(ActionPlugin)

public:
    BluetoothPlugin();

private: // From ActionPlugin
    Action* createInstance(QObject* parent);
    void qmlRegisterTypes();
    const QVariant property(const char* name) const;
};

#endif // BLUETOOTHPLUGIN_H

