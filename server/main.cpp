#if 0 // DEBUG TO FILE
#define DEBUG_TO_FILE
#include <identifiers.h>
#include <QTime>
#include <QDir>
#include <QFile>
#include <QTextStream>

void customMessageHandler(QtMsgType type, const char *msg)
{
    const QString time(QTime::currentTime().toString("hh:mm.ss"));

    QString txt;
    switch (type) {
    case QtDebugMsg:
        txt = QString("[D]: %1 %2").arg(time).arg(msg);
        break;
    case QtWarningMsg:
        txt = QString("[W]: %1 %2").arg(time).arg(msg);
        break;
    case QtCriticalMsg:
        txt = QString("[C]: %1 %2").arg(time).arg(msg);
        break;
    case QtFatalMsg:
        txt = QString("[F]: %1 %2").arg(time).arg(msg);
        abort();
    }

    QDir logPath(QDir::homePath());
    logPath.cd(Identifiers::situationsDir);
    QFile outFile(logPath.filePath("serverlog.txt"));
    outFile.open(QIODevice::WriteOnly|QIODevice::Append);
    QTextStream ts(&outFile);
    ts << txt << endl;
}
#endif

#include "server.h"

Q_DECL_EXPORT int main(int argc, char* argv[])
{
#ifdef DEBUG_TO_FILE
    qInstallMsgHandler(customMessageHandler);
#endif

    return Server(argc, argv).exec();
}

