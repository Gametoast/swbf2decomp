#pragma once

#include "PebbleFL/Common/PblHashTable.h"
#include "PebbleFL/Common/PblRtti.h"

#include "WeaponCannon.h"
#include "WeaponCannon.h"
#include "PebbleFL/Common/PblHashTable.h"

// VTABLE: SWBF2 0x007b0d68
class WeaponGrapplingHookClass : public WeaponCannonClass
{
public:
    WeaponGrapplingHookClass(unsigned int uHash);
    WeaponGrapplingHookClass(WeaponGrapplingHookClass* pBase, unsigned int uHash);
    virtual ~WeaponGrapplingHookClass();

    virtual WeaponClass* Derive(unsigned int uHash);
    virtual Weapon* Build(const WeaponDesc& desc);
    // FUNCTION: SWBF2 0x00684d10
    // WeaponGrapplingHookClass::IsRtti
    // FUNCTION: SWBF2 0x00684d40
    // WeaponGrapplingHookClass::GetDerivedRtti
    // FUNCTION: SWBF2 0x00684d50
    // WeaponGrapplingHookClass::GetDerivedRttiName
    PBL_DECLARE_DERIVED_RTTI(WeaponGrapplingHookClass, WeaponCannonClass)
};
 class Ordnance;
class NetPktGroup;

// VTABLE: SWBF2 0x007b0da0
class WeaponGrapplingHook : public WeaponCannon
{
public:
    WeaponGrapplingHook(const WeaponGrapplingHookClass* pClass, const WeaponDesc& desc);
    virtual ~WeaponGrapplingHook();

    // FUNCTION: SWBF2 0x00684c90
    // WeaponGrapplingHook::IsRtti
    // FUNCTION: SWBF2 0x00684cc0
    // WeaponGrapplingHook::GetDerivedRtti
    // FUNCTION: SWBF2 0x00684cd0
    // WeaponGrapplingHook::GetDerivedRttiName
    PBL_DECLARE_DERIVED_RTTI(WeaponGrapplingHook, WeaponCannon)
    virtual void Write(NetPktGroup& group);
    virtual void Read(NetPktGroup& group);

protected:
    virtual bool CheckFire() const;

public:
    virtual void SetOrdnance(Ordnance* pOrdnance);

    bool mCableOut;
};

