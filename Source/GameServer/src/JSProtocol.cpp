//----------------------------------------------------------------------------------
// # Project:	Zodea eMU GameServer
// # Company:	RealCoderZ Software Incorpration. © 2012 http://www.realcoderz.com
// # Author:	WolF & M.E.S & EmaLeto
// # Hint:		This Script For Join Server Protocol Class.
//----------------------------------------------------------------------------------
#include "stdafx.h"
#include "JSProtocol.h"
#include "Main.H"
#include "GameMain.h"
#include "LogProc.H"
#include "Structures.H"
#include "Notice.H"
#include "Functions.H"


GJSProtocol JSProto;
// -------------------------------------------------------------------------------------------------------------------------------------------------------
GJSProtocol :: GJSProtocol()
{
	return;
}
// -------------------------------------------------------------------------------------------------------------------------------------------------------
GJSProtocol :: ~GJSProtocol()
{
	return;
}
// -------------------------------------------------------------------------------------------------------------------------------------------------------
void JSProtocolCore(BYTE HeadCode, LPBYTE aRecv, int iSize)
{
	switch ( HeadCode )
	{
		case JS_PROTO_CORE_SRV_LOGIN:
		{
			JSProto.JoinServerLogin((JOIN_SERVER_RESULT *)aRecv);				// Done & Revised By WolF	[24/12/2011]
		} break;
		// -----
		case JS_PROTO_CORE_ACC_REQUEST:
		{
			JSProto.AccountRequest((JOIN_SERVER_ACC_REQ_RESULT *)aRecv);		// Done & Revised By WolF	[24/12/2011]
		} break;
		// -----
		case JS_PROTO_CORE_JOIN_FAIL:
		{
			//JSProto.AccountFail();											// Done & Revised By WolF	[24/12/2011]
		} break;
		// -----
		case JS_PROTO_CORE_ACC_CLOSE:
		{
			//JSProto.UserClose();												// Done & Revised By WolF	[24/12/2011]
		} break;
	}
}
// -------------------------------------------------------------------------------------------------------------------------------------------------------
void GJSProtocol::JServerLoginInfo()
{
	JOIN_SERVER_INFO	JSInfo	= {0};
	// -----
	C1HeadSet((LPBYTE)&JSInfo, JS_PROTO_CORE_SRV_LOGIN, sizeof(JOIN_SERVER_INFO));
	// -----
	JSInfo.Port			= Port[0];
	JSInfo.ServerCode	= gGameServerCode;
	// -----
	strcpy(JSInfo.ServerName, szServerName);
	// -----
	wsJServerCli.DataSend((char*)&JSInfo, JSInfo.h.size);
}
// -------------------------------------------------------------------------------------------------------------------------------------------------------
void GJSProtocol::JoinServerLogin(JOIN_SERVER_RESULT * lpMsg)	
{
	if ( lpMsg->Result != 1 )
	{
		CLog.Error("JoinServer Manager Couldn't Add Server :: Result (%d)", lpMsg->Result);
		return;
	}
	// -----
	Connected[0]	=	TRUE;
	// -----
	gMainApp.bServerReady++;
	// -----
	SendMessage(gMainApp.m_hWnd, WM_START_SERVER, 0, 0);
}
// -------------------------------------------------------------------------------------------------------------------------------------------------------
void GJSProtocol::AccountRequest(JOIN_SERVER_ACC_REQ_RESULT * lpMsg)
{
	char	szAccountID[11]		= {0};
	char	JominNumber[14]		= {0};
	// -----
	memcpy(szAccountID, lpMsg->AccountID, 10);
	memcpy(JominNumber, lpMsg->JoominNumber, 13);
	// -----
	if(lpMsg->Result == 1)
	{
		if(!gObjSetAccountLogin(lpMsg->aIndex, szAccountID, lpMsg->UserNumber, lpMsg->DBNumber, JominNumber))
		{
			AccountFail(lpMsg->aIndex, szAccountID, lpMsg->DBNumber, lpMsg->UserNumber);
			// -----
			lpMsg->Result = ACC_REQ_MAX_USER;
		}
		// -----
		if(lpMsg->UserNumber == 0 && lpMsg->DBNumber == 0)
		{
			CLog.Error("User-Number & DB-Number are both Zero [%s]/[%d]/[%d].", szAccountID, lpMsg->UserNumber, lpMsg->DBNumber);
		}
	}
	// -----
	gObj[lpMsg->aIndex].LoginMsgSnd = 0;
	// -----
	if( gObj[lpMsg->aIndex].LoginMsgCount >= MSTR.NumOfLoginAttemps )
	{
		lpMsg->Result = ACC_REQ_LOGIN_THREE_TIMES;
	}
	// -----
	for(int i=OBJMIN; i<OBJMAX+1; i++)
	{
		if( gObj[i].Connected >= PLAYER_LOGGED )
		{
			if( gObj[i].AccountID[0] == szAccountID[0] )
			{
				if (strcmp(gObj[i].AccountID , szAccountID) == 0 )
				{
					SendMsg.MessageOut(i, 0x01, "[Attention] SomeOne Try Logging on Your Account!");
					break;
				}
			}
		}
	}
	// -----
	GCJoinResult(lpMsg->Result, lpMsg->aIndex);
	// -----
	if( lpMsg->Result != ACC_REQ_LOGIN_THREE_TIMES )
	{
		if ( gObj[lpMsg->aIndex].LoginMsgCount > MSTR.NumOfLoginAttemps )
		{
			gNet.CloseClient(lpMsg->aIndex);
		}
	}
}
// -------------------------------------------------------------------------------------------------------------------------------------------------------
void GJSProtocol::AccountFail(short aIndex, char * szAccountID, int DBNumber, int UserNumber)
{
	JOIN_SERVER_JOIN_FAIL	JSJoinFail	= {0};
	// -----
	C1HeadSet((LPBYTE)&JSJoinFail, JS_PROTO_CORE_JOIN_FAIL, sizeof(JOIN_SERVER_JOIN_FAIL));
	// -----
	JSJoinFail.aIndex		= aIndex;
	JSJoinFail.DBNumber		= DBNumber;
	JSJoinFail.UserNumber	= UserNumber;
	// -----
	strcpy(JSJoinFail.AccountID , szAccountID);
	// -----
	wsJServerCli.DataSend((char*)&JSJoinFail, JSJoinFail.h.size);
}
// -------------------------------------------------------------------------------------------------------------------------------------------------------
void GJSProtocol::UserClose(LPOBJ lpObj)
{
	JOIN_SERVER_USER_CLOSE	JSUserClose	= {0};
	// -----
	C1HeadSet((LPBYTE)&JSUserClose, JS_PROTO_CORE_ACC_CLOSE, sizeof(JOIN_SERVER_USER_CLOSE));
	// -----
	JSUserClose.Level		= lpObj->Level;
	JSUserClose.DbClass		= lpObj->DbClass;
	// -----
	memcpy(JSUserClose.AccountID , lpObj->AccountID, 10);
	memcpy(JSUserClose.PlayerName, lpObj->BackName, 10);
	// -----
	wsJServerCli.DataSend((char*)&JSUserClose, JSUserClose.h.size);
}
// -------------------------------------------------------------------------------------------------------------------------------------------------------