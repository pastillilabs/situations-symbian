#ifndef CONDITION_H
#define CONDITION_H

#include "common_global.h"
#include <QObject>
#include <QVariantMap>

/**
  * Condition
  */
class COMMONSHARED_EXPORT Condition
    : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(Condition)
    Q_PROPERTY(bool active READ active WRITE setActive NOTIFY activeChanged)
    Q_PROPERTY(QVariantMap data READ data WRITE setData NOTIFY dataChanged)

public:
    explicit Condition(QObject* parent = 0);
    virtual ~Condition();

    void start();
    void stop();
    bool running() const;

    bool active() const;
    void setActive(bool active);

    const QVariantMap data() const;
    void setData(const QVariantMap& data);

signals:
    void activeChanged(bool active);
    void dataChanged(const QVariantMap& data);

protected:
    virtual void doStart() = 0;
    virtual void doStop() = 0;

    virtual void doStore(QVariantMap& data) const = 0;
    virtual void doRestore(const QVariantMap& data) = 0;

private:
    bool mRunning;
    bool mActive;
};

#endif // CONDITION_H

