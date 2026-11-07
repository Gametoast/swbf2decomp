#pragma once

#include "PebbleFL/Common/PblHashTable.h"
#include "PebbleFL/Common/PblRtti.h"

#include <stddef.h>

#include "Weapon.h"
#include "WeaponCannon.h"
#include "PebbleFL/Common/PblHashTable.h"

// VTABLE: SWBF2 0x007b1950
class WeaponRepairClass : public WeaponClass
{
public:
    WeaponRepairClass(unsigned int uHash);
    WeaponRepairClass(WeaponRepairClass* pBase, unsigned int uHash);
    virtual ~WeaponRepairClass();

    virtual WeaponClass* Derive(unsigned int uHash);
    virtual Weapon* Build(const WeaponDesc& desc);
    // FUNCTION: SWBF2 0x006905f0
    // WeaponRepairClass::IsRtti
    // FUNCTION: SWBF2 0x00690620
    // WeaponRepairClass::GetDerivedRtti
    // FUNCTION: SWBF2 0x00690630
    // WeaponRepairClass::GetDerivedRttiName
    PBL_DECLARE_DERIVED_RTTI(WeaponRepairClass, WeaponClass)
    virtual void SetProperty(unsigned int uProperty, const char* pszValue);

    float mShotDelay;
    float mAddHealth[8];
    float mAddAmmo[8];
    float mAddBuild;
    float mAddRebuild;
};

// VTABLE: SWBF2 0x007b1988
class WeaponRepair : public Weapon
{
public:
    WeaponRepair(const WeaponRepairClass* pClass, const WeaponDesc& desc);

    virtual ~WeaponRepair();
    bool Update(float fDeltaTime);
    // FUNCTION: SWBF2 0x006905a0
    // WeaponRepair::IsRtti
    // FUNCTION: SWBF2 0x006905d0
    // WeaponRepair::GetDerivedRtti
    // FUNCTION: SWBF2 0x006905e0
    // WeaponRepair::GetDerivedRttiName
    PBL_DECLARE_DERIVED_RTTI(WeaponRepair, Weapon)
    bool IsLocked() const;
    void Select(unsigned int uFlags, bool bOffhand);
    void Deselect(unsigned int uFlags, bool bOffhand);
    bool IsBusy() const;

protected:
    void EnterFire();
    bool UpdateFire(float fDeltaTime);
    void ExitFire();
    bool CheckFire() const;
    bool FireDone() const;
    bool Fire(float fDeltaTime);

public:

    bool mFired;

    char mUnmodeledTail[0x120 - 0x111];
};
 
 
 
 
 

