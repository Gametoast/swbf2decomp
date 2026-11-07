#pragma once

#include "PebbleFL/Common/PblHashTable.h"
#include "PebbleFL/Common/PblRtti.h"

#include <stddef.h>

#include "UnitAgent.h"
#include "EventInfo.h"
#include "CmdInfo.h"
#include "CombatHelper.h"

// VTABLE: SWBF2 0x007af8f8
class UnitDeathmatchAgent : public UnitAgent
{
public:

    enum
    {
        STATE_START = 1,
        STATE_RunPoint = 2,
        STATE_RunEnemy = 3,
        STATE_Combat = 4,
        STATE_Stationary = 5,
        STATE_FINAL = 6
    };

    UnitDeathmatchAgent(UnitController* pCtrl);

    virtual void Kill();
    // FUNCTION: SWBF2 0x006653a0
    // UnitDeathmatchAgent::IsRtti
    // FUNCTION: SWBF2 0x006653d0
    // UnitDeathmatchAgent::GetDerivedRtti
    // FUNCTION: SWBF2 0x006653e0
    // UnitDeathmatchAgent::GetDerivedRttiName
    PBL_DECLARE_DERIVED_RTTI(UnitDeathmatchAgent, UnitAgent)

    int EventHandler(AI::EventInfo* pInfo);

    virtual const char* NFINAL_GetStateString(int iState) const;
protected:
    virtual void EnterState();
    virtual void ExitState();
    virtual void UpdateStatePostVision(float dt);
    void DoJetJumpStuff();
public:

    CmdInfo mCombatCmd;
    CombatHelper mCombatHelper;
    float mNextJumpTime;
};

 
 
 
 

