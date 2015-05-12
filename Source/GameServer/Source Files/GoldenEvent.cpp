#include "../Header Files/stdafx.h"
#include "../Header Files/GoldenEvent.h"
#include "../Header Files/Defines.h"
#include "../Header Files/LogProc.H"
#include "../Header Files/GameMain.h"
#include "../Header Files/gObjMonster.h"
#include "../Header Files/DSProtocol.h"
#include "../Header Files/Notice.H"

CGoldenEvent gGoldenEvent;

CGoldenEvent::CGoldenEvent()
{
	gGoldenEvent.Init();
}

CGoldenEvent::~CGoldenEvent()
{
	
}

void CGoldenEvent::Run()
{
	if ( gGoldenEvent.Enabled == 0 )
	{
		return;
	}
	gGoldenEvent.SendInvasionEffect();
}

void CGoldenEvent::Init()
{
	// ---- Map Set to -1
	gGoldenEvent.m_BossGoldDragonMapNumber = 0;
	gGoldenEvent.m_BossGoldGreatDragonMapNumber[0] = -1;
	gGoldenEvent.m_BossGoldGreatDragonMapNumber[1] = -1;
	// ----
	gGoldenEvent.Enabled					= GetPrivateProfileInt("GoldenEvent","Enabled",0,GOLDEN_FILE_PATH);
	gGoldenEvent.DurationTime				= GetPrivateProfileInt("GoldenEvent","DurationTime",0,GOLDEN_FILE_PATH);
	gGoldenEvent.AuxDurationTime			= gGoldenEvent.DurationTime*60;
	gGoldenEvent.Started					= 0;

	for (int i=0;i<200;i++)
	{
		gGoldenEvent.MonsterIndex[i] = -1;
	}

	gGoldenEvent.MonsterCount = 0;

	return;
}

void CGoldenEvent::RegenGoldGoblen()
{
	int iMonsterIndex = gObjAddMonster(3);

	if( iMonsterIndex >= 0 )
	{
		LPOBJ lpObj = &gObj[iMonsterIndex];
		gObjSetMonster(iMonsterIndex, 78);

		while ( gMSetBase.GetBoxPosition(3, 50, 50, 200, 200, lpObj->X, lpObj->Y) == 0 )
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
		lpObj->MapNumber = 3;
		lpObj->Dir = rand()%8;			
		lpObj->Level = 20;
		lpObj->RegenTime = 0;
		lpObj->DieRegen = 0;
		lpObj->m_MoveRange = 2;

		gGoldenEvent.MonsterIndex[gGoldenEvent.MonsterCount] = iMonsterIndex;
		gGoldenEvent.MonsterCount++;

		CLog.LogAdd("Make Golden Goblin: %d, %d,%d", lpObj->MapNumber, lpObj->X, lpObj->Y);
	}
}

void CGoldenEvent::RegenGoldTitan()
{
	int iMonsterIndex;

	iMonsterIndex = gObjAddMonster(2);

	if( iMonsterIndex >= 0 )
	{
		LPOBJ lpObj = &gObj[iMonsterIndex];
		gObjSetMonster(iMonsterIndex, 53);

		while ( gMSetBase.GetBoxPosition(2, 50, 50, 200, 200, lpObj->X, lpObj->Y) == 0 )
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
		lpObj->MapNumber = 2;
		lpObj->Dir = rand()%8;			
		lpObj->Level = 20;
		lpObj->RegenTime = 0;
		lpObj->DieRegen = 0;
		lpObj->m_MoveRange = 2;

		gGoldenEvent.MonsterIndex[gGoldenEvent.MonsterCount] = iMonsterIndex;
		gGoldenEvent.MonsterCount++;

		gGoldenEvent.m_BossGoldTitanMapX = lpObj->X;
		gGoldenEvent.m_BossGoldTitanMapY = lpObj->Y;

		CLog.LogAdd("Make Golden Titan: %d, %d,%d",	lpObj->MapNumber, lpObj->X, lpObj->Y);
	}

	for (int i=0;i<7;i++)
	{
		iMonsterIndex = gObjAddMonster(2);

		if( iMonsterIndex >= 0 )
		{
			LPOBJ lpObj = &gObj[iMonsterIndex];
			gObjSetMonster(iMonsterIndex, 54);

			gMSetBase.GetBoxPosition(2, gGoldenEvent.m_BossGoldTitanMapX-4, gGoldenEvent.m_BossGoldTitanMapY-4,gGoldenEvent.m_BossGoldTitanMapX+4, gGoldenEvent.m_BossGoldTitanMapY+4, lpObj->X, lpObj->Y);

			lpObj->TX = lpObj->X;
			lpObj->MTX = lpObj->X;
			lpObj->m_OldX = lpObj->X;
			lpObj->StartX = lpObj->X;
			lpObj->TY = lpObj->Y;
			lpObj->MTY = lpObj->Y;
			lpObj->m_OldY = lpObj->Y;
			lpObj->StartY = lpObj->Y;
			lpObj->MapNumber = 2;
			lpObj->Dir = rand()%8;			
			lpObj->Level = 20;
			lpObj->RegenTime = 0;
			lpObj->DieRegen = 0;
			lpObj->m_MoveRange = 2;

			gGoldenEvent.MonsterIndex[gGoldenEvent.MonsterCount] = iMonsterIndex;
			gGoldenEvent.MonsterCount++;

			CLog.LogAdd("Make Golden Soldier: %d, %d,%d",	lpObj->MapNumber, lpObj->X, lpObj->Y);
		}
	}
}

