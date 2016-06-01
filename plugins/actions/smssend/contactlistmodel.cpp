#include "contactlistmodel.h"
#include <QContactPhoneNumber>
#include <QContactName>
#include <QContactDisplayLabel>
#include <QContactAction>
#include <QContactDetailFilter>

ContactListModel::ContactListModel(QObject* parent)
    : QAbstractListModel(parent)
{
    QHash<int, QByteArray> roles;
    roles[NameRole] = "contactName";
    roles[IdRole] = "contactId";
    setRoleNames(roles);

    updateContacts();
}

void ContactListModel::updateContacts()
{
    QContactManager contactManager;

    QContactDetailFilter filter;
    filter.setDetailDefinitionName(QContactType::DefinitionName, QContactType::FieldType);
    filter.setMatchFlags(QContactFilter::MatchExactly);
    filter.setValue(QContactType::TypeContact);

    QContactSortOrder sortOrder;
    sortOrder.setDetailDefinitionName(QContactName::DefinitionName, QContactName::FieldLastName);
    sortOrder.setCaseSensitivity(Qt::CaseInsensitive);
    sortOrder.setBlankPolicy(QContactSortOrder::BlanksLast);
    sortOrder.setDirection(Qt::AscendingOrder);

    QList<QContactSortOrder> sortOrders;
    sortOrders.append(sortOrder);

    QList<QContact> contactList(contactManager.contacts(filter, sortOrders));
    foreach(const QContact& contact, contactList)
    {
        QString contactName;
        const QContactName name(contact.detail<QContactName>());
        const QList<QContactPhoneNumber> numbers(contact.details<QContactPhoneNumber>());

        QStringList mobileNumbers;
        foreach(const QContactPhoneNumber& number, numbers)
        {
            if(number.subTypes().contains(QContactPhoneNumber::SubTypeMobile)) mobileNumbers.append(number.number());
        }

        if(!mobileNumbers.isEmpty())
        {
            if(name.lastName().isEmpty() || name.firstName().isEmpty())
            {
                contactName = name.firstName() + name.lastName();
            }
            else
            {
                contactName = qtTrId("action_sms_send_recipient").arg(name.firstName()).arg(name.lastName());
            }

            if(!contactName.isEmpty())
            {
                mContactNames.append(contactName);
                mContactIds.append(contact.localId());
                mContactNumbers.append(mobileNumbers);
            }
        }
    }
}

int ContactListModel::indexById(const int id) const
{
    return mContactIds.indexOf(id);
}

int ContactListModel::idByIndex(const int index) const
{
    return mContactIds.at(index);
}

QString ContactListModel::name(const int index) const
{
    return mContactNames.at(index);
}

QStringList ContactListModel::numbers(const int index) const
{
    return mContactNumbers.at(index);
}

int ContactListModel::count() const
{
    return mContactIds.count();
}

int ContactListModel::rowCount(const QModelIndex& /*parent*/) const
{
    return mContactIds.count();
}

QVariant ContactListModel::data(const QModelIndex& index, int role) const
{
    QVariant data;
    if(index.row() >= 0 && index.row() < mContactIds.count())
    {
        switch(role)
        {
        case NameRole:
            data = name(index.row());
            break;
        case IdRole:
            data = mContactIds.at(index.row());
            break;
        default:
            Q_ASSERT(false);
        }
    }

    return data;
}

