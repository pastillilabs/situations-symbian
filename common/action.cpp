#include "action.h"
#include <identifiers.h>
#include <QTimerEvent>

Action::Action(QObject* parent)
    : QObject(parent)
    , mRunning(false)
    , mStartTimerId(0)
    , mStartDelay(0)
    , mRestore(false)
{
}

Action::~Action()
{
    if(mStartTimerId) killTimer(mStartTimerId);
}

void Action::initialize()
{
    setValue(currentValue());
}

void Action::start(bool running)
{
    doCancelRestore();

    if(!mRunning && running)
    {
        doStart(true);
        doExecute(mValue);
        mRunning = true;
    }

    if(!mRunning && !mStartTimerId)
    {
        mStartTimerId = startTimer(mStartDelay);
    }
}

void Action::stop(bool running)
{
    if(mStartTimerId)
    {
        killTimer(mStartTimerId);
        mStartTimerId = 0;
        mRunning = false;
    }

    if(mRunning)
    {
        if(mRestore) doRestore();
        doStop(running);
        mRunning = false;
    }
}

bool Action::running() const
{
    return mRunning;
}

const QVariant Action::value() const
{
    return mValue;
}

void Action::setValue(const QVariant value)
{
    if(mValue != value)
    {
        mValue = value;
        emit valueChanged(value);
    }
}

const QVariantMap Action::data() const
{
    QVariantMap data;
    data[Identifiers::keyValue] = mValue;
    data[Identifiers::keyStartDelay] = mStartDelay;
    data[Identifiers::keyRestore] = mRestore;
    doGetData(data);
    return data;
}

void Action::setData(const QVariantMap data)
{
    setValue(data[Identifiers::keyValue]);
    if(data.contains(Identifiers::keyStartDelay)) setStartDelay(data[Identifiers::keyStartDelay].toInt());
    setRestore(data[Identifiers::keyRestore].toBool());
    doSetData(data);
    emit dataChanged(data);

    if(mRunning) doExecute(mValue);
}

int Action::startDelay() const
{
    return mStartDelay;
}

void Action::setStartDelay(const int startDelay)
{
    if(startDelay != mStartDelay)
    {
        mStartDelay = startDelay;
        emit startDelayChanged(startDelay);
    }
}

bool Action::restore() const
{
    return mRestore;
}

void Action::setRestore(const bool restore)
{
    if(restore != mRestore)
    {
        mRestore = restore;
        emit restoreChanged(restore);
    }
}

const QVariant Action::currentValue() const
{
    return QVariant();
}

void Action::doExecute(const QVariant& /*value*/)
{
}

void Action::doStart(bool /*running*/)
{
}

void Action::doStop(bool /*running*/)
{
}

void Action::doRestore()
{
}

void Action::doCancelRestore()
{
}

void Action::doGetData(QVariantMap& /*data*/) const
{
}

void Action::doSetData(const QVariantMap& /*data*/)
{
}

void Action::timerEvent(QTimerEvent* event)
{
    const int timerId(event->timerId());
    if(timerId == mStartTimerId)
    {
        killTimer(mStartTimerId);
        mStartTimerId = 0;
        doStart(false);
        doExecute(mValue);
        mRunning = true;
    }
}

