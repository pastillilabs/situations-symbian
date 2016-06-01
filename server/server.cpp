#include "server.h"
#include <situation.h>
#include <conditiongroup.h>
#include <condition.h>
#include <action.h>
#include <actiongroup.h>
#include <messageids.h>
#include <identifiers.h>
#include <QLocalSocket>
#include <QStringList>
#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QProcess> // FOR SERVER DEBUGGING

#ifdef Q_OS_SYMBIAN
#include <akndiscreetpopup.h>
#include <aknglobalnote.h>
#include <avkon.hrh>
#include <featdiscovery.h>
#include <publicruntimeids.hrh>
#endif

static const int STARTTIMEOUT(500); // 0.5 sec
static const int SHUTDOWNTIMEOUT(10*1000); // 10 sec

static const int SERVER_VERSION(3);

static const char* SERVER_FILE = "server";
static const char* SITUATIONS_FILE = "situations";
static const char* PLUGINS_FILE = "plugins";

Server::Server(int& argc, char** argv)
    : QCoreApplication(argc, argv)
    , mNotifications(true)
    , mManualMode(false)
    , mWaitForCallEnd(false)
    , mStartTimerId(0)
    , mStartCounter(3)
    , mShutdownTimerId(0)
    , mOut(&mOutBuffer, QIODevice::WriteOnly)
{
#ifdef Q_OS_SYMBIAN
    TRAP_IGNORE(CAknDiscreetPopup::InitL());
#endif

    mOut.setVersion(QDataStream::Qt_4_7);
    mStorageManager.addStorage(SERVER_FILE);
    mStorageManager.addStorage(SITUATIONS_FILE);
    mStorageManager.addStorage(PLUGINS_FILE);

    // Connect signals
    connect(&mServer, SIGNAL(newConnection()), this, SLOT(newConnection()));

    connect(&mSituationManager, SIGNAL(enabledChanged(bool)), this, SLOT(enabledChanged(bool)));
    connect(&mSituationManager, SIGNAL(proposalChanged(int)), this, SLOT(proposalChanged(int)));
    connect(&mSituationManager, SIGNAL(currentChanged(int)), this, SLOT(currentChanged(int)));
    connect(&mSituationManager, SIGNAL(activeChanged(int,bool)), this, SLOT(activeChanged(int,bool)));
    connect(&mSituationManager, SIGNAL(addSituation(Situation&)), this, SLOT(addSituation(Situation&)));
    connect(&mSituationManager, SIGNAL(rmvSituation(Situation&)), this, SLOT(rmvSituation(Situation&)));

    connect(&mCallMonitor, SIGNAL(activeChanged(bool)), this, SLOT(callActiveChanged(bool)));
    connect(&mConfirmationQuery, SIGNAL(queryDone(int)), this, SLOT(queryDone(int)));
}

Server::~Server()
{
#ifdef Q_OS_SYMBIAN
    TRAP_IGNORE(CAknDiscreetPopup::Release());
#endif
}

int Server::exec()
{
    int result(1);

#if defined Q_OS_SYMBIAN
    // If connection succeeds, do not start another server
    QLocalSocket socket;
    socket.connectToServer(Identifiers::SERVERNAME);
    if(!socket.waitForConnected(STARTTIMEOUT))
    {
#ifdef Q_OS_SYMBIAN
        // Move private data to public data dir
        QDir dir;
        if(dir.rename("situations.dat", QDir::home().filePath("situations.dat")))
        {
            QProcess importer;
            importer.start("situationsdataimporter.exe");
            importer.waitForFinished();
        }
#endif

        mTranslationManager.addComponent("common");
        mTranslationManager.addComponent("server");
        restore();
        start();
        result = QCoreApplication::exec();
    }
#else
    mTranslationManager.addComponent("common");
    mTranslationManager.addComponent("server");
    restore();
    start();
    result = QCoreApplication::exec();
#endif

    return result;
}

