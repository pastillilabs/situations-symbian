#include "nfcscanner.h"
#include <QProcess>

#ifdef Q_OS_SYMBIAN
#include <aknappui.h>
#include <gsfwviewuids.h>
#endif

NfcScanner::NfcScanner(QObject* parent)
    : QObject(parent)
{
    connect(&mNearFieldManager, SIGNAL(targetDetected(QNearFieldTarget*)), this, SLOT(targetDetected(QNearFieldTarget*)));
    connect(&mNearFieldManager, SIGNAL(targetLost(QNearFieldTarget*)), this, SLOT(targetLost(QNearFieldTarget*)));
    mNearFieldManager.setTargetAccessModes(QNearFieldManager::NoTargetAccess);
    mNearFieldManager.startTargetDetection();
}

NfcScanner::~NfcScanner()
{
    mNearFieldManager.stopTargetDetection();
}

const QVariant NfcScanner::uid() const
{
    return mUid.isEmpty() ? QVariant() : mUid;
}

bool NfcScanner::power() const
{
    return mNearFieldManager.isAvailable();
}

void NfcScanner::powerOn()
{
#ifdef Q_OS_SYMBIAN
     if(iAvkonAppUi) TRAP_IGNORE(iAvkonAppUi->ActivateViewL(TVwsViewId(KUidGS, KGSConPluginUid)));
#endif
}

void NfcScanner::targetDetected(QNearFieldTarget* target)
{
    if(target)
    {
        mUid = target->uid();
        emit uidChanged(uid());
    }
}

void NfcScanner::targetLost(QNearFieldTarget* /*target*/)
{
    mUid.clear();
    emit uidChanged(uid());
}

