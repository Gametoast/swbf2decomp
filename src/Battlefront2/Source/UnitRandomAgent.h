#pragma once

#include "PebbleFL/Common/PblHashTable.h"
#include "PebbleFL/Common/PblRtti.h"

#include <stddef.h>

#include "UnitAgent.h"
#include "PebbleFL/Common/PblHashTable.h"
#include "PebbleFL/Common/PblVector.h"

// VTABLE: SWBF2 0x007afc58
class UnitRandomAgent : public UnitAgent
{
public:

    // the modtools build's state names: START RndGoto
    enum State
    {
        STATE_START = 1,
        STATE_RandomGoto = 2,
        STATE_FINAL = 3
    };

    UnitRandomAgent(UnitController* pCtrl);

    // FUNCTION: SWBF2 0x00669320
    // UnitRandomAgent::IsRtti
    // FUNCTION: SWBF2 0x00669350
    // UnitRandomAgent::GetDerivedRtti
    // FUNCTION: SWBF2 0x00669360
    // UnitRandomAgent::GetDerivedRttiName
    PBL_DECLARE_DERIVED_RTTI(UnitRandomAgent, UnitAgent)
    virtual bool ShouldQueueCurCmd(const UnitCmdInfo& cmd) const;
    virtual const char* NFINAL_GetStateString(int iState) const;
    virtual int EventHandler(AI::EventInfo* pInfo);

protected:
    virtual void EnterState();
    virtual void ExitState();
    virtual void UpdateStatePostVision(float dt);

    bool IsPointValid(const PblVector3& vPoint);
    void PickRandomDest();

public:
    PblVector3 mStartPos;
    float mRandTime;

};
 
 
 

