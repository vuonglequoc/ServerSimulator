//---------------------------------------------------------------------------
// # Project:		HaRiBO MU GameServer - Supported Season 6
// # Company:		RealCoderZ MU Development © 2011
// # Description:	Imperial Fort Event
// # Developed By:	M.E.S & WolF
//---------------------------------------------------------------------------

#include "../Header Files/stdafx.h"
#include "../Header Files/ImperialFort.h"
#include "../Header Files/gObjMonster.h"
#include "../Header Files/DSProtocol.h"
#include "../Header Files/Defines.h"
#include "../Header Files/Notice.H"
#include "../Header Files/Functions.H"
#include "../Header Files/ReadScript.h"
#include "../Header Files/GameMain.h"
#include "../Header Files/MoveSystem.h"
#include "../Header Files/SendProto.H"


static const struct IMPERIALFORT_ZONE
{
	unsigned char btStartX;
	unsigned char btStartY;
	unsigned char btEndX;
	unsigned char btEndY;

} ImperialFortGate4[10] = {

	81, 66, 82, 70,
	51, 66, 51, 70,
	106, 66, 111, 69,
	106, 66, 111, 69,
	106, 66, 111, 69,
	106, 66, 111, 69,
	106, 66, 111, 69,
	106, 66, 111, 69,
	106, 66, 111, 69,
	106, 66, 111, 69
};

