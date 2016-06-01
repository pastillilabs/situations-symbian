#include "nfccondition.h"
#include <identifiers.h>

NfcEngine NfcCondition::mNfcEngine;

NfcCondition::NfcCondition(QObject *parent)
    : Condition(parent)
    , mVirtualTag(false)
    , mVirtualActive(false)
    , mScanning(false)
{
    mNfcEngine.start(*this);
}

NfcCondition::~NfcCondition()
{
    mNfcEngine.stop(*this);
    doStop();
}

QString NfcCondition::name() const
{
    return mName;
}

void NfcCondition::setName(const QString name)
{
    if(name != mName)
    {
        mName = name;
        emit nameChanged(name);
    }
}

QVariant NfcCondition::uid() const
{
    return mUid;
}

void NfcCondition::setUid(const QVariant uid)
{
    if(uid != mUid)
    {
        mUid = uid;
        emit uidChanged(uid);
    }
}

bool NfcCondition::virtualTag() const
{
    return mVirtualTag;
}

void NfcCondition::setVirtualTag(bool virtualTag)
{
    if(virtualTag != mVirtualTag)
    {
        mVirtualTag = virtualTag;
        emit virtualTagChanged(virtualTag);

        mVirtualActive = false;
        if(running()) resolveActive();
    }
}

QString NfcCondition::uidString() const
{
    return uid().toByteArray().toHex();
}

void NfcCondition::resolveActive()
{
    bool active(false);
    if(mNfcEngine.uid().isValid())
    {
        active = (mNfcEngine.uid() == mUid);

        if(active && mVirtualTag) mVirtualActive = !mVirtualActive;
    }

    if(running() && mScanning) setActive(active || mVirtualActive);
}

void NfcCondition::engineUidChanged(const QVariant /*uid*/)
{
    resolveActive();
}

void NfcCondition::doStart()
{
    mScanning = true;
    resolveActive();
}

void NfcCondition::doStop()
{
    setActive(false);
    mScanning = false;
}

void NfcCondition::doStore(QVariantMap &data) const
{
    data[Identifiers::keyName] = mName;
    data[Identifiers::keyValue] = mUid;
    data[Identifiers::keyType] = mVirtualTag;
}

void NfcCondition::doRestore(const QVariantMap& data)
{
    setName(data[Identifiers::keyName].toString());
    setUid(data[Identifiers::keyValue]);
    setVirtualTag(data[Identifiers::keyType].toBool());
}

