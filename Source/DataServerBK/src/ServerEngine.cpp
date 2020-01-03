// ServerEngine.cpp: implementation of the CServerEngine class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ServerEngine.h"
#include "eDataServer.h"
#include "LogProc.h"
#include "Sprotocol.h"
#include "CSprotocol.h"
#include "DSprotocol.h"
#include "EDSprotocol.h"
#include "RSprotocol.h"
#include "Eprotocol.h"


tagSERVER_ENGINE * g_Server = NULL;

void gObjServerInit()
{
	g_Server = new tagSERVER_ENGINE[g_dwMaxServerGroups]; 

	if ( g_Server == NULL )
	{
		MsgBox("[Server Engine] Memory Allocation Error");
		return;
	}

	memset(g_Server, 0, sizeof(tagSERVER_ENGINE) * g_dwMaxServerGroups);

	for(int i=0;i<g_dwMaxServerGroups;i++)
	{
		g_Server[i].m_Index = -1;
		g_Server[i].m_Socket = INVALID_SOCKET;
		g_Server[i].m_State = SS_CLOSED;
		g_Server[i].m_Type = ST_NONE;
		g_Server[i].ProtocolCore1 = NULL;
		g_Server[i].ProtocolCore2 = NULL;
		g_Server[i].PerSocketContext = new _PER_SOCKET_CONTEXT;
		g_Server[i].m_ServerCode = -1;
	}
}


int gObjServerAddSearch()
{
	if ( g_Server == NULL )
		return -1;

	for(int i=0;i<g_dwMaxServerGroups;i++)
	{
		if ( g_Server[i].m_Index == -1 )
		{
			return i;
		}
	}

	return -1;
}


int gObjServerAdd(SOCKET Socket, char * Ip, int ServerIndex, eSERVER_TYPE eServerType)
{
	g_Server[ServerIndex].m_Index = ServerIndex;
	memcpy(g_Server[ServerIndex].m_ServerIp, Ip, 16);
	g_Server[ServerIndex].m_State = SS_CONNECTED;
	g_Server[ServerIndex].m_Type = eServerType;
	g_Server[ServerIndex].m_Socket = Socket;

	switch ( g_Server[ServerIndex].m_Type )
	{
		case ST_FSGATE:
			break;
		case ST_CHATSERVER:
			break;
		case ST_CONNECTSERVER:
			break;
			
		case ST_JOINSERVER:
			g_Server[ServerIndex].ProtocolCore1 = SProtocolCore;
			break;
		case ST_DATASERVER1:
			g_Server[ServerIndex].ProtocolCore1 = DSProtocolCore;
			break;
		case ST_DATASERVER2:
			g_Server[ServerIndex].ProtocolCore1 = DSProtocolCore;
			break;
		case ST_EXDATASERVER:
			g_Server[ServerIndex].ProtocolCore1 = EDSProtocolCore;
			break;
		case ST_EVENTSERVER:
			g_Server[ServerIndex].ProtocolCore1 = EProtocolCore;
			break;
		case ST_RANKINGSERVER:
			g_Server[ServerIndex].ProtocolCore1 = RSProtocolCore;
			break;
		case ST_CASHSHOPSERVER:
			g_Server[ServerIndex].ProtocolCore2 = CSProtocolCore;
			break;
	}
	
	LogAddTD("[Server Engine] Connect : Index : %d - IP : %s - ServerType : %d",
		ServerIndex, Ip, eServerType);

	return ServerIndex;
}



void gObjServerDel(int aIndex)
{
	g_Server[aIndex].m_Index = -1;
	g_Server[aIndex].m_Socket = INVALID_SOCKET;
	g_Server[aIndex].m_ServerIp[0];
	g_Server[aIndex].m_State = SS_CLOSED;
	g_Server[aIndex].m_Type = ST_NONE;
	g_Server[aIndex].ProtocolCore1 = NULL;
	g_Server[aIndex].ProtocolCore2 = NULL;
}





int MuLoginFindServer(WORD ServerCode)
{
	for(int i=0;i<MAX_LOGIN_SERVER;i++)
	{
		if ( g_MuLoginServerData[i].m_wServerCode == ServerCode )
		{
			return i;
		}
	}

	return -1;
}


