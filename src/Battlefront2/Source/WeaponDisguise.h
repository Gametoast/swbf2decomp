#pragma once

#include "PebbleFL/Common/PblHashTable.h"
#include "PebbleFL/Common/PblRtti.h"

#include <stddef.h>

#include "Weapon.h"
#include "PebbleFL/Common/PblHashTable.h"

// VTABLE: SWBF2 0x007b0abc
class WeaponDisguiseClass : public WeaponClass
{
public:
    WeaponDisguiseClass(unsigned int uHash);

    WeaponDisguiseClass(WeaponDisguiseClass* pBase, unsigned int uHash);
    virtual ~WeaponDisguiseClass();

    virtual WeaponClass* Derive(unsigned int uHash);
    virtual Weapon* Build(const WeaponDesc& desc);
    // FUNCTION: SWBF2 0x00682cb0
    // WeaponDisguiseClass::IsRtti
    // FUNCTION: SWBF2 0x00682ce0
    // WeaponDisguiseClass::GetDerivedRtti
    // FUNCTION: SWBF2 0x00682cf0
    // WeaponDisguiseClass::GetDerivedRttiName
    PBL_DECLARE_DERIVED_RTTI(WeaponDisguiseClass, WeaponClass)
    virtual void SetProperty(unsigned int uProperty, const char* pszValue);

    float m_fChangeRate;
};
  
 

#include "WeaponCannon.h"

class Controllable;
class GameModel;
class Team;
class Trigger;

// VTABLE: SWBF2 0x007b0af4
class WeaponDisguise : public Weapon
{
public:
    enum DISGUISE_STATE
    {
        STATE_OFF = 0,
        STATE_ON = 1
    };

    WeaponDisguise(const WeaponDisguiseClass* pClass, const WeaponDesc& desc);
    virtual ~WeaponDisguise();

    // FUNCTION: SWBF2 0x00682c60
    // WeaponDisguise::IsRtti
    // FUNCTION: SWBF2 0x00682c90
    // WeaponDisguise::GetDerivedRtti
    // FUNCTION: SWBF2 0x00682ca0
    // WeaponDisguise::GetDerivedRttiName
    PBL_DECLARE_DERIVED_RTTI(WeaponDisguise, Weapon)
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
    virtual bool CheckFire() const;
    virtual bool FireDone() const;
    virtual bool CheckEmpty() const;
    virtual bool CheckReload() const;

    void InputToggleDisguise();
    void WhoTwiggedMe();

public:

    void StartRaisingDisguise();
    void FinishRaisingDisguise();
    void StartDroppingDisguise();
    void FinishDroppingDisguise();

    const GameModel* mSavedOwnerModel;
    Team* mOtherTeam;
    DISGUISE_STATE m_eState;
};

