#ifndef SWBF2_REDENGINEFL_GRAPHICS_REDRENDERFLAGS_H
#define SWBF2_REDENGINEFL_GRAPHICS_REDRENDERFLAGS_H

// Flags passed down every Render call (RedSceneObject::Render, RedModel::Render, ...) and kept per render item; the PC_ ones
// are set and read only by the PC renderer. Names and values from the modtools build's type information.
enum RENDERFLAGS
{
    RENDERFLAG_CLIPPED                       = 0x1,
    RENDERFLAG_TEST_BBOX                     = 0x2,
    RENDERFLAG_RENDER_DETAIL                 = 0x4,
    RENDERFLAG_POSTSHADOW                    = 0x8,
    RENDERFLAG_NO_LIGHTING                   = 0x10,
    RENDERFLAG_NO_Z                          = 0x20,
    RENDERFLAG_BLUR                          = 0x40,
    RENDERFLAG_STATIC                        = 0x80,
    RENDERFLAG_REFLECTION                    = 0x100,
    PC_RENDERFLAG_IMMEDIATE                  = 0x200,
    PC_RENDERFLAG_SHADOWVOLUME_ZFAIL         = 0x400,
    PC_RENDERFLAG_FF_HALF_LIGHTING           = 0x800,
    PC_RENDERFLAG_ZPREPASS                   = 0x1000,
    RENDERFLAG_RENDER_SHADOWVOLUMES          = 0x10000,
    RENDERFLAG_RENDER_TO_SKY                 = 0x20000,
    RENDERFLAG_RENDER_FAR_LODS               = 0x40000,
    RENDERFLAG_NO_ZBUFFER_FADE               = 0x40000,
    RENDERFLAG_RENDER_FIRSTPERSON            = 0x80000,
    RENDERFLAG_REGISTER_REFLECTION_PLANE     = 0x100000,
    RENDERFLAG_RENDER_REFLECTION             = 0x200000,
    RENDERFLAG_RENDER_REFLECTION_SKYDOMEONLY = 0x400000,
    RENDERFLAG_RENDER_ADDITIVE               = 0x800000,
    RENDERFLAG_RENDER_CACHED                 = 0x1000000,
    RENDERFLAG_RENDER_TO_TERRAIN             = 0x2000000,
    RENDERFLAG_RENDER_TO_INTERFACE           = 0x4000000,
    RENDERFLAG_PSEUDO_REFLECTIVE             = 0x8000000,
    RENDERFLAG_PSEUDO_REFLECTED              = 0x10000000,
    RENDERFLAG_ZBUFFER_ALPHA_FADE            = 0x20000000,
    RENDERFLAG_USE_Z_WHEN_FADE               = 0x40000000,
    RENDERFLAG_NO_LIGHTCOLLECTION            = 0x80000000,
    RENDERFLAG_FORCE_DWORD                   = 0x7fffffff,
};

#endif
