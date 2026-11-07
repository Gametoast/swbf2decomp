#ifndef SWBF2_REDENGINEFL_GRAPHICS_REDMATERIAL_H
#define SWBF2_REDENGINEFL_GRAPHICS_REDMATERIAL_H

#include <stddef.h>

#include "RedColor.h"
#include "RedTextureTransform.h"

struct RedMaterialAnimationData;

 
 
 
 
 

class RedLight;

struct RedMaterialAnimationData
{
    RedColorValue diffuseColor;
    unsigned int attachedLightName;
    RedLight* attachedLight;
    unsigned char scrollRateU;
    unsigned char scrollRateV;
    unsigned char minIntensity;
    unsigned char blinkRate;
    unsigned char numRows;
    unsigned char numCols;
    unsigned char frameRate;
};
 

// RedMaterial::materialFlags (read from the model files); names and values from the modtools build's type information.
enum MaterialFlags
{
    MATERIAL_FLAG_LIGHTING             = 0x1,
    MATERIAL_FLAG_ALPHAMAP_MASK        = 0x2,
    MATERIAL_FLAG_ALPHAMAP_OPACITY     = 0x4,
    MATERIAL_FLAG_ALPHAMAP_GLOSS       = 0x8,
    MATERIAL_FLAG_GLOW                 = 0x10,
    MATERIAL_FLAG_PERPIXELLIGHTING     = 0x20,
    MATERIAL_FLAG_ALPHABLEND_ADDITIVE  = 0x40,
    MATERIAL_FLAG_SPECULAR             = 0x80,
    MATERIAL_FLAG_ENVMAP               = 0x100,
    MATERIAL_FLAG_VERTEXCOLOR_LIGHTING = 0x200,
    MATERIAL_FLAG_BUMPDETAIL           = 0x800,
    MATERIAL_FLAG_PSEUDO_REFLECTIVE    = 0x1000,
    MATERIAL_FLAG_PSEUDO_REFLECTED     = 0x2000,
    MATERIAL_FLAG_PARTIAL_ZWRITE       = 0x4000,
    MATERIAL_FLAG_BACK_THEN_FRONT      = 0x8000,
    MATERIAL_FLAG_CULL_NONE            = 0x10000,
    MATERIAL_FLAG_ZENABLE_FALSE        = 0x20000,
    MATERIAL_FLAG_ZWRITEENABLE_TRUE    = 0x40000,
    MATERIAL_FLAG_COLORMASK_ALPHA      = 0x80000,
    MATERIAL_FLAG_TEXSAMPLE_POINT      = 0x100000,
    MATERIAL_FLAG_TEX_CLAMP            = 0x200000,
    MATERIAL_FLAG_ANIMATIONSCROLL      = 0x1000000,
    MATERIAL_FLAG_ANIMATIONBLINK       = 0x2000000,
    MATERIAL_FLAG_ANIMATIONTEXTURE     = 0x4000000,
    MATERIAL_FLAG_ANIMATIONLIGHT       = 0x8000000,
    MATERIAL_FLAG_HEIGHTFIELD          = 0x10000000,
    MATERIAL_FLAG_FORCE_DWORD          = 0x7fffffff,
};

struct RedMaterial
{
    unsigned int diffuseMapHash;
    unsigned int detailMapHash;
    unsigned int bumpMapHash;
    unsigned int envMapHash;
    unsigned int materialFlags;
    RedColorValue diffuseColor;
    RedColorValue specularColor;
    float specularPower;
    RedTextureTransform diffuseTexTransform;
    RedTextureTransform detailTexTransform;
    RedMaterialAnimationData* animationData;
};
 
 
 
 
 
 
 
 

#endif
