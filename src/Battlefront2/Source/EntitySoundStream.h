#pragma once

#include "PebbleFL/Common/PblHashTable.h"
#include "PebbleFL/Common/PblRtti.h"

#include <stddef.h>

#include "EntitySound.h"
#include "Thread.h"
#include "PebbleFL/Common/PblHashTable.h"
#include "SoundFL/source/sndsoundstream.h"

#include "RedEngineFL/Memory/RedMemoryPool.h"

// VTABLE: SWBF2 0x0079d458
class EntitySoundStreamClass : public EntitySoundClass
{
public:
    EntitySoundStreamClass(unsigned int uHash);
    EntitySoundStreamClass(EntitySoundStreamClass* pBase, unsigned int uHash);
    virtual ~EntitySoundStreamClass();

    virtual EntityClass* Derive(unsigned int uHash);
    virtual Entity* Build(const EntityDesc& desc);
    // FUNCTION: SWBF2 0x004fba10
    // EntitySoundStreamClass::IsRtti
    // FUNCTION: SWBF2 0x004fba40
    // EntitySoundStreamClass::GetDerivedRtti
    // FUNCTION: SWBF2 0x004fba50
    // EntitySoundStreamClass::GetDerivedRttiName
    PBL_DECLARE_DERIVED_RTTI(EntitySoundStreamClass, EntitySoundClass)

};

// VTABLE: SWBF2 0x0079d47c EntitySound
// VTABLE: SWBF2 0x0079d4b4 Thread
class EntitySoundStream : public EntitySound, public Thread
{
public:
    static void SetupMemoryPool(unsigned int uCount, unsigned int uSize, unsigned int uGrow) { sMemoryPool.Setup(uCount, uSize, uGrow); }
    EntitySoundStream(EntitySoundStreamClass* pClass, const EntityDesc& desc);
    virtual ~EntitySoundStream();

    // FUNCTION: SWBF2 0x004fb9b0
    // EntitySoundStream::IsRtti
    // FUNCTION: SWBF2 0x004fb9e0
    // EntitySoundStream::GetDerivedRtti
    // FUNCTION: SWBF2 0x004fb9f0
    // EntitySoundStream::GetDerivedRttiName
    PBL_DECLARE_DERIVED_RTTI(EntitySoundStream, Entity)
    virtual void Play();
    virtual bool Update(float fDeltaTime);

    static void* operator new(unsigned int uSize) { return sMemoryPool.Allocate(uSize); }
    // SYNTHETIC: SWBF2 0x004fba00
    // EntitySoundStream::operator delete
    static void operator delete(void* pBlock) { sMemoryPool.Free(pBlock); }

    static MemoryPool sMemoryPool;

    Snd::SoundStream mSoundStream;
};

// VTABLE: SWBF2 0x0079fc28
class SoundStreamRegionFactory : public RedRegionFactory
{
public:
    SoundStreamRegionFactory() : RedRegionFactory("soundstream") {}

    // SYNTHETIC: SWBF2 0x00539440
    // SoundStreamRegionFactory::`scalar deleting destructor'

    // FUNCTION: SWBF2 0x005393d0
    virtual void* CreateRegion(RedRegionDesc& desc, const char* pcName) const
    {
        EntitySoundStreamClass soundClass(0x93D6C380);
        return EntitySoundClass::CreateSoundFromRegion(desc, pcName, GetPrefix(), soundClass);
    }
};


