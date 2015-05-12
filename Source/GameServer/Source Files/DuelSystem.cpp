//---------------------------------------------------------------------------
// # Project:		HaRiBO MU GameServer - Supported Season 6
// # Company:		RealCoderZ MU Development © 2011
// # Description:	Duel System
// # Developed By:	M.E.S & WolF
//---------------------------------------------------------------------------
#include "../Header Files/stdafx.h"
#include "../Header Files/DuelSystem.h"
#include "../Header Files/LogProc.H"
#include "../Header Files/Notice.H"
#include "../Header Files/Functions.h"
#include "../Header Files/Gate.h"
#include "../Header Files/MoveSystem.h"
#include "../Header Files/SendProto.H"


CDuelSystem gDuel;

CDuelSystem::CDuelSystem(void)
{
	// ----
	gDuel.Init();
}

CDuelSystem::~CDuelSystem(void)
{
	// ----
}
// Done 100%
void CDuelSystem::Init()
{
	// ----
	gDuel.DuelEnabled = 1;
	gDuel.DuelMinLevel = 1;
	gDuel.DuelMaxLevel = 400;
	gDuel.NeedMoneyToDuel = 0;
	gDuel.MoneyToDuel = 0;
	// ----
	for (int i=0;i<MAX_DUEL_ROOMS;i++)
	{
		gDuel.m_Rooms[i].bFree = true;
		gDuel.m_Rooms[i].bWaiting = false;
		gDuel.m_Rooms[i].bHasWinner = false;
		gDuel.m_Rooms[i].lpObj01 = NULL;
		gDuel.m_Rooms[i].btPoints01 = 0;
		gDuel.m_Rooms[i].lpObj02 = NULL;
		gDuel.m_Rooms[i].btPoints02 = 0;
		gDuel.m_Rooms[i].dwTickCount = 0;
		gDuel.m_Rooms[i].dwStartTime = 0;
	}
	// ----
	gDuel.m_Gates[0].UserGate01 = 295;
	gDuel.m_Gates[0].UserGate02 = 296;
	gDuel.m_Gates[0].LearnerGate = 303;
	gDuel.m_Gates[1].UserGate01 = 297;
	gDuel.m_Gates[1].UserGate02 = 298;
	gDuel.m_Gates[1].LearnerGate = 304;
	gDuel.m_Gates[2].UserGate01 = 299;
	gDuel.m_Gates[2].UserGate02 = 300;
	gDuel.m_Gates[2].LearnerGate = 305;
	gDuel.m_Gates[3].UserGate01 = 301;
	gDuel.m_Gates[3].UserGate02 = 302;
	gDuel.m_Gates[3].LearnerGate = 306;
	// ----
	gDuel.m_Respawn[0].LowX = 89;
	gDuel.m_Respawn[0].LowY = 62;
	gDuel.m_Respawn[0].HighX = 110;
	gDuel.m_Respawn[0].HighY = 81;
	gDuel.m_Respawn[1].LowX = 89;
	gDuel.m_Respawn[1].LowY = 113;
	gDuel.m_Respawn[1].HighX = 110;
	gDuel.m_Respawn[1].HighY = 130;
	gDuel.m_Respawn[2].LowX = 141;
	gDuel.m_Respawn[2].LowY = 63;
	gDuel.m_Respawn[2].HighX = 163;
	gDuel.m_Respawn[2].HighY = 82;
	gDuel.m_Respawn[3].LowX = 142;
	gDuel.m_Respawn[3].LowY = 111;
	gDuel.m_Respawn[3].HighX = 163;
	gDuel.m_Respawn[3].HighY = 131;
}

