#pragma once

#include "PebbleFL/Common/PblHashTable.h"
#include "PebbleFL/Common/PblRtti.h"

#include <stddef.h>

#include "WeaponDispenser.h"
#include "PebbleFL/Common/PblHashTable.h"

// VTABLE: SWBF2 0x007b1800
class WeaponRemoteClass : public WeaponDispenserClass
{
public:
    WeaponRemoteClass(unsigned int uHash);

    WeaponRemoteClass(WeaponRemoteClass* pBase, unsigned int uHash);

    virtual ~WeaponRemoteClass();

    virtual WeaponDispenserClass* Derive(unsigned int uHash);
    virtual WeaponDispenser* Build(const WeaponDesc& desc);
    // FUNCTION: SWBF2 0x0068ff10
    // WeaponRemoteClass::IsRtti
    // FUNCTION: SWBF2 0x0068ff40
    // WeaponRemoteClass::GetDerivedRtti
    // FUNCTION: SWBF2 0x0068ff50
    // WeaponRemoteClass::GetDerivedRttiName
    PBL_DECLARE_DERIVED_RTTI(WeaponRemoteClass, WeaponDispenserClass)
    virtual void SetProperty(unsigned int uProperty, const char* pszValue);

    bool mSwitchImmediately;
    bool mAllowDestruct;
};

// VTABLE: SWBF2 0x007b1838
class WeaponRemote : public WeaponDispenser
{
public:
    WeaponRemote(const WeaponRemoteClass* pClass, const WeaponDesc& desc);

    virtual bool Update(float fDeltaTime);
    virtual bool IsRtti(unsigned int uRtti) const;

    virtual ~WeaponRemote();
    virtual unsigned int GetDerivedRtti() const;
    virtual const char* GetDerivedRttiName() const;
    virtual bool IsBusy() const;
    virtual void Write(NetPktGroup& group);
    virtual void Read(NetPktGroup& group);

protected:

    virtual bool CheckCharge() const;
    virtual bool CheckFire() const;
    virtual bool FireDone() const;
    virtual Entity* Fire();

public:

    static PblHash rttiHashWeaponRemote;
    static const char rttiNameWeaponRemote[];

    PblHandle<GameObject> mItem;
};

// FUNCTION: SWBF2 0x0068fec0
inline bool WeaponRemote::IsRtti(unsigned int uRtti) const
{
    if (rttiHashWeaponRemote == uRtti)
    {
        return true;
    }
    return WeaponDispenser::IsRtti(uRtti);
}

// FUNCTION: SWBF2 0x0068fef0
inline unsigned int WeaponRemote::GetDerivedRtti() const
{
    return rttiHashWeaponRemote;
}

// FUNCTION: SWBF2 0x0068ff00
inline const char* WeaponRemote::GetDerivedRttiName() const
{
    return "WeaponRemote";
}
