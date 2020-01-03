#include "stdafx.h"
#include "Functions.H"
#include "User.H"
#include "GameMain.H"
#include "Main.H"
#include "LogProc.H"
#include "MonsterAttr.H"
#include "MonsterItemMng.H"
#include "BloodCastle.H"
#include "ChaosCastle.H"
#include "DevilSquare.H"
#include "IllusionTemple.H"
#include "Raklion.h"
#include "ImperialFort.h"
#include "DoubleGoer.h"
#include "CastleSiege.h"
#include "SendProto.H"


char *GetMonthName(int Month)
{
	switch (Month)
	{
		case 1:
			return "January";
		case 2:
			return "February";
		case 3:
			return "March";
		case 4:
			return "April";
		case 5:
			return "May";
		case 6:
			return "June";
		case 7:
			return "July";
		case 8:
			return "August";
		case 9:
			return "September";
		case 10:
			return "October";
		case 11:
			return "November";
		case 12:
			return "December";
	}
	return NULL;
}
// -----------------------------------------------------------------------------------------------------------------------------------------
char *GetDayName(int Day)
{
	switch (Day)
	{
		case 0:
			return "Sunday";
		case 1:
			return "Monday";
		case 2:
			return "Tuesday";
		case 3:
			return "Wednesday";
		case 4:
			return "Thursday";
		case 5:
			return "Friday";
		case 6:
			return "Saturday";
	}
	return NULL;
}
// -----------------------------------------------------------------------------------------------------------------------------------------
void GCFireWorkSend(int aIndex, int Amount)
{
	int CordX = gObj[aIndex].X;
	int CordY = gObj[aIndex].Y;

	BYTE Packet[7] = {0xC1, 0x07, 0xF3, 0x40, 0x00, CordX, CordY};
	for (int i=0; i<Amount; i++)
	gSendProto.DataSend(aIndex, Packet, Packet[1]);
}
// -----------------------------------------------------------------------------------------------------------------------------------------
char *GetMapName(int MapId)
{
	switch (MapId)
	{
		case 0:
			return "Lorencia";
		case 1:
			return "Dungeon";
		case 2:
			return "Devais";
		case 3:
			return "Noria";
		case 4:
			return "Lost Tower";
		case 5:
			return "Exile";
		case 6:
			return "Arena";
		case 7:
			return "Atlans";
		case 8:
			return "Tarkan";
		case 9:
			return "Devil Square";
		case 10:
			return "Icarus";
		case 30:
			return "Valley of Loren";
		case 31:
			return "Land of Trial";
		case 32:
			return "Devil Square";
		case 33:
			return "Aida";
		case 34:
			return "Crywolf";
		case 35:
			return "Null";
		case 36:
			return "Kalima";
		case 37:
			return "Kantru1";
		case 38:
			return "Kantru2";
		case 39:
			return "Kantru3";
		case 40:
			return "Silent";
		case 41:
			return "Barracks";
		case 42:
			return "Refuge";
		case 43:
			return "Null";
		case 44:
			return "Null";
		case 51:
			return "Elbeland";
		case 52:
			return "Blood Castle";
		case 53:
			return "Chaos Castle";
		case 56:
			return "Swamp of Calmness";
		case 57:
			return "Raklion";
		case 58:
			return "Raklion";
	}
	if (MapId >= 11 && MapId <= 17)
		return "Blood Castle";

	if (MapId >= 18 && MapId <= 23)
		return "Chaos Castle";

	if (MapId >= 24 && MapId <= 29)
		return "Kalima";

	if (MapId >= 45 && MapId <= 50)
		return "Illusion Temple";
	
	return "Unknown";
}
// -----------------------------------------------------------------------------------------------------------------------------------------
void SummonMonster(DWORD PlayerID)
{
	/*LPOBJ lpObj = &gObj[PlayerID];

	short FinalPosX = lpObj->X+1;
	short FinalPosY = lpObj->Y+1;
	// ---- Dark Wizards
	if(lpObj->Class == DB_DARK_WIZARD)
	{
		gObjMonsterCall(PlayerID,CMD_SMN_DWMonster,FinalPosX,FinalPosY);
	}
	else if(lpObj->Class == DB_SOUL_MASTER)
	{
		gObjMonsterCall(PlayerID,CMD_SMN_SMMonster,FinalPosX,FinalPosY);
	}
	else if(lpObj->Class == DB_GRAND_MASTER)
	{
		gObjMonsterCall(PlayerID,CMD_SMN_GMMonster,FinalPosX,FinalPosY);
	}
	// ---- Dark Knights
	if(lpObj->Class == DB_DARK_KNIGHT)
	{
		gObjMonsterCall(PlayerID,CMD_SMN_DKMonster,FinalPosX,FinalPosY);
	}
	else if(lpObj->Class == DB_BLADE_KNIGHT)
	{
		gObjMonsterCall(PlayerID,CMD_SMN_BKMonster,FinalPosX,FinalPosY);
	}
	else if(lpObj->Class == DB_BLADE_MASTER)
	{
		gObjMonsterCall(PlayerID,CMD_SMN_BMMonster,FinalPosX,FinalPosY);
	}
	// ---- Fairy Elfs
	if(lpObj->Class == DB_FAIRY_ELF)
	{
		gObjMonsterCall(PlayerID,CMD_SMN_FEMonster,FinalPosX,FinalPosY);
	}
	else if(lpObj->Class == DB_MUSE_ELF)
	{
		gObjMonsterCall(PlayerID,CMD_SMN_MEMonster,FinalPosX,FinalPosY);
	}
	else if(lpObj->Class == DB_HIGH_ELF)
	{
		gObjMonsterCall(PlayerID,CMD_SMN_HEMonster,FinalPosX,FinalPosY);
	}
	// ---- Magic Gladiators
	if(lpObj->Class == DB_MAGIC_GLADIATOR)
	{
		gObjMonsterCall(PlayerID,CMD_SMN_MGMonster,FinalPosX,FinalPosY);
	}
	else if(lpObj->Class == DB_DUEL_MASTER)
	{
		gObjMonsterCall(PlayerID,CMD_SMN_DMMonster,FinalPosX,FinalPosY);
	}
	// ---- Dark Lords
	if(lpObj->Class == DB_DARK_LORD)
	{
		gObjMonsterCall(PlayerID,CMD_SMN_DLMonster,FinalPosX,FinalPosY);
	}
	else if(lpObj->Class == DB_LORD_EMPEROR)
	{
		gObjMonsterCall(PlayerID,CMD_SMN_LEMonster,FinalPosX,FinalPosY);
	}
	// ---- Summoners
	if(lpObj->Class == DB_SUMMONER)
	{
		gObjMonsterCall(PlayerID,CMD_SMN_SUMonster,FinalPosX,FinalPosY);
	}
	else if(lpObj->Class == DB_BLOODY_SUMMONER)
	{
		gObjMonsterCall(PlayerID,CMD_SMN_BSMonster,FinalPosX,FinalPosY);
	}
	else if(lpObj->Class == DB_DIMENSION_MASTER)
	{
		gObjMonsterCall(PlayerID,CMD_SMN_DSMonster,FinalPosX,FinalPosY);
	}
	// ---- Rage Fighters
	if(lpObj->Class == DB_RAGE_FIGHTER)
	{
		gObjMonsterCall(PlayerID,CMD_SMN_SUMonster,FinalPosX,FinalPosY);
	}
	else if(lpObj->Class == DB_FIST_MASTER)
	{
		gObjMonsterCall(PlayerID,CMD_SMN_BSMonster,FinalPosX,FinalPosY);
	}*/
}
// -----------------------------------------------------------------------------------------------------------------------------------------
void DisconnectAllPlayers()
{
	int n = 0;
	gDisconnect = TRUE;

	for ( n=OBJ_STARTUSERINDEX; n< OBJMAX; n++)
	{
		LPOBJ pObj = &gObj[n];

		if( pObj->Connected >= PLAYER_CONNECTED )
		{
			if( pObj->Type == OBJ_USER )
			{
				gNet.CloseClient(n);
			}
		}
	}

	CLog.LogAddC(TColor.Red(), Func_Module, "[Attention]: All Users have been Disconnected.");
}
// -----------------------------------------------------------------------------------------------------------------------------------------
void LogOutAllUsers()
{
	static BOOL gLogAllUsersOut = FALSE;

	if ( gLogAllUsersOut == FALSE )
	{
		gLogAllUsersOut = TRUE;
		// -----
		DisconnectAllPlayers();
		// -----
		Sleep(1000);
		// -----
		CLog.LogAddC(TColor.Red(), Func_Module, "[Attention]: Whole Connection Closed.");
	}
}
// -----------------------------------------------------------------------------------------------------------------------------------------
void RemoveAllMonstersAndReLoad()
{
	for( int n=0; n<OBJ_MAXMONSTER; n++)
	{
		LPOBJ pObjMonster = &gObj[n];
		// -----
		if( pObjMonster->Type == OBJ_MONSTER || pObjMonster->Type == OBJ_NPC )
		{
			if( pObjMonster->MapNumber == MAP_INDEX_KANTURU_BOSS )	continue;
			// -----
			if( pObjMonster->m_iCurrentAI )
			{
				pObjMonster->Live = FALSE;
				gObjViewportListProtocolDestroy(&gObj[n]);
				gObjViewportClose(&gObj[n]);
			}
			// -----
			gObjDel(n);
		}
	}
	// -----
	gMAttr.LoadAttr(RFile.szRFileName[18]);
	// -----
	#if(!GS_CASTLE)
		gMSetBase.LoadSetBase(RFile.szRFileName[19]);
	#else
		gMSetBase.LoadSetBase("..\\Data\\MonsterSetBase_CS.txt");
	#endif
	// -----
	g_MonsterItemMng.Init();
	// -----
	gObjMonCount = 0;
	// -----
	AddAllMonsters();
}
// -----------------------------------------------------------------------------------------------------------------------------------------
void ReadMapAttrName()
{
	char MapAttrName[MAX_NUMBER_MAP][36] =
	{
		"..\\Data\\Terrains\\Terrain1.att",	// Lorencia 0
		"..\\Data\\Terrains\\Terrain2.att",	// Dungeon 1
		"..\\Data\\Terrains\\Terrain3.att",	// Devias 2
		"..\\Data\\Terrains\\Terrain4.att",	// Noria 3
		"..\\Data\\Terrains\\Terrain5.att",	// Losttower 4
		"..\\Data\\Terrains\\Terrain6.att",	// Exile 5
		"..\\Data\\Terrains\\Terrain7.att",	// Stadium 6
		"..\\Data\\Terrains\\Terrain8.att",	// Atlans 7
		"..\\Data\\Terrains\\Terrain9.att",	// Tarkan 8
		"..\\Data\\Terrains\\Terrain10.att",	// DevilSquare 1 ~ 4 9
		"..\\Data\\Terrains\\Terrain11.att",	// Icarus 10
		"..\\Data\\Terrains\\Terrain12.att",	// BloodCastle 1 11
		"..\\Data\\Terrains\\Terrain12.att",	// BloodCastle 2 12
		"..\\Data\\Terrains\\Terrain12.att",	// BloodCastle 3 13
		"..\\Data\\Terrains\\Terrain12.att",	// BloodCastle 4 14
		"..\\Data\\Terrains\\Terrain12.att",	// BloodCastle 5 15
		"..\\Data\\Terrains\\Terrain12.att",	// BloodCastle 6 16
		"..\\Data\\Terrains\\Terrain12.att",	// BloodCastle 7 17
		"..\\Data\\Terrains\\Terrain19.att",	// ChaosCastle 1 18
		"..\\Data\\Terrains\\Terrain19.att",	// ChaosCastle 2 19
		"..\\Data\\Terrains\\Terrain19.att",	// ChaosCastle 3 20
		"..\\Data\\Terrains\\Terrain19.att",	// ChaosCastle 4 21
		"..\\Data\\Terrains\\Terrain19.att",	// ChaosCastle 5 22
		"..\\Data\\Terrains\\Terrain19.att",	// ChaosCastle 6 23
		"..\\Data\\Terrains\\Terrain25.att",	// Kalima 1 24
		"..\\Data\\Terrains\\Terrain25.att",	// Kalima 2 25
		"..\\Data\\Terrains\\Terrain25.att",	// Kalima 3 26
		"..\\Data\\Terrains\\Terrain25.att",	// Kalima 4 27
		"..\\Data\\Terrains\\Terrain25.att",	// Kalima 5 28
		"..\\Data\\Terrains\\Terrain25.att",	// Kalima 6 29
		"..\\Data\\Terrains\\Terrain31.att",	// Valley Of Loren 30
		"..\\Data\\Terrains\\Terrain32.att",	// Land Of Trials 31
		"..\\Data\\Terrains\\Terrain33.att",	// DevilSquare 5 ~ 8 32
		"..\\Data\\Terrains\\Terrain34.att",	// Aida 33
		"..\\Data\\Terrains\\Terrain35.att",	// CryWolf First Zone 34
		"..\\Data\\Terrains\\Terrain36.att",	// CryWolf Second Zone 35
		"..\\Data\\Terrains\\Terrain37.att",	// Kalima 7 36
		"..\\Data\\Terrains\\Terrain38.att",	// Kanturu 1 37
		"..\\Data\\Terrains\\Terrain39.att",	// Kanturu 2 38
		"..\\Data\\Terrains\\Terrain40.att",	// Kanturu Boss 39
		"..\\Data\\Terrains\\Terrain41.att",	// Silent Map 40
		"..\\Data\\Terrains\\Terrain42.att",	// Barracks 41
		"..\\Data\\Terrains\\Terrain43.att",	// Refuge 42
		"..\\Data\\Terrains\\Terrain43.att",	// ~~~~ 43
		"..\\Data\\Terrains\\Terrain43.att",	// ~~~~ 44
		"..\\Data\\Terrains\\Terrain47.att",	// Illusion Temple 1 45
		"..\\Data\\Terrains\\Terrain47.att",	// Illusion Temple 2 46
		"..\\Data\\Terrains\\Terrain47.att",	// Illusion Temple 3 47
		"..\\Data\\Terrains\\Terrain47.att",	// Illusion Temple 4 48
		"..\\Data\\Terrains\\Terrain47.att",	// Illusion Temple 5 49
		"..\\Data\\Terrains\\Terrain47.att",	// Illusion Temple 6 50
		"..\\Data\\Terrains\\Terrain52.att",	// Elbeland 51
		"..\\Data\\Terrains\\Terrain12.att",	// BloodCastle 8 52
		"..\\Data\\Terrains\\Terrain19.att",	// ChaosCastle 7 53
		"..\\Data\\Terrains\\Terrain52.att",	// ~~~~ 54
		"..\\Data\\Terrains\\Terrain52.att",	// ~~~~ 55
		"..\\Data\\Terrains\\Terrain57.att",	// Swamp Of Calmness 56
		"..\\Data\\Terrains\\Terrain58.att",	// Raklion 57
		"..\\Data\\Terrains\\Terrain59.att",	// Raklion Boss 58
		"..\\Data\\Terrains\\Terrain58.att",	// NULL = 59
		"..\\Data\\Terrains\\Terrain58.att",	// NULL = 60
		"..\\Data\\Terrains\\Terrain58.att",	// NULL = 61
		"..\\Data\\Terrains\\Terrain63.att",	// X-Mas Map = 62
		"..\\Data\\Terrains\\Terrain64.att",	// Vulcan 63
		"..\\Data\\Terrains\\Terrain65.att",	// Vulcan Room 64
		"..\\Data\\Terrains\\Terrain66.att",	// Double Goer 1 65
		"..\\Data\\Terrains\\Terrain67.att",	// Double Goer 2 66
		"..\\Data\\Terrains\\Terrain68.att",	// Double Goer 3 67
		"..\\Data\\Terrains\\Terrain69.att",	// Double Goer 4 68
		"..\\Data\\Terrains\\Terrain70.att",	// Imperial Fort 1 69
		"..\\Data\\Terrains\\Terrain71.att",	// Imperial Fort 2 70
		"..\\Data\\Terrains\\Terrain72.att",	// Imperial Fort 3 71
		"..\\Data\\Terrains\\Terrain73.att",	// Imperial Fort 4 72
		"..\\Data\\Terrains\\Terrain75.att",	// ~~~~ 73
		"..\\Data\\Terrains\\Terrain75.att",  // ~~~~ 74
		"..\\Data\\Terrains\\Terrain75.att",  // ~~~~ 75
		"..\\Data\\Terrains\\Terrain75.att",  // ~~~~ 76
		"..\\Data\\Terrains\\Terrain75.att",  // ~~~~ 77
		"..\\Data\\Terrains\\Terrain75.att",  // ~~~~ 78
		"..\\Data\\Terrains\\Terrain80.att",  // Loren Market 79
		"..\\Data\\Terrains\\Terrain81.att",	// Kalrutan 1 80
		"..\\Data\\Terrains\\Terrain82.att",	// Kalrutan 2 81
	};

	for ( int n=0; n<MAX_NUMBER_MAP; n++)
	{
		MapC[n].ItemInit();
		MapC[n].LoadMapAttr( MapAttrName[n], n );
	}
}
// -----------------------------------------------------------------------------------------------------------------------------------------
int GetWarehouseUsedHowMuch(int UserLevel, BOOL IsLock)
{
	int rZen = 0;
	// -----
	rZen = (UserLevel * UserLevel)* 0.1 * 0.4;
	// -----
	if (bCanWarehouseLock == TRUE)
	{
		if ( IsLock != 0)
		{
			rZen = rZen + ( UserLevel * 2 );
		}
	}
	// -----
	if ( rZen < 1 )
	{
		rZen=1;
	}
	// -----
	if ( rZen >= 1000 )
	{
		rZen = (rZen/100)*100;
	}
	// -----
	else if ( rZen >= 100 )
	{
		rZen = (rZen/10)*10;
	}
	// -----
	if ( rZen == 0 )
	{
		rZen=1;
	}
	// -----
	return rZen;
}
// -----------------------------------------------------------------------------------------------------------------------------------------
void AddAllMonsters()
{
	gMainApp.bCurPaintPlayer = 0;

	int result;

	for ( int n=0; n< gMSetBase.m_Count; n++)
	{
#if(GS_CASTLE==0)
		if ( DS_MAP_RANGE(gMSetBase.m_Mp[n].m_MapNumber) != FALSE )
		{
			continue;
		}

		if ( gUser.BloodCastleMapRange(gMSetBase.m_Mp[n].m_MapNumber) == true )
		{
			if ( gMSetBase.m_Mp[n].m_Type != 232 )
			{
				continue;
			}
		}

		if ( gUser.ChaosCastleMapRange(gMSetBase.m_Mp[n].m_MapNumber) == true )
		{
			continue;
		}

		if ( DG_MAP_RANGE(gMSetBase.m_Mp[n].m_MapNumber) == true )
		{
			continue;
		}

		if ( IF_MAP_RANGE(gMSetBase.m_Mp[n].m_MapNumber) == true )
		{
			continue;
		}

		if ( gMSetBase.m_Mp[n].m_MapNumber == MAP_INDEX_RAKLION_BOSS )
		{
			continue;
		}


		if( IT_MAP_RANGE(gMSetBase.m_Mp[n].m_MapNumber) != FALSE) /* illusion  */
		{
			continue;
		}
#endif

		if ( gMSetBase.m_Mp[n].m_Type == 413 && gMSetBase.m_Mp[n].m_Dir == 1 )
		{
			continue;
		}
			
		result = gObjAddMonster(gMSetBase.m_Mp[n].m_MapNumber);

		if ( result >= 0 )
		{
			gObjSetPosMonster(result, n);
			gObjSetMonster(result, gMSetBase.m_Mp[n].m_Type);
			gMainApp.bCurPaintPlayer++;

			if ( gUser.BloodCastleMapRange(gObj[result].MapNumber) == true )
			{
				if ( gObj[result].Class == 232 )
				{
					gObj[result].m_cBloodCastleIndex = g_BloodCastle.GetBloodCastleIndex(gObj[result].MapNumber);
				}
			}
#if(GS_CASTLE)
			if ( gObj[result].Class == 216 )
			{
				g_CastleSiege.SetCrownIndex(result);
			}
#endif
		}
	}
#if(GS_CASTLE==0)
	g_DevilSquare.Init();
	g_BloodCastle.Init();
	g_ChaosCastle.Init();
	gRaklion.Init();
	gImperialFort.Init();
	gDoubleGoer.Init();
#endif
}
// -----------------------------------------------------------------------------------------------------------------------------------------
char* FileMissed(int File)
{
	switch (File)
	{
		case 0:
			return "CheckSum.dat";
		case 1:
			return "Message%s.wtf";
		case 2:
			return "Item(Kor).txt";
		case 3:
			return "Skill(Kor).txt";
		case 4:
			return "Item(%s).txt";
		case 5:
			return "Skill(%s).txt";
		case 6:
			return "MoveSystem.txt";
		case 7:
			return "Gate.txt";
		case 8:
			return "Quests.txt";
		case 9:
			return "ItemSetOption(%s).txt";
		case 10:
			return "ItemSetType(%s).txt";
		case 11:
			return "SkillSpear.hit";
		case 12:
			return "SkillElect.hit";
		case 13:
			return "JewelOfHarmonyOption.txt";
		case 14:
			return "JewelOfHarmonySmelt.txt";
		case 15:
			return "ItemAddOption.txt";
		case 16:
			return "ConnectMember.txt";
		case 17:
			return "MapServerInfo.dat";
		case 18:
			return "Monster.txt";

		#if(!GS_CASTLE)
		case 19:
			return "MonsterSetBase.txt";
		#else
			return "MonsterSetBase_CS.txt";
		#endif

		case 20:
			return "Iplist.dat";
		case 21:
			return "Connect.ini";
		case 22:
			return "Dec1.dat";
		case 23:
			return "Enc2.dat";
		case 24:
			return "MonsterSkillElement.txt";
		case 25:
			return "MonsterSkillUnit.txt";
		case 26:
			return "MonsterSkill.txt";
		case 27:
			return "MonsterAIElement.txt";
		case 28:
			return "MonsterAutomata.txt";
		case 29:
			return "MonsterAIUnit.txt";
		case 30:
			return "MonsterAIRule.txt";
		case 31:
			return "MonsterAIGroup.txt";
		case 32:
			return "Crywolf.dat";
		case 33:
			return "SkillAdditionInfo.dat";
		case 34:
			return "Kanturu.dat";
		case 35:
			return "KanturuMonsterSetBase.txt";
		case 36:
			return "Shop01.txt";
		case 37:
			return "Shop02.txt";
		case 38:
			return "Shop03.txt";
		case 39:
			return "Shop04.txt";
		case 40:
			return "Shop05.txt";
		case 41:
			return "Shop06.txt";
		case 42:
			return "Shop07.txt";
		case 43:
			return "Shop08.txt";
		case 44:
			return "Shop09.txt";
		case 45:
			return "Shop10.txt";
		case 46:
			return "Shop11.txt";
		case 47:
			return "Shop12.txt";
		case 48:
			return "Shop13.txt";
		case 49:
			return "Shop14.txt";
		case 50:
			return "Shop15.txt";
		case 51:
			return "Shop16.txt";
		case 52:
			return "Shop17.txt";
		case 53:
			return "Shop18.txt";
		case 54:
			return "Shop19.txt";
		case 55:
			return "Shop20.txt";
		case 56:
			return "Shop21.txt";
		case 57:
			return "Shop22.txt";
		case 58:
			return "Shop23.txt";
		case 59:
			return "Shop24.txt";
		case 60:
			return "Shop25.txt";
		case 61:
			return "Shop26.txt";
		case 62:
			return "Shop27.txt";
		case 63:
			return "Shop28.txt";
		case 64:
			return "Shop29.txt";
	}
	return "UnKnown";
}
// -----------------------------------------------------------------------------------------------------------------------------------------
bool gObjIsConnected(int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];
	// ------
	if( !OBJMAX_RANGE(aIndex) ) return false;
	// -----
	if( lpObj->Type != OBJ_USER || lpObj->Connected < PLAYER_PLAYING || lpObj->CloseCount > 0) return false;
	// -----
	return true;
}
// -----------------------------------------------------------------------------------------------------------------------------------------
bool gObjIsConnected(LPOBJ lpObj)
{
	if( !OBJMAX_RANGE(lpObj->m_Index) ) return false;
	// -----
	if( lpObj->Type != OBJ_USER || lpObj->Connected < PLAYER_PLAYING || lpObj->CloseCount > 0) return false;
	// -----
	return true;
}
// -----------------------------------------------------------------------------------------------------------------------------------------
unsigned char GetRandomExeOption(unsigned char MaxOption)
{
	unsigned char ExeOptions[6] = {1,2,4,8,16,32};
	// ----
	unsigned char Random = 0;
	unsigned char RandomEx = 0;
	unsigned char ReturnValue = 0;

	Random = rand()%(MaxOption+1);

	for (int i=0;i<Random;i++)
	{
		while ( true )
		{
			RandomEx = ExeOptions[rand()%6];

			if ( (ReturnValue&RandomEx) == 0 )
			{
				ReturnValue+=RandomEx;
				break;
			}
		}
	}
	return ReturnValue;
}
// -----------------------------------------------------------------------------------------------------------------------------------------
unsigned char Role(unsigned char range)
{
	srand(static_cast<int>(time(NULL)));
	return (rand() % range);
}

