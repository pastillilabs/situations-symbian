#ifndef THEMEACTION_H
#define THEMEACTION_H

#include <action.h>

/**
  * ThemeAction
  */
class ThemeAction
    : public Action
{
private:
    Q_OBJECT

public:
    explicit ThemeAction(QObject* parent = 0);
    ~ThemeAction();

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

#endif // THEMEACTION_H

