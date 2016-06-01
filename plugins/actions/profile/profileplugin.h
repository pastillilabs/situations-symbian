#ifndef PROFILEPLUGIN_H
#define PROFILEPLUGIN_H

#include <actionplugin.h>

/**
  * ProfilePlugin
  */
class ProfilePlugin
    : public QObject
    , public ActionPlugin
{
private:
    Q_OBJECT
    Q_INTERFACES(ActionPlugin)

public:
    ProfilePlugin();

private: // From ActionPlugin
    Action* createInstance(QObject* parent);
    void qmlRegisterTypes();
    const QVariant property(const char* name) const;
};

#endif // PROFILEPLUGIN_H

