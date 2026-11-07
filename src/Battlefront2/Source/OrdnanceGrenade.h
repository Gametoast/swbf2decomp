#pragma once

#include "PebbleFL/Common/PblHashTable.h"
#include "PebbleFL/Common/PblRtti.h"

#include <stddef.h>

#include "OrdnanceShell.h"

// VTABLE: SWBF2 0x007aca48
class OrdnanceGrenadeClass : public OrdnanceShellClass
{
public:
    virtual ~OrdnanceGrenadeClass();
    OrdnanceGrenadeClass(unsigned int uHash);
    OrdnanceGrenadeClass(OrdnanceGrenadeClass* pBase, unsigned int uHash);

    virtual OrdnanceClass* Derive(unsigned int uHash);
    virtual Ordnance* Build(const OrdnanceDesc& desc);
    // FUNCTION: SWBF2 0x006000c0
    // OrdnanceGrenadeClass::IsRtti
    // FUNCTION: SWBF2 0x00600100
    // OrdnanceGrenadeClass::GetDerivedRtti
    // FUNCTION: SWBF2 0x00600110
    // OrdnanceGrenadeClass::GetDerivedRttiName
    PBL_DECLARE_DERIVED_RTTI(OrdnanceGrenadeClass, OrdnanceShellClass)
    virtual void SetProperty(unsigned int uProperty, const char* pszValue);

    float mRebound;
    float mFriction;
    GameSound mSoundCollisionLeaf;
    GameSound mSoundCollisionOther;
    unsigned int mImpactEffectWater;
    bool mStartTimerOnContact;
    bool mAlignVertical;

};

// VTABLE: SWBF2 0x007aca78 PblHandled
// VTABLE: SWBF2 0x007aca80
// VTABLE: SWBF2 0x007acae8 Thread
class OrdnanceGrenade : public OrdnanceShell
{
public:
    OrdnanceGrenade(OrdnanceGrenadeClass* pClass, const OrdnanceDesc& desc);
    virtual ~OrdnanceGrenade();

    virtual bool Update(float fDeltaTime);
    // FUNCTION: SWBF2 0x00600030
    // OrdnanceGrenade::IsRtti
    // FUNCTION: SWBF2 0x00600070
    // OrdnanceGrenade::GetDerivedRtti
    // FUNCTION: SWBF2 0x00600080
    // OrdnanceGrenade::GetDerivedRttiName
    PBL_DECLARE_DERIVED_RTTI(OrdnanceGrenade, OrdnanceShell)
    virtual bool CheckCollision();
    virtual bool Collide(CollisionObject* pObject, const PblVector3& vPos,
                         const PblVector3& vNormal, int iBodyId);
    virtual void Render(unsigned char ucViewport, float fDist, unsigned int uiFlags);
    virtual void SoundEvent();

    int mNumBounces;
    float mBeginTime;
    float mLastEventTime;

};

