#pragma once

#include "PebbleFL/Common/PblHashTable.h"
#include "PebbleFL/Common/PblRtti.h"

#include <stddef.h>

#include "CommandPost.h"
#include "EntityBuildingAnimatedArmed.h"

// VTABLE: SWBF2 0x00797888 EntityBuildingAnimatedArmedClass
// VTABLE: SWBF2 0x00797b08 CommandPostClass
class CommandBuildingAnimatedArmedClass : public EntityBuildingAnimatedArmedClass,
                                          public CommandPostClass
{
public:
    CommandBuildingAnimatedArmedClass(unsigned int uHash);

    CommandBuildingAnimatedArmedClass(CommandBuildingAnimatedArmedClass* pBase, unsigned int uHash);

    virtual ~CommandBuildingAnimatedArmedClass();

    virtual EntityClass* Derive(unsigned int uHash);
    virtual Entity* Build(const EntityDesc& desc);
    // FUNCTION: SWBF2 0x00476890
    // CommandBuildingAnimatedArmedClass::IsRtti
    // FUNCTION: SWBF2 0x004768e0
    // CommandBuildingAnimatedArmedClass::GetDerivedRtti
    // FUNCTION: SWBF2 0x004768f0
    // CommandBuildingAnimatedArmedClass::GetDerivedRttiName
    PBL_DECLARE_DERIVED_RTTI(CommandBuildingAnimatedArmedClass, EntityBuildingAnimatedArmedClass)
    virtual void SetProperty(unsigned int uProperty, const char* pszValue);

};

// VTABLE: SWBF2 0x007978d4 Damageable
// VTABLE: SWBF2 0x00797900 CollisionObject
// VTABLE: SWBF2 0x00797954 EntityEx
// VTABLE: SWBF2 0x00797a88
// VTABLE: SWBF2 0x00797aa0 RedSceneObject
// VTABLE: SWBF2 0x00797b14 PblHandled
class CommandBuildingAnimatedArmed : public EntityBuildingAnimatedArmed
{
public:

    CommandBuildingAnimatedArmed(CommandBuildingAnimatedArmedClass* pClass, const EntityDesc& desc);

    // FUNCTION: SWBF2 0x004767d0
    // CommandBuildingAnimatedArmed::IsRtti
    // FUNCTION: SWBF2 0x00476830
    // CommandBuildingAnimatedArmed::GetDerivedRtti
    // FUNCTION: SWBF2 0x00476840
    // CommandBuildingAnimatedArmed::GetDerivedRttiName
    PBL_DECLARE_DERIVED_RTTI(CommandBuildingAnimatedArmed, EntityBuildingAnimatedArmed)
    virtual ~CommandBuildingAnimatedArmed();
    virtual void SetProperty(unsigned int uProperty, const char* pszValue);
    virtual void Init();
    virtual void SetTeam(int iTeam);
    virtual EntityType GetEntityType() const;
    virtual void Kill();
    virtual void Respawn();
    virtual void WriteObject(NetPktGroup& group, NetObjState* pState);
    virtual void ReadObject(NetPktGroup& group, NetObjState* pState);
    virtual GameObject* AddToClient();

    CommandBuildingAnimatedArmedClass* mClass;
    CommandPost* mPost;
};

