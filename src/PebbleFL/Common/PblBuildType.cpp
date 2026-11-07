#include "PebbleFL/Common/PblBuildType.h"

namespace PblBuildType
{
// GLOBAL: SWBF2 0x007e2e00
eGAMELANGUAGE gForceLang1 = eLANG_UNRESTRICTED;
// GLOBAL: SWBF2 0x007e2e04
eGAMELANGUAGE gForceLang2 = eLANG_UNRESTRICTED;
// GLOBAL: SWBF2 0x007e2e08
eGAMELANGUAGE gForceLang3 = eLANG_UNRESTRICTED;
// GLOBAL: SWBF2 0x007e2e0c
bool gFinalBuild = true;
// GLOBAL: SWBF2 0x007e2e0d
bool gXBox_DVDDemo = true;
// GLOBAL: SWBF2 0x007e2e0e
bool gE3Demo = true;
// GLOBAL: SWBF2 0x007e2e0f
bool gDemoAutoAssign = true;
// GLOBAL: SWBF2 0x007e2e10
bool gDemoEnglishOnly = true;

// GLOBAL: SWBF2 0x009cef71
bool gDemoBuild;
// GLOBAL: SWBF2 0x009cef72
bool gCreateMemoryProfile;
// GLOBAL: SWBF2 0x009cef73
bool gDemoHasSplitscreen;
// GLOBAL: SWBF2 0x009cef74
bool gDemoHost;
// GLOBAL: SWBF2 0x009cef76
bool gDemoNoPressStart;
// GLOBAL: SWBF2 0x009cef77
bool gDemoNoInputRebind;
// Nothing reads these two; they are Steam's bytes at 0x009cef70 and 0x009cef75 (which is which is not known).
bool gDemoCanSoftReboot;
bool gPS2ExceptionHandler;
}