void CGoldenEvent::RegenGoldDercon()
{
	int MapNumber=0;
	int Map[5]={0,3,2,4,51};

	MapNumber = Map[rand()%5];

	GCMapEventStateSend(MapNumber, 1, 3);

	int iMonsterIndex = gObjAddMonster(MapNumber);

	if( iMonsterIndex >= 0 )
	{
		LPOBJ lpObj = &gObj[iMonsterIndex];
		gObjSetMonster(iMonsterIndex, 79);

		while ( gMSetBase.GetBoxPosition(MapNumber, 50, 50, 200, 200, lpObj->X, lpObj->Y) == 0 )
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
		lpObj->MapNumber = MapNumber;
		lpObj->Dir = rand()%8;			
		lpObj->Level = 20;
		lpObj->RegenTime = 0;
		lpObj->DieRegen = 0;
		lpObj->m_MoveRange = 2;

		gGoldenEvent.MonsterIndex[gGoldenEvent.MonsterCount] = iMonsterIndex;
		gGoldenEvent.MonsterCount++;
		gGoldenEvent.m_BossGoldDragonMapNumber = lpObj->MapNumber;

		CLog.LogAdd("Make Golden Dragon: %d, %d,%d", lpObj->MapNumber, lpObj->X, lpObj->Y);
	}

	//this->CheckGoldDercon(MapNumber);
}

void CGoldenEvent::RegenGoldLizardKing()
{
	int iMonsterIndex;

	iMonsterIndex = gObjAddMonster(7);

	if( iMonsterIndex >= 0 )
	{
		LPOBJ lpObj = &gObj[iMonsterIndex];
		gObjSetMonster(iMonsterIndex, 80);

		while ( gMSetBase.GetBoxPosition(7, 50, 50, 200, 200, lpObj->X, lpObj->Y) == 0 )
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
		lpObj->MapNumber = 7;
		lpObj->Dir = rand()%8;			
		lpObj->Level = 20;
		lpObj->RegenTime = 0;
		lpObj->DieRegen = 0;
		lpObj->m_MoveRange = 2;

		gGoldenEvent.MonsterIndex[gGoldenEvent.MonsterCount] = iMonsterIndex;
		gGoldenEvent.MonsterCount++;

		gGoldenEvent.m_BossGoldLizardKingMapX = lpObj->X;
		gGoldenEvent.m_BossGoldLizardKingMapY = lpObj->Y;

		CLog.LogAdd("Make Golden Lizard: %d, %d,%d", lpObj->MapNumber, lpObj->X, lpObj->Y);
	}

	for (int i=0;i<7;i++)
	{
		iMonsterIndex = gObjAddMonster(7);

		if( iMonsterIndex >= 0 )
		{
			LPOBJ lpObj = &gObj[iMonsterIndex];
			gObjSetMonster(iMonsterIndex, 81);

			gMSetBase.GetBoxPosition(7, gGoldenEvent.m_BossGoldLizardKingMapX-4, gGoldenEvent.m_BossGoldLizardKingMapY-4,gGoldenEvent.m_BossGoldLizardKingMapX+4, gGoldenEvent.m_BossGoldLizardKingMapY+4, lpObj->X, lpObj->Y);

			lpObj->TX = lpObj->X;
			lpObj->MTX = lpObj->X;
			lpObj->m_OldX = lpObj->X;
			lpObj->StartX = lpObj->X;
			lpObj->TY = lpObj->Y;
			lpObj->MTY = lpObj->Y;
			lpObj->m_OldY = lpObj->Y;
			lpObj->StartY = lpObj->Y;
			lpObj->MapNumber = 7;
			lpObj->Dir = rand()%8;			
			lpObj->Level = 20;
			lpObj->RegenTime = 0;
			lpObj->DieRegen = 0;
			lpObj->m_MoveRange = 2;

			gGoldenEvent.MonsterIndex[gGoldenEvent.MonsterCount] = iMonsterIndex;
			gGoldenEvent.MonsterCount++;

			CLog.LogAdd("Make Golden Vepar: %d, %d,%d", lpObj->MapNumber, lpObj->X, lpObj->Y);
		}
	}
}

