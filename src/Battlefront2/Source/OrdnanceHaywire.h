#pragma once

#include "PebbleFL/Common/PblHashTable.h"
#include "PebbleFL/Common/PblRtti.h"

#include <stddef.h>

#include "OrdnanceSticky.h"

// VTABLE: SWBF2 0x007acba0
class OrdnanceHaywireClass : public OrdnanceStickyClass
{
public:
    virtual ~OrdnanceHaywireClass();
    OrdnanceHaywireClass(unsigned int uHash);
    OrdnanceHaywireClass(OrdnanceHaywireClass* pBase, unsigned int uHash);

    virtual OrdnanceClass* Derive(unsigned int uHash);
    virtual Ordnance* Build(const OrdnanceDesc& desc);
    // FUNCTION: SWBF2 0x00600de0
    // OrdnanceHaywireClass::IsRtti
    // FUNCTION: SWBF2 0x00600e20
    // OrdnanceHaywireClass::GetDerivedRtti
    // FUNCTION: SWBF2 0x00600e30
    // OrdnanceHaywireClass::GetDerivedRttiName
    PBL_DECLARE_DERIVED_RTTI(OrdnanceHaywireClass, OrdnanceStickyClass)
    virtual void SetProperty(unsigned int uProperty, const char* pszValue);

    float mDisableTime;

};

// VTABLE: SWBF2 0x007acb5c Thread
// VTABLE: SWBF2 0x007acb54 PblHandled
// VTABLE: SWBF2 0x007acbd0
class OrdnanceHaywire : public OrdnanceSticky
{
public:
    OrdnanceHaywire(OrdnanceHaywireClass* pClass, const OrdnanceDesc& desc);
    virtual ~OrdnanceHaywire();
    virtual bool Update(float fDeltaTime);
    // FUNCTION: SWBF2 0x00600d50
    // OrdnanceHaywire::IsRtti
    // FUNCTION: SWBF2 0x00600d90
    // OrdnanceHaywire::GetDerivedRtti
    // FUNCTION: SWBF2 0x00600da0
    // OrdnanceHaywire::GetDerivedRttiName
    PBL_DECLARE_DERIVED_RTTI(OrdnanceHaywire, OrdnanceSticky)

};

