#include "batterycondition.h"
#include <identifiers.h>

static const char* KEY_LESS_THAN("lessThan");

BatteryCondition::BatteryCondition(QObject *parent)
    : Condition(parent)
    , mLessThan(true)
    , mValue(50)
    , mSystemDeviceInfo(0)
{
}

BatteryCondition::~BatteryCondition()
{
    doStop();
}

bool BatteryCondition::lessThan() const
{
    return mLessThan;
}

void BatteryCondition::setLessThan(bool lessThan)
{
    if(lessThan != mLessThan)
    {
        mLessThan = lessThan;
        emit lessThanChanged(lessThan);
    }
}

int BatteryCondition::value() const
{
    return mValue;
}

void BatteryCondition::setValue(const int value)
{
    if(value != mValue)
    {
        mValue = value;
        emit valueChanged(value);
    }
}

void BatteryCondition::resolveActive()
{
    const int level(mSystemDeviceInfo->batteryLevel());
    const bool active(mLessThan ? level < mValue : level > mValue);
    setActive(active);
}

void BatteryCondition::batteryLevelChanged(int /*level*/)
{
    resolveActive();
}


void BatteryCondition::doStart()
{
    if(!mSystemDeviceInfo)
    {
        mSystemDeviceInfo = new QSystemDeviceInfo(this);
        connect(mSystemDeviceInfo, SIGNAL(batteryLevelChanged(int)), this, SLOT(batteryLevelChanged(int)));
    }

    resolveActive();
}

void BatteryCondition::doStop()
{
    delete mSystemDeviceInfo;
    mSystemDeviceInfo = 0;

    setActive(false);
}

void BatteryCondition::doStore(QVariantMap& data) const
{
    data[Identifiers::keyValue] = mValue;
    data[KEY_LESS_THAN] = mLessThan;
}

void BatteryCondition::doRestore(const QVariantMap& data)
{
    setValue(data[Identifiers::keyValue].toInt());
    setLessThan(data[KEY_LESS_THAN].toBool());
}

