#ifndef PROFILELISTMODEL_H
#define PROFILELISTMODEL_H

#include <QStringList>
#include <QAbstractListModel>

#ifdef Q_OS_SYMBIAN
#include <MProEngProfileNameArrayObserver.h>

class MProEngEngine;
class MProEngNotifyHandler;
#endif

/**
  * ProfileListModel
  */
class ProfileListModel
    : public QAbstractListModel
#ifdef Q_OS_SYMBIAN
    , private MProEngProfileNameArrayObserver
#endif
{
private:
    Q_OBJECT

    enum Roles {
        NameRole = Qt::UserRole + 1
    };

public:
    explicit ProfileListModel(QObject* parent = 0);
    ~ProfileListModel();

    Q_INVOKABLE int id(const int index) const;
    Q_INVOKABLE int index(const int id) const;

private:
    void syncProfiles();

private: // From QAbstractListModel
    int rowCount(const QModelIndex& parent = QModelIndex()) const;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;

#ifdef Q_OS_SYMBIAN
private: // From MProEngProfileNameArrayObserver
    void HandleProfileNameArrayModificationL();
    void HandleProfileNameArrayNotificationError(TInt error);
#endif

private:
    QStringList mNameList;
    QList<int> mIdList;

#ifdef Q_OS_SYMBIAN
    MProEngNotifyHandler* mProfileNotifyHandler;
#endif
};

#endif // PROFILELISTMODEL_H

