#pragma once

#include "PebbleFL/Common/PblHashTable.h"
#include "PebbleFL/Common/PblRtti.h"

#include <stddef.h>

#include "OrdnanceBullet.h"

// VTABLE: SWBF2 0x007acd0c
class OrdnanceLaserClass : public OrdnanceBulletClass
{
public:
    OrdnanceLaserClass(unsigned int uHash);
    virtual ~OrdnanceLaserClass();
    OrdnanceLaserClass(OrdnanceLaserClass* pBase, unsigned int uHash);

    virtual OrdnanceClass* Derive(unsigned int uHash);
    virtual Ordnance* Build(const OrdnanceDesc& desc);
    // FUNCTION: SWBF2 0x00601180
    // OrdnanceLaserClass::IsRtti
    // FUNCTION: SWBF2 0x006011b0
    // OrdnanceLaserClass::GetDerivedRtti
    // FUNCTION: SWBF2 0x006011c0
    // OrdnanceLaserClass::GetDerivedRttiName
    PBL_DECLARE_DERIVED_RTTI(OrdnanceLaserClass, OrdnanceBulletClass)
    virtual void SetProperty(unsigned int uProperty, const char* pszValue);

    float mLaserWidth;
    float mLaserLength;
    unsigned int mTextureId;
    RedColor mGlowColor;
    RedColor mEndGlowColor;
    float mGlowLength;
    float mBlurLength;
    float mFadeOutSpeed;

};

// VTABLE: SWBF2 0x007acc38 PblHandled
// VTABLE: SWBF2 0x007acc40
// VTABLE: SWBF2 0x007acccc Thread
class OrdnanceLaser : public OrdnanceBullet
{
public:
    OrdnanceLaser(OrdnanceLaserClass* pClass, const OrdnanceDesc& desc);
    virtual ~OrdnanceLaser();

protected:
    virtual bool Update(float fDeltaTime);

public:
    // FUNCTION: SWBF2 0x00601100
    // OrdnanceLaser::IsRtti
    // FUNCTION: SWBF2 0x00601130
    // OrdnanceLaser::GetDerivedRtti
    // FUNCTION: SWBF2 0x00601140
    // OrdnanceLaser::GetDerivedRttiName
    PBL_DECLARE_DERIVED_RTTI(OrdnanceLaser, OrdnanceBullet)
    virtual void KillOrdnance();
    virtual void Render(unsigned char ucViewport, float fDist, unsigned int uiFlags);

    PblVector3 mStartPos;
    float mGrowLength;
    float mFadeOutTime;
    bool mDeadFlag;
    float mMaxLength;

};