void CGoldenEvent::RegenGoldTantalo()
{
	int iMonsterIndex;

	iMonsterIndex = gObjAddMonster(8);

	if( iMonsterIndex >= 0 )
	{
		LPOBJ lpObj = &gObj[iMonsterIndex];
		gObjSetMonster(iMonsterIndex, 82);

		while ( gMSetBase.GetBoxPosition(8, 50, 50, 200, 200, lpObj->X, lpObj->Y) == 0 )
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
		lpObj->MapNumber = 8;
		lpObj->Dir = rand()%8;			
		lpObj->Level = 20;
		lpObj->RegenTime = 0;
		lpObj->DieRegen = 0;
		lpObj->m_MoveRange = 2;

		gGoldenEvent.MonsterIndex[gGoldenEvent.MonsterCount] = iMonsterIndex;
		gGoldenEvent.MonsterCount++;

		gGoldenEvent.m_BossGoldTantalloMapX = lpObj->X;
		gGoldenEvent.m_BossGoldTantalloMapY = lpObj->Y;

		CLog.LogAdd("Make Golden Tantalo: %d, %d,%d", lpObj->MapNumber, lpObj->X, lpObj->Y);
	}

	for (int i=0;i<7;i++)
	{
		iMonsterIndex = gObjAddMonster(8);

		if( iMonsterIndex >= 0 )
		{
			LPOBJ lpObj = &gObj[iMonsterIndex];
			gObjSetMonster(iMonsterIndex, 83);

			gMSetBase.GetBoxPosition(8, gGoldenEvent.m_BossGoldTantalloMapX-4, gGoldenEvent.m_BossGoldTantalloMapY-4,gGoldenEvent.m_BossGoldTantalloMapX+4, gGoldenEvent.m_BossGoldTantalloMapY+4, lpObj->X, lpObj->Y);

			lpObj->TX = lpObj->X;
			lpObj->MTX = lpObj->X;
			lpObj->m_OldX = lpObj->X;
			lpObj->StartX = lpObj->X;
			lpObj->TY = lpObj->Y;
			lpObj->MTY = lpObj->Y;
			lpObj->m_OldY = lpObj->Y;
			lpObj->StartY = lpObj->Y;
			lpObj->MapNumber = 8;
			lpObj->Dir = rand()%8;			
			lpObj->Level = 20;
			lpObj->RegenTime = 0;
			lpObj->DieRegen = 0;
			lpObj->m_MoveRange = 2;

			gGoldenEvent.MonsterIndex[gGoldenEvent.MonsterCount] = iMonsterIndex;
			gGoldenEvent.MonsterCount++;

			CLog.LogAdd("Make Golden Wheel: %d, %d,%d", lpObj->MapNumber, lpObj->X, lpObj->Y);
		}
	}
}

void CGoldenEvent::RegenGoldRabbit()
{
	int iMonsterIndex = gObjAddMonster(51);

	if( iMonsterIndex >= 0 )
	{
		LPOBJ lpObj = &gObj[iMonsterIndex];
		gObjSetMonster(iMonsterIndex, 502);

		while ( gMSetBase.GetBoxPosition(51, 50, 50, 200, 200, lpObj->X, lpObj->Y) == 0 )
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
		lpObj->MapNumber = 51;
		lpObj->Dir = rand()%8;			
		lpObj->Level = 20;
		lpObj->RegenTime = 0;
		lpObj->DieRegen = 0;
		lpObj->m_MoveRange = 2;

		gGoldenEvent.MonsterIndex[gGoldenEvent.MonsterCount] = iMonsterIndex;
		gGoldenEvent.MonsterCount++;

		CLog.LogAdd("Make Golden Rabbit: %d, %d,%d", lpObj->MapNumber, lpObj->X, lpObj->Y);
	}
}

