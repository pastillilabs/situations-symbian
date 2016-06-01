#ifndef TRANSLATIONUPDATER_H
#define TRANSLATIONUPDATER_H

#include <QObject>
#include <QDir>
#include <QFile>
#include <QtNetwork/QNetworkAccessManager>

class QNetworkReply;

/**
  * TranslationUpdater
  */
class TranslationUpdater
    : public QObject
{
private:
    Q_OBJECT

public:
    explicit TranslationUpdater(QObject* parent = 0);

    Q_INVOKABLE void update();
    Q_INVOKABLE void cancel();

signals:
    void ready();
    void notAvailable();

private slots:
    void finished();

private:
    void extract(const QFile& packageFile, const QDir& dir);

private:
    QNetworkAccessManager mNetworkAccessManager;

    QNetworkReply* mNetworkReply;
    QString mLocale;
};

#endif // TRANSLATIONUPDATER_H

