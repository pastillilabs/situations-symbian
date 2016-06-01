#ifndef MOBILEDATAPLUGIN_H
#define MOBILEDATAPLUGIN_H

#include <actionplugin.h>

/**
  * MobileDataPlugin
  */
class MobileDataPlugin
    : public QObject
    , public ActionPlugin
{
private:
    Q_OBJECT
    Q_INTERFACES(ActionPlugin)

public:
    MobileDataPlugin();

private: // From ActionPlugin
    Action* createInstance(QObject* parent);
    void qmlRegisterTypes();
    const QVariant property(const char* name) const;
};

#endif // MOBILEDATAPLUGIN_H

