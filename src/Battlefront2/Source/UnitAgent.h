#pragma once

#include "PebbleFL/Common/PblHashTable.h"
#include "PebbleFL/Common/PblRtti.h"

#include <stddef.h>

#include "Agent.h"
#include "AttackHelper.h"
#include "Controllable.h"
#include "UnitController.h"
#include "RedEngineFL/Memory/RedMemoryPool.h"

class AILowLevel;
class GameObject;
class PblHash;
class UnitController;
class UnitCmdInfo;
namespace AI
{
class EventInfo;
}

// VTABLE: SWBF2 0x007af6a4
class UnitAgent : public Agent
{

    friend class CombatHelper;

public:

    static void* operator new(unsigned int uiSize) { return sMemoryPool.Allocate(uiSize); }
    // SYNTHETIC: SWBF2 0x0065dd30
    // UnitAgent::operator delete
    static void operator delete(void* pBlock) { sMemoryPool.Free(pBlock); }
    static MemoryPool sMemoryPool;

    UnitAgent(UnitController* pCtrl);
    virtual ~UnitAgent();
    virtual void Kill();
    // FUNCTION: SWBF2 0x0065dce0
    // UnitAgent::IsRtti
    // FUNCTION: SWBF2 0x0065dd10
    // UnitAgent::GetDerivedRtti
    // FUNCTION: SWBF2 0x0065dd20
    // UnitAgent::GetDerivedRttiName
    PBL_DECLARE_DERIVED_RTTI(UnitAgent, Agent)
protected:
    virtual void UpdateState(float dt);
public:

    virtual void UpdateStatePostVision(float dt) = 0;
protected:
    virtual void FaceTargetCommon(ControllableHeight eHeight, bool bFlag, const PblVector3& vTarget);
public:
    virtual void FaceTarget();
    virtual void FaceTarget(ControllableHeight eHeight);
    virtual void FaceTarget(ControllableHeight eHeight, const PblVector3& vTarget);
    virtual bool ShouldQueueCurCmd(const UnitCmdInfo& cmd) const;
    virtual float GetUpdateRate() const;
    virtual const char* NFINAL_GetStateString(int iState) const = 0;
    virtual int EventHandler(AI::EventInfo* pInfo);

    const Controllable* GetMe() const
    {
        return mCtrl->mOwner;
    }
    Controllable* GetMe()
    {
        return mCtrl->mOwner;
    }
    AILowLevel& GetLowLevel()
    {
        return mCtrl->mLowLevel;
    }

    bool RunForVehicle(const GameObject* pVehicle) const;

protected:
    bool AreNearbyUnitsInCombat();
    void PlayDamageEventVO(AI::EventInfo* pInfo);
    int DamagedEventPlayerCentricness(AI::EventInfo* pInfo);

    int HandleDamagedEvent(AI::EventInfo* pInfo);

private:
    bool ShouldAgentGetInNearbyVehicle(const GameObject* pVehicle) const;
    int HandleSquadCommandEvent(AI::EventInfo* pInfo);
    int Handle_Vision_CanSeeEnemy(AI::EventInfo* pInfo);
    int Handle_Audible_CanHearEnemy(AI::EventInfo* pInfo);

public:

    UnitController* mCtrl;
    AttackHelper mAttackHelper;
    float mCheckNearbyUnitsTime;
    bool mNearbyUnitsInCombat;
};

 


