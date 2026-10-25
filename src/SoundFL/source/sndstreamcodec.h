#pragma once

#include "sndstreamnode.h"
#include "sndstreamsourceproducer.h"

namespace Snd
{

class StreamCodec : public StreamNode,
                    public StreamNodeSink,
                    public StreamSourceProducer
{
public:

    // FUNCTION: SWBF2 0x0073d100
    virtual ~StreamCodec()
    {

    }

    virtual StreamPacket* GetPacket(unsigned int uiOutput);

protected:

    bool BufferPackets();
    bool Update();
};

 

}
