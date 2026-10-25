#include <string.h>
#include <winsock2.h>

#include "NetMatch.h"
#include "NetSetup.h"
#include "pcNetSocket.h"
#include "NetComm.h"
#include "NetGame.h"
#include "NetPacket.h"
#include "PebbleFL/Common/PblJournal.h"
#include "PebbleFL/Common/PblUnicode.h"
#include "Steam/gfGalaxy.h"

#include "Team.h"
#include "PlayerProfile.h"
#include "LuaCallbacks_Options.h"

const char* GetNextMissionForLobby();
const char* GetEncodedLobbyPassword();
extern bool netAutoAim, netAutoAssignTeams, netTeamDamage, netShowNames, netHeroesEnabled;
extern int netNumBots, netPlayerLimit, netMinPlayersToAutoLaunchDedicated;
extern int gSpawnInvicibility;
extern int netHeroRuleUnlock, netHeroRuleUnlockVal, netHeroRuleTeam;
extern int netHeroRulePlayer, netHeroRuleRespawn, netHeroRuleRespawnVal;

#include "Steam/gfSteamAuth.h"
#include "Steam/utf8.h"
#include "FLGuiManager.h"
#include "ShellLoop.h"
#include "PebbleFL/Common/PblRandom.h"

void* GetGalaxyFriends();
void* GetGalaxyUser();
extern bool netLoginPending;
extern int netLoginFailureReason;
static void HandleQueryPackets();

// GLOBAL: SWBF2 0x01e64f30
char netLoginOverrideName[256];

extern int netDirectPort;
extern int netLoginErrorLevel;
extern int updatePercent;
extern int netNoCableCount;
extern int netConnectState;
extern int netMatchOperation;
extern int netLobbyState;
extern int netLanRegisterState;
extern char const* gameErrorMsg;
extern int gameErrorLvl;
extern char const* netLoginErrorMessage;
extern float netNextLanQueryTime;
extern int netLanQueryCount;
extern unsigned int netLanQuerySequence;
extern unsigned __int64 netLocalGSID;
extern unsigned int netBroadcastSendSocket;
extern unsigned int netBroadcastRecvSocket;
extern bool netLobbyRequestPending;
extern bool netLeavingLobby;
extern bool netHandlingFindPacket;
// GLOBAL: SWBF2 0x007e8e8c
static NetArch archFilter = NET_ARCH_ANY;
extern unsigned __int64 netEnteredLobbyId;
extern unsigned __int64 netCreatedLobbyId;
extern unsigned __int64 netFindSessionId;
extern char netLocalIPText[18];
extern NetAddr myAddr;

void* GetGalaxyMatchmaking();
hostent* getlocalhost();
bool PumpGalaxyMatchmaking();
void ResetLanRegistration();

void ReadString(NetPktGroup& group, char* text, unsigned int capacity);
void WriteString(NetPktGroup* group, const char* text, unsigned int capacity);

// FUNCTION: SWBF2 0x005d4690
unsigned int NetSetup::GetPassword()
{
    return NetSetup::mPasswordHash;
}

// FUNCTION: SWBF2 0x005d4b00
bool __fastcall IsPrivateIP(unsigned long netAddr)
{
    unsigned long hostAddr = ntohl(netAddr);
    unsigned int first = hostAddr >> 24;
    unsigned int second = (hostAddr >> 16) & 0xff;
    return first == 10 || (first == 172 && second >= 16 && second <= 31) ||
           (first == 192 && second == 168);
}

// FUNCTION: SWBF2 0x005d4b40
unsigned long GetPrivateIP()
{
    hostent* host = getlocalhost();
    if (!host)
    {
        return 0;
    }
    for (char** addr = host->h_addr_list; *addr; ++addr)
    {
        unsigned long ip = *(unsigned long*)*addr;
        if (IsPrivateIP(ip))
        {
            return ip;
        }
    }
    return 0;
}

// FUNCTION: SWBF2 0x005d5a60
char NetAddr::GetAddr(void* pOut, int) const
{
    ((NetSocketPeerRecord*)pOut)->mGalaxyId = mGalaxyId;
    if (!NetComm::IsLAN())
    {
        return 0;
    }
    if (!hasPriv)
    {
        __debugbreak();
    }
    return (char)NetSocket::SetAddr(pOut, (unsigned int)privAddr, (unsigned short)privPort);
}

// FUNCTION: SWBF2 0x005d5b70
bool __fastcall IsSameNet(const unsigned int* pLeft, const unsigned int* pRight)
{
    return NetComm::IsLAN() ||
           (pLeft[0] == pRight[0] && pLeft[1] == pRight[1]);
}

// FUNCTION: SWBF2 0x005d5bc0
int __fastcall ReadSessionId(NetPktGroup* pGroup, unsigned int* pWords)
{
    unsigned int words[2];
    pGroup->ReadBytes((char*)words, sizeof(words));
    pWords[0] = words[0];
    pWords[1] = words[1];
    return words[1];
}

