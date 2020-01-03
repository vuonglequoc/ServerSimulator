//**********************************************************************//
// Project:	HaRiBO MU GameServer - Season 6								//
// Company: RealCoderZ MU Development © 2010							//
// Description: MU NPC Talk System Structures & Functions				//
// Coded & Modified By:	WolF & M.E.S									//
//**********************************************************************//
#include "stdafx.h"
#include "user.h"
#include "NpcTalk.h"
#include "gObjMonster.h"
#include "protocol.h"
#include "GameMain.h"
#include "BloodCastle.h"
#include "gObjMonster.h"
#include "DevilSquare.h"
#include "QuestInfo.h"
#include "CastleSiege.h"
#include "CastleSiegeSync.h"
#include "KanturuEntranceNPC.h"
#include "JewelOfHarmonySystem.h"
#include "Gamemain.h"
#include "Event.h"
#include "IllusionTemple.h"
#include "Functions.H"
#include "DuelSystem.h"
#include "DoubleGoer.h"
#include "ImperialFort.h"
#include "Notice.H"
#include "SantaEvent.h"
#include "LuckyCoin.h"
#include "MossMerchant.h"
#include "GoldenArcher.h"
#include "SendProto.H"


int NpcNumberTalk;

CNpcTalk gNpcTalk;

CNpcTalk::CNpcTalk(void)
{
	// ----
}

CNpcTalk::~CNpcTalk(void)
{
	// ----
}

