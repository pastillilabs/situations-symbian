#ifndef FLIGHTMODEPLUGIN_H
#define FLIGHTMODEPLUGIN_H

#include <actionplugin.h>

/**
  * FlightmodePlugin
  */
class FlightmodePlugin
    : public QObject
    , public ActionPlugin
{
private:
    Q_OBJECT
    Q_INTERFACES(ActionPlugin)

public:
    FlightmodePlugin();

private: // From ActionPlugin
    Action* createInstance(QObject* parent);
    void qmlRegisterTypes();
    const QVariant property(const char* name) const;
};

#endif // FLIGHTMODEPLUGIN_H

