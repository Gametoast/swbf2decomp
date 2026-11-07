#pragma once

#include "PebbleFL/Common/PblHashTable.h"
#include "PebbleFL/Common/PblRtti.h"

#include <stddef.h>

#include "UnitAgent.h"
#include "PebbleFL/Common/PblHandle.h"
#include "PebbleFL/Common/PblHashTable.h"

class GameObject;

// VTABLE: SWBF2 0x007af724
class UnitBoardAgent : public UnitAgent
{
public:

    // the modtools build's state names: START Goto Board
    enum State
    {
        STATE_START = 1,
        STATE_Goto = 2,
        STATE_Board = 3,
        STATE_FINAL = 4
    };

    UnitBoardAgent(UnitController* pCtrl, GameObject* pVehicle, int iVOResponseHandle);

    // FUNCTION: SWBF2 0x00661f80
    // UnitBoardAgent::IsRtti
    // FUNCTION: SWBF2 0x00661fb0
    // UnitBoardAgent::GetDerivedRtti
    // FUNCTION: SWBF2 0x00661fc0
    // UnitBoardAgent::GetDerivedRttiName
    PBL_DECLARE_DERIVED_RTTI(UnitBoardAgent, UnitAgent)
    virtual bool ShouldQueueCurCmd(const UnitCmdInfo& cmd) const;
    virtual const char* NFINAL_GetStateString(int iState) const;
    virtual int EventHandler(AI::EventInfo* pInfo);

protected:
    virtual void EnterState();
    virtual void ExitState();
    virtual void UpdateStatePostVision(float dt);

public:
    PblHandle<GameObject> mVehicle;
    int mVOResponseHandle;
    float mStateEnterTime;

};
 
 
 
 

