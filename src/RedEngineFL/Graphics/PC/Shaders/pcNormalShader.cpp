#include <d3dx9math.h>
#include "pcNormalShader.h"

#include "PebbleFL/Common/PblChunk.h"
#include <string.h>

#include "RedEngineFL/Graphics/PC/pcRedRenderer.h"

#include "RedEngineFL/Graphics/PC/pcRedVertexFormat.h"

#include "PebbleFL/Common/PblTimeBase.h"

#include "RedEngineFL/Graphics/PC/pcRedTexture.h"

#include "RedEngineFL/Memory/engineAlloc.h"

#include <math.h>
#include "RedEngineFL/Graphics/PC/pcRedPrimitive.h"
#include "RedEngineFL/Graphics/RedColor.h"
#include "RedEngineFL/Graphics/RedLight.h"
#include "RedEngineFL/Graphics/RedShadingState.h"
#include "RedEngineFL/Memory/RedMemoryPool.h"
#include "PebbleFL/Common/PblMath.h"
#include "PebbleFL/Common/PblMatrix.h"

#include "RedEngineFL/Graphics/RedSegment.h"
#include "RedEngineFL/Graphics/RedCamera.h"

#pragma init_seg(lib)

// GLOBAL: SWBF2 0x007846d0
// __xmm@3f8000003e4ccccd3e4ccccd3e4ccccd

void __cdecl PblTraceP(const char* Format, ...);


// GLOBAL: SWBF2 0x009c926c
// pcNormalShader::s_uiID
unsigned int pcNormalShader::s_uiID;
// GLOBAL: SWBF2 0x009c9270
// pcNormalShader::GetStateFuncArray
pcRedShader::GetStateFuncT pcNormalShader::GetStateFuncArray[MAX_PIPELINES];
// GLOBAL: SWBF2 0x009c927c
// pcNormalShader::BeginFuncArray
pcRedShader::BeginFuncT pcNormalShader::BeginFuncArray[MAX_PIPELINES];
// GLOBAL: SWBF2 0x009c9288
// pcNormalShader::EndFuncArray
pcRedShader::EndFuncT pcNormalShader::EndFuncArray[MAX_PIPELINES];
// GLOBAL: SWBF2 0x009c9294
// pcNormalShader::RenderFuncArray
pcRedShader::RenderFuncT pcNormalShader::RenderFuncArray[MAX_PIPELINES];

// GLOBAL: SWBF2 0x009c9378
PblTimeBase::Stamp s_animationTimeStamp;

// GLOBAL: SWBF2 0x009c9390
unsigned int PIPELINE1_STATE_MAP[32];

// GLOBAL: SWBF2 0x009c9410
unsigned int PIPELINE1_INV_STATE_MAP[32];

// The features GetState_Prog1 combines; PIPELINE1_STATE_MAP turns a combination into a state index and PIPELINE1_INV_STATE_MAP
// turns it back (HARDEDGE, TRANSPARENT and SHADOW are the words of the modtools build's state names).
enum
{
    PIPELINE1_HARDEDGE = 0x1,      // alpha-tested (MATERIAL_FLAG_ALPHAMAP_MASK)
    PIPELINE1_TRANSPARENT = 0x2,
    PIPELINE1_PROJECTED = 0x4,     // a projected light in the same pass
    PIPELINE1_SHADOW = 0x8,
    PIPELINE1_UNLIT = 0x10         // RENDERFLAG_NO_LIGHTING
};

// GLOBAL: SWBF2 0x009c9490

RedTexture* s_pNullDetailMap;

// GLOBAL: SWBF2 0x009c9498
D3DMATERIAL9 s_d3dMat;

// FUNCTION: SWBF2 0x006fe7e0
pcNormalShader::pcNormalShader()
{
    memset(GetStateFuncArray, 0, sizeof(GetStateFuncArray));
    memset(BeginFuncArray, 0, sizeof(BeginFuncArray));
    memset(EndFuncArray, 0, sizeof(EndFuncArray));
    memset(RenderFuncArray, 0, sizeof(RenderFuncArray));

    s_uiID = _Register(s_uiRenderTypeHash, this);

    _InitPipelineFuncArray();
}

// FUNCTION: SWBF2 0x006fe8b0
pcNormalShader::pcNormalShader(unsigned int uiFlags,
                               const unsigned int* pParams,
                               unsigned int uiNumParams,
                               unsigned int uiMaterialFlags,
                               const RedColor& color0,
                               const RedColor& color1,
                               float fSpecularPower)
{
    _BindPipeline(GetStateFuncArray, BeginFuncArray, EndFuncArray, RenderFuncArray);
    _Init(uiFlags, pParams, uiNumParams, uiMaterialFlags,
          color0, color1, fSpecularPower);
}

// FUNCTION: SWBF2 0x006ff990
void pcNormalShader::InitFixedFunctionShaderDef() const
{
    // GLOBAL: SWBF2 0x00784668
    static const pcRedShaderDef::FixedFunctionStateT aFFStates[5] =
    {
        { 1, { 0x42, 0x00, 0x00, 0x00 } },
        { 1, { 0x42, 0x00, 0x00, 0x00 } },
        { 2, { 0x42, 0x21, 0x00, 0x00 } },
        { 1, { 0x42, 0x00, 0x00, 0x00 } },
        { 2, { 0x42, 0x21, 0x00, 0x00 } },
    };

    GetShaderDef()->InitFixedFunction(0x28, 5, aFFStates);
}

// FUNCTION: SWBF2 0x007011d0
pcRedShader* pcNormalShader::_Create(unsigned int uiFlags,
                                     const unsigned int* pParams,
                                     unsigned int uiNumParams,
                                     unsigned int uiMaterialFlags,
                                     const RedColor& color0,
                                     const RedColor& color1,
                                     float fSpecularPower) const
{
    return new pcNormalShader(uiFlags, pParams, uiNumParams, uiMaterialFlags,
                              color0, color1, fSpecularPower);
}

// FUNCTION: SWBF2 0x00701260
void pcNormalShader::_Init(unsigned int uiFlags,
                           const unsigned int* pParams,
                           unsigned int uiNumParams,
                           unsigned int uiMaterialFlags,
                           const RedColor& color0,
                           const RedColor& color1,
                           float fSpecularPower)
{
    s_pNullDetailMap = RedTexture::s_textureHashTable.Find(PblHash("null_detailmap"));

    unsigned int uiState = RVF_POSITION_XYZ;
    if (uiFlags & RVF_NORMAL)
    {
        uiState = RVF_POSITION_XYZ | RVF_NORMAL;
    }
    if (uiFlags & RVF_WEIGHTS_MASK)
    {
        uiState |= (uiFlags & RVF_WEIGHTS_MASK);
    }
    if (uiFlags & RVF_TEXCOORDS_1)
    {
        uiState |= RVF_TEXCOORDS_1;
    }

    memset(&m_material, 0, sizeof(m_material));

    RedColorValue diffuse;
    diffuse.r = color0.r * (1.0f / 255.0f);
    diffuse.g = color0.g * (1.0f / 255.0f);
    diffuse.b = color0.b * (1.0f / 255.0f);
    diffuse.a = color0.a * (1.0f / 255.0f);
    m_material.diffuseColor = diffuse;

    RedColorValue specular;
    specular.r = color1.r * (1.0f / 255.0f);
    specular.g = color1.g * (1.0f / 255.0f);
    specular.b = color1.b * (1.0f / 255.0f);
    specular.a = color1.a * (1.0f / 255.0f);
    m_material.specularColor = specular;

    m_material.materialFlags = uiMaterialFlags;
    m_material.specularPower = fSpecularPower;

    if (uiNumParams > 0)
    {
        m_material.diffuseMapHash = pParams[0];
    }
    if (uiNumParams > 1)
    {
        m_material.bumpMapHash = pParams[1];
    }
    if (uiNumParams > 2)
    {
        m_material.detailMapHash = pParams[2];
    }
    if (uiNumParams > 3)
    {
        m_material.envMapHash = pParams[3];
    }

    if (uiFlags & RVF_LIGHTING)
    {
        uiState |= RVF_LIGHTING;
    }
    else if (uiFlags & RVF_COLOR)
    {
        uiState |= RVF_COLOR;
    }

    if (uiFlags != 0)
    {
        m_pVertexInputFormat = pcRedVertexFormat::Create(uiFlags, uiState);
        if (m_material.materialFlags & MATERIAL_FLAG_ALPHAMAP_MASK)
        {
            m_pZPrepassVertexInputFormat = pcRedVertexFormat::Create(uiFlags, uiState & (RVF_POSITION_XYZ | RVF_WEIGHTS_MASK | RVF_COLOR | RVF_LIGHTING | RVF_TEXCOORDS_1));
        }
        else
        {
            m_pZPrepassVertexInputFormat = pcRedVertexFormat::Create(uiFlags, uiState & (RVF_POSITION_XYZ | RVF_WEIGHTS_MASK));
        }
    }
    else
    {
        m_pVertexInputFormat = 0;
        m_pZPrepassVertexInputFormat = 0;
    }

    m_material.diffuseTexTransform.m_bTransform = 0;
    m_material.detailTexTransform.m_bTransform = 0;
    m_material.animationData = 0;
    pcRedShader::m_material = &m_material;
    m_lightingInSeparatePass = false;
}

