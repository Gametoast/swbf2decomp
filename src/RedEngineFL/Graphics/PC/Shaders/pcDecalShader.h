#pragma once

#include <stddef.h>
#include "RedEngineFL/Graphics/PC/pcRedShader.h"

class pcRedVertexFormat;

// VTABLE: SWBF2 0x00785310
class pcDecalShader : public pcRedShader
{
public:

    pcDecalShader(unsigned int uiFlags,
                  const unsigned int* pParams,
                  unsigned int uiNumParams,
                  unsigned int uiMaterialFlags,
                  const RedColor& color0,
                  const RedColor& color1,
                  float fArg7);

    virtual ~pcDecalShader()
    {
    }

    // SYNTHETIC: SWBF2 0x0070f0f0
    // pcDecalShader::`scalar deleting destructor'

    // FUNCTION: SWBF2 0x0070f3b0
    // pcDecalShader::GetID
    // FUNCTION: SWBF2 0x0070f3e0
    // pcDecalShader::GetRenderTypeHash
    // FUNCTION: SWBF2 0x0070f370
    // pcDecalShader::Bind
    // FUNCTION: SWBF2 0x0070f3f0
    // pcDecalShader::GetShaderDef
    DECLARE_PC_RED_SHADER_METHODS()

    virtual RenderPhaseT GetRenderPhase(const RenderItem*, unsigned int) const;

private:
    virtual void _StaticInit() const;

public:

    unsigned int GetState_Fixed(const RenderItem*) const;
    unsigned int GetState_Prog1(const RenderItem*) const;
    void Begin_Fixed(unsigned int, unsigned int) const;
    void Begin_Prog1(unsigned int, unsigned int) const;
    void End_Fixed(unsigned int, unsigned int) const;
    void End_Prog1(unsigned int, unsigned int) const;
    void Render_Fixed(const RenderItem*, unsigned int) const;
    void Render_Prog1(const RenderItem*, unsigned int) const;

    unsigned int m_uiTextureHash;
    unsigned int m_uiBumpHash;
    unsigned int m_uiMaterialFlags;
    pcRedVertexFormat* m_pVertexFormat;

    // GLOBAL: SWBF2 0x009ca634
    static unsigned int s_uiID;

    // GLOBAL: SWBF2 0x009ca638
    static GetStateFuncT GetStateFuncArray[MAX_PIPELINES];

    // GLOBAL: SWBF2 0x009ca644
    static BeginFuncT BeginFuncArray[MAX_PIPELINES];

    // GLOBAL: SWBF2 0x009ca650
    static EndFuncT EndFuncArray[MAX_PIPELINES];

    // GLOBAL: SWBF2 0x009ca65c
    static RenderFuncT RenderFuncArray[MAX_PIPELINES];

    // GLOBAL: SWBF2 0x009ca678
    static pcRedShaderDef s_shaderDef;

    static pcDecalShader s_Factory;

    // GLOBAL: SWBF2 0x009ca6d0
    static unsigned int s_uiRenderTypeHash;

private:

    pcDecalShader();

    void _Init(unsigned int uiFlags,
               const unsigned int* pParams,
               unsigned int uiNumParams,
               unsigned int uiMaterialFlags,
               const RedColor& color0,
               const RedColor& color1,
               float fArg7);

};
 
 
 
