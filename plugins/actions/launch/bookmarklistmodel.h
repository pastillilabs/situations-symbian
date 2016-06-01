#ifndef BOOKMARKLISTMODEL_H
#define BOOKMARKLISTMODEL_H

#include <QAbstractListModel>

/**
  * BookmarkListModel
  */
class BookmarkListModel
    : public QAbstractListModel
{
private:
    Q_OBJECT

    enum Roles {
        NameRole = Qt::UserRole + 1,
        UidRole
    };

public:
    explicit BookmarkListModel(QObject* parent = 0);

    Q_INVOKABLE void update();

private: // From QAbstractListModel
    int rowCount(const QModelIndex& parent = QModelIndex()) const;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;

private:
    QMap<QString, QVariant> mItemMap;
};

#endif // BOOKMARKLISTMODEL_H