// FUNCTION: SWBF2 0x00701460
void pcNormalShader::_InitPipelineFuncArray()
{
    GetStateFuncArray[PIPELINE_FIXED] =
        (GetStateFuncT)(&pcNormalShader::GetState_Fixed);
    BeginFuncArray[PIPELINE_FIXED] =
        (BeginFuncT)(&pcNormalShader::Begin_Fixed);
    EndFuncArray[PIPELINE_FIXED] =
        (EndFuncT)(&pcNormalShader::End_Fixed);
    RenderFuncArray[PIPELINE_FIXED] =
        (RenderFuncT)(&pcNormalShader::Render_Fixed);

    GetStateFuncArray[PIPELINE_PROG1] =
        (GetStateFuncT)(&pcNormalShader::GetState_Prog1);
    BeginFuncArray[PIPELINE_PROG1] =
        (BeginFuncT)(&pcNormalShader::Begin_Prog1);
    EndFuncArray[PIPELINE_PROG1] =
        (EndFuncT)(&pcNormalShader::End_Prog1);
    RenderFuncArray[PIPELINE_PROG1] =
        (RenderFuncT)(&pcNormalShader::Render_Prog1);
}

// FUNCTION: SWBF2 0x007014c0
void pcNormalShader::_Read(PblFileChunk& chunk) const
{
    while (!chunk.IsDone())
    {
        PblFileChunk child = chunk.ReadNextChild();
        if (child._iId != CHUNK_ID('N', 'A', 'M', 'E'))
        {
            continue;
        }

        char acName[256];
        child.ReadString(acName, 256);
        const unsigned int uiHash = PblHash(acName);
        PblTraceP("pcRedShaderDef [%s]\tHash 0x%08x\n", acName, uiHash);

        pcRedShaderDef* pDef = GetShaderDef();
        if (pDef)
        {
            pDef->Read(chunk);
        }
        else
        {
            PblTraceP("Error: pcRedShaderDef 0x%08x not found\n", uiHash);
        }
    }
}

// FUNCTION: SWBF2 0x007015a0
void pcNormalShader::_StaticInit() const
{
    memset(&s_d3dMat, 0, sizeof(s_d3dMat));

    s_animationTimeStamp.Mark(RedRenderer::GetTimeBase());

    memset(PIPELINE1_STATE_MAP, 0, sizeof(PIPELINE1_STATE_MAP));
    PIPELINE1_STATE_MAP[0] = 4;
    PIPELINE1_STATE_MAP[PIPELINE1_HARDEDGE] = 5;
    PIPELINE1_STATE_MAP[PIPELINE1_TRANSPARENT] = 6;
    PIPELINE1_STATE_MAP[PIPELINE1_HARDEDGE | PIPELINE1_TRANSPARENT] = 7;
    PIPELINE1_STATE_MAP[PIPELINE1_PROJECTED] = 8;
    PIPELINE1_STATE_MAP[PIPELINE1_HARDEDGE | PIPELINE1_PROJECTED] = 9;
    PIPELINE1_STATE_MAP[PIPELINE1_TRANSPARENT | PIPELINE1_PROJECTED] = 10;
    PIPELINE1_STATE_MAP[PIPELINE1_HARDEDGE | PIPELINE1_TRANSPARENT | PIPELINE1_PROJECTED] = 11;
    PIPELINE1_STATE_MAP[PIPELINE1_SHADOW] = 12;
    PIPELINE1_STATE_MAP[PIPELINE1_HARDEDGE | PIPELINE1_SHADOW] = 13;
    PIPELINE1_STATE_MAP[PIPELINE1_PROJECTED | PIPELINE1_SHADOW] = 14;
    PIPELINE1_STATE_MAP[PIPELINE1_HARDEDGE | PIPELINE1_PROJECTED | PIPELINE1_SHADOW] = 15;
    PIPELINE1_STATE_MAP[PIPELINE1_TRANSPARENT | PIPELINE1_SHADOW] = 16;
    PIPELINE1_STATE_MAP[PIPELINE1_HARDEDGE | PIPELINE1_TRANSPARENT | PIPELINE1_SHADOW] = 17;
    PIPELINE1_STATE_MAP[PIPELINE1_TRANSPARENT | PIPELINE1_PROJECTED | PIPELINE1_SHADOW] = 18;
    PIPELINE1_STATE_MAP[PIPELINE1_HARDEDGE | PIPELINE1_TRANSPARENT | PIPELINE1_PROJECTED | PIPELINE1_SHADOW] = 19;
    PIPELINE1_STATE_MAP[PIPELINE1_UNLIT] = 0;
    PIPELINE1_STATE_MAP[PIPELINE1_HARDEDGE | PIPELINE1_UNLIT] = 1;
    PIPELINE1_STATE_MAP[PIPELINE1_TRANSPARENT | PIPELINE1_UNLIT] = 2;
    PIPELINE1_STATE_MAP[PIPELINE1_HARDEDGE | PIPELINE1_TRANSPARENT | PIPELINE1_UNLIT] = 3;

    memset(PIPELINE1_INV_STATE_MAP, 0, sizeof(PIPELINE1_INV_STATE_MAP));
    PIPELINE1_INV_STATE_MAP[4] = 0;
    PIPELINE1_INV_STATE_MAP[5] = PIPELINE1_HARDEDGE;
    PIPELINE1_INV_STATE_MAP[6] = PIPELINE1_TRANSPARENT;
    PIPELINE1_INV_STATE_MAP[7] = PIPELINE1_HARDEDGE | PIPELINE1_TRANSPARENT;
    PIPELINE1_INV_STATE_MAP[8] = PIPELINE1_PROJECTED;
    PIPELINE1_INV_STATE_MAP[9] = PIPELINE1_HARDEDGE | PIPELINE1_PROJECTED;
    PIPELINE1_INV_STATE_MAP[10] = PIPELINE1_TRANSPARENT | PIPELINE1_PROJECTED;
    PIPELINE1_INV_STATE_MAP[11] = PIPELINE1_HARDEDGE | PIPELINE1_TRANSPARENT | PIPELINE1_PROJECTED;
    PIPELINE1_INV_STATE_MAP[12] = PIPELINE1_SHADOW;
    PIPELINE1_INV_STATE_MAP[13] = PIPELINE1_HARDEDGE | PIPELINE1_SHADOW;
    PIPELINE1_INV_STATE_MAP[14] = PIPELINE1_PROJECTED | PIPELINE1_SHADOW;
    PIPELINE1_INV_STATE_MAP[15] = PIPELINE1_HARDEDGE | PIPELINE1_PROJECTED | PIPELINE1_SHADOW;
    PIPELINE1_INV_STATE_MAP[16] = PIPELINE1_TRANSPARENT | PIPELINE1_SHADOW;
    PIPELINE1_INV_STATE_MAP[17] = PIPELINE1_HARDEDGE | PIPELINE1_TRANSPARENT | PIPELINE1_SHADOW;
    PIPELINE1_INV_STATE_MAP[18] = PIPELINE1_TRANSPARENT | PIPELINE1_PROJECTED | PIPELINE1_SHADOW;
    PIPELINE1_INV_STATE_MAP[19] = PIPELINE1_HARDEDGE | PIPELINE1_TRANSPARENT | PIPELINE1_PROJECTED | PIPELINE1_SHADOW;
    PIPELINE1_INV_STATE_MAP[0] = PIPELINE1_UNLIT;
    PIPELINE1_INV_STATE_MAP[1] = PIPELINE1_HARDEDGE | PIPELINE1_UNLIT;
    PIPELINE1_INV_STATE_MAP[2] = PIPELINE1_TRANSPARENT | PIPELINE1_UNLIT;
    PIPELINE1_INV_STATE_MAP[3] = PIPELINE1_HARDEDGE | PIPELINE1_TRANSPARENT | PIPELINE1_UNLIT;
}

