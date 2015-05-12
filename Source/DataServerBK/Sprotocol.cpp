// Sprotocol.cpp: implementation of the Sprotocol class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Sprotocol.h"
#include "giocp.h"
#include "LogProc.h"
#include "winutil.h"
#include "MD5.h"
#include "WzUdp.h"
#include "ServerEngine.h"

CQuery g_MeMuOnlineDB;

#pragma pack(1)




int g_iUserNumber=0;
WzUdp g_ConnecServerUDP;
tagJOIN_SERVER_USER_DATA g_MuLoginUserData[MAX_LOGIN_USER];
tagJOIN_SERVER_SERVER_DATA g_MuLoginServerData[MAX_LOGIN_SERVER];
std::vector<std::string> g_vecMuUseLog;
std::vector<tagUSER_DATA> g_vecMapMove;
int g_MuLoginUserDataCount = 0;
int g_MuLoginServerDataCount = 0;

#pragma pack()

struct PMSG_LOGINSEVER_LIVE
{
	PBMSG_HEAD h;	// C1:02
	DWORD RQueue;
};

unsigned long __stdcall ConnectServerThread(LPVOID)
{
	PMSG_LOGINSEVER_LIVE pMsg;
	PHeadSetB((LPBYTE)&pMsg, 0x02, sizeof(pMsg));

	while ( true )
	{
		pMsg.RQueue = 0;
		g_ConnecServerUDP.SendData((LPBYTE)&pMsg, pMsg.h.size);
		Sleep(1000);
	}

	return TRUE;
}


void InsertDataMuLog(LPTSTR ServerName, LPTSTR Id, LPTSTR Ip, LPTSTR State, LPTSTR IpGuid, int UsedTime, int PayCode)
{
	SYSTEMTIME sysTime;
	GetLocalTime(&sysTime);
	TCHAR szTemp[256];

	wsprintf(szTemp, "INSERT INTO UseLog%04d%02d%02d (ServerName, Id, Ip, State, IpGuid, UsedTime, PayCode) Values('%s','%s','%s','%s',%d, %d, %d)",
		sysTime.wYear, sysTime.wMonth, sysTime.wDay, ServerName, Id, Ip, State, IpGuid, UsedTime, PayCode);

	g_vecMuUseLog.push_back(szTemp);
}


void InitMeMuOnlineDB()
{
	g_vecMuUseLog.clear();
	g_vecMapMove.clear();

	if ( g_MeMuOnlineDB.Connect(g_MeMuOnlineDNS, g_UserID, g_Password) == TRUE )
	{
		LogAddC(4, "[MeMuOnlineDB] Connection Successfull!");
	}
	else
	{
		LogAddC(2, "[MeMuOnlineDB] Error On Connection!");
	}

	for(int i=0;i<MAX_LOGIN_USER;i++)
	{
		memset(g_MuLoginUserData[i].m_AccoundID, 0, sizeof(g_MuLoginUserData[i].m_AccoundID));
		g_MuLoginUserData[i].iUserNumber = -1;
		g_MuLoginUserData[i].m_bRequestMapSvrMove = false;
		g_MuLoginUserData[i].m_ServerCode = -1;
	}

	for(int i=0;i<MAX_LOGIN_SERVER;i++)
	{
		memset(g_MuLoginServerData[i].m_szServerName, 0, sizeof(g_MuLoginServerData[i].m_szServerName));
		g_MuLoginServerData[i].m_wPort = 0;
		g_MuLoginServerData[i].m_wServerCode = -1;
	}

	g_ConnecServerUDP.CreateSocket();
	g_ConnecServerUDP.SendSet("127.0.0.1", 55557);

	DWORD dwThreadID;
	HANDLE hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ConnectServerThread, NULL, 0, &dwThreadID);
}



