#include "GInclude.h"
#include "GProtocol.h"
#include "Main.h"
#include "TDbManager.h"
#include "TServerManager.h"
//-------------------------------------------------------------------------------------------------------------------------------------------
// # Developed by Leo123[BoR]
// # Credits : Deathway , eRRoR , Hybrid
//-------------------------------------------------------------------------------------------------------------------------------------------

void JSProtocolCore(int aIndex, BYTE HeadCode, LPBYTE aRecv, int iSize)
{
	g_Log.SetStatusText(g_Colors.Black(), g_Colors.GreenYellow(), "%s ON_GO", MAIN_PROJECT_NAME);
	// ----
	switch (HeadCode)
	{
		case JMSG_SERVER_LOGIN:
		{
			JoinServerLogin(aIndex, (SDHP_SERVERINFO *)aRecv);
		}
		break;
		case JMSG_IDPASS:
		{
			JGPAccountRequest(aIndex, (SDHP_IDPASS *)aRecv);
		}
		break;
		case JMSG_JOIN_FAIL:
		{
			GJPAccountFail(aIndex, (SDHP_JOINFAIL *)aRecv);
		}
		break;
		case JMSG_COMMAND_BLOCK:
		{
			GJPAccountBlock(aIndex, (SDHP_COMMAND_BLOCK *)aRecv);
		}	
		break;
		case JMSG_USER_CLOSE_BY_ID:
		{
			GJPUserClose(aIndex, (SDHP_USERCLOSE_ID *)aRecv);
		}
		break;
		case JMSG_MAP_SVR_MOVE:
		{
			GJReqMapSvrMove(aIndex, (PMSG_REQ_MAPSVRMOVE *)aRecv);	
		}
		break;
		case JMSG_MAP_SVR_AUTH:
		{
			GJReqMapSvrAuth(aIndex, (PMSG_REQ_MAPSVRAUTH *)aRecv);
		}
		break;
		case 0x7C:
		{
			//GJNotifyMaxUserCount(aIndex, (PMSG_NOTIFY_MAXUSER *)aRecv);
		}
		break;
		case 0xA1:
		{
			GJAllNoticeSend(aIndex,(PMSG_SETENTIRENOTICE *)aRecv);
		}
		break;
		case 0xA2:
		{
			GJUserNoticeSend(aIndex,(PMSG_USERNOTICE *)aRecv);
		}
		break;
	}
	// ----
	g_Log.SetStatusText(g_Colors.White(), g_Colors.SkyBlue() , "%s STATE_WAIT", MAIN_PROJECT_NAME);
}
//-------------------------------------------------------------------------------------------------------------------------------------------

void JoinServerLogin(int aIndex, SDHP_SERVERINFO * lpMsg)
{
	SDHP_RESULT pMsg	=	{0};
	// ----
	pMsg.h.set(JMSG_SERVER_LOGIN, sizeof(pMsg));
	pMsg.Result = 0;
	// ----
	if(g_ServerManager.AddServer(aIndex, lpMsg->ServerCode, lpMsg->ServerName, lpMsg->Port) == true)
	{
		g_Log.LogAdd(g_Colors.BlueViolet(), "[GProtocol][JoinServerLogin] :: GS(%s) Index(%d) Port(%d) connected", lpMsg->ServerName,
			lpMsg->ServerCode, lpMsg->Port);
		// ----
		pMsg.Result = 1;
	}
	g_ServerManager.DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
}
//-------------------------------------------------------------------------------------------------------------------------------------------

