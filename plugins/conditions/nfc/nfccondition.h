#ifndef NFCCONDITION_H
#define NFCCONDITION_H

#include <condition.h>
#include "nfcengine.h"

/**
  * NfcCondition
  */
class NfcCondition
    : public Condition
{
private:
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QVariant uid READ uid WRITE setUid NOTIFY uidChanged)
    Q_PROPERTY(bool virtualTag READ virtualTag WRITE setVirtualTag NOTIFY virtualTagChanged)

public:
    NfcCondition(QObject* parent = 0);
    ~NfcCondition();

    QString name() const;
    void setName(const QString name);

    QVariant uid() const;
    void setUid(const QVariant uid);

    bool virtualTag() const;
    void setVirtualTag(bool virtualTag);

    Q_INVOKABLE QString uidString() const;

signals:
    void nameChanged(QString name);
    void uidChanged(QVariant uid);
    void virtualTagChanged(bool virtualTag);

private:
    void resolveActive();

private slots:
    void engineUidChanged(const QVariant uid);

private: // From Condition
    void doStart();
    void doStop();
    void doStore(QVariantMap& data) const;
    void doRestore(const QVariantMap& data);

private:
    QString mName;
    QVariant mUid;
    bool mVirtualTag;
    bool mVirtualActive;

    bool mScanning;
    static NfcEngine mNfcEngine;
};

#endif // NFCCONDITION_H

