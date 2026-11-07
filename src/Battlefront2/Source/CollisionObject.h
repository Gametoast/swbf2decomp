#pragma once

#include <stddef.h>

#include "PebbleFL/Common/PblVector.h"

#include "PebbleFL/Common/PblList.h"
#include "TreeGrid.h"
class CollisionModel;
class CollisionObject;
class CollisionOptimizationInfo;
class CollisionResult;
class Entity;
class FoleyFX;
class FoleyFXCollideeClass;
class FoleyFXColliderClass;
class FoleyFXGroup;
class GameObject;
class PblMatrix;
class PblVector3;
class RedPose;
class Restrictor;
class MemoryPool;
class TreeGridObject;
class TreeGridStack;

enum CollisionObjectType
{
    COLL_UNKNOWN = 0,
    COLL_ITEM = 1,
    COLL_SOFT = 2,
    COLL_ASTEROID = 4,
    COLL_RIGID = 8,
    COLL_STATIC = 16,
    COLL_ATTACHED = 32,
    COLL_SPECIAL = 64,
    COLL_TERRAIN = 128,
    COLL_WATER = 256,
    COLL_LEAFPATCH = 512,
    COLL_ORDNANCE = 1024,
    COLL_TOWCABLE = 2048,
    COLL_SHIELD = 4096,
    COLL_TARGETABLE = 8192,
    COLL_HOLE = 16384
};

class FoleyFXCollidee
{
public:

    FoleyFXCollidee(FoleyFXCollideeClass* pClass);

    FoleyFXCollideeClass* GetFoleyFXCollideeClass() const;

    FoleyFXCollideeClass* mCollideeClass;
};

class FoleyFXCollider
{
public:

    FoleyFXCollider(FoleyFXColliderClass* pClass);

    FoleyFX* GetFoleyFX(unsigned int uRtti) const;

    FoleyFXColliderClass* GetFoleyFXColliderClass() const;
    bool CollisionCallback(CollisionObjectType eType,
                           FoleyFXCollidee* const pCollidee,
                           const PblVector3* const pPos);

    void SetupFoleyFX(FoleyFXGroup* const pGroup);

    FoleyFXColliderClass* mColliderClass;
    FoleyFX* mFoleyFX;
    FoleyFXGroup* mFoleyFXGroup;
};


 

// VTABLE: SWBF2 0x00797460
class CollisionObject : public TreeGridObject, public FoleyFXCollider, public FoleyFXCollidee
{
public:
    static const int kNumLastCollided = 3;

    virtual ~CollisionObject();
    virtual const PblVector3 & GetPosition() const = 0;
    // FUNCTION: SWBF2 0x00463a70
    virtual void SetPosition(const PblVector3 &)
    {
    }
    // FUNCTION: SWBF2 0x00454dd0
    virtual void SetMatrix(const PblMatrix &)
    {
    }
    // FUNCTION: SWBF2 0x00443fc0
    virtual bool CanProcedurallyAnimate()
    {
        return true;
    }
    virtual float GetSmallestCollisionDim() const;
    virtual bool CollisionCallback(const CollisionResult *, CollisionObject *, Restrictor *);
    virtual float RayHit(const PblVector3 &, const PblVector3 &, float, PblVector3 *, int *, bool, float);
    virtual bool RayTest(const PblVector3 &, const PblVector3 &, float);
    virtual bool CheckForCollision(CollisionObject *, Restrictor *, CollisionOptimizationInfo *);
    virtual void SetTerrainHeight(int, float);
    virtual Entity * GetEntity();
    virtual const Entity * GetEntity() const;
    virtual GameObject * GetGameObject();
    virtual const GameObject * GetGameObject() const;
    // FUNCTION: SWBF2 0x00443fe0
    virtual bool CollideTrisWithAllPrimitives() const
    {
        return true;
    }
    virtual const PblMatrix & GetMatrix() const = 0;
    virtual const RedPose * GetPose() const;
    // FUNCTION: SWBF2 0x00443ff0
    virtual bool IsAChunk() const
    {
        return false;
    }

    virtual void RecalculateAABB();

    CollisionObject(CollisionObjectType eType, const CollisionModel* pModel,
                    FoleyFXColliderClass* const pColliderClass,
                    FoleyFXCollideeClass* const pCollideeClass);

    void AddStaticToCollisionManager();

    void AddSoftToCollisionManager();

    void AddRigidToCollisionManager();

    void AddItemToCollisionManager();
    void AddAsteroidToCollisionManager();
    void AddAttachedToCollisionManager();
    void AddShieldToCollisionManager();

    void SetMass(float fMass) { mMass = fMass; }

    float GetMass() const { return mMass; }
    void RemoveFromCollisionManager();
    bool IsInCollisionList();

    void SeparateObject(const PblVector3& vNormal, float fOverlap);
    void RestrictObject(const PblVector3& vNormal, float fOverlap, Restrictor* pRestrictor);

    void SetCollisionModel(CollisionModel* pModel);

    void PlayImpactEffects(const PblVector3* pPos, const PblVector3* pVel) const;

    void SetCollisionType(CollisionObjectType eType)
    {
        SetObjectData(0, eType);
    }
    CollisionObjectType GetCollisionType() const;

    // FUNCTION: SWBF2 0x0045db10
    const CollisionModel* GetCollisionModel() const { return mCollisionModel; }

    PblList<CollisionObject>::Node mCollisionObjectNode;
    CollisionModel* mCollisionModel;
    CollisionObject* mParent;
    float mMass;
    PblBox mAABB;
    bool mProcedurallyAnimated : 1;
    bool mLastCollidedWithNonTerrain : 1;
    PblVector3 mLastPosition;
    CollisionObject* mLastCollidedObjs[kNumLastCollided];

    static bool sCollidingWithMesh;
    static bool sCollideTrisWithAllPrimitives;
};
 
 
 
 
 

// FUNCTION: SWBF2 0x0043ec10
inline CollisionObjectType CollisionObject::GetCollisionType() const
{
    const CollisionObject* pObject = (const CollisionObject*)this;
    return (CollisionObjectType)pObject->GetObjectData(0);
}
