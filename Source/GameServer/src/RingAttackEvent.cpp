#include "stdafx.h"
#include "RingAttackEvent.h"
#include "Defines.h"
#include "gObjMonster.h"
#include "Notice.H"
#include "GameMain.h"
#include "DSProtocol.h"
#include "LogProc.H"
#include "ReadScript.h"

static int g_RingEventMapNum[MAX_RINGMONSTER_MAP] = { MAP_INDEX_LORENCIA,MAP_INDEX_DEVIAS,MAP_INDEX_NORIA};
static int g_RingMapPosition[MAX_RINGMONSTER_MAP][4] =
{
	66, 44, 191, 197,
	111, 14, 232, 125,
	136, 53, 223, 181
};

CRingAttackEvent gRingAttackEvent;

CRingAttackEvent::CRingAttackEvent()
{
	gRingAttackEvent.Init();
}

CRingAttackEvent::~CRingAttackEvent()
{
	
}

void CRingAttackEvent::Start()
{
	if ( gRingAttackEvent.Enabled == 0 )
	{
		return;
	}
	if ( gRingAttackEvent.Started == 0 )
	{
		gRingAttackEvent.Started = 1;
		SendMsg.MessageOutAll(0x00, "White Wizard and his orcs has invaded us");
		gRingAttackEvent.AddMonsters();
		_beginthread(RingAttack_EventStart,0,NULL);
	}
}

void CRingAttackEvent::Init()
{
	gRingAttackEvent.Enabled				= GetPrivateProfileInt("WhiteWizard","Enabled",0,RINGAT_FILE_PATH);
	gRingAttackEvent.DurationTime			= GetPrivateProfileInt("WhiteWizard","DurationTime",0,RINGAT_FILE_PATH);
	gRingAttackEvent.AuxDurationTime		= gRingAttackEvent.DurationTime*60;
	gRingAttackEvent.Started				= 0;
	gRingAttackEvent.MonsterCount			= 0;

	for (int i=0;i<200;i++)
	{
		gRingAttackEvent.MonsterIndex[i] = -1;
	}

	for (int i=0;i<3;i++)
	{
		gRingAttackEvent.BossMapNumber[i] = 0;
		gRingAttackEvent.BossMapX[i] = 0;
		gRingAttackEvent.BossMapY[i] = 0;
	}
}

void CRingAttackEvent::End()
{
	gRingAttackEvent.AuxDurationTime		= gRingAttackEvent.DurationTime*60;
	gRingAttackEvent.Started				= 0;

	for (int i=0;i<3;i++)
	{
		gRingAttackEvent.BossMapNumber[i] = 0;
		gRingAttackEvent.BossMapX[i] = 0;
		gRingAttackEvent.BossMapY[i] = 0;
	}

	gRingAttackEvent.DeleteMonsters();
}

void CRingAttackEvent::DeleteMonsters()
{
	for (int i=0;i<200;i++)
	{
		if ( gRingAttackEvent.MonsterIndex[i] != -1 )
		{
			gObjDel(gRingAttackEvent.MonsterIndex[i]);
			gRingAttackEvent.MonsterIndex[i] = -1;
		}
	}
	gRingAttackEvent.MonsterCount = 0;
}

bool CRingAttackEvent::DropManager(LPOBJ lpObj)
{
	bool bResult = false;
	int MaxHitUser;
	int ItemNumber;
	BYTE Socket[5];
	Socket[0] = 0xFF;
	Socket[1] = 0xFF;
	Socket[2] = 0xFF;
	Socket[3] = 0xFF;
	Socket[4] = 0xFF;
	// ----
	switch ( lpObj->Class )
	{
	case 135:
		MaxHitUser = gObjMonsterTopHitDamageUser(lpObj);
		ItemNumber = ItemGetNumberMake(14, 13);
		ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, (BYTE)lpObj->X, (BYTE)lpObj->Y, ItemNumber, 0, 0, 0, 0, 0, MaxHitUser, 0, 0, Socket);
		bResult = true;
		break;
	case 136:
	case 137:
		if ( (rand()%100) < 30 )
		{
			MaxHitUser = gObjMonsterTopHitDamageUser(lpObj);
			ItemNumber = ItemGetNumberMake(13, 20);
			ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, (BYTE)lpObj->X, (BYTE)lpObj->Y, ItemNumber, 0, 30, 0, 0, 0, MaxHitUser, 0, 0, Socket);
		}
		else
		{
			MapC[lpObj->MapNumber].MoneyItemDrop(10000, (BYTE)lpObj->X, (BYTE)lpObj->Y);
		}
		bResult = true;
		break;
	}
	return bResult;
}

