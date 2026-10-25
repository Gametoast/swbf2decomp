#pragma once

#include <stddef.h>

#include "sndstreamnode.h"
#include "sndstreampacket.h"

namespace Snd
{

// VTABLE: SWBF2 0x00786d18
class StreamResampler : public StreamNode,
                        public StreamNodeSink,
                        public StreamNodeSource
{
public:
    StreamResampler();
    virtual ~StreamResampler();

    void SetBuffer(void* pBuffer, unsigned int uiSize);
    void Flush();

    void Update();
    virtual StreamPacket* GetPacket(unsigned int uiOutput);

private:
    void CompletePacket();

public:

    unsigned int mBufferSize;
    unsigned int mStepSize;
    StreamPacket* mInputPacket;
    unsigned int mInputPacketReadPos;
    StreamPacket mOutputPacket;
    int mSampleOffset;
    int mPrevSample;
};

 
 
 
 

}
