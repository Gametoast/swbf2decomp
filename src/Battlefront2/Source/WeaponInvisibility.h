#pragma once

#include "PebbleFL/Common/PblHashTable.h"
#include "PebbleFL/Common/PblRtti.h"

#include <stddef.h>

#include "Weapon.h"
#include "WeaponCannon.h"
#include "PebbleFL/Common/PblHashTable.h"

// VTABLE: SWBF2 0x007b1010
class WeaponInvisibilityClass : public WeaponClass
{
public:
    WeaponInvisibilityClass(unsigned int uHash);
    WeaponInvisibilityClass(WeaponInvisibilityClass* pBase, unsigned int uHash);
    virtual ~WeaponInvisibilityClass();

    enum TRIGGER_TYPE
    {
        TRIGGER_PRESS = 0,
        TRIGGER_TOGGLE = 1,
        TRIGGER_HOLD = 2
    };

    virtual WeaponClass* Derive(unsigned int uHash);
    virtual Weapon* Build(const WeaponDesc& desc);
    // FUNCTION: SWBF2 0x00685d10
    // WeaponInvisibilityClass::IsRtti
    // FUNCTION: SWBF2 0x00685d40
    // WeaponInvisibilityClass::GetDerivedRtti
    // FUNCTION: SWBF2 0x00685d50
    // WeaponInvisibilityClass::GetDerivedRttiName
    PBL_DECLARE_DERIVED_RTTI(WeaponInvisibilityClass, WeaponClass)
    virtual void SetProperty(unsigned int uProperty, const char* pszValue);

    TRIGGER_TYPE m_eInvisibilityTrigger;
    float m_fInvisibilityMin;
    float m_fInvisibilityMax;
    float m_fSpeedForInvisibilityMin;
    float m_fSpeedForInvisibilityMax;
    float m_fInvisibilityIncRate;
    float m_fInvisibilityDecRate;
    float m_fInvisibilityPersistTime;
    float m_fFlickerAmplitude;
    float m_fFlickerTimeMin;
    float m_fFlickerTimeMax;
    float mInitialSalvoDelay;
    float mInitialEnergyDrain;
};

// VTABLE: SWBF2 0x007b1048
class WeaponInvisibility : public Weapon
{
public:
    WeaponInvisibility(const WeaponInvisibilityClass* pClass, const WeaponDesc& desc);
    virtual ~WeaponInvisibility();

    // FUNCTION: SWBF2 0x00685cc0
    // WeaponInvisibility::IsRtti
    // FUNCTION: SWBF2 0x00685cf0
    // WeaponInvisibility::GetDerivedRtti
    // FUNCTION: SWBF2 0x00685d00
    // WeaponInvisibility::GetDerivedRttiName
    PBL_DECLARE_DERIVED_RTTI(WeaponInvisibility, Weapon)
    virtual void Select(unsigned int uFlags, bool bOffhand);
    virtual void Deselect(unsigned int uFlags, bool bOffhand);
    virtual void Write(NetPktGroup& group);
    virtual void Read(NetPktGroup& group);

protected:
    virtual void EnterIdle();
    virtual bool UpdateIdle(float fDeltaTime);
    virtual void ExitIdle();
    virtual void EnterFire();
    virtual bool UpdateFire(float fDeltaTime);
    virtual void ExitFire();
    virtual bool UpdateReload(float fDeltaTime);
    virtual void EnterEmpty();
    virtual bool UpdateEmpty(float fDeltaTime);
    virtual void ExitEmpty();
    virtual bool CheckFire() const;
    virtual bool FireDone() const;
    virtual bool CheckEmpty() const;
    virtual bool CheckReload() const;

    void Fire();
    void InputInvisibilityOn();
    void InputInvisibilityOff();
    void StartInvisibility();
    void StopInvisibility();
    void UpdateInvisibility(float fDeltaTime);

public:

    void StartFlicker();

    float m_fInvisibilityFactor;
    float m_fInvisibilityPersistTimer;
    float m_fInvisibilityFlickerTimer;
    bool m_bInvisibilityOn;
    bool m_bInvisibilityActive;
    bool mFired;
    float mOwnerHealth;
};

