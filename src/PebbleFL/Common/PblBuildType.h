#pragma once

#include "RedEngineFL/Util/RedLocalizeDB.h"

// The build's switches (which build this is, what its demo mode allows). Nothing in the shipped game changes them.
namespace PblBuildType
{
extern eGAMELANGUAGE gForceLang1;
extern eGAMELANGUAGE gForceLang2;
extern eGAMELANGUAGE gForceLang3;
extern bool gFinalBuild;
extern bool gXBox_DVDDemo;
extern bool gE3Demo;
extern bool gDemoAutoAssign;
extern bool gDemoEnglishOnly;
extern bool gDemoBuild;
extern bool gCreateMemoryProfile;
extern bool gDemoHasSplitscreen;
extern bool gDemoHost;
extern bool gDemoNoPressStart;
extern bool gDemoNoInputRebind;
extern bool gDemoCanSoftReboot;
extern bool gPS2ExceptionHandler;
}
