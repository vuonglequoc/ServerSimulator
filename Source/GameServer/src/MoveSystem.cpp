//---------------------------------------------------------------------------
// # Project:		HaRiBO MU GameServer - Supported Season 6
// # Company:		RealCoderZ MU Development � 2011
// # Description:	Move System
// # Developed By:	WolF & M.E.S
//---------------------------------------------------------------------------
#include "stdafx.h"
#include "MoveSystem.h"
#include "Gate.h"
#include "WzMemScript.h"
#include "Readscript.h"
#include "User.h"
#include "Notice.H"
#include "GameMain.h"
#include "zzzitem.h"
#include "MapClass.h"
#include "Defines.h"
#include "LogProc.H"
#include "Functions.h"
#include "DuelSystem.h"
#include "BloodCastle.h"
#include "ImperialFort.h"
#include "ChaosCastle.h"
#include "MapServerManager.h"
#include "JSProtocol.H"
#include "Packets.h"
#include "DSProtocol.h"
#include "DBSockMng.h"
#include "PersonalShop.h"
#include "SendProto.H"


CMoveSystem gMove;
// --------------------------------------------------------------------------------------------------------------------------------------------------
CMoveSystem::CMoveSystem()
{
	return;
}
// --------------------------------------------------------------------------------------------------------------------------------------------------
CMoveSystem::~CMoveSystem()
{
	return;
}
// --------------------------------------------------------------------------------------------------------------------------------------------------
void CMoveSystem::InitMove()
{
	for(int i=0; i<MOVMAX; i++)
	{
		MOVSTR[i].aIndex		= 0;
		MOVSTR[i].Money			= 0;
		MOVSTR[i].MinLevel		= 0;
		MOVSTR[i].MaxLevel		= 0;
		MOVSTR[i].AllowPK		= 0;
		MOVSTR[i].TargetGate	= 0;
		MOVSTR[i].IsBattleZone	= 0;
		MOVSTR[i].MoveForChar	= -1;
	}
}
// --------------------------------------------------------------------------------------------------------------------------------------------------
void CMoveSystem::LoadMoveFile(LPSTR filename)
{
	int Token;
	int m_aIndex;
	int m_Money			= 0;
	int m_MinLevel		= 0;
	int m_MaxLevel		= 0;
	int m_AllowPK		= 0;
	int m_TargetGate	= 0;
	int m_IsBattleZone	= 0;
	int	m_MoveForChar	= -1;
	char m_MapName[40]  = {0};
	// -----
	int MoveReq			= 0;
	// -----
	this->InitMove();
	// -----
	SMDFile = fopen(filename, "r");
	// -----
	while(true)
	{
		Token = GetToken();
		// -----
		if(MoveReq == MOVMAX)
			break;
		// -----
		if ( Token == 2 )
			break;
		// -----
		if ( Token == 1 )
		{
			m_aIndex = (int)TokenNumber;
			// -----
			Token = GetToken();
			memcpy(m_MapName, TokenString, sizeof(m_MapName));
			// -----
			Token = GetToken();
			m_Money = (int)TokenNumber;
			// -----
			Token = GetToken();
			m_MinLevel = (int)TokenNumber;
			// -----
			Token = GetToken();
			m_MaxLevel = (int)TokenNumber;
			// -----
			Token = GetToken();
			m_AllowPK = (int)TokenNumber;
			// -----
			Token = GetToken();
			m_MoveForChar = (int)TokenNumber;
			// -----
			Token = GetToken();
			m_IsBattleZone = (int)TokenNumber;
			// -----
			Token = GetToken();
			m_TargetGate = (int)TokenNumber;
			// -----
			SetInfo(m_aIndex, m_MapName, m_Money, m_MinLevel, m_MaxLevel, m_AllowPK, m_MoveForChar, m_IsBattleZone, m_TargetGate);	
			MoveReq++;
		}
		else
		{
			InitMove();
			// -----
			fclose(SMDFile);
			// -----
			CLog.MsgBox("Move Information Data Load Failed.\n      You have to restart the GameServer.");
			// -----
			return;
		}
	}
	// -----
	fclose(SMDFile);
}
// --------------------------------------------------------------------------------------------------------------------------------------------------
void CMoveSystem::SetInfo(int iIndex,char *iMapName,int iMoney,int iMinLevel,int iMaxLevel,int iAllowPK,int iMoveForChar,int iIsBattleZone,int iGate)
{
	MOVSTR[iIndex].aIndex		= iIndex;
	MOVSTR[iIndex].Money		= iMoney;
	MOVSTR[iIndex].MinLevel		= iMinLevel;
	MOVSTR[iIndex].MaxLevel		= iMaxLevel;
	MOVSTR[iIndex].AllowPK		= iAllowPK;
	MOVSTR[iIndex].MoveForChar	= iMoveForChar;
	MOVSTR[iIndex].IsBattleZone	= iIsBattleZone;
	MOVSTR[iIndex].TargetGate	= iGate;
	strcpy(MOVSTR[iIndex].MapName,iMapName);
}
// --------------------------------------------------------------------------------------------------------------------------------------------------
void CMoveSystem::CheckRequirements(int aIndex, int MapNumber, BYTE X, BYTE Y)
{
	LPOBJ lpObj = &gObj[aIndex];
	// -----
	//--  Check Target Map Status
	if ( MapNumberCheck(MapNumber) == FALSE)
	{
		CLog.LogAddC(TColor.Red(), "Error: [%s][%s] Unknown Map Number [%d].", lpObj->AccountID, lpObj->Name, MapNumber);
		SendMsg.MessageOut(aIndex, 0x01, "[WARNING]: You are attempting to move to Unknown Map.");
		return;
	}
	// -----
	//-- Check The Coordonates Status
	if ( X > MOVMAX || X < MOVMIN ) return;
	if ( Y > MOVMAX || Y < MOVMIN ) return;
	// -----
	//-- Checking Required Money
	if ( lpObj->Money < MOVSTR[MapNumber].Money )
	{
		if ( HRB_CheckMoveMoneyMsg == TRUE )
		{
			SendMsg.MessageOut(aIndex, 0x01, "[WARNING]: You don't have enough zen to warp.");
		}
		return;
	}
	// -----
	//-- Checking Required Level
	if( !REQ_LEVEL_MOVE_RANGE(lpObj->Level) )
	{
		CLog.LogAddC(TColor.Red(), "Error: [%s][%s]: Trying to moving, While he is out of Range of Required Level!", lpObj->AccountID, lpObj->Name);
		// ----
		if ( HRB_CheckMoveLevelMsg == TRUE )
		{
			SendMsg.MessageOut(aIndex, 0x01, "[WARNING]: You are Out Of Range Of Required Level.");
			SendMsg.MessageOut(aIndex, 0x01, "MinLevel:(%d) - MaxLevel:(%d) to warp to [%s].", MOVSTR[MapNumber].MinLevel, MOVSTR[MapNumber].MaxLevel,
				GetMapName(MapNumber));
		}
		return;
	}
	// -----
	//-- Characters Type Specificity & Ability to Move
	if ( MOVSTR[MapNumber].MoveForChar == 1 ) //------------> For Normal Players Only
	{
		if ( (lpObj->AuthorityCode&1) == TRUE ) 
		{
			SendMsg.MessageOut(aIndex, 0x01, "[WARNING]: This Map is Only available for Normal Players Only.");
			return;
		}
	}
	else if ( MOVSTR[MapNumber].MoveForChar == 2 ) //-------> For Game Masters Only
	{
		if ( (lpObj->AuthorityCode&1) == FALSE )
		{
			SendMsg.MessageOut(aIndex, 0x01, "[WARNING]: This Map is Only available for Game Masters Only.");
			return;
		}
	}
	else if ( MOVSTR[MapNumber].MoveForChar == -1 ) //------> For All ( Normal & Game Masters )
	{
	}
	else
	{
		CLog.LogAddC(TColor.Red(), "Error: [%s][%s]: There is a Wrong Value in MoveSystem.txt in 'MoveForChar'.", lpObj->AccountID, lpObj->Name);
		return;
	}
	// -----
	//-- Checking BattleZones
	if ( MOVSTR[MapNumber].IsBattleZone == 1 )
	{
	}
	else
	{
	}
	// -----
	//--  Check Equipment To Move Atlans
	if ( MapNumber == MAP_INDEX_ATLANS )
	{
		if ( lpObj->pInventory[GUARDIAN].IsItem() != FALSE )
		{
			if ( lpObj->pInventory[GUARDIAN].m_Type == ITEMGET(13,2) ) // Horn Of Uniria
			{
				SendMsg.MessageOut(aIndex, 0x01, "You unable to warp to %s While Riding a Uniria.",GetMapName(MapNumber));
				return;
			}
			// ----
			if ( lpObj->pInventory[GUARDIAN].m_Type == ITEMGET(13,3) ) // Dinorant
			{
				SendMsg.MessageOut(aIndex, 0x01, "You unable to warp to %s While Riding a Dinorant.",GetMapName(MapNumber));
				return;
			}
		}
	}
	// -----
	//--  Check Equipment To Move Icarus
	if ( MapNumber == MAP_INDEX_ICARUS )
	{
		if(  lpObj->pInventory[GUARDIAN].m_Type != ITEMGET(13,3)  && 
			 lpObj->pInventory[WINGS].m_Type != ITEMGET(13,30) &&
			 lpObj->pInventory[GUARDIAN].m_Type != ITEMGET(13,37) &&
		    (lpObj->pInventory[WINGS].m_Type < ITEMGET(12,0)   ||  lpObj->pInventory[WINGS].m_Type > ITEMGET(12,6) ) &&
		    (lpObj->pInventory[WINGS].m_Type < ITEMGET(12,36)  ||  lpObj->pInventory[WINGS].m_Type > ITEMGET(12,43)) )
		{
			SendMsg.MessageOut(aIndex, 0x01, "You unable to warp to %s without Wing or Dinorant or Fernir.", GetMapName(MapNumber));
			return;
		}
		// ----
		if(lpObj->pInventory[GUARDIAN].m_Type == ITEMGET(13,2))
		{
			SendMsg.MessageOut(aIndex, 0x01, "You unable to warp to %s while Riding a Uniria.",GetMapName(MapNumber));
			return;
		}
		// ----
		if(lpObj->pInventory[RING_02].m_Type == ITEMGET(13,10) || lpObj->pInventory[RING_01].m_Type == ITEMGET(13,10))
		{
			SendMsg.MessageOut(aIndex, 0x01, "You unable to warp to %s while wearing Transformation Ring.",GetMapName(MapNumber));
			return;
		}
	}
	// -----
	//-- Checking Character If in Trade.
	if ( lpObj->m_IfState.use == 1 && lpObj->m_IfState.type == 1 )
	{
		CGTradeCancelButtonRecv(lpObj->m_Index);
	}
	// -----
	//-- Checking Personal Shop Status
	if ( lpObj->m_bPShopOpen == 1 )
	{
		CPShop.ClosePersonalShop(lpObj->m_Index);
	}
	// -----
	//-- Checking The Vault Status
	if ( lpObj->m_IfState.use == 1 && lpObj->m_IfState.type == 6 )
	{
		CGWarehouseUseEnd(lpObj->m_Index);
	}
	// -----
	//-- Checking NPC Status
	if ( lpObj->m_IfState.use == 1 && lpObj->m_IfState.type == 3 )
	{
		lpObj->TargetShopNumber = -1;
		lpObj->m_IfState.type	= 0;
		lpObj->m_IfState.use	= 0;
	}
	// -----
	//-- Checking If the User in Duel
	if( gDuel.IsOnDuel(lpObj) == true )
	{
		SendMsg.MessageOut(aIndex, 0x01, "You are in a Duel. You can't warp");
		return;
	}
}
// --------------------------------------------------------------------------------------------------------------------------------------------------
void CMoveSystem::RandWarp(int aIndex, int MapNumber) // Random Warping
{	
	LPOBJ lpObj = &gObj[aIndex];
	// -----
	if ( !OBJMAX_RANGE(aIndex) )
	{
		CLog.LogAddC(TColor.Red(), "Error: Protocol Index wrong!", lpObj->AccountID, lpObj->Name);
		return;
	}
	// -----
	if (GateC.iX1[MOVSTR[MapNumber].TargetGate] == GateC.iX2[MOVSTR[MapNumber].TargetGate])
	{
		GateC.iX2[MOVSTR[MapNumber].TargetGate]++;
	}
	// -----
	if (GateC.iY1[MOVSTR[MapNumber].TargetGate] == GateC.iY2[MOVSTR[MapNumber].TargetGate]) 
	{
		GateC.iY2[MOVSTR[MapNumber].TargetGate]++;
	}
	// -----
	MOVSTR->FinalX = min(GateC.iX1[MOVSTR[MapNumber].TargetGate],GateC.iX2[MOVSTR[MapNumber].TargetGate])+rand()%(max(GateC.iX1[MOVSTR[MapNumber].TargetGate],GateC.iX2[MOVSTR[MapNumber].TargetGate])-min(GateC.iX1[MOVSTR[MapNumber].TargetGate],GateC.iX2[MOVSTR[MapNumber].TargetGate]));
	MOVSTR->FinalY = min(GateC.iY1[MOVSTR[MapNumber].TargetGate],GateC.iY2[MOVSTR[MapNumber].TargetGate])+rand()%(max(GateC.iY1[MOVSTR[MapNumber].TargetGate],GateC.iY2[MOVSTR[MapNumber].TargetGate])-min(GateC.iY1[MOVSTR[MapNumber].TargetGate],GateC.iY2[MOVSTR[MapNumber].TargetGate]));
	// -----
	BYTE  TeleportMap	= GateC.iMapNum[MOVSTR[MapNumber].TargetGate];
	int	  TotalZen		= MOVSTR[MapNumber].Money;
	// -----
	CheckRequirements(aIndex, TeleportMap, MOVSTR->FinalX, MOVSTR->FinalY);
	// -----
	lpObj->m_State		= 32;
	lpObj->X			= MOVSTR->FinalX;
	lpObj->Y			= MOVSTR->FinalY;
	lpObj->Money	   -= TotalZen;
	// -----
	lpObj->TX			= MOVSTR->FinalX;
	lpObj->TY			= MOVSTR->FinalY;
	// -----
	lpObj->MapNumber	= TeleportMap;
	lpObj->PathCount	= 0;
	lpObj->Teleport		= 0;
	// -----
	gObjClearViewport(lpObj);
	GCTeleportSend(lpObj, 1, TeleportMap, lpObj->X, lpObj->Y, lpObj->Dir);
	GCMoneySend(aIndex, lpObj->Money);
	// -----
	if(lpObj->m_Change >= 0)
	{
		gObjViewportListProtocolCreate(lpObj);
	}
	// -----
	lpObj->RegenMapNumber	= TeleportMap;
	lpObj->RegenMapX		= MOVSTR->FinalX;
	lpObj->RegenMapY		= MOVSTR->FinalY;
	lpObj->RegenOk			= 1;
	// -----
	//-- Log Message for Successful Moving
	CLog.LogAddC(TColor.Green(), "[MoveSystem] CheckRandomdMove() - MapServer Check OK [%s][%s]: MAP-[%d] (%d)(%d)", lpObj->AccountID,
		lpObj->Name, TeleportMap, MOVSTR->FinalX, MOVSTR->FinalY);
}
// --------------------------------------------------------------------------------------------------------------------------------------------------
void CMoveSystem::SpecWarp(int aIndex, int MapNumber, BYTE X, BYTE Y) // Specified Warping
{
	LPOBJ lpObj = &gObj[aIndex];
	// -----
	if ( !OBJMAX_RANGE(aIndex) )
	{
		CLog.LogAddC(TColor.Red(), "Error: Protocol Index wrong!", lpObj->AccountID, lpObj->Name);
		return;
	}
	// -----
	CheckRequirements(aIndex, MapNumber, X, Y);
	// -----
	BYTE  TeleportMap	= GateC.iMapNum[MOVSTR[MapNumber].TargetGate];
	// -----
	lpObj->m_State		= 32;
	lpObj->X			= X;
	lpObj->Y			= Y;
	// -----
	lpObj->TX			= X;
	lpObj->TY			= Y;
	// -----
	lpObj->MapNumber	= TeleportMap;
	lpObj->PathCount	= 0;
	lpObj->Teleport		= 0;
	// -----
	gObjClearViewport(lpObj);
	GCTeleportSend(lpObj, 1, TeleportMap, lpObj->X, lpObj->Y, lpObj->Dir);
	GCMoneySend(aIndex, lpObj->Money);
	// -----
	if(lpObj->m_Change >= 0)
	{
		gObjViewportListProtocolCreate(lpObj);
	}
	// -----
	lpObj->RegenMapNumber	= TeleportMap;
	lpObj->RegenMapX		= X;
	lpObj->RegenMapY		= Y;
	lpObj->RegenOk			= 1;
	// -----
	//-- Log Message for Successful Moving
	CLog.LogAddC(TColor.Green(), "[MoveSystem] CheckSpecifiedMove() - MapServer Check OK [%s][%s]: MAP-[%d] (%d)(%d)", lpObj->AccountID,
		lpObj->Name, TeleportMap, X, Y);
}
// --------------------------------------------------------------------------------------------------------------------------------------------------
void PMoveProc(PMSG_MOVE* lpMove, int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];
	// -----
	PMSG_RECVMOVE pMove;
	// -----
	unsigned char	ax;
	unsigned char	ay;
	unsigned char	sx;
	unsigned char	sy;
	unsigned char	pathtable;
	// -----
	if( !OBJMAX_RANGE(aIndex) )
	{
		CLog.LogAddC(TColor.Red(), "Error: Protocol Index wrong!", lpObj->AccountID, lpObj->Name);
		return;
	}
	// -----
	if( lpObj->RegenOk > NULL ) return;
	// -----
	if( lpObj->SkillRecallParty_Time > NULL )
	{
		lpObj->SkillRecallParty_Time = 0;
		SendMsg.MessageOut(aIndex, 0x01, "Summoning cancelled.");
	}
	// -----
	if( (GetTickCount()-lpObj->m_LastMoveTime) < 100 ) return;
	// -----
	if( lpObj->Teleport ) return;
	// -----
	if( lpObj->m_SkillHarden )
	{
		gObjSetPosition(lpObj->m_Index, lpObj->X, lpObj->Y);
		return;
	}
	// -----
	if ( lpObj->m_iSkillStunTime > 0 )
	{
		gObjSetPosition(lpObj->m_Index, lpObj->X, lpObj->Y);
		return;
	}
	// -----
	lpObj->m_LastMoveTime	= GetTickCount();
	lpObj->m_Rest			= 0;
	lpObj->PathCur			= 0;
	lpObj->Dir				= lpMove->Path[0] >> 4;
	lpObj->PathCount		= lpMove->Path[0] & 0x0F;
	// -----
	if ( lpObj->PathCount > 15 )
	{
		CLog.LogAddC(TColor.Red(), "Error: [%s][%s] Path Count (%d)", lpObj->AccountID, lpObj->Name, lpObj->PathCount);
		return;
	}
	// -----
	for( int n=0; n<15; n++)
	{
		lpObj->PathX[n]		= 0;
		lpObj->PathY[n]		= 0;
		lpObj->PathOri[n]	= 0;
	}
	// -----
	sx = lpMove->X;
	sy = lpMove->Y;
	// -----
	if( gObjCheckXYMapTile(lpObj, 1) == TRUE )
	{
		lpObj->PathCount	= 0;
		lpObj->PathCur		= 0;
		lpObj->PathStartEnd = 0;
		// -----
		gObjSetPosition(lpObj->m_Index, lpObj->X, lpObj->Y);
		return;
	}
	// -----
	lpObj->PathX[0]			= lpMove->X;
	ax						= lpObj->PathX[0];
	lpObj->PathY[0]			= lpMove->Y;
	ay						= lpObj->PathY[0];
	lpObj->PathDir[0]		= lpObj->Dir;
	lpObj->PathStartEnd		= 1;
	// -----
	if( lpObj->PathCount > 0 )
	{
		lpObj->PathCur = 1;
		lpObj->PathCount++;
	}
	// -----
	for(int n=1; n<lpObj->PathCount; n++)
	{
		if( (n%2) == 1 )
		{
			pathtable = lpMove->Path[(n+1) / 2] >> 4;
		}
		else
		{
			pathtable = lpMove->Path[(n+1) / 2] & 0x0F;
		}
		// -----
		ax += RoadPathTable[pathtable*2];
		ay += RoadPathTable[pathtable*2+1];
		// -----
		lpObj->PathOri[n-1] = pathtable;
		lpObj->PathDir[n]	= pathtable;
		lpObj->PathX[n]		= ax;
		lpObj->PathY[n]		= ay;
	}
	// -----
	if ( lpObj->PathCount > 0 )
	{
		int		nextX;
		int		nextY;
		BYTE	MapNumber;
		BYTE	attr;
		// -----
		nextX = lpObj->PathX[1];
		nextY = lpObj->PathY[1];
		MapNumber = lpObj->MapNumber;
		attr = MapC[MapNumber].GetAttr(nextX, nextY);
		// -----
		if( ( (attr&4) == 4) || ( (attr&8) == 8) )
		{
			for ( int n=0 ; n<15 ; n++)
			{
				lpObj->PathX[n]		= 0;
				lpObj->PathY[n]		= 0;
				lpObj->PathOri[n]	= 0;
			}
			// -----
			lpObj->PathCount		= 0;
			lpObj->PathCur			= 0;
			lpObj->PathStartEnd		= 0;
			// -----
			gObjSetPosition(lpObj->m_Index, lpObj->X, lpObj->Y);
			return;
		}
	}
	// -----
	lpObj->TX = (BYTE)ax;
	lpObj->TY = (BYTE)ay;
	// -----
