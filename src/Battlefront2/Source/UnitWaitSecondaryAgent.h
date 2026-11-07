#pragma once

#include "PebbleFL/Common/PblHashTable.h"
#include "PebbleFL/Common/PblRtti.h"

#include <stddef.h>

#include "UnitAgent.h"
#include "PebbleFL/Common/PblHashTable.h"

class GameObject;

// VTABLE: SWBF2 0x007afe84
class UnitWaitSecondaryAgent : public UnitAgent
{
public:

    // the modtools build's state names: START Wait
    enum State
    {
        STATE_START = 1,
        STATE_Wait = 2,
        STATE_FINAL = 3
    };

    UnitWaitSecondaryAgent(UnitController* pCtrl, float fWaitTime, bool bArg);

    // FUNCTION: SWBF2 0x0066b670
    // UnitWaitSecondaryAgent::IsRtti
    // FUNCTION: SWBF2 0x0066b6a0
    // UnitWaitSecondaryAgent::GetDerivedRtti
    // FUNCTION: SWBF2 0x0066b6b0
    // UnitWaitSecondaryAgent::GetDerivedRttiName
    PBL_DECLARE_DERIVED_RTTI(UnitWaitSecondaryAgent, UnitAgent)
    virtual bool ShouldQueueCurCmd(const UnitCmdInfo& cmd) const;
    virtual int EventHandler(AI::EventInfo* pInfo);
    virtual const char* NFINAL_GetStateString(int iState) const;

    void TakeoffNow() { mWaitUntilMissionTime = 0.0f; }

protected:
    virtual void EnterState();
    virtual void ExitState();
    virtual void UpdateStatePostVision(float dt);

    GameObject* IsPlayerLookingAtVehicle();

public:
    float mWaitUntilMissionTime;
    float mPlayerWaitTimeout;
    float mImpatientSoundTime;

};
 
 
 

