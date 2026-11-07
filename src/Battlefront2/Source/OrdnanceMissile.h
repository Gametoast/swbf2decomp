#pragma once

#include "PebbleFL/Common/PblHashTable.h"
#include "PebbleFL/Common/PblRtti.h"

#include <stddef.h>

#include "OrdnanceBullet.h"

// VTABLE: SWBF2 0x007acecc
class OrdnanceMissileClass : public OrdnanceBulletClass
{
public:
    OrdnanceMissileClass(unsigned int uHash);
    virtual ~OrdnanceMissileClass();
    OrdnanceMissileClass(OrdnanceMissileClass* pBase, unsigned int uHash);

    virtual OrdnanceClass* Derive(unsigned int uHash);
    virtual Ordnance* Build(const OrdnanceDesc& desc);
    // FUNCTION: SWBF2 0x00604950
    // OrdnanceMissileClass::IsRtti
    // FUNCTION: SWBF2 0x00604980
    // OrdnanceMissileClass::GetDerivedRtti
    // FUNCTION: SWBF2 0x00604990
    // OrdnanceMissileClass::GetDerivedRttiName
    PBL_DECLARE_DERIVED_RTTI(OrdnanceMissileClass, OrdnanceBulletClass)
    virtual void SetProperty(unsigned int uProperty, const char* pszValue);
    // FUNCTION: SWBF2 0x006049a0
    virtual float GetVelocity() const
    {
        return 3.0e8f;
    }

    float mScaleRate;
    float mTurnRate;
    float mWaverRate;
    float mWaverTurn;
    GameSound mLockedOnSound;
    float mMinSpeed;
    float mAcceleration;
};

class GameObject;

// VTABLE: SWBF2 0x007ace84 Thread
// VTABLE: SWBF2 0x007acec4 PblHandled
// VTABLE: SWBF2 0x007acefc
class OrdnanceMissile : public OrdnanceBullet
{
public:
    OrdnanceMissile(OrdnanceMissileClass* pClass, const OrdnanceDesc& desc);
    virtual ~OrdnanceMissile();

    // FUNCTION: SWBF2 0x006048d0
    // OrdnanceMissile::IsRtti
    // FUNCTION: SWBF2 0x00604900
    // OrdnanceMissile::GetDerivedRtti
    // FUNCTION: SWBF2 0x00604910
    // OrdnanceMissile::GetDerivedRttiName
    PBL_DECLARE_DERIVED_RTTI(OrdnanceMissile, OrdnanceBullet)

protected:
    virtual bool Update(float fDeltaTime);

public:
    PblVector3 mTargetDir;
    PblHandle<GameObject const> mTarget;
    int mBodyID;
    float mScale;
    float mWaverPitch;
    float mWaverYaw;
    GameSoundControllable mLockedOnSoundCtrl;
    PblVector3 mVelocityDir;
    float mGoalSpeed;
    float mSpeed;

};

