#pragma once

// VTABLE: SWBF2 0x0079f348
class Fsm
{
public:
    // States every state machine shares: none yet, and finished (Kill, IsDone). The modtools build's state-name tables
    // call them "NONE" and "GLOBDONE"; each machine numbers its own states from 1 (STATE_START).
    enum
    {
        STATE_NONE = 0,
        STATE_GLOBDONE = 100
    };

    Fsm();

protected:
    virtual void EnterState() = 0;
    virtual void ExitState() = 0;
    virtual void UpdateState(float dt) = 0;

public:
    virtual ~Fsm();
    virtual void Kill();
    virtual bool IsDone() const;
    virtual void FSMUpdate(float dt, bool b);
    virtual void SetNextState(int state);

protected:
    int mCurState;
    int mNextState;
};

// VTABLE: SWBF2 0x0079f324
class HelperFsm : public Fsm
{
public:
    HelperFsm(int iCurState, int iNextState);

protected:
    void FSMChangeState();

    virtual void SetNextState(int state);
};
