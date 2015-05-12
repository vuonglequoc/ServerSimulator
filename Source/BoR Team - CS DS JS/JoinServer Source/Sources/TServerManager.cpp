#include "GInclude.h"
#include "TServerManager.h"
#include "TDbManager.h"
#include "Main.h"

//-------------------------------------------------------------------------------------------------------------------------------------------
// # Made by Leo123[BoR] && huihui
// # huihui : added tagSERVER_DATA Clear , tagUSER_DATA Clear

//-------------------------------------------------------------------------------------------------------------------------------------------

tagSERVER_DATA	m_ServObj[MAX_OBJECT];
tagUSER_DATA	m_UserObj[MAX_USER];
bool			g_UseMD5;	
CServerManager	g_ServerManager;			
//-------------------------------------------------------------------------------------------------------------------------------------------

void tagSERVER_DATA::Clear()
{
	memset(m_szServerName, 0, sizeof(m_szServerName));
	// ----
	m_Index					= SLOT_FREE;
	m_ServerCode			= SLOT_FREE;
	m_Port					= SLOT_FREE;
	m_ConnectedCount		= 0;
	m_MaxUser				= 0;
}
//-------------------------------------------------------------------------------------------------------------------------------------------

void tagUSER_DATA::Clear()
{
	memset(m_szAccountID, 0 , sizeof(m_szAccountID));
	// ----
	m_ServerIndex			= SLOT_FREE;
	m_ServerCode			= SLOT_FREE;
	iUserNumber				= SLOT_FREE;
	nNumber					= 0;
	BillCheckTimer			= 0;
	BillCheckSendTimer		= 0;
	m_bRequestMapSvrMove	= false;
}
//-------------------------------------------------------------------------------------------------------------------------------------------


CServerManager::CServerManager(void)
{
	Init();
}
//-------------------------------------------------------------------------------------------------------------------------------------------

CServerManager::~CServerManager(void)
{
	// ----
}
//-------------------------------------------------------------------------------------------------------------------------------------------

void CServerManager::Init()
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
//-------------------------------------------------------------------------------------------------------------------------------------------

void CServerManager::OnError(const char * szMessage, ...)
{
	g_Log.VLogAddFormat(g_Colors.Red(), szMessage);
}
//-------------------------------------------------------------------------------------------------------------------------------------------

void CServerManager::OnMessage(const char * szMessage, ...)
{
	g_Log.VLogAddFormat(g_Colors.Green(), szMessage);
}
//-------------------------------------------------------------------------------------------------------------------------------------------

void CServerManager::OnRecv(int aIndex, BYTE HeadCode, LPBYTE aRecv, int iSize)
{
	JSProtocolCore(aIndex, HeadCode , aRecv , iSize);
}
//-------------------------------------------------------------------------------------------------------------------------------------------

void CServerManager::OnSearch(int aIndex)
{

}
//-------------------------------------------------------------------------------------------------------------------------------------------

void CServerManager::OnConnect(int aIndex)
{
	g_Log.LogAdd(g_Colors.Blue(), "[CServerManager][OnConnect] :: index(%d)", aIndex);
	// ----
	m_ServerConnectedCount++;
}
//-------------------------------------------------------------------------------------------------------------------------------------------

void CServerManager::OnDisconnect(int aIndex)
{
	g_Log.LogAdd(g_Colors.Blue(), "[CServerManager][OnDisconnect] :: index(%d)", aIndex);
	// ----
	DelServer(aIndex);
	// ----
	m_ServerConnectedCount--;
}
//-------------------------------------------------------------------------------------------------------------------------------------------

bool CServerManager::IsCodeExist(int ServerCode)
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
//-------------------------------------------------------------------------------------------------------------------------------------------

bool CServerManager::AddServer(int ServerIndex,int ServerCode, char * szServerName, int Port)
{
	bool bReturn = false;
	// ----
	g_Log.LogAdd(g_Colors.Blue(), "[CServerManager][AddServer] :: Index(%d) Code(%d) Name(%s)",ServerIndex ,
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
		g_Log.LogAdd(g_Colors.Red(), "[CServerManager][AddServer] :: GS(%s) trying to connect with a ServerCode(%d) already used",
			szServerName, ServerCode);
	}
	// ----
	return bReturn;
}
//-------------------------------------------------------------------------------------------------------------------------------------------

