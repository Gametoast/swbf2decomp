#pragma once

#include "PebbleFL/Common/PblHashTable.h"
#include "PebbleFL/Common/PblRtti.h"

#include <stddef.h>

#include "FoleyFXOrdnance.h"

// VTABLE: SWBF2 0x0079f2b4
class FoleyFXWalker : public FoleyFXImpact
{
public:
    static void* operator new(unsigned int) { return smFreeList.Alloc(); }
    // SYNTHETIC: SWBF2 0x0052c080
    // FoleyFXWalker::operator delete
    static void operator delete(void* p) { smFreeList.Free(p); }

    FoleyFXWalker();

    // FUNCTION: SWBF2 0x0052d000
    // FoleyFXWalker::IsRtti
    // FUNCTION: SWBF2 0x0052d030
    // FoleyFXWalker::GetDerivedRtti
    // FUNCTION: SWBF2 0x0052d040
    // FoleyFXWalker::GetDerivedRttiName
    PBL_DECLARE_DERIVED_RTTI(FoleyFXWalker, FoleyFX)
    virtual ~FoleyFXWalker();
    virtual bool Read(PblFileChunk& chunk, unsigned int uChunkID);

    GameSound mFootstep[6];

    static Snd::FreeList smFreeList;
};
 
 


