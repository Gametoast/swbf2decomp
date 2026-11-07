#ifndef SWBF2_GRAPHICS_PC_PCREDVERTEXFORMAT_H
#define SWBF2_GRAPHICS_PC_PCREDVERTEXFORMAT_H

#include <stddef.h>
#include <d3d9.h>
#include <unknwn.h>

#include "PebbleFL/Common/PblList.h"
#include "PebbleFL/Common/PblRef.h"

// The Red vertex format word (RVF): which streams a vertex buffer or a shader's input has and how they are stored. Every model
// segment, effect and shader passes one (pcRedShader::Create, pcRedVertexFormat::Create). No build we have names it: read from
// pcRedVertexFormat's declaration builder (each bit's D3D usage and type) and RVFtoFVF.
enum RedVertexFormatFlags
{
    RVF_POSITION_MASK = 0x3,           // position components minus one
    RVF_POSITION_XY = 0x1,
    RVF_POSITION_XYZ = 0x2,
    RVF_POSITION_XYZRHW = 0x3,         // already transformed (POSITIONT)
    RVF_WEIGHTS_MASK = 0x1C,           // blend weight count (soft skinning), with blend indices
    RVF_WEIGHTS_SHIFT = 2,
    RVF_WEIGHTS_1 = 0x4,
    RVF_WEIGHTS_2 = 0x8,
    RVF_WEIGHTS_3 = 0xC,
    RVF_WEIGHTS_4 = 0x10,
    RVF_NORMAL = 0x20,
    RVF_TANGENTS = 0x40,               // tangent and binormal
    RVF_COLOR = 0x80,
    RVF_LIGHTING = 0x100,              // a second colour holding baked vertex lighting ("vertex lit")
    RVF_TEXCOORDS_MASK = 0x600,        // texture coordinate set count
    RVF_TEXCOORDS_SHIFT = 9,
    RVF_TEXCOORDS_1 = 0x200,
    RVF_TEXCOORDS_2 = 0x400,
    RVF_TEXCOORDS_3 = 0x600,
    RVF_STREAMS_MASK = 0x7FF,          // the streams above; the bits below say how a buffer stores them
    RVF_CUSTOM_DECLARATION = 0x800,    // laid out by an explicit D3D declaration (Create(rvf, pDeclaration) sets it as the input)
    RVF_COMPRESSED_POSITION = 0x1000,  // SHORT4 / SHORT2
    RVF_COMPRESSED_WEIGHTS = 0x2000,   // D3DCOLOR
    RVF_COMPRESSED_NORMALS = 0x4000,   // normal, tangent and binormal as D3DCOLOR
    RVF_COMPRESSED_TEXCOORDS = 0x8000, // SHORT2
    RVF_COMPRESSED_MASK = 0xF000
};

// VTABLE: SWBF2 0x007857bc
class pcRedVertexFormat : public PblRef
{
public:

    virtual ~pcRedVertexFormat();

    // SYNTHETIC: SWBF2 0x007142e0
    // pcRedVertexFormat::`scalar deleting destructor'

    unsigned int m_uiVertexBufferFVF;
    unsigned int m_uiVertexInputRVF;
    unsigned int m_uiVertexBufferRVF;
    IDirect3DVertexDeclaration9* m_pVertexDeclaration;
    PblList<pcRedVertexFormat>::Node m_node;

    pcRedVertexFormat();

    static pcRedVertexFormat* Create(unsigned int uiVertexBufferRVF,
                                     unsigned int uiVertexInputRVF);

    static pcRedVertexFormat* Create(unsigned int uiRVF)
    {
        return Create(uiRVF, uiRVF);
    }

    static pcRedVertexFormat* Create(unsigned int uiVertexBufferRVF,
                                     const D3DVERTEXELEMENT9* pDeclaration);

    static IDirect3DVertexDeclaration9* CreateVertexDeclaration(
        unsigned int uiVertexBufferRVF, unsigned int uiVertexInputRVF);

    static unsigned int RVFtoFVF(unsigned int uiRVF);

    static void Cleanup();

    static PblList<pcRedVertexFormat> s_vertexFormatList;
};

 
 

 #endif