void JGPAccountRequest(int aIndex, SDHP_IDPASS * lpMsg)
{
	char	szAccountID[11]		= {0};
	char	szIp[16]			= {0};
	char	szPass[11]			= {0};
	char	szTemp[100]			= {0};
	char	JoominNumber[14]	= {0};
	DWORD	dwAccKey			= {0};
	// ----
	int		Number				= lpMsg->Number;
	int		DBNumber			= 0;
	int		UserNumber			= 0;
	int		BlocCode			= 0;
	UCHAR	Result				= (UCHAR)eIDPASSRESULT_SUCCESS;
	// ----
	MD5 pMD5Hash;
	// ----
	BuxConvert(lpMsg->Id, 10);
	BuxConvert(lpMsg->Pass, 10);
	// ----
	memcpy(szAccountID, lpMsg->Id , 10);
	memcpy(szPass, lpMsg->Pass , 10);
	memcpy(szIp, lpMsg->IpAddress, 15);
	// ----
	if((SQLSyntexCheck(szAccountID) == true)	&&
	   (SQLSyntexCheck(szPass) == true)			&&
	   (SpaceSyntexCheck(szAccountID) == true))
	{																										// #
		if(g_ServerManager.CheckUser(szAccountID) == false)
		{																									// ##
			if(g_DbManager.ExecFormat("SELECT * FROM MEMB_INFO WHERE memb___id='%s'", szAccountID) == true)
			{
				if(g_DbManager.Fetch() == SQL_SUCCESS)
				{																							// ####
					DBNumber = g_DbManager.GetInt("memb_guid");
					BlocCode = g_DbManager.GetInt("bloc_code");
					// ----
					g_DbManager.GetStr("sno__numb", JoominNumber);
					// ----
					if(g_UseMD5 == true)
					{
						BYTE btBinaryPass[16] = {0};
						// ----
						// # MD5 Login
						// ----
						wsprintf(szTemp, "SELECT memb__pwd from MEMB_INFO where memb___id='%s'", szAccountID);
						// ----
						if(g_DbManager.GetAsBinary(szTemp, btBinaryPass) > 0)
						{
							dwAccKey = MakeAccountKey(szAccountID);
							// ----
							if(pMD5Hash.MD5_CheckValue(szPass, (char*)btBinaryPass, dwAccKey) == true)
							{
								Result = (UCHAR)eIDPASSRESULT_SUCCESS;
							}
							else
							{
								Result = eIDPASSRESULT_BADPASS;
								// ----
								g_Log.LogAdd(g_Colors.BlueViolet(), "[GProtocol][JGPAccountRequest][MD5] :: Wrong Password - ID : %s",
									szAccountID);
							}
						}
						else
						{
							Result = (UCHAR)eIDPASSRESULT_BADPASS;
							// ----
							g_Log.LogAdd(g_Colors.BlueViolet(), "[GProtocol][JGPAccountRequest][MD5] :: Password is blank - ID : %s",
								szAccountID);
						}
					}
					else
					{
						// ----
						// # NORMAL Login
						// ----
						char szPassword[11] = {0};
						// ----
						g_DbManager.GetStr("memb__pwd", szPassword);
						// ----
						if(strcmp(szPass, szPassword) == 0)
						{
							Result = (UCHAR)eIDPASSRESULT_SUCCESS;
						}
						else
						{
							Result = eIDPASSRESULT_BADPASS;
							// ----
							g_Log.LogAdd(g_Colors.BlueViolet(), "[GProtocol][JGPAccountRequest][NON-MD5] :: Wrong Password - ID : %s",
								szAccountID);
						}
					}
					// ----
					g_DbManager.Clear();
					// ----
					if(Result == (UCHAR)eIDPASSRESULT_SUCCESS)
					{
						if(BlocCode > 0)
						{
							Result = (UCHAR)eIDPASSRESULT_ACCBANED;
						}
						else
						{
							if(g_ServerManager.AddUser(aIndex, szAccountID, szIp, lpMsg->Number) == false)
							{
								Result = (UCHAR)eIDPASSRESULT_MAXSERVER;
							}
						}
					}
				}																							// ####
				else
				{
					Result = (UCHAR)eIDPASSRESULT_BADID;
					// ----
					g_Log.LogAdd(g_Colors.BlueViolet(), "[GProtocol][JGPAccountRequest] :: Account doesn't exist - ID : %s", szAccountID);
				}
			}																								// ###
			else
			{
				g_Log.LogAdd(g_Colors.BlueViolet(), "[GProtocol][JGPAccountRequest] :: Fail ExecFormat ID : %s",
					szAccountID);
				// ----
				Result = (UCHAR)eIDPASSRESULT_TEMPBLOCK;
			}
			// ----
			g_DbManager.Clear();
		}																									// ##
		else
		{
			g_Log.LogAdd(g_Colors.BlueViolet(), "[GProtocol][JGPAccountRequest] :: Account already connected ID : %s",
				szAccountID);
			// ----
			Result = (UCHAR)eIDPASSRESULT_INUSE;
		}
	}																										// #
	else
	{
		g_Log.LogAdd(g_Colors.BlueViolet(), "[GProtocol][JGPAccountRequest] :: (ANTIHACK) Special characters in... ID : %s",
				szAccountID);
		// ----
		Result = (UCHAR)eIDPASSRESULT_TEMPBLOCK;
	}
	// ----
	//g_Log.LogAdd(g_Colors.Orange(), "(DEBUG) got passwd %s && compare with %s || acc %s", lpMsg->Pass,
		//szPassword, szAccountID);
	// ----
	SDHP_IDPASSRESULT msg(Result, Number, UserNumber, DBNumber, szAccountID, JoominNumber);
	g_ServerManager.DataSend(aIndex, (LPBYTE)&msg, msg.h.size);
}
//-------------------------------------------------------------------------------------------------------------------------------------------

