#include "client.h"
#include "version.h"
#include <messageids.h>
#include <identifiers.h>
#include <situation.h>
#include <conditiongroup.h>
#include <QProcess>
#include <QTimerEvent>
#include <QCoreApplication>

static const int CONNECTIONRETRYTIMEOUT(500);
static const int MAXCONNECTIONTRYCOUNT(10);

Client::Client(QObject* parent)
    : QObject(parent)
    , mConnectionAttempts(0)
    , mOut(&mOutBuffer, QIODevice::WriteOnly)
    , mReqAddSituation(false)
    , mMessageSize(0)
    , mNotifications(true)
{
    mOut.setVersion(QDataStream::Qt_4_7);

    connect(&mSocket, SIGNAL(connected()), this, SIGNAL(connected()));
    connect(&mSocket, SIGNAL(readyRead()), this, SLOT(readyRead()));
    connect(&mSocket, SIGNAL(error(QLocalSocket::LocalSocketError)), this, SLOT(error(QLocalSocket::LocalSocketError)));

    connect(&mSituationManager, SIGNAL(addSituation(Situation&)), this, SLOT(onAddSituation(Situation&)));
    connect(&mSituationManager, SIGNAL(rmvSituation(Situation&)), this, SLOT(onRmvSituation(Situation&)));
}

Client::~Client()
{
    mSocket.abort();
}

PluginManager& Client::pluginManager()
{
    return mPluginManager;
}

SituationManager& Client::situationManager()
{
    return mSituationManager;
}

bool Client::notifications() const
{
    return mNotifications;
}

void Client::connectToServer()
{
    ++mConnectionAttempts;
    mSocket.connectToServer(Identifiers::SERVERNAME);
}

void Client::reqPlugins()
{
    mOut << (int)ReqPlugins;
    send();
}

void Client::reqSituations()
{
    mOut << (int)ReqSituations;
    send();
}

void Client::reqSetEnabled(bool enabled)
{
    mOut << (int)ReqSetEnabled;
    mOut << enabled;
    send();
}

void Client::reqSetCurrent(const int situation)
{
    mOut << (int)ReqSetCurrent;
    mOut << situation;
    send();
}

void Client::reqUpdateTranslation()
{
    mOut << (int)ReqUpdateTranslation;
    send();
}

void Client::reqAddSituation(const QString& locId, const QString& name)
{
    mReqAddSituation = true;

    mOut << (int)ReqAddSituation;
    mOut << locId;
    mOut << name;
    send();
}

void Client::reqRmvSituation(const int situation)
{
    mOut << (int)ReqRmvSituation;
    mOut << situation;
    send();
}

void Client::reqMoveSituation(const int situation, const int target)
{
    mOut << (int)ReqMoveSituation;
    mOut << situation;
    mOut << target;
    send();
}

void Client::reqSetSituationName(const int situation, const QString& name)
{
    mOut << (int)ReqSetSituationName;
    mOut << situation;
    mOut << name;
    send();
}

void Client::reqSetSituationEnabled(const int situation, bool enabled)
{
    mOut << (int)ReqSetSituationEnabled;
    mOut << situation;
    mOut << enabled;
    send();
}

void Client::reqAddCondition(const int situation, const int group, const QVariantMap& data)
{
    mOut << (int)ReqAddCondition;
    mOut << situation;
    mOut << group;
    mOut << data;
    send();
}

void Client::reqRmvCondition(const int situation, const int group, const int condition)
{
    mOut << (int)ReqRmvCondition;
    mOut << situation;
    mOut << group;
    mOut << condition;
    send();
}

void Client::reqSetConditionData(const int situation, const int group, const int condition, const QVariantMap& data)
{
    mOut << (int)ReqSetConditionData;
    mOut << situation;
    mOut << group;
    mOut << condition;
    mOut << data;
    send();
}

void Client::reqAddAction(const int situation, const int group, const QVariantMap& data)
{
    mOut << (int)ReqAddAction;
    mOut << situation;
    mOut << group;
    mOut << data;
    send();
}

void Client::reqRmvAction(const int situation, const int group, const int action)
{
    mOut << (int)ReqRmvAction;
    mOut << situation;
    mOut << group;
    mOut << action;
    send();
}

void Client::reqSetActionData(const int situation, const int group, const int action, const QVariantMap& data)
{
    mOut << (int)ReqSetActionData;
    mOut << situation;
    mOut << group;
    mOut << action;
    mOut << data;
    send();
}

void Client::reqSetConditionGroupNegative(const int situation, const int group, const bool negative)
{
    mOut << (int)ReqSetConditionGroupNegative;
    mOut << situation;
    mOut << group;
    mOut << negative;
    send();
}

