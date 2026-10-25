#include "sndsource2d3d.h"
#include "PebbleFL/Common/PblFile.h"
#include "PebbleFL/Common/PblChunk.h"

namespace Snd
{

// FUNCTION: SWBF2 0x0074b2a0
float Source2D3D::CalculatePitch(const Source3D* pSource3D) const
{
    if (!Get3DMode())
    {
        return mPitch;
    }
    return pSource3D->CalculatePitch() * mPitch;
}

// FUNCTION: SWBF2 0x0074b2d0
bool Source2D3D::Read(PblFileChunk& chunk, unsigned int uiPropertyID)
{
    if (uiPropertyID == 0x1A8B16B5u)
    {
        PblFile* const pFile = chunk._pFile;
        pFile->Read8();
        Set3DMode(pFile->ReadFloat() > 0.0f);
        return true;
    }
    return Source::Read(chunk, uiPropertyID)
           || Source2D::Read(chunk, uiPropertyID)
           || Source3D::Properties::Read(chunk, uiPropertyID);
}

}