void GJPAccountFail(int aIndex, SDHP_JOINFAIL * lpMsg)
{
	char szAccountID[11]	= {0};
	// ----
	memcpy(szAccountID, lpMsg->Id, 10);
	// ----
	g_Log.LogAdd(g_Colors.BlueViolet(), "[GProtocol][GJPAccountFail] :: ID : %s", szAccountID);
	// ----
	g_ServerManager.DelUser(szAccountID);
}
//-------------------------------------------------------------------------------------------------------------------------------------------

void GJPAccountBlock(int aIndex, SDHP_COMMAND_BLOCK * lpMsg)
{
	char szAccountID[11]	= {0};
	// ----
	memcpy(szAccountID, lpMsg->Id, 10);
	// ----
	g_Log.LogAdd(g_Colors.BlueViolet(), "[GProtocol][GJPAccountBlock] :: ID : %s", szAccountID);
	// ----
	g_DbManager.ExecFormat("UPDATE MEMB_INFO SET bloc_code = 1 WHERE memb___id = '%s'", szAccountID);
	g_DbManager.Fetch();
	g_DbManager.Clear();
}
//-------------------------------------------------------------------------------------------------------------------------------------------

void GJPUserClose(int aIndex, SDHP_USERCLOSE_ID * lpMsg)
{
	char szAccountID[11] = {0};
	memcpy(szAccountID, lpMsg->szId, 10);
	// ----
	g_Log.LogAdd(g_Colors.BlueViolet(), "[GProtocol][GJPUserClose] :: ID : %s", lpMsg->szId);
	// ----
	g_ServerManager.DelUser(szAccountID);
}
//-------------------------------------------------------------------------------------------------------------------------------------------

