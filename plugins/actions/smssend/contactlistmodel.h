#ifndef CONTACTLISTMODEL_H
#define CONTACTLISTMODEL_H

#include <QStringList>
#include <QAbstractListModel>
#include <QContactManager>

QTM_USE_NAMESPACE

/**
  * ContactListModel
  */
class ContactListModel
    : public QAbstractListModel
{
private:
    Q_OBJECT

    enum Roles {
        NameRole = Qt::UserRole + 1,
        IdRole
    };

public:
    explicit ContactListModel(QObject* parent = 0);

    void updateContacts();

    Q_INVOKABLE int indexById(const int id) const;
    Q_INVOKABLE int idByIndex(const int index) const;
    Q_INVOKABLE QString name(const int index) const;
    Q_INVOKABLE QStringList numbers(const int index) const;
    Q_INVOKABLE int count() const;

private: // From QAbstractListModel
    int rowCount(const QModelIndex& parent = QModelIndex()) const;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;

private:
    QList<QContactLocalId> mContactIds;
    QStringList mContactNames;
    QList<QStringList> mContactNumbers;
};

#endif // CONTACTLISTMODEL_H

