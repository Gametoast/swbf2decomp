#include "CommandBuilding.h"

#include "GameModel.h"
#include "NetGame.h"

// FUNCTION: SWBF2 0x004763d0
CommandBuildingClass::CommandBuildingClass(unsigned int uHash)
    : EntityBuildingAnimatedClass(uHash), CommandPostClass(uHash)
{
}

// FUNCTION: SWBF2 0x00476460
CommandBuildingClass::CommandBuildingClass(CommandBuildingClass* pBase, unsigned int uHash)
    : EntityBuildingAnimatedClass(pBase, uHash), CommandPostClass(pBase, uHash)
{
}

// FUNCTION: SWBF2 0x004764e0
CommandBuildingClass::~CommandBuildingClass()
{
}

// SYNTHETIC: SWBF2 0x00476430
// CommandBuildingClass::`scalar deleting destructor'
// SYNTHETIC: SWBF2 0x004766c3
// [thunk]:CommandBuildingClass::`scalar deleting destructor' adjustor{1680}

// SYNTHETIC: SWBF2 0x00402630
// `dynamic initializer for 'CommandBuildingClass::rttiHashCommandBuildingClass''
// CommandBuildingClass::rttiHashCommandBuildingClass

// GLOBAL: SWBF2 0x01eb83c4
// CommandBuildingClass::rttiHashCommandBuildingClass
PblHash CommandBuildingClass::rttiHashCommandBuildingClass("CommandBuildingClass");
// GLOBAL: SWBF2 0x00788bb4
// CommandBuildingClass::rttiNameCommandBuildingClass
const char CommandBuildingClass::rttiNameCommandBuildingClass[] = "CommandBuildingClass";







// SYNTHETIC: SWBF2 0x00402620
// `dynamic initializer for 'CommandBuilding::rttiHashCommandBuilding''
// CommandBuilding::rttiHashCommandBuilding

// GLOBAL: SWBF2 0x01eb667c
// CommandBuilding::rttiHashCommandBuilding
PblHash CommandBuilding::rttiHashCommandBuilding("CommandBuilding");
// GLOBAL: SWBF2 0x00788ba4
// CommandBuilding::rttiNameCommandBuilding
const char CommandBuilding::rttiNameCommandBuilding[] = "CommandBuilding";

// SYNTHETIC: SWBF2 0x00476070
// CommandBuilding::`scalar deleting destructor'
// SYNTHETIC: SWBF2 0x0047669a
// [thunk]:CommandBuilding::`scalar deleting destructor' adjustor{608}
// SYNTHETIC: SWBF2 0x004766a5
// [thunk]:CommandBuilding::`scalar deleting destructor' adjustor{148}
// SYNTHETIC: SWBF2 0x004766b0
// [thunk]:CommandBuilding::`scalar deleting destructor' adjustor{320}
// SYNTHETIC: SWBF2 0x004766bb
// [thunk]:CommandBuilding::`scalar deleting destructor' adjustor{12}
// SYNTHETIC: SWBF2 0x004766ce
// [thunk]:CommandBuilding::`scalar deleting destructor' adjustor{512}

// FUNCTION: SWBF2 0x004761c0
CommandBuilding::~CommandBuilding()
{
    NetGame::Destroyed(this);
}

// FUNCTION: SWBF2 0x004762e0
void CommandBuilding::Respawn()
{
    EntityBuilding::Respawn();
    if (!mPost->IsThreadActive())
    {
        if (!NetGame::IsEnabled() || !NetGame::IsClient())
        {
            NetGame::CreateEvent_ChangeTeam(mPost, (unsigned __int64)-1);
        }
        mPost->Respawn();
    }
}







// FUNCTION: SWBF2 0x00476110
CommandBuilding::CommandBuilding(CommandBuildingClass* pClass, const EntityDesc& desc)
    : EntityBuildingAnimated(pClass, desc)
{
    mClass = pClass;
    mPost = CommandPost::BuildPost(this, pClass);
    NetGame::Created(this);
}

// FUNCTION: SWBF2 0x00476250
void CommandBuilding::Init()
{
    EntityBuildingAnimated::Init();
    if (mState == STATE_ALIVE)
    {
        mPost->Respawn();
    }
}

// FUNCTION: SWBF2 0x00476270
void CommandBuilding::Kill()
{
    EntityBuildingAnimated::Kill();
    if (mPost->IsThreadActive())
    {
        if (!NetGame::IsEnabled() || !NetGame::IsClient())
        {
            NetGame::CreateEvent_ChangeTeam(mPost, (unsigned __int64)-1);
        }
        mPost->Kill();
    }
}

// FUNCTION: SWBF2 0x00476350
void CommandBuilding::SetProperty(unsigned int uProperty, const char* pszValue)
{
    GameObject::SetProperty(uProperty, pszValue);
    mPost->SetProperty(uProperty, pszValue);
}

// FUNCTION: SWBF2 0x00476380
void CommandBuilding::SetTeam(int iTeam)
{
    int iOldTeam = mTeam;
    GameObject::SetTeam(iTeam);
    mPerceivedTeam = iTeam;
    mPost->SetTeam(iOldTeam, iTeam);
}

// FUNCTION: SWBF2 0x00476060
EntityType CommandBuilding::GetEntityType() const
{
    return ENT_POST;
}

// FUNCTION: SWBF2 0x00476540
void CommandBuildingClass::SetProperty(unsigned int uProperty, const char* pszValue)
{
    EntityBuildingAnimatedClass::SetProperty(uProperty, pszValue);
    CommandPostClass::SetProperty(uProperty, pszValue, mModel ? mModel->_mLod[0] : 0, 0);
}

// FUNCTION: SWBF2 0x00476580
EntityClass* CommandBuildingClass::Derive(unsigned int uHash)
{
    return new CommandBuildingClass(this, uHash);
}

// FUNCTION: SWBF2 0x004765f0
Entity* CommandBuildingClass::Build(const EntityDesc& desc)
{
    CommandBuilding* pBuilding = new CommandBuilding(this, desc);
    if (pBuilding)
    {
        pBuilding->AddStaticToCollisionManager();
        pBuilding->ActivatePhysics();
        if (!NetGame::IsEnabled() || !NetGame::IsClient())
        {
            pBuilding->BuildEffects();
        }
    }
    return pBuilding;
}