void Client::reqSetNotifications(bool enabled)
{
    mOut << (int)ReqSetNotifications;
    mOut << enabled;
    send();
}

void Client::send()
{
    QByteArray buffer;
    QDataStream sizeStream(&buffer, QIODevice::WriteOnly);
    sizeStream << mOutBuffer.size();
    buffer += mOutBuffer;

    // Send message to server
    mSocket.write(buffer);
    mSocket.flush();

    // Reset data stream & buffer
    mOut.device()->seek(0);
    mOutBuffer.clear();
}

void Client::receive(QDataStream& in)
{
    // Read message id
    int messageId(0);
    in >> messageId;

    // Handle message
    switch(messageId)
    {
    case ResPlugins:
        resPlugins(in);
        break;
    case ResSituations:
        resSituations(in);
        break;
    case IndAddPlugin:
        indAddPlugin(in);
        break;
    case IndRmvPlugin:
        indRmvPlugin(in);
        break;
    case IndSetEnabled:
        indSetEnabled(in);
        break;
    case IndSetProposal:
        indSetProposal(in);
        break;
    case IndSetCurrent:
        indSetCurrent(in);
        break;
    case IndSetNotifications:
        indSetNotifications(in);
        break;
    case IndAddSituation:
        indAddSituation(in);
        break;
    case IndRmvSituation:
        indRmvSituation(in);
        break;
    case IndMoveSituation:
        indMoveSituation(in);
        break;
    case IndSetSituationName:
        indSetSituationName(in);
        break;
    case IndSetSituationEnabled:
        indSetSituationEnabled(in);
        break;
    case IndSetSituationActive:
        indSetSituationActive(in);
        break;
    case IndAddCondition:
        indAddCondition(in);
        break;
    case IndRmvCondition:
        indRmvCondition(in);
        break;
    case IndSetConditionData:
        indSetConditionData(in);
        break;
    case IndAddAction:
        indAddAction(in);
        break;
    case IndRmvAction:
        indRmvAction(in);
        break;
    case IndSetActionData:
        indSetActionData(in);
        break;
    case IndSetConditionGroupNegative:
        indSetConditionGroupNegative(in);
        break;
    default:
        Q_ASSERT(false);
    }

    // Check if there's more
    if(!in.atEnd()) readyRead();
}

void Client::resPlugins(QDataStream& in)
{
    QVariantMap data;
    in >> data;
    mPluginManager << data;

    emit(pluginsUpdated());
}

void Client::resSituations(QDataStream& in)
{
    QVariantMap data;
    in >> data;
    mSituationManager << data;
    mSituationManager.setEnabled(data[Identifiers::keyEnabled].toBool());
    mSituationManager.setCurrent(data[Identifiers::keyCurrent].toInt());
    mSituationManager.setProposal(data[Identifiers::keyProposal].toInt());
    mNotifications = data[Identifiers::keyNotifications].toBool();

    QList<bool> activeArray;
    in >> activeArray;
    for(int i(0); i < mSituationManager.count(); ++i)
    {
        Situation* s(mSituationManager.situation(i));
        s->setActive(activeArray.at(i));
    }

    emit(situationsUpdated());
}

void Client::indAddPlugin(QDataStream& in)
{
    bool condition(false);
    QString name;

    // Read response
    in >> condition;
    in >> name;

    if(condition)
    {
        mPluginManager.addCondition(name);
    }
    else
    {
        mPluginManager.addAction(name);
    }
}

void Client::indRmvPlugin(QDataStream& in)
{
    bool condition(false);
    QString name;

    // Read response
    in >> condition;
    in >> name;

    if(condition)
    {
        mPluginManager.rmvCondition(name);
    }
    else
    {
        mPluginManager.rmvAction(name);
    }
}

void Client::indSetEnabled(QDataStream& in)
{
    bool enabled(false);

    // Read response
    in >> enabled;

    mSituationManager.setEnabled(enabled);
}

void Client::indSetProposal(QDataStream& in)
{
    int situation(0);

    // Read response
    in >> situation;

    mSituationManager.setProposal(situation);
}

void Client::indSetCurrent(QDataStream& in)
{
    int situation(0);

    // Read response
    in >> situation;

    mSituationManager.setCurrent(situation);
}

void Client::indSetNotifications(QDataStream& in)
{
    bool enabled(false);

    // Read response
    in >> enabled;

    if(enabled != mNotifications)
    {
        mNotifications = enabled;
        emit notificationsChanged(enabled);
    }
}