#if(GS_CASTLE)
	if( lpObj->Type == 1 )
	{
		int nextX	= lpObj->X;
		int nextY	= lpObj->Y;
		int iResult = 0;
		WORD attr	= 0;
		// -----
		for(int i = 0; i < 256;i++)
		{	
			if(nextX > lpObj->TX)
			{
				nextX -= 1;
			}
			// -----
			if(nextX < lpObj->TX)
			{
				nextX += 1;
			}
			// -----
			if(nextY > lpObj->TY)
			{
				nextY -= 1;
			}
			// -----
			if(nextY < lpObj->TY)
			{
				nextY += 1;
			}
			// -----
			attr = (WORD)MapC[lpObj->MapNumber].GetAttr(nextX,nextY);
			// -----
			if ( (attr&16) == 16 )
			{
				iResult = 1;
				break;
			}
			// -----
			if( nextX == lpObj->TX)
			{
				if(nextY == lpObj->TY) break;
			}
		}
		// -----
		if(iResult == 1)
		{
			lpObj->m_Rest		= 1;
			lpObj->PathCur		= 0;
			lpObj->PathCount	= 0;
			lpObj->PathStartEnd = 0;
			// -----
			gObjSetPosition(lpObj->m_Index,lpObj->X,lpObj->Y);
			// -----
			PMSG_ACTIONRESULT pActionResult;
			// -----
			C1HeadSet((LPBYTE)&pActionResult, 0x18, sizeof(pActionResult));
			// -----
			pActionResult.NumberH			= SET_NUMBERH(aIndex);
			pActionResult.NumberL			= SET_NUMBERL(aIndex);
			pActionResult.ActionNumber		= 0x7A;
			pActionResult.Dir				= lpObj->Dir;
			pActionResult.TargetNumberH		= 0;
			pActionResult.TargetNumberL		= 0;
			// -----
			gSendProto.DataSend(lpObj->m_Index, (LPBYTE)&pActionResult, sizeof(pActionResult));
			return;	
		}
	}
