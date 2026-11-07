#pragma once

#include "PebbleFL/Common/PblHashTable.h"
#include "PebbleFL/Common/PblRtti.h"

#include <stddef.h>

#include "FoleyFXOrdnance.h"

// VTABLE: SWBF2 0x0079f280
class FoleyFXSoldier : public FoleyFXImpact
{
public:
    static void* operator new(unsigned int) { return smFreeList.Alloc(); }
    static void operator delete(void* p) { smFreeList.Free(p); }

    FoleyFXSoldier();

    // FUNCTION: SWBF2 0x0052cc40
    // FoleyFXSoldier::IsRtti
    // FUNCTION: SWBF2 0x0052cc70
    // FoleyFXSoldier::GetDerivedRtti
    // FUNCTION: SWBF2 0x0052cc80
    // FoleyFXSoldier::GetDerivedRttiName
    PBL_DECLARE_DERIVED_RTTI(FoleyFXSoldier, FoleyFX)
    virtual ~FoleyFXSoldier();
    virtual bool Read(PblFileChunk& chunk, unsigned int uChunkID);

    GameSound mFootstepLeftWalk;
    GameSound mFootstepRightWalk;
    GameSound mFootstepLeftRun;
    GameSound mFootstepRightRun;
    GameSound mFootstepLeftStop;
    GameSound mFootstepRightStop;
    GameSound mJump;
    GameSound mLand;
    GameSound mRoll;
    GameSound mProne;
    GameSound mSquat;
    GameSound mStand;
    GameSound mBodyFall;

    static Snd::FreeList smFreeList;
};
 
 
 

