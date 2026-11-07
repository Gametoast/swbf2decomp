#pragma once

#include "PebbleFL/Common/PblHashTable.h"
#include "PebbleFL/Common/PblRtti.h"

#include <stddef.h>

#include "EntityBuildingArmedDynamic.h"
#include "GameSound.h"
#include "RedEngineFL/Memory/RedMemoryPool.h"

class Character;

// VTABLE: SWBF2 0x0079c66c
class EntityPortableTurretClass : public EntityBuildingArmedDynamicClass
{
public:
    EntityPortableTurretClass(unsigned int uHash);

    EntityPortableTurretClass(EntityPortableTurretClass* pBase, unsigned int uHash);

    virtual ~EntityPortableTurretClass();

    virtual EntityClass* Derive(unsigned int uHash);
    virtual Entity* Build(const EntityDesc& desc);
    // FUNCTION: SWBF2 0x004d7c80
    // EntityPortableTurretClass::IsRtti
    // FUNCTION: SWBF2 0x004d7cd0
    // EntityPortableTurretClass::GetDerivedRtti
    // FUNCTION: SWBF2 0x004d7ce0
    // EntityPortableTurretClass::GetDerivedRttiName
    PBL_DECLARE_DERIVED_RTTI(EntityPortableTurretClass, EntityBuildingArmedDynamicClass)
    virtual void SetProperty(unsigned int uProperty, const char* pszValue);

    GameSound mCollisionSound;
    GameSound mCollisionFoliageSound;
    GameSound mCollisionWaterSound;
    float mLifeTime;
    float mGravity;
    float mRebound;
    float mFriction;

};

class CollisionResult;
class Restrictor;

// VTABLE: SWBF2 0x0079c4b8
// VTABLE: SWBF2 0x0079c4d0 RedSceneObject
// VTABLE: SWBF2 0x0079c538 EntityEx
// VTABLE: SWBF2 0x0079c6b8 PblHandled
// VTABLE: SWBF2 0x0079c6c0 Damageable
// VTABLE: SWBF2 0x0079c6ec CollisionObject
class EntityPortableTurret : public EntityBuildingArmedDynamic
{
public:
    static void SetupMemoryPool(unsigned int uCount, unsigned int uSize, unsigned int uGrow) { sMemoryPool.Setup(uCount, uSize, uGrow); }

    EntityPortableTurret(EntityPortableTurretClass* pClass, const EntityDesc& desc);

    // FUNCTION: SWBF2 0x004d7b90
    // EntityPortableTurret::IsRtti
    // FUNCTION: SWBF2 0x004d7bf0
    // EntityPortableTurret::GetDerivedRtti
    // FUNCTION: SWBF2 0x004d7c00
    // EntityPortableTurret::GetDerivedRttiName
    PBL_DECLARE_DERIVED_RTTI(EntityPortableTurret, EntityBuildingArmedDynamic)
    // FUNCTION: SWBF2 0x004d7c20
    virtual const PblVector3& GetVelocity() const
    {
        return mVelocity;
    }
    // FUNCTION: SWBF2 0x004d7c30
    virtual void SetVelocity(const PblVector3& vVelocity)
    {
        mVelocity = vVelocity;
    }
    virtual void RecalculateAABB();
    virtual bool CollisionCallback(const CollisionResult* pResult, CollisionObject* pOther,
                                   Restrictor* pRestrictor);
    virtual bool Update(float fDeltaTime);

    static void* operator new(unsigned int uSize) { return sMemoryPool.Allocate(uSize); }
    // SYNTHETIC: SWBF2 0x004d7c10
    // EntityPortableTurret::operator delete
    static void operator delete(void* pBlock) { sMemoryPool.Free(pBlock); }

    static MemoryPool sMemoryPool;

    EntityPortableTurretClass* mClass;
    float mLifeTimer;
    PblVector3 mVelocity;
    Character* mOwnerChar;
    bool mStationary;
    bool mBecomeStationaryNow;

    char _tailPad[6];
};


