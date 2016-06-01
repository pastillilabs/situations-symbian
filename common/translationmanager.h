#ifndef TRANSLATIONMANAGER_H
#define TRANSLATIONMANAGER_H

#include "common_global.h"
#include <QObject>
#include <QMap>

class QTranslator;

/**
  * TranslationManager
  */
class COMMONSHARED_EXPORT TranslationManager
    : public QObject
{
private:
    Q_OBJECT
    Q_DISABLE_COPY(TranslationManager)

public:
    explicit TranslationManager(QObject* parent = 0);

    bool addComponent(const QString& component);
    void rmvComponent(const QString& component);

    void reloadTranslations();

private:
    bool loadResourceTranslation(const QString& component, QTranslator& translator);
    bool loadExternalTranslation(const QString& component, QTranslator& translator);

private:
    QMap<QString, QTranslator*> mResourceTranslations;
    QMap<QString, QTranslator*> mExternalTranslations;
};

#endif // TRANSLATIONMANAGER_H

