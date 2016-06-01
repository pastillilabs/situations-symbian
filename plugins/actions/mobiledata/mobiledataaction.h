#ifndef MOBILEDATAACTION_H
#define MOBILEDATAACTION_H

#include <action.h>

/**
  * MobileDataAction
  */
class MobileDataAction
    : public Action
{
private:
    Q_OBJECT

public:
    explicit MobileDataAction(QObject* parent = 0);
    ~MobileDataAction();

private: // From Action
    const QVariant currentValue() const;
    void doExecute(const QVariant& value);
    void doRestore();
    void doCancelRestore();

private: // From QObject
    void timerEvent(QTimerEvent* event);

private:
    static int mRestoreTimerId;
    static QVariant mRestoreValue;
};

#endif // MOBILEDATAACTION_H

