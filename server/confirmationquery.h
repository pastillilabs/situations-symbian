#ifndef CONFIRMATIONQUERY_H
#define CONFIRMATIONQUERY_H

#include <QObject>

#ifdef Q_OS_SYMBIAN
#include "globalquerysymbian.h"
#endif

/**
  * ConfirmationQuery
  */
class ConfirmationQuery
    : public QObject
#ifdef Q_OS_SYMBIAN
    , private GlobalQuerySymbian::Observer
#endif
{
private:
    Q_OBJECT

public:
    explicit ConfirmationQuery(QObject* parent = 0);
    ~ConfirmationQuery();

    void show(const QString& situation);
    void hide();

signals:
    void queryDone(const int result);

private: // From QObject
    void timerEvent(QTimerEvent* timerEvent);

#ifdef Q_OS_SYMBIAN
private: // GlobalQuerySymbian::Observer
    void onGlobalQueryDone(const int result);
#endif

private:
    int mTimerId;
#ifdef Q_OS_SYMBIAN
    GlobalQuerySymbian* mQuery;
#endif
};

#endif // CONFIRMATIONQUERY_H

