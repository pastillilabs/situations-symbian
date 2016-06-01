#include "connectivitycondition.h"
#include <identifiers.h>
#include <QStringList>

ConnectivityEngine ConnectivityCondition::mEngine;

ConnectivityCondition::ConnectivityCondition(QObject* parent)
    : Condition(parent)
    , mType(0)
{
}

ConnectivityCondition::~ConnectivityCondition()
{
    doStop();
}

int ConnectivityCondition::type() const
{
    return mType;
}

void ConnectivityCondition::setType(const int type)
{
    if(running()) doStop();

    if(type != mType)
    {
        mType = type;
        emit(typeChanged(type));
    }

    if(running()) doStart();
}

const QString& ConnectivityCondition::name() const
{
    return mName;
}

void ConnectivityCondition::setName(const QString& name)
{
    if(name != mName)
    {
        mName = name;
        emit(nameChanged(name));
    }

    resolveActive();
}

QString ConnectivityCondition::nameById(const int id) const
{
    QString name;

    switch(id)
    {
    case TypeWlan:
        name = qtTrId("connectivity_item_wlan");
        break;
    case TypeBt:
        name = qtTrId("connectivity_item_bluetooth");
        break;
    default:
        break;
    }

    return name;
}

void ConnectivityCondition::connectionsChanged(const QStringList& /*connections*/)
{
    resolveActive();
}

void ConnectivityCondition::resolveActive()
{
    bool active(false);
    bool scanning(false);
    switch(mType)
    {
    case TypeWlan:
        scanning = mEngine.scanningWlan();
        active = mEngine.wlanConnections().contains(mName);
        if(active) mEngine.doneWlan();
        break;
    case TypeBt:
        scanning = mEngine.scanningBt();
        active = mEngine.btConnections().contains(mName);
        if(active) mEngine.doneBt();
        break;
    default:
        break;
    }

    // Only set if not scanning or setting to active (connection found)
    if(active || !scanning) setActive(active);
}

void ConnectivityCondition::doStart()
{
    switch(mType)
    {
    case TypeWlan:
        mEngine.startWlan(*this);
        break;
    case TypeBt:
        mEngine.startBt(*this);
        break;
    default:
        break;
    }

    resolveActive();
}

void ConnectivityCondition::doStop()
{
    switch(mType)
    {
    case TypeWlan:
        mEngine.stopWlan(*this);
        break;
    case TypeBt:
        mEngine.stopBt(*this);
        break;
    default:
        break;
    }

    setActive(false);
}

void ConnectivityCondition::doStore(QVariantMap& data) const
{
    data[Identifiers::keyValue] = mType;
    data[Identifiers::keyName] = mName;
}

void ConnectivityCondition::doRestore(const QVariantMap& data)
{
    setType(data[Identifiers::keyValue].toInt());
    setName(data[Identifiers::keyName].toString());
}

