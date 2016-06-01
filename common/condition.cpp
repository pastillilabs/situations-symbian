#include "condition.h"

Condition::Condition(QObject* parent)
    : QObject(parent)
    , mRunning(false)
    , mActive(false)
{
}

Condition::~Condition()
{
}

void Condition::start()
{
    mRunning = true;
    doStart();
}

void Condition::stop()
{
    mRunning = false;
    doStop();
}

bool Condition::running() const
{
    return mRunning;
}

bool Condition::active() const
{
    return mActive;
}

void Condition::setActive(bool active)
{
    if(mActive != active)
    {
        mActive = active;
        emit(activeChanged(mActive));
    }
}

const QVariantMap Condition::data() const
{
    QVariantMap data;
    doStore(data);
    return data;
}

void Condition::setData(const QVariantMap& data)
{
    doRestore(data);
    emit dataChanged(data);

    if(running()) doStart();
}