static const float kInv255 = 1.0f / 255.0f;

// GLOBAL: SWBF2 0x009c9388

RedColor s_colorDiffusePrev;

// GLOBAL: SWBF2 0x009c9494

bool s_bTextureTransformPrev;

// GLOBAL: SWBF2 0x009c9495

bool s_bDetailTextureTransformPrev;

class PblMatrix;
extern PblMatrix gMatrixIdentity;



// SYNTHETIC: SWBF2 0x006fe9b0
// RedShadingState::`scalar deleting destructor'

// FUNCTION: SWBF2 0x006ff560
RedShadingState* pcNormalShader::CreateShadingState() const
{
    RedShadingState* pState = new RedShadingState(GetRenderTypeHash());
    pState->AddIntParam(0x891891e9 /* "diffuse_texture" */) = m_material.diffuseMapHash;
    pState->AddTextureTransformParam(0x3f0d728a).m_bTransform = 0;
    return pState;
}

// FUNCTION: SWBF2 0x006ff630
void pcNormalShader::End_Fixed(unsigned int, unsigned int) const
{
}

// FUNCTION: SWBF2 0x006ff640
void pcNormalShader::End_Prog1(unsigned int, unsigned int) const
{
}

// FUNCTION: SWBF2 0x006ff670
RenderPhaseT pcNormalShader::GetRenderPhase(const RenderItem* pItem, unsigned int) const
{
    if ((pItem->pRootShader->m_material->materialFlags & (MATERIAL_FLAG_ALPHAMAP_OPACITY | MATERIAL_FLAG_ALPHABLEND_ADDITIVE)) || pItem->tweakColor.a < 0xff)
    {
        return RENDERPHASE_TRANSPARENT;
    }
    return RENDERPHASE_OPAQUE;
}

// FUNCTION: SWBF2 0x006ff6b0
unsigned short pcNormalShader::GetShaderChain(pcRedShader** const ppOut, unsigned short,
                                              const RenderItem* pItem)
{
    unsigned short uiCount = 0;
    const RedMaterial* pMaterial = pItem->pRootShader->m_material;
    m_lightingInSeparatePass = false;
    if (pMaterial->animationData)
    {
        UpdateAnimation();
    }

    if (RedRenderer::pcGetRenderCaps().pipeline == PIPELINE_FIXED || !RedRenderer::pcIsNearScene() ||
        (pItem->flags & (RENDERFLAG_NO_LIGHTING | RENDERFLAG_RENDER_TO_SKY | RENDERFLAG_RENDER_REFLECTION | RENDERFLAG_PSEUDO_REFLECTED)))
    {
        ppOut[0] = this;
        return 1;
    }

    if (!(pMaterial->materialFlags & (MATERIAL_FLAG_ALPHAMAP_OPACITY | MATERIAL_FLAG_ALPHABLEND_ADDITIVE)) && pItem->tweakColor.a >= 0xff &&
        !(pMaterial->diffuseColor.a < 1.0f) && !(pItem->flags & RENDERFLAG_ZBUFFER_ALPHA_FADE) &&
        (pMaterial->materialFlags & MATERIAL_FLAG_PERPIXELLIGHTING) && RedRenderer::pcGetRenderOptions().bumpMapping)
    {
        ppOut[0] = RedRenderer::pcGetSystemShader(SYSTEM_SHADER_PERPIXELDIFFUSELIGHTING);
        uiCount = 1;
        m_lightingInSeparatePass = true;
    }
    ppOut[uiCount++] = this;

    if (RedRenderer::pcGetRenderOptions().specularQuality)
    {
        if (((pMaterial->materialFlags & MATERIAL_FLAG_SPECULAR) && pItem->pLightingState->numSpecularLights > 0) ||
            (pMaterial->materialFlags & MATERIAL_FLAG_ENVMAP))
        {
            ppOut[uiCount++] = RedRenderer::pcGetSystemShader(SYSTEM_SHADER_SPECULARLIGHTING);
        }
    }
    return uiCount;
}

// FUNCTION: SWBF2 0x006ff7d0
unsigned int pcNormalShader::GetState_Fixed(const RenderItem* pItem) const
{
    if (!RedRenderer::pcIsNearScene())
    {
        return 0;
    }

    const RedMaterial* pMaterial = pItem->pRootShader->m_material;
    const bool bShadowed = RedShadowMap::IsShadowMapEnabled() && !(pItem->flags & (RENDERFLAG_POSTSHADOW | RENDERFLAG_NO_LIGHTING | RENDERFLAG_PSEUDO_REFLECTIVE | RENDERFLAG_PSEUDO_REFLECTED));
    const bool bTransparent = (pMaterial->materialFlags & (MATERIAL_FLAG_ALPHAMAP_OPACITY | MATERIAL_FLAG_ALPHABLEND_ADDITIVE)) || pItem->tweakColor.a < 0xff ||
                              pMaterial->diffuseColor.a < 1.0f || (pItem->flags & RENDERFLAG_ZBUFFER_ALPHA_FADE);
    if (bShadowed)
    {
        return bTransparent ? 3 : 4;
    }
    if (bTransparent)
    {
        return 3;
    }
    return (pMaterial->materialFlags & MATERIAL_FLAG_ALPHAMAP_MASK) ? 2 : 1;
}

// FUNCTION: SWBF2 0x006ff870
unsigned int pcNormalShader::GetState_Prog1(const RenderItem* pItem) const
{
    const RedMaterial* pMaterial = pItem->pRootShader->m_material;
    const bool bTransparent = (pMaterial->materialFlags & (MATERIAL_FLAG_ALPHAMAP_OPACITY | MATERIAL_FLAG_ALPHABLEND_ADDITIVE)) || pItem->tweakColor.a < 0xff ||
                              pMaterial->diffuseColor.a < 1.0f || (pItem->flags & RENDERFLAG_ZBUFFER_ALPHA_FADE);
    const bool bShadowed = (!bTransparent || (pItem->flags & RENDERFLAG_PSEUDO_REFLECTIVE)) &&
                           RedShadowMap::IsShadowMapEnabled() && !(pItem->flags & (RENDERFLAG_POSTSHADOW | RENDERFLAG_NO_LIGHTING | RENDERFLAG_RENDER_TO_SKY | RENDERFLAG_PSEUDO_REFLECTED));
    const bool bSeparate = m_lightingInSeparatePass && !(pItem->flags & RENDERFLAG_PSEUDO_REFLECTED);
    const bool bProjected = pItem->pLightingState->pProjectedLight && !bSeparate &&
                            !(pItem->flags & (RENDERFLAG_NO_LIGHTING | RENDERFLAG_RENDER_TO_SKY | RENDERFLAG_PSEUDO_REFLECTIVE | RENDERFLAG_PSEUDO_REFLECTED));

    unsigned int uiState = bTransparent ? PIPELINE1_TRANSPARENT : 0;
    if (pMaterial->materialFlags & MATERIAL_FLAG_ALPHAMAP_MASK)
    {
        uiState |= PIPELINE1_HARDEDGE;
    }
    if (bProjected)
    {
        uiState |= PIPELINE1_PROJECTED;
    }
    if (bShadowed)
    {
        uiState |= PIPELINE1_SHADOW;
    }
    if (pItem->flags & RENDERFLAG_NO_LIGHTING)
    {
        uiState |= PIPELINE1_UNLIT;
    }
    return PIPELINE1_STATE_MAP[uiState];
}

// FUNCTION: SWBF2 0x006ff950
int pcNormalShader::GetDiffuseMapHash(int iItem, int)
{
    const RenderItem* pItem = (const RenderItem*)iItem;
    if (!pItem)
    {
        return 0;
    }
    const RedMaterial* pMaterial = pItem->pRootShader->m_material;
    if (pItem->pShadingState)
    {
        return pItem->pShadingState->GetIntParam(0x891891e9 /* "diffuse_texture" */);
    }
    return pMaterial->diffuseMapHash;
}

// FUNCTION: SWBF2 0x006ff980
unsigned int pcNormalShader::GetZPrepassState() const
{
    return ((unsigned char)m_material.materialFlags & MATERIAL_FLAG_ALPHAMAP_MASK) != 0;
}

