#pragma once
#include <stddef.h>
#include "Steam/galaxy_api.h"

struct NetSocketPeerRecord
{

    galaxy::api::GalaxyID mGalaxyId;
    unsigned char mSocketAddress[16];
};
 
 

class NetSocket
{
public:

    static void StartSockets();

    static void OpenBroadcastSockets();
    static int OpenDirectSocket(bool bHost);

    static void CloseDirectSocket();

    static void GetAddr(const void* pPeer, int& iIP, int& iPort);

    static int SendSocket(int iDest, char* pData, int iLen);
    static int SendVoiceSocket(int iDest, char* pData, int iLen, int iChannel);

    static void RecvSockets();

    static void RecvGalaxySockets();

    static unsigned short __cdecl SetAddr(void* pDest, unsigned int uIP, unsigned short uPort);
};
