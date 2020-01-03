#include "stdafx.h"
#include "UnderWorld.h"
#include "Defines.h"
#include "LogProc.H"
#include "GameMain.h"
#include "gObjMonster.h"
#include "DSProtocol.h"
#include "Notice.H"

CUnderworld gUnderworld;

CUnderworld::CUnderworld()
{
	gUnderworld.Init();
}

CUnderworld::~CUnderworld()
{

}

void CUnderworld::Init()
{
	gUnderworld.Enabled					= GetPrivateProfileInt("UnderWorld","Enabled",0,UNDERWD_FILE_PATH);
	gUnderworld.DurationTime			= GetPrivateProfileInt("UnderWorld","DurationTime",0,UNDERWD_FILE_PATH);
	gUnderworld.AuxDurationTime			= gUnderworld.DurationTime*60;
	gUnderworld.Started					= 0;
	gUnderworld.MonsterCount			= 0;
	gUnderworld.BossPosX				= 0;
	gUnderworld.BossPosY				= 0;

	for (int i=0;i<50;i++)
	{
		gUnderworld.MonsterIndex[i] = -1;
	}
}

void CUnderworld::Start()
{
	if ( gUnderworld.Enabled == 0 )
	{
		return;
	}
	if ( gUnderworld.Started == 0 )
	{
		SendMsg.MessageOutAll(0x00, "UnderWorld Invasion Started");
		CLog.LogAdd("UNDERWORLD EVENT STARTED");
		GCMapEventStateSend(0, 1, 2);
		gUnderworld.Started = 1;
		gUnderworld.MonstersAdd();
		_beginthread(UnderWorld_EventStart,0,NULL);
	}
}

void CUnderworld::End()
{
	SendMsg.MessageOutAll(0x00, "UnderWorld Invasion End");
	CLog.LogAdd("UNDERWORLD EVENT END");
	gUnderworld.AuxDurationTime			= gUnderworld.DurationTime*60;
	gUnderworld.Started					= 0;
	gUnderworld.BossPosX				= 0;
	gUnderworld.BossPosY				= 0;
	gUnderworld.DeleteMonsters();
}

void CUnderworld::MonstersAdd()
{
	int iMonsterIndex;

	iMonsterIndex = gObjAddMonster(0);

	if( iMonsterIndex >= 0 )
	{
		LPOBJ lpObj = &gObj[iMonsterIndex];
		gObjSetMonster(iMonsterIndex, 55);

		while ( gMSetBase.GetBoxPosition(0, 50, 50, 200, 200, lpObj->X, lpObj->Y) == 0 )
		{

		}

		lpObj->TX = lpObj->X;
		lpObj->MTX = lpObj->X;
		lpObj->m_OldX = lpObj->X;
		lpObj->StartX = lpObj->X;
		lpObj->TY = lpObj->Y;
		lpObj->MTY = lpObj->Y;
		lpObj->m_OldY = lpObj->Y;
		lpObj->StartY = lpObj->Y;
		lpObj->MapNumber = 0;
		lpObj->Dir = rand()%8;			
		lpObj->Level = 20;
		lpObj->RegenTime = 0;
		lpObj->DieRegen = 0;
		//lpObj->m_MoveRange = 2;

		gUnderworld.MonsterIndex[gUnderworld.MonsterCount] = iMonsterIndex;
		gUnderworld.MonsterCount++;

		gUnderworld.BossPosX = lpObj->X;
		gUnderworld.BossPosY = lpObj->Y;

		CLog.LogAdd("Make Death King: %d, %d,%d", lpObj->MapNumber, lpObj->X, lpObj->Y);
	}

	for (int i=0;i<7;i++)
	{
		iMonsterIndex = gObjAddMonster(0);

		if( iMonsterIndex >= 0 )
		{
			LPOBJ lpObj = &gObj[iMonsterIndex];
			gObjSetMonster(iMonsterIndex, 56);

			gMSetBase.GetBoxPosition(0, gUnderworld.BossPosX-4, gUnderworld.BossPosY-4,gUnderworld.BossPosX+4, gUnderworld.BossPosY+4, lpObj->X, lpObj->Y);

			lpObj->TX = lpObj->X;
			lpObj->MTX = lpObj->X;
			lpObj->m_OldX = lpObj->X;
			lpObj->StartX = lpObj->X;
			lpObj->TY = lpObj->Y;
			lpObj->MTY = lpObj->Y;
			lpObj->m_OldY = lpObj->Y;
			lpObj->StartY = lpObj->Y;
			lpObj->MapNumber = 0;
			lpObj->Dir = rand()%8;			
			lpObj->Level = 20;
			lpObj->RegenTime = 0;
			lpObj->DieRegen = 0;
			//lpObj->m_MoveRange = 2;

			gUnderworld.MonsterIndex[gUnderworld.MonsterCount] = iMonsterIndex;
			gUnderworld.MonsterCount++;

			CLog.LogAdd("Make Death Bone: %d, %d,%d",	lpObj->MapNumber, lpObj->X, lpObj->Y);
		}
	}
}

void CUnderworld::DeleteMonsters()
{
	gUnderworld.MonsterCount			= 0;

	for (int i=0;i<50;i++)
	{
		if ( gUnderworld.MonsterIndex[i] != -1 )
		{
			gObjDel(gUnderworld.MonsterIndex[i]);
			gUnderworld.MonsterIndex[i] = -1;
		}
	}
}

void UnderWorld_EventStart(void * lpParam)
{
	while ( true )
	{
		if ( gUnderworld.AuxDurationTime <= 0 )
		{
			gUnderworld.End();
			_endthread();
		}
		gUnderworld.AuxDurationTime--;
		Sleep(1000);
	}
}

bool CUnderworld::DropCheck(LPOBJ lpObj)
{
	int MaxHitUser;
	int ItemType;
	bool bResult = false;
	BYTE Socket[5];
	Socket[0] = 0xFF;
	Socket[1] = 0xFF;
	Socket[2] = 0xFF;
	Socket[3] = 0xFF;
	Socket[4] = 0xFF;
	// ----
	switch ( lpObj->Class )
	{
	case 55:	// -- Death King
		MaxHitUser = gObjMonsterTopHitDamageUser(lpObj);
		ItemType = ItemGetNumberMake(12, 15);
		ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, lpObj->X, lpObj->Y, ItemType, 0, 0, 0, 0, 0, MaxHitUser, 0, 0, Socket);
		bResult = true;
		break;
	case 56:	// -- Death Bone
		MapC[lpObj->MapNumber].MoneyItemDrop(10000, lpObj->X, lpObj->Y);
		bResult = true;
		break;
	}
	return bResult;
}