#include "cellcondition.h"
#include <identifiers.h>

CellCondition::CellCondition(QObject *parent)
    : Condition(parent)
    , mRecording(false)
    , mAdding(false)
    , mSystemNetworkInfo(0)
{
}

CellCondition::~CellCondition()
{
    doStop();
}

QString CellCondition::name() const
{
    return mName;
}

void CellCondition::setName(const QString name)
{
    if(name != mName)
    {
        mName = name;
        emit nameChanged(name);
    }
}

int CellCondition::cellCount() const
{
    return mCellList.count();
}

bool CellCondition::recording() const
{
    return mRecording;
}

bool CellCondition::adding() const
{
    return mAdding;
}

void CellCondition::startRecording(bool adding)
{
    if(!mRecording)
    {
        mSystemNetworkInfo = new QSystemNetworkInfo(this);
        connect(mSystemNetworkInfo, SIGNAL(cellIdChanged(int)), this, SLOT(cellIdChanged(int)));
    }

    mAdding = adding;
    emit addingChanged(mAdding);

    mRecording = true;
    emit recordingChanged(mRecording);

    if(adding) addCell(mSystemNetworkInfo->cellId());
    else rmvCell(mSystemNetworkInfo->cellId());
}

void CellCondition::stopRecording()
{
    if(mSystemNetworkInfo) disconnect(mSystemNetworkInfo, SIGNAL(cellIdChanged(int)), this, SLOT(cellIdChanged(int)));
    delete mSystemNetworkInfo;
    mSystemNetworkInfo = 0;

    mRecording = false;
    emit recordingChanged(mRecording);
}

void CellCondition::resolveActive()
{
    setActive(mCellList.contains(mSystemNetworkInfo->cellId()));
}

void CellCondition::addCell(int cell)
{
    if(cell > 0 && !mCellList.contains(cell))
    {
        mCellList.append(cell);
        emit cellCountChanged(mCellList.count());
    }
}

void CellCondition::rmvCell(int cell)
{
    if(mCellList.removeOne(cell))
    {
        emit cellCountChanged(mCellList.count());
    }
}

void CellCondition::cellIdChanged(int cellId)
{
    resolveActive();

    if(mRecording)
    {
        if(mAdding) addCell(cellId);
        else rmvCell(cellId);
    }
}

void CellCondition::doStart()
{
    if(!mSystemNetworkInfo)
    {
        mSystemNetworkInfo = new QSystemNetworkInfo(this);
        connect(mSystemNetworkInfo, SIGNAL(cellIdChanged(int)), this, SLOT(cellIdChanged(int)));
    }

    resolveActive();
}

void CellCondition::doStop()
{
    delete mSystemNetworkInfo;
    mSystemNetworkInfo = 0;

    setActive(false);
}

void CellCondition::doStore(QVariantMap &data) const
{
    QVariantList cells;
    foreach(const int cell, mCellList) cells.append(cell);

    data[Identifiers::keyValue] = cells;
    data[Identifiers::keyName] = mName;
}

void CellCondition::doRestore(const QVariantMap& data)
{
    const QVariantList cells(data[Identifiers::keyValue].toList());

    mCellList.clear();
    foreach(const QVariant& cell, cells) mCellList.append(cell.toInt());
    emit cellCountChanged(mCellList.count());

    setName(data[Identifiers::keyName].toString());
}

