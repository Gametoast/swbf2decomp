#pragma once

#include "PebbleFL/Common/PblHashTable.h"
#include "PebbleFL/Common/PblRtti.h"

#include <stddef.h>

#include "UnitAgent.h"
#include "CoverHelper.h"
#include "CmdInfo.h"
#include "CombatHelper.h"
#include "PebbleFL/Common/PblHandle.h"
#include "PebbleFL/Common/PblHashTable.h"

class GameObject;

// VTABLE: SWBF2 0x007afb10
class UnitFollowAgent : public UnitAgent
{
public:

    // the modtools build's state names: START FlwTight LoosePasve LooseCombat LooseSeek FwdCover
    enum State
    {
        STATE_START = 1,
        STATE_FollowTight = 2,
        STATE_LoosePassive = 3,
        STATE_LooseCombat = 4,
        STATE_LooseSeek = 5,
        STATE_ForwardCover = 6,
        STATE_FINAL = 7
    };

    UnitFollowAgent(UnitController* pCtrl, PblHandle<GameObject const> hTarget, bool bTightFollow);

    // FUNCTION: SWBF2 0x00667f00
    // UnitFollowAgent::IsRtti
    // FUNCTION: SWBF2 0x00667f30
    // UnitFollowAgent::GetDerivedRtti
    // FUNCTION: SWBF2 0x00667f40
    // UnitFollowAgent::GetDerivedRttiName
    PBL_DECLARE_DERIVED_RTTI(UnitFollowAgent, UnitAgent)
    virtual void Kill();
    virtual bool ShouldQueueCurCmd(const UnitCmdInfo& cmd) const;
    virtual float GetUpdateRate() const;
    virtual const char* NFINAL_GetStateString(int iState) const;
    virtual int EventHandler(AI::EventInfo* pInfo);

protected:
    virtual void EnterState();
    virtual void ExitState();
    virtual void UpdateStatePostVision(float dt);

    bool CheckForForwardHintNodes();
    bool IsInLooseRange();

public:
    CoverHelper mCoverHelper;
    CmdInfo mCombatCmd;
    CombatHelper mCombatHelper;
    PblHandle<GameObject const> mTarget;
    bool mTightFollowMode;

};

 
 
 
 
 
 