#endif
	// -----
	if( gUser.BloodCastleMapRange(lpObj->MapNumber) == TRUE && lpObj->Type == OBJ_USER )
	{
		switch ( g_BloodCastle.GetCurrentState(g_BloodCastle.GetBloodCastleIndex(lpObj->MapNumber)) )
		{
			case 0x01:
			{
				if ( lpObj->TY > 15 )
				{
					lpObj->m_Rest			= 1;
					lpObj->PathCur			= 0;
					lpObj->PathCount		= 0;
					lpObj->PathStartEnd		= 0;
					// -----
					gObjSetPosition(lpObj->m_Index, lpObj->X, lpObj->Y);
					// -----
					PMSG_ACTIONRESULT pActionResult;
					// -----
					C1HeadSet((LPBYTE)&pActionResult, 0x18, sizeof(pActionResult));
					// -----
					pActionResult.NumberH		= SET_NUMBERH(aIndex);
					pActionResult.NumberL		= SET_NUMBERL(aIndex);
					pActionResult.ActionNumber	= 0x7A;
					pActionResult.Dir			= lpObj->Dir;
					pActionResult.TargetNumberH = 0;
					pActionResult.TargetNumberL = 0;
					// -----
					gSendProto.DataSend(lpObj->m_Index, (LPBYTE)&pActionResult, sizeof(pActionResult));
					return;
				}
			}break;
			// -----
			case 0x02:
			{
				if( lpObj->Y > 17 && g_BloodCastle.CheckPlayStart(g_BloodCastle.GetBloodCastleIndex(lpObj->MapNumber)) == FALSE )
				{
					gObjMoveGate(lpObj->m_Index, lpObj->MapNumber+55);
					return;
				}
				// -----
				if( lpObj->TY > 15 && g_BloodCastle.CheckPlayStart(g_BloodCastle.GetBloodCastleIndex(lpObj->MapNumber)) == FALSE )
				{
					lpObj->m_Rest			= 1;
					lpObj->PathCur			= 0;
					lpObj->PathCount		= 0;
					lpObj->PathStartEnd		= 0;
					// -----
					gObjSetPosition(lpObj->m_Index, lpObj->X, lpObj->Y);
					// -----
					PMSG_ACTIONRESULT pActionResult;
					// -----
					C1HeadSet((LPBYTE)&pActionResult, 0x18, sizeof(pActionResult));
					// -----
					pActionResult.NumberH		= SET_NUMBERH(aIndex);
					pActionResult.NumberL		= SET_NUMBERL(aIndex);
					pActionResult.ActionNumber	= 0x7A;
					pActionResult.Dir			= lpObj->Dir;
					pActionResult.TargetNumberH = 0;
					pActionResult.TargetNumberL = 0;
					// -----
					gSendProto.DataSend(lpObj->m_Index, (LPBYTE)&pActionResult, sizeof(pActionResult));
					return;
				}
				// -----
				if ( lpObj->TY > 76 &&  g_BloodCastle.m_BridgeData[g_BloodCastle.GetBloodCastleIndex(lpObj->MapNumber)].m_bCASTLE_DOOR_LIVE )
				{
					lpObj->m_Rest		= 1;
					lpObj->PathCur		= 0;
					lpObj->PathCount	= 0;
					lpObj->PathStartEnd = 0;
					// -----
					gObjSetPosition(lpObj->m_Index, lpObj->X, lpObj->Y);
					// -----
					PMSG_ACTIONRESULT pActionResult;
					// -----
					C1HeadSet((LPBYTE)&pActionResult, 0x18, sizeof(pActionResult));
					// -----
					pActionResult.NumberH		= SET_NUMBERH(aIndex);
					pActionResult.NumberL		= SET_NUMBERL(aIndex);
					pActionResult.ActionNumber	= 0x7A;
					pActionResult.Dir			= lpObj->Dir;
					pActionResult.TargetNumberH = 0;
					pActionResult.TargetNumberL = 0;
					// -----
					gSendProto.DataSend(lpObj->m_Index, (LPBYTE)&pActionResult, sizeof(pActionResult));
					return;
				}
			}break;
		}
	}
	// -----
	gImperialFort.CanMoveNextStage(lpObj);
	// -----
	C1HeadSet((LPBYTE)&pMove, 0xD3, sizeof(pMove));
	// -----
	pMove.NumberH	= SET_NUMBERH(aIndex);
	pMove.NumberL	= SET_NUMBERL(aIndex);
	pMove.X			= ax;
	pMove.Y			= ay;
	pMove.Path		= lpObj->Dir << 4;
	// -----
	if( lpObj->Type == OBJ_USER )
	{
		if( lpObj->m_IfState.use == 1 )
		{
			if ( lpObj->m_IfState.type == 3 )
			{
				lpObj->TargetShopNumber = -1;
				lpObj->m_IfState.type	= 0;
				lpObj->m_IfState.use	= 0;
			}
		}
		// -----
		if ( !gObjPositionCheck(lpObj))
		{
			lpObj->PathCur		= 0;
			lpObj->PathCount	= 0;
			ax					= lpObj->X;
			ay					= lpObj->Y;
			lpObj->TX			= ax;
			lpObj->TY			= ay;
			pMove.X				= ax;
			pMove.Y				= ay;
		}
		// -----
		gSendProto.DataSend(aIndex, (LPBYTE)&pMove, pMove.h.size);
	}
	// -----
	int MVL = MAX_VIEWPORT;
	// -----
	if( lpObj->Type == OBJ_MONSTER ) MVL = MAX_VIEWPORT_MONSTER;
	// -----
	for (int n=0; n<MVL; n++)
	{
		if ( lpObj->VpPlayer2[n].state == TRUE )
		{
			int number = lpObj->VpPlayer2[n].number;
			// -----
			if ( number >= 0 )
			{
				if ( gObj[number].Connected > PLAYER_CONNECTED )
				{
					if ( gObj[number].Live )
					{
						if ( gObj[number].Type == OBJ_USER )
						{
							gSendProto.DataSend(lpObj->VpPlayer2[n].number, (LPBYTE)&pMove, pMove.h.size);
						}
					}
				}
				else
				{
					lpObj->VpPlayer2[n].number	= -1;
					lpObj->VpPlayer2[n].state	= 0;
					lpObj->VPCount2--;
				}
			}
		}
	}
	// -----
	MapC[lpObj->MapNumber].ClearStandAttr(lpObj->m_OldX, lpObj->m_OldY);
	MapC[lpObj->MapNumber].SetStandAttr(lpObj->TX, lpObj->TY);
	// -----
	lpObj->m_OldX		= lpObj->TX;
	lpObj->m_OldY		= lpObj->TY;
	lpObj->X			= sx;
	lpObj->Y			= sy;
	lpObj->m_ViewState	= 0;
}
// --------------------------------------------------------------------------------------------------------------------------------------------------
bool gObjCheckXYMapTile(LPOBJ lpObj, int iDbgName)
{
	if ( lpObj->Type != OBJ_USER ) return FALSE;
	// -----
	if ( (lpObj->Authority&2 ) == 2 ) return FALSE;
	// -----
	if ( gUser.ChaosCastleMapRange(lpObj->MapNumber) == TRUE )
	{
		if ( g_ChaosCastle.GetCurrentState(g_ChaosCastle.GetChaosCastleIndex(lpObj->MapNumber)) == 2 ) return FALSE;
	}
	// -----
	int x			= lpObj->X;
	int y			= lpObj->Y;
	int mapnumber	= lpObj->MapNumber;
	// -----
	if ( mapnumber > MAX_NUMBER_MAP-1 )
	{
		mapnumber = MAP_INDEX_LORENCIA;
	}
	// -----
	int MapMinLevel = MapMinUserLevel[mapnumber];
	// -----
	MapMinLevel = 0;
	// -----
	if ( lpObj->Class == CLASS_MAGUMSA || lpObj->Class == CLASS_DARKLORD )
	{
		if ( MapMinLevel > 0 )
		{
			if ( MapMinUserLevel[mapnumber] > 0 )
			{
				MapMinLevel = MapMinUserLevel[mapnumber] - (MapMinUserLevel[mapnumber] / 3 * 2);
			}
		}
	}
	// -----
	if ( lpObj->Class == CLASS_ELF && lpObj->Level < 10 )
	{
		if ( lpObj->MapNumber != MAP_INDEX_NORIA )
		{
			CLog.LogAddC(TColor.Purple(), "[%s][%s]:(%s)[%d/%d] Invalid Location, Forced to move!", lpObj->AccountID, lpObj->Name,
				GetMapName(lpObj->MapNumber), lpObj->X, lpObj->Y);
			// -----
			PMSG_TELEPORT pMsg;
			// -----
			pMsg.MoveNumber = 27;
			// -----
			gObjMoveGate(lpObj->m_Index, pMsg.MoveNumber);
		}
	}
	else
	{
		if ( MapMinLevel > lpObj->Level )
		{
			CLog.LogAddC(TColor.Purple(), "[%s][%s]:(%s)[%d/%d] Invalid Location, Forced to move!", lpObj->AccountID, lpObj->Name,
				GetMapName(lpObj->MapNumber), lpObj->X, lpObj->Y);
			// -----
			PMSG_TELEPORT pMsg;
			// -----
			pMsg.MoveNumber = 17;
			// -----
			gObjMoveGate(lpObj->m_Index, pMsg.MoveNumber);
		}
	}
	// -----
	BYTE attr = MapC[mapnumber].GetAttr(x, y);
	// -----
	if ( (attr&4) == 4 || (attr&8) == 8 )
	{
		BYTE attr2 = MapC[mapnumber].GetAttr(x+2, y);
		BYTE attr3 = MapC[mapnumber].GetAttr(x, y+2);
		BYTE attr4 = MapC[mapnumber].GetAttr(x-2, y);
		BYTE attr5 = MapC[mapnumber].GetAttr(x, y-2);
		// -----
		if ( attr2 > 1 && attr3 > 1 && attr4 > 1 && attr5 > 1 )
		{
			CLog.LogAddC(TColor.Purple(), "[CHECK POSITION]:[%s][%s]:(%s)[%d/%d] Invalid Location, Forced to move!", lpObj->AccountID, lpObj->Name, 
				GetMapName(lpObj->MapNumber), lpObj->X, lpObj->Y);
			// -----
			PMSG_TELEPORT pMsg;
			// -----
			pMsg.MoveNumber = 17;
			// -----
			gObjMoveGate(lpObj->m_Index, pMsg.MoveNumber);
			
			return true;
		}
	}
	// -----
	return false;
}
// --------------------------------------------------------------------------------------------------------------------------------------------------
void gObjSetPosition(int aIndex, int X, int Y)
{
	LPOBJ lpObj = &gObj[aIndex];
	// -----
	PMSG_POSISTION_SET pMove;
	// -----
	pMove.h.c			= 0xC1;
	pMove.h.headcode	= PMSG_POSISTION_SET_HEADCODE;
	pMove.h.size		= sizeof(pMove);
	// -----
	pMove.X				= X;
	pMove.Y				= Y;
	// -----
	lpObj->m_Rest		= 0;
	// -----
	MapC[lpObj->MapNumber].ClearStandAttr(lpObj->m_OldX,lpObj->m_OldY);
	MapC[lpObj->MapNumber].SetStandAttr(X,Y);
	// -----
	lpObj->m_OldX		= X;
	lpObj->m_OldY		= Y;
	// -----
	RecvPositionSetProc(&pMove,lpObj->m_Index);
}
// --------------------------------------------------------------------------------------------------------------------------------------------------
void gObjTeleport(int aIndex, BYTE Map, BYTE X, BYTE Y)
{
	LPOBJ lpObj = &gObj[aIndex];
	// -----
	if (aIndex < 0 || aIndex > OBJMAX - 1) return;
	// -----
	if ( MapNumberCheck(Map) == FALSE)
	{
		CLog.LogAddC(TColor.Red(), "Error: [%s][%s] Unknown Map Number [%d].", lpObj->AccountID, lpObj->Name, Map);
		return;
	}
	// -----
	if (lpObj->MapNumber != Map)
	{
		short sSvrCode = g_MapServerManager.CheckMoveMapSvr(lpObj->m_Index, Map, lpObj->m_sPrevMapSvrCode);
		// -----
		if(sSvrCode != gGameServerCode)
		{
			if(sSvrCode == -1)
			{
				CLog.LogAddC(TColor.Red(), "Error: [%s][%s] Map Server Move Failed : CheckMoveMapSvr() == -1",lpObj->AccountID,lpObj->Name);
				return;
			}
			// -----
			GJReqMapSvrMove(lpObj->m_Index, sSvrCode, Map, X, Y);
			// -----
			CLog.LogAddC(TColor.Green(), "[%s][%s] Requested to Move Map Server:(%d).", lpObj->AccountID, lpObj->Name, sSvrCode);
			return;
		}
	}
	// -----
	if(lpObj->m_IfState.use == 1 && lpObj->m_IfState.type == 1)
	{
		CGTradeCancelButtonRecv(lpObj->m_Index);
	}
	// -----
	if(lpObj->m_IfState.use == 1 && lpObj->m_IfState.type == 6)
	{
		CGWarehouseUseEnd(lpObj->m_Index);
	}
	// -----
	lpObj->m_State			= 32;
	gObj[aIndex].X			= X;
	gObj[aIndex].Y			= Y;
	// -----
	gObj[aIndex].TX			= X;
	gObj[aIndex].TY			= Y;
	// -----
	gObj[aIndex].MapNumber	= Map;
	gObj[aIndex].PathCount	= 0;
	gObj[aIndex].Teleport	= 0;
	// -----
	gObjClearViewport(&gObj[aIndex]);
	// -----
	GCTeleportSend(lpObj, 1, Map, lpObj->X, lpObj->Y, lpObj->Dir);
	// -----
	if(lpObj->m_Change >= 0)
	{
		gObjViewportListProtocolCreate(lpObj);
	}
	// -----
	gObj[aIndex].RegenMapNumber = Map;
	gObj[aIndex].RegenMapX		= X;
	gObj[aIndex].RegenMapY		= Y;
	gObj[aIndex].RegenOk		= 1;
}
// --------------------------------------------------------------------------------------------------------------------------------------------------
void RecvPositionSetProc(PMSG_POSISTION_SET * lpMove, int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];
	// -----
	if ( !OBJMAX_RANGE(aIndex) )
	{
		CLog.LogAddC(TColor.Red(), "Error: [%s][%s]: Trying to moving outside the Limited Range!", lpObj->AccountID, lpObj->Name);
		return;
	}
	// -----
	if ( PacketCheckTime(lpObj) == FALSE ) return;
	// -----
	if ( lpObj->Teleport != 0 ) return;
	// -----
	lpObj->X = lpMove->X;
	lpObj->Y = lpMove->Y;
	// -----
	if ( gUser.ChaosCastleMapRange(lpObj->MapNumber) == TRUE )
	{
		if ( (GetTickCount() - lpObj->m_iChaosCastleBlowTime ) < 1000 ) return;
	}
	// -----
	if ( gObjCheckXYMapTile(lpObj, 2 ) == TRUE) return;
	// -----
	PMSG_RECV_POSISTION_SET pMove;
	// -----
	C1HeadSet((LPBYTE)&pMove, 0xDF, sizeof(pMove));
	// -----
	pMove.NumberH	= SET_NUMBERH(aIndex);
	pMove.NumberL	= SET_NUMBERL(aIndex);
	pMove.X			= lpMove->X;
	pMove.Y			= lpMove->Y;
	lpObj->TX		= lpMove->X;
	lpObj->TY		= lpMove->Y;
	// -----
	if ( gObjPositionCheck(lpObj) == FALSE )return;
	// -----
	CreateFrustrum(lpObj->X, lpObj->Y, aIndex);
	// -----
	if ( lpObj->Type == OBJ_USER )
	{
		gSendProto.DataSend(aIndex, (UCHAR *)&pMove, pMove.h.size);
	}
	// -----
	int MVL = MAX_VIEWPORT;
	// -----
	if ( lpObj->Type == OBJ_MONSTER )
	{
		MVL = MAX_VIEWPORT_MONSTER;
	}
	// -----
	for ( int n=0; n<MVL; n++)
	{
		if ( lpObj->VpPlayer2[n].type == OBJ_USER )
		{
			if ( lpObj->VpPlayer2[n].state == 1 )
			{
				int Index = lpObj->VpPlayer2[n].number;
				// -----
				if ( gObj[Index].Connected > PLAYER_CONNECTED && gObj[Index].Live != FALSE)
				{
					gSendProto.DataSend(lpObj->VpPlayer2[n].number, (UCHAR *)&pMove, pMove.h.size);
				}
				else
				{
					lpObj->VpPlayer2[n].number = -1;
					lpObj->VpPlayer2[n].state = FALSE;
					lpObj->VPCount2--;
				}
			}
		}
	}
	// -----
	MapC[lpObj->MapNumber].ClearStandAttr(lpObj->m_OldX, lpObj->m_OldY);
	MapC[lpObj->MapNumber].SetStandAttr(lpObj->TX, lpObj->TY);
	// -----
	lpObj->m_OldX = lpObj->TX;
	lpObj->m_OldY = lpObj->TY;
}
// --------------------------------------------------------------------------------------------------------------------------------------------------
void GJReqMapSvrMove(int iIndex, WORD wDesMapSvrCode, WORD wMapNumber, BYTE btX, BYTE btY)
{
	PMSG_REQ_MAPSVRMOVE pMsg;
	// -----
	if ( gObjIsConnected(iIndex) == PLAYER_EMPTY )
	{
		CLog.LogAddC(TColor.Red(), "[MapServerMng] Packet Error GJ [0x7A] - User not Connected : %d", iIndex);
	}
	else
	{
		C1HeadSet((LPBYTE)&pMsg, 0x7A, sizeof(PMSG_REQ_MAPSVRMOVE));
		// -----
		pMsg.iIndex = iIndex;
		// -----
		memcpy(pMsg.szAccountID , gObj[iIndex].AccountID, sizeof( pMsg.szAccountID)-1);
		pMsg.szAccountID[10]	= 0;
		// -----
		memcpy(pMsg.szCharName, gObj[iIndex].Name, sizeof( pMsg.szCharName)-1);
		pMsg.szCharName[10]		= 0;
		// -----
		pMsg.wCurMapSvrCode		= gGameServerCode;
		pMsg.wDstMapSvrCode		= wDesMapSvrCode;
		pMsg.wMapNumber			= wMapNumber;
		pMsg.btX				= btX;
		pMsg.btY				= btY;
		// -----
		wsJServerCli.DataSend((char*)&pMsg, pMsg.h.size);
	}
}
// --------------------------------------------------------------------------------------------------------------------------------------------------
void GCTeleportSend(LPOBJ lpObj, BYTE MoveNumber, BYTE MapNumber, BYTE MapX, BYTE MapY, BYTE Dir)
{
	PMSG_TELEPORT_RESULT pMsg;
	// -----
	if ( lpObj->Type != OBJ_USER ) return;
	// -----
	pMsg.h.c		= 0xC3;
	pMsg.h.size		= sizeof(pMsg);
	pMsg.h.headcode = 0x1C;
	pMsg.MoveNumber = MoveNumber;
	pMsg.MapNumber	= MapNumber;
	pMsg.MapX		= MapX;
	pMsg.MapY		= MapY;
	pMsg.Dir		= Dir;
	// -----
	if ( MoveNumber == 0 )
		gSendProto.DataSend(lpObj->m_Index, (UCHAR*)&pMsg, pMsg.h.size);
	else
		gSendProto.DataSend(lpObj->m_Index, (UCHAR*)&pMsg, pMsg.h.size);
}
// --------------------------------------------------------------------------------------------------------------------------------------------------
void CGTeleportRecv(PMSG_TELEPORT* lpMsg, int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];
	// -----
	if ( !OBJMAX_RANGE(aIndex) ) return;
	// -----
	if ( lpObj->m_iSkillStunTime > 0 ) return;
	// -----
	if ( lpObj->m_SkillSleepTime > 0 ) return;
	// -----
	if ( lpMsg->MoveNumber == 0 )
	{
		CMagicInf * lpMagic;
		unsigned char x;
		unsigned char y;
		PMSG_MAGICATTACK_RESULT pAttack;
		// -----
		C3HeadSet((LPBYTE)&pAttack, 0x19, sizeof(pAttack));
		// -----
		pAttack.MagicNumberH = SET_NUMBERH(6);
		pAttack.MagicNumberL = SET_NUMBERL(6);
		pAttack.SourceNumberH = SET_NUMBERH(aIndex);
		pAttack.SourceNumberL = SET_NUMBERL(aIndex);
		pAttack.TargetNumberH = SET_NUMBERH(aIndex);
		pAttack.TargetNumberL = SET_NUMBERL(aIndex);
		// -----
		lpMagic = gObjGetMagicSearch(lpObj, 6);
		x		= lpMsg->MapX;
		y		= lpMsg->MapY;
		// -----
		if ( gObjCheckTeleportArea(aIndex, x, y) == FALSE )
		{
			CLog.LogAddC(TColor.Red(), "[%s][%s] Try Teleport Not Move Area [%d,%d]", lpObj->AccountID, lpObj->Name, x, y);
			// -----
			PMSG_TELEPORT_RESULT pTeleportResult;
			// -----
			pTeleportResult.h.c			= 0xC3;
			pTeleportResult.h.size		= sizeof(pTeleportResult);
			pTeleportResult.h.headcode	= 0x1C;
			pTeleportResult.GateH		= SET_NUMBERH(aIndex);
			pTeleportResult.GateL		= SET_NUMBERL(aIndex);
			pTeleportResult.MoveNumber	= 0;
			pTeleportResult.MapNumber	= lpObj->MapNumber;
			pTeleportResult.MapX		= lpObj->X;
			pTeleportResult.MapY		= lpObj->Y;
			pTeleportResult.Dir			= lpObj->Dir;
			// -----
			gSendProto.DataSend(aIndex, (LPBYTE)&pTeleportResult, pTeleportResult.h.size);
			return;
		}
		// -----
#if(GS_CASTLE)
		if(gObj[aIndex].MapNumber == MAP_INDEX_CASTLESIEGE )
		{
			if( g_CastleSiege.CheckTeleportMagicAxisY(lpObj->Y,x,y) == FALSE )
			{
				y = lpObj->Y;
			}
		}
#endif
		// -----
		if ( lpMagic )
		{
			int UseMana = gObjMagicManaUse(lpObj, lpMagic);
			// -----
			if ( UseMana >= 0 )
			{
				int UseBP = gObjMagicBPUse(lpObj, lpMagic);
				// -----
				if ( UseBP >= 0 )
				{
					if ( gObj[aIndex].Type == OBJ_USER )
						gSendProto.DataSend(aIndex, (LPBYTE)&pAttack, pAttack.h.size);
					// -----
					gSendProto.VPMsgSend(lpObj, (LPBYTE)&pAttack, pAttack.h.size);
					// -----
					gObjTeleportMagicUse(aIndex, x, y);
					// -----
					gObj[aIndex].Mana	= UseMana;
					gObj[aIndex].BP		= UseBP;
					// -----
					GCManaSend(lpObj->m_Index, lpObj->Mana, -1, 0, lpObj->BP);
				}
			}
		}
	}
	// -----
	else if ( GateC.IsInGate(aIndex, lpMsg->MoveNumber) == TRUE )
	{
		gObjMoveGate(aIndex, lpMsg->MoveNumber);
	}
	// -----
	else
	{
		//gObjClearViewport(&gObj[aIndex]);
		GCTeleportSend(&gObj[aIndex], lpMsg->MoveNumber, gObj[aIndex].MapNumber, gObj[aIndex].X, gObj[aIndex].Y, gObj[aIndex].Dir);
		// -----
	#if(GS_CASTLE)
		if(gObj[aIndex].MapNumber == MAP_INDEX_CASTLESIEGE)
		{
			if(g_CastleSiege.GetCastleState() == 7)
			{
				g_CastleSiege.NotifySelfCsJoinSide(aIndex);
				g_CastleSiege.NotifyCsSelfLeftTime(aIndex);
			}
		}
	#endif
	}
}
// --------------------------------------------------------------------------------------------------------------------------------------------------
void CGTargetTeleportRecv(PMSG_TARGET_TELEPORT * lpMsg, int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];
	// -----
	if ( !OBJMAX_RANGE(aIndex)) return;
	// -----
	if ( lpObj->m_iSkillStunTime > 0 ) return;
	// -----
	CMagicInf * lpMagic;
	BYTE X;
	BYTE Y;
	PMSG_MAGICATTACK_RESULT pAttack;
	// -----
	C3HeadSet((LPBYTE)&pAttack, 0x19, sizeof(pAttack));
	// -----
	pAttack.MagicNumberH	= SET_NUMBERH(AT_SKILL_TARGET_TELEPORT);
	pAttack.MagicNumberL	= SET_NUMBERL(AT_SKILL_TARGET_TELEPORT);
	pAttack.SourceNumberH	= SET_NUMBERH(aIndex);
	pAttack.SourceNumberL	= SET_NUMBERL(aIndex);
	int iTargetIndex		= MAKE_NUMBERW(lpMsg->NumberL, lpMsg->NumberH);	// #error? parameters inverse?
	pAttack.TargetNumberH	= SET_NUMBERH(iTargetIndex);	// #error? parameters inverse?
	pAttack.TargetNumberL	= SET_NUMBERL(iTargetIndex);	// #error? parameters inverse?
	lpMagic					= gObjGetMagicSearch(lpObj, AT_SKILL_TARGET_TELEPORT);
	X						= lpMsg->MapX;
	Y						= lpMsg->MapY;
	// -----
	if ( !gObjIsConnectedGP(iTargetIndex) )
	{
		CLog.LogAddC(TColor.Red(), "[%s][%s] Try Target Teleport Not Move Area [%d,%d]", lpObj->AccountID, lpObj->Name, X, Y);
		return;
	}
	// -----
	if ( lpObj->PartyNumber != gObj[iTargetIndex].PartyNumber || lpObj->PartyNumber == -1 || gObj[iTargetIndex].PartyNumber == -1) return;
	// -----
	if ( !gObjCheckTeleportArea(iTargetIndex, X, Y))
	{
		CLog.LogAddC(TColor.Red(), "[%s][%s] Try Target Teleport Not Move Area [%d,%d]", lpObj->AccountID, lpObj->Name, X, Y);
		return;
	}
	// -----
	if ( lpMagic )
	{
		int UseMana = gObjMagicManaUse(lpObj, lpMagic);
		// -----
		if ( UseMana >= 0 )
		{
			int UseBP = gObjMagicBPUse(lpObj, lpMagic);
			// -----
			if ( UseBP >= 0 )
			{
				if ( lpObj->Type == OBJ_USER )
					gSendProto.DataSend(aIndex, (LPBYTE)&pAttack, pAttack.h.size);
				// -----
				gSendProto.VPMsgSend(&gObj[aIndex], (LPBYTE)&pAttack, pAttack.h.size);
				gObjTeleportMagicUse(iTargetIndex, X, Y);
				lpObj->Mana	= UseMana;
				lpObj->BP	= UseBP;
				GCManaSend(lpObj->m_Index, lpObj->Mana, -1, 0, lpObj->BP);
				// -----
				return;
			}
		}
	}
}
// --------------------------------------------------------------------------------------------------------------------------------------------------
void CGReqMoveOtherServer(PMSG_REQ_MOVE_OTHERSERVER * lpMsg, int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];

	if ( !gObjIsConnectedGP(aIndex) )
	{
		CLog.LogAddC(2, "error-L3 [%s][%d]", __FILE__, __LINE__);
		return;
	}

	if ( lpObj->m_MoveOtherServer )
		return;

	lpObj->m_MoveOtherServer = true;

	PMSG_DEFRESULT pResult;
	C1HeadSet((LPBYTE)&pResult, 0x99, sizeof(pResult));
	
	if ( !gObjJoominCheck(aIndex, lpMsg->LastJoominNumber))
	{
		pResult.result = 0;

		gSendProto.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		lpObj->m_MoveOtherServer = false;

		CLog.LogAdd("[CharTrasfer] Fail (JoominNumber) [%s][%s]", lpObj->AccountID, lpObj->Name);
		return;
	}

	SDHP_CHARACTER_TRANSFER pCharTransfer;

	GJSetCharacterInfo(lpObj, lpObj->m_Index, FALSE);
	C1HeadSet((LPBYTE)&pCharTransfer, 0xCF, sizeof(pCharTransfer));
	pCharTransfer.Number = aIndex;
	memcpy(pCharTransfer.Account, lpObj->AccountID, sizeof(pCharTransfer.Account));

	cDBSMng.Send((PCHAR)&pCharTransfer, sizeof(pCharTransfer));

	CLog.LogAdd("[CharTrasfer] Request Character Trasfer [%s][%s]", lpObj->AccountID, lpObj->Name);
}

