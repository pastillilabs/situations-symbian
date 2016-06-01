#ifndef NFCPLUGIN_H
#define NFCPLUGIN_H

#include <conditionplugin.h>

/**
  * NfcPlugin
  */
class NfcPlugin
    : public QObject
    , public ConditionPlugin
{
private:
    Q_OBJECT
    Q_INTERFACES(ConditionPlugin)

public:
    NfcPlugin();

private: // From ConditionPlugin
    Condition* createInstance(QObject* parent);
    void qmlRegisterTypes();
    const QVariant property(const char* name) const;
};

#endif // NFCPLUGIN_H

