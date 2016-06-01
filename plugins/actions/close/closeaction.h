#ifndef CLOSEACTION_H
#define CLOSEACTION_H

#include <action.h>

/**
  * CloseAction
  */
class CloseAction
    : public Action
{
private:
    Q_OBJECT
    Q_PROPERTY(bool situationEnd READ situationEnd WRITE setSituationEnd NOTIFY situationEndChanged)

public:
    explicit CloseAction(QObject* parent = 0);
    ~CloseAction();

    bool situationEnd() const;
    void setSituationEnd(bool situationEnd);

    Q_INVOKABLE const QString nameById(const QVariant id) const;

private:
    void close();

signals:
    void situationEndChanged(bool situationEnd);

private: // From Action
    void doStart(bool running);
    void doStop(bool running);

    void doGetData(QVariantMap& data) const;
    void doSetData(const QVariantMap& data);

private:
    bool mSituationEnd;
};

#endif // CLOSEACTION_H

