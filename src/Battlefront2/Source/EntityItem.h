#pragma once

#include "PebbleFL/Common/PblHashTable.h"
#include "PebbleFL/Common/PblRtti.h"

#include <stddef.h>

#include "GameObject.h"
#include "RedEngineFL/Graphics/RedColor.h"

class FLEffectClass;

// VTABLE: SWBF2 0x0079bd44
class EntityItemClass : public GameObjectClass
{
public:
    EntityItemClass(unsigned int uHash);

    EntityItemClass(EntityItemClass* pBase, unsigned int uHash);
    virtual ~EntityItemClass();
    virtual EntityClass* Derive(unsigned int uHash);

    virtual Entity* Build(const EntityDesc& desc);
    // FUNCTION: SWBF2 0x004cabd0
    // EntityItemClass::IsRtti
    // FUNCTION: SWBF2 0x004cac10
    // EntityItemClass::GetDerivedRtti
    // FUNCTION: SWBF2 0x004cac20
    // EntityItemClass::GetDerivedRttiName
    PBL_DECLARE_DERIVED_RTTI(EntityItemClass, GameObjectClass)
    virtual void SetProperty(unsigned int uProperty, const char* pszValue);

    float mLifeSpan;
    bool mNoLifeSpanDisplay : 1;
    GameSound mCollisionSound;
    GameSound mCollisionFoliageSound;
    GameSound mCollisionWaterSound;
    FLEffectClass* mTrailEffect;
    float mGravity;
    float mRebound;
    float mFriction;
    unsigned int m_uiLightOdfNameHash;
    RedColor m_colorLight;
    float m_fRadiusLight;
};
 
 
 
 
 
 
 
 
 
 
 
 

#include "Thread.h"

class Character;
class CollisionResult;
class EntityLight;
class FLEffectObject;
class NetPktGroup;
class Restrictor;

// VTABLE: SWBF2 0x0079bda8 EntityEx
// VTABLE: SWBF2 0x0079befc CollisionObject
// VTABLE: SWBF2 0x0079bf50 RedSceneObject
// VTABLE: SWBF2 0x0079bed0 Damageable
// VTABLE: SWBF2 0x0079bec8 PblHandled
// VTABLE: SWBF2 0x0079bd90
class EntityItem : public GameObject, public Thread
{
public:
    EntityItem(EntityItemClass* pClass, const EntityDesc& desc);
    virtual ~EntityItem();

    // FUNCTION: SWBF2 0x004caae0
    // EntityItem::IsRtti
    // FUNCTION: SWBF2 0x004cab20
    // EntityItem::GetDerivedRtti
    // FUNCTION: SWBF2 0x004cab30
    // EntityItem::GetDerivedRttiName
    PBL_DECLARE_DERIVED_RTTI(EntityItem, GameObject)
    virtual const PblVector3& GetVelocity() const;
    virtual void SetVelocity(const PblVector3& vVelocity);
    virtual float GetMaxSpeed() const;
    virtual float GetMaxStrafe() const;
    virtual bool IsSuitableEnemy() const;
    virtual void ActivatePhysics();
    virtual void DeactivatePhysics();
    virtual void WriteCreate(NetPktGroup& group);
    virtual void ReadCreate(NetPktGroup& group);

    virtual void SetPosition(const PblVector3& vPosition);

    virtual void SetMatrix(const PblMatrix& matrix);
    virtual bool CollisionCallback(const CollisionResult* pResult, CollisionObject* pOther,
                                   Restrictor* pRestrictor);

    virtual void Render(unsigned char ucLod, float fAlpha, unsigned int uiFlags);

    virtual bool Update(float fDeltaTime);

    Character* GetOwner() const { return mOwner; }
    float GetLifeTimer() const { return mLifeTimer; }
    bool NoDisplayLifeTimer() const;

    PblVector3 mVelocity;
    bool mStationary;
    EntityItemClass* mClass;
    float mLifeTimer;
    Character* mOwner;
    EntityLight* m_pLight;
    PblHandle<FLEffectObject> mTrailEmitterObj;

};

