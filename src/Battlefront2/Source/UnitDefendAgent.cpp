#include "UnitDefendAgent.h"
#include "EventInfo.h"
#include "UnitCmdInfo.h"
#include "UnitController.h"
#include "AILowLevel.h"
#include "AttackHelper.h"

// SYNTHETIC: SWBF2 0x00406ca0
// `dynamic initializer for 'UnitDefendAgent::rttiHashUnitDefendAgent''
// UnitDefendAgent::rttiHashUnitDefendAgent

// GLOBAL: SWBF2 0x01f993b4
// UnitDefendAgent::rttiHashUnitDefendAgent
// GLOBAL: SWBF2 0x0078b800
// UnitDefendAgent::rttiNameUnitDefendAgent
PBL_IMPLEMENT_RTTI(UnitDefendAgent)









// FUNCTION: SWBF2 0x00665c70
UnitDefendAgent::UnitDefendAgent(UnitController* pCtrl, const AI::GoalTarget& target)
    : UnitAgent(pCtrl),
      mDefendHelper(this),
      mCombatHelper(this),
      mTarget(AI::GTT_NONE),
      mMaintainVisualContact(true)
{
    mTarget = target;
    mCurState = STATE_START;
    mNextState = STATE_Defend;
}

// FUNCTION: SWBF2 0x00665d20
void UnitDefendAgent::Kill()
{
    mDefendHelper.Kill();
    mCombatHelper.Kill();
    UnitAgent::Kill();
}

// FUNCTION: SWBF2 0x00665d50
float UnitDefendAgent::GetUpdateRate() const
{
    switch (mCurState)
    {
    case STATE_Defend:
        return mDefendHelper.GetUpdateRate();
    case STATE_Combat:
        return 0.8f;
    default:
        return 1.5f;
    }
}

// FUNCTION: SWBF2 0x00665d90
void UnitDefendAgent::EnterState()
{
    switch (mCurState)
    {
    case STATE_Defend:
        mDefendHelper.Init(mTarget);
        break;
    case STATE_Combat:
        if (mCombatCmd.GetCmdType() == CMD_NONE)
        {
            mCombatCmd = UnitCombatInfo(UnitCombatInfo::ENGAGE);
        }
        mCombatHelper.Init((const UnitCombatInfo&)mCombatCmd);
        break;
    }
}

// FUNCTION: SWBF2 0x00665df0
void UnitDefendAgent::ExitState()
{
    switch (mCurState)
    {
    case STATE_Defend:
        mDefendHelper.Clean();
        break;
    case STATE_Combat:
        mCombatHelper.Clean();
        break;
    }
}

// FUNCTION: SWBF2 0x00665e10
void UnitDefendAgent::UpdateStatePostVision(float dt)
{
    if (mCurState != STATE_Combat)
    {
        mAttackHelper.Update();
    }
    mCtrl->mLowLevel.SetHeight(HEIGHT_STAND);

    switch (mCurState)
    {
    case STATE_Defend:
        mDefendHelper.FSMUpdate(dt, false);
        if (mDefendHelper.IsDone())
        {
            SetNextState(STATE_GLOBDONE);
        }
        break;
    case STATE_Combat:
        mCombatHelper.FSMUpdate(dt, false);
        if (mCombatHelper.IsDone())
        {
            SetNextState(STATE_Defend);
        }
        break;
    }
}

// FUNCTION: SWBF2 0x00665ec0
int UnitDefendAgent::EventHandler(AI::EventInfo* pInfo)
{
    int iResult;
    switch (mCurState)
    {
    case STATE_Defend:
        iResult = mDefendHelper.EventHandler(pInfo);
        if (iResult != AI::EVT_NotHandled)
        {
            return iResult;
        }
        break;
    case STATE_Combat:
        iResult = mCombatHelper.EventHandler(pInfo);
        if (iResult != AI::EVT_NotHandled)
        {
            return iResult;
        }
        break;
    }

    switch (pInfo->mType)
    {
    case AI::EVT_Grenade:
    case AI::EVT_Damaged:
    case AI::EVT_Vision_CanSeeEnemy:
    case AI::EVT_Audible_CanHearEnemy:
    case AI::EVT_EndInvisibility:
        if (UnitAgent::EventHandler(pInfo) == AI::EVT_Handled)
        {
            pInfo->mNextCmd = UnitCmdInfo(CMD_NONE, NULL, PblVector3(0, 0, 0), NULL);
            if (mCurState != STATE_Combat)
            {
                mCombatCmd = pInfo->mNextCmd;
                SetNextState(STATE_Combat);
                return AI::EVT_Handled;
            }
        }
        return AI::EVT_Ignored;

    default:
        return UnitAgent::EventHandler(pInfo);
    }
}

// FUNCTION: SWBF2 0x00665ff0
const char* UnitDefendAgent::NFINAL_GetStateString(int iState) const
{
    return 0;
}
