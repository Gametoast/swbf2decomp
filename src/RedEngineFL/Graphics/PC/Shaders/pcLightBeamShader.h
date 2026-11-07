#pragma once

#include <stddef.h>
#include "RedEngineFL/Graphics/PC/pcRedShader.h"

class pcRedVertexFormat;

// VTABLE: SWBF2 0x007849f8
class pcLightBeamShader : public pcRedShader
{
public:

    pcLightBeamShader(unsigned int uiFlags,
                      const unsigned int* pParams,
                      unsigned int uiNumParams,
                      unsigned int uiMaterialFlags,
                      const RedColor& color0,
                      const RedColor& color1,
                      float fArg7);

    virtual ~pcLightBeamShader()
    {
    }

    // SYNTHETIC: SWBF2 0x007045d0
    // pcLightBeamShader::`scalar deleting destructor'

    // FUNCTION: SWBF2 0x00704890
    // pcLightBeamShader::GetID
    // FUNCTION: SWBF2 0x007048c0
    // pcLightBeamShader::GetRenderTypeHash
    // FUNCTION: SWBF2 0x00704850
    // pcLightBeamShader::Bind
    // FUNCTION: SWBF2 0x007048d0
    // pcLightBeamShader::GetShaderDef
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

    pcRedVertexFormat* m_pVertexFormat;
    unsigned int m_uiMaterialFlags;

    // GLOBAL: SWBF2 0x009c995c
    static unsigned int s_uiID;

    // GLOBAL: SWBF2 0x009c9960
    static GetStateFuncT GetStateFuncArray[MAX_PIPELINES];

    // GLOBAL: SWBF2 0x009c996c
    static BeginFuncT BeginFuncArray[MAX_PIPELINES];

    // GLOBAL: SWBF2 0x009c9978
    static EndFuncT EndFuncArray[MAX_PIPELINES];

    // GLOBAL: SWBF2 0x009c9984
    static RenderFuncT RenderFuncArray[MAX_PIPELINES];

    // GLOBAL: SWBF2 0x009c9994
    static unsigned int s_uiRenderTypeHash;

    // GLOBAL: SWBF2 0x009c999c
    static pcRedShaderDef s_shaderDef;

    static pcLightBeamShader s_Factory;

private:

    pcLightBeamShader();

    void _Init(unsigned int uiFlags,
               const unsigned int* pParams,
               unsigned int uiNumParams,
               unsigned int uiMaterialFlags,
               const RedColor& color0,
               const RedColor& color1,
               float fArg7);

};
 
 
 
