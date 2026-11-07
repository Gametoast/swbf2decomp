#pragma once

#include "PebbleFL/Common/PblHashTable.h"
#include "PebbleFL/Common/PblRtti.h"

#include <stddef.h>

#include "UnitAgent.h"
#include "UnitCmdInfo.h"
#include "CombatHelper.h"

namespace AI
{
class EventInfo;
}

// VTABLE: SWBF2 0x007af7b0
class UnitCombatAgent : public UnitAgent
{
public:

    enum
    {
        STATE_START = 1,
        STATE_Combat = 2,
        STATE_FINAL = 3
    };

    UnitCombatAgent(UnitController* pCtrl, const UnitCombatInfo& info);

    int EventHandler(AI::EventInfo* pInfo);

    virtual void Kill();
    // FUNCTION: SWBF2 0x00662e10
    // UnitCombatAgent::IsRtti
    // FUNCTION: SWBF2 0x00662e40
    // UnitCombatAgent::GetDerivedRtti
    // FUNCTION: SWBF2 0x00662e50
    // UnitCombatAgent::GetDerivedRttiName
    PBL_DECLARE_DERIVED_RTTI(UnitCombatAgent, UnitAgent)
    virtual bool ShouldQueueCurCmd(const UnitCmdInfo& cmd) const;
    virtual float GetUpdateRate() const;

    virtual const char* NFINAL_GetStateString(int iState) const;
protected:
    virtual void EnterState();
    virtual void ExitState();
    virtual void UpdateStatePostVision(float dt);
public:

    UnitCombatInfo mCombatInfo;
    CombatHelper mCombatHelper;
};

 
 

