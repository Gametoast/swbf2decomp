#pragma once

#include "PebbleFL/Common/PblHashTable.h"
#include "PebbleFL/Common/PblRtti.h"

#include <stddef.h>

#include "FoleyFX.h"
#include "GameSound.h"
#include "PebbleFL/Common/PblHashTable.h"
#include "SoundFL/source/sndfreelist.h"

// VTABLE: SWBF2 0x0079f240
class FoleyFXImpact : public FoleyFX
{
public:
    static void* operator new(unsigned int) { return smFreeList.Alloc(); }
    static void operator delete(void* p) { smFreeList.Free(p); }

    FoleyFXImpact();

    // FUNCTION: SWBF2 0x0052c9e0
    // FoleyFXImpact::IsRtti
    // FUNCTION: SWBF2 0x0052ca10
    // FoleyFXImpact::GetDerivedRtti
    // FUNCTION: SWBF2 0x0052ca20
    // FoleyFXImpact::GetDerivedRttiName
    PBL_DECLARE_DERIVED_RTTI(FoleyFXImpact, FoleyFX)
    virtual ~FoleyFXImpact();
    virtual bool Read(PblFileChunk& chunk, unsigned int uChunkID);

    GameSound mImpact;

    static Snd::FreeList smFreeList;
};
 
 

