#pragma once

#include "PebbleFL/Common/PblHashTable.h"

#include "Weapon.h"
#include "PebbleFL/Common/PblRtti.h"
#include "PebbleFL/Common/PblHashTable.h"

// VTABLE: SWBF2 0x007b0430
class WeaponBinocularsClass : public WeaponClass
{
public:
    WeaponBinocularsClass(unsigned int uHash);
    WeaponBinocularsClass(WeaponBinocularsClass* pBase, unsigned int uHash);
    virtual ~WeaponBinocularsClass();

    virtual WeaponClass* Derive(unsigned int uHash);
    virtual Weapon* Build(const WeaponDesc& desc);
    // FUNCTION: SWBF2 0x0067e170
    // WeaponBinocularsClass::IsRtti
    // FUNCTION: SWBF2 0x0067e1a0
    // WeaponBinocularsClass::GetDerivedRtti
    // FUNCTION: SWBF2 0x0067e1b0
    // WeaponBinocularsClass::GetDerivedRttiName
    PBL_DECLARE_DERIVED_RTTI(WeaponBinocularsClass, WeaponClass)
    virtual void SetProperty(unsigned int uProperty, const char* pszValue);
};
  

#include "WeaponCannon.h"

// VTABLE: SWBF2 0x007b0468
class WeaponBinoculars : public Weapon
{
public:
    WeaponBinoculars(const WeaponBinocularsClass* pClass, const WeaponDesc& desc);
    virtual ~WeaponBinoculars();
    // FUNCTION: SWBF2 0x0067e120
    // WeaponBinoculars::IsRtti
    // FUNCTION: SWBF2 0x0067e150
    // WeaponBinoculars::GetDerivedRtti
    // FUNCTION: SWBF2 0x0067e160
    // WeaponBinoculars::GetDerivedRttiName
    PBL_DECLARE_DERIVED_RTTI(WeaponBinoculars, Weapon)

};
  

