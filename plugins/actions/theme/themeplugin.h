#ifndef THEMEPLUGIN_H
#define THEMEPLUGIN_H

#include <actionplugin.h>

/**
  * ThemePlugin
  */
class ThemePlugin
    : public QObject
    , public ActionPlugin
{
private:
    Q_OBJECT
    Q_INTERFACES(ActionPlugin)

public:
    ThemePlugin();

private: // From ActionPlugin
    Action* createInstance(QObject* parent);
    void qmlRegisterTypes();
    const QVariant property(const char* name) const;
};

#endif // THEMEPLUGIN_H

