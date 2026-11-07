#pragma once

#include "PebbleFL/Common/PblHashTable.h"
#include "PebbleFL/Common/PblRtti.h"

#include "UnitController.h"

// VTABLE: SWBF2 0x007afae0 Controller
// VTABLE: SWBF2 0x007afaa0 CommandFsm
// VTABLE: SWBF2 0x007afaf8 PblHandled
class UnitFlyController : public UnitController
{
public:
    UnitFlyController(Controllable* pOwner);

    // SYNTHETIC: SWBF2 0x00667d40
    // UnitFlyController::`scalar deleting destructor'

    // FUNCTION: SWBF2 0x00667cf0
    // UnitFlyController::IsRtti
    // FUNCTION: SWBF2 0x00667d20
    // UnitFlyController::GetDerivedRtti
    // FUNCTION: SWBF2 0x00667d30
    // UnitFlyController::GetDerivedRttiName
    PBL_DECLARE_DERIVED_RTTI(UnitFlyController, UnitController)

protected:
    virtual void ApplyNextState();
    virtual void EnterState();
    virtual void ExitState();
    virtual void UpdateState(float dt);
};

 

