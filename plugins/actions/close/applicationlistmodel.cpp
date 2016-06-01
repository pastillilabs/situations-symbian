#include "applicationlistmodel.h"

#ifdef Q_OS_SYMBIAN
#include <apgcli.h>
#endif

static const QChar STRING_TERMINATOR(156);

ApplicationListModel::ApplicationListModel(QObject* parent)
    : QAbstractListModel(parent)
{
    QHash<int, QByteArray> roles;
    roles[NameRole] = "applicationName";
    roles[UidRole] = "applicationUid";
    setRoleNames(roles);
}

void ApplicationListModel::updateApps()
{
    beginResetModel();

    mApplicationList.clear();

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
            mApplicationList.insert(QString::fromUtf16(info.iCaption.Ptr(), info.iCaption.Length()), uid);
        }
    }

    apaLsSession.Close();
#endif

#ifdef Q_WS_SIMULATOR
    // Dummy data
    mApplicationList.insert("App1", 1);
    mApplicationList.insert("App2", 2);
    mApplicationList.insert("App3", 3);
#endif

    endResetModel();
}

int ApplicationListModel::rowCount(const QModelIndex& /*parent*/) const
{
#ifdef Q_OS_SYMBIAN
    return mApplicationList.count() + 1;
#endif
#ifdef Q_WS_SIMULATOR
    return mApplicationList.count();
#endif
}

QVariant ApplicationListModel::data(const QModelIndex& index, int role) const
{
    QVariant data;
    if(index.row() >= 0 && index.row() < mApplicationList.count())
    {
        switch(role)
        {
        case NameRole:
#ifdef Q_OS_SYMBIAN
            if(index.row() == 0) data = qtTrId("action_close_all");
            else data = mApplicationList.keys().at(index.row() - 1);
#endif
#ifdef Q_WS_SIMULATOR
            data = mApplicationList.keys().at(index.row());
#endif
            break;
        case UidRole:
#ifdef Q_OS_SYMBIAN
            if(index.row() == 0) data = 0;
            else data = mApplicationList.values().at(index.row() - 1);
#endif
#ifdef Q_WS_SIMULATOR
            data = mApplicationList.values().at(index.row());
#endif
            break;
        default:
            Q_ASSERT(false);
        }
    }

    return data;
}