void CServerManager::DelServer(int ServerIndex)
{
	g_Log.LogAdd(g_Colors.Blue(), "[CServerManager][DelServer] :: Index(%d)", ServerIndex);
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
	char szTemp[100] = {0};
	// ----
//	sprintf(szTemp, "%s%d", SHARED_LOADPERCENT, lpServ->m_Port);
	// ----
//	g_SharedManager.WriteMemoryInt(szTemp, -1);
	// ----
	lpServ->Clear();
}
//-------------------------------------------------------------------------------------------------------------------------------------------

int CServerManager::GetPortByCode(int ServerCode)
{
	for(int i = 0 ; i != MAX_OBJECT ; i++)
	{
		tagSERVER_DATA * lpServ = & m_ServObj[i];
		// ----
		if(lpServ->m_ServerCode == ServerCode)
		{
			return lpServ->m_Port;
		}
	}
	// ----
	return 0;
}
//-------------------------------------------------------------------------------------------------------------------------------------------

bool CServerManager::AddUser(int ServerIndex, char * szAccountID, char * szIP, short nNumber)
{
	bool bReturn	= false;
	// ----
	g_Log.LogAdd(g_Colors.Blue(), "[CServerManager][AddUser] :: Index(%d) ID(%s)", ServerIndex,
		szAccountID);
	// ----
	tagSERVER_DATA * lpServ = & m_ServObj[ServerIndex];
	// ----
	if(ServerIndex == lpServ->m_Index)
	{
		for(int i = 0 ; i != MAX_USER ; i++)
		{
			tagUSER_DATA * lpUser = & m_UserObj[i];
			// ----
			if(lpUser->m_szAccountID[0] == 0)
			{
				if(lpUser->m_szAccountID[1] == 0)
				{
					memcpy(lpUser->m_szAccountID, szAccountID, 10);
					// ----
					lpUser->m_ServerIndex			= ServerIndex;
					lpUser->iUserNumber				= m_UserConnectedCount;
					lpUser->nNumber					= nNumber;	// aIndex from GS
					lpUser->m_ServerCode			= m_ServObj[ServerIndex].m_ServerCode;
					lpUser->m_bRequestMapSvrMove	= false;
					lpUser->BillCheckTimer			= GetTickCount();
					lpUser->BillCheckSendTimer		= GetTickCount();
					// ----
					lpServ->m_ConnectedCount++;
					// ----
					m_UserConnectedCount++;
					// ----
					bReturn = true;
					// ----
					g_DbManager.ExecFormat("EXEC WZ_CONNECT_MEMB '%s','%s','%s'", szAccountID, m_ServObj[ServerIndex].m_szServerName, szIP);
					// ----
					g_DbManager.Fetch();
					g_DbManager.Clear();
					// ----
					break;
				}
				else continue;
			}
			else continue;
		}
	}
	else
	{
		g_Log.LogAdd(g_Colors.Red(), "[CServerManager][AddUser] :: Trying add user to non active GS(%d)", ServerIndex);
	}
	// ----
	return bReturn;
}
//-------------------------------------------------------------------------------------------------------------------------------------------

void CServerManager::DelUser(const char * szAccountID)
{
	g_Log.LogAdd(g_Colors.Blue(), "[CServerManager][DelUser] :: ID(%s)", szAccountID);
	// ----
	DelUserByIndex(GetUserIndexByID(szAccountID));
}
//-------------------------------------------------------------------------------------------------------------------------------------------

bool CServerManager::CheckUser(const char * szAccountID)
{
	bool bReturn		= false;
	// ----
	int nIndex			= SLOT_FREE;
	char AccountID[11]	= {0};
	USHORT isOnline		= 0;
	// ----
	g_Log.LogAdd(g_Colors.Blue(), "[CServerManager][CheckUser] :: ID(%s)", szAccountID);
	// ----
	nIndex = GetUserIndexByID(szAccountID);
	// ----
	if(g_DbManager.ExecFormat("SELECT * FROM MEMB_STAT WHERE memb___id='%s'", szAccountID) == true)
	{
		if(g_DbManager.Fetch() == SQL_SUCCESS)
		{
			g_DbManager.GetStr("memb___id", AccountID);
			// ----
			if(strcmp(AccountID, szAccountID) == 0) // point here
			{
				isOnline = g_DbManager.GetInt("ConnectStat");
				// ----
				if(isOnline != 1)
				{
					if(0 <= nIndex && MAX_USER > nIndex)
					{
						if(m_UserObj[nIndex].m_bRequestMapSvrMove == true)
						{
							DelUserByIndex(nIndex);
							// ----
							bReturn = true;
						}
					}
				}
				else
				{
					g_Log.LogAdd(g_Colors.LimeGreen(), "[CServerManager][CheckUser] :: Account [%s] is already connected with state %d", szAccountID, isOnline);
					// ----
					bReturn = true;
				}
			}
		}
	}
	// ----
	g_DbManager.Clear();
	// ----
	return bReturn;
}
//-------------------------------------------------------------------------------------------------------------------------------------------

