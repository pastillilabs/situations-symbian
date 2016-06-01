#ifndef SMSSENDACTION_H
#define SMSSENDACTION_H

#include <action.h>

/**
  * SmsSendAction
  */
class SmsSendAction
    : public Action
{
private:
    Q_OBJECT

    Q_PROPERTY(int contactId READ contactId WRITE setContactId NOTIFY contactIdChanged)
    Q_PROPERTY(QString contactNumber READ contactNumber WRITE setContactNumber NOTIFY contactNumberChanged)
    Q_PROPERTY(bool situationEnd READ situationEnd WRITE setSituationEnd NOTIFY situationEndChanged)

public:
    explicit SmsSendAction(QObject* parent = 0);
    ~SmsSendAction();

    void setContactId(int contactId);
    int contactId() const;

    void setContactNumber(const QString number);
    const QString contactNumber() const;

    void setSituationEnd(bool situationEnd);
    bool situationEnd() const;

    Q_INVOKABLE QString nameById(const int id) const;

signals:
    void contactIdChanged(int contactId);
    void contactNumberChanged(QString contactNumber);
    void situationEndChanged(bool situationEnd);

private: // From Action
    void doStart(bool running);
    void doStop(bool running);

    void doGetData(QVariantMap& data) const;
    void doSetData(const QVariantMap& data);

private:
    int mContactId;
    QString mContactNumber;
    bool mSituationEnd;
};

#endif // SMSSENDACTION_H

