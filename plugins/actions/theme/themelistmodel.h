#ifndef THEMELISTMODEL_H
#define THEMELISTMODEL_H

#include <QStringList>
#include <QAbstractListModel>

/**
  * ThemeListModel
  */
class ThemeListModel
    : public QAbstractListModel
{
private:
    Q_OBJECT

    enum Roles {
        NameRole = Qt::UserRole + 1
    };

public:
    explicit ThemeListModel(QObject* parent = 0);
    ~ThemeListModel();

    Q_INVOKABLE int id(const int index) const;
    Q_INVOKABLE int index(const int id) const;

private:
    void syncThemes();

private: // From QAbstractListModel
    int rowCount(const QModelIndex& parent = QModelIndex()) const;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;

private:
    QStringList mNameList;
    QList<int> mIdList;
};

#endif // THEMELISTMODEL_H

