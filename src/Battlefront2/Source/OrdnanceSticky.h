#pragma once

#include "PebbleFL/Common/PblHashTable.h"
#include "PebbleFL/Common/PblRtti.h"

#include <stddef.h>

#include "OrdnanceShell.h"

// VTABLE: SWBF2 0x007ad138
class OrdnanceStickyClass : public OrdnanceShellClass
{
public:
    OrdnanceStickyClass(unsigned int uHash);
    OrdnanceStickyClass(OrdnanceStickyClass* pBase, unsigned int uHash);
    virtual ~OrdnanceStickyClass();

    virtual OrdnanceClass* Derive(unsigned int uHash);
    virtual Ordnance* Build(const OrdnanceDesc& desc);
    // FUNCTION: SWBF2 0x005f8c30
    // OrdnanceStickyClass::IsRtti
    // FUNCTION: SWBF2 0x00605fb0
    // OrdnanceStickyClass::GetDerivedRtti
    // FUNCTION: SWBF2 0x00605fc0
    // OrdnanceStickyClass::GetDerivedRttiName
    PBL_DECLARE_DERIVED_RTTI(OrdnanceStickyClass, OrdnanceShellClass)
    virtual void SetProperty(unsigned int uProperty, const char* pszValue);

    GameSound mSoundCollisionWater;
    GameSound mSoundCollisionLeaf;
    GameSound mSoundCollisionOther;
    bool mStartTimerOnContact;
    bool mAlignVertical;
    static const unsigned int kStickTerrain = 0x80000000;   // beside TargetInfo's types, one bit per HealthType
    int mStickTypes;

};

class StickInfo;

// VTABLE: SWBF2 0x007ad08c
// VTABLE: SWBF2 0x007ad0f4 Thread
// VTABLE: SWBF2 0x007ad168 PblHandled
class OrdnanceSticky : public OrdnanceShell
{
public:
    OrdnanceSticky(OrdnanceStickyClass* pClass, const OrdnanceDesc& desc);
    virtual ~OrdnanceSticky();

    // FUNCTION: SWBF2 0x005f8bf0
    // OrdnanceSticky::IsRtti
    // FUNCTION: SWBF2 0x00605f60
    // OrdnanceSticky::GetDerivedRtti
    // FUNCTION: SWBF2 0x00605f70
    // OrdnanceSticky::GetDerivedRttiName
    PBL_DECLARE_DERIVED_RTTI(OrdnanceSticky, OrdnanceShell)
    virtual bool CheckCollision();
    virtual bool Collide(CollisionObject* pObject, const PblVector3& vPos,
                         const PblVector3& vNormal, int iBodyId);
    virtual void SoundEvent();

    virtual void Render(unsigned char ucViewport, float fDist, unsigned int uiFlags);

    virtual bool Update(float fDeltaTime);

public:
    StickInfo* mStickInfo;
    unsigned char mNumBounces;
    float mLastEventTime;
    float mBeginTime;

};