void SProtocolCore(int aIndex, BYTE HeadCode, LPBYTE aRecv, int iSize)
{
	if(g_UseJoinServer == TRUE)
	{
	g_ServerInfoDisplayer.CheckJSConnection(TRUE);
#if (TRACE_PACKET == 1 )
	LogAddHeadHex("JOIN_SERVER", aRecv, iSize);
#endif

	switch ( HeadCode )
	{
		case 0x00:
			JoinServerLogin(aIndex, (SDHP_SERVERINFO *)aRecv);
			break;
		case 0x01:
			JGPAccountRequest(aIndex, (SDHP_IDPASS *)aRecv);
			break;
		case 0x02:
			GJPAccountFail(aIndex, (SDHP_JOINFAIL *)aRecv);
			break;
		case 0x04:
			GJPAccountBlock(aIndex, (SDHP_COMMAND_BLOCK *)aRecv);
			break;
		case 0x05:
			GJPUserClose(aIndex, (SDHP_USERCLOSE_ID *)aRecv);
			break;
		case 0x06:
			GCJoinBillCheckSend(aIndex, (SDHP_BILLSEARCH *) aRecv);
			break;
		case 0x30:
			LoveHeartEventRecv(aIndex, (SDHP_LOVEHEARTEVENT *) aRecv);
			break;
		case 0x31:
			LoveHeartCreateSend(aIndex, (SDHP_LOVEHEARTCREATE *) aRecv);
			break;
		case 0x7A:
			GJReqMapSvrMove(aIndex, (PMSG_REQ_MAPSVRMOVE *) aRecv);
			break;
		case 0x7B:
			GJReqMapSvrAuth(aIndex, (PMSG_REQ_MAPSVRAUTH *) aRecv);
			break;
		case 0x7C:
			GJNotifyMaxUserCount(aIndex, (PMSG_NOTIFY_MAXUSER *) aRecv);
			break;
		/*case 0xA0:
			DisconnectSpecificUser	// No Reference in GS via packet 0x09
				break;*/
		/*case 0xA1:
			SentNotice to ALL Servers	// No Reference in GS via packet 0x40
				break;*/
		/*case 0xA2:
			RecvNotice Result	// No Reference in GS via packet 0x41
				break;*/
		/*case 0xA3:
			ExitCodeSendUSer // No Reference in GS via packet 0x20
				break;*/
		/*case 0xA4:
			Send Notice to Specific Server // No Reference in GS via packet 0x40
				break;*/
	}

	g_ServerInfoDisplayer.CheckJSConnection(FALSE);
	}
}


void JoinServerLogin(int aIndex, SDHP_SERVERINFO * lpMsg)
{
	SDHP_RESULT pResult = {0};

	PHeadSetB((LPBYTE)&pResult, 0x00, sizeof(pResult));
	pResult.Result = 1;

	if ( MuLoginAddServer(lpMsg->ServerName, lpMsg->ServerCode, lpMsg->Port) == TRUE )
	{
		LogAddTD("[Server Manager] GameServer connected %s PORT : %d CODE : %d",
			lpMsg->ServerName, lpMsg->Port, lpMsg->ServerCode);

		g_Server[aIndex].m_ServerCode = lpMsg->ServerCode;
	}
	else
	{
		pResult.Result = 0;
	}

	DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
}