// ----------------------------------------------------------------------------------------
CImperialFort gImperialFort;
// ----------------------------------------------------------------------------------------
CImperialFort::CImperialFort(void)
{
	return;
}
// ----------------------------------------------------------------------------------------
CImperialFort::~CImperialFort(void)
{
	return;
}
// ----------------------------------------------------------------------------------------
void CImperialFort::Init()
{
	gImperialFort.Enabled					= GetPrivateProfileInt(IMPERIAL_HDR,"Enabled",0,IMPERIAL_FILE_PATH);
	gImperialFort.MinLevel					= GetPrivateProfileInt(IMPERIAL_HDR,"MinLevel",0,IMPERIAL_FILE_PATH);
	gImperialFort.MaxLevel					= GetPrivateProfileInt(IMPERIAL_HDR,"MaxLevel",0,IMPERIAL_FILE_PATH);
	gImperialFort.Running				= false;
	gImperialFort.EventState			= 0;
	gImperialFort.EventMap				= 0;
	gImperialFort.EventDayOfWeek		= -1;
	gImperialFort.EventX				= 0;
	gImperialFort.EventY				= 0;
	gImperialFort.UserCount				= 0;
	gImperialFort.UserIndex[0]			= 0;
	gImperialFort.UserIndex[1]			= 0;
	gImperialFort.UserIndex[2]			= 0;
	gImperialFort.UserIndex[3]			= 0;
	gImperialFort.UserIndex[4]			= 0;
	gImperialFort.EventMaxLevel			= 0;
	gImperialFort.CanKillGate			= 0;
	gImperialFort.NormalBossNum			= 0;
	// ----
	for (int i=0;i<17;i++)
	{
		gImperialFort.MonsterDefense[i]	= 0;
		gImperialFort.MonsterHP[i]		= 0;
		gImperialFort.MonsterLevel[i]	= 0;
		gImperialFort.MonsterMaxDmg[i]	= 0;
		gImperialFort.MonsterMinDmg[i]	= 0;
		gImperialFort.MonsterMP[i]		= 0;
	}
	// ----
	gImperialFort.StageTimer[0] = 236;	//236
	gImperialFort.StageTimer[1] = 2348;	//2348
	gImperialFort.StageTimer[2] = 236;	//236
	gImperialFort.StageTimer[3] = 2348;	//2348
	gImperialFort.StageTimer[4] = 236;	//236
	gImperialFort.StageTimer[5] = 2348;	//2348
	gImperialFort.StageTimer[6] = 2348;	//2348
	gImperialFort.StageTimer[7] = 2348;	//2348
	gImperialFort.StageTimer[8] = 2348;	//2348
	gImperialFort.StageTimer[9] = 236;
	// ----
	for (int i=0;i<255;i++)
	{
		gImperialFort.MonstersAddIndex[i] = -1;
		gImperialFort.GatesAddIndex[i] = -1;
		gImperialFort.GatesAddNum[i] = -1;
	}
	gImperialFort.MonstersAddCount = 0;
	gImperialFort.GatesAddCount = 0;


	// ---- Event Day 1
	gImperialFort.GateStartX[0][0] = 220;
	gImperialFort.GateEndX[0][0] = 243;
	gImperialFort.GateStartY[0][0] = 32;
	gImperialFort.GateEndY[0][0] = 48;

	gImperialFort.GateStartX[1][0] = 225;
	gImperialFort.GateEndX[1][0] = 235;
	gImperialFort.GateStartY[1][0] = 58;
	gImperialFort.GateEndY[1][0] = 67;

	gImperialFort.GateStartX[2][0] = 205;
	gImperialFort.GateEndX[2][0] = 212;
	gImperialFort.GateStartY[2][0] = 78;
	gImperialFort.GateEndY[2][0] = 84;

	gImperialFort.GateStartX[3][0] = 180;
	gImperialFort.GateEndX[3][0] = 190;
	gImperialFort.GateStartY[3][0] = 19;
	gImperialFort.GateEndY[3][0] = 38;

	gImperialFort.GateStartX[4][0] = 152;
	gImperialFort.GateEndX[4][0] = 162;
	gImperialFort.GateStartY[4][0] = 23;
	gImperialFort.GateEndY[4][0] = 31;

	gImperialFort.GateStartX[5][0] = 152;
	gImperialFort.GateEndX[5][0] = 156;
	gImperialFort.GateStartY[5][0] = 57;
	gImperialFort.GateEndY[5][0] = 65;

	gImperialFort.GateStartX[6][0] = 172;
	gImperialFort.GateEndX[6][0] = 187;
	gImperialFort.GateStartY[6][0] = 82;
	gImperialFort.GateEndY[6][0] = 102;

	gImperialFort.TrapStartX[0][0] = 218;
	gImperialFort.TrapEndX[0][0] = 234;
	gImperialFort.TrapStartY[0][0] = 58;
	gImperialFort.TrapEndY[0][0] = 85;

	gImperialFort.TrapStartX[1][0] = 151;
	gImperialFort.TrapEndX[1][0] = 163;
	gImperialFort.TrapStartY[1][0] = 23;
	gImperialFort.TrapEndY[1][0] = 48;

	// ---- Event Day 2
	gImperialFort.GateStartX[0][1] = 57;
	gImperialFort.GateEndX[0][1] = 72;
	gImperialFort.GateStartY[0][1] = 52;
	gImperialFort.GateEndY[0][1] = 75;

	gImperialFort.GateStartX[1][1] = 31;
	gImperialFort.GateEndX[1][1] = 47;
	gImperialFort.GateStartY[1][1] = 62;
	gImperialFort.GateEndY[1][1] = 67;

	gImperialFort.GateStartX[2][1] = 9;
	gImperialFort.GateEndX[2][1] = 16;
	gImperialFort.GateStartY[2][1] = 62;
	gImperialFort.GateEndY[2][1] = 67;

	gImperialFort.GateStartX[3][1] = 26;
	gImperialFort.GateEndX[3][1] = 49;
	gImperialFort.GateStartY[3][1] = 96;
	gImperialFort.GateEndY[3][1] = 110;

	gImperialFort.GateStartX[4][1] = 38;
	gImperialFort.GateEndX[4][1] = 45;
	gImperialFort.GateStartY[4][1] = 120;
	gImperialFort.GateEndY[4][1] = 130;

	gImperialFort.GateStartX[5][1] = 53;
	gImperialFort.GateEndX[5][1] = 57;
	gImperialFort.GateStartY[5][1] = 157;
	gImperialFort.GateEndY[5][1] = 162;

	gImperialFort.GateStartX[6][1] = 95;
	gImperialFort.GateEndX[6][1] = 102;
	gImperialFort.GateStartY[6][1] = 104;
	gImperialFort.GateEndY[6][1] = 118;

	gImperialFort.TrapStartX[0][1] = 27;
	gImperialFort.TrapEndX[0][1] = 44;
	gImperialFort.TrapStartY[0][1] = 62;
	gImperialFort.TrapEndY[0][1] = 67;

	gImperialFort.TrapStartX[1][1] = 38;
	gImperialFort.TrapEndX[1][1] = 56;
	gImperialFort.TrapStartY[1][1] = 120;
	gImperialFort.TrapEndY[1][1] = 148;

	// ---- Event Day 3
	gImperialFort.GateStartX[0][2] = 130;
	gImperialFort.GateEndX[0][2] = 143;
	gImperialFort.GateStartY[0][2] = 180;
	gImperialFort.GateEndY[0][2] = 204;

	gImperialFort.GateStartX[1][2] = 108;
	gImperialFort.GateEndX[1][2] = 116;
	gImperialFort.GateStartY[1][2] = 185;
	gImperialFort.GateEndY[1][2] = 205;

	gImperialFort.GateStartX[2][2] = 78;
	gImperialFort.GateEndX[2][2] = 86;
	gImperialFort.GateStartY[2][2] = 193;
	gImperialFort.GateEndY[2][2] = 197;

	gImperialFort.GateStartX[3][2] = 213;
	gImperialFort.GateEndX[3][2] = 232;
	gImperialFort.GateStartY[3][2] = 137;
	gImperialFort.GateEndY[3][2] = 150;

	gImperialFort.GateStartX[4][2] = 220;
	gImperialFort.GateEndX[4][2] = 224;
	gImperialFort.GateStartY[4][2] = 163;
	gImperialFort.GateEndY[4][2] = 173;

	gImperialFort.GateStartX[5][2] = 222;
	gImperialFort.GateEndX[5][2] = 224;
	gImperialFort.GateStartY[5][2] = 196;
	gImperialFort.GateEndY[5][2] = 202;

	gImperialFort.GateStartX[6][2] = 157;
	gImperialFort.GateEndX[6][2] = 176;
	gImperialFort.GateStartY[6][2] = 220;
	gImperialFort.GateEndY[6][2] = 230;

	gImperialFort.TrapStartX[0][2] = 91;
	gImperialFort.TrapEndX[0][2] = 118;
	gImperialFort.TrapStartY[0][2] = 185;
	gImperialFort.TrapEndY[0][2] = 205;

	gImperialFort.TrapStartX[1][2] = 220;
	gImperialFort.TrapEndX[1][2] = 224;
	gImperialFort.TrapStartY[1][2] = 162;
	gImperialFort.TrapEndY[1][2] = 191;

	// ---- Event Day 4
	gImperialFort.GateStartX[0][3] = 60;
	gImperialFort.GateEndX[0][3] = 78;
	gImperialFort.GateStartY[0][3] = 59;
	gImperialFort.GateEndY[0][3] = 78;

	gImperialFort.GateStartX[1][3] = 37;
	gImperialFort.GateEndX[1][3] = 47;
	gImperialFort.GateStartY[1][3] = 66;
	gImperialFort.GateEndY[1][3] = 74;

	gImperialFort.GateStartX[2][3] = 29;
	gImperialFort.GateEndX[2][3] = 32;
	gImperialFort.GateStartY[2][3] = 93;
	gImperialFort.GateEndY[2][3] = 98;

	gImperialFort.GateStartX[3][3] = 20;
	gImperialFort.GateEndX[3][3] = 47;
	gImperialFort.GateStartY[3][3] = 179;
	gImperialFort.GateEndY[3][3] = 199;

	gImperialFort.GateStartX[4][3] = 55;
	gImperialFort.GateEndX[4][3] = 67;
	gImperialFort.GateStartY[4][3] = 188;
	gImperialFort.GateEndY[4][3] = 193;

	gImperialFort.GateStartX[5][3] = 66;
	gImperialFort.GateEndX[5][3] = 72;
	gImperialFort.GateStartY[5][3] = 158;
	gImperialFort.GateEndY[5][3] = 163;

	gImperialFort.GateStartX[6][3] = 159;
	gImperialFort.GateEndX[6][3] = 175;
	gImperialFort.GateStartY[6][3] = 127;
	gImperialFort.GateEndY[6][3] = 139;

	gImperialFort.GateStartX[7][3] = 200;
	gImperialFort.GateEndX[7][3] = 220;
	gImperialFort.GateStartY[7][3] = 131;
	gImperialFort.GateEndY[7][3] = 135;

	gImperialFort.GateStartX[8][3] = 221;
	gImperialFort.GateEndX[8][3] = 226;
	gImperialFort.GateStartY[8][3] = 162;
	gImperialFort.GateEndY[8][3] = 166;

	gImperialFort.GateStartX[9][3] = 195;
	gImperialFort.GateEndX[9][3] = 211;
	gImperialFort.GateStartY[9][3] = 20;
	gImperialFort.GateEndY[9][3] = 32;

	gImperialFort.TrapStartX[0][3] = 29;
	gImperialFort.TrapEndX[0][3] = 49;
	gImperialFort.TrapStartY[0][3] = 66;
	gImperialFort.TrapEndY[0][3] = 86;

	gImperialFort.TrapStartX[1][3] = 57;
	gImperialFort.TrapEndX[1][3] = 57;
	gImperialFort.TrapStartY[1][3] = 189;
	gImperialFort.TrapEndY[1][3] = 193;

	gImperialFort.TrapStartX[2][3] = 64;
	gImperialFort.TrapEndX[2][3] = 64;
	gImperialFort.TrapStartY[2][3] = 189;
	gImperialFort.TrapEndY[2][3] = 193;

	gImperialFort.TrapStartX[3][3] = 67;
	gImperialFort.TrapEndX[3][3] = 69;
	gImperialFort.TrapStartY[3][3] = 181;
	gImperialFort.TrapEndY[3][3] = 181;

	gImperialFort.TrapStartX[4][3] = 67;
	gImperialFort.TrapEndX[4][3] = 69;
	gImperialFort.TrapStartY[4][3] = 174;
	gImperialFort.TrapEndY[4][3] = 174;

	gImperialFort.TrapStartX[5][3] = 204;
	gImperialFort.TrapEndX[5][3] = 226;
	gImperialFort.TrapStartY[5][3] = 131;
	gImperialFort.TrapEndY[5][3] = 155;

	for (int i=0;i<255;i++)
	{
		gImperialFort.NormalMonsterPos[0][i] = -1;
		gImperialFort.NormalMonsterPos[1][i] = -1;
		gImperialFort.NormalMonsterPos[2][i] = -1;
		gImperialFort.NormalMonsterPos[3][i] = -1;
	}
	for (int i=0;i<20;i++)
	{
		gImperialFort.GatesPos[0][i] = -1;
		gImperialFort.GatesPos[1][i] = -1;
		gImperialFort.GatesPos[2][i] = -1;
		gImperialFort.GatesPos[3][i] = -1;
	}
	gImperialFort.GatesCount[0] = 0;
	gImperialFort.GatesCount[1] = 0;
	gImperialFort.GatesCount[2] = 0;
	gImperialFort.GatesCount[3] = 0;
	gImperialFort.NormalMonsterCount[0] = 0;
	gImperialFort.NormalMonsterCount[1] = 0;
	gImperialFort.NormalMonsterCount[2] = 0;
	gImperialFort.NormalMonsterCount[3] = 0;
	BossMonsterPos[0][0] = -1;
	BossMonsterPos[1][0] = -1;
	BossMonsterPos[2][0] = -1;
	BossMonsterPos[3][0] = -1;
	BossMonsterPos[0][1] = -1;
	BossMonsterPos[1][1] = -1;
	BossMonsterPos[2][1] = -1;
	BossMonsterPos[3][1] = -1;
	FinalBossPos = -1;

	for (int n=0;n<gMSetBase.m_Count;n++)
	{
		char Type = gImperialFort.GetIndexType(gMSetBase.m_Mp[n].m_MapNumber);

		if ( Type >= 0 && Type <= 3 )
		{
			if ( Type == 0 )
			{
				if ( gMSetBase.m_Mp[n].m_Type == 508 )
				{
					gImperialFort.BossMonsterPos[0][0] = n;
				}
				else if ( gMSetBase.m_Mp[n].m_Type == 511 )
				{
					gImperialFort.BossMonsterPos[0][1] = n;
				}
				else if ( (gMSetBase.m_Mp[n].m_Type >= 512 && gMSetBase.m_Mp[n].m_Type <= 521) || gMSetBase.m_Mp[n].m_Type == 526 )
				{
					gImperialFort.NormalMonsterPos[0][gImperialFort.NormalMonsterCount[0]] = n;
					gImperialFort.NormalMonsterCount[0]++;
				}
				else if ( gMSetBase.m_Mp[n].m_Type == 524 || gMSetBase.m_Mp[n].m_Type == 525 || gMSetBase.m_Mp[n].m_Type == 527 || gMSetBase.m_Mp[n].m_Type == 528 )
				{
					gImperialFort.GatesPos[0][gImperialFort.GatesCount[0]] = n;
					gImperialFort.GatesCount[0]++;
				}
			}
			else if ( Type == 1 )
			{
				if ( gMSetBase.m_Mp[n].m_Type == 509 )
				{
					gImperialFort.BossMonsterPos[1][0] = n;
				}
				else if ( gMSetBase.m_Mp[n].m_Type == 507 )
				{
					gImperialFort.BossMonsterPos[1][1] = n;
				}
				else if ( (gMSetBase.m_Mp[n].m_Type >= 512 && gMSetBase.m_Mp[n].m_Type <= 521) || gMSetBase.m_Mp[n].m_Type == 526 )
				{
					gImperialFort.NormalMonsterPos[1][gImperialFort.NormalMonsterCount[1]] = n;
					gImperialFort.NormalMonsterCount[1]++;
				}
				else if ( gMSetBase.m_Mp[n].m_Type == 524 || gMSetBase.m_Mp[n].m_Type == 525 || gMSetBase.m_Mp[n].m_Type == 527 || gMSetBase.m_Mp[n].m_Type == 528 )
				{
					gImperialFort.GatesPos[1][gImperialFort.GatesCount[1]] = n;
					gImperialFort.GatesCount[1]++;
				}
			}
			else if ( Type == 2 )
			{
				if ( gMSetBase.m_Mp[n].m_Type == 506 )
				{
					gImperialFort.BossMonsterPos[2][0] = n;
				}
				else if ( gMSetBase.m_Mp[n].m_Type == 510 )
				{
					gImperialFort.BossMonsterPos[2][1] = n;
				}
				else if ( (gMSetBase.m_Mp[n].m_Type >= 512 && gMSetBase.m_Mp[n].m_Type <= 521) || gMSetBase.m_Mp[n].m_Type == 526 )
				{
					gImperialFort.NormalMonsterPos[2][gImperialFort.NormalMonsterCount[2]] = n;
					gImperialFort.NormalMonsterCount[2]++;
				}
				else if ( gMSetBase.m_Mp[n].m_Type == 524 || gMSetBase.m_Mp[n].m_Type == 525 || gMSetBase.m_Mp[n].m_Type == 527 || gMSetBase.m_Mp[n].m_Type == 528 )
				{
					gImperialFort.GatesPos[2][gImperialFort.GatesCount[2]] = n;
					gImperialFort.GatesCount[2]++;
				}
			}
			else if ( Type == 3 )
			{
				if ( gMSetBase.m_Mp[n].m_Type == 504 )
				{
					gImperialFort.FinalBossPos = n;
				}
				else if ( gMSetBase.m_Mp[n].m_Type == 505 )
				{
					gImperialFort.BossMonsterPos[Type][0] = n;
				}
				else if ( (gMSetBase.m_Mp[n].m_Type >= 512 && gMSetBase.m_Mp[n].m_Type <= 521) || gMSetBase.m_Mp[n].m_Type == 526 )
				{
					gImperialFort.NormalMonsterPos[Type][gImperialFort.NormalMonsterCount[Type]] = n;
					gImperialFort.NormalMonsterCount[Type]++;
				}
				else if ( gMSetBase.m_Mp[n].m_Type == 524 || gMSetBase.m_Mp[n].m_Type == 525 || gMSetBase.m_Mp[n].m_Type == 527 || gMSetBase.m_Mp[n].m_Type == 528 )
				{
					gImperialFort.GatesPos[Type][gImperialFort.GatesCount[Type]] = n;
					gImperialFort.GatesCount[Type]++;
				}
			}
		}
	}

	gImperialFort.ReadFile();
}
// ----------------------------------------------------------------------------------------
char CImperialFort::GetIndexType(int MapNumber)
{
	char btResult = -1;

	switch ( MapNumber )
	{
	case 69:
		btResult = 0;
		break;
	case 70:
		btResult = 1;
		break;
	case 71:
		btResult = 2;
		break;
	case 72:
		btResult = 3;
		break;
	}

	return btResult;
}
// ----------------------------------------------------------------------------------------
void CImperialFort::Run()
{
	if ( gImperialFort.Enabled == 0 )
	{
		return;
	}
	// ----
	if ( gImperialFort.Running == 0 )
	{
		return;
	}
	// ----
	for (int i=0;i<5;i++)
	{
		if ( gImperialFort.UserIndex[i] != 0 )
		{
			if ( gObjIsConnected(&gObj[gImperialFort.UserIndex[i]]) == true )
			{
				if ( gObj[gImperialFort.UserIndex[i]].MapNumber != gImperialFort.EventMap )
				{
					gImperialFort.SendFailure();
					gImperialFort.UserIndex[i] = 0;
					gImperialFort.UserCount--;
				}
				else
				{
					gImperialFort.MoveToGate(gImperialFort.UserIndex[i]);
				}
			}
			else
			{
				gImperialFort.UserIndex[i] = 0;
				gImperialFort.UserCount--;
			}
		}
	}
	if ( gImperialFort.Running == 1 && gImperialFort.UserCount == 0 )
	{
		gImperialFort.Clear();
		return;
	}
}
// ----------------------------------------------------------------------------------------
void CImperialFort::DropItem(LPOBJ lpObj,unsigned char X, unsigned char Y, unsigned char Type)
{
	int RandomItem;
	int Item;
	unsigned char Skill;
	unsigned char Luck;
	unsigned char Opt;
	unsigned char Level;
	unsigned char Dur;
	unsigned char Exe;
	unsigned char Anc;
	unsigned char Socket[5];
	
	Socket[0] = 0xFF;
	Socket[1] = 0xFF;
	Socket[2] = 0xFF;
	Socket[3] = 0xFF;
	Socket[4] = 0xFF;
	Skill = 0;
	Luck = 0;
	Opt = 0;
	Level = 0;
	Dur = 0;
	Exe = 0;
	Anc = 0;
	Item = 0;

	RandomItem = rand()%gImperialFort.ItemCount[Type];

	Item = ITEMGET(gImperialFort.ItemType[Type][RandomItem],gImperialFort.ItemIndex[Type][RandomItem]);

	if ( rand()%1000 < 50 )
	{
		Skill = gImperialFort.ItemSkill[Type][RandomItem];
	}

	if ( rand()%1000 < 50 )
	{
		Luck = gImperialFort.ItemLuck[Type][RandomItem];
	}

	if ( rand()%1000 < 50 )
	{
		if ( gImperialFort.ItemLevel[Type][RandomItem] > 0 )
		{
			Level = rand()%(gImperialFort.ItemLevel[Type][RandomItem]+1);
		}
		else
		{
			Level = 0;
		}
	}

	if ( rand()%1000 < 50 )
	{
		if ( gImperialFort.ItemOpt[Type][RandomItem] > 0 )
		{
			Opt = rand()%(gImperialFort.ItemOpt[Type][RandomItem]+1);
		}
		else
		{
			Opt = 0;
		}
	}

	if ( rand()%1000 < 50 )
	{
		Exe = GetRandomExeOption(gImperialFort.ItemExe[Type][RandomItem]+1);
	}

	if ( rand()%1000 < 10 )
	{
		if ( gImperialFort.ItemAnc[Type][RandomItem] == 5 || gImperialFort.ItemAnc[Type][RandomItem] == 10 )
		{
			Anc = gImperialFort.ItemAnc[Type][RandomItem];
		}
		else
		{
			Anc = 0;
		}
	}
	if ( rand()%1000 < 50 )
	{
		Socket[0] = gImperialFort.ItemSocket[0][Type][RandomItem];

		if ( rand()%1000 < 50 )
		{
			Socket[1] = gImperialFort.ItemSocket[1][Type][RandomItem];

			if ( rand()%1000 < 50 )
			{
				Socket[2] = gImperialFort.ItemSocket[2][Type][RandomItem];

				if ( rand()%1000 < 50 )
				{
					Socket[3] = gImperialFort.ItemSocket[3][Type][RandomItem];

					if ( rand()%1000 < 50 )
					{
						Socket[4] = gImperialFort.ItemSocket[4][Type][RandomItem];
					}
				}
			}
		}
	}

	ItemSerialCreateSend(lpObj->m_Index,gImperialFort.EventMap,X,Y,Item,Level,Dur,Luck,Skill,Opt,lpObj->m_Index,Exe,Anc,Socket);
}
// ----------------------------------------------------------------------------------------
void CImperialFort::ReadFile()
{
	SMDFile = fopen(IMPERIAL_REW_FILE, "r");

	if ( SMDFile == NULL )
	{
		CLog.MsgBox("[ImperialFort]: Read File -> Info file Load Fail [%s]", IMPERIAL_REW_FILE);
		return;
	}

	int Token;
	int type = -1;

	while ( true )
	{
	    Token = GetToken();

        if( Token == 2)
		{
            break;
		}

		type = TokenNumber;

		while ( true )
		{
			if ( type < 0 || type > 7 )
			{
				break;
			}
		    Token = GetToken();

			if ( strcmp("end", TokenString ) == 0)
			{
				break;
			}

			gImperialFort.ItemType[type][gImperialFort.ItemCount[type]] = TokenNumber;
				
			Token = GetToken();
			gImperialFort.ItemIndex[type][gImperialFort.ItemCount[type]] = TokenNumber;

			Token = GetToken();
			gImperialFort.ItemLevel[type][gImperialFort.ItemCount[type]] = TokenNumber;

			Token = GetToken();
			gImperialFort.ItemDur[type][gImperialFort.ItemCount[type]] = TokenNumber;

			Token = GetToken();
			gImperialFort.ItemSkill[type][gImperialFort.ItemCount[type]] = TokenNumber;

			Token = GetToken();
			gImperialFort.ItemLuck[type][gImperialFort.ItemCount[type]] = TokenNumber;

			Token = GetToken();
			gImperialFort.ItemOpt[type][gImperialFort.ItemCount[type]] = TokenNumber;

			Token = GetToken();
			gImperialFort.ItemExe[type][gImperialFort.ItemCount[type]] = TokenNumber;

			Token = GetToken();
			gImperialFort.ItemAnc[type][gImperialFort.ItemCount[type]] = TokenNumber;

			Token = GetToken();
			gImperialFort.ItemSocket[0][type][gImperialFort.ItemCount[type]] = TokenNumber;

			Token = GetToken();
			gImperialFort.ItemSocket[1][type][gImperialFort.ItemCount[type]] = TokenNumber;

			Token = GetToken();
			gImperialFort.ItemSocket[2][type][gImperialFort.ItemCount[type]] = TokenNumber;

			Token = GetToken();
			gImperialFort.ItemSocket[3][type][gImperialFort.ItemCount[type]] = TokenNumber;

			Token = GetToken();
			gImperialFort.ItemSocket[4][type][gImperialFort.ItemCount[type]] = TokenNumber;

			gImperialFort.ItemCount[type]++;
		        
		}
    }

	fclose(SMDFile);
	return;
}
// ----------------------------------------------------------------------------------------
void CImperialFort::ProtocolManager(unsigned char * aRecv,int aIndex)
{
	switch ( aRecv[3] )
	{
	case 0x01:
		gImperialFort.CheckCanEnter(aIndex);
		break;
	}
}
// ----------------------------------------------------------------------------------------
void CImperialFort::CheckCanEnter(int aIndex)
{
	if ( gImperialFort.Enabled == 0 )
	{
		SendMsg.MessageOut(aIndex, 0x01, "[ImperialFort]: Event disabled");
		return;
	}
	else if ( gImperialFort.Running == true )
	{
		SendMsg.MessageOut(aIndex, 0x01, "[ImperialFort]: Event has already started");
		return;
	}
	// ----
	LPOBJ lpObj = &gObj[aIndex];
	SYSTEMTIME t;
	GetLocalTime(&t);
	// ----
	if ( lpObj->Level < gImperialFort.MinLevel )
	{
		SendMsg.MessageOut(aIndex, 0x01, "[ImperialFort]: Min level required %d",gImperialFort.MinLevel);
		return;
	}
	else if ( lpObj->Level > gImperialFort.MaxLevel )
	{
		SendMsg.MessageOut(aIndex, 0x01, "[ImperialFort]: Max level required %d",gImperialFort.MaxLevel);
		return;
	}
	// ----
	/*if ( lpObj->PartyNumber == -1 )
	{
		SendMsg.MessageOut(aIndex,"[ImperialFort]: You need to be in Party to enter");
		return;
	}*/
	// ----
	if ( gObjGetItemCountInIventory(aIndex,14,102,0) < 1 )
	{
		SendMsg.MessageOut(aIndex, 0x01, "[ImperialFort]: You need Gaions Order to enter");
		return;
	}
	else if ( gObjGetItemCountInIventory(aIndex,14,109,0) < 1 && t.wDayOfWeek == 0 )
	{
		SendMsg.MessageOut(aIndex, 0x01, "[ImperialFort]: You need Complete Secromicon to enter");
		return;
	}
	// ----
	if ( t.wDayOfWeek == 0 )
	{
		gObjDelteItemCountInInventory(aIndex,14,109,1);
	}
	gObjDelteItemCountInInventory(aIndex,14,102,1);
	// ----
	if(t.wDayOfWeek == 1 || t.wDayOfWeek == 4)
	{
		gImperialFort.EventDay = 1;
		gImperialFort.EventMap = 69;
		gImperialFort.EventX = 231 + (rand() % 7 );
		gImperialFort.EventY = 17 + (rand() % 5 );
	}
	else if(t.wDayOfWeek == 2 || t.wDayOfWeek == 5)
	{
		gImperialFort.EventDay = 2;
		gImperialFort.EventMap = 70;
		gImperialFort.EventX = 81 + (rand() % 5 );
		gImperialFort.EventY = 62 + (rand() % 7 );
	}
	else if(t.wDayOfWeek == 3 || t.wDayOfWeek == 6)
	{
		gImperialFort.EventDay = 3;
		gImperialFort.EventMap = 71;
		gImperialFort.EventX = 150 + (rand() % 4 );
		gImperialFort.EventY = 187 + (rand() % 7 );
	}
	else if(t.wDayOfWeek == 0)
	{
		gImperialFort.EventDay = 4;
		gImperialFort.EventMap = 72;
		gImperialFort.EventX = 85 + (rand() % 8 );
		gImperialFort.EventY = 64 + (rand() % 8 );
	}
	gImperialFort.EventDayOfWeek = t.wDayOfWeek;
	switch ( gImperialFort.EventDayOfWeek )
	{
	case 0:
	case 1:
	case 2:
	case 3:
		gImperialFort.NormalBossNum = 0;
		break;
	case 4:
	case 5:
	case 6:
		gImperialFort.NormalBossNum = 1;
		break;
	}
	// ----
	gImperialFort.MapTeleport(lpObj);
}
// ----------------------------------------------------------------------------------------
void CImperialFort::MapTeleport(LPOBJ lpObj)
{
	if ( lpObj->PartyNumber == -1 )
	{
		gObjTeleport(lpObj->m_Index,gImperialFort.EventMap,gImperialFort.EventX,gImperialFort.EventY);
		gImperialFort.UserIndex[gImperialFort.UserCount] = lpObj->m_Index;
		gImperialFort.UserCount++;
	}
	else
	{
		for (int i=OBJMIN;i<OBJMAX;i++)
		{
			LPOBJ lpPartyObj = &gObj[i];
			// ----
			if ( lpPartyObj->PartyNumber == lpObj->PartyNumber )
			{
				gObjTeleport(lpPartyObj->m_Index,gImperialFort.EventMap,gImperialFort.EventX,gImperialFort.EventY);
				gImperialFort.UserIndex[gImperialFort.UserCount] = lpPartyObj->m_Index;
				gImperialFort.UserCount++;
			}
		}
	}
	// ----
	for (int i=0;i<5;i++)
	{
		if ( gImperialFort.UserIndex[i] != 0 )
		{
			if ( (gObj[gImperialFort.UserIndex[i]].Level+gObj[gImperialFort.UserIndex[i]].MasterLevel) > gImperialFort.EventMaxLevel )
			{
				gImperialFort.EventMaxLevel = gObj[gImperialFort.UserIndex[i]].Level+gObj[gImperialFort.UserIndex[i]].MasterLevel;
			}
		}
	}
	gImperialFort.InitTimer();
	gImperialFort.Running = 1;
	gImperialFort.EventState = 0;
	gImperialFort.EventStage = 1;
	gImperialFort.StandByRun = 1;
	gImperialFort.CanUseSkill = 0;

	gImperialFort.SetGates();
	
	_beginthread(ImperialFort_StartStandBy,0,NULL);
}
// ----------------------------------------------------------------------------------------
void CImperialFort::InitTimer()
{
	PMSG_FORT_TIMER pMsg;
	
	pMsg.c = 0xC1;
	pMsg.size = 0x0D;
	pMsg.headcode = 0xF7;
	pMsg.subcode = 0x04;
	pMsg.Status = 0x01;
	pMsg.Unk4 = 0x00;
	pMsg.TimerL = LOBYTE(gImperialFort.StageTimer[0]);
	pMsg.TimerH = HIBYTE(gImperialFort.StageTimer[0]);
	pMsg.Unk5 = 0x00;
	pMsg.Mob = 0;

	for (int i=0;i<5;i++)
	{
		if ( gImperialFort.UserIndex[i] != 0 )
		{
			gSendProto.DataSend(gImperialFort.UserIndex[i], (BYTE*)&pMsg, pMsg.size);
		}
	}
}
// ----------------------------------------------------------------------------------------
void CImperialFort::Clear()
{
	for (int i=0;i<5;i++)
	{
		if ( gImperialFort.UserIndex[i] != 0 )
		{
			if ( gObj[gImperialFort.UserIndex[i]].MapNumber == gImperialFort.EventMap )
			{
				gObjMoveGate(gImperialFort.UserIndex[i],22);
				gImperialFort.EventGateUserOpen[i] = 0;
				gImperialFort.UserIndex[i] = 0;
			}
		}
	}

	gImperialFort.Running = 0;
	gImperialFort.EventState = 0;
	gImperialFort.EventState = 0;
	gImperialFort.EventMaxLevel = 0;
	gImperialFort.CanKillGate = 0;
	gImperialFort.CanUseSkill = 0;
	gImperialFort.EventDay = 0;
	gImperialFort.EventGateOpen = 0;
	gImperialFort.EventMap = 0;
	

	gImperialFort.StageTimer[0] = 236;	//236
	gImperialFort.StageTimer[1] = 2348;	//2348
	gImperialFort.StageTimer[2] = 236;	//236
	gImperialFort.StageTimer[3] = 2348;	//2348
	gImperialFort.StageTimer[4] = 236;	//236
	gImperialFort.StageTimer[5] = 2348;	//2348
	gImperialFort.StageTimer[6] = 2348;	//2348
	gImperialFort.StageTimer[7] = 2348;	//2348
	gImperialFort.StageTimer[8] = 2348;	//2348
	gImperialFort.StageTimer[9] = 236;
}
// ----------------------------------------------------------------------------------------
bool CImperialFort::DropCheck(LPOBJ lpObj)
{
	int MaxHitUser;
	int type;
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
	case 508:	// -- Destler (First Imperial Fort Boss)
		MaxHitUser = gObjMonsterTopHitDamageUser(lpObj);
		type = ItemGetNumberMake(14, 103);
		ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, lpObj->X, lpObj->Y, type, 0, 255.0f, 0, 0, 0, MaxHitUser, 0, 0, Socket);
		gImperialFort.DropItem(&gObj[MaxHitUser],lpObj->X,lpObj->Y,0);
		bResult = true;
		break;
	case 509:	// -- Vermont (Second Imperial Fort Boss)
		MaxHitUser = gObjMonsterTopHitDamageUser(lpObj);
		type = ItemGetNumberMake(14, 104);
		ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, lpObj->X, lpObj->Y, type, 0, 255.0f, 0, 0, 0, MaxHitUser, 0, 0, Socket);
		gImperialFort.DropItem(&gObj[MaxHitUser],lpObj->X,lpObj->Y,1);
		bResult = true;
		break;
	case 510:	// -- Kato (Third Imperial Fort Boss)
		MaxHitUser = gObjMonsterTopHitDamageUser(lpObj);
		type = ItemGetNumberMake(14, 105);
		ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, lpObj->X, lpObj->Y, type, 0, 255.0f, 0, 0, 0, MaxHitUser, 0, 0, Socket);
		gImperialFort.DropItem(&gObj[MaxHitUser],lpObj->X,lpObj->Y,2);
		bResult = true;
		break;
	case 511:	// -- Galia (Fourth Imperial Fort Boss)
		MaxHitUser = gObjMonsterTopHitDamageUser(lpObj);
		type = ItemGetNumberMake(14, 106);
		ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, lpObj->X, lpObj->Y, type, 0, 255.0f, 0, 0, 0, MaxHitUser, 0, 0, Socket);
		gImperialFort.DropItem(&gObj[MaxHitUser],lpObj->X,lpObj->Y,3);
		bResult = true;
		break;
	case 507:	// -- Erkanne (Fifth Imperial Fort Boss)
		MaxHitUser = gObjMonsterTopHitDamageUser(lpObj);
		type = ItemGetNumberMake(14, 107);
		ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, lpObj->X, lpObj->Y, type, 0, 255.0f, 0, 0, 0, MaxHitUser, 0, 0, Socket);
		gImperialFort.DropItem(&gObj[MaxHitUser],lpObj->X,lpObj->Y,4);
		bResult = true;
		break;
	case 506:	// -- Raymond (Sixth Imperial Fort Boss)
		MaxHitUser = gObjMonsterTopHitDamageUser(lpObj);
		type = ItemGetNumberMake(14, 108);
		ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, lpObj->X, lpObj->Y, type, 0, 255.0f, 0, 0, 0, MaxHitUser, 0, 0, Socket);
		gImperialFort.DropItem(&gObj[MaxHitUser],lpObj->X,lpObj->Y,5);
		bResult = true;
		break;
	case 505:	// -- Jerint (First Sunday Imperial Fort Boss)
		MaxHitUser = gObjMonsterTopHitDamageUser(lpObj);
		gImperialFort.DropItem(&gObj[MaxHitUser],lpObj->X,lpObj->Y,6);
		bResult = true;
		break;
	case 504:	// -- Final Boss
		MaxHitUser = gObjMonsterTopHitDamageUser(lpObj);
		gImperialFort.DropItem(&gObj[MaxHitUser],lpObj->X,lpObj->Y,7);
		bResult = true;
		break;
	}
	return bResult;
}
// ----------------------------------------------------------------------------------------
void CImperialFort::SetMonstersByLevel()
{
	if ( gImperialFort.EventMaxLevel <= 100 )
	{
		// ---- Level
		for (int i=0;i<17;i++)
		{
			gImperialFort.MonsterLevel[i]		= 49;
			gImperialFort.MonsterMP[i]			= 0;
		}
		// ---- Final Boss
		gImperialFort.MonsterDefense[0]		= 105;
		gImperialFort.MonsterHP[0]			= 12500;
		gImperialFort.MonsterMaxDmg[0]		= 150;
		gImperialFort.MonsterMinDmg[0]		= 135;
		
		gImperialFort.MonsterDefense[1]		= 90;
		gImperialFort.MonsterHP[1]			= 9300;
		gImperialFort.MonsterMaxDmg[1]		= 141;
		gImperialFort.MonsterMinDmg[1]		= 126;

		gImperialFort.MonsterDefense[2]		= 84;
		gImperialFort.MonsterHP[2]			= 8000;
		gImperialFort.MonsterMaxDmg[2]		= 132;
		gImperialFort.MonsterMinDmg[2]		= 117;

		gImperialFort.MonsterDefense[3]		= 84;
		gImperialFort.MonsterHP[3]			= 8000;
		gImperialFort.MonsterMaxDmg[3]		= 132;
		gImperialFort.MonsterMinDmg[3]		= 117;

		for (int i=4;i<8;i++)
		{
			gImperialFort.MonsterDefense[i]		= 81;
			gImperialFort.MonsterHP[i]			= 7800;
			gImperialFort.MonsterMaxDmg[i]		= 123;
			gImperialFort.MonsterMinDmg[i]		= 108;
		}

		for (int i=8;i<14;i++)
		{
			gImperialFort.MonsterDefense[i]		= 66;
			gImperialFort.MonsterHP[i]			= 6500;
			gImperialFort.MonsterMaxDmg[i]		= 114;
			gImperialFort.MonsterMinDmg[i]		= 99;
		}

		gImperialFort.MonsterDefense[14]	= 60;
		gImperialFort.MonsterHP[14]			= 5800;
		gImperialFort.MonsterMaxDmg[14]		= 105;
		gImperialFort.MonsterMinDmg[14]		= 90;

		gImperialFort.MonsterDefense[15]	= 45;
		gImperialFort.MonsterHP[15]			= 5000;
		gImperialFort.MonsterMaxDmg[15]		= 90;
		gImperialFort.MonsterMinDmg[15]		= 75;

		gImperialFort.MonsterDefense[16]	= 51;
		gImperialFort.MonsterHP[16]			= 5500;
		gImperialFort.MonsterMaxDmg[16]		= 99;
		gImperialFort.MonsterMinDmg[16]		= 84;

	}
	else
	{

	}
}
// ----------------------------------------------------------------------------------------
void CImperialFort::SendFailure()
{
	PMSG_FORT_TIMER pMsg;
	// ----
	pMsg.c = 0xC1;
	pMsg.size = 0x0D;
	pMsg.headcode = 0xF7;
	pMsg.subcode = 0x06;
	pMsg.Status = 0x00;
	pMsg.Unk4 = 0x00;
	pMsg.TimerL = 0x00;
	pMsg.TimerH = 0x00;
	pMsg.Unk5 = 0x00;
	pMsg.Mob = 0x00;

	for (int i=0;i<5;i++)
	{
		if ( gImperialFort.UserIndex[i] != 0 )
		{
			gSendProto.DataSend(gImperialFort.UserIndex[i],(BYTE*)&pMsg,pMsg.size);
		}
	}
}
// ----------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------
// ----------------------------------- STAGES TIMERS --------------------------------------
void ImperialFort_StartStandBy(void * lpParam)
{
	while ( true )
	{
		if(gImperialFort.StageTimer[0] < 1)
		{	
			gImperialFort.StageTimer[0] = 0;
			gImperialFort.EventState = 1;
			gImperialFort.EventStage = 1;
			gImperialFort.CanUseSkill = 1;
			gImperialFort.CanKillGate = 1;
			gImperialFort.StandByRun = 0;

			gImperialFort.SetMonsters(0);

			_beginthread(ImperialFort_StartStage1,0,NULL);
			_endthread();
		}

		gImperialFort.StageTimer[0] -= 4;

		PMSG_FORT_TIMER pMsg;
		pMsg.c = 0xC1;
		pMsg.size = 0x0D;
		pMsg.headcode = 0xF7;
		pMsg.subcode = 0x04;
		pMsg.Status = 0x01;
		pMsg.Unk4 = 0x00;
		pMsg.TimerL = LOBYTE(gImperialFort.StageTimer[0]);
		pMsg.TimerH = HIBYTE(gImperialFort.StageTimer[0]);
		pMsg.Unk5 = 0x00;
		pMsg.Mob = 0;
		
		for(int i=0;i<5;i++)
		{
			if ( gImperialFort.UserIndex[i] != 0 )
			{
				gSendProto.DataSend(gImperialFort.UserIndex[i], (BYTE*)&pMsg, pMsg.size);
			}
		}

		Sleep(1000);
	}
}

