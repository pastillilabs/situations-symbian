#include "launchaction.h"
#include <identifiers.h>
#include <QDesktopServices>
#include <QUrl>

#ifdef Q_OS_SYMBIAN
#include <w32std.h>
#include <apgcli.h>
#include <apgtask.h>
#include <apacmdln.h>
#include <FavouritesSession.h>
#include <FavouritesDb.h>
#include <FavouritesItemList.h>
#endif

static const QChar STRING_TERMINATOR(156);
static const int DEFAULT_DELAY(1000);

LaunchAction::LaunchAction(QObject* parent)
    : Action(parent)
    , mType(TypeApplication)
    , mSituationEnd(false)
{
    setStartDelay(DEFAULT_DELAY);
}

LaunchAction::~LaunchAction()
{
}

int LaunchAction::type() const
{
    return mType;
}

void LaunchAction::setType(const int type)
{
    if(type != mType)
    {
        mType = type;
        emit typeChanged(type);
        emit valueChanged(value()); // Emit also value update to refresh item UI correctly
    }
}

bool LaunchAction::situationEnd() const
{
    return mSituationEnd;
}

void LaunchAction::setSituationEnd(bool situationEnd)
{
    if(situationEnd != mSituationEnd)
    {
        mSituationEnd = situationEnd;
        emit(situationEndChanged(situationEnd));
    }
}

const QString LaunchAction::nameById(const QVariant id) const
{
    QString name;

#ifdef Q_OS_SYMBIAN
    const TInt uid(value().toInt());
    if(mType == TypeApplication)
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
        RFavouritesSession favouritesSession;
        favouritesSession.Connect();

        RFavouritesDb favouritesDb;
        favouritesDb.Open(favouritesSession, KBrowserBookmarks);

        CFavouritesItemList* list(new CFavouritesItemList());
        favouritesDb.GetAll(*list, KFavouritesNullUid, CFavouritesItem::EItem);
        const CFavouritesItem* item(list->ItemByUid(uid));
        if(item) name = QString::fromUtf16(item->Name().Ptr(), item->Name().Length());

        delete list;
        favouritesDb.Close();
        favouritesSession.Close();
    }
#endif

#ifdef Q_WS_SIMULATOR
    name = id.toString();
#endif

    return name;
}

void LaunchAction::launch()
{
#ifdef Q_OS_SYMBIAN
    const TInt uid(value().toInt());
    if(mType == TypeApplication)
    {
        RWsSession wsSession;
        wsSession.Connect();

        TApaTaskList taskList(wsSession);
        TApaTask task(taskList.FindApp(TUid::Uid(uid)));
        if(!task.Exists())
        {
            RApaLsSession apaLsSession;
            apaLsSession.Connect();

            TApaAppInfo appInfo;
            if(apaLsSession.GetAppInfo(appInfo, TUid::Uid(uid)) == KErrNone)
            {
                CApaCommandLine* cmdLine(0);
                TRAPD(err, cmdLine = CApaCommandLine::NewL());
                if(err == KErrNone)
                {
                    TRAP_IGNORE(cmdLine->SetExecutableNameL(appInfo.iFullName));
                    TRAP_IGNORE(cmdLine->SetCommandL(EApaCommandRun));
                    apaLsSession.StartApp(*cmdLine);
                    delete cmdLine;
                }
            }

            apaLsSession.Close();
        }
        else
        {
            task.BringToForeground();
        }

        wsSession.Close();
    }
    else
    {
        RFavouritesSession favouritesSession;
        favouritesSession.Connect();

        RFavouritesDb favouritesDb;
        favouritesDb.Open(favouritesSession, KBrowserBookmarks);

        CFavouritesItemList* list(new CFavouritesItemList());
        favouritesDb.GetAll(*list, KFavouritesNullUid, CFavouritesItem::EItem);
        const CFavouritesItem* item(list->ItemByUid(uid));
        if(item)
        {
            QDesktopServices::openUrl(QUrl(QString::fromUtf16(item->Url().Ptr(), item->Url().Length())));
        }

        delete list;
        favouritesDb.Close();
        favouritesSession.Close();
    }
#endif
}

void LaunchAction::doStart(bool running)
{
    if(!mSituationEnd && !running) launch();
}

void LaunchAction::doStop(bool running)
{
    if(mSituationEnd && !running) launch();
}

void LaunchAction::doGetData(QVariantMap& data) const
{
    data[Identifiers::keyType] = mType;
    data[Identifiers::keySituationEnd] = mSituationEnd;
}

void LaunchAction::doSetData(const QVariantMap& data)
{
    setType(data[Identifiers::keyType].toInt());
    setSituationEnd(data[Identifiers::keySituationEnd].toBool());
}