// FUNCTION: SWBF2 0x005d5bf0
void NetMatch::StartRegisterGame(bool)
{
    netMatchOperation = 0;
    if (NetComm::IsLAN())
    {
        if (!netLanRegisterState)
        {
            netDirectPort = NetSocket::OpenDirectSocket(true);
            NetAddr* addr = &NetSetupP::GetMyAddr();
            if (addr->hasPriv)
            {
                addr->privPort = NetComm::GetDirectPort();
                netLanRegisterState = 1;
                ResetLanRegistration();
                netMatchOperation = 1;
            }
        }
        return;
    }
    if (!gf::isInitialized() || !GetGalaxyMatchmaking() || netEnteredLobbyId ||
        netLobbyRequestPending || netLobbyState)
    {
        return;
    }

    netMatchOperation = 1;
    void* matchmaking = GetGalaxyMatchmaking();
    ((galaxy::api::IMatchmaking*)matchmaking)->CreateLobby(2, NetSetup::GetMaxPlayers(), false, 2);
    if (!gf::checkError())
    {
        netLobbyState = 3;
        netLobbyRequestPending = true;
        netLeavingLobby = false;
        netCreatedLobbyId = 0;
        netEnteredLobbyId = 0;
    }
}

// FUNCTION: SWBF2 0x005d5e50
bool NetMatch::GetPlayerDataByAddress()
{
    return false;
}

// FUNCTION: SWBF2 0x005d5e60
bool NetMatch::PingServerByAddress(const NetAddr&)
{
    return gf::isInitialized() != 0;
}

// FUNCTION: SWBF2 0x005d5f00
int NetMatch::GetPercentDone()
{
    return NetMatch::IsDoneFindGames() ? 101 : updatePercent;
}

// FUNCTION: SWBF2 0x005d5f20
char __fastcall NetMatch::EndFindGames(bool clearMatchmaking)
{
    if (clearMatchmaking)
    {
        netMatchOperation = 0;
        if (netLanRegisterState != 2)
        {
            NetSocket::CloseDirectSocket();
            netDirectPort = 0;
        }
    }
    if (NetComm::IsLAN())
    {
        closesocket((SOCKET)netBroadcastSendSocket);
        netBroadcastSendSocket = 0;
        int result = closesocket((SOCKET)netBroadcastRecvSocket);
        netBroadcastRecvSocket = 0;
        return (char)result;
    }
    netLobbyState = 0;
    return 0;
}

// FUNCTION: SWBF2 0x005d5f70
void NetMatch::UpdateFindGames()
{
    if (NetComm::IsLAN())
    {
        UpdateFindLanGames();
    }
    else
    {
        PumpGalaxyMatchmaking();
    }
}

// FUNCTION: SWBF2 0x005d5f90
bool NetMatch::IsDoneFindGames()
{
    return !NetComm::IsLAN() && netLobbyState == 2;
}

// FUNCTION: SWBF2 0x005d5fb0
void NetMatch::StartJoinGame(NetAddr& address, NetKey&)
{
    netMatchOperation = 2;
    if (NetComm::IsLAN())
    {
        NetAddr* addr = &NetSetupP::GetMyAddr();
        if (addr->hasPriv)
        {
            addr->privPort = NetComm::GetDirectPort();
        }
        else
        {
            __debugbreak();
        }
    }
    else if (gf::isInitialized())
    {
        void* matchmaking = GetGalaxyMatchmaking();
        ((galaxy::api::IMatchmaking*)matchmaking)->JoinLobby(address.mGalaxyId);
        if (gf::checkError())
        {
            return;
        }
        netLobbyState = 3;
        netConnectState = 0;
    }
    else
    {
        netLobbyState = 3;
        netConnectState = 0;
    }
    netEnteredLobbyId = 0;
    netCreatedLobbyId = 0;
}

// FUNCTION: SWBF2 0x005d6040
char NetMatch::EndJoinGame()
{
    netMatchOperation = 0;
    char result = NetComm::IsLAN();
    if (result)
    {
        if (netLanRegisterState != 2)
        {
            NetSocket::CloseDirectSocket();
            netDirectPort = 0;
        }
    }
    else
    {
        netLobbyState = 0;
    }
    netEnteredLobbyId = 0;
    return result;
}

// FUNCTION: SWBF2 0x005d60a0
bool NetMatch::StartConnect(NetAddr& address, NetKey& key, int destination)
{
    return NetComm::IsLAN();
}

// FUNCTION: SWBF2 0x005d60b0
bool NetMatch::UpdateConnect(const NetAddr* pAddr, const NetKey* pKey, int iDest, int* pReason)
{
    if (NetComm::IsLAN())
    {
        *pReason = 13;
        return true;
    }
    *pReason = netEnteredLobbyId != 0 ? 13 : JRR_CONNECTION_LOST;
    PumpGalaxyMatchmaking();
    if (netLobbyState == 3)
    {
        return true;
    }
    switch (netConnectState)
    {
    case 0:
        return true;
    case 1:
        *pReason = 11;
        return false;
    case 2:
        *pReason = JRR_FULL;
        return false;
    default:
        *pReason = 12;
        return false;
    }
}

// FUNCTION: SWBF2 0x005d6150
void NetMatch::EndConnect(NetAddr&, NetKey&, int) {}

