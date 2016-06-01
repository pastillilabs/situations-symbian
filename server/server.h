#ifndef SERVER_H
#define SERVER_H

#include "callmonitor.h"
#include "confirmationquery.h"
#include <translationmanager.h>
#include <situationmanager.h>
#include <storagemanager.h>
#include <QCoreApplication>
#include <QLocalServer>

/**
  * Server
  */
class Server
    : public QCoreApplication
{
private:
    Q_OBJECT

public:
    Server(int& argc, char** argv);
    ~Server();

    int exec();

private:
    void start();

    void reqVersion(QLocalSocket& client);
    void reqPlugins(QLocalSocket& client);
    void reqSituations(QLocalSocket& client);
    void reqAddPlugin(QDataStream& in);
    void reqRmvPlugin(QDataStream& in);
    void reqSetEnabled(QDataStream& in);
    void reqSetCurrent(QDataStream& in);
    void reqSetNotifications(QDataStream& in);
    void reqAddSituation(QDataStream& in);
    void reqRmvSituation(QDataStream& in);
    void reqMoveSituation(QDataStream& in);
    void reqSetSituationName(QDataStream& in);
    void reqSetSituationEnabled(QDataStream& in);
    void reqAddCondition(QDataStream& in);
    void reqRmvCondition(QDataStream& in);
    void reqSetConditionData(QDataStream& in);
    void reqAddAction(QDataStream& in);
    void reqRmvAction(QDataStream& in);
    void reqSetActionData(QDataStream& in);
    void reqSetConditionGroupNegative(QDataStream& in);
    void reqUpdateTranslation(QDataStream& in);

    void sendClient(QLocalSocket& client);
    void sendAll();
    void receive(QDataStream& in, QLocalSocket& client);
    void readyRead(QLocalSocket& client);

    void setManualMode(bool manualMode);

    void storeServer();
    void storePlugins();
    void storeSituations();
    void restore();

    void loadDefaultPlugins();
    void createInitialContent();

private slots:
    void newConnection();
    void disconnected();
    void readyRead();

    void enabledChanged(bool enabled);
    void proposalChanged(const int situation);
    void currentChanged(const int situation);
    void activeChanged(const int situation, bool active);

    void addSituation(Situation& situation);
    void rmvSituation(Situation& situation);

    void callActiveChanged(bool active);

    void queryDone(const int result);

private: // From QObject
    void timerEvent(QTimerEvent* timerEvent);

private:
    QLocalServer mServer;
    QMap<QLocalSocket*, int> mClients;
    QList<QLocalSocket*> mInitialisedClients;

    TranslationManager mTranslationManager;
    PluginManager mPluginManager;
    SituationManager mSituationManager;
    StorageManager mStorageManager;

    bool mNotifications;
    bool mManualMode;
    bool mWaitForCallEnd;
    int mStartTimerId;
    int mStartCounter;
    int mShutdownTimerId;

    CallMonitor mCallMonitor;
    ConfirmationQuery mConfirmationQuery;

    QByteArray mOutBuffer;
    QDataStream mOut;
};

#endif // SERVER_H