void Client::indAddSituation(QDataStream &in)
{
    QVariantMap data;
    in >> data;
    Situation* s(mSituationManager.createSituation("", ""));
    (*s) << data;

    if(mReqAddSituation)
    {
        mReqAddSituation = false;
        emit(addSituation());
    }
}

void Client::indRmvSituation(QDataStream &in)
{
    int situation(0);

    // Read response
    in >> situation;

    mSituationManager.destroySituation(situation);
}

void Client::indMoveSituation(QDataStream &in)
{
    int situation(0);
    int target(0);

    // Read response
    in >> situation;
    in >> target;

    mSituationManager.moveSituation(situation, target);
}

void Client::indSetSituationName(QDataStream &in)
{
    int situation(0);
    QString name;

    // Read response
    in >> situation;
    in >> name;

    Situation* s(mSituationManager.situation(situation));
    if(s) s->setName(name);
}

void Client::indSetSituationEnabled(QDataStream &in)
{
    int situation(0);
    bool enabled(false);

    // Read response
    in >> situation;
    in >> enabled;

    Situation* s(mSituationManager.situation(situation));
    if(s) s->setEnabled(enabled);
}

void Client::indSetSituationActive(QDataStream &in)
{
    int situation(0);
    bool active(false);

    // Read response
    in >> situation;
    in >> active;

    Situation* s(mSituationManager.situation(situation));
    if(s) s->setActive(active);
}

void Client::indAddCondition(QDataStream &in)
{
    int situation(0);
    int group(0);
    QVariantMap data;

    // Read response
    in >> situation;
    in >> group;
    in >> data;

    Situation* s(mSituationManager.situation(situation));
    if(s) s->addCondition(group, data);
}

void Client::indRmvCondition(QDataStream& in)
{
    int situation(0);
    int group(0);
    int condition(0);

    // Read response
    in >> situation;
    in >> group;
    in >> condition;

    Situation* s(mSituationManager.situation(situation));
    if(s) s->rmvCondition(group, condition);
}

void Client::indSetConditionData(QDataStream& in)
{
    int situation(0);
    int group(0);
    int condition(0);
    QVariantMap data;

    // Read response
    in >> situation;
    in >> group;
    in >> condition;
    in >> data;

    Situation* s(mSituationManager.situation(situation));
    if(s) s->setConditionData(group, condition, data);
}

void Client::indAddAction(QDataStream &in)
{
    int situation(0);
    int group(0);
    QVariantMap data;

    // Read response
    in >> situation;
    in >> group;
    in >> data;

    Situation* s(mSituationManager.situation(situation));
    if(s) s->addAction(group, data);
}

void Client::indRmvAction(QDataStream& in)
{
    int situation(0);
    int group(0);
    int action(0);

    // Read response
    in >> situation;
    in >> group;
    in >> action;

    Situation* s(mSituationManager.situation(situation));
    if(s) s->rmvAction(group, action);
}

void Client::indSetActionData(QDataStream& in)
{
    int situation(0);
    int group(0);
    int action(0);
    QVariantMap data;

    // Read response
    in >> situation;
    in >> group;
    in >> action;
    in >> data;

    Situation* s(mSituationManager.situation(situation));
    if(s) s->setActionData(group, action, data);
}

void Client::indSetConditionGroupNegative(QDataStream& in)
{
    int situation(0);
    int group(0);
    bool negative(false);

    // Read response
    in >> situation;
    in >> group;
    in >> negative;

    Situation* s(mSituationManager.situation(situation));
    if(s) s->setConditionGroupNegative(group, negative);
}

void Client::readyRead()
{
    QDataStream in(&mSocket);
    in.setVersion(QDataStream::Qt_4_7);

    if(mMessageSize == 0 && mSocket.bytesAvailable() >= sizeof(mMessageSize)) in >> mMessageSize;
    if(mSocket.bytesAvailable() >= mMessageSize)
    {
        mMessageSize = 0;
        receive(in);
    }
}

void Client::error(QLocalSocket::LocalSocketError /*error*/)
{
    if(mConnectionAttempts > MAXCONNECTIONTRYCOUNT) QCoreApplication::quit();
    if(mConnectionAttempts == 1) QProcess::startDetached(SERVER_EXECUTABLE);

    // Start reconnect timer
    startTimer(CONNECTIONRETRYTIMEOUT);
}

void Client::onAddSituation(Situation& situation)
{
    situation.initialize(mPluginManager);
}

void Client::onRmvSituation(Situation& /*situation*/)
{
}

void Client::timerEvent(QTimerEvent* event)
{
    killTimer(event->timerId());
    connectToServer();
}

