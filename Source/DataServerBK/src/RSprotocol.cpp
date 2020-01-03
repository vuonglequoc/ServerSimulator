// RSprotocol.cpp: implementation of the RSprotocol class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RSprotocol.h"
#include "giocp.h"
#include "LogProc.h"
#include "winutil.h"


CQuery g_RankingDB;



void InitRankingDB()
{
	if ( g_RankingDB.Connect(g_RankingServerDNS, g_UserID, g_Password) == TRUE )
	{
		LogAddC(4, "[RankingDB] Connection Successfull!");
	}
	else
	{
		LogAddC(2, "[RankingDB] Error On Connection!");
	}
}



void RSProtocolCore(int aIndex, BYTE HeadCode, LPBYTE aRecv, int iSize)
{
#if (TRACE_PACKET == 1 )
	LogAddHeadHex("RANKING_SERVER", aRecv, iSize);
#endif
	g_ServerInfoDisplayer.CheckRKDSConnection(TRUE);

	switch ( HeadCode )
	{
	case 0x01:
		DevilSqureScore((PMSG_ANS_EVENTUSERSCORE *)aRecv);
		break;
	/*case 0x02:	// No gameserver reference on this ? o_O
		RankUpdate();
		break;
	case 0x07:	// No gameserver reference on this ? o_O
		BloodCastleAccPoint((PMSG_ANS_BCACCPOINT *)aRecv);
		break;
	case 0x08:	// No gameserver reference on this ? o_O
		BloodCastleEnterCheck((PMSG_ANS_BLOODCASTLESCORE_5TH *)aRecv);
		break;*/
	case 0x0B:
		EGReqBloodCastleEnterCount(aIndex,(PMSG_REQ_BLOODCASTLE_ENTERCOUNT *)aRecv);
		break;
	case 0x0D:
		BloodCastleScore_5TH((PMSG_ANS_BLOODCASTLESCORE_5TH *)aRecv);
		break;
	}
	g_ServerInfoDisplayer.CheckRKDSConnection(FALSE);
}

void DevilSqureScore(PMSG_ANS_EVENTUSERSCORE * lpMsg)
{
	LogAddTD("[DEVIL] SERVER:%d-%d, SQUARE:%d, ACCNT:%s, CHAR:%s, CLASS:%d, SCORE:%d",
		lpMsg->ServerCode/20+1,lpMsg->ServerCode % 20 + 1,lpMsg->SquareNum,lpMsg->AccountID,lpMsg->GameID,lpMsg->Class,lpMsg->Score);

	g_RankingDB.ExecQuery("EXEC SP_POINT_ACCUMULATION '%d', '%d', '%s', '%s', '%d', '%d'",
		lpMsg->ServerCode/20+1,lpMsg->SquareNum,lpMsg->AccountID,lpMsg->GameID,lpMsg->Class,lpMsg->Score);

	g_RankingDB.Close();
}

void EGReqBloodCastleEnterCount(int aIndex,PMSG_REQ_BLOODCASTLE_ENTERCOUNT * lpMsg)
{
	PMSG_ANS_BLOODCASTLE_ENTERCOUNT pMsg;

	pMsg.h.c = 0xC1;
	pMsg.h.headcode = 0x0B;
	pMsg.h.size = sizeof(pMsg);

	memcpy(pMsg.AccountID,lpMsg->AccountID,sizeof(pMsg.AccountID));
	memcpy(pMsg.GameID,lpMsg->GameID,sizeof(pMsg.GameID));

	pMsg.ServerCode = lpMsg->ServerCode;
	pMsg.iObjIndex = lpMsg->iObjIndex;

	if(g_RankingDB.ExecQuery("EXEC SP_LEFT_ENTERCOUNT_BC '%s', '%s', '%d'",lpMsg->AccountID,lpMsg->GameID,lpMsg->ServerCode) == TRUE)
	{
		pMsg.iLeftCount = g_RankingDB.GetAsInteger("LeftEnterCount");
	}
	else
	{
		pMsg.iLeftCount = -1;
	}
	g_RankingDB.Close();
	DataSend(aIndex,(LPBYTE)&pMsg,sizeof(pMsg));
}

void BloodCastleScore_5TH(PMSG_ANS_BLOODCASTLESCORE_5TH * lpMsg)
{
	LogAddTD("[BLOOD] SERVER:%d-%d, BRIDGE:%d, ACCNT:%s, CHAR:%s, CLASS:%d, SCORE:%d, LEFTTIME:%d, ALIVEPARTY:%d",
		lpMsg->ServerCode / 20 + 1,lpMsg->ServerCode % 20 + 1,lpMsg->BridgeNum,lpMsg->AccountID,lpMsg->GameID,lpMsg->Class,lpMsg->Score,lpMsg->iLeftTime,lpMsg->iAlivePartyCount);

	g_RankingDB.ExecQuery("EXEC SP_POINT_ACCM_BC_5TH '%d', '%d', '%s', '%s', '%d', '%d', '%d', '%d'",
		lpMsg->ServerCode / 20 + 1,lpMsg->BridgeNum,lpMsg->AccountID,lpMsg->GameID,lpMsg->Class,lpMsg->Score,lpMsg->iLeftTime,lpMsg->iAlivePartyCount);
	g_RankingDB.Close();
}

/*void RankUpdate()
{
	LogAddTD("------------- Mu Ranking Update !! -------------");
	g_RankingDB.ExecQuery("EXEC SP_RANKING");
}

void BloodCastleAccPoint(PMSG_ANS_BCACCPOINT * lpMsg)
{
	LogAddTD("[BLOOD] SERVER:%d-%d, BRIDGE:%d, ACCNT:%s, CHAR:%s, CLASS:%d, SCORE:%d, PCROOM GUID:%d",
		lpMsg->ServerCode / 20 + 1,lpMsg->ServerCode % 20 + 1,lpMsg->Bridge,lpMsg->AccountID,lpMsg->GameID,lpMsg->Class,lpMsg->Score,lpMsg->PCRoomGuid);

	g_RankingDB.ExecQuery("EXEC SP_POINT_ACCM_BC '%d', '%d', '%s', '%s', '%d', '%d', '%d'",
		lpMsg->ServerCode / 20 + 1,lpMsg->Bridge,lpMsg->AccountID,lpMsg->GameID,lpMsg->Class,lpMsg->Score,lpMsg->PCRoomGuid);
}*/

