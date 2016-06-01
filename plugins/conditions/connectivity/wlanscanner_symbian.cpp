#include "wlanscanner.h"
#include <wlanmgmtclient.h>
#include <wlanscaninfo.h>
#include <wlancontainer.h>

CWlanScanner* CWlanScanner::NewL(Observer& aObserver)
{
    CWlanScanner* self(new(ELeave) CWlanScanner(aObserver));
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
}

CWlanScanner::~CWlanScanner()
{
    Cancel();
    iAvailableIaps.Close();
    iWlanArray.Close();

    delete iCommsDb;
    delete iWlanMgmtClient;
    delete iScanInfo;
}

void CWlanScanner::StartScanning()
{
    if(iScanState == EIdle && !IsActive())
    {
        TRAP_IGNORE(RunL());
    }
}

CWlanScanner::CWlanScanner(Observer& aObserver)
    : CActive(EPriorityStandard)
    , iObserver(aObserver)
    , iScanState(EIdle)
{
}

void CWlanScanner::ConstructL()
{
    iWlanMgmtClient = CWlanMgmtClient::NewL();
    iScanInfo = CWlanScanInfo::NewL();
    iCommsDb = CCommsDatabase::NewL();
    
    CActiveScheduler::Add(this);
}

void CWlanScanner::DoCancel()
{
    iWlanMgmtClient->CancelGetAvailableIaps();
    iWlanMgmtClient->CancelGetScanResults();

    iScanState = EIdle;
}

void CWlanScanner::RunL()
{
    if(iScanState == EIdle)
    {
        iAvailableIaps.Reset();
        iWlanArray.Reset();

        // Get available IAPs
        TInt cacheLifetime(0);
        TUint maxDelay(0);
        iWlanMgmtClient->GetAvailableIaps(cacheLifetime, maxDelay, iStatus, iAvailableIaps);
        SetActive();

        iScanState = EBroadcastScan;
    }
    else if(iScanState == EBroadcastScan)
    {
        User::LeaveIfError(iStatus.Int());

        // Process available IAPs
        const TInt iapCount(iAvailableIaps.Count());
        for(TInt i(0); i < iapCount; ++i)
        {
            TWlanSsid ssid;
            TRAPD(error, ssid = GetWlanSsidFromIapL(iAvailableIaps[i]));
            if(error != KErrNotFound) User::LeaveIfError(error);
            if(error == KErrNone) iWlanArray.AppendL(ssid);
        }

        // Do scan
        iWlanMgmtClient->GetScanResults(iStatus, *iScanInfo);
        SetActive();

        iScanState = EProcessBroadcastScan;
    }
    else if(iScanState == EProcessBroadcastScan)
    {
        User::LeaveIfError(iStatus.Int());

        // Process scan results
        for(iScanInfo->First(); !iScanInfo->IsDone(); iScanInfo->Next())
        {
            // Get ssid
            TUint8 ieLen(0);
            const TUint8* ieData(0);
            const TInt ret(iScanInfo->InformationElement(0/*802Dot11SsidIE*/, ieLen, &ieData));
            User::LeaveIfError(ret);
            if(ieLen)
            {
                TWlanSsid ssid;
                ssid.Copy(ieData, ieLen);
                iWlanArray.AppendL(ssid);
            }
        }

        // Notify scan completed
        iObserver.WlanScanComplete(iWlanArray);
        iScanState = EIdle;
    }
}

TInt CWlanScanner::RunError(TInt aError)
{
    iAvailableIaps.Reset();
    iWlanArray.Reset();
    
    if(aError != KErrNotReady) iObserver.NotifyError(aError);
    iScanState = EIdle;
    
    return KErrNone;
}

TWlanSsid CWlanScanner::GetWlanSsidFromIapL(const TUint aIapID)
{
    TWlanSsid ssid;
    TUint32 serviceId(0);

    CCommsDbTableView* commsDbIapTableView(iCommsDb->OpenViewMatchingUintLC(TPtrC(IAP), TPtrC(COMMDB_ID), aIapID));
    User::LeaveIfError(commsDbIapTableView->GotoFirstRecord());
    commsDbIapTableView->ReadUintL(TPtrC(IAP_SERVICE), serviceId);
    CleanupStack::PopAndDestroy(commsDbIapTableView);

    CCommsDbTableView* wlanTableView(iCommsDb->OpenViewMatchingUintLC(TPtrC(WLAN_SERVICE), TPtrC(WLAN_SERVICE_ID), serviceId));
    User::LeaveIfError(wlanTableView->GotoFirstRecord());
    wlanTableView->ReadTextL(TPtrC(NU_WLAN_SSID), ssid);
    CleanupStack::PopAndDestroy(wlanTableView);

    return ssid;
}

