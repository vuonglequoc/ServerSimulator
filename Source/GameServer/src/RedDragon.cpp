#include "stdafx.h"
#include "RedDragon.h"
#include "Defines.h"
#include "Notice.H"
#include "User.h"
#include "GameMain.h"
#include "LogProc.H"

CRedDragon gRedDragon;

CRedDragon::CRedDragon()
{
	gRedDragon.Init();
}

CRedDragon::~CRedDragon()
{

}

void CRedDragon::Init()
{
	gRedDragon.Enabled					= GetPrivateProfileInt("RedDragon","Enabled",0,REDDRAGON_FILE_PATH);
	gRedDragon.MaxDragons				= GetPrivateProfileInt("RedDragon","MaxRedDragons",0,REDDRAGON_FILE_PATH);
	gRedDragon.DurationTime				= GetPrivateProfileInt("RedDragon","DurationTime",0,REDDRAGON_FILE_PATH);
	gRedDragon.AuxDurationTime			= gRedDragon.DurationTime*60;
	gRedDragon.Started					= 0;
	gRedDragon.MonsterCount				= 0;
	gRedDragon.MapNumber				= 0;

	for (int i=0;i<50;i++)
	{
		gRedDragon.MonsterIndex[i] = -1;
	}
}

void CRedDragon::Run()
{
	if ( gRedDragon.Enabled == 0 )
	{
		return;
	}
	gRedDragon.SendInvasionEffect();
}

void CRedDragon::Start()
{
	if ( gRedDragon.Enabled == 0 )
	{
		return;
	}
	if ( gRedDragon.Started == 0 )
	{
		gRedDragon.Started = 1;
		SendMsg.MessageOutAll(0x00, "Red Dragon Invasion Started");
		CLog.LogAdd("RED DRAGON EVENT STARTED");
		gRedDragon.AddMonsters();
		_beginthread(RedDragon_EventStart,0,NULL);
	}
}

void CRedDragon::End()
{
	SendMsg.MessageOutAll(0x00, "Red Dragon Invasion End");
	CLog.LogAdd("RED DRAGON EVENT END");
	GCMapEventStateSend(gRedDragon.MapNumber , 0, 1 );
	gRedDragon.AuxDurationTime			= gRedDragon.DurationTime*60;
	gRedDragon.Started					= 0;
	gRedDragon.MapNumber				= 0;
	gRedDragon.DeleteMonsters();
}

void CRedDragon::DeleteMonsters()
{
	for (int i=0;i<50;i++)
	{
		if ( gRedDragon.MonsterIndex[i] != -1 )
		{
			gObjDel(gRedDragon.MonsterIndex[i]);
			gRedDragon.MonsterIndex[i] = -1;
		}
	}
	gRedDragon.MonsterCount = 0;
}

void CRedDragon::AddMonsters()
{
	int iMonsterIndex;
	int Map[3] = {0,2,3};
	gRedDragon.MapNumber=Map[rand()%3];

	GCMapEventStateSend(gRedDragon.MapNumber , 1, 1 );

	for (int i=0;i<gRedDragon.MaxDragons;i++)
	{
		iMonsterIndex = gObjAddMonster(gRedDragon.MapNumber);

		if( iMonsterIndex >= 0 )
		{
			LPOBJ lpObj = &gObj[iMonsterIndex];
			gObjSetMonster(iMonsterIndex, 44);

			switch ( gRedDragon.MapNumber )
			{
			case 0:
				if ( (rand()%3) == 0 )
				{
					gMSetBase.GetBoxPosition(gRedDragon.MapNumber, 135, 61, 146, 70, lpObj->X, lpObj->Y);
				}
				else if ( (rand()%3) == 1 )
				{
					gMSetBase.GetBoxPosition(gRedDragon.MapNumber, 120, 204, 126, 219, lpObj->X, lpObj->Y);
				}
				else
				{
					gMSetBase.GetBoxPosition(gRedDragon.MapNumber, 67, 116, 77, 131, lpObj->X, lpObj->Y);
				}
				break;

			case 2:
				if ((rand()%2) == 0)
				{
					gMSetBase.GetBoxPosition(gRedDragon.MapNumber, 155, 16, 169, 55, lpObj->X, lpObj->Y);
				}
				else
				{
					gMSetBase.GetBoxPosition(gRedDragon.MapNumber, 193, 110, 232, 126, lpObj->X, lpObj->Y);
				}
				break;

			case 3:
				if ((rand()%2) == 0)
				{
					gMSetBase.GetBoxPosition(gRedDragon.MapNumber, 198, 161, 223, 175, lpObj->X, lpObj->Y);
				}
				else
				{
					gMSetBase.GetBoxPosition(gRedDragon.MapNumber, 136, 50, 164, 59, lpObj->X, lpObj->Y);
				}
				break;
			}

			lpObj->TX = lpObj->X;
			lpObj->MTX = lpObj->X;
			lpObj->m_OldX = lpObj->X;
			lpObj->StartX = lpObj->X;
			lpObj->TY = lpObj->Y;
			lpObj->MTY = lpObj->Y;
			lpObj->m_OldY = lpObj->Y;
			lpObj->StartY = lpObj->Y;
			lpObj->MapNumber = gRedDragon.MapNumber;
			lpObj->Dir = rand()%8;			
			lpObj->Level = 20;
			lpObj->RegenTime = 0;
			lpObj->DieRegen = 0;
			lpObj->m_MoveRange = 2;

			gRedDragon.MonsterIndex[gRedDragon.MonsterCount] = iMonsterIndex;
			gRedDragon.MonsterCount++;

			CLog.LogAdd("Make Red Dragon: %d, %d,%d", lpObj->MapNumber, lpObj->X, lpObj->Y);
		}
	}
}

void RedDragon_EventStart(void * lpParam)
{
	while ( true )
	{
		if ( gRedDragon.AuxDurationTime <= 0 )
		{
			gRedDragon.End();
			_endthread();
		}
		gRedDragon.AuxDurationTime--;
		Sleep(1000);
	}
}

void CRedDragon::SendInvasionEffect()
{
	if ( gRedDragon.Enabled == 0 )
	{
		return;
	}
	if ( gRedDragon.Started == 0 )
	{
		return;
	}
	int Count = 0;
	for (int i=0;i<50;i++)
	{
		if ( gRedDragon.MonsterIndex[i] != -1 )
		{
			if ( gObj[gRedDragon.MonsterIndex[i]].Life > 0.0f )
			{
				Count++;
			}
		}
	}
	if ( Count > 0 )
	{
		GCMapEventStateSend(gRedDragon.MapNumber , 1, 1 );
	}
	else
	{
		GCMapEventStateSend(gRedDragon.MapNumber , 0, 1 );
	}
}