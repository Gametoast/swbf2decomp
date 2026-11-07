#pragma once

#include "PebbleFL/Common/PblHashTable.h"
#include "PebbleFL/Common/PblRtti.h"

#include <stddef.h>

#include "CommandPost.h"
#include "EntityBuildingAnimated.h"
#include "PebbleFL/Common/PblHashTable.h"

// VTABLE: SWBF2 0x007977f8 EntityBuildingAnimatedClass
// VTABLE: SWBF2 0x007977e4 CommandPostClass
class CommandBuildingClass : public EntityBuildingAnimatedClass, public CommandPostClass
{
public:
    CommandBuildingClass(unsigned int uHash);

    CommandBuildingClass(CommandBuildingClass* pBase, unsigned int uHash);
    virtual ~CommandBuildingClass();

    virtual EntityClass* Derive(unsigned int uHash);
    virtual Entity* Build(const EntityDesc& desc);
    // FUNCTION: SWBF2 0x004760a0
    // CommandBuildingClass::IsRtti
    // FUNCTION: SWBF2 0x004760f0
    // CommandBuildingClass::GetDerivedRtti
    // FUNCTION: SWBF2 0x00476100
    // CommandBuildingClass::GetDerivedRttiName
    PBL_DECLARE_DERIVED_RTTI(CommandBuildingClass, EntityBuildingAnimatedClass)
    virtual void SetProperty(unsigned int uProperty, const char* pszValue);

};

// VTABLE: SWBF2 0x007975bc RedSceneObject
// VTABLE: SWBF2 0x00797624 CollisionObject
// VTABLE: SWBF2 0x00797678 EntityEx
// VTABLE: SWBF2 0x007977a0 Damageable
// VTABLE: SWBF2 0x007977cc
// VTABLE: SWBF2 0x007977f0 PblHandled
class CommandBuilding : public EntityBuildingAnimated
{
public:

    CommandBuilding(CommandBuildingClass* pClass, const EntityDesc& desc);

    // FUNCTION: SWBF2 0x00475ff0
    // CommandBuilding::IsRtti
    // FUNCTION: SWBF2 0x00476040
    // CommandBuilding::GetDerivedRtti
    // FUNCTION: SWBF2 0x00476050
    // CommandBuilding::GetDerivedRttiName
    PBL_DECLARE_DERIVED_RTTI(CommandBuilding, EntityBuildingAnimated)
    virtual ~CommandBuilding();
    virtual void SetProperty(unsigned int uProperty, const char* pszValue);
    virtual void Init();
    virtual void SetTeam(int iTeam);
    virtual void Kill();
    virtual void Respawn();

    EntityType GetEntityType() const;
    virtual void WriteObject(NetPktGroup& group, NetObjState* pState);
    virtual void ReadObject(NetPktGroup& group, NetObjState* pState);
    virtual GameObject* AddToClient();

    CommandBuildingClass* mClass;
    CommandPost* mPost;
};

