#pragma once

#include "PebbleFL/Common/PblHashTable.h"
#include "PebbleFL/Common/PblRtti.h"

#include <stddef.h>

#include "Ordnance.h"
#include "Battlefront2Lod.h"
#include "RedEngineFL/Util/RedSceneObject.h"

class Aimer;
class FLEffectClass;

// VTABLE: SWBF2 0x007ac684
class OrdnanceBeamClass : public OrdnanceClass
{
public:
    virtual ~OrdnanceBeamClass();
    OrdnanceBeamClass(unsigned int uHash);

    OrdnanceBeamClass(OrdnanceBeamClass* pBase, unsigned int uHash);

    virtual OrdnanceClass* Derive(unsigned int uHash);
    virtual Ordnance* Build(const OrdnanceDesc& desc);
    virtual bool IsRtti(unsigned int uRtti) const;
    virtual unsigned int GetDerivedRtti() const;
    virtual const char* GetDerivedRttiName() const;
    static const char rttiNameOrdnanceBeamClass[];
    virtual void SetProperty(unsigned int uProperty, const char* pszValue);
    // FUNCTION: SWBF2 0x005f9690
    virtual float GetRange() const
    {
        return mRange;
    }

    float mRange;
    float mBeamWidth;
    unsigned int mBeamTextureId;
    RedColor mBeamColor;
    float mGlowWidth;
    unsigned int mGlowTextureId;
    RedColor mGlowColor;
    bool mPassThrough : 1;
    FLEffectClass* mImpactEffect;

    static PblHash rttiHashOrdnanceBeamClass;
};

class BeamLodData : public RedLodData
{
public:
    BeamLodData() { mLowRezFlags = RedLodData::HAS_LOWREZ | RedLodData::HAS_TRANSPARENT; }
};

// VTABLE: SWBF2 0x007ac5dc
// VTABLE: SWBF2 0x007ac644 Thread
// VTABLE: SWBF2 0x007ac6b4 PblHandled
class OrdnanceBeam : public Ordnance, public RedSceneObject
{
public:
    OrdnanceBeam(OrdnanceBeamClass* pClass, const OrdnanceDesc& desc);
    virtual ~OrdnanceBeam();

    virtual bool Update(float fDeltaTime);
    // FUNCTION: SWBF2 0x005f95f0
    // OrdnanceBeam::IsRtti
    // FUNCTION: SWBF2 0x005f9620
    // OrdnanceBeam::GetDerivedRtti
    // FUNCTION: SWBF2 0x005f9630
    // OrdnanceBeam::GetDerivedRttiName
    PBL_DECLARE_DERIVED_RTTI(OrdnanceBeam, Ordnance)
    virtual void CreateRicochet(const PblVector3& vPos, const PblVector3& vNormal,
                                GameObject* pObject) const;
    virtual void Render(unsigned char ucViewport, float fDist, unsigned int uiFlags);

    void SetDead();

    PblVector3 mDirection;
    float mLength;
    const Aimer* mAttachAimer;
    OrdnanceBeam* m_pRicochet;
    bool mDeadFlag;

};

// FUNCTION: SWBF2 0x005f9640
inline bool OrdnanceBeamClass::IsRtti(unsigned int uRtti) const
{
    if (rttiHashOrdnanceBeamClass == uRtti)
    {
        return true;
    }
    return OrdnanceClass::IsRtti(uRtti);
}

