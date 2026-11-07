#include <string.h>

#include "pcRedVertexFormat.h"

#include "RedEngineFL/Memory/engineAlloc.h"

#include "pcRedRenderer.h"

// FUNCTION: SWBF2 0x00714220
pcRedVertexFormat::pcRedVertexFormat()
{


    m_uiVertexBufferFVF = 0;
    m_uiVertexInputRVF = 0;
    m_uiVertexBufferRVF = 0;
    m_pVertexDeclaration = 0;
}

// FUNCTION: SWBF2 0x00714270
pcRedVertexFormat::~pcRedVertexFormat()
{
    if (m_pVertexDeclaration)
    {
        m_pVertexDeclaration->Release();
    }
}

// GLOBAL: SWBF2 0x007e22bc
PblList<pcRedVertexFormat> pcRedVertexFormat::s_vertexFormatList;

// GLOBAL: SWBF2 0x007857c4

extern const unsigned char kVertexDeclarationTypes[5];
const unsigned char kVertexDeclarationTypes[5] = {
    D3DDECLTYPE_UNUSED, D3DDECLTYPE_FLOAT1, D3DDECLTYPE_FLOAT2,
    D3DDECLTYPE_FLOAT3, D3DDECLTYPE_FLOAT4
};

// FUNCTION: SWBF2 0x00714420
pcRedVertexFormat* pcRedVertexFormat::Create(unsigned int uiVertexBufferRVF,
                                             unsigned int uiVertexInputRVF)
{
    if (uiVertexBufferRVF == 0)
    {
        return 0;
    }

    const unsigned int uiInput = uiVertexInputRVF & RVF_STREAMS_MASK;

    PblList<pcRedVertexFormat>::Node* pNode = s_vertexFormatList.GetFirst();
    while (pNode->_pObject != s_vertexFormatList._head._pObject)
    {
        pcRedVertexFormat* pFormat = pNode->_pObject;
        if (pFormat->m_uiVertexBufferRVF > uiVertexBufferRVF)
        {
            break;
        }
        if (pFormat->m_uiVertexBufferRVF == uiVertexBufferRVF)
        {
            if (pFormat->m_uiVertexInputRVF > uiInput)
            {
                break;
            }
            if (pFormat->m_uiVertexInputRVF == uiInput)
            {
                return pNode->_pObject;
            }
        }
        pNode = pNode->_pNext;
    }

    pcRedVertexFormat* pFormat = new pcRedVertexFormat;

    pFormat->m_uiVertexBufferRVF = uiVertexBufferRVF;
    pFormat->m_uiVertexInputRVF = uiInput;
    pFormat->m_pVertexDeclaration =
        CreateVertexDeclaration(uiVertexBufferRVF, uiInput);
    pFormat->m_uiVertexBufferFVF = RVFtoFVF(uiVertexBufferRVF);

    s_vertexFormatList.AddBefore(pNode, &pFormat->m_node, pFormat);

    return pFormat;
}

// FUNCTION: SWBF2 0x007144e0
pcRedVertexFormat* pcRedVertexFormat::Create(unsigned int uiVertexBufferRVF,
                                             const D3DVERTEXELEMENT9* pDeclaration)
{
    pcRedVertexFormat* pFormat = new pcRedVertexFormat;

    pFormat->m_uiVertexBufferRVF = uiVertexBufferRVF;
    pFormat->m_uiVertexInputRVF = RVF_CUSTOM_DECLARATION;

    IDirect3DVertexDeclaration9* pVertexDeclaration;
    RedRenderer::pcGetDevice()->CreateVertexDeclaration(pDeclaration, &pVertexDeclaration);
    pFormat->m_pVertexDeclaration = pVertexDeclaration;
    pFormat->m_uiVertexBufferFVF = RVFtoFVF(uiVertexBufferRVF);

    s_vertexFormatList.AddLast(&pFormat->m_node, pFormat);

    return pFormat;
}