void CGoldenEvent::RegenGoldKnight()
{
	int iMonsterIndex = gObjAddMonster(1);

	if( iMonsterIndex >= 0 )
	{
		LPOBJ lpObj = &gObj[iMonsterIndex];
		gObjSetMonster(iMonsterIndex, 493);

		while ( gMSetBase.GetBoxPosition(1, 50, 50, 200, 200, lpObj->X, lpObj->Y) == 0 )
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
		lpObj->MapNumber = 1;
		lpObj->Dir = rand()%8;			
		lpObj->Level = 20;
		lpObj->RegenTime = 0;
		lpObj->DieRegen = 0;
		lpObj->m_MoveRange = 2;

		gGoldenEvent.MonsterIndex[gGoldenEvent.MonsterCount] = iMonsterIndex;
		gGoldenEvent.MonsterCount++;

		CLog.LogAdd("Make Golden Dark Knight: %d, %d,%d", lpObj->MapNumber, lpObj->X, lpObj->Y);
	}
}

void CGoldenEvent::RegenGoldDevil()
{
	int iMonsterIndex = gObjAddMonster(4);

	if( iMonsterIndex >= 0 )
	{
		LPOBJ lpObj = &gObj[iMonsterIndex];
		gObjSetMonster(iMonsterIndex, 494);

		while ( gMSetBase.GetBoxPosition(4, 50, 50, 200, 200, lpObj->X, lpObj->Y) == 0 )
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
		lpObj->MapNumber = 4;
		lpObj->Dir = rand()%8;			
		lpObj->Level = 20;
		lpObj->RegenTime = 0;
		lpObj->DieRegen = 0;
		lpObj->m_MoveRange = 2;

		gGoldenEvent.MonsterIndex[gGoldenEvent.MonsterCount] = iMonsterIndex;
		gGoldenEvent.MonsterCount++;

		CLog.LogAdd("Make Golden Devil: %d, %d,%d", lpObj->MapNumber, lpObj->X, lpObj->Y);
	}
}

void CGoldenEvent::RegenGoldStoneGolem()
{
	int iMonsterIndex = gObjAddMonster(33);

	if( iMonsterIndex >= 0 )
	{
		LPOBJ lpObj = &gObj[iMonsterIndex];
		gObjSetMonster(iMonsterIndex, 495);

		while ( gMSetBase.GetBoxPosition(33, 50, 50, 200, 200, lpObj->X, lpObj->Y) == 0 )
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
		lpObj->MapNumber = 33;
		lpObj->Dir = rand()%8;			
		lpObj->Level = 20;
		lpObj->RegenTime = 0;
		lpObj->DieRegen = 0;
		lpObj->m_MoveRange = 2;

		gGoldenEvent.MonsterIndex[gGoldenEvent.MonsterCount] = iMonsterIndex;
		gGoldenEvent.MonsterCount++;

		CLog.LogAdd("Make Golden Stone Golem: %d, %d,%d", lpObj->MapNumber, lpObj->X, lpObj->Y);
	}
}

void CGoldenEvent::RegenGoldCrust()
{
	int iMonsterIndex = gObjAddMonster(10);

	if( iMonsterIndex >= 0 )
	{
		LPOBJ lpObj = &gObj[iMonsterIndex];
		gObjSetMonster(iMonsterIndex, 496);

		while ( gMSetBase.GetBoxPosition(10, 50, 50, 200, 200, lpObj->X, lpObj->Y) == 0 )
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
		lpObj->MapNumber = 10;
		lpObj->Dir = rand()%8;			
		lpObj->Level = 20;
		lpObj->RegenTime = 0;
		lpObj->DieRegen = 0;
		lpObj->m_MoveRange = 2;

		gGoldenEvent.MonsterIndex[gGoldenEvent.MonsterCount] = iMonsterIndex;
		gGoldenEvent.MonsterCount++;

		CLog.LogAdd("Make Golden Crust: %d, %d,%d", lpObj->MapNumber, lpObj->X, lpObj->Y);
	}
}

