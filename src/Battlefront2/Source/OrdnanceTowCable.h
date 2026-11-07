#pragma once

#include "PebbleFL/Common/PblHashTable.h"
#include "PebbleFL/Common/PblRtti.h"

#include <stddef.h>

#include "OrdnanceBullet.h"

class pcRedShader;

// VTABLE: SWBF2 0x007ad19c
class OrdnanceTowCableClass : public OrdnanceBulletClass
{
public:
    virtual ~OrdnanceTowCableClass();
    OrdnanceTowCableClass(unsigned int uHash);
    OrdnanceTowCableClass(OrdnanceTowCableClass* pBase, unsigned int uHash);

    virtual OrdnanceClass* Derive(unsigned int uHash);
    virtual Ordnance* Build(const OrdnanceDesc& desc);
    // FUNCTION: SWBF2 0x00607330
    // OrdnanceTowCableClass::IsRtti
    // FUNCTION: SWBF2 0x00607360
    // OrdnanceTowCableClass::GetDerivedRtti
    // FUNCTION: SWBF2 0x00607370
    // OrdnanceTowCableClass::GetDerivedRttiName
    PBL_DECLARE_DERIVED_RTTI(OrdnanceTowCableClass, OrdnanceBulletClass)
    virtual void SetProperty(unsigned int uProperty, const char* pszValue);

    float mInitialCableLimit;
    float mCableLimit;

};

class WeaponTowCable;

enum TowCableState
{
    TOW_ATTACHING = 0,
    TOW_WRAP = 1,
    TOW_FALL = 2,
    TOW_DAMAGE = 3,
};

struct TowCableAttachPointData
{
    TowCableAttachPointData() {}

    float mFinalHeight;
    float mFallSpeed;
    bool mIsAttached;
    int mBodyID;
    PblVector3 mWorldBodyOffset;
    PblVector3 mWorldInitialBodyOffset;
    PblVector3 mBodyOffset;
    PblVector3 mInitialBodyOffset;
    PblVector3 mFinalSlope;
    PblVector3 mSlopeIn;
    PblVector3 mSlopeOut;
};

class TowCableRenderObject
{
public:
    static const int kNumAttachPoints = 13;
    TowCableRenderObject();
    ~TowCableRenderObject() {}

    void Clear();
    void AddAttachPoint(int iBodyID, const PblVector3& vOffset, const PblVector3& vSlope);
    void UpdateAttachPoints(float fDeltaTime, GameObject* pObject);
    void Render(GameObject* pObject, unsigned char ucViewport, float fDist, unsigned int uiFlags);

    TowCableState mTowState;
    TowCableAttachPointData mAttachPoints[kNumAttachPoints];
};

// VTABLE: SWBF2 0x007ad1cc Thread
// VTABLE: SWBF2 0x007ad20c
// VTABLE: SWBF2 0x007ad274 PblHandled
class OrdnanceTowCable : public OrdnanceBullet
{
public:
    OrdnanceTowCable(OrdnanceTowCableClass* pClass, const OrdnanceDesc& desc);
    virtual ~OrdnanceTowCable();

    // FUNCTION: SWBF2 0x006072a0
    // OrdnanceTowCable::IsRtti
    // FUNCTION: SWBF2 0x006072d0
    // OrdnanceTowCable::GetDerivedRtti
    // FUNCTION: SWBF2 0x006072e0
    // OrdnanceTowCable::GetDerivedRttiName
    PBL_DECLARE_DERIVED_RTTI(OrdnanceTowCable, OrdnanceBullet)
    virtual bool Collide(CollisionObject* pObject, const PblVector3& vPos,
                         const PblVector3& vNormal, int iBodyId);
    virtual void Render(unsigned char ucViewport, float fDist, unsigned int uiFlags);

    void SetFakeCable(bool bFake) { mFakeCable = bFake; }

    // SYNTHETIC: SWBF2 0x006072f0
    // OrdnanceTowCable::operator delete
    static void operator delete(void* pBlock) { sMemoryPool.Free(pBlock); }

    static void* operator new(unsigned int uSize) { return sMemoryPool.Allocate(uSize); }

protected:
    virtual bool Update(float fDeltaTime);

    void CalculateWorldSpacePoints();
    void ConvertPointsToWorldSpace();
    void RecalculateCenterAndRadius();
    void TowCableFailure();

public:
    WeaponTowCable* mFireWeapon;
    float mTowCableFallTime;
    PblVector3 mLastOwnerFirePos;
    PblVector3 mLastOwnerFireDir;
    TowCableRenderObject mTowCable;
    PblHandle<GameObject> mHitWalker;
    bool mFakeCable;
    bool mDeleteNextUpdate;

    static MemoryPool sMemoryPool;

    static void PlatformInit();
    static pcRedShader* sShader;
    static void PlatformCleanup();
    static unsigned int sTextureHash;
    static float sCableThickness;
};


