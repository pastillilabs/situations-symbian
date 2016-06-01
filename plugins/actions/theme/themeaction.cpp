#include "themeaction.h"
#include <identifiers.h>
#include <QTimerEvent>

#ifdef Q_OS_SYMBIAN
#include <e32base.h>
#include <aknsutils.h>
#include <AknSSrvClient.h>
#include <AknsPkgID.h>

#include <centralrepository.h>
#include <AknSkinsInternalCRKeys.h>
#endif

int ThemeAction::mRestoreTimerId(0);
QVariant ThemeAction::mRestoreValue;

ThemeAction::ThemeAction(QObject* parent)
    : Action(parent)
{
}

ThemeAction::~ThemeAction()
{
    doCancelRestore();
}

QString ThemeAction::nameById(const int id) const
{
    QString name;

#ifdef Q_OS_SYMBIAN
    RAknsSrvSession aknsSrvSession;
    TInt err(aknsSrvSession.Connect());
    if(err == KErrNone)
    {
        CArrayPtr<CAknsSrvSkinInformationPkg>* skinInfoArray(0);
        TRAP(err, skinInfoArray = aknsSrvSession.EnumerateSkinPackagesL(EAknsSrvAll));
        if(err == KErrNone)
        {
            for(TInt i(0); i < skinInfoArray->Count() && name.isEmpty(); ++i)
            {
                CAknsSrvSkinInformationPkg* info(skinInfoArray->At(i));
                TPtrC namePtr(info->Name());
                TInt themeId(info->PID().iNumber);

                if(themeId == id) name = QString::fromUtf16(namePtr.Ptr(), namePtr.Length());
            }

            skinInfoArray->ResetAndDestroy();
            delete skinInfoArray;
        }

        aknsSrvSession.Close();
    }
#else
    name = QString::number(id);
#endif

    return name;
}

const QVariant ThemeAction::currentValue() const
{
#ifdef Q_OS_SYMBIAN
    int current(0);
    CRepository* repository(0);
    TRAPD(err, repository = CRepository::NewL(KCRUidPersonalisation));
    if(err == KErrNone)
    {
        TAknsPkgIDBuf currentIDBuf;
        TAknsPkgID currentID;
        repository->Get(KPslnActiveSkinUid, currentIDBuf);
        TRAP_IGNORE(currentID.SetFromDesL(currentIDBuf));
        current = currentID.iNumber;
        delete repository;
    }
    return current;
#endif

#ifdef Q_WS_SIMULATOR
    return 0;
#endif
}

void ThemeAction::doExecute(const QVariant& value)
{
#ifdef Q_OS_SYMBIAN
    const int current(currentValue().toInt());
    const int next(value.toInt());

    if(restore() && !mRestoreValue.isValid())
    {
        mRestoreValue = current;
    }

    if(current != next)
    {
        RAknsSrvSession aknsSrvSession;
        TInt err(aknsSrvSession.Connect());
        if(err == KErrNone)
        {
            CArrayPtr<CAknsSrvSkinInformationPkg>* skinInfoArray(0);
            TRAP(err, skinInfoArray = aknsSrvSession.EnumerateSkinPackagesL(EAknsSrvAll));
            if(err == KErrNone)
            {
                for(TInt i(0); i < skinInfoArray->Count(); ++i)
                {
                    const TAknsPkgID pkgId(skinInfoArray->At(i)->PID());
                    if(pkgId.iNumber == next)
                    {
                        if(aknsSrvSession.SetAllDefinitionSets(pkgId) == KErrNone)
                        {
                            TAknsPkgIDBuf newBuf;
                            pkgId.CopyToDes(newBuf);
                            CRepository* repository(0);
                            TRAP(err, repository = CRepository::NewL(KCRUidPersonalisation));
                            if(err == KErrNone) repository->Set(KPslnActiveSkinUid, newBuf);
                            delete repository;
                        }

                        break;
                    }
                }
                skinInfoArray->ResetAndDestroy();
                delete skinInfoArray;
            }

            aknsSrvSession.Close();
        }
    }
#endif

#ifdef Q_WS_SIMULATOR
    Q_UNUSED(value);
#endif
}

void ThemeAction::doRestore()
{
    if(!mRestoreTimerId) mRestoreTimerId = startTimer(0);
}

void ThemeAction::doCancelRestore()
{
    if(mRestoreTimerId) killTimer(mRestoreTimerId);
    mRestoreTimerId = 0;

    if(!restore()) mRestoreValue.clear();
}

void ThemeAction::timerEvent(QTimerEvent* event)
{
    const int timerId(event->timerId());
    if(timerId == mRestoreTimerId)
    {
        killTimer(mRestoreTimerId);
        mRestoreTimerId = 0;
        if(mRestoreValue.isValid()) doExecute(mRestoreValue);
        mRestoreValue.clear();
    }

    Action::timerEvent(event);
}