void CGoldenEvent::RegenGoldSatyros()
{
	int iMonsterIndex = gObjAddMonster(38);

	if( iMonsterIndex >= 0 )
	{
		LPOBJ lpObj = &gObj[iMonsterIndex];
		gObjSetMonster(iMonsterIndex, 497);

		while ( gMSetBase.GetBoxPosition(38, 50, 50, 200, 200, lpObj->X, lpObj->Y) == 0 )
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
		lpObj->MapNumber = 38;
		lpObj->Dir = rand()%8;			
		lpObj->Level = 20;
		lpObj->RegenTime = 0;
		lpObj->DieRegen = 0;
		lpObj->m_MoveRange = 2;

		gGoldenEvent.MonsterIndex[gGoldenEvent.MonsterCount] = iMonsterIndex;
		gGoldenEvent.MonsterCount++;

		CLog.LogAdd("Make Golden Satyros: %d, %d,%d", lpObj->MapNumber, lpObj->X, lpObj->Y);
	}
}

void CGoldenEvent::RegenGoldTwinTail()
{
	int iMonsterIndex = gObjAddMonster(39);

	if( iMonsterIndex >= 0 )
	{
		LPOBJ lpObj = &gObj[iMonsterIndex];
		gObjSetMonster(iMonsterIndex, 498);

		while ( gMSetBase.GetBoxPosition(39, 50, 50, 200, 200, lpObj->X, lpObj->Y) == 0 )
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
		lpObj->MapNumber = 39;
		lpObj->Dir = rand()%8;			
		lpObj->Level = 20;
		lpObj->RegenTime = 0;
		lpObj->DieRegen = 0;
		lpObj->m_MoveRange = 2;

		gGoldenEvent.MonsterIndex[gGoldenEvent.MonsterCount] = iMonsterIndex;
		gGoldenEvent.MonsterCount++;

		CLog.LogAdd("Make Golden Twin Tail: %d, %d,%d", lpObj->MapNumber, lpObj->X, lpObj->Y);
	}
}

void CGoldenEvent::RegenGoldIronKnight()
{
	int iMonsterIndex = gObjAddMonster(57);

	if( iMonsterIndex >= 0 )
	{
		LPOBJ lpObj = &gObj[iMonsterIndex];
		gObjSetMonster(iMonsterIndex, 499);

		while ( gMSetBase.GetBoxPosition(57, 50, 50, 200, 200, lpObj->X, lpObj->Y) == 0 )
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
		lpObj->MapNumber = 57;
		lpObj->Dir = rand()%8;			
		lpObj->Level = 20;
		lpObj->RegenTime = 0;
		lpObj->DieRegen = 0;
		lpObj->m_MoveRange = 2;

		gGoldenEvent.MonsterIndex[gGoldenEvent.MonsterCount] = iMonsterIndex;
		gGoldenEvent.MonsterCount++;

		CLog.LogAdd("Make Golden Iron Knight: %d, %d,%d", lpObj->MapNumber, lpObj->X, lpObj->Y);
	}
}

void CGoldenEvent::RegenGoldNapin()
{
	int iMonsterIndex = gObjAddMonster(56);

	if( iMonsterIndex >= 0 )
	{
		LPOBJ lpObj = &gObj[iMonsterIndex];
		gObjSetMonster(iMonsterIndex, 500);

		while ( gMSetBase.GetBoxPosition(56, 50, 50, 200, 200, lpObj->X, lpObj->Y) == 0 )
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
		lpObj->MapNumber = 56;
		lpObj->Dir = rand()%8;			
		lpObj->Level = 20;
		lpObj->RegenTime = 0;
		lpObj->DieRegen = 0;
		lpObj->m_MoveRange = 2;

		gGoldenEvent.MonsterIndex[gGoldenEvent.MonsterCount] = iMonsterIndex;
		gGoldenEvent.MonsterCount++;

		CLog.LogAdd("Make Golden Goblin: %d, %d,%d", lpObj->MapNumber, lpObj->X, lpObj->Y);
	}
}

