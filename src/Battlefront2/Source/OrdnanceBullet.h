#pragma once

#include "PebbleFL/Common/PblHashTable.h"
#include "PebbleFL/Common/PblRtti.h"

#include <stddef.h>

#include "Ordnance.h"
#include "Battlefront2Lod.h"
#include "RedEngineFL/Util/RedSceneObject.h"
#include "PebbleFL/Common/PblHashTable.h"

class RedModel;

// VTABLE: SWBF2 0x007ac6e0
class OrdnanceBulletClass : public OrdnanceClass
{
public:
    virtual ~OrdnanceBulletClass();
    OrdnanceBulletClass(unsigned int uHash);
    OrdnanceBulletClass(OrdnanceBulletClass* pBase, unsigned int uHash);

    virtual OrdnanceClass* Derive(unsigned int uHash);
    virtual Ordnance* Build(const OrdnanceDesc& desc);
    // FUNCTION: SWBF2 0x005f8b30
    // OrdnanceBulletClass::IsRtti
    // FUNCTION: SWBF2 0x005fa870
    // OrdnanceBulletClass::GetDerivedRtti
    // FUNCTION: SWBF2 0x005fa880
    // OrdnanceBulletClass::GetDerivedRttiName
    PBL_DECLARE_DERIVED_RTTI(OrdnanceBulletClass, OrdnanceClass)
    virtual void SetProperty(unsigned int uProperty, const char* pszValue);
    // FUNCTION: SWBF2 0x005f8b60
    virtual float GetVelocity() const
    {
        return mVelocity;
    }
    // FUNCTION: SWBF2 0x005f8b70
    virtual float GetRange() const
    {
        return mVelocity * mLifeSpan;
    }

    const RedModel* mModel;
    bool mStraightBullet;
    bool mFadeColorLight;
    RedLodData mLodData;
    float mVelocity;
    unsigned int mImpactEffectSoft;
    unsigned int mImpactEffectRigid;
    unsigned int mImpactEffectStatic;
    unsigned int mImpactEffectTerrain;
    unsigned int mImpactEffectWater;
    unsigned int mImpactEffectShieldSmall;
    unsigned int mImpactEffectShieldLarge;
    unsigned int mImpactDecalStatic;
    unsigned int mImpactDecalTerrain;
    unsigned int m_uiLightOdfNameHash;
    RedColor m_colorLight;
    RedColor m_endColorLight;
    float m_fRadiusLight;

};

class EntityLight;

// VTABLE: SWBF2 0x007ac710 Thread
// VTABLE: SWBF2 0x007ac750 PblHandled
// VTABLE: SWBF2 0x007ac758
class OrdnanceBullet : public Ordnance, public RedSceneObject
{
public:
    OrdnanceBullet(OrdnanceBulletClass* pClass, const OrdnanceDesc& desc);
    virtual ~OrdnanceBullet();

protected:
    virtual bool Update(float fDeltaTime);

public:
    // FUNCTION: SWBF2 0x005f8ae0
    // OrdnanceBullet::IsRtti
    // FUNCTION: SWBF2 0x005fa820
    // OrdnanceBullet::GetDerivedRtti
    // FUNCTION: SWBF2 0x005fa830
    // OrdnanceBullet::GetDerivedRttiName
    PBL_DECLARE_DERIVED_RTTI(OrdnanceBullet, Ordnance)
    virtual const PblVector3& GetOldPosition() const;
    virtual const PblVector3& GetVelocity() const;
    virtual bool Collide(CollisionObject* pObject, const PblVector3& vPos,
                         const PblVector3& vNormal, int iBodyId);
    virtual void CreateRicochet(const PblVector3& vPos, const PblVector3& vNormal,
                                GameObject* pObject) const;
    virtual void Render(unsigned char ucViewport, float fDist, unsigned int uiFlags);

    PblVector3 mOldPosition;
    PblVector3 mVelocity;
    PblVector3 mFinalCollisionNormal;
    CollisionObject* mFinalCollision;
    PblHandle<GameObject> mFinalCollisionHandle;
    bool mFinalCollisionOnNextUpdate;
    EntityLight* m_pLight;
    float mOffscreenTime;

};

