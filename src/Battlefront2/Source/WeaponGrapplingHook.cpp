#include "WeaponGrapplingHook.h"

// SYNTHETIC: SWBF2 0x00407170
// `dynamic initializer for 'WeaponGrapplingHookClass::rttiHashWeaponGrapplingHookClass''
// WeaponGrapplingHookClass::rttiHashWeaponGrapplingHookClass

// GLOBAL: SWBF2 0x01f9a8c4
// WeaponGrapplingHookClass::rttiHashWeaponGrapplingHookClass
// GLOBAL: SWBF2 0x0078bfc0
// WeaponGrapplingHookClass::rttiNameWeaponGrapplingHookClass
PBL_IMPLEMENT_RTTI(WeaponGrapplingHookClass)

// SYNTHETIC: SWBF2 0x00407160
// `dynamic initializer for 'WeaponGrapplingHook::rttiHashWeaponGrapplingHook''
// WeaponGrapplingHook::rttiHashWeaponGrapplingHook

// GLOBAL: SWBF2 0x01f9a8c8
// WeaponGrapplingHook::rttiHashWeaponGrapplingHook
// GLOBAL: SWBF2 0x0078bfdc
// WeaponGrapplingHook::rttiNameWeaponGrapplingHook
PBL_IMPLEMENT_RTTI(WeaponGrapplingHook)

// FUNCTION: SWBF2 0x00684de0
WeaponGrapplingHookClass::WeaponGrapplingHookClass(unsigned int uHash)
    : WeaponCannonClass(uHash)
{
}

// FUNCTION: SWBF2 0x00684e30
WeaponGrapplingHookClass::WeaponGrapplingHookClass(WeaponGrapplingHookClass* pBase,
                                                   unsigned int uHash)
    : WeaponCannonClass(pBase, uHash)
{
}

// FUNCTION: SWBF2 0x00684e50
WeaponGrapplingHookClass::~WeaponGrapplingHookClass()
{
}

// SYNTHETIC: SWBF2 0x00684e00
// WeaponGrapplingHookClass::`scalar deleting destructor'







// FUNCTION: SWBF2 0x00684e60
WeaponClass* WeaponGrapplingHookClass::Derive(unsigned int uHash)
{
    return new WeaponGrapplingHookClass(this, uHash);
}

// FUNCTION: SWBF2 0x00684ed0
Weapon* WeaponGrapplingHookClass::Build(const WeaponDesc& desc)
{
    return new WeaponGrapplingHook(this, desc);
}

// FUNCTION: SWBF2 0x00684d60
WeaponGrapplingHook::WeaponGrapplingHook(const WeaponGrapplingHookClass* pClass, const WeaponDesc& desc)
    : WeaponCannon(pClass, desc)
{
    mCableOut = false;
}

// FUNCTION: SWBF2 0x00684d90
WeaponGrapplingHook::~WeaponGrapplingHook()
{
}

// SYNTHETIC: SWBF2 0x00684ce0
// WeaponGrapplingHook::`scalar deleting destructor'







// FUNCTION: SWBF2 0x00684dc0
bool WeaponGrapplingHook::CheckFire() const
{
    if (mCableOut)
    {
        return false;
    }
    return WeaponCannon::CheckFire();
}

// FUNCTION: SWBF2 0x00684da0
void WeaponGrapplingHook::SetOrdnance(Ordnance* pOrdnance)
{
    mCableOut = pOrdnance != 0;
}