void GJReqMapSvrMove(int aIndex, PMSG_REQ_MAPSVRMOVE * lpMsg)
{
	/* Leo123 : to this function must be added enum for fResult */
	// ----
	char szAccountID[11]	= {0};
	DWORD fResult			= 0;
	int	iUserIndex			= SLOT_FREE;
	tagUSER_DATA * lpUser	= NULL;
	// ----
	int	iJoinAuthCode1		= GetTickCount();
	int	iJoinAuthCode2		= GetTickCount();
	int	iJoinAuthCode3		= GetTickCount();
	int	iJoinAuthCode4		= GetTickCount();
	// ----
	memcpy(szAccountID, lpMsg->szAccountID, 10);
	// ----
	g_Log.LogAdd(g_Colors.BlueViolet(), "[GProtocol][GJReqMapSvrMove] :: ID : %s", szAccountID);
	// ----
	if(lstrlen(szAccountID) > 1)
	{
		if((g_ServerManager.IsCodeExist(lpMsg->wCurMapSvrCode)) && (g_ServerManager.IsCodeExist(lpMsg->wDstMapSvrCode)))
		{
			if(m_ServObj[aIndex].m_ConnectedCount < m_ServObj[aIndex].m_MaxUser)
			{
				iUserIndex = g_ServerManager.GetUserIndexByID(szAccountID);
				// ----
				if(iUserIndex != SLOT_FREE)
				{
					lpUser = & m_UserObj[iUserIndex];
					// ----
					if(lpUser->m_bRequestMapSvrMove == false)
					{
						lpUser->m_bRequestMapSvrMove	= true;
						// ----
						lpUser->m_nMoveMapTime			= GetTickCount();
					}
					else
					{
						// # User Requested the Move
						fResult = 4;
					}
				}
				else
				{
					// # User not connected
					fResult = 1;
				}
			}
			else
			{
				// # Limit User reached
				fResult = 3;
			}
		}
		else
		{
			// # No dest server connected
			fResult = 2;
		}
	}
	else
	{
		// # User not connected
		fResult = 1;
	}
	// ----
	PMSG_ANS_MAPSVRMOVE		pMsg		= {0};
	// ----
	pMsg.h.set(JMSG_MAP_SVR_MOVE, sizeof(pMsg));
	// ----
	memcpy(pMsg.szAccountID, lpMsg->szAccountID, 11);
	memcpy(pMsg.szCharName, lpMsg->szCharName, 11);
	// ----
	iJoinAuthCode1				= GetTickCount();
	iJoinAuthCode2				= GetTickCount() + rand() % 10000;
	iJoinAuthCode3				= GetTickCount() + rand() % 777;
	iJoinAuthCode4				= GetTickCount() + rand() % 8911;
	// ----
	pMsg.iIndex					= lpMsg->iIndex;
	pMsg.btX					= lpMsg->btX;
	pMsg.btY					= lpMsg->btY;
	pMsg.wCurMapSvrCode			= lpMsg->wCurMapSvrCode;
	pMsg.wDstMapSvrCode			= lpMsg->wDstMapSvrCode;
	pMsg.wMapNumber				= lpMsg->wMapNumber;
	pMsg.iJoinAuthCode1			= iJoinAuthCode1;
	pMsg.iJoinAuthCode2			= iJoinAuthCode2;
	pMsg.iJoinAuthCode3			= iJoinAuthCode3;
	pMsg.iJoinAuthCode4			= iJoinAuthCode4;
	pMsg.iResult				= fResult;
	// ----
	if((lpUser != NULL) && (fResult == 0))
	{
		lpUser->iJoinAuthCode1	= iJoinAuthCode1;
		lpUser->iJoinAuthCode2	= iJoinAuthCode2;
		lpUser->iJoinAuthCode3	= iJoinAuthCode3;
		lpUser->iJoinAuthCode4	= iJoinAuthCode4;
		lpUser->wPrevMapSvrCode = lpMsg->wCurMapSvrCode;
		lpUser->wMapNumber		= lpMsg->wMapNumber;
		lpUser->btX				= lpMsg->btX;
		lpUser->btY				= lpMsg->btY;
	}
	// ---- (TEST)
	g_Log.LogAdd(g_Colors.Bisque(), "[GProtocol][Test] :: (DEBUG) fResult :%d, cur: %d , max: %d", fResult, m_ServObj[aIndex].m_ConnectedCount, m_ServObj[aIndex].m_MaxUser);
	// ----
	g_ServerManager.DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
}
//-------------------------------------------------------------------------------------------------------------------------------------------

