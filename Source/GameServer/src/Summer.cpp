#include "stdafx.h"
#include "Summer.h"
#include "Defines.h"
#include "Notice.H"
#include "User.h"
#include "GameMain.h"
#include "LogProc.H"

CSummer gSummer;

CSummer::CSummer()
{
	gSummer.Init();
}

CSummer::~CSummer()
{

}

void CSummer::Init()
{
	gSummer.Enabled			= GetPrivateProfileInt("Summer","Enabled",0,SUMMER_FILE_PATH);
	gSummer.Duration		= GetPrivateProfileInt("Summer","DurationTime",0,SUMMER_FILE_PATH);
	gSummer.MaxGhost		= GetPrivateProfileInt("Summer","MaxFireFlameGhost",0,SUMMER_FILE_PATH);
	if ( gSummer.MaxGhost > 200 )
	{
		gSummer.MaxGhost = 200;
	}
	gSummer.AuxDuration		= gSummer.Duration*60;
	gSummer.Started			= 0;
	gSummer.MonsterCount	= 0;

	for (int i=0;i<200;i++)
	{
		gSummer.MonsterIndex[i] = -1;
	}
}

void CSummer::Start()
{
	if ( gSummer.Enabled == 0 )
	{
		return;
	}
	if ( gSummer.Started == 0 )
	{
		gSummer.Started = 1;
		SendMsg.MessageOutAll(0x00, "[Summer]: Event has started");
		gSummer.AddMonsters();
		_beginthread(Summer_EventStart,0,NULL);
	}
}

void CSummer::Clear()
{
	gSummer.Started			= 0;
	gSummer.AuxDuration		= gSummer.Duration*60;

	gSummer.RemoveMonsters();
}

void CSummer::RemoveMonsters()
{
	for ( int i=0;i<200;i++)
	{
		if ( gSummer.MonsterIndex[i] != -1 )
		{
			gObjDel(gSummer.MonsterIndex[i]);
			gSummer.MonsterIndex[i] = -1;
		}
	}
	gSummer.MonsterCount = 0;
}

void CSummer::AddMonsters()
{
	BYTE Map[6] = {7,4,33,8,37,57};
	BYTE MapNumber;
	int iMonsterIndex;
	// ----
	for (int i=0;i<gSummer.MaxGhost;i++)
	{
		MapNumber = Map[rand()%6];

		iMonsterIndex = gObjAddMonster(MapNumber);

		if( iMonsterIndex >= 0 )
		{
			LPOBJ lpObj = &gObj[iMonsterIndex];
			gObjSetMonster(iMonsterIndex, 463);

			while ( gMSetBase.GetBoxPosition(MapNumber, 80, 80, 170, 170, lpObj->X, lpObj->Y) == 0 )
			{

			}

			lpObj->Life = lpObj->MaxLife;
			lpObj->TX = lpObj->X;
			lpObj->TY = lpObj->Y;
			lpObj->MTX = lpObj->X;
			lpObj->MTY = lpObj->Y;
			lpObj->StartX = (BYTE)lpObj->X;
			lpObj->StartY = (BYTE)lpObj->Y;
			lpObj->MapNumber = MapNumber;
			//lpObj->m_MoveRange = 4;
			lpObj->Dir = rand()%8;

			gSummer.MonsterIndex[gSummer.MonsterCount] = iMonsterIndex;
			gSummer.MonsterCount++;

			CLog.LogAdd("Make Fire Flame Ghost Map %d Coords (%d,%d)",lpObj->MapNumber,lpObj->X,lpObj->Y);
		}
	}
}

void Summer_EventStart(void * lpParam)
{
	while ( true )
	{
		if ( gSummer.AuxDuration <= 0 )
		{
			SendMsg.MessageOutAll(0x00, "[Summer]: Event has finished");
			gSummer.Clear();
			_endthread();
		}
		gSummer.AuxDuration--;
		Sleep(1000);
	}
}