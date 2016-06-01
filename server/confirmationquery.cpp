#include "confirmationquery.h"

static const int CONFIRMATIONQUERYTIMEOUT(30 * 1000);

ConfirmationQuery::ConfirmationQuery(QObject* parent)
    : QObject(parent)
    , mTimerId(0)
#ifdef Q_OS_SYMBIAN
    , mQuery(new GlobalQuerySymbian(*this))
#endif
{
}

ConfirmationQuery::~ConfirmationQuery()
{
    hide();
#ifdef Q_OS_SYMBIAN
    delete mQuery;
#endif
}

void ConfirmationQuery::show(const QString& situation)
{
#ifdef Q_OS_SYMBIAN
    TRAP_IGNORE(mQuery->ShowL(qtTrId("situation_activation_query").arg(situation)));
#endif

    if(mTimerId) killTimer(mTimerId);
    mTimerId = startTimer(CONFIRMATIONQUERYTIMEOUT);
}

void ConfirmationQuery::hide()
{
    if(mTimerId) killTimer(mTimerId);
    mTimerId = 0;
#ifdef Q_OS_SYMBIAN
    mQuery->Cancel();
#endif
}

void ConfirmationQuery::timerEvent(QTimerEvent* /*timerEvent*/)
{
    hide();
}

#ifdef Q_OS_SYMBIAN
void ConfirmationQuery::onGlobalQueryDone(const int result)
{
    killTimer(mTimerId);
    mTimerId = 0;
    emit queryDone(result);
}
#endif

