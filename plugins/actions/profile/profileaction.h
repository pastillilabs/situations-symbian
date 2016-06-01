#ifndef PROFILEACTION_H
#define PROFILEACTION_H

#include <action.h>

#ifdef Q_OS_SYMBIAN
class MProEngEngine;
#endif

/**
  * ProfileAction
  */
class ProfileAction
    : public Action
{
private:
    Q_OBJECT

public:
    explicit ProfileAction(QObject* parent = 0);
    ~ProfileAction();

    Q_INVOKABLE QString nameById(const int id) const;

private: // From Action
    const QVariant currentValue() const;
    void doExecute(const QVariant& value);
    void doRestore();
    void doCancelRestore();

private: // From QObject
    void timerEvent(QTimerEvent* event);

private:
#ifdef Q_OS_SYMBIAN
    MProEngEngine* mEngine;
#endif

    static int mRestoreTimerId;
    static QVariant mRestoreValue;
};

#endif // PROFILEACTION_H