// FUNCTION: SWBF2 0x006ff9b0
bool pcNormalShader::IsTransparent() const
{
    return (m_material.materialFlags & (MATERIAL_FLAG_ALPHAMAP_OPACITY | MATERIAL_FLAG_ALPHABLEND_ADDITIVE)) != 0;
}

// FUNCTION: SWBF2 0x00700e20
bool pcNormalShader::SortAsZWritingTransparent() const
{
    return (m_material.materialFlags & (MATERIAL_FLAG_ALPHAMAP_MASK | MATERIAL_FLAG_ALPHAMAP_OPACITY | MATERIAL_FLAG_ALPHABLEND_ADDITIVE)) == 0;
}

// FUNCTION: SWBF2 0x006ff9c0
void pcNormalShader::RenderZPrepass(const RenderItem* pItem) const
{
    const RedMaterial* pMaterial = pItem->pRootShader->m_material;
    RedRenderer::pcSetCullMode((pMaterial->materialFlags & MATERIAL_FLAG_CULL_NONE) ? PCREDCULL_NONE : PCREDCULL_BACK);
    if (pItem->flags & RENDERFLAG_NO_Z)
    {
        RedRenderer::pcSetRenderState(D3DRS_ZWRITEENABLE, FALSE);
    }
    else
    {
        RedRenderer::pcSetRenderState(D3DRS_ZWRITEENABLE, TRUE);
    }

    if (GetZPrepassState() == 1)
    {
        RedRenderer::pcSetCullMode((pMaterial->materialFlags & MATERIAL_FLAG_CULL_NONE) ? PCREDCULL_NONE : PCREDCULL_BACK);

        const unsigned int uiDiffuseMap = pItem->pShadingState
                                              ? pItem->pShadingState->GetIntParam(0x891891e9 /* "diffuse_texture" */)
                                              : pMaterial->diffuseMapHash;
        const RedTexture* pTexture;
        if (uiDiffuseMap)
        {
            pTexture = RedTexture::Find(uiDiffuseMap);
            if (pTexture)
            {
                RedRenderer::pcSetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
                RedRenderer::pcSetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
            }
        }
        else
        {
            pTexture = 0;
        }
        RedRenderer::pcSetTexture(0, pTexture);

        const RedTextureTransform* pTransform = &pMaterial->diffuseTexTransform;
        if (pItem->pShadingState)
        {
            pTransform = &pItem->pShadingState->GetTextureTransformParam(0x3f0d728a);
        }
        if (!pTransform->m_bTransform)
        {
            pTransform = 0;
        }

        RedRenderer::pcSetTexture(0, pTexture);
        if (pTexture)
        {
            RedRenderer::pcSetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
            RedRenderer::pcSetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
        }
        RedRenderer::pcSetRenderState(D3DRS_ALPHAREF, 0x7f);

        RedColor materialColor = pMaterial->diffuseColor;
        const RedColor color = pItem->tweakColor * materialColor;

        pcRedVertexShaderConstantRegister* pRegs = RedRenderer::pcLockVertexShaderConstants(39, 3);
        pRegs[39].SetColor(RedColorValue(color));
        if (pTransform)
        {
            const float fTx = (*pTransform)(2, 0);
            const float fYx = (*pTransform)(1, 0);
            const float fXx = (*pTransform)(0, 0);
            pRegs[40].component[0] = fXx;
            pRegs[40].component[1] = fYx;
            pRegs[40].component[2] = 0.0f;
            pRegs[40].component[3] = fTx;
            const float fTy = (*pTransform)(2, 1);
            const float fYy = (*pTransform)(1, 1);
            const float fXy = (*pTransform)(0, 1);
            pRegs[41].component[0] = fXy;
            pRegs[41].component[1] = fYy;
            pRegs[41].component[3] = fTy;
        }
        else
        {
            pRegs[40].component[0] = 1.0f;
            pRegs[40].component[1] = 0.0f;
            pRegs[40].component[2] = 0.0f;
            pRegs[40].component[3] = 0.0f;
            pRegs[41].component[0] = 0.0f;
            pRegs[41].component[1] = 1.0f;
            pRegs[41].component[3] = 0.0f;
        }
        pRegs[41].component[2] = 0.0f;
        RedRenderer::pcUnlockVertexShaderConstants(pRegs);
    }

    RedRenderer::pcLoadFormat(m_pZPrepassVertexInputFormat ? m_pZPrepassVertexInputFormat
                                                           : pItem->pPrim->m_vertexFormat);
    pItem->pPrim->Draw();
}

// SYNTHETIC: SWBF2 0x004097f0
// `dynamic initializer for 'pcNormalShader::s_uiRenderTypeHash''

// GLOBAL: SWBF2 0x009c938c
// pcNormalShader::s_uiRenderTypeHash
unsigned int pcNormalShader::s_uiRenderTypeHash = PblHash("Normal");

// SYNTHETIC: SWBF2 0x004097b0
// `dynamic initializer for 'pcNormalShader::s_Factory''
// SYNTHETIC: SWBF2 0x00769c30
// `dynamic atexit destructor for 'pcNormalShader::s_Factory''

// GLOBAL: SWBF2 0x009c92a8
// pcNormalShader::s_Factory
pcNormalShader pcNormalShader::s_Factory;

// SYNTHETIC: SWBF2 0x004097d0
// `dynamic initializer for 'pcNormalShader::s_shaderDef''
// SYNTHETIC: SWBF2 0x00769c50
// `dynamic atexit destructor for 'pcNormalShader::s_shaderDef''

// GLOBAL: SWBF2 0x009c934c
// pcNormalShader::s_shaderDef
// STRING: SWBF2 0x00784650
pcRedShaderDef pcNormalShader::s_shaderDef("normal_shader.xml");

// SYNTHETIC: SWBF2 0x00409850
// `dynamic initializer for 'kuiLinearRamp''

// GLOBAL: SWBF2 0x009c92a4
// kuiLinearRamp
static unsigned int kuiLinearRamp = PblHash("linear_ramp");

