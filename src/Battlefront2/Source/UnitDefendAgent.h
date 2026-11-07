#pragma once

#include "PebbleFL/Common/PblHashTable.h"
#include "PebbleFL/Common/PblRtti.h"

#include <stddef.h>

#include "UnitAgent.h"
#include "AIGoal.h"
#include "CmdInfo.h"
#include "CombatHelper.h"
#include "DefendHelper.h"

// VTABLE: SWBF2 0x007af960
class UnitDefendAgent : public UnitAgent
{
public:

    // the modtools build's state names: Start Defend Combat
    enum State
    {
        STATE_START = 1,
        STATE_Defend = 2,
        STATE_Combat = 3,
        STATE_FINAL = 4
    };

    UnitDefendAgent(UnitController* pCtrl, const AI::GoalTarget& target);
    // SYNTHETIC: SWBF2 0x00665bb0
    // UnitDefendAgent::`scalar deleting destructor'
    // SYNTHETIC: SWBF2 0x00665be0
    // UnitDefendAgent::~UnitDefendAgent
    virtual void Kill();
    // FUNCTION: SWBF2 0x00665b60
    // UnitDefendAgent::IsRtti
    // FUNCTION: SWBF2 0x00665b90
    // UnitDefendAgent::GetDerivedRtti
    // FUNCTION: SWBF2 0x00665ba0
    // UnitDefendAgent::GetDerivedRttiName
    PBL_DECLARE_DERIVED_RTTI(UnitDefendAgent, UnitAgent)
    virtual float GetUpdateRate() const;
    virtual const char* NFINAL_GetStateString(int iState) const;
    virtual int EventHandler(AI::EventInfo* pInfo);

protected:
    virtual void EnterState();
    virtual void ExitState();
    virtual void UpdateStatePostVision(float dt);

public:
    DefendHelper mDefendHelper;
    CmdInfo mCombatCmd;
    CombatHelper mCombatHelper;
    AI::GoalTarget mTarget;
    bool mMaintainVisualContact;

};

 
 
 
 
 

