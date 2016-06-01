#ifndef BRIGHTNESSPLUGIN_H
#define BRIGHTNESSPLUGIN_H

#include <actionplugin.h>

/**
  * BrightnessPlugin
  */
class BrightnessPlugin
    : public QObject
    , public ActionPlugin
{
private:
    Q_OBJECT
    Q_INTERFACES(ActionPlugin)

public:
    BrightnessPlugin();

private: // From ActionPlugin
    Action* createInstance(QObject* parent);
    void qmlRegisterTypes();
    const QVariant property(const char* name) const;
};

#endif // BRIGHTNESSPLUGIN_H

