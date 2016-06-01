#ifndef LAUNCHACTION_H
#define LAUNCHACTION_H

#include <action.h>

/**
  * LaunchAction
  */
class LaunchAction
    : public Action
{
private:
    Q_OBJECT
    Q_ENUMS(Type)
    Q_PROPERTY(int type READ type WRITE setType NOTIFY typeChanged)
    Q_PROPERTY(bool situationEnd READ situationEnd WRITE setSituationEnd NOTIFY situationEndChanged)

public:
    enum Type {
        TypeApplication = 0,
        TypeBookmark
    };

public:
    explicit LaunchAction(QObject* parent = 0);
    ~LaunchAction();

    int type() const;
    void setType(const int type);
    bool situationEnd() const;
    void setSituationEnd(bool situationEnd);

    Q_INVOKABLE const QString nameById(const QVariant id) const;

private:
    void launch();

signals:
    void typeChanged(int type);
    void situationEndChanged(bool situationEnd);

private: // From Action
    void doStart(bool running);
    void doStop(bool running);

    void doGetData(QVariantMap& data) const;
    void doSetData(const QVariantMap& data);

private:
    int mType;
    bool mSituationEnd;
};

#endif // LAUNCHACTION_H