// FUNCTION: SWBF2 0x006fea10
void pcNormalShader::Begin_Fixed(unsigned int uiState, unsigned int uiPass) const
{
    RedRenderer::pcSetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
    RedRenderer::pcSetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
    RedRenderer::pcSetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

    switch (uiState)
    {
    case 0:
        RedRenderer::pcSetRenderState(D3DRS_COLORWRITEENABLE, 7);
        RedRenderer::pcSetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
        RedRenderer::pcSetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
        RedRenderer::pcSetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
        RedRenderer::pcSetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
        RedRenderer::pcSetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
        RedRenderer::pcSetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);
        RedRenderer::pcSetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TEXTURE);
        RedRenderer::pcSetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
        RedRenderer::pcSetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
        RedRenderer::pcSetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TEXTURE);
        RedRenderer::pcSetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
        RedRenderer::pcSetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
        return;

    case 1:
        RedRenderer::pcSetRenderState(D3DRS_COLORWRITEENABLE, 0xf);
        RedRenderer::pcSetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
        RedRenderer::pcSetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
        RedRenderer::pcSetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
        RedRenderer::pcSetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
        RedRenderer::pcSetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
        RedRenderer::pcSetTexture(1, RedTexture::Find(kuiLinearRamp));
        RedRenderer::pcSetSamplerState(1, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
        RedRenderer::pcSetSamplerState(1, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
        RedRenderer::pcSetSamplerState(1, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
        RedRenderer::pcSetSamplerState(1, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
        RedRenderer::pcSetSamplerState(1, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
        RedRenderer::pcSetTextureStageState(1, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2);
        RedRenderer::pcSetTextureStageState(1, D3DTSS_TEXCOORDINDEX, D3DTSS_TCI_CAMERASPACEPOSITION);
        RedRenderer::pcFFSetTextureTransform(1, RedRenderer::pcGetNearSceneFadeTextureTransform());
        RedRenderer::pcSetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
        RedRenderer::pcSetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);
        RedRenderer::pcSetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TEXTURE);
        RedRenderer::pcSetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
        RedRenderer::pcSetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_CURRENT);
        RedRenderer::pcSetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
        RedRenderer::pcSetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_CURRENT);
        RedRenderer::pcSetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
        RedRenderer::pcSetTextureStageState(1, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
        RedRenderer::pcSetTextureStageState(2, D3DTSS_COLOROP, D3DTOP_DISABLE);
        RedRenderer::pcSetTextureStageState(2, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
        return;

    case 2:
        if (uiPass == 0)
        {
            RedRenderer::pcSetRenderState(D3DRS_COLORWRITEENABLE, 7);
            RedRenderer::pcSetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
            RedRenderer::pcSetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
            RedRenderer::pcSetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
            RedRenderer::pcSetRenderState(D3DRS_ALPHAREF, 0x7f);
            RedRenderer::pcSetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
            RedRenderer::pcSetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
            RedRenderer::pcSetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
            RedRenderer::pcSetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
            RedRenderer::pcSetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);
            RedRenderer::pcSetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TEXTURE);
            RedRenderer::pcSetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
            RedRenderer::pcSetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
            RedRenderer::pcSetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TEXTURE);
            RedRenderer::pcSetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
            RedRenderer::pcSetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
            return;
        }
        break;

    case 3:
        RedRenderer::pcSetRenderState(D3DRS_COLORWRITEENABLE, 7);
        RedRenderer::pcSetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
        RedRenderer::pcSetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
        RedRenderer::pcSetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
        RedRenderer::pcSetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
        RedRenderer::pcSetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
        RedRenderer::pcSetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
        RedRenderer::pcSetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
        RedRenderer::pcSetTexture(1, RedTexture::Find(kuiLinearRamp));
        RedRenderer::pcSetSamplerState(1, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
        RedRenderer::pcSetSamplerState(1, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
        RedRenderer::pcSetSamplerState(1, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
        RedRenderer::pcSetSamplerState(1, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
        RedRenderer::pcSetSamplerState(1, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
        RedRenderer::pcSetTextureStageState(1, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2);
        RedRenderer::pcSetTextureStageState(1, D3DTSS_TEXCOORDINDEX, D3DTSS_TCI_CAMERASPACEPOSITION);
        RedRenderer::pcFFSetTextureTransform(1, RedRenderer::pcGetNearSceneFadeTextureTransform());
        RedRenderer::pcSetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
        RedRenderer::pcSetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);
        RedRenderer::pcSetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TEXTURE);
        RedRenderer::pcSetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
        RedRenderer::pcSetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
        RedRenderer::pcSetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TEXTURE);
        RedRenderer::pcSetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
        RedRenderer::pcSetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_CURRENT);
        RedRenderer::pcSetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
        RedRenderer::pcSetTextureStageState(1, D3DTSS_ALPHAARG1, D3DTA_CURRENT);
        RedRenderer::pcSetTextureStageState(1, D3DTSS_ALPHAARG2, D3DTA_TEXTURE);
        RedRenderer::pcSetTextureStageState(2, D3DTSS_COLOROP, D3DTOP_DISABLE);
        RedRenderer::pcSetTextureStageState(2, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
        return;

    case 4:
    {
        if (uiPass == 0)
        {
            RedRenderer::pcSetRenderState(D3DRS_COLORWRITEENABLE, 7);
            RedRenderer::pcSetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
            RedRenderer::pcSetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
            RedRenderer::pcSetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
            RedRenderer::pcSetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
            RedRenderer::pcSetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
            RedRenderer::pcSetTexture(1, RedShadowMap::GetShadowMapTexture());
            RedRenderer::pcSetSamplerState(1, D3DSAMP_ADDRESSU, RedShadowMap::GetShadowMapWrapMode());
            RedRenderer::pcSetSamplerState(1, D3DSAMP_ADDRESSV, RedShadowMap::GetShadowMapWrapMode());
            RedRenderer::pcSetSamplerState(1, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
            RedRenderer::pcSetSamplerState(1, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
            RedRenderer::pcSetSamplerState(1, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
            RedRenderer::pcSetTextureStageState(1, D3DTSS_TEXTURETRANSFORMFLAGS,
                                                D3DTTFF_COUNT3 | D3DTTFF_PROJECTED);
            RedRenderer::pcSetTextureStageState(1, D3DTSS_TEXCOORDINDEX, D3DTSS_TCI_CAMERASPACEPOSITION);

            PblMatrix mCamera = RedRenderer::GetCamera()->GetMatrix();
            mCamera.SetZAxis(-mCamera[2]);
            const PblVector4& vU = RedShadowMap::_vShadowMapTransformU;
            const PblVector4& vV = RedShadowMap::_vShadowMapTransformV;
            const PblVector4& vW = RedShadowMap::_vShadowMapTransformW;
            PblMatrix mShadow(PblVector3(vU.x, vV.x, vW.x), PblVector3(vU.y, vV.y, vW.y),
                              PblVector3(vU.z, vV.z, vW.z), PblVector3(vU.w, vV.w, vW.w));
            RedRenderer::pcFFSetTextureTransform(1, mCamera * mShadow);

            RedRenderer::pcSetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
            RedRenderer::pcSetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);
            RedRenderer::pcSetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TEXTURE);
            RedRenderer::pcSetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
            RedRenderer::pcSetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
            RedRenderer::pcSetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_MODULATE);
            RedRenderer::pcSetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_CURRENT);
            RedRenderer::pcSetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_TEXTURE);
            RedRenderer::pcSetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
            RedRenderer::pcSetTextureStageState(1, D3DTSS_ALPHAARG1, D3DTA_CURRENT);
            RedRenderer::pcSetTextureStageState(2, D3DTSS_COLOROP, D3DTOP_DISABLE);
            RedRenderer::pcSetTextureStageState(2, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
            return;
        }
        break;
    }

    default:
        return;
    }

    RedRenderer::pcSetRenderState(D3DRS_FOGENABLE, FALSE);
    RedRenderer::pcSetRenderState(D3DRS_COLORWRITEENABLE, 8);
    RedRenderer::pcSetRenderState(D3DRS_ZWRITEENABLE, FALSE);
    RedRenderer::pcSetRenderState(D3DRS_ZFUNC, D3DCMP_EQUAL);
    RedRenderer::pcSetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
    RedRenderer::pcSetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
    RedRenderer::pcSetTexture(0, RedTexture::Find(kuiLinearRamp));
    RedRenderer::pcSetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
    RedRenderer::pcSetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
    RedRenderer::pcSetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2);
    RedRenderer::pcSetTextureStageState(0, D3DTSS_TEXCOORDINDEX, D3DTSS_TCI_CAMERASPACEPOSITION);
    RedRenderer::pcFFSetTextureTransform(0, RedRenderer::pcGetNearSceneFadeTextureTransform());
    RedRenderer::pcSetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
    RedRenderer::pcSetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    RedRenderer::pcSetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
    RedRenderer::pcSetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
    RedRenderer::pcSetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
    RedRenderer::pcSetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
}

