#ifndef POWERSAVEACTION_H
#define POWERSAVEACTION_H

#include <action.h>

#ifdef Q_OS_SYMBIAN
class PsmClientObserver;
class CPsmClient;
#endif

/**
  * PowersaveAction
  */
class PowersaveAction
    : public Action
{
private:
    Q_OBJECT

public:
    explicit PowersaveAction(QObject* parent = 0);
    ~PowersaveAction();

private: // From Action
    const QVariant currentValue() const;
    void doExecute(const QVariant& value);
    void doRestore();
    void doCancelRestore();

private: // From QObject
    void timerEvent(QTimerEvent* event);

private:
#ifdef Q_OS_SYMBIAN
    PsmClientObserver* mPsmClientObserver;
    CPsmClient* mPsmClient;
#endif

    static int mRestoreTimerId;
    static QVariant mRestoreValue;
};

#endif // POWERSAVEACTION_H