// FUNCTION: SWBF2 0x005d6310
char NetMatch::SetNoCableError()
{
    int count = ++netNoCableCount;
    if (count > 4 && netLoginErrorLevel < 9)
    {
        NetSetup::SetErrorMode(true);
        bool link = NetComm::IsLAN();
        netLoginErrorLevel = 9;
        netLoginErrorMessage = link ? "xlive.errors.nocable_syslink" : "xlive.errors.nocable";
        return link;
    }
    return (char)count;
}

// FUNCTION: SWBF2 0x005d6580
void NetMatch::SetLocalGSID(unsigned int lo, unsigned int hi)
{
    netLocalGSID = ((unsigned __int64)hi << 32) | lo;
    NetGame::SetPlayerGSID(0, netLocalGSID);
}

// FUNCTION: SWBF2 0x005d6710
void NetMatch::ClearGameError()
{
    gameErrorMsg = 0;
    gameErrorLvl = 0;
}

// FUNCTION: SWBF2 0x005d67f0
void __fastcall NetMatch::ReceiveLanRegisterPacket(NetPacket* packet, const void* source)
{
    if (netIsHost)
    {
        HandleFindPacket(packet);
    }
    else
    {
        HandleFoundPacket(packet, source);
    }
}

// FUNCTION: SWBF2 0x005d72d0
void __fastcall HandleFindPacket(NetPacket* packet)
{
    if (netHandlingFindPacket)
    {
        packet->Free();
        return;
    }
    netHandlingFindPacket = true;
    if (packet->mData[2])
    {
        packet->Free();
        netHandlingFindPacket = false;
        return;
    }

    NetPktGroup group;
    group.Set(packet);
    group.BeginRead();
    unsigned int challenge = 0, type = 0;
    group.ReadBits(challenge, 32);
    group.ReadBits(type, 12);
    bool accepted = false;
    if (type == 1)
    {
        group.ReadBits(type, 2);
        accepted = archFilter == NET_ARCH_ANY || type == (unsigned int)archFilter;
    }
    packet->Free();
    if (accepted)
    {
        group.BeginWrite();
        group.WriteBits(challenge, 32);
        group.WriteBits(1, 12);
        group.WriteBits(NetComm::GetMyArch(), 2);
        WriteString(&group, NetSetup::GetHostName(), 64);
        WriteString(&group, NetSetup::GetGameType(), 64);
        if (NetComm::IsLAN())
        {
            group.WriteBits(0, 32);
            group.WriteBits(0, 32);
            group.WriteBits(NetComm::GetDirectPort(), 16);
        }
        group.WriteBits(NetSetup::GetMaxPlayers(), 6);
        group.WriteBits(NetSetup::GetNumPlayers(), 6);
        group.EndWrite();
        NetComm::Send(68, PKT_FOUND, group.pktPtr);
    }
    netHandlingFindPacket = false;
}

// FUNCTION: SWBF2 0x005d7450
void __fastcall HandleFoundPacket(NetPacket* packet, const void* source)
{
    NetPktGroup group;
    group.Set(packet);
    group.BeginRead();
    if ((unsigned char)packet->mData[2] == 1)
    {
        unsigned int sequence;
        group.ReadBits(sequence, 32);
        if (sequence == netLanQuerySequence)
        {
            unsigned int type;
            group.ReadBits(type, 12);
            if (type == 1)
            {
                group.ReadBits(type, 2);
                FindGameData data = {};
                ReadString(group, data.mName, 64);
                ReadString(group, data.mMapType, 32);
                NetAddr address;
                memset(&address, 0, sizeof(address));
                NetKey key;

                NetSessionId session;
                if (NetComm::IsLAN())
                {
                    group.ReadBits(key.id, 32);
                    unsigned int unusedAddress;
                    group.ReadBits(unusedAddress, 32);
                    unsigned int port;
                    group.ReadBits(port, 16);
                    int sourceIp, sourcePort;
                    NetSocket::GetAddr(source, sourceIp, sourcePort);
                    NetAddr from(sourceIp, port, 0);
                    address = from;
                }
                group.ReadBits((unsigned int&)data.mMaxPlayers, 6);
                group.ReadBits((unsigned int&)data.mNumPlayers, 6);
                NetSetupP::AddGame(&address);
                NetSetupP::UpdateGame(&address, &key, &session, NetSetup::GetGameVersion(), &data, 0);
            }
        }
    }
    packet->Free();
}

// FUNCTION: SWBF2 0x005d7650
void UpdateFindLanGames()
{
    float now = NetComm::GetTime();
    if (now > netNextLanQueryTime)
    {
        ++netLanQueryCount;
        netNextLanQueryTime = now + 0.2f;
        NetPktGroup packet;
        packet.BeginWrite();
        packet.WriteBits(netLanQuerySequence, 32);
        packet.WriteBits(1, 12);
        packet.WriteBits(NetComm::GetMyArch(), 2);
        packet.EndWrite();
        NetComm::Send(68, PKT_FIND, packet.pktPtr);
    }
    for (NetPacket* packet = NetComm::Recv(PKT_FOUND, PKT_FIND);
         packet; packet = NetComm::Recv(PKT_FOUND, PKT_FIND))
    {
        HandleFoundPacket(packet, 0);
    }
}

