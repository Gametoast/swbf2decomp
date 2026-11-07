#pragma once

#include "PebbleFL/Common/PblHashTable.h"
#include "PebbleFL/Common/PblRtti.h"

#include <stddef.h>

#include "CommandPost.h"
#include "EntityBuildingArmed.h"
#include "PebbleFL/Common/PblHashTable.h"

// VTABLE: SWBF2 0x00797cc8 EntityBuildingArmedClass
// VTABLE: SWBF2 0x00797d14 CommandPostClass
class CommandBuildingArmedClass : public EntityBuildingArmedClass, public CommandPostClass
{
public:
    CommandBuildingArmedClass(unsigned int uHash);

    CommandBuildingArmedClass(CommandBuildingArmedClass* pBase, unsigned int uHash);

    virtual ~CommandBuildingArmedClass();

    virtual EntityClass* Derive(unsigned int uHash);
    virtual Entity* Build(const EntityDesc& desc);
    // FUNCTION: SWBF2 0x00476f90
    // CommandBuildingArmedClass::IsRtti
    // FUNCTION: SWBF2 0x00476fe0
    // CommandBuildingArmedClass::GetDerivedRtti
    // FUNCTION: SWBF2 0x00476ff0
    // CommandBuildingArmedClass::GetDerivedRttiName
    PBL_DECLARE_DERIVED_RTTI(CommandBuildingArmedClass, EntityBuildingArmedClass)
    virtual void SetProperty(unsigned int uProperty, const char* pszValue);

};

// VTABLE: SWBF2 0x00797b50 Damageable
// VTABLE: SWBF2 0x00797b7c
// VTABLE: SWBF2 0x00797b94 EntityEx
// VTABLE: SWBF2 0x00797d20 PblHandled
// VTABLE: SWBF2 0x00797d28 RedSceneObject
// VTABLE: SWBF2 0x00797d90 CollisionObject
class CommandBuildingArmed : public EntityBuildingArmed
{
public:

    CommandBuildingArmed(CommandBuildingArmedClass* pClass, const EntityDesc& desc);

    // FUNCTION: SWBF2 0x00476ee0
    // CommandBuildingArmed::IsRtti
    // FUNCTION: SWBF2 0x00476f30
    // CommandBuildingArmed::GetDerivedRtti
    // FUNCTION: SWBF2 0x00476f40
    // CommandBuildingArmed::GetDerivedRttiName
    PBL_DECLARE_DERIVED_RTTI(CommandBuildingArmed, EntityBuildingArmed)
    virtual ~CommandBuildingArmed();
    virtual void SetProperty(unsigned int uProperty, const char* pszValue);
    virtual void Init();
    virtual void SetTeam(int iTeam);
    virtual EntityType GetEntityType() const;
    virtual void Kill();
    virtual void Respawn();
    virtual void WriteObject(NetPktGroup& group, NetObjState* pState);
    virtual void ReadObject(NetPktGroup& group, NetObjState* pState);
    virtual GameObject* AddToClient();

    CommandBuildingArmedClass* mClass;
    CommandPost* mPost;
};