void CDuelSystem::Run()
{
	for(int i = 0; i < MAX_DUEL_ROOMS; i++)
	{
		if( gDuel.m_Rooms[i].bFree == false )
		{
			if( GetTickCount() - gDuel.m_Rooms[i].dwStartTime < 5000)
			{
				continue;
			}
			// ----
			for(int u = 0; u < MAX_DUEL_LEARNERS; u++)
			{
				if( gDuel.m_Rooms[i].lpLearners[u] == NULL )
				{
					continue;
				}
				// ----
				if( gObjIsConnected(gDuel.m_Rooms[i].lpLearners[u]) == true )
				{
					if( gDuel.m_Rooms[i].lpLearners[u]->MapNumber != MAP_INDEX_VULCANROOM )
					{
						gDuel.SendDuelEnd(gDuel.m_Rooms[i].lpLearners[u]);
						GCStateInfoSend(gDuel.m_Rooms[i].lpLearners[u], 0, eVS_INVISIBLE);
						GCStateInfoSend(gDuel.m_Rooms[i].lpLearners[u], 0, eVS_TRANSPARENCY);
						GCStateInfoSend(gDuel.m_Rooms[i].lpLearners[u], 0, eVS_DUEL_INTERFACE);
						gDuel.SendSpectatorRemove(u, i);
						gDuel.m_Rooms[i].lpLearners[u] = NULL;
					}
				}
				else
				{
					gDuel.m_Rooms[i].lpLearners[u] = NULL;
					gDuel.SendSpectatorList(i);
				}
			}

			if( gDuel.m_Rooms[i].lpObj01 != NULL && gDuel.m_Rooms[i].lpObj02 != NULL )
			{
				if( gObjIsConnected(gDuel.m_Rooms[i].lpObj01) == false || gObjIsConnected(gDuel.m_Rooms[i].lpObj02) == false )
				{
					gDuel.RoomReset(i,false,false);
					continue;
				}
				else if ( (gDuel.m_Rooms[i].lpObj01->MapNumber != MAP_INDEX_VULCANROOM || gDuel.m_Rooms[i].lpObj02->MapNumber != MAP_INDEX_VULCANROOM) && gDuel.m_Rooms[i].bHasWinner == false )
				{
					gDuel.RoomReset(i,false,false);
					continue;
				}
				else if ( gDuel.m_Rooms[i].lpObj01->Connected < PLAYER_PLAYING || gDuel.m_Rooms[i].lpObj02->Connected < PLAYER_PLAYING )
				{
					gDuel.RoomReset(i,false,false);
					continue;
				}
			}
			else
			{
				gDuel.RoomReset(i,false,false);
				continue;
			}

			if( gDuel.m_UpdateLifebarTime < GetTickCount() )
			{
				gDuel.SendLifebarStatus(i);
				gDuel.m_UpdateLifebarTime = GetTickCount() + 2000;
			}

			if( gDuel.m_Rooms[i].dwTickCount != 0 )
			{
				if( gDuel.m_Rooms[i].dwTickCount < GetTickCount() )
				{
					gDuel.RoomReset(i,false,false);
				}
			}
		}
	}

	if( gDuel.m_UpdateTickCount < GetTickCount() )
	{
		for(int i = OBJ_STARTUSERINDEX; i < OBJMAX; i++)
		{
			if((gObj[i].m_IfState.use) && gObj[i].m_IfState.type == 20)
			{
				gDuel.SendDuelStatus(&gObj[i]);
			}
		}
		this->m_UpdateTickCount = GetTickCount() + 6000;
	}

	return;
}
// Done 100%
void CDuelSystem::DuelProtocolManager(unsigned char * aRecv,int aIndex)
{
	switch ( aRecv[2] )
	{
	case 0xAA:
		switch ( aRecv[3] )
		{
		case 0x01:
			gDuel.DuelRequest(aRecv,aIndex);
			break;
		case 0x02:
			gDuel.DuelRequestAnswer(aRecv,aIndex);
			break;
		case 0x07:
			gDuel.DuelRequestSpectator(aRecv,aIndex);
			break;
		case 0x09:
			gDuel.DuelExitSpectator((int)aRecv[4],aIndex);
			break;
		}
		break;
	case 0xAB:
		break;
	case 0xAC:
		break;
	}
}
// Done 100%
void CDuelSystem::DuelRequestSpectator(unsigned char * aRecv,int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];
	// ----
	int Room = aRecv[4];
	// ----
	if( Room >= 0 && Room < MAX_DUEL_ROOMS)
	{
		if( gDuel.m_Rooms[Room].bFree == true ) 
		{
			SendMsg.MessageOut(aIndex, 0x01, "[Duel]: There is no Duel in curse in Room %d",Room+1);
			return;
		}
		for(int i = 0; i < MAX_DUEL_LEARNERS; i++)
		{
			if( gDuel.m_Rooms[Room].lpLearners[i] == NULL)
			{
				int mgt = gDuel.m_Gates[Room].LearnerGate;
				short x,y,level;
				BYTE map = lpObj->MapNumber,dir;
				int result = GateC.GetGate(mgt,(BYTE &)x,(BYTE &)y,(BYTE &)map,(BYTE &)dir,(short &)level);
				gObjTeleport(aIndex,map,x,y);
				gDuel.m_Rooms[Room].lpLearners[i] = lpObj;
				gDuel.SendSpectatorList(lpObj, Room);
				gDuel.SendSpectatorAdd(i, Room);
				SendMsg.MessageOut(aIndex, 0x01, "[Duel]: You are now in Dueling Room %d",(Room+1));
				GCStateInfoSend(lpObj, 1, eVS_INVISIBLE,TRUE,TRUE);
				GCStateInfoSend(lpObj, 1, eVS_TRANSPARENCY);
				GCStateInfoSend(lpObj, 1, eVS_DUEL_INTERFACE);
				gDuel.SendLifebarInit(lpObj, Room);
				gDuel.SendLifebarStatus(lpObj, Room);
				gDuel.UpdateDuelScore(Room);
				return;
			}
		}
		SendMsg.MessageOut(aIndex, 0x01, "[Duel]: Room %d is full. Try later",Room+1);
		return;
	}
}
// Done 100%
void CDuelSystem::DuelExitSpectator(int Room, int aIndex)
{
	gObjMoveGate(aIndex, 294);
}
// Done 100%
void CDuelSystem::DuelRequest(unsigned char * aRecv,int aIndex)
{
	if ( gDuel.DuelEnabled == false )
	{
		GCServerMsgStringSend("[DuelSystem]: Duel has been disabled",aIndex,1);
		return;
	}
	int aTargetIndex = aRecv[4] * 256 + aRecv[5];
	// ----
	if ( OBJMAX_RANGE(aTargetIndex) == FALSE )
	{
		GCServerMsgStringSend("[DuelSystem] Target Index out of range",aIndex,1);
		return;
	}
	// ----
	if ( aTargetIndex == aIndex )
	{
		GCServerMsgStringSend("[DuelSystem] Same index",aIndex,1);
		return;
	}
	// ----
	LPOBJ lpObj = &gObj[aIndex];
	LPOBJ lpTargetObj = &gObj[aTargetIndex];
	// ----
	if ( gDuel.CanDuel(lpObj,lpTargetObj) == false )
	{
		return;
	}

	PMSG_DUEL_QUESTION_START pMsg;

	pMsg.h.c = 0xC1;
	pMsg.h.headcode = 0xAA;
	pMsg.h.subcode = 0x02;
	pMsg.h.size = sizeof(pMsg);
	pMsg.NumberH = SET_NUMBERH(lpObj->m_Index);
	pMsg.NumberL = SET_NUMBERL(lpObj->m_Index);
	memcpy(pMsg.szName, lpObj->Name, sizeof(pMsg.szName));

	gSendProto.DataSend(aTargetIndex, (LPBYTE)&pMsg, pMsg.h.size);

	GCServerMsgStringSend("[DuelSystem]: Duel Requested",aIndex,1);

	return;
}
// Done 100%
void CDuelSystem::DuelRequestAnswer(unsigned char * aRecv,int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];
	// ----
	int iDuelIndex = -1;
	int iDuelRoom = lpObj->m_iDuelRoom;
	// ----
	PMSG_DUEL_START pMsgSend;
	
	pMsgSend.h.c = 0xC1;
	pMsgSend.h.headcode = 0xAA;
	pMsgSend.h.subcode = 0x01;
	pMsgSend.bDuelStart = 0;
	pMsgSend.h.size = sizeof(pMsgSend);
	// ----
	int DuelResult = aRecv[4];
	// ----
	if ( iDuelRoom < 0 || iDuelRoom > MAX_DUEL_ROOMS-1 )
	{
		GCServerMsgStringSend("[DuelSystem] Duel Room Error",aIndex,1);
		return;
	}
	// ----
	if ( lpObj->CloseType != -1 )
	{
		GCServerMsgStringSend("[DuelSystem] Error, Closing Account",aIndex,1);
		return;
	}
	// ----
	iDuelIndex = aRecv[5]*256 + aRecv[6];
	// ----
	if ( !OBJMAX_RANGE(iDuelIndex) )
	{
		GCServerMsgStringSend("[DuelSystem] Duel Index Error",aIndex,1);
		return;
	}
	// ----
	if ( iDuelIndex == lpObj->m_Index )
	{
		GCServerMsgStringSend("[DuelSystem] Error Same User Index",aIndex,1);
		return;
	}
	// ----
	LPOBJ lpTargetObj = &gObj[iDuelIndex];
	// ----
	if ( gObjIsConnected(lpTargetObj) == false )
	{
		GCServerMsgStringSend("[DuelSystem] Error Duel Player Disconnected",aIndex,1);
		return;
	}
	// ----
	if ( lpTargetObj->Type != OBJ_USER )
	{
		GCServerMsgStringSend("[DuelSystem] Error Duel Index is not user",aIndex,1);
		return;
	}
	// ----
	if ( lpTargetObj->CloseCount >= 0 )
	{
		GCServerMsgStringSend("[DuelSystem] Error Duel Index is Closing Accout",aIndex,1);
		return;
	}
	// ----
	if ( lpTargetObj->m_iDuelUserReserved == lpObj->m_Index )
	{
		if ( DuelResult == 0 )
		{
			GCServerMsgStringSend("[DuelSystem] Duel canceled",aIndex,1);
			gDuel.RoomReset(iDuelRoom, true, true);
			pMsgSend.bDuelStart = 0x0F;
			memcpy(pMsgSend.szName, lpObj->Name, sizeof(pMsgSend.szName));
			gSendProto.DataSend(iDuelIndex, (LPBYTE)&pMsgSend, pMsgSend.h.size);
		}
		else if ( DuelResult == 1 )
		{
			GCServerMsgStringSend("[DuelSystem] Duel Acepted",aIndex,1);
			if ( gDuel.NeedMoneyToDuel == true )
			{
				if ( lpObj->Money < gDuel.MoneyToDuel )
				{
					gDuel.SendDuelEnd(lpObj);
					gDuel.RoomReset(iDuelRoom,true,false);
					pMsgSend.bDuelStart = 0x0e;
					pMsgSend.h.size = sizeof(pMsgSend);
					gSendProto.DataSend(lpObj->m_Index, (BYTE*)&pMsgSend, pMsgSend.h.size);
					return;
				}
				else if ( lpTargetObj->Money < gDuel.MoneyToDuel )
				{
					gDuel.SendDuelEnd(lpTargetObj);
					gDuel.RoomReset(iDuelRoom,true,false);
					pMsgSend.bDuelStart = 0x0e;
					pMsgSend.h.size = sizeof(pMsgSend);
					gSendProto.DataSend(lpTargetObj->m_Index, (BYTE*)&pMsgSend, pMsgSend.h.size);
					return;
				}
			}
			if( gObjMoveGate(lpObj->m_Index, gDuel.m_Gates[iDuelRoom].UserGate01) == false )
			{
				GCServerMsgStringSend("[DuelSystem] Error Move Gate 1",aIndex,1);
				this->RoomReset(iDuelRoom, true, true);
				return;
			}
			if( gObjMoveGate(iDuelIndex, gDuel.m_Gates[iDuelRoom].UserGate02) == false )
			{
				GCServerMsgStringSend("[DuelSystem] Error Move Gate 2",aIndex,1);
				this->RoomReset(iDuelRoom, true, true);
				return;
			}
			// ----
			if ( gDuel.NeedMoneyToDuel == true )
			{
				lpObj->Money -= gDuel.MoneyToDuel;
				GCMoneySend(lpObj->m_Index,lpObj->Money);
				// ----
				lpTargetObj->Money -= gDuel.MoneyToDuel;
				GCMoneySend(lpTargetObj->m_Index,lpTargetObj->Money);
			}
			// ----
			lpTargetObj->m_iDuelUserReserved = -1;
			lpTargetObj->m_iDuelUserRequested = -1;
			lpTargetObj->m_btDuelScore = 0;
			lpTargetObj->m_iDuelUser = lpObj->m_Index;
			// ----
			lpObj->m_iDuelUserRequested = -1;
			lpObj->m_iDuelUserReserved = -1;
			lpObj->m_btDuelScore = 0;
			lpObj->m_iDuelUser = iDuelIndex;
			// ----
			gDuel.m_Rooms[iDuelRoom].dwStartTime = GetTickCount();
			gDuel.m_Rooms[iDuelRoom].dwTickCount = GetTickCount() + (DUEL_TIME * 60000);
			// ----
			gDuel.m_Rooms[iDuelRoom].bFree = false;
			gDuel.m_Rooms[iDuelRoom].bWaiting = false;
			gDuel.m_Rooms[iDuelRoom].bHasWinner = false;
			// ----
			pMsgSend.bDuelStart = 0;
			pMsgSend.NumberH = SET_NUMBERH(iDuelIndex);
			pMsgSend.NumberL = SET_NUMBERL(iDuelIndex);
			memcpy(pMsgSend.szName, lpTargetObj->Name, sizeof(pMsgSend.szName));

			gSendProto.DataSend(lpObj->m_Index, (LPBYTE)&pMsgSend, pMsgSend.h.size);

			pMsgSend.NumberH = SET_NUMBERH(lpObj->m_Index);
			pMsgSend.NumberL = SET_NUMBERL(lpObj->m_Index);
			memcpy(pMsgSend.szName, lpObj->Name, sizeof(pMsgSend.szName));

			gSendProto.DataSend(iDuelIndex, (LPBYTE)&pMsgSend, pMsgSend.h.size);

			SendMsg.MessageOutAll(0x00, "Duel Started: %s vs %s on Room %d",lpObj->Name,lpTargetObj->Name,iDuelRoom+1);
		}
	}
	else
	{
		gDuel.RoomReset(iDuelRoom,false,false);
		gSendProto.DataSend(iDuelIndex, (LPBYTE)&pMsgSend, pMsgSend.h.size);
		return;
	}
}
// Done 100%
void CDuelSystem::UpdateDuelScore(int iRoom)
{
	if( iRoom < 0 || iRoom > MAX_DUEL_ROOMS - 1 )
	{
		return;
	}
	// ----
	if( gDuel.m_Rooms[iRoom].lpObj01 == NULL || gDuel.m_Rooms[iRoom].lpObj02 == NULL)
	{
		return;
	}
	// ----
	int aIndex1 = gDuel.m_Rooms[iRoom].lpObj01->m_Index;
	int aIndex2 = gDuel.m_Rooms[iRoom].lpObj02->m_Index;
	// ----
	if ( !OBJMAX_RANGE(aIndex1) || !OBJMAX_RANGE(aIndex2) )
	{
		return;
	}
	// ----
	if ( gObjIsConnected(gDuel.m_Rooms[iRoom].lpObj01) == false || gObjIsConnected(gDuel.m_Rooms[iRoom].lpObj02) == false )
	{
		return;
	}
	// ----
	if ( gObj[aIndex1].Type != OBJ_USER || gObj[aIndex2].Type != OBJ_USER )
	{
		return;
	}
	// ----
	PMSG_DUEL_SCORE pMsg;

	pMsg.h.c = 0xC1;
	pMsg.h.size = sizeof(pMsg);
	pMsg.h.headcode = 0xAA;
	pMsg.h.subcode = 0x04;
	pMsg.NumberH1 = SET_NUMBERH(aIndex1);
	pMsg.NumberL1 = SET_NUMBERL(aIndex1);
	pMsg.NumberH2 = SET_NUMBERH(aIndex2);
	pMsg.NumberL2 = SET_NUMBERL(aIndex2);
	pMsg.btDuelScore1 = gDuel.m_Rooms[iRoom].btPoints01;
	pMsg.btDuelScore2 = gDuel.m_Rooms[iRoom].btPoints02;

	gSendProto.DataSend(aIndex1, (LPBYTE)&pMsg, pMsg.h.size);
	gSendProto.DataSend(aIndex2, (LPBYTE)&pMsg, pMsg.h.size);

	for(int i = 0; i < MAX_DUEL_LEARNERS; i++)
	{
		if( gDuel.m_Rooms[iRoom].lpLearners[i] == NULL ) 
		{
			continue;
		}
		gSendProto.DataSend(gDuel.m_Rooms[iRoom].lpLearners[i]->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
	}
}

void CDuelSystem::DuelCheckEnd(LPOBJ lpObj,LPOBJ lpTargetObj)
{
	int iRoom = lpObj->m_iDuelRoom;
	// ----
	if ( gDuel.m_Rooms[iRoom].lpObj01 == lpObj )
	{
		if ( gDuel.m_Rooms[iRoom].btPoints01 >= 10 )
		{
			GCStateInfoSend(lpObj,1,103);
			lpObj->GladiatorsGloryTime = 3600;
			gDuel.m_Rooms[iRoom].bHasWinner = true;
		}
	}
	else if ( gDuel.m_Rooms[iRoom].lpObj02 == lpObj )
	{
		if ( gDuel.m_Rooms[iRoom].btPoints02 >= 10 )
		{
			GCStateInfoSend(lpObj,1,103);
			lpObj->GladiatorsGloryTime = 3600;
			gDuel.m_Rooms[iRoom].bHasWinner = true;
		}
	}
}

int CDuelSystem::GetSpectatorCount(int iRoom)
{
	if( iRoom < 0 || iRoom >= MAX_DUEL_ROOMS ) 
	{
		return -1;
	}

	int iCount = 0;
	for(int i = 0; i < MAX_DUEL_LEARNERS; i++)
	{
		if( gDuel.m_Rooms[iRoom].lpLearners[i] != NULL ) 
		{
			iCount++;
		}
	}

	return iCount;
}

void CDuelSystem::SendDuelStatus(LPOBJ lpObj)
{
	PMSG_DUEL_STATUS pMsg;

	pMsg.h.c = 0xC1;
	pMsg.h.size = sizeof(pMsg);
	pMsg.h.headcode = 0xAA;
	pMsg.h.subcode = 0x06;

	ZeroMemory(&pMsg.pRoomStatus, sizeof(pMsg.pRoomStatus));
	
	for(int i = 0; i < MAX_DUEL_ROOMS; i++)
	{
		if( gDuel.m_Rooms[i].bFree == false )
		{
			if( gDuel.m_Rooms[i].lpObj01 == NULL || gDuel.m_Rooms[i].lpObj02 == NULL )
			{
				continue;
			}

			pMsg.pRoomStatus[i].btDuelRunning = TRUE;

			int iSpecCount = gDuel.GetSpectatorCount(i);
			// ----
			if( iSpecCount < 0 || iSpecCount >= 10 )
			{
				pMsg.pRoomStatus[i].btDuelOpen = FALSE;
			}
			else
			{
				pMsg.pRoomStatus[i].btDuelOpen = TRUE;
			}
			memcpy(&pMsg.pRoomStatus[i].szName1[0], &gDuel.m_Rooms[i].lpObj01->Name[0], 10);
			memcpy(&pMsg.pRoomStatus[i].szName2[0], &gDuel.m_Rooms[i].lpObj02->Name[0], 10);
		}

	}

	gSendProto.DataSend(lpObj->m_Index, (BYTE*)&pMsg, pMsg.h.size);
}

void CDuelSystem::SendLifebarInit(LPOBJ lpObj,int iRoom)
{
	if(iRoom < 0 || iRoom >= MAX_DUEL_ROOMS)
	{
		return;
	}

	if( gDuel.m_Rooms[iRoom].lpObj01 == NULL || gDuel.m_Rooms[iRoom].lpObj02 == NULL)
	{
		return;
	}

	PMSG_DUEL_LIFEBAR_NAME pMsg2;
	pMsg2.h.c = 0xC1;
	pMsg2.h.size = sizeof(pMsg2);
	pMsg2.h.headcode = 0xAA;
	pMsg2.h.subcode = 0x07;
	pMsg2.Type = 0x00;
	
	memcpy(pMsg2.szName1, gDuel.m_Rooms[iRoom].lpObj01->Name, 10);
	memcpy(pMsg2.szName2, gDuel.m_Rooms[iRoom].lpObj02->Name, 10);

	pMsg2.btObjId1H = HIBYTE(gDuel.m_Rooms[iRoom].lpObj01->m_Index);
	pMsg2.btObjId1L = LOBYTE(gDuel.m_Rooms[iRoom].lpObj01->m_Index);

	pMsg2.btObjId2H = HIBYTE(gDuel.m_Rooms[iRoom].lpObj02->m_Index);
	pMsg2.btObjId2L = LOBYTE(gDuel.m_Rooms[iRoom].lpObj02->m_Index);

	gSendProto.DataSend(lpObj->m_Index, (BYTE*)&pMsg2, pMsg2.h.size);

	PMSG_DUEL_LIFEBAR_INIT pMsg;
	pMsg.h.c = 0xC1;
	pMsg.h.size = sizeof(pMsg);
	pMsg.h.headcode = 0xAA;
	pMsg.h.subcode = 0x0D;

	gSendProto.DataSend(lpObj->m_Index, (BYTE*)&pMsg, pMsg.h.size);
}

void CDuelSystem::SendLifebarStatus(int iRoom)
{
	if( iRoom < 0 || iRoom >= MAX_DUEL_ROOMS )
	{
		return;
	}

	if( gDuel.m_Rooms[iRoom].lpObj01 == NULL || gDuel.m_Rooms[iRoom].lpObj02 == NULL )
	{
		return;
	}

	if( gDuel.m_Rooms[iRoom].bFree == false )
	{
		for(int u = 0; u < MAX_DUEL_LEARNERS; u++)
		{
			if( gDuel.m_Rooms[iRoom].lpLearners[u] != NULL )
			{
				gDuel.SendLifebarStatus(gDuel.m_Rooms[iRoom].lpLearners[u], iRoom);
			}
		}
	}
}

void CDuelSystem::SendLifebarStatus(LPOBJ lpObj, int iRoom)
{
	if( iRoom < 0 || iRoom >= MAX_DUEL_ROOMS )
	{
		return;
	}

	if( gDuel.m_Rooms[iRoom].lpObj01 == NULL || gDuel.m_Rooms[iRoom].lpObj02 == NULL )
	{
		return;
	}

	PMSG_DUEL_LIFEBAR_REFILL pMsg;
	pMsg.h.c = 0xC1;
	pMsg.h.size = sizeof(pMsg);
	pMsg.h.headcode = 0xAA;
	pMsg.h.subcode = 0x05;

	pMsg.btObjId01H = HIBYTE(gDuel.m_Rooms[iRoom].lpObj01->m_Index);
	pMsg.btObjId01L = LOBYTE(gDuel.m_Rooms[iRoom].lpObj01->m_Index);

	pMsg.btObjId02H = HIBYTE(gDuel.m_Rooms[iRoom].lpObj02->m_Index);
	pMsg.btObjId02L = LOBYTE(gDuel.m_Rooms[iRoom].lpObj02->m_Index);

	pMsg.btLife01 = gDuel.m_Rooms[iRoom].lpObj01->Life / ((gDuel.m_Rooms[iRoom].lpObj01->MaxLife + gDuel.m_Rooms[iRoom].lpObj01->AddLife) / 100);
	pMsg.btLife02 = gDuel.m_Rooms[iRoom].lpObj02->Life / ((gDuel.m_Rooms[iRoom].lpObj02->MaxLife + gDuel.m_Rooms[iRoom].lpObj02->AddLife) / 100);

	pMsg.btShield01 = gDuel.m_Rooms[iRoom].lpObj01->iShield / ((gDuel.m_Rooms[iRoom].lpObj01->iMaxShield + gDuel.m_Rooms[iRoom].lpObj01->iAddShield) / 100);
	pMsg.btShield02 = gDuel.m_Rooms[iRoom].lpObj02->iShield / ((gDuel.m_Rooms[iRoom].lpObj02->iMaxShield + gDuel.m_Rooms[iRoom].lpObj02->iAddShield) / 100);

	gSendProto.DataSend(lpObj->m_Index, (BYTE*)&pMsg, pMsg.h.size);
}

void CDuelSystem::SendSpectatorList(int iRoom)
{
	if( iRoom < 0 || iRoom >= MAX_DUEL_ROOMS )
	{
		return;
	}

	if( gDuel.m_Rooms[iRoom].bFree == false )
	{
		for(int u = 0; u < MAX_DUEL_LEARNERS; u++)
		{
			if( gDuel.m_Rooms[iRoom].lpLearners[u] != NULL )
			{
				gDuel.SendSpectatorList(gDuel.m_Rooms[iRoom].lpLearners[u], iRoom);
			}
		}
	}
}

void CDuelSystem::SendSpectatorList(LPOBJ lpObj, int iRoom)
{
	if( iRoom < 0 || iRoom >= MAX_DUEL_ROOMS )
	{
		return;
	}

	if( gDuel.m_Rooms[iRoom].lpObj01 == NULL || gDuel.m_Rooms[iRoom].lpObj02 == NULL )
	{
		return;
	}

	PMSG_DUEL_SPEC_LIST pMsg;
	pMsg.h.c = 0xC1;
	pMsg.h.headcode = 0xAA;
	pMsg.h.subcode = 0x0B;
	pMsg.btCount = 0;

	if( gDuel.m_Rooms[iRoom].bFree == false )
	{
		for(int u = 0; u < MAX_DUEL_LEARNERS; u++)
		{
			if( gDuel.m_Rooms[iRoom].lpLearners[u] != NULL )
			{
				memcpy(&pMsg.szName[pMsg.btCount++][0], gDuel.m_Rooms[iRoom].lpLearners[u]->Name, 10);
			}
		}
	}

	pMsg.h.size = 5 + (pMsg.btCount * 10);

	gSendProto.DataSend(lpObj->m_Index, (BYTE*)&pMsg, pMsg.h.size);
}

void CDuelSystem::SendSpectatorAdd(int iSpecIndex, int iRoom)
{
	if(iRoom < 0 || iRoom >= MAX_DUEL_ROOMS)
	{
		return;
	}

	if( iSpecIndex < 0 || iSpecIndex >= MAX_DUEL_LEARNERS )
	{
		return;
	}

	if( gDuel.m_Rooms[iRoom].lpLearners[iSpecIndex] == NULL) return;

	if( gDuel.m_Rooms[iRoom].lpObj01 == NULL || gDuel.m_Rooms[iRoom].lpObj02 == NULL )
	{
		return;
	}

	PMSG_DUEL_SPEC_ADD pMsg;
	pMsg.h.c = 0xC1;
	pMsg.h.size = sizeof(pMsg);
	pMsg.h.headcode = 0xAA;
	pMsg.h.subcode = 0x08;
	memcpy(pMsg.szName, gDuel.m_Rooms[iRoom].lpLearners[iSpecIndex]->Name, 10);

	for(int i = 0; i < MAX_DUEL_LEARNERS; i++)
	{
		if(i == iSpecIndex) continue;
		if(gDuel.m_Rooms[iRoom].lpLearners[i] != NULL)
		{
			gSendProto.DataSend(gDuel.m_Rooms[iRoom].lpLearners[i]->m_Index, (BYTE*)&pMsg, pMsg.h.size);
		}
	}
}

void CDuelSystem::SendSpectatorRemove(int iSpecIndex, int iRoom)
{
	if( iRoom < 0 || iRoom >= MAX_DUEL_ROOMS )
	{
		return;
	}

	if( iSpecIndex < 0 || iSpecIndex >= MAX_DUEL_LEARNERS )
	{
		return;
	}

	if( gDuel.m_Rooms[iRoom].lpLearners[iSpecIndex] == NULL )
	{
		return;
	}

	if( gDuel.m_Rooms[iRoom].lpObj01 == NULL || gDuel.m_Rooms[iRoom].lpObj02 == NULL )
	{
		return;
	}

	PMSG_DUEL_SPEC_ADD_DEL pMsg;
	pMsg.h.c = 0xC1;
	pMsg.h.size = sizeof(pMsg);
	pMsg.h.headcode = 0xAA;
	pMsg.h.subcode = 0x0A;
	memcpy(pMsg.szName, gDuel.m_Rooms[iRoom].lpLearners[iSpecIndex]->Name, 10);

	for(int i = 0; i < MAX_DUEL_LEARNERS; i++)
	{
		if ( i == iSpecIndex )
		{
			continue;
		}
		if( gDuel.m_Rooms[iRoom].lpLearners[i] != NULL )
		{
			gSendProto.DataSend(gDuel.m_Rooms[iRoom].lpLearners[i]->m_Index, (BYTE*)&pMsg, pMsg.h.size);
		}
	}
}

void CDuelSystem::SendDuelEnd(LPOBJ lpObj)
{
	if( lpObj == NULL )
	{
		return;
	}
	// ----
	if( gObjIsConnected(lpObj) == false )
	{
		return;
	}
	// ----
	BYTE lpMsgClose[5] = { 0xC1, 0x05, 0xAA, 0x03, 0x00 } ;

	gSendProto.DataSend(lpObj->m_Index, &lpMsgClose[0], lpMsgClose[1]);
}
// Done 100%
int CDuelSystem::GetFreeRoom()
{
	for (int i=0;i<MAX_DUEL_ROOMS;i++)
	{
		if ( gDuel.m_Rooms[i].bFree == true && gDuel.m_Rooms[i].bWaiting == false )
		{
			return i;
		}
	}
	return -1;
}
// Done 100%
int CDuelSystem::GetUserDuelRoom(LPOBJ lpObj)
{
	for (int i=0;i<MAX_DUEL_ROOMS;i++)
	{
		if ( gDuel.m_Rooms[i].lpObj01 == lpObj || gDuel.m_Rooms[i].lpObj02 == lpObj )
		{
			return i;
		}
	}
	return -1;
}

void CDuelSystem::RoomReset(int iRoom, bool dontmove, bool dontsendend)
{
	if ( gDuel.m_Rooms[iRoom].bHasWinner == true )
	{
		if ( gDuel.m_Rooms[iRoom].btPoints01 >= 10 )
		{
			SendMsg.MessageOutAll(0x00, "[Duel] %s [%d] defeated %s [%d] in Room %d",gDuel.m_Rooms[iRoom].lpObj01->Name,gDuel.m_Rooms[iRoom].btPoints01,gDuel.m_Rooms[iRoom].lpObj02->Name,gDuel.m_Rooms[iRoom].btPoints02,iRoom+1);
		}
		else if ( gDuel.m_Rooms[iRoom].btPoints02 >= 10 )
		{
			SendMsg.MessageOutAll(0x00, "[Duel] %s [%d] defeated %s [%d] in Room %d",gDuel.m_Rooms[iRoom].lpObj02->Name,gDuel.m_Rooms[iRoom].btPoints02,gDuel.m_Rooms[iRoom].lpObj01->Name,gDuel.m_Rooms[iRoom].btPoints01,iRoom+1);
		}
	}
	// ----
	if( gDuel.m_Rooms[iRoom].lpObj01 != NULL )
	{
		if( gObjIsConnected(gDuel.m_Rooms[iRoom].lpObj01) == true )
		{
			if( dontsendend == false )
			{
				gDuel.SendDuelEnd(gDuel.m_Rooms[iRoom].lpObj01);
			}
			if( gDuel.m_Rooms[iRoom].lpObj01->MapNumber == MAP_INDEX_VULCANROOM )
			{
				if( dontmove == false )
				{
					gObjMoveGate(gDuel.m_Rooms[iRoom].lpObj01->m_Index, 294);
				}
			}
		}
		gDuel.UserDuelReset(gDuel.m_Rooms[iRoom].lpObj01);
	}

	if( gDuel.m_Rooms[iRoom].lpObj02 != NULL )
	{
		if( gObjIsConnected(gDuel.m_Rooms[iRoom].lpObj02) == true )
		{
			if( dontsendend == false )
			{
				gDuel.SendDuelEnd(gDuel.m_Rooms[iRoom].lpObj02);
			}
			if( gDuel.m_Rooms[iRoom].lpObj02->MapNumber == MAP_INDEX_VULCANROOM )
			{
				if( dontmove == false )
				{
					gObjMoveGate(gDuel.m_Rooms[iRoom].lpObj02->m_Index, 294);
				}
			}
		}
		gDuel.UserDuelReset(gDuel.m_Rooms[iRoom].lpObj02);
	}

	for(int i = 0; i < MAX_DUEL_LEARNERS; i++)
	{
		if( gDuel.m_Rooms[iRoom].lpLearners[i] != NULL )
		{
			gDuel.SendDuelEnd(gDuel.m_Rooms[iRoom].lpLearners[i]);
			GCStateInfoSend(gDuel.m_Rooms[iRoom].lpLearners[i], 0, eVS_INVISIBLE);
			GCStateInfoSend(gDuel.m_Rooms[iRoom].lpLearners[i], 0, eVS_TRANSPARENCY);
			GCStateInfoSend(gDuel.m_Rooms[iRoom].lpLearners[i], 0, eVS_DUEL_INTERFACE);
			gObjMoveGate(gDuel.m_Rooms[iRoom].lpLearners[i]->m_Index, 294);
		}
		gDuel.m_Rooms[iRoom].lpLearners[i] = NULL;
	}

	gDuel.m_Rooms[iRoom].lpObj01 = NULL;
	gDuel.m_Rooms[iRoom].btPoints01 = 0;

	gDuel.m_Rooms[iRoom].lpObj02 = NULL;
	gDuel.m_Rooms[iRoom].btPoints02 = 0;

	gDuel.m_Rooms[iRoom].bFree = true;
	gDuel.m_Rooms[iRoom].bWaiting = false;
	gDuel.m_Rooms[iRoom].bHasWinner = false;

	gDuel.m_Rooms[iRoom].dwTickCount = 0;
	gDuel.m_Rooms[iRoom].dwStartTime = 0;

	//CLog.LogAdd("[Duel Manager] Room Reset - Number: %d", iRoom + 1);
}

void CDuelSystem::UserDuelReset(LPOBJ lpObj)
{
	if ( lpObj == NULL ) 
	{	
		return;
	}
	// ----
	lpObj->m_iDuelRoom = -1;
	lpObj->m_iDuelUser = -1;
	lpObj->m_iDuelUserRequested = -1;
	lpObj->m_iDuelUserReserved = -1;
	lpObj->m_btDuelScore = 0;
}

void CDuelSystem::DuelReset(LPOBJ lpObj)
{
	int iDuelIndex = lpObj->m_iDuelUser;

	if ( OBJMAX_RANGE(iDuelIndex) )
	{
		lpObj->m_iDuelUser = -1;
		lpObj->m_iDuelUserReserved = -1;
		lpObj->m_btDuelScore = 0;
		lpObj->m_iDuelTickCount = 0;
		//gDarkSpirit[lpObj->m_Index].ReSetTarget(iDuelIndex);
		//gDarkSpirit[iDuelIndex].ReSetTarget(lpObj->m_Index);
		gObj[iDuelIndex].m_iDuelUser = -1;
		gObj[iDuelIndex].m_iDuelUserReserved = -1;
		gObj[iDuelIndex].m_iDuelUserRequested = -1;
		gObj[iDuelIndex].m_iDuelTickCount = 0;
	}
}
// Done 100%
void CDuelSystem::MoveDeadUser(LPOBJ lpObj)
{
	int Room = gDuel.GetUserDuelRoom(lpObj);
	short X;
	short Y;
	short FinalX;
	short FinalY;
	// ----
	if ( Room != -1 )
	{
		X = gDuel.m_Respawn[Room].HighX - gDuel.m_Respawn[Room].LowX;
		FinalX = (rand()%X ) + gDuel.m_Respawn[Room].LowX;
		// ----
		Y = gDuel.m_Respawn[Room].HighY - gDuel.m_Respawn[Room].LowY;
		FinalY = (rand()%Y ) + gDuel.m_Respawn[Room].LowY;
		// ----
		lpObj->X = FinalX;
		lpObj->Y = FinalY;
	}
	return;
}
// ---- Conditions Check
// Done 100%
bool CDuelSystem::IsOnDuel(LPOBJ lpObj)
{
	bool bResult = false;
	// ----
	if ( lpObj->Type == OBJ_USER )
	{
		int iDuelIndex = lpObj->m_iDuelUser;
		// ----
		if ( OBJMAX_RANGE(iDuelIndex) )
		{
			bResult = true;
		}
	}
	return bResult;
}
// Done 100%
bool CDuelSystem::AreOnDuel(LPOBJ lpObj,LPOBJ lpTargetObj)
{
	bool bResult = false;

	if ( lpObj->Type == OBJ_USER && lpTargetObj->Type == OBJ_USER )
	{
		if ( lpObj->m_iDuelUser == lpTargetObj->m_Index && lpTargetObj->m_iDuelUser == lpObj->m_Index )
		{
			bResult = true;
		}
	}

	return bResult;
}
// Done 100%
bool CDuelSystem::CanDuel(LPOBJ lpObj,LPOBJ lpTargetObj)
{
	// ---- Check Index Range
	if ( OBJMAX_RANGE(lpObj->m_Index) == FALSE || OBJMAX_RANGE(lpTargetObj->m_Index) == FALSE )
	{
		return false;
	}
	// ---- Check Index Connected
	if ( gObjIsConnected(lpObj) == false || gObjIsConnected(lpTargetObj) == false )
	{
		return false;
	}
	// ---- Check Index is User
	if ( lpObj->Type != OBJ_USER || lpTargetObj->Type != OBJ_USER )
	{
		return false;
	}
	// ---- Check Player is on Duel
	if ( gDuel.IsOnDuel(lpObj) == true )
	{
		SendMsg.MessageOut(lpObj->m_Index, 0x01, "[Duel]: You are currently in duel");
		return false;
	}
	// ---- Check Player is on Duel
	if ( lpObj->m_iDuelUserRequested != -1 )
	{
		SendMsg.MessageOut(lpObj->m_Index, 0x01, "[Duel]: You are currently in duel");
		return false;
	}
	// ---- Check Player is on Duel
	if ( lpObj->m_iDuelUserReserved != -1 )
	{
		SendMsg.MessageOut(lpObj->m_Index, 0x01, "[Duel]: You are currently in duel");
		return false;
	}
	// ---- Check Opponent is on Duel
	if ( gDuel.IsOnDuel(lpTargetObj) == true )
	{
		SendMsg.MessageOut(lpObj->m_Index, 0x01, "[Duel]: Opponent is currently in duel");
		return false;
	}
	// ---- Check Opponent is on Duel
	if ( lpTargetObj->m_iDuelUserRequested != -1 )
	{
		SendMsg.MessageOut(lpObj->m_Index, 0x01, "[Duel]: Opponent is currently in duel");
		return false;
	}
	// ---- Check Opponent is on Duel
	if ( lpTargetObj->m_iDuelUserReserved != -1 )
	{
		SendMsg.MessageOut(lpObj->m_Index, 0x01, "[Duel]: Opponent is currently in duel");
		return false;
	}
	// ---- Players Level Check
	if ( gDuel.PlayersLevelCheck(lpObj,lpTargetObj) == false )
	{
		return false;
	}
	// ---- Players Money Check
	if ( gDuel.PlayersMoneyCheck(lpObj,lpTargetObj) == false )
	{
		return false;
	}
	// ----
	int DuelRoom = gDuel.GetFreeRoom();
	// ----
	if ( DuelRoom == -1 )
	{
		GCServerMsgStringSend("[Duel]: No room available for Duel",lpObj->m_Index,1);
		return false;
	}

	if ( lpObj->lpGuild && lpObj->lpGuild->WarState == 1 )
	{
		GCServerMsgStringSend("[Duel]: You can't make a Duel while you are in a Guild War",lpObj->m_Index,1);
		return false;
	}

	if ( lpTargetObj->lpGuild && lpTargetObj->lpGuild->WarState == 1 )
	{
		GCServerMsgStringSend("[Duel]: You can't make a Duel while your opponent is in a Guild War",lpObj->m_Index,1);
		return false;
	}

	for (int n=0;n<MAX_SELF_DEFENSE;n++)
	{
		if ( lpObj->SelfDefense[n] >= 0 || lpTargetObj->SelfDefense[n] >= 0 )
		{
			GCServerMsgStringSend("[Duel]: You can't make Duel while you have active selfdefense",lpObj->m_Index,1);
			return false;
		}
	}

	// ----
	lpObj->m_iDuelUser = -1;
	lpObj->m_iDuelUserReserved = lpTargetObj->m_Index;
	lpTargetObj->m_iDuelUserRequested = lpObj->m_Index;
	lpObj->m_iDuelRoom = DuelRoom;
	lpTargetObj->m_iDuelRoom = DuelRoom;
	// ----
	gDuel.m_Rooms[DuelRoom].lpObj01 = lpObj;
	gDuel.m_Rooms[DuelRoom].lpObj02 = lpTargetObj;
	gDuel.m_Rooms[DuelRoom].bWaiting = true;
	// ----
	return true;
}
// Done 100%
bool CDuelSystem::PlayersLevelCheck(LPOBJ lpObj,LPOBJ lpTargetObj)
{
	if ( lpObj->Level < gDuel.DuelMinLevel || lpObj->Level > gDuel.DuelMaxLevel )
	{
		SendMsg.MessageOut(lpObj->m_Index, 0x01, "[Duel]: You don't have required level to make duel");
		SendMsg.MessageOut(lpObj->m_Index, 0x01, "[Duel]: Min level to make duel: %d",gDuel.DuelMinLevel);
		SendMsg.MessageOut(lpObj->m_Index, 0x01, "[Duel]: Max level to make duel: %d",gDuel.DuelMaxLevel);
		return false;
	}
	else if ( lpTargetObj->Level < gDuel.DuelMinLevel || lpTargetObj->Level > gDuel.DuelMaxLevel )
	{
		SendMsg.MessageOut(lpObj->m_Index, 0x01, "[Duel]: Opponent doesn't have required level to make duel");
		SendMsg.MessageOut(lpObj->m_Index, 0x01, "[Duel]: Min level to make duel: %d",gDuel.DuelMinLevel);
		SendMsg.MessageOut(lpObj->m_Index, 0x01, "[Duel]: Max level to make duel: %d",gDuel.DuelMaxLevel);
		return false;
	}
	// ----
	return true;
}
// Done 100%
bool CDuelSystem::PlayersMoneyCheck(LPOBJ lpObj,LPOBJ lpTargetObj)
{
	if ( gDuel.NeedMoneyToDuel == true )
	{
		if ( lpObj->Money < gDuel.MoneyToDuel )
		{
			SendMsg.MessageOut(lpObj->m_Index, 0x01, "[Duel]: You don't have enought zen to make duel");
			SendMsg.MessageOut(lpObj->m_Index, 0x01, "[Duel]: Need %d zen to make duel",gDuel.MoneyToDuel);
			return false;
		}
		else if ( lpTargetObj->Money < gDuel.MoneyToDuel )
		{
			SendMsg.MessageOut(lpObj->m_Index, 0x01, "[Duel]: Opponent doesn't have enought zen to make duel");
			SendMsg.MessageOut(lpObj->m_Index, 0x01, "[Duel]: Need %d zen to make duel",gDuel.MoneyToDuel);
			return false;
		}
	}
	return true;
}