void JGPAccountRequest(int aIndex, SDHP_IDPASS * aRecv)
{
	SDHP_IDPASSRESULT pResult ={0};

	// Copy Data
	PHeadSetB((LPBYTE)&pResult, 0x01, sizeof(pResult));
	char szAccountID[11]={0};
	char szPass[11]={0};
	char szJoominNumber[14]={0};
	char szIp[16]={0};
	BuxConvert(aRecv->Id, 10);
	BuxConvert(aRecv->Pass, 10);
	memcpy(szAccountID, aRecv->Id , 10);
	memcpy(szPass, aRecv->Pass, 10);
	memcpy(pResult.Id, aRecv->Id, 10);
	memcpy(szIp, aRecv->IpAddress, 15);
	pResult.Number = aRecv->Number;

	// Prevent SQLInjection
	if ( SQLSyntexCheck(szAccountID) == FALSE ||
		 SQLSyntexCheck(szPass) == FALSE )
	{
		pResult.result = 2;
		DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	BOOL bErrorFlag = FALSE;
	int BlocCode = 0;

	if ( SpaceSyntexCheck(szAccountID) == TRUE )
	{
		pResult.result = 1;
		
		
		// Search if Connected
		for(int i=0;i<MAX_LOGIN_USER;i++)
		{
			if ( g_MuLoginUserData[i].m_AccoundID[0] == szAccountID[0] )
			{
				if ( g_MuLoginUserData[i].m_AccoundID[1] == szAccountID[1] )
				{
					if ( lstrcmp(g_MuLoginUserData[i].m_AccoundID, szAccountID ) == 0 )
					{
						LogAddTD("[MeMuOnline] Account already connected ID : %s", szAccountID);
						JGOtherJoin(aIndex, szAccountID);
						pResult.result = 3;
						bErrorFlag = TRUE;
						
						break;
					}
				}
			}
		}

		// Add User to List
		if ( bErrorFlag == FALSE )
		{
			if (g_MeMuOnlineDB.ExecQuery("SELECT * FROM MEMB_INFO WHERE memb___id='%s'", szAccountID) == TRUE )
			{
				SQLRETURN Result = g_MeMuOnlineDB.Fetch();

				if ( /*Result == SQL_SUCCESS ||*/ Result == SQL_NO_DATA )
				{
					pResult.result = 2;
					bErrorFlag = TRUE;
					LogAddTD("[MeMuOnline] Account doesn't exist - ID : %s", szAccountID);
					g_MeMuOnlineDB.Close();
				}
				else
				{
					pResult.UserNumber = g_iUserNumber;
					g_iUserNumber++;
					pResult.DBNumber = g_MeMuOnlineDB.GetAsInteger("memb_guid");
					g_MeMuOnlineDB.GetAsString("sno__numb", szJoominNumber);
					memcpy(pResult.JoominNumber, szJoominNumber, sizeof(pResult.JoominNumber));
					BlocCode = g_MeMuOnlineDB.GetAsInteger("bloc_code");
					g_MeMuOnlineDB.Close();

					if ( g_UseMD5 == TRUE )
					{
						BYTE btBinaryPass[16]={0};
						char szTemp[100];
						wsprintf(szTemp, "SELECT memb__pwd from MEMB_INFO where memb___id='%s'", szAccountID);
						
						if ( g_MeMuOnlineDB.GetAsBinary(szTemp, btBinaryPass) < 0 )
						{
							bErrorFlag = TRUE;
							pResult.result = 0;
							LogAddTD("[MeMuOnlineDB] Password field is blank - ID : %s", szAccountID);
						}

						DWORD dwAccKey = MakeAccountKey(szAccountID);
						
						MD5 pMD5Hash;
						
						if ( pMD5Hash.MD5_CheckValue(szPass, (char*)btBinaryPass, dwAccKey) == false )
						{
							pResult.result = 0;
							bErrorFlag = TRUE;
							LogAddTD("[MeMuOnline] Wrong Password - ID : %s", szAccountID);
						}
					}
					else
					{
						TCHAR szPassword[11];
						g_MeMuOnlineDB.GetAsString("memb__pwd", szPassword);

						if ( lstrcmp(szPass, szPassword) )
						{
							pResult.result = 0;
							bErrorFlag = TRUE;
							LogAddTD("[MeMuOnline] Wrong Password - ID : %s", szAccountID);
						}
					}

					g_MeMuOnlineDB.Close();
				}
			}
		}
		
		if ( BlocCode == 0x46 )	pResult.result = 15;

		if ( bErrorFlag == FALSE )
		{
			if ( BlocCode > 0x30 )
				if ( BlocCode == 0x41 ) pResult.result = 14;
				else if ( BlocCode == 0x49 ) pResult.result = 17;
				else pResult.result = 5;

//#error There is a return -2 
			if ( MuLoginAddUser(g_Server[aIndex].m_ServerCode, szAccountID) == FALSE )	
			{
				pResult.result = 4;
				InsertDataMuLog(NULL, NULL, NULL, NULL, NULL, 0, 0);

				int iServerIndex = MuLoginFindServer(g_Server[aIndex].m_ServerCode);

				if ( iServerIndex != -1 )
					g_MeMuOnlineDB.ExecQuery("EXEC WZ_CONNECT_MEMB '%s','%s','%s'", szAccountID, g_MuLoginServerData[iServerIndex].m_szServerName, szIp);
				else
					g_MeMuOnlineDB.ExecQuery("EXEC WZ_CONNECT_MEMB '%s','%s','%s'", szAccountID, "", szIp);

				g_MeMuOnlineDB.Fetch();
				g_MeMuOnlineDB.Close();
			}

		}
	}

	DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
}


void GJPAccountFail(int aIndex, SDHP_JOINFAIL * aRecv)
{
	char szAccountID[11]={0};
	memcpy(szAccountID, aRecv->Id, 10);
	MuLoginDeleteUser(szAccountID);
	
	InsertDataMuLog(NULL, NULL, NULL, NULL, NULL, 0, 0);
	g_MeMuOnlineDB.ExecQuery("EXEC WZ_DISCONNECT_MEMB '%s'", szAccountID);
	g_MeMuOnlineDB.Fetch();
	g_MeMuOnlineDB.Close();	
}



void GJPAccountBlock(int aIndex, SDHP_COMMAND_BLOCK * aRecv)
{
	char szAccountID[11]={0};
	memcpy(szAccountID, aRecv->Id, 10);
	MuLoginDeleteUser(szAccountID);
	g_MeMuOnlineDB.ExecQuery("UPDATE MEMB_INFO SET bloc_code = 1 WHERE memb___id = '%s'", szAccountID);
	//"Insert INTO JoinLog (ServerName, Id, State, Ip, SDateTime, EDateTime) Values('%s','%s','%s','%s','%s','%s')"

	g_MeMuOnlineDB.Fetch();
	g_MeMuOnlineDB.Close();
}




void GJPUserClose(int aIndex, SDHP_USERCLOSE_ID * aRecv )
{
	char szAccountID[11]={0};
	memcpy(szAccountID, aRecv->szId, 10);
	MuLoginDeleteUser(szAccountID);
	InsertDataMuLog(NULL, NULL, NULL, NULL, NULL, 0, 0);
	g_MeMuOnlineDB.ExecQuery("EXEC WZ_DISCONNECT_MEMB '%s'", szAccountID);
	g_MeMuOnlineDB.Fetch();
	g_MeMuOnlineDB.Close();	
}


void GCJoinBillCheckSend(int aIndex, SDHP_BILLSEARCH * aRecv)
{
	SDHP_BILLSEARCH_RESULT pResult;

//#error Still Need the Remian Time Check
	PHeadSetB((LPBYTE)&pResult, 0x06, sizeof(pResult));
	char szAccountID[11]={0};
	memcpy(szAccountID, aRecv->Id, 10);
}


void JGOtherJoin(int aIndex, LPTSTR szAccountID)
{
	SDHP_OTHERJOINMSG pMsg;

	PHeadSetB((LPBYTE)&pMsg.h, 0x08, sizeof(pMsg));
	memcpy(pMsg.AccountID, szAccountID, 10);

	DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
}



void LoveHeartEventRecv(int aIndex, SDHP_LOVEHEARTEVENT * aRecv)
{
	char szAccountID[11]={0};
	char szName[11]={0};
	memcpy(szAccountID, aRecv->Account, 10);
	memcpy(szName, aRecv->Name, 10);
	DWORD dwHeartCount=0;

	SDHP_LOVEHEARTEVENT_RESULT pResult;

	PHeadSetB((LPBYTE)&pResult, 0x30, sizeof(pResult));
	pResult.Result = 0;
	memcpy(pResult.Name, aRecv->Name, 10);

	if ( g_MeMuOnlineDB.ExecQuery("SELECT heart_count FROM LoveHeartCount WHERE Number=0") == TRUE )
	{
		if ( g_MeMuOnlineDB.Fetch() == SQL_NO_DATA )
		{
			g_MeMuOnlineDB.Close();
			g_MeMuOnlineDB.ExecQuery("UPDATE LoveHeartCount SET heart_count=0 WHERE Number=0");
			g_MeMuOnlineDB.Fetch();
			g_MeMuOnlineDB.Close();
		}
		else
		{
			dwHeartCount = g_MeMuOnlineDB.GetAsInteger("heart_count");
		}
	}
	g_MeMuOnlineDB.Close();

	dwHeartCount++;

	if ( dwHeartCount > 217000 )
	{
		g_MeMuOnlineDB.ExecQuery("UPDATE LoveHeartCount SET heart_count=0 WHERE Number=0");
		g_MeMuOnlineDB.Close();
	}

	g_MeMuOnlineDB.ExecQuery("UPDATE LoveHeartCount SET heart_count= %d where Number=0", dwHeartCount);
	g_MeMuOnlineDB.Fetch();
	g_MeMuOnlineDB.Close();

	g_MeMuOnlineDB.ExecQuery("INSERT INTO LoveHeartAll ( Number, Id, Server, Name ) Values (%d, '%s','%s','%s')",
		dwHeartCount, szAccountID, g_MuLoginServerData[MuLoginFindServer(g_Server[aIndex].m_ServerCode)].m_szServerName, szName);
	g_MeMuOnlineDB.Fetch();
	g_MeMuOnlineDB.Close();

	int iIndex = MuLoginFindUser(szAccountID);

	if ( iIndex >= 0 )	// if Is Connected
	{
		if ( (rand()%217000) == iIndex )	// if Wins
		{
			int iServerIndex = MuLoginFindServer(g_Server[aIndex].m_ServerCode);

			if ( iServerIndex != -1 )
				g_MeMuOnlineDB.ExecQuery("INSERT INTO LoveHeart ( Number, Id, Server, Name ) Values (%d, '%s','%s','%s')", dwHeartCount, szAccountID, g_MuLoginServerData[iServerIndex].m_szServerName, szName);
			else
				g_MeMuOnlineDB.ExecQuery("INSERT INTO LoveHeart ( Number, Id, Server, Name ) Values (%d, '%s','%s','%s')", dwHeartCount, szAccountID, "", szName);

			g_MeMuOnlineDB.Fetch();
			g_MeMuOnlineDB.Close();
			pResult.Result = 1;

			if ( iServerIndex != iIndex )
				LogAddC(2, "[MeMuOnlineDB] [LOVE HEART] Event Winner : %s:%s - Server : %s",szAccountID, szName, g_MuLoginServerData[iServerIndex].m_szServerName);
			else
				LogAddC(2, "[MeMuOnlineDB] [LOVE HEART] Event Winner : %s:%s - Server : %s",szAccountID, szName, "");
		}
	}	
	pResult.Number = dwHeartCount;
	DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
}



void LoveHeartCreateSend(int aIndex, SDHP_LOVEHEARTCREATE * aRecv)
{
	g_MeMuOnlineDB.ExecQuery("UPDATE LoveHeartCount SET heart_count=heart_count+1  where Number=1");
	g_MeMuOnlineDB.Fetch();
	g_MeMuOnlineDB.Close();
}



void GJReqMapSvrMove(int aIndex, PMSG_REQ_MAPSVRMOVE * aRecv)
{
	char szAccoundID[11]={0};
	DWORD fResult = 0;
	memcpy(szAccoundID, aRecv->szAccountID, 10);

	if ( lstrlen(szAccoundID) < 1 )
		fResult = 1;

	int iOldServerIndex = MuLoginFindServer(aRecv->wCurMapSvrCode);
	int iNewServerIndex = MuLoginFindServer(aRecv->wDstMapSvrCode);

	// No Dest Server Connected
	if ( iOldServerIndex == -1 || iNewServerIndex == -1 )
		fResult = 2;
	// Limit User reached
	else if ( g_MuLoginServerData[iNewServerIndex].m_wCurrentUser >= g_MuLoginServerData[iNewServerIndex].m_wMaxUser )
		fResult = 3;

	PMSG_ANS_MAPSVRMOVE pResult;
	tagUSER_DATA pUserData;

	PHeadSetB((LPBYTE)&pResult, 0x7A, sizeof(pResult));
	int iUserIndex=-1;

	// Some Other Checks
	if ( fResult == 0 )
	{
		iUserIndex = MuLoginFindUser(szAccoundID);

		// User not Connected
		if ( iUserIndex == -1 )
			fResult = 1;
		// User Requested the Move
		else if ( g_MuLoginUserData[iUserIndex].m_bRequestMapSvrMove != false )
			fResult = 4;
		else
			g_MuLoginUserData[iUserIndex].m_bRequestMapSvrMove = true;
		
	}

	pResult.iIndex = aRecv->iIndex;
	pResult.btX = aRecv->btX;
	pResult.btY = aRecv->btY;
	memcpy(pResult.szAccountID, aRecv->szAccountID, 11);
	memcpy(pResult.szCharName, aRecv->szCharName, 11);
	pResult.wCurMapSvrCode = aRecv->wCurMapSvrCode;
	pResult.wDstMapSvrCode = aRecv->wDstMapSvrCode;
	pResult.iResult = fResult;
	pResult.iJoinAuthCode1 = GetTickCount();
	pResult.iJoinAuthCode2 = GetTickCount() + rand() % 10000;
	pResult.iJoinAuthCode3 = GetTickCount() + rand() % 777;
	pResult.iJoinAuthCode4 = GetTickCount() + rand() % 8911;
	pResult.wMapNumber = aRecv->wMapNumber;

	if ( iUserIndex != -1 )
		pUserData.iUserNumber = g_MuLoginUserData[iUserIndex].iUserNumber;

	memcpy(&pUserData.pMapServerMoveData, &pResult, sizeof(pResult));
	g_vecMapMove.push_back(pUserData);
	
	DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
}




void GJReqMapSvrAuth(int aIndex, PMSG_REQ_MAPSVRAUTH * aRecv)
{
	char szAccoundID[11]={0};
	DWORD fResult = 0;
	memcpy(szAccoundID, aRecv->szAccountID, 10);

	if ( lstrlen(szAccoundID) < 1 )
		fResult = 1;

	PMSG_ANS_MAPSVRAUTH pResult;

	PHeadSetB((LPBYTE)&pResult, 0x7B, sizeof(pResult));
	memcpy(pResult.szAccountID, aRecv->szAccountID, 11);
	memcpy(pResult.szCharName, aRecv->szCharName, 11);
	pResult.iIndex = aRecv->iIndex;
	
	if ( fResult == 0 )
	{
		int iUserIndex = MuLoginFindUser(szAccoundID);

		// User Not Connected
		if ( iUserIndex == -1 )
			fResult = 1;
		else if ( g_MuLoginUserData[iUserIndex].m_bRequestMapSvrMove == true )
		{
			tagUSER_DATA pDataUser;
			BOOL bFlagFound = FALSE;

			for(std::vector<tagUSER_DATA>::iterator _Iter = g_vecMapMove.begin(); _Iter != g_vecMapMove.end() ; _Iter++)
			{
				pDataUser = (*(_Iter));
				
				if ( pDataUser.pMapServerMoveData.szAccountID[0] == aRecv->szAccountID[0] )
				{
					if ( pDataUser.pMapServerMoveData.szAccountID[1] == aRecv->szAccountID[1] )
					{
						if ( lstrcmp(pDataUser.pMapServerMoveData.szAccountID, aRecv->szAccountID) == 0 )
						{
							if ( lstrcmp(pDataUser.pMapServerMoveData.szCharName, aRecv->szCharName) == 0 )
							{
								bFlagFound = TRUE;
								g_vecMapMove.erase(_Iter);
								break;
							}
						}
					}
				}
			}

			if ( bFlagFound == TRUE )
			{
				if ( CHECK_LIMIT(pDataUser.pMapServerMoveData.wCurMapSvrCode, MAX_LOGIN_SERVER) &&
					 CHECK_LIMIT(pDataUser.pMapServerMoveData.wDstMapSvrCode, MAX_LOGIN_SERVER)   )
				{
					if ( aRecv->iJoinAuthCode1 == pDataUser.pMapServerMoveData.iJoinAuthCode1 &&
						 aRecv->iJoinAuthCode2 == pDataUser.pMapServerMoveData.iJoinAuthCode2 &&
						 aRecv->iJoinAuthCode3 == pDataUser.pMapServerMoveData.iJoinAuthCode3 &&
						 aRecv->iJoinAuthCode4 == pDataUser.pMapServerMoveData.iJoinAuthCode4 )
					{
						g_MuLoginUserData[iUserIndex].m_bRequestMapSvrMove = false;
						pResult.btX = pDataUser.pMapServerMoveData.btX;
						pResult.btY = pDataUser.pMapServerMoveData.btY;
						pResult.wPrevMapSvrCode = pDataUser.pMapServerMoveData.wCurMapSvrCode;
						pResult.wMapNumber = pDataUser.pMapServerMoveData.wMapNumber;

						g_MeMuOnlineDB.ExecQuery("SELECT * FROM MEMB_INFO WHERE memb___id='%s'", szAccoundID);

						if ( g_MeMuOnlineDB.Fetch() != SQL_NO_DATA )
						{
							pResult.iUserNumber = pDataUser.iUserNumber;
							g_MeMuOnlineDB.GetAsString("sno__numb", pResult.cJoominNumber);
							pResult.iDBNumber = g_MeMuOnlineDB.GetAsInteger("memb_guid");
							pResult.btBlockCode = (BYTE)g_MeMuOnlineDB.GetAsInteger("bloc_code");
						}
						g_MeMuOnlineDB.Close();
					}
					else
					{
						fResult = 2;
					}
				}
				else
				{
					fResult = 3;
				}
			}
			else
			{
				fResult = 4;
			}
		}
		else
		{
			fResult = 4;
		}
	}

	pResult.iResult = fResult;

	DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
}


void GJNotifyMaxUserCount(int aIndex, PMSG_NOTIFY_MAXUSER * aRecv)
{
	int iIndex = MuLoginFindServer(aRecv->iSvrCode);

	if ( iIndex != -1 )
	{
		g_MuLoginServerData[iIndex].m_wCurrentUser = aRecv->iCurUserCount;
		g_MuLoginServerData[iIndex].m_wMaxUser = aRecv->iMaxUserCount;
	}
}

