#include "applicationlistmodel.h"

#ifdef Q_OS_SYMBIAN
#include <apgcli.h>
#include <FavouritesSession.h>
#include <FavouritesDb.h>
#include <FavouritesItemList.h>
#endif

static const QChar STRING_TERMINATOR(156);

ApplicationListModel::ApplicationListModel(QObject* parent)
    : QAbstractListModel(parent)
{
    QHash<int, QByteArray> roles;
    roles[NameRole] = "itemName";
    roles[UidRole] = "itemUid";
    setRoleNames(roles);
}

void ApplicationListModel::update()
{
    beginResetModel();

    mItemMap.clear();

#ifdef Q_OS_SYMBIAN
    RApaLsSession apaLsSession;
    apaLsSession.Connect();
    apaLsSession.GetAllApps();

    TApaAppInfo info;
    while(apaLsSession.GetNextApp(info) == KErrNone)
    {
        TApaAppCapabilityBuf capBuf;
        if(apaLsSession.GetAppCapability(capBuf, info.iUid) == KErrNone && capBuf().iAppIsHidden == EFalse)
        {
            const int uid(info.iUid.iUid);
            mItemMap.insert(QString::fromUtf16(info.iCaption.Ptr(), info.iCaption.Length()), uid);
        }
    }

    apaLsSession.Close();
#endif

#ifdef Q_WS_SIMULATOR
    mItemMap.insert("App1", 1);
    mItemMap.insert("App2", 2);
    mItemMap.insert("App3", 3);
#endif

    endResetModel();
}

int ApplicationListModel::rowCount(const QModelIndex& /*parent*/) const
{
    return mItemMap.count();
}

QVariant ApplicationListModel::data(const QModelIndex& index, int role) const
{
    QVariant data;
    if(index.row() >= 0 && index.row() < mItemMap.count())
    {
        switch(role)
        {
        case NameRole:
            data = mItemMap.keys().at(index.row());
            break;
        case UidRole:
            data = mItemMap.values().at(index.row());
            break;
        default:
            Q_ASSERT(false);
            break;
        }
    }

    return data;
}

