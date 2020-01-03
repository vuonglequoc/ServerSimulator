#include "stdafx.h"
#include "BloodCastle.h"
#include "ChaosCastle.h"
#include "GameMain.h"
#include "Main.H"
#include "LogProc.H"
#include "ReadScript.h"
#include "DSProtocol.h"
#include "Protocol.h"
#include "ObjUseSkill.h"
#include "ChaosBox.h"
#include "CastleSiegeSync.h"
#include "DSProtocol.h"
#include "CrywolfSync.h"
#include "Defines.H"
#include "Functions.H"
#include "Party.h"
#include "Notice.H"
#include "SendProto.H"


CBloodCastle g_BloodCastle;


static const struct BLOOD_ZONE
{
	unsigned char btStartX;
	unsigned char btStartY;
	unsigned char btEndX;
	unsigned char btEndY;

} g_btCastleEntranceMapXY[MAX_BLOOD_CASTLE_LEVEL] = {

	13, 15, 15, 23,	// Blood Castle 1
	13, 15, 15, 23,	// Blood Castle 2
	13, 15, 15, 23,	// Blood Castle 3
	13, 15, 15, 23,	// Blood Castle 4
	13, 15, 15, 23,	// Blood Castle 5
	13, 15, 15, 23,	// Blood Castle 6
	13, 15, 15, 23,	// Blood Castle 7
	13, 15, 15, 23	// Blood Castle 8

}, g_btCastleBridgeMapXY[MAX_BLOOD_CASTLE_LEVEL] = {

	13, 70, 15, 75,	// Bridge of Blood Castle 1
	13, 70, 15, 75,	// Bridge of Blood Castle 2
	13, 70, 15, 75,	// Bridge of Blood Castle 3
	13, 70, 15, 75,	// Bridge of Blood Castle 4
	13, 70, 15, 75,	// Bridge of Blood Castle 5
	13, 70, 15, 75,	// Bridge of Blood Castle 6
	13, 70, 15, 75,	// Bridge of Blood Castle 7
	13, 70, 15, 75	// Bridge of Blood Castle 8

}, g_btCastleDoorMapXY[MAX_BLOOD_CASTLE_LEVEL][3]= {

	// Blood Castle 1
	13, 76, 15, 79,	// Door Itself
	11, 80, 25, 89,	// Zone Beginh Door
	 8, 80, 10, 83,	// Altar 

	// Blood Castle 2
	13, 76, 15, 79,	// Door Itself
	11, 80, 25, 89,	// Zone Beginh Door
	 8, 80, 10, 83,	// Altar 

	// Blood Castle 3
	13, 76, 15, 79,	// Door Itself
	11, 80, 25, 89,	// Zone Beginh Door
	 8, 80, 10, 83,	// Altar 

	// Blood Castle 4
	13, 76, 15, 79,	// Door Itself
	11, 80, 25, 89,	// Zone Beginh Door
	 8, 80, 10, 83,	// Altar 

	// Blood Castle 5
	13, 76, 15, 79,	// Door Itself
	11, 80, 25, 89,	// Zone Beginh Door
	 8, 80, 10, 83,	// Altar 

	// Blood Castle 6
	13, 76, 15, 79,	// Door Itself
	11, 80, 25, 89,	// Zone Beginh Door
	 8, 80, 10, 83,	// Altar 

	// Blood Castle 7
	13, 76, 15, 79,	// Door Itself
	11, 80, 25, 89,	// Zone Beginh Door
	 8, 80, 10, 83,	// Altar 

	// Blood Castle 8
	13, 76, 15, 79,	// Door Itself
	11, 80, 25, 89,	// Zone Beginh Door
	 8, 80, 10, 83	// Altar 
};

CBloodCastle::CBloodCastle(void)
{
	for ( int i=0;i<MAX_BLOOD_CASTLE_LEVEL;i++ )
	{
		this->m_BridgeData[i].m_iBC_STATE = BC_STATE_NONE;
		this->m_BridgeData[i].m_iMapNumber = g_BloodCastle.GetBloodCastleMap(i);
		this->m_BridgeData[i].m_iBridgeIndex = i;
		this->m_BridgeData[i].m_iBC_REMAIN_MSEC = -1;
		this->m_BridgeData[i].m_iBC_TICK_COUNT = -1;
		this->m_BridgeData[i].m_nSaintStatuePosNum = -1;
		this->m_BridgeData[i].m_nCastleDoorPosNum = -1;
		this->m_BridgeData[i].m_iAngelKingPosNum = -1;
		InitializeCriticalSection(&this->m_BridgeData[i].m_critUserData);
		this->ClearBridgeData(i);
	}
}

CBloodCastle::~CBloodCastle(void)
{
	for ( int i =0; i<MAX_BLOOD_CASTLE_LEVEL ; i++ )
	{
		DeleteCriticalSection(&this->m_BridgeData[i].m_critUserData);
	}
}

void CBloodCastle::ReadFile()
{
	SMDFile = fopen(BLODDCASTLE_REWARD, "r");

	if ( SMDFile == NULL )
	{
		CLog.MsgBox("[BloodCastle] Info file Load Fail [%s]", BLODDCASTLE_REWARD);
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

			g_BloodCastle.Type[type][ItemCount[type]] = TokenNumber;
				
			Token = GetToken();
			g_BloodCastle.Index[type][ItemCount[type]] = TokenNumber;

			Token = GetToken();
			g_BloodCastle.Level[type][ItemCount[type]] = TokenNumber;

			Token = GetToken();
			g_BloodCastle.Dur[type][ItemCount[type]] = TokenNumber;

			Token = GetToken();
			g_BloodCastle.Skill[type][ItemCount[type]] = TokenNumber;

			Token = GetToken();
			g_BloodCastle.Luck[type][ItemCount[type]] = TokenNumber;

			Token = GetToken();
			g_BloodCastle.Opt[type][ItemCount[type]] = TokenNumber;

			Token = GetToken();
			g_BloodCastle.Exe[type][ItemCount[type]] = TokenNumber;

			Token = GetToken();
			g_BloodCastle.Anc[type][ItemCount[type]] = TokenNumber;

			Token = GetToken();
			g_BloodCastle.Socket[0][type][ItemCount[type]] = TokenNumber;

			Token = GetToken();
			g_BloodCastle.Socket[1][type][ItemCount[type]] = TokenNumber;

			Token = GetToken();
			g_BloodCastle.Socket[2][type][ItemCount[type]] = TokenNumber;

			Token = GetToken();
			g_BloodCastle.Socket[3][type][ItemCount[type]] = TokenNumber;

			Token = GetToken();
			g_BloodCastle.Socket[4][type][ItemCount[type]] = TokenNumber;

			g_BloodCastle.ItemCount[type]++;
		        
		}
    }

	fclose(SMDFile);
	return;
}

char CBloodCastle::GetBloodCastleIndex(int MapNumber)
{
	int btIndex = -1;
	switch ( MapNumber )
	{
	case MAP_INDEX_BLOODCASTLE1:
		btIndex = 0;
		break;
	case MAP_INDEX_BLOODCASTLE2:
		btIndex = 1;
		break;
	case MAP_INDEX_BLOODCASTLE3:
		btIndex = 2;
		break;
	case MAP_INDEX_BLOODCASTLE4:
		btIndex = 3;
		break;
	case MAP_INDEX_BLOODCASTLE5:
		btIndex = 4;
		break;
	case MAP_INDEX_BLOODCASTLE6:
		btIndex = 5;
		break;
	case MAP_INDEX_BLOODCASTLE7:
		btIndex = 6;
		break;
	case MAP_INDEX_BLOODCASTLE8:
		btIndex = 7;
		break;
	}
	return btIndex;
}

int CBloodCastle::GetBloodCastleMap(int BridgeNumber)
{
	int MapNumber = 0;

	switch ( BridgeNumber )
	{
	case 0:
		MapNumber = MAP_INDEX_BLOODCASTLE1;
		break;
	case 1:
		MapNumber = MAP_INDEX_BLOODCASTLE2;
		break;
	case 2:
		MapNumber = MAP_INDEX_BLOODCASTLE3;
		break;
	case 3:
		MapNumber = MAP_INDEX_BLOODCASTLE4;
		break;
	case 4:
		MapNumber = MAP_INDEX_BLOODCASTLE5;
		break;
	case 5:
		MapNumber = MAP_INDEX_BLOODCASTLE6;
		break;
	case 6:
		MapNumber = MAP_INDEX_BLOODCASTLE7;
		break;
	case 7:
		MapNumber = MAP_INDEX_BLOODCASTLE8;
		break;
	}

	return MapNumber;
}

void CBloodCastle::LogBloodCastleWeaponDrop(int BridgeIndex,int level)
{
	/*switch ( level )
	{

	}*/

	/*
	CLog.LogAdd("[Blood Castle] (%d) Ultimate Weapon Dropped -> (%s)",
							g_BloodCastle.GetBloodCastleIndex(lpObj->MapNumber), (level !=0)?((level==1)?"Sword":"Crossbow"):"Staff");
	*/

	return;
}

void CBloodCastle::GiveReward(int aIndex,int BridgeIndex)
{
	LPOBJ lpObj = &gObj[aIndex];
	// ----
	int MaxItemCount = (rand()%(g_BloodCastle.MaxReward[BridgeIndex] - g_BloodCastle.MinReward[BridgeIndex])) + g_BloodCastle.MinReward[BridgeIndex];

	if ( MaxItemCount <= 0 )
	{
		return;
	}

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
	
	for (int i=0;i<MaxItemCount;i++)
	{
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

		RandomItem = rand()%g_BloodCastle.ItemCount[BridgeIndex];

		Item = ITEMGET(g_BloodCastle.Type[BridgeIndex][RandomItem],g_BloodCastle.Index[BridgeIndex][RandomItem]);

		if ( rand()%1000 < 50 )
		{
			Skill = g_BloodCastle.Skill[BridgeIndex][RandomItem];
		}

		if ( rand()%1000 < 50 )
		{
			Luck = g_BloodCastle.Luck[BridgeIndex][RandomItem];
		}

		if ( rand()%1000 < 50 )
		{
			if ( g_BloodCastle.Level[BridgeIndex][RandomItem] > 0 )
			{
				Level = rand()%(g_BloodCastle.Level[BridgeIndex][RandomItem]+1);
			}
			else
			{
				Level = 0;
			}
		}

		if ( rand()%1000 < 50 )
		{
			if ( g_BloodCastle.Opt[BridgeIndex][RandomItem] > 0 )
			{
				Opt = rand()%(g_BloodCastle.Opt[BridgeIndex][RandomItem]+1);
			}
			else
			{
				Opt = 0;
			}
		}

		if ( rand()%1000 < 50 )
		{
			Exe = GetRandomExeOption(g_BloodCastle.Exe[BridgeIndex][RandomItem]+1);
		}

		if ( rand()%1000 < 10 )
		{
			if ( g_BloodCastle.Anc[BridgeIndex][RandomItem] == 5 || g_BloodCastle.Anc[BridgeIndex][RandomItem] == 10 )
			{
				Anc = g_BloodCastle.Anc[BridgeIndex][RandomItem];
			}
			else
			{
				Anc = 0;
			}
		}
		if ( rand()%1000 < 50 )
		{
			Socket[0] = g_BloodCastle.Socket[0][BridgeIndex][RandomItem];

			if ( rand()%1000 < 50 )
			{
				Socket[1] = g_BloodCastle.Socket[1][BridgeIndex][RandomItem];

				if ( rand()%1000 < 50 )
				{
					Socket[2] = g_BloodCastle.Socket[2][BridgeIndex][RandomItem];

					if ( rand()%1000 < 50 )
					{
						Socket[3] = g_BloodCastle.Socket[3][BridgeIndex][RandomItem];

						if ( rand()%1000 < 50 )
						{
							Socket[4] = g_BloodCastle.Socket[4][BridgeIndex][RandomItem];
						}
					}
				}
			}
		}

		ItemSerialCreateSend(aIndex,lpObj->MapNumber,lpObj->X,lpObj->Y,Item,Level,Dur,Luck,Skill,Opt,aIndex,Exe,Anc,Socket);

	}
}

void CBloodCastle::Init()
{
	// ---- Common
	g_BloodCastle.Enabled							= GetPrivateProfileInt("BloodCastle","BloodCastleEnabled",0,BLOODCASTLE_PATH);
	g_BloodCastle.TimeMinOpen						= GetPrivateProfileInt("BloodCastle","BloodCastleTimeOpen",0,BLOODCASTLE_PATH);
	g_BloodCastle.TimeMinPlay						= GetPrivateProfileInt("BloodCastle","BloodCastleTimePlay",0,BLOODCASTLE_PATH);
	g_BloodCastle.TimeMinRest						= GetPrivateProfileInt("BloodCastle","BloodCastleTimeRest",0,BLOODCASTLE_PATH);
	g_BloodCastle.StartHour							= GetPrivateProfileInt("BloodCastle","BloodCastleStartHour",0,BLOODCASTLE_PATH);
	g_BloodCastle.m_iBC_NORMAL_ITEM_DROP			= GetPrivateProfileInt("BloodCastle","BloodCastleNormalItemDrop",0,BLOODCASTLE_PATH);
	g_BloodCastle.m_iBC_EXCEL_ITEM_DROP				= GetPrivateProfileInt("BloodCastle","BloodCastleExeItemDrop",0,BLOODCASTLE_PATH);
	g_BloodCastle.m_iBC_MONSTER_REGEN				= GetPrivateProfileInt("BloodCastle","BloodCastleMonsterRegen",0,BLOODCASTLE_PATH);
	// ----

	g_BloodCastle.BloodCastleMixSuccess[0]			= GetPrivateProfileInt("BloodCastle","BloodCastle1MixSuccessRate",0,BLOODCASTLE_PATH);
	g_BloodCastle.BloodCastleMixSuccess[1]			= GetPrivateProfileInt("BloodCastle","BloodCastle2MixSuccessRate",0,BLOODCASTLE_PATH);
	g_BloodCastle.BloodCastleMixSuccess[2]			= GetPrivateProfileInt("BloodCastle","BloodCastle3MixSuccessRate",0,BLOODCASTLE_PATH);
	g_BloodCastle.BloodCastleMixSuccess[3]			= GetPrivateProfileInt("BloodCastle","BloodCastle4MixSuccessRate",0,BLOODCASTLE_PATH);
	g_BloodCastle.BloodCastleMixSuccess[4]			= GetPrivateProfileInt("BloodCastle","BloodCastle5MixSuccessRate",0,BLOODCASTLE_PATH);
	g_BloodCastle.BloodCastleMixSuccess[5]			= GetPrivateProfileInt("BloodCastle","BloodCastle6MixSuccessRate",0,BLOODCASTLE_PATH);
	g_BloodCastle.BloodCastleMixSuccess[6]			= GetPrivateProfileInt("BloodCastle","BloodCastle7MixSuccessRate",0,BLOODCASTLE_PATH);
	g_BloodCastle.BloodCastleMixSuccess[7]			= GetPrivateProfileInt("BloodCastle","BloodCastle8MixSuccessRate",0,BLOODCASTLE_PATH);

	g_BloodCastle.BloodCastleMixMoney[0]			= GetPrivateProfileInt("BloodCastle","BloodCastle1MixMoney",0,BLOODCASTLE_PATH);
	g_BloodCastle.BloodCastleMixMoney[1]			= GetPrivateProfileInt("BloodCastle","BloodCastle2MixMoney",0,BLOODCASTLE_PATH);
	g_BloodCastle.BloodCastleMixMoney[2]			= GetPrivateProfileInt("BloodCastle","BloodCastle3MixMoney",0,BLOODCASTLE_PATH);
	g_BloodCastle.BloodCastleMixMoney[3]			= GetPrivateProfileInt("BloodCastle","BloodCastle4MixMoney",0,BLOODCASTLE_PATH);
	g_BloodCastle.BloodCastleMixMoney[4]			= GetPrivateProfileInt("BloodCastle","BloodCastle5MixMoney",0,BLOODCASTLE_PATH);
	g_BloodCastle.BloodCastleMixMoney[5]			= GetPrivateProfileInt("BloodCastle","BloodCastle6MixMoney",0,BLOODCASTLE_PATH);
	g_BloodCastle.BloodCastleMixMoney[6]			= GetPrivateProfileInt("BloodCastle","BloodCastle7MixMoney",0,BLOODCASTLE_PATH);
	g_BloodCastle.BloodCastleMixMoney[7]			= GetPrivateProfileInt("BloodCastle","BloodCastle8MixMoney",0,BLOODCASTLE_PATH);

	g_BloodCastle.m_BridgeData[0].m_iCastleStatueHealth		= GetPrivateProfileInt("BloodCastle","BloodCastle1StatueHP",0,BLOODCASTLE_PATH);
	g_BloodCastle.m_BridgeData[1].m_iCastleStatueHealth		= GetPrivateProfileInt("BloodCastle","BloodCastle2StatueHP",0,BLOODCASTLE_PATH);
	g_BloodCastle.m_BridgeData[2].m_iCastleStatueHealth		= GetPrivateProfileInt("BloodCastle","BloodCastle3StatueHP",0,BLOODCASTLE_PATH);
	g_BloodCastle.m_BridgeData[3].m_iCastleStatueHealth		= GetPrivateProfileInt("BloodCastle","BloodCastle4StatueHP",0,BLOODCASTLE_PATH);
	g_BloodCastle.m_BridgeData[4].m_iCastleStatueHealth		= GetPrivateProfileInt("BloodCastle","BloodCastle5StatueHP",0,BLOODCASTLE_PATH);
	g_BloodCastle.m_BridgeData[5].m_iCastleStatueHealth		= GetPrivateProfileInt("BloodCastle","BloodCastle6StatueHP",0,BLOODCASTLE_PATH);
	g_BloodCastle.m_BridgeData[6].m_iCastleStatueHealth		= GetPrivateProfileInt("BloodCastle","BloodCastle7StatueHP",0,BLOODCASTLE_PATH);
	g_BloodCastle.m_BridgeData[7].m_iCastleStatueHealth		= GetPrivateProfileInt("BloodCastle","BloodCastle8StatueHP",0,BLOODCASTLE_PATH);

	g_BloodCastle.m_BridgeData[0].m_iCastleDoorHealth		= GetPrivateProfileInt("BloodCastle","BloodCastle1DoorHP",0,BLOODCASTLE_PATH);
	g_BloodCastle.m_BridgeData[1].m_iCastleDoorHealth		= GetPrivateProfileInt("BloodCastle","BloodCastle2DoorHP",0,BLOODCASTLE_PATH);
	g_BloodCastle.m_BridgeData[2].m_iCastleDoorHealth		= GetPrivateProfileInt("BloodCastle","BloodCastle3DoorHP",0,BLOODCASTLE_PATH);
	g_BloodCastle.m_BridgeData[3].m_iCastleDoorHealth		= GetPrivateProfileInt("BloodCastle","BloodCastle4DoorHP",0,BLOODCASTLE_PATH);
	g_BloodCastle.m_BridgeData[4].m_iCastleDoorHealth		= GetPrivateProfileInt("BloodCastle","BloodCastle5DoorHP",0,BLOODCASTLE_PATH);
	g_BloodCastle.m_BridgeData[5].m_iCastleDoorHealth		= GetPrivateProfileInt("BloodCastle","BloodCastle6DoorHP",0,BLOODCASTLE_PATH);
	g_BloodCastle.m_BridgeData[6].m_iCastleDoorHealth		= GetPrivateProfileInt("BloodCastle","BloodCastle7DoorHP",0,BLOODCASTLE_PATH);
	g_BloodCastle.m_BridgeData[7].m_iCastleDoorHealth		= GetPrivateProfileInt("BloodCastle","BloodCastle8DoorHP",0,BLOODCASTLE_PATH);

	g_BloodCastle.BloodCastleZenReward[0]			= GetPrivateProfileInt("BloodCastle","BloodCastle1ZenReward",0,BLOODCASTLE_PATH);
	g_BloodCastle.BloodCastleZenReward[1]			= GetPrivateProfileInt("BloodCastle","BloodCastle2ZenReward",0,BLOODCASTLE_PATH);
	g_BloodCastle.BloodCastleZenReward[2]			= GetPrivateProfileInt("BloodCastle","BloodCastle3ZenReward",0,BLOODCASTLE_PATH);
	g_BloodCastle.BloodCastleZenReward[3]			= GetPrivateProfileInt("BloodCastle","BloodCastle4ZenReward",0,BLOODCASTLE_PATH);
	g_BloodCastle.BloodCastleZenReward[4]			= GetPrivateProfileInt("BloodCastle","BloodCastle5ZenReward",0,BLOODCASTLE_PATH);
	g_BloodCastle.BloodCastleZenReward[5]			= GetPrivateProfileInt("BloodCastle","BloodCastle6ZenReward",0,BLOODCASTLE_PATH);
	g_BloodCastle.BloodCastleZenReward[6]			= GetPrivateProfileInt("BloodCastle","BloodCastle7ZenReward",0,BLOODCASTLE_PATH);
	g_BloodCastle.BloodCastleZenReward[7]			= GetPrivateProfileInt("BloodCastle","BloodCastle8ZenReward",0,BLOODCASTLE_PATH);

	g_BloodCastle.MinReward[0]						= GetPrivateProfileInt("BloodCastle","BloodCastle1MinRewardCount",0,BLOODCASTLE_PATH);
	g_BloodCastle.MinReward[1]						= GetPrivateProfileInt("BloodCastle","BloodCastle2MinRewardCount",0,BLOODCASTLE_PATH);
	g_BloodCastle.MinReward[2]						= GetPrivateProfileInt("BloodCastle","BloodCastle3MinRewardCount",0,BLOODCASTLE_PATH);
	g_BloodCastle.MinReward[3]						= GetPrivateProfileInt("BloodCastle","BloodCastle4MinRewardCount",0,BLOODCASTLE_PATH);
	g_BloodCastle.MinReward[4]						= GetPrivateProfileInt("BloodCastle","BloodCastle5MinRewardCount",0,BLOODCASTLE_PATH);
	g_BloodCastle.MinReward[5]						= GetPrivateProfileInt("BloodCastle","BloodCastle6MinRewardCount",0,BLOODCASTLE_PATH);
	g_BloodCastle.MinReward[6]						= GetPrivateProfileInt("BloodCastle","BloodCastle7MinRewardCount",0,BLOODCASTLE_PATH);
	g_BloodCastle.MinReward[7]						= GetPrivateProfileInt("BloodCastle","BloodCastle8MinRewardCount",0,BLOODCASTLE_PATH);

	g_BloodCastle.MaxReward[0]						= GetPrivateProfileInt("BloodCastle","BloodCastle1MaxRewardCount",0,BLOODCASTLE_PATH);
	g_BloodCastle.MaxReward[1]						= GetPrivateProfileInt("BloodCastle","BloodCastle2MaxRewardCount",0,BLOODCASTLE_PATH);
	g_BloodCastle.MaxReward[2]						= GetPrivateProfileInt("BloodCastle","BloodCastle3MaxRewardCount",0,BLOODCASTLE_PATH);
	g_BloodCastle.MaxReward[3]						= GetPrivateProfileInt("BloodCastle","BloodCastle4MaxRewardCount",0,BLOODCASTLE_PATH);
	g_BloodCastle.MaxReward[4]						= GetPrivateProfileInt("BloodCastle","BloodCastle5MaxRewardCount",0,BLOODCASTLE_PATH);
	g_BloodCastle.MaxReward[5]						= GetPrivateProfileInt("BloodCastle","BloodCastle6MaxRewardCount",0,BLOODCASTLE_PATH);
	g_BloodCastle.MaxReward[6]						= GetPrivateProfileInt("BloodCastle","BloodCastle7MaxRewardCount",0,BLOODCASTLE_PATH);
	g_BloodCastle.MaxReward[7]						= GetPrivateProfileInt("BloodCastle","BloodCastle8MaxRewardCount",0,BLOODCASTLE_PATH);

	for (int h=0;h<MAX_BLOOD_CASTLE_LEVEL;h++)
	{
		g_BloodCastle.ItemCount[h] = 0;
		for (int j=0;j<255;j++)
		{
			g_BloodCastle.Type[h][j] = -1;
			g_BloodCastle.Index[h][j] = -1;
			g_BloodCastle.Level[h][j] = 0;
			g_BloodCastle.Dur[h][j] = 0;
			g_BloodCastle.Skill[h][j] = 0;
			g_BloodCastle.Luck[h][j] = 0;
			g_BloodCastle.Exe[h][j] = 0;
			g_BloodCastle.Anc[h][j] = 0;
			g_BloodCastle.Socket[0][h][j] = 255;
			g_BloodCastle.Socket[1][h][j] = 255;
			g_BloodCastle.Socket[2][h][j] = 255;
			g_BloodCastle.Socket[3][h][j] = 255;
			g_BloodCastle.Socket[4][h][j] = 255;
		}

		if ( g_BloodCastle.MinReward[h] < 0 ) 
		{
			g_BloodCastle.MinReward[h] = 0;
		}
		if ( g_BloodCastle.MaxReward[h] < 0 )
		{
			g_BloodCastle.MaxReward[h] = 0;
		}
		if ( g_BloodCastle.MinReward[h] > g_BloodCastle.MaxReward[h] )
		{
			g_BloodCastle.MinReward[h] = g_BloodCastle.MaxReward[h];
		}

	}



	for ( int i=0; i<MAX_BLOOD_CASTLE_LEVEL ; i++ )
	{
		for ( int n=0;n<MAX_BLOOD_CASTLE_BOSS_MONSTER;n++)
		{
			this->m_BridgeData[i].m_nBossMonsterPosNum[n] = -1;
		}
		this->m_BridgeData[i].m_iAngelKingPosNum = -1;
	}

	for ( int n =0; n<gMSetBase.m_Count;n++)
	{
		if ( gUser.BloodCastleMapRange(gMSetBase.m_Mp[n].m_MapNumber) == true )
		{
			unsigned short btMonsterType = gMSetBase.m_Mp[n].m_Type;
			unsigned char btMapNumber = gMSetBase.m_Mp[n].m_MapNumber;
			unsigned char btBloodCastleIndex = g_BloodCastle.GetBloodCastleIndex(btMapNumber);

			if ( btMonsterType == 232 )
			{
				this->m_BridgeData[btBloodCastleIndex].m_iAngelKingPosNum = n;
				continue;
			}
			
			if ( BC_STATUE_RANGE(btMonsterType - 132) == true )
			{
				this->m_BridgeData[btBloodCastleIndex].m_nSaintStatuePosNum = n;
				continue;
			}
			
			if ( btMonsterType == 131 )
			{
				this->m_BridgeData[btBloodCastleIndex].m_nCastleDoorPosNum = n;
				continue;
			}
			
			if ( btMonsterType == 89 || btMonsterType == 95 || btMonsterType == 112 || btMonsterType == 118 || btMonsterType == 124 || btMonsterType == 130 || btMonsterType == 143 || btMonsterType == 433 )
			{
				for ( int l=0;l<MAX_BLOOD_CASTLE_BOSS_MONSTER;l++)
				{
					if ( this->m_BridgeData[btBloodCastleIndex].m_nBossMonsterPosNum[l] == -1 )
					{
						this->m_BridgeData[btBloodCastleIndex].m_nBossMonsterPosNum[l] = n;
						break;
					}
				}
			}
		}

	}

	for (int i=0;i<MAX_BLOOD_CASTLE_LEVEL;i++)
	{
		this->SetState(i, BC_STATE_CLOSED);
	}

	g_BloodCastle.ReadFile();

	CLog.LogAdd("[Blood Castle]: Initializated");
}