// FUNCTION: SWBF2 0x005d7770
unsigned int NetMatch::HostGetAddr(const NetAddr* pAddr, void* pOut, int iDest)
{
    unsigned __int64* peer = NetCommP::GetAddr(iDest);
    if (!NetComm::IsLAN())
    {
        memcpy(pOut, peer, sizeof(*peer));
        return (unsigned int)(*peer >> 32);
    }
    int ip, port;
    NetSocket::GetAddr(peer, ip, port);
    if (!IsPrivateIP(ip))
    {
        __debugbreak();
    }
    else
    {
        NetAddr* output = (NetAddr*)pOut;
        output->mGalaxyId = pAddr->mGalaxyId;
        output->hasPriv = pAddr->hasPriv;
        output->privAddr = ip;
        output->privPort = port;
    }
    myAddr.CopyFieldsFrom(pAddr);
    return (unsigned int)&myAddr;
}

// FUNCTION: SWBF2 0x005d7830
char* NetMatch::GetIPAddr()
{
    netLocalIPText[0] = 0;
    hostent* host = getlocalhost();
    if (host)
    {
        for (char** addr = host->h_addr_list; *addr; ++addr)
        {
            strlcpy(netLocalIPText, inet_ntoa(*(in_addr*)*addr), sizeof(netLocalIPText));
        }
        JournalString(netLocalIPText, sizeof(netLocalIPText));
    }
    return netLocalIPText;
}

// FUNCTION: SWBF2 0x005d7890
unsigned int __fastcall NetMatch::GetSessionId(NetSessionId* pId)
{
    pId->id = netCreatedLobbyId;
    return (unsigned int)(netCreatedLobbyId >> 32);
}

// FUNCTION: SWBF2 0x005d6370
char const* NetMatch::GetLoginOverrideName(int  )
{
    if (NetComm::IsLAN() || !gf::isInitialized())
    {
        return 0;
    }

    if (!netLoginOverrideName[0])
    {
        std::string sName;
        const char* pcName = 0;
        if (gf::_sap())
        {
            pcName = gf::_sap()->GetPersonaName();
        }
        if (!pcName)
        {
            void* pFriends = GetGalaxyFriends();
            pcName = ((galaxy::api::IFriends*)pFriends)->GetPersonaName();
        }
        sName = pcName;

        std::string sValid;
        utf8::replace_invalid(sName.begin(), sName.end(), std::back_inserter(sValid), '?');
        sName.clear();

        std::string::iterator it = sValid.begin();
        std::string::iterator end = sValid.end();
        while (it != end)
        {
            unsigned int cp = utf8::next(it, end);
            if (cp > 0x80)
            {
                cp = '?';
            }
            utf8::append(cp, std::back_inserter(sName));
        }

        memset(netLoginOverrideName, 0, sizeof(netLoginOverrideName));
        sName.copy(netLoginOverrideName, sizeof(netLoginOverrideName) - 1);
    }
    return netLoginOverrideName;
}

// FUNCTION: SWBF2 0x005d62b0
bool NetMatch::UpdateLogin(int iViewport)
{
    PumpGalaxyMatchmaking();
    return !netLoginPending;
}

// FUNCTION: SWBF2 0x005d5e00
void NetMatch::UpdateRegisterGame()
{
    if (NetComm::IsLAN())
    {
        if (netLanRegisterState)
        {
            NetPacket* pPkt;
            while ((pPkt = NetComm::Recv(PKT_FIND, PKT_FOUND)) != 0)
            {
                HandleFindPacket(pPkt);
            }
            HandleQueryPackets();
        }
    }
    else
    {
        HandleQueryPackets();
        PumpGalaxyMatchmaking();
    }
}

// FUNCTION: SWBF2 0x005d62d0
bool NetMatch::IsLoginSuccessful(int  , bool  )
{
    if (!gf::isInitialized())
    {
        return false;
    }
    void* pUser = GetGalaxyUser();
    return ((galaxy::api::IUser*)pUser)->SignedIn();
}

// FUNCTION: SWBF2 0x005d62f0
bool NetMatch::UpdatePresence(bool  )
{
    return false;
}

// FUNCTION: SWBF2 0x005d6300
void NetMatch::GetLoginError(char const*& pcError, int& iSeverity)
{
    pcError = netLoginErrorMessage;
    iSeverity = netLoginErrorLevel;
}

// FUNCTION: SWBF2 0x005d65e0
void NetMatch::CancelLogin(int  , bool  )
{
    netLoginErrorMessage = 0;
    netLoginErrorLevel = 0;
    netNoCableCount = 0;
    if (gf::isInitialized())
    {
        void* pFriends = GetGalaxyFriends();
        ((galaxy::api::IFriends*)pFriends)->DeleteRichPresence("connect");
    }
    netLobbyState = 0;
    netCreatedLobbyId = 0;
    netEnteredLobbyId = 0;
    netConnectState = 0;
    netLoginPending = false;
    netLoginFailureReason = 0;
    netLobbyRequestPending = false;
    netLeavingLobby = false;
    updatePercent = 0;
}

// FUNCTION: SWBF2 0x005d6680
void NetMatch::GetGameError(char const*& pcError, int& iSeverity)
{
    pcError = gameErrorMsg;
    iSeverity = gameErrorLvl;
}