void CGoldenEvent::RegenGoldGreatDragon()
{
	int MapNumber=-1;
	int Map[3]={37,38,57};

	//this->CheckGoldGreatDragon(MapNumber);

	MapNumber = Map[rand()%3];

	int iMonsterIndex = gObjAddMonster(MapNumber);

	if( iMonsterIndex >= 0 )
	{
		LPOBJ lpObj = &gObj[iMonsterIndex];
		gObjSetMonster(iMonsterIndex, 501);

		while ( gMSetBase.GetBoxPosition(MapNumber, 50, 50, 200, 200, lpObj->X, lpObj->Y) == 0 )
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
		lpObj->MapNumber = MapNumber;
		lpObj->Dir = rand()%8;			
		lpObj->Level = 20;
		lpObj->RegenTime = 0;
		lpObj->DieRegen = 0;
		lpObj->m_MoveRange = 2;

		gGoldenEvent.MonsterIndex[gGoldenEvent.MonsterCount] = iMonsterIndex;
		gGoldenEvent.MonsterCount++;

		CLog.LogAdd("Make Golden Great Dragon: %d, %d,%d", lpObj->MapNumber, lpObj->X, lpObj->Y);
	}
	//this->CheckGoldGreatDragon(MapNumber);
}

void CGoldenEvent::Start()
{
	if ( gGoldenEvent.Enabled == 0 )
	{
		return;
	}
	if ( gGoldenEvent.Started == 0 )
	{
		gGoldenEvent.Started = 1;

		SendMsg.MessageOutAll(0x00, "Golden Invasion Started");

		gGoldenEvent.RegenGoldGoblen();
		gGoldenEvent.RegenGoldTitan();
		gGoldenEvent.RegenGoldDercon();
		gGoldenEvent.RegenGoldLizardKing();
		gGoldenEvent.RegenGoldTantalo();
		gGoldenEvent.RegenGoldCrust();
		gGoldenEvent.RegenGoldDevil();
		gGoldenEvent.RegenGoldGreatDragon();
		gGoldenEvent.RegenGoldIronKnight();
		gGoldenEvent.RegenGoldKnight();
		gGoldenEvent.RegenGoldNapin();
		gGoldenEvent.RegenGoldRabbit();
		gGoldenEvent.RegenGoldSatyros();
		gGoldenEvent.RegenGoldStoneGolem();
		gGoldenEvent.RegenGoldTwinTail();

		_beginthread(GoldenInvasion_EventStart,0,NULL);
	}
}

void CGoldenEvent::End()
{
	SendMsg.MessageOutAll(0x00, "Golden Invasion End");
	GCMapEventStateSend(gGoldenEvent.m_BossGoldDragonMapNumber, 0, 3);
	gGoldenEvent.Started					= 0;
	gGoldenEvent.AuxDurationTime			= gGoldenEvent.DurationTime*60;
	gGoldenEvent.MonsterCount				= 0;
	gGoldenEvent.m_BossGoldDragonMapNumber	= 0;
	gGoldenEvent.DeleteMonsters();
}

void CGoldenEvent::DeleteMonsters()
{
	for (int i=0;i<200;i++)
	{
		if ( gGoldenEvent.MonsterIndex[i] != -1 )
		{
			gObjDel(gGoldenEvent.MonsterIndex[i]);
			gGoldenEvent.MonsterIndex[i] = -1;
		}
	}
}