void ImperialFort_StartStage1(void * lpParam)
{
	while ( true )
	{
		gImperialFort.StageTimer[1] -= 4;
		// ---- Check Monsters Killed And Can Kill Next Gate
		if ( gImperialFort.MonstersAddCount <= 0 && gImperialFort.CanKillGate == 0 )
		{
			gImperialFort.CanKillGate = 1;
		}
		else if ( gImperialFort.MonstersAddCount > 0 && gImperialFort.EventState == 0 )
		{
			gImperialFort.CanKillGate = 0;
		}
		// ---- Check Timer
		if ( gImperialFort.StandByRun == 1 )
		{
			gImperialFort.EventState = 1;
			gImperialFort.CanUseSkill = 0;
			gImperialFort.CanKillGate = 0;
			gImperialFort.StandByRun = 0;

			_beginthread(ImperialFort_StandBy1,0,NULL);
			_endthread();
		}
		if ( gImperialFort.StageTimer[1] < 1 )
		{
			gImperialFort.StageTimer[1] = 0;
			gImperialFort.EventState = 0;

			gImperialFort.SendFailure();

			gImperialFort.Clear();
			_endthread();			// End Function
		}
		// ----
		PMSG_FORT_TIMER pMsg;
		// ----
		pMsg.c = 0xC1;
		pMsg.size = 0x0D;
		pMsg.headcode = 0xF7;
		pMsg.subcode = 0x04;
		pMsg.Status = 0x02;
		pMsg.Unk4 = 0x01;
		pMsg.TimerL = LOBYTE(gImperialFort.StageTimer[1]);
		pMsg.TimerH = HIBYTE(gImperialFort.StageTimer[1]);
		pMsg.Unk5 = 0x00;
		pMsg.Mob = gImperialFort.MonstersAddCount;
		
		for(int i=0;i<5;i++)
		{
			if ( gImperialFort.UserIndex[i] != 0 )
			{
				gSendProto.DataSend(gImperialFort.UserIndex[i], (BYTE*)&pMsg, pMsg.size);
			}
		}
		Sleep(1000);
	}
}