void Server::start()
{
    while(mStartCounter && !mServer.listen(Identifiers::SERVERNAME))
    {
        --mStartCounter;
        QLocalServer::removeServer(Identifiers::SERVERNAME);
    }

    mSituationManager.start();
    if(!mSituationManager.enabled()) mShutdownTimerId = startTimer(SHUTDOWNTIMEOUT);

#if 0 && defined Q_WS_SIMULATOR // FOR SERVER DEBUGGING: START CLIENT FROM SERVER
    QProcess::startDetached("situationsapplication.exe");
#endif
}

void Server::reqVersion(QLocalSocket& client)
{
    // Write response
    mOut << ResVersion;
    mOut << SERVER_VERSION;

    sendClient(client);
}

void Server::reqPlugins(QLocalSocket& client)
{
    QVariantMap data;
    mPluginManager >> data;

    // Write response
    mOut << ResPlugins;
    mOut << data;

    sendClient(client);
}

void Server::reqSituations(QLocalSocket& client)
{
    QVariantMap data;
    mSituationManager >> data;
    data[Identifiers::keyEnabled] = mSituationManager.enabled();
    data[Identifiers::keyCurrent] = mSituationManager.current();
    data[Identifiers::keyProposal] = mSituationManager.proposal();
    data[Identifiers::keyNotifications] = mNotifications;

    // Write response
    mOut << ResSituations;
    mOut << data;

    QList<bool> activeArray;
    for(int i(0); i < mSituationManager.count(); ++i)
    {
        Situation* s(mSituationManager.situation(i));
        activeArray.append(s->active());
    }
    mOut << activeArray;

    sendClient(client);
    mInitialisedClients.append(&client);
}

void Server::reqAddPlugin(QDataStream &in)
{
    bool condition(false);
    QString name;

    // Read request data
    in >> condition;
    in >> name;

    // Write response
    mOut << IndAddPlugin;
    mOut << condition;
    mOut << name;
    sendAll();

    // Handle request
    if(condition)
    {
        mPluginManager.addCondition(name);
    }
    else
    {
        mPluginManager.addAction(name);
    }

    storePlugins();
    storeSituations();
}

void Server::reqRmvPlugin(QDataStream &in)
{
    bool condition(false);
    QString name;

    // Read request data
    in >> condition;
    in >> name;

    // Write response
    mOut << IndRmvPlugin;
    mOut << condition;
    mOut << name;
    sendAll();

    // Handle request
    if(condition)
    {
        mPluginManager.rmvCondition(name);
    }
    else
    {
        mPluginManager.rmvAction(name);
    }

    storePlugins();
    storeSituations();
}

void Server::reqSetEnabled(QDataStream &in)
{
    bool enabled(false);

    // Read request data
    in >> enabled;

    // Write response
    mOut << IndSetEnabled;
    mOut << enabled;
    sendAll();

    // Handle request
    mSituationManager.setEnabled(enabled);
    storeServer();
}

void Server::reqSetCurrent(QDataStream &in)
{
    int situation(0);

    // Read request data
    in >> situation;

    // Handle request
    mSituationManager.setCurrent(situation);
}

void Server::reqSetNotifications(QDataStream &in)
{
    bool enabled(false);

    // Read request data
    in >> enabled;

    // Write response
    mOut << IndSetNotifications;
    mOut << enabled;
    sendAll();

    // Handle request
    mNotifications = enabled;
    storeServer();
}

void Server::reqAddSituation(QDataStream& in)
{
    QString locId;
    QString name;

    // Read request data
    in >> locId;
    in >> name;

    // Handle request
    Situation* s(mSituationManager.createSituation(locId, name));

    // Add default actions to situation
    foreach(ActionGroup* group, s->actions().list())
    {
        if(group->property(Identifiers::groupInitToDefault).toBool()) group->addAction(QVariantMap());
    }

    storeSituations();

    // Write response
    QVariantMap data;
    (*s) >> data;

    mOut << IndAddSituation;
    mOut << data;
    sendAll();
}

void Server::reqRmvSituation(QDataStream& in)
{
    int situation(0);

    // Read request data
    in >> situation;

    // Write response
    mOut << IndRmvSituation;
    mOut << situation;
    sendAll();

    // Handle request
    if(situation == mSituationManager.current())
    {
        if(mManualMode) mSituationManager.setCurrent(mSituationManager.proposal());
        mManualMode = false;
    }
    mSituationManager.destroySituation(situation);
    storeSituations();
}

