#ifndef POWERSAVEPLUGIN_H
#define POWERSAVEPLUGIN_H

#include <actionplugin.h>

/**
  * PowersavePlugin
  */
class PowersavePlugin
    : public QObject
    , public ActionPlugin
{
private:
    Q_OBJECT
    Q_INTERFACES(ActionPlugin)

public:
    PowersavePlugin();

public: // From ActionPlugin
    Action* createInstance(QObject* parent);
    void qmlRegisterTypes();
    const QVariant property(const char* name) const;
};

#endif // POWERSAVEPLUGIN_H

