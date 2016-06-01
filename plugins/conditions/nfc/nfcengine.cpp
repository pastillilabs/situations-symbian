#include "nfcengine.h"
#include "nfcscanner.h"

NfcEngine::NfcEngine(QObject* parent)
    : QObject(parent)
    , mScanner(0)
{
}

NfcEngine::~NfcEngine()
{
    delete mScanner;
}

void NfcEngine::start(QObject& client)
{
    if(!mScanner) mScanner = new NfcScanner(this);

    if(!mClients.contains(&client))
    {
        mClients.append(&client);
        connect(mScanner, SIGNAL(uidChanged(QVariant)), &client, SLOT(engineUidChanged(QVariant)));
    }
}

void NfcEngine::stop(QObject& client)
{
    mClients.removeAll(&client);
    disconnect(mScanner, SIGNAL(uidChanged(QVariant)), &client, SLOT(engineUidChanged(QVariant)));

    if(mClients.isEmpty())
    {
        delete mScanner;
        mScanner = 0;
    }
}

const QVariant NfcEngine::uid() const
{
    QVariant uid;
    if(mScanner) uid = mScanner->uid();
    return uid;
}

