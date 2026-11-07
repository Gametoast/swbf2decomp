#pragma once

#include <stddef.h>
#include "RedEngineFL/Graphics/PC/pcRedShader.h"

#include "RedEngineFL/Graphics/RedMaterial.h"

class pcRedVertexFormat;

// VTABLE: SWBF2 0x007847c8
class pcZPrepassShader : public pcRedShader
{
public:

    virtual ~pcZPrepassShader()
    {
    }

    // SYNTHETIC: SWBF2 0x00702500
    // pcZPrepassShader::`scalar deleting destructor'

    // FUNCTION: SWBF2 0x00702760
    // pcZPrepassShader::GetID
    // FUNCTION: SWBF2 0x00702790
    // pcZPrepassShader::GetRenderTypeHash
    // FUNCTION: SWBF2 0x00702720
    // pcZPrepassShader::Bind
    // FUNCTION: SWBF2 0x007027a0
    // pcZPrepassShader::GetShaderDef
    DECLARE_PC_RED_SHADER_METHODS()

    virtual RenderPhaseT GetRenderPhase(const RenderItem*, unsigned int) const;

private:

    // FUNCTION: SWBF2 0x007029f0

    // FUNCTION: SWBF2 0x007027a0



public:

    unsigned int GetState_Fixed(const RenderItem*) const;
    unsigned int GetState_Prog1(const RenderItem*) const;
    void Begin_Fixed(unsigned int, unsigned int) const;
    void Begin_Prog1(unsigned int, unsigned int) const;
    void End_Fixed(unsigned int, unsigned int) const;
    void End_Prog1(unsigned int, unsigned int) const;
    void Render_Fixed(const RenderItem*, unsigned int) const;
    void Render_Prog1(const RenderItem*, unsigned int) const;

    RedMaterial m_material;
    unsigned int m_uiState;
    pcRedVertexFormat* m_pVertexInputFormat;

    // GLOBAL: SWBF2 0x009c95a8
    static unsigned int s_uiID;

    // GLOBAL: SWBF2 0x009c95ac
    static GetStateFuncT GetStateFuncArray[MAX_PIPELINES];

    // GLOBAL: SWBF2 0x009c95b8
    static BeginFuncT BeginFuncArray[MAX_PIPELINES];

    // GLOBAL: SWBF2 0x009c95c4
    static EndFuncT EndFuncArray[MAX_PIPELINES];

    // GLOBAL: SWBF2 0x009c95d0
    static RenderFuncT RenderFuncArray[MAX_PIPELINES];

    // GLOBAL: SWBF2 0x009c95e8
    static unsigned int s_uiRenderTypeHash;

    // GLOBAL: SWBF2 0x009c95ec
    static pcRedShaderDef s_shaderDef;

    // GLOBAL: SWBF2 0x009c9618
    static pcZPrepassShader s_Factory;

private:

public:

    // FUNCTION: SWBF2 0x00702460
    pcZPrepassShader(unsigned int uiFlags,
                     const unsigned int* pParams,
                     unsigned int uiNumParams,
                     unsigned int uiMaterialFlags,
                     const RedColor& color0,
                     const RedColor& color1,
                     float fSpecularPower);

private:
    // FUNCTION: SWBF2 0x00702390
    pcZPrepassShader();

    // FUNCTION: SWBF2 0x00702a80
    void _Init(unsigned int uiFlags,
               const unsigned int* pParams,
               unsigned int uiNumParams,
               unsigned int uiMaterialFlags,
               const RedColor& color0,
               const RedColor& color1,
               float fSpecularPower);

    // FUNCTION: SWBF2 0x00702bf0
};
 
 
