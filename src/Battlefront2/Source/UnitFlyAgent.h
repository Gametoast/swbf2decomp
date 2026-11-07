#pragma once

#include "PebbleFL/Common/PblHashTable.h"
#include "PebbleFL/Common/PblRtti.h"

#include <stddef.h>

#include "UnitAgent.h"
#include "PebbleFL/Common/PblHashTable.h"

class BaseHint;
class EntityFlyer;

// VTABLE: SWBF2 0x007afa34
class UnitFlyAgent : public UnitAgent
{
public:

    // the modtools build's state names: START EXITHANGAR PICKUP FLY LAND DROPOFF
    enum State
    {
        STATE_START = 1,
        STATE_ExitHangar = 2,
        STATE_Pickup = 3,
        STATE_Fly = 4,
        STATE_Land = 5,
        STATE_DropOff = 6,
        STATE_FINAL = 7
    };

    UnitFlyAgent(UnitController* pCtrl);

    // FUNCTION: SWBF2 0x006666f0
    // UnitFlyAgent::IsRtti
    // FUNCTION: SWBF2 0x00666720
    // UnitFlyAgent::GetDerivedRtti
    // FUNCTION: SWBF2 0x00666730
    // UnitFlyAgent::GetDerivedRttiName
    PBL_DECLARE_DERIVED_RTTI(UnitFlyAgent, UnitAgent)
    virtual const char* NFINAL_GetStateString(int iState) const;
    virtual float GetUpdateRate() const;
    virtual int EventHandler(AI::EventInfo* pInfo);

protected:
    virtual void EnterState();
    virtual void ExitState();
    virtual void UpdateStatePostVision(float dt);

    const EntityFlyer* GetFlyer() const;
    EntityFlyer* GetFlyer();
    bool IsTransport() const;
    bool IsNearLandingPt();

    bool IsRoofOverhead() const;
    bool IsNearATAT();

public:
    const BaseHint* mHint;
    float mNextHintCheckTime;
    bool mGotoNonOwnedCP;
    float mWaitTime;

};
 
 
 

