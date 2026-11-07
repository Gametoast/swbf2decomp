#pragma once

// How many objects each game memory pool reserves when it is created (MemoryPool::Setup / Create's count). A pool
// is not limited to its count: when it is full it takes another block of its grow count from its heap. A level's
// script can change most counts before the pools are created (SetMemoryPoolSize("EntitySoldier", n), by the pool's
// name), so these are the defaults the game starts from. A count of 0 reserves nothing until something asks for room.
namespace PoolCount
{
// soldiers and what they carry (GameLoop::Init)
const unsigned int kEntitySoldier = 600;
const unsigned int kWeapon = 2400;
const unsigned int kAmmoCounter = kWeapon;   // one per weapon: SetMemoryPoolSize("Weapon") raises these two with it
const unsigned int kEnergyBar = kWeapon;
const unsigned int kAimer = 100;
const unsigned int kStickInfo = 60;
const unsigned int kOrdnance = 120;
const unsigned int kAcklayData = 0;          // AcklayData::Init

// vehicles, turrets and items (GameLoop::Init)
const unsigned int kEntityDroid = 16;
const unsigned int kMountedTurret = 50;
const unsigned int kPassengerSlot = 0;
const unsigned int kCommandFlyer = 0;
const unsigned int kPowerupItem = 25;
const unsigned int kEntityMine = 40;
const unsigned int kEntityPortableTurret = 6;

// players (GameLoop::Init)
const unsigned int kPlayerController = 64;
const unsigned int kTeamAssist = 64;

// AI (AIUtil::Init, PathFinder::Initialize, PathManager::Initialize)
const unsigned int kUnitController = 600;
const unsigned int kUnitAgent = 600;
const unsigned int kNavigator = 600;
const unsigned int kPathFollower = 600;
const unsigned int kConnectivityGraphFollower = 600;
const unsigned int kBaseHint = 350;
const unsigned int kAIGoal = 20;
const unsigned int kRayRequest = 201;        // VisionManager::RayRequest
const unsigned int kPathNode = 1024;
const unsigned int kObstacle = 512;
const unsigned int kPathRequest = 750;
const unsigned int kAIPath = 1200;
const unsigned int kAIPathNode = 9600;

// melee combos (Combo::Init)
const unsigned int kCombo = 32;
const unsigned int kComboState = 100;
const unsigned int kComboTransition = 85;
const unsigned int kComboCondition = 70;
const unsigned int kComboAttack = 120;
const unsigned int kComboDamageSample = 1200;
const unsigned int kComboDeflect = 8;

// world
const unsigned int kTreeGridStack = 550;     // GameLoop::Init
const unsigned int kActiveRegion = 64;       // ActiveRegion::Init
const unsigned int kTimer = 64;              // TimerEvent::Init
const unsigned int kEntityCloth = 16;        // ClothData::Init
const unsigned int kClothData = 32;
const unsigned int kTentacleSimulator = 34;  // TentacleSimulator::Init
const unsigned int kDecal = 0;               // DecalManager::Init

// effects (GameLoop::Init, unless noted)
const unsigned int kLightFlash = 16;
const unsigned int kEntityLight = 100;       // SetMemoryPoolSize("EntityLight", n) adds 60 to a level's count
const unsigned int kLightningBoltEffectObject = 16;
const unsigned int kParticleEmitterObject = 192;
const unsigned int kParticleEmitterInfoData = 1024;
const unsigned int kParticleEmitter = 1024;
const unsigned int kPositionTransformer = 1024;
const unsigned int kSizeTransformer = 1024;
const unsigned int kRotationTransformer = 0;
const unsigned int kColorTransformer = 1536;
const unsigned int kShieldEffect = 64;       // ShieldEffectManager::Init
const unsigned int kFLEffectOffsetMatrix = 64;   // FLEffectObject::Init

// sound (GameLoop::Init, Music::Load, TriggerSoundRegion)
const unsigned int kEntitySoundStatic = 112;
const unsigned int kEntitySoundStream = 30;
const unsigned int kSoundSpaceRegion = 30;
const unsigned int kMusic = 32;
const unsigned int kTriggerSoundRegion = 32;
const unsigned int kTriggerSoundProperties = 32;
const unsigned int kTriggerSoundGroup = 32;

// interface and cameras
const unsigned int kGuiEvent = 64;           // GuiManager::InitAll
const unsigned int kViewportData = 1;        // SpaceAssault: one viewport (the PC game has no split screen)

// network (NetGame.cpp)
const unsigned int kNetObj = 128;
const unsigned int kNetObjState = 8320;
}
