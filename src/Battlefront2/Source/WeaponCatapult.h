#pragma once

#include "PebbleFL/Common/PblHashTable.h"

#include <stddef.h>

#include "Weapon.h"
#include "PebbleFL/Common/PblRtti.h"
#include "PebbleFL/Common/PblHashTable.h"

// VTABLE: SWBF2 0x007b06d0
class WeaponCatapultClass : public WeaponClass
{
public:
    WeaponCatapultClass(unsigned int uHash);

    WeaponCatapultClass(WeaponCatapultClass* pBase, unsigned int uHash);

    virtual ~WeaponCatapultClass();

    virtual WeaponClass* Derive(unsigned int uHash);
    virtual Weapon* Build(const WeaponDesc& desc);
    // FUNCTION: SWBF2 0x00680c30
    // WeaponCatapultClass::IsRtti
    // FUNCTION: SWBF2 0x00680c60
    // WeaponCatapultClass::GetDerivedRtti
    // FUNCTION: SWBF2 0x00680c70
    // WeaponCatapultClass::GetDerivedRttiName
    PBL_DECLARE_DERIVED_RTTI(WeaponCatapultClass, WeaponClass)
    virtual void SetProperty(unsigned int uProperty, const char* pszValue);

    const RedModel* mOrdnanceModel;
    unsigned int mArmHash;
    float mCockedAngle;
};
  
 
 
 

#include "WeaponCannon.h"

class EntityBuildingArmed;

// VTABLE: SWBF2 0x007b0708
class WeaponCatapult : public Weapon
{
public:
    WeaponCatapult(const WeaponCatapultClass* pClass, const WeaponDesc& desc);
    virtual ~WeaponCatapult();

    // FUNCTION: SWBF2 0x00680be0
    // WeaponCatapult::IsRtti
    // FUNCTION: SWBF2 0x00680c10
    // WeaponCatapult::GetDerivedRtti
    // FUNCTION: SWBF2 0x00680c20
    // WeaponCatapult::GetDerivedRttiName
    PBL_DECLARE_DERIVED_RTTI(WeaponCatapult, Weapon)
    virtual void Render(const PblMatrix& matrix, const RedPose* pPose, const RedColor& color,
                        unsigned int uFlags, bool bArg) const;

protected:
    virtual void EnterFire();
    virtual bool UpdateFire(float fDeltaTime);
    virtual void EnterReload();
    virtual bool UpdateReload(float fDeltaTime);
    virtual void ExitReload();
    virtual bool CheckFire() const;
    virtual bool FireDone() const;

    virtual bool CheckReload() const;

    void SetArmAngle(float fAngle);

    bool Fire();

public:

    PblMatrix mArmPoseMatrix;
    const WeaponCatapultClass* mCatapultClass;
    float mLastArmAngle;
    float mNextArmAngle;
    float mArmAccel;
    float mArmSpeed;
    float mCurrArmAngle;
    bool mShowOrdnance;
    EntityBuildingArmed* mCatapultBuilding;
};
  
 
 
 
 
 
 
 
 
 

