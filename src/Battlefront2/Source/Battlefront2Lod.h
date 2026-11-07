#pragma once

#include <stddef.h>

class RedLodData
{
public:
    // a level of detail's bit in _LodBitmask
    static int LodBit(int iLod) { return 1 << iLod; }

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
    bool HasLod(int iLod) const { return (_LodBitmask & LodBit(iLod)) != 0; }

    int GetLodClass() const { return _LodClass; }
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
 
 
 
 
 
 

class OrdnanceLodData : public RedLodData
{
public:
    OrdnanceLodData();
};
 

class NoLodData : public RedLodData
{
public:
    NoLodData();
};
 

class SmallEffectLodData : public RedLodData
{
public:
    SmallEffectLodData();
};
class MediumEffectLodData : public RedLodData
{
public:
    MediumEffectLodData();
};
class LargeEffectLodData : public RedLodData
{
public:
    LargeEffectLodData();
};
extern SmallEffectLodData smallEffectLodData;
extern MediumEffectLodData mediumEffectLodData;
extern LargeEffectLodData largeEffectLodData;
 

class RedModel;

class ModelLodData : public RedLodData
{
public:
    ModelLodData(int iLodType, const RedModel* pLod0, const RedModel* pLod1,
                 const RedModel* pLod2, const RedModel* pLod3);
};
 

class SoldierLodData : public RedLodData
{
public:
    SoldierLodData(const RedModel* pModel, const RedModel* pLowResModel);
};
