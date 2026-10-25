#include "EntityBuildingArmedDynamic.h"
#include "RedEngineFL/Memory/RedMemoryPool.h"

// SYNTHETIC: SWBF2 0x00402930
// `dynamic initializer for 'EntityBuildingArmedDynamicClass::rttiHashEntityBuildingArmedDynamicClass''
// GLOBAL: SWBF2 0x01ebb894
// EntityBuildingArmedDynamicClass::rttiHashEntityBuildingArmedDynamicClass
PblHash EntityBuildingArmedDynamicClass::rttiHashEntityBuildingArmedDynamicClass("EntityBuildingArmedDynamicClass");
const char EntityBuildingArmedDynamicClass::rttiNameEntityBuildingArmedDynamicClass[] = "EntityBuildingArmedDynamicClass";

// FUNCTION: SWBF2 0x00496c50
EntityBuildingArmedDynamicClass::EntityBuildingArmedDynamicClass(unsigned int uHash)
    : EntityBuildingArmedClass(uHash)
{
}

// FUNCTION: SWBF2 0x00496ca0
EntityBuildingArmedDynamicClass::EntityBuildingArmedDynamicClass(EntityBuildingArmedDynamicClass* pBase,
                                                                 unsigned int uHash)
    : EntityBuildingArmedClass(pBase, uHash)
{
}

// FUNCTION: SWBF2 0x00496cc0
EntityBuildingArmedDynamicClass::~EntityBuildingArmedDynamicClass()
{
}

// SYNTHETIC: SWBF2 0x00496c70
// EntityBuildingArmedDynamicClass::`scalar deleting destructor'

// FUNCTION: SWBF2 0x00496cd0
EntityClass* EntityBuildingArmedDynamicClass::Derive(unsigned int uHash)
{
    return new EntityBuildingArmedDynamicClass(this, uHash);
}

// FUNCTION: SWBF2 0x00496d40
Entity* EntityBuildingArmedDynamicClass::Build(const EntityDesc& desc)
{
    if (EntityBuildingArmedDynamic::sMemoryPool.mUsed >=
        EntityBuildingArmedDynamic::sMemoryPool.GetCount())
    {
        return 0;
    }

    EntityBuildingArmedDynamic* pBuilding = new EntityBuildingArmedDynamic(this, desc);
    if (pBuilding)
    {
        pBuilding->AddStaticToCollisionManager();
        pBuilding->ActivatePhysics();
    }
    return pBuilding;
}







// SYNTHETIC: SWBF2 0x00402920
// `dynamic initializer for 'EntityBuildingArmedDynamic::rttiHashEntityBuildingArmedDynamic''
// GLOBAL: SWBF2 0x01ebb890
// EntityBuildingArmedDynamic::rttiHashEntityBuildingArmedDynamic
PblHash EntityBuildingArmedDynamic::rttiHashEntityBuildingArmedDynamic("EntityBuildingArmedDynamic");
const char EntityBuildingArmedDynamic::rttiNameEntityBuildingArmedDynamic[] = "EntityBuildingArmedDynamic";

// GLOBAL: SWBF2 0x01ebb898
// SYNTHETIC: SWBF2 0x00402900
// `dynamic initializer for 'EntityBuildingArmedDynamic::sMemoryPool''
// SYNTHETIC: SWBF2 0x00766f00
// `dynamic atexit destructor for 'EntityBuildingArmedDynamic::sMemoryPool''

MemoryPool EntityBuildingArmedDynamic::sMemoryPool("EntityBuildingArmedDynamic", 0x6b0);





// FUNCTION: SWBF2 0x00496b40
EntityType EntityBuildingArmedDynamic::GetEntityType() const
{
    return ENT_ARMED_DYNAMIC;
}



// FUNCTION: SWBF2 0x00496c00
EntityBuildingArmedDynamic::EntityBuildingArmedDynamic(EntityBuildingArmedDynamicClass* pClass,
                                                       const EntityDesc& desc)
    : EntityBuildingArmed(pClass, desc)
{
}

// SYNTHETIC: SWBF2 0x00496b50
// EntityBuildingArmedDynamic::`scalar deleting destructor'
// SYNTHETIC: SWBF2 0x00496dda
// [thunk]:EntityBuildingArmedDynamic::`scalar deleting destructor' adjustor{148}
// SYNTHETIC: SWBF2 0x00496de5
// [thunk]:EntityBuildingArmedDynamic::`scalar deleting destructor' adjustor{320}
// SYNTHETIC: SWBF2 0x00496df0
// [thunk]:EntityBuildingArmedDynamic::`scalar deleting destructor' adjustor{12}
// SYNTHETIC: SWBF2 0x00496df8
// [thunk]:EntityBuildingArmedDynamic::`scalar deleting destructor' adjustor{512}
// SYNTHETIC: SWBF2 0x00496e03
// [thunk]:EntityBuildingArmedDynamic::`scalar deleting destructor' adjustor{608}

inline void* EntityBuildingArmedDynamic::operator new(unsigned int uSize)
{
    return sMemoryPool.Allocate(uSize);
}

inline void EntityBuildingArmedDynamic::operator delete(void* pBlock)
{
    sMemoryPool.Free(pBlock);
}
