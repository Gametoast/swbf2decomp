#pragma once

#include "PebbleFL/Common/PblHashTable.h"
#include "PebbleFL/Common/PblRtti.h"

#include <stddef.h>

#include "UnitAgent.h"
#include "SnipeHelper.h"
#include "PatrolHelper.h"
#include "RepairHelper.h"
#include "MineHelper.h"
#include "PebbleFL/Common/PblHandle.h"
#include "PebbleFL/Common/PblHashTable.h"
#include "PebbleFL/Common/PblVector.h"

class CommandPost;
class GameObject;
class UnitController;
namespace AI
{
class EventInfo;
}

// VTABLE: SWBF2 0x007afd40
class UnitTrooperAgent : public UnitAgent
{
public:

    enum State
    {
        STATE_START = 1,
        STATE_Goto = 2,
        STATE_Convoy = 3,
        STATE_Patrol = 4,
        STATE_MineLaying = 5,
        STATE_Aid = 6,
        STATE_Snipe = 7,
        STATE_FINAL = 8
    };

    UnitTrooperAgent(UnitController* pCtrl, const CommandPost* pPost);

    virtual void Kill();
    // FUNCTION: SWBF2 0x0066a3f0
    // UnitTrooperAgent::IsRtti
    // FUNCTION: SWBF2 0x0066a420
    // UnitTrooperAgent::GetDerivedRtti
    // FUNCTION: SWBF2 0x0066a430
    // UnitTrooperAgent::GetDerivedRttiName
    PBL_DECLARE_DERIVED_RTTI(UnitTrooperAgent, UnitAgent)
    virtual float GetUpdateRate() const;

    virtual const char* NFINAL_GetStateString(int iState) const;

protected:
    virtual void EnterState();
    virtual void ExitState();

    virtual void UpdateStatePostVision(float dt);

    bool CheckForVehicleConvoy();

public:
    virtual int EventHandler(AI::EventInfo* pInfo);

    ConstCommandPostHandle mPost;
    PblVector3 mCPPos;
    PblVector3 mCPOffset;
    float mCPTime;
    PblVector3 mSrcPos;
    SnipeHelper mSnipeHelper;
    PatrolHelper mPatrolHelper;
    RepairHelper mRepairHelper;
    MineHelper mMineHelper;
    PblHandle<GameObject> mConvoyParent;

};

 
 
 
 
 
 
 

