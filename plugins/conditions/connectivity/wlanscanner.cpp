#include "wlanscanner.h"

#ifdef Q_OS_SYMBIAN
#include <wlanstate.h>
#endif

WlanScanner::WlanScanner(QObject* parent)
    : QObject(parent)
    , mScanning(false)
#ifdef Q_OS_SYMBIAN
    , mWlanState(0)
    , mWlanScanner(0)
#endif
{
#ifdef Q_OS_SYMBIAN
    QT_TRAP_THROWING(mWlanState = CWlanState::NewL(NULL, CWlanState::EDialogOptGlobal));
    QT_TRAP_THROWING(mWlanScanner = CWlanScanner::NewL(*this));
#endif
}

WlanScanner::~WlanScanner()
{
    stop();

#ifdef Q_OS_SYMBIAN
    delete mWlanScanner;
    delete mWlanState;
#endif
}

const QStringList& WlanScanner::connections() const
{
    return mConnections;
}

int WlanScanner::count() const
{
    return mConnections.count();
}

bool WlanScanner::scanning() const
{
    return mScanning;
}

void WlanScanner::start()
{
    if(!scanning())
    {
        if(power())
        {
#ifdef Q_OS_SYMBIAN
            mWlanScanner->StartScanning();
            setScanning(true);
#endif
        }
    }
}

void WlanScanner::stop()
{
    if(scanning())
    {
#ifdef Q_OS_SYMBIAN
        if(mWlanScanner->IsActive()) mWlanScanner->Cancel();
#endif
    }

    setScanning(false);
}

void WlanScanner::clear()
{
    mConnections.clear();
    emit connectionsChanged(mConnections);
    emit countChanged(mConnections.count());
}

bool WlanScanner::power() const
{
    bool power(false);

#ifdef Q_OS_SYMBIAN
    power = mWlanState->GetState() != CWlanState::EWlanStateOff;
#endif

    return power;
}

void WlanScanner::powerOn()
{
#ifdef Q_OS_SYMBIAN
    TRAP_IGNORE(mWlanState->SetStateL(CWlanState::EWlanStateOn));
#endif
}

void WlanScanner::setScanning(bool scanning)
{
    if(mScanning != scanning)
    {
        mScanning = scanning;
        emit scanningChanged(scanning);
    }
}


#ifdef Q_OS_SYMBIAN
void WlanScanner::WlanScanComplete(const RArray<TWlanSsid>& wlanArray)
{
    mConnections.clear();

    for(TUint i(0); i < wlanArray.Count(); ++i)
    {
        const TWlanSsid ssidBuf(wlanArray[i]);
        const QString ssid(QString::fromUtf8(reinterpret_cast<const char*>(ssidBuf.Ptr()), ssidBuf.Length()));
        if(!ssid.isEmpty() && !mConnections.contains(ssid)) mConnections.append(ssid);
    }

    stop();
    emit connectionsChanged(mConnections);
    emit countChanged(mConnections.count());
}

void WlanScanner::NotifyError(const TInt aError)
{
    stop();
}
#endif

