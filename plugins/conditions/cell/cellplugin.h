#ifndef CELLPLUGIN_H
#define CELLPLUGIN_H

#include <conditionplugin.h>

/**
  * CellPlugin
  */
class CellPlugin
    : public QObject
    , public ConditionPlugin
{
private:
    Q_OBJECT
    Q_INTERFACES(ConditionPlugin)

public:
    CellPlugin();

private: // From ConditionPlugin
    Condition* createInstance(QObject* parent);
    void qmlRegisterTypes();
    const QVariant property(const char* name) const;
};

#endif // CELLPLUGIN_H

