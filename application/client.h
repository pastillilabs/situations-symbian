#ifndef CLIENT_H
#define CLIENT_H

#include <situationmanager.h>
#include <QLocalSocket>

/**
  * Client
  */
class Client
    : public QObject
{
private:
    Q_OBJECT
    Q_PROPERTY(bool notifications READ notifications NOTIFY notificationsChanged)

public:
    explicit Client(QObject* parent = 0);
    ~Client();

    PluginManager& pluginManager();
    SituationManager& situationManager();

    bool notifications() const;

    Q_INVOKABLE void connectToServer();

    Q_INVOKABLE void reqPlugins();
    Q_INVOKABLE void reqSituations();
    Q_INVOKABLE void reqSetEnabled(bool enabled);
    Q_INVOKABLE void reqSetCurrent(const int situation);
    Q_INVOKABLE void reqSetNotifications(bool enabled);
    Q_INVOKABLE void reqAddSituation(const QString& locId, const QString& name);
    Q_INVOKABLE void reqRmvSituation(const int situation);
    Q_INVOKABLE void reqMoveSituation(const int situation, const int target);
    Q_INVOKABLE void reqSetSituationName(const int situation, const QString& name);
    Q_INVOKABLE void reqSetSituationEnabled(const int situation, bool enabled);
    Q_INVOKABLE void reqAddCondition(const int situation, const int group, const QVariantMap& data);
    Q_INVOKABLE void reqRmvCondition(const int situation, const int group, const int condition);
    Q_INVOKABLE void reqSetConditionData(const int situation, const int group, const int condition, const QVariantMap& data);
    Q_INVOKABLE void reqAddAction(const int situation, const int group, const QVariantMap& data);
    Q_INVOKABLE void reqRmvAction(const int situation, const int group, const int action);
    Q_INVOKABLE void reqSetActionData(const int situation, const int group, const int action, const QVariantMap& data);
    Q_INVOKABLE void reqSetConditionGroupNegative(const int situation, const int group, const bool negative);
    Q_INVOKABLE void reqUpdateTranslation();

signals:
    void connected();
    void pluginsUpdated();
    void situationsUpdated();
    void addSituation();

    void notificationsChanged(bool enabled);

private:
    void send();
    void receive(QDataStream& in);

    void resPlugins(QDataStream& in);
    void resSituations(QDataStream& in);
    void indAddPlugin(QDataStream& in);
    void indRmvPlugin(QDataStream& in);
    void indSetEnabled(QDataStream& in);
    void indSetProposal(QDataStream& in);
    void indSetCurrent(QDataStream& in);
    void indSetNotifications(QDataStream& in);
    void indAddSituation(QDataStream& in);
    void indRmvSituation(QDataStream& in);
    void indMoveSituation(QDataStream& in);
    void indSetSituationName(QDataStream& in);
    void indSetSituationEnabled(QDataStream& in);
    void indSetSituationActive(QDataStream& in);
    void indAddCondition(QDataStream& in);
    void indRmvCondition(QDataStream& in);
    void indSetConditionData(QDataStream& in);
    void indAddAction(QDataStream& in);
    void indRmvAction(QDataStream& in);
    void indSetActionData(QDataStream& in);
    void indSetConditionGroupNegative(QDataStream& in);

private slots:
    void readyRead();
    void error(QLocalSocket::LocalSocketError error);

    void onAddSituation(Situation& situation);
    void onRmvSituation(Situation& situation);

private: // From QObject
    void timerEvent(QTimerEvent* event);

private:
    QLocalSocket mSocket;
    int mConnectionAttempts;

    QByteArray mOutBuffer;
    QDataStream mOut;

    PluginManager mPluginManager;
    SituationManager mSituationManager;

    bool mReqAddSituation;
    int mMessageSize;

    bool mNotifications;
};

#endif // CLIENT_H

