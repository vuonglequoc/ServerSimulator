//-----------------------------------------------------------------------------------------------------------
#ifndef GCS_PRTOCOOL
#define GCS_PROTOCOL
//-----------------------------------------------------------------------------------------------------------

#include "..\..\Protocol\ConnectProtocol.h"
//-----------------------------------------------------------------------------------------------------------

enum /* CONNECTSERVER HEADERS */
{
	CS_SERVER_SELECT	= 0x03,
	CS_CLIENT_CONNECT	= 0x06
};
//-----------------------------------------------------------------------------------------------------------

struct ServerInfo
{
	USHORT	ServerCode;
	BYTE	Percent;
	BYTE	UNK;
};
//-----------------------------------------------------------------------------------------------------------

struct ServerList
{
	PWMSG_HEAD			h;
	BYTE				SubHead;
	BYTE				ServersCountH;
	BYTE				ServersCountL;
	ServerInfo			Servers[361];
};
//-----------------------------------------------------------------------------------------------------------

void CSProtocolCore(int aIndex, BYTE HeadCode, LPBYTE aRecv, int iSize);
// ----
void CSConnectResultSend(int aIndex);
void CSServerListSend(int aIndex);
void ServerListReq(int aIndex);
void ServerInfoReq(int aIndex, USHORT ServerCode);
//-----------------------------------------------------------------------------------------------------------
#endif /* GCS_PROTOCOL */
//-----------------------------------------------------------------------------------------------------------
