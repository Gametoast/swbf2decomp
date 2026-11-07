#pragma once

#include "PebbleFL/Common/PblHashTable.h"
#include "PebbleFL/Common/PblRtti.h"

#include <stddef.h>

#include "Weapon.h"
#include "WeaponCannon.h"
#include "PebbleFL/Common/PblHandle.h"
#include "PebbleFL/Common/PblHashTable.h"

class Entity;
class EntityClass;
class GameObject;
class NetPktGroup;

// VTABLE: SWBF2 0x007b0be4
class WeaponDispenserClass : public WeaponClass
{
public:
    virtual ~WeaponDispenserClass();
    WeaponDispenserClass(unsigned int uHash);
    WeaponDispenserClass(WeaponDispenserClass* pBase, unsigned int uHash);

    virtual WeaponClass* Derive(unsigned int uHash);
    virtual Weapon* Build(const WeaponDesc& desc);
    // FUNCTION: SWBF2 0x00682430
    // WeaponDispenserClass::IsRtti
    // FUNCTION: SWBF2 0x006836d0
    // WeaponDispenserClass::GetDerivedRtti
    // FUNCTION: SWBF2 0x006836e0
    // WeaponDispenserClass::GetDerivedRttiName
    PBL_DECLARE_DERIVED_RTTI(WeaponDispenserClass, WeaponClass)
    virtual void SetProperty(unsigned int uProperty, const char* pszValue);
    virtual void PostReadInit();
    virtual bool PostLoadInit();

    EntityClass* mDispenseClass;
    float mVelocity;
    float mShotDelay;
    float mInitialSalvoDelay[2];
    float mMaxPressedTime;
    float mMinStrength;
    float mMaxStrength;
    int mMaxItems;
    bool mHideOnFire;
    bool mHideOnEmpty;
    PblVector3 mFirePointExtraOffset;
    PblHandle<GameObject>* mTrackItemPool;
    PblHandle<GameObject>* mTrackItemArray[600];
    int mTrackItemCount[600];
    PblVector3 m_avFireOffset[3];
};

// VTABLE: SWBF2 0x007b0c1c
class WeaponDispenser : public Weapon
{
public:
    WeaponDispenser(const WeaponDispenserClass* pClass, const WeaponDesc& desc);
    virtual ~WeaponDispenser();

    virtual bool Update(float fDeltaTime);

    virtual void Write(NetPktGroup& group);
    virtual void Read(NetPktGroup& group);

    // FUNCTION: SWBF2 0x00682400
    // WeaponDispenser::IsRtti
    // FUNCTION: SWBF2 0x006836b0
    // WeaponDispenser::GetDerivedRtti
    // FUNCTION: SWBF2 0x006836c0
    // WeaponDispenser::GetDerivedRttiName
    PBL_DECLARE_DERIVED_RTTI(WeaponDispenser, Weapon)
    bool IsBusy() const;
    void SignalFire();
    void EnterFire();
    void ExitFire();
    void EnterCharge();
    bool UpdateCharge(float fDeltaTime);
    void ExitCharge();
    void ExitReload();
    void EnterEmpty();
    void ExitEmpty();
    bool CheckCharge() const;
    bool CheckFire() const;
    bool FireDone() const;
    int* GetItemCount() const;
    const GameObject* GetItem(int iIndex) const;

protected:

    virtual bool UpdateFire(float fDeltaTime);
    virtual Entity* Fire();

public:

    bool mNetFiredFlag;
    bool mFired;
    float mStrength;
    PblHandle<GameObject>** mItemArray;
    int* mItemCount;
};