void DGMoveOtherServer(SDHP_CHARACTER_TRANSFER_RESULT * lpMsg)
{
	if ( !gObjIsConnectedGP(lpMsg->Number))
	{
		CLog.LogAddC(2, "error-L3 [%s][%d]", __FILE__, __LINE__);
		return;
	}

	LPOBJ lpObj = &gObj[lpMsg->Number];

	if ( lpMsg->Result != 0 )
	{
		PMSG_DEFRESULT pResult;

		C1HeadSet((LPBYTE)&pResult, 0x99, sizeof(pResult));
		pResult.result = 1;

		gSendProto.DataSend(lpMsg->Number, (LPBYTE)&pResult, pResult.h.size);

		CLog.LogAdd("[CharTrasfer] Fail [%s][%s] (%d)", lpObj->AccountID, lpObj->Name, lpMsg->Result);
		
		lpObj->m_MoveOtherServer = 0;
		
		//GCServerMsgStringSend("문제 발생시 change@webzen.co.kr로 문의해 주시기바랍니다",lpObj->m_Index, 1);
		// Deathway translation here
		return;
	}

	CLog.LogAdd("[CharTrasfer] Success [%s][%s] (%d)", lpObj->AccountID, lpObj->Name, lpMsg->Result);

	//GCServerMsgStringSend("접속이 종료됩니다.", lpObj->m_Index, 1);// Deathway translation here
	//GCServerMsgStringSend("분할 서버로 접속해주시기 바랍니다.", lpObj->m_Index, 1);// Deathway translation here
	GJSetCharacterInfo(lpObj, lpObj->m_Index, 0);
	lpObj->LoadWareHouseInfo = false;
	gObjCloseSet(lpObj->m_Index, 2);
	lpObj->m_MoveOtherServer = 0;
}

