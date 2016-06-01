#ifndef BRIGHTNESSACTION_H
#define BRIGHTNESSACTION_H

#include <action.h>


/**
  * BrightnessAction
  */
class BrightnessAction
    : public Action
{
private:
    Q_OBJECT

public:
    explicit BrightnessAction(QObject* parent = 0);
    ~BrightnessAction();

    Q_INVOKABLE int minBrightness() const;
    Q_INVOKABLE int maxBrightness() const;

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

#endif // BRIGHTNESSACTION_H