// FUNCTION: SWBF2 0x00714570
IDirect3DVertexDeclaration9* pcRedVertexFormat::CreateVertexDeclaration(
    unsigned int uiVertexBufferRVF, unsigned int uiVertexInputRVF)
{
    D3DVERTEXELEMENT9 aElements[16];
    memset(aElements, 0, sizeof(aElements));
    for (int i = 0; i < 16; i++)
    {
        aElements[i].Type = D3DDECLTYPE_UNUSED;
    }

    if (uiVertexBufferRVF == 0 || uiVertexInputRVF == 0)
    {
        return 0;
    }

    unsigned short usOffset = 0;
    int uiCount = 0;

    if (uiVertexBufferRVF & RVF_POSITION_MASK)
    {
        const unsigned short usBufComponents =
            (unsigned short)((uiVertexBufferRVF & RVF_POSITION_MASK) + 1);
        const unsigned short usInComponents =
            (unsigned short)((uiVertexInputRVF & RVF_POSITION_MASK) + 1);

        if (uiVertexBufferRVF & RVF_COMPRESSED_POSITION)
        {
            if (usInComponents > 2)
            {
                aElements[0].Type = D3DDECLTYPE_SHORT4;
                aElements[0].Offset = 0;
                aElements[0].Usage = D3DDECLUSAGE_POSITION;
                aElements[0].UsageIndex = 0;
                uiCount = 1;
                usOffset = 8;
            }
            else if (usInComponents != 0)
            {
                aElements[0].Type = D3DDECLTYPE_SHORT2;
                aElements[0].Offset = 0;
                aElements[0].Usage = D3DDECLUSAGE_POSITION;
                aElements[0].UsageIndex = 0;
                uiCount = 1;
                usOffset = 4;
            }
        }
        else
        {
            if (usInComponents > 1)
            {
                aElements[0].Type = kVertexDeclarationTypes[usInComponents];
                aElements[0].Offset = 0;

                aElements[0].UsageIndex = 0;
                aElements[0].Usage = (usInComponents == 4)
                                         ? D3DDECLUSAGE_POSITIONT
                                         : D3DDECLUSAGE_POSITION;
                uiCount = 1;
            }
            usOffset = (unsigned short)(usBufComponents * 4);
        }
    }

    if (uiVertexBufferRVF & RVF_WEIGHTS_MASK)
    {
        const unsigned short usBufWeights = (unsigned short)((uiVertexBufferRVF & RVF_WEIGHTS_MASK) >> RVF_WEIGHTS_SHIFT);
        unsigned short usInWeights = (unsigned short)((uiVertexInputRVF & RVF_WEIGHTS_MASK) >> RVF_WEIGHTS_SHIFT);
        if (usInWeights > usBufWeights)
        {
            usInWeights = usBufWeights;
        }

        if (usBufWeights > 1)
        {
            if (uiVertexBufferRVF & RVF_COMPRESSED_WEIGHTS)
            {
                if (usInWeights > 1)
                {
                    aElements[uiCount].Type = D3DDECLTYPE_D3DCOLOR;
                    aElements[uiCount].Offset = usOffset;
                    aElements[uiCount].Usage = D3DDECLUSAGE_BLENDWEIGHT;
                    aElements[uiCount].UsageIndex = 0;
                    ++uiCount;
                }
                usOffset = (unsigned short)(usOffset + 4);
            }
            else
            {
                if (usInWeights > 1)
                {
                    aElements[uiCount].Offset = usOffset;
                    aElements[uiCount].Usage = D3DDECLUSAGE_BLENDWEIGHT;
                    aElements[uiCount].UsageIndex = 0;

                    aElements[uiCount].Type = kVertexDeclarationTypes[usInWeights - 1];
                    ++uiCount;
                }
                usOffset = (unsigned short)(usOffset + usBufWeights * 4 - 4);
            }
        }

        if (usInWeights != 0)
        {
            aElements[uiCount].Type = D3DDECLTYPE_D3DCOLOR;
            aElements[uiCount].Offset = usOffset;
            aElements[uiCount].Usage = D3DDECLUSAGE_BLENDINDICES;
            aElements[uiCount].UsageIndex = 0;
            ++uiCount;
        }
        usOffset = (unsigned short)(usOffset + 4);
    }

    if (uiVertexBufferRVF & RVF_NORMAL)
    {
        if (uiVertexBufferRVF & RVF_COMPRESSED_NORMALS)
        {
            if (uiVertexInputRVF & RVF_NORMAL)
            {
                aElements[uiCount].Type = D3DDECLTYPE_D3DCOLOR;
                aElements[uiCount].Offset = usOffset;
                aElements[uiCount].Usage = D3DDECLUSAGE_NORMAL;
                aElements[uiCount].UsageIndex = 0;
                ++uiCount;
            }
            usOffset = (unsigned short)(usOffset + 4);
        }
        else
        {
            if (uiVertexInputRVF & RVF_NORMAL)
            {
                aElements[uiCount].Type = D3DDECLTYPE_FLOAT3;
                aElements[uiCount].Offset = usOffset;
                aElements[uiCount].Usage = D3DDECLUSAGE_NORMAL;
                aElements[uiCount].UsageIndex = 0;
                ++uiCount;
            }
            usOffset = (unsigned short)(usOffset + 12);
        }
    }

    if (uiVertexBufferRVF & RVF_TANGENTS)
    {
        if (uiVertexBufferRVF & RVF_COMPRESSED_NORMALS)
        {
            if (uiVertexInputRVF & RVF_TANGENTS)
            {
                D3DVERTEXELEMENT9& tangent = aElements[uiCount];
                D3DVERTEXELEMENT9& binormal = aElements[uiCount + 1];
                tangent.Type = D3DDECLTYPE_D3DCOLOR;
                tangent.Offset = usOffset;
                tangent.Usage = D3DDECLUSAGE_TANGENT;
                tangent.UsageIndex = 0;
                binormal.Offset = (unsigned short)(usOffset + 4);
                binormal.Type = D3DDECLTYPE_D3DCOLOR;
                binormal.Usage = D3DDECLUSAGE_BINORMAL;
                binormal.UsageIndex = 0;
                uiCount += 2;
            }
            usOffset = (unsigned short)(usOffset + 8);
        }
        else
        {
            if (uiVertexInputRVF & RVF_TANGENTS)
            {
                D3DVERTEXELEMENT9& tangent = aElements[uiCount];
                D3DVERTEXELEMENT9& binormal = aElements[uiCount + 1];
                tangent.Type = D3DDECLTYPE_FLOAT3;
                tangent.Offset = usOffset;
                tangent.Usage = D3DDECLUSAGE_TANGENT;
                tangent.UsageIndex = 0;
                binormal.Offset = (unsigned short)(usOffset + 12);
                binormal.Type = D3DDECLTYPE_FLOAT3;
                binormal.Usage = D3DDECLUSAGE_BINORMAL;
                binormal.UsageIndex = 0;
                uiCount += 2;
            }
            usOffset = (unsigned short)(usOffset + 24);
        }
    }

    unsigned char ucColourIndex = 0;
    if (uiVertexBufferRVF & RVF_COLOR)
    {
        if (uiVertexInputRVF & RVF_COLOR)
        {
            aElements[uiCount].Type = D3DDECLTYPE_D3DCOLOR;
            ucColourIndex = 1;
            aElements[uiCount].Offset = usOffset;
            aElements[uiCount].Usage = D3DDECLUSAGE_COLOR;
            aElements[uiCount].UsageIndex = 0;
            ++uiCount;
        }
        usOffset = (unsigned short)(usOffset + 4);
    }
    if (uiVertexBufferRVF & RVF_LIGHTING)
    {
        if (uiVertexInputRVF & RVF_LIGHTING)
        {
            aElements[uiCount].Type = D3DDECLTYPE_D3DCOLOR;
            aElements[uiCount].Offset = usOffset;
            aElements[uiCount].Usage = D3DDECLUSAGE_COLOR;
            aElements[uiCount].UsageIndex = ucColourIndex;
            ++uiCount;
        }
        usOffset = (unsigned short)(usOffset + 4);
    }

    if (uiVertexBufferRVF & RVF_TEXCOORDS_MASK)
    {
        const unsigned char ucIn = (unsigned char)((uiVertexInputRVF & RVF_TEXCOORDS_MASK) >> RVF_TEXCOORDS_SHIFT);
        const unsigned char ucBuf = (unsigned char)((uiVertexBufferRVF & RVF_TEXCOORDS_MASK) >> RVF_TEXCOORDS_SHIFT);
        const unsigned char ucNum = (ucIn > ucBuf) ? ucBuf : ucIn;

        if (uiVertexBufferRVF & RVF_COMPRESSED_TEXCOORDS)
        {
            for (unsigned char uc = 0; uc < ucNum; uc++)
            {
                aElements[uiCount].UsageIndex = uc;
                aElements[uiCount].Offset = usOffset;
                usOffset = (unsigned short)(usOffset + 4);
                aElements[uiCount].Type = D3DDECLTYPE_SHORT2;
                aElements[uiCount].Usage = D3DDECLUSAGE_TEXCOORD;
                ++uiCount;
            }
        }
        else
        {
            for (unsigned char uc = 0; uc < ucNum; uc++)
            {
                aElements[uiCount].UsageIndex = uc;
                aElements[uiCount].Offset = usOffset;
                usOffset = (unsigned short)(usOffset + 8);
                aElements[uiCount].Type = D3DDECLTYPE_FLOAT2;
                aElements[uiCount].Usage = D3DDECLUSAGE_TEXCOORD;
                ++uiCount;
            }
        }
    }

    const D3DVERTEXELEMENT9 kEnd = D3DDECL_END();
    aElements[uiCount] = kEnd;

    IDirect3DVertexDeclaration9* pDeclaration;
    RedRenderer::pcGetDevice()->CreateVertexDeclaration(aElements, &pDeclaration);
    return pDeclaration;
}