void ImperialFort_StandBy1(void * lpParam)
{
	while(true)
	{	
		if( gImperialFort.StageTimer[2] < 1 )
		{	
			gImperialFort.StageTimer[2] = 0;
			gImperialFort.EventState = 1;
			gImperialFort.EventStage = 2;
			gImperialFort.CanKillGate = 1;
			gImperialFort.CanUseSkill = 1;

			gImperialFort.ClearMonsters();
			
			gImperialFort.SetMonsters(1);
			
			_beginthread(ImperialFort_StartStage2,0,NULL);
			_endthread();			// End Function
		}

		gImperialFort.StageTimer[2] -= 4;

		PMSG_FORT_TIMER pMsg;
		pMsg.c = 0xC1;
		pMsg.size = 0x0D;
		pMsg.headcode = 0xF7;
		pMsg.subcode = 0x04;
		pMsg.Status = 0x01;
		pMsg.Unk4 = 0x00;
		pMsg.TimerL = LOBYTE(gImperialFort.StageTimer[2]);
		pMsg.TimerH = HIBYTE(gImperialFort.StageTimer[2]);
		pMsg.Unk5 = 0x00;
		pMsg.Mob = gImperialFort.MonstersAddCount;

		for (int i=0;i<5;i++)
		{
			if ( gImperialFort.UserIndex[i] != 0 )
			{
				gSendProto.DataSend(gImperialFort.UserIndex[i], (BYTE*)&pMsg, pMsg.size);
			}
		}
		Sleep(1000);
	}
	_endthread();
}

void ImperialFort_StartStage2(void * lpParam)
{
	while ( true )
	{
		gImperialFort.StageTimer[3] -= 4;
		// ---- Check Monsters Killed
		if ( gImperialFort.MonstersAddCount <= 0 && gImperialFort.CanKillGate == 0 )
		{
			gImperialFort.CanKillGate = 1;
		}
		else if ( gImperialFort.MonstersAddCount > 0 && gImperialFort.EventState == 0 )
		{
			gImperialFort.CanKillGate = 0;
		}
		if ( gImperialFort.StandByRun == 1 )
		{
			gImperialFort.EventState = 1;
			gImperialFort.StandByRun = 0;
			gImperialFort.CanUseSkill = 0;
			gImperialFort.CanKillGate = 0;

			_beginthread(ImperialFort_StandBy2,0,NULL);
			_endthread();
		}
		// ---- Check Timer
		if ( gImperialFort.StageTimer[3] < 1 )
		{
			gImperialFort.StageTimer[3] = 0;
			gImperialFort.EventState = 0;

			gImperialFort.SendFailure();

			gImperialFort.Clear();
			_endthread();			// End Function
		}
		// ----
		PMSG_FORT_TIMER pMsg;
		// ----
		pMsg.c = 0xC1;
		pMsg.size = 0x0D;
		pMsg.headcode = 0xF7;
		pMsg.subcode = 0x04;
		pMsg.Status = 0x02;
		pMsg.Unk4 = 0x00;
		pMsg.TimerL = LOBYTE(gImperialFort.StageTimer[3]);
		pMsg.TimerH = HIBYTE(gImperialFort.StageTimer[3]);
		pMsg.Unk5 = 0x00;
		pMsg.Mob = gImperialFort.MonstersAddCount;
		
		for(int i=0;i<5;i++)
		{
			if ( gImperialFort.UserIndex[i] != 0 )
			{
				gSendProto.DataSend(gImperialFort.UserIndex[i], (BYTE*)&pMsg, pMsg.size);
			}
		}
		Sleep(1000);
	}
}