char * GetStatus(int Status)
{
	switch (Status)
	{
		case 0:
			return "Empty";
		case 1:
			return "Connecting";
		case 2:
			return "Logging";
		case 3:
			return "Playing";
		default:
			return "Unknown";
	}
}

static BYTE BuxCode[3] = { 0xFC, 0xCF, 0xAB };

void BuxConvert(char* buf, int size)
{
	for (int n=0; n<size; n++)
	{
		buf[n] ^= BuxCode[n%3];
	}
}
// -------------------------------------------------------------------------------------------------------------------------------------------------------
void C1HeadSet(LPBYTE lpBuf, BYTE Head, int Size)
{
	lpBuf[0] = 0xC1;
	lpBuf[1] = Size;
	lpBuf[2] = Head;
}
// -------------------------------------------------------------------------------------------------------------------------------------------------------
void C1SubHeadSet(LPBYTE lpBuf, BYTE Head, BYTE SubH, int Size)
{
	lpBuf[0] = 0xC1;
	lpBuf[1] = Size;
	lpBuf[2] = Head;
	lpBuf[3] = SubH;
}
// -------------------------------------------------------------------------------------------------------------------------------------------------------
void C2HeadSet(LPBYTE lpBuf, BYTE Head, int Size) 
{
	lpBuf[0] = 0xC2;
	lpBuf[1] = SET_NUMBERH(Size);
	lpBuf[2] = SET_NUMBERL(Size);
	lpBuf[3] = Head;
}
// -------------------------------------------------------------------------------------------------------------------------------------------------------
void C2SubHeadSet(LPBYTE lpBuf, BYTE Head, BYTE SubH, int Size)
{
	lpBuf[0] = 0xC2;
	lpBuf[1] = SET_NUMBERH(Size);
	lpBuf[2] = SET_NUMBERL(Size);
	lpBuf[3] = Head;
	lpBuf[4] = SubH;
}
// -------------------------------------------------------------------------------------------------------------------------------------------------------
void C3HeadSet(LPBYTE lpBuf, BYTE Head, int Size)
{
	lpBuf[0] = 0xC3;
	lpBuf[1] = Size;
	lpBuf[2] = Head;
}
// -------------------------------------------------------------------------------------------------------------------------------------------------------
void C3SubPHeadSet(LPBYTE lpBuf, BYTE Head, BYTE SubH, int Size)
{
	lpBuf[0] = 0xC3;
	lpBuf[1] = Size;
	lpBuf[2] = Head;
	lpBuf[3] = SubH;
}

bool FolderExists(std::string file)
{
	DWORD returnvalue;
	returnvalue = GetFileAttributes(file.c_str());
	if(returnvalue == ((DWORD)-1))
	{
		return false;
	}
	else
	{
		return true;
	}
}