// FUNCTION: SWBF2 0x006ff280
void pcNormalShader::Begin_Prog1(unsigned int uiState, unsigned int) const
{
    const unsigned int uiFlags = PIPELINE1_INV_STATE_MAP[uiState];
    RedRenderer::pcSetRenderState(D3DRS_COLORWRITEENABLE,
                                  (RedRenderer::pcIsNearScene() && !(uiFlags & PIPELINE1_TRANSPARENT)) ? 0xf : 7);
    RedRenderer::pcSetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

    RedRenderer::pcSetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
    RedRenderer::pcSetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
    RedRenderer::pcSetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
    RedRenderer::pcSetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
    RedRenderer::pcSetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
    RedRenderer::pcSetSamplerState(1, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
    RedRenderer::pcSetSamplerState(1, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
    RedRenderer::pcSetSamplerState(1, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
    RedRenderer::pcSetSamplerState(1, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
    RedRenderer::pcSetSamplerState(1, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

    if (uiFlags & PIPELINE1_PROJECTED)
    {
        RedRenderer::pcSetTextureStageState(2, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_PROJECTED);
        RedRenderer::pcSetSamplerState(2, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
        RedRenderer::pcSetSamplerState(2, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
        RedRenderer::pcSetSamplerState(2, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
    }

    if (uiFlags & PIPELINE1_SHADOW)
    {
        RedRenderer::pcSetTexture(3, RedShadowMap::GetShadowMapTexture());
        RedRenderer::pcSetSamplerState(3, D3DSAMP_ADDRESSU, RedShadowMap::GetShadowMapWrapMode());
        RedRenderer::pcSetSamplerState(3, D3DSAMP_ADDRESSV, RedShadowMap::GetShadowMapWrapMode());
        RedRenderer::pcSetTextureStageState(3, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_PROJECTED);
        RedRenderer::pcSetSamplerState(3, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
        RedRenderer::pcSetSamplerState(3, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
        RedRenderer::pcSetSamplerState(3, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
    }

    pcRedVertexShaderConstantRegister* pRegs = RedRenderer::pcLockVertexShaderConstants(39, 6);
    pRegs[39].SetColor(RedColorValue(RedColor::WHITE));
    pRegs[40].SetFloats(1.0f, 0.0f, 0.0f, 0.0f);
    pRegs[41].SetFloats(1.0f, 0.0f, 0.0f, 0.0f);
    pRegs[42].SetFloats(0.0f, 1.0f, 0.0f, 0.0f);
    pRegs[43].SetFloats(1.0f, 0.0f, 0.0f, 0.0f);
    pRegs[44].SetFloats(0.0f, 1.0f, 0.0f, 0.0f);
    RedRenderer::pcUnlockVertexShaderConstants(pRegs);

    s_colorDiffusePrev = RedColor::WHITE;
    s_bTextureTransformPrev = false;
    s_bDetailTextureTransformPrev = false;
}

// FUNCTION: SWBF2 0x006ffd60
void pcNormalShader::Render_Fixed(const RenderItem* pItem, unsigned int uiPass) const
{
    const RedMaterial* pMaterial = pItem->pRootShader->m_material;

    RedColor materialColor = pMaterial->diffuseColor;
    const RedColor color = pItem->tweakColor * materialColor;

    s_d3dMat.Diffuse.r = color.r * kInv255;
    s_d3dMat.Diffuse.g = color.g * kInv255;
    s_d3dMat.Diffuse.b = color.b * kInv255;
    s_d3dMat.Diffuse.a = color.a * kInv255;
    s_d3dMat.Ambient = s_d3dMat.Diffuse;
    RedRenderer::pcGetDevice()->SetMaterial(&s_d3dMat);

    const unsigned int uiRVF = pItem->pPrim->m_vertexFormat->m_uiVertexBufferRVF;
    if (uiRVF & RVF_LIGHTING)
    {
        RedRenderer::pcSetRenderState(D3DRS_COLORVERTEX, TRUE);
        RedRenderer::pcSetRenderState(D3DRS_EMISSIVEMATERIALSOURCE, D3DMCS_COLOR1);
        RedRenderer::pcSetRenderState(D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_MATERIAL);
        RedRenderer::pcSetRenderState(D3DRS_AMBIENTMATERIALSOURCE, D3DMCS_MATERIAL);
    }
    else if (uiRVF & RVF_COLOR)
    {
        RedRenderer::pcSetRenderState(D3DRS_COLORVERTEX, TRUE);
        RedRenderer::pcSetRenderState(D3DRS_EMISSIVEMATERIALSOURCE, D3DMCS_MATERIAL);
        RedRenderer::pcSetRenderState(D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_COLOR1);
        RedRenderer::pcSetRenderState(D3DRS_AMBIENTMATERIALSOURCE, D3DMCS_COLOR1);
        const D3DCOLOR ambient = color.GetARGB();
        if (ambient != RedColor::WHITE.GetARGB() && !(pItem->uiEnvState & 0x1f))
        {
            RedRenderer::pcSetRenderState(D3DRS_AMBIENT, ambient);
        }
    }
    else
    {
        RedRenderer::pcSetRenderState(D3DRS_COLORVERTEX, FALSE);
    }

    RedRenderer::pcSetCullMode((pMaterial->materialFlags & MATERIAL_FLAG_CULL_NONE) ? PCREDCULL_NONE : PCREDCULL_BACK);

    bool bTexture = true;
    switch (pItem->uiShaderState)
    {
    case 0:
    {
        const unsigned int uiMatFlags = pMaterial->materialFlags;
        const bool bBlend = (uiMatFlags & (MATERIAL_FLAG_ALPHAMAP_OPACITY | MATERIAL_FLAG_ALPHABLEND_ADDITIVE)) || color.a < 0xff || (pItem->flags & RENDERFLAG_ZBUFFER_ALPHA_FADE);
        const bool bAlphaTest = (uiMatFlags & MATERIAL_FLAG_ALPHAMAP_MASK) != 0;
        const bool bTransparent = (uiMatFlags & (MATERIAL_FLAG_ALPHAMAP_MASK | MATERIAL_FLAG_ALPHAMAP_OPACITY | MATERIAL_FLAG_ALPHABLEND_ADDITIVE)) != 0;
        if (bBlend)
        {
            const bool bZWrite = !bTransparent && !(pItem->flags & RENDERFLAG_NO_Z);
            RedRenderer::pcSetRenderState(D3DRS_ZWRITEENABLE, bZWrite ? TRUE : FALSE);
            RedRenderer::pcSetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
            RedRenderer::pcSetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
            if (pMaterial->materialFlags & MATERIAL_FLAG_ALPHABLEND_ADDITIVE)
            {
                RedRenderer::pcSetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
                RedRenderer::pcSetFogForBlendMode(RedRenderer::FOGFORBLEND_ADD);
            }
            else
            {
                RedRenderer::pcSetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
                RedRenderer::pcSetFogForBlendMode(RedRenderer::FOGFORBLEND_NORMAL);
            }
        }
        else
        {
            RedRenderer::pcSetRenderState(D3DRS_ZWRITEENABLE, (pItem->flags & RENDERFLAG_NO_Z) ? FALSE : TRUE);
            RedRenderer::pcSetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
        }
        if (bTransparent)
        {
            RedRenderer::pcSetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
            RedRenderer::pcSetRenderState(D3DRS_ALPHAREF, bAlphaTest ? color.a >> 1 : 1);
        }
        else
        {
            RedRenderer::pcSetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
        }
        break;
    }

    case 2:
        if (uiPass)
        {
            bTexture = false;
            break;
        }

    case 1:
        RedRenderer::pcSetRenderState(D3DRS_ZWRITEENABLE, (pItem->flags & RENDERFLAG_NO_Z) ? FALSE : TRUE);
        break;

    case 3:
    {
        const bool bAlphaTest = (pMaterial->materialFlags & MATERIAL_FLAG_ALPHAMAP_MASK) != 0;
        const bool bTransparent = (pMaterial->materialFlags & (MATERIAL_FLAG_ALPHAMAP_MASK | MATERIAL_FLAG_ALPHAMAP_OPACITY | MATERIAL_FLAG_ALPHABLEND_ADDITIVE)) != 0;
        const bool bZWrite = !bTransparent && !(pItem->flags & RENDERFLAG_NO_Z);
        RedRenderer::pcSetRenderState(D3DRS_ZWRITEENABLE, bZWrite ? TRUE : FALSE);
        if (pMaterial->materialFlags & MATERIAL_FLAG_ALPHABLEND_ADDITIVE)
        {
            RedRenderer::pcSetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
            RedRenderer::pcSetFogForBlendMode(RedRenderer::FOGFORBLEND_ADD);
        }
        else
        {
            RedRenderer::pcSetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
            RedRenderer::pcSetFogForBlendMode(RedRenderer::FOGFORBLEND_NORMAL);
        }
        if (bTransparent)
        {
            RedRenderer::pcSetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
            RedRenderer::pcSetRenderState(D3DRS_ALPHAREF, bAlphaTest ? color.a >> 1 : 1);
        }
        else
        {
            RedRenderer::pcSetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
        }
        break;
    }

    case 4:
    {
        if (uiPass)
        {
            bTexture = false;
            break;
        }
        const bool bAlphaTest = (pMaterial->materialFlags & MATERIAL_FLAG_ALPHAMAP_MASK) != 0;
        if (bAlphaTest)
        {
            RedRenderer::pcSetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
            RedRenderer::pcSetRenderState(D3DRS_ALPHAREF, 0x7f);
        }
        else
        {
            RedRenderer::pcSetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
        }
        RedRenderer::pcSetRenderState(D3DRS_ZWRITEENABLE, (pItem->flags & RENDERFLAG_NO_Z) ? FALSE : TRUE);
        break;
    }

    default:
        bTexture = false;
        break;
    }

    if (bTexture)
    {
        unsigned int uiDiffuseMap;
        const RedTextureTransform* pTransform = 0;
        if (pItem->pShadingState)
        {
            uiDiffuseMap = pItem->pShadingState->GetIntParam(0x891891e9 /* "diffuse_texture" */);
            pTransform = &pItem->pShadingState->GetTextureTransformParam(0x3f0d728a);
            if (!pTransform->m_bTransform)
            {
                pTransform = 0;
            }
        }
        else
        {
            uiDiffuseMap = pMaterial->diffuseMapHash;
        }
        RedRenderer::pcSetTexture(0, uiDiffuseMap ? RedTexture::Find(uiDiffuseMap) : 0);

        if (pTransform)
        {
            PblMatrix mTransform = gMatrixIdentity;
            mTransform.SetXAxis((*pTransform)(0, 0), (*pTransform)(0, 1), 0.0f, 0.0f);
            mTransform.SetYAxis((*pTransform)(1, 0), (*pTransform)(1, 1), 0.0f, 0.0f);
            mTransform.SetZAxis((*pTransform)(2, 0), (*pTransform)(2, 1), 1.0f, 0.0f);
            RedRenderer::pcSetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2);
            RedRenderer::pcFFSetTextureTransform(0, mTransform);
        }
        else
        {
            RedRenderer::pcSetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
        }
    }

    RedRenderer::pcLoadFormat(m_pVertexInputFormat ? m_pVertexInputFormat : pItem->pPrim->m_vertexFormat);
    pItem->pPrim->Draw();
}

// FUNCTION: SWBF2 0x007003c0
void pcNormalShader::Render_Prog1(const RenderItem* pItem, unsigned int) const
{
    const unsigned int uiFlags = PIPELINE1_INV_STATE_MAP[pItem->uiShaderState];
    RedRenderer::pcLoadFormat(m_pVertexInputFormat ? m_pVertexInputFormat : pItem->pPrim->m_vertexFormat);

    const RedMaterial* pMaterial = pItem->pRootShader->m_material;
    const bool bBlend = (uiFlags & PIPELINE1_TRANSPARENT) != 0;
    const unsigned int uiMatFlags = pMaterial->materialFlags;
    const bool bAlphaTest = (uiMatFlags & MATERIAL_FLAG_ALPHAMAP_MASK) != 0;
    const bool bTransparent = (uiMatFlags & (MATERIAL_FLAG_ALPHAMAP_MASK | MATERIAL_FLAG_ALPHAMAP_OPACITY | MATERIAL_FLAG_ALPHABLEND_ADDITIVE)) != 0;
    const bool bGlow = (uiMatFlags & MATERIAL_FLAG_GLOW) != 0;

    RedColor materialColor = pMaterial->diffuseColor;
    const RedColor color = pItem->tweakColor * materialColor;

    const bool bSeparate = m_lightingInSeparatePass && !(pItem->flags & RENDERFLAG_PSEUDO_REFLECTED);
    RedRenderer::pcSetCullMode((uiMatFlags & MATERIAL_FLAG_CULL_NONE) ? PCREDCULL_NONE : PCREDCULL_BACK);

    if (bBlend)
    {
        const bool bZWrite = !bTransparent && !(pItem->flags & RENDERFLAG_NO_Z);
        RedRenderer::pcSetRenderState(D3DRS_ZWRITEENABLE, bZWrite);
        RedRenderer::pcSetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
        RedRenderer::pcSetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
        if (pMaterial->materialFlags & MATERIAL_FLAG_ALPHABLEND_ADDITIVE)
        {
            RedRenderer::pcSetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
            RedRenderer::pcSetFogForBlendMode(RedRenderer::FOGFORBLEND_ADD);
        }
        else
        {
            RedRenderer::pcSetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
            RedRenderer::pcSetFogForBlendMode(RedRenderer::FOGFORBLEND_NORMAL);
        }
        RedColorValue pixelShaderConstant;
        pixelShaderConstant.b = bTransparent ? 1.0f : 0.0f;
        pixelShaderConstant.a = bGlow ? 1.0f : 0.0f;
        RedRenderer::pcGetDevice()->SetPixelShaderConstantF(0, &pixelShaderConstant.r, 1);
    }
    else if (bSeparate)
    {
        RedRenderer::pcSetRenderState(D3DRS_ZWRITEENABLE, (pItem->flags & RENDERFLAG_NO_Z) ? FALSE : TRUE);
        RedRenderer::pcSetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
        RedRenderer::pcSetRenderState(D3DRS_SRCBLEND, D3DBLEND_DESTCOLOR);
        RedRenderer::pcSetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO);
        RedRenderer::pcSetFogForBlendMode(RedRenderer::FOGFORBLEND_NORMAL);
        RedColorValue pixelShaderConstant;
        pixelShaderConstant.b = bTransparent ? 1.0f : 0.0f;
        pixelShaderConstant.a = bGlow ? 1.0f : 0.0f;
        RedRenderer::pcGetDevice()->SetPixelShaderConstantF(0, &pixelShaderConstant.r, 1);
    }
    else
    {
        RedColorValue pixelShaderConstant;
        pixelShaderConstant.b = 0.0f;
        pixelShaderConstant.a = bGlow ? 1.0f : 0.0f;
        RedRenderer::pcGetDevice()->SetPixelShaderConstantF(0, &pixelShaderConstant.r, 1);
        RedRenderer::pcSetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
        RedRenderer::pcSetRenderState(D3DRS_ZWRITEENABLE, (pItem->flags & RENDERFLAG_NO_Z) ? FALSE : TRUE);
        RedRenderer::pcSetFogForBlendMode(RedRenderer::FOGFORBLEND_NORMAL);
    }

    if (bAlphaTest)
    {
        RedRenderer::pcSetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
        RedRenderer::pcSetRenderState(D3DRS_ALPHAREF, 1);
    }
    else
    {
        RedRenderer::pcSetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
    }

    unsigned int uiDiffuseMap;
    const RedTextureTransform* pTransform = &pMaterial->diffuseTexTransform;
    if (pItem->pShadingState)
    {
        uiDiffuseMap = pItem->pShadingState->GetIntParam(0x891891e9 /* "diffuse_texture" */);
        pTransform = &pItem->pShadingState->GetTextureTransformParam(0x3f0d728a);
    }
    else
    {
        uiDiffuseMap = pMaterial->diffuseMapHash;
    }
    const RedTexture* pDiffuse = uiDiffuseMap ? RedTexture::Find(uiDiffuseMap) : 0;
    const RedTexture* pDetail = pMaterial->detailMapHash ? RedTexture::Find(pMaterial->detailMapHash)
                                                          : s_pNullDetailMap;
    RedRenderer::pcSetTexture(0, pDiffuse);
    RedRenderer::pcSetTexture(1, pDetail);

    const RedTextureTransform& detail = pMaterial->detailTexTransform;
    pcRedVertexShaderConstantRegister* pRegs;
    if (!pTransform->m_bTransform && !detail.m_bTransform && !s_bTextureTransformPrev && !s_bDetailTextureTransformPrev &&
        D3DCOLOR_ARGB(color.a, color.r, color.g, color.b) ==
            D3DCOLOR_ARGB(s_colorDiffusePrev.a, s_colorDiffusePrev.r, s_colorDiffusePrev.g, s_colorDiffusePrev.b))
    {
        pRegs = RedRenderer::pcLockVertexShaderConstants(40, 1);
        if (bSeparate)
        {
            const float fLightingScale = RedRenderer::GetLightingScale();
            pRegs[40].SetFloats(0.0f, fLightingScale, 0.0f, 0.0f);
        }
        else
        {
            pRegs[40].SetFloats(1.0f, 0.0f, 0.0f, 0.0f);
        }
    }
    else
    {
        s_bTextureTransformPrev = pTransform->m_bTransform != 0;
        s_bDetailTextureTransformPrev = detail.m_bTransform != 0;
        s_colorDiffusePrev = color;

        pRegs = RedRenderer::pcLockVertexShaderConstants(39, 6);
        pRegs[39].SetColor(RedColorValue(color));
        if (bSeparate)
        {
            const float fLightingScale = RedRenderer::GetLightingScale();
            pRegs[40].SetFloats(0.0f, fLightingScale, 0.0f, 0.0f);
        }
        else
        {
            pRegs[40].SetFloats(1.0f, 0.0f, 0.0f, 0.0f);
        }

        pRegs[41].SetFloats((*pTransform)(0, 0), (*pTransform)(1, 0), 0.0f, (*pTransform)(2, 0));
        pRegs[42].SetFloats((*pTransform)(0, 1), (*pTransform)(1, 1), 0.0f, (*pTransform)(2, 1));
        pRegs[43].SetFloats(detail(0, 0), detail(1, 0), 0.0f, detail(2, 0));
        pRegs[44].SetFloats(detail(0, 1), detail(1, 1), 0.0f, detail(2, 1));
    }
    RedRenderer::pcUnlockVertexShaderConstants(pRegs);

    if (uiFlags & PIPELINE1_PROJECTED)
    {
        RedRenderer::pcSetSamplerState(2, D3DSAMP_ADDRESSU, pItem->pLightingState->pProjectedLight->GetProjectionTextureWrapMode());
        RedRenderer::pcSetSamplerState(2, D3DSAMP_ADDRESSV, pItem->pLightingState->pProjectedLight->GetProjectionTextureWrapMode());
        RedRenderer::pcSetSamplerState(2, D3DSAMP_ADDRESSW, pItem->pLightingState->pProjectedLight->GetProjectionTextureWrapMode());
        RedRenderer::pcSetTexture(2, RedTexture::Find(pItem->pLightingState->pProjectedLight->m_uiProjTexHash));
        RedColorValue light(pItem->pLightingState->pProjectedLight->GetColor());
        light.a = (float)(int)pItem->pLightingState->pProjectedLight->IsShadowCaster();
        RedRenderer::pcGetDevice()->SetPixelShaderConstantF(1, &light.r, 1);
    }
    else
    {
        RedRenderer::pcSetTexture(2, RedTexture::Find(PblHash("white")));
    }

    if (pItem->flags & RENDERFLAG_RENDER_DETAIL)
    {
        if (bTransparent && color.a == 0xff && (pMaterial->materialFlags & MATERIAL_FLAG_PARTIAL_ZWRITE))
        {
            RedRenderer::pcSetRenderState(D3DRS_ALPHAREF, 0xfc);
            RedRenderer::pcSetRenderState(D3DRS_ZWRITEENABLE, TRUE);
            pItem->pPrim->Draw();
            RedRenderer::pcSetRenderState(D3DRS_ALPHAREF, 1);
            RedRenderer::pcSetRenderState(D3DRS_ZWRITEENABLE, FALSE);
        }
        if ((pMaterial->materialFlags & (MATERIAL_FLAG_BACK_THEN_FRONT | MATERIAL_FLAG_CULL_NONE)) == (MATERIAL_FLAG_BACK_THEN_FRONT | MATERIAL_FLAG_CULL_NONE))
        {
            RedRenderer::pcSetCullMode(PCREDCULL_FRONT);
            pItem->pPrim->Draw();
            RedRenderer::pcSetCullMode(PCREDCULL_BACK);
        }
    }
    pItem->pPrim->Draw();
}

// FUNCTION: SWBF2 0x00700c40
void pcNormalShader::SetData(const RedShaderData& data)
{
    if (m_material.materialFlags & (MATERIAL_FLAG_ANIMATIONSCROLL | MATERIAL_FLAG_ANIMATIONBLINK | MATERIAL_FLAG_ANIMATIONTEXTURE | MATERIAL_FLAG_ANIMATIONLIGHT))
    {
        m_material.animationData = new RedMaterialAnimationData;
        memset(m_material.animationData, 0, sizeof(RedMaterialAnimationData));
        m_material.animationData->diffuseColor = m_material.diffuseColor;
        m_material.animationData->scrollRateU = 0;
        m_material.animationData->scrollRateV = 0;
        m_material.animationData->minIntensity = 0xff;
        m_material.animationData->blinkRate = 0;
        m_material.animationData->numRows = 1;
        m_material.animationData->numCols = 1;
        m_material.animationData->frameRate = 0;
        m_material.animationData->attachedLightName = 0;
        m_material.animationData->attachedLight = 0;

        if (m_material.materialFlags & MATERIAL_FLAG_ANIMATIONSCROLL)
        {
            m_material.animationData->scrollRateU = data.uiData0;
            m_material.animationData->scrollRateV = data.uiData1;
        }
        else if (m_material.materialFlags & MATERIAL_FLAG_ANIMATIONTEXTURE)
        {
            m_material.animationData->numRows = (unsigned char)(int)sqrt((float)data.uiData0);
            m_material.animationData->numCols = m_material.animationData->numRows;
            m_material.animationData->frameRate = data.uiData1;
        }
        else if (m_material.materialFlags & MATERIAL_FLAG_ANIMATIONBLINK)
        {
            m_material.animationData->minIntensity = data.uiData0;
            m_material.animationData->blinkRate = data.uiData1;
        }

        if (m_material.materialFlags & MATERIAL_FLAG_ANIMATIONLIGHT)
        {
            m_material.animationData->attachedLightName = data.attachedLightName;
        }
    }
    else
    {
        float fScaleX = 1.0f;
        float fScaleY = 1.0f;
        if (data.uiData0)
        {
            fScaleX = (float)data.uiData0;
        }
        if (data.uiData1)
        {
            fScaleY = (float)data.uiData1;
        }
        if (fScaleX != 1.0f || fScaleY != 1.0f)
        {
            m_material.detailTexTransform.SetScale(fScaleX, fScaleY);
        }
    }
}

// FUNCTION: SWBF2 0x00700e30
void pcNormalShader::UpdateAnimation()
{
    RedMaterialAnimationData* pAnim = m_material.animationData;
    const float fTime = s_animationTimeStamp.GetElapsed();
    float fScrollU = 0.0f;
    float fScrollV = 0.0f;

    if (m_material.materialFlags & MATERIAL_FLAG_ANIMATIONSCROLL)
    {
        fScrollU -= pAnim->scrollRateU * kInv255 * fTime;
        fScrollV -= pAnim->scrollRateV * kInv255 * fTime;
    }
    else if (m_material.materialFlags & MATERIAL_FLAG_ANIMATIONTEXTURE)
    {
        const int iFrame = (int)(pAnim->frameRate * fTime) % (pAnim->numRows * pAnim->numCols);
        const int iRow = iFrame / pAnim->numCols;
        const int iCol = iFrame - pAnim->numCols * iRow;
        fScrollU += (float)iCol / (float)pAnim->numCols;
        fScrollV += (float)iRow / (float)pAnim->numRows;
    }
    else if (m_material.materialFlags & MATERIAL_FLAG_ANIMATIONBLINK)
    {
        // GLOBAL: SWBF2 0x009c94dc
        static float s_fBlinkScale = PblMath::fTwoPi * 10.0f * kInv255;
        const float fSin = PblMath::Sin(pAnim->blinkRate * s_fBlinkScale * fTime) * 0.5f + 0.5f;
        const float fIntensity = pAnim->minIntensity * kInv255 * (1.0f - fSin) + fSin;
        m_material.diffuseColor.r = pAnim->diffuseColor.r * fIntensity;
        m_material.diffuseColor.g = pAnim->diffuseColor.g * fIntensity;
        m_material.diffuseColor.b = pAnim->diffuseColor.b * fIntensity;
    }

    if (m_material.materialFlags & MATERIAL_FLAG_ANIMATIONLIGHT)
    {
        // GLOBAL: SWBF2 0x009c94e4
        static RedColorValue s_minColor(0.2f, 0.2f, 0.2f, 1.0f);
        if (!m_material.animationData->attachedLight)
        {
            m_material.animationData->attachedLight = (RedLight*)RedLight::Find(pAnim->attachedLightName);
        }
        const RedLight* pLight = m_material.animationData->attachedLight;
        if (pLight)
        {
            const float fIntensity = pLight->GetIntensity();
            m_material.diffuseColor.r = (pAnim->diffuseColor.r - s_minColor.r) * fIntensity + s_minColor.r;
            m_material.diffuseColor.g = (pAnim->diffuseColor.g - s_minColor.g) * fIntensity + s_minColor.g;
            m_material.diffuseColor.b = (pAnim->diffuseColor.b - s_minColor.b) * fIntensity + s_minColor.b;
        }
    }

    if (fScrollU > 1.0f || fScrollU < -1.0f)
    {
        fScrollU -= (float)(int)fScrollU;
    }
    if (fScrollV > 1.0f || fScrollV < -1.0f)
    {
        fScrollV -= (float)(int)fScrollV;
    }

    m_material.diffuseTexTransform.SetOffset(fScrollU, fScrollV);
}

// SYNTHETIC: SWBF2 0x00409810
// PblMath::`dynamic initializer for 'fOneOverHalfPi''

// SYNTHETIC: SWBF2 0x00409830
// PblMath::`dynamic initializer for 'fOneOverTwoPi''

// GLOBAL: SWBF2 0x009c9268
// g_pcNormalShader
unsigned int g_pcNormalShader;
