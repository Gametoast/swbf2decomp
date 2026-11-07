#pragma once

#include "PebbleFL/Common/PblHashTable.h"
#include "PebbleFL/Common/PblRtti.h"

#include <stddef.h>

#include "EntitySound.h"
#include "GameSound.h"
#include "Thread.h"
#include "RedEngineFL/Memory/RedMemoryPool.h"
#include "PebbleFL/Common/PblHashTable.h"

// VTABLE: SWBF2 0x0079d408
class EntitySoundStaticClass : public EntitySoundClass
{
public:
    EntitySoundStaticClass(unsigned int uHash);
    EntitySoundStaticClass(EntitySoundStaticClass* pBase, unsigned int uHash);
    virtual ~EntitySoundStaticClass();

    virtual EntityClass* Derive(unsigned int uHash);
    virtual Entity* Build(const EntityDesc& desc);
    // FUNCTION: SWBF2 0x004fb480
    // EntitySoundStaticClass::IsRtti
    // FUNCTION: SWBF2 0x004fb4b0
    // EntitySoundStaticClass::GetDerivedRtti
    // FUNCTION: SWBF2 0x004fb4c0
    // EntitySoundStaticClass::GetDerivedRttiName
    PBL_DECLARE_DERIVED_RTTI(EntitySoundStaticClass, EntitySoundClass)

};

// VTABLE: SWBF2 0x0079d3d0 EntitySound
// VTABLE: SWBF2 0x0079d3b8 Thread
class EntitySoundStatic : public EntitySound, public Thread
{
public:
    static void SetupMemoryPool(unsigned int uCount, unsigned int uSize, unsigned int uGrow) { sMemoryPool.Setup(uCount, uSize, uGrow); }
    EntitySoundStatic(EntitySoundStaticClass* pClass, const EntityDesc& desc);

    // FUNCTION: SWBF2 0x004fb420
    // EntitySoundStatic::IsRtti
    // FUNCTION: SWBF2 0x004fb450
    // EntitySoundStatic::GetDerivedRtti
    // FUNCTION: SWBF2 0x004fb460
    // EntitySoundStatic::GetDerivedRttiName
    PBL_DECLARE_DERIVED_RTTI(EntitySoundStatic, Entity)
    virtual ~EntitySoundStatic();
    virtual void Play();
    virtual bool Update(float fDeltaTime);

    static void* operator new(unsigned int uSize);

    // SYNTHETIC: SWBF2 0x004fb470
    // EntitySoundStatic::operator delete
    static void operator delete(void* pBlock) { sMemoryPool.Free(pBlock); }

    static MemoryPool sMemoryPool;

    GameSoundControllable mSound;
    GameSound mSoundProperties;
};

// VTABLE: SWBF2 0x0079fbf0
class SoundStaticRegionFactory : public RedRegionFactory
{
public:
    SoundStaticRegionFactory() : RedRegionFactory("soundstatic") {}

    // SYNTHETIC: SWBF2 0x005393a0
    // SoundStaticRegionFactory::`scalar deleting destructor'

    // FUNCTION: SWBF2 0x00539330
    virtual void* CreateRegion(RedRegionDesc& desc, const char* pcName) const
    {
        EntitySoundStaticClass soundClass(0xEDE52A96);
        return EntitySoundClass::CreateSoundFromRegion(desc, pcName, GetPrefix(), soundClass);
    }
};


