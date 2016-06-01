#ifndef CELLCONDITION_H
#define CELLCONDITION_H

#include <condition.h>
#include <QSystemNetworkInfo>

QTM_USE_NAMESPACE

/**
  * CellCondition
  */
class CellCondition
    : public Condition
{
private:
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(int cellCount READ cellCount NOTIFY cellCountChanged)
    Q_PROPERTY(bool recording READ recording NOTIFY recordingChanged)
    Q_PROPERTY(bool adding READ adding NOTIFY addingChanged)

public:
    CellCondition(QObject* parent = 0);
    ~CellCondition();

    QString name() const;
    void setName(const QString name);

    int cellCount() const;
    bool recording() const;
    bool adding() const;

    Q_INVOKABLE void startRecording(bool adding);
    Q_INVOKABLE void stopRecording();

signals:
    void nameChanged(QString name);
    void cellCountChanged(int cellCount);
    void recordingChanged(bool recording);
    void addingChanged(bool adding);

private:
    void resolveActive();
    void addCell(int cell);
    void rmvCell(int cell);

private slots:
    void cellIdChanged(int cellId);

private: // From Condition
    void doStart();
    void doStop();
    void doStore(QVariantMap& data) const;
    void doRestore(const QVariantMap& data);

private:
    QString mName;
    QList<int> mCellList;

    bool mRecording;
    bool mAdding;
    QSystemNetworkInfo* mSystemNetworkInfo;
};

#endif // CELLCONDITION_H