void ImperialFort_StandBy2(void * lpParam)
{
	while(true)
	{	
		if( gImperialFort.StageTimer[4] < 1 )
		{	
			gImperialFort.StageTimer[4] = 0;
			gImperialFort.EventState = 1;
			gImperialFort.EventStage = 3;
			gImperialFort.CanUseSkill = 1;
			gImperialFort.CanKillGate = 1;

			gImperialFort.ClearMonsters();
			
			gImperialFort.SetMonsters(2);
			
			_beginthread(ImperialFort_StartStage3,0,NULL);
			_endthread();			// End Function
		}

		gImperialFort.StageTimer[4] -= 4;

		PMSG_FORT_TIMER pMsg;
		pMsg.c = 0xC1;
		pMsg.size = 0x0D;
		pMsg.headcode = 0xF7;
		pMsg.subcode = 0x04;
		pMsg.Status = 0x01;
		pMsg.Unk4 = 0x00;
		pMsg.TimerL = LOBYTE(gImperialFort.StageTimer[4]);
		pMsg.TimerH = HIBYTE(gImperialFort.StageTimer[4]);
		pMsg.Unk5 = 0x00;
		pMsg.Mob = gImperialFort.MonstersAddCount;

		for (int i=0;i<5;i++)
		{
			if ( gImperialFort.UserIndex[i] != 0 )
			{
				gSendProto.DataSend(gImperialFort.UserIndex[i], (BYTE*)&pMsg, pMsg.size);
			}
		}
		Sleep(1000);
	}
	_endthread();
}

void ImperialFort_StartStage3(void * lpParam)
{
	while ( true )
	{
		gImperialFort.StageTimer[5] -= 4;
		// ---- Check Monsters Killed
		if ( gImperialFort.MonstersAddCount <= 0 )
		{
			gImperialFort.EventStage = 4;
			gImperialFort.EventState = 1;

			gImperialFort.ClearMonsters();

			gImperialFort.SetMonsters(3);
			_beginthread(ImperialFort_StartStage4,0,NULL);
			_endthread();
		}
		// ---- Check Timer
		if ( gImperialFort.StageTimer[5] < 1 )
		{
			gImperialFort.StageTimer[5] = 0;
			gImperialFort.EventState = 0;
			gImperialFort.SendFailure();
			
			gImperialFort.Clear();
			_endthread();			// End Function
		}
		// ----
		PMSG_FORT_TIMER pMsg;
		// ----
		pMsg.c = 0xC1;
		pMsg.size = 0x0D;
		pMsg.headcode = 0xF7;
		pMsg.subcode = 0x04;
		pMsg.Status = 0x02;
		pMsg.Unk4 = 0x00;
		pMsg.TimerL = LOBYTE(gImperialFort.StageTimer[5]);
		pMsg.TimerH = HIBYTE(gImperialFort.StageTimer[5]);
		pMsg.Unk5 = 0x00;
		pMsg.Mob = gImperialFort.MonstersAddCount;
		
		for(int i=0;i<5;i++)
		{
			if ( gImperialFort.UserIndex[i] != 0 )
			{
				gSendProto.DataSend(gImperialFort.UserIndex[i], (BYTE*)&pMsg, pMsg.size);
			}
		}
		Sleep(1000);
	}
}

void ImperialFort_StartStage4(void * lpParam)
{
	while ( true )
	{
		gImperialFort.StageTimer[6] -= 4;
		// ---- Check Monsters Killed
		if ( gImperialFort.MonstersAddCount <= 0 )
		{
			if ( gImperialFort.EventDay == 4 )
			{
				gImperialFort.EventStage = 5;
				gImperialFort.EventState = 1;

				gImperialFort.ClearMonsters();

				gImperialFort.SetMonsters(4);
				_beginthread(ImperialFort_StartStage5,0,NULL);
				_endthread();
			}
			else
			{
				//End Event Succes
				gImperialFort.EventState = 0;

				PMSG_FORT_TIMER pMsg1;
				pMsg1.c = 0xC1;
				pMsg1.size = 0x0D;
				pMsg1.headcode = 0xF7;
				pMsg1.subcode = 0x04;
				pMsg1.Status = 0x01;
				pMsg1.Unk4 = 0x00;
				pMsg1.TimerL = 0;
				pMsg1.TimerH = 0;
				pMsg1.Unk5 = 0x00;
				pMsg1.Mob = 0;

				PMSG_FORT_END_MESSAGE pMsg2;
				pMsg2.c = 0xC1;
				pMsg2.size = 0x0D;
				pMsg2.headcode = 0xF7;
				pMsg2.subcode = 0x06;
				pMsg2.Type = 0x02;
				pMsg2.Experience = 10000;
				
				for(int i=0;i<5;i++)
				{
					if ( gImperialFort.UserIndex[i] != 0 )
					{
						gSendProto.DataSend(gImperialFort.UserIndex[i], (BYTE*)&pMsg1, pMsg1.size);
						gSendProto.DataSend(gImperialFort.UserIndex[i], (BYTE*)&pMsg2, pMsg2.size);
					}
				}
				_beginthread( ImperialFort_Winner, 0, NULL  );		
				_endthread();
			}
		}
		// ---- Check Timer
		if ( gImperialFort.StageTimer[6] < 1 )
		{
			gImperialFort.StageTimer[6] = 0;
			gImperialFort.EventState = 0;

			gImperialFort.SendFailure();
			
			gImperialFort.Clear();
			_endthread();			// End Function
		}
		// ----
		PMSG_FORT_TIMER pMsg;
		// ----
		pMsg.c = 0xC1;
		pMsg.size = 0x0D;
		pMsg.headcode = 0xF7;
		pMsg.subcode = 0x04;
		pMsg.Status = 0x02;
		pMsg.Unk4 = 0x00;
		pMsg.TimerL = LOBYTE(gImperialFort.StageTimer[6]);
		pMsg.TimerH = HIBYTE(gImperialFort.StageTimer[6]);
		pMsg.Unk5 = 0x00;
		pMsg.Mob = gImperialFort.MonstersAddCount;
		
		for(int i=0;i<5;i++)
		{
			if ( gImperialFort.UserIndex[i] != 0 )
			{
				gSendProto.DataSend(gImperialFort.UserIndex[i], (BYTE*)&pMsg, pMsg.size);
			}
		}
		Sleep(1000);
	}
}

void ImperialFort_StartStage5(void * lpParam)
{
	while ( true )
	{
		gImperialFort.StageTimer[7] -= 4;
		// ---- Check Monsters Killed
		if ( gImperialFort.MonstersAddCount <= 0 && gImperialFort.CanKillGate == 0)
		{
			gImperialFort.CanKillGate = 1;
		}
		else if ( gImperialFort.MonstersAddCount > 0 && gImperialFort.EventState == 0 )
		{
			gImperialFort.CanKillGate = 0;
		}
		if ( gImperialFort.StandByRun == 1 )
		{
			gImperialFort.EventState = 1;
			gImperialFort.StandByRun = 0;
			gImperialFort.CanUseSkill = 0;
			gImperialFort.CanKillGate = 0;

			_beginthread(ImperialFort_StandBy3,0,NULL);
			_endthread();
		}
		// ---- Check Timer
		if ( gImperialFort.StageTimer[7] < 1 )
		{
			gImperialFort.StageTimer[7] = 0;
			gImperialFort.EventState = 0;

			gImperialFort.SendFailure();
			
			gImperialFort.Clear();
			_endthread();			// End Function
		}
		// ----
		PMSG_FORT_TIMER pMsg;
		// ----
		pMsg.c = 0xC1;
		pMsg.size = 0x0D;
		pMsg.headcode = 0xF7;
		pMsg.subcode = 0x04;
		pMsg.Status = 0x02;
		pMsg.Unk4 = 0x00;
		pMsg.TimerL = LOBYTE(gImperialFort.StageTimer[7]);
		pMsg.TimerH = HIBYTE(gImperialFort.StageTimer[7]);
		pMsg.Unk5 = 0x00;
		pMsg.Mob = gImperialFort.MonstersAddCount;
		
		for(int i=0;i<5;i++)
		{
			if ( gImperialFort.UserIndex[i] != 0 )
			{
				gSendProto.DataSend(gImperialFort.UserIndex[i], (BYTE*)&pMsg, pMsg.size);
			}
		}
		Sleep(1000);
	}
}

void ImperialFort_StandBy3(void * lpParam)
{
	while(true)
	{	
		if( gImperialFort.StageTimer[9] < 1 )
		{	
			gImperialFort.StageTimer[9] = 0;
			gImperialFort.EventState = 1;
			gImperialFort.EventStage = 3;
			gImperialFort.CanUseSkill = 1;
			gImperialFort.CanKillGate = 1;

			gImperialFort.ClearMonsters();
			
			gImperialFort.SetMonsters(5);
			
			_beginthread(ImperialFort_StartStage6,0,NULL);
			_endthread();			// End Function
		}

		gImperialFort.StageTimer[9] -= 4;

		PMSG_FORT_TIMER pMsg;
		pMsg.c = 0xC1;
		pMsg.size = 0x0D;
		pMsg.headcode = 0xF7;
		pMsg.subcode = 0x04;
		pMsg.Status = 0x01;
		pMsg.Unk4 = 0x00;
		pMsg.TimerL = LOBYTE(gImperialFort.StageTimer[9]);
		pMsg.TimerH = HIBYTE(gImperialFort.StageTimer[9]);
		pMsg.Unk5 = 0x00;
		pMsg.Mob = gImperialFort.MonstersAddCount;

		for (int i=0;i<5;i++)
		{
			if ( gImperialFort.UserIndex[i] != 0 )
			{
				gSendProto.DataSend(gImperialFort.UserIndex[i], (BYTE*)&pMsg, pMsg.size);
			}
		}
		Sleep(1000);
	}
	_endthread();
}

void ImperialFort_StartStage6(void * lpParam)
{
	while ( true )
	{
		gImperialFort.StageTimer[8] -= 4;
		// ---- Check Monsters Killed
		if ( gImperialFort.MonstersAddCount <= 0 )
		{
			//End Event Succes
			gImperialFort.EventState = 0;

			PMSG_FORT_TIMER pMsg;
			pMsg.c = 0xC1;
			pMsg.size = 0x0D;
			pMsg.headcode = 0xF7;
			pMsg.subcode = 0x04;
			pMsg.Status = 0x01;
			pMsg.Unk4 = 0x00;
			pMsg.TimerL = 0;
			pMsg.TimerH = 0;
			pMsg.Unk5 = 0x00;
			pMsg.Mob = 0;

			int Experience = 1234567890;
		
			PMSG_FORT_END_MESSAGE pMsg2;
			pMsg2.c = 0xC1;
			pMsg2.size = 0x0D;
			pMsg2.headcode = 0xF7;
			pMsg2.subcode = 0x06;
			pMsg2.Type = 0x02;
			pMsg2.Experience = Experience;

			for ( int i=0;i<5;i++)
			{
				if ( gImperialFort.UserIndex[i] != 0 )
				{
					gSendProto.DataSend(gImperialFort.UserIndex[i], (BYTE*)&pMsg2, pMsg2.size);
				}
			}

			_beginthread(ImperialFort_Winner,0,NULL);
			_endthread();			// End Function
		}
		// ---- Check Timer
		if ( gImperialFort.StageTimer[8] < 1 )
		{
			gImperialFort.StageTimer[8] = 0;
			gImperialFort.EventState = 0;

			gImperialFort.SendFailure();
			
			gImperialFort.Clear();
			_endthread();			// End Function
		}
		// ----
		PMSG_FORT_TIMER pMsg;
		// ----
		pMsg.c = 0xC1;
		pMsg.size = 0x0D;
		pMsg.headcode = 0xF7;
		pMsg.subcode = 0x04;
		pMsg.Status = 0x02;
		pMsg.Unk4 = 0x00;
		pMsg.TimerL = LOBYTE(gImperialFort.StageTimer[8]);
		pMsg.TimerH = HIBYTE(gImperialFort.StageTimer[8]);
		pMsg.Unk5 = 0x00;
		pMsg.Mob = gImperialFort.MonstersAddCount;
		
		for(int i=0;i<5;i++)
		{
			if ( gImperialFort.UserIndex[i] != 0 )
			{
				gSendProto.DataSend(gImperialFort.UserIndex[i], (BYTE*)&pMsg, pMsg.size);
			}
		}
		Sleep(1000);
	}
}

