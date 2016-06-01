#ifndef CLOSEPLUGIN_H
#define CLOSEPLUGIN_H

#include <actionplugin.h>

/**
  * ClosePlugin
  */
class ClosePlugin
    : public QObject
    , public ActionPlugin
{
private:
    Q_OBJECT
    Q_INTERFACES(ActionPlugin)

public:
    ClosePlugin();

private: // From ActionPlugin
    Action* createInstance(QObject* parent);
    void qmlRegisterTypes();
    const QVariant property(const char* name) const;
};

#endif // CLOSEPLUGIN_H

