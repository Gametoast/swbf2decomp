#pragma once

#include "PebbleFL/Common/PblHashTable.h"
#include "PebbleFL/Common/PblRtti.h"

#include <stddef.h>

#include "UnitAgent.h"
#include "EventInfo.h"
#include "PebbleFL/Common/PblHashTable.h"

// VTABLE: SWBF2 0x007afd98
class UnitUnBoardAgent : public UnitAgent
{
public:

    // the modtools build's state names: START
    enum State
    {
        STATE_START = 1,
        STATE_FINAL = 2
    };

    UnitUnBoardAgent(UnitController* pCtrl);

    // FUNCTION: SWBF2 0x0066b1b0
    // UnitUnBoardAgent::IsRtti
    // FUNCTION: SWBF2 0x0066b1e0
    // UnitUnBoardAgent::GetDerivedRtti
    // FUNCTION: SWBF2 0x0066b1f0
    // UnitUnBoardAgent::GetDerivedRttiName
    PBL_DECLARE_DERIVED_RTTI(UnitUnBoardAgent, UnitAgent)
    virtual bool ShouldQueueCurCmd(const UnitCmdInfo& cmd) const;
    virtual int EventHandler(AI::EventInfo* pInfo);
    virtual const char* NFINAL_GetStateString(int iState) const;

protected:
    virtual void EnterState();
    virtual void ExitState();
    virtual void UpdateStatePostVision(float dt);

public:
};
 

