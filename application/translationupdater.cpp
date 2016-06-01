#include "translationupdater.h"
#include <identifiers.h>
#include <QProcess>
#include <QLocale>
#include <QtNetwork/QNetworkReply>

#ifdef Q_OS_SYMBIAN
static const QString LOC_BASE_URL("http://www.pastillilabs.com/Situations/situations_symbian/localisation/");
static const QString LOC_PACKAGE("%1.zip");

#include <zipfile.h>
#endif

#ifdef Q_WS_SIMULATOR
static const QString LOC_BASE_URL("http://www.pastillilabs.com/Situations/situations_n9/localisation/");
static const QString LOC_PACKAGE("%1.tar.gz");
#endif

TranslationUpdater::TranslationUpdater(QObject* parent)
    : QObject(parent)
    , mNetworkReply(0)
{
}

void TranslationUpdater::update()
{
    if(mNetworkReply == 0)
    {
        mLocale = QLocale::system().name();
        const QUrl url(LOC_BASE_URL + LOC_PACKAGE.arg(mLocale));

        mNetworkReply = mNetworkAccessManager.get(QNetworkRequest(url));
        connect(mNetworkReply, SIGNAL(finished()), this, SLOT(finished()));
    }
}

void TranslationUpdater::cancel()
{
    mNetworkReply = 0;
}

void TranslationUpdater::finished()
{
    QNetworkReply* reply(qobject_cast<QNetworkReply*>(sender()));
    if(reply == mNetworkReply)
    {
        switch(reply->error())
        {
        case QNetworkReply::NoError:
            {
                const int httpStatusCode(reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toUInt());
                switch(httpStatusCode)
                {
                case 200: // OK
                    {
                        QDir dir(QDir::homePath());
                        dir.mkdir(Identifiers::situationsDir);
                        dir.cd(Identifiers::situationsDir);
                        dir.mkdir(Identifiers::locDir);
                        dir.cd(Identifiers::locDir);

                        const QString packageName(LOC_PACKAGE.arg(mLocale));

                        QFile packageFile(dir.filePath(packageName));
                        packageFile.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Unbuffered);
                        packageFile.write(reply->readAll());
                        packageFile.flush();
                        packageFile.close();

#ifdef Q_OS_SYMBIAN
                        TRAP_IGNORE(extract(packageFile, dir));
#else
                        extract(packageFile, dir);
#endif

                        QFile::remove(dir.filePath(packageName));

                        emit ready();

                        mNetworkReply = 0;
                        mLocale.clear();
                    }
                    break;
                case 302: // Found (redirect)
                    {
                        const QUrl redirect(reply->header(QNetworkRequest::LocationHeader).toUrl());
                        mNetworkReply = mNetworkAccessManager.get(QNetworkRequest(redirect));
                        connect(mNetworkReply, SIGNAL(finished()), this, SLOT(finished()));
                    }
                    break;
                default:
                    break;
                }
            }
            break;
        case QNetworkReply::ContentNotFoundError:
            {
                const QString stripLocale(mLocale.section('_', 0, 0));
                if(stripLocale != mLocale)
                {
                    mLocale = stripLocale;

                    const QUrl url(LOC_BASE_URL + LOC_PACKAGE.arg(mLocale));
                    mNetworkReply = mNetworkAccessManager.get(QNetworkRequest(url));
                    connect(mNetworkReply, SIGNAL(finished()), this, SLOT(finished()));
                }
                else
                {
                    emit notAvailable();

                    mNetworkReply = 0;
                    mLocale.clear();
                }
            }
            break;
        default:
            emit notAvailable();

            mNetworkReply = 0;
            mLocale.clear();
            break;
        }
    }

    reply->deleteLater();
}

void TranslationUpdater::extract(const QFile& packageFile, const QDir& dir)
{
#ifdef Q_OS_SYMBIAN
    RFs fs;
    fs.Connect();
    CleanupClosePushL(fs);

    const TFileName zipFileName(QDir::toNativeSeparators(packageFile.fileName()).utf16());
    CZipFile* zipFile(CZipFile::NewL(fs, zipFileName));
    CleanupStack::PushL(zipFile);

    CZipFileMemberIterator* fileMembers(zipFile->GetMembersL());
    CleanupStack::PushL(fileMembers);

    CZipFileMember* member(fileMembers->NextL());
    while(member != 0)
    {
        CleanupStack::PushL(member);

        const QString memberName(QString::fromUtf16(member->Name()->Ptr(), member->Name()->Length()));

        TFileName fn(QDir::toNativeSeparators(dir.filePath(memberName)).utf16());

        RFile extractedMember;
        extractedMember.Replace(fs, fn, EFileShareAny|EFileWrite);
        CleanupClosePushL(extractedMember);

        RZipFileMemberReaderStream* fileStream(0);
        if(zipFile->GetInputStreamL(member, fileStream) == KErrNone)
        {
            CleanupStack::PushL(fileStream);

            const TUint16 size(member->UncompressedSize());

            RBuf8 bytes;
            bytes.CreateL(size);
            bytes.CleanupClosePushL();

            User::LeaveIfError(fileStream->Read(bytes, size));
            User::LeaveIfError(extractedMember.Write(bytes));

            CleanupStack::PopAndDestroy(); // bytes
            CleanupStack::PopAndDestroy(fileStream);
        }

        CleanupStack::PopAndDestroy(); // extractedMember
        CleanupStack::PopAndDestroy(member);

        member = fileMembers->NextL();
    }

    CleanupStack::PopAndDestroy(fileMembers);
    CleanupStack::PopAndDestroy(zipFile);
    CleanupStack::PopAndDestroy(); // fs
#endif
}