void GJReqMapSvrAuth(int aIndex, PMSG_REQ_MAPSVRAUTH * lpMsg)
{
	char szAccountID[11]	= {0};
	char JoominNumber[14]	= {0};
	DWORD fResult			= 0;
	int iUserIndex			= SLOT_FREE;
	tagUSER_DATA * lpUser	= NULL;
	// ----
	int	DBNumber			= 0;
	int	UserNumber			= SLOT_FREE;
	int	BlocCode			= 0;
	// ----
	WORD wPrevMapSvrCode	= 0x00;
	WORD wMapNumber			= 0x00;
	BYTE btX				= 0x00;
	BYTE btY				= 0x00;
	// ----
	memcpy(szAccountID, lpMsg->szAccountID, 10);
	// ----
	g_Log.LogAdd(g_Colors.BlueViolet(), "[GProtocol][GJReqMapSvrAuth] :: ID : %s", szAccountID);
	// ----
	if(lstrlen(szAccountID) > 1)
	{
		if(g_ServerManager.IsCodeExist(lpMsg->wDstMapSvrCode) == true)
		{
			if(m_ServObj[aIndex].m_ConnectedCount < m_ServObj[aIndex].m_MaxUser)
			{
				iUserIndex = g_ServerManager.GetUserIndexByID(szAccountID);
				// ----
				if(iUserIndex != SLOT_FREE)
				{
					lpUser = & m_UserObj[iUserIndex];
					// ----
					if(lpUser->m_bRequestMapSvrMove == true)
					{
						if( lpMsg->iJoinAuthCode1 == lpUser->iJoinAuthCode1 &&
							 lpMsg->iJoinAuthCode2 == lpUser->iJoinAuthCode2 &&
							 lpMsg->iJoinAuthCode3 == lpUser->iJoinAuthCode3 &&
							 lpMsg->iJoinAuthCode4 == lpUser->iJoinAuthCode4 
							)
						{
							lpUser->m_bRequestMapSvrMove = false;
							// ----
							wPrevMapSvrCode = lpUser->wPrevMapSvrCode;
							wMapNumber		= lpUser->wMapNumber;
							btX				= lpUser->btX;
							btY				= lpUser->btY;
							UserNumber		= lpUser->iUserNumber;
							// ----
							g_DbManager.ExecFormat("SELECT * FROM MEMB_INFO WHERE memb___id='%s'",
								szAccountID);
							// ----
							if(g_DbManager.Fetch() != SQL_NO_DATA)
							{
								g_DbManager.GetStr("sno__numb", JoominNumber);
								// ----
								DBNumber	= g_DbManager.GetInt("memb_guid");
								BlocCode	= (BYTE)g_DbManager.GetInt("bloc_code");
							}
							// ----
							g_DbManager.Clear();
						}
						else
						{
							fResult = 2;
						}
					}
					else
					{
						fResult = 4;
					}
				}
				else
				{
					fResult = 1;
				}
			}
			else
			{
				fResult = 3;
			}
		}
		else
		{
			fResult = 2;
		}
	}
	else
	{
		fResult = 1;
	}
	// ----
	g_Log.LogAdd(g_Colors.Bisque(), "[GProtocol][Test] :: (DEBUG) fResult :%d", fResult);
	// ----
	PMSG_ANS_MAPSVRAUTH pMsg = {0};
	// ----
	pMsg.h.set(JMSG_MAP_SVR_AUTH, sizeof(pMsg));
	// ----
	pMsg.iIndex				= lpMsg->iIndex;
	pMsg.wPrevMapSvrCode	= wPrevMapSvrCode;
	pMsg.wMapNumber			= wMapNumber;
	pMsg.btX				= btX;
	pMsg.btY				= btY;
	pMsg.iResult			= fResult;
	pMsg.iUserNumber		= UserNumber;
	pMsg.iDBNumber			= DBNumber;
	pMsg.btBlockCode			= BlocCode;
	// ----
	memcpy(pMsg.szAccountID, szAccountID, 11);
	memcpy(pMsg.cJoominNumber, JoominNumber , 13);
	memcpy(pMsg.szCharName, lpMsg->szCharName, 11);
	// ----
	g_ServerManager.DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
}
//-------------------------------------------------------------------------------------------------------------------------------------------

