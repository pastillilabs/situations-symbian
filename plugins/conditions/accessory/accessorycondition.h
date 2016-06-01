#ifndef ACCESSORYCONDITION_H
#define ACCESSORYCONDITION_H

#include <condition.h>
#include <QSystemDeviceInfo>
#include <QVariantList>

#ifdef Q_OS_SYMBIAN
#include <accmonitor.h>
#endif

QTM_USE_NAMESPACE

/**
  * AccessoryCondition
  */
class AccessoryCondition
    : public Condition
#ifdef Q_OS_SYMBIAN
    , public MAccMonitorObserver
#endif
{
private:
    Q_OBJECT
    Q_ENUMS(Type)
    Q_PROPERTY(int value READ value WRITE setValue NOTIFY valueChanged)

public:
    enum Type {
        TypeAudio = 0,
        TypeVideo,
        TypeCarKit,
        TypeCharger
    };

public:
    AccessoryCondition(QObject* parent = 0);
    ~AccessoryCondition();

    int value() const;
    void setValue(const int value);

    Q_INVOKABLE QString nameById(const int id) const;

signals:
    void valueChanged(int value);

private:
    void resolveActive();

private slots:
    void powerStateChanged(QSystemDeviceInfo::PowerState state);

private: // From Condition
    void doStart();
    void doStop();
    void doStore(QVariantMap& data) const;
    void doRestore(const QVariantMap& data);

#ifdef Q_OS_SYMBIAN
private: // From MAccMonitorObserver
    void ConnectedL(CAccMonitorInfo* aAccessoryInfo);
    void DisconnectedL(CAccMonitorInfo* aAccessoryInfo);
    void AccMonitorObserverError(TInt aError);
#endif

private:
    int mValue;

    QSystemDeviceInfo* mSystemDeviceInfo;

#ifdef Q_OS_SYMBIAN
    CAccMonitor* mAccMonitor;
#endif
};

#endif // ACCESSORYCONDITION_H

