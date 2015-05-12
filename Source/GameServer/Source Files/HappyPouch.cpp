#include "../Header Files/stdafx.h"
#include "../Header Files/HappyPouch.h"
#include "../Header Files/Defines.h"
#include "../Header Files/Notice.H"
#include "../Header Files/User.h"
#include "../Header Files/GameMain.h"
#include "../Header Files/LogProc.H"

CHappyPouch gHappyPouch;

CHappyPouch::CHappyPouch()
{
	gHappyPouch.Init();
}

CHappyPouch::~CHappyPouch()
{

}

void CHappyPouch::Init()
{
	gHappyPouch.Enabled			= GetPrivateProfileInt("HappyPouch","Enabled",0,HAPPYPOUC_FILE_PATH);
	gHappyPouch.Duration		= GetPrivateProfileInt("HappyPouch","DurationTime",0,HAPPYPOUC_FILE_PATH);
	gHappyPouch.MaxGhost		= GetPrivateProfileInt("HappyPouch","MaxHappyPouch",0,HAPPYPOUC_FILE_PATH);

	if ( gHappyPouch.MaxGhost > 200 )
	{
		gHappyPouch.MaxGhost = 200;
	}
	gHappyPouch.AuxDuration		= gHappyPouch.Duration*60;
	gHappyPouch.Started			= 0;
	gHappyPouch.MonsterCount	= 0;

	for (int i=0;i<200;i++)
	{
		gHappyPouch.MonsterIndex[i] = -1;
	}
}

void CHappyPouch::Start()
{
	if ( gHappyPouch.Enabled == 0 )
	{
		return;
	}
	if ( gHappyPouch.Started == 0 )
	{
		gHappyPouch.Started = 1;
		SendMsg.MessageOutAll(0x00, "[HappyPouch]: Event has started");
		gHappyPouch.AddMonsters();
		_beginthread(HappyPouch_EventStart,0,NULL);
	}
}

void CHappyPouch::Clear()
{
	gHappyPouch.Started			= 0;
	gHappyPouch.AuxDuration		= gHappyPouch.Duration*60;

	gHappyPouch.RemoveMonsters();
}

void CHappyPouch::RemoveMonsters()
{
	for ( int i=0;i<200;i++)
	{
		if ( gHappyPouch.MonsterIndex[i] != -1 )
		{
			gObjDel(gHappyPouch.MonsterIndex[i]);
			gHappyPouch.MonsterIndex[i] = -1;
		}
	}
	gHappyPouch.MonsterCount = 0;
}

void CHappyPouch::AddMonsters()
{
	BYTE Map[6] = {7,4,33,8,37,57};
	BYTE MapNumber;
	int iMonsterIndex;
	// ----
	for (int i=0;i<gHappyPouch.MaxGhost;i++)
	{
		MapNumber = Map[rand()%6];

		iMonsterIndex = gObjAddMonster(MapNumber);

		if( iMonsterIndex >= 0 )
		{
			LPOBJ lpObj = &gObj[iMonsterIndex];
			gObjSetMonster(iMonsterIndex, 365);

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

			gHappyPouch.MonsterIndex[gHappyPouch.MonsterCount] = iMonsterIndex;
			gHappyPouch.MonsterCount++;

			CLog.LogAdd("Make Pouch of Blessing Map %d Coords (%d,%d)",lpObj->MapNumber,lpObj->X,lpObj->Y);
		}
	}
}

void HappyPouch_EventStart(void * lpParam)
{
	while ( true )
	{
		if ( gHappyPouch.AuxDuration <= 0 )
		{
			SendMsg.MessageOutAll(0x00, "[HappyPouch]: Event has finished");
			gHappyPouch.Clear();
			_endthread();
		}
		gHappyPouch.AuxDuration--;
		Sleep(1000);
	}
}