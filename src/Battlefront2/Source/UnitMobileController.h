#pragma once

#include "PebbleFL/Common/PblHashTable.h"
#include "PebbleFL/Common/PblRtti.h"

#include "UnitController.h"

// VTABLE: SWBF2 0x007afbe8 CommandFsm
// VTABLE: SWBF2 0x007afc28 PblHandled
// VTABLE: SWBF2 0x007afc30 Controller
class UnitMobileController : public UnitController
{
public:

    UnitMobileController(Controllable* pOwner);

    // FUNCTION: SWBF2 0x00669180
    // UnitMobileController::IsRtti
    // FUNCTION: SWBF2 0x006691b0
    // UnitMobileController::GetDerivedRtti
    // FUNCTION: SWBF2 0x006691c0
    // UnitMobileController::GetDerivedRttiName
    PBL_DECLARE_DERIVED_RTTI(UnitMobileController, UnitController)

protected:

    virtual void ApplyNextState();
};

