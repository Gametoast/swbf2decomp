#pragma once

#include <stddef.h>
#include "RedEngineFL/Graphics/PC/pcRedShader.h"

class RedTexture;
class pcRedVertexFormat;

// VTABLE: SWBF2 0x00784868
class pcParticleShader : public pcRedShader
{
public:

    pcParticleShader(unsigned int uiFlags,
                     const unsigned int* pParams,
                     unsigned int uiNumParams,
                     unsigned int uiMaterialFlags,
                     const RedColor& color0,
                     const RedColor& color1,
                     float fArg7);

    virtual ~pcParticleShader()
    {
    }

    // SYNTHETIC: SWBF2 0x00702e80
    // pcParticleShader::`scalar deleting destructor'

    // FUNCTION: SWBF2 0x00703480
    // pcParticleShader::GetID
    // FUNCTION: SWBF2 0x007034c0
    // pcParticleShader::GetRenderTypeHash
    // FUNCTION: SWBF2 0x00703440
    // pcParticleShader::Bind
    // FUNCTION: SWBF2 0x007034d0
    // pcParticleShader::GetShaderDef
    DECLARE_PC_RED_SHADER_METHODS()

    virtual RenderPhaseT GetRenderPhase(const RenderItem*, unsigned int) const;

private:

    virtual void _StaticInit() const;




public:

    void NearFadeEnable(bool bEnable) { m_nearFadeEnable = bEnable; }

    void SetShadowMode(bool bEnable) { m_shadowEnable = bEnable; }

    void SetTextureScale(float fScale) { m_fTextureScale = fScale; }

    unsigned int GetState_Fixed(const RenderItem*) const;
    unsigned int GetState_Prog1(const RenderItem*) const;
    void Begin_Fixed(unsigned int, unsigned int) const;
    void Begin_Prog1(unsigned int, unsigned int) const;
    void End_Fixed(unsigned int, unsigned int) const;
    void End_Prog1(unsigned int, unsigned int) const;
    void Render_Fixed(const RenderItem*, unsigned int) const;
    void Render_Prog1(const RenderItem*, unsigned int) const;

    unsigned int m_uiTextureHash;
    unsigned int m_uiMaterialFlags;
    bool m_nearFadeEnable;
    pcRedVertexFormat* m_pVertexFormat;
    float m_fTextureScale;
    float m_blurScale;
    int m_blurAmount;
    bool m_blurEnable;
    RedTexture* m_blurBuffer;
    bool m_shadowEnable;

    // GLOBAL: SWBF2 0x009c96b4
    static unsigned int s_uiID;

    // GLOBAL: SWBF2 0x009c96b8
    static GetStateFuncT GetStateFuncArray[MAX_PIPELINES];

    // GLOBAL: SWBF2 0x009c96c4
    static BeginFuncT BeginFuncArray[MAX_PIPELINES];

    // GLOBAL: SWBF2 0x009c96d0
    static EndFuncT EndFuncArray[MAX_PIPELINES];

    // GLOBAL: SWBF2 0x009c96dc
    static RenderFuncT RenderFuncArray[MAX_PIPELINES];

    // GLOBAL: SWBF2 0x009c96ec
    static unsigned int s_uiRenderTypeHash;

    // GLOBAL: SWBF2 0x009c9780
    static pcRedShaderDef s_shaderDef;

    static pcParticleShader s_Factory;

private:

    pcParticleShader();

    void _Init(unsigned int uiFlags,
               const unsigned int* pParams,
               unsigned int uiNumParams,
               unsigned int uiMaterialFlags,
               const RedColor& color0,
               const RedColor& color1,
               float fArg7);

};
 
 
 
 
 
 
 
 
 
 
