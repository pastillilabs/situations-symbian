#include "accessorycondition.h"
#include <identifiers.h>
#include <QVariant>

AccessoryCondition::AccessoryCondition(QObject *parent)
    : Condition(parent)
    , mValue(-1)
    , mSystemDeviceInfo(0)
#ifdef Q_OS_SYMBIAN
    , mAccMonitor(0)
#endif
{
}

AccessoryCondition::~AccessoryCondition()
{
    doStop();
}

int AccessoryCondition::value() const
{
    return mValue;
}

void AccessoryCondition::setValue(const int value)
{
    if(value != mValue)
    {
        mValue = value;
        emit valueChanged(value);
    }
}

QString AccessoryCondition::nameById(const int id) const
{
    QString name;

    switch(id) {
    case TypeAudio:
        name = qtTrId("dialog_accessory_audio");
        break;
    case TypeVideo:
        name = qtTrId("dialog_accessory_video");
        break;
    case TypeCarKit:
        name = qtTrId("dialog_accessory_carkit");
        break;
    case TypeCharger:
        name = qtTrId("dialog_accessory_charger");
        break;
    default:
        break;
    }

    return name;
}

void AccessoryCondition::resolveActive()
{
    bool active(false);

#ifdef Q_OS_SYMBIAN
    RConnectedAccessories accessories;
    TRAP_IGNORE(mAccMonitor->GetConnectedAccessoriesL(accessories));

    const TInt count(accessories.Count());
    for(TInt i = 0; i < count && !active; ++i)
    {
        CAccMonitorInfo* accMonitorInfo(accessories[i]);
        const TAccMonCapability accDeviceType(accMonitorInfo->AccDeviceType());

        switch(mValue)
        {
        case TypeAudio:
            active = accDeviceType == KAccMonHeadset || accDeviceType == KAccMonLoopset;
            break;
        case TypeVideo:
            active = accDeviceType == KAccMonAVDevice;
            break;
        case TypeCarKit:
            active = accDeviceType == KAccMonCarKit;
            break;
        default:
            break;
        }
    }

    accessories.Close();
#endif

    if(mValue == TypeCharger)
    {
        const QSystemDeviceInfo::PowerState powerState(mSystemDeviceInfo->currentPowerState());
        active = (powerState == QSystemDeviceInfo::WallPower ||
                  powerState == QSystemDeviceInfo::WallPowerChargingBattery);
    }

    setActive(active);
}

void AccessoryCondition::powerStateChanged(QSystemDeviceInfo::PowerState /*state*/)
{
    resolveActive();
}

void AccessoryCondition::doStart()
{
    if(!mSystemDeviceInfo)
    {
        mSystemDeviceInfo = new QSystemDeviceInfo(this);
        connect(mSystemDeviceInfo, SIGNAL(powerStateChanged(QSystemDeviceInfo::PowerState)), this, SLOT(powerStateChanged(QSystemDeviceInfo::PowerState)));
    }

#ifdef Q_OS_SYMBIAN
    if(!mAccMonitor)
    {
        TRAPD(err, mAccMonitor = CAccMonitor::NewL());
        if(err == KErrNone) TRAP_IGNORE(mAccMonitor->StartObservingL(this));
    }
#endif

    resolveActive();
}

void AccessoryCondition::doStop()
{
    delete mSystemDeviceInfo;
    mSystemDeviceInfo = 0;

#ifdef Q_OS_SYMBIAN
    if(mAccMonitor) mAccMonitor->StopObserving();
    delete mAccMonitor;
    mAccMonitor = NULL;
#endif

    setActive(false);
}

void AccessoryCondition::doStore(QVariantMap& data) const
{
    data[Identifiers::keyValue] = mValue;
}

void AccessoryCondition::doRestore(const QVariantMap& data)
{
    setValue(data[Identifiers::keyValue].toInt());
}

#ifdef Q_OS_SYMBIAN
void AccessoryCondition::ConnectedL(CAccMonitorInfo* /*aAccessoryInfo*/)
{
    resolveActive();
}

void AccessoryCondition::DisconnectedL(CAccMonitorInfo* /*aAccessoryInfo*/)
{
    resolveActive();
}

void AccessoryCondition::AccMonitorObserverError(TInt /*aError*/)
{
}
#endif