// FUNCTION: SWBF2 0x00714910
unsigned int pcRedVertexFormat::RVFtoFVF(unsigned int uiRVF)
{
    unsigned int uiFVF = 0;

    if (uiRVF & RVF_CUSTOM_DECLARATION)
    {
        return 0;
    }
    if (uiRVF & RVF_TANGENTS)
    {
        return 0;
    }
    if (uiRVF & RVF_COMPRESSED_MASK)
    {
        return 0;
    }

    const unsigned int uiPosition = uiRVF & RVF_POSITION_MASK;
    if (uiPosition == RVF_POSITION_XY)
    {
        return 0;
    }
    if (uiPosition == RVF_POSITION_XYZ)
    {
        uiFVF = D3DFVF_XYZ;
    }
    else if (uiPosition == RVF_POSITION_XYZRHW)
    {
        uiFVF = D3DFVF_XYZRHW;
    }

    if (uiRVF & RVF_WEIGHTS_1)
    {
        uiFVF |= D3DFVF_XYZB1;
    }
    else if (uiRVF & RVF_WEIGHTS_2)
    {
        uiFVF |= D3DFVF_XYZB2;
    }
    else if (uiRVF & RVF_WEIGHTS_3)
    {
        uiFVF |= D3DFVF_XYZB3;
    }
    else if (uiRVF & RVF_WEIGHTS_4)
    {
        uiFVF |= D3DFVF_XYZB4;
    }

    if (uiRVF & RVF_WEIGHTS_MASK)
    {
        uiFVF |= D3DFVF_LASTBETA_UBYTE4;
    }
    if (uiRVF & RVF_NORMAL)
    {
        uiFVF |= D3DFVF_NORMAL;
    }
    if (uiRVF & (RVF_COLOR | RVF_LIGHTING))
    {
        uiFVF |= D3DFVF_DIFFUSE;
    }
    if ((uiRVF & RVF_COLOR) && (uiRVF & RVF_LIGHTING))
    {
        uiFVF |= D3DFVF_SPECULAR;
    }

    if (uiRVF & RVF_TEXCOORDS_1)
    {
        return uiFVF | D3DFVF_TEX1;
    }
    if (uiRVF & RVF_TEXCOORDS_2)
    {
        uiFVF |= D3DFVF_TEX2;
    }

    return uiFVF;
}

// FUNCTION: SWBF2 0x00714310
void pcRedVertexFormat::Cleanup()
{
    while (s_vertexFormatList.GetCount())
    {
        PblList<pcRedVertexFormat>::Node* pNode = s_vertexFormatList.GetFirst();
        pcRedVertexFormat* pFormat = pNode->_pObject;
        s_vertexFormatList.Remove(pNode);
        while (pFormat->Release() > 0)
        {
        }
    }
    s_vertexFormatList.Clear();
}
