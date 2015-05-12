#include "GInclude.h"
#include "RSProtocol.h"
#include "Main.h"
#include "TDbManager.h"
#include "TRSManager.h"
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void RSProtocolCore(TRSManager * thisManager, int aIndex, BYTE HeadCode, LPBYTE aRecv, int iSize)
{
	g_Log.SetStatusText(g_Colors.White(), g_Colors.GreenYellow(), "%s ON_GO", MAIN_PROJECT_NAME);
	// ----
	switch ( HeadCode )
	{
		case 0x01:
		{
			DevilSqureScore(thisManager, (PMSG_ANS_EVENTUSERSCORE *)aRecv);
		}
		break;

		case 0x02:	
		{
			RankUpdate(thisManager);
		}
		break;

		case 0x0B:
		{
			EGReqBloodCastleEnterCount(thisManager, aIndex, (PMSG_REQ_BLOODCASTLE_ENTERCOUNT *)aRecv);
		}
		break;
	}
	// ----
	g_Log.SetStatusText(g_Colors.Black(), g_Colors.LightCoral() , "%s STATE_WAIT", MAIN_PROJECT_NAME);
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void DevilSqureScore(TRSManager * thisManager, PMSG_ANS_EVENTUSERSCORE * lpMsg)
{
	thisManager->OnMessage("[RSProtocol][DevilSqureScore] :: Server: %d-%d, Square: %d, Account: %s, Char: %s, Class: %d, Score: %d", 
		((lpMsg->ServerCode / 20) + 1), ((lpMsg->ServerCode % 20) + 1), lpMsg->SquareNum, lpMsg->AccountID, lpMsg->GameID, 
		lpMsg->Class, lpMsg->Score);
	// ----
	g_DbManager.ExecFormat("EXEC SP_POINT_ACCUMULATION '%d', '%d', '%s', '%s', '%d', '%d'",
		(lpMsg->ServerCode / 20 + 1), lpMsg->SquareNum, lpMsg->AccountID, lpMsg->GameID, lpMsg->Class, lpMsg->Score);
	// ----
	g_DbManager.Clear();
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void EGReqBloodCastleEnterCount(TRSManager * thisManager, int aIndex,PMSG_REQ_BLOODCASTLE_ENTERCOUNT * lpMsg)
{
	PMSG_ANS_BLOODCASTLE_ENTERCOUNT pMsg = {0};
	// ----
	pMsg.h.c		= 0xC1;
	pMsg.h.headcode	= 0x0B;
	pMsg.h.size		= sizeof(pMsg);
	// ----
	memcpy(pMsg.AccountID, lpMsg->AccountID, sizeof(pMsg.AccountID));
	memcpy(pMsg.GameID, lpMsg->GameID, sizeof(pMsg.GameID));
	// ----
	pMsg.ServerCode = lpMsg->ServerCode;
	pMsg.iObjIndex	= lpMsg->iObjIndex;
	// ----
	if(g_DbManager.ExecFormat("EXEC SP_LEFT_ENTERCOUNT_BC '%s', '%s', '%d'", lpMsg->AccountID, lpMsg->GameID, lpMsg->ServerCode) == true)
	{
		pMsg.iLeftCount = g_DbManager.GetInt("LeftEnterCount");
	}
	else
	{
		pMsg.iLeftCount = -1;
	}
	// ----
	g_DbManager.Clear();
	// ----
	thisManager->DataSend(aIndex,(LPBYTE)&pMsg,sizeof(pMsg));
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void BloodCastleScore_5TH(TRSManager * thisManager, PMSG_ANS_BLOODCASTLESCORE_5TH * lpMsg)
{
	thisManager->OnMessage("[RSProtocol][BloodCastleScore_5TH] :: SERVER:%d-%d, BRIDGE:%d, ACCNT:%s, CHAR:%s, CLASS:%d, SCORE:%d, LEFTTIME:%d, ALIVEPARTY:%d",
		lpMsg->ServerCode / 20 + 1,lpMsg->ServerCode % 20 + 1,lpMsg->BridgeNum,lpMsg->AccountID,lpMsg->GameID,lpMsg->Class,lpMsg->Score,lpMsg->iLeftTime,lpMsg->iAlivePartyCount);
	// ----
	g_DbManager.ExecFormat("EXEC SP_POINT_ACCM_BC_5TH '%d', '%d', '%s', '%s', '%d', '%d', '%d', '%d'",
		(lpMsg->ServerCode / 20 + 1), lpMsg->BridgeNum, lpMsg->AccountID, lpMsg->GameID, lpMsg->Class, 
		lpMsg->Score, lpMsg->iLeftTime, lpMsg->iAlivePartyCount);
	// ----
	g_DbManager.Clear();
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void RankUpdate(TRSManager * thisManager)
{
	thisManager->OnMessage("[RSProtocol][RankUpdate] :: update");
	// ----
	g_DbManager.Exec("EXEC SP_RANKING");
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------