bool CServerManager::UserExist(const char * szAccountID)
{
	bool bReturn	= false;
	// ----
	g_Log.LogAdd(g_Colors.Blue(), "[CServerManager][UserExist] :: ID(%s)", szAccountID);
	// ----
	if(GetUserIndexByID(szAccountID) != SLOT_FREE)
	{
		bReturn = true;
	}
	// ----
	return bReturn;
}
//-------------------------------------------------------------------------------------------------------------------------------------------

int CServerManager::GetUserIndexByID(const char * szAccountID)
{
	int iReturn		= SLOT_FREE;
	// ----
	g_Log.LogAdd(g_Colors.Blue(), "[CServerManager][GetUserIndexByID] :: ID(%s)", szAccountID);
	// ----
	for(int i = 0 ; i != MAX_USER ; i++)
	{
		tagUSER_DATA * lpUser = & m_UserObj[i];
		// ----
		if(lpUser->m_szAccountID[0] == szAccountID[0])
		{
			if(lpUser->m_szAccountID[1] == szAccountID[1])
			{
				if(strcmp(lpUser->m_szAccountID, szAccountID) == 0)
				{
					iReturn = i;
					break;
				}
				else continue;
			}
			else continue;
		}
		else continue;
	}
	// ----
	return iReturn;
}
//-------------------------------------------------------------------------------------------------------------------------------------------

void CServerManager::DelUserByIndex(int nIndex)
{
	if(0 <= nIndex && MAX_USER > nIndex)
	{
		tagUSER_DATA * lpUser = & m_UserObj[nIndex];
		// ----
		g_Log.LogAdd(g_Colors.Blue(), "[CServerManager][DelUserByIndex] :: ID(%s)", lpUser->m_szAccountID);
		// ----
		m_ServObj[lpUser->m_ServerIndex].m_ConnectedCount--;
		// ----
		g_DbManager.ExecFormat("EXEC WZ_DISCONNECT_MEMB '%s'", lpUser->m_szAccountID);
		g_DbManager.Fetch();
		g_DbManager.Clear();
		// ----
		// do delete send c1 xx 07/09 # wrong userindex || both for vip system. omg no regular d/c function in gs :O
		SDHP_USER_DISCONNECT pMsg	=	{0};
		// ----
		pMsg.h.set(0x09, sizeof(pMsg));
		// ----
		memcpy(pMsg.AccountID, lpUser->m_szAccountID, sizeof(lpUser->m_szAccountID));
		// ----
		pMsg.aIndex	= lpUser->nNumber;
		// ----
		g_ServerManager.DataSendAll((LPBYTE)&pMsg, pMsg.h.size);
		// ----
		lpUser->Clear();
		// ----
		m_UserConnectedCount--;
	}
}
//-------------------------------------------------------------------------------------------------------------------------------------------

void CServerManager::StuckAccountScan()
{
	int ActualTime = GetTickCount();
	// ----
	for(int i = 0; i != MAX_USER; i++)
	{
		tagUSER_DATA * lpUser = & m_UserObj[i];
		// ----
		if(lpUser->m_bRequestMapSvrMove == true)
		{
			if((ActualTime - lpUser->m_nMoveMapTime) <= 15000)
			{
				lpUser->m_bRequestMapSvrMove == false;
				// ----
				DelUserByIndex(lpUser->iUserNumber);
				// ----
				g_Log.LogAdd(g_Colors.HotPink(), "[CServerManager][StuckAccountScan] :: Account [%s] was unstucked!",
					lpUser->m_szAccountID);
			}
		}
	}
}
//-------------------------------------------------------------------------------------------------------------------------------------------