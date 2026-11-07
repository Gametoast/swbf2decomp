#pragma once

#include "PebbleFL/Common/PblHashTable.h"
#include "PebbleFL/Common/PblRtti.h"

#include <stddef.h>

#include "OrdnanceBullet.h"
#include "SoldierAnimationBank.h"

// VTABLE: SWBF2 0x007ac940
class OrdnanceGrapplingHookClass : public OrdnanceBulletClass
{
public:
    OrdnanceGrapplingHookClass(unsigned int uHash);
    virtual ~OrdnanceGrapplingHookClass();
    OrdnanceGrapplingHookClass(OrdnanceGrapplingHookClass* pBase, unsigned int uHash);

    virtual OrdnanceClass* Derive(unsigned int uHash);
    virtual Ordnance* Build(const OrdnanceDesc& desc);
    // FUNCTION: SWBF2 0x005ff110
    // OrdnanceGrapplingHookClass::IsRtti
    // FUNCTION: SWBF2 0x005ff140
    // OrdnanceGrapplingHookClass::GetDerivedRtti
    // FUNCTION: SWBF2 0x005ff150
    // OrdnanceGrapplingHookClass::GetDerivedRttiName
    PBL_DECLARE_DERIVED_RTTI(OrdnanceGrapplingHookClass, OrdnanceBulletClass)
    virtual void SetProperty(unsigned int uProperty, const char* pszValue);

    SoldierAnimationBank::ANIMATION mAttachSoldierAnimation;

};

class EntitySoldier;
class WeaponGrapplingHook;

enum GrapplingHookState
{
    HOOK_ATTACHING = 0,
    HOOK_WAITING = 1,
    HOOK_LIFTING = 2,
    HOOK_HANGING = 3,
    HOOK_ABORTING = 4
};

// VTABLE: SWBF2 0x007ac9d8 Thread
// VTABLE: SWBF2 0x007ac970
// VTABLE: SWBF2 0x007aca18 PblHandled
class OrdnanceGrapplingHook : public OrdnanceBullet
{
public:
    OrdnanceGrapplingHook(OrdnanceGrapplingHookClass* pClass, const OrdnanceDesc& desc);
    virtual ~OrdnanceGrapplingHook();

    // FUNCTION: SWBF2 0x005ff090
    // OrdnanceGrapplingHook::IsRtti
    // FUNCTION: SWBF2 0x005ff0c0
    // OrdnanceGrapplingHook::GetDerivedRtti
    // FUNCTION: SWBF2 0x005ff0d0
    // OrdnanceGrapplingHook::GetDerivedRttiName
    PBL_DECLARE_DERIVED_RTTI(OrdnanceGrapplingHook, OrdnanceBullet)
    virtual bool Collide(CollisionObject* pObject, const PblVector3& vPos,
                         const PblVector3& vNormal, int iBodyId);
    virtual void Render(unsigned char ucViewport, float fDist, unsigned int uiFlags);

    void MoveSoldier(EntitySoldier* pSoldier, float fDeltaTime);

protected:
    virtual bool Update(float fDeltaTime);

    void HookFailure();

public:
    GrapplingHookState mState;
    WeaponGrapplingHook* mFireWeapon;
    PblHandle<GameObject> mHitObject;
    int mCollBodyID;
    PblVector3 mCollOffset;
    PblVector3 mWeaponOffset;
    float mMaxLifetime;
    float mWaitTime;
    float mDeltaTime;

    static void PlatformInit();

    static void PlatformCleanup();
    static class pcRedShader* sShader;

    static unsigned int sTextureHash;
};