void GJNotifyMaxUserCount(int aIndex, PMSG_NOTIFY_MAXUSER * lpMsg)
{
	if((aIndex != SLOT_FREE) && (g_ServerManager.IsCodeExist(lpMsg->iSvrCode)))
	{
		m_ServObj[aIndex].m_ConnectedCount	= lpMsg->iCurUserCount;
		m_ServObj[aIndex].m_MaxUser			= lpMsg->iMaxUserCount;
		// ----
		if(g_Main.m_CsHandle != NULL)
		{
			WORD Port		= g_ServerManager.GetPortByCode(lpMsg->iSvrCode);
			WORD Pracent	= ((lpMsg->iCurUserCount * 100) / lpMsg->iMaxUserCount);
			// ----
			if(PostMessage(g_Main.m_CsHandle, WM_APP, MSG_TO_CS, MAKELPARAM(Port,Pracent))  == false)
			{
				g_Log.LogAdd(g_Colors.Red(), "[JSProtocol][GJNotifyMaxUserCount] :: PostMessage failed with error code %d", GetLastError());
			}
		}
		else
		{
			g_Log.LogAdd(g_Colors.Red(), "[JSProtocol][GJNotifyMaxUserCount] :: Can't find eCS");
		}
	}
}
//-------------------------------------------------------------------------------------------------------------------------------------------

void GJAllNoticeSend(int aIndex, PMSG_SETENTIRENOTICE * lpMsg)
{
	char szNotice[60]	= {0};
	BYTE result			= 0x00;
	// ----
	memcpy(szNotice, lpMsg->Notice, 60);
	// ----
	g_Log.LogAdd(g_Colors.BlueViolet(), "[GProtocol][GJAllNoticeSend] :: %s", szNotice);
	// ----
	if(strlen(szNotice) > 1)
	{
		result = 0x01;
		// ----
		SDHP_NOTICE	pMsg = {0};
		pMsg.h.set(0x40, sizeof(pMsg));
		// ----
		memcpy(pMsg.Notice, szNotice, 60);
		// ----
		g_ServerManager.DataSendAll((LPBYTE)&pMsg, pMsg.h.size);
		g_ServerManager.Disconnect(aIndex);
	}
}
//-------------------------------------------------------------------------------------------------------------------------------------------

void GJUserNoticeSend(int aIndex, PMSG_USERNOTICE * lpMsg)
{
	char szNotice[61]		= {'\0'};
	char szAccountID[11]	= {'\0'};
	int iUserIndex			= SLOT_FREE;
	int iServerIndex		= SLOT_FREE;
	// ----
	memcpy(szNotice, lpMsg->Notice, 60);
	memcpy(szAccountID, lpMsg->AccountID, 10);
	// ----
	g_Log.LogAdd(g_Colors.BlueViolet(), "[GProtocol][GJAllNoticeSend] :: %s:%s", szAccountID, szNotice);
	// ----
	if((strlen(szNotice) > 1 ) && (strlen(szAccountID) > 1))
	{
		iUserIndex = g_ServerManager.GetUserIndexByID(szAccountID);
		// ----
		if(iUserIndex != SLOT_FREE)
		{
			iServerIndex = m_UserObj[iUserIndex].m_ServerIndex;
			// ----
			SDHP_USER_NOTICE msg(szAccountID,szNotice);
			g_ServerManager.DataSend(iServerIndex, (LPBYTE)&msg, msg.h.size);
			// ----
			g_ServerManager.Disconnect(aIndex);
		}
	}
}
//-------------------------------------------------------------------------------------------------------------------------------------------
