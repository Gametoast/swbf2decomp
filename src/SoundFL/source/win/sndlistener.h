#pragma once

#include "SoundFL/source/sndlistenerbase.h"

namespace Snd
{

class Listener : public ListenerBase
{
public:

    Listener();
    // FUNCTION: SWBF2 0x00733e90
    ~Listener()
    {
    }

    void Update();
};

 

}
