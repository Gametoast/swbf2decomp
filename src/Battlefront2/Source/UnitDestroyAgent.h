#pragma once

#include "PebbleFL/Common/PblHashTable.h"
#include "PebbleFL/Common/PblRtti.h"

#include <stddef.h>

#include "UnitAgent.h"
#include "AIGoal.h"

// VTABLE: SWBF2 0x007af9cc
class UnitDestroyAgent : public UnitAgent
{
public:
    UnitDestroyAgent(UnitController* pCtrl, const AI::GoalTarget& target);
    // FUNCTION: SWBF2 0x00666000
    // UnitDestroyAgent::IsRtti
    // FUNCTION: SWBF2 0x00666030
    // UnitDestroyAgent::GetDerivedRtti
    // FUNCTION: SWBF2 0x00666040
    // UnitDestroyAgent::GetDerivedRttiName
    PBL_DECLARE_DERIVED_RTTI(UnitDestroyAgent, UnitAgent)
    virtual const char* NFINAL_GetStateString(int iState) const;
    virtual int EventHandler(AI::EventInfo* pInfo);

protected:

    enum
    {
        STATE_START = 1,
        STATE_Goto = 2,
        STATE_WaitForSpawn = 3,
        STATE_FINAL = 4
    };

    virtual void EnterState();
    virtual void ExitState();
    virtual void UpdateStatePostVision(float dt);

    void CheckTargetPosition(const PblVector3& vPos);

public:
    AI::GoalTarget mTarget;
    PblVector3 mLastSeekPos;

};

 
 
 

