#pragma once

#include "PebbleFL/Common/PblHashTable.h"
#include "PebbleFL/Common/PblRtti.h"

#include <stddef.h>

#include "UnitAgent.h"
#include "CmdInfo.h"
#include "CombatHelper.h"
#include "DefendHelper.h"
#include "PebbleFL/Common/PblHandle.h"

class FlagItem;
int GetFormationType(const UnitController* pCtrl);

// VTABLE: SWBF2 0x007af88c
class UnitCTFOffenseAgent : public UnitAgent
{
public:

    // the modtools build's state names: START PickupFl RtrnFlag DfndCary AttkFlag DfndDrop CampFlag Combat
    enum State
    {
        STATE_START = 1,
        STATE_PickupFlag = 2,
        STATE_ReturnFlag = 3,
        STATE_DefendCarrier = 4,
        STATE_AttackFlag = 5,
        STATE_DefendDropped = 6,
        STATE_CampFlag = 7,
        STATE_Combat = 8,
        STATE_FINAL = 9
    };

    UnitCTFOffenseAgent(UnitController* pCtrl, const FlagItem* pFlag, const PblVector3& vPos);
    // SYNTHETIC: SWBF2 0x00664630
    // UnitCTFOffenseAgent::`scalar deleting destructor'
    // SYNTHETIC: SWBF2 0x00664660
    // UnitCTFOffenseAgent::~UnitCTFOffenseAgent
    virtual void Kill();

    // FUNCTION: SWBF2 0x006645e0
    // UnitCTFOffenseAgent::IsRtti
    // FUNCTION: SWBF2 0x00664610
    // UnitCTFOffenseAgent::GetDerivedRtti
    // FUNCTION: SWBF2 0x00664620
    // UnitCTFOffenseAgent::GetDerivedRttiName
    PBL_DECLARE_DERIVED_RTTI(UnitCTFOffenseAgent, UnitAgent)
    virtual float GetUpdateRate() const;

    virtual const char* NFINAL_GetStateString(int iState) const;
    virtual int EventHandler(AI::EventInfo* pInfo);

    virtual void EnterState();
    virtual void ExitState();
    virtual void UpdateStatePostVision(float dt);

    bool IsFlagNeutral() const;
    bool IsFlagInMyHands() const;
    bool IsFlagInAlliedHandsButNotMine() const;
    bool IsFlagInEnemyHands() const;
    void FaceTargetIfClose(ControllableHeight eHeight);

    CmdInfo mCombatCmd;
    CombatHelper mCombatHelper;
    DefendHelper mDefendHelper;
    PblHandle<const GameObject> mFlag;
    PblVector3 mCapturePos;
    bool mPathing;
    float mPathTime;
    float mDefendCarrierOffset;
};

 
 
 
 
 

