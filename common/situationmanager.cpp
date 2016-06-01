#include "situationmanager.h"
#include "situation.h"
#include "identifiers.h"

SituationManager::SituationManager(QObject *parent)
    : QAbstractListModel(parent)
    , mEnabled(true)
    , mProposal(-1)
    , mCurrent(-1)
    , mRunning(false)
    , mEditing(false)
{
    QHash<int, QByteArray> roles;
    roles[SituationRole] = "situation";
    roles[SituationConditionsRole] = "situationConditions";
    roles[SituationActionsRole] = "situationActions";
    setRoleNames(roles);
}

void SituationManager::operator<<(const QVariantMap& in)
{
    // TODO: RESET SITUATIONS(?)

    const QVariantList situations(in[Identifiers::keySituations].toList());
    foreach(const QVariant& data, situations)
    {
        Situation* s(createSituation("", ""));
        (*s) << data.toMap();
    }
}

void SituationManager::operator>>(QVariantMap& out) const
{
    QVariantList situations;
    foreach(const Situation* situation, mSituations)
    {
        QVariantMap data;
        (*situation) >> data;
        situations.append(data);
    }

    out[Identifiers::keySituations] = situations;
}

Situation* SituationManager::createSituation(const QString& locId, const QString& name)
{
    // Create new Situation
    Situation* s(new Situation(this, locId, name));
    connect(s, SIGNAL(activeChanged(bool)), this, SLOT(activeChanged(bool)));

    beginInsertRows(QModelIndex(), mSituations.count(), mSituations.count());
    mSituations.append(s);
    emit addSituation(*s);
    endInsertRows();

    if(mRunning && mEnabled) s->start();
    return s;
}

void SituationManager::destroySituation(const int situation)
{
    if(situation >= 0 && situation < mSituations.count())
    {
        beginRemoveRows(QModelIndex(), situation, situation);
        Situation* s(mSituations.takeAt(situation));
        emit rmvSituation(*s);
        endRemoveRows();

        if(situation == mCurrent) s->finish();
        delete s;

        // Reset current situation
        if(mRunning && mEnabled && situation == mCurrent) evaluate();
    }
}

void SituationManager::moveSituation(const int situation, const int target)
{
    Situation* s(SituationManager::situation(situation));
    if(s && target < mSituations.count())
    {
        beginMoveRows(QModelIndex(), situation, situation, QModelIndex(), target);
        mSituations.move(situation, target);
        endMoveRows();

        // Re-evaluate current proposal
        if(mRunning && mEnabled)
        {
            // If moving active situation, update current & proposal index
            if(mCurrent == situation) mCurrent = target;
            if(mProposal == situation) mProposal = target;

            evaluate();
        }
    }
}

Situation* SituationManager::situation(const int situation) const
{
    Situation* s(0);
    if(situation >= 0 && situation < mSituations.count()) s = mSituations.at(situation);
    return s;
}

int SituationManager::count() const
{
    return mSituations.count();
}

bool SituationManager::enabled() const
{
    return mEnabled;
}

int SituationManager::proposal() const
{
    return mProposal;
}

int SituationManager::current() const
{
    return mCurrent;
}

void SituationManager::setEnabled(bool enabled)
{
    if(enabled != mEnabled)
    {
        mEnabled = enabled;
        emit enabledChanged(enabled);
    }

    if(mRunning) mEnabled ? startSituations() : stopSituations();
}

void SituationManager::setProposal(const int situation)
{
    if(situation != mProposal)
    {
        mProposal = situation;
        emit(proposalChanged(situation));
    }
}

void SituationManager::setCurrent(const int situation)
{
    if(situation != mCurrent)
    {
        Situation* prev(SituationManager::situation(mCurrent));
        Situation* next(SituationManager::situation(situation));

        mCurrent = situation;
        emit(currentChanged(situation));

        if(mRunning && mEnabled)
        {
            if(prev) prev->finish();
            if(next) next->execute();
        }
    }
}

void SituationManager::start()
{
    if(!mRunning)
    {
        mRunning = true;
        if(mEnabled) startSituations();
    }
}

void SituationManager::stop()
{
    if(mRunning)
    {
        mRunning = false;
        if(mEnabled) stopSituations();
    }
}

bool SituationManager::running() const
{
    return mRunning;
}

void SituationManager::evaluate()
{
    int situation(0);

    // Iterate through situations to find the last active one
    for(int i(0); i < mSituations.count(); ++i)
    {
        const Situation* s(mSituations.at(i));
        if(s->active() && s->enabled()) situation = i;
    }

    // Set the proposed situation
    setProposal(situation);
}

void SituationManager::startSituations()
{
    Q_ASSERT(mRunning && mEnabled);

    // Start situations
    mEditing = true;
    QList<Situation*>::const_iterator s(mSituations.constBegin());
    while(s != mSituations.constEnd())
    {
        (*s)->start();
        ++s;
    }
    mEditing = false;

    // Execute current situation
    const int current(mCurrent);
    mCurrent = -1;
    setCurrent(current);

    // Re-evaluate proposal
    mProposal = -1;
    evaluate();
}

void SituationManager::stopSituations()
{
    // Finish current situation
    Situation* current(situation(mCurrent));
    if(current) current->finish();

    // Reset proposal
    setProposal(-1);

    // Stop situations
    mEditing = true;
    QList<Situation*>::const_iterator s(mSituations.constBegin());
    while(s != mSituations.constEnd())
    {
        (*s)->stop();
        ++s;
    }
    mEditing = false;
}

void SituationManager::activeChanged(bool active)
{
    Situation* s(qobject_cast<Situation*>(sender()));
    emit activeChanged(mSituations.indexOf(s), active);

    if(mRunning && mEnabled && !mEditing) evaluate();
}

int SituationManager::rowCount(const QModelIndex& /*parent*/) const
{
    return mSituations.count();
}

QVariant SituationManager::data(const QModelIndex& index, int role) const
{
    QVariant data;
    if(index.row() >= 0 && index.row() < mSituations.count())
    {
        Situation* situation(mSituations.at(index.row()));
        switch(role)
        {
        case SituationRole:
            data = QVariant::fromValue(qobject_cast<QObject*>(situation));
            break;
        case SituationConditionsRole:
            data = QVariant::fromValue(qobject_cast<QObject*>(&situation->conditions()));
            break;
        case SituationActionsRole:
            data = QVariant::fromValue(qobject_cast<QObject*>(&situation->actions()));
            break;
        default:
            Q_ASSERT(false);
        }
    }

    return data;
}