void Server::reqMoveSituation(QDataStream &in)
{
    int situation(0);
    int target(0);

    // Read request data
    in >> situation;
    in >> target;

    // Write response
    mOut << IndMoveSituation;
    mOut << situation;
    mOut << target;
    sendAll();

    // Handle request
    mSituationManager.moveSituation(situation, target);
    storeSituations();
}

void Server::reqSetSituationName(QDataStream &in)
{
    int situation(0);
    QString name;

    // Read request data
    in >> situation;
    in >> name;

    // Handle request
    Situation* s(mSituationManager.situation(situation));
    if(s)
    {
        s->setName(name);
        storeSituations();

        // Write response
        mOut << IndSetSituationName;
        mOut << situation;
        mOut << name;
        sendAll();
    }
}

void Server::reqSetSituationEnabled(QDataStream &in)
{
    int situation(0);
    bool enabled(false);

    // Read request data
    in >> situation;
    in >> enabled;

    // Handle request
    Situation* s(mSituationManager.situation(situation));
    if(s)
    {
        s->setEnabled(enabled);
        storeSituations();

        // Write response
        mOut << IndSetSituationEnabled;
        mOut << situation;
        mOut << enabled;
        sendAll();
    }
}

void Server::reqAddCondition(QDataStream& in)
{
    int situation(0);
    int group(0);
    QVariantMap data;

    // Read request data
    in >> situation;
    in >> group;
    in >> data;

    // Handle request
    Situation* s(mSituationManager.situation(situation));
    if(s)
    {
        Condition* condition(s->addCondition(group, data));
        if(condition)
        {
            storeSituations();

            // Write response
            mOut << IndAddCondition;
            mOut << situation;
            mOut << group;
            mOut << condition->data();
            sendAll();
        }
    }
}

void Server::reqRmvCondition(QDataStream& in)
{
    int situation(0);
    int group(0);
    int condition(0);

    // Read request data
    in >> situation;
    in >> group;
    in >> condition;

    // Handle request
    Situation* s(mSituationManager.situation(situation));
    if(s)
    {
        s->rmvCondition(group, condition);
        storeSituations();

        // Write response
        mOut << IndRmvCondition;
        mOut << situation;
        mOut << group;
        mOut << condition;
        sendAll();
    }
}

void Server::reqSetConditionData(QDataStream& in)
{
    int situation(0);
    int group(0);
    int condition(0);
    QVariantMap data;

    // Read request data
    in >> situation;
    in >> group;
    in >> condition;
    in >> data;

    // Handle request
    Situation* s(mSituationManager.situation(situation));
    if(s)
    {
        s->setConditionData(group, condition, data);
        storeSituations();

        // Write response
        mOut << IndSetConditionData;
        mOut << situation;
        mOut << group;
        mOut << condition;
        mOut << data;
        sendAll();
    }
}

void Server::reqAddAction(QDataStream& in)
{
    int situation(0);
    int group(0);
    QVariantMap data;

    // Read request data
    in >> situation;
    in >> group;
    in >> data;

    // Handle request
    Situation* s(mSituationManager.situation(situation));
    if(s)
    {
        Action* action(s->addAction(group, data));
        if(action)
        {
            storeSituations();

            // Write response
            mOut << IndAddAction;
            mOut << situation;
            mOut << group;
            mOut << action->data();
            sendAll();
        }
    }
}

void Server::reqRmvAction(QDataStream& in)
{
    int situation(0);
    int group(0);
    int action(0);

    // Read request data
    in >> situation;
    in >> group;
    in >> action;

    // Handle request
    Situation* s(mSituationManager.situation(situation));
    if(s)
    {
        s->rmvAction(group, action);
        storeSituations();

        // Write response
        mOut << IndRmvAction;
        mOut << situation;
        mOut << group;
        mOut << action;
        sendAll();
    }
}

void Server::reqSetActionData(QDataStream& in)
{
    int situation(0);
    int group(0);
    int action(0);
    QVariantMap data;

    // Read request data
    in >> situation;
    in >> group;
    in >> action;
    in >> data;

    // Handle request
    Situation* s(mSituationManager.situation(situation));
    if(s)
    {
        s->setActionData(group, action, data);
        storeSituations();

        // Write response
        mOut << IndSetActionData;
        mOut << situation;
        mOut << group;
        mOut << action;
        mOut << data;
        sendAll();
    }
}