// GLOBAL: SWBF2 0x01e6504c
char const* gameErrorMsg;

// GLOBAL: SWBF2 0x01e65050
int gameErrorLvl;

// GLOBAL: SWBF2 0x01e64f08
int updatePercent;

// FUNCTION: SWBF2 0x005d5b00
void __fastcall ReadAddr(NetPktGroup* pGroup, NetAddr* pAddr)
{
    pGroup->ReadBytes((char*)pAddr, 8);
    unsigned int uValue;
    pGroup->ReadBits(uValue, 1);
    pAddr->hasPriv = uValue != 0;
    if (pAddr->hasPriv)
    {
        pGroup->ReadBits(uValue, 32);
        pAddr->privAddr = uValue;
        pGroup->ReadBits(uValue, 16);
        pAddr->privPort = uValue;
    }
}

// FUNCTION: SWBF2 0x005d7760
bool NetMatch::UpdateFindAddress()
{
    return true;
}

// FUNCTION: SWBF2 0x005d7820
int __fastcall NetMatch::SetAddr(unsigned int* pOut, const unsigned int* pIn)
{
    pOut[0] = pIn[0];
    pOut[1] = pIn[1];
    return pIn[1];
}

// FUNCTION: SWBF2 0x005d5ba0
void __fastcall WriteSessionId(NetPktGroup* pGroup, const unsigned int* pWords)
{
    unsigned int words[2] = { pWords[0], pWords[1] };
    pGroup->WriteBytes((const char*)words, sizeof(words));
}

// FUNCTION: SWBF2 0x005d5aa0
void __fastcall WriteAddr(NetPktGroup* pGroup, const NetAddr* pAddr)
{
    unsigned __int64 publicId = pAddr->mGalaxyId.value;
    pGroup->WriteBytes((const char*)&publicId, sizeof(publicId));
    pGroup->WriteBits(pAddr->hasPriv ? 1 : 0, 1);
    if (pAddr->hasPriv)
    {
        pGroup->WriteBits((unsigned int)pAddr->privAddr, 32);
        pGroup->WriteBits((unsigned int)pAddr->privPort, 16);
    }
}

// FUNCTION: SWBF2 0x005d7750
void NetMatch::EndFindAddress() {}

// FUNCTION: SWBF2 0x005d7810
void NetMatch::ClientGetAddr() {}

// FUNCTION: SWBF2 0x005d6730
NetAddr::NetAddr(int addr, int port, int overridePort)
{
    mGalaxyId.value = 0;
    if (!IsPrivateIP((unsigned long)addr))
    {
        __debugbreak();
        return;
    }
    privAddr = addr;
    privPort = overridePort ? overridePort : port;
    hasPriv = true;
}

// FUNCTION: SWBF2 0x005d7700
void NetMatch::StartFindAddress()
{
    NetAddr* addr = &NetSetupP::GetMyAddr();
    if (NetComm::IsLAN())
    {
        unsigned long ip = GetPrivateIP();
        addr->privAddr = ip;
        if (ip)
        {
            addr->mGalaxyId.value = 0;
            addr->hasPriv = true;
        }
    }
    else
    {
        addr->hasPriv = false;
        addr->privAddr = 0;
        addr->privPort = 0;
    }
}

// FUNCTION: SWBF2 0x005d65b0
unsigned __int64 NetMatch::GetLocalGSID()
{
    return netLocalGSID;
}

// FUNCTION: SWBF2 0x005d78a0
void NetMatch::SetRegisterArchFilter()
{
    archFilter = NET_ARCH_ANY;
}

// FUNCTION: SWBF2 0x005d67b0
void EndRegisterLanGame()
{
    NetMatch::SetRegisterArchFilter();
    closesocket(netBroadcastSendSocket);
    netBroadcastSendSocket = 0;
    closesocket(netBroadcastRecvSocket);
    netBroadcastRecvSocket = 0;
}

// FUNCTION: SWBF2 0x005d6690
void NetMatch::SetGameError(const char* message, int severity)
{
    extern const char* gameErrorMsg;
    extern int gameErrorLvl;
    gameErrorMsg = message;
    gameErrorLvl = severity;
    if (netInShell || severity < 6)
    {
        return;
    }
    GuiManager* gui = GuiManager::Instance();
    if (gui->IsErrorOpen())
    {
        gui->CloseError(false, true);
    }
    NetSetup::RequestQuit(0);
    ShellLoop::SetNetCableErrorOnEnter(severity, message);
    gameErrorMsg = 0;
    gameErrorLvl = 0;
}

// FUNCTION: SWBF2 0x005d5cf0
void NetMatch::EndRegisterGame()
{
    if (NetComm::IsLAN())
    {
        EndRegisterLanGame();
        NetSocket::CloseDirectSocket();
        netDirectPort = 0;
        netLanRegisterState = 0;
        return;
    }
    if (!gf::isInitialized())
    {
        return;
    }
    if (netCreatedLobbyId)
    {
        void* matchmaking = GetGalaxyMatchmaking();
        ((galaxy::api::IMatchmaking*)matchmaking)->SetLobbyData(
            galaxy::api::GalaxyID(netCreatedLobbyId), "galaxy_lobby_cancelled", gf::utils::ToString(true));
    }
    if (netEnteredLobbyId)
    {
        void* matchmaking = GetGalaxyMatchmaking();
        ((galaxy::api::IMatchmaking*)matchmaking)->LeaveLobby(galaxy::api::GalaxyID(netEnteredLobbyId));
        netLeavingLobby = true;
    }
    netEnteredLobbyId = 0;
    netCreatedLobbyId = 0;
}

