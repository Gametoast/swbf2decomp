#pragma once

#include "PebbleFL/Common/PblHashTable.h"
#include "PebbleFL/Common/PblRtti.h"

#include <stddef.h>

#include "EntityFlyer.h"
#include "GameLoop.h"
#include "GameSound.h"
#include "RedEngineFL/Graphics/RedPose.h"
#include "SoundFL/source/sndsoundparameterized.h"
#include "Zephyr/Source/ZephyrPoseDyn.h"
#include "Zephyr/Source/ZephyrSkeleton.h"
#include "PebbleFL/Common/PblHashTable.h"
#include "RedEngineFL/Memory/RedMemoryPool.h"

class Aimer;
class ControllableDestination;
class EnergyBar;
class MountedTurret;
class Obstacle;
class ParticleEmitterObject;
class PassengerSlot;
class RedPose;
class Weapon;

// VTABLE: SWBF2 0x0079a2b8
class EntityCarrierClass : public EntityFlyerClass
{
public:

    struct CargoInfo
    {
        unsigned int mHash;
        PblVector3 mOffset;
    };

    EntityCarrierClass(unsigned int uHash);
    virtual ~EntityCarrierClass();

    EntityCarrierClass(EntityCarrierClass* pSrc, unsigned int uHash);

    virtual EntityClass* Derive(unsigned int uHash);
    virtual Entity* Build(const EntityDesc& desc);
    // FUNCTION: SWBF2 0x00496f00
    // EntityCarrierClass::IsRtti
    // FUNCTION: SWBF2 0x00496f40
    // EntityCarrierClass::GetDerivedRtti
    // FUNCTION: SWBF2 0x00496f50
    // EntityCarrierClass::GetDerivedRttiName
    PBL_DECLARE_DERIVED_RTTI(EntityCarrierClass, EntityFlyerClass)
    virtual void SetProperty(unsigned int uProperty, const char* pszValue);

    CargoInfo mCargoInfo[4];
    int mCargoCount;
    GameSound mSoundCargoPickup;
    GameSound mSoundCargoDropoff;

};

struct EntityFlyerRuntimeLayout
{
    void* mPrimaryVptr;
    unsigned char _toCollisionVptr[0x08];
    void* mCollisionVptr;
    unsigned char _toVptr94[0x84];
    void* mVptr94;
    unsigned char _toVptr140[0xa8];
    void* mVptr140;
    unsigned char _toVptr200[0xbc];
    void* mVptr200;
    unsigned char _toControllable[0x3c];
    void* mControllableVptr;
    unsigned char _toVptr258[0x14];
    void* mVptr258;
    unsigned char _toPathFollower[0x170];
    void* mPathFollowerVptr;
    unsigned char _toVelocity[0x170];
    PblVector3 mVelocity;
    unsigned char _toState[0x18];
    EntityFlyer::State mState;
    unsigned char _toClass[0xc4];
    EntityFlyerClass* mClass;
    unsigned char _toPose[0x1230];
    unsigned char mPoseAndTail[0x530];
};

// VTABLE: SWBF2 0x0079a168 CollisionObject
// VTABLE: SWBF2 0x0079a1bc Thread
// VTABLE: SWBF2 0x0079a28c PblHandled
// VTABLE: SWBF2 0x0079a294
// VTABLE: SWBF2 0x0079a304 Trackable
// VTABLE: SWBF2 0x0079a34c EntityEx
// VTABLE: SWBF2 0x0079a470 Damageable
// VTABLE: SWBF2 0x0079a49c RedSceneObject
class EntityCarrier : public EntityFlyer
{
public:
    struct CargoSlot
    {
        PblVector3 mOffset;
        PblHandle<GameObject> mObject;

        // FUNCTION: SWBF2 0x00496ed0
        CargoSlot() : mObject() {}

        // FUNCTION: SWBF2 0x00496ef0
        ~CargoSlot() {}
    };

    CargoSlot mCargoArray[4];
    int mCargoCount;

    char _tailPad[0xc];

    static MemoryPool sMemoryPool;

    EntityCarrier(EntityCarrierClass* pClass, const EntityDesc& desc);
    virtual ~EntityCarrier();
    virtual void Kill();
    virtual bool Update(float fDeltaTime);

    static void* operator new(unsigned int uSize);
    // FUNCTION: SWBF2 0x00496e90
    static void operator delete(void* pBlock) { sMemoryPool.Free(pBlock); }

    // FUNCTION: SWBF2 0x00496e20
    // EntityCarrier::IsRtti
    // FUNCTION: SWBF2 0x00496e70
    // EntityCarrier::GetDerivedRtti
    // FUNCTION: SWBF2 0x00496e80
    // EntityCarrier::GetDerivedRttiName
    PBL_DECLARE_DERIVED_RTTI(EntityCarrier, EntityFlyer)

    bool AttachCargo(int, GameObject* pGameObject);
    bool DetachCargo(int iSlot);
    void UpdateLandedHeight();
    void ActivatePhysics();

    EntityCarrierClass* GetClass() const
    {
        return (EntityCarrierClass*)mClass;
    }
};

struct EntityCarrierRuntimeLayout
{
    EntityFlyerRuntimeLayout mFlyer;
    struct CargoSlot
    {
        PblVector3 mOffset;
        void* mObject;
        unsigned int mSavedHandleId;
    } mCargoArray[4];
    int mCargoCount;
    unsigned char _tail[0x0c];
};

// FUNCTION: SWBF2 0x004a9cd0
inline unsigned int EntityFlyer::GetDerivedRtti() const
{
    return rttiHashEntityFlyer;
}

// FUNCTION: SWBF2 0x004a9ce0
inline const char* EntityFlyer::GetDerivedRttiName() const
{
    return "EntityFlyer";
}

// FUNCTION: SWBF2 0x00477680
inline bool EntityFlyer::IsRtti(unsigned int uRtti) const
{
    if (rttiHashEntityFlyer == uRtti)
    {
        return true;
    }
    return GameObject::IsRtti(uRtti);
}