BOOL MuLoginAddServer(LPTSTR ServerName, WORD ServerCode, WORD Port)
{
	//if ( MuLoginFindServer(ServerCode) != -1 )
	if ( MuLoginFindServer(ServerCode) != -1 )
	{
		LogAddC(2, "[MeMuOnlineDB] GameServer %s trying to connect with a ServerCode %d already used",
			ServerName, ServerCode);

		return FALSE;
	}

	for(int i=0;i<MAX_LOGIN_SERVER;i++)
	{
		if ( g_MuLoginServerData[i].m_szServerName[0] == 0 )
		{
			if ( g_MuLoginServerData[i].m_szServerName[1] == 0 )
			{
				if ( g_MuLoginServerData[i].m_wServerCode == -1 )
				{
					g_MuLoginServerData[i].m_wPort = Port;
					g_MuLoginServerData[i].m_wServerCode = ServerCode;
					memcpy(g_MuLoginServerData[i].m_szServerName, ServerName, 50);
					g_MuLoginServerDataCount++;

					return TRUE;
				}
			}
		}
	}

	return FALSE;
}





void MuLoginDeleteServer(WORD ServerCode)
{
	for(int i=0;i<MAX_LOGIN_SERVER;i++)
	{
		if ( g_MuLoginServerData[i].m_wServerCode == ServerCode )
		{
			memset(g_MuLoginServerData[i].m_szServerName, 0, 50);
			g_MuLoginServerData[i].m_wPort = 0;
			g_MuLoginServerData[i].m_wServerCode = -1;
			g_MuLoginServerDataCount--;
			break;
		}
	}
}







DWORD MakeAccountKey(LPTSTR lpszAccountID)
{
	int len = strlen(lpszAccountID);

	if ( len <= 0 )
		return 0;

	DWORD dwKey = 0;

	for(int i=0;i<len;i++)
		dwKey += lpszAccountID[i] + 17;
	
	dwKey = (dwKey + (10 - len) * 17) % 256;
	return dwKey % 256;
}


BOOL MuLoginAddUser(WORD ServerCode, char * szAccountID)
{
	for(int i=0;i<MAX_LOGIN_USER;i++)
	{
		if ( g_MuLoginUserData[i].m_AccoundID[0] == 0 )
		{
			if ( g_MuLoginUserData[i].m_AccoundID[1] == 0 )
			{
				memcpy(g_MuLoginUserData[i].m_AccoundID, szAccountID, 10);
				g_MuLoginUserData[i].iUserNumber = g_iUserNumber;
				g_MuLoginUserData[i].m_ServerCode = ServerCode;
				g_MuLoginUserData[i].m_bRequestMapSvrMove = false;
				g_iUserNumber++;
				g_MuLoginUserDataCount++;

				return TRUE;
			}
		}
	}

	return FALSE;
}


int MuLoginFindUser(char * szAccountID)
{
	for(int i=0;i<MAX_LOGIN_USER;i++)
	{
		if ( g_MuLoginUserData[i].m_AccoundID[0] == szAccountID[0] )
		{
			if ( g_MuLoginUserData[i].m_AccoundID[1] == szAccountID[1] )
			{
				if ( lstrcmp(g_MuLoginUserData[i].m_AccoundID, szAccountID) == 0 )
				{
					return i;
				}
			}
		}
	}

	return -1;
}


void MuLoginDeleteUser(char * szAccountID)
{
	for(int i=0;i<MAX_LOGIN_USER;i++)
	{
		if ( g_MuLoginUserData[i].m_AccoundID[0] == szAccountID[0] )
		{
			if ( g_MuLoginUserData[i].m_AccoundID[1] == szAccountID[1] )
			{
				if ( lstrcmp(g_MuLoginUserData[i].m_AccoundID, szAccountID) == 0 )
				{
					memset(g_MuLoginUserData[i].m_AccoundID, 0, 10);
					g_MuLoginUserData[i].m_ServerCode = -1;
					g_MuLoginUserData[i].m_bRequestMapSvrMove = false;
					g_MuLoginUserData[i].iUserNumber = -1;
					g_MuLoginUserDataCount--;
					break;
				}
			}
		}
	}
}



void gObjServerDataSendEXDB(void * pData, int iDataSize)
{
	for(int i=0;i<g_dwMaxServerGroups;i++)
	{
		if ( g_Server[i].m_Index != -1 &&
			 g_Server[i].m_State >= SS_GUILD )
		{
			DataSend(g_Server[i].m_Index, (LPBYTE)pData, iDataSize);
		}
	}
}