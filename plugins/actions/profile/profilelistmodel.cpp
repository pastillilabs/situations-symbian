#include "profilelistmodel.h"

#ifdef Q_OS_SYMBIAN
#include <MProEngEngine.h>
#include <MProEngNotifyHandler.h>
#include <MProEngProfileNameArray.h>
#include <proengfactory.h>
#include <e32base.h>
#endif

ProfileListModel::ProfileListModel(QObject* parent)
    : QAbstractListModel(parent)
#ifdef Q_OS_SYMBIAN
    , mProfileNotifyHandler(0)
#endif
{
    QHash<int, QByteArray> roles;
    roles[NameRole] = "profileName";
    setRoleNames(roles);

#ifdef Q_OS_SYMBIAN
    QT_TRAP_THROWING(mProfileNotifyHandler = ProEngFactory::NewNotifyHandlerL());
    TRAP_IGNORE(mProfileNotifyHandler->RequestProfileNameArrayNotificationsL(*this));
#endif

    syncProfiles();
}

ProfileListModel::~ProfileListModel()
{
#ifdef Q_OS_SYMBIAN
    if(mProfileNotifyHandler) mProfileNotifyHandler->CancelAll();
    delete mProfileNotifyHandler;
#endif
}

int ProfileListModel::id(const int index) const
{
    int id(0);
    if(index >= 0 && index < mIdList.count())
    {
        id = mIdList.at(index);
    }

    return id;
}

int ProfileListModel::index(const int id) const
{
    return mIdList.indexOf(id);
}

void ProfileListModel::syncProfiles()
{
    beginResetModel();

    mNameList.clear();
    mIdList.clear();

#ifdef Q_OS_SYMBIAN
    MProEngEngine* engine(0);
    TRAPD(err, engine = ProEngFactory::NewEngineL());
    if(err == KErrNone)
    {
        TRAP_IGNORE(
            MProEngProfileNameArray* array(engine->ProfileNameArrayLC());
            for(TInt i(0); i < array->MdcaCount(); ++i)
            {
                const TPtrC name(array->MdcaPoint(i));
                const TInt id(array->ProfileId(i));

                mNameList.append(QString::fromUtf16(name.Ptr(), name.Length()));
                mIdList.append(id);
            }

            CleanupStack::PopAndDestroy(array);
        );

        engine->Release();
    }
#else
    mNameList << "Profile 0";
    mNameList << "Profile 1";
    mNameList << "Profile 2";
    mNameList << "Profile 3";
    mNameList << "Profile 4";
    mNameList << "Profile 5";

    mIdList << 0 << 1 << 2 << 3 << 4 << 5;
#endif

    endResetModel();
}

int ProfileListModel::rowCount(const QModelIndex& /*parent*/) const
{
    return mNameList.count();
}

QVariant ProfileListModel::data(const QModelIndex& index, int role) const
{
    QVariant data;
    if(index.row() >= 0 && index.row() < mNameList.count())
    {
        switch(role)
        {
        case NameRole:
            data = mNameList.at(index.row());
            break;
        default:
            Q_ASSERT(false);
        }
    }

    return data;
}

#ifdef Q_OS_SYMBIAN
void ProfileListModel::HandleProfileNameArrayModificationL()
{
    syncProfiles();
}

void ProfileListModel::HandleProfileNameArrayNotificationError(TInt /*error*/)
{
}
#endif

