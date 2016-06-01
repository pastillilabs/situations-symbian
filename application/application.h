#ifndef APPLICATION_H
#define APPLICATION_H

#include "qmlapplicationviewer.h"
#include "translationupdater.h"
#include "client.h"
#include <translationmanager.h>
#include <storagemanager.h>
#include <QApplication>

/**
  * Application
  */
class Application
    : public QApplication
{
private:
    Q_OBJECT
    Q_PROPERTY(bool advancedMode READ advancedMode WRITE setAdvancedMode NOTIFY advancedModeChanged)

public:
    Application(int& argc, char** argv);
    ~Application();

    int exec();
    
    void setAdvancedMode(bool advancedMode);
    bool advancedMode() const;

signals:
    void advancedModeChanged(bool advancedMode);

private slots:
    void addConditionPlugin(ConditionPlugin& plugin, int index, const QString& name);
    void addActionPlugin(ActionPlugin& plugin, int index, const QString& name);
    void rmvConditionPlugin(ConditionPlugin& plugin, const QString& name);
    void rmvActionPlugin(ActionPlugin& plugin, const QString& name);

private:
    void store();
    void restore();

private:
    const QString mVersionString;
    QmlApplicationViewer mViewer;

    TranslationManager mTranslationManager;
    TranslationUpdater mTranslationUpdater;
    StorageManager mStorageManager;
    Client mClient;

    bool mAdvancedMode;
};

#endif // APPLICATION_H

