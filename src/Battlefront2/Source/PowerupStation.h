#pragma once

#include "PebbleFL/Common/PblHashTable.h"
#include "PebbleFL/Common/PblRtti.h"

#include <stddef.h>

#include "EntityBuilding.h"
#include "GameSound.h"
#include "Thread.h"
#include "PebbleFL/Common/PblMatrix.h"
#include "RedEngineFL/Graphics/RedPose.h"

// VTABLE: SWBF2 0x007adb6c
class PowerupStationClass : public EntityBuildingClass
{
public:
    PowerupStationClass(unsigned int uHash);
    PowerupStationClass(PowerupStationClass* pBase, unsigned int uHash);
    virtual ~PowerupStationClass();

    virtual EntityClass* Derive(unsigned int uHash);
    virtual Entity* Build(const EntityDesc& desc);
    // FUNCTION: SWBF2 0x006245a0
    // PowerupStationClass::IsRtti
    // FUNCTION: SWBF2 0x006245e0
    // PowerupStationClass::GetDerivedRtti
    // FUNCTION: SWBF2 0x006245f0
    // PowerupStationClass::GetDerivedRttiName
    PBL_DECLARE_DERIVED_RTTI(PowerupStationClass, EntityBuildingClass)
    virtual void SetProperty(unsigned int uProperty, const char* pszValue);

    float mPowerupDelay;
    float mAddHealth[8];
    float mAddAmmo[8];
    float mAddEnergy[8];
    float mBuffOffenseTimer;
    float mBuffOffenseMult;
    float mBuffDefenseTimer;
    float mBuffDefenseMult;
    float mBuffHealthTimer;
    float mBuffHealthRate;
    float mDebuffDamageTimer;
    float mDebuffDamageRate;
    float mIdleRotateSpeed;
    float mIdleWaitTime;
    PblMatrix mBaseWobbleMatrix;
    unsigned int mIdleWobbleHash;
    unsigned int mIdleWobbleLeftFootHash;
    unsigned int mIdleWobbleRightFootHash;
    float mIdleWobbleFactor;
    unsigned int mActiveRotateHash[4];
    unsigned int mActiveSpinHash;
    GameSound mPowerupSound;
    GameSound mAmbientSound;
};

class RedRegion;

// VTABLE: SWBF2 0x007adbb8 CollisionObject
// VTABLE: SWBF2 0x007adc0c EntityEx
// VTABLE: SWBF2 0x007add34 RedSceneObject
// VTABLE: SWBF2 0x007add9c
// VTABLE: SWBF2 0x007addb4 PblHandled
// VTABLE: SWBF2 0x007addbc Damageable
class PowerupStation : public EntityBuilding, public Thread
{
public:
    enum PowerupStationState
    {
        PS_IDLE = 0,
        PS_WAIT = 1,
        PS_FACE = 2,
        PS_APPLY = 3,
    };

    PowerupStation(PowerupStationClass* pClass, const EntityDesc& desc);
    virtual ~PowerupStation();
    virtual void SetProperty(unsigned int uProperty, const char* pszValue);
    virtual bool Update(float fDeltaTime);

    // FUNCTION: SWBF2 0x006244f0
    // PowerupStation::IsRtti
    // FUNCTION: SWBF2 0x00624540
    // PowerupStation::GetDerivedRtti
    // FUNCTION: SWBF2 0x00624550
    // PowerupStation::GetDerivedRttiName
    PBL_DECLARE_DERIVED_RTTI(PowerupStation, EntityBuilding)
    EntityType GetEntityType() const;
    bool AddToTeamManager() const;
    virtual void WriteObject(NetPktGroup& group, NetObjState* pState);
    virtual void ReadObject(NetPktGroup& group, NetObjState* pState);
    virtual GameObject* AddToClient();
    const PblMatrix* GetRenderMatrix() const;
    const RedPose* GetRenderPose() const;

    int GetObjectsInRegion(RedRegion* pRegion, GameObject** const ppObjects, int iMaxObjects) const;
    bool IsInsideRegion(RedRegion* pRegion, const GameObject* pObject) const;

    PowerupStationClass* mClass;
    RedRegion* mEffectRegion;
    float mPowerupTimer;
    PowerupStationState mPowerupState;
    RedPose mPose;
    float mRotateWaitTime;
    char mRotateDir;
    float mIdleWobblePeriodicity;
    PblMatrix mRenderMatrix;
    PblMatrix mProceduralMatrix[5];
    GameSoundControllable mAmbientSound;
};