void Server::reqSetConditionGroupNegative(QDataStream& in)
{
    int situation(0);
    int group(0);
    bool negative(false);

    // Read request data
    in >> situation;
    in >> group;
    in >> negative;

    // Handle request
    Situation* s(mSituationManager.situation(situation));
    if(s)
    {
        s->setConditionGroupNegative(group, negative);
        storeSituations();

        // Write response
        mOut << IndSetConditionGroupNegative;
        mOut << situation;
        mOut << group;
        mOut << negative;
        sendAll();
    }
}

void Server::reqUpdateTranslation(QDataStream& /*in*/)
{
    mTranslationManager.reloadTranslations();
}

void Server::sendClient(QLocalSocket& client)
{
    QByteArray buffer;
    QDataStream sizeStream(&buffer, QIODevice::WriteOnly);
    sizeStream << mOutBuffer.size();
    buffer += mOutBuffer;

    // Send message to client
    client.write(buffer);
    client.flush();

    // Reset data stream & buffer
    mOut.device()->seek(0);
    mOutBuffer.clear();
}

void Server::sendAll()
{
    QByteArray buffer;
    QDataStream sizeStream(&buffer, QIODevice::WriteOnly);
    sizeStream << mOutBuffer.size();
    buffer += mOutBuffer;

    // Send message to all clients
    foreach(QLocalSocket* socket, mClients.keys())
    {
        Q_ASSERT(socket);
        if(mInitialisedClients.indexOf(socket) >= 0)
        {
            socket->write(buffer);
            socket->flush();
        }
    }

    // Reset data stream & buffer
    mOut.device()->seek(0);
    mOutBuffer.clear();
}

void Server::receive(QDataStream &in, QLocalSocket& client)
{
    // Read message id
    int messageId(0);
    in >> messageId;

    // Handle message
    switch(messageId)
    {
    case ReqVersion:
        reqVersion(client);
        break;
    case ReqPlugins:
        reqPlugins(client);
        break;
    case ReqSituations:
        reqSituations(client);
        break;
    case ReqAddPlugin:
        reqAddPlugin(in);
        break;
    case ReqRmvPlugin:
        reqRmvPlugin(in);
        break;
    case ReqSetEnabled:
        reqSetEnabled(in);
        break;
    case ReqSetCurrent:
        reqSetCurrent(in);
        break;
    case ReqSetNotifications:
        reqSetNotifications(in);
        break;
    case ReqAddSituation:
        reqAddSituation(in);
        break;
    case ReqRmvSituation:
        reqRmvSituation(in);
        break;
    case ReqMoveSituation:
        reqMoveSituation(in);
        break;
    case ReqSetSituationName:
        reqSetSituationName(in);
        break;
    case ReqSetSituationEnabled:
        reqSetSituationEnabled(in);
        break;
    case ReqAddCondition:
        reqAddCondition(in);
        break;
    case ReqRmvCondition:
        reqRmvCondition(in);
        break;
    case ReqSetConditionData:
        reqSetConditionData(in);
        break;
    case ReqAddAction:
        reqAddAction(in);
        break;
    case ReqRmvAction:
        reqRmvAction(in);
        break;
    case ReqSetActionData:
        reqSetActionData(in);
        break;
    case ReqSetConditionGroupNegative:
        reqSetConditionGroupNegative(in);
        break;
    case ReqUpdateTranslation:
        reqUpdateTranslation(in);
        break;
    default:
        Q_ASSERT(false);
    }

    // Check if there's more
    if(!in.atEnd()) readyRead(client);
}

void Server::readyRead(QLocalSocket& client)
{
    QDataStream in(&client);
    in.setVersion(QDataStream::Qt_4_7);

    int messageSize(mClients.value(&client));

    if(messageSize == 0 && client.bytesAvailable() >= sizeof(messageSize))
    {
        in >> messageSize;
        mClients[&client] = messageSize;
    }
    if(client.bytesAvailable() >= messageSize)
    {
        mClients[&client] = 0;
        receive(in, client);
    }
}

