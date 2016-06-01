#ifndef WLANACTION_H
#define WLANACTION_H

#include <action.h>

#ifdef Q_OS_SYMBIAN
class CWlanState;
#endif

/**
  * WlanAction
  */
class WlanAction
    : public Action
{
private:
    Q_OBJECT

public:
    explicit WlanAction(QObject* parent = 0);
    ~WlanAction();

public: // From Action
    const QVariant currentValue() const;
    void doExecute(const QVariant& value);
    void doRestore();
    void doCancelRestore();

private: // From QObject
    void timerEvent(QTimerEvent* event);

private:
#ifdef Q_OS_SYMBIAN
    CWlanState* mWlanState;
#endif

    static int mRestoreTimerId;
    static QVariant mRestoreValue;
};

#endif // WLANACTION_H