bool CGoldenEvent::CheckGoldenDrop(LPOBJ lpObj)
{
	bool bResult = false;
	// ----------------------------
	// GOLDEN MONSTERS
	// ----------------------------
	int MaxGoldenHitUser;
	int type;
	BYTE level;
	BYTE x;
	BYTE y;
	BYTE Socket[5];
	Socket[0] = 0xFF;
	Socket[1] = 0xFF;
	Socket[2] = 0xFF;
	Socket[3] = 0xFF;
	Socket[4] = 0xFF;
	// ----
	switch ( lpObj->Class )
	{
	case 43:	// -- Golden Budge Dragon
		MaxGoldenHitUser = gObjMonsterTopHitDamageUser(lpObj);
		x = lpObj->X;
		y = lpObj->Y;
		level = 0;
		type = ItemGetNumberMake(14, 11);
		ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, x, y, type, level, 255.0f, 0, 0, 0, MaxGoldenHitUser, 0, 0, Socket);
		bResult = true;
		break;
	case 53:	// -- Golden Titan
		MaxGoldenHitUser = gObjMonsterTopHitDamageUser(lpObj);
		x = lpObj->X;
		y = lpObj->Y;
		level = 9;
		type = ItemGetNumberMake(14, 11);
		ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, x, y, type, level, 255.0f, 0, 0, 0, MaxGoldenHitUser, 0, 0, Socket);
		bResult = true;
		break;
	case 54:	// -- Golden Soldier
		MaxGoldenHitUser = gObjMonsterTopHitDamageUser(lpObj);
		x = lpObj->X;
		y = lpObj->Y;
		level = 8;
		type = ItemGetNumberMake(14, 11);
		ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, x, y, type, level, 255.0f, 0, 0, 0, MaxGoldenHitUser, 0, 0, Socket);
		bResult = true;
		break;
	case 78:	// -- Golden Goblin
		MaxGoldenHitUser = gObjMonsterTopHitDamageUser(lpObj);
		x = lpObj->X;
		y = lpObj->Y;
		level = 8;
		type = ItemGetNumberMake(14, 11);
		ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, x, y, type, level, 255.0f, 0, 0, 0, MaxGoldenHitUser, 0, 0, Socket);
		bResult = true;
		break;
	case 79:	// -- Golden Dragon
		MaxGoldenHitUser = gObjMonsterTopHitDamageUser(lpObj);
		x = lpObj->X;
		y = lpObj->Y;
		level = 9;
		type = ItemGetNumberMake(14, 11);
		// ----
		if ( lpObj->MapNumber == MAP_INDEX_LOSTTOWER )
		{
			int Random;
			Random = rand()%3+1;
			// ----
			for (int i=0;i<Random;i++)
			{
				ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, x, y, type, level, 255.0f, 0, 0, 0, MaxGoldenHitUser, 0, 0, Socket);
				x++;
				y++;
			}
		}
		else
		{
			ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, x, y, type, level, 255.0f, 0, 0, 0, MaxGoldenHitUser, 0, 0, Socket);
		}
		bResult = true;
		break;
	case 80:	// -- Golden Vepar
		MaxGoldenHitUser = gObjMonsterTopHitDamageUser(lpObj);
		x = lpObj->X;
		y = lpObj->Y;
		level = 9;
		type = ItemGetNumberMake(14, 11);
		ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, x, y, type, level, 255.0f, 0, 0, 0, MaxGoldenHitUser, 0, 0, Socket);
		bResult = true;
		break;
	case 81:	// -- Golden Lizard King
		MaxGoldenHitUser = gObjMonsterTopHitDamageUser(lpObj);
		x = lpObj->X;
		y = lpObj->Y;
		level = 9;
		type = ItemGetNumberMake(14, 11);
		ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, x, y, type, level, 255.0f, 0, 0, 0, MaxGoldenHitUser, 0, 0, Socket);
		bResult = true;	
		break;
	case 82:	// -- Golden Tantalo
		MaxGoldenHitUser = gObjMonsterTopHitDamageUser(lpObj);
		x = lpObj->X;
		y = lpObj->Y;
		level = 10;
		type = ItemGetNumberMake(14, 11);
		ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, x, y, type, level, 255.0f, 0, 0, 0, MaxGoldenHitUser, 0, 0, Socket);
		bResult = true;
		break;
	case 83:	// -- Golden Wheel
		MaxGoldenHitUser = gObjMonsterTopHitDamageUser(lpObj);
		x = lpObj->X;
		y = lpObj->Y;
		level = 10;
		type = ItemGetNumberMake(14, 11);
		ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, x, y, type, level, 255.0f, 0, 0, 0, MaxGoldenHitUser, 0, 0, Socket);
		bResult = true;
		break;
	case 502:	// -- Golden Rabit
		MaxGoldenHitUser = gObjMonsterTopHitDamageUser(lpObj);
		x = lpObj->X;
		y = lpObj->Y;
		level = 8;
		type = ItemGetNumberMake(14, 11);
		ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, x, y, type, level, 255.0f, 0, 0, 0, MaxGoldenHitUser, 0, 0, Socket);
		bResult = true;
		break;
	case 493:	// -- Golden Death Knight
		MaxGoldenHitUser = gObjMonsterTopHitDamageUser(lpObj);
		x = lpObj->X;
		y = lpObj->Y;
		level = 8;
		type = ItemGetNumberMake(14, 11);
		ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, x, y, type, level, 255.0f, 0, 0, 0, MaxGoldenHitUser, 0, 0, Socket);
		bResult = true;
		break;
	case 494:	// -- Golden Devil
		MaxGoldenHitUser = gObjMonsterTopHitDamageUser(lpObj);
		x = lpObj->X;
		y = lpObj->Y;
		level = 9;
		type = ItemGetNumberMake(14, 11);
		ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, x, y, type, level, 255.0f, 0, 0, 0, MaxGoldenHitUser, 0, 0, Socket);
		bResult = true;
		break;
	case 495:	// -- Golden Stone Golem
		MaxGoldenHitUser = gObjMonsterTopHitDamageUser(lpObj);
		x = lpObj->X;
		y = lpObj->Y;
		level = 10;
		type = ItemGetNumberMake(14, 11);
		ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, x, y, type, level, 255.0f, 0, 0, 0, MaxGoldenHitUser, 0, 0, Socket);
		bResult = true;
		break;
	case 496:	// -- Golden Crust
		MaxGoldenHitUser = gObjMonsterTopHitDamageUser(lpObj);
		x = lpObj->X;
		y = lpObj->Y;
		level = 10;
		type = ItemGetNumberMake(14, 11);
		ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, x, y, type, level, 255.0f, 0, 0, 0, MaxGoldenHitUser, 0, 0, Socket);
		bResult = true;
		break;
	case 497:	// -- Golden Satyros
		MaxGoldenHitUser = gObjMonsterTopHitDamageUser(lpObj);
		x = lpObj->X;
		y = lpObj->Y;
		level = 11;
		type = ItemGetNumberMake(14, 11);
		ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, x, y, type, level, 255.0f, 0, 0, 0, MaxGoldenHitUser, 0, 0, Socket);
		bResult = true;
		break;
	case 498:	// -- Golden Twin Tail
		MaxGoldenHitUser = gObjMonsterTopHitDamageUser(lpObj);
		x = lpObj->X;
		y = lpObj->Y;
		level = 11;
		type = ItemGetNumberMake(14, 11);
		ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, x, y, type, level, 255.0f, 0, 0, 0, MaxGoldenHitUser, 0, 0, Socket);
		bResult = true;
		break;
	case 500:	// -- Golden Napin
		MaxGoldenHitUser = gObjMonsterTopHitDamageUser(lpObj);
		x = lpObj->X;
		y = lpObj->Y;
		level = 12;
		type = ItemGetNumberMake(14, 11);
		ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, x, y, type, level, 255.0f, 0, 0, 0, MaxGoldenHitUser, 0, 0, Socket);
		bResult = true;
		break;
	case 499:	// -- Golden Iron Knight
		MaxGoldenHitUser = gObjMonsterTopHitDamageUser(lpObj);
		x = lpObj->X;
		y = lpObj->Y;
		level = 12;
		type = ItemGetNumberMake(14, 11);
		ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, x, y, type, level, 255.0f, 0, 0, 0, MaxGoldenHitUser, 0, 0, Socket);
		bResult = true;
		break;
	case 501:	// -- Golden Great Dragon
		MaxGoldenHitUser = gObjMonsterTopHitDamageUser(lpObj);
		x = lpObj->X;
		y = lpObj->Y;
		level = 12;
		type = ItemGetNumberMake(14, 11);
		ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, x, y, type, level, 255.0f, 0, 0, 0, MaxGoldenHitUser, 0, 0, Socket);
		bResult = true;
		break;
	}
	return bResult;
}

void GoldenInvasion_EventStart(void * lpParam)
{
	while ( true )
	{
		if ( gGoldenEvent.AuxDurationTime <= 0 )
		{
			gGoldenEvent.End();
			_endthread();
		}
		gGoldenEvent.AuxDurationTime--;
		Sleep(1000);
	}
}

void CGoldenEvent::SendInvasionEffect()
{
	if ( gGoldenEvent.Enabled == 0 )
	{
		return;
	}
	if ( gGoldenEvent.Started == 0 )
	{
		return;
	}
	int Count = 0;
	for (int i=0;i<200;i++)
	{
		if ( gGoldenEvent.MonsterIndex[i] != -1 )
		{
			if ( gObj[gGoldenEvent.MonsterIndex[i]].Class == 79 && gObj[gGoldenEvent.MonsterIndex[i]].Live != 0)
			{
				Count++;
			}
		}
	}
	if ( Count > 0 )
	{
		GCMapEventStateSend(gGoldenEvent.m_BossGoldDragonMapNumber, 1, 3);
	}
	else
	{
		GCMapEventStateSend(gGoldenEvent.m_BossGoldDragonMapNumber, 0, 3);
	}
}