void CBloodCastle::RequestEnterBloodCastle(PMSG_REQ_MOVEBLOODCASTLE* lpMsg, int iIndex)
{
	if ( OBJMAX_RANGE(iIndex) == false )
	{
		CLog.LogAdd("[Blood Castle]: Request Enter Index Error %d",iIndex);
		return;
	}
	
	unsigned char btBridgeNumber = lpMsg->iBridgeNumber - 1;
	unsigned char btInvisibleCourtItemPos = lpMsg->iItemPos;
	int iITEM_LEVEL = 0;
	unsigned long dwITEM_SERIAL = 0;
	PMSG_RESULT_MOVEBLOODCASTLE pResult;

	C1HeadSet((LPBYTE)&pResult, 0x9A, sizeof(pResult));
	pResult.Result = 0;

	if ( gObj[iIndex].Type != OBJ_USER || gObj[iIndex].Connected <= PLAYER_LOGGED )
	{
		return;
	}

	if ( gObj[iIndex].m_IfState.use && gObj[iIndex].m_IfState.type != 12 )
	{
		return;
	}

	if ( gObj[iIndex].m_PK_Level >= 4 )
	{
		pResult.Result = 7;
		gSendProto.DataSend(iIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	if ( g_BloodCastle.CheckCanEnter(btBridgeNumber) == false )
	{
		pResult.Result = 2;
		gSendProto.DataSend(iIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	if ( gObj[iIndex].pInventory[btInvisibleCourtItemPos].IsItem() == true )
	{
		if ( gObj[iIndex].pInventory[btInvisibleCourtItemPos].m_Type == ITEMGET(13,18) )
		{
			iITEM_LEVEL = gObj[iIndex].pInventory[btInvisibleCourtItemPos].m_Level;
			dwITEM_SERIAL = gObj[iIndex].pInventory[btInvisibleCourtItemPos].m_Number;
			
			if ( BC_CLOACK_LEVEL_RANGE(iITEM_LEVEL) == false )
			{
				pResult.Result = 1;
				gSendProto.DataSend(iIndex, (LPBYTE)&pResult, pResult.h.size);
				return;
			}
		}
		else if ( gObj[iIndex].pInventory[btInvisibleCourtItemPos].m_Type == ITEMGET(13,47) &&
				  gObj[iIndex].pInventory[btInvisibleCourtItemPos].m_Durability > 0.0f )
		{
			for ( int i=0;i<MAX_CLOACK_LEVEL;i++)
			{
				if ( g_BloodCastle.CheckEnterLevel(iIndex, i+1) == 0 )
				{
					iITEM_LEVEL = i+1;
					break;
				}
			}

			dwITEM_SERIAL = gObj[iIndex].pInventory[btInvisibleCourtItemPos].m_Number;

			if ( BC_CLOACK_LEVEL_RANGE(iITEM_LEVEL) == false )
			{
				pResult.Result = 1;
				gSendProto.DataSend(iIndex, (LPBYTE)&pResult, pResult.h.size);
				return;
			}
		}
		else
		{
			pResult.Result = 1;
			gSendProto.DataSend(iIndex, (LPBYTE)&pResult, pResult.h.size);
			return;
		}
	}
	else
	{
		pResult.Result = 1;
		gSendProto.DataSend(iIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}
	
	int iRESULT = g_BloodCastle.CheckEnterLevel(iIndex, iITEM_LEVEL);

	if ( iRESULT== 0 )
	{
		pResult.Result = 0;
		gSendProto.DataSend(iIndex, (LPBYTE)&pResult, pResult.h.size);
	}
	else 
	{
		if ( iRESULT == -1 )
		{
			pResult.Result = 4;
			gSendProto.DataSend(iIndex, (LPBYTE)&pResult, pResult.h.size);
			return;
		}

		if ( iRESULT == 1 )
		{
			pResult.Result = 3;
			gSendProto.DataSend(iIndex, (LPBYTE)&pResult, pResult.h.size);
			return;
		}
		return;
	}
		
	int iBC_INDEX = g_BloodCastle.EnterUserBridge(btBridgeNumber, iIndex);

	if ( iBC_INDEX == -1 )
	{
		pResult.Result = 5;
		gSendProto.DataSend(iIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	if ( (gObj[iIndex].pInventory[btInvisibleCourtItemPos].m_Type == ITEMGET(13,18) || (gObj[iIndex].pInventory[btInvisibleCourtItemPos].m_Type == ITEMGET(13,47)) && gObj[iIndex].pInventory[btInvisibleCourtItemPos].m_Durability == 1.0f ))
	{
		gObjInventoryDeleteItem(iIndex, btInvisibleCourtItemPos);
		GCInventoryItemDeleteSend(iIndex, btInvisibleCourtItemPos, 1);
		gSendProto.DataSend(iIndex, (LPBYTE)&pResult, pResult.h.size);
	}
	else if (gObj[iIndex].pInventory[btInvisibleCourtItemPos].m_Type == ITEMGET(13,47) && gObj[iIndex].pInventory[btInvisibleCourtItemPos].m_Durability > 1.0f )
	{
		gObj[iIndex].pInventory[btInvisibleCourtItemPos].m_Durability -= 1.0f;
		GCItemDurSend2(iIndex, btInvisibleCourtItemPos, gObj[iIndex].pInventory[btInvisibleCourtItemPos].m_Durability, 0);
	}

	gObj[iIndex].m_cBloodCastleIndex = btBridgeNumber;
	gObj[iIndex].m_cBloodCastleSubIndex = iBC_INDEX;
	gObj[iIndex].m_iBloodCastleEXP = 0;
	gObj[iIndex].m_IfState.state = 0;
	gObj[iIndex].m_IfState.type = 0;
	gObj[iIndex].m_IfState.use = 0;

	g_BloodCastle.SearchUserDeleteQuestItem(iIndex);
	SendMsg.MessageOut(iIndex, 0x01, "You have entered Blood Castle %d",btBridgeNumber+1);
	
	switch ( btBridgeNumber )
	{
		case 0:
			gObjMoveGate(iIndex, 66);
			break;
		case 1:
			gObjMoveGate(iIndex, 67);
			break;
		case 2:
			gObjMoveGate(iIndex, 68);
			break;
		case 3:
			gObjMoveGate(iIndex, 69);
			break;
		case 4:
			gObjMoveGate(iIndex, 70);
			break;
		case 5:
			gObjMoveGate(iIndex, 71);
			break;
		case 6:
			gObjMoveGate(iIndex, 80);
			break;
		case 7:
			gObjMoveGate(iIndex, 271);
			break;
	}

	CLog.LogAdd("[Blood Castle] (%d) (Account:%s, Name:%s) Entered Blood Castle (Invisible Cloak Serial:%d)", iITEM_LEVEL, gObj[iIndex].AccountID, gObj[iIndex].Name, dwITEM_SERIAL);
}

void CBloodCastle::CheckSync(int iBridgeIndex)
{
	if ( BC_BRIDGE_RANGE(iBridgeIndex) == false )
	{
		return;
	}

	tm * today;
	time_t ltime;

	time(&ltime);
	today = localtime(&ltime);

	if ( BC_TIME_RANGE(g_BloodCastle.StartHour-1) == FALSE )
	{
		g_BloodCastle.StartHour = 1;
	}

	switch ( g_BloodCastle.StartHour )
	{
		case 2:
			
			if ( (today->tm_hour%2) == 0 )
			{
				if ( ( today->tm_min*60 + today->tm_sec ) >= 1800 )
				{
					this->m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC = (9000 - (today->tm_min*60 + today->tm_sec)) * 1000;
				}
				else
				{
					this->m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC = (1800 - (today->tm_min*60 + today->tm_sec)) * 1000;
				}
			}
			else
			{
				this->m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC = (5400 - (today->tm_min*60 + today->tm_sec)) * 1000;
			}

			if ( this->m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC < 0 )
			{
				this->m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC += 3600000;
			}

			this->m_BridgeData[iBridgeIndex].m_iBC_TICK_COUNT = GetTickCount();
			break;

		default:

			if ( ( today->tm_min*60 + today->tm_sec ) >= 1800 )
			{
				this->m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC = (5400 - (today->tm_min*60 + today->tm_sec)) * 1000;
			}
			else
			{
				this->m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC = (1800 - (today->tm_min*60 + today->tm_sec)) * 1000;
			}

			if ( this->m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC < 0 )
			{
				this->m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC += 3600000;
			}
			this->m_BridgeData[iBridgeIndex].m_iBC_TICK_COUNT = GetTickCount();

			break;

	}

	CLog.LogAdd("[Blood Castle]: (%d) -- Min Remain: [%d]", iBridgeIndex+1, this->m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC / 60000);


}

void CBloodCastle::ClearBridgeData(int iBridgeIndex)
{
	if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
	{
		return;
	}

	this->m_BridgeData[iBridgeIndex].m_iTOTAL_EXP = 0;
	this->m_BridgeData[iBridgeIndex].m_iBC_MONSTER_MAX_COUNT = 0;
	this->m_BridgeData[iBridgeIndex].m_iBC_MONSTER_KILL_COUNT = 0;
	this->m_BridgeData[iBridgeIndex].m_iBC_MONSTER_SUCCESS_MSG_COUNT = 0;
	this->m_BridgeData[iBridgeIndex].m_iBC_BOSS_MONSTER_MAX_COUNT = 0;
	this->m_BridgeData[iBridgeIndex].m_iBC_BOSS_MONSTER_KILL_COUNT = 0;
	this->m_BridgeData[iBridgeIndex].m_iBC_BOSS_MONSTER_SUCCESS_MSG_COUNT = 0;
	this->m_BridgeData[iBridgeIndex].m_btBC_QUEST_ITEM_NUMBER = 0;
	this->m_BridgeData[iBridgeIndex].m_dwBC_TICK_DOOR_OPEN = -1;
	this->m_BridgeData[iBridgeIndex].m_nBC_QUESTITEM_SERIAL = -1;
	this->m_BridgeData[iBridgeIndex].m_iBC_QUEST_ITEM_USER_INDEX = -1;
	this->m_BridgeData[iBridgeIndex].m_iMISSION_SUCCESS = -1;
	this->m_BridgeData[iBridgeIndex].m_iBC_NOTIFY_COUNT = -1;
	this->m_BridgeData[iBridgeIndex].m_bCASTLE_DOOR_LIVE = TRUE;
	this->m_BridgeData[iBridgeIndex].m_bBC_REWARDED = false;
	this->m_BridgeData[iBridgeIndex].m_bBC_MSG_BEFORE_ENTER = false;
	this->m_BridgeData[iBridgeIndex].m_bBC_MSG_BEFORE_PLAY = false;
	this->m_BridgeData[iBridgeIndex].m_bBC_MSG_BEFORE_END = false;
	this->m_BridgeData[iBridgeIndex].m_bBC_MSG_BEFORE_QUIT = false;
	this->m_BridgeData[iBridgeIndex].m_bBC_CAN_ENTER = false;
	this->m_BridgeData[iBridgeIndex].m_bBC_CAN_PARTY = false;
	this->m_BridgeData[iBridgeIndex].m_bBC_PLAY_START = false;
	this->m_BridgeData[iBridgeIndex].m_bBC_MONSTER_KILL_COMPLETE = false;
	this->m_BridgeData[iBridgeIndex].m_bBC_BOSS_MONSTER_KILL_COMPLETE = false;
	this->m_BridgeData[iBridgeIndex].m_bBC_DOOR_TERMINATE_COMPLETE = false;
	this->m_BridgeData[iBridgeIndex].m_iExtraEXP_Kill_Door_Index = -10;
	this->m_BridgeData[iBridgeIndex].m_iExtraEXP_Kill_Door_Party = -10;
	this->m_BridgeData[iBridgeIndex].m_iExtraEXP_Kill_Statue_Index = -10;
	this->m_BridgeData[iBridgeIndex].m_iExtraEXP_Kill_Statue_Party = -10;
	this->m_BridgeData[iBridgeIndex].m_iExtraEXP_Win_Quest_Index = -10;
	this->m_BridgeData[iBridgeIndex].m_iExtraEXP_Win_Quest_Party = -10;

	memset(this->m_BridgeData[iBridgeIndex].m_szKill_Door_CharName , 0, sizeof(this->m_BridgeData[iBridgeIndex].m_szKill_Door_CharName));
	memset(this->m_BridgeData[iBridgeIndex].m_szKill_Status_CharName, 0, sizeof(this->m_BridgeData[iBridgeIndex].m_szKill_Status_CharName));
	memset(this->m_BridgeData[iBridgeIndex].m_szWin_Quest_CharName, 0, sizeof(this->m_BridgeData[iBridgeIndex].m_szWin_Quest_CharName));
	memset(this->m_BridgeData[iBridgeIndex].m_szKill_Door_AccountID, 0, sizeof(this->m_BridgeData[iBridgeIndex].m_szKill_Door_AccountID));
	memset(this->m_BridgeData[iBridgeIndex].m_szKill_Status_AccountID, 0, sizeof(this->m_BridgeData[iBridgeIndex].m_szKill_Status_AccountID));
	memset(this->m_BridgeData[iBridgeIndex].m_szWin_Quest_AccountID, 0, sizeof(this->m_BridgeData[iBridgeIndex].m_szWin_Quest_AccountID));
	

	for ( int i=0;i<MAX_BLOOD_CASTLE_SUB_BRIDGE;i++)
	{
		if ( this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex != -1 )
		{
			gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].m_iBloodCastleEXP = 0;
			gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].m_cBloodCastleIndex = -1;
			gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].m_cBloodCastleSubIndex = -1;
			gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].m_bBloodCastleComplete = false;
		}

		this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iEXP = 0;
		this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iScore = 0;
		this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex = -1;
		this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iUserState= 0;
		this->m_BridgeData[iBridgeIndex].m_UserData[i].m_bSendQuitMsg = false;
		this->m_BridgeData[iBridgeIndex].m_UserData[i].m_bLiveWhenDoorBreak = false;
		this->m_BridgeData[iBridgeIndex].m_iBC_COMPLETE_USER_INDEX = -1;
	}
}

void CBloodCastle::SetState(int iBridgeIndex, int iBC_STATE)
{
	if ( BC_BRIDGE_RANGE(iBridgeIndex) == false )
	{
		return;
	}

	if ( iBC_STATE < BC_STATE_NONE || iBC_STATE > BC_STATE_PLAYEND )
	{
		return;
	}

	this->m_BridgeData[iBridgeIndex].m_iBC_STATE = iBC_STATE;

	switch ( this->m_BridgeData[iBridgeIndex].m_iBC_STATE )
	{
		case BC_STATE_NONE:
			this->SetState_None(iBridgeIndex);
			break;

		case BC_STATE_CLOSED:
			this->SetState_Closed(iBridgeIndex);
			break;

		case BC_STATE_PLAYING:
			this->SetState_Playing(iBridgeIndex);
			break;

		case BC_STATE_PLAYEND:
			this->SetState_PlayEnd(iBridgeIndex);
			break;
	}
}

void CBloodCastle::Run()
{
	if ( g_BloodCastle.Enabled == 1 )
	{
		for (int iBridgeIndex=0;iBridgeIndex<MAX_BLOOD_CASTLE_LEVEL;iBridgeIndex++)
		{
			switch ( this->m_BridgeData[iBridgeIndex].m_iBC_STATE )
			{
				case BC_STATE_NONE:
					this->ProcState_None(iBridgeIndex);
					break;

				case BC_STATE_CLOSED:
					this->ProcState_Closed(iBridgeIndex);
					break;

				case BC_STATE_PLAYING:
					this->ProcState_Playing(iBridgeIndex);
					break;

				case BC_STATE_PLAYEND:
					this->ProcState_PlayEnd(iBridgeIndex);
					break;
			}
		}
	}
}

void CBloodCastle::ProcState_None(int iBridgeIndex)
{
	return;
}

void CBloodCastle::ProcState_Closed(int iBridgeIndex)
{
	int iTICK_MSEC = GetTickCount() - this->m_BridgeData[iBridgeIndex].m_iBC_TICK_COUNT;

	if ( iTICK_MSEC >= 1000 )
	{
		this->m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC -= iTICK_MSEC;
		this->m_BridgeData[iBridgeIndex].m_iBC_TICK_COUNT = GetTickCount();

		if ( g_BloodCastle.Enabled != 0 )
		{
			if ( this->m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC <= ( g_BloodCastle.TimeMinOpen * 60 * 1000 ) && this->m_BridgeData[iBridgeIndex].m_bBC_CAN_ENTER == false)
			{
				this->m_BridgeData[iBridgeIndex].m_bBC_CAN_ENTER = true;
				this->m_BridgeData[iBridgeIndex].m_bBC_CAN_PARTY = true;
			}

			if ( this->m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC <= ( g_BloodCastle.TimeMinOpen * 60 * 1000 ) && this->m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC > 0 && (this->m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC/60000) !=this->m_BridgeData[iBridgeIndex].m_iBC_NOTIFY_COUNT)
			{
				this->m_BridgeData[iBridgeIndex].m_iBC_NOTIFY_COUNT = this->m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC / 60000;

				if ( iBridgeIndex == 0 )
				{
					//PMSG_NOTICE pNotice;

					//TNotice::MakeNoticeMsgEx(&pNotice, 0, lMsg.Get( MSGGET(4, 136)), this->m_BridgeData[iBridgeIndex].m_iBC_NOTIFY_COUNT+1);
					//this->SendAllUserAnyMsg((BYTE *)&pNotice, pNotice.h.size);
				}
			}

			if ( this->m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC <= 30000 && this->m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC > 0 && this->m_BridgeData[iBridgeIndex].m_bBC_MSG_BEFORE_ENTER == false )
			{
				this->m_BridgeData[iBridgeIndex].m_bBC_MSG_BEFORE_ENTER = true;

				if ( iBridgeIndex == 0 )
				{
					PMSG_SET_DEVILSQUARE pMsg;

					C1HeadSet((LPBYTE)&pMsg, 0x92, sizeof(pMsg));
					pMsg.Type = 3;

					for (int i= OBJ_STARTUSERINDEX;i<OBJMAX;i++)
					{
						if ( gObj[i].Connected == PLAYER_PLAYING && gObj[i].Type == OBJ_USER)
						{
							if ( gUser.BloodCastleMapRange(gObj[i].MapNumber) == false )
							{
								if ( gUser.ChaosCastleMapRange(gObj[i].MapNumber) == false )
								{
									gSendProto.DataSend(i, (UCHAR*)&pMsg, pMsg.h.size);
								}
							}
						}
					}
				}
			}
		}
	}

	if ( this->m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC <= 0 )
	{
		if ( g_BloodCastle.Enabled != FALSE )
		{
			this->SetState(iBridgeIndex, BC_STATE_PLAYING);
		}
		else
		{
			this->SetState(iBridgeIndex, BC_STATE_CLOSED);
		}
	}
}

void CBloodCastle::ProcState_Playing(int iBridgeIndex)
{
	int iTICK_MSEC = GetTickCount() - this->m_BridgeData[iBridgeIndex].m_iBC_TICK_COUNT;

	if ( iTICK_MSEC >= 1000 )
	{
		this->m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC -= iTICK_MSEC;
		this->m_BridgeData[iBridgeIndex].m_iBC_TICK_COUNT = GetTickCount();

		if ( this->m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC <= ((g_BloodCastle.TimeMinPlay*60-30)*1000) )
		{
			if ( this->m_BridgeData[iBridgeIndex].m_bBC_MSG_BEFORE_PLAY == false )
			{
				this->m_BridgeData[iBridgeIndex].m_bBC_MSG_BEFORE_PLAY = true;
				
				PMSG_SET_DEVILSQUARE pMsg;
				C1HeadSet((LPBYTE)&pMsg, 0x92, sizeof(pMsg));
				pMsg.Type = 4;
				this->SendBridgeAnyMsg((BYTE*)&pMsg, sizeof(pMsg), iBridgeIndex);
			}
		}

		// ---- Check can open bridge
		if ( this->m_BridgeData[iBridgeIndex].m_bBC_MONSTER_KILL_COMPLETE == true )
		{
			if ( this->m_BridgeData[iBridgeIndex].m_dwBC_TICK_DOOR_OPEN != -1 )
			{
				if ( GetTickCount() > this->m_BridgeData[iBridgeIndex].m_dwBC_TICK_DOOR_OPEN )
				{
					this->ReleaseCastleBridge(iBridgeIndex);
					this->SendCastleBridgeBlockInfo(iBridgeIndex, 0);
					this->m_BridgeData[iBridgeIndex].m_dwBC_TICK_DOOR_OPEN = -1; // Prevent multiple openings
				}
			}
		}

		// Set Play Quest
		if ( this->m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC <= ((g_BloodCastle.TimeMinPlay*60-60)*1000) && this->m_BridgeData[iBridgeIndex].m_bBC_PLAY_START == false )
		{
			//PMSG_NOTICE pNotice;

			this->m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC = (g_BloodCastle.TimeMinPlay*60)*1000;
			this->m_BridgeData[iBridgeIndex].m_bBC_CAN_PARTY = false;
			//TNotice::MakeNoticeMsgEx((TNotice*)&pNotice, 0, lMsg.Get(MSGGET(4, 137)), iBridgeIndex+1);
			//this->SendBridgeAnyMsg( (LPBYTE)&pNotice, pNotice.h.size, iBridgeIndex);
			this->ReleaseCastleEntrance(iBridgeIndex);
			this->SendCastleEntranceBlockInfo(iBridgeIndex, 0);
			this->m_BridgeData[iBridgeIndex].m_bBC_PLAY_START = true;
			this->SetMonster(iBridgeIndex);
			this->SendNoticeState(iBridgeIndex, 0);

			CLog.LogAdd("[Blood Castle] (%d) Blood Castle Quest Start", iBridgeIndex+1);
		}

		if ( this->m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC <= 30000 && this->m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC > 0 && this->m_BridgeData[iBridgeIndex].m_bBC_MSG_BEFORE_END == false)	// Set counter to kick
		{
			this->m_BridgeData[iBridgeIndex].m_bBC_MSG_BEFORE_END = true;
			PMSG_SET_DEVILSQUARE pMsg;
			C1HeadSet((LPBYTE)&pMsg, 0x92, sizeof(pMsg));
			pMsg.Type = 5;
			this->SendBridgeAnyMsg((BYTE *)&pMsg, sizeof(pMsg), iBridgeIndex);
		}

		if ( this->CheckEveryUserDie(iBridgeIndex) == true )
		{
			//PMSG_NOTICE pNotice;

			//TNotice::MakeNoticeMsg(&pNotice, 0, lMsg.Get(MSGGET(4, 138)));
			//this->SendBridgeAnyMsg( (LPBYTE)&pNotice, pNotice.h.size, iBridgeIndex);
			this->GiveReward_Fail(iBridgeIndex);
			this->SetState(iBridgeIndex, BC_STATE_CLOSED);
		}
		else
		{
			if ( this->m_BridgeData[iBridgeIndex].m_bBC_PLAY_START == true )
			{
				if ( this->m_BridgeData[iBridgeIndex].m_bBC_DOOR_TERMINATE_COMPLETE == false || this->m_BridgeData[iBridgeIndex].m_bBC_BOSS_MONSTER_KILL_COMPLETE != false )
				{
					this->SendNoticeState(iBridgeIndex, 1);
				}
				else
				{
					this->SendNoticeState(iBridgeIndex, 4);
				}
			}
		}

		if ( this->CheckWinnerExist(iBridgeIndex) == true )
		{
			if ( this->CheckWinnerValid(iBridgeIndex) == true )
			{
				//if ( this->CheckWinnerPartyComplete(iBridgeIndex) == true )
				//{
					this->GiveReward_Win(this->m_BridgeData[iBridgeIndex].m_iBC_COMPLETE_USER_INDEX, iBridgeIndex);
					this->SetState(iBridgeIndex, BC_STATE_PLAYEND);
					return;
				//}
			}
			else
			{
				this->GiveReward_Fail(iBridgeIndex);
				this->SetState(iBridgeIndex, BC_STATE_PLAYEND); 
				return;
			}
		}
	}

	if ( this->m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC <= 0 )
	{
		if ( this->m_BridgeData[iBridgeIndex].m_bBC_REWARDED == false )
		{
			CLog.LogAdd("[Blood Castle] (%d) Blood Castle Quest Failed -> Time Out", iBridgeIndex+1);
		}

		this->SetState(iBridgeIndex, BC_STATE_PLAYEND);
	}
}

void CBloodCastle::ProcState_PlayEnd(int iBridgeIndex)
{
	int iTICK_MSEC = GetTickCount() - this->m_BridgeData[iBridgeIndex].m_iBC_TICK_COUNT;

	if ( iTICK_MSEC >= 1000 )
	{
		this->m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC -= iTICK_MSEC;
		this->m_BridgeData[iBridgeIndex].m_iBC_TICK_COUNT = GetTickCount();

		if ( this->m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC <= 30000 && this->m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC > 0 && this->m_BridgeData[iBridgeIndex]. m_bBC_MSG_BEFORE_QUIT == false)
		{
			this->m_BridgeData[iBridgeIndex]. m_bBC_MSG_BEFORE_QUIT = true;

			PMSG_SET_DEVILSQUARE pMsg;

			C1HeadSet((LPBYTE)&pMsg, 0x92, sizeof(pMsg));
			pMsg.Type = 6;

			this->SendBridgeAnyMsg((BYTE *)&pMsg, sizeof(pMsg), iBridgeIndex);
		}


	}

	if ( this->m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC <= 0 )
	{
		this->SetState(iBridgeIndex, BC_STATE_CLOSED);
	}
}

void CBloodCastle::SetState_None(int iBridgeIndex)
{
	if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
	{
		return;
	}

	this->m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC = -1;
	this->m_BridgeData[iBridgeIndex].m_iBC_TICK_COUNT = -1;
	this->SendNoticeState(iBridgeIndex, 2);
	this->ClearBridgeData(iBridgeIndex);
	this->ClearMonster(iBridgeIndex, 1);

	for (int n=OBJ_STARTUSERINDEX;n<OBJMAX;n++)
	{
		if ( gObj[n].MapNumber == iBridgeIndex + 11 && gObj[n].Connected == PLAYER_PLAYING )
		{
			gObjMoveGate(n, 22);
		}
	}

	this->BlockCastleDoor(iBridgeIndex);
}

void CBloodCastle::SetState_Closed(int iBridgeIndex)
{
	if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
	{
		return;
	}

	this->SendNoticeState(iBridgeIndex, 2);
	this->ClearBridgeData(iBridgeIndex);
	this->ClearMonster(iBridgeIndex, 1);
	this->CheckAngelKingExist(iBridgeIndex);

	for (int n=OBJ_STARTUSERINDEX;n<OBJMAX;n++)
	{
		if ( gObj[n].MapNumber == g_BloodCastle.GetBloodCastleMap(iBridgeIndex) && gObj[n].Connected > PLAYER_LOGGED )
		{
			this->SearchUserDeleteQuestItem(n);
			gObjMoveGate(n, 22);
		}
	}

	this->BlockCastleDoor(iBridgeIndex);
	this->BlockCastleBridge(iBridgeIndex);
	this->BlockCastleEntrance(iBridgeIndex);
	this->CheckSync(iBridgeIndex);
	CLog.LogAdd("[Blood Castle]: (%d) SetState CLOSED", iBridgeIndex+1);
}

void CBloodCastle::SetState_Playing(int iBridgeIndex)
{
	if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
	{
		return;
	}

	this->m_BridgeData[iBridgeIndex].m_bBC_CAN_ENTER = false;
	this->m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC = g_BloodCastle.TimeMinPlay*60*1000;
	this->CheckUsersOnConnect(iBridgeIndex);

	//PMSG_NOTICE pNotice;

	//TNotice::MakeNoticeMsgEx(&pNotice, 1, lMsg.Get(MSGGET(4, 139)), iBridgeIndex+1, 60);
	//this->SendBridgeAnyMsg( (LPBYTE)&pNotice, pNotice.h.size, iBridgeIndex);

	PMSG_SERVERCMD ServerCmd;

	C1SubHeadSet((LPBYTE)&ServerCmd, 0xF3, 0x40, sizeof(ServerCmd));
	ServerCmd.CmdType = 1;
	ServerCmd.X = 45;
	ServerCmd.Y = 0;

	this->SendBridgeAnyMsg((BYTE *)&ServerCmd, ServerCmd.h.size, iBridgeIndex);

	for (int n=OBJ_STARTUSERINDEX;n<OBJMAX;n++)
	{
		if ( gObj[n].MapNumber == g_BloodCastle.GetBloodCastleMap(iBridgeIndex) && gObj[n].Connected > PLAYER_LOGGED )
		{
			this->SearchUserDeleteQuestItem(n);
		}
	}

	CLog.LogAdd("[Blood Castle]: (%d) SetState PLAYING", iBridgeIndex+1);
}

void CBloodCastle::SetState_PlayEnd(int iBridgeIndex)
{
	if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
	{
		return;
	}

	this->SendNoticeState(iBridgeIndex, 2);
	this->ClearMonster(iBridgeIndex, 0);
	this->m_BridgeData[iBridgeIndex].m_bBC_CAN_ENTER = false;
	this->m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC = g_BloodCastle.TimeMinRest*60*1000;

	CLog.LogAdd("[Blood Castle]: (%d) SetState PLAYEND", iBridgeIndex+1);

	for (int n=0;n<MAX_BLOOD_CASTLE_SUB_BRIDGE;n++)
	{
		if ( OBJMAX_RANGE(this->m_BridgeData[iBridgeIndex].m_UserData[n].m_iIndex) != FALSE )
		{
			this->SearchUserDeleteQuestItem(this->m_BridgeData[iBridgeIndex].m_UserData[n].m_iIndex);
		}
	}
}

int  CBloodCastle::GetCurrentState(int iBridgeIndex)
{
	if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
	{
		return -1;
	}
	return this->m_BridgeData[iBridgeIndex].m_iBC_STATE;
}

int  CBloodCastle::GetCurrentRemainSec(int iBridgeIndex)
{
	return this->m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC / 1000;
}

int  CBloodCastle::CheckEnterLevel(int iIndex, int iLevel)	// RET : [2:Error][1:OverLevel][0:InLevel - Success][-1:UnderLevel]
{
	if ( OBJMAX_RANGE(iIndex) == false )
	{
		return 2;
	}

	if ( gObj[iIndex].Type != OBJ_USER || gObj[iIndex].Connected <= PLAYER_LOGGED )
	{
		return 2;
	}

	if ( gUser.gObjIsMasteringLevel(&gObj[iIndex]) == true )
	{
		if ( g_sttBLOODCASTLE_LEVEL[iLevel-1].iLOWER_BOUND_MAGUMSA == 400 &&
			 g_sttBLOODCASTLE_LEVEL[iLevel-1].iUPPER_BOUND_MAGUMSA == 400 &&
			 g_sttBLOODCASTLE_LEVEL[iLevel-1].iLOWER_BOUND == 400 &&
			 g_sttBLOODCASTLE_LEVEL[iLevel-1].iUPPER_BOUND == 400)
		{
			 return 0;
		}
		else
		{
			return 1;
		}
	}
	else
	{
		if ( gObj[iIndex].Class == CLASS_DARKLORD || gObj[iIndex].Class == CLASS_MAGUMSA || gObj[iIndex].Class == CLASS_FIGHTER )
		{
			if ( gObj[iIndex].Level >= g_sttBLOODCASTLE_LEVEL[iLevel-1].iLOWER_BOUND_MAGUMSA && gObj[iIndex].Level <= g_sttBLOODCASTLE_LEVEL[iLevel-1].iUPPER_BOUND_MAGUMSA )
			{
				return 0;
			}
			if ( gObj[iIndex].Level < g_sttBLOODCASTLE_LEVEL[iLevel-1].iLOWER_BOUND_MAGUMSA )
			{
				return -1;
			}
			if ( gObj[iIndex].Level > g_sttBLOODCASTLE_LEVEL[iLevel-1].iUPPER_BOUND_MAGUMSA )
			{
				return 1;
			}
		}
		else
		{
			if ( gObj[iIndex].Level >= g_sttBLOODCASTLE_LEVEL[iLevel-1].iLOWER_BOUND && gObj[iIndex].Level <= g_sttBLOODCASTLE_LEVEL[iLevel-1].iUPPER_BOUND )
			{
				return 0;
			}
			if ( gObj[iIndex].Level < g_sttBLOODCASTLE_LEVEL[iLevel-1].iLOWER_BOUND )
			{
				return -1;
			}
			if ( gObj[iIndex].Level > g_sttBLOODCASTLE_LEVEL[iLevel-1].iUPPER_BOUND )
			{
				return 1;
			}
		}
	}

	return 2;
}

bool CBloodCastle::CheckEnterFreeTicket(int iIndex)
{
	if ( !OBJMAX_RANGE(iIndex) )
	{
		return false;
	}

	if ( gObj[iIndex].Type != OBJ_USER || gObj[iIndex].Connected <= PLAYER_LOGGED )
	{
		return false;
	}

	for (int x=0;x<MAIN_INVENTORY_SIZE;x++)
	{
		if ( gObj[iIndex].pInventory[x].IsItem() == TRUE )
		{
			if ( gObj[iIndex].pInventory [x].m_Type == ITEMGET(13,47) )
			{
				return true;
			}
		}
	}

	return false;
}

bool CBloodCastle::BloodCastleChaosMix(int iIndex, int iLEVEL)
{
	if ( OBJMAX_RANGE(iIndex) == FALSE )
	{
		return false;
	}

	if ( gUser.BloodCastleMapRange(g_BloodCastle.GetBloodCastleMap(iLEVEL-1))  == false )
	{
		return false;
	}

	BYTE Socket[5];
	Socket[0] = 0xFF;
	Socket[1] = 0xFF;
	Socket[2] = 0xFF;
	Socket[3] = 0xFF;
	Socket[4] = 0xFF;

	BOOL bMIX_RESULT = FALSE;

	PMSG_CHAOSMIXRESULT pMsg;

	C1HeadSet((LPBYTE)&pMsg.h, 0x86, sizeof(PMSG_CHAOSMIXRESULT));
	pMsg.Result = CB_ERROR;
	gObj[iIndex].ChaosLock = TRUE;

	CLog.LogAdd("[Blood Castle]: Mix Chaos Mix Start (Account:%s, Name:%s, Level:%d)", gObj[iIndex].AccountID, gObj[iIndex].Name, iLEVEL);
	LogChaosItem(&gObj[iIndex], "Blood Castle");

	int iMIX_SUCCESS_RATE = g_BloodCastle.BloodCastleMixSuccess[iLEVEL - 1];

	if ( iMIX_SUCCESS_RATE < 0 || iMIX_SUCCESS_RATE > 100 )
	{
		gSendProto.DataSend(iIndex, (UCHAR*)&pMsg, pMsg.h.size);
		CLog.LogAdd("[Blood Castle]: Chaos Mix Failed - Mix Rate out of bound (Account:%s, Name:%s, BCLevel:%d)", gObj[iIndex].AccountID, gObj[iIndex].Name, iLEVEL);
		return false;
	}

	if ( g_CrywolfSync.GetOccupationState() == 0 && g_iCrywolfApplyMvpBenefit )
	{
		iMIX_SUCCESS_RATE += g_CrywolfSync.GetPlusChaosRate();
	}

	if ( iMIX_SUCCESS_RATE > 80 )
	{
		iMIX_SUCCESS_RATE = 80;
	}

	if ( gObj[iIndex].ChaosSuccessRate > 10 )
	{
		pMsg.Result = 0xF0;
		gObj[iIndex].ChaosLock = FALSE;

		gSendProto.DataSend(iIndex, (LPBYTE)&pMsg, pMsg.h.size);
	}

	iMIX_SUCCESS_RATE += gObj[iIndex].ChaosSuccessRate;
	int iMIX_NEED_MONEY = g_BloodCastle.BloodCastleMixMoney[iLEVEL - 1];
	int iChaosTaxMoney = (int)((__int64)(iMIX_NEED_MONEY) * (__int64)(g_CastleSiegeSync.GetTaxRateChaos(iIndex)) / (__int64)100);

	if ( iChaosTaxMoney < 0 )
	{
		iChaosTaxMoney = 0;
	}

	iMIX_NEED_MONEY += iChaosTaxMoney;

	if ( iMIX_NEED_MONEY <  0 )
	{
		gSendProto.DataSend(iIndex, (UCHAR*)&pMsg, pMsg.h.size);
		CLog.LogAdd("[Blood Castle]: Chaos Mix Failed - Mix Money < 0 (Account:%s, Name:%s, BCLevel:%d)", gObj[iIndex].AccountID, gObj[iIndex].Name, iLEVEL);
		return false;
	}

	if ( (gObj[iIndex].Money - iMIX_NEED_MONEY) < 0 )
	{
		pMsg.Result = CB_BC_NOT_ENOUGH_ZEN;
		gSendProto.DataSend(iIndex, (UCHAR*)&pMsg, pMsg.h.size);
		CLog.LogAdd("[Blood Castle]: Chaos Mix Failed - Not Enough Money (Account:%s, Name:%s, BCLevel:%d)", gObj[iIndex].AccountID, gObj[iIndex].Name, iLEVEL);
		return false;
	}

	gObj[iIndex].Money -= iMIX_NEED_MONEY;
	g_CastleSiegeSync.AddTributeMoney(iChaosTaxMoney);
	GCMoneySend(iIndex, gObj[iIndex].Money);

	if ( (rand()%100) < iMIX_SUCCESS_RATE )
	{
		int item_num = ITEMGET(13,18);
		ItemSerialCreateSend(iIndex, -1, 0, 0, item_num, iLEVEL, 255, 0, 0, 0, -1, 0, 0, Socket);
		CLog.LogAdd("[Blood Castle]: [%s][%s] CBMix Success %d Money : %d-%d", gObj[iIndex].AccountID, gObj[iIndex].Name, iMIX_SUCCESS_RATE, gObj[iIndex].Money, iMIX_NEED_MONEY);
	}
	else
	{
		gChaosBox.ChaosBoxInit(&gObj[iIndex]);
		GCUserChaosBoxSend(&gObj[iIndex], 0);
		gSendProto.DataSend(iIndex, (UCHAR*)&pMsg, pMsg.h.size);
		CLog.LogAdd("[Blood Castle]: [%s][%s] CBMix Fail %d Money : %d-%d", gObj[iIndex].AccountID, gObj[iIndex].Name, iMIX_SUCCESS_RATE, gObj[iIndex].Money, iMIX_NEED_MONEY);

		return false;
	}

	::gObjInventoryCommit(iIndex);

	return true;
}

static const struct ST_BC_EVENT_SCORE
{
	int unk0;
	int unk4;
	int unk8;
	int unkC;
	int unk10;

} g_iBC_EventScore[MAX_BLOOD_CASTLE_LEVEL] = 
{
	600, 300, 1000, 800, 400,
	600, 300, 1000, 800, 400,
	600, 300, 1005, 800, 400,
	600, 300, 1005, 800, 400,         
	600, 300, 1005, 800, 400,
	600, 300, 1005, 800, 400,
	600, 300, 1005, 800, 400,
	600, 300, 1005, 800, 400
};

static const int g_iBC_EventScore_Fail[MAX_BLOOD_CASTLE_LEVEL] = { -300,-300,-300,-300,-300,-300,-300,-300 };	

static const struct  ST_BC_ADD_EXP
{
	int unk0;
	int unk4;
	int unk8;
	int unkC;

} g_iBC_Add_Exp[MAX_BLOOD_CASTLE_LEVEL] =
{
    20000, 20000, 5000, 160,
	50000, 50000, 10000, 180,
	80000, 80000, 15000, 200,
	90000, 90000, 20000, 220,
	100000, 100000, 25000, 240,
	110000, 110000, 30000, 260,
	120000, 120000, 35000, 280,
	130000, 130000, 40000, 300
};

static const int g_iBC_Party_EventPoint[MAX_USER_IN_PARTY]	= {5, 10, 15, 20, 30 };
static const int g_iBC_MONSTER_CHANGE_STATE[2][3] =
{
	//	DamageMin	DamageMax	Defense
	10,	20,	-14,
	20, 40, -32
};

int  CBloodCastle::CheckChoasMixItem(int iIndex)
{
	if ( OBJMAX_RANGE(iIndex) == FALSE )
	{
		return false;
	}

	int iCHAOS_MIX_LEVEL = 0;
	bool bIsChaosGemExist = false;
	bool bIsAngelKingPaperExist = false;
	bool bIsBloodBoneExist = false;
	bool bIsOtherItemExist = false;
	int iEventItemCount = 0;
	int iAngelKingPaperLevel = 0;
	int iBloodBoneLevel = 0;
	int iCharmOfLuckCount=0;

	for ( int i=0;i<CHAOS_BOX_SIZE;i++)
	{
		if ( gObj[iIndex].pChaosBox[i].IsItem() == TRUE )
		{
			if ( gObj[iIndex].pChaosBox[i].m_Type == ITEMGET(12,15) ) // Chaos
			{
				bIsChaosGemExist = true;
			}
			else if ( gObj[iIndex].pChaosBox[i].m_Type == ITEMGET(13,16) ) //Scroll of Archangel
			{
				int iSCROLL_LEVEL = gObj[iIndex].pChaosBox[i].m_Level;
				iEventItemCount++;
				bIsAngelKingPaperExist = true;
				iAngelKingPaperLevel = iSCROLL_LEVEL;
			}
			else if ( gObj[iIndex].pChaosBox[i].m_Type == ITEMGET(13,17) ) //Blood Bone
			{
				int iBLOOD_BONE_LEVEL = gObj[iIndex].pChaosBox[i].m_Level;
				iEventItemCount++;
				bIsBloodBoneExist = true;
				iBloodBoneLevel = iBLOOD_BONE_LEVEL;
			}
			else if ( gObj[iIndex].pChaosBox[i].m_Type == ITEMGET(14,53) ) //Charm
			{
				iCharmOfLuckCount += (int)gObj[iIndex].pChaosBox[i].m_Durability;
			}
			else
			{
				bIsOtherItemExist = true;
			}
		}
	}

	gObj[iIndex].ChaosSuccessRate = iCharmOfLuckCount;

	if ( bIsOtherItemExist != false )
	{
		return 8;
	}

	if ( bIsAngelKingPaperExist == false && bIsBloodBoneExist == false )
	{
		return 0;
	}

	if ( bIsAngelKingPaperExist == false || bIsBloodBoneExist == false )
	{
		return 11;
	}

	if ( iEventItemCount > 2 )
	{
		return 12;
	}

	if ( iCharmOfLuckCount > 10 )
	{
		return 15;
	}

	if ( iAngelKingPaperLevel != iBloodBoneLevel )
	{
		return 9;
	}

	if ( BC_BRIDGE_RANGE(iAngelKingPaperLevel-1) == FALSE )
	{
		return 9;
	}

	if ( BC_BRIDGE_RANGE(iBloodBoneLevel-1) == FALSE )
	{
		return 9;
	}

	if ( bIsChaosGemExist == false )
	{
		return 10;
	}

	if ( gObj[iIndex].Class == CLASS_DARKLORD || gObj[iIndex].Class == CLASS_MAGUMSA || gObj[iIndex].Class == CLASS_FIGHTER )
	{
		if ( gObj[iIndex].Level < g_sttBLOODCASTLE_LEVEL[0].iLOWER_BOUND_MAGUMSA )
		{
			return 14;
		}
	}
	else
	{
		if ( gObj[iIndex].Level < g_sttBLOODCASTLE_LEVEL[0].iLOWER_BOUND )
		{
			return 14;
		}
	}

	if ( bIsChaosGemExist != false && bIsAngelKingPaperExist != false && bIsBloodBoneExist != false )
	{
		return iAngelKingPaperLevel;
	}

	return 0;
}

int  CBloodCastle::CheckEnterItem(int iIndex)
{
	int iITEM_LEVEL = 0;

	if ( OBJMAX_RANGE(iIndex) == FALSE )
	{
		return 0;
	}

	if ( gObj[iIndex].Type != OBJ_USER || gObj[iIndex].Connected <= PLAYER_LOGGED )
	{
		return 0;
	}

	for ( int x=0;x<MAIN_INVENTORY_SIZE;x++)
	{
		if ( gObj[iIndex].pInventory[x].IsItem() == true )
		{
			if ( gObj[iIndex].pInventory[x].m_Type == ITEMGET(13,18) ) // Invisibility Cloak
			{
				iITEM_LEVEL = gObj[iIndex].pInventory[x].m_Level;

				if ( CHECK_LIMIT(iITEM_LEVEL, MAX_CLOACK_LEVEL) == false )
				{
					iITEM_LEVEL = 0;

				}
				if ( iITEM_LEVEL != 0 )
				{
					return iITEM_LEVEL;
				}
			}
			
			if ( gObj[iIndex].pInventory[x].m_Type == ITEMGET(13,47) ) // 
			{
				iITEM_LEVEL = 10;
			}
		}
	}

	return iITEM_LEVEL;
}

int  CBloodCastle::CheckQuestItem(int iIndex)
{
	int iITEM_LEVEL = -1;
	int BridgeIndex = -1;

	if ( OBJMAX_RANGE(iIndex) == false )
	{
		return -1;
	}

	if ( gUser.BloodCastleMapRange(gObj[iIndex].MapNumber) == false )
	{
		return -1;
	}

	BridgeIndex = g_BloodCastle.GetBloodCastleIndex(gObj[iIndex].MapNumber);

	if ( this->m_BridgeData[BridgeIndex].m_nBC_QUESTITEM_SERIAL == -1 )
	{
		return -1;
	}

	if ( gObj[iIndex].Type != OBJ_USER || gObj[iIndex].Connected <= PLAYER_LOGGED )
	{
		return -1;
	}

	for ( int x=0;x<MAIN_INVENTORY_SIZE;x++)
	{
		if ( gObj[iIndex].pInventory[x].IsItem() == TRUE )
		{
			if ( gObj[iIndex].pInventory[x].m_Type == ITEMGET(13,19) ) // Absolute Weapon of Archangel QUEST ITEM
			{
				//if ( gObj[iIndex].pInventory[x].m_Number	== this->m_BridgeData[BridgeIndex].m_nBC_QUESTITEM_SERIAL )
				//{
					iITEM_LEVEL = gObj[iIndex].pInventory[x].m_Level;

					if ( iITEM_LEVEL < 0 || iITEM_LEVEL > 2 )
					{
						iITEM_LEVEL = -1;
						
					}

					break;
				//}
			}
		}
	}

	return iITEM_LEVEL;
}

bool CBloodCastle::CheckWalk(int iIndex, int iMoveX, int iMoveY)
{
	int BridgeIndex = -1;

	if ( OBJMAX_RANGE(iIndex) == false )
	{
		return false;
	}
	
	if ( gUser.BloodCastleMapRange(gObj[iIndex].MapNumber) == false )
	{
		return false;
	}

	if ( gObj[iIndex].Type != OBJ_USER || gObj[iIndex].Connected <= PLAYER_LOGGED )
	{
		return false;
	}

	BridgeIndex = g_BloodCastle.GetBloodCastleIndex(gObj[iIndex].MapNumber);
	
	if ( this->GetCurrentState(BridgeIndex) == TRUE )
	{
		BYTE btMapAttr = MapC[gObj[iIndex].MapNumber].GetAttr(iMoveX, iMoveY);

		if ( (btMapAttr&1) != 1 )
		{
			return true;
		}
	}

	return false;
}

bool CBloodCastle::CheckCanEnter(int iBridgeIndex)
{
	if ( BC_BRIDGE_RANGE(iBridgeIndex) == false )
	{
		return false;
	}

	return this->m_BridgeData[iBridgeIndex].m_bBC_CAN_ENTER;
}

bool CBloodCastle::CheckCanParty(int iBridgeIndex)
{
	if ( BC_BRIDGE_RANGE(iBridgeIndex) == false )
	{
		return false;
	}

	return this->m_BridgeData[iBridgeIndex].m_bBC_CAN_PARTY;
}

bool CBloodCastle::CheckQuestItemSerial(int iBridgeIndex, CMapItem * lpItem)
{
	if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
	{
		return false;
	}

	/*if ( this->m_BridgeData[iBridgeIndex].m_nBC_QUESTITEM_SERIAL == -1 )
	{
		return false;
	}*/
	
	if ( lpItem->IsItem() == TRUE )
	{
		if ( lpItem->m_Type == ITEMGET(13,19) ) // Absolute Weapon
		{
			int iLEVEL = lpItem->m_Level;

			if ( BC_WEAPON_LEVEL_RANGE(iLEVEL) != FALSE )
			{
				//if ( this->m_BridgeData[iBridgeIndex].m_nBC_QUESTITEM_SERIAL == lpItem->m_Number )
				//{
					return true;
				//}
			}
		}
	}
	return false;
}

bool CBloodCastle::CheckPlayStart(int iBridgeIndex)
{
	if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
	{
		return false;
	}

	return this->m_BridgeData[iBridgeIndex].m_bBC_PLAY_START;
}

int  CBloodCastle::GetRemainTime(int iBridgeIndex)
{
	int iREMAIN_MINUTE = 0;

	if ( this->GetCurrentState(iBridgeIndex) == 1 )
	{
		iREMAIN_MINUTE = this->m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC / 60000 - g_BloodCastle.TimeMinOpen + 1;
	}
	else
	{
		tm * today;
		time_t ltime;
		time(&ltime);
		today = localtime(&ltime);

		int iSTART_HOUR = g_BloodCastle.StartHour;

		if ( iSTART_HOUR != 2 )
		{
			//__asm JMP label2
		}
		else
		{
			if ( (today->tm_hour % 2) == 0 )
			{
				if ( (today->tm_min * 60 + today->tm_sec) >= 1800 )
				{
					iREMAIN_MINUTE = (9000 - (today->tm_min * 60 + today->tm_sec)) / 60;
				}
				else
				{
					iREMAIN_MINUTE = (1800 - (today->tm_min * 60 + today->tm_sec)) / 60;
				}
			}
			else
			{
				iREMAIN_MINUTE = (1800 - (today->tm_min * 60 + today->tm_sec)) / 60;
			}

			if ( iREMAIN_MINUTE < 0 )
			{
				iREMAIN_MINUTE += 60;
				
			}

			__asm JMP skip;
		}
		
		if ( today->tm_min * 60 + today->tm_sec >= 1800 )
		{
			iREMAIN_MINUTE = (5400 - (today->tm_min * 60 + today->tm_sec)) / 60;
		}
		else
		{
			iREMAIN_MINUTE = (1800 - (today->tm_min * 60 + today->tm_sec)) / 60;
		}

		if ( iREMAIN_MINUTE < 0 )
		{
			iREMAIN_MINUTE += 60;
		}


	}

	skip:
	return iREMAIN_MINUTE;
}

void CBloodCastle::ClearMonster(int iBridgeIndex, bool bClearCastleDoor)
{
	if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
	{
		return;
	}

	for ( int n=0;n<OBJ_MAXMONSTER;n++)
	{
		if ( gObj[n].MapNumber == g_BloodCastle.GetBloodCastleMap(iBridgeIndex) )
		{
			if ( bClearCastleDoor == false && gObj[n].Class == 131)
			{
				continue;
			}

			if ( gObj[n].Class == 232 )
			{
				continue;
			}

			gObjDel(n);
		}
	}
}

void CBloodCastle::SetMonster(int iBridgeIndex)
{
	if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
	{
		return;
	}

	this->SetMonsterKillCount(iBridgeIndex);
	int nCastleDoorPosNum = this->m_BridgeData[iBridgeIndex].m_nCastleDoorPosNum;
	unsigned short btMonsterType = gMSetBase.m_Mp[nCastleDoorPosNum].m_Type;
	BYTE btMapNumber = gMSetBase.m_Mp[nCastleDoorPosNum].m_MapNumber;
	BYTE btBloodCastleIndex = g_BloodCastle.GetBloodCastleIndex(btMapNumber);

	if ( nCastleDoorPosNum != -1 )
	{
		if ( btMonsterType == 131 )
		{
			int iIndex = ::gObjAddMonster(btMapNumber);

			if ( iIndex >= 0 )
			{
				::gObjSetPosMonster(iIndex, nCastleDoorPosNum);
				::gObjSetMonster(iIndex, btMonsterType);

				gObj[iIndex].m_cBloodCastleIndex = btBloodCastleIndex;
				gObj[iIndex].m_ItemRate = this->m_iBC_NORMAL_ITEM_DROP;
				gObj[iIndex].Dir = 1;
				gObj[iIndex].m_PosNum = -1;
				gObj[iIndex].Live = TRUE;
				gObj[iIndex].DieRegen = FALSE;
				gObj[iIndex].m_State = 1;
				gObj[iIndex].MaxRegenTime = 0;
				gObj[iIndex].MaxLife = this->m_BridgeData[btBloodCastleIndex].m_iCastleStatueHealth;
				gObj[iIndex].Life = this->m_BridgeData[btBloodCastleIndex].m_iCastleStatueHealth;
			}
		}
	}

	int result;
	for ( int n=0;n<gMSetBase.m_Count;n++)
	{
		if ( gUser.BloodCastleMapRange(gMSetBase.m_Mp[n].m_MapNumber) == true  )
		{
			unsigned short btIndex = gMSetBase.m_Mp[n].m_Type;
			BYTE btMap   = gMSetBase.m_Mp[n].m_MapNumber;
			BYTE btBridgeIndex = g_BloodCastle.GetBloodCastleIndex(btMap);
			
			if ( btBridgeIndex != iBridgeIndex )
			{
				continue;
			}

			if ( btIndex == 232 )
			{
				continue;
			}

			if ( btIndex == 131 )
			{
				continue;
			}

			if ( btIndex == 89 || btIndex == 95 || btIndex == 112  || btIndex == 118 || btIndex == 124 || btIndex == 130 || btIndex == 143 || btIndex == 433 )
			{
				continue;
			}

			if ( BC_STATUE_RANGE(btIndex-132) != FALSE )
			{
				continue;
			}

			result = gObjAddMonster(gMSetBase.m_Mp[n].m_MapNumber);

			if ( result >= 0 )
			{
				gObj[result].m_PosNum = n;
				gObj[result].X = gMSetBase.m_Mp[n].m_X;
				gObj[result].Y = gMSetBase.m_Mp[n].m_Y;
				gObj[result].MapNumber = gMSetBase.m_Mp[n].m_MapNumber;
				gObj[result].TX = gObj[result].X;
				gObj[result].TY = gObj[result].Y;
				gObj[result].m_OldX = gObj[result].X;
				gObj[result].m_OldY = gObj[result].Y;
				gObj[result].Dir = gMSetBase.m_Mp[n].m_Dir;
				gObj[result].StartX = (BYTE)gObj[result].X;
				gObj[result].StartY = (BYTE)gObj[result].Y;
				gObjSetMonster(result, btIndex);
				gObj[result].MaxRegenTime = this->m_iBC_MONSTER_REGEN;
				gObj[result].m_cBloodCastleIndex = btBridgeIndex;
				gObj[result].Dir = rand() % 8;
			}
		}
	}
	
}

void CBloodCastle::SetBossMonster(int iBridgeIndex)
{
	int result;

	if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
	{
		return;
	}

	for ( int x=0;x<MAX_BLOOD_CASTLE_BOSS_MONSTER;x++)
	{
		if ( this->m_BridgeData[iBridgeIndex].m_nBossMonsterPosNum[x] != -1 )
		{
			int nBossMonsterPosNum = this->m_BridgeData[iBridgeIndex].m_nBossMonsterPosNum[x];
			unsigned short btMonsterType = gMSetBase.m_Mp[nBossMonsterPosNum].m_Type;
			BYTE btMapNumber = gMSetBase.m_Mp[nBossMonsterPosNum].m_MapNumber;
			BYTE btBloodCastleIndex = g_BloodCastle.GetBloodCastleIndex(btMapNumber);

			if ( btBloodCastleIndex != iBridgeIndex )
			{
				continue;
			}

			if ( btMonsterType == 89 || btMonsterType == 95 || btMonsterType == 112 || btMonsterType == 118 || btMonsterType == 124 || btMonsterType == 130 || btMonsterType == 143 || btMonsterType == 433 )
			{
				result = ::gObjAddMonster(btMapNumber);

				if ( result >= 0 )
				{
					::gObjSetPosMonster(result, nBossMonsterPosNum);
					::gObjSetMonster(result, btMonsterType);
					gObj[result].MaxRegenTime = this->m_iBC_MONSTER_REGEN;
					gObj[result].m_cBloodCastleIndex = btBloodCastleIndex;
					gObj[result].m_ItemRate = this->m_iBC_NORMAL_ITEM_DROP;
					gObj[result].Dir = rand() % 8;
				}
			}
		}
	}
}

void CBloodCastle::SetSaintStatue(int iBridgeIndex)
{
	if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
	{
		return;
	}

	int nSaintStatuePosNum = this->m_BridgeData[iBridgeIndex].m_nSaintStatuePosNum;
	unsigned short btMonsterType = gMSetBase.m_Mp[nSaintStatuePosNum].m_Type;
	BYTE btMapNumber = gMSetBase.m_Mp[nSaintStatuePosNum].m_MapNumber;
	BYTE btBloodCastleIndex = g_BloodCastle.GetBloodCastleIndex(btMapNumber);
	
	if ( nSaintStatuePosNum != -1 )
	{
		if ( BC_STATUE_RANGE(btMonsterType-132) != FALSE )
		{
			int result = gObjAddMonster(btMapNumber);

			if ( result >= 0 )
			{
				::gObjSetPosMonster(result, nSaintStatuePosNum);
				::gObjSetMonster(result, btMonsterType);
				gObj[result].Class = rand() % 3 + 132;
				gObj[result].m_cBloodCastleIndex = btBloodCastleIndex;
				gObj[result].m_ItemRate = this->m_iBC_NORMAL_ITEM_DROP;
				gObj[result].Dir = 1;
				gObj[result].m_PosNum = -1;
				gObj[result].Live = TRUE;
				gObj[result].DieRegen = 0;
				gObj[result].m_State = 1;
				gObj[result].MaxRegenTime = 0;
				gObj[result].MaxLife = this->m_BridgeData[btBloodCastleIndex].m_iCastleStatueHealth;
				gObj[result].Life = this->m_BridgeData[btBloodCastleIndex].m_iCastleStatueHealth;
			}
		}
		else
		{
			CLog.LogAdd("[Blood Castle][Bug Tracer] (%d) It's not Saint Statue( Map:%d, BloodCastleIndex:%d, Type:%d )",
				iBridgeIndex+1, btMapNumber, g_BloodCastle.GetBloodCastleIndex(gMSetBase.m_Mp[nSaintStatuePosNum].m_MapNumber), btMonsterType);
		}
	}

}

int  CBloodCastle::LeaveUserBridge(int iBridgeIndex, int iBridgeSubIndex, int iUserIndex)
{
	if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
	{
		return -1;
	}

	if ( BC_SUB_BRIDGE_RANGE(iBridgeSubIndex) == FALSE )
	{
		return -1;
	}

	int iRET_VAL = -1;

	::EnterCriticalSection(&this->m_BridgeData[iBridgeIndex].m_critUserData);

	if ( this->m_BridgeData[iBridgeIndex].m_UserData[iBridgeSubIndex].m_iIndex == iUserIndex )
	{
		iRET_VAL = iUserIndex;
		this->m_BridgeData[iBridgeIndex].m_UserData[iBridgeSubIndex].m_iIndex = -1;
		this->m_BridgeData[iBridgeIndex].m_UserData[iBridgeSubIndex].m_iEXP = 0;
		this->m_BridgeData[iBridgeIndex].m_UserData[iBridgeSubIndex].m_iScore = 0;
		this->m_BridgeData[iBridgeIndex].m_UserData[iBridgeSubIndex].m_iUserState = 0;
	}

	::LeaveCriticalSection(&this->m_BridgeData[iBridgeIndex].m_critUserData);

	return iRET_VAL;
}

int  CBloodCastle::EnterUserBridge(int iBridgeIndex, int iUserIndex)
{
	if ( BC_BRIDGE_RANGE(iBridgeIndex) == false )
	{
		return -1;
	}

	int iRET_VAL = -1;

	::EnterCriticalSection(&this->m_BridgeData[iBridgeIndex].m_critUserData);

	for ( int i=0;i<MAX_BLOOD_CASTLE_SUB_BRIDGE;i++)
	{
		if ( this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex != -1 )
		{
			if ( this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex == iUserIndex )
			{
				iRET_VAL = i;
				break;
			}
		}

		if ( this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex == -1 )
		{
			iRET_VAL = i;
			this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex = iUserIndex;
			this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iEXP = 0;
			this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iScore = 0;
			this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iUserState = 0;
			gObj[iUserIndex].m_bBloodCastleComplete = false;
			break;
		}
	}

	::LeaveCriticalSection(&this->m_BridgeData[iBridgeIndex].m_critUserData);

	return iRET_VAL;
}

int  CBloodCastle::LevelUp(int iIndex, int iAddExp, int iEventType)
{
	if ( OBJMAX_RANGE(iIndex) == FALSE )
	{
		return 0;
	}

	int iLEFT_EXP = 0;

	CLog.LogAdd("Experience : Map[%d]-(%d,%d) [%s][%s](%d) %u %d MonsterIndex : %d, EventType : %d",
		gObj[iIndex].MapNumber, gObj[iIndex].X, gObj[iIndex].Y,
		gObj[iIndex].AccountID,	gObj[iIndex].Name,
		gObj[iIndex].Level, gObj[iIndex].Experience,
		iAddExp, 0, iEventType);

	::gObjSetExpPetItem(iIndex, iAddExp);

	if ( gObj[iIndex].Level >= HRB_CharMaxLevel )
	{
		::GCServerMsgStringSend(lMsg.Get(MSGGET(4, 112)), gObj[iIndex].m_Index, 1);
		return 0;
	}

	if ( (gObj[iIndex].Experience + iAddExp) < gObj[iIndex].NextExp )
	{
		gObj[iIndex].Experience += iAddExp;
	}
	else
	{
		iLEFT_EXP = gObj[iIndex].Experience + iAddExp - gObj[iIndex].NextExp;
		gObj[iIndex].Experience = gObj[iIndex].NextExp;
		gObj[iIndex].Level++;

		if ( gObj[iIndex].Class == CLASS_DARKLORD || gObj[iIndex].Class == CLASS_MAGUMSA )
		{
			gObj[iIndex].LevelUpPoint += 7;
		}
		else
		{
			gObj[iIndex].LevelUpPoint += 5;
		}

		if ( gObj[iIndex].PlusStatQuestClear != false )
		{
			gObj[iIndex].LevelUpPoint++;

			CLog.LogAdd("[%s][%s] LevelUp PlusStatQuest Clear AddStat %d",
				gObj[iIndex].AccountID, gObj[iIndex].Name, gObj[iIndex].LevelUpPoint);
		}

		gObj[iIndex].MaxLife += gCharInfo.sCharInfo[gObj[iIndex].Class].LevelLife;
		gObj[iIndex].MaxMana += gCharInfo.sCharInfo[gObj[iIndex].Class].LevelMana;
		gObj[iIndex].Life = gObj[iIndex].MaxLife;
		gObj[iIndex].Mana = gObj[iIndex].MaxMana;
		gObjNextExpCal(&gObj[iIndex]);
		gObjSetBP(gObj[iIndex].m_Index);

		/*short AddPoint = 0;
		short MaxAddPoint = 0;
		short MinusPoint = 0;
		short MaxMinusPoint = 0;

		gObjGetStatPointState(gObj[iIndex].m_Index, AddPoint, MaxAddPoint, MinusPoint, MaxMinusPoint);*/


		GCLevelUpMsgSend(gObj[iIndex].m_Index, 1);//gObj[iIndex].Level, gObj[iIndex].LevelUpPoint, 
		//	(int)((float)gObj[iIndex].AddLife + gObj[iIndex].MaxLife), (int)((float)gObj[iIndex].AddMana + gObj[iIndex].MaxMana),
		//	gObj[iIndex].MaxBP + gObj[iIndex].AddBP, AddPoint, MaxAddPoint);
		gObjCalcMaxLifePower(gObj[iIndex].m_Index);
		CLog.LogAdd(lMsg.Get(MSGGET(2, 8)), gObj[iIndex].AccountID, gObj[iIndex].Name, gObj[iIndex].Level);
	}

	GJSetCharacterInfo(&gObj[iIndex], gObj[iIndex].m_Index, 0);
	
	return iLEFT_EXP;
}

void CBloodCastle::CheckUsersOnConnect(int iBridgeIndex)
{
	if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
	{
		return;
	}

	::EnterCriticalSection(&this->m_BridgeData[iBridgeIndex].m_critUserData);

	for ( int i=0;i<MAX_BLOOD_CASTLE_SUB_BRIDGE;i++)
	{
		if ( this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex != -1 )
		{
			if ( gObjIsConnected(this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex) == FALSE )
			{
				this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex = -1;
			}
			else
			{
				if ( gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].MapNumber != g_BloodCastle.GetBloodCastleMap(iBridgeIndex) )
				{
					this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex = -1;
				}
			}
		}
	}

	::LeaveCriticalSection(&this->m_BridgeData[iBridgeIndex].m_critUserData);

}

bool CBloodCastle::AddExperience(int iIndex, int iEXP)
{
	if ( OBJMAX_RANGE(iIndex) == false )
	{
		return false;
	}

	if ( BC_BRIDGE_RANGE(gObj[iIndex].m_cBloodCastleIndex) == FALSE )
	{
		return false;
	}

	if ( BC_SUB_BRIDGE_RANGE(gObj[iIndex].m_cBloodCastleSubIndex) == FALSE )
	{
		return false;
	}

	if ( iEXP > 0 )
	{
		this->m_BridgeData[gObj[iIndex].m_cBloodCastleIndex].m_UserData[gObj[iIndex].m_cBloodCastleSubIndex].m_iEXP += iEXP;
		gObj[iIndex].m_iBloodCastleEXP += iEXP;
	}

	return true;
}

void CBloodCastle::BlockCastleEntrance(int iBridgeIndex)
{
	if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
	{
		return;
	}

	for ( int i= ::g_btCastleEntranceMapXY[iBridgeIndex].btStartX;i<= ::g_btCastleEntranceMapXY[iBridgeIndex].btEndX;i++)
	{
		for ( int j= ::g_btCastleEntranceMapXY[iBridgeIndex].btStartY;j<= ::g_btCastleEntranceMapXY[iBridgeIndex].btEndY;j++)
		{

			MapC[g_BloodCastle.GetBloodCastleMap(iBridgeIndex)].m_attrbuf[j * 256 + i] |= 4;
		}
	}
}

void CBloodCastle::ReleaseCastleEntrance(int iBridgeIndex)
{
	if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
	{
		return;
	}

	for ( int i= ::g_btCastleEntranceMapXY[iBridgeIndex].btStartX;i<= ::g_btCastleEntranceMapXY[iBridgeIndex].btEndX;i++)
	{
		for ( int j= ::g_btCastleEntranceMapXY[iBridgeIndex].btStartY;j<= ::g_btCastleEntranceMapXY[iBridgeIndex].btEndY;j++)
		{
			MapC[g_BloodCastle.GetBloodCastleMap(iBridgeIndex)].m_attrbuf[j * 256 + i] &= ~4;
		}
	}
}

void CBloodCastle::BlockCastleBridge(int iBridgeIndex)
{
	if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
	{
		return;
	}

	for ( int i= ::g_btCastleBridgeMapXY[iBridgeIndex].btStartX;i<= ::g_btCastleBridgeMapXY[iBridgeIndex].btEndX;i++)
	{
		for ( int j= ::g_btCastleBridgeMapXY[iBridgeIndex].btStartY;j<= ::g_btCastleBridgeMapXY[iBridgeIndex].btEndY;j++)
		{
			MapC[g_BloodCastle.GetBloodCastleMap(iBridgeIndex)].m_attrbuf[j * 256 + i] |= 8;
		}
	}
}

void CBloodCastle::ReleaseCastleBridge(int iBridgeIndex)
{
	if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
	{
		return;
	}

	for ( int i= ::g_btCastleBridgeMapXY[iBridgeIndex].btStartX;i<= ::g_btCastleBridgeMapXY[iBridgeIndex].btEndX;i++)
	{
		for ( int j= ::g_btCastleBridgeMapXY[iBridgeIndex].btStartY;j<= ::g_btCastleBridgeMapXY[iBridgeIndex].btEndY;j++)
		{
			MapC[g_BloodCastle.GetBloodCastleMap(iBridgeIndex)].m_attrbuf[j * 256 + i] &= ~8;

			CLog.LogAdd("[Blood Castle][Bug Tracer] ReleaseCastleBridge-> %d", iBridgeIndex+1);
		}
	}
}

void CBloodCastle::BlockCastleDoor(int iBridgeIndex)
{
	if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
	{
		return;
	}

	for ( int x=0;x<3;x++)
	{
		for ( int y=::g_btCastleDoorMapXY[iBridgeIndex][x].btStartX; y <= ::g_btCastleDoorMapXY[iBridgeIndex][x].btEndX ;y++)
		{
			for ( int z = ::g_btCastleDoorMapXY[iBridgeIndex][x].btStartY; z <= ::g_btCastleDoorMapXY[iBridgeIndex][x].btEndY ; z++)
			{
				MapC[g_BloodCastle.GetBloodCastleMap(iBridgeIndex)].m_attrbuf[z * 256 + y] |= 4;
			}
		}
	}
}

void CBloodCastle::ReleaseCastleDoor(int iBridgeIndex)
{
	if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
	{
		return;
	}

	for ( int x=0;x<3;x++)
	{
		for ( int y=::g_btCastleDoorMapXY[iBridgeIndex][x].btStartX; y <= ::g_btCastleDoorMapXY[iBridgeIndex][x].btEndX ;y++)
		{
			for ( int z = ::g_btCastleDoorMapXY[iBridgeIndex][x].btStartY; z <= ::g_btCastleDoorMapXY[iBridgeIndex][x].btEndY ; z++)
			{
				MapC[g_BloodCastle.GetBloodCastleMap(iBridgeIndex)].m_attrbuf[z * 256 + y] &= ~4;
			}
		}
	}
}

void CBloodCastle::BlockSector(int iMAP_NUM, int iSTART_X, int iSTART_Y, int iEND_X, int iEND_Y)
{
	if ( gUser.BloodCastleMapRange(iMAP_NUM) == false )
	{
		return;
	}

	for ( int i=iSTART_X;i<=iEND_X;i++)
	{
		for ( int j=iSTART_Y;j<=iEND_Y;j++ )
		{
			MapC[iMAP_NUM].m_attrbuf[j * 256 + i] |= 4;
		}
	}
}

void CBloodCastle::ReleaseSector(int iMAP_NUM, int iSTART_X, int iSTART_Y, int iEND_X, int iEND_Y)
{

	if ( gUser.BloodCastleMapRange(iMAP_NUM) == false )
	{
		return;
	}

	for ( int i=iSTART_X;i<=iEND_X;i++)
	{
		for ( int j=iSTART_Y;j<=iEND_Y;j++ )
		{
			MapC[iMAP_NUM].m_attrbuf[j * 256 + i] &= ~4;
		}
	}

}

void CBloodCastle::SendCastleEntranceBlockInfo(int iBridgeIndex, bool bLive)
{
	if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
	{
		return;
	}

	char cTEMP_BUF[256];
	PMSG_SETMAPATTR_COUNT * lpMsg = (PMSG_SETMAPATTR_COUNT *)cTEMP_BUF;

	C1HeadSet((LPBYTE)lpMsg, 0x46, sizeof(PMSG_SETMAPATTR_COUNT)+sizeof(PMSG_SETMAPATTR)*6);
	PMSG_SETMAPATTR * lpMsgBody = (PMSG_SETMAPATTR *)&cTEMP_BUF[7];
	lpMsg->btType = 0;
	lpMsg->btCount = 1;
	lpMsg->btMapAttr = 4;
	(bLive)?(lpMsg->btMapSetType=0) :( lpMsg->btMapSetType=1);

	lpMsgBody[0].btX = ::g_btCastleEntranceMapXY[iBridgeIndex].btStartX;
	lpMsgBody[0].btY = ::g_btCastleEntranceMapXY[iBridgeIndex].btStartY;
	lpMsgBody[1].btX   = ::g_btCastleEntranceMapXY[iBridgeIndex].btEndX;
	lpMsgBody[1].btY   = ::g_btCastleEntranceMapXY[iBridgeIndex].btEndY;

	for (int i=OBJ_MAXMONSTER;i<OBJMAX;i++)
	{
		if ( gObj[i].MapNumber == g_BloodCastle.GetBloodCastleMap(iBridgeIndex) )
		{
			if ( gObj[i].Connected > PLAYER_LOGGED )
			{
				gSendProto.DataSend(i, (unsigned char *)lpMsg, lpMsg->h.size);
			}
		}
	}

}

void CBloodCastle::SendCastleBridgeBlockInfo(int iBridgeIndex, bool bLive)
{
	if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
	{
		return;
	}

	char cTEMP_BUF[256];
	PMSG_SETMAPATTR_COUNT * lpMsg = (PMSG_SETMAPATTR_COUNT *)cTEMP_BUF;

	C1HeadSet((LPBYTE)lpMsg, 0x46, sizeof(PMSG_SETMAPATTR_COUNT)+sizeof(PMSG_SETMAPATTR)*6);
	PMSG_SETMAPATTR * lpMsgBody = (PMSG_SETMAPATTR *)&cTEMP_BUF[7];
	lpMsg->btType = 0;
	lpMsg->btCount = 1;
	lpMsg->btMapAttr = 8;
	(bLive)?lpMsg->btMapSetType=0:lpMsg->btMapSetType=1;

	lpMsgBody[0].btX = ::g_btCastleEntranceMapXY[iBridgeIndex].btStartX;
	lpMsgBody[0].btY = ::g_btCastleEntranceMapXY[iBridgeIndex].btStartY;
	lpMsgBody[1].btX   = ::g_btCastleEntranceMapXY[iBridgeIndex].btEndX;
	lpMsgBody[1].btY   = ::g_btCastleEntranceMapXY[iBridgeIndex].btEndY;

	for ( int i=OBJ_MAXMONSTER;i<OBJMAX;i++)
	{
		if ( gObj[i].MapNumber == g_BloodCastle.GetBloodCastleMap(iBridgeIndex) )
		{
			if ( gObj[i].Connected > PLAYER_LOGGED )
			{
				gSendProto.DataSend(i, (UCHAR *)lpMsg, lpMsg->h.size);

				CLog.LogAdd("[Blood Castle][Bug Tracer] SendCastleBridgeBlockInfo-> Bridge:%d/Live:%d, (%d,%d,%d,%d)",
					iBridgeIndex+1, bLive, lpMsgBody[0].btX, lpMsgBody[0].btY, lpMsgBody[1].btX, lpMsgBody[1].btY);
			}
		}
	}
}

void CBloodCastle::SendCastleDoorBlockInfo(int iBridgeIndex, bool bLive)
{
	if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
	{
		return;
	}

	char cTEMP_BUF[256];
	PMSG_SETMAPATTR_COUNT * lpMsg = (PMSG_SETMAPATTR_COUNT *)cTEMP_BUF;

	C1HeadSet((LPBYTE)lpMsg, 0x46, sizeof(PMSG_SETMAPATTR_COUNT)+sizeof(PMSG_SETMAPATTR)*6);
	PMSG_SETMAPATTR * lpMsgBody = (PMSG_SETMAPATTR *)&cTEMP_BUF[7];
	lpMsg->btType = 0;
	lpMsg->btCount = 3;
	lpMsg->btMapAttr = 4;
	(bLive)?lpMsg->btMapSetType=0:lpMsg->btMapSetType=1;

	lpMsgBody[0].btX = ::g_btCastleDoorMapXY[iBridgeIndex][0].btStartX;
	lpMsgBody[0].btY = ::g_btCastleDoorMapXY[iBridgeIndex][0].btStartY;
	lpMsgBody[1].btX   = ::g_btCastleDoorMapXY[iBridgeIndex][0].btEndX;
	lpMsgBody[1].btY   = ::g_btCastleDoorMapXY[iBridgeIndex][0].btEndY;

	lpMsgBody[2].btX = ::g_btCastleDoorMapXY[iBridgeIndex][1].btStartX;
	lpMsgBody[2].btY = ::g_btCastleDoorMapXY[iBridgeIndex][1].btStartY;
	lpMsgBody[3].btX   = ::g_btCastleDoorMapXY[iBridgeIndex][1].btEndX;
	lpMsgBody[3].btY   = ::g_btCastleDoorMapXY[iBridgeIndex][1].btEndY;

	lpMsgBody[4].btX = ::g_btCastleDoorMapXY[iBridgeIndex][2].btStartX;
	lpMsgBody[4].btY = ::g_btCastleDoorMapXY[iBridgeIndex][2].btStartY;
	lpMsgBody[5].btX   = ::g_btCastleDoorMapXY[iBridgeIndex][2].btEndX;
	lpMsgBody[5].btY   = ::g_btCastleDoorMapXY[iBridgeIndex][2].btEndY;

	for ( int i=OBJ_MAXMONSTER;i<OBJMAX;i++)
	{
		if ( gObj[i].MapNumber == g_BloodCastle.GetBloodCastleMap(iBridgeIndex) )
		{
			if ( gObj[i].Connected > PLAYER_LOGGED )
			{
				gSendProto.DataSend(i, (UCHAR *)lpMsg, lpMsg->h.size);
			}
		}
	}
}

void CBloodCastle::SendNoticeMessage(int iBridgeIndex, char * lpszMSG)
{
	if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
	{
		return;
	}

	//PMSG_NOTICE pNotice;

	//TNotice::MakeNoticeMsg( &pNotice, 0, lpszMSG);

	for ( int i=0;i<MAX_BLOOD_CASTLE_SUB_BRIDGE;i++)
	{
		if ( this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex != -1 )
		{
			if ( gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].Connected > PLAYER_LOGGED )
			{
				if ( gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].m_cBloodCastleIndex != -1 )
				{
					if ( gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].m_cBloodCastleSubIndex != -1 )
					{
						//gSendProto.DataSend(this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, (UCHAR*)&pNotice, pNotice.h.size);
					}
				}
			}
		}
	}
}

