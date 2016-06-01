#include "closeaction.h"
#include <identifiers.h>
#include <QDesktopServices>

#ifdef Q_OS_SYMBIAN
#include <w32std.h>
#include <apgcli.h>
#include <apgtask.h>
#endif

static const QChar STRING_TERMINATOR(156);

CloseAction::CloseAction(QObject* parent)
    : Action(parent)
    , mSituationEnd(false)
{
}

CloseAction::~CloseAction()
{
}

bool CloseAction::situationEnd() const
{
    return mSituationEnd;
}

void CloseAction::setSituationEnd(bool situationEnd)
{
    if(situationEnd != mSituationEnd)
    {
        mSituationEnd = situationEnd;
        emit situationEndChanged(situationEnd);
    }
}

const QString CloseAction::nameById(const QVariant id) const
{
#ifdef Q_OS_SYMBIAN
    QString name;
    const TInt uid(id.toInt());
    if(uid)
    {
        RApaLsSession apaLsSession;
        apaLsSession.Connect();
        TApaAppInfo appInfo;
        apaLsSession.GetAppInfo(appInfo, TUid::Uid(uid));
        apaLsSession.Close();

        name = QString::fromUtf16(appInfo.iCaption.Ptr(), appInfo.iCaption.Length());
    }
    else
    {
        name = qtTrId("action_close_all");
    }

    return name;
#endif

#ifdef Q_WS_SIMULATOR
    return QString("TestApp");
#endif
}

void CloseAction::close()
{
#ifdef Q_OS_SYMBIAN
    const TInt uid(value().toInt());
    RWsSession wsSession;
    wsSession.Connect();

    if(uid)
    {
        // Close single
        TApaTaskList taskList(wsSession);
        TApaTask task(taskList.FindApp(TUid::Uid(uid)));
        if(task.Exists()) task.EndTask();
    }
    else
    {
        // Close all
        RApaLsSession apaLsSession;
        apaLsSession.Connect();
        apaLsSession.GetAllApps();

        TApaAppInfo info;
        while(apaLsSession.GetNextApp(info) == KErrNone)
        {
            TApaAppCapabilityBuf capBuf;
            if(apaLsSession.GetAppCapability(capBuf, info.iUid) == KErrNone && capBuf().iAppIsHidden == EFalse)
            {
                TApaTaskList taskList(wsSession);
                TApaTask task(taskList.FindApp(info.iUid));
                if(task.Exists()) task.EndTask();
            }
        }

        apaLsSession.Close();
    }

    wsSession.Close();
#endif
}

void CloseAction::doStart(bool running)
{
    if(!mSituationEnd && !running) close();
}

void CloseAction::doStop(bool running)
{
    if(mSituationEnd && !running) close();
}

void CloseAction::doGetData(QVariantMap& data) const
{
    data[Identifiers::keySituationEnd] = mSituationEnd;
}

void CloseAction::doSetData(const QVariantMap& data)
{
    setSituationEnd(data[Identifiers::keySituationEnd].toBool());
}

