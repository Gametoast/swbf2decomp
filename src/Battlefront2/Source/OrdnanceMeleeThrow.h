#pragma once

#include "PebbleFL/Common/PblHashTable.h"
#include "PebbleFL/Common/PblRtti.h"

#include <stddef.h>

#include "Ordnance.h"
#include "Battlefront2Lod.h"
#include "RedEngineFL/Util/RedSceneObject.h"
#include "PebbleFL/Common/PblMatrix.h"

// VTABLE: SWBF2 0x007ace2c
class OrdnanceMeleeThrowClass : public OrdnanceClass
{
public:
    OrdnanceMeleeThrowClass(unsigned int uHash);
    OrdnanceMeleeThrowClass(OrdnanceMeleeThrowClass* pBase, unsigned int uHash);
    virtual ~OrdnanceMeleeThrowClass();

    virtual OrdnanceClass* Derive(unsigned int uHash);
    virtual Ordnance* Build(const OrdnanceDesc& desc);
    // FUNCTION: SWBF2 0x00602220
    // OrdnanceMeleeThrowClass::IsRtti
    // FUNCTION: SWBF2 0x00602250
    // OrdnanceMeleeThrowClass::GetDerivedRtti
    // FUNCTION: SWBF2 0x00602260
    // OrdnanceMeleeThrowClass::GetDerivedRttiName
    PBL_DECLARE_DERIVED_RTTI(OrdnanceMeleeThrowClass, OrdnanceClass)
    virtual void SetProperty(unsigned int uProperty, const char* pszValue);
    // FUNCTION: SWBF2 0x00602270
    virtual float GetVelocity() const
    {
        return mVelocity;
    }
    // FUNCTION: SWBF2 0x00602280
    virtual float GetRange() const
    {
        return mVelocity * mLifeSpan;
    }

    RedLodData mLodData;
    float mVelocity;
    float m_fReturnDuration;
    float m_fOmega;
    float m_fDamageLength;
    float m_fDamageWidth;
    unsigned int m_bDamageLengthFromEdge : 1;
    unsigned int m_bDamageWidthFromEdge : 1;
    unsigned int z_uiReserved0 : 30;
    unsigned int m_uiLightOdfNameHash;
    RedColor m_colorLight;
    float m_fRadiusLight;
    GameSound m_OnSound;

};

class EntityLight;
class WeaponMeleeThrow;

// VTABLE: SWBF2 0x007acd7c PblHandled
// VTABLE: SWBF2 0x007acd84 Thread
// VTABLE: SWBF2 0x007acdc4
class OrdnanceMeleeThrow : public Ordnance, public RedSceneObject
{
public:
    OrdnanceMeleeThrow(OrdnanceMeleeThrowClass* pClass, const OrdnanceDesc& desc);
    virtual ~OrdnanceMeleeThrow();

protected:
    virtual bool Update(float fDeltaTime);

public:
    // FUNCTION: SWBF2 0x00602180
    // OrdnanceMeleeThrow::IsRtti
    // FUNCTION: SWBF2 0x006021b0
    // OrdnanceMeleeThrow::GetDerivedRtti
    // FUNCTION: SWBF2 0x006021c0
    // OrdnanceMeleeThrow::GetDerivedRttiName
    PBL_DECLARE_DERIVED_RTTI(OrdnanceMeleeThrow, Ordnance)
    virtual const PblVector3& GetOldPosition() const;
    virtual const PblVector3& GetVelocity() const;
    virtual bool Collide(CollisionObject* pObject, const PblVector3& vPos,
                         const PblVector3& vNormal, int iBodyId);
    virtual void Render(unsigned char ucViewport, float fDist, unsigned int uiFlags);

protected:
    void ApplyDamage(float fDeltaTime);
    void Return();

public:
    PblHandle<GameObject> m_pOwner;
    WeaponMeleeThrow* m_pWeapon;
    PblMatrix m_mMatrix;
    PblVector3 mOldPosition;
    PblVector3 mVelocity;
    PblVector3 m_vRotationAxis;
    void* m_pDamageData;
    EntityLight* m_pLight;
    bool m_bLastFrameIndicator;
    float mOffscreenTime;
    float m_fReturnTime;

};

