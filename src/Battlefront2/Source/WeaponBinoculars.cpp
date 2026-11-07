#include "WeaponBinoculars.h"
#include "RedEngineFL/Memory/RedMemoryPool.h"
#include <new>

// FUNCTION: SWBF2 0x0067e350
Weapon* WeaponBinocularsClass::Build(const WeaponDesc& desc)
{
    PostLoadInit();
    return new WeaponBinoculars(this, desc);
}

// SYNTHETIC: SWBF2 0x00407080
// `dynamic initializer for 'WeaponBinoculars::rttiHashWeaponBinoculars''
// WeaponBinoculars::rttiHashWeaponBinoculars

// GLOBAL: SWBF2 0x01f9a88c
// WeaponBinoculars::rttiHashWeaponBinoculars
// GLOBAL: SWBF2 0x0078be34
// WeaponBinoculars::rttiNameWeaponBinoculars
PBL_IMPLEMENT_RTTI(WeaponBinoculars)



// FUNCTION: SWBF2 0x0067e1c0
WeaponBinoculars::WeaponBinoculars(const WeaponBinocularsClass* pClass, const WeaponDesc& desc)
    : Weapon(pClass, desc)
{
}

// FUNCTION: SWBF2 0x0067e210
WeaponBinoculars::~WeaponBinoculars()
{
}

// SYNTHETIC: SWBF2 0x0067e1e0
// WeaponBinoculars::`scalar deleting destructor'





// SYNTHETIC: SWBF2 0x00407090
// `dynamic initializer for 'WeaponBinocularsClass::rttiHashWeaponBinocularsClass''
// WeaponBinocularsClass::rttiHashWeaponBinocularsClass

// GLOBAL: SWBF2 0x01f9a890
// WeaponBinocularsClass::rttiHashWeaponBinocularsClass
// GLOBAL: SWBF2 0x0078be54
// WeaponBinocularsClass::rttiNameWeaponBinocularsClass
PBL_IMPLEMENT_RTTI(WeaponBinocularsClass)

// FUNCTION: SWBF2 0x0067e220
WeaponBinocularsClass::WeaponBinocularsClass(unsigned int uHash)
    : WeaponClass(uHash)
{
    mLockOnAngle = 0.0f;
    mTargetSides = 0;
    mTargetTypes = 0;
    mAITargetTypes = 0;
}

// FUNCTION: SWBF2 0x0067e2a0
WeaponBinocularsClass::WeaponBinocularsClass(WeaponBinocularsClass* pBase, unsigned int uHash)
    : WeaponClass(pBase, uHash)
{
}

// FUNCTION: SWBF2 0x0067e2c0
WeaponBinocularsClass::~WeaponBinocularsClass()
{
}

// SYNTHETIC: SWBF2 0x0067e270
// WeaponBinocularsClass::`scalar deleting destructor'







// FUNCTION: SWBF2 0x0067e2d0
void WeaponBinocularsClass::SetProperty(unsigned int uProperty, const char* pszValue)
{
    WeaponClass::SetProperty(uProperty, pszValue);
}

// FUNCTION: SWBF2 0x0067e2e0
WeaponClass* WeaponBinocularsClass::Derive(unsigned int uHash)
{
    return new WeaponBinocularsClass(this, uHash);
}
