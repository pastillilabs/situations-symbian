#ifndef CALLMONITOR_H
#define CALLMONITOR_H

#include <QObject>

#ifdef Q_OS_SYMBIAN
#include "callmonitorsymbian.h"
#endif

/**
  * CallMonitor
  */
class CallMonitor
    : public QObject
#ifdef Q_OS_SYMBIAN
    , private CallMonitorSymbian::Observer
#endif
{
private:
    Q_OBJECT

public:
    explicit CallMonitor(QObject* parent = 0);

    bool active() const;

signals:
    void activeChanged(bool active);

#ifdef Q_OS_SYMBIAN
private: // From CallMonitorSymbian::Observer
    void callStatusChanged(CTelephony::TCallStatus& status);
#endif

private:
    bool mActive;

#ifdef Q_OS_SYMBIAN
    CallMonitorSymbian mCallMonitor;
#endif
};

#endif // CALLMONITOR_H

