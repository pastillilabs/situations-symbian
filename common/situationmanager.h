#ifndef SITUATIONMANAGER_H
#define SITUATIONMANAGER_H

#include "common_global.h"
#include "pluginmanager.h"
#include <QAbstractListModel>

class Situation;
class ConditionGroupListModel;
class ActionGroupListModel;

/**
  * SituationManager
  */
class COMMONSHARED_EXPORT SituationManager
    : public QAbstractListModel
{
private:
    Q_OBJECT
    Q_DISABLE_COPY(SituationManager)

    Q_PROPERTY(bool enabled READ enabled NOTIFY enabledChanged)
    Q_PROPERTY(int proposal READ proposal NOTIFY proposalChanged)
    Q_PROPERTY(int current READ current NOTIFY currentChanged)

    enum Roles {
        SituationRole = Qt::UserRole + 1,
        SituationConditionsRole,
        SituationActionsRole
    };

public:
    explicit SituationManager(QObject* parent = 0);

    void operator<<(const QVariantMap& in);
    void operator>>(QVariantMap& out) const;

    Situation* createSituation(const QString& locId, const QString& name);
    void destroySituation(const int situation);
    void moveSituation(const int situation, const int target);
    Situation* situation(const int situation) const;

    int count() const;

    bool enabled() const;
    int proposal() const;
    int current() const;

    void setEnabled(bool enabled);
    void setProposal(const int situation);
    void setCurrent(const int situation);

    void start();
    void stop();
    bool running() const;

private:
    void evaluate();
    void startSituations();
    void stopSituations();
    
signals:
    void enabledChanged(bool enabled);
    void proposalChanged(const int situation);
    void currentChanged(const int situation);
    void activeChanged(const int situation, bool active);

    void addSituation(Situation& situation);
    void rmvSituation(Situation& situation);

private slots:
    void activeChanged(bool active);

private: // From QAbstractListModel
    int rowCount(const QModelIndex& parent = QModelIndex()) const;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;

private:
    QList<Situation*> mSituations;

    bool mEnabled;
    int mProposal;
    int mCurrent;
    bool mRunning;
    bool mEditing;
};

#endif // SITUATIONMANAGER_H

