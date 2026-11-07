#pragma once

#include "PebbleFL/Common/PblHashTable.h"
#include "PebbleFL/Common/PblRtti.h"

#include <stddef.h>

#include "Entity.h"
#include "PebbleFL/Common/PblHashTable.h"

class FLEffectClass;

// VTABLE: SWBF2 0x0079b1a0
class EntityEffectClass : public EntityClass
{
public:
    EntityEffectClass(unsigned int uHash);

    virtual ~EntityEffectClass();
    virtual EntityClass* Derive(unsigned int uHash);
    virtual Entity* Build(const EntityDesc& desc);
    // FUNCTION: SWBF2 0x004a99b0
    // EntityEffectClass::IsRtti
    // FUNCTION: SWBF2 0x004a99e0
    // EntityEffectClass::GetDerivedRtti
    // FUNCTION: SWBF2 0x004a99f0
    // EntityEffectClass::GetDerivedRttiName
    PBL_DECLARE_DERIVED_RTTI(EntityEffectClass, EntityClass)
    virtual void SetProperty(unsigned int uProperty, const char* pszValue);

    FLEffectClass* mEffectClass;
};
  
 

