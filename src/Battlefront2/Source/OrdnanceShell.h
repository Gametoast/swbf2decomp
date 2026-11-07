#pragma once

#include "PebbleFL/Common/PblHashTable.h"
#include "PebbleFL/Common/PblRtti.h"

#include <stddef.h>

#include "OrdnanceBullet.h"
#include "PebbleFL/Common/PblHashTable.h"

// VTABLE: SWBF2 0x007acf88
class OrdnanceShellClass : public OrdnanceBulletClass
{
public:
    virtual ~OrdnanceShellClass();
    OrdnanceShellClass(unsigned int uHash);
    OrdnanceShellClass(OrdnanceShellClass* pBase, unsigned int uHash);

    virtual OrdnanceClass* Derive(unsigned int uHash);
    virtual Ordnance* Build(const OrdnanceDesc& desc);
    // FUNCTION: SWBF2 0x005f8bb0
    // OrdnanceShellClass::IsRtti
    // FUNCTION: SWBF2 0x00605b90
    // OrdnanceShellClass::GetDerivedRtti
    // FUNCTION: SWBF2 0x00605ba0
    // OrdnanceShellClass::GetDerivedRttiName
    PBL_DECLARE_DERIVED_RTTI(OrdnanceShellClass, OrdnanceBulletClass)
    virtual void SetProperty(unsigned int uProperty, const char* pszValue);
    // FUNCTION: SWBF2 0x005f8be0
    virtual float GetGravity() const
    {
        return mGravity;
    }

    float mGravity;

};

// VTABLE: SWBF2 0x007acfb8 PblHandled
// VTABLE: SWBF2 0x007acfc0
// VTABLE: SWBF2 0x007ad028 Thread
class OrdnanceShell : public OrdnanceBullet
{
public:
    OrdnanceShell(OrdnanceShellClass* pClass, const OrdnanceDesc& desc);
    virtual ~OrdnanceShell();

    // FUNCTION: SWBF2 0x005f8b80
    // OrdnanceShell::IsRtti
    // FUNCTION: SWBF2 0x00605b40
    // OrdnanceShell::GetDerivedRtti
    // FUNCTION: SWBF2 0x00605b50
    // OrdnanceShell::GetDerivedRttiName
    PBL_DECLARE_DERIVED_RTTI(OrdnanceShell, OrdnanceBullet)

protected:
    virtual bool Update(float fDeltaTime);

public:
    float mGravity;

};