void Server::setManualMode(bool manualMode)
{
    if(mManualMode != manualMode)
    {
        mManualMode = manualMode;

        if(mSituationManager.running() && mNotifications)
        {
            QString titleString(qtTrId("automatic_mode_activation_notification"));
            if(manualMode) titleString = qtTrId("manual_mode_activation_notification");

#ifdef Q_OS_SYMBIAN
            TPtrC title(reinterpret_cast<const TText*>(titleString.constData()),titleString.length());

            CAknGlobalNote* note(CAknGlobalNote::NewLC());
            note->ShowNoteL(EAknGlobalInformationNote, title);
            CleanupStack::PopAndDestroy(note);
#endif
        }
    }
}

void Server::storeServer()
{
    QVariantMap data;
    data[Identifiers::keyManual] = mManualMode;
    data[Identifiers::keyEnabled] = mSituationManager.enabled();
    data[Identifiers::keyCurrent] = mSituationManager.current();
    data[Identifiers::keyNotifications] = mNotifications;

    mStorageManager.write(SERVER_FILE, data);
}

void Server::storePlugins()
{
    QVariantMap data;
    mPluginManager >> data;

    mStorageManager.write(PLUGINS_FILE, data);
}

void Server::storeSituations()
{
    QVariantMap data;
    mSituationManager >> data;

    mStorageManager.write(SITUATIONS_FILE, data);
}

void Server::restore()
{
    loadDefaultPlugins();

    QVariantMap pluginData;
    if(mStorageManager.read(PLUGINS_FILE, pluginData))
    {
        mPluginManager << pluginData;
    }

    QVariantMap situationData;
    if(mStorageManager.read(SITUATIONS_FILE, situationData))
    {
        mSituationManager << situationData;
    }
    else
    {
        createInitialContent();
    }

    QVariantMap serverData;
    if(mStorageManager.read(SERVER_FILE, serverData))
    {
        const int current(serverData.value(Identifiers::keyCurrent, 0).toInt());
        const bool enabled(serverData.value(Identifiers::keyEnabled, true).toBool());
        const bool manual(serverData.value(Identifiers::keyManual).toBool());
        const bool notifications(serverData.value(Identifiers::keyNotifications, true).toBool());

        setManualMode(manual);
        if(!manual) mSituationManager.setProposal(current);
        mSituationManager.setCurrent(current);
        mSituationManager.setEnabled(enabled);
        mNotifications = notifications;
    }
}

void Server::loadDefaultPlugins()
{
#if defined Q_WS_SYMBIAN_UI
    mPluginManager.addCondition("time");
    mPluginManager.addCondition("calendar");
    mPluginManager.addCondition("accessory");
    mPluginManager.addCondition("battery");
    mPluginManager.addCondition("cell");
#ifdef Q_OS_SYMBIAN
    TBool nfcSupported(ETrue);
    TRAP_IGNORE(nfcSupported = CFeatureDiscovery::IsFeatureSupportedL(KFeatureIdNfc));
    if(nfcSupported) mPluginManager.addCondition("nfc");
#else
    mPluginManager.addCondition("nfc");
#endif
    mPluginManager.addCondition("connectivity");
    mPluginManager.addCondition("location");

    mPluginManager.addAction("profile");
    mPluginManager.addAction("theme");
    mPluginManager.addAction("mobilenetwork");
    mPluginManager.addAction("mobiledata");
    mPluginManager.addAction("wlan");
    mPluginManager.addAction("bluetooth");
    mPluginManager.addAction("brightness");
    mPluginManager.addAction("powersave");
    mPluginManager.addAction("smsreply");
    mPluginManager.addAction("smssend");
    mPluginManager.addAction("launch");
    mPluginManager.addAction("close");
#endif
}

void Server::createInitialContent()
{
    // Situations
    Situation* general(mSituationManager.createSituation("situation_name_general", ""));
    mSituationManager.createSituation("situation_name_user1", "");
    mSituationManager.createSituation("situation_name_user2", "");
    mSituationManager.createSituation("situation_name_user3", "");
    mSituationManager.createSituation("situation_name_user4", "");

    // Add default actions to general situation
    foreach(ActionGroup* group, general->actions().list())
    {
        if(group->property(Identifiers::groupInitToDefault).toBool()) group->addAction(QVariantMap());
    }

    mSituationManager.setEnabled(true);
}

