#pragma once

#include "PebbleFL/Common/PblHashTable.h"
#include "PebbleFL/Common/PblRtti.h"

#include <stddef.h>

#include "UnitAgent.h"
#include "PebbleFL/Common/PblHashTable.h"
#include "PebbleFL/Common/PblVector.h"

// VTABLE: SWBF2 0x007afb78
class UnitHoldAgent : public UnitAgent
{
public:

    // the modtools build's state names: START Wait
    enum State
    {
        STATE_START = 1,
        STATE_Wait = 2,
        STATE_FINAL = 3
    };

    UnitHoldAgent(UnitController* pCtrl);

    // FUNCTION: SWBF2 0x00668d50
    // UnitHoldAgent::IsRtti
    // FUNCTION: SWBF2 0x00668d80
    // UnitHoldAgent::GetDerivedRtti
    // FUNCTION: SWBF2 0x00668d90
    // UnitHoldAgent::GetDerivedRttiName
    PBL_DECLARE_DERIVED_RTTI(UnitHoldAgent, UnitAgent)
    virtual bool ShouldQueueCurCmd(const UnitCmdInfo& cmd) const;
    virtual const char* NFINAL_GetStateString(int iState) const;
    virtual int EventHandler(AI::EventInfo* pInfo);

protected:
    virtual void EnterState();
    virtual void ExitState();
    virtual void UpdateStatePostVision(float dt);

    void SetNewSearchHeading();

public:
    float mInsubValue;
    PblVector3 mSearchPos;

};
 
 
 

