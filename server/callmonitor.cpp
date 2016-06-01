#include "callmonitor.h"

CallMonitor::CallMonitor(QObject* parent)
    : QObject(parent)
    , mActive(false)
#ifdef Q_OS_SYMBIAN
    , mCallMonitor(*this)
#endif
{
}

bool CallMonitor::active() const
{
    return mActive;
}

#ifdef Q_OS_SYMBIAN
void CallMonitor::callStatusChanged(CTelephony::TCallStatus& status)
{
    bool active(false);
    if(status != CTelephony::EStatusUnknown && status != CTelephony::EStatusIdle) active = true;

    if(active != mActive)
    {
        mActive = active;
        emit activeChanged(active);
    }
}
#endif

