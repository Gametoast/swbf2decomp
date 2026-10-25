#pragma once

#include <stddef.h>

#include "sndstreamnode.h"
#include "sndstreampacket.h"

namespace Snd
{

class StreamSplitter : public StreamNode, public StreamNodeSink, public StreamNodeSource
{
public:
    StreamSplitter();

    virtual ~StreamSplitter();

    void Setup(StreamPacket** const ppQueue, unsigned short* const puRefCounts,
               unsigned short uNumRefCounts, unsigned short* const puRemaining,
               unsigned short uNumRemaining);

    void Flush();

    virtual StreamPacket* GetPacket(unsigned int uiOutput);

    StreamPacket** mPackets;
    unsigned short* mRemaining;
    unsigned short* mRefCount;
    unsigned short mNumOutputs;
    unsigned short mNumPackets;
    unsigned short mReadPos;
    unsigned short mFlushing;
};

 
 
 
 
 
 

}
