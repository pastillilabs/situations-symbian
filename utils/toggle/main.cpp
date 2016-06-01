#include <QtCore/QCoreApplication>

//#define DEBUG_TO_FILE

#if defined DEBUG_TO_FILE
#include <QTime>
#include <QDir>
#include <QFile>
#include <QTextStream>

void trace(const QString msg)
{
    const QString time(QTime::currentTime().toString("hh:mm.ss"));

    QString txt(QString("[D]: %1 %2").arg(time).arg(msg));

    QDir logPath(QDir::homePath());
    logPath.cd("situations");
    QFile outFile(logPath.filePath("togglelog.txt"));
    outFile.open(QIODevice::WriteOnly|QIODevice::Append);
    QTextStream ts(&outFile);
    ts << txt << endl;
}
#endif

#ifdef Q_OS_SYMBIAN
#include <QDir>
#include <QPluginLoader>
#include <QLibraryInfo>

#include <cellulardatastate.h>
#include <etelmm.h>
#include <e32property.h>
#include <rmmcustomapi.h>

static const QString MOBILEDATA("FBDC70D3571343f2A4B25026CC7943E0");
static const QString MOBILENETWORK("A847C5F0843D4e9eBC3A1FC433FE178B");

int mobileData(const QString p1, const QString p2);
int mobileNetwork(const QString p1, const QString p2);
#endif

static const QString GET("get");
static const QString SET("set");
static const QString ON("on");
static const QString OFF("off");

/**
  * main
  */
Q_DECL_EXPORT int main(int argc, char* argv[])
{
    QCoreApplication a(argc, argv);
    Q_UNUSED(a)

    int retVal(0);
    const QString command(argc > 1 ? argv[1] : "");
    const QString p1(argc > 2 ? argv[2] : "");
    const QString p2(argc > 3 ? argv[3] : "");

#ifdef Q_OS_SYMBIAN
    if(command == MOBILEDATA)
    {
        retVal = mobileData(p1, p2);
    }
    else if(command == MOBILENETWORK)
    {
        retVal = mobileNetwork(p1, p2);
    }
    else
    {
        QDir pluginsDir(QLibraryInfo::location(QLibraryInfo::PluginsPath));
        QPluginLoader loader(pluginsDir.absoluteFilePath(command));
        QObject* plugin(loader.instance());
        if(plugin)
        {
            QMetaObject::invokeMethod(plugin, "handle", Q_RETURN_ARG(int, retVal), Q_ARG(const QString, p1), Q_ARG(const QString, p2));
        }
    }
#endif

#ifdef Q_WS_SIMULATOR
    Q_UNUSED(command)
    Q_UNUSED(p1)
    Q_UNUSED(p2)
#endif

    return retVal;
}

#ifdef Q_OS_SYMBIAN
int mobileData(const QString p1, const QString p2)
{
    int value(0);

    CCellularDataState* cellularDataState(0);
    TRAPD(err, cellularDataState = CCellularDataState::NewL(NULL, CCellularDataState::EDialogModeDefault));
    if(err == KErrNone)
    {
        const bool state(cellularDataState->GetState() != CCellularDataState::ECellularDataStateOff);

        if(p1 == GET)
        {
            if(state) value = 1;
        }
        if(p1 == SET)
        {
            if(p2 == ON && !state)
            {
                TRAP_IGNORE(cellularDataState->SetStateL(CCellularDataState::ECellularDataStateOn));
            }
            if(p2 == OFF && state)
            {
                TRAP_IGNORE(cellularDataState->SetStateL(CCellularDataState::ECellularDataStateOff));
            }
        }

        delete cellularDataState;
    }

    return value;
}

int mobileNetwork(const QString p1, const QString p2)
{
    int value(0);

    RTelServer telServer;
    if(telServer.Connect() == KErrNone)
    {
        TFileName tsyName;
        telServer.RTelServer::GetTsyName(0, tsyName);
        telServer.LoadPhoneModule(tsyName);

        TInt numPhones;
        telServer.EnumeratePhones(numPhones);

        if(numPhones >= 1)
        {
            RTelServer::TPhoneInfo info;
            telServer.GetPhoneInfo(0, info);

            RMobilePhone mobilePhone;
            RMmCustomAPI customAPI;
            mobilePhone.Open(telServer, info.iName);
            customAPI.Open(mobilePhone);

            TUint32 state(0);
            customAPI.GetCurrentSystemNetworkModes(state);

            if(p1 == GET)
            {
                switch(state)
                {
                case RMmCustomAPI::KCapsNetworkModeDual:
                    value = 0;
                    break;
                case RMmCustomAPI::KCapsNetworkModeUmts:
                    value = 1;
                    break;
                case RMmCustomAPI::KCapsNetworkModeGsm:
                    value = 2;
                    break;
                default:
                    break;
                }
            }
            if(p1 == SET)
            {
                switch(p2.toInt())
                {
                case 0:
                    customAPI.SetSystemNetworkMode(RMmCustomAPI::KCapsNetworkModeDual);
                    break;
                case 1:
                    customAPI.SetSystemNetworkMode(RMmCustomAPI::KCapsNetworkModeUmts);
                    break;
                case 2:
                    customAPI.SetSystemNetworkMode(RMmCustomAPI::KCapsNetworkModeGsm);
                    break;
                default:
                    break;
                }
            }

            mobilePhone.Close();
        }

        telServer.Close();
    }

    return value;
}

#endif

