#ifndef WLANSCANNER_SYMBIAN_H
#define WLANSCANNER_SYMBIAN_H

#include <wlanmgmtcommon.h>
#include <commdb.h>

class CWlanMgmtClient;
class CWlanScanInfo;

/**
  * CWlanScanner
  */
class CWlanScanner
    : public CActive
{
public:
    /**
      * Observer
      */
    class Observer
    {
    public:
        virtual void WlanScanComplete(const RArray<TWlanSsid>& wlanArray) = 0;
        virtual void NotifyError(const TInt aError) = 0;
    };

private:
    enum TWsfScanState
    {
        EIdle,
        EBroadcastScan,
        EProcessBroadcastScan
    };

public:
    static CWlanScanner* NewL(Observer& aObserver);
    ~CWlanScanner();

    void StartScanning();

private:
    CWlanScanner(Observer& aObserver);
    void ConstructL();

    TWlanSsid GetWlanSsidFromIapL(const TUint aIapID);

private: // From CActive
    void DoCancel();
    void RunL();
    TInt RunError(TInt aError);

private:
    Observer& iObserver;

    CWlanMgmtClient* iWlanMgmtClient;
    CWlanScanInfo* iScanInfo;
    CCommsDatabase* iCommsDb;

    TWsfScanState iScanState;

    RArray<TUint> iAvailableIaps;
    RArray<TWlanSsid> iWlanArray;
};

#endif