// FUNCTION: SWBF2 0x005d5e30
void NetMatch::ChangeRegisterGame()
{
}

// FUNCTION: SWBF2 0x005d5e40
void __cdecl NetMatch::ReceiveRegisterPacket(const char*  , int  , const void*  )
{
}

// FUNCTION: SWBF2 0x005d65c0
void NetMatch::HandleLoginError(bool)
{
    netLoginErrorMessage = 0;
    netLoginErrorLevel = 0;
    netNoCableCount = 0;
}

// FUNCTION: SWBF2 0x005d5f60
void NetMatch::PauseFindGames() {}

// FUNCTION: SWBF2 0x005d6810
static void StartFindLanGames()
{
    NetSocket::OpenBroadcastSockets();
    netNextLanQueryTime = 0.0f;
    netLanQueryCount = 0;
    netLanQuerySequence = netRandom.GetInt();
}

// FUNCTION: SWBF2 0x005d5e70
void NetMatch::StartFindGames(int, const NetSessionId* id)
{
    if (id)
    {
        netFindSessionId = id->id;
    }
    netMatchOperation = 2;
    if (NetComm::IsLAN())
    {
        if (!netDirectPort)
        {
            netDirectPort = NetSocket::OpenDirectSocket(false);
        }
        StartFindLanGames();
        return;
    }
    if (gf::isInitialized())
    {
        netLobbyState = 1;
        void* matchmaking = GetGalaxyMatchmaking();
        ((galaxy::api::IMatchmaking*)matchmaking)->AddRequestLobbyListStringFilter("galaxy_lobby_cancelled", "1", 1);
        matchmaking = GetGalaxyMatchmaking();
        ((galaxy::api::IMatchmaking*)matchmaking)->RequestLobbyList(false);
    }
}

// FUNCTION: SWBF2 0x005d6090
void NetMatch::UpdateJoinGame()
{
    if (!NetComm::IsLAN())
    {
        PumpGalaxyMatchmaking();
    }
}

// GLOBAL: SWBF2 0x007ec514
unsigned int netLobbyPublishPumps = 5;

