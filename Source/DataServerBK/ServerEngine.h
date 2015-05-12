// ServerEngine.h: interface for the CServerEngine class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SERVERENGINE_H__7F7AC7E2_C979_41EA_95BF_375D4A6729C5__INCLUDED_)
#define AFX_SERVERENGINE_H__7F7AC7E2_C979_41EA_95BF_375D4A6729C5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "giocp.h"

enum eSERVER_STATE
{
	SS_CLOSED,
	SS_CONNECTED,
	SS_LOGGED,
	SS_GUILD
};




#define MAX_SERVER_TYPE (ST_CASHSHOPSERVER+1)

struct tagSERVER_ENGINE
{
	int m_Index;
	SOCKET m_Socket;
	eSERVER_STATE m_State;
	eSERVER_TYPE m_Type;
	char m_ServerIp[16];
	struct _PER_SOCKET_CONTEXT * PerSocketContext;
	void (*ProtocolCore1)(int, BYTE, LPBYTE, int);	// For Classic Protocol
	void (*ProtocolCore2)(int, DWORD, LPBYTE, int);	// For Second Generation Protocol(Like CashShop)
	WORD m_ServerCode;
};


void gObjServerInit();
int gObjServerAddSearch();
int gObjServerAdd(SOCKET Socket, char * Ip, int ServerIndex, eSERVER_TYPE eServerType);
void gObjServerDel(int aIndex);
int MuLoginFindServer(WORD ServerCode);
BOOL MuLoginAddServer(LPTSTR ServerName, WORD ServerCode, WORD Port);
void MuLoginDeleteServer(WORD ServerCode);
DWORD MakeAccountKey(LPTSTR lpszAccountID);
BOOL MuLoginAddUser(WORD ServerCode, char * szAccountID);
int MuLoginFindUser(char * szAccountID);
void MuLoginDeleteUser(char * szAccountID);
void gObjServerDataSendEXDB(void * pData, int iDataSize);

extern tagSERVER_ENGINE * g_Server;

#endif // !defined(AFX_SERVERENGINE_H__7F7AC7E2_C979_41EA_95BF_375D4A6729C5__INCLUDED_)
