#ifndef MOBILENETWORKACTION_H
#define MOBILENETWORKACTION_H

#include <action.h>

/**
  * MobileNetworkAction
  */
class MobileNetworkAction
    : public Action
{
private:
    Q_OBJECT

public:
    explicit MobileNetworkAction(QObject* parent = 0);
    ~MobileNetworkAction();

    Q_INVOKABLE QString nameById(const int id) const;

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

#endif // MOBILENETWORKACTION_H

