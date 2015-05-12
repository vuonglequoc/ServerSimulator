/*#include "GInclude.h"
#include "TExManager.h"
#include "Main.h"
#include "ExProtocol.h"
#include "TDbManager.h"

//-----------------------------------------------------------------------------------------------------------
// # Made by Leo123[BoR]

//-----------------------------------------------------------------------------------------------------------

TExManager		g_TExManager[];
int				g_ConnectedCount;
tagSERVER_DATA	m_ServObj[MAX_OBJECT];
tagUSER_DATA	m_UserObj[MAX_USER];
//-----------------------------------------------------------------------------------------------------------

TExManager::TExManager(void)
{
	// ----
}
//-----------------------------------------------------------------------------------------------------------

TExManager::~TExManager(void)
{
	// ----
}
//-----------------------------------------------------------------------------------------------------------

void TExManager::Init()
{
	m_ServerConnectedCount		= 0;
	m_ServerLoggedCount			= 0;
	// ----
	for(int i = 0 ; i != MAX_OBJECT ; i++)
	{
		m_ServObj[i].Clear();
	}
	// ----
	for(int i = 0 ; i != MAX_USER ; i++)
	{
		m_UserObj[i].Clear();
	}
}
//-----------------------------------------------------------------------------------------------------------

void TExManager::OnError(const char * szMessage, ...)
{
	g_Log.VLogAddFormat(g_Colors.Red(), szMessage);
}
//-----------------------------------------------------------------------------------------------------------

void TExManager::OnMessage(const char * szMessage, ...)
{
	g_Log.VLogAddFormat(g_Colors.Green(), szMessage);
}
//-----------------------------------------------------------------------------------------------------------

void TExManager::OnRecv(int aIndex, BYTE HeadCode, LPBYTE aRecv, int iSize)
{
	ExProtocolCore(this, aIndex, HeadCode , aRecv , iSize);
}
//-----------------------------------------------------------------------------------------------------------

void TExManager::OnSearch(int aIndex)
{
}
//-----------------------------------------------------------------------------------------------------------

void TExManager::OnConnect(int aIndex)
{
	g_Log.LogAdd(g_Colors.Blue(), "[TExManager] :: (OnConnect) :: index(%d)", aIndex);
	// ----
	g_ConnectedCount++;
}
//-----------------------------------------------------------------------------------------------------------

void TExManager::OnDisconnect(int aIndex)
{
	g_Log.LogAdd(g_Colors.Blue(), "[TExManager] :: (OnDisconnect) :: index(%d)", aIndex);
	// ----
	g_ConnectedCount--;
}
//-----------------------------------------------------------------------------------------------------------

bool TExManager::IsCodeExist(int ServerCode)
{
	bool bReturn = false;
	// ----
	for(int i = 0 ; i != MAX_OBJECT ; i++)
	{
		tagSERVER_DATA * lpServ = & m_ServObj[i];
		// ----
		if(lpServ->m_ServerCode == ServerCode)
		{
			bReturn = true;
			// ----
			break;
		}
	}
	// ----
	return bReturn;
}
//-----------------------------------------------------------------------------------------------------------

/*bool TExManager::AddServer(int ServerIndex,int ServerCode, char * szServerName, int Port)
{
	bool bReturn = false;
	// ----
	g_Log.LogAdd(g_Colors.Blue(), "[CServerManager] :: (AddServer) :: Index(%d) Code(%d) Name(%s)",ServerIndex ,
		ServerCode, szServerName);
	// ----
	if(IsCodeExist(ServerCode) == false)
	{
		tagSERVER_DATA * lpServ = & m_ServObj[ServerIndex];
		// ----
		memcpy(lpServ->m_szServerName, szServerName, sizeof(lpServ->m_szServerName));
		// ----
		lpServ->m_Port			= Port;
		lpServ->m_ServerCode	= ServerCode;
		lpServ->m_Index			= ServerIndex;
		// ----
		m_ServerLoggedCount++;
		// ----
		bReturn = true;
	}
	else
	{
		g_Log.LogAdd(g_Colors.Red(), "[CServerManager] :: GS(%s) trying to connect with a ServerCode(%d) already used",
			szServerName, ServerCode);
	}
	// ----
	return bReturn;
}
//-----------------------------------------------------------------------------------------------------------

void TExManager::DelServer(int ServerIndex)
{
	g_Log.LogAdd(g_Colors.Blue(), "[CServerManager] :: (DelServer) :: Index(%d)", ServerIndex);
	// ----
	tagSERVER_DATA * lpServ = & m_ServObj[ServerIndex];
	// ----
	for(int i = 0 ; i != MAX_USER ; i++)
	{
		if(m_UserObj[i].m_ServerIndex == ServerIndex)
		{
			DelUserByIndex(i);
		}
	}
	// ----
	lpServ->Clear();
}
//-----------------------------------------------------------------------------------------------------------

void TExManager::DelUserByIndex(int nIndex)
{
	if(0 <= nIndex && MAX_USER > nIndex)
	{
		tagUSER_DATA * lpUser = & m_UserObj[nIndex];
		// ----
		g_Log.LogAdd(g_Colors.Blue(), "[CServerManager] :: (DelUserByIndex) :: ID(%s)", lpUser->m_szAccountID);
		// ----
		m_ServObj[lpUser->m_ServerIndex].m_ConnectedCount--;
		// ----
		g_DbManager.ExecFormat("EXEC WZ_DISCONNECT_MEMB '%s'", lpUser->m_szAccountID);
		g_DbManager.Fetch();
		g_DbManager.Clear();
		// ----
		lpUser->Clear();
		// ----
		m_UserConnectedCount--;
	}
}*/
//-----------------------------------------------------------------------------------------------------------*/