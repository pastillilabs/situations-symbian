#ifndef BATTERYCONDITION_H
#define BATTERYCONDITION_H

#include <condition.h>
#include <QSystemDeviceInfo>

QTM_USE_NAMESPACE

/**
  * BatteryCondition
  */
class BatteryCondition
    : public Condition
{
private:
    Q_OBJECT
    Q_PROPERTY(bool lessThan READ lessThan WRITE setLessThan NOTIFY lessThanChanged)
    Q_PROPERTY(int value READ value WRITE setValue NOTIFY valueChanged)

public:
    BatteryCondition(QObject* parent = 0);
    ~BatteryCondition();

    bool lessThan() const;
    void setLessThan(bool lessThan);
    int value() const;
    void setValue(const int value);

signals:
    void lessThanChanged(bool lessThan);
    void valueChanged(int value);

private:
    void resolveActive();

private slots:
    void batteryLevelChanged(int level);

private: // From Condition
    void doStart();
    void doStop();
    void doStore(QVariantMap& data) const;
    void doRestore(const QVariantMap& data);

private:
    bool mLessThan;
    int mValue;

    QSystemDeviceInfo* mSystemDeviceInfo;
};

#endif // BATTERYCONDITION_H