void MoveMonsterProc()
{
	DWORD MoveTime;
	int DelayTime;
	LPOBJ lpObj;
	
	for(int n = 0; n < OBJMAX;n++)
	{
		lpObj = &gObj[n];
		
		if(lpObj->m_iCurrentAI != 0)
		{
			if(lpObj->Type == OBJ_MONSTER || lpObj->Type == OBJ_NPC)
			{
				continue;
			}
		}
		
		if(lpObj->Connected == PLAYER_PLAYING)
		{
			if(lpObj->Type == OBJ_MONSTER)
			{
#if(!GS_CASTLE)
				if(ATTRIBUTE_RANGE(lpObj->m_Attribute) != FALSE)
				{
					CreateFrustrum(lpObj->X,lpObj->Y,n);
					continue;
				}
#else
				if( ATTRIBUTE_RANGE(lpObj->m_Attribute) != FALSE ||
					(lpObj->Class != 277 &&
					lpObj->Class != 283 &&
					lpObj->Class != 288 &&
					lpObj->Class != 278 &&
					lpObj->Class != 216 &&
					lpObj->Class != 217 &&
					lpObj->Class != 218 &&
					lpObj->Class != 219) &&
					(CRYWOLF_ALTAR_CLASS_RANGE(lpObj->Class) != FALSE ||
					CRYWOLF_STATUE_CHECK(lpObj->Class) != FALSE) )
				{
					CreateFrustrum(lpObj->X,lpObj->Y,n);
					continue;
				}
#endif
			}
			
			if(lpObj->m_State == 2)
			{
				if( lpObj->PathCount != 0 && lpObj->m_SkillHarden == 0 && lpObj->m_iSkillStunTime == 0 && lpObj->m_SkillSleepTime == 0 )
				{
					if(lpObj->DelayLevel != 0)
					{
						DelayTime = 300;
					}
					else
					{
						DelayTime = 0;
					}
					
					if(lpObj->Type == OBJ_MONSTER && lpObj->m_RecallMon >= 100)
					{
						lpObj->m_MoveSpeed = 200;
					}
					else
					{
						lpObj->m_MoveSpeed = 400;
					}
					
					if(lpObj->PathDir[lpObj->PathCur]%2 == 0)
					{
						MoveTime = (lpObj->m_MoveSpeed + DelayTime)*(double)1.3;
					}
					else
					{
						MoveTime = lpObj->m_MoveSpeed + DelayTime;
					}
					
					if( (GetTickCount() - lpObj->PathTime) > MoveTime && lpObj->PathCur < 14)
					{
						int nextX = lpObj->PathX[lpObj->PathCur];
						int nextY = lpObj->PathY[lpObj->PathCur];
						BYTE mapnumber = lpObj->MapNumber;
						BYTE attr = MapC[mapnumber].GetAttr(nextX,nextY);
						
						if( lpObj->Type == OBJ_USER &&
							( (attr & 4) == 4 || (attr & 8) == 8) )
						{
							CLog.LogAdd("[ CHECK POSITION ] MoveMosterProc [%s][%s] Map[%d]-(%d,%d) User(%d,%d) Can not Move Position Attr[%d]",
								lpObj->AccountID,lpObj->Name,lpObj->MapNumber,nextX,nextY,lpObj->X,lpObj->Y,attr);
							for(n = 0; n < 15;n++)
							{
								lpObj->PathX[n] = 0;
								lpObj->PathY[n] = 0;
								lpObj->PathOri[n] = 0;
							}
							
							lpObj->PathCount = 0;
							lpObj->PathCur = 0;
							lpObj->PathTime = GetTickCount();
							
							if(lpObj->Type == OBJ_MONSTER || lpObj->Type == OBJ_NPC)
							{
								lpObj->PathStartEnd = 0;
							}
							
							if(lpObj->Type == OBJ_USER)
							{
								gObjSetPosition(lpObj->m_Index,lpObj->X,lpObj->Y);
							}
						}
						else
						{
							lpObj->X = lpObj->PathX[lpObj->PathCur];
							lpObj->Y = lpObj->PathY[lpObj->PathCur];
							lpObj->Dir = lpObj->PathDir[lpObj->PathCur];
							
							lpObj->PathTime = GetTickCount();
							lpObj->PathCur++;
							
							if(lpObj->PathCur >= lpObj->PathCount)
							{
								lpObj->PathCur = 0;
								lpObj->PathCount = 0;
								
								if(lpObj->Type == OBJ_MONSTER || lpObj->Type == OBJ_NPC)
								{
									lpObj->PathStartEnd = 0;
								}
							}
						}
					}
				}
				CreateFrustrum(lpObj->X,lpObj->Y,n);
			}
		}
	}
}

// ---- Get Map Move Level For All Characters
int GetMapMoveLevel(LPOBJ lpObj, int mapnumber, int max_over)
{
	int overLevel = 0;

	if ( mapnumber > MAX_NUMBER_MAP-1 )
	{
		mapnumber = 0;
		CLog.LogAdd("Error-L3 : Map Number not %s %d", __FILE__, __LINE__);
		return 0;
	}

	if ( max_over != 0 )
	{
		overLevel = max_over;
	}
	else
	{
		max_over = MapMinUserLevel[mapnumber];
		overLevel = MapMinUserLevel[mapnumber];
	}
	
	if ( lpObj->Class == CLASS_DARKLORD  || lpObj->Class == CLASS_MAGUMSA || lpObj->Class == CLASS_FIGHTER ) // Dark Lord, Magic Gladiator & Rage Fighter
	{
		if ( overLevel > 0 && max_over > 0 )
		{
			overLevel = (max_over/3)*2;
		}
	}

	return overLevel;
}