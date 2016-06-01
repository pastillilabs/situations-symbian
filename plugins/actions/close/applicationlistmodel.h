#ifndef APPLICATIONLISTMODEL_H
#define APPLICATIONLISTMODEL_H

#include <QAbstractListModel>

/**
  * ApplicationListModel
  */
class ApplicationListModel
    : public QAbstractListModel
{
private:
    Q_OBJECT

    enum Roles {
        NameRole = Qt::UserRole + 1,
        UidRole
    };

public:
    explicit ApplicationListModel(QObject* parent = 0);

    Q_INVOKABLE void updateApps();

private: // From QAbstractListModel
    int rowCount(const QModelIndex& parent = QModelIndex()) const;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;

private:
    QMap<QString, QVariant> mApplicationList;
};

#endif // APPLICATIONLISTMODEL_H

