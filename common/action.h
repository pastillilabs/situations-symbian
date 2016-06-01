#ifndef ACTION_H
#define ACTION_H

#include "common_global.h"
#include <QObject>
#include <QVariantMap>

/**
  * Action
  */
class COMMONSHARED_EXPORT Action
    : public QObject
{
private:
    Q_OBJECT
    Q_DISABLE_COPY(Action)

    Q_PROPERTY(QVariant value READ value WRITE setValue NOTIFY valueChanged)
    Q_PROPERTY(QVariantMap data READ data WRITE setData NOTIFY dataChanged)
    Q_PROPERTY(int startDelay READ startDelay WRITE setStartDelay NOTIFY startDelayChanged)
    Q_PROPERTY(bool restore READ restore WRITE setRestore NOTIFY restoreChanged)

public:
    explicit Action(QObject* parent = 0);
    virtual ~Action();

    void initialize();
    void start(bool running);
    void stop(bool running);
    bool running() const;

    const QVariant value() const;
    void setValue(const QVariant value);

    const QVariantMap data() const;
    void setData(const QVariantMap data);

    int startDelay() const;
    void setStartDelay(const int startDelay);

    bool restore() const;
    void setRestore(const bool restore);

signals:
    void valueChanged(const QVariant& value);
    void dataChanged(const QVariantMap& data);
    void startDelayChanged(const int startDelay);
    void restoreChanged(const bool restore);

protected:
    virtual const QVariant currentValue() const;
    virtual void doExecute(const QVariant& value);
    virtual void doStart(bool running);
    virtual void doStop(bool running);
    virtual void doRestore();
    virtual void doCancelRestore();

    virtual void doGetData(QVariantMap& data) const;
    virtual void doSetData(const QVariantMap& data);

protected: // From QObject
    void timerEvent(QTimerEvent* event);

private:
    bool mRunning;

    int mStartTimerId;
    int mStartDelay;
    bool mRestore;

    QVariant mValue;
};

#endif // ACTION_H

