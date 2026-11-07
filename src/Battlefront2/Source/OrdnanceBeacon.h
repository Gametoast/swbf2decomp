#pragma once

#include "PebbleFL/Common/PblHashTable.h"
#include "PebbleFL/Common/PblRtti.h"

#include <stddef.h>

#include "OrdnanceSticky.h"

class OrdnanceClass;

// VTABLE: SWBF2 0x007ac588
class OrdnanceBeaconClass : public OrdnanceStickyClass
{
public:
    OrdnanceBeaconClass(unsigned int uHash);
    OrdnanceBeaconClass(OrdnanceBeaconClass* pBase, unsigned int uHash);
    virtual ~OrdnanceBeaconClass();

    virtual OrdnanceClass* Derive(unsigned int uHash);
    virtual Ordnance* Build(const OrdnanceDesc& desc);
    // FUNCTION: SWBF2 0x005f8d00
    // OrdnanceBeaconClass::IsRtti
    // FUNCTION: SWBF2 0x005f8d40
    // OrdnanceBeaconClass::GetDerivedRtti
    // FUNCTION: SWBF2 0x005f8d50
    // OrdnanceBeaconClass::GetDerivedRttiName
    PBL_DECLARE_DERIVED_RTTI(OrdnanceBeaconClass, OrdnanceStickyClass)
    virtual void SetProperty(unsigned int uProperty, const char* pszValue);

    float mAimAzimuth;
    float mAimElevation;
    float mAimDistance;
    int mSalvoCount;
    float mSalvoDelay;
    float mScatterDistance;
    OrdnanceClass* mOrdnanceClass;
};

// VTABLE: SWBF2 0x007ac53c Thread
// VTABLE: SWBF2 0x007ac4d4
// VTABLE: SWBF2 0x007ac580 PblHandled
class OrdnanceBeacon : public OrdnanceSticky
{
public:
    OrdnanceBeacon(OrdnanceBeaconClass* pClass, const OrdnanceDesc& desc);
    virtual ~OrdnanceBeacon();

    // FUNCTION: SWBF2 0x005f8c70
    // OrdnanceBeacon::IsRtti
    // FUNCTION: SWBF2 0x005f8cb0
    // OrdnanceBeacon::GetDerivedRtti
    // FUNCTION: SWBF2 0x005f8cc0
    // OrdnanceBeacon::GetDerivedRttiName
    PBL_DECLARE_DERIVED_RTTI(OrdnanceBeacon, OrdnanceSticky)
    virtual bool Update(float fDeltaTime);

    int mSalvoCount;

};

