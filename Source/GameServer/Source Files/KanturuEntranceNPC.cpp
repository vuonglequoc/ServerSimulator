//**********************************************************************//
// Project:	HaRiBO MU GameServer - Season 4								//
// Company: RealCoderZ MU Development © 2010							//
// Description: MU Kanturu Enterance NPC Structure & Functions			//
// Decompilation By: Deathway											//
// Coded & Modified By:	WolF & M.E.S									//
//**********************************************************************//
#include "../Header Files/stdafx.h"
#include "../Header Files/KanturuEntranceNPC.h"
#include "../Header Files/Kanturu.h"
#include "../Header Files/KanturuBattleUserMng.h"
#include "../Header Files/LogProc.H"
#include "../Header Files/KanturuUtil.h"
#include "../Header Files/Functions.H"


static CKanturuUtil KANTURU_UTIL;

CKanturuEntranceNPC g_KanturuEntranceNPC;


CKanturuEntranceNPC::CKanturuEntranceNPC()
{
	return;
}

CKanturuEntranceNPC::~CKanturuEntranceNPC()
{
	return;
}

struct PMSG_ANS_KANTURU_STATE_INFO
{
	PBMSG_HEAD2 h;
	BYTE btState;
	BYTE btDetailState;
	BYTE btEnter;
	BYTE btUserCount;
	int iRemainTime;
};

void CKanturuEntranceNPC::NotifyEntranceInfo(int iIndex)
{
	PMSG_ANS_KANTURU_STATE_INFO pMsg = {0};

	C1SubHeadSet((LPBYTE)&pMsg, 0xD1, 0x00, sizeof(pMsg));
	int iCurrentState = g_Kanturu.GetKanturuState();
	int iCurrentDetailState = g_Kanturu.GetKanturuDetailState();
	pMsg.btState = iCurrentState;
	pMsg.btDetailState = iCurrentDetailState;
	pMsg.btEnter = g_Kanturu.CheckCanEnterKanturuBattle();
	pMsg.btUserCount = g_KanturuBattleUserMng.GetUserCount();
	pMsg.iRemainTime = g_Kanturu.GetRemainTime();

	KANTURU_UTIL.SendDataToUser(iIndex, (LPBYTE)&pMsg, sizeof(pMsg));
}

void CKanturuEntranceNPC::NotifyResultEnterKanturuBossMap(int iIndex)
{
	if ( !gObjIsConnected(iIndex))
	{
		return;
	}

	LPOBJ lpObj = &gObj[iIndex];
	BOOL bMoveGateSuccess = FALSE;
	int iResult = g_Kanturu.CheckEnterKanturu(iIndex);

	if ( iResult == 0 )
	{
		int iCurrentState = g_Kanturu.GetKanturuState();

		if ( iCurrentState == KANTURU_STATE_BATTLE_OF_MAYA )
		{
			bMoveGateSuccess = gObjMoveGate(iIndex, 133);

			if ( bMoveGateSuccess ==TRUE )
			{
				g_KanturuBattleUserMng.AddUserData(iIndex);

				CLog.LogAdd("[ KANTURU ][ Battle User ] Add User [%s][%s] Current Battle User:%d State:[%d-%d] (CharInfoSave : Class=%d, Level=%d, LvPoint=%d, Exp=%u, Str=%d, Dex=%d, Vit=%d, Energy=%d, LeaderShip=%d, Map=%d, Pk=%d",
					gObj[iIndex].AccountID, gObj[iIndex].Name,
					g_KanturuBattleUserMng.GetUserCount(), g_Kanturu.GetKanturuState(),
					g_Kanturu.GetKanturuDetailState(),
					gObj[iIndex].Class, gObj[iIndex].Level, gObj[iIndex].LevelUpPoint,
					gObj[iIndex].Experience, gObj[iIndex].Strength, gObj[iIndex].Dexterity,
					gObj[iIndex].Vitality, gObj[iIndex].Energy, gObj[iIndex].Leadership, 
					gObj[iIndex].MapNumber, gObj[iIndex].m_PK_Level);

				KANTURU_UTIL.NotifyKanturuCurrentState(iIndex, 
					g_Kanturu.GetKanturuState(),g_Kanturu.GetKanturuDetailState());
#if(!GS_CASTLE)
				gObj[iIndex].m_bKanturuEntranceByNPC = TRUE;
#endif
			}
		}
		else if ( iCurrentState == KANTURU_STATE_TOWER_OF_REFINEMENT )
		{
			bMoveGateSuccess = gObjMoveGate(iIndex, 135);

			KANTURU_UTIL.NotifyKanturuCurrentState(iIndex, 
				g_Kanturu.GetKanturuState(),g_Kanturu.GetKanturuDetailState());
#if(!GS_CASTLE)
			gObj[iIndex].m_bKanturuEntranceByNPC = TRUE;
#endif
		}
		else
		{
			CLog.LogAdd("[ KANTURU ][ BossMapMove ] Error - State(%d) Character[%s][%s]",
				iCurrentState, lpObj->AccountID, lpObj->Name);
		}
	}
	else
	{
		KANTURU_UTIL.NotifyKanturuEntranceReqResult(iIndex, iResult);

		CLog.LogAdd("[ KANTURU ][ BossMapMove ] Fail - Result(%d) Character[%s][%s]",
			iResult, lpObj->AccountID, lpObj->Name);
#if(!GS_CASTLE)
		gObj[iIndex].m_bKanturuEntranceByNPC = FALSE;
#endif
	}
}