void ImperialFort_Winner(void * lpParam)
{
	Sleep(15000);
	gImperialFort.Clear();
}
// ----------------------------------------------------------------------------------------




// ----------------------------------------------------------------------------------------
bool CImperialFort::MoveToGate(int aIndex)
{
	bool bResult = false;
	// ----
	if ( gImperialFort.EventMap == 72 && gObj[aIndex].MapNumber == 72 )
	{
		if ( (gObj[aIndex].X >= 28 && gObj[aIndex].X <= 34) && (gObj[aIndex].Y >= 93 && gObj[aIndex].Y <= 99) )
		{
			if ( gImperialFort.MonstersAddCount <= 0 )
			{
				gObjTeleport(aIndex,72,33,163);
				if ( gImperialFort.StandByRun == 0 )
				{
					gImperialFort.StandByRun = 1;
				}
				bResult = true;
			}
		}
		else if ( (gObj[aIndex].X >= 66 && gObj[aIndex].X <= 72) && (gObj[aIndex].Y >= 158 && gObj[aIndex].Y <= 164) )
		{
			if ( gImperialFort.MonstersAddCount <= 0 )
			{
				gObjTeleport(aIndex,72,146,156);
				if ( gImperialFort.StandByRun == 0 )
				{
					gImperialFort.StandByRun = 1;
				}
				bResult = true;
			}
		}
		else if ( (gObj[aIndex].X >= 221 && gObj[aIndex].X <= 227) && (gObj[aIndex].Y >= 163 && gObj[aIndex].Y <= 169) )
		{
			if ( gImperialFort.MonstersAddCount <= 0 )
			{
				gObjTeleport(aIndex,72,242,24);
				if ( gImperialFort.StandByRun == 0 )
				{
					gImperialFort.StandByRun = 1;
				}
				bResult = true;
			}
		}
		else
		{
			bResult = true;
		}
	}
	else if ( gImperialFort.EventMap == 71 && gObj[aIndex].MapNumber == 71 )
	{
		if ( (gObj[aIndex].X >= 80 && gObj[aIndex].X <= 86) && (gObj[aIndex].Y >= 192 && gObj[aIndex].Y <= 198) )
		{
			if ( gImperialFort.MonstersAddCount <= 0 )
			{
				gObjTeleport(aIndex,71,223,122);
				if ( gImperialFort.StandByRun == 0 )
				{
					gImperialFort.StandByRun = 1;
				}
				bResult = true;
			}
		}
		else if ( (gObj[aIndex].X >= 220 && gObj[aIndex].X <= 226) && (gObj[aIndex].Y >= 199 && gObj[aIndex].Y <= 205) )
		{
			if ( gImperialFort.MonstersAddCount <= 0 )
			{
				gObjTeleport(aIndex,71,167,206);
				if ( gImperialFort.StandByRun == 0 )
				{
					gImperialFort.StandByRun = 1;
				}
				bResult = true;
			}
		}
		else
		{
			bResult = true;
		}
	}
	else if ( gImperialFort.EventMap == 70 && gObj[aIndex].MapNumber == 70 )
	{
		if ( (gObj[aIndex].X >= 8 && gObj[aIndex].X <= 14) && (gObj[aIndex].Y >= 62 && gObj[aIndex].Y <= 68) )
		{
			if ( gImperialFort.MonstersAddCount <= 0 )
			{
				gObjTeleport(aIndex,70,36,84);
				if ( gImperialFort.StandByRun == 0 )
				{
					gImperialFort.StandByRun = 1;
				}
				bResult = true;
			}
		}
		else if ( (gObj[aIndex].X >= 119 && gObj[aIndex].X <= 125) && (gObj[aIndex].Y >= 108 && gObj[aIndex].Y <= 114) )
		{
			if ( gImperialFort.MonstersAddCount <= 0 )
			{
				gObjTeleport(aIndex,70,122,111);
				if ( gImperialFort.StandByRun == 0 )
				{
					gImperialFort.StandByRun = 1;
				}
				bResult = true;
			}
		}
		else
		{
			bResult = true;
		}
	}
	else if ( gImperialFort.EventMap == 69 && gObj[aIndex].MapNumber == 69 )
	{
		if ( (gObj[aIndex].X >= 207 && gObj[aIndex].X <= 213) && (gObj[aIndex].Y >= 78 && gObj[aIndex].Y <= 84) )
		{
			if ( gImperialFort.MonstersAddCount <= 0 )
			{
				gObjTeleport(aIndex,69,203,25);
				if ( gImperialFort.StandByRun == 0 )
				{
					gImperialFort.StandByRun = 1;
				}
				bResult = true;
			}
		}
		else if ( (gObj[aIndex].X >= 151 && gObj[aIndex].X <= 157) && (gObj[aIndex].Y >= 58 && gObj[aIndex].Y <= 64) )
		{
			if ( gImperialFort.MonstersAddCount <= 0 )
			{
				gObjTeleport(aIndex,69,180,66);
				if ( gImperialFort.StandByRun == 0 )
				{
					gImperialFort.StandByRun = 1;
				}
				bResult = true;
			}
		}
		else
		{
			bResult = true;
		}
	}
	else
	{
		bResult = true;
	}
	return bResult;
}
// ----------------------------------------------------------------------------------------
void CImperialFort::CanMoveNextStage(LPOBJ lpObj)
{
	if ( gImperialFort.Running == 0 )
	{
		return;
	}
	if ( lpObj->MapNumber != gImperialFort.EventMap )
	{
		return;
	}
	// --- Gate 1
	if ( (lpObj->X >= gImperialFort.GateStartX[0][gImperialFort.EventDay-1] && lpObj->X <= gImperialFort.GateEndX[0][gImperialFort.EventDay-1] ) &&
		 (lpObj->Y >= gImperialFort.GateStartY[0][gImperialFort.EventDay-1] && lpObj->Y <= gImperialFort.GateEndY[0][gImperialFort.EventDay-1] ) )
	{
		for(int i=0;i<5;i++)
		{
			if( gImperialFort.UserIndex[i] != 0)
			{	
				if( gImperialFort.UserIndex[i] == lpObj->m_Index )
				{
					if( gImperialFort.EventGateOpen == 1 )
					{
						PMSG_FORT_OPENCLOSE_GATE pMsg;
						pMsg.c = 0xC1;
						pMsg.size = 0x06;
						pMsg.headcode = 0xBF;
						pMsg.subcode = 0x09;
						pMsg.Status = 0x01;
						pMsg.Result = 0x03;
						gSendProto.DataSend(lpObj->m_Index,(BYTE*)&pMsg, pMsg.size);
						gImperialFort.EventGateUserOpen[i] = 0;
					}
				}
			}
		}
	}
	// --- Gate 2
	if ( (lpObj->X >= gImperialFort.GateStartX[1][gImperialFort.EventDay-1] && lpObj->X <= gImperialFort.GateEndX[1][gImperialFort.EventDay-1] ) &&
		 (lpObj->Y >= gImperialFort.GateStartY[1][gImperialFort.EventDay-1] && lpObj->Y <= gImperialFort.GateEndY[1][gImperialFort.EventDay-1] ) )
	{
		for(int i=0;i<5;i++)
		{
			if( gImperialFort.UserIndex[i] != 0)
			{	
				if( gImperialFort.UserIndex[i] == lpObj->m_Index )
				{
					if( gImperialFort.EventGateOpen == 2 )
					{
						PMSG_FORT_OPENCLOSE_GATE pMsg;
						pMsg.c = 0xC1;
						pMsg.size = 0x06;
						pMsg.headcode = 0xBF;
						pMsg.subcode = 0x09;
						pMsg.Status = 0x01;
						pMsg.Result = 0x03;
						gSendProto.DataSend(lpObj->m_Index,(BYTE*)&pMsg, pMsg.size);
						gImperialFort.EventGateUserOpen[i] = 0;
					}
				}
			}
		}
	}
	// --- Gate 3
	if ( (lpObj->X >= gImperialFort.GateStartX[2][gImperialFort.EventDay-1] && lpObj->X <= gImperialFort.GateEndX[2][gImperialFort.EventDay-1] ) &&
		 (lpObj->Y >= gImperialFort.GateStartY[2][gImperialFort.EventDay-1] && lpObj->Y <= gImperialFort.GateEndY[2][gImperialFort.EventDay-1] ) )
	{
		for(int i=0;i<5;i++)
		{
			if( gImperialFort.UserIndex[i] != 0)
			{	
				if( gImperialFort.UserIndex[i] == lpObj->m_Index )
				{
					if( gImperialFort.EventGateOpen == 3 )
					{
						PMSG_FORT_OPENCLOSE_GATE pMsg;
						pMsg.c = 0xC1;
						pMsg.size = 0x06;
						pMsg.headcode = 0xBF;
						pMsg.subcode = 0x09;
						pMsg.Status = 0x01;
						pMsg.Result = 0x03;
						gSendProto.DataSend(lpObj->m_Index,(BYTE*)&pMsg, pMsg.size);
						gImperialFort.EventGateUserOpen[i] = 0;
					}
				}
			}
		}
	}
	// --- Gate 4
	if ( (lpObj->X >= gImperialFort.GateStartX[3][gImperialFort.EventDay-1] && lpObj->X <= gImperialFort.GateEndX[3][gImperialFort.EventDay-1] ) &&
		 (lpObj->Y >= gImperialFort.GateStartY[3][gImperialFort.EventDay-1] && lpObj->Y <= gImperialFort.GateEndY[3][gImperialFort.EventDay-1] ) )
	{
		for(int i=0;i<5;i++)
		{
			if( gImperialFort.UserIndex[i] != 0)
			{	
				if( gImperialFort.UserIndex[i] == lpObj->m_Index )
				{
					if( gImperialFort.EventGateOpen == 4 )
					{
						PMSG_FORT_OPENCLOSE_GATE pMsg;
						pMsg.c = 0xC1;
						pMsg.size = 0x06;
						pMsg.headcode = 0xBF;
						pMsg.subcode = 0x09;
						pMsg.Status = 0x01;
						pMsg.Result = 0x03;
						gSendProto.DataSend(lpObj->m_Index,(BYTE*)&pMsg, pMsg.size);
						gImperialFort.EventGateUserOpen[i] = 0;
					}
				}
			}
		}
	}
	// --- Gate 5
	if ( (lpObj->X >= gImperialFort.GateStartX[4][gImperialFort.EventDay-1] && lpObj->X <= gImperialFort.GateEndX[4][gImperialFort.EventDay-1] ) &&
		 (lpObj->Y >= gImperialFort.GateStartY[4][gImperialFort.EventDay-1] && lpObj->Y <= gImperialFort.GateEndY[4][gImperialFort.EventDay-1] ) )
	{
		for(int i=0;i<5;i++)
		{
			if( gImperialFort.UserIndex[i] != 0)
			{	
				if( gImperialFort.UserIndex[i] == lpObj->m_Index )
				{
					if( gImperialFort.EventGateOpen == 5 )
					{
						PMSG_FORT_OPENCLOSE_GATE pMsg;
						pMsg.c = 0xC1;
						pMsg.size = 0x06;
						pMsg.headcode = 0xBF;
						pMsg.subcode = 0x09;
						pMsg.Status = 0x01;
						pMsg.Result = 0x03;
						gSendProto.DataSend(lpObj->m_Index,(BYTE*)&pMsg, pMsg.size);
						gImperialFort.EventGateUserOpen[i] = 0;
					}
				}
			}
		}
	}
	// --- Gate 6
	if ( (lpObj->X >= gImperialFort.GateStartX[5][gImperialFort.EventDay-1] && lpObj->X <= gImperialFort.GateEndX[5][gImperialFort.EventDay-1] ) &&
		 (lpObj->Y >= gImperialFort.GateStartY[5][gImperialFort.EventDay-1] && lpObj->Y <= gImperialFort.GateEndY[5][gImperialFort.EventDay-1] ) )
	{
		for(int i=0;i<5;i++)
		{
			if( gImperialFort.UserIndex[i] != 0)
			{	
				if( gImperialFort.UserIndex[i] == lpObj->m_Index )
				{
					if( gImperialFort.EventGateOpen == 6 )
					{
						PMSG_FORT_OPENCLOSE_GATE pMsg;
						pMsg.c = 0xC1;
						pMsg.size = 0x06;
						pMsg.headcode = 0xBF;
						pMsg.subcode = 0x09;
						pMsg.Status = 0x01;
						pMsg.Result = 0x03;
						gSendProto.DataSend(lpObj->m_Index,(BYTE*)&pMsg, pMsg.size);
						gImperialFort.EventGateUserOpen[i] = 0;
					}
				}
			}
		}
	}
	// --- Gate 7
	if ( (lpObj->X >= gImperialFort.GateStartX[6][gImperialFort.EventDay-1] && lpObj->X <= gImperialFort.GateEndX[6][gImperialFort.EventDay-1] ) &&
		 (lpObj->Y >= gImperialFort.GateStartY[6][gImperialFort.EventDay-1] && lpObj->Y <= gImperialFort.GateEndY[6][gImperialFort.EventDay-1] ) )
	{
		for(int i=0;i<5;i++)
		{
			if( gImperialFort.UserIndex[i] != 0)
			{	
				if( gImperialFort.UserIndex[i] == lpObj->m_Index )
				{
					if( gImperialFort.EventGateOpen == 7 )
					{
						PMSG_FORT_OPENCLOSE_GATE pMsg;
						pMsg.c = 0xC1;
						pMsg.size = 0x06;
						pMsg.headcode = 0xBF;
						pMsg.subcode = 0x09;
						pMsg.Status = 0x01;
						pMsg.Result = 0x03;
						gSendProto.DataSend(lpObj->m_Index,(BYTE*)&pMsg, pMsg.size);
						gImperialFort.EventGateUserOpen[i] = 0;
					}
				}
			}
		}
	}
	if ( gImperialFort.EventDay == 4 )
	{
		// --- Gate 8
		if ( (lpObj->X >= gImperialFort.GateStartX[7][gImperialFort.EventDay-1] && lpObj->X <= gImperialFort.GateEndX[7][gImperialFort.EventDay-1] ) &&
			 (lpObj->Y >= gImperialFort.GateStartY[7][gImperialFort.EventDay-1] && lpObj->Y <= gImperialFort.GateEndY[7][gImperialFort.EventDay-1] ) )
		{
			for(int i=0;i<5;i++)
			{
				if( gImperialFort.UserIndex[i] != 0)
				{	
					if( gImperialFort.UserIndex[i] == lpObj->m_Index )
					{
						if( gImperialFort.EventGateOpen == 8 )
						{
							PMSG_FORT_OPENCLOSE_GATE pMsg;
							pMsg.c = 0xC1;
							pMsg.size = 0x06;
							pMsg.headcode = 0xBF;
							pMsg.subcode = 0x09;
							pMsg.Status = 0x01;
							pMsg.Result = 0x03;
							gSendProto.DataSend(lpObj->m_Index,(BYTE*)&pMsg, pMsg.size);
							gImperialFort.EventGateUserOpen[i] = 0;
						}
					}
				}
			}
		}

		// --- Gate 9
		if ( (lpObj->X >= gImperialFort.GateStartX[8][gImperialFort.EventDay-1] && lpObj->X <= gImperialFort.GateEndX[8][gImperialFort.EventDay-1] ) &&
			 (lpObj->Y >= gImperialFort.GateStartY[8][gImperialFort.EventDay-1] && lpObj->Y <= gImperialFort.GateEndY[8][gImperialFort.EventDay-1] ) )
		{
			for(int i=0;i<5;i++)
			{
				if( gImperialFort.UserIndex[i] != 0)
				{	
					if( gImperialFort.UserIndex[i] == lpObj->m_Index )
					{
						if( gImperialFort.EventGateOpen == 9 )
						{
							PMSG_FORT_OPENCLOSE_GATE pMsg;
							pMsg.c = 0xC1;
							pMsg.size = 0x06;
							pMsg.headcode = 0xBF;
							pMsg.subcode = 0x09;
							pMsg.Status = 0x01;
							pMsg.Result = 0x03;
							gSendProto.DataSend(lpObj->m_Index,(BYTE*)&pMsg, pMsg.size);
							gImperialFort.EventGateUserOpen[i] = 0;
						}
					}
				}
			}
		}

		// --- Gate 10
		if ( (lpObj->X >= gImperialFort.GateStartX[9][gImperialFort.EventDay-1] && lpObj->X <= gImperialFort.GateEndX[9][gImperialFort.EventDay-1] ) &&
			 (lpObj->Y >= gImperialFort.GateStartY[9][gImperialFort.EventDay-1] && lpObj->Y <= gImperialFort.GateEndY[9][gImperialFort.EventDay-1] ) )
		{
			for(int i=0;i<5;i++)
			{
				if( gImperialFort.UserIndex[i] != 0)
				{	
					if( gImperialFort.UserIndex[i] == lpObj->m_Index )
					{
						if( gImperialFort.EventGateOpen == 10 )
						{
							PMSG_FORT_OPENCLOSE_GATE pMsg;
							pMsg.c = 0xC1;
							pMsg.size = 0x06;
							pMsg.headcode = 0xBF;
							pMsg.subcode = 0x09;
							pMsg.Status = 0x01;
							pMsg.Result = 0x03;
							gSendProto.DataSend(lpObj->m_Index,(BYTE*)&pMsg, pMsg.size);
							gImperialFort.EventGateUserOpen[i] = 0;
						}
					}
				}
			}
		}
	}
	if ( gImperialFort.EventDay != 4 )
	{
		if ( (lpObj->X >= gImperialFort.TrapStartX[0][gImperialFort.EventDay-1] && lpObj->X <= gImperialFort.TrapEndX[0][gImperialFort.EventDay-1] ) &&
			 (lpObj->Y >= gImperialFort.TrapStartY[0][gImperialFort.EventDay-1] && lpObj->Y <= gImperialFort.TrapEndY[0][gImperialFort.EventDay-1] ) )
		{
			gImperialFort.GiveUserDamage(lpObj);
		}

		if ( (lpObj->X >= gImperialFort.TrapStartX[1][gImperialFort.EventDay-1] && lpObj->X <= gImperialFort.TrapEndX[1][gImperialFort.EventDay-1] ) &&
			 (lpObj->Y >= gImperialFort.TrapStartY[1][gImperialFort.EventDay-1] && lpObj->Y <= gImperialFort.TrapEndY[1][gImperialFort.EventDay-1] ) )
		{
			gImperialFort.GiveUserDamage(lpObj);
		}
	}
	else
	{
		if ( (lpObj->X >= gImperialFort.TrapStartX[0][gImperialFort.EventDay-1] && lpObj->X <= gImperialFort.TrapEndX[0][gImperialFort.EventDay-1] ) &&
			 (lpObj->Y >= gImperialFort.TrapStartY[0][gImperialFort.EventDay-1] && lpObj->Y <= gImperialFort.TrapEndY[0][gImperialFort.EventDay-1] ) )
		{
			gImperialFort.GiveUserDamage(lpObj);
		}

		if ( (lpObj->X >= gImperialFort.TrapStartX[1][gImperialFort.EventDay-1] && lpObj->X <= gImperialFort.TrapEndX[1][gImperialFort.EventDay-1] ) &&
			 (lpObj->Y >= gImperialFort.TrapStartY[1][gImperialFort.EventDay-1] && lpObj->Y <= gImperialFort.TrapEndY[1][gImperialFort.EventDay-1] ) )
		{
			gImperialFort.GiveUserDamage(lpObj);
		}

		if ( (lpObj->X >= gImperialFort.TrapStartX[2][gImperialFort.EventDay-1] && lpObj->X <= gImperialFort.TrapEndX[2][gImperialFort.EventDay-1] ) &&
			 (lpObj->Y >= gImperialFort.TrapStartY[2][gImperialFort.EventDay-1] && lpObj->Y <= gImperialFort.TrapEndY[2][gImperialFort.EventDay-1] ) )
		{
			gImperialFort.GiveUserDamage(lpObj);
		}

		if ( (lpObj->X >= gImperialFort.TrapStartX[3][gImperialFort.EventDay-1] && lpObj->X <= gImperialFort.TrapEndX[3][gImperialFort.EventDay-1] ) &&
			 (lpObj->Y >= gImperialFort.TrapStartY[3][gImperialFort.EventDay-1] && lpObj->Y <= gImperialFort.TrapEndY[3][gImperialFort.EventDay-1] ) )
		{
			gImperialFort.GiveUserDamage(lpObj);
		}

		if ( (lpObj->X >= gImperialFort.TrapStartX[4][gImperialFort.EventDay-1] && lpObj->X <= gImperialFort.TrapEndX[4][gImperialFort.EventDay-1] ) &&
			 (lpObj->Y >= gImperialFort.TrapStartY[4][gImperialFort.EventDay-1] && lpObj->Y <= gImperialFort.TrapEndY[4][gImperialFort.EventDay-1] ) )
		{
			gImperialFort.GiveUserDamage(lpObj);
		}

		if ( (lpObj->X >= gImperialFort.TrapStartX[5][gImperialFort.EventDay-1] && lpObj->X <= gImperialFort.TrapEndX[5][gImperialFort.EventDay-1] ) &&
			 (lpObj->Y >= gImperialFort.TrapStartY[5][gImperialFort.EventDay-1] && lpObj->Y <= gImperialFort.TrapEndY[5][gImperialFort.EventDay-1] ) )
		{
			gImperialFort.GiveUserDamage(lpObj);
		}
	}
}
// ----------------------------------------------------------------------------------------
void CImperialFort::GiveUserDamage(LPOBJ lpObj)
{
	int iDamage = 0;
	iDamage = rand() % 1500 + 500;

	if ( lpObj->Connected != PLAYER_PLAYING)
	{
		return;
	}

	if ( lpObj->Life <= 0.0 )
	{
		return;
	}

	PMSG_ATTACKRESULT pResult;
	
	pResult.h.c = 0xC1;
	pResult.h.size = sizeof(pResult);
	pResult.h.headcode = 0xD7;
	pResult.NumberH = HIBYTE(lpObj->m_Index);
	pResult.NumberL = LOBYTE(lpObj->m_Index);
	pResult.DamageH = HIBYTE(iDamage);
	pResult.DamageL = LOBYTE(iDamage);
	pResult.btShieldDamageH = 0;
	pResult.btShieldDamageL = 0;
	pResult.DamageType = 0;

	if ( lpObj->Type == OBJ_USER )
	{
		gSendProto.DataSend(lpObj->m_Index, (LPBYTE)&pResult, pResult.h.size);
	}

	lpObj->Life -= iDamage;

	if ( lpObj->Life <= 0.0 )
	{
		gObj->Life = 0;
		gObj->m_State = 4;
		gObj->KillerType = 1;
		gObj->RegenTime = GetTickCount();
		gObj->DieRegen = 1;
		gObj->RegenMapNumber = 2;
		gObj->RegenMapX = 215;
		gObj->RegenMapY = 38;
		gObj->PathCount = 0;

		GCDiePlayerSend(gObj, gObj->m_Index, 0, 0);
	}
}
// ----------------------------------------------------------------------------------------
void CImperialFort::MonsterKilledManager(LPOBJ lpObj,int MaxHitUser)
{
	if ( gImperialFort.Enabled == 0 )
	{
		return;
	}
	if ( gImperialFort.Running == 0 )
	{
		return;
	}
	// ----
	PMSG_FORT_OPENCLOSE_GATE pMsg;
	pMsg.c = 0xC1;
	pMsg.size = 0x06;
	pMsg.headcode = 0xBF;
	pMsg.subcode = 0x09;
	pMsg.Status = 0x01;
	pMsg.Result = 0x02;
	// ----
	if ( lpObj->Type == OBJ_MONSTER )
	{
		if ( lpObj->Class != 524 && lpObj->Class != 525 && lpObj->Class != 528 && lpObj->Class != 527 )
		{
			for (int i=0;i<255;i++)
			{
				if ( gImperialFort.MonstersAddIndex[i] != -1 )
				{
					if ( (gImperialFort.MonstersAddIndex[i] == lpObj->m_Index) && (gImperialFort.EventMap == lpObj->MapNumber) )
					{
						gImperialFort.MonstersAddCount--;
					}
				}
			}

			switch ( lpObj->Class )
			{
				
			case 504:
				SendMsg.MessageOutMap(gImperialFort.EventMap, OBJ_SEND_ALL_GOLDEN, "Gaion Kharein has been killed by %s", gObj[MaxHitUser].Name);
				break;
			case 505:
				SendMsg.MessageOutMap(gImperialFort.EventMap, OBJ_SEND_ALL_GOLDEN, "Jerint has been killed by %s", gObj[MaxHitUser].Name);
				break;
			case 506:
				SendMsg.MessageOutMap(gImperialFort.EventMap, OBJ_SEND_ALL_GOLDEN, "Raymond has been killed by %s", gObj[MaxHitUser].Name);	
				break;
			case 507:
				SendMsg.MessageOutMap(gImperialFort.EventMap, OBJ_SEND_ALL_GOLDEN, "Erkanne has been killed by %s", gObj[MaxHitUser].Name);			
				break;
			case 508:
				SendMsg.MessageOutMap(gImperialFort.EventMap, OBJ_SEND_ALL_GOLDEN, "Destler has been killed by %s", gObj[MaxHitUser].Name);			
				break;
			case 509:
				SendMsg.MessageOutMap(gImperialFort.EventMap, OBJ_SEND_ALL_GOLDEN, "Vermont has been killed by %s", gObj[MaxHitUser].Name);			
				break;
			case 510:
				SendMsg.MessageOutMap(gImperialFort.EventMap, OBJ_SEND_ALL_GOLDEN, "Kato has been killed by %s", gObj[MaxHitUser].Name);			
				break;
			case 511:
				SendMsg.MessageOutMap(gImperialFort.EventMap, OBJ_SEND_ALL_GOLDEN, "Galia has been killed by %s", gObj[MaxHitUser].Name);			
				break;
			}
		}
		else
		{
			for (int i=0;i<255;i++)
			{
				if ( gImperialFort.GatesAddIndex[i] != -1 )
				{
					if ( (gImperialFort.GatesAddIndex[i] == lpObj->m_Index) && (gImperialFort.GatesAddNum[i] == 1) )
					{
						gImperialFort.EventState = 0;
					}

					if ( gImperialFort.EventState != 1 && gImperialFort.GatesAddNum[i] == 3 && gImperialFort.GatesAddIndex[i] == lpObj->m_Index)
					{
						gImperialFort.EventState = 2;
					}

					if ( gImperialFort.GatesAddNum[i] == 4 && gImperialFort.GatesAddIndex[i] == lpObj->m_Index)
					{
						gImperialFort.EventState = 0;
					}

					if ( gImperialFort.EventState != 1 && gImperialFort.GatesAddNum[i] == 6 && gImperialFort.GatesAddIndex[i] == lpObj->m_Index)
					{
						gImperialFort.EventState = 2;
					}

					if ( gImperialFort.GatesAddNum[i] == 7 && gImperialFort.GatesAddIndex[i] == lpObj->m_Index)
					{
						gImperialFort.EventState = 0;
					}

					if ( gImperialFort.EventState != 1 && gImperialFort.GatesAddNum[i] == 9 && gImperialFort.GatesAddIndex[i] == lpObj->m_Index)
					{
						gImperialFort.EventState = 2;
					}
				}
				// ----
				for(int i=0;i<5;i++)
				{
					if( gImperialFort.UserIndex[i] != 0 && gImperialFort.EventGateUserOpen[i] == 0 )
					{	
						gImperialFort.EventGateUserOpen[i] = 1;
						gSendProto.DataSend(gImperialFort.UserIndex[i],(BYTE*)&pMsg, pMsg.size);
					}
				}
			}
			gImperialFort.EventGateOpen++;
		}
	}
}
// ----------------------------------------------------------------------------------------
void CImperialFort::SetMonsters(int Stage)
{
	char Type = GetIndexType(gImperialFort.EventMap);

	if ( Stage != 3 && Stage != 5 )
	{
		if ( gImperialFort.NormalMonsterCount[Type] <= 0 )
		{
			CLog.LogAdd("[ImperialFort]: Set Monsters -> No Monsters Founded");
			return;
		}
	}
	int result;

	if ( Stage != 3 && Stage != 5 )
	{
		for (int i=0;i<gImperialFort.NormalMonsterCount[Type];i++)
		{
			if ( gImperialFort.NormalMonsterPos[Type][i] != -1 )
			{
				if ( Stage == gMSetBase.m_Mp[gImperialFort.NormalMonsterPos[Type][i]].m_Dir )
				{
					result = gObjAddMonster(gImperialFort.EventMap);

					if ( result >= 0 )
					{
						gObj[result].m_PosNum = gImperialFort.NormalMonsterPos[Type][i];
						gObj[result].X = gMSetBase.m_Mp[gImperialFort.NormalMonsterPos[Type][i]].m_X;
						gObj[result].Y = gMSetBase.m_Mp[gImperialFort.NormalMonsterPos[Type][i]].m_Y;
						gObj[result].MapNumber = gImperialFort.EventMap;
						gObj[result].TX = gObj[result].X;
						gObj[result].TY = gObj[result].Y;
						gObj[result].m_OldX = gObj[result].X;
						gObj[result].m_OldY = gObj[result].Y;
						gObj[result].StartX = gObj[result].X;
						gObj[result].StartY = gObj[result].Y;
						gObjSetMonster(result, gMSetBase.m_Mp[gImperialFort.NormalMonsterPos[Type][i]].m_Type);
						gObj[result].MaxRegenTime = 0;
						gObj[result].Dir = rand()%8;

						gImperialFort.MonstersAddIndex[gImperialFort.MonstersAddCount] = result;
						gImperialFort.MonstersAddCount++;
					}
				}
			}
		}
	}
	else
	{
		if ( Stage == 3 )
		{
			if ( gImperialFort.BossMonsterPos[Type][gImperialFort.NormalBossNum] == -1 )
			{
				return;
			}

			result = gObjAddMonster(gImperialFort.EventMap);

			if ( result >= 0 )
			{
				gObj[result].m_PosNum = gImperialFort.BossMonsterPos[Type][gImperialFort.NormalBossNum];
				gObj[result].X = gMSetBase.m_Mp[gImperialFort.BossMonsterPos[Type][gImperialFort.NormalBossNum]].m_X;
				gObj[result].Y = gMSetBase.m_Mp[gImperialFort.BossMonsterPos[Type][gImperialFort.NormalBossNum]].m_Y;
				gObj[result].MapNumber = gImperialFort.EventMap;
				gObj[result].TX = gObj[result].X;
				gObj[result].TY = gObj[result].Y;
				gObj[result].m_OldX = gObj[result].X;
				gObj[result].m_OldY = gObj[result].Y;
				gObj[result].StartX = gObj[result].X;
				gObj[result].StartY = gObj[result].Y;
				gObjSetMonster(result, gMSetBase.m_Mp[gImperialFort.BossMonsterPos[Type][gImperialFort.NormalBossNum]].m_Type);
				gObj[result].MaxRegenTime = 0;
				gObj[result].Dir = rand()%8;

				gImperialFort.MonstersAddIndex[gImperialFort.MonstersAddCount] = result;
				gImperialFort.MonstersAddCount++;
			}
		}
		else if ( Stage == 5 )
		{
			if ( gImperialFort.FinalBossPos == -1 )
			{
				return;
			}

			result = gObjAddMonster(gImperialFort.EventMap);

			if ( result >= 0 )
			{
				gObj[result].m_PosNum = gImperialFort.FinalBossPos;
				gObj[result].X = gMSetBase.m_Mp[gImperialFort.FinalBossPos].m_X;
				gObj[result].Y = gMSetBase.m_Mp[gImperialFort.FinalBossPos].m_Y;
				gObj[result].MapNumber = gImperialFort.EventMap;
				gObj[result].TX = gObj[result].X;
				gObj[result].TY = gObj[result].Y;
				gObj[result].m_OldX = gObj[result].X;
				gObj[result].m_OldY = gObj[result].Y;
				gObj[result].StartX = gObj[result].X;
				gObj[result].StartY = gObj[result].Y;
				gObjSetMonster(result, gMSetBase.m_Mp[gImperialFort.FinalBossPos].m_Type);
				gObj[result].MaxRegenTime = 0;
				gObj[result].Dir = rand()%8;

				gImperialFort.MonstersAddIndex[gImperialFort.MonstersAddCount] = result;
				gImperialFort.MonstersAddCount++;
			}
		}
	}
}
// ----------------------------------------------------------------------------------------
void CImperialFort::SetGates()
{
	char Type = GetIndexType(gImperialFort.EventMap);

	if ( gImperialFort.GatesCount[Type] <= 0 )
	{
		CLog.LogAdd("[ImperialFort]: Set Gates -> No gates founded");
		return;
	}
	
	int result;

	for (int i=0;i<gImperialFort.GatesCount[Type];i++)
	{
		if ( gImperialFort.GatesPos[Type][i] != -1 )
		{
			result = gObjAddMonster(gImperialFort.EventMap);

			if ( result >= 0 )
			{
				gObj[result].m_PosNum = gImperialFort.GatesPos[Type][i];
				gObj[result].X = gMSetBase.m_Mp[gImperialFort.GatesPos[Type][i]].m_X;
				gObj[result].Y = gMSetBase.m_Mp[gImperialFort.GatesPos[Type][i]].m_Y;
				gObj[result].MapNumber = gImperialFort.EventMap;
				gObj[result].TX = gObj[result].X;
				gObj[result].TY = gObj[result].Y;
				gObj[result].m_OldX = gObj[result].X;
				gObj[result].m_OldY = gObj[result].Y;
				gObj[result].StartX = (BYTE)gObj[result].X;
				gObj[result].StartY = (BYTE)gObj[result].Y;
				gObjSetMonster(result, gMSetBase.m_Mp[gImperialFort.GatesPos[Type][i]].m_Type);
				gObj[result].MaxRegenTime = 0;
				gObj[result].Dir = gMSetBase.m_Mp[gImperialFort.GatesPos[Type][i]].m_Dir;

				gImperialFort.GatesAddIndex[gImperialFort.GatesAddCount] = result;
				gImperialFort.GatesAddNum[gImperialFort.GatesAddCount] = gMSetBase.m_Mp[gImperialFort.GatesPos[Type][i]].m_Dis;
				gImperialFort.GatesAddCount++;
			}
		}
	}
}
// ----------------------------------------------------------------------------------------
void CImperialFort::ClearMonsters()
{
	if ( gImperialFort.MonstersAddCount <= 0 )
	{
		gImperialFort.MonstersAddCount = 0;
		return;
	}

	for (int i=0;i<gImperialFort.MonstersAddCount;i++)
	{
		if ( gImperialFort.MonstersAddIndex[i] != -1 )
		{
			gObjDel(gImperialFort.MonstersAddIndex[i]);
			gImperialFort.MonstersAddIndex[i] = -1;
		}
	}
	gImperialFort.MonstersAddCount = 0;
}
// ----------------------------------------------------------------------------------------
int CImperialFort::CheckScrapPaperPos(LPOBJ lpObj)
{
	for ( int x = 0;x<MAIN_INVENTORY_SIZE;x++ )
	{
		if ( lpObj->pInventory[x].IsItem() == true )
		{
			if ( lpObj->pInventory[x].m_Type == ITEMGET(14, 101) )
			{
				int iITEM_DUR = (int)lpObj->pInventory[x].m_Durability;

				if ( SCRAP_PAPER_RANGE(iITEM_DUR) == TRUE )
				{
						return x;
				}
			}
		}
	}

	return -1;
}
// ----------------------------------------------------------------------------------------
void CImperialFort::ReleseGate(char GateNumber,char Map)
{

}

void CImperialFort::BlockGate(char GateNumber,char Map)
{

}

void CImperialFort::SendBlockInfo(char GateNumber,char Map, int aIndex)
{

}