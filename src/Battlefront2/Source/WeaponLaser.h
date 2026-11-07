#pragma once

#include "PebbleFL/Common/PblHashTable.h"

#include <stddef.h>

#include "Weapon.h"
#include "PebbleFL/Common/PblRtti.h"
#include "PebbleFL/Common/PblHashTable.h"

// VTABLE: SWBF2 0x007b1158
class WeaponLaserClass : public WeaponClass
{
public:
    WeaponLaserClass(unsigned int uHash);
    WeaponLaserClass(WeaponLaserClass* pBase, unsigned int uHash);
    virtual ~WeaponLaserClass();

    virtual WeaponClass* Derive(unsigned int uHash);
    virtual Weapon* Build(const WeaponDesc& desc);
    // FUNCTION: SWBF2 0x00686a90
    // WeaponLaserClass::IsRtti
    // FUNCTION: SWBF2 0x00686ac0
    // WeaponLaserClass::GetDerivedRtti
    // FUNCTION: SWBF2 0x00686ad0
    // WeaponLaserClass::GetDerivedRttiName
    PBL_DECLARE_DERIVED_RTTI(WeaponLaserClass, WeaponClass)
    virtual void SetProperty(unsigned int uProperty, const char* pszValue);

    float mShotDelay;
    float mSalvoTime;
};
  
 
 

#include "WeaponCannon.h"

// VTABLE: SWBF2 0x007b1190
class WeaponLaser : public Weapon
{
public:
    WeaponLaser(const WeaponLaserClass* pClass, const WeaponDesc& desc);
    virtual ~WeaponLaser();

    // FUNCTION: SWBF2 0x00686a10
    // WeaponLaser::IsRtti
    // FUNCTION: SWBF2 0x00686a40
    // WeaponLaser::GetDerivedRtti
    // FUNCTION: SWBF2 0x00686a50
    // WeaponLaser::GetDerivedRttiName
    PBL_DECLARE_DERIVED_RTTI(WeaponLaser, Weapon)
    virtual bool IsBusy() const;
    virtual void Write(NetPktGroup& group);
    virtual void Read(NetPktGroup& group);

protected:
    virtual void EnterFire();
    virtual bool UpdateFire(float fDeltaTime);
    virtual void ExitFire();
    virtual void EnterEmpty();
    virtual bool CheckFire() const;
    virtual bool FireDone() const;

    bool Fire(float fTime);

public:

    float mSalvoTimer;
};
  
 

