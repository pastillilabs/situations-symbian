#include "themelistmodel.h"

#ifdef Q_OS_SYMBIAN
#include <e32base.h>
#include <aknsutils.h>
#include <AknSSrvClient.h>
#include <AknsPkgID.h>
#endif

ThemeListModel::ThemeListModel(QObject* parent)
    : QAbstractListModel(parent)
{
    QHash<int, QByteArray> roles;
    roles[NameRole] = "themeName";
    setRoleNames(roles);

#ifdef Q_OS_SYMBIAN
    TRAP_IGNORE(syncThemes());
#else
    syncThemes();
#endif
}

ThemeListModel::~ThemeListModel()
{
}

int ThemeListModel::id(const int index) const
{
    int id(0);
    if(index >= 0 && index < mIdList.count())
    {
        id = mIdList.at(index);
    }

    return id;
}

int ThemeListModel::index(const int id) const
{
    return mIdList.indexOf(id);
}

void ThemeListModel::syncThemes()
{
    beginResetModel();

    mNameList.clear();
    mIdList.clear();

#ifdef Q_OS_SYMBIAN
    RAknsSrvSession aknsSrvSession;
    TInt err(aknsSrvSession.Connect());
    if(err == KErrNone)
    {
        CArrayPtr<CAknsSrvSkinInformationPkg>* skinInfoArray(0);
        TRAP(err, skinInfoArray = aknsSrvSession.EnumerateSkinPackagesL(EAknsSrvAll));
        if(err == KErrNone)
        {
            for(TInt i(0); i < skinInfoArray->Count(); ++i)
            {
                const CAknsSrvSkinInformationPkg* info(skinInfoArray->At(i));

                const TPtrC name(info->Name());
                const TInt id(info->PID().iNumber);

                mNameList.append(QString::fromUtf16(name.Ptr(), name.Length()));
                mIdList.append(id);
            }

            skinInfoArray->ResetAndDestroy();
            delete skinInfoArray;
        }

        aknsSrvSession.Close();
    }
#else
    // Dummy data
    mNameList << "Theme 1";
    mNameList << "Theme 2";
    mNameList << "Theme 3";
    mIdList << 0;
    mIdList << 1;
    mIdList << 2;
#endif

    endResetModel();
}

int ThemeListModel::rowCount(const QModelIndex& /*parent*/) const
{
    return mNameList.count();
}

QVariant ThemeListModel::data(const QModelIndex& index, int role) const
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

