//---------------------------------------------------------------------------
// # Project:		HaRiBO MU GameServer - Supported Season 6
// # Company:		RealCoderZ MU Development © 2011
// # Description:	Evil Santa Invasion Event
// # Developed By:	M.E.S & WolF
//---------------------------------------------------------------------------

#include "../Header Files/stdafx.h"
#include "../Header Files/SantaEvent.h"
#include "../Header Files/Defines.h"
#include "../Header Files/gObjMonster.h"
#include "../Header Files/Notice.H"
#include "../Header Files/GameMain.h"
#include "../Header Files/DSProtocol.h"
#include "../Header Files/LogProc.H"
#include "../Header Files/ReadScript.h"

static int g_SantaEventMapNum[1] = { MAP_INDEX_LORENCIA};
static int g_SantaMapPosition[1][4] =
{
	66, 44, 191, 197
};

CSantaEvent gSantaEvent;

CSantaEvent::CSantaEvent()
{
	gSantaEvent.Init();
}

CSantaEvent::~CSantaEvent()
{
	
}

void CSantaEvent::Start()
{
	if ( gSantaEvent.Enabled == 0 )
	{
		return;
	}
	if ( gSantaEvent.Started == 0 )
	{
		gSantaEvent.Started = 1;
		SendMsg.MessageOutAll(0x00, "Evil Santa and his cursed goblins has invaded us");
		gSantaEvent.AddMonsters();
		_beginthread(SantaInvasion_EventStart,0,NULL);
	}
}

void CSantaEvent::Init()
{
	gSantaEvent.Enabled				= GetPrivateProfileInt("SantaEvent","Enabled",0,SANTA_FILE_PATH);
	gSantaEvent.DurationTime		= GetPrivateProfileInt("SantaEvent","DurationTime",0,SANTA_FILE_PATH);
	gSantaEvent.AuxDurationTime		= gSantaEvent.DurationTime*60;
	gSantaEvent.Started				= 0;
	gSantaEvent.MonsterCount		= 0;

	for (int i=0;i<200;i++)
	{
		gSantaEvent.MonsterIndex[i] = -1;
	}

	gSantaEvent.BossMapX = 0;
	gSantaEvent.BossMapY = 0;
}

void CSantaEvent::End()
{
	gSantaEvent.AuxDurationTime		= gSantaEvent.DurationTime*60;
	gSantaEvent.Started				= 0;

	gSantaEvent.BossMapX = 0;
	gSantaEvent.BossMapY = 0;

	gSantaEvent.DeleteMonsters();
}

void CSantaEvent::DeleteMonsters()
{
	for (int i=0;i<200;i++)
	{
		if ( gSantaEvent.MonsterIndex[i] != -1 )
		{
			gObjDel(gSantaEvent.MonsterIndex[i]);
			gSantaEvent.MonsterIndex[i] = -1;
		}
	}
	gSantaEvent.MonsterCount = 0;
}

bool CSantaEvent::DropManager(LPOBJ lpObj)
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
	case 476:
		MaxHitUser = gObjMonsterTopHitDamageUser(lpObj);
		ItemNumber = ItemGetNumberMake(14, 13);
		ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, (BYTE)lpObj->X, (BYTE)lpObj->Y, ItemNumber, 0, 0, 0, 0, 0, MaxHitUser, 0,0, Socket);
		bResult = true;
		break;
	case 466:
		if ( (rand()%100) < 30 )
		{
			MaxHitUser = gObjMonsterTopHitDamageUser(lpObj);
			ItemNumber = ItemGetNumberMake(13, 66);
			ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, (BYTE)lpObj->X, (BYTE)lpObj->Y, ItemNumber, 0, 0, 0, 0, 0, MaxHitUser, 0, 0, Socket);
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

