#ifndef FLIGHTMODEACTION_H
#define FLIGHTMODEACTION_H

#include <action.h>

/**
  * FlightmodeAction
  */
class FlightmodeAction
    : public Action
{
private:
    Q_OBJECT

public:
    explicit FlightmodeAction(QObject* parent = 0);
    ~FlightmodeAction();

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

#endif // FLIGHTMODEACTION_H

