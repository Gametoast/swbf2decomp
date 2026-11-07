#pragma once

#include "PebbleFL/Common/PblHashTable.h"
#include "PebbleFL/Common/PblRtti.h"

#include "UnitController.h"

// VTABLE: SWBF2 0x007afccc Controller
// VTABLE: SWBF2 0x007afce4 PblHandled
// VTABLE: SWBF2 0x007afcec CommandFsm
class UnitStationaryController : public UnitController
{
public:
    UnitStationaryController(Controllable* pOwner);

    // FUNCTION: SWBF2 0x00669720
    // UnitStationaryController::IsRtti
    // FUNCTION: SWBF2 0x00669750
    // UnitStationaryController::GetDerivedRtti
    // FUNCTION: SWBF2 0x00669760
    // UnitStationaryController::GetDerivedRttiName
    PBL_DECLARE_DERIVED_RTTI(UnitStationaryController, UnitController)

protected:
    virtual void ApplyNextState();
};

 

