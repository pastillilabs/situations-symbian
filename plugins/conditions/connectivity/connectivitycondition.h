#ifndef CONNECTIVITYCONDITION_H
#define CONNECTIVITYCONDITION_H

#include <condition.h>
#include "connectivityengine.h"

/**
  * ConnectivityCondition
  */
class ConnectivityCondition
    : public Condition
{
private:
    Q_OBJECT
    Q_ENUMS(Type)
    Q_PROPERTY(int type READ type WRITE setType NOTIFY typeChanged)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)

public:
    enum Type {
        TypeWlan = 0,
        TypeBt
    };

public:
    ConnectivityCondition(QObject* parent = 0);
    ~ConnectivityCondition();

    int type() const;
    void setType(const int type);
    const QString& name() const;
    void setName(const QString& name);

    Q_INVOKABLE QString nameById(const int id) const;

signals:
    void typeChanged(int type);
    void nameChanged(QString name);

private slots:
    void connectionsChanged(const QStringList& connections);

private:
    void resolveActive();

private: // From Condition
    void doStart();
    void doStop();
    void doStore(QVariantMap& data) const;
    void doRestore(const QVariantMap& data);

private:
    int mType;
    QString mName;

    static ConnectivityEngine mEngine;
};

#endif // CONNECTIVITYCONDITION_H

