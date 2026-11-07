#pragma once

#include "RedEngineFL/Portals/Portal.h"
#include "PebbleFL/Common/PblBits.h"
#include "PebbleFL/Common/PblVector.h"
#include "RedEngineFL/Graphics/RedRenderFlags.h"

class PblBox;
class PblMatrix;
class RedCamera;
class RedLodData;
class RedSceneAABox;

// VTABLE: SWBF2 0x00783730
class RedSceneObject : public pvs::DynDisplayable
{

    friend class EntityWalker;

    friend class EntityHover;

    friend class EntityFlyer;

public:
    RedSceneObject(const RedLodData* pLodData);
    virtual ~RedSceneObject();

    // FUNCTION: SWBF2 0x00444000
    virtual void GetAABound(geo::Box&) const
    {
    }
    // FUNCTION: SWBF2 0x00444010
    virtual void GetSphereBound(geo::Sphere& rSphere) const
    {
        rSphere = geo::Sphere(geo::Vector3<float>(_Sphere.x, _Sphere.y, _Sphere.z), _Sphere.w);
    }

    virtual void Activate();
    virtual void Deactivate();
    virtual void Render(unsigned char ucViewport, float fDist, unsigned int uiFlags) = 0;

    // FUNCTION: SWBF2 0x00444060
    virtual const PblMatrix& GetBBoxMatrix() const
    {
        return gMatrixIdentity;
    }
    // FUNCTION: SWBF2 0x00427110
    virtual const PblBox& GetBBox() const
    {
        return *(const PblBox*)0;
    }
    // FUNCTION: SWBF2 0x00444070
    virtual bool IsReflected() const
    {
        return true;
    }

    bool IsClipped() const;

    void SetClipped(bool bClipped)
    {
        if (bClipped)
        {
            _uiRenderFlags |= RENDERFLAG_CLIPPED;
        }
        else
        {
            _uiRenderFlags &= ~RENDERFLAG_CLIPPED;
        }
    }

    // FUNCTION: SWBF2 0x00444080
    void AddToScene() { Activate(); }

    // FUNCTION: SWBF2 0x00444090
    void RemoveFromScene() { Deactivate(); }

    // FUNCTION: SWBF2 0x004940e0
    bool IsInScene() const { return _bActive; }
    bool IsOccluded() const;
    void UpdateOcclusionQuery(unsigned int uiViewport);

    static bool ShouldRender();

    const RedLodData* GetLodData() const { return _pLodData; }

    const PblVector3& GetRenderPos() const { return *(const PblVector3*)&_Sphere; }
    float GetPriorityMod() { return _fPriorityMod; }

protected:

    void SetTestBBox(bool bTest)
    {
        if (bTest)
        {
            _uiRenderFlags |= RENDERFLAG_TEST_BBOX;
        }
        else
        {
            _uiRenderFlags &= ~RENDERFLAG_TEST_BBOX;
        }
    }
    virtual void SetRenderPos(const PblVector3& vPos);
    virtual void SetRenderRadius(float fRadius);

public:
    RedSceneObject* mNextSceneObject;
    RedSceneObject* mPrevSceneObject;
    RedSceneAABox* mSceneAABox;
    PblSphere _Sphere;
    RedLodData* _pLodData;
    unsigned int _uiRenderFlags;
    bool _bActive;
    float _fPriorityMod;
    int _iFrameNum[1];
    unsigned int _uiOcclusionIndex[1];

    static unsigned int renderDbgIndex;
    static unsigned int renderDbgMask;
    static unsigned int renderDbgBits;
};

 
 
 
 
 
 

class RedLodData
{
public:

    enum LowResFlags
    {
        HAS_LOWREZ = 1,
        CROSSFADE_LOWREZ = 2,
        HAS_TRANSPARENT = 4
    };

    RedLodData()
    {
        _LodBitmask = 0;
        mLowRezFlags = 0;
        mMaxDist = 1000.0f;
    }

    int GetCost(int iLod) const { return _LodCost[iLod]; }
    bool HasLod(int iLod) const { return (_LodBitmask & Bit(iLod)) != 0; }

    int GetLodClass() const { return _LodClass; }
    float GetMaxDist() const { return mMaxDist; }
    void SetMaxDist(float fMaxDist) { mMaxDist = fMaxDist; }
    int HasLowRes() const { return mLowRezFlags & HAS_LOWREZ; }
    int CrossFadeLowRes() const { return mLowRezFlags & CROSSFADE_LOWREZ; }
    int HasTransparentSegment() const { return mLowRezFlags & HAS_TRANSPARENT; }

    int _LodCost[4];
    int _LodBitmask;
    int _LodClass;
    unsigned char mLowRezFlags;
    float mMaxDist;
};

bool BoxClipsNearPlane(const RedCamera* pCamera, const PblVector3& vCenter,
                       const PblVector3& vExtents);
