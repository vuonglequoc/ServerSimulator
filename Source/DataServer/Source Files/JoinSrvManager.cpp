#include "../Header Files/StdAfx.H"
#include "../Header Files/Database.H"
#include "../Header Files/LogProc.H"

	
JServerManager JSrvManager;
// ------------------------------------------------------------------------------------------------------------------------------------------------------
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
// ------------------------------------------------------------------------------------------------------------------------------------------------------
void tagUSER_DATA::Clear()
{
	memset(m_szAccountID, 0 , sizeof(m_szAccountID));
	// ----
	m_ServerIndex			= SLOT_FREE;
	m_ServerCode			= SLOT_FREE;
	iUserNumber				= SLOT_FREE;
	nNumber					= 0;
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------
JServerManager::JServerManager(void)
{
	TSync.CSLock();
	// -----
	this->Initialize();
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------
JServerManager::~JServerManager(void)
{
	TSync.CSUnlock();
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------
void JServerManager::Initialize()
{
	m_SrvConnectedCount		= 0;
	// ----
	for ( int i = 0; i != MAX_OBJECT; i++ )
	{
		m_ServObj[i].Clear();
	}
	// ----
	for ( int i = 0; i != MAX_USER; i++ )
	{
		m_UserObj[i].Clear();
	}
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------
void JServerManager::OnRecv(int aIndex, BYTE HeadCode, LPBYTE aRecv, int iSize)
{
	JSProto.JSProtocolCore(aIndex, HeadCode , aRecv , iSize);
}
// --------------------------------------------------------------------------------------------------------------------------------------------------------
void JServerManager::OnConnect()
{
	m_SrvConnectedCount++;
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------
void JServerManager::OnDisconnect(int aIndex)
{
	DelServer(aIndex);
	// -----
	m_SrvConnectedCount--;
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------
bool JServerManager::IsCodeNotExist(int ServerCode)
{
	bool bReturn = true;
	// ----
	for(int i = 0 ; i != MAX_OBJECT ; i++)
	{
		tagSERVER_DATA * lpServ = & m_ServObj[i];
		// ----
		if(lpServ->m_ServerCode == ServerCode)
		{
			bReturn = false;
		}
	}
	// ----
	return bReturn;
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------
bool JServerManager::AddServer(int ServerIndex, int ServerCode, char * szServerName, int Port)
{
	bool bReturn = true;
	// -----
	if ( IsCodeNotExist(ServerCode) == true )
	{
		tagSERVER_DATA * lpServ = & m_ServObj[ServerIndex];
		// -----
		memcpy(lpServ->m_szServerName, szServerName, sizeof(lpServ->m_szServerName));
		// -----
		lpServ->m_Port			= Port;
		lpServ->m_ServerCode	= ServerCode;
		lpServ->m_Index			= ServerIndex;
		// -----
		CLog.Connect("[AddServer] :: Index(%d) Code(%d) Name(%s).", ServerIndex, ServerCode, szServerName);
	}
	else
	{
		CLog.Error("[AddServer] :: GS(%s) trying to connect with Already Used Server Code [%d].", szServerName, ServerCode);
		// -----
		bReturn = false;
	}
	// -----
	return bReturn;
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------
void JServerManager::DelServer(int ServerIndex)
{
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
	CLog.Disconnect("[DelServer] :: Disconnected ServerCode [%d].", ServerIndex);
	// ----
	lpServ->Clear();
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------
bool JServerManager::AddUser(int ServerIndex, char * szAccountID, char * szIP, short nNumber)
{
	bool bReturn	= false;
	// ----
	CLog.LogIn("[AddUser] :: ServerCode [%d] UserID (%s) is Connecting...", ServerIndex, szAccountID);
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
					lpUser->nNumber					= nNumber;
					lpUser->m_ServerCode			= m_ServObj[ServerIndex].m_ServerCode;
					// ----
					lpServ->m_ConnectedCount++;
					// ----
					m_UserConnectedCount++;
					// ----
					bReturn = true;
					// ----
					gDataBase.ExecFormat("EXEC WZ_CONNECT_MEMB '%s','%s','%s'", szAccountID, m_ServObj[ServerIndex].m_szServerName, szIP);
					// ----
					CLog.LogIn("[AddUser] :: UserID (%s) is Connected.", szAccountID);
					// ----
					gDataBase.Fetch();
					gDataBase.Clear();
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
		CLog.Error("[AddUser] :: Attemping to add User to Non-Active GS Code [%d].", ServerIndex);
	}
	// ----
	return bReturn;
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------
void JServerManager::DelUser(const char * szAccountID)
{
	CLog.LogOut("[DelUser] :: ID (%s) is Disconnecting...", szAccountID);
	// -----
	DelUserByIndex(GetUserIndexByID(szAccountID));
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------
bool JServerManager::CheckUser(const char * szAccountID)
{
	bool bReturn		= false;
	char AccountID[11]	= {0};
	USHORT isOnline		= 0;
	// -----
	int nIndex			= GetUserIndexByID(szAccountID);
	// -----
	if(gDataBase.ExecFormat("SELECT * FROM MEMB_STAT WHERE memb___id='%s'", szAccountID) == true)
	{
		if(gDataBase.Fetch() == SQL_SUCCESS)
		{
			gDataBase.GetStr("memb___id", AccountID);
			// -----
			if(strcmp(AccountID, szAccountID) == 0)
			{
				isOnline = gDataBase.GetInt("ConnectStat");
				// -----
				if(isOnline != 1)
				{
					if(0 <= nIndex && MAX_USER > nIndex)
					{
						DelUserByIndex(nIndex);
						// -----
						bReturn = true;
					}
				}
				else
				{
					CLog.Error("[CheckUser] :: Account (%s) is already Connected! ConnectStat: [%d].", szAccountID, isOnline);
					// -----
					bReturn = true;
				}
			}
		}
	}
	// -----
	gDataBase.Clear();
	// -----
	return bReturn;
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------
int JServerManager::GetUserIndexByID(const char * szAccountID)
{
	int iReturn		= SLOT_FREE;
	// -----
	for(int i = 0 ; i != MAX_USER ; i++)
	{
		tagUSER_DATA * lpUser = & m_UserObj[i];
		// -----
		if ( lpUser->m_szAccountID[0] == szAccountID[0] )
		{
			if ( lpUser->m_szAccountID[1] == szAccountID[1] )
			{
				if ( strcmp(lpUser->m_szAccountID, szAccountID) == NULL )
				{
					iReturn = i;
					// -----
					break;
				}
				else continue;
			}
			else continue;
		}
		else continue;
	}
	// -----
	return iReturn;
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------
void JServerManager::DelUserByIndex(int nIndex)
{
	if(0 <= nIndex && MAX_USER > nIndex)
	{
		tagUSER_DATA * lpUser = & m_UserObj[nIndex];
		// -----
		m_ServObj[lpUser->m_ServerIndex].m_ConnectedCount--;
		// -----
		gDataBase.ExecFormat("EXEC WZ_DISCONNECT_MEMB '%s'", lpUser->m_szAccountID);
		gDataBase.Fetch();
		gDataBase.Clear();
		// -----
		CLog.LogOut("[DelUser] :: ID (%s) is Disconnected.", lpUser->m_szAccountID);
		// -----
		gDataBase.ExecFormat("UPDATE MEMB_STAT SET ConnectStat='0' WHERE memb___id='%s'", lpUser->m_szAccountID);
		gDataBase.Fetch();
		gDataBase.Clear();
		// -----
		JS_USER_DISCONNECT pMsg	=	{0};
		// -----
		pMsg.h.Set(0x09, sizeof(pMsg));
		// -----
		memcpy(pMsg.AccountID, lpUser->m_szAccountID, sizeof(lpUser->m_szAccountID));
		// -----
		pMsg.aIndex	= lpUser->nNumber;
		// -----
		DataSendAll((LPBYTE)&pMsg, pMsg.h.Size);
		// -----
		lpUser->Clear();
		// -----
		m_UserConnectedCount--;
	}
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------