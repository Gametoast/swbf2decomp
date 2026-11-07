#pragma once

#include "PebbleFL/Common/PblHashTable.h"
#include "PebbleFL/Common/PblRtti.h"

#include <stddef.h>

#include "OrdnanceBullet.h"

// VTABLE: SWBF2 0x007ac8d4
class OrdnanceFatRayClass : public OrdnanceBulletClass
{
public:
    OrdnanceFatRayClass(unsigned int uHash);
    virtual ~OrdnanceFatRayClass();
    OrdnanceFatRayClass(OrdnanceFatRayClass* pBase, unsigned int uHash);

    virtual OrdnanceClass* Derive(unsigned int uHash);
    virtual Ordnance* Build(const OrdnanceDesc& desc);
    // FUNCTION: SWBF2 0x005fe7b0
    // OrdnanceFatRayClass::IsRtti
    // FUNCTION: SWBF2 0x005fe7e0
    // OrdnanceFatRayClass::GetDerivedRtti
    // FUNCTION: SWBF2 0x005fe7f0
    // OrdnanceFatRayClass::GetDerivedRttiName
    PBL_DECLARE_DERIVED_RTTI(OrdnanceFatRayClass, OrdnanceBulletClass)
    virtual void SetProperty(unsigned int uProperty, const char* pszValue);

    float mMaxDamage;
    float mMinDamage;
    float mDamageDeduction;
    float mMaxPush;
    float mMinPush;
    float mPushDeduction;
    float mRayRadius;

};

// VTABLE: SWBF2 0x007ac82c
// VTABLE: SWBF2 0x007ac894 Thread
// VTABLE: SWBF2 0x007ac904 PblHandled
class OrdnanceFatRay : public OrdnanceBullet
{
public:
    OrdnanceFatRay(OrdnanceFatRayClass* pClass, const OrdnanceDesc& desc);
    virtual ~OrdnanceFatRay();

    // FUNCTION: SWBF2 0x005fe730
    // OrdnanceFatRay::IsRtti
    // FUNCTION: SWBF2 0x005fe760
    // OrdnanceFatRay::GetDerivedRtti
    // FUNCTION: SWBF2 0x005fe770
    // OrdnanceFatRay::GetDerivedRttiName
    PBL_DECLARE_DERIVED_RTTI(OrdnanceFatRay, OrdnanceBullet)

    virtual bool CheckCollision();

    float mDamage;
    float mPush;

};