void CSantaEvent::AddMonsters()
{
	int iMonsterIndex = gObjAddMonster(0);

	if( iMonsterIndex >= 0 )
	{
		LPOBJ lpObj = &gObj[iMonsterIndex];
		gObjSetMonster(iMonsterIndex, 476);

		while ( gMSetBase.GetBoxPosition(0, g_SantaMapPosition[0][0], g_SantaMapPosition[0][1], g_SantaMapPosition[0][2], g_SantaMapPosition[0][3], lpObj->X, lpObj->Y) == 0 )
		{

		}

		gSantaEvent.BossMapX = lpObj->X;
		gSantaEvent.BossMapY = lpObj->Y; 
			
		lpObj->X = gSantaEvent.BossMapX;
		lpObj->TX = gSantaEvent.BossMapX;
		lpObj->MTX = gSantaEvent.BossMapX;
		lpObj->m_OldX = gSantaEvent.BossMapX;
		lpObj->StartX = gSantaEvent.BossMapX;
		lpObj->Y = gSantaEvent.BossMapY;
		lpObj->TY = gSantaEvent.BossMapY;
		lpObj->MTY = gSantaEvent.BossMapY;
		lpObj->m_OldY = gSantaEvent.BossMapY;
		lpObj->StartY = gSantaEvent.BossMapY;
		lpObj->MapNumber = 0;
		lpObj->Dir = rand()%8;			
		lpObj->Level = 87;
		lpObj->RegenTime = 0;
		lpObj->DieRegen = 0;
		lpObj->m_MoveRange = 1;

		gSantaEvent.MonsterIndex[gSantaEvent.MonsterCount] = iMonsterIndex;
		gSantaEvent.MonsterCount++;

		CLog.LogAdd("Make Evil Santa Claus Map %d Coords (%d,%d)",0,gSantaEvent.BossMapX,gSantaEvent.BossMapY);
	}

	for (int n=0;n<15;n++)
	{
		int iMonsterIndex = gObjAddMonster(0);

		if( iMonsterIndex >= 0 )
		{
			LPOBJ lpObj = &gObj[iMonsterIndex];

			gObjSetMonster(iMonsterIndex, 466);

			gMSetBase.GetBoxPosition(0, gSantaEvent.BossMapX-4, gSantaEvent.BossMapY-4,gSantaEvent.BossMapX+4, gSantaEvent.BossMapY+4, lpObj->X, lpObj->Y);

			lpObj->TX = lpObj->X;
			lpObj->MTX = lpObj->X;
			lpObj->m_OldX = lpObj->X;
			lpObj->StartX = lpObj->X;
			lpObj->Y = lpObj->Y;
			lpObj->TY = lpObj->Y;
			lpObj->MTY = lpObj->Y;
			lpObj->m_OldY = lpObj->Y;
			lpObj->StartY = lpObj->Y;
			lpObj->MapNumber = 0;
			lpObj->Dir = rand()%8;			
			lpObj->Level = 87;
			lpObj->RegenTime = 0;
			lpObj->DieRegen = 0;
			lpObj->m_MoveRange = 1;

			gSantaEvent.MonsterIndex[gSantaEvent.MonsterCount] = iMonsterIndex;
			gSantaEvent.MonsterCount++;

			CLog.LogAdd("Make Cursed Goblin Map %d Coords (%d,%d)",lpObj->MapNumber,lpObj->X,lpObj->Y);
		}
	}
}