bool CNpcTalk::NpcTalk(LPOBJ lpNpc, LPOBJ lpObj)
{
	int NpcNumber = lpNpc->Class;

	if ( NpcNumber < 0 )
	{
		CLog.LogAdd("[NpcTalk]: NPC Number Error %d",NpcNumber);
		return false;
	}

	if ( lpObj->m_IfState.use > 0 )
	{
		return true;
	}

	switch ( NpcNumber )
	{
		case NPC_MARLON:
			gQeustNpcTeleport.TalkRefAdd();
			lpObj->TargetShopNumber = NpcNumber;
		break;

		case NPC_SEVINA_THE_PRIEST:
			if ( NpcQuestCheck(lpNpc, lpObj) == true )
			{
				return true;
			}
		break;

		#if(!GS_CASTLE)
		case NPC_GATEWAY_MACHINE:
			if ( NpcMainatenceMachine ( lpNpc, lpObj ) == TRUE )
			{
				return true;
			}
		break;
		#endif

		case NPC_ORACLE_LAYLA:
			if ( NpcReira ( lpNpc, lpObj ) == TRUE )
			{
				return true;
			}
		break;

		case NPC_PASI_THE_MAGE:
			if ( NpcFasi ( lpNpc, lpObj ) == TRUE )
			{
				return true;
			}
		break;

		case 241:
			if ( NpcGuildMasterTalk( lpNpc, lpObj ) == TRUE )
			{
				return true;
			}
			break;
		
		case 239:
			if ( NpcBattleAnnouncer( lpNpc, lpObj ) == TRUE )
			{
				return true;
			}
			break;

		case 249:
			if ( NpcRolensiaGuard( lpNpc, lpObj ) == TRUE )
			{
				return true;
			}
			break;

		case 238:
			if ( NpcChaosGoblelin( lpNpc, lpObj ) == TRUE )
			{
				return true;
			}
			break;

		case 237:
			if ( NpcRusipher( lpNpc, lpObj ) == TRUE )
			{
				return true;
			}
			break;

		case 253:
			if ( NpcPosionGirl( lpNpc, lpObj ) == TRUE )
			{
				return true;
			}
			break;

		case 245:
			if ( NpcDeviasWizard( lpNpc, lpObj ) == TRUE )
			{
				return true;
			}
			break;

		case 247:
			if ( NpcDeviasGuard( lpNpc, lpObj ) == TRUE )
			{
				return true;
			}
			break;

		case 240:
			if ( NpcWarehouse( lpNpc, lpObj ) == TRUE )
			{
				return true;
			}
			break;

		case 226:
			if ( NpcDarkSpiritTrainer( lpNpc, lpObj ) == TRUE )
			{
				return true;
			}
			break;

		case 242:
			if ( NpcNoriaRara( lpNpc, lpObj ) == TRUE )
			{
				return true;
			}
			break;

		case 244:
			if ( NpcDeviasMadam( lpNpc, lpObj ) == TRUE )
			{
				return true;
			}
			break;

		case 236:
			if ( NpcEventChipNPC( lpNpc, lpObj ) == TRUE )
			{
				return true;
			}
			break;

		case NPC_PET_TRAINER:
			if ( NpcServerDivision( lpNpc, lpObj ) == TRUE )
			{
				return true;
			}
			break;

		case 232:
			if ( NpcAngelKing( lpNpc, lpObj ) == TRUE )
			{
				return true;
			}
			break;

		case 233:
			if ( NpcAngelMessanger( lpNpc, lpObj ) == TRUE )
			{
				return true;
			}
			break;

		case 255:
			if ( NpcRolensiaMadam( lpNpc, lpObj ) == TRUE )
			{
				return true;
			}
			break;

		case 250:
			if ( NpcRoadMerchant( lpNpc, lpObj ) == TRUE )
			{
				return true;
			}
			break;

		case 246:
			if ( NpcDeviasWeapon( lpNpc, lpObj ) == TRUE )
			{
				return true;
			}
			break;

		case 251:
			if ( NpcRorensiaSmith( lpNpc, lpObj ) == TRUE )
			{
				return true;
			}
			break;

		case 243:
			if ( NpcNoriJangIn( lpNpc, lpObj ) == TRUE )
			{
				return true;
			}
			break;

		case 256:
			if ( NpcJewelMixDealer( lpNpc, lpObj ) == TRUE )
			{
				return true;
			}
			break;

		case 221:
			if ( NpcSiegeMachine_Attack( lpNpc, lpObj ) == TRUE )
			{
				return true;
			}
			break;

		case 222:
			if ( NpcSiegeMachine_Defense( lpNpc, lpObj ) == TRUE )
			{
				return true;
			}
			break;

		case 223:
			if ( NpcElderCircle( lpNpc, lpObj ) == TRUE )
			{
				return true;
			}
			break;

		case 224:
			if ( NpcCastleGuard( lpNpc, lpObj ) == TRUE )
			{
				return true;
			}
			break;

		case 219:
			if ( NpcCastleGateLever( lpNpc, lpObj ) == TRUE )
			{
				return true;
			}
			break;

		case 216:
			if ( NpcCastleCrown( lpNpc, lpObj ) == TRUE )
			{
				return true;
			}
			break;

		case 217:
		case 218:
			if ( NpcCastleSwitch( lpNpc, lpObj ) == TRUE )
			{
				return true;
			}
			break;

#if(GS_CASTLE)
		case 220:
			if( ::NpcCastleHuntZoneGuard(lpNpc,lpObj) == TRUE)
			{
				return true;
			}
#endif

		case 257:
			lpObj->QuestNPC = 257;
			if ( NpcShadowPhantom( lpNpc, lpObj ) == TRUE )
			{
				return true;
			}
			break;

		case 258:
			if(lpObj->m_IfState.use != 0)
			{
				return true;
			}

			PMSG_TALKRESULT pMsg;

			pMsg.h.c = 0xC3;
			pMsg.h.headcode = 0x30;
			pMsg.h.size = 9 ;
			pMsg.result = 0x19;

			gSendProto.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);

			return true;
			break;

		case 368:
		case 369:
		case 370:
			if ( g_kJewelOfHarmonySystem.NpcJewelOfHarmony( lpNpc, lpObj ) == TRUE )
			{
				return true;
			}
			break;

		case 375:
			if ( NpcChaosCardMaster( lpNpc, lpObj ) == TRUE )
			{
				return true;
			}
			break;

		case 371:
			if ( NpcEliteSoldier( lpNpc, lpObj ) == TRUE )
			{
				return true;
			}
			break;

		case 376:
			if ( NpcDrinkSellerHillary( lpNpc, lpObj ) == TRUE )
			{
				return true;
			}
			break;

		case 377:
			if ( NpcDrinkSellerLindsay( lpNpc, lpObj ) == TRUE )
			{
				return true;
			}
			break;

		case 379:
			if ( NpcFireworksGirl( lpNpc, lpObj ) == true )
			{
				return true;
			}
			break;

		case 414:
			if ( NpcEleganceAllen( lpNpc, lpObj ) == true )
			{
				return true;
			}
			break;

		case 415:
			if ( NpcSilvia( lpNpc, lpObj ) == true )
			{
				return true;
			}
			break;

		case 416:
			if ( NpcLeah( lpNpc, lpObj ) == true )
			{
				return true;
			}
			break;

		case 417:
			if ( NpcMarseille( lpNpc, lpObj ) == true )
			{
				return true;
			}
			break;

#if(!GS_CASTLE)
		case 380:
		case 381:
		case 382:
		case 383:
		case 384:
		case 385:
			if( g_IllusionTemple.NpcTalk ( lpNpc, lpObj ) == true)
			{
				return true;
			}
			break;

#endif
		case 450:	// Cherry Blossom NPC
			if ( NpcCherryBlossom(lpNpc,lpObj) == true )
			{
				return true;
			}
			break;

		case 451:

			break;
		case 452:
			if ( NpcSeedMaster( lpNpc, lpObj ) == true )
			{
				return true;
			}
			break;

		case 453:
			if ( NpcSeedResearch( lpNpc, lpObj ) == true )
			{
				return true;
			}
			break;

		case 479:
			if ( NpcTitusGatekeeper( lpNpc, lpObj ) == true )
			{
				return true;
			}
			break;

		case 492:
			if ( NpcMoss( lpNpc, lpObj ) == true )
			{
				return true;
			}
			break;
		case 547:
			if ( NpcMarketUnionJulia ( lpNpc, lpObj ) == true )
			{
				return true;
			}
			break;
		case 543:
			if ( NpcDuprianGen ( lpNpc, lpObj ) == true )
			{
				return true;
			}
			break;
		case 544:
			if ( NpcVanertGen ( lpNpc, lpObj ) == true )
			{
				return true;
			}
			break;
		case 522:
			if ( NpcImperialFort ( lpNpc,lpObj ) == true )
			{
				return true;
			}
			break;
		case 540:
			if ( NpcDoubleGoer (lpNpc, lpObj ) == true )
			{
				return true;
			}
			break;
		case 541:	// -- Double Goer Silver Chest
#if(!GS_CASTLE)
			gDoubleGoer.OpenChest(lpObj->m_Index,lpNpc,0);
			return true;
			break;
		case 542:	// -- Double Goer Gold Chest
			gDoubleGoer.OpenChest(lpObj->m_Index,lpNpc,1);
			return true;
			break;
#endif
		case 478:	// -- Delgado
			if ( gNpcTalk.NpcDelgado(lpNpc,lpObj) == true )
			{
				return true;
			}
			break;

		case 465:	// -- Santa Claus
			break;
		case 467:	// -- Snowman
			if ( NpcSnowMan(lpNpc,lpObj) == true )
			{
				return true;
			}
			break;
		case 468:	// Yellow Little Santa
			gSantaEvent.LittleSantaBuffer(lpObj,0);
			return true;
			break;
		case 469:	// Green Little Santa
			gSantaEvent.LittleSantaBuffer(lpObj,1);
			return true;
			break;
		case 470:	// Red Little Santa
			gSantaEvent.LittleSantaBuffer(lpObj,2);
			return true;
			break;
		case 471:	// Blue Little Santa
			gSantaEvent.LittleSantaBuffer(lpObj,3);
			return true;
			break;
		case 472:	// White Little Santa
			gSantaEvent.LittleSantaBuffer(lpObj,4);
			return true;
			break;
		case 473:	// Black Little Santa
			gSantaEvent.LittleSantaBuffer(lpObj,5);
			return true;
			break;
		case 474:	// Orange Little Santa
			gSantaEvent.LittleSantaBuffer(lpObj,6);
			return true;
			break;
		case 475:	// Violet Little Santa
			gSantaEvent.LittleSantaBuffer(lpObj,7);
			return true;
			break;
		case 566:
			lpObj->QuestNPC = 566;
			if ( NpcGyro(lpNpc,lpObj) == true )
			{
				return true;
			}
			break;
		case 567:
			lpObj->QuestNPC = 567;
			if ( NpcOracle(lpNpc,lpObj) == true )
			{
				return true;
			}
			break;
		case 568:
			lpObj->QuestNPC = 568;
			if ( NpcMercenary(lpNpc,lpObj) == true )
			{
				return true;
			}
			break;
	}

	return FALSE;
}
// ----------------------------------------------------------------------------------------------------------------------
bool CNpcTalk::NpcDelgado(LPOBJ lpNpc, LPOBJ lpObj)
{
	if ( gLuckyCoin.Enabled == 0 )
	{
		return true;
	}

	if(lpObj->m_IfState.use != 0)
	{
		return true;
	}

	/*BYTE LuckyCoinPacket[5] = {0xC3,0x05,0x30,0x20,0x01};

	gSendProto.DataSend(lpObj->m_Index,LuckyCoinPacket,LuckyCoinPacket[1]);*/

	PMSG_TALKRESULT pMsg;

	pMsg.h.c = 0xC3;
	pMsg.h.headcode = 0x30;
	pMsg.h.size = 0x09;
	pMsg.result = 0x20;

	gSendProto.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);

	return true;

}
// ----------------------------------------------------------------------------------------------------------------------
#if(!GS_CASTLE)
bool NpcMainatenceMachine(LPOBJ lpNpc, LPOBJ lpObj)
{
	g_KanturuEntranceNPC.NotifyEntranceInfo(lpObj->m_Index);
	return true;
}
#endif
// ----------------------------------------------------------------------------------------------------------------------
bool NpcReira(LPOBJ lpNpc, LPOBJ lpObj)
{
	return false;
}
// ----------------------------------------------------------------------------------------------------------------------
bool NpcBattleAnnouncer(LPOBJ lpNpc, LPOBJ lpObj)
{
#if (GS_CASTLE)

#endif

	return false;
}
// ----------------------------------------------------------------------------------------------------------------------
bool NpcFasi(LPOBJ lpNpc, LPOBJ lpObj)
{
	if ( gLanguage != 0 )
	{
		if ( g_DevilSquare.Enabled == 1 )
		{
			GCServerCmd(lpObj->m_Index, 1, 6, 0);
		}
	}

	if ( lpObj->m_PK_Level > 4 )
	{
		int hour = 0;
		int min = 0;

		if ( lpNpc->TargetNumber != lpObj->m_Index )
		{
			if ( gObjGetPkTime(lpObj, hour, min) == TRUE )
			{
				char szTemp[100];

				wsprintf(szTemp, lMsg.Get(MSGGET(4, 79)), hour, min);

				ChatTargetSend(lpNpc, szTemp, lpObj->m_Index);

				lpNpc->TargetNumber = lpObj->m_Index;

				/*if ( gPkLimitFree == FALSE )
				{
					return TRUE;
				}*/
			}
		}
		/*else if ( gPkLimitFree == FALSE )
		{
			int numbertext = rand() % 3;

			switch ( numbertext )
			{
				case 0:
					ChatTargetSend(lpNpc, lMsg.Get(MSGGET(4, 98)), lpObj->m_Index);
					break;

				case 1:
					ChatTargetSend(lpNpc, lMsg.Get(MSGGET(4, 99)), lpObj->m_Index);
					break;

				case 2:
					ChatTargetSend(lpNpc, lMsg.Get(MSGGET(4, 100)), lpObj->m_Index);
					break;
			}

			lpNpc->TargetNumber = 0;	// Apply Deathway Fix, change 0 to -1;
			return true;
		}*/
	}

	return false;
}
// ----------------------------------------------------------------------------------------------------------------------
bool NpcGuildMasterTalk(LPOBJ lpNpc, LPOBJ lpObj)
{
	if ( gGuildCreate == FALSE )
	{
		ChatTargetSend(lpNpc, "Guild Creation is closed", lpObj->m_Index);
		return true;
	}

	if ( lpObj->GuildNumber > 0 )
	{
		ChatTargetSend(lpNpc, lMsg.Get(MSGGET(4, 116)), lpObj->m_Index);
		return true;
	}

	int capacity = gObjGuildMasterCapacity(lpObj);

	if ( capacity == 0 )
	{
		ChatTargetSend(lpNpc, lMsg.Get(MSGGET(4, 117)), lpObj->m_Index);
	}
	else if ( capacity == 2 )
	{
		ChatTargetSend(lpNpc, lMsg.Get(MSGGET(4, 118)), lpObj->m_Index);
	}
	else
	{
		GCGuildMasterQuestionSend(lpObj->m_Index);
	}

	return true;
}
// ----------------------------------------------------------------------------------------------------------------------
bool NpcRolensiaGuard(LPOBJ lpNpc, LPOBJ lpObj)
{
	if ( gLanguage != 0 )
	{
	
	}

	ChatTargetSend(lpNpc, "BK-MU Online", lpObj->m_Index);
	return TRUE;
}
// ----------------------------------------------------------------------------------------------------------------------
bool NpcChaosGoblelin(LPOBJ lpNpc, LPOBJ lpObj)
{
	gUserFindDevilSquareKeyEyes(lpObj->m_Index);

	return FALSE;
}
// ----------------------------------------------------------------------------------------------------------------------
bool g_bUseLotterySystem = TRUE;
// ----------------------------------------------------------------------------------------------------------------------
bool NpcChaosCardMaster(LPOBJ lpNpc, LPOBJ lpObj)
{
	if ( g_bUseLotterySystem == FALSE )
		return FALSE;

	if ( lpNpc->m_IfState.use == 1 && lpNpc->m_IfState.type == 1 ) // Testar!
		return TRUE;

	if ( bCanChaosBox == TRUE )
	{
		if ( lpObj->m_IfState.use > 0 )
			return FALSE;

		if ( lpObj->m_bPShopOpen == true )
		{
			CLog.LogAdd("[%s][%s] is Already Opening PShop, ChaosBox Failed",
				lpObj->AccountID, lpObj->Name);

			GCServerMsgStringSend(lMsg.Get(MSGGET(4,194)), lpObj->m_Index, 1);
			return TRUE;
		}

		PMSG_TALKRESULT pMsg;

		pMsg.h.c = 0xC3;
		pMsg.h.headcode = 0x30;
		pMsg.h.size = sizeof(pMsg);
		pMsg.result = 0x15;

		gSendProto.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);

		lpObj->m_IfState.use = 1;
		lpObj->m_IfState.type = 7;

		CLog.LogAdd("[JewelMix] [%s][%s] Jewel Mix Dealer Start",
			lpObj->AccountID, lpObj->Name);

		gObjItemTextSave(lpObj);
		gObjWarehouseTextSave(lpObj);
	}

	return TRUE;
}
// ----------------------------------------------------------------------------------------------------------------------
bool NpcRusipher(LPOBJ lpNpc, LPOBJ lpObj)
{
	if ( g_DevilSquare.Enabled == 0 )
	{
		return true;
	}


	int aIndex = lpObj->m_Index;

	if ( (lpObj->m_IfState.use ) > 0 )
	{
		return true;
	}

	if ( gUserFindDevilSquareInvitation(aIndex) == FALSE )
	{
		GCServerCmd(lpObj->m_Index, 1, 2 ,0);
	}
	else if ( g_DevilSquare.GetState() == 1 )
	{
		PMSG_TALKRESULT pMsg;

		pMsg.h.c = 0xC3;
		pMsg.h.headcode = 0x30;
		pMsg.h.size = sizeof(pMsg);
		pMsg.result = 0x04;

		gSendProto.DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);

		lpObj->m_IfState.use = 1;
		lpObj->m_IfState.type = 12;
	}
	else
	{
		GCServerCmd(lpObj->m_Index, 1, 3, 0);
	}

	return true;
}
// ----------------------------------------------------------------------------------------------------------------------
bool NpcPosionGirl(LPOBJ lpNpc, LPOBJ lpObj)
{
	return false;
}
// ----------------------------------------------------------------------------------------------------------------------
bool NpcDeviasWizard(LPOBJ lpNpc, LPOBJ lpObj)
{
	return false;
}
// ----------------------------------------------------------------------------------------------------------------------
bool NpcDeviasGuard(LPOBJ lpNpc, LPOBJ lpObj)
{
	//ChatTargetSend(lpNpc, "HaRiBO ProJecT E-MU Online", lpObj->m_Index);
	ChatTargetSend(lpNpc, "BK-MU Online", lpObj->m_Index);
	return true;
}
// ----------------------------------------------------------------------------------------------------------------------
bool NpcDeviasWareHousemen(LPOBJ lpNpc, LPOBJ lpObj)
{
	return false;
}
// ----------------------------------------------------------------------------------------------------------------------
bool NpcWarehouse(LPOBJ lpNpc, LPOBJ lpObj)
{
	return false;
}
// ----------------------------------------------------------------------------------------------------------------------
bool NpcNoriaRara(LPOBJ lpNpc, LPOBJ lpObj)
{
	if ( gLanguage != 0 )
	{
	
	}
	/*else if ( gEnableEventNPCTalk != FALSE )
	{
		GCServerCmd(lpObj->m_Index, 4, 2, 0);
	}*/

	return false;
}
// ----------------------------------------------------------------------------------------------------------------------
bool NpcDeviasMadam(LPOBJ lpNpc, LPOBJ lpObj)
{
	if ( gLanguage != 0 )
	{
	
	}
	/*else if ( gEnableEventNPCTalk != FALSE )
	{
		GCServerCmd(lpObj->m_Index, 4, 1, 0);
	}*/

	return false;
}
// ----------------------------------------------------------------------------------------------------------------------
struct PMSG_REQ_VIEW_EC_MN
{
	PBMSG_HEAD h;
	int iINDEX;
	char szUID[11];
};
// ----------------------------------------------------------------------------------------------------------------------
bool NpcEventChipNPC(LPOBJ lpNpc, LPOBJ lpObj)
{
	gGAEvent.NPCTalk_GoldenArcher(lpObj->m_Index);

	/*if ( (lpObj->m_IfState.use) > 0 )
	{
		return TRUE;
	}
	*/

	/*if ( lpNpc->MapNumber == 0 )
	{
		PMSG_REQ_VIEW_EC_MN pMsgTha;

		C1HeadSet((LPBYTE)&pMsgTha, 0x01, sizeof(pMsgTha));
		pMsgTha.iINDEX = lpObj->m_Index;
		strcpy(pMsgTha.szUID, lpObj->AccountID);
		pEventObj = lpNpc;

		DataSendEventChip((char*)&pMsgTha, sizeof(pMsgTha));
		
		lpObj->m_IfState.type = 9;
		lpObj->m_IfState.state = 0;
		lpObj->m_IfState.use = 1;

		return TRUE;
	}

	if ( lpNpc->MapNumber == 0 ) // Lucky Number
	{
		PMSG_EVENTCHIPINFO Result;

		C1HeadSet((LPBYTE)&Result, 0x94, sizeof(Result));
		Result.Type = 2;
		
		gSendProto.DataSend(lpObj->m_Index, (LPBYTE)&Result, Result.h.size);

		lpObj->m_IfState.type = 9;
		lpObj->m_IfState.state = 0;
		lpObj->m_IfState.use = 1;

		return TRUE;
	}
	*/
	return true;
}
// ----------------------------------------------------------------------------------------------------------------------
bool NpcRorensiaSmith(LPOBJ lpNpc, LPOBJ lpObj)
{
	return FALSE;
}
// ----------------------------------------------------------------------------------------------------------------------
bool NpcNoriJangIn(LPOBJ lpNpc, LPOBJ lpObj)
{
	return FALSE;
}
// ----------------------------------------------------------------------------------------------------------------------
bool NpcQuestCheck(LPOBJ lpNpc, LPOBJ lpObj)
{
	if ( g_QuestInfo.NpcTalk(lpNpc, lpObj) != FALSE )
	{
		return true;
	}

	return false;
}
// ----------------------------------------------------------------------------------------------------------------------
bool NpcServerDivision(LPOBJ lpNpc, LPOBJ lpObj)
{
	if ( lpObj->lpGuild != NULL )
	{
		GCServerCmd(lpObj->m_Index, 6, 0, 0);
		return TRUE;
	}

	lpObj->m_IfState.type = 11;
	lpObj->m_IfState.state = 0;
	lpObj->m_IfState.use = 1;

	return FALSE;
}
// ----------------------------------------------------------------------------------------------------------------------
bool NpcRoadMerchant(LPOBJ lpNpc, LPOBJ lpObj)
{
	return FALSE;
}
// ----------------------------------------------------------------------------------------------------------------------
bool NpcAngelKing(LPOBJ lpNpc, LPOBJ lpObj)
{
	int iITEM_LEVEL = 0;

	if ( BC_BRIDGE_RANGE(lpObj->m_cBloodCastleIndex) == false )
	{
		GCServerCmd(lpObj->m_Index, 1, 0x18, 0);
		return false;
	}

	CLog.LogAdd("[Blood Castle] (%d) [%s][%s] Talk to Angel King", lpObj->m_cBloodCastleIndex+1, lpObj->AccountID, lpObj->Name);

	if ( g_BloodCastle.m_BridgeData[lpObj->m_cBloodCastleIndex].m_bBC_REWARDED == true )
	{
		GCServerCmd(lpObj->m_Index, 1, 0x2E, 0);
		return false;
	}

	if ( g_BloodCastle.GetCurrentState(lpObj->m_cBloodCastleIndex) != BC_STATE_PLAYING || g_BloodCastle.CheckPlayStart(lpObj->m_cBloodCastleIndex) == false)
	{
		GCServerCmd(lpObj->m_Index, 1, 0x18, 0);
		return false;
	}

	if ( lpObj->m_bBloodCastleComplete == true)
	{
		GCServerCmd(lpObj->m_Index, 1, 0x2E, 0);
		return false;
	}

	if ( g_BloodCastle.CheckQuestItem(lpObj->m_Index) == 0 || g_BloodCastle.CheckQuestItem(lpObj->m_Index) == 1 || g_BloodCastle.CheckQuestItem(lpObj->m_Index) == 2 )
	{
		if ( lpNpc->m_cBloodCastleIndex != lpObj->m_cBloodCastleIndex )
		{
			GCServerCmd(lpObj->m_Index, 1, 0x18, 0);
			return false;
		}

		if ( g_BloodCastle.CheckUserBridgeMember(lpObj->m_cBloodCastleIndex, lpObj->m_Index) == false )
		{
			GCServerCmd(lpObj->m_Index, 1, 0x18, 0);
			return false;
		}

		g_BloodCastle.SetUserState(lpObj->m_Index, 2);
		g_BloodCastle.m_BridgeData[lpObj->m_cBloodCastleIndex].m_iExtraEXP_Win_Quest_Party = lpObj->PartyNumber;
		g_BloodCastle.m_BridgeData[lpObj->m_cBloodCastleIndex].m_iExtraEXP_Win_Quest_Index = lpObj->m_Index;
		memcpy(g_BloodCastle.m_BridgeData[lpObj->m_cBloodCastleIndex].m_szWin_Quest_CharName, lpObj->Name, 10);
		memcpy(g_BloodCastle.m_BridgeData[lpObj->m_cBloodCastleIndex].m_szWin_Quest_AccountID, lpObj->AccountID, 10);
		g_BloodCastle.m_BridgeData[lpObj->m_cBloodCastleIndex].m_szWin_Quest_CharName[10] = 0;	// Zero String terminated
		g_BloodCastle.m_BridgeData[lpObj->m_cBloodCastleIndex].m_szWin_Quest_AccountID[10] = 0;	// Zero String Terminated
		
		if ( g_BloodCastle.SetBridgeWinner(lpObj->m_cBloodCastleIndex, lpObj->m_Index) == true )
		{
			lpObj->m_bBloodCastleComplete = true;

			GCServerCmd(lpObj->m_Index, 1, 0x17, 0);
		}
		else
		{
			return false;
		}

		g_BloodCastle.SearchUserDeleteQuestItem(lpObj->m_Index);
	}
	else
	{
		if ( g_BloodCastle.CheckUserWinnerParty(lpObj->m_cBloodCastleIndex, lpObj->m_Index) == TRUE )
		{
			lpObj->m_bBloodCastleComplete = true;

			GCServerCmd(lpObj->m_Index, 1, 0x17, 0);

			return false;
		}
		GCServerCmd(lpObj->m_Index, 1, 0x18, 0);
	}
	
	return false;
}
// ----------------------------------------------------------------------------------------------------------------------
bool NpcAngelMessanger(LPOBJ lpNpc, LPOBJ lpObj)
{
	if ( (lpObj->m_IfState.use ) > 0 )
	{
		return true;
	}

	int iIndex = lpObj->m_Index;

	if ( gObj[iIndex].m_PK_Level >= 4 )
	{
		GCServerCmd(iIndex, 0x39, 0, 0);
		return true;
	}

	int iITEM_LEVEL = 0;

	iITEM_LEVEL = g_BloodCastle.CheckEnterItem(lpObj->m_Index);

	if ( iITEM_LEVEL != 0 )
	{
		if ( iITEM_LEVEL != 10 && g_BloodCastle.CheckEnterLevel(lpObj->m_Index, iITEM_LEVEL-1) )
		{
			if ( g_BloodCastle.CheckEnterFreeTicket(lpObj->m_Index) == true )
			{
				iITEM_LEVEL = 10;
			}
		}

		if ( iITEM_LEVEL == 10 )
		{
			for (int i=0;i<MAX_CLOACK_LEVEL;i++)
			{
				if ( g_BloodCastle.CheckEnterLevel(lpObj->m_Index, i) == FALSE )
				{
					iITEM_LEVEL = i;
				}
			}
		}

		if ( g_BloodCastle.GetCurrentState(iITEM_LEVEL-1) == BC_STATE_CLOSED && g_BloodCastle.CheckCanEnter(iITEM_LEVEL-1) != false )
		{
			PMSG_TALKRESULT pMsg;

			pMsg.h.c = 0xC3;
			pMsg.h.headcode = 0x30;
			pMsg.h.size = sizeof(pMsg);
			pMsg.result = 0x06;

			gSendProto.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);

			lpObj->m_IfState.use = 1;
			lpObj->m_IfState.type = 12;
		}
		else
		{
			GCServerCmd(lpObj->m_Index, 1, 0x14, 0);
		}
	}
	else
	{
		GCServerCmd(lpObj->m_Index, 1, 0x15, 0);
	}

	return FALSE;
}
// ----------------------------------------------------------------------------------------------------------------------
bool NpcRolensiaMadam(LPOBJ lpNpc, LPOBJ lpObj)
{
	return FALSE;
}
// ----------------------------------------------------------------------------------------------------------------------
bool NpcDeviasWeapon(LPOBJ lpNpc, LPOBJ lpObj)
{
	return FALSE;
}
// ----------------------------------------------------------------------------------------------------------------------
bool NpcDarkSpiritTrainer(LPOBJ lpNpc, LPOBJ lpObj)
{
	if ( bCanChaosBox == TRUE )
	{
		PMSG_TALKRESULT pMsg;

		pMsg.h.c = 0xC3;
		pMsg.h.headcode = 0x30;
		pMsg.h.size = sizeof(pMsg);
		
		if ( lpObj->m_bPShopOpen == true )
		{
			CLog.LogAdd("[%s][%s] is Already Opening PShop, ChaosBox Failed",
				lpObj->AccountID, lpObj->Name);
			GCServerMsgStringSend(lMsg.Get(MSGGET(4, 194)), lpObj->m_Index, 1);

			return TRUE;
		}

		lpObj->m_IfState.type = 13;
		lpObj->m_IfState.state = 0;
		lpObj->m_IfState.use = 1;
		
		pMsg.result = 0x07;
		lpObj->bIsChaosMixCompleted = false;
		pMsg.level1 = g_DevilSquare.DS1ChaosSuccessRate;
		pMsg.level2 = g_DevilSquare.DS2ChaosSuccessRate;
		pMsg.level3 = g_DevilSquare.DS3ChaosSuccessRate;
		pMsg.level4 = g_DevilSquare.DS4ChaosSuccessRate;
		pMsg.level5 = g_DevilSquare.DS5ChaosSuccessRate;
		pMsg.level6 = g_DevilSquare.DS6ChaosSuccessRate;
		pMsg.level7 = g_DevilSquare.DS7ChaosSuccessRate;

		gSendProto.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);

		GCAnsCsMapSvrTaxInfo(lpObj->m_Index, 1, g_CastleSiegeSync.GetTaxRateChaos(lpObj->m_Index));
		gObjInventoryTrans(lpObj->m_Index);

		CLog.LogAdd("[%s][%s] Open Chaos Box", lpObj->AccountID, lpObj->Name);
		gObjItemTextSave(lpObj);
		gObjWarehouseTextSave(lpObj);
	}

	return TRUE;
}
// ----------------------------------------------------------------------------------------------------------------------
bool NpcJewelMixDealer(LPOBJ lpNpc, LPOBJ lpObj)
{
	if ( bCanChaosBox == TRUE )
	{
		if ( (lpObj->m_IfState.use) > 0 )
		{
			return TRUE;
		}

		PMSG_TALKRESULT pMsg;

		pMsg.h.c = 0xC3;
		pMsg.h.headcode = 0x30;
		pMsg.h.size = sizeof(pMsg);
		pMsg.result = 0x09;

		gSendProto.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);

		lpObj->m_IfState.use = 1;
		lpObj->m_IfState.type = 12;

		CLog.LogAdd("[JewelMix] [%s][%s] Jewel Mix Dealer Start", lpObj->AccountID, lpObj->Name);

		gObjItemTextSave(lpObj);
		gObjWarehouseTextSave(lpObj);

	}

	return TRUE;
}
// ----------------------------------------------------------------------------------------------------------------------
struct PMSG_ANS_USE_WEAPON_INTERFACE
{
	PBMSG_HEAD2 h;
	BYTE btResult;
	BYTE btWeaponType;
	BYTE btObjIndexH;
	BYTE btObjIndexL;
};
// ----------------------------------------------------------------------------------------------------------------------
bool NpcSiegeMachine_Attack(LPOBJ lpNpc, LPOBJ lpObj)
{
	if ( (lpObj->m_IfState.use) > 0 )
	{
		return TRUE;
	}

	PMSG_ANS_USE_WEAPON_INTERFACE pResult = {0};

	C1SubHeadSet((LPBYTE)&pResult, 0xB7, 0x00, sizeof(pResult));

	pResult.btResult = 0;
	pResult.btWeaponType = 1;
	pResult.btObjIndexH = SET_NUMBERH(lpNpc->m_Index);
	pResult.btObjIndexL = SET_NUMBERL(lpNpc->m_Index);

	if ( gObjIsConnectedGP(lpNpc->m_iWeaponUser) == FALSE )
	{
		lpNpc->m_iWeaponUser = 0;
		lpNpc->m_btWeaponState = 1;
	}

	lpNpc->m_btWeaponState = 1;


	if ( lpNpc->m_btWeaponState == 1 )
	{
		pResult.btResult = 1;
		lpNpc->m_btWeaponState = 2;
		lpNpc->m_iWeaponUser = lpObj->m_Index;

		gSendProto.DataSend(lpObj->m_Index, (LPBYTE)&pResult, pResult.h.size);

		lpObj->m_IfState.use = 1;
		lpObj->m_IfState.type = 15;
	}
	else
	{
		gSendProto.DataSend(lpObj->m_Index, (LPBYTE)&pResult, pResult.h.size);
	}

	return TRUE;
}
// ----------------------------------------------------------------------------------------------------------------------
bool NpcSiegeMachine_Defense(LPOBJ lpNpc, LPOBJ lpObj)
{
	if ( (lpObj->m_IfState.use ) > 0 )
	{
		return TRUE;
	}

	PMSG_ANS_USE_WEAPON_INTERFACE pResult = {0};

	C1SubHeadSet((LPBYTE)&pResult, 0xB7, 0x00, sizeof(pResult));

	pResult.btResult = 0;
	pResult.btWeaponType = 2;
	pResult.btObjIndexH = SET_NUMBERH(lpNpc->m_Index);
	pResult.btObjIndexL = SET_NUMBERL(lpNpc->m_Index);

	if ( gObjIsConnectedGP(lpNpc->m_iWeaponUser) == FALSE )
	{
		lpNpc->m_iWeaponUser = 0;
		lpNpc->m_btWeaponState = 1;
	}

	lpNpc->m_btWeaponState = 1;


	if ( lpNpc->m_btWeaponState == 1 )
	{
		pResult.btResult = 1;
		lpNpc->m_btWeaponState = 2;
		lpNpc->m_iWeaponUser = lpObj->m_Index;

		gSendProto.DataSend(lpObj->m_Index, (LPBYTE)&pResult, pResult.h.size);

		lpObj->m_IfState.use = 1;
		lpObj->m_IfState.type = 16;
	}
	else
	{
		gSendProto.DataSend(lpObj->m_Index, (LPBYTE)&pResult, pResult.h.size);
	}

	return TRUE;
}
// ----------------------------------------------------------------------------------------------------------------------
bool NpcElderCircle(LPOBJ lpNpc, LPOBJ lpObj)
{
#if(GS_CASTLE)
	if ( (lpObj->m_IfState.use ) > 0 )
	{
		return TRUE;
	}

	if(g_CastleSiege.GetCastleState() == 7)
	{
		::ChatTargetSend(lpNpc,lMsg.Get(0x655),lpObj->m_Index);
		return TRUE;
	}

	if(strcmp(lpObj->GuildName,"") == 0)
	{
		::ChatTargetSend(lpNpc,lMsg.Get(0x656),lpObj->m_Index);
		return TRUE;
	}

	if(!g_CastleSiege.CheckGuildOwnCastle(lpObj->GuildName)
		|| (lpObj->GuildStatus != 128 && lpObj->GuildStatus != 64) )
	{
		::ChatTargetSend(lpNpc,lMsg.Get(0x656),lpObj->m_Index);
		return TRUE;
	}

	PMSG_TALKRESULT pResult;

	pResult.h.c = 0xC3;
	pResult.h.headcode = 0x30;
	pResult.h.size = sizeof(pResult);
	pResult.result = 0x0C;

	lpObj->m_IfState.use = 1;
	lpObj->m_IfState.type = 12;

	pResult.level1 = 0;
	pResult.level2 = 0;
	pResult.level3 = 0;
	pResult.level4 = 0;
	pResult.level5 = 0;
	pResult.level6 = 0;
	
	if(bCanChaosBox == TRUE)
	{
		lpObj->m_IfState.type = 7;
		lpObj->bIsChaosMixCompleted = FALSE;
		lpObj->m_bIsCastleNPCUpgradeCompleted = FALSE;
		pResult.level1 = 1;

		::GCAnsCsMapSvrTaxInfo(lpObj->m_Index,TRUE,g_CastleSiegeSync.GetTaxRateChaos(lpObj->m_Index));
		
		gObjInventoryTrans(lpObj->m_Index);
		
		CLog.LogAdd("[%s][%s] Open Chaos Box",lpObj->AccountID,lpObj->Name);
	
		gObjItemTextSave(lpObj);
		gObjWarehouseTextSave(lpObj);
	}

	gSendProto.DataSend(lpObj->m_Index, (LPBYTE)&pResult, pResult.h.size);
#endif
	return TRUE;
}
// ----------------------------------------------------------------------------------------------------------------------
bool NpcCastleGuard(LPOBJ lpNpc, LPOBJ lpObj)
{
	if ( (lpObj->m_IfState.use ) > 0 )
	{
		return TRUE;
	}

#if(GS_CASTLE)
	if(g_CastleSiege.GetCastleState() == 7)
	{
		ChatTargetSend(lpNpc,lMsg.Get(0x657),lpObj->m_Index);
		return TRUE;
	}
#endif

	PMSG_TALKRESULT pMsg;

	pMsg.h.c = 0xC3;
	pMsg.h.headcode = 0x30;
	pMsg.h.size = sizeof(pMsg);
	pMsg.result = 0x0D;

	gSendProto.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);

	lpObj->m_IfState.use = 1;
	lpObj->m_IfState.type = 12;


	return TRUE;
}
// ----------------------------------------------------------------------------------------------------------------------
bool NpcCastleGateLever(LPOBJ lpNpc, LPOBJ lpObj)
{
	if ( (lpObj->m_IfState.use) > 0 )
	{
		return TRUE;
	}
#if(GS_CASTLE)

	BOOL bControlEnable = 0;
	BYTE btResult = 0;
	int iGateIndex = -1;
	
	if(g_CastleSiege.GetCastleState() == 7)
	{
		if(lpObj->m_btCsJoinSide != 1)
		{
			btResult = 4;
			bControlEnable = FALSE;
		}
		else
		{
			bControlEnable = TRUE;
		}
	}
	else{
		if(g_CastleSiege.CheckCastleOwnerMember(lpObj->m_Index) == FALSE)
		{
			if(g_CastleSiege.CheckCastleOwnerUnionMember(lpObj->m_Index) == FALSE)
			{
				btResult = 4;
				bControlEnable = FALSE;
			}
		}
		else
		{
			bControlEnable = TRUE;
		}
	}

	if(bControlEnable != FALSE)
	{
		if(g_CastleSiege.CheckLeverAlive(lpNpc->m_Index) == TRUE)
		{
			if(g_CastleSiege.CheckCsGateAlive(lpObj->m_iCsGateLeverLinkIndex) == TRUE)
			{
				btResult = 1;
				iGateIndex = lpNpc->m_iCsGateLeverLinkIndex;
			}
			else
			{
				btResult = 2;
			}
		}
		else
		{
			btResult = 3;
		}
	}

	::GCAnsCsGateState(lpObj->m_Index,btResult,iGateIndex);

	if(btResult != 1)
	{
		return TRUE;
	}

	lpObj->m_IfState.use = 1;
	lpObj->m_IfState.type = 12;

#endif

	return TRUE;
}
// ----------------------------------------------------------------------------------------------------------------------
bool NpcCastleCrown(LPOBJ lpNpc, LPOBJ lpObj)
{
#if(GS_CASTLE)
	int iUserIndex;

	if(!gObjIsConnected(lpObj->m_Index))
		return TRUE;

	if( g_CastleSiege.GetCastleState() != 7 )
		return TRUE;
	
	if( (lpObj->m_btCsJoinSide < 2) || (lpObj->m_bCsGuildInvolved == FALSE) )
		return TRUE;

	if(g_CastleSiege.CheckUnionGuildMaster(lpObj->m_Index) == FALSE)
		return TRUE;

	if( abs(lpNpc->Y - lpObj->Y) > 3 ||
		abs(lpNpc->X - lpObj->X) > 3 )
		return TRUE;

	if(g_CastleSiege.GetRegCrownAvailable() == FALSE)
		return TRUE;

	iUserIndex = g_CastleSiege.GetCrownUserIndex();

	if(gObjIsConnected(iUserIndex) == FALSE)
	{
		int iCrownSwitchIndex1,iCrownSwitchIndex2;//Não é o nome da variável
		iCrownSwitchIndex1 = g_CastleSiege.GetCrownSwitchUserIndex(217);
		iCrownSwitchIndex2 = g_CastleSiege.GetCrownSwitchUserIndex(218);
		
		if(gObjIsConnected(iCrownSwitchIndex1) == FALSE ||
			gObjIsConnected(iCrownSwitchIndex2) == FALSE)
		{
			::GCAnsCsAccessCrownState(lpObj->m_Index,4);
			CLog.LogAdd("[CastleSiege] [%s][%s] Failed to Register Castle Crown (GUILD:%s)",
				lpObj->AccountID,lpObj->Name,lpObj->GuildName);
			return TRUE;
		}

		if(lpObj->m_btCsJoinSide != gObj[iCrownSwitchIndex1].m_btCsJoinSide ||
			lpObj->m_btCsJoinSide != gObj[iCrownSwitchIndex2].m_btCsJoinSide)
		{
			::GCAnsCsAccessCrownState(lpObj->m_Index,4);
			CLog.LogAdd("[CastleSiege] [%s][%s] Failed to Register Castle Crown (GUILD:%s) (S1:%s/%s)(S2:%s/%s)",
				lpObj->AccountID,lpObj->Name,lpObj->GuildName,gObj[iCrownSwitchIndex1].Name,gObj[iCrownSwitchIndex1].GuildName,
				gObj[iCrownSwitchIndex2].Name,gObj[iCrownSwitchIndex2].GuildName);
			return TRUE;
		}

		::GCAnsCsAccessCrownState(lpObj->m_Index,0);
		g_CastleSiege.SetCrownUserIndex(lpObj->m_Index);
		g_CastleSiege.SetCrownAccessUserX(lpObj->X);
		g_CastleSiege.SetCrownAccessUserY(lpObj->Y);
		g_CastleSiege.SetCrownAccessTickCount();
		g_CastleSiege.NotifyAllUserCsProgState(0,lpObj->GuildName);
		
		CLog.LogAdd("[CastleSiege] [%s][%s] Start to Register Castle Crown (GUILD:%s)",lpObj->AccountID,lpObj->Name,
			lpObj->GuildName);
	}
	else
	{	
		::GCAnsCsAccessCrownState(lpObj->m_Index,3);
	}
#endif
	return TRUE;
}
// ----------------------------------------------------------------------------------------------------------------------
bool NpcCastleSwitch(LPOBJ lpNpc, LPOBJ lpObj)
{
#if(GS_CASTLE)
	int iUserIndex;
	
	if(!gObjIsConnected(lpObj->m_Index))
		return TRUE;
	
	if(g_CastleSiege.GetCastleState() != 7)
		return TRUE;

	if(lpObj->m_btCsJoinSide < 2 )
		return TRUE;

	if( (abs(lpObj->Y - lpNpc->Y) >= 3) ||
		(abs(lpObj->X - lpNpc->X) > 3) )
	{
		return TRUE;
	}

	if(g_CastleSiege.CheckGuardianStatueExist() == TRUE)
	{
		MsgOutput(lpObj->m_Index,lMsg.Get(0x6c8));
		return TRUE;
	}

	iUserIndex = g_CastleSiege.GetCrownSwitchUserIndex(lpNpc->Class);

	if(gObjIsConnected(iUserIndex) == 0)
	{
		::GCAnsCsAccessSwitchState(lpObj->m_Index,lpNpc->m_Index,-1,TRUE);

		g_CastleSiege.SetCrownSwitchUserIndex(lpNpc->Class,lpObj->m_Index);

		CLog.LogAdd("[CastleSiege] [%s][%s] Start to Push Castle Crown Switch (GUILD:%s) - CS X:%d/Y:%d",lpObj->AccountID,
			lpObj->Name,lpObj->GuildName,lpNpc->X,lpNpc->Y);
	}
	else
	{
		if(lpObj->m_Index != iUserIndex)
		{
			::GCAnsCsAccessSwitchState(lpObj->m_Index,lpNpc->m_Index,iUserIndex,2);
		}
	}	
#endif
	return TRUE;
}
// ----------------------------------------------------------------------------------------------------------------------
bool NpcShadowPhantom(LPOBJ lpNpc, LPOBJ lpObj)
{
	BYTE QuestNPCPacket[12] = { 0xC1, 0x0C, 0xF9, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}; 
	gSendProto.DataSend(lpObj->m_Index,QuestNPCPacket,QuestNPCPacket[1]);
	// ----
	return TRUE;
}
// ----------------------------------------------------------------------------------------------------------------------
#if(GS_CASTLE)
bool NpcCastleHuntZoneGuard(LPOBJ lpNpc,LPOBJ lpObj)
{
	if(!gObjIsConnected(lpObj->m_Index))
		return TRUE;

	if ( (lpObj->m_IfState.use ) > 0 )
	{
		return TRUE;
	}



	PMSG_ANS_GUARD_IN_CASTLE_HUNTZONE pResult = {0};

	C1SubHeadSet((LPBYTE)&pResult,0xB9,0x03,sizeof(PMSG_ANS_GUARD_IN_CASTLE_HUNTZONE));
	pResult.btResult = TRUE;
	pResult.iMaxPrice = 300000;
	pResult.iUnitOfPrice = 10000;
	pResult.btUsable = 0;
	pResult.iCurrentPrice = 0;

	pResult.iCurrentPrice = g_CastleSiegeSync.GetTaxHuntZone(lpObj->m_Index,FALSE);

	if(g_CastleSiege.GetHuntZoneEnter())
	{
		pResult.btUsable = TRUE;
	}

	if(lpObj->lpGuild != NULL)
	{
		if(g_CastleSiege.CheckCastleOwnerUnionMember(lpObj->m_Index))
		{
			pResult.btResult = 2;
		}

		if(g_CastleSiege.CheckCastleOwnerMember(lpObj->m_Index))
		{
			if(lpObj->GuildStatus == 128)
			{
				pResult.btResult = 3;
			}
		}
	}
	else
	{
		if(g_CastleSiege.GetHuntZoneEnter())
		{
			pResult.btResult = TRUE;
		}
	}

	gSendProto.DataSend(lpObj->m_Index,(LPBYTE)&pResult,pResult.h.size);

	return TRUE;
}
#endif
// ----------------------------------------------------------------------------------------------------------------------
bool NpcEliteSoldier(LPOBJ lpNpc, LPOBJ lpObj)
{
	return FALSE;
}
// ----------------------------------------------------------------------------------------------------------------------
bool NpcDrinkSellerHillary(LPOBJ lpNpc, LPOBJ lpObj)
{
	return FALSE;
}
// ----------------------------------------------------------------------------------------------------------------------
bool NpcDrinkSellerLindsay(LPOBJ lpNpc, LPOBJ lpObj)
{
	return FALSE;
}
// ----------------------------------------------------------------------------------------------------------------------
bool NpcFireworksGirl(LPOBJ lpNpc, LPOBJ lpObj)
{
	return FALSE;
}
// ----------------------------------------------------------------------------------------------------------------------
bool NpcEleganceAllen(LPOBJ lpNpc, LPOBJ lpObj)
{
	return FALSE;
}
// ----------------------------------------------------------------------------------------------------------------------
bool NpcSilvia(LPOBJ lpNpc, LPOBJ lpObj)
{
	return FALSE;
}
// ----------------------------------------------------------------------------------------------------------------------
bool NpcLeah(LPOBJ lpNpc, LPOBJ lpObj)
{
	return FALSE;
}
// ----------------------------------------------------------------------------------------------------------------------
bool NpcMarseille(LPOBJ lpNpc, LPOBJ lpObj)
{
	return FALSE;
}
// ----------------------------------------------------------------------------------------------------------------------
bool NpcMoss(LPOBJ lpNpc, LPOBJ lpObj)
{
	if ( gMossMerchant.Enabled == false )
	{
		return true;
	}

	if ( gMossMerchant.Running == false )
	{
		return true;
	}
	BYTE Send2[6] = {0xC3,0x06,0x30,0x00,0x27,0x00};								 // SHOP WINDOW
			BYTE Send[71] = {0xC2,0x00,71,0x31,0x00,5,                                       // Head
							0x00,71,0x00,0x01,0x00,0x00,13*16,0x00,0xFF,0xFF,0xFF,0xFF,0xFF, // ITEM1
							0x02,72,0x00,0x01,0x00,0x00,13*16,0x00,0xFF,0xFF,0xFF,0xFF,0xFF, // ITEM2
							0x04,73,0x00,0x01,0x00,0x00,13*16,0x00,0xFF,0xFF,0xFF,0xFF,0xFF, // ITEM3
							0x06,74,0x00,0x01,0x00,0x00,13*16,0x00,0xFF,0xFF,0xFF,0xFF,0xFF, // ITEM4
							0x18,75,0x00,0x01,0x00,0x00,13*16,0x00,0xFF,0xFF,0xFF,0xFF,0xFF  // ITEM5
							};

							gSendProto.DataSend(lpObj->m_Index,Send2,6);
							gSendProto.DataSend(lpObj->m_Index,Send,71);

							lpObj->TargetShopNumber = 492;
							lpObj->m_IfState.use    = 1;
							lpObj->m_IfState.type   = 3;

	return true;
}
// ----------------------------------------------------------------------------------------------------------------------
bool NpcSeedMaster(LPOBJ lpNpc, LPOBJ lpObj)
{

	if(lpObj->m_IfState.use != 0)
	{
		return TRUE;
	}

	lpObj->m_IfState.use = 1;
	lpObj->m_IfState.type = 7;

	PMSG_TALKRESULT pMsg;

	pMsg.h.c = 0xC3;
	pMsg.h.headcode = 0x30;
	pMsg.h.size = 9;
	pMsg.result = 0x17;

	gSendProto.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);

	return TRUE;

}
// ----------------------------------------------------------------------------------------------------------------------
bool NpcSeedResearch(LPOBJ lpNpc, LPOBJ lpObj)
{

	if ( (lpObj->m_IfState.use ) > 0 )
	{
		return TRUE;
	}

	lpObj->m_IfState.use = 1;
	lpObj->m_IfState.type = 7;

	PMSG_TALKRESULT pMsg;

	pMsg.h.c = 0xC3;
	pMsg.h.headcode = 0x30;
	pMsg.h.size = 9 ;
	pMsg.result = 0x18;

	gSendProto.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);

	return TRUE;

}
// ----------------------------------------------------------------------------------------------------------------------
bool NpcTitusGatekeeper(LPOBJ lpNpc, LPOBJ lpObj)
{
	if ( (lpObj->m_IfState.use ) > 0 )
	{
		return TRUE;
	}

	lpObj->m_IfState.use = 1;
	lpObj->m_IfState.type = 20;

	PMSG_TALKRESULT pMsg;

	pMsg.h.c = 0xC3;
	pMsg.h.headcode = 0x30;
	pMsg.h.size = 9;
	pMsg.result = 0x21;

	gSendProto.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);

	gDuel.SendDuelStatus(lpObj);
	
	return TRUE;

}
// ----------------------------------------------------------------------------------------------------------------------
bool NpcMarketUnionJulia(LPOBJ lpNpc, LPOBJ lpObj)
{
	if ( lpObj->MapNumber != 79 )
	{
		BYTE NewPack[5]={0xC3,0x0D,0x30, 0x25, 0x00}; // From Lorencia to Loren Market
		gSendProto.DataSend (lpObj->m_Index,NewPack , NewPack[1]);
		return TRUE;
	}
	else
	{
		BYTE NewPack[5]={0xC3,0x0D,0x30, 0x25, 0x01}; // From Loren Market to Lorencia
		gSendProto.DataSend (lpObj->m_Index,NewPack , NewPack[1]);
		return TRUE;
	}
	return FALSE;
}
// ----------------------------------------------------------------------------------------------------------------------
bool NpcVanertGen (LPOBJ lpNpc, LPOBJ lpObj)
{
	lpObj->QuestNPC = 544;
	lpObj->GenNPC = 544;
	BYTE NewPack[12]={0xC1,0x0C,0xF9,0x01,0x20,0x02,0x00,0x00,0x00,0x00,0x00};
	gSendProto.DataSend (lpObj->m_Index, NewPack, NewPack[1]);
	return true;
}
// ----------------------------------------------------------------------------------------------------------------------
bool NpcDuprianGen (LPOBJ lpNpc, LPOBJ lpObj)
{
	lpObj->QuestNPC = 543;
	lpObj->GenNPC = 543;
	BYTE NewPack[12]={0xC1,0x0C,0xF9,0x01,0x1F,0x02,0x00,0x00,0x00,0x00,0x00};
	gSendProto.DataSend (lpObj->m_Index, NewPack, NewPack[1]);
	return true;
}
// ----------------------------------------------------------------------------------------------------------------------
bool NpcImperialFort (LPOBJ lpNpc, LPOBJ lpObj)
{
	PMSG_TALKRESULT pMsg;

	pMsg.h.c = 0xC3;
	pMsg.h.size = 0x0A;
	pMsg.h.headcode = 0x30;
	pMsg.result = 0x24;
	// ----
	gSendProto.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
	// ----
	return true;
}
// ----------------------------------------------------------------------------------------------------------------------
bool NpcDoubleGoer (LPOBJ lpNpc, LPOBJ lpObj)
{
#if(!GS_CASTLE)
	BYTE DGPacket[5] = {0xC3,0x05,0x30,0x23,0x00}; // Double Goer Event NPC Able
	// ----
	if ( gDoubleGoer.Running == 1 )
	{
		if ( gDoubleGoer.Timer < 60 )
		{
			DGPacket[4] = 1;
		}
		else
		{
			DGPacket[4] = gDoubleGoer.Timer/60;
		}
	}
	// ----
	gSendProto.DataSend (lpObj->m_Index, DGPacket, DGPacket[1]);
#endif
	return true;
}
// ----------------------------------------------------------------------------------------------------------------------
bool NpcCherryBlossom (LPOBJ lpNpc, LPOBJ lpObj)
{
	if(lpObj->m_IfState.use != 0)
	{
		return true;
	}

	lpObj->m_IfState.use = 1;
	lpObj->m_IfState.type = 7;

	PMSG_TALKRESULT pMsg;

	pMsg.h.c = 0xC3;
	pMsg.h.size = 0x09;
	pMsg.h.headcode = 0x30;
	pMsg.result = 0x16;
	
	gSendProto.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);

	return true;
}
// ----------------------------------------------------------------------------------------------------------------------
bool NpcGyro (LPOBJ lpNpc, LPOBJ lpObj)
{
	NPC_QUEST_MSG pMsg;

	pMsg.c = 0xC1;
	pMsg.size = 0x0C;
	pMsg.headcode = 0xF9;
	pMsg.Unk1 = 0x01;
	pMsg.NumberL = LOBYTE(lpNpc->Class);
	pMsg.NumberH = HIBYTE(lpNpc->Class);
	// ----
	gSendProto.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.size);
	// ----
	return true;
}
// ----------------------------------------------------------------------------------------------------------------------
bool NpcOracle (LPOBJ lpNpc, LPOBJ lpObj)
{
	NPC_QUEST_MSG pMsg;

	pMsg.c = 0xC1;
	pMsg.size = 0x0C;
	pMsg.headcode = 0xF9;
	pMsg.Unk1 = 0x01;
	pMsg.NumberL = LOBYTE(lpNpc->Class);
	pMsg.NumberH = HIBYTE(lpNpc->Class);
	// ----
	gSendProto.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.size);
	// ----
	return true;
}
// ----------------------------------------------------------------------------------------------------------------------
bool NpcMercenary (LPOBJ lpNpc, LPOBJ lpObj)
{
	NPC_QUEST_MSG pMsg;

	pMsg.c = 0xC1;
	pMsg.size = 0x0C;
	pMsg.headcode = 0xF9;
	pMsg.Unk1 = 0x01;
	pMsg.NumberL = LOBYTE(lpNpc->Class);
	pMsg.NumberH = HIBYTE(lpNpc->Class);
	// ----
	gSendProto.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.size);
	// ----
	return true;
}

