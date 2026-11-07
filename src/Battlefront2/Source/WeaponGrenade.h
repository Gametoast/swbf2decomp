#pragma once

#include "PebbleFL/Common/PblHashTable.h"
#include "PebbleFL/Common/PblRtti.h"

#include <stddef.h>

#include "Weapon.h"
#include "WeaponCannon.h"
#include "PebbleFL/Common/PblHashTable.h"

// VTABLE: SWBF2 0x007b0ebc
class WeaponGrenadeClass : public WeaponClass
{
public:
    WeaponGrenadeClass(unsigned int uHash);

    WeaponGrenadeClass(WeaponGrenadeClass* pBase, unsigned int uHash);

    virtual ~WeaponGrenadeClass();

    virtual WeaponClass* Derive(unsigned int uHash);
    virtual Weapon* Build(const WeaponDesc& desc);
    // FUNCTION: SWBF2 0x00684f90
    // WeaponGrenadeClass::IsRtti
    // FUNCTION: SWBF2 0x00684fc0
    // WeaponGrenadeClass::GetDerivedRtti
    // FUNCTION: SWBF2 0x00684fd0
    // WeaponGrenadeClass::GetDerivedRttiName
    PBL_DECLARE_DERIVED_RTTI(WeaponGrenadeClass, WeaponClass)
    virtual void SetProperty(unsigned int uProperty, const char* pszValue);
    virtual bool PostLoadInit();

    float mShotDelay;
    float mInitialSalvoDelay[3];
    float mShotElevate;
    float mMaxPressedTime;
    bool mHideOnFire;
    char mForcedFireAnim;
    PblVector3 m_avFireOffset[3];

};

// VTABLE: SWBF2 0x007b0ef4
class WeaponGrenade : public Weapon
{
public:
    WeaponGrenade(const WeaponGrenadeClass* pClass, const WeaponDesc& desc);

    virtual ~WeaponGrenade();
    // FUNCTION: SWBF2 0x00684f40
    // WeaponGrenade::IsRtti
    // FUNCTION: SWBF2 0x00684f70
    // WeaponGrenade::GetDerivedRtti
    // FUNCTION: SWBF2 0x00684f80
    // WeaponGrenade::GetDerivedRttiName
    PBL_DECLARE_DERIVED_RTTI(WeaponGrenade, Weapon)
    void SignalFire();
    bool IsBusy() const;
    void Write(NetPktGroup& group);
    void Read(NetPktGroup& group);
    void EnterFire();
    bool CheckCharge() const;
    bool CheckFire() const;

protected:
    bool UpdateFire(float fDeltaTime);
    void ExitFire();
    void EnterCharge();
    bool UpdateCharge(float fDeltaTime);
    void ExitCharge();
    void ExitReload();
    void EnterEmpty();
    bool FireDone() const;

    bool Fire();

public:

    bool mNetFiredFlag;
    bool mFired;
    char m_iFirePos;
    float mStrength;

    char mUnmodeledTail[0x120 - 0x118];
};

