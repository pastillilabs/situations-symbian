#include "bookmarklistmodel.h"

#ifdef Q_OS_SYMBIAN
#include <apgcli.h>
#include <FavouritesSession.h>
#include <FavouritesDb.h>
#include <FavouritesItemList.h>
#endif

BookmarkListModel::BookmarkListModel(QObject* parent)
    : QAbstractListModel(parent)
{
    QHash<int, QByteArray> roles;
    roles[NameRole] = "itemName";
    roles[UidRole] = "itemUid";
    setRoleNames(roles);
}

void BookmarkListModel::update()
{
    beginResetModel();

    mItemMap.clear();

#ifdef Q_OS_SYMBIAN
    RFavouritesSession favouritesSession;
    favouritesSession.Connect();

    RFavouritesDb favouritesDb;
    favouritesDb.Open(favouritesSession, KBrowserBookmarks);

    CFavouritesItemList* list(new CFavouritesItemList()); // Must create on heap
    favouritesDb.GetAll(*list, KFavouritesNullUid, CFavouritesItem::EItem);

    for(TInt i(0); i < list->Count(); ++i)
    {
        CFavouritesItem* item(list->At(i));
        TPtrC name(item->Name());
        if(!name.Length()) name.Set(item->Url());

        mItemMap.insert(QString::fromUtf16(name.Ptr(), name.Length()), item->Uid());
    }

    delete list;
    list = 0;
    favouritesDb.Close();
    favouritesSession.Close();
#endif

#ifdef Q_WS_SIMULATOR
    mItemMap.insert("Bookmark1", 1);
    mItemMap.insert("Bookmark2", 2);
    mItemMap.insert("Bookmark3", 3);
#endif

    endResetModel();
}

int BookmarkListModel::rowCount(const QModelIndex& /*parent*/) const
{
    return mItemMap.count();
}

QVariant BookmarkListModel::data(const QModelIndex& index, int role) const
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

