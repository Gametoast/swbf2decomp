#pragma once

#include "PebbleFL/Common/PblHashTable.h"
#include "PebbleFL/Common/PblRtti.h"

#include <stddef.h>

#include "EntityBuildingArmed.h"
#include "PebbleFL/Common/PblHashTable.h"

class MemoryPool;

// VTABLE: SWBF2 0x0079a03c
class EntityBuildingArmedDynamicClass : public EntityBuildingArmedClass
{
public:
    EntityBuildingArmedDynamicClass(unsigned int uHash);

    EntityBuildingArmedDynamicClass(EntityBuildingArmedDynamicClass* pBase, unsigned int uHash);

    virtual ~EntityBuildingArmedDynamicClass();

    virtual EntityClass* Derive(unsigned int uHash);
    virtual Entity* Build(const EntityDesc& desc);
    // FUNCTION: SWBF2 0x00496b90
    // EntityBuildingArmedDynamicClass::IsRtti
    // FUNCTION: SWBF2 0x00496be0
    // EntityBuildingArmedDynamicClass::GetDerivedRtti
    // FUNCTION: SWBF2 0x00496bf0
    // EntityBuildingArmedDynamicClass::GetDerivedRttiName
    PBL_DECLARE_DERIVED_RTTI(EntityBuildingArmedDynamicClass, EntityBuildingArmedClass)

};

// VTABLE: SWBF2 0x00799ebc
// VTABLE: SWBF2 0x00799ed4 EntityEx
// VTABLE: SWBF2 0x0079a008 PblHandled
// VTABLE: SWBF2 0x0079a010 Damageable
// VTABLE: SWBF2 0x0079a088 RedSceneObject
// VTABLE: SWBF2 0x0079a0f0 CollisionObject
class EntityBuildingArmedDynamic : public EntityBuildingArmed
{
public:

    EntityBuildingArmedDynamic(EntityBuildingArmedDynamicClass* pClass, const EntityDesc& desc);

    // FUNCTION: SWBF2 0x00496ac0
    // EntityBuildingArmedDynamic::IsRtti
    // FUNCTION: SWBF2 0x00496b10
    // EntityBuildingArmedDynamic::GetDerivedRtti
    // FUNCTION: SWBF2 0x00496b20
    // EntityBuildingArmedDynamic::GetDerivedRttiName
    PBL_DECLARE_DERIVED_RTTI(EntityBuildingArmedDynamic, EntityBuildingArmed)

    virtual EntityType GetEntityType() const;
    virtual void AddSends(void (*pfnSend)(GameObject*, int), int iArg);
    virtual void WriteObject(NetPktGroup& group, NetObjState* pState);
    virtual void ReadObject(NetPktGroup& group, NetObjState* pState);

    static void* operator new(unsigned int uSize);
    static void operator delete(void* pBlock);

    static MemoryPool sMemoryPool;
};

