#include "translationmanager.h"
#include "identifiers.h"
#include <QTranslator>
#include <QCoreApplication>
#include <QLocale>
#include <QDir>

static const QString LOCFILE("%1_%2");
static const QString RESDIR(":/ts/%1");

TranslationManager::TranslationManager(QObject* parent)
    : QObject(parent)
{
}

bool TranslationManager::addComponent(const QString& component)
{
    bool systemLocale(false);

    // Load resource translation by default
    if(!mResourceTranslations.contains(component))
    {
        QTranslator* translator(new QTranslator(this));
        systemLocale = loadResourceTranslation(component, *translator);

        mResourceTranslations.insert(component, translator);
        QCoreApplication::installTranslator(translator);
    }

    // Load ext translation if available
    if(!mExternalTranslations.contains(component))
    {
        QTranslator* translator(new QTranslator(this));
        if(loadExternalTranslation(component, *translator))
        {
            mExternalTranslations.insert(component, translator);
            QCoreApplication::installTranslator(translator);
        }
        else
        {
            delete translator;
        }
    }

    return systemLocale;
}

void TranslationManager::rmvComponent(const QString& component)
{
    QTranslator* translator(mResourceTranslations.take(component));
    if(translator)
    {
        QCoreApplication::removeTranslator(translator);
        delete translator;
    }

    translator = mExternalTranslations.take(component);
    if(translator)
    {
        QCoreApplication::removeTranslator(translator);
        delete translator;
    }
}

void TranslationManager::reloadTranslations()
{
    const QList<QString> keys(mResourceTranslations.keys());
    foreach(const QString& key, keys)
    {
        rmvComponent(key);
        addComponent(key);
    }
}

bool TranslationManager::loadResourceTranslation(const QString& component, QTranslator& translator)
{
    bool systemLocale(false);

    const QString locDir(RESDIR.arg(component));
    const QString locFile(LOCFILE.arg(component).arg(QLocale::system().name()));

    systemLocale = translator.load(locFile, locDir);
    if(!systemLocale)
    {
        // Default to english
        translator.load(LOCFILE.arg(component).arg("en"), locDir);
    }

    return systemLocale;
}

bool TranslationManager::loadExternalTranslation(const QString& component, QTranslator& translator)
{
    QDir dir(QDir::homePath());
    dir.cd(Identifiers::situationsDir);
    dir.cd(Identifiers::locDir);

    const QString locFile(LOCFILE.arg(component).arg(QLocale::system().name()));
    return translator.load(locFile, dir.path());
}

