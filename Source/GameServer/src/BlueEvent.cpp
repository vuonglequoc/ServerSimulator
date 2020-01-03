#include "stdafx.h"
#include "BlueEvent.h"
#include "Defines.h"
#include "Notice.H"
#include "User.h"
#include "GameMain.h"
#include "LogProc.H"

CBlueEvent gBlueEvent;

CBlueEvent::CBlueEvent(void)
{
	return;
}

CBlueEvent::~CBlueEvent(void)
{
	return;
}

void CBlueEvent::Init()
{
	gBlueEvent.Enabled = GetPrivateProfileInt(BLUEEVENT_HDR,"Enabled",0,BLUEEVENTFILE_PATH);

	if ( gBlueEvent.Enabled == 1 )
	{
		gBlueEvent.Duration = GetPrivateProfileInt(BLUEEVENT_HDR,"DurationTime",0,BLUEEVENTFILE_PATH);
	
		gBlueEvent.AuxDuration = gBlueEvent.Duration*60;
		gBlueEvent.Started = 0;
		gBlueEvent.MonsterCount = 0;

		for (int i=0;i<200;i++)
		{
			gBlueEvent.MonsterIndex[i] = -1;
		}
		CLog.LogAdd("[BlueEvent]: Init -> Enabled");
	}
	else
	{
		CLog.LogAdd("[BlueEvent]: Init -> Disabled");
	}
}

void CBlueEvent::Start()
{
	if ( gBlueEvent.Enabled == 0 )
	{
		return;
	}
	if ( gBlueEvent.Started == 0 )
	{
		gBlueEvent.Started = 1;
		SendMsg.MessageOutAll(0x00, "[BlueEvent]: Event has started");
		CLog.LogAdd("[BlueEvent]: Event has started");
		gBlueEvent.AddMonsters();
		_beginthread(BlueEvent_EventStart,0,NULL);
	}
}

void CBlueEvent::Clear()
{
	gBlueEvent.Started = 0;
	gBlueEvent.AuxDuration = gBlueEvent.Duration*60;
	gBlueEvent.RemoveMonsters();
}

void CBlueEvent::RemoveMonsters()
{
	for ( int i=0;i<200;i++)
	{
		if ( gBlueEvent.MonsterIndex[i] != -1 )
		{
			gObjDel(gBlueEvent.MonsterIndex[i]);
			gBlueEvent.MonsterIndex[i] = -1;
		}
	}
	gBlueEvent.MonsterCount = 0;
}

void CBlueEvent::AddMonsters()
{
	int result = -1;
	// ----
	for (int i=0;i<gMSetBase.m_Count;i++)
	{
		if ( gMSetBase.m_Mp[i].m_Type == 413 && gMSetBase.m_Mp[i].m_Dir == 1 )
		{
			result = gObjAddMonster(gMSetBase.m_Mp[i].m_MapNumber);

			if ( result >= 0 )
			{
				gObj[result].X = gMSetBase.m_Mp[i].m_X;
				gObj[result].Y = gMSetBase.m_Mp[i].m_Y;
				gObj[result].MapNumber = gMSetBase.m_Mp[i].m_MapNumber;
				gObj[result].TX = gObj[result].X;
				gObj[result].TY = gObj[result].Y;
				gObj[result].m_OldX = gObj[result].X;
				gObj[result].m_OldY = gObj[result].Y;
				gObj[result].StartX = gObj[result].X;
				gObj[result].StartY = gObj[result].Y;
				gObjSetMonster(result, gMSetBase.m_Mp[i].m_Type);
				gObj[result].Dir = rand()%8;

				if ( gBlueEvent.MonsterCount == 200 )
				{
					CLog.LogAdd("[BUG TRACER][BlueEvent]: Add Monsters -> Add Monster Error. Max Monster Count = 200");
				}
				else
				{
					gBlueEvent.MonsterIndex[gBlueEvent.MonsterCount] = result;
					gBlueEvent.MonsterCount++;
					CLog.LogAdd("[BlueEvent]: Add Monsters -> Index:%d, Map:%d, X:%d, Y:%d",result,gObj[result].MapNumber,gObj[result].X,gObj[result].Y);
				}
			}
		}
	}
}

void BlueEvent_EventStart(void * lpParam)
{
	while ( true )
	{
		if ( gBlueEvent.AuxDuration <= 0 )
		{
			SendMsg.MessageOutAll(0x00, "[BlueEvent]: Event has finished");
			gBlueEvent.Clear();
			_endthread();
		}
		gBlueEvent.AuxDuration--;
		Sleep(1000);
	}
}