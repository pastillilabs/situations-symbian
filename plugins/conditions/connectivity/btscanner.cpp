#include "btscanner.h"
#include <QBluetoothLocalDevice>

#ifdef Q_OS_SYMBIAN
#include <es_sock.h>
#include <btdevice.h>
#include <btsdp.h>
#include <bttypes.h>

const TInt KBTEngMaxAddrArraySize = 10;
_LIT(KL2Cap, "BTLinkManager");
#endif

QTM_USE_NAMESPACE

BtScanner::BtScanner(QObject* parent)
    : QObject(parent)
    , mScanning(false)
    , mBtDiscoveryAgent(0)
{
}

BtScanner::~BtScanner()
{
    stop();
}

const QStringList& BtScanner::connections() const
{
    return mConnections;
}

int BtScanner::count() const
{
    return mConnections.count();
}

bool BtScanner::scanning() const
{
    return mScanning;
}

void BtScanner::start()
{
    if(!scanning())
    {
        if(power())
        {
            mConnections.clear();

            mBtDiscoveryAgent = new QBluetoothDeviceDiscoveryAgent(this);
            connect(mBtDiscoveryAgent, SIGNAL(deviceDiscovered(const QBluetoothDeviceInfo&)), this, SLOT(deviceDiscovered(const QBluetoothDeviceInfo&)));
            connect(mBtDiscoveryAgent, SIGNAL(finished()), this, SLOT(finished()));
            connect(mBtDiscoveryAgent, SIGNAL(error(QBluetoothDeviceDiscoveryAgent::Error)), this, SLOT(error(QBluetoothDeviceDiscoveryAgent::Error)));

#ifdef Q_OS_SYMBIAN
            // Check active BT connections (possibly hidden!)
            RSocketServ socketServ;
            TInt err(socketServ.Connect());
            if(err == KErrNone)
            {
                TProtocolDesc pInfo;
                TProtocolName protocolName;
                protocolName = KL2Cap;
                socketServ.FindProtocol(protocolName, pInfo);

                RHostResolver hr;
                if(hr.Open(socketServ, pInfo.iAddrFamily, pInfo.iProtocol) == KErrNone)
                {
                    CBluetoothPhysicalLinks* links(0);
                    TRAP(err, links = CBluetoothPhysicalLinks::NewL(*this, socketServ));
                    if(err == KErrNone)
                    {
                        RBTDevAddrArray addrArray;
                        links->Enumerate(addrArray, KBTEngMaxAddrArraySize);

                        for(TInt i(0); i < addrArray.Count(); ++i)
                        {
                            TInquirySockAddr addr;
                            addr.SetBTAddr(addrArray[i]);
                            addr.SetAction(KHostResName);

                            TNameEntry entry;
                            TRequestStatus status;

                            hr.GetByAddress(addr, entry, status);
                            User::WaitForRequest(status);

                            const THostName& deviceName(entry().iName);
                            const QString name(QString::fromUtf16(reinterpret_cast<const ushort*>(deviceName.Ptr()), deviceName.Length()));
                            mConnections.append(name);
                        }
                    }

                    delete links;
                    hr.Close();
                }

                socketServ.Close();
            }
#endif

            mBtDiscoveryAgent->start();
            setScanning(true);

            if(mConnections.count())
            {
                emit connectionsChanged(mConnections);
                emit countChanged(mConnections.count());
            }
        }
    }
}

void BtScanner::stop()
{
    if(scanning())
    {
        mBtDiscoveryAgent->stop();
        delete mBtDiscoveryAgent;
        mBtDiscoveryAgent = 0;

        setScanning(false);
        emit connectionsChanged(mConnections);
    }
}

void BtScanner::clear()
{
    mConnections.clear();
    emit connectionsChanged(mConnections);
    emit countChanged(mConnections.count());
}

bool BtScanner::power() const
{
    QBluetoothLocalDevice btLocalDevice;
    return (btLocalDevice.hostMode() != QBluetoothLocalDevice::HostPoweredOff);
}

void BtScanner::powerOn()
{
    QBluetoothLocalDevice btLocalDevice;
    btLocalDevice.powerOn();
}

void BtScanner::setScanning(bool scanning)
{
    if(mScanning != scanning)
    {
        mScanning = scanning;
        emit scanningChanged(scanning);
    }
}

void BtScanner::deviceDiscovered(const QBluetoothDeviceInfo& info)
{
    if(!info.name().isEmpty() && !mConnections.contains(info.name()))
    {
        mConnections.append(info.name());
        emit connectionsChanged(mConnections);
        emit countChanged(mConnections.count());
    }
}

void BtScanner::finished()
{
    stop();
}

void BtScanner::error(QBluetoothDeviceDiscoveryAgent::Error)
{
    stop();
}

#ifdef Q_OS_SYMBIAN
void BtScanner::HandleCreateConnectionCompleteL(TInt /*aErr*/)
{
}

void BtScanner::HandleDisconnectCompleteL(TInt /*aErr*/)
{
}

void BtScanner::HandleDisconnectAllCompleteL(TInt /*aErr*/)
{
}
#endif
