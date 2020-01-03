//**********************************************************************//
// Project:	HaRiBO MU GameServer - Season 4								//
// Company: RealCoderZ MU Development © 2010							//
// Description: IllusionTemple Event Season 4							//
// Decompilation By: Deathway											//
// Coded & Modified By:	WolF & M.E.S									//
//**********************************************************************//
#include "stdafx.h"
#include "IllusionTemple.h"
#include "User.h"
#include "GameMain.h"
#include "ChaosCastle.h"
#include "BloodCastle.h"
#include "prodef.h"
#include "Gate.h"
#include "LogProc.H"
#include "Functions.H"


#if(!GS_CASTLE)
bool CIllusionTemple::SetProc_Closed(int iTempleIndex)
{
	this->SynchronizeTemple(iTempleIndex);

	for(int i = OBJ_STARTUSERINDEX; i < OBJMAX; i++)
	{

		if(gObj[i].MapNumber == MAP_INDEX_ILLUSION1 + iTempleIndex)
		{

			for(int it = 0; it < INVENTORY_SIZE; it++)
			{
				if(gObj[i].pInventory[it].IsItem())
				{
					if(gObj[i].pInventory[it].m_Type == ITEMGET(14, 64))
					{
						gObjInventoryDeleteItem(gObj[i].m_Index, it);
						GCInventoryItemDeleteSend(gObj[i].m_Index, it, 1);
					}
				}
			}

			gObj[i].m_Change = -1;
			gObjViewportListProtocolCreate(&gObj[i]);

			gObj[i].m_IllusionIndex = -1;
			gObj[i].m_IllusionTeam = -1;
			gObj[i].m_IllusionKillCount = 0;

			gObjMoveGate(i, 257);
		}
	}

	this->m_IllusionTemple[iTempleIndex].m_Statues[IT_STATUE_UP] = -1;
	this->m_IllusionTemple[iTempleIndex].m_Statues[IT_STATUE_DOWN] = -1;
	this->m_IllusionTemple[iTempleIndex].m_BallOwner = -1;
	this->m_IllusionTemple[iTempleIndex].m_iUserCount = 0;
	this->m_IllusionTemple[iTempleIndex].m_Teams[IT_TEAM_RED].m_Members.clear();
	this->m_IllusionTemple[iTempleIndex].m_Teams[IT_TEAM_RED].m_BallOwner = -1;
	this->m_IllusionTemple[iTempleIndex].m_Teams[IT_TEAM_RED].m_Score = 0;
	this->m_IllusionTemple[iTempleIndex].m_Teams[IT_TEAM_BLUE].m_Members.clear();
	this->m_IllusionTemple[iTempleIndex].m_Teams[IT_TEAM_BLUE].m_BallOwner = -1;
	this->m_IllusionTemple[iTempleIndex].m_Teams[IT_TEAM_BLUE].m_Score = 0;

	return TRUE;
}
// -----------------------------------------------------------------------------------------------------------------------
void CIllusionTemple::DoProc_Closed(int iTempleIndex)
{
	int iTICK_MSEC = GetTickCount() - this->m_IllusionTemple[iTempleIndex].m_TickCount;

	if ( iTICK_MSEC >= 1000 )
	{
		this->m_IllusionTemple[iTempleIndex].m_RemainTime -= iTICK_MSEC;
		this->m_IllusionTemple[iTempleIndex].m_TickCount = GetTickCount();

		if(this->m_IllusionTemple[iTempleIndex].m_RemainTime < IT_OPEN_TIME_MIN * 60000)
		{
			this->SetTempleState(iTempleIndex, IT_STATE_OPEN);
		}
	}
}
// -----------------------------------------------------------------------------------------------------------------------
bool CIllusionTemple::SetProc_Open(int iTempleIndex)
{
	this->m_IllusionTemple[iTempleIndex].m_RemainTime = IT_OPEN_TIME_MIN * 60000;
	this->m_IllusionTemple[iTempleIndex].m_SendNoticeCount = IT_OPEN_TIME_MIN;
	this->m_IllusionTemple[iTempleIndex].m_TickCount = GetTickCount(); // Instant UpDate!
	return TRUE;
}
// -----------------------------------------------------------------------------------------------------------------------
void CIllusionTemple::DoProc_Open(int iTempleIndex)
{
	DWORD Tick = GetTickCount() - this->m_IllusionTemple[iTempleIndex].m_TickCount;

	if(Tick > 1000)
	{
		this->m_IllusionTemple[iTempleIndex].m_TickCount = GetTickCount();
		this->m_IllusionTemple[iTempleIndex].m_RemainTime -= Tick;

		if(this->m_IllusionTemple[iTempleIndex].m_SendNoticeCount > 0)
		{
			if((iTempleIndex == 0) && 
				(this->m_IllusionTemple[iTempleIndex].m_RemainTime <= this->m_IllusionTemple[iTempleIndex].m_SendNoticeCount * 60000) && 
				(this->m_IllusionTemple[iTempleIndex].m_RemainTime > (this->m_IllusionTemple[iTempleIndex].m_SendNoticeCount-1) * 60000))
			{
				//PMSG_NOTICE pMsg;
				//TNotice::MakeNoticeMsgEx(&pMsg, 0, "Illusion temple fechara em %d minutos", this->m_IllusionTemple[iTempleIndex].m_SendNoticeCount); 
				//this->SendAllUserAnyMsg((BYTE*)&pMsg, pMsg.h.size);
				this->m_IllusionTemple[iTempleIndex].m_SendNoticeCount--;
			}
		}
	}

	if(this->m_IllusionTemple[iTempleIndex].m_RemainTime <= 0)
	{
		this->SetTempleState(iTempleIndex, IT_STATE_PREPARE);
	}
}
// -----------------------------------------------------------------------------------------------------------------------
bool CIllusionTemple::SetProc_Prepare(int iTempleIndex)
{
	ILLUSION_TEMPLE_DATA* lpTemple = &this->m_IllusionTemple[iTempleIndex];

	int iUserCount = 0;

	for(int i = OBJ_STARTUSERINDEX; i < OBJMAX; i++)
	{
		if((gObj[i].MapNumber == MAP_INDEX_ILLUSION1 + iTempleIndex) && (gObjIsConnected(i) == TRUE))
		{
			ILLUSION_OBJ obj;
			int iTeamIndex = iUserCount % 2;

			obj.m_BonusCount = 0;
			obj.m_Index = i;
			obj.m_TeamIndex = iTeamIndex;
			obj.m_TempleIndex = iTempleIndex;

			lpTemple->m_Teams[iTeamIndex].m_Members.push_back(obj);

			switch(iTeamIndex)
			{
			case IT_TEAM_RED:
				gObj[i].m_Change = IT_MASK_RED;
				gObjMoveGate(i, 154 + iTempleIndex);
				break;
			case IT_TEAM_BLUE:
				gObj[i].m_Change = IT_MASK_BLUE;
				gObjMoveGate(i, 148 + iTempleIndex);
				break;
			}

			gObj[i].m_IllusionTeam = iTeamIndex;

			iUserCount++;
		}
	}

	if(iUserCount < MIN_ILLUSION_PLAYERCOUNT)
	{
		this->SetTempleState(iTempleIndex, IT_STATE_CLOSED);
		CLog.LogAdd("[IllusionTemple] Event Closed - Min Users Not Reached (%d)", iTempleIndex + 1);
		return FALSE;
	}

	lpTemple->m_TickCount = GetTickCount() + (IT_PREPARE_TIME * 60000);

	this->SendMsgTemple(iTempleIndex, 1, "Prepare-se, o evento comecara em %d minuto(s).", IT_PREPARE_TIME);

	return TRUE;
}
// -----------------------------------------------------------------------------------------------------------------------
void CIllusionTemple::DoProc_Prepare(int iTempleIndex)
{
	ILLUSION_TEMPLE_DATA* lpTemple = &this->m_IllusionTemple[iTempleIndex];

	if((int)GetTickCount() > lpTemple->m_TickCount)
	{
		this->SetTempleState(iTempleIndex, IT_STATE_BATTLE);
	}
}
// -----------------------------------------------------------------------------------------------------------------------
#pragma pack(1)
struct PMSG_ILLUSION_STATE
{
	PBMSG_HEAD2	h;
	BYTE Unknow;
	BYTE State;
};
// -----------------------------------------------------------------------------------------------------------------------
#pragma pack()
bool CIllusionTemple::SetProc_Battle(int iTempleIndex)
{
	this->SetTempleMonsters(iTempleIndex);

	PMSG_ILLUSION_STATE pMsg;
	C1SubHeadSet((BYTE*)&pMsg, 0xBF, 0x09, sizeof(pMsg));
	pMsg.Unknow = 0;

	pMsg.State = 1;
	this->SendDataTemple(iTempleIndex, (BYTE*)&pMsg, pMsg.h.size);

	pMsg.State = 2;
	this->SendDataTemple(iTempleIndex, (BYTE*)&pMsg, pMsg.h.size);

	C1SubHeadSet((BYTE*)&pMsg, 0xBF, 0x06, 0x05);
	pMsg.Unknow = 0;

	this->SendDataTemple(iTempleIndex, (BYTE*)&pMsg, pMsg.h.size);

	this->UpdateTempleStatus(iTempleIndex);

	this->SendMsgTemple(iTempleIndex, 0, "[IllusionTemple] Event has been started!");

	this->m_IllusionTemple[iTempleIndex].m_TickCount = GetTickCount() + ILLUSION_PLAY_TIME * 60000;

	return TRUE;
}
// -----------------------------------------------------------------------------------------------------------------------
void CIllusionTemple::DoProc_Battle(int iTempleIndex)
{
	if(this->GetUserCount(iTempleIndex) < MIN_ILLUSION_PLAYERCOUNT) // FIX Check Here (Time = 0 Players)
	{
		this->SetTempleState(iTempleIndex, IT_STATE_END);
		CLog.LogAdd("[IllusionTemple] All Users Out, Ending Event (%d)", iTempleIndex + 1);
	}

	if(this->m_IllusionTemple[iTempleIndex].m_TickCount < (int)GetTickCount())
	{
		this->SetTempleState(iTempleIndex, IT_STATE_END);
		return;
	}

	if(this->m_IllusionTemple[iTempleIndex].m_StatisticsTickCount < (int)GetTickCount())
	{
		this->UpdateTempleStatus(iTempleIndex);
		this->m_IllusionTemple[iTempleIndex].m_StatisticsTickCount = GetTickCount() + 1000;
	}
}
// -----------------------------------------------------------------------------------------------------------------------
bool CIllusionTemple::SetProc_End(int iTempleIndex)
{
	PMSG_ILLUSION_STATE pMsg;
	C1SubHeadSet((BYTE*)&pMsg, 0xBF, 0x09, sizeof(pMsg));
	pMsg.Unknow = 0;

	pMsg.State = 3;
	this->SendDataTemple(iTempleIndex, (BYTE*)&pMsg, pMsg.h.size);

	for(int i = OBJ_STARTUSERINDEX; i < OBJMAX; i++)
	{
		if(IT_MAP_RANGE(gObj[i].MapNumber) == TRUE)
		{
			for(int it = 0; it < INVENTORY_SIZE; it++)
			{
				if(gObj[i].pInventory[it].IsItem())
				{
					if(gObj[i].pInventory[it].m_Type == ITEMGET(14, 64))
					{
						gObjInventoryDeleteItem(gObj[i].m_Index, i);
						GCInventoryItemDeleteSend(gObj[i].m_Index, i, 1);
					}
				}
			}
			if(iTempleIndex == (gObj[i].MapNumber - MAP_INDEX_ILLUSION1))
			{
				gObj[i].m_Change = -1;
				gObjViewportListProtocolCreate(&gObj[i]);
				gObjMoveGate(i, 142 + iTempleIndex);
			}
		}
	}

	this->UnSetTempleMonsters(iTempleIndex);

	// Send Items!
	// Give Bonus!

	this->m_IllusionTemple[iTempleIndex].m_TickCount = GetTickCount() + 60000;

	return TRUE;
}
// -----------------------------------------------------------------------------------------------------------------------
void CIllusionTemple::DoProc_End(int iTempleIndex)
{
	if((int)GetTickCount() > this->m_IllusionTemple[iTempleIndex].m_TickCount)
	{
		this->SetTempleState(iTempleIndex, IT_STATE_CLOSED);
	}
}
// -----------------------------------------------------------------------------------------------------------------------
#endif