void Server::newConnection()
{
    QLocalSocket* socket(mServer.nextPendingConnection());
    if(socket)
    {
        if(mShutdownTimerId) killTimer(mShutdownTimerId);
        mClients.insert(socket, 0);

        connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
        connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
    }
}

void Server::disconnected()
{
    QLocalSocket* socket(qobject_cast<QLocalSocket*>(sender()));
    if(socket)
    {
        mClients.take(socket);
        socket->deleteLater();

        // Exit if manager is not enabled & there are no more clients
        if(!mSituationManager.enabled() && !mClients.count()) mShutdownTimerId = startTimer(SHUTDOWNTIMEOUT);
    }
}

void Server::readyRead()
{
    QLocalSocket* client(qobject_cast<QLocalSocket*>(sender()));
    if(client) readyRead(*client);
}

void Server::enabledChanged(bool enabled)
{
    if(mSituationManager.running() && !enabled && mNotifications)
    {
        QString titleString(qtTrId("situations_disabled_notification"));

#ifdef Q_OS_SYMBIAN
        TPtrC title(reinterpret_cast<const TText*>(titleString.constData()),titleString.length());

        CAknGlobalNote* note(CAknGlobalNote::NewLC());
        note->ShowNoteL(EAknGlobalInformationNote, title);
        CleanupStack::PopAndDestroy(note);
#endif
    }
}

void Server::proposalChanged(const int situation)
{
    if(mSituationManager.running())
    {
        mOut << IndSetProposal;
        mOut << situation;
        sendAll();

        mWaitForCallEnd = mCallMonitor.active();

        // Handle proposal
        if(mManualMode || mWaitForCallEnd)
        {
            if(situation != mSituationManager.current())
            {
                Situation* s(mSituationManager.situation(situation));
                if(s) mConfirmationQuery.show(s->name());
            }
            else
            {
                setManualMode(false);
            }
        }
        else
        {
            mSituationManager.setCurrent(situation);
        }
    }
}

void Server::currentChanged(const int situation)
{
    if(mSituationManager.running())
    {
        mOut << IndSetCurrent;
        mOut << situation;
        sendAll();

        setManualMode(situation != mSituationManager.proposal());

        mWaitForCallEnd = false;
        mConfirmationQuery.hide();
        storeServer();

        // Notify situation start
        const Situation* s(mSituationManager.situation(situation));
        if(s && mNotifications)
        {
    #ifdef Q_OS_SYMBIAN
            const QString& name(s->name());

            const QString titleString(qtTrId("situation_activation_notification"));
            TPtrC title(reinterpret_cast<const TText*>(titleString.constData()),titleString.length());
            TPtrC text(reinterpret_cast<const TText*>(name.constData()),name.length());

            TRAP_IGNORE(CAknDiscreetPopup::ShowGlobalPopupL(title, text, KAknsIIDNone, KNullDesC, 0, 0, KAknDiscreetPopupDurationLong));
    #endif
        }
    }
}

void Server::activeChanged(const int situation, bool active)
{
    if(mSituationManager.running())
    {
        mOut << IndSetSituationActive;
        mOut << situation;
        mOut << active;
        sendAll();
    }
}

void Server::addSituation(Situation& situation)
{
    situation.initialize(mPluginManager);
}

void Server::rmvSituation(Situation& /*situation*/)
{
}

void Server::callActiveChanged(bool active)
{
    if(active == false && mWaitForCallEnd) proposalChanged(mSituationManager.proposal());
}

void Server::queryDone(const int result)
{
    if(result)
    {
        setManualMode(false);
        mSituationManager.setCurrent(mSituationManager.proposal());
    }
}

void Server::timerEvent(QTimerEvent* timerEvent)
{
    const int timerId(timerEvent->timerId());
    killTimer(timerId);

    if(timerId == mStartTimerId)
    {
        --mStartCounter;
        start();
    }

    if(timerId == mShutdownTimerId)
    {
        QCoreApplication::exit();
    }
}