void CRingAttackEvent::AddMonsters()
{
	int OrcMob[2] = {136,137};
	int Mob;
	// ----
	for (int i=0;i<MAX_RINGMONSTER_MAP;i++)
	{
		int iMonsterIndex = gObjAddMonster(g_RingEventMapNum[i]);

		if( iMonsterIndex >= 0 )
		{
			LPOBJ lpObj = &gObj[iMonsterIndex];
			gObjSetMonster(iMonsterIndex, 135);

			while ( gMSetBase.GetBoxPosition(g_RingEventMapNum[i], g_RingMapPosition[i][0], g_RingMapPosition[i][1], g_RingMapPosition[i][2], g_RingMapPosition[i][3], lpObj->X, lpObj->Y) == 0 )
			{

			}

			gRingAttackEvent.BossMapX[i] = lpObj->X;
			gRingAttackEvent.BossMapY[i] = lpObj->Y; 
			
			lpObj->X = gRingAttackEvent.BossMapX[i];
			lpObj->TX = gRingAttackEvent.BossMapX[i];
			lpObj->MTX = gRingAttackEvent.BossMapX[i];
			lpObj->m_OldX = gRingAttackEvent.BossMapX[i];
			lpObj->StartX = gRingAttackEvent.BossMapX[i];
			lpObj->Y = gRingAttackEvent.BossMapY[i];
			lpObj->TY = gRingAttackEvent.BossMapY[i];
			lpObj->MTY = gRingAttackEvent.BossMapY[i];
			lpObj->m_OldY = gRingAttackEvent.BossMapY[i];
			lpObj->StartY = gRingAttackEvent.BossMapY[i];
			lpObj->MapNumber = g_RingEventMapNum[i];
			lpObj->Dir = rand()%8;			
			lpObj->Level = 87;
			lpObj->RegenTime = 0;
			lpObj->DieRegen = 0;
			lpObj->m_MoveRange = 1;

			gRingAttackEvent.BossMapNumber[i] = g_RingEventMapNum[i];
			gRingAttackEvent.MonsterIndex[gRingAttackEvent.MonsterCount] = iMonsterIndex;
			gRingAttackEvent.MonsterCount++;

			CLog.LogAdd("Make White Wizard Map %d Coords (%d,%d)",g_RingEventMapNum[i],gRingAttackEvent.BossMapX[i],gRingAttackEvent.BossMapY[i]);
		}

		for (int n=0;n<15;n++)
		{
			int iMonsterIndex = gObjAddMonster(g_RingEventMapNum[i]);

			if( iMonsterIndex >= 0 )
			{
				LPOBJ lpObj = &gObj[iMonsterIndex];
				Mob = OrcMob[rand()%2];
				if ( Mob < 136 )
				{
					Mob = 136;
				}
				else if ( Mob > 137 )
				{
					Mob = 137;
				}
				gObjSetMonster(iMonsterIndex, Mob);

				gMSetBase.GetBoxPosition(g_RingEventMapNum[i], gRingAttackEvent.BossMapX[i]-4, gRingAttackEvent.BossMapY[i]-4,gRingAttackEvent.BossMapX[i]+4, gRingAttackEvent.BossMapY[i]+4, lpObj->X, lpObj->Y);

				lpObj->TX = lpObj->X;
				lpObj->MTX = lpObj->X;
				lpObj->m_OldX = lpObj->X;
				lpObj->StartX = lpObj->X;
				lpObj->Y = lpObj->Y;
				lpObj->TY = lpObj->Y;
				lpObj->MTY = lpObj->Y;
				lpObj->m_OldY = lpObj->Y;
				lpObj->StartY = lpObj->Y;
				lpObj->MapNumber = g_RingEventMapNum[i];
				lpObj->Dir = rand()%8;			
				lpObj->Level = 87;
				lpObj->RegenTime = 0;
				lpObj->DieRegen = 0;
				lpObj->m_MoveRange = 1;

				gRingAttackEvent.MonsterIndex[gRingAttackEvent.MonsterCount] = iMonsterIndex;
				gRingAttackEvent.MonsterCount++;

				CLog.LogAdd("Make Orc Map %d Coords (%d,%d)",lpObj->MapNumber,lpObj->X,lpObj->Y);
			}
		}
	}
}

bool CRingAttackEvent::CanMove(LPOBJ lpObj, BYTE tx, BYTE ty)
{
	if ( lpObj->Class == 135 || lpObj->Class == 136 || lpObj->Class == 137 )
	{
		tx -= lpObj->StartX;
		ty -= lpObj->StartY;
		int dis = sqrt((float)(tx*tx + ty*ty));
	
		if ( dis > 3 )
		{
			return false;
		}
		else
		{
			return true;
		}
	}
	else
	{
		return true;
	}
}

void RingAttack_EventStart(void * lpParam)
{
	while ( true )
	{
		if ( gRingAttackEvent.AuxDurationTime <= 0 )
		{
			gRingAttackEvent.End();
			_endthread();
		}
		gRingAttackEvent.AuxDurationTime--;
		Sleep(1000);
	}
}