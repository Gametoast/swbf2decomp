#include "DefendGoal.h"
#include "Character.h"
#include "Controllable.h"
#include "UnitCmdInfo.h"
#include "UnitController.h"

// SYNTHETIC: SWBF2 0x004027f0
// `dynamic initializer for 'AI::DefendGoal::rttiHashDefendGoal''

// GLOBAL: SWBF2 0x01eb9808
// AI::DefendGoal::rttiHashDefendGoal
// GLOBAL: SWBF2 0x00788e78
// AI::DefendGoal::rttiNameDefendGoal
PBL_IMPLEMENT_RTTI_IN(AI, DefendGoal)







// SYNTHETIC: SWBF2 0x0048b540
// AI::DefendGoal::`scalar deleting destructor'

// FUNCTION: SWBF2 0x0048b570
AI::DefendGoal::DefendGoal(unsigned char uTeam, float fWeight, const GoalTarget& target)
    : AIGoal(AIGOAL_DEFEND, uTeam, fWeight)
{
    mTarget = target;
}

// FUNCTION: SWBF2 0x0048b5d0
void AI::DefendGoal::Assign(Character* pChr)
{
    if (!pChr || !pChr->GetControllable())
    {
        return;
    }
    UnitController* pController = UnitControllerPtr(pChr->GetControllable()->mCtrl);
    if (pController)
    {
        UnitGoalAgentInfo info(CMD_DEFEND, &mTarget);
        pController->ForceCommand(info);
    }
}