void CBloodCastle::SendNoticeScore(int iBridgeIndex)
{
	if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
	{
		return;
	}

	//PMSG_NOTICE pNotice;
	//pNotice.type = 0;

	for ( int i=0;i<MAX_BLOOD_CASTLE_SUB_BRIDGE;i++)
	{
		if ( this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex != -1 )
		{
			if ( gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].Connected > PLAYER_LOGGED )
			{
				if ( gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].m_cBloodCastleIndex != -1 )
				{
					if ( gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].m_cBloodCastleSubIndex != -1 )
					{
						//TNotice::MakeNoticeMsgEx(&pNotice, 0, lMsg.Get(MSGGET(4, 140)), iBridgeIndex+1, this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iEXP);
						//gSendProto.DataSend(this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, (UCHAR*)&pNotice, pNotice.h.size);
					}
				}
			}
		}
	}
}

void CBloodCastle::SendNoticeState(int iBridgeIndex, int iPlayState)
{
	if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
	{
		return;
	}

	PMSG_STATEBLOODCASTLE pMsg;

	C1HeadSet((LPBYTE)&pMsg, 0x9B, sizeof(PMSG_STATEBLOODCASTLE));
	pMsg.btPlayState = iPlayState;
	pMsg.wRemainSec = this->m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC / 1000;

	if ( iPlayState == 4 )
	{
		pMsg.wMaxKillMonster = this->m_BridgeData[iBridgeIndex].m_iBC_BOSS_MONSTER_MAX_COUNT;
		pMsg.wCurKillMonster = this->m_BridgeData[iBridgeIndex].m_iBC_BOSS_MONSTER_KILL_COUNT;
	}
	else
	{
		pMsg.wMaxKillMonster = this->m_BridgeData[iBridgeIndex].m_iBC_MONSTER_MAX_COUNT;
		pMsg.wCurKillMonster = this->m_BridgeData[iBridgeIndex].m_iBC_MONSTER_KILL_COUNT;
	}

	pMsg.wUserHaveWeapon = this->m_BridgeData[iBridgeIndex].m_iBC_QUEST_ITEM_USER_INDEX;
	pMsg.btWeaponNum = this->m_BridgeData[iBridgeIndex].m_btBC_QUEST_ITEM_NUMBER + 1;

	for ( int i=0;i<MAX_BLOOD_CASTLE_SUB_BRIDGE;i++)
	{
		if ( this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex != -1 )
		{
			if ( gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].Connected > PLAYER_LOGGED )
			{
				if ( gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].m_cBloodCastleIndex != -1 )
				{
					if ( gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].m_cBloodCastleSubIndex != -1 )
					{
						gSendProto.DataSend(this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, (UCHAR *)&pMsg, pMsg.h.size);
					}
				}
			}
		}
	}
}

bool CBloodCastle::CheckUserBridgeMember(int iBridgeIndex, int iIndex)
{
	if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
	{
		return false;
	}

	if ( OBJMAX_RANGE(iIndex) == FALSE )
	{
		return false;
	}

	for ( int i=0;i<MAX_BLOOD_CASTLE_SUB_BRIDGE;i++)
	{
		if ( this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex == iIndex )
		{
			return true;
		}
	}

	return false;
}

int  CBloodCastle::GetAliveUserTotalEXP(int iBridgeIndex)
{
	if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
	{
		return 0;
	}

	int iRET_EXP = 0;

	for ( int i=0;i<MAX_BLOOD_CASTLE_SUB_BRIDGE;i++)
	{
		if ( this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex != -1 )
		{
			if ( gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].Connected > PLAYER_LOGGED )
			{
				if ( gUser.BloodCastleMapRange(gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].MapNumber) == true )
				{
					iRET_EXP += this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iEXP;
				}
			}
		}
	}

	return iRET_EXP;
}

void CBloodCastle::SearchUserDeleteQuestItem(int iIndex)
{
	if ( gObjIsConnected(iIndex) == FALSE )
	{
		return;
	}

	for ( int x=0;x<INVENTORY_SIZE;x++)
	{
		if ( gObj[iIndex].pInventory[x].IsItem() == TRUE )
		{
			if ( gObj[iIndex].pInventory[x].m_Type == ITEMGET(13,19) )
			{
				int iLEVEL = gObj[iIndex].pInventory[x].m_Level;

				if ( BC_WEAPON_LEVEL_RANGE(iLEVEL) == true )
				{
					::gObjInventoryItemSet(iIndex, x, -1);
					::gObjInventoryDeleteItem(iIndex, x);
					::GCInventoryItemDeleteSend(iIndex, x, TRUE);

					CLog.LogAdd("[Blood Castle] Force (Account:%s, Name:%s) to Delete Angel King's Weapon (%d) [Serial:%d]",
						gObj[iIndex].AccountID, gObj[iIndex].Name, iLEVEL, gObj[iIndex].pInventory[x].m_Number);
				}
			}
		}
	}
}

void CBloodCastle::SearchUserDropQuestItem(int iIndex)
{
	if ( OBJMAX_RANGE(iIndex) == FALSE )
	{
		return;
	}

	if ( gObj[iIndex].Type != OBJ_USER || gObj[iIndex].Connected <= PLAYER_LOGGED )
	{
		return;
	}

	for ( int x=0;x<INVENTORY_SIZE;x++)
	{
		if ( gObj[iIndex].pInventory[x].IsItem() == TRUE )
		{
			if ( gObj[iIndex].pInventory[x].m_Type == ITEMGET(13,19) )
			{
				int iLEVEL = gObj[iIndex].pInventory[x].m_Level;

				if ( BC_WEAPON_LEVEL_RANGE(iLEVEL) != FALSE )
				{
					BYTE pMsg[6];
					pMsg[5] = x;
					pMsg[3] = (BYTE)gObj[iIndex].X;
					pMsg[4] = (BYTE)gObj[iIndex].Y;

					this->DropItemDirectly(g_BloodCastle.GetBloodCastleIndex(gObj[iIndex].MapNumber), gObj[iIndex].m_Index, ITEMGET(13, 19), x);

					if ( gUser.BloodCastleMapRange(gObj[iIndex].MapNumber) == true )
					{
						int iBC_INDEX = g_BloodCastle.GetBloodCastleIndex(gObj[iIndex].MapNumber);

						if ( this->m_BridgeData[iBC_INDEX].m_nBC_QUESTITEM_SERIAL != -1 )
						{
							if ( this->m_BridgeData[iBC_INDEX].m_nBC_QUESTITEM_SERIAL == gObj[iIndex].pInventory[x].m_Number )
							{
								this->m_BridgeData[iBC_INDEX].m_iBC_QUEST_ITEM_USER_INDEX = -1;
							}
						}

						CLog.LogAdd("[Blood Castle] Force (Account:%s, Name:%s) to Drop Angel King's Weapon (%d) [Serial:%d-%d]",
							gObj[iIndex].AccountID, gObj[iIndex].Name, iLEVEL, this->m_BridgeData[iBC_INDEX].m_nBC_QUESTITEM_SERIAL,
							gObj[iIndex].pInventory[x].m_Number);

					}
					else
					{
						CLog.LogAdd("[Blood Castle] Force (Account:%s, Name:%s) to Drop Angel King's Weapon (%d) [Serial:%d]",
							gObj[iIndex].AccountID, gObj[iIndex].Name, iLEVEL,
							gObj[iIndex].pInventory[x].m_Number);
					}
				}
			}
		}
	}
}

void CBloodCastle::SetUserState(int iIndex, int iState)
{
	if ( OBJMAX_RANGE(iIndex) == FALSE )
	{
		return;
	}

	int iBridgeIndex = gObj[iIndex].m_cBloodCastleIndex;
	int iBridgeSubIndex = gObj[iIndex].m_cBloodCastleSubIndex;

	if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
	{
		return;
	}

	if ( BC_SUB_BRIDGE_RANGE(iBridgeSubIndex) == FALSE )
	{
		return;
	}

	switch ( iState )
	{
		case 0:
			this->m_BridgeData[iBridgeIndex].m_UserData[iBridgeSubIndex].m_iUserState = 0;
			break;

		case 1:
			this->m_BridgeData[iBridgeIndex].m_UserData[iBridgeSubIndex].m_iUserState = 1;
			break;

		case 2:
			if ( gObj[iIndex].PartyNumber >= 0 )
			{
				for ( int i=0;i<MAX_BLOOD_CASTLE_SUB_BRIDGE;i++)
				{
					if ( i == iBridgeSubIndex )
					{
						continue;
					}

					if ( gObj[iIndex].PartyNumber == gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].PartyNumber )
					{
						if ( gUser.BloodCastleMapRange( gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex ].MapNumber ) == true )
						{
							this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iUserState = 3;
						}
						else
						{
							this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iUserState = 4;
						}
					}
				}
			}

			this->m_BridgeData[iBridgeIndex].m_UserData[iBridgeSubIndex].m_iUserState = 2;
			this->m_BridgeData[iBridgeIndex].m_iMISSION_SUCCESS = iBridgeSubIndex;
			break;

		case 3:
			this->m_BridgeData[iBridgeIndex].m_UserData[iBridgeSubIndex].m_iUserState = 3;
			break;

		case 4:
			this->m_BridgeData[iBridgeIndex].m_UserData[iBridgeSubIndex].m_iUserState = 4;
			break;
	}
}

struct ST_BC_SCORE	// size == 0x18
{
	char CharName[MAX_ACCOUNT_LEN];	// unk0
	int  iSCORE;	// unkC
	int  iEXP;	// unk10
	int  iZEN;	// unk14
};

struct GCS_BC_GIVE_REWARD	//Send[C1:93] - #error WARNING (Deathway) - Check with Packet of DevilSquareGround.h Ranking 
{
	PHEADB PHeader;
	bool bWinner;
	BYTE btType;
	ST_BC_SCORE m_stBCCharScore[MAX_BLOOD_CASTLE_SUB_BRIDGE+1];
};

// ///////////////////////////////////
// START REVIEW HERE

void CBloodCastle::GiveReward_Win(int iIndex, int iBridgeIndex)
{
	this->FixUsersPlayStateWin(iBridgeIndex);
	char szNOTIFY_MSG[256];
	int iREWARD_EXP = 0;
	int iREWARD_ZEN = 0;
	int iREWARD_SCR = 0;
	int iLEFT_TIME = this->m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC / 1000;
	int iALIVE_PARTYCOUNT = this->GetWinnerPartyCompleteCount(iBridgeIndex);
	int iADD_PARTYPOINT = this->GetWinnerPartyCompletePoint(iBridgeIndex);

	CLog.LogAdd("[Blood Castle] (%d) Winner Party Point - Alive Party Member : %d, Alive Party Point : %d",
		iBridgeIndex+1, iALIVE_PARTYCOUNT, iADD_PARTYPOINT);

	if ( OBJMAX_RANGE(iIndex) == FALSE )
	{
		return;
	}

	if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
	{
		return;
	}
	
	if ( this->m_BridgeData[iBridgeIndex].m_bBC_REWARDED != false )
	{
		return;
	}

	if ( gObj[iIndex].Connected > PLAYER_LOGGED )
	{
		//gObj[iIndex].Name[MAX_ACCOUNT_LEN] = 0;
		wsprintf(szNOTIFY_MSG, lMsg.Get(MSGGET(4, 141)), gObj[iIndex].Name);
	}
	else
	{
		wsprintf(szNOTIFY_MSG, lMsg.Get(MSGGET(4, 142)));
	}

	if ( this->m_BridgeData[iBridgeIndex].m_iMISSION_SUCCESS != -1 )
	{
		int iTOTAL_EXP = this->GetAliveUserTotalEXP(iBridgeIndex);
		CLog.LogAdd("[Blood Castle] (%d) Quest Complete - All Alive User's Total EXP:%d", iBridgeIndex+1, iTOTAL_EXP);

		for ( int i=0;i<MAX_BLOOD_CASTLE_SUB_BRIDGE;i++)
		{
			if ( this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex == -1 )
			{
				continue;
			}

			if ( gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].Connected < PLAYER_PLAYING )
			{
				continue;
			}

			if ( gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].m_cBloodCastleIndex == -1 || gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].m_cBloodCastleSubIndex == -1 ||  gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].m_cBloodCastleIndex != iBridgeIndex )
			{
				continue;
			}

			//PMSG_NOTICE pNotice;
			//TNotice::MakeNoticeMsg(&pNotice, 10, szNOTIFY_MSG);
			//TNotice::SetNoticeProperty(&pNotice, 10, _ARGB(255, 128, 149, 196), 1, 0, 20);
			//TNotice::SendNoticeToUser(this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, (TNotice*)&pNotice);

			int iADD_EXP = 0;

			if ( (gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].PartyNumber != -1 && gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].PartyNumber == this->m_BridgeData[iBridgeIndex].m_iExtraEXP_Kill_Door_Party) || this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex == this->m_BridgeData[iBridgeIndex].m_iExtraEXP_Kill_Door_Index)
			{
				iADD_EXP += ::g_iBC_Add_Exp[iBridgeIndex].unk0;
			}
			else
			{
				if ( gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].PartyNumber != -1 )
				{
					if ( this->m_BridgeData[iBridgeIndex].m_UserData[i].m_bLiveWhenDoorBreak != false )
					{
						iADD_EXP += ::g_iBC_Add_Exp[iBridgeIndex].unk0 / 2;
					}
				}
			}

			if ( (gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].PartyNumber != -1 && gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].PartyNumber == this->m_BridgeData[iBridgeIndex].m_iExtraEXP_Kill_Statue_Party) || this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex == this->m_BridgeData[iBridgeIndex].m_iExtraEXP_Kill_Statue_Index)
			{
				iADD_EXP += ::g_iBC_Add_Exp[iBridgeIndex].unk4;
			}

			if ( (gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].PartyNumber != -1 && gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].PartyNumber == this->m_BridgeData[iBridgeIndex].m_iExtraEXP_Win_Quest_Party) || this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex == this->m_BridgeData[iBridgeIndex].m_iExtraEXP_Win_Quest_Index)
			{
				iADD_EXP += ::g_iBC_Add_Exp[iBridgeIndex].unk8;
			}

			switch ( this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iUserState  )
			{
				case 0:
					iADD_EXP += this->m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC / 1000 * ::g_iBC_Add_Exp[iBridgeIndex].unkC;
					iREWARD_EXP = this->CalcSendRewardEXP(this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, iADD_EXP);
					iREWARD_ZEN = this->CalcSendRewardZEN(this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, g_BloodCastle.BloodCastleZenReward[iBridgeIndex]);
					iREWARD_SCR = ::g_iBC_EventScore[iBridgeIndex].unk0;
					this->SendRewardScore(this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, iREWARD_SCR, iLEFT_TIME, 0);
					break;

				case 1:
					iADD_EXP += this->m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC / 1000 * ::g_iBC_Add_Exp[iBridgeIndex].unkC;
					iREWARD_EXP = this->CalcSendRewardEXP(this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, iADD_EXP);
					iREWARD_ZEN = this->CalcSendRewardZEN(this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, g_BloodCastle.BloodCastleZenReward[iBridgeIndex]);
					iREWARD_SCR = ::g_iBC_EventScore[iBridgeIndex].unk4;
					this->SendRewardScore(this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, iREWARD_SCR, iLEFT_TIME, 0);
					break;

				case 2:
					iADD_EXP += this->m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC / 1000 * ::g_iBC_Add_Exp[iBridgeIndex].unkC;
					iREWARD_EXP = this->CalcSendRewardEXP(this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, iADD_EXP);
					iREWARD_ZEN = this->CalcSendRewardZEN(this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, g_BloodCastle.BloodCastleZenReward[iBridgeIndex]);
					iREWARD_SCR = ::g_iBC_EventScore[iBridgeIndex].unk8;
					iREWARD_SCR += iADD_PARTYPOINT;

					if ( gUser.BloodCastleMapRange(gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].MapNumber ) == true )
					{
						g_BloodCastle.GiveReward(this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex,iBridgeIndex);
						//this->DropChaosGem(this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex);
					}


					this->SendRewardScore(this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, iREWARD_SCR, iLEFT_TIME, iALIVE_PARTYCOUNT);
					break;

				case 3:
					iADD_EXP += this->m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC / 1000 * ::g_iBC_Add_Exp[iBridgeIndex].unkC;
					iREWARD_EXP = this->CalcSendRewardEXP(this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, iADD_EXP);
					iREWARD_ZEN = this->CalcSendRewardZEN(this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, g_BloodCastle.BloodCastleZenReward[iBridgeIndex]);
					iREWARD_SCR = ::g_iBC_EventScore[iBridgeIndex].unkC;
					iREWARD_SCR += iADD_PARTYPOINT;

					if ( gUser.BloodCastleMapRange(gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].MapNumber ) == true )
					{
						this->DropChaosGem(this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex);
					}


					this->SendRewardScore(this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, iREWARD_SCR, iLEFT_TIME, iALIVE_PARTYCOUNT);
					break;

				case 4:
					iADD_EXP += this->m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC / 1000 * ::g_iBC_Add_Exp[iBridgeIndex].unkC;
					iREWARD_EXP = this->CalcSendRewardEXP(this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, iADD_EXP);
					iREWARD_ZEN = this->CalcSendRewardZEN(this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, g_BloodCastle.BloodCastleZenReward[iBridgeIndex]);
					iREWARD_SCR = ::g_iBC_EventScore[iBridgeIndex].unk10;
					this->SendRewardScore(this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, iREWARD_SCR, iLEFT_TIME, iALIVE_PARTYCOUNT);
					break;
			}

			GCS_BC_GIVE_REWARD pMsg;

			pMsg.bWinner = true;
			pMsg.btType = -1;
			memcpy(pMsg.m_stBCCharScore[0].CharName , gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].Name, MAX_ACCOUNT_LEN);
			pMsg.m_stBCCharScore[0].iEXP = iREWARD_EXP;
			pMsg.m_stBCCharScore[0].iZEN = iREWARD_ZEN;
			pMsg.m_stBCCharScore[0].iSCORE = iREWARD_SCR;
			C1HeadSet((LPBYTE)&pMsg.PHeader, 0x93, sizeof(GCS_BC_GIVE_REWARD) - (sizeof(ST_BC_SCORE) * (MAX_BLOOD_CASTLE_SUB_BRIDGE -1)) );

			gSendProto.DataSend(this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, (UCHAR*)&pMsg, pMsg.PHeader.uSize);
		}

		this->m_BridgeData[iBridgeIndex].m_bBC_REWARDED = true;
	}
}

void CBloodCastle::GiveReward_Fail(int iBridgeIndex)
{
	this->FixUsersPlayStateFail(iBridgeIndex);

	if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
	{
		return;
	}

	int iUserWhoGotUltimateWeapon = -1;
	iUserWhoGotUltimateWeapon = this->GetWhoGotUltimateWeapon(iBridgeIndex);

	if ( iUserWhoGotUltimateWeapon != -1 )
	{
		if ( OBJMAX_RANGE(iUserWhoGotUltimateWeapon) != FALSE )
		{
			CLog.LogAdd("[Blood Castle] (%d) (Account:%s, Name:%s) got Ultimate Weapon (%d) but Failed - Time Out",
				iBridgeIndex+1, gObj[iUserWhoGotUltimateWeapon].AccountID, gObj[iUserWhoGotUltimateWeapon].Name,
				this->m_BridgeData[iBridgeIndex].m_btBC_QUEST_ITEM_NUMBER);
		}
	}

	for ( int i=0;i<MAX_BLOOD_CASTLE_SUB_BRIDGE;i++)
	{
		if ( this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex == -1 )
		{
			continue;
		}

		if ( gObj[ this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex ].Connected < PLAYER_PLAYING )
		{
			continue;
		}

		if ( gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].m_cBloodCastleIndex == -1 || gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].m_cBloodCastleSubIndex == -1 )
		{
			continue;
		}	

		int iADD_EXP = 0;

		if ( (gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].PartyNumber != -1 && gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].PartyNumber == this->m_BridgeData[iBridgeIndex].m_iExtraEXP_Kill_Door_Party) || this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex == this->m_BridgeData[iBridgeIndex].m_iExtraEXP_Kill_Door_Index)
		{
			iADD_EXP += ::g_iBC_Add_Exp[iBridgeIndex].unk0;
		}
		else
		{
			if ( gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].PartyNumber != -1 )
			{
				if ( this->m_BridgeData[iBridgeIndex].m_UserData[i].m_bLiveWhenDoorBreak != false )
				{
					iADD_EXP += ::g_iBC_Add_Exp[iBridgeIndex].unk0 / 2;
				}
			}
		}

		if ( (gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].PartyNumber != -1 && gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].PartyNumber == this->m_BridgeData[iBridgeIndex].m_iExtraEXP_Kill_Statue_Party) || this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex == this->m_BridgeData[iBridgeIndex].m_iExtraEXP_Kill_Statue_Index)
		{
			iADD_EXP += ::g_iBC_Add_Exp[iBridgeIndex].unk4;
		}

		this->CalcSendRewardEXP(this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, iADD_EXP);
		this->SendRewardScore(this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, ::g_iBC_EventScore_Fail[iBridgeIndex], 0, 0);

		CLog.LogAdd("[Blood Castle] (%d) Quest Fail Rewarded (Account:%s, Name:%s, Score:%d, ExtEXP:%d, LeftTime:%d)",
			iBridgeIndex+1, gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].AccountID,
			gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].Name,
			::g_iBC_EventScore_Fail[iBridgeIndex], iADD_EXP, 0);

		GCS_BC_GIVE_REWARD pMsg;

		pMsg.bWinner = false;
		pMsg.btType = -1;
		memcpy(pMsg.m_stBCCharScore[0].CharName, gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].Name, MAX_ACCOUNT_LEN);
		pMsg.m_stBCCharScore[0].iEXP = iADD_EXP;
		pMsg.m_stBCCharScore[0].iZEN = 0;
		pMsg.m_stBCCharScore[0].iSCORE = g_iBC_EventScore_Fail[iBridgeIndex];
		C1HeadSet((LPBYTE)&pMsg.PHeader, 0x93, sizeof(GCS_BC_GIVE_REWARD) - (sizeof(ST_BC_SCORE) * (MAX_BLOOD_CASTLE_SUB_BRIDGE -1)) );

		if ( gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].m_cBloodCastleIndex != -1 && gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].m_cBloodCastleSubIndex != -1 )
		{
			gSendProto.DataSend(this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, (UCHAR*)&pMsg, pMsg.PHeader.uSize);			

			CLog.LogAdd("[Blood Castle] (%d) Send User Quest Fail Message (Account:%s, Name:%s [State:%d], MapNumber:%d)",
				iBridgeIndex+1, gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].AccountID,
				gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].Name, 
				this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iUserState,
				gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].MapNumber);
		}	
	}
}

int  CBloodCastle::CalcSendRewardEXP(int iIndex, int iEXP)
{
	if ( iEXP <= 0 )
	{
		return 0;
	}

	int iRET_EXP = 0;
	int iCAL_EXP = iEXP;

	if ( g_CrywolfSync.GetOccupationState() == 1 && g_iCrywolfApplyMvpPenalty )
		iEXP = iEXP * g_CrywolfSync.GetGettingExpPenaltyRate() / 100;

	if ( gObj[iIndex].m_wExprienceRate == 0 )
	{
		iEXP = 0;
		iCAL_EXP = 0;
	}

	if ( OBJMAX_RANGE(iIndex) == FALSE )
	{
		return 0;
	}

	if ( gObj[iIndex].Connected < PLAYER_PLAYING )
	{
		return 0;
	}

	iRET_EXP = iCAL_EXP;

	if ( gObj[iIndex].Type == OBJ_USER )
	{
		while ( iCAL_EXP > 0 )
		{
			if ( iCAL_EXP > 0 )
			{
				if ( gObj[iIndex].m_wExprienceRate > 0 )
				{
					iCAL_EXP = this->LevelUp(iIndex, iCAL_EXP, 4);
				}
				else
				{
					iEXP = 0;
					iCAL_EXP = 0;
					iRET_EXP = 0;
				}
			}
		}

		PMSG_KILLPLAYER_EXT pMsg;

		C3HeadSet((LPBYTE)&pMsg, 0x9C, sizeof(pMsg));
		pMsg.NumberH = -1;
		pMsg.NumberL = -1;
		pMsg.ExpH = SET_NUMBERHW(iEXP);
		pMsg.ExpL = SET_NUMBERLW(iEXP);
		pMsg.DamageH = 0;
		pMsg.DamageL = 0;

		gSendProto.DataSend(iIndex, (UCHAR*)&pMsg, pMsg.h.size);
	}

	return iRET_EXP;
}

int  CBloodCastle::CalcSendRewardZEN(int iIndex, int iZEN)
{
	if ( iZEN <= 0 )
	{
		return 0;
	}

	int iRET_ZEN = 0;

	if ( OBJMAX_RANGE(iIndex) == FALSE )
	{
		return 0;
	}

	if ( gObj[iIndex].Connected < PLAYER_PLAYING )
	{
		return 0;
	}

	if ( gObjCheckMaxZen(iIndex, iZEN) == FALSE )
	{
		iRET_ZEN = MAX_ZEN - gObj[iIndex].Money;
		gObj[iIndex].Money += iRET_ZEN;

		return iRET_ZEN;
	}

	gObj[iIndex].Money += iZEN;
	iRET_ZEN = iZEN;
	GCMoneySend(iIndex, gObj[iIndex].Money);


	return iRET_ZEN;
}

void CBloodCastle::DropChaosGem(int iIndex)
{
	if ( OBJMAX_RANGE(iIndex) == FALSE )
	{
		return;
	}
}

struct PMSG_ANS_BLOODCASTLESCORE_5TH
{
	PBMSG_HEAD h;	// C1:0D
	char AccountID[10];	// 3
	char GameID[10];	// D
	int ServerCode;	// 18
	int Score;	// 1C
	int Class;	// 20
	int BridgeNum;	// 24
	int iLeftTime;	// 28
	int iAlivePartyCount;	// 2C
};

void CBloodCastle::SendRewardScore(int iIndex, int iSCORE, int iLeftTime, int iAlivePartyCount)
{
	if ( OBJMAX_RANGE(iIndex) == FALSE )
	{
		return;
	}

	PMSG_ANS_BLOODCASTLESCORE_5TH pMsg;

	pMsg.h.c = 0xC1;
	pMsg.h.headcode = 0x0D;
	pMsg.h.size = sizeof(pMsg);
	pMsg.Score = iSCORE;
	pMsg.BridgeNum = gObj[iIndex].m_cBloodCastleIndex;
	pMsg.Class = gObj[iIndex].Class;
	pMsg.ServerCode = gGameServerCode;
	pMsg.iLeftTime = iLeftTime;
	memcpy(pMsg.AccountID, gObj[iIndex].AccountID, MAX_ACCOUNT_LEN);
	memcpy(pMsg.GameID, gObj[iIndex].Name, MAX_ACCOUNT_LEN);
	pMsg.iAlivePartyCount = iAlivePartyCount;

	if( ::Connected[3] == FALSE && ::Connected[2] == FALSE )
	{
		wsRServerCli.Close();
		wsRServerCli.CreateSocket(gMainApp.m_hWnd);

		if ( GMRankingServerConnect(gDevilSquareEventServerIp, WM_GM_RANKING_CLIENT_MSG_PROC) == FALSE )
		{
			::Connected[3] = FALSE;
			CLog.LogAdd("Can not connect Ranking Server");

			return;
		}

		::Connected[3] = TRUE;
	}

	if ( ::Connected[2] == FALSE && ::Connected[3] != FALSE )
	{
		wsRServerCli.DataSend((char *)&pMsg, pMsg.h.size);
	}
}

void CBloodCastle::SendBridgeAnyMsg(BYTE * lpMsg, int iSize, int iBridgeIndex)
{
	if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
	{
		return;
	}

	for ( int i=0;i<MAX_BLOOD_CASTLE_SUB_BRIDGE;i++)
	{
		if ( this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex != -1 )
		{
			if ( gObj[ this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex ].Connected == PLAYER_PLAYING )
			{
				if ( gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].m_cBloodCastleIndex != -1 && gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].m_cBloodCastleSubIndex != -1 )
				{
					gSendProto.DataSend(this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, lpMsg, iSize);
				}
			}
		}
	}

}

void CBloodCastle::SendAllUserAnyMsg(BYTE * lpMsg, int iSize)
{
	for ( int i=OBJ_STARTUSERINDEX;i<OBJMAX;i++)
	{
		if ( gObj[i].Connected == PLAYER_PLAYING )
		{
			if ( gObj[i].Type == OBJ_USER )
			{
				gSendProto.DataSend(i, lpMsg, iSize);
			}
		}
	}
}

void CBloodCastle::SetMonsterKillCount(int iBridgeIndex)
{
	if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
	{
		return;
	}

	int iTOT_USER_COUNT = 0;
	int iLIVE_USER_COUNT = 0;
	int iKILL_USER_COUNT = 0;

	for ( int i=0;i<MAX_BLOOD_CASTLE_SUB_BRIDGE;i++)
	{
		if ( this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex != -1 )
		{
			iTOT_USER_COUNT++;

			if ( gUser.BloodCastleMapRange(gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].MapNumber) == true )
			{
				if ( gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].Connected > PLAYER_LOGGED )
				{
					iLIVE_USER_COUNT++;
				}
				else
				{
					iKILL_USER_COUNT++;
				}
			}
			else
			{
				iKILL_USER_COUNT++;
			}

		}
	}

	this->m_BridgeData[iBridgeIndex].m_iBC_MONSTER_MAX_COUNT = iLIVE_USER_COUNT * 40;
	this->m_BridgeData[iBridgeIndex].m_iBC_MONSTER_KILL_COUNT = 0;

	CLog.LogAdd("[Blood Castle] (%d) Monster Kill Count Set - Monster:%d, Current Monster:%d, USER TOT:%d, LIVE:%d, DEAD:%d",
		iBridgeIndex+1, this->m_BridgeData[iBridgeIndex].m_iBC_MONSTER_MAX_COUNT,
		this->m_BridgeData[iBridgeIndex].m_iBC_MONSTER_KILL_COUNT, 
		iTOT_USER_COUNT, iLIVE_USER_COUNT, iKILL_USER_COUNT);
}

bool CBloodCastle::CheckMonsterKillCount(int iBridgeIndex)
{
	if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
	{
		return false;
	}

	if( this->m_BridgeData[iBridgeIndex].m_iBC_MONSTER_KILL_COUNT >= this->m_BridgeData[iBridgeIndex].m_iBC_MONSTER_MAX_COUNT )
	{
		CLog.LogAdd("[Blood Castle][Bug Tracer] (%d) All Monster Kill Success(Max Kill Count:%d / Kill Count:%d)",
			iBridgeIndex+1, this->m_BridgeData[iBridgeIndex].m_iBC_MONSTER_MAX_COUNT,
			this->m_BridgeData[iBridgeIndex].m_iBC_MONSTER_KILL_COUNT);

		return true;
	}

	return false;
}

bool CBloodCastle::CheckMonsterKillSuccess(int iBridgeIndex)
{
	if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
	{
		return false;
	}

	return this->m_BridgeData[iBridgeIndex].m_bBC_MONSTER_KILL_COMPLETE;
}

bool CBloodCastle::CheckBossKillCount(int iBridgeIndex)
{
	if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
	{
		return false;
	}

	if ( this->m_BridgeData[iBridgeIndex].m_iBC_BOSS_MONSTER_KILL_COUNT >= this->m_BridgeData[iBridgeIndex].m_iBC_BOSS_MONSTER_MAX_COUNT )
	{
		CLog.LogAdd("[Blood Castle][Bug Tracer] (%d) All Boss Monster Kill Success(Max Boss Kill Count:%d / Boss Kill Count:%d)",
			iBridgeIndex+1, this->m_BridgeData[iBridgeIndex].m_iBC_BOSS_MONSTER_MAX_COUNT,
			this->m_BridgeData[iBridgeIndex].m_iBC_BOSS_MONSTER_KILL_COUNT);

		return true;
	}

	return false;
}

bool CBloodCastle::CheckBossKillSuccess(int iBridgeIndex)
{
	if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
	{
		return false;
	}

	return this->m_BridgeData[iBridgeIndex].m_bBC_BOSS_MONSTER_KILL_COMPLETE;
}

bool CBloodCastle::CheckEveryUserDie(int iBridgeIndex)
{
	if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
	{
		return false;
	}

	bool bRET_VAL = true;

	for ( int i=0;i<MAX_BLOOD_CASTLE_SUB_BRIDGE;i++)
	{
		if ( this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex != -1 )
		{
			if ( OBJMAX_RANGE(this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex) == FALSE )
			{
				/*CLog.LogAdd("error-L3 : [Blood Castle] (%d) User Out of Bound UserIndex:%d, SubIndex:%d",
					iBridgeIndex+1, this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, i);*/
				
				continue;
			}

			if ( gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].MapNumber == g_BloodCastle.GetBloodCastleMap(iBridgeIndex) )
			{
				if ( gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].Connected > PLAYER_LOGGED )
				{
					bRET_VAL = false;
				}
				else
				{
					/*CLog.LogAdd("error-L3 : [Blood Castle] (%d) Connection Closed UserIndex:%d, SubIndex:%d",
						iBridgeIndex+1, this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, i);*/
				}
			}
			else
			{
				if ( this->m_BridgeData[iBridgeIndex].m_UserData[i].m_bSendQuitMsg == false )
				{
					this->m_BridgeData[iBridgeIndex].m_UserData[i].m_bSendQuitMsg = true;

					PMSG_STATEBLOODCASTLE pMsg;

					C1HeadSet((LPBYTE)&pMsg, 0x9B, sizeof(PMSG_STATEBLOODCASTLE));
					pMsg.btPlayState = 2;
					pMsg.wRemainSec = 0;
					pMsg.wMaxKillMonster = 0;
					pMsg.wCurKillMonster = 0;
					pMsg.wUserHaveWeapon = 0;
					pMsg.btWeaponNum = -1;

					gSendProto.DataSend(this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, (UCHAR*)&pMsg, pMsg.h.size);
				}
			}
		}
	}

	return bRET_VAL;
}

bool CBloodCastle::CheckAngelKingExist(int iBridgeIndex)
{
	if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
	{
		return false;
	}

	bool bRET_VAL = false;

	if ( this->m_BridgeData[iBridgeIndex].m_iAngelKingPosNum == -1 )
	{
		CLog.LogAdd("[Blood Castle]: (%d) Angel King's Monster Position Doesn't Exist", iBridgeIndex+1);
		return false;
	}

	for ( int i=0;i<OBJ_STARTUSERINDEX;i++)
	{
		if ( gObj[i].Connected == PLAYER_PLAYING && gObj[i].Type == OBJ_NPC )
		{
			if ( gObj[i].Class == 232 )
			{
				if (gObj[i].MapNumber == g_BloodCastle.GetBloodCastleMap(iBridgeIndex) )
				{
					bRET_VAL = true;
					break;
				}
			}
		}
	}

	if ( bRET_VAL == false )
	{
		int result = gObjAddMonster(g_BloodCastle.GetBloodCastleMap(iBridgeIndex));

		if ( result >= 0 )
		{
			gObjSetPosMonster(result, this->m_BridgeData[iBridgeIndex].m_iAngelKingPosNum);
			gObjSetMonster(result, 232);
			gObj[result].m_cBloodCastleIndex = g_BloodCastle.GetBloodCastleIndex(gObj[result].MapNumber);
			bRET_VAL = true;
		}
		else
		{
			CLog.LogAdd("[Blood Castle] (%d) Angel King - gObjAdd() failed (no space to add monster, result:%d)",
				iBridgeIndex+1, result);

			return false;
		}
	}

	return bRET_VAL;
}

int  CBloodCastle::GetWhoGotUltimateWeapon(int iBridgeIndex)
{
	if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
	{
		return -1;
	}

	int iBridgeUserIndex = -1;

	for ( int i=0;i<MAX_BLOOD_CASTLE_SUB_BRIDGE;i++)
	{
		int iIndex = this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex;

		if ( OBJMAX_RANGE(iIndex) == FALSE )
		{
			continue;
		}

		if( gObj[iIndex].Type != OBJ_USER || gObj[iIndex].Connected <= PLAYER_LOGGED )
		{
			continue;
		}

		if ( gUser.BloodCastleMapRange( gObj[iIndex].MapNumber ) == false )
		{
			continue;
		}

		for ( int x=0;x<INVENTORY_SIZE;x++)
		{
			if ( gObj[iIndex].pInventory[x].IsItem() == TRUE )
			{
				if ( gObj[iIndex].pInventory[x].m_Type == ITEMGET(13,19) )
				{
					int iLEVEL = gObj[iIndex].pInventory[x].m_Level;

					if ( BC_WEAPON_LEVEL_RANGE(iLEVEL) != FALSE )
					{
						if ( this->m_BridgeData[iBridgeIndex].m_nBC_QUESTITEM_SERIAL != -1 )
						{
							if ( this->m_BridgeData[iBridgeIndex].m_nBC_QUESTITEM_SERIAL == gObj[iIndex].pInventory[x].m_Number )
							{
								iBridgeUserIndex = iIndex;
								break;
							}
						}
					}
				}
			}
		}

	}

	return iBridgeUserIndex;
}

int  CBloodCastle::GetCurrentLiveUserCount(int iBridgeIndex)
{
	if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
	{
		return -1;
	}

	int iRetLiveUserCount = 0;

	for ( int i=0;i<MAX_BLOOD_CASTLE_SUB_BRIDGE;i++)
	{
		int iIndex = this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex;

		if ( OBJMAX_RANGE(iIndex) == FALSE )
		{
			continue;
		}

		if ( gUser.BloodCastleMapRange( gObj[iIndex].MapNumber ) == false )
		{
			continue;
		}

		if( gObj[iIndex].Type != OBJ_USER  )
		{
			continue;
		}

		if ( gObj[iIndex].Connected > PLAYER_LOGGED )
		{
			iRetLiveUserCount++;
		}
	}

	return iRetLiveUserCount;
}

