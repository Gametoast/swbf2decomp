#pragma once

#include "PebbleFL/Common/PblHashTable.h"
#include "PebbleFL/Common/PblRtti.h"

#include <stddef.h>

#include "WeaponCannon.h"
#include "WeaponCannon.h"
#include "PebbleFL/Common/PblHashTable.h"

#include "PebbleFL/Common/PblHandle.h"

class GameObject;
class WeaponLauncherClass;

// VTABLE: SWBF2 0x007b12a4
class WeaponLauncher : public WeaponCannon
{
public:
    WeaponLauncher(const WeaponLauncherClass* pClass, const WeaponDesc& desc);

    virtual ~WeaponLauncher();
    // FUNCTION: SWBF2 0x006872a0
    // WeaponLauncher::IsRtti
    // FUNCTION: SWBF2 0x006872d0
    // WeaponLauncher::GetDerivedRtti
    // FUNCTION: SWBF2 0x006872e0
    // WeaponLauncher::GetDerivedRttiName
    PBL_DECLARE_DERIVED_RTTI(WeaponLauncher, Weapon)
    // FUNCTION: SWBF2 0x006872f0
    bool IsLocked() const { return mLocked; }
    // FUNCTION: SWBF2 0x00687300
    const GameObject* GetLocked() const { return const_cast<WeaponLauncher*>(this)->mCurTarget; }
    // FUNCTION: SWBF2 0x00687330
    int GetLockedTargetBodyId() const { return mCurTargetBodyID; }
    void Write(NetPktGroup& group);
    void Read(NetPktGroup& group);

protected:
    void EnterIdle();
    void ExitFire();
    void EnterCharge();
    bool UpdateCharge(float fDeltaTime);
    void ExitCharge();
    void EnterEmpty();
    bool CheckCharge() const;
    bool CheckFire() const;

    void SetupTarget(const GameObject** ppTarget, int* pBodyId) const;

public:

    PblHandle<const GameObject> mCurTarget;
    int mCurTargetBodyID;
    bool mLocked;
    float mTrackingSoundInterval;
    float mTrackingSoundTimer;

    char mUnmodeledTail[8];
};

// VTABLE: SWBF2 0x007b139c
class WeaponLauncherClass : public WeaponCannonClass
{
public:
    WeaponLauncherClass(unsigned int uHash);
    WeaponLauncherClass(WeaponLauncherClass* pBase, unsigned int uHash);
    virtual ~WeaponLauncherClass();

    virtual WeaponClass* Derive(unsigned int uHash);

    virtual WeaponLauncher* Build(const WeaponDesc& desc);
    // FUNCTION: SWBF2 0x00687340
    // WeaponLauncherClass::IsRtti
    // FUNCTION: SWBF2 0x00687370
    // WeaponLauncherClass::GetDerivedRtti
    // FUNCTION: SWBF2 0x00687380
    // WeaponLauncherClass::GetDerivedRttiName
    PBL_DECLARE_DERIVED_RTTI(WeaponLauncherClass, WeaponCannonClass)
    virtual void SetProperty(unsigned int uProperty, const char* pszValue);

    float mLockTime;
    bool mClearLockOnFire;
    bool mHasTrackingAmmo;
};

