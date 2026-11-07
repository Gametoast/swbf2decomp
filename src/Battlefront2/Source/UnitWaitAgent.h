#pragma once

#include "PebbleFL/Common/PblHashTable.h"
#include "PebbleFL/Common/PblRtti.h"

#include <stddef.h>

#include "UnitAgent.h"
#include "PebbleFL/Common/PblHashTable.h"
#include "PebbleFL/Common/PblVector.h"

// VTABLE: SWBF2 0x007afe14
class UnitWaitAgent : public UnitAgent
{
public:

    // the modtools build's state names: START Wait
    enum State
    {
        STATE_START = 1,
        STATE_Wait = 2,
        STATE_FINAL = 3
    };

    UnitWaitAgent(UnitController* pCtrl);

    // FUNCTION: SWBF2 0x0066b330
    // UnitWaitAgent::IsRtti
    // FUNCTION: SWBF2 0x0066b360
    // UnitWaitAgent::GetDerivedRtti
    // FUNCTION: SWBF2 0x0066b370
    // UnitWaitAgent::GetDerivedRttiName
    PBL_DECLARE_DERIVED_RTTI(UnitWaitAgent, UnitAgent)
    virtual const char* NFINAL_GetStateString(int iState) const;

protected:
    virtual void EnterState();
    virtual void ExitState();
    virtual void UpdateStatePostVision(float dt);

    void SetNewSearchHeading();

public:
    PblVector3 mSearchPos;
    float mTargetTimer;

};
 
 
 