// FUNCTION: SWBF2 0x005d6880
static void HandleQueryPackets()
{
    if (NetComm::IsLAN())
    {
        NetPacket* pPkt;
        while ((pPkt = NetComm::Recv(PKT_QUERY, PKT_NONE)) != 0)
        {
            if (!NetGame::IsHost())
            {
                pPkt->Free();
                continue;
            }

            int iSrc = (unsigned char)pPkt->mData[3];
            NetPktGroup group;
            group.Set(pPkt);
            group.BeginRead();
            unsigned int uWord = 0;
            unsigned int uNibble = 0;
            unsigned int uNone;
            group.ReadBits(uNone, 1);
            if (!uNone)
            {
                group.ReadBits(uWord, 32);
                group.ReadBits(uNibble, 4);
            }
            pPkt->Free();

            int iReinf0 = Team::GetPtr(1) ? Team::GetPtr(1)->GetCurReinforcements() : 300;
            int iReinf1 = Team::GetPtr(2) ? Team::GetPtr(2)->GetCurReinforcements() : 300;
            int iPlaying = 0;
            for (int i = 0; i < 64; i++)
            {
                bool bPlaying = NetGame::IsPlaying(i, false);
                if (!NetGame::IsClient())
                {
                    bPlaying |= NetGame::IsPlayingNext(i);
                }
                if (bPlaying)
                {
                    iPlaying++;
                }
            }
            unsigned char u8Turns = PlayerProfile::GetCurProfile()->mOnlineOptions.mu8TurnsPerSecond;

            group.BeginWrite();
            group.WriteBits(uWord, 32);
            group.WriteBits(uNibble, 4);
            WriteString(&group, NetSetup::GetHostName(), 64);
            WriteString(&group, NetSetup::GetGameType(), 64);
            WriteString(&group, GetNextMissionForLobby(), 64);
            group.WriteBits(netAutoAim != 0, 1);
            group.WriteBits(netAutoAssignTeams != 0, 1);
            group.WriteBits(netTeamDamage != 0, 1);
            group.WriteBits(NetSetup::mPasswordHash != 0, 1);
            group.WriteBits(netNumBots, 8);
            group.WriteBits(iReinf0, 11);
            group.WriteBits(iReinf1, 11);
            group.WriteBits(iPlaying, 7);
            group.WriteBits(netPlayerLimit, 7);
            group.WriteBits(1, 3);
            group.WriteBits(u8Turns, 8);
            group.WriteBits(netMinPlayersToAutoLaunchDedicated, 7);
            group.WriteBits(PlayerPrefs::mAllUserPrefs[0].mDifficulty, 2);
            group.WriteBits(netShowNames != 0, 1);
            group.WriteBits(gSpawnInvicibility, 6);
            group.WriteBits(netHeroesEnabled != 0, 1);
            if (netHeroesEnabled)
            {
                group.WriteBits(netHeroRuleUnlock, 4);
                group.WriteBits(netHeroRuleUnlockVal, 8);
                group.WriteBits(netHeroRuleTeam, 3);
                group.WriteBits(netHeroRulePlayer, 4);
                group.WriteBits(netHeroRuleRespawn, 2);
                group.WriteBits(netHeroRuleRespawnVal, 10);
            }
            group.EndWrite();
            NetComm::Send(iSrc, PKT_QUERY_RESPONSE, group.pktPtr);
        }
        return;
    }

    if (!NetGame::IsHost() || netCreatedLobbyId == 0)
    {
        return;
    }

    if (netLobbyPublishPumps >= 5)
    {
        netLobbyPublishPumps = 0;
        galaxy::api::IMatchmaking* matchmaking;
        matchmaking = (galaxy::api::IMatchmaking*)GetGalaxyMatchmaking();
        matchmaking->SetLobbyData(
            galaxy::api::GalaxyID(netCreatedLobbyId), "fgd_str_host_name", NetSetup::GetHostName());
        matchmaking = (galaxy::api::IMatchmaking*)GetGalaxyMatchmaking();
        matchmaking->SetLobbyData(
            galaxy::api::GalaxyID(netCreatedLobbyId), "fgd_str_game_type", NetSetup::GetGameType());
        matchmaking = (galaxy::api::IMatchmaking*)GetGalaxyMatchmaking();
        matchmaking->SetLobbyData(
            galaxy::api::GalaxyID(netCreatedLobbyId), "fgd_str_map_name", GetNextMissionForLobby());
        matchmaking = (galaxy::api::IMatchmaking*)GetGalaxyMatchmaking();
        matchmaking->SetLobbyData(
            galaxy::api::GalaxyID(netCreatedLobbyId), "fgr_bool_netAutoAim", gf::utils::ToString(netAutoAim));
        matchmaking = (galaxy::api::IMatchmaking*)GetGalaxyMatchmaking();
        matchmaking->SetLobbyData(
            galaxy::api::GalaxyID(netCreatedLobbyId), "fgr_bool_netAutoAssignTeams", gf::utils::ToString(netAutoAssignTeams));
        matchmaking = (galaxy::api::IMatchmaking*)GetGalaxyMatchmaking();
        matchmaking->SetLobbyData(
            galaxy::api::GalaxyID(netCreatedLobbyId), "fgr_bool_netTeamDamage", gf::utils::ToString(netTeamDamage));
        matchmaking = (galaxy::api::IMatchmaking*)GetGalaxyMatchmaking();
        matchmaking->SetLobbyData(
            galaxy::api::GalaxyID(netCreatedLobbyId), "fgd_bool_has_password", gf::utils::ToString(NetSetup::mPasswordHash != 0));
        if (NetSetup::mPasswordHash)
        {
            matchmaking = (galaxy::api::IMatchmaking*)GetGalaxyMatchmaking();
            matchmaking->SetLobbyData(
            galaxy::api::GalaxyID(netCreatedLobbyId), "galaxy_lobby_password", GetEncodedLobbyPassword());
        }

        int iReinf0 = Team::GetPtr(1) ? Team::GetPtr(1)->GetCurReinforcements() : 300;
        int iReinf1 = Team::GetPtr(2) ? Team::GetPtr(2)->GetCurReinforcements() : 300;
        int iPlaying = 0;
        for (int i = 0; i < 64; i++)
        {
            bool bPlaying = NetGame::IsPlaying(i, false);
            if (!NetGame::IsClient())
            {
                bPlaying |= NetGame::IsPlayingNext(i);
            }
            if (bPlaying)
            {
                iPlaying++;
            }
        }
        matchmaking = (galaxy::api::IMatchmaking*)GetGalaxyMatchmaking();
        matchmaking->SetLobbyData(
            galaxy::api::GalaxyID(netCreatedLobbyId), "fgr_int_netNumBots", gf::utils::ToString(netNumBots));
        matchmaking = (galaxy::api::IMatchmaking*)GetGalaxyMatchmaking();
        matchmaking->SetLobbyData(
            galaxy::api::GalaxyID(netCreatedLobbyId), "fgr_int_r0", gf::utils::ToString(iReinf0));
        matchmaking = (galaxy::api::IMatchmaking*)GetGalaxyMatchmaking();
        matchmaking->SetLobbyData(
            galaxy::api::GalaxyID(netCreatedLobbyId), "fgr_int_r1", gf::utils::ToString(iReinf1));
        matchmaking = (galaxy::api::IMatchmaking*)GetGalaxyMatchmaking();
        matchmaking->SetLobbyData(
            galaxy::api::GalaxyID(netCreatedLobbyId), "fgd_int_numPlayers", gf::utils::ToString(iPlaying));
        matchmaking = (galaxy::api::IMatchmaking*)GetGalaxyMatchmaking();
        matchmaking->SetLobbyData(
            galaxy::api::GalaxyID(netCreatedLobbyId), "fgd_int_netPlayerLimit", gf::utils::ToString(netPlayerLimit));
        unsigned char u8Turns = PlayerProfile::GetCurProfile()->mOnlineOptions.mu8TurnsPerSecond;
        matchmaking = (galaxy::api::IMatchmaking*)GetGalaxyMatchmaking();
        matchmaking->SetLobbyData(
            galaxy::api::GalaxyID(netCreatedLobbyId), "fgd_int_serverType", gf::utils::ToString(1));
        matchmaking = (galaxy::api::IMatchmaking*)GetGalaxyMatchmaking();
        matchmaking->SetLobbyData(
            galaxy::api::GalaxyID(netCreatedLobbyId), "fgd_int_turns_per_second", gf::utils::ToString((int)u8Turns));
        matchmaking = (galaxy::api::IMatchmaking*)GetGalaxyMatchmaking();
        matchmaking->SetLobbyData(
            galaxy::api::GalaxyID(netCreatedLobbyId), "fgr_int_netMinPlayersToAutoLaunchDedicated", gf::utils::ToString(netMinPlayersToAutoLaunchDedicated));
        matchmaking = (galaxy::api::IMatchmaking*)GetGalaxyMatchmaking();
        matchmaking->SetLobbyData(
            galaxy::api::GalaxyID(netCreatedLobbyId), "fgr_int_mDifficulty", gf::utils::ToString(PlayerPrefs::mAllUserPrefs[0].mDifficulty));
        matchmaking = (galaxy::api::IMatchmaking*)GetGalaxyMatchmaking();
        matchmaking->SetLobbyData(
            galaxy::api::GalaxyID(netCreatedLobbyId), "fgr_int_netShowNames", gf::utils::ToString(netShowNames));
        matchmaking = (galaxy::api::IMatchmaking*)GetGalaxyMatchmaking();
        matchmaking->SetLobbyData(
            galaxy::api::GalaxyID(netCreatedLobbyId), "fgr_int_gSpawnInvicibility", gf::utils::ToString(gSpawnInvicibility));
        matchmaking = (galaxy::api::IMatchmaking*)GetGalaxyMatchmaking();
        matchmaking->SetLobbyData(
            galaxy::api::GalaxyID(netCreatedLobbyId), "fgr_int_netHeroesEnabled", gf::utils::ToString(netHeroesEnabled));
        if (netHeroesEnabled)
        {
            matchmaking = (galaxy::api::IMatchmaking*)GetGalaxyMatchmaking();
            matchmaking->SetLobbyData(
            galaxy::api::GalaxyID(netCreatedLobbyId), "fgr_uint_netHeroRuleUnlock", gf::utils::ToString((unsigned int)netHeroRuleUnlock));
            matchmaking = (galaxy::api::IMatchmaking*)GetGalaxyMatchmaking();
            matchmaking->SetLobbyData(
            galaxy::api::GalaxyID(netCreatedLobbyId), "fgr_int_netHeroRuleUnlockVal", gf::utils::ToString(netHeroRuleUnlockVal));
            matchmaking = (galaxy::api::IMatchmaking*)GetGalaxyMatchmaking();
            matchmaking->SetLobbyData(
            galaxy::api::GalaxyID(netCreatedLobbyId), "fgr_uint_netHeroRuleTeam", gf::utils::ToString((unsigned int)netHeroRuleTeam));
            matchmaking = (galaxy::api::IMatchmaking*)GetGalaxyMatchmaking();
            matchmaking->SetLobbyData(
            galaxy::api::GalaxyID(netCreatedLobbyId), "fgr_uint_netHeroRulePlayer", gf::utils::ToString((unsigned int)netHeroRulePlayer));
            matchmaking = (galaxy::api::IMatchmaking*)GetGalaxyMatchmaking();
            matchmaking->SetLobbyData(
            galaxy::api::GalaxyID(netCreatedLobbyId), "fgr_uint_netHeroRuleRespawn", gf::utils::ToString((unsigned int)netHeroRuleRespawn));
            matchmaking = (galaxy::api::IMatchmaking*)GetGalaxyMatchmaking();
            matchmaking->SetLobbyData(
            galaxy::api::GalaxyID(netCreatedLobbyId), "fgr_uint_netHeroRuleRespawnVal", gf::utils::ToString((unsigned int)netHeroRuleRespawnVal));
        }
        matchmaking = (galaxy::api::IMatchmaking*)GetGalaxyMatchmaking();
        matchmaking->SetLobbyData(
            galaxy::api::GalaxyID(netCreatedLobbyId), "galaxy_revision", gf::utils::ToString(1u));
        matchmaking = (galaxy::api::IMatchmaking*)GetGalaxyMatchmaking();
        matchmaking->SetLobbyData(
            galaxy::api::GalaxyID(netCreatedLobbyId), "galaxy_lobby_cancelled", gf::utils::ToString(false));

        void* pMatchmaking = GetGalaxyMatchmaking();
        ((galaxy::api::IMatchmaking*)pMatchmaking)->SetLobbyJoinable(galaxy::api::GalaxyID(netCreatedLobbyId), true);
    }
    netLobbyPublishPumps++;
}