bool NpcSnowMan (LPOBJ lpNpc, LPOBJ lpObj)
{
	//BYTE NewPack[5]={0xC3,0x0D,0x30, 0x19, 0x00}; // From Loren Market to Lorencia
	//DataSend (lpObj->m_Index,NewPack , NewPack[1]);
	return true;
}
/*
//---- Illusion Temple
if(lpObj->m_IfState.use != 0)
	{
		return true;
	}

	PMSG_TALKRESULT pMsg;

	pMsg.h.c = 0xC3;
	pMsg.h.headcode = 0x30;
	pMsg.h.size = 9 ;
	pMsg.result = 0x14;

	gSendProto.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);

	return true;


	// ---- Re Initialization Helpeer
	if(lpObj->m_IfState.use != 0)
	{
		return true;
	}

	PMSG_TALKRESULT pMsg;

	pMsg.h.c = 0xC3;
	pMsg.h.headcode = 0x30;
	pMsg.h.size = 9 ;
	pMsg.result = 0x19;

	gSendProto.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);

	return true;


	// ---- Delgano NPC
	if(lpObj->m_IfState.use != 0)
	{
		return true;
	}

	PMSG_TALKRESULT pMsg;

	pMsg.h.c = 0xC3;
	pMsg.h.headcode = 0x30;
	pMsg.h.size = 9 ;
	pMsg.result = 0x20;

	gSendProto.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);

	return true;

	// ---- Merchant
	if(lpObj->m_IfState.use != 0)
	{
		return true;
	}

	PMSG_TALKRESULT pMsg;

	pMsg.h.c = 0xC3;
	pMsg.h.headcode = 0x30;
	pMsg.h.size = 9 ;
	pMsg.result = 0x22;

	gSendProto.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);

	return true;

	// ---- Lucky Smelting
	if(lpObj->m_IfState.use != 0)
	{
		return true;
	}

	PMSG_TALKRESULT pMsg;

	pMsg.h.c = 0xC3;
	pMsg.h.headcode = 0x30;
	pMsg.h.size = 9 ;
	pMsg.result = 0x26;

	gSendProto.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);

	return true;
*/