bool CSantaEvent::CanMove(LPOBJ lpObj, BYTE tx, BYTE ty)
{
	if ( lpObj->Class == 466 || lpObj->Class == 476 )
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

void SantaInvasion_EventStart(void * lpParam)
{
	while ( true )
	{
		if ( gSantaEvent.AuxDurationTime <= 0 )
		{
			gSantaEvent.End();
			_endthread();
		}
		gSantaEvent.AuxDurationTime--;
		Sleep(1000);
	}
}

void CSantaEvent::LittleSantaBuffer(LPOBJ lpObj,int Type)
{
	switch ( Type )
	{
	case 0:
		gSantaEvent.LittleSantaCancelBuffer(lpObj);
		GCStateInfoSend(lpObj,1,92);
		lpObj->LittleSantaTime1 = 30*60;
		lpObj->AddLife+=500;
		GCReFillSend(lpObj->m_Index,lpObj->MaxLife+lpObj->AddLife,0xFE,0,lpObj->iMaxShield+lpObj->iAddShield);
		break;
	case 1:
		gSantaEvent.LittleSantaCancelBuffer(lpObj);
		GCStateInfoSend(lpObj,1,95);
		lpObj->LittleSantaTime2 = 30*60;
		lpObj->m_Defense+=100;
		break;
	case 2:
		lpObj->Life = lpObj->MaxLife+lpObj->AddLife;
		GCReFillSend(lpObj->m_Index, lpObj->Life, 0xFF, 0, lpObj->iShield);
		break;
	case 3:
		lpObj->Mana = lpObj->MaxMana+lpObj->AddMana;
		GCManaSend(lpObj->m_Index,lpObj->Mana,0xFF,0,lpObj->BP);
		break;
	case 4:
		gSantaEvent.LittleSantaCancelBuffer(lpObj);
		GCStateInfoSend(lpObj,1,93);
		lpObj->LittleSantaTime3 = 30*60;
		lpObj->AddMana+=500;
		GCManaSend(lpObj->m_Index,lpObj->MaxMana+lpObj->AddMana,0xFE,0,lpObj->MaxBP+lpObj->AddBP);
		break;
	case 5:
		gSantaEvent.LittleSantaCancelBuffer(lpObj);
		GCStateInfoSend(lpObj,1,96);
		lpObj->LittleSantaTime4 = 30*60;
		lpObj->m_AttackSpeed+=15;
		lpObj->m_MagicSpeed+=15;
		break;
	case 6:
		gSantaEvent.LittleSantaCancelBuffer(lpObj);
		GCStateInfoSend(lpObj,1,94);
		lpObj->LittleSantaTime5 = 30*60;
		lpObj->m_AttackDamageLeft+=30;
		lpObj->m_AttackDamageRight+=30;
		lpObj->m_AttackDamageMinLeft+=30;
		lpObj->m_AttackDamageMaxLeft+=30;
		lpObj->m_AttackDamageMinRight+=30;
		lpObj->m_AttackDamageMaxRight+=30;
		lpObj->m_AttackDamageMax+=30;
		lpObj->m_AttackDamageMin+=30;
		lpObj->m_MagicDamageMax+=30;
		lpObj->m_MagicDamageMin+=30;
		break;
	case 7:
		gSantaEvent.LittleSantaCancelBuffer(lpObj);
		GCStateInfoSend(lpObj,1,97);
		lpObj->LittleSantaTime6 = 30*60;
		break;
	}
}

void CSantaEvent::LittleSantaCancelBuffer(LPOBJ lpObj)
{
	if ( lpObj->LittleSantaTime1 > 0 )
	{
		lpObj->LittleSantaTime1 = 0;
		lpObj->AddLife-=500;
		if ( lpObj->Life > (lpObj->MaxLife+lpObj->AddLife) )
		{
			lpObj->Life = lpObj->MaxLife+lpObj->AddLife;
			GCReFillSend(lpObj->m_Index,lpObj->Life,0xFF,0,lpObj->iShield);
		}
		GCReFillSend(lpObj->m_Index,lpObj->MaxLife + lpObj->AddLife,0xFE,0,lpObj->iMaxShield + lpObj->iAddShield);
		GCStateInfoSend(lpObj,0,92);
	}

	if ( lpObj->LittleSantaTime2 > 0 )
	{
		lpObj->LittleSantaTime2 = 0;
		lpObj->m_Defense-=100;
		GCStateInfoSend(lpObj,0,95);
	}

	if ( lpObj->LittleSantaTime3 > 0 )
	{
		lpObj->LittleSantaTime3 = 0;
		lpObj->AddMana-=500;
		if ( lpObj->Mana > (lpObj->MaxMana+lpObj->AddMana) )
		{
			lpObj->Mana = lpObj->MaxMana+lpObj->AddMana;
			GCManaSend(lpObj->m_Index,lpObj->Mana,0xFF,0,lpObj->BP);
		}
		GCManaSend(lpObj->m_Index,lpObj->MaxMana+lpObj->AddMana,0xFE,0,lpObj->MaxBP+lpObj->AddBP);
		GCStateInfoSend(lpObj,0,93);
	}

	if ( lpObj->LittleSantaTime4 > 0 )
	{
		lpObj->LittleSantaTime4 = 0;
		lpObj->m_AttackSpeed-=15;
		lpObj->m_MagicSpeed-=15;
		GCStateInfoSend(lpObj,0,96);
	}

	if ( lpObj->LittleSantaTime5 > 0 )
	{
		lpObj->LittleSantaTime5 = 0;
		lpObj->m_AttackDamageLeft-=30;
		lpObj->m_AttackDamageRight-=30;
		lpObj->m_AttackDamageMinLeft-=30;
		lpObj->m_AttackDamageMaxLeft-=30;
		lpObj->m_AttackDamageMinRight-=30;
		lpObj->m_AttackDamageMaxRight-=30;
		lpObj->m_AttackDamageMax-=30;
		lpObj->m_AttackDamageMin-=30;
		lpObj->m_MagicDamageMax-=30;
		lpObj->m_MagicDamageMin-=30;
		GCStateInfoSend(lpObj,0,94);
	}

	if ( lpObj->LittleSantaTime6 > 0 )
	{
		lpObj->LittleSantaTime6 = 0;
		GCStateInfoSend(lpObj,0,97);
	}
}