BOOL CBloodCastle::DropItemDirectly(int iBridgeIndex, int iIndex, int iItemType, int iItemPos)
{
	if ( gUser.BloodCastleMapRange(g_BloodCastle.GetBloodCastleMap(iBridgeIndex)) == false )
	{
		CLog.LogAdd("[Blood Castle] [%s][%s] DropItemDirectly() failed (iBridgeIndex wrong - %d)",
			gObj[iIndex].AccountID, gObj[iIndex].Name, iBridgeIndex);

		return false;
	}

	PMSG_ITEMTHROW_RESULT pResult;

	C1HeadSet((LPBYTE)&pResult, 0x23, sizeof(pResult));
	pResult.Result = TRUE;
	pResult.Ipos = iItemPos;
	int map_num = gObj[iIndex].MapNumber;
	int type = gObj[iIndex].pInventory[iItemPos].m_Type;
	int level = gObj[iIndex].pInventory[iItemPos].m_Level;
	float dur = gObj[iIndex].pInventory[iItemPos].m_Durability;
	BOOL ret = gObj[iIndex].pInventory[iItemPos].IsItem();
	BYTE Option1 = gObj[iIndex].pInventory[iItemPos].m_Option1;
	BYTE Option2 = gObj[iIndex].pInventory[iItemPos].m_Option2;
	BYTE Option3 = gObj[iIndex].pInventory[iItemPos].m_Option3;
	BYTE NOption = gObj[iIndex].pInventory[iItemPos].m_NewOption;
	DWORD s_num = gObj[iIndex].pInventory[iItemPos].m_Number;
	BYTE ItemExOption = g_kJewelOfHarmonySystem.GetItemStrengthenOption(&gObj[iIndex].pInventory[iItemPos]);
	BYTE ItemExLevel = g_kJewelOfHarmonySystem.GetItemOptionLevel(&gObj[iIndex].pInventory[iItemPos]);
	BYTE Socket[5];
	Socket[0] = gObj[iIndex].pInventory[iItemPos].m_ItemSocket[0];
	Socket[1] = gObj[iIndex].pInventory[iItemPos].m_ItemSocket[1];
	Socket[2] = gObj[iIndex].pInventory[iItemPos].m_ItemSocket[2];
	Socket[3] = gObj[iIndex].pInventory[iItemPos].m_ItemSocket[3];
	Socket[4] = gObj[iIndex].pInventory[iItemPos].m_ItemSocket[4];

	BYTE NewOption[MAX_EXOPTION_SIZE];
	::ItemIsBufExOption(NewOption, &gObj[iIndex].pInventory[iItemPos]);
	int PetLevel = gObj[iIndex].pInventory[iItemPos].m_PetItem_Level;
	int PetExp = gObj[iIndex].pInventory[iItemPos].m_PetItem_Exp;
	BYTE SOption = gObj[iIndex].pInventory[iItemPos].m_SetOption;
	BYTE ItemEffectEx = gObj[iIndex].pInventory[iItemPos].m_ItemOptionEx;
	int item_number = gObj[iIndex].pInventory[iItemPos].m_Number;
	char szItemName[50] = "Item";
	int aAntiLootIndex = -1;

	if ( MapC[map_num].ItemDrop(type, level, dur, gObj[iIndex].X, gObj[iIndex].Y,Option1, Option2, Option3, NOption, SOption, item_number, aAntiLootIndex, PetLevel, PetExp, ItemEffectEx, Socket) == TRUE )
	{
		::gObjInventoryDeleteItem(iIndex, iItemPos);
		pResult.Result = TRUE;

		/*CLog.LogAdd(lMsg.Get(MSGGET(1, 223)), gObj[iIndex].AccountID, gObj[iIndex].Name, gObj[iIndex].MapNumber,
			gObj[iIndex].X, gObj[iIndex].Y, s_num, szItemName, type, level, Option1, Option2, Option3, (int)dur, NewOption[0],
			NewOption[1], NewOption[2], NewOption[3], NewOption[4], NewOption[5], NewOption[6], SOption,
			ItemEffectEx>>7, ItemExOption, ItemExLevel);*/
	}
	else
	{
		pResult.Result = FALSE;
	}

	gSendProto.DataSend(iIndex, (UCHAR*)&pResult, pResult.h.size);

	if ( pResult.Result == TRUE )
	{
		if ( iItemPos < INVENTORY_BAG_START )
		{
			if ( iItemPos== 10 || iItemPos == 11 )
			{
				gObjUseSkill.SkillChangeUse(iIndex);
			}

			::gObjMakePreviewCharSet(iIndex);

			PMSG_USEREQUIPMENTCHANGED pMsg;

			C1HeadSet((LPBYTE)&pMsg, 0x25, sizeof(PMSG_USEREQUIPMENTCHANGED));
			pMsg.NumberH = SET_NUMBERH(iIndex);
			pMsg.NumberL = SET_NUMBERL(iIndex);
			ItemByteConvert(pMsg.ItemInfo, gObj[iIndex].pInventory[iItemPos]);
			pMsg.ItemInfo[I_OPTION] = iItemPos * 16; // iItemPos << 16;
			pMsg.ItemInfo[I_OPTION] |= LevelSmallConvert(iIndex, iItemPos) & 0x0F;

			gSendProto.DataSend(iIndex, (UCHAR*)&pMsg, pMsg.h.size);
			gSendProto.VPMsgSend(&gObj[iIndex], (UCHAR*)&pMsg, pMsg.h.size);
		}
	}

	return (pResult.Result);
}

bool CBloodCastle::CheckUserHaveUlimateWeapon(int iIndex)
{
	if ( OBJMAX_RANGE(iIndex ) == FALSE )
	{
		return false;
	}

	if ( gObj[iIndex].Type != OBJ_USER || gObj[iIndex].Connected <= PLAYER_LOGGED )
	{
		return false;
	}

	bool bRetVal = false;

	for ( int x=0;x<INVENTORY_SIZE;x++)
	{
		if ( gObj[iIndex].pInventory[x].IsItem() == TRUE )
		{
			if ( gObj[iIndex].pInventory[x].m_Type == ITEMGET(0,19) || gObj[iIndex].pInventory[x].m_Type == ITEMGET(5,10) || gObj[iIndex].pInventory[x].m_Type == ITEMGET(4,18) )
			{
				bRetVal = true;
				break;
			}
		}
	}

	return bRetVal;
}

bool CBloodCastle::CheckWinnerExist(int iBridgeIndex)
{
	if ( !BC_BRIDGE_RANGE(iBridgeIndex))
		return false;

	if ( this->m_BridgeData[iBridgeIndex].m_iBC_COMPLETE_USER_INDEX != -1 )
		return true;

	return false;
}

bool CBloodCastle::CheckWinnerValid(int iBridgeIndex)
{
	if ( BC_BRIDGE_RANGE(iBridgeIndex) == false )
	{
		CLog.LogAdd("[Blood Castle] (%d) CBloodCastle::CheckWinnerValid() - !CHECK_LIMIT(iBridgeIndex, MAX_BLOODCASTLE_BRIDGE_COUNT)", iBridgeIndex+1);
		return false;
	}

	if ( this->m_BridgeData[iBridgeIndex].m_iBC_COMPLETE_USER_INDEX == -1 )
	{
		CLog.LogAdd("[Blood Castle] (%d) CBloodCastle::CheckWinnerValid() - m_BridgeData[iBridgeIndex].m_iBC_COMPLETE_USER_INDEX == -1", iBridgeIndex+1);
		return false;
	}

	if ( gObjIsConnected(this->m_BridgeData[iBridgeIndex].m_iBC_COMPLETE_USER_INDEX) == false )
	{
		CLog.LogAdd("[Blood Castle] (%d) CBloodCastle::CheckWinnerValid() - !gObjIsConnected(m_BridgeData[iBridgeIndex].m_iBC_COMPLETE_USER_INDEX)", iBridgeIndex+1);
		return false;
	}

	if ( gObj[this->m_BridgeData[iBridgeIndex].m_iBC_COMPLETE_USER_INDEX].m_cBloodCastleIndex == -1 || gObj[this->m_BridgeData[iBridgeIndex].m_iBC_COMPLETE_USER_INDEX].m_cBloodCastleSubIndex == -1 || gObj[this->m_BridgeData[iBridgeIndex].m_iBC_COMPLETE_USER_INDEX].m_cBloodCastleIndex != iBridgeIndex )
	{
		CLog.LogAdd("[Blood Castle] (%d) CBloodCastle::CheckWinnerValid() - (gObj[m_BridgeData[iBridgeIndex].m_iBC_COMPLETE_USER_INDEX].m_cBloodCastleIndex == -1) ...",
			iBridgeIndex+1);
		return false;
	}

	if ( gUser.BloodCastleMapRange(gObj[this->m_BridgeData[iBridgeIndex].m_iBC_COMPLETE_USER_INDEX].MapNumber) == false )
	{
		CLog.LogAdd("[Blood Castle] (%d) CBloodCastle::CheckWinnerValid() - !CHECK_BLOODCASTLE(gObj[m_BridgeData[iBridgeIndex].m_iBC_COMPLETE_USER_INDEX].MapNumber)",
			iBridgeIndex+1);

		return false;
	}

	return true;
}

bool CBloodCastle::CheckUserWinnerParty(int iBridgeIndex, int iIndex)
{
	if ( !BC_BRIDGE_RANGE(iBridgeIndex))
		return false;

	if ( gObjIsConnected(this->m_BridgeData[iBridgeIndex].m_iBC_COMPLETE_USER_INDEX) == FALSE )
		return false;

	if ( gObjIsConnected(iIndex) == FALSE )
		return false;

	int iPartyIndex1 = gObj[iIndex].PartyNumber;
	int iPartyIndex2 = gObj[this->m_BridgeData[iBridgeIndex].m_iBC_COMPLETE_USER_INDEX].PartyNumber;

	if ( OBJMAX_RANGE(iPartyIndex1) != FALSE && iPartyIndex1 == iPartyIndex2 )
		return true;

	return false;
}

bool CBloodCastle::CheckPartyExist(int iIndex)
{
	if ( !gObjIsConnected(iIndex))
		return false;

	int iPartyIndex = gObj[iIndex].PartyNumber;
	int iUserIndex;

	if ( !OBJMAX_RANGE(iPartyIndex))
		return false;

	for ( int iPartyUserIndex =0;iPartyUserIndex<MAX_USER_IN_PARTY;iPartyUserIndex++)
	{
		iUserIndex = gParty.m_PartyInfo[iPartyIndex].Number[iPartyUserIndex];

		if ( gObjIsConnected(iUserIndex))
		{
			if ( gUser.BloodCastleMapRange(gObj[iUserIndex].MapNumber) == true && BC_BRIDGE_RANGE(gObj[iUserIndex].m_cBloodCastleIndex) )
			{
				if ( gObj[iUserIndex].Live == 1 )
				{
					if ( gObj[iUserIndex].m_bBloodCastleComplete == false )
					{
						return true;
					}
				}
			}
		}
	}

	return false;
}

bool CBloodCastle::CheckWinnerPartyComplete(int iBridgeIndex)
{
	if ( !BC_BRIDGE_RANGE(iBridgeIndex) )
		return false;

	if ( !gObjIsConnected(this->m_BridgeData[iBridgeIndex].m_iBC_COMPLETE_USER_INDEX))
		return false;

	int iPartyIndex = gObj[this->m_BridgeData[iBridgeIndex].m_iBC_COMPLETE_USER_INDEX].PartyNumber;
	int iUserIndex;

	if ( OBJMAX_RANGE(iPartyIndex) == false )
		return false;	// #error why true??

	for ( int iPartyUserIndex =0;iPartyUserIndex<MAX_USER_IN_PARTY;iPartyUserIndex++)
	{
		iUserIndex = gParty.m_PartyInfo[iPartyIndex].Number[iPartyUserIndex];

		if ( gObjIsConnected(iUserIndex))
		{
			if ( gUser.BloodCastleMapRange(gObj[iUserIndex].MapNumber) == true && BC_BRIDGE_RANGE(gObj[iUserIndex].m_cBloodCastleIndex) )
			{
				if ( gObj[iUserIndex].Live == 1 )
				{
					if ( gObj[iUserIndex].m_bBloodCastleComplete == false )
					{
						return false;
					}
				}
			}
		}
	}

	return true;
}

bool CBloodCastle::SetBridgeWinner(int iBridgeIndex, int iIndex)
{
	if ( BC_BRIDGE_RANGE(iBridgeIndex) == false )
	{
		return false;
	}

	if ( gObjIsConnected(this->m_BridgeData[iBridgeIndex].m_iBC_COMPLETE_USER_INDEX) == true )
	{
		return false;
	}

	if ( gObjIsConnected(iIndex) == false )
	{
		return false;
	}

	if ( gUser.BloodCastleMapRange(gObj[iIndex].MapNumber) == false)
	{
		return false;
	}

	this->m_BridgeData[iBridgeIndex].m_iBC_COMPLETE_USER_INDEX = iIndex;

	return true;
}

int CBloodCastle::GetWinnerPartyCompleteCount(int iBridgeIndex)
{
	if ( !BC_BRIDGE_RANGE(iBridgeIndex) )
		return false;

	if ( !gObjIsConnected(this->m_BridgeData[iBridgeIndex].m_iBC_COMPLETE_USER_INDEX))
		return false;

	int iPartyIndex = gObj[this->m_BridgeData[iBridgeIndex].m_iBC_COMPLETE_USER_INDEX].PartyNumber;
	
	if ( !OBJMAX_RANGE(iPartyIndex))
		return false;

	int iPartyComplete=0;
	int iUserIndex;

	for ( int iPartyUserIndex =0;iPartyUserIndex<MAX_USER_IN_PARTY;iPartyUserIndex++)
	{
		iUserIndex = gParty.m_PartyInfo[iPartyIndex].Number[iPartyUserIndex];

		if ( gObjIsConnected(iUserIndex))
		{
			if ( gUser.BloodCastleMapRange(gObj[iUserIndex].MapNumber) == true && BC_BRIDGE_RANGE(gObj[iUserIndex].m_cBloodCastleIndex) )
			{
				if ( gObj[iUserIndex].Live == 1 )
				{
					if ( gObj[iUserIndex].m_bBloodCastleComplete == true )
					{
						iPartyComplete++;
					}
				}
			}
		}
	}

	return iPartyComplete;
}

int CBloodCastle::GetWinnerPartyCompletePoint(int iBridgeIndex)
{
	int iPartyComplete = this->GetWinnerPartyCompleteCount(iBridgeIndex);
	iPartyComplete--;

	if ( CHECK_LIMIT(iPartyComplete, MAX_USER_IN_PARTY) )
		return g_iBC_Party_EventPoint[iPartyComplete];

	return 0;
}

void CBloodCastle::ChangeMonsterState(int iBridgeIndex, int iIndex)
{
	if ( !BC_BRIDGE_RANGE(iBridgeIndex) )
		return;

	int iAddDamageMax = 0;
	int iAddDamageMin = 0;
	int iAddDefense = 0;

	if ( this->m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC < 300000 )
	{
		iAddDamageMin = g_iBC_MONSTER_CHANGE_STATE[1][0];
		iAddDamageMax = g_iBC_MONSTER_CHANGE_STATE[1][1];
		iAddDefense = g_iBC_MONSTER_CHANGE_STATE[1][2];
	}
	else if ( this->m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC < 600000 )
	{
		iAddDamageMin = g_iBC_MONSTER_CHANGE_STATE[0][0];
		iAddDamageMax = g_iBC_MONSTER_CHANGE_STATE[0][1];
		iAddDefense = g_iBC_MONSTER_CHANGE_STATE[0][2];
	}

	LPMONSTER_ATTRIBUTE lpMA = gMAttr.GetAttr(gObj[iIndex].Class);

	if ( lpMA == NULL )
		return;

	gObj[iIndex].m_AttackDamageMin = lpMA->m_DamageMin + iAddDamageMin;
	gObj[iIndex].m_AttackDamageMax = lpMA->m_DamageMax + iAddDamageMax;
	gObj[iIndex].m_Defense = lpMA->m_Defense + iAddDefense;
}

void CBloodCastle::FixUsersPlayStateWin(int iBridgeIndex)
{
	if ( !BC_BRIDGE_RANGE(iBridgeIndex) )
		return;

	if ( this->m_BridgeData[iBridgeIndex].m_iMISSION_SUCCESS != -1 )
	{
		for ( int i=0;i<MAX_BLOOD_CASTLE_SUB_BRIDGE;i++)
		{
			if ( this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex == -1 )
				continue;

			if ( gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].Connected < PLAYER_PLAYING )
				continue;

			if ( gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].m_cBloodCastleIndex == -1 ||
				 gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].m_cBloodCastleSubIndex == -1 ||
				 gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].m_cBloodCastleIndex != iBridgeIndex )
				continue;

			LPOBJ lpObj = &gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex];

			switch ( this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iUserState )
			{
				case 0:
					if ( OBJMAX_RANGE(lpObj->PartyNumber) && lpObj->PartyNumber == gObj[this->m_BridgeData[iBridgeIndex].m_iBC_COMPLETE_USER_INDEX].PartyNumber )
					{
						if ( gUser.BloodCastleMapRange(lpObj->MapNumber) == true && lpObj->Live == TRUE && lpObj->Life > 0.0 )
						{
							this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iUserState = 3;
						}
						else
						{
							this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iUserState = 4;
						}
					}
					else 
					{
						if ( gUser.BloodCastleMapRange(lpObj->MapNumber) == false || lpObj->Live == 0 || lpObj->Life <= 0.0 )
						{
							this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iUserState = 1;
						}
					}
					break;
				case 1:
					if ( OBJMAX_RANGE(lpObj->PartyNumber) && lpObj->PartyNumber == gObj[this->m_BridgeData[iBridgeIndex].m_iBC_COMPLETE_USER_INDEX].PartyNumber )
					{
						this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iUserState = 4;
					}
					break;
				case 3:
					if ( OBJMAX_RANGE(lpObj->PartyNumber) && lpObj->PartyNumber == gObj[this->m_BridgeData[iBridgeIndex].m_iBC_COMPLETE_USER_INDEX].PartyNumber )
					{
						if ( gUser.BloodCastleMapRange(lpObj->MapNumber) == false || lpObj->Live == 0 || lpObj->Life <= 0.0 )
						{
							this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iUserState = 4;
						}
					}
					else 
					{
						if ( gUser.BloodCastleMapRange(lpObj->MapNumber) == true && lpObj->Live == TRUE && lpObj->Life > 0.0 )
						{
							this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iUserState = 0;
						}
						else
						{
							this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iUserState = 1;
						}
					}
					break;
				case 4:
					if ( !OBJMAX_RANGE(lpObj->PartyNumber) || lpObj->PartyNumber != gObj[this->m_BridgeData[iBridgeIndex].m_iBC_COMPLETE_USER_INDEX].PartyNumber )
					{
						this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iUserState = 1;
					}
					break;
			}

			CLog.LogAdd("[Blood Castle] (%d) [%s][%s] FixUsersPlayStateWin() - State : %d",
				iBridgeIndex+1, lpObj->AccountID, lpObj->Name, this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iUserState);

		}
	}
}

void CBloodCastle::FixUsersPlayStateFail(int iBridgeIndex)
{
	if ( !BC_BRIDGE_RANGE(iBridgeIndex) )
		return;

	for ( int i=0;i<MAX_BLOOD_CASTLE_SUB_BRIDGE;i++)
	{
		if ( this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex == -1 )
			continue;

		if ( gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].Connected < PLAYER_PLAYING )
			continue;

		if ( gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].m_cBloodCastleIndex == -1 ||
			 gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].m_cBloodCastleSubIndex == -1 ||
			 gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].m_cBloodCastleIndex != iBridgeIndex )
			continue;

		LPOBJ lpObj = &gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex];

		switch ( this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iUserState )
		{
			case 3:
				if ( gUser.BloodCastleMapRange(lpObj->MapNumber) == false || lpObj->Live == 0 || lpObj->Life <= 0.0 )
				{
					this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iUserState = 1;
				}
				else
				{
					this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iUserState = 0;
				}
				break;
			case 4:
				this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iUserState = 1;
				break;
		}

		CLog.LogAdd("[Blood Castle] (%d) [%s][%s] FixUsersPlayStateFail() - State : %d", iBridgeIndex+1, lpObj->AccountID, lpObj->Name, this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iUserState);
	}
}

