#pragma once

#include "Battlefront2/Source/EventManager.h"
#include "PebbleFL/Common/PblHandle.h"
#include "Timer.h"
#include "LuaHelper.h"

namespace EventManager
{

// VTABLE: SWBF2 0x0079e250
// class LuaCallback<CommandPost, struct DamageOwner>
template <class ITEM, class PARAM>
class LuaCallback : public Event<ITEM, PARAM>::Callback
{
public:

    LuaCallback(int iRef);
    virtual ~LuaCallback();
    virtual bool operator()(ITEM& item, PARAM& param);

    int mFunction;
};

template <class ITEM>
bool LuaPushItem(ITEM& item);

template <>
bool LuaPushItem<char>(char& item);

class TimerMatches
{
public:

    TimerMatches(const Timer* pTimer) : mTimer(pTimer) {}
    TimerMatches(const char* pcName);

    static const char* GetName() { return ""; }

    bool operator==(const TimerMatches& other) const
    {
        return mTimer == other.mTimer;
    }

    bool operator()(Timer& timer, char&) const
    {
        return mTimer == &timer;
    }

    PblHandle<Timer const> mTimer;
};

namespace TimerEvent
{
extern Event<Timer, char> mElapsed;
void Cleanup();
}

}

namespace EventManager
{
template <class ITEM, class PARAM>
inline LuaCallback<ITEM, PARAM>::~LuaCallback()
{
    if (mFunction != -1)
    {
        lua_unref(LuaHelper::mState, mFunction);
    }
}
}
