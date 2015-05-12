//----------------------------------------------------------------------------------
// # Project:	Zodea eMU GameServer
// # Company:	RealCoderZ Software Incorpration. © 2012 http://www.realcoderz.com
// # Author:	WolF & M.E.S & Kristians
// # Hint:		This Script For Character Info Class.
//----------------------------------------------------------------------------------
#include "../Header Files/Stdafx.H"
#include "CharacterInfo.H"
#include "../Header Files/Structures.H"
#include "../Header Files/LogProc.H"


CCharacterInfo gCharInfo;
// -------------------------------------------------------------------------------------------------------------------------------------------------------
CCharacterInfo::CCharacterInfo()
{
	return;
}
// -------------------------------------------------------------------------------------------------------------------------------------------------------
CCharacterInfo::~CCharacterInfo()
{
	return;
}
// -------------------------------------------------------------------------------------------------------------------------------------------------------
void CCharacterInfo::Initialize()
{
	try
	{
		// ----- Normal Points
		CLASS_WIZARD_POINTS			=	MSTR.NormalPoint[0];			// Default 5
		CLASS_KNIGHT_POINTS			=	MSTR.NormalPoint[1];			// Default 5
		CLASS_ELF_POINTS			=	MSTR.NormalPoint[2];			// Default 5
		CLASS_MAGUMSA_POINTS		=	MSTR.NormalPoint[3];			// Default 7
		CLASS_DARKLORD_POINTS		=	MSTR.NormalPoint[4];			// Default 7
		CLASS_SUMMONER_POINTS		=	MSTR.NormalPoint[5];			// Default 5
		CLASS_FIGHTER_POINTS		=	MSTR.NormalPoint[6];			// Default 7
		// -----
		// ----- Plus Points After 2° Quest Finished
		CLASS_WIZARD_POINTS_PLUS	=	MSTR.SecondQuestPointPlus[0];	// Default 1
		CLASS_KNIGHT_POINTS_PLUS	=	MSTR.SecondQuestPointPlus[1];	// Default 1
		CLASS_ELF_POINTS_PLUS		=	MSTR.SecondQuestPointPlus[2];	// Default 1
		CLASS_SUMMONER_POINTS_PLUS	=	MSTR.SecondQuestPointPlus[3];	// Default 1
		// -----
		// ----- Plus Points After 3° Quest Finished (Master)
		CLASS_WIZARD_MPOINTS		=	MSTR.MasterPointsPlus[0];		// Default 1
		CLASS_KNIGHT_MPOINTS		=	MSTR.MasterPointsPlus[1];		// Default 1
		CLASS_ELF_MPOINTS			=	MSTR.MasterPointsPlus[2];		// Default 1
		CLASS_MAGUMSA_MPOINTS		=	MSTR.MasterPointsPlus[3];		// Default 1
		CLASS_DARKLORD_MPOINTS		=	MSTR.MasterPointsPlus[4];		// Default 1
		CLASS_SUMMONER_MPOINTS		=	MSTR.MasterPointsPlus[5];		// Default 1
		CLASS_FIGHTER_MPOINTS		=	MSTR.MasterPointsPlus[6];		// Default 1
		// -----
		WizardRingCreate			=	true;
		// -----
		// -----   	 Class Type	  -  Str - Dex - Vit - Ene - Led - Life	- Mana - LvlLife - LvlMana  VitToLife EneToMana
		SetCharInfo( CLASS_WIZARD   , 18,  18,   15,   30,   00,   60 ,    60,    1.0f,	    2.0f,	  2.0f,    2.0f );
		SetCharInfo( CLASS_KNIGHT   , 28,  20,   25,   10,   00,   110,    20,    2.0f,	    0.5f,	  3.0f,	   1.0f );
		SetCharInfo( CLASS_ELF      , 22,  25,   20,   15,   00,   80 ,    30,    1.0f,	    1.5f,	  2.0f,	   1.5f );
		SetCharInfo( CLASS_MAGUMSA  , 26,  26,   26,   26,   00,   110,    60,    1.0f,	    1.0f,	  2.0f,	   2.0f );
		SetCharInfo( CLASS_DARKLORD , 26,  20,   20,   15,   25,   90 ,    40,    1.5f,	    1.0f,	  2.0f,	   1.5f );
		SetCharInfo( CLASS_SUMMONER , 21,  21,   18,   23,   00,   70 ,    20,    1.0f,	    1.5f,	  2.0f,	   1.7f );
		SetCharInfo( CLASS_FIGHTER  , 25,  27,   32,   20,   00,   100,    40,    1.0f,	    1.0f,	  2.0f,	   2.0f );
	}
	catch(...)
	{
		CLog.Error("[CHARACTER-INFO] Initializing Could not be proceeded.");
	}
}
// -------------------------------------------------------------------------------------------------------------------------------------------------------
void CCharacterInfo::SetCharInfo(int ClassType, int Str, int Dex, int Vit, int Ene, int Led, float Life, float Mana, float LevelLife, float LevelMana, 
								 float VitToLife, float EneToMana)
{
	sCharInfo[ClassType].Strength		= Str;
	sCharInfo[ClassType].Dexterity		= Dex;
	sCharInfo[ClassType].Vitality		= Vit;
	sCharInfo[ClassType].Energy			= Ene;
	sCharInfo[ClassType].Leadership		= Led;
	// ------
	sCharInfo[ClassType].Life			= Life;
	sCharInfo[ClassType].MaxLife		= Life;
	sCharInfo[ClassType].LevelLife		= LevelLife;
	sCharInfo[ClassType].Mana			= Mana;
	sCharInfo[ClassType].MaxMana		= Mana;
	sCharInfo[ClassType].LevelMana		= LevelMana;
	// ------
	sCharInfo[ClassType].VitalityToLife = VitToLife;
	sCharInfo[ClassType].EnergyToMana	= EneToMana;
	// ------
	SetEquipment(ClassType);
}
// -------------------------------------------------------------------------------------------------------------------------------------------------------
void CCharacterInfo::SetEquipment(int Class)
{
	int		Item1		= NULL;
	int		Item2		= NULL;
	BYTE	Socket[5]	= {0};
	// -----
	try
	{
		for( int n=0; n<MAX_PLAYER_EQUIPMENT; n++ )
		{
		    sCharInfo[Class].Equipment[n].Clear();
		}
		// -----
		if( Class >= 0 && Class < 7 )
		{
			switch(Class)
			{
				case CLASS_WIZARD:
				{
					Item1	= 0x00;
					Item2	= 0x00;
				} break;
				// -----
				case CLASS_KNIGHT:
				{
					Item1	= ITEMGET(1, 0);
					Item2	= 0x00;
				} break;
				// -----
				case CLASS_ELF:
				{
					Item1	= ITEMGET(4, 0);
					Item2	= ITEMGET(4, 15);
				} break;
				// -----
				case CLASS_MAGUMSA:
				{
					Item1	= ITEMGET(6, 0);
					Item2	= ITEMGET(0, 1);
				} break;
				// -----
				case CLASS_DARKLORD:
				{
					Item1	= ITEMGET(6, 0);
					Item2	= ITEMGET(0, 1);
				} break;
				// -----
				case CLASS_SUMMONER:
				{
					Item1	= ITEMGET(1, 0);
					Item2	= 0x00;
				} break;
				// -----
				case CLASS_FIGHTER:
				{
					Item1	= ITEMGET(1, 0);
					Item2	= 0x00;
				} break;
			}
		}
		// -----
		memset(Socket, 0xFF, sizeof(Socket));
		// -----
		if ( Class != CLASS_WIZARD )
		{
			if ( Class == CLASS_ELF || Class == CLASS_MAGUMSA || Class == CLASS_DARKLORD )
			{
				sCharInfo[Class].Equipment[EQUIPMENT_WEAPON_RIGHT].m_Level		= 0;
				sCharInfo[Class].Equipment[EQUIPMENT_WEAPON_LEFT].m_Level		= 0;
				// -----
				sCharInfo[Class].Equipment[EQUIPMENT_WEAPON_RIGHT].Convert(Item1,0,0,0,0,0,0,Socket);
				sCharInfo[Class].Equipment[EQUIPMENT_WEAPON_LEFT].Convert(Item2,0,0,0,0,0,0,Socket);
			}
			else
			{
				sCharInfo[Class].Equipment[EQUIPMENT_WEAPON_RIGHT].m_Level		= 0;
				// -----
				sCharInfo[Class].Equipment[EQUIPMENT_WEAPON_RIGHT].Convert(Item1,0,0,0,0,0,0,Socket);
			}
		}
		// -----
		if ( WizardRingCreate = true )
		{
			Item1 = ITEMGET(13, 20);
			sCharInfo[Class].Equipment[12].m_Level = 1;
			sCharInfo[Class].Equipment[12].Convert(Item1,0,0,0,0,0,0,Socket);
			// -----
			Item1 = ITEMGET(13, 20);
			sCharInfo[Class].Equipment[13].m_Level = 2;
			sCharInfo[Class].Equipment[13].Convert(Item1,0,0,0,0,0,0,Socket);
		}
	}
	catch(...)
	{
		CLog.Error("[CHARACTER-INFO] Setting Character Info Could not be proceeded.");
	}
}
// -------------------------------------------------------------------------------------------------------------------------------------------------------
int CCharacterInfo::GetTotalPoint(int Class)
{
	if( Class < 0 || Class >= MAX_CLASS_TYPE )
		return FALSE;
	// -----
	return sCharInfo[Class].Strength + sCharInfo[Class].Dexterity + sCharInfo[Class].Vitality + sCharInfo[Class].Energy + sCharInfo[Class].Leadership;
}