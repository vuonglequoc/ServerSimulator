//**********************************************************************//
// Project:	HaRiBO MU GameServer - Season 6								//
// Company: RealCoderZ MU Development � 2010							//
// Description: MU Users Functions & Structures							//
// Coded & Modified By:	WolF & M.E.S									//
//**********************************************************************//
#include "stdafx.h"
#include "Main.H"
#include "LogProc.H"
#include "GameMain.h"
#include "MapServerManager.h"
#include "gObjMonster.h"
#include "ObjCalCharacter.h"
#include "ObjUseSkill.h"
#include "DarkSpirit.h"
#include "DSProtocol.h"
#include "DevilSquare.h"
#include "protocol.h"
#include "AcceptIp.h"
#include "BattleGround.h"
#include "MonsterAttr.h"
#include "ChaosBox.h"
#include "PacketChecksum.h"
#include "QuestInfo.h"
#include "GoldenEvent.h"
#include "Gate.h"
#include "ObjAttack.h"
#include "JSProtocol.H"
#include "MultiAttackHAckCheck.h"
#include "BattleSoccer.h"
#include "BattleSoccerManager.h"
#include "TUnion.h"
#include "TMonsterAIElement.h"
#include "Weapon.h"
#include "CrywolfSync.h"
#include "Kanturu.h"
#include "CashShop.h"
#include "Mercenary.h"
#include "KalimaGate.h"
#include "KanturuBattleUserMng.h"
#include "Guardian.h"
#include "JSProtocol.H"
#include "ItemAddOption.h"
#include "StatMng.h"
#include "SetItemOption.h"
#include "ChaosCastle.h"
#include "ReadScript.h"
#include "SkillAdditionInfo.h"
#include "CashItemPeriodSystem.h"
#include "TMonsterSkillManager.h"
#include "user.h"
#include "CastleSiege.h"
#include "LifeStone.h"
#include "GuardianStatue.h"
#include "CannonTower.h"
#include "CastleCrown.h"
#include "CastleCrownSwitch.h"
#include "Crywolf.h"
#include "CrywolfAltar.h"
#include "MasterSkillSystem.h"
#include "IllusionTemple.h"
#include "MoveSystem.h"
#include "Defines.h"
#include "GenSystem.h"
#include "RingAttackEvent.h"
#include "Functions.h"
#include "NpcTalk.h"
#include "DuelSystem.h"
#include "Notice.H"
#include "PersonalShop.H"
#include "Party.h"
#include "ImperialFort.h"
#include "DoubleGoer.h"
#include "Raklion.h"
#include "RedDragon.h"
#include "SantaEvent.h"
#include "CherryBlossom.h"
#include "Halloween.h"
#include "SQuest.h"
#include "Kanturu.h"
#include "SpecialItems.h"
#include "Pets.h"
#include "PKSystem.h"
#include "EDSProtocol.H"
#include "SendProto.H"
#include "../Test/CharacterInfo.h"


//-------------------------------------------------------------------------------------------------------------------------------------------
CUser gUser;
//-------------------------------------------------------------------------------------------------------------------------------------------
CUser::CUser(void)
{
	return;
}
//-------------------------------------------------------------------------------------------------------------------------------------------
CUser::~CUser(void)
{
	return;
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void CUser::ExpandedInventory1ItemBoxSet(LPOBJ lpObj,int itempos,int xl,int yl, unsigned char set_byte)
{
	int itemposx = (itempos - INVETORY_WEAR_SIZE)%8;
	int itemposy = (itempos - INVETORY_WEAR_SIZE)/4;

	int xx,yy;

	for(int y = 0; y < yl; y ++)
	{
		yy = itemposy + y;

		for(int x = 0; x < xl; x++)
		{
			xx = itemposx + x;

			if( ExtentCheck(xx,yy,8,4) == true )
			{
				*(BYTE*)(lpObj->pInventoryMap + (itemposy + y)*8+(itemposx + x)) = set_byte;
			}
			else
			{
				return;
			}
		}
	}
}

bool CUser::IsMasterClass(short Class)
{
	bool bReturn = false;

	switch ( Class )
	{
	case DB_GRAND_MASTER:
	case DB_BLADE_MASTER:
	case DB_HIGH_ELF:
	case DB_DUEL_MASTER:
	case DB_LORD_EMPEROR:
	case DB_DIMENSION_MASTER:
	case DB_FIST_MASTER:
		bReturn = true;
	break;
	}

	return bReturn;
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void CUser::BuffTimer(LPOBJ lpObj)
{
	gSpecialItem.TimerEffect(lpObj);

	// ---- Gladiators Glory
	if ( lpObj->GladiatorsGloryTime > 0 )
	{
		lpObj->GladiatorsGloryTime--;
		
		if ( lpObj->GladiatorsGloryTime <= 0 )
		{
			lpObj->GladiatorsGloryTime = 0;
			GCStateInfoSend(lpObj,0,103);
		}
	}

	// ---- Little Santas Buffers
	if ( lpObj->LittleSantaTime1 > 0 )
	{
		lpObj->LittleSantaTime1--;

		if ( lpObj->LittleSantaTime1 <= 0 )
		{
			lpObj->LittleSantaTime1 = 0;
			lpObj->AddLife-=500;
			if ( lpObj->AddLife < 0 )
			{
				lpObj->AddLife = 0;
			}
			if ( lpObj->Life > (lpObj->MaxLife+lpObj->AddLife) )
			{
				lpObj->Life = lpObj->MaxLife+lpObj->AddLife;
				GCReFillSend(lpObj->m_Index,lpObj->Life,0xFF,0,lpObj->iShield);
			}
			GCReFillSend(lpObj->m_Index,lpObj->MaxLife + lpObj->AddLife,0xFE,0,lpObj->iMaxShield + lpObj->iAddShield);
			GCStateInfoSend(lpObj,0,92);
		}
	}

	if ( lpObj->LittleSantaTime2 > 0 )
	{
		lpObj->LittleSantaTime2--;

		if ( lpObj->LittleSantaTime2 <= 0 )
		{
			lpObj->LittleSantaTime2 = 0;
			lpObj->m_Defense-=100;
			GCStateInfoSend(lpObj,0,95);
		}
	}

	if ( lpObj->LittleSantaTime3 > 0 )
	{
		lpObj->LittleSantaTime3--;

		if ( lpObj->LittleSantaTime3 <= 0 )
		{
			lpObj->LittleSantaTime3 = 0;
			lpObj->AddMana-=500;
			if ( lpObj->AddMana < 0 )
			{
				lpObj->AddMana = 0;
			}
			if ( lpObj->Mana > (lpObj->MaxMana+lpObj->AddMana) )
			{
				lpObj->Mana = lpObj->MaxMana+lpObj->AddMana;
				GCManaSend(lpObj->m_Index,lpObj->Mana,0xFF,0,lpObj->BP);
			}
			GCManaSend(lpObj->m_Index,lpObj->MaxMana+lpObj->AddMana,0xFE,0,lpObj->MaxBP+lpObj->AddBP);
			GCStateInfoSend(lpObj,0,93);
		}
	}

	if ( lpObj->LittleSantaTime4 > 0 )
	{
		lpObj->LittleSantaTime4--;

		if ( lpObj->LittleSantaTime4 <= 0 )
		{
			lpObj->LittleSantaTime4 = 0;
			lpObj->m_AttackSpeed-=15;
			lpObj->m_MagicSpeed-=15;
			GCStateInfoSend(lpObj,0,96);
		}
	}

	if ( lpObj->LittleSantaTime5 > 0 )
	{
		lpObj->LittleSantaTime5--;

		if ( lpObj->LittleSantaTime5 <= 0 )
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
	}

	if ( lpObj->LittleSantaTime6 > 0 )
	{
		lpObj->LittleSantaTime6--;

		if ( lpObj->LittleSantaTime6 <= 0 )
		{
			lpObj->LittleSantaTime6 = 0;
			GCStateInfoSend(lpObj,0,97);
		}
	}
}
//-------------------------------------------------------------------------------------------------------------------------------------------
bool CUser::BloodCastleMapRange(int MapNumber)
{
	bool bResult = false;
	// ----
	switch ( MapNumber )
	{
	case MAP_INDEX_BLOODCASTLE1:
	case MAP_INDEX_BLOODCASTLE2:
	case MAP_INDEX_BLOODCASTLE3:
	case MAP_INDEX_BLOODCASTLE4:
	case MAP_INDEX_BLOODCASTLE5:
	case MAP_INDEX_BLOODCASTLE6:
	case MAP_INDEX_BLOODCASTLE7:
	case MAP_INDEX_BLOODCASTLE8:
		bResult = true;
		break;
	}
	// ----
	return bResult;
}
//-------------------------------------------------------------------------------------------------------------------------------------------
bool CUser::DevilSquareMapRange(int MapNumber)
{
	return false;
}
//-------------------------------------------------------------------------------------------------------------------------------------------
bool CUser::ChaosCastleMapRange(int MapNumber)
{
	bool bResult = false;
	// ----
	switch ( MapNumber )
	{
	case MAP_INDEX_CHAOSCASTLE1:
	case MAP_INDEX_CHAOSCASTLE2:
	case MAP_INDEX_CHAOSCASTLE3:
	case MAP_INDEX_CHAOSCASTLE4:
	case MAP_INDEX_CHAOSCASTLE5:
	case MAP_INDEX_CHAOSCASTLE6:
	case MAP_INDEX_CHAOSCASTLE7:
		bResult = true;
		break;
	}
	// ----
	return bResult;
}
//-------------------------------------------------------------------------------------------------------------------------------------------
bool CUser::KalimaMapRange(int MapNumber)
{
	bool bResult = false;
	// ----
	switch ( MapNumber )
	{
	case MAP_INDEX_KALIMA1:
	case MAP_INDEX_KALIMA2:
	case MAP_INDEX_KALIMA3:
	case MAP_INDEX_KALIMA4:
	case MAP_INDEX_KALIMA5:
	case MAP_INDEX_KALIMA6:
	case MAP_INDEX_KALIMA7:
		bResult = true;
		break;
	}
	// ----
	return bResult;
}
//-------------------------------------------------------------------------------------------------------------------------------------------
bool CUser::IllusionTempleMapRange(int MapNumber)
{
	bool bResult = false;
	// ----
	switch ( MapNumber )
	{
	case MAP_INDEX_ILLUSION1:
	case MAP_INDEX_ILLUSION2:
	case MAP_INDEX_ILLUSION3:
	case MAP_INDEX_ILLUSION4:
	case MAP_INDEX_ILLUSION5:
	case MAP_INDEX_ILLUSION6:
		bResult = true;
		break;
	}
	// ----
	return bResult;
}
//-------------------------------------------------------------------------------------------------------------------------------------------
bool CUser::gObjIsMasteringLevel(LPOBJ lpObj)
{
	if( !OBJMAX_RANGE(lpObj->m_Index) )
	{
		return false;
	}
	if( lpObj->Type == OBJ_USER )
	{
		if( lpObj->Level < HRB_CharMaxLevel )
		{
			return false;
		}

		switch(lpObj->DbClass)
		{
		case DB_GRAND_MASTER:
		case DB_BLADE_MASTER:
		case DB_HIGH_ELF:
		case DB_DUEL_MASTER:
		case DB_LORD_EMPEROR:
		case DB_DIMENSION_MASTER:
		case DB_FIST_MASTER:
			return true;
			break;
		default:
			return false;
			break;
		}

	}
	else
	{
		return false;
	}
}
//-------------------------------------------------------------------------------------------------------------------------------------------
bool CUser::gObjIsRightPlayerClass (BYTE Class)
{
	if ( Class == 0x00 || Class == 0x01 || Class == 0x03 ||		/*Dark Wizard*/
		 Class == 0x10 || Class == 0x11 || Class == 0x13 ||		/*Dark Knight*/
		 Class == 0x20 || Class == 0x21 || Class == 0x23 ||		/*Fairy ElF*/
		 Class == 0x30 || Class == 0x33 ||						/*Magic Gladiator*/
		 Class == 0x40 || Class == 0x43 ||						/*Dark Lord*/
		 Class == 0x50 || Class == 0x51 || Class == 0x53 ||		/*Summoner*/
		 Class == 0x60 || Class == 0x63 )						/*Rage Fighter*/
	{
		return true;
	}
	else
	{
		return false;
	}
}
//-------------------------------------------------------------------------------------------------------------------------------------------
bool CUser::gObjIsSecondClassComplete(LPOBJ lpObj)
{
	if( !OBJMAX_RANGE(lpObj->m_Index) )
	{
		return false;
	}
	if( lpObj->Type == OBJ_USER )
	{
		if( lpObj->Level < 220 )
		{
			return false;
		}

		switch(lpObj->DbClass)
		{
		case DB_SOUL_MASTER:
		case DB_BLADE_KNIGHT:
		case DB_MUSE_ELF:
		case DB_BLOODY_SUMMONER:
			return true;
			break;
		default:
			return false;
			break;
		}

	}
	else
	{
		return false;
	}
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void CUser::gObjInventoryDeleteItemComplete(int aIndex, int ItemType, int Level, int Skill, int Luck, int AddOpt, int Exe)
{
	LPOBJ lpObj = &gObj[aIndex];
	// ----
	for ( int n=12;n<MAIN_INVENTORY_SIZE;n++ )
	{
		if ( lpObj->pInventory[n].IsItem() == true )
		{
			if ( lpObj->pInventory[n].m_Type == ItemType )
			{
				if ( Level == -1 || lpObj->pInventory[n].m_Level == Level )
				{
					if ( Skill == -1 || lpObj->pInventory[n].m_Option1 == Skill )
					{
						if ( Luck == -1 || lpObj->pInventory[n].m_Option2 == Luck )
						{
							if ( AddOpt == -1 || lpObj->pInventory[n].m_Option3 == AddOpt )
							{
								if ( Exe == -1 || lpObj->pInventory[n].m_NewOption == Exe )
								{
									gObjInventoryDeleteItem(lpObj->m_Index, n);
									GCInventoryItemDeleteSend(lpObj->m_Index, n, 0);
								}
							}
						}
					}
				}
			}
		}
	}
	
}
//-------------------------------------------------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------------------------------------------------
int ChangeCount; 
int lOfsChange;
int GuildUserCount;
int GuildUserOfs;
int GuildInfoCount;
int GuildInfoOfs;
int skillSuccess;
int gCurConnectUser;
int gDisconnect;
int gObjTotalUser;
int gItemLoopMax;
int gItemLoop;
int gItemLoop2;
int gObjCSFlag;


CViewportGuild ViewGuildMng;

OBJECTSTRUCT gObj[OBJMAX];
int gObjCount;
int gObjMonCount;
int gObjCallMonCount;

MessageStateMachine gSMMsg[OBJMAX][MAX_MONSTER_SEND_MSG];	
ExMessageStateMachine gSMAttackProcMsg[OBJMAX][MAX_MONSTER_SEND_ATTACK_MSG];
BILL_CLASS m_ObjBill[OBJMAX];
HANDLE hThread_gObjMove;

// --------------------------------------------------------------------------------
// --------------------------------	STRUCTS	---------------------------------------
// --------------------------------------------------------------------------------

struct PWMSG_COUNT 
{
	struct PWMSG_HEAD h;
	unsigned char count;
};

struct PBMSG_COUNT 
{
	struct PBMSG_HEAD h;
	unsigned char count;
};

struct PMSG_ITEMVIEWPORTCREATE 
{
	unsigned char NumberH;
	unsigned char NumberL;
	unsigned char px;
	unsigned char py;
	unsigned char ItemInfo[MAX_ITEM_INFO];
};

struct PMSG_VIEWPORTCREATE
{
	unsigned char NumberH;
	unsigned char NumberL;
	unsigned char X;
	unsigned char Y;
	unsigned char CharSet[18];
	char Id[10];
	unsigned char TX;
	unsigned char TY;
	unsigned char DirAndPkLevel;
	unsigned char btViewSkillCount;
};

struct PMSG_VIEWPORTCREATE_CHANGE
{
	unsigned char NumberH;
	unsigned char NumberL;
	unsigned char X;
	unsigned char Y;
	unsigned char SkinH;
	unsigned char SkinL;
	char Id[10];
	unsigned char TX;
	unsigned char TY;
	unsigned char DirAndPkLevel;
	unsigned char CharSet[18];
	unsigned char btViewSkillCount;
};

struct PMSG_VIEWPORTDESTROY
{
	unsigned char NumberH;
	unsigned char NumberL;
};

struct PMSG_MONSTER_VIEWPORTCREATE
{
	unsigned char NumberH;
	unsigned char NumberL;
	unsigned char Type_HI;
	unsigned char Type_LO;
	unsigned char X;
	unsigned char Y;
	unsigned char TX;
	unsigned char TY;
	unsigned char Path;
	unsigned char btViewSkillCount;
};

struct PMSG_CALLMONSTER_VIEWPORTCREATE
{
	unsigned char NumberH;
	unsigned char NumberL;
	unsigned char Type_HI;
	unsigned char Type_LO;
	unsigned char X;
	unsigned char Y;
	unsigned char TX;
	unsigned char TY;
	unsigned char Path;
	unsigned char Id[10];
	unsigned char btViewSkillCount;
};

struct PMSG_GUILDVIEWPORT_USER
{
	unsigned char NumberH;
	unsigned char NumberL;
	unsigned char GNumberH;
	unsigned char GNumberL;
};

struct PMSG_GUILDVIEWPORT
{
	unsigned char NumberH;
	unsigned char NumberL;
	char GuildName[8];
	unsigned char Mark[32];
};

struct PMSG_EX_SKILL_COUNT
{
	struct PBMSG_HEAD h;
	unsigned char NumberH;
	unsigned char NumberL;
	unsigned char Type;
	unsigned char Count;
};

struct PMSG_ANS_PSHOP_VIEWPORT_NOTIFY
{
	struct PWMSG_HEAD2 h;
	unsigned char btCount;
};

struct PMSG_PSHOP_VIEWPORT_NOTIFY
{
  unsigned char btNumberH;
  unsigned char btNumberL;
  char szPShopText[36];
};

struct PMSG_CHARREGEN 
{
	PBMSG_HEAD h;
	BYTE subcode;
	BYTE MapX;
	BYTE MapY;
	BYTE MapNumber;
	BYTE Dir;
	WORD Life;
	WORD Mana;
	WORD wShield;
	WORD BP;
	DWORD Unk;
	BYTE Exp1;
	BYTE Exp2;
	BYTE Exp3;
	BYTE Exp4;
	unsigned int Money;
};

struct PMSG_POSMAGIC_RESULT
{
	unsigned char NumberH;
	unsigned char NumberL;
};

struct PMSG_POSMAGIC_COUNT
{
	struct PBMSG_HEAD h;
	unsigned char NumberH;
	unsigned char NumberL;
	unsigned char MagicNumberH;
	unsigned char MagicNumberL;
	unsigned char MapX;
	unsigned char MapY;
	unsigned char Count;
};

struct PMSG_USE_STAT_FRUIT
{
	struct PBMSG_HEAD h;
	unsigned char result;
#if(GS_CASTLE)
	unsigned short btStatValue;
#else
	unsigned char btStatValue;
#endif
	unsigned char btFruitType;
};

typedef struct tagITEM_BYTE_DB
{
	BYTE btType;
	BYTE btOption;
	BYTE btDurability;
	BYTE btExcellentOption;
	BYTE btAncientOption;
} 
ITEM_BYTE_DB, * LPITEM_BYTE_DB;

struct PMSG_UNION_VIEWPORT_NOTIFY_COUNT
{
	struct PWMSG_HEAD h;
	unsigned char btCount;
};

struct PMSG_UNION_VIEWPORT_NOTIFY
{
	unsigned char btNumberH;
	unsigned char btNumberL;
	int iGuildNumber;
	unsigned char btGuildRelationShip;
	char szUnionName[8];
};

struct PMSG_NOTIFY_REGION_OF_WEAPON
{
	struct PBMSG_HEAD2 h;
	unsigned char btWeaponType;
	unsigned char btPointX;
	unsigned char btPointY;
};

struct PMSG_NOTIFY_TARGET_OF_WEAPON
{
	struct PBMSG_HEAD2 h;
	unsigned char NumberH;
	unsigned char NumberL;
	unsigned char btWeaponType;
};

struct PMSG_WEAPON_OWNER_VIEWPORT_NOTIFY_COUNT
{
	struct PWMSG_HEAD h;
	unsigned char btCount;
};

struct PMSG_WEAPON_OWNER_VIEWPORT_NOTIFY
{
	unsigned char btObjType;
	unsigned char btObjClassH;
	unsigned char btObjClassL;
	unsigned char btObjIndexH;
	unsigned char btObjIndexL;
	short btX;
	short btY;
	unsigned char CharSet[17];
	unsigned char btViewSkillCount;
};

// --------------------------------------------------------------------------------
// ------------------------------	FUNCTIONS	-----------------------------------
// --------------------------------------------------------------------------------

int gObjViewSkillBufferConvert(LPOBJ lpObj, LPWORD lpViewSkillBuffer)
{
	if (lpViewSkillBuffer == NULL)
	{
		return 0;
	}

	int iCount = 0;

	for(int i=0; i<MAX_VIEWSKILL; i++)
	{
		if ( lpObj->m_ViewSkillState[i] != 0 )
		{
			lpViewSkillBuffer[iCount] = i;
			iCount++;
		}
	}
	return iCount;
}

BYTE GuildInfoBuf[10000];
BYTE GuildUserBuf[10000];
BYTE SendGBufChange[5000];

PMSG_VIEWPORTCREATE pViewportCreate;
PMSG_VIEWPORTCREATE_CHANGE pViewportCreateChange;
PMSG_VIEWPORTDESTROY pViewportDestroy;
PMSG_MONSTER_VIEWPORTCREATE pMonsterViewportCreate;
PMSG_CALLMONSTER_VIEWPORTCREATE pCallMonsterViewportCreate;

PWMSG_COUNT pItemCount;
PMSG_ITEMVIEWPORTCREATE pItemViewportCreate;
PMSG_VIEWPORTDESTROY pItemViewportDestroy;

short RoadPathTable[MAX_ROAD_PATH_TABLE] = { -1, -1, 0, -1, 1, -1, 1, 0, 1, 1, 0, 1, -1, 1, -1, 0 };	// THIS VARIABLE HAS SOME STRANGE THINGS ONT ITS UBICATION
int  gServerMaxUser = 500;
BOOL g_EnergyCheckOff = 1;


void gObjSkillUseProcTime500(LPOBJ lpObj)
{
	if ( lpObj->SkillHellFire2State != 0 )
	{
		//unsigned long dwCurrentTick = GetTickCount();
		DWORD dwCurrentTick = GetTickCount();
		lpObj->SkillHellFire2Count = (dwCurrentTick - lpObj->SkillHellFire2Time) / 500;

		if ( lpObj->SkillHellFire2Count > 12 )
		{
			lpObj->SkillHellFire2Count = 12;
		}

		if(dwCurrentTick >= (lpObj->SkillHellFire2Time + 6000))
		{
			CMagicInf * lpMagic = gObjGetMagicSearch(lpObj,0x28);

			if(lpMagic != 0)
			{
				gObjUseSkill.RunningSkill(lpObj->m_Index,0,lpMagic,0);
			}
		}
		else
		{
			int mana = MagicDamageC.SkillGetMana(40);
			mana = mana * 20 / 100;
			BOOL bSuccess = TRUE;

			if ( mana > lpObj->Mana )
			{
				bSuccess = FALSE;
				int count = 0;

				while ( true )
				{
					count++;

					if ( gObjIsConnected(lpObj) == FALSE )
					{
						CLog.LogAdd("★[CHECK_LOG_INFINITY] gObjIsConnected() error %s %d", __FILE__, __LINE__);
						break;
					}

					if ( count > 100 )
					{
						CLog.LogAdd("★[CHECK_LOG_INFINITY] ( _count > 100 ) error %s %d", __FILE__, __LINE__);
						break;
					}

					int pos = gObjGetManaItemPos(lpObj->m_Index);

					if ( pos == -1 )
					{
						break;
					}

					PMSG_USEITEM pMsg;
					pMsg.inventoryPos = pos;
					pMsg.invenrotyTarget = 0;
					CGUseItemRecv(&pMsg, lpObj->m_Index);

					if ( mana <= lpObj->Mana )
					{
						bSuccess = TRUE;
						break;
					}
				}
			}
			
			if ( bSuccess == FALSE )
			{
				CMagicInf * lpMagic = gObjGetMagicSearch(lpObj, 40);
				
				if (lpMagic != NULL )
				{
					gObjUseSkill.RunningSkill(lpObj->m_Index, 0, lpMagic, 0);
				}
			}
			else
			{
				lpObj->Mana -= mana;

				if ( lpObj->Mana < 0.0f )
				{
					lpObj->Mana = 0.0f;
				}
				// This Warning is Because Mana in game is not Float, so it have to be changed to int
				GCManaSend(lpObj->m_Index, lpObj->Mana, -1, 0, lpObj->BP);
				PMSG_EX_SKILL_COUNT pMsg;
				C1HeadSet( (LPBYTE)&pMsg, 0xBA, sizeof(pMsg));
				pMsg.Type = 0x28;
				pMsg.Count = lpObj->SkillHellFire2Count;
				pMsg.NumberH = SET_NUMBERH(lpObj->m_Index);
				pMsg.NumberL = SET_NUMBERL(lpObj->m_Index);

				gSendProto.DataSend(lpObj->m_Index, (UCHAR *)&pMsg, pMsg.h.size);
				gSendProto.VPMsgSend(lpObj, (UCHAR *)&pMsg, pMsg.h.size);
			}
		}
	}
}

void MonsterAndMsgProc()
{
	LPOBJ lpObj;
	int n;
	int aIndex;

	for ( n=0;n<OBJMAX;n++)
	{
		lpObj = &gObj[n];

		if ( lpObj->Connected == PLAYER_PLAYING  )
		{
			if ( lpObj->Type == OBJ_MONSTER || lpObj->Type == OBJ_NPC) 
			{
				if(lpObj->m_iCurrentAI != 0)
				{
					continue;
				}
				gObjMonsterProcess(lpObj);
			}
			else
			{
				gObjSkillUseProcTime500(lpObj);
				gObjMsgProc(lpObj);
				CreateFrustrum(lpObj->X, lpObj->Y, n);
			}

			if ( lpObj->Type == OBJ_USER )
			{
				gDarkSpirit[n].Run();
			}
		}
		else if ( lpObj->Connected >= PLAYER_LOGGED )
		{
			if ( lpObj->Type == OBJ_USER )
			{
				gObjMsgProc(lpObj);

				if ( lpObj->Connected == PLAYER_PLAYING )
				{
					CreateFrustrum(lpObj->X, lpObj->Y, n);
				}
			}
		}
	}

	for ( n=0;n<OBJMAXUSER;n++)
	{
		lpObj = &gObj[n+OBJ_STARTUSERINDEX];

		if ( lpObj->Connected == PLAYER_PLAYING  )
		{
			aIndex = lpObj->m_Index;

			for ( int i=0;i<MAX_MONSTER_SEND_ATTACK_MSG;i++)
			{
				if ( gSMAttackProcMsg[n][i].MsgCode >= 0 )
				{
					if ( GetTickCount() > gSMAttackProcMsg[n][i].MsgTime )
					{
						gObjStateAttackProc(lpObj, gSMAttackProcMsg[n][i].MsgCode, gSMAttackProcMsg[n][i].SendUser,
							gSMAttackProcMsg[n][i].SubCode, gSMAttackProcMsg[n][i].SubCode2);
						gSMAttackProcMsg[n][i].MsgCode = -1;
					}
				}
			}
		}
	}
	TMonsterSkillManager::MonsterSkillProc();
}

void gObjInit()
{
	gObjCount=OBJ_STARTUSERINDEX;
	gObjMonCount=0;
	gObjCallMonCount= OBJ_MAXMONSTER;

	CItem * pTempInventory = new CItem[INVENTORY_SIZE];
	BYTE * pTempInventoryMap = new BYTE [INVENTORY_MAP_SIZE];

	for ( int n = 0; n< OBJMAX ; n++)
	{
		memset(&gObj[n], 0, sizeof(OBJECTSTRUCT));
		gObj[n].Type = (BYTE)OBJ_EMPTY;
		gObj[n].m_socket = INVALID_SOCKET;
		gObj[n].PathTime = GetTickCount();
		gObj[n].m_RecallMon = -1;
		gObj[n].m_lpMagicBack = new CMagicInf[MAGIC_SIZE];
		gObj[n].Magic = new CMagicInf[MAGIC_SIZE];
		
		if ( n >= OBJ_STARTUSERINDEX )
		{
			gObj[n].Inventory1 = new CItem[INVENTORY_SIZE];
			gObj[n].Inventory2 = new CItem[INVENTORY_SIZE];
			gObj[n].InventoryMap1 = new BYTE[INVENTORY_MAP_SIZE];
			gObj[n].InventoryMap2 = new BYTE[INVENTORY_MAP_SIZE];
		}
		else
		{
			gObj[n].Inventory1 = pTempInventory;
			gObj[n].Inventory2 = pTempInventory;
			gObj[n].InventoryMap1 = pTempInventoryMap;
			gObj[n].InventoryMap2 = pTempInventoryMap;
		}

		if ( n >= OBJ_STARTUSERINDEX )
		{
			gObj[n].Trade = new CItem[TRADE_BOX_SIZE];
			gObj[n].TradeMap = new BYTE[TRADE_BOX_MAP_SIZE];
			gObj[n].pWarehouse = new CItem[WAREHOUSE_SIZE];
			gObj[n].pWarehouseMap = new BYTE[WAREHOUSE_SIZE];
		}
		
		gObj[n].WarehouseCount = 0;
		gObj[n].pChaosBox = NULL;
		
		if ( n >= OBJ_STARTUSERINDEX )
		{
			gObj[n].PerSocketContext = new _PER_SOCKET_CONTEXT;
		}

		InitializeCriticalSection(&gObj[n].m_critPShopTrade);
	}
	InitFrustrum();
}

void gObjEnd()
{
	DisconnectAllPlayers();
	LogOutAllUsers();

	for ( int n=0; n< OBJMAX; n++)
	{
		if ( gObj[n].m_lpMagicBack != NULL )
		{
			delete [] gObj[n].m_lpMagicBack;
		}

		if ( n >= OBJ_STARTUSERINDEX )
		{
			if ( gObj[n].Inventory1 != NULL )
			{
				delete gObj[n].Inventory1;
			}

			if ( gObj[n].Inventory2 != NULL )
			{
				delete gObj[n].Inventory2;
			}

			if ( gObj[n].InventoryMap1 != NULL )
			{
				delete gObj[n].InventoryMap1;
			}

			if ( gObj[n].InventoryMap2 != NULL )
			{
				delete gObj[n].InventoryMap2;
			}

			if ( gObj[n].PerSocketContext != NULL )
			{
				delete gObj[n].PerSocketContext;
			}

			if ( gObj[n].Trade != NULL )
			{
				delete gObj[n].Trade;
			}

			if ( gObj[n].TradeMap != NULL )
			{
				delete gObj[n].TradeMap;
			}

			if ( gObj[n].pWarehouse != NULL )
			{
				delete gObj[n].pWarehouse;
			}

			if ( gObj[n].pWarehouseMap != NULL )
			{
				delete gObj[n].pWarehouseMap;
			}
		}

		DeleteCriticalSection( &gObj[n].m_critPShopTrade );
	}
}

void gObjClearViewportOfMine(LPOBJ lpObj)
{
	for (int i=0;i<MAX_VIEWPORT;i++)
	{
		lpObj->VpPlayer[i].state =0;
		lpObj->VpPlayer[i].number =-1;
	}

	lpObj->VPCount=0;
}

void gObjClearViewport(LPOBJ lpObj)
{
	for ( int i=0;i<MAX_VIEWPORT;i++)
	{
		lpObj->VpPlayer[i].state=0;
		lpObj->VpPlayer[i].number =-1;
		lpObj->VpPlayer2[i].state=0;
		lpObj->VpPlayer2[i].number=-1;
	}

	lpObj->VPCount = 0;
	lpObj->VPCount2 = 0;
}

void gObjCloseSet(int aIndex, int Flag)
{
	if ( aIndex < 0 || aIndex > OBJMAX-1 )
	{
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if ( lpObj->CloseCount > 0 )
	{
		return;
	}

	if ( lpObj->Connected == PLAYER_PLAYING )
	{
		
		if ( gUser.BloodCastleMapRange(lpObj->MapNumber) == true )
		{
			g_BloodCastle.SearchUserDropQuestItem(aIndex);
		}

		if ( (GetTickCount() - lpObj->MySelfDefenseTime )< 30000)
		{
			GCServerMsgStringSend(lMsg.Get(MSGGET(4, 109)), lpObj->m_Index, 1);
			return;
		}

		if ( (lpObj->m_IfState.use != 0 ) && ( ( lpObj->m_IfState.type == 1 ) || (lpObj->m_IfState.type == 6) || (lpObj->m_IfState.type == 13) || (lpObj->m_IfState.type == 7)  ) ) 
		{
			gObjSaveChaosBoxItemList(lpObj);
			switch ( lpObj->m_IfState.type )
			{
				case 1:
					GCServerMsgStringSend(lMsg.Get(MSGGET(4, 110)), lpObj->m_Index, 1);
					break;

				case 6:
					GCServerMsgStringSend(lMsg.Get(MSGGET(4, 111)), lpObj->m_Index, 1);
					break;

				case 7:
					GCServerMsgStringSend(lMsg.Get(MSGGET(4, 127)), lpObj->m_Index, 1);
					break;

				case 13:
					GCServerMsgStringSend(lMsg.Get(MSGGET(4, 229)), lpObj->m_Index, 1);
					break;
			}

			return;
		}
	}

	lpObj->CloseCount = 6;
	lpObj->CloseType = Flag;
	lpObj->bEnableDelCharacter = 1;
}

void gObjCharTradeClear(LPOBJ lpObj)
{
	if ( lpObj->Type != OBJ_USER )
	{
		return;
	}

	memset(lpObj->TradeMap, 0xFF , TRADE_BOX_SIZE);

	for ( int i=0 ; i< TRADE_BOX_SIZE ; i++)
	{
		lpObj->Trade[i].Clear();
	}

	lpObj->TradeMoney = 0;
	lpObj->TradeOk = false;
}

void gObjCharZeroSet(int aIndex)
{
	int i;
	LPOBJ lpObj = &gObj[aIndex];
	int n;

	memset( lpObj->Name, 0, MAX_ACCOUNT_LEN);
	memset( lpObj->GuildName, 0, sizeof (lpObj->GuildName ));
	lpObj->m_Option = 3;
	lpObj->m_iScriptMaxLife = 0;
	lpObj->Level = 0;

	memset( lpObj->BackName,0,sizeof(lpObj->BackName));

	lpObj->Life = 0;
	lpObj->MaxLife = 0;
	lpObj->AddLife = 0;
	lpObj->AddMana = 0;
	lpObj->Live = FALSE;
	lpObj->Dir = 0;
	lpObj->X = 0;
	lpObj->Y = 0;
	lpObj->TX = 0;
	lpObj->TY = 0;
	lpObj->MTX = 0;
	lpObj->MTY = 0;
	lpObj->MapNumber = MAP_INDEX_LORENCIA;
	lpObj->RegenTime = 0;
	lpObj->m_State = 0;
	lpObj->RegenOk = 0;
	lpObj->lpAttackObj = 0;
	lpObj->PartyTargetUser = -1;
	lpObj->m_MoveGateNumber = -1;
	lpObj->CloseCount = -1;
	lpObj->CloseType = -1;
	lpObj->m_IfState.state = -1;
	lpObj->m_IfState.type = -1;
	lpObj->m_IfState.use = 0;
	lpObj->m_InterfaceTime = 0;
	lpObj->m_ActionNumber = 0;
	lpObj->m_State = 0;
	lpObj->m_StateSub = 0;
	lpObj->m_ViewState = 0;
	lpObj->m_Rest = 0;

	lpObj->iMaxShield = 0;
	lpObj->iShield = 0;
	lpObj->iAddShield = 0;
	lpObj->iFillShield = 0;
	lpObj->iFillShieldMax = 0;
	lpObj->iFillShieldCount = 0;
	lpObj->dwShieldAutoRefillTimer = 0;
	lpObj->m_wItemEffectType = 0;
	lpObj->m_iItemEffectValidTime = 0;
	lpObj->m_wCashPoint = 0;
	lpObj->m_iPeriodItemEffectIndex = -1;
	lpObj->m_wExprienceRate = 100;
	lpObj->m_wItemDropRate = 100;

	memset(lpObj->m_ViewSkillState, 0, sizeof(lpObj->m_ViewSkillState));

	lpObj->m_PoisonBeattackCount = 0;
	lpObj->m_ColdBeattackCount = 0;
	lpObj->m_Attribute = 0;
	lpObj->m_ImmuneToMagicCount = 0;
	lpObj->m_ImmuneToHarmCount = 0;
	lpObj->m_iMonsterBattleDelay = 0;
	lpObj->m_cKalimaGateExist = FALSE;
	lpObj->m_iKalimaGateIndex = -1;
	lpObj->m_cKalimaGateEnterCount = 0;
	lpObj->TradeOk = false;
	lpObj->sHDCount = 0;
	lpObj->PathCount = 0;
	lpObj->PathCur = 0;
	lpObj->PathStartEnd = 0;
	lpObj->VPCount = 0;
	lpObj->VPCount2 = 0;
	lpObj->m_ShopTime = 0;
	lpObj->m_ManaFillCount = 0;
	lpObj->FillLife = 0;
	lpObj->iObjectSecTimer = 0;
	lpObj->m_FriendServerOnline = FALSE;
	lpObj->SetOpAddMaxAttackDamage = 0;
	lpObj->SetOpAddMinAttackDamage = 0;
	lpObj->SetOpAddDamage = 0;
	lpObj->SetOpIncAGValue = 0;
	lpObj->SetOpAddCriticalDamageSuccessRate = 0;
	lpObj->SetOpAddCriticalDamage = 0;
	lpObj->SetOpAddExDamageSuccessRate = 0;
	lpObj->SetOpAddExDamage = 0;
	lpObj->SetOpAddSkillAttack = 0;
	lpObj->AddStrength = 0;
	lpObj->AddDexterity = 0;
	lpObj->AddVitality = 0;
	lpObj->AddEnergy = 0;
	lpObj->SetOpAddAttackDamage = 0;
	lpObj->SetOpAddDefence = 0;
	lpObj->SetOpAddMagicPower = 0;
	lpObj->SetOpAddDefenceRate = 0;
	lpObj->SetOpIgnoreDefense = 0;
	lpObj->SetOpDoubleDamage = 0;
	lpObj->SetOpTwoHandSwordImproveDamage = 0;
	lpObj->SetOpImproveSuccessAttackRate = 0;
	lpObj->SetOpReflectionDamage = 0;
	lpObj->SetOpImproveSheldDefence = 0;
	lpObj->SetOpDecreaseAG = 0;
	lpObj->SetOpImproveItemDropRate = 0;
	memset( lpObj->m_AddResistance, 0, sizeof(lpObj->m_AddResistance));
	lpObj->IsFullSetItem = false;
	lpObj->DamageMinus = 0;
	lpObj->DamageReflect = 0;
	lpObj->MonsterDieGetMoney = 0;
	lpObj->MonsterDieGetLife = 0;
	lpObj->MonsterDieGetMana = 0;
	lpObj->m_CriticalDamage = 0;
	lpObj->m_ExcelentDamage = 0;
	lpObj->UseMagicNumber = 0;
	lpObj->UseMagicTime = 0;
	lpObj->UseMagicCount = 0;
	lpObj->OSAttackSerial = -1;
	lpObj->SASCount = 0;
	lpObj->SkillAttackTime = GetTickCount();
	lpObj->m_AttackSpeed = 0;
	lpObj->m_MagicSpeed = 0;
	lpObj->m_TotalAttackTime = GetTickCount();
	lpObj->m_TotalAttackCount = 0;
	lpObj->CheckSumTime = GetTickCount() + 10000;
	lpObj->CheckSumTableNum = -1;
	lpObj->m_SumLastAttackTime = 0;
	lpObj->m_DetectCount = 0;
	lpObj->m_DetectSpeedHackTime = 0;
	lpObj->m_SpeedHackPenalty = 0;
	lpObj->m_DetectedHackKickCount = 0;
	lpObj->IsInBattleGround = false;
	lpObj->m_dwPKTimer = GetTickCount();
	lpObj->CheckTickCount = 0;
	lpObj->m_AttackSpeedHackDetectedCount = 0;

	for ( n =0;n<MAX_VIEWPORT ;n++)
	{
		lpObj->VpPlayer[n].state = 0;
		lpObj->VpPlayer2[n].state = 0;
		lpObj->VpPlayer[n].number = -1;
		lpObj->VpPlayer2[n].number = -1;
	}

	lpObj->GuildNumber = 0;
	lpObj->lpGuild = NULL;
	lpObj->PartyNumber = -1;
	lpObj->PartyTargetUser = -1;
	lpObj->m_Drink = 0;
	lpObj->Authority = 0;
	lpObj->AuthorityCode = 0;
	lpObj->Penalty = 0;
	lpObj->m_RecallMon = -1;
	lpObj->m_Change = -1;
	lpObj->GuildStatus = -1;
	lpObj->LastAttackerID = -1;
	lpObj->m_cChaosCastleIndex= -1;
	lpObj->m_cChaosCastleSubIndex = -1;
	lpObj->m_iChaosCastleBlowTime = 0;
	lpObj->m_cKillUserCount = 0;
	lpObj->m_cKillMonsterCount = 0;
	lpObj->m_InWebzen = false;
	memset( lpObj->CharSet, 0, sizeof(lpObj->CharSet));

	for ( i =0; i<MAX_SELF_DEFENSE;i++)
	{
		lpObj->SelfDefense[i] = -1;
		lpObj->SelfDefenseTime[i] = 0;
	}

	lpObj->MySelfDefenseTime = 0;
	lpObj->m_MagicDefense = 0;
	lpObj->m_SkillDefense = 0;
	lpObj->m_SkillDefenseTime = 0;
	lpObj->m_SkillAttack = 0;
	lpObj->m_SkillAttackTime = 0;
	lpObj->m_SkillAttack2 = 0;
	lpObj->m_SkillAttackTime2 = 0;
	lpObj->LoadWareHouseInfo = false;
	lpObj->m_SkillAddLife = 0;
	lpObj->m_SkillAddLifeTime = 0;
	lpObj->m_SkillHarden = 0;
	lpObj->m_SkillHardenTime = 0;
	lpObj->m_SkillMagumReduceDefense = 0;
	lpObj->m_SkillMagumReduceDefenseTime = 0;
	lpObj->m_SkyBossMonSheildLinkIndex = -1;
	lpObj->m_SkyBossMonSheild = 0;
	lpObj->m_SkyBossMonSheildTime = 0;
	lpObj->m_bAttackerKilled = false;
	lpObj->m_MoveOtherServer = 0;
	lpObj->m_BossGoldDerconMapNumber = -1;
	lpObj->TargetNumber =	0;
	lpObj->TargetShopNumber = 0;
	lpObj->WarehouseMoney = 0;
	lpObj->WarehouseSave = 0;
	lpObj->WarehouseLock = -1;
	lpObj->WarehouseCount = 0;
	lpObj->WarehousePW = 0;
	lpObj->WarehouseUnfailLock = 0;
	lpObj->m_bDevilSquareAuth = false;
	lpObj->m_TimeCount = 0;
	lpObj->EventChipCount = 0;
	lpObj->MutoNumber = 0;
	lpObj->HaveWeaponInHand = false;
	lpObj->m_ReqWarehouseOpen = 0;
	lpObj->iStoneCount = 0;
	lpObj->m_iDuelUserReserved = -1;
	lpObj->m_iDuelUserRequested = -1;
	lpObj->m_iDuelUser = -1;
	lpObj->m_btDuelScore = 0;
	lpObj->m_iDuelTickCount = 0;
	lpObj->m_MaxLifePower = 0;
	lpObj->m_WizardSkillDefense = 0;
	lpObj->m_WizardSkillDefenseTime = 0;
	lpObj->m_PacketChecksumTime = 0;
	lpObj->AddBP = 0;
	lpObj->m_CheckLifeTime = 0;
	memset(lpObj->m_Quest, (BYTE)-1, sizeof(lpObj->m_Quest));
	lpObj->m_SendQuestInfo = 0;
	gPacketCheckSum.ClearCheckSum(lpObj->m_Index);
	lpObj->m_bSentGGAuth = false;
	lpObj->m_NPggCheckSumSendTime = 0;
	lpObj->PlusStatQuestClear = false;
	lpObj->ComboSkillquestClear = false;
	lpObj->m_LastTeleportTime = 0;
	lpObj->m_ClientHackLogCount = 0;
	lpObj->UseEventServer = FALSE;

	if ( lpObj->Type == OBJ_USER )
	{
		memset(lpObj->pWarehouseMap, (BYTE)-1, WAREHOUSE_SIZE);
		memset(lpObj->TradeMap, (BYTE)-1, TRADE_BOX_SIZE);
	}

	for ( i=0;i<MAX_MAGIC ; i++)
	{
		lpObj->Magic[i].Clear();
	}

	lpObj->MagicCount = 0;

	for ( i=0;i<MAX_MONSTER_SEND_MSG;i++)
	{
		gSMMsg[aIndex][i].MsgCode = -1;
	}

	lpObj->m_bIsInMonsterHerd = false;
	lpObj->m_bIsMonsterAttackFirst = true;
	lpObj->m_lpMonsterHerd = NULL;

	if ( (aIndex-OBJ_STARTUSERINDEX) >= 0 )
	{
		for ( i=0;i<MAX_MONSTER_SEND_ATTACK_MSG;i++)
		{
			gSMAttackProcMsg[aIndex-OBJ_STARTUSERINDEX][i].MsgCode = -1;
		}
	}

	lpObj->SkillHellFire2State = 0;
	lpObj->SkillHellFire2Count = 0;
	lpObj->m_PoisonType = 0;
	lpObj->m_IceType = 0;
	lpObj->Leadership = 0;
	lpObj->AddLeadership = 0;
	lpObj->ChatLimitTime = 0;
	lpObj->ChatLimitTimeSec = 0;
	lpObj->SkillRecallParty_Time = 0;
	lpObj->SkillAddCriticalDamageTime = 0;
	lpObj->SkillAddCriticalDamage = 0;
	lpObj->SkillLongSpearChange = false;
	lpObj->SkillDelay.Init();

	lpObj->bEnableDelCharacter = 1;

	if ( lpObj->Type == OBJ_USER )
	{
		for (i=0;i<WAREHOUSE_SIZE;i++)
		{
			lpObj->pWarehouse[i].Clear();
		}
	}

	::gMultiAttackHackCheck[aIndex].Init();
	lpObj->pTransaction = 0;
	::gObjMonsterHitDamageInit(lpObj);
	::gObjSetInventory1Pointer(&gObj[aIndex]);

	if ( lpObj->Type == OBJ_USER )
	{
		memset(lpObj->pInventoryMap, (BYTE)-1, INVENTORY_MAP_SIZE);
	}

	lpObj->m_bPShopOpen = false;
	lpObj->m_bPShopTransaction = false;
	lpObj->m_bPShopItemChange = false;
	lpObj->m_bPShopRedrawAbs = false;
	memset(lpObj->m_szPShopText, 0, sizeof(lpObj->m_szPShopText));
	lpObj->m_bPShopWantDeal = false;
	lpObj->m_iPShopDealerIndex = -1;
	memset(lpObj->m_szPShopDealerName, 0, sizeof(lpObj->m_szPShopDealerName));
	memset(lpObj->m_iVpPShopPlayer, 0, sizeof(lpObj->m_iVpPShopPlayer));
	lpObj->m_wVpPShopPlayerCount =0;
	lpObj->iGuildUnionTimeStamp = 0;
	::gObjCharTradeClear(lpObj);

	if ( lpObj->Type == OBJ_USER )
	{
		for ( i=0;i<WAREHOUSE_SIZE;i++)
		{
			lpObj->pWarehouse[i].Clear();
		}

		for ( i=0;i<INVENTORY_SIZE;i++)
		{
			lpObj->Inventory1[i].Clear();
		}

		for ( i=0;i<INVENTORY_SIZE;i++)
		{
			lpObj->Inventory2[i].Clear();
		}

		for ( i=0;i<TRADE_BOX_SIZE;i++)
		{
			lpObj->Trade[i].Clear();
		}
	}

	lpObj->bIsChaosMixCompleted = false;
	lpObj->m_bMapSvrMoveQuit = false;

#if(GS_CASTLE)
	lpObj->m_btCsNpcExistVal1 = 0;
	lpObj->m_btCsNpcType = 0;
	lpObj->m_btCsGateOpen = 0;
	lpObj->m_iCsGateLeverLinkIndex = -1;
	lpObj->m_btCsNpcDfLevel = 0;
	lpObj->m_btCsNpcRgLevel = 0;
	lpObj->m_btCsJoinSide = 0;
	lpObj->m_bCsGuildInvolved = 0;
#endif
	lpObj->m_bIsCastleNPCUpgradeCompleted = false;
	lpObj->m_btWeaponState = 0;
	lpObj->m_btKillCount = 0;
	lpObj->m_iSkillStunTime = 0;
	lpObj->m_iSkillBrandOfSkillTime = 0;
	lpObj->m_iSkillInvisibleTime = 0;
	lpObj->m_iSkillManaSwellTime = 0;
	lpObj->m_iSkillManaSwell = 0;
	lpObj->m_iPotionBlessTime = 0;
	lpObj->m_iPotionSoulTime = 0;
	lpObj->m_iAccumulatedDamage = 0;
#if(GS_CASTLE)
	lpObj->m_iCreatedActivationTime = 0;
	lpObj->m_btLifeStoneCount = 0;
#endif
	lpObj->m_iAccumulatedCrownAccessTime = 0;
	lpObj->m_iSkillNPCHelpTime = 0;
	lpObj->m_iSkillNPCDefense = 0;
	lpObj->m_iSkillNPCAttack = 0;
	lpObj->m_iMuseElfInfinityArrowSkillTime = 0;
	lpObj->m_MonsterSkillElementInfo.Reset();
	lpObj->m_iCrywolfMVPScore = 0;
	lpObj->m_iSkillDistanceErrorCount = 0;
	lpObj->m_dwSkillDistanceErrorTick = 0;
	g_kJewelOfHarmonySystem.InitEffectValue(&lpObj->m_JewelOfHarmonyEffect);
	g_Item380System.InitEffectValue(&lpObj->m_ItemOptionExFor380);

#if(!GS_CASTLE)
	lpObj->m_bKanturuEntranceByNPC = 0;
#endif

#if(!GS_CASTLE)
	lpObj->m_IllusionIndex = -1;
	lpObj->m_IllusionTeam = -1;
	lpObj->m_IllusionKillCount = 0;
	lpObj->m_IllusionShieldSkillTime = 0;
#endif

	gItemSocket.InitEffect(&lpObj->ItemSocketEffect);

	lpObj->m_iDuelRoom = -1;

	//-- Skills Time Clear
	lpObj->m_SkillSleepTime = 0;
	lpObj->m_SkillReflex = 0;
	lpObj->m_SkillReflexTime = 0;
	lpObj->m_SkillWeakness = 0;
	lpObj->m_SkillWeaknessTime = 0;
	lpObj->m_SkillInnovation = 0;
	lpObj->m_SkillInnovationTime = 0;
	lpObj->m_SkillBerserker = 0;
	lpObj->m_SkillBerserkerTime = 0;
	lpObj->m_SkillNight = 0;
	lpObj->m_SkillNightTime = 0;

	lpObj->m_RageFighterSkillDefenseRate = 0;
	lpObj->m_RageFighterSkillDefenseRateTime = 0;
	lpObj->m_RageFighterSkillFitnessAddLife = 0;
	lpObj->m_RageFighterSkillFitnessTime = 0;
	lpObj->m_RageFighterSkillRedArmorIgnore = 0;
	lpObj->m_RageFighterSkillRedArmorIgnoreTime = 0;

	lpObj->GenContribution = 0;
	lpObj->GenFamily = 0;
	lpObj->GenLevel = 0;
	lpObj->GenNextContribution = 0;
	lpObj->GenRanking = 0;
	lpObj->GenReward = 0;

	lpObj->LittleSanta1 = 0;
	lpObj->LittleSanta2 = 0;
	lpObj->LittleSanta3 = 0;
	lpObj->LittleSanta4 = 0;
	lpObj->LittleSanta5 = 0;
	lpObj->LittleSanta6 = 0;

	lpObj->LittleSantaTime1 = 0;
	lpObj->LittleSantaTime2 = 0;
	lpObj->LittleSantaTime3 = 0;
	lpObj->LittleSantaTime4 = 0;
	lpObj->LittleSantaTime5 = 0;
	lpObj->LittleSantaTime6 = 0;

	for (int i=0;i<600;i++)
	{
		lpObj->NewQuestInfo[i] = 0;
	}

	for ( int i=0;i<7;i++)
	{
		lpObj->SpecialBuff[i] = 0;
		lpObj->SpecialBuffEffectTime[i] = 0;
		lpObj->SpecialBuffEffectType1[i] = -1;
		lpObj->SpecialBuffEffectType2[i] = -1;
		lpObj->SpecialBuffEffectValue1[i] = -1;
		lpObj->SpecialBuffEffectValue2[i] = -1;
		lpObj->SpecialBuffItem[i] = -1;
	}

	lpObj->SkillMagicCircle = 0;
	lpObj->SkillMagicCircleTime = 0;

	lpObj->IsMarried = 0;
	lpObj->SendRequest = 0;
	lpObj->RecvRequest = 0;
	memset(lpObj->SendRequestName, 0, MAX_ACCOUNT_LEN);
	memset(lpObj->RecvRequestName, 0, MAX_ACCOUNT_LEN);
	memset(lpObj->MarryName, 0, MAX_ACCOUNT_LEN);
	
	::gObjClearViewport(&gObj[aIndex]);
}

int gObjGetSocket(SOCKET socket)
{
	for (int n=OBJ_STARTUSERINDEX;n<OBJMAX;n++)
	{
		if (gObj[n].Connected != PLAYER_EMPTY)
		{
			if (gObj[n].m_socket == socket)
			{
				return n;
			}
		}
	}
	return -1;
}

void gObjSetTradeOption( int aIndex, int option)
{
	if ( gObjIsConnected(aIndex) == TRUE )
	{
		if ( option == 0 )
		{
			gObj[aIndex].m_Option = 0;
			GCServerMsgStringSend(lMsg.Get(MSGGET(4, 97)), aIndex, 1);
		}
		else
		{
			gObj[aIndex].m_Option |= 1;
			GCServerMsgStringSend(lMsg.Get(MSGGET(4, 96)), aIndex, 1);
		}
	}
}

void gObjSetDuelOption(int aIndex, int option)
{
	if ( gObjIsConnected(aIndex) == TRUE )
	{
		if ( option == 0 )
		{
			gObj[aIndex].m_Option = 0;
		}
		else
		{
			gObj[aIndex].m_Option |= 2;
		}
	}
}


//Done 100%
void DbItemSetInByte(LPOBJ lpObj, struct SDHP_DBCHAR_INFORESULT* lpMsg, int ItemDbByte, bool* bAllItemExist)
{
	int n;
	int itype;
	int _type;
	CItem item;
	BYTE OptionData;
	WORD hiWord;
	WORD loWord;
	bool bIsItemExist = true;

	for(n = 0; n < INVENTORY_SIZE;n++)
	{
		bIsItemExist = true;
		lpObj->pInventory[n].Clear();
		itype = lpMsg->dbInventory[n*ItemDbByte];

		if(lpMsg->dbInventory[n*ItemDbByte] == 0xFF && (lpMsg->dbInventory[n*ItemDbByte+DBI_NOPTION_DATA] & 0x80) == 0x80 && (lpMsg->dbInventory[n*ItemDbByte+DBI_OPTION380_DATA] & 0xF0) == 0xF0)
		{
			continue;
		}

		itype |= (lpMsg->dbInventory[n*ItemDbByte+DBI_OPTION380_DATA] & 0xF0) * 32;
		itype |= (lpMsg->dbInventory[n*ItemDbByte+DBI_NOPTION_DATA] & 0x80) * 2;
		_type = itype;

		if(lpMsg->dbInventory[n*ItemDbByte] == 0xFF
			&& (lpMsg->dbInventory[n*ItemDbByte+DBI_NOPTION_DATA] & 0x80) == 0x80
			&& (lpMsg->dbInventory[n*ItemDbByte+DBI_OPTION380_DATA] & 0xF0) == 0xF0)
		{
			itype = -1;
		}

		if(IsItem(_type) == FALSE)
		{
			bIsItemExist = false;

			if(bAllItemExist != false)
			{
				*bAllItemExist = false;
			}
		}

		if(_type == ITEMGET(14,17) || _type == ITEMGET(14,18))
		{
			if( lpMsg->dbInventory[n*ItemDbByte+DBI_DUR] == 0 )
			{
				itype = -1;
			}
		}

		if(_type == ITEMGET(14,19))
		{
			if(lpMsg->dbInventory[n*ItemDbByte+DBI_DUR]==(BYTE)-1)
			{
				itype = -1;
			}
		}

		if(itype != -1)
		{
			item.m_Level = DBI_GET_LEVEL(lpMsg->dbInventory[n*ItemDbByte+DBI_OPTION_DATA]);

			if(_type != ITEMGET(14,11))
			{
				if(item.m_Level > 15)
				{
					item.m_Level = 0;
				}
			}

			OptionData = lpMsg->dbInventory[n*ItemDbByte+DBI_OPTION_DATA];
			item.m_Option1 = DBI_GET_SKILL(OptionData);
			item.m_Option2 = DBI_GET_LUCK(OptionData);
			item.m_Option3 = DBI_GET_OPTION(OptionData);

			if(_type == ITEMGET(13,3))
			{
				item.m_Option3 |= DBI_GET_OPTION28(lpMsg->dbInventory[n*ItemDbByte+DBI_NOPTION_DATA]) >> 7;
			}
			else
			{
				if((lpMsg->dbInventory[n*ItemDbByte+DBI_NOPTION_DATA] & 0x70)== 0x70)
				{
					item.m_Option3 = 7;
				}
			}

			item.m_Durability = lpMsg->dbInventory[n*ItemDbByte+DBI_DUR];
			item.m_JewelOfHarmonyOption = lpMsg->dbInventory[n*ItemDbByte+DBI_JOH_DATA];


			item.m_ItemOptionEx = DBI_GET_380OPTION(lpMsg->dbInventory[n*ItemDbByte+DBI_OPTION380_DATA]);

			if(item.m_ItemOptionEx)
			{
				item.m_Type = itype;
				if(g_Item380System.Is380Item(&item)==false)
				{
					item.m_ItemOptionEx = 0;
					CLog.LogAdd("[380Item][%s][%s] Invalid 380 Item Option in Inventory pos[%d]", lpObj->AccountID,lpObj->Name,n);
				}
			}

			item.m_ItemSocket[0] = lpMsg->dbInventory[n*ItemDbByte+DBI_SOCKET_1];
			item.m_ItemSocket[1] = lpMsg->dbInventory[n*ItemDbByte+DBI_SOCKET_2];
			item.m_ItemSocket[2] = lpMsg->dbInventory[n*ItemDbByte+DBI_SOCKET_3];
			item.m_ItemSocket[3] = lpMsg->dbInventory[n*ItemDbByte+DBI_SOCKET_4];
			item.m_ItemSocket[4] = lpMsg->dbInventory[n*ItemDbByte+DBI_SOCKET_5];

			item.Convert(itype,item.m_Option1,item.m_Option2,item.m_Option3,DBI_GET_NOPTION(lpMsg->dbInventory[n*ItemDbByte+DBI_NOPTION_DATA]),lpMsg->dbInventory[n*ItemDbByte+DBI_SOPTION_DATA],item.m_ItemOptionEx,item.m_ItemSocket);

			if(_type == ITEMGET(14,7))
			{

			}
			else
			{
				if(_type >= ITEMGET(14,0) && _type <= ITEMGET(14,8) || _type >= ITEMGET(14,38) && _type <= ITEMGET(14,40))
				{
					if(item.m_Durability == 0.0f)
					{
						item.m_Durability = 1.0f;
					}

					if (HRB_FixPotions == TRUE)
					{
						if(item.m_Durability > 255.0f)
						{
							item.m_Durability = 255.0f;
						}
					}
					else
					{
						if(item.m_Durability > 3.0f)
						{
							item.m_Durability = 3.0f;
						}
					}
				}
#if(GS_CASTLE)//Review the number of items that each one must have ...
				else if(_type == ITEMGET(14,70) || _type == ITEMGET(14,71))//cashshop
				{
					if(item.m_Durability == 0.0f)
					{
						item.m_Durability = 1.0f;
					}
					
					if(item.m_Durability > 100.0f)
					{
						item.m_Durability = 100.0f;
					}
				}
				
				if( _type == ITEMGET(14,78) || _type == ITEMGET(14,79)
					|| _type == ITEMGET(14,80) || _type == ITEMGET(14,81) 
					|| _type == ITEMGET(14,82) )
				{
					if(item.m_Durability == 0.0f)
					{
						item.m_Durability = 1.0f;
					}
					
					if(item.m_Durability > 3.0f)
					{
						item.m_Durability = 3.0f;
					}
				}
				
				if( _type == ITEMGET(14,63) || _type == ITEMGET(14,64) )
				{
					if(item.m_Durability == 0.0f)
					{
						item.m_Durability = 1.0f;
					}
					
					if(item.m_Durability > 1.0f)
					{
						item.m_Durability = 1.0f;
					}
				}
#endif
				else if(_type >= ITEMGET(14,46) && _type <= ITEMGET(14,50))
				{
					if(item.m_Durability == 0.0f)
					{
						item.m_Durability = 1.0f;
					}

					if(item.m_Durability > 3.0f)
					{
						item.m_Durability = 3.0f;
					}
				}
				else if(_type >= ITEMGET(14,35) && _type <= ITEMGET(14,37))
				{
					if(item.m_Durability == 0.0f)
					{
						item.m_Durability = 1.0f;
					}

					if(item.m_Durability > 1.0f)
					{
						item.m_Durability = 1.0f;
					}
				}
				else if(_type != ITEMGET(13,10) && _type != ITEMGET(14,29) && _type != ITEMGET(14,21))
				{
					if(item.m_Level == 3)
					{
						if(item.m_Durability > item.m_BaseDurability && bIsItemExist == 1)
						{
							item.m_Durability = item.m_BaseDurability;
						}
					}
				}
			}

			lpObj->pInventory[n].m_Option1 = item.m_Option1;
			lpObj->pInventory[n].m_Option2 = item.m_Option2;
			lpObj->pInventory[n].m_Option3 = item.m_Option3;
			lpObj->pInventory[n].m_JewelOfHarmonyOption = item.m_JewelOfHarmonyOption;
			lpObj->pInventory[n].m_ItemOptionEx = item.m_ItemOptionEx;
			lpObj->pInventory[n].m_ItemSocket[0] = item.m_ItemSocket[0];
			lpObj->pInventory[n].m_ItemSocket[1] = item.m_ItemSocket[1];
			lpObj->pInventory[n].m_ItemSocket[2] = item.m_ItemSocket[2];
			lpObj->pInventory[n].m_ItemSocket[3] = item.m_ItemSocket[3];
			lpObj->pInventory[n].m_ItemSocket[4] = item.m_ItemSocket[4];


			hiWord = ((lpMsg->dbInventory[n*ItemDbByte+DBI_SERIAL2])&0xFF)&0xFF | (((lpMsg->dbInventory[n*ItemDbByte+DBI_SERIAL1])&0xFF)&0xFF) * 256;
			loWord = ((lpMsg->dbInventory[n*ItemDbByte+DBI_SERIAL4])&0xFF)&0xFF | (((lpMsg->dbInventory[n*ItemDbByte+DBI_SERIAL3])&0xFF)&0xFF) * 256;

			item.m_Number = ((loWord &0xFFFF)&0xFFFF) | ((hiWord & 0xFFFF)&0xFFFF) << 16;

			gObjInventoryInsertItemPos(lpObj->m_Index,item,n,0);

			if(bIsItemExist == false)
			{
				lpObj->pInventory[n].m_bItemExist = 0;

				CLog.LogAdd("error-L2 : Unknown Item found [%s][%s] (Type:%d, LEV:%d, DUR:%d OP1:%d, OP2:%d, OP3:%d, NEWOP:%d, SET:%d)",
					lpObj->AccountID,lpObj->Name,lpObj->pInventory[n].m_Type,lpObj->pInventory[n].m_Level,
					lpObj->pInventory[n].m_Durability,lpObj->pInventory[n].m_Option1,
					lpObj->pInventory[n].m_Option2,lpObj->pInventory[n].m_Option3,
					lpObj->pInventory[n].m_NewOption,lpObj->pInventory[n].m_SetOption);
			}
		}
	}
	gObjRequestPetItemInfo(lpObj->m_Index, 0);
}

// --------------------------------------------------
// [Sunday, 20 Feb, 2011] M.E.S ~ Set BP
// --------------------------------------------------
void gObjSetBP(int aIndex)
{
	int Strength	= gObj[aIndex].Strength		+	gObj[aIndex].AddStrength;
	int Dexterity	= gObj[aIndex].Dexterity	+	gObj[aIndex].AddDexterity;
	int Vitality	= gObj[aIndex].Vitality		+	gObj[aIndex].AddVitality;
	int Energy		= gObj[aIndex].Energy		+	gObj[aIndex].AddEnergy;
	int Leadership	= gObj[aIndex].Leadership	+	gObj[aIndex].AddLeadership;
	// ------
	switch ( gObj[aIndex].Class )
	{
		case CLASS_WIZARD:		
			gObj[aIndex].MaxBP = (int)(Strength * 0.2) + (Dexterity * 0.4) + (Vitality * 0.3) + (Energy * 0.2);		
			break;
			// ------
		case CLASS_KNIGHT:		
			gObj[aIndex].MaxBP = (int)(Strength * 0.15) + (Dexterity * 0.2) + (Vitality * 0.3) + (Energy * 1.0);	
			break;
			// ------
		case CLASS_ELF:			
			gObj[aIndex].MaxBP = (int)(Strength * 0.3) + (Dexterity * 0.2) + (Vitality * 0.3) + (Energy * 0.2);		
			break;
		case CLASS_MAGUMSA:		
			gObj[aIndex].MaxBP = (int)(Strength * 0.2) + (Dexterity * 0.25) + (Vitality * 0.3) + (Energy * 0.15);	
			break;
			// ------
		case CLASS_DARKLORD:	
			gObj[aIndex].MaxBP = (int)(Strength * 0.3) + (Dexterity * 0.2) + (Vitality * 0.1) + (Energy * 0.15) + (Leadership * 0.3);	
			break;
			// ------
		case CLASS_SUMMONER:	
			gObj[aIndex].MaxBP = (int)(Strength * 0.3) + (Dexterity * 0.2) + (Vitality * 0.3) + (Energy * 0.2);	
			break;
			// ------
		case CLASS_FIGHTER:		
			gObj[aIndex].MaxBP = (Strength * 0.15) + (Dexterity * 0.2) + (Vitality * 0.3) + (Energy * 1.0);			
			break;
			// ------
	}
}

int gObjSetCharacter(LPBYTE lpdata, int aIndex)
{
	int itype;
	int n;
	char szCharName[MAX_ACCOUNT_LEN+1];
	SDHP_DBCHAR_INFORESULT * lpMsg = (SDHP_DBCHAR_INFORESULT *)lpdata;

	if ( OBJMAX_RANGE(aIndex) == FALSE )
	{
		CLog.LogAdd("[gObjSetCharacter]: Index Error -> Invalid Index: %d, File: %s, Line: %d",aIndex, __FILE__, __LINE__);
		return FALSE;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if ( lpObj->Connected < PLAYER_LOGGED )
	{
		return FALSE;
	}

	if ( gObj[aIndex].Connected == PLAYER_PLAYING )
	{
		CLog.LogAddC(TColor.Red(), lMsg.Get(MSGGET(1, 242)), gObj[aIndex].AccountID, gObj[aIndex].Name);
		return FALSE;
	}
	// -- Check if Player have right Class - Added by M.E.S
	if ( gUser.gObjIsRightPlayerClass(lpMsg->Class) == false )
	{
		return FALSE;
	}
	
	gObjCharZeroSet(aIndex);

	if ( gChaosBox.ChaosBoxInit(lpObj) == false )
	{
		CLog.LogAdd("[gObjSetCharacter]: ChaosBox Init Error -> File: %s, Line: %d", __FILE__, __LINE__);
	}

	lpObj->m_PosNum = 0;
	lpObj->Level = lpMsg->Level;
	lpObj->LevelUpPoint = lpMsg->LevelUpPoint;
	lpObj->DbClass = lpMsg->Class;
	lpObj->Class = lpMsg->Class >> 4;
	lpObj->ChangeUP = lpMsg->Class & 0x07 & 0x01;
	lpObj->ChangeUP2 = (lpMsg->Class & 0x07 & 0x02) >> 1;

	szCharName[MAX_ACCOUNT_LEN] = 0;
	memcpy(szCharName, lpMsg->Name , MAX_ACCOUNT_LEN);
	strcpy(lpObj->Name, szCharName);

	if ( lpObj->Level < 1 )
	{
		CLog.LogAddC(TColor.Red(), "[gObjSetCharacter]: Level Error ->  %s Level lower than 1", lpMsg->Name);
		return FALSE;
	}

	lpObj->X = lpMsg->MapX;
	lpObj->Y = lpMsg->MapY;
	lpObj->MapNumber = lpMsg->MapNumber;
	lpObj->StartX = lpObj->X;
	lpObj->StartY = lpObj->Y;
	
	// Check if Map Exist
	if ( MAX_MAP_RANGE(lpObj->MapNumber) == FALSE )
	{
		CLog.LogAdd("[gObjSetCharacter]: Map Number Error -> Map Number[%d] doesn't exist, File: %s, Line: %d",lpObj->MapNumber, __FILE__, __LINE__);
		// ----
		if ( lpObj->Class == CLASS_ELF  )
		{
			lpObj->MapNumber = MAP_INDEX_NORIA;
			MapC[lpObj->MapNumber].GetMapPos(lpObj->MapNumber, lpObj->X, lpObj->Y);
		}
		else if( lpObj->Class == CLASS_SUMMONER  )
		{
			int mgt = 267;
			short x,y,level;
			BYTE map = lpObj->MapNumber,dir;

			int result = GateC.GetGate(mgt,(BYTE &)x,(BYTE &)y,(BYTE &)map,(BYTE &)dir,(short &)level);

			if(result >= 0)
			{
				lpObj->MapNumber = map;
				lpObj->X = x;
				lpObj->Y = y;
			}
		}
		else
		{
			lpObj->MapNumber = MAP_INDEX_LORENCIA;
			MapC[lpObj->MapNumber].GetLevelPos(lpObj->Level, lpObj->X, lpObj->Y);
		}
	}
	// Moving to Start Map if level is lower than 6
	if ( lpObj->Level < 6 )
	{
		if ( lpObj->Class == CLASS_ELF  )
		{
			lpObj->MapNumber = MAP_INDEX_NORIA;
			MapC[lpObj->MapNumber].GetMapPos(lpObj->MapNumber, lpObj->X, lpObj->Y);
		}
		else if( lpObj->Class == CLASS_SUMMONER  )
		{
			int mgt = 267;
			short x,y,level;
			BYTE map = lpObj->MapNumber,dir;

			int result = GateC.GetGate(mgt,(BYTE &)x,(BYTE &)y,(BYTE &)map,(BYTE &)dir,(short &)level);

			if(result >= 0)
			{
				lpObj->MapNumber = map;
				lpObj->X = x;
				lpObj->Y = y;
			}
		}
		else
		{
			lpObj->MapNumber = MAP_INDEX_LORENCIA;
			MapC[lpObj->MapNumber].GetLevelPos(lpObj->Level, lpObj->X, lpObj->Y);
		}
	}
	// Moving from Devil Square to Noria
	if ( DS_MAP_RANGE(lpObj->MapNumber) != FALSE )
	{
		lpObj->MapNumber = MAP_INDEX_NORIA;
		MapC[lpObj->MapNumber].GetMapPos(lpObj->MapNumber, lpObj->X, lpObj->Y);
	}
	// Moving from Blood Castle to Devias
	if ( gUser.BloodCastleMapRange(lpObj->MapNumber) == true )
	{
		lpObj->MapNumber = MAP_INDEX_DEVIAS;
		MapC[lpObj->MapNumber].GetMapPos(lpObj->MapNumber, lpObj->X, lpObj->Y);
	}
	// Moving from Chaos Castle to Devias
	if ( gUser.ChaosCastleMapRange(lpObj->MapNumber) == true )
	{
		lpObj->MapNumber = MAP_INDEX_DEVIAS;
		MapC[lpObj->MapNumber].GetMapPos(lpObj->MapNumber, lpObj->X, lpObj->Y);
	}
	// Moving from Double Goer to Elbeland
	if ( DG_MAP_RANGE(lpObj->MapNumber) == true )
	{
		int mgt = 267;
		short x,y,level;
		BYTE map = lpObj->MapNumber,dir;

		int result = GateC.GetGate(mgt,(BYTE &)x,(BYTE &)y,(BYTE &)map,(BYTE &)dir,(short &)level);

		if(result >= 0)
		{
			lpObj->MapNumber = map;
			lpObj->X = x;
			lpObj->Y = y;
		}
	}
	// Moving from Imperial Fort to Devias
	if ( IF_MAP_RANGE(lpObj->MapNumber) == true )
	{
		lpObj->MapNumber = MAP_INDEX_DEVIAS;
		MapC[lpObj->MapNumber].GetMapPos(lpObj->MapNumber, lpObj->X, lpObj->Y);
	}

	if ( lpObj->MapNumber == 58 )
	{
		lpObj->MapNumber = MAP_INDEX_RAKLION;
		lpObj->X = 180;
		lpObj->Y = 25;
	}

	lpObj->m_cBloodCastleIndex = -1;
	lpObj->m_cBloodCastleSubIndex = -1;
	lpObj->m_iBloodCastleEXP = 0;
	lpObj->m_bBloodCastleComplete = 0;

	lpObj->m_cChaosCastleIndex = -1;
	lpObj->m_cChaosCastleSubIndex = -1;
	lpObj->m_iChaosCastleBlowTime = 0;
	lpObj->m_cKillUserCount = 0;
	lpObj->m_cKillMonsterCount = 0;

#if(!GS_CASTLE)
	// Moving from Illusion Temple to Elbeland
	if ( IT_MAP_RANGE(lpObj->MapNumber) != FALSE )
	{
		lpObj->MapNumber = MAP_INDEX_ELBELAND;
		MapC[lpObj->MapNumber].GetMapPos(lpObj->MapNumber, lpObj->X, lpObj->Y);
	} 
#endif
	// Moving from Vulcan Room to Vulcan Map
	if ( lpObj->MapNumber == MAP_INDEX_VULCANROOM )
	{
		lpObj->MapNumber = MAP_INDEX_VULCAN;
		MapC[lpObj->MapNumber].GetMapPos(lpObj->MapNumber, lpObj->X, lpObj->Y);
	}

#if GS_CASTLE == 1
	if ( lpObj->MapNumber == MAP_INDEX_CASTLESIEGE && g_CastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE )
	{
		short sX;
		short sY;
		short sLVL;
		unsigned char btMAPNUM = lpObj->MapNumber;
		unsigned char btDIR;

		int iMapNumber = GateC.GetGate(100,(unsigned char &)sX,(unsigned char &)sY,(BYTE &)btMAPNUM,(BYTE &)btDIR,(short &)sLVL);

		if(iMapNumber >= 0)
		{
			lpObj->MapNumber = btMAPNUM;
			lpObj->X = sX;
			lpObj->Y = sY;

			lpObj->m_sDestMapNumber = btMAPNUM;
			lpObj->m_btDestX = sX;
			lpObj->m_btDestY = sY;
		}
	}

	if ( lpObj->MapNumber == MAP_INDEX_CASTLEHUNTZONE )
	{
		short sX;
		short sY;
		short sLVL;
		unsigned char btMAPNUM = lpObj->MapNumber;
		unsigned char btDIR;

		int iMapNumber = GateC.GetGate(106,(unsigned char &)sX,(unsigned char &)sY,(BYTE &)btMAPNUM,(BYTE &)btDIR,(short &)sLVL);

		if(iMapNumber >= 0)
		{
			lpObj->MapNumber = btMAPNUM;
			lpObj->X = sX;
			lpObj->Y = sY;

			lpObj->m_sDestMapNumber = btMAPNUM;
			lpObj->m_btDestX = sX;
			lpObj->m_btDestY = sY;
		}
	}

#else

	if ( lpObj->MapNumber == MAP_INDEX_KANTURU_BOSS )
	{
		short sX;
		short sY;
		short sLVL;
		unsigned char btMAPNUM = lpObj->MapNumber;
		unsigned char btDIR;

		int iMapNumber = GateC.GetGate(137,(BYTE &)sX,(BYTE &)sY,(BYTE &)btMAPNUM,(BYTE &)btDIR,(short &)sLVL);

		if(iMapNumber >= 0)
		{
			lpObj->MapNumber = btMAPNUM;
			lpObj->X = sX;
			lpObj->Y = sY;
		}
	}
#endif

	if ( lpObj->MapNumber == MAP_INDEX_BARRACK_BALGASS || lpObj->MapNumber == MAP_INDEX_BALGASS_REFUGE )
	{
		BYTE sX;
		BYTE sY;
		short sLevel;
		int attr = 258;
		unsigned char mapNumber = lpObj->MapNumber;
		unsigned char btDir;

		int iMapNumber = GateC.GetGate(attr, sX, sY, mapNumber, btDir, sLevel);

		if( iMapNumber >= 0 )
		{
			lpObj->MapNumber = mapNumber;
			lpObj->X = sX;
			lpObj->Y = sY;
		}
	}

	if ( lpObj->m_bMapSvrMoveReq == true )
	{
		short sSVR_CODE = ::g_MapServerManager.CheckMoveMapSvr(lpObj->m_Index, lpObj->m_sDestMapNumber, lpObj->m_sPrevMapSvrCode);

		if ( sSVR_CODE == gGameServerCode )
		{
			lpObj->MapNumber = (BYTE)lpObj->m_sDestMapNumber;
			lpObj->X = lpObj->m_btDestX;
			lpObj->Y = lpObj->m_btDestY;
			lpObj->StartX = (BYTE)lpObj->X;
			lpObj->StartY = (BYTE)lpObj->Y;
		}
		else	
		{
			CLog.LogAddC(TColor.Red(), "[MapServerMng] Dest Map Server doesn't have Map [%s][%s] : %d", lpObj->AccountID, lpObj->Name, lpObj->m_sDestMapNumber);
			gObjDel(lpObj->m_Index);
			return FALSE;
		}
	}

	BYTE attr = MapC[lpObj->MapNumber].GetAttr(lpObj->X, lpObj->Y);
	int result;
	BYTE x;
	BYTE y;
	BYTE mapNumber;
	BYTE dir;
	short level;
	int gt;
	int userlevel;

	if ( (attr&4) == 4 || (attr&8) == 8 )
	{
		CLog.LogAdd(lMsg.Get(MSGGET(1, 243)));
		MapC[lpObj->MapNumber].GetMapPos(lpObj->MapNumber, lpObj->X, lpObj->Y);
	}

	mapNumber = MAP_INDEX_LORENCIA;
	userlevel = gObj[aIndex].Level;
	x = lpObj->X;
	y = lpObj->Y;
	dir = lpObj->Dir;
	level = lpObj->Level;
	mapNumber = lpObj->MapNumber;
	gt = 0;

	if( g_Kanturu.BlockKanturuMapEnter == 1)
	{
		if(gObj[aIndex].MapNumber >= MAP_INDEX_KANTURU1 && gObj[aIndex].MapNumber <= MAP_INDEX_KANTURU_BOSS)
		{
			CLog.LogAdd("[ KANTURU ][ Forced Move From Kanturu to Lorencia ] (%s)(%s) (BeforeMap:%d(%d/%d))", gObj[aIndex].AccountID,gObj[aIndex].Name,gObj[aIndex].MapNumber,gObj[aIndex].X,gObj[aIndex].Y);
			gObj[aIndex].MapNumber = MAP_INDEX_LORENCIA;
			MapC[lpObj->MapNumber].GetMapPos(lpObj->MapNumber,(short &)lpObj->X,(short &)lpObj->Y);
		}
	}

	if(g_iBlockCastleSiegeMapEnter == 1)
	{
		if(gObj[aIndex].MapNumber == MAP_INDEX_CASTLESIEGE || gObj[aIndex].MapNumber == MAP_INDEX_CASTLEHUNTZONE || gObj[aIndex].MapNumber == MAP_INDEX_CRYWOLF_FIRSTZONE || gObj[aIndex].MapNumber == MAP_INDEX_CRYWOLF_SECONDZONE)
		{
			CLog.LogAdd("[ KANTURU ][ Forced Move From CastleSiegeMap to Lorencia ] (%s)(%s) (BeforeMap:%d(%d/%d))", gObj[aIndex].AccountID,gObj[aIndex].Name,gObj[aIndex].MapNumber,gObj[aIndex].X,gObj[aIndex].Y);
			gObj[aIndex].MapNumber = MAP_INDEX_LORENCIA;
			MapC[lpObj->MapNumber].GetMapPos(lpObj->MapNumber,(short &)lpObj->X,(short &)lpObj->Y);
		}
	}

	if ( lpObj->MapNumber == MAP_INDEX_DEVIAS )
	{
		gt = 22;
	}
	else if ( lpObj->MapNumber == MAP_INDEX_NORIA )
	{
		gt = 27;
	}
	else if ( lpObj->MapNumber == MAP_INDEX_LOSTTOWER )
	{
		gt = 42;
	}

	if ( gt > 0 )
	{
		result = GateC.GetGate(gt, x, y, mapNumber, dir, level);

		if ( result >= 0 )
		{
			if ( lpObj->Class == CLASS_DARKLORD || lpObj->Class == CLASS_MAGUMSA || lpObj->Class == CLASS_FIGHTER )
			{
				if ( level > 0 )
				{
					level = level / 3 * 2;
				}
			}

			if ( level != 0 && lpObj->Level < level)
			{
				if ( lpObj->Class == CLASS_ELF )
				{
					lpObj->MapNumber = MAP_INDEX_NORIA;
					MapC[lpObj->MapNumber].GetMapPos(lpObj->MapNumber, lpObj->X, lpObj->Y);
				}
				else if( lpObj->Class = CLASS_SUMMONER )
				{
					lpObj->MapNumber = MAP_INDEX_ELBELAND;
					MapC[lpObj->MapNumber].GetMapPos(lpObj->MapNumber, lpObj->X, lpObj->Y);
				}
				else
				{
					lpObj->MapNumber = MAP_INDEX_LORENCIA;
					MapC[lpObj->MapNumber].GetLevelPos(lpObj->Level, lpObj->X, lpObj->Y);
				}
			}
		}
		else
		{
			CLog.LogAdd("[gObjSetCharacter]: Gate Error -> Result: %d, File: %s, Line: %d)", result, __FILE__, __LINE__);
		}
	}

	lpMsg->MapX = (BYTE)lpObj->X;
	lpMsg->MapY = (BYTE)lpObj->Y;
	lpObj->TX = lpMsg->MapX;
	lpObj->TY = lpMsg->MapY;
	lpObj->m_OldX = lpObj->X;
	lpObj->m_OldY = lpObj->Y;
	lpObj->Experience = lpMsg->Exp;
	lpObj->Strength = lpMsg->Str ;
	lpObj->Dexterity = lpMsg->Dex ;
	lpObj->Vitality = lpMsg->Vit ;
	lpObj->Energy = lpMsg->Energy ;
	lpObj->Life = lpMsg->Life;
	lpObj->Life /= 10.0f;
	lpObj->MaxLife = lpMsg->MaxLife;
	lpObj->MaxLife /= 10.0f;
	lpObj->Mana = lpMsg->Mana;
	lpObj->Mana /= 10.0f;
	lpObj->MaxMana = lpMsg->MaxMana;
	lpObj->MaxMana /= 10.0f;
	lpObj->Money = lpMsg->Money;
	lpObj->Leadership = lpMsg->Leadership;
	lpObj->ChatLimitTime = lpMsg->ChatLitmitTime;
	lpObj->iFruitPoint = lpMsg->iFruitPoint;
	lpObj->MLExperience = lpMsg->MasterExp;
	lpObj->MasterLevel = lpMsg->MasterLevel;
	lpObj->MasterPoints = lpMsg->MasterPoints;
	lpObj->MLNextExp = lpMsg->MasterNextExp;
	lpObj->AddMana = 0;
	lpObj->AddLife = 0;

	if ( lpObj->Mana < 1.0f )
	{
		lpObj->Mana = 1.0f;
	}

	if ( lpObj->Life < 1.0f )
	{
		lpObj->Life = 1.0f;
	}

	if ( lpObj->Experience < 1 )
	{
		if ( lpObj->Level > 1 )
		{
			lpObj->Experience = gLevelExperience[lpObj->Level-1];
		}
	}

	lpObj->MaxLife = gCharInfo.sCharInfo[lpObj->Class].Life + (lpObj->Level - 1) * gCharInfo.sCharInfo[lpObj->Class].LevelLife  + ((lpObj->Vitality - gCharInfo.sCharInfo[lpObj->Class].Vitality ) * gCharInfo.sCharInfo[lpObj->Class].VitalityToLife);
	lpObj->MaxMana = gCharInfo.sCharInfo[lpObj->Class].Mana + (lpObj->Level - 1) * gCharInfo.sCharInfo[lpObj->Class].LevelMana  + ((lpObj->Energy - gCharInfo.sCharInfo[lpObj->Class].Energy ) * gCharInfo.sCharInfo[lpObj->Class].EnergyToMana);

	lpObj->VitalityToLife = gCharInfo.sCharInfo[lpObj->Class].VitalityToLife;
	lpObj->EnergyToMana = gCharInfo.sCharInfo[lpObj->Class].EnergyToMana;

	::gObjSetBP(aIndex);
	lpObj->BP = lpObj->MaxBP / 2;
	::gObjCalcMaxLifePower(aIndex);
	lpObj->m_PK_Count = lpMsg->PkCount;
	lpObj->m_PK_Level = lpMsg->PkLevel;
	lpObj->m_PK_Time  = lpMsg->PkTime;

	if ( lpObj->m_PK_Level <= 3 )
	{
		lpObj->m_PK_Level = 2;
	}

	if ( lpObj->m_PK_Level == 3 )
	{
		lpObj->m_PK_Count = 0;
		lpObj->m_PK_Time = 0;
	}

	lpObj->MaxRegenTime = 4000;	// Resurrection of the player if die
	lpObj->m_MoveSpeed = 1000;

	memcpy(lpObj->m_Quest, lpMsg->dbQuest, sizeof(lpObj->m_Quest));
	memcpy(lpObj->NewQuestInfo, lpMsg->NewQuest, sizeof(lpObj->NewQuestInfo));

	lpObj->QuestRegistered = lpMsg->MercenaryReg;
	lpObj->QuestTutorial = lpMsg->TutorialReg;

	if ( lpObj->m_Quest[0] == 0 )
	{
		memset(lpObj->m_Quest, (BYTE)-1, sizeof(lpObj->m_Quest));
	}

	if ( ::g_QuestInfo.GetQuestState(lpObj, 2) == 2 )	// Ring of Honor
	{
		if ( lpObj->Level < QUEST_MINLEVEL_PLUSSTAT )	// Repair Bug of Marlon Before
		{
			::g_QuestInfo.ReSetQuestState(lpObj, 2);
			CLog.LogAdd("[%s][%s] Find Invalid QuestInfo (%d)", lpObj->AccountID, lpObj->Name, 2);
		}
		else
		{
			lpObj->PlusStatQuestClear = true;
		}
	}

	if ( ::g_QuestInfo.GetQuestState(lpObj, 3) == 2 )	// Dark Stone
	{
		if ( lpObj->Level < QUEST_MINLEVEL_PLUSSTAT )	// Repair Bug of Marlon After
		{
			::g_QuestInfo.ReSetQuestState(lpObj, 3);
			CLog.LogAdd("[%s][%s] Find Invalid QuestInfo (%d)", lpObj->AccountID, lpObj->Name, 3);
		}
		else
		{
			lpObj->ComboSkillquestClear = true;
		}
	}
	// 3� Quest Load
	if(lpObj->m_Quest[30] == 0xFF)
		lpObj->m_Quest[30] = 0;
	
	if(lpObj->m_Quest[31] == 0xFF)
		lpObj->m_Quest[31] = 0;
	
	if(lpObj->m_Quest[32] == 0xFF)
		lpObj->m_Quest[32] = 0;
	
	if(lpObj->m_Quest[33] == 0xFF)
		lpObj->m_Quest[33] = 0;
	
	if(lpObj->m_Quest[34] == 0xFF)
		lpObj->m_Quest[34] = 0;

	::gObjSetInventory1Pointer(lpObj);
	lpObj->GuildNumber = 0;
	lpObj->lpGuild = NULL;

	if ( lpObj->Class == CLASS_WIZARD  || lpObj->Class == CLASS_ELF  || lpObj->Class == CLASS_KNIGHT  || lpObj->Class == CLASS_DARKLORD  || lpObj->Class == CLASS_MAGUMSA || lpObj->Class == CLASS_SUMMONER || lpObj->Class == CLASS_FIGHTER )
	{
		BOOL bEnergyBall = TRUE; // Dark Wizard First Skill
		BOOL bForceSkill = TRUE; // Dark Lord First Skill
		BOOL IsGetInitinityArrowSkill = FALSE; // Muse Elf Infinity Arrow

		gObjMagicAddEnergyCheckOnOff(0);

		for ( n=0;n<MAX_MAGIC;n++)
		{
			itype = (lpMsg->dbMagicList[(n*3)+1] + (lpMsg->dbMagicList[n*3]*256));
			
			if ( itype != (BYTE)-1 && itype != 0xFFFF)
			{
				if ( itype == 17 )
				{
					bEnergyBall = FALSE;
				}

				if ( itype == 60 )
				{
					bForceSkill = FALSE;
				}

				if ( itype == 77 )
				{
					IsGetInitinityArrowSkill = 1;
					gObjMagicAdd(lpObj,itype,lpMsg->dbMagicList[n*3+DBM_LEVEL]&0xF);
				}

				// Weapons & Shield Skills - Don't have to be added
				if ( itype != 18 && itype != 19 && itype != 20 && itype != 21 && itype != 22 && itype != 23 && itype != 24 && itype != 25 && itype != 49 && itype != 56 && itype != 62 && itype != 66 && itype != 76 && itype != 223 && itype != 224 && itype != 225 && itype != 54 && itype != 260 && itype != 261 && itype != 270 )
				{
					if ( gSkillTree.IsMasterMagic(itype) == false )
					{
						gObjMagicAdd(lpObj, itype, lpMsg->dbMagicList[n*3+DBM_LEVEL] & 0x0F);
					}
					else
					{
						gSkillTree.MasterMagicAdd(lpObj,itype);
					}
				}
			}
		}

		if ( lpObj->Class == CLASS_WIZARD )	// Dark Wizard
		{
			if ( bEnergyBall == TRUE )
			{
				gObjMagicAdd(lpObj, 17, 0);
				bEnergyBall = FALSE;
			}
		}
		else if ( lpObj->Class == CLASS_DARKLORD ) // Dark Lord
		{
			if ( bForceSkill == TRUE )
			{
				gObjMagicAdd(lpObj, 60, 0);
				bForceSkill = FALSE;
			}
		}

		if ( IsGetInitinityArrowSkill == 0 && lpObj->Class == CLASS_ELF) // Elf
		{
			if(lpObj->Level >= g_SkillAdditionInfo.GetInfinityArrowUseLevel())
			{
				if(lpObj->ChangeUP == 1)
				{
					int iAddSkillPosition = gObjMagicAdd(lpObj,77,0);

					if(iAddSkillPosition > 0)
					{
						CLog.LogAdd("[%s][%s] Add Infinity Arrow Skill (Character Level : %d)(ChangeUp: %d)", lpObj->AccountID,lpObj->Name,lpObj->Level,lpObj->ChangeUP);
					}
				}
			}
		}
		// ---------------------------
		// Castle Siege Skills
		// ---------------------------
		switch ( lpObj->Class )
		{
		case CLASS_WIZARD:
			gObjMagicAdd(lpObj, 45, 0); // Mana Glaive
			break;
		case CLASS_KNIGHT:
			gObjMagicAdd(lpObj, 44, 0); // Crescent Moon Slash
			break;
		case CLASS_ELF:
			gObjMagicAdd(lpObj, 46, 0); // Starfall
			break;
		case CLASS_MAGUMSA:
			gObjMagicAdd(lpObj, 57, 0); // Spiral Slash
			gObjMagicAdd(lpObj, 73, 0); // Mana Rays
			break;
		case CLASS_DARKLORD:
			gObjMagicAdd(lpObj, 74, 0);	// Fire Blast
			break;
		case CLASS_SUMMONER:
			gObjMagicAdd(lpObj, 45, 0); // Mana Glaive
			break;
		case CLASS_FIGHTER:
			gObjMagicAdd(lpObj, 269, 0); // Charge
			break;
		}
		
		gObjMagicAdd(lpObj,67,0); // Stern
		gObjMagicAdd(lpObj,68,0); // Cancel Stern
		gObjMagicAdd(lpObj,69,0); // Swell Mana
		gObjMagicAdd(lpObj,70,0); // Invisibility
		gObjMagicAdd(lpObj,71,0); // Cancel Invisibility
		gObjMagicAdd(lpObj,72,0); // Cancel Magic

#if(!GS_CASTLE) 
		// Illusion Temple Skills
		gObjMagicDel(lpObj,210,0); // Spell of Protection
		gObjMagicDel(lpObj,211,0); // Spell of Restriction
		gObjMagicDel(lpObj,212,0); // Spell of Pursuit
		gObjMagicDel(lpObj,213,0); // Shied-Burn
#endif
		::gObjMagicAddEnergyCheckOnOff(TRUE);
	}

	bool bAllItemExist = true;

	::DbItemSetInByte(lpObj, lpMsg, 16, &bAllItemExist);

	if ( lpObj->Level <=5 )
	{
		for ( int i=MAIN_INVENTORY_SIZE;i<INVENTORY_SIZE;i++)
		{
			BYTE NewOption[MAX_EXOPTION_SIZE];

			if ( lpObj->Inventory1[i].IsItem() == TRUE )
			{
				for ( int j=0;j<MAX_EXOPTION_SIZE;j++)
				{
					NewOption[j] = 0;
				}

				if ( ( lpObj->Inventory1[i].m_NewOption & 0x20 ) != 0 )
				{
					NewOption[0] = TRUE;
				}

				if ( ( lpObj->Inventory1[i].m_NewOption & 0x10 ) != 0 )
				{
					NewOption[1] = TRUE;
				}

				if ( ( lpObj->Inventory1[i].m_NewOption & 0x08 ) != 0 )
				{
					NewOption[2] = TRUE;
				}

				if ( ( lpObj->Inventory1[i].m_NewOption & 0x04 ) != 0 )
				{
					NewOption[3] = TRUE;
				}

				if ( ( lpObj->Inventory1[i].m_NewOption & 0x02 ) != 0 )
				{
					NewOption[4] = TRUE;
				}

				if ( ( lpObj->Inventory1[i].m_NewOption & 0x01 ) != 0 )
				{
					NewOption[5] = TRUE;
				}
			}

			lpObj->Inventory1[i].Clear();
		}

		memset(lpObj->InventoryMap1+64, (BYTE)-1, PSHOP_MAP_SIZE);
	}

	lpObj->Live = TRUE;
	lpObj->Type = OBJ_USER;
	lpObj->TargetNumber = -1;
	lpObj->Connected = PLAYER_PLAYING;
	::gObjMakePreviewCharSet(aIndex);
	CreateFrustrum(lpObj->X, lpObj->Y, aIndex);
	MapC[lpObj->MapNumber].SetStandAttr(lpObj->X, lpObj->Y);
	lpObj->m_OldX = lpObj->X;
	lpObj->m_OldY = lpObj->Y;

	gObjNextExpCal(lpObj);

	if ( lpObj->Life == 0.0f )
	{
		lpObj->Live = TRUE;
		lpObj->m_State = 4;
		lpObj->DieRegen = TRUE;
		lpObj->RegenTime = GetTickCount();
	}
	else
	{
		lpObj->m_State = 1;
	}
	// Check all different Admin types
	if ( (lpObj->Authority & 2) == 2 || (lpObj->Authority & 10) == 10 || (lpObj->Authority & 32) == 32)
	{
	    GCStateInfoSend(lpObj, 1, eVS_ADMIN);
	}

	if ( (lpMsg->CtlCode & 8) == 8 )
	{
		lpObj->Authority = 0x02;
		CLog.LogAddC(2, "[%s][%s] Administrator joined the Server", lpObj->AccountID, lpObj->Name);
		cGMMng.AddGameMaster(lpObj->Name, lpObj->m_Index);
		GCStateInfoSend(lpObj, 1, eVS_ADMIN);
	}
	else
	{
		lpObj->Authority = 1;
	}

	if ((lpMsg->CtlCode & 0x20) == 0x20)
	{
		lpObj->Authority = 0x20;
		CLog.LogAddC(TColor.Red(), "[%s][%s] GameMaster joined the Server.", lpObj->AccountID, lpObj->Name);
		cGMMng.AddGameMaster(lpObj->Name, lpObj->m_Index);
		GCStateInfoSend(lpObj, 1, eVS_GAMEMASTER);
	}

	lpObj->Penalty = 0;

	if ( lpObj->m_cAccountItemBlock != 0 )
	{
		lpObj->Penalty |= 8;
	}

	if ( (lpMsg->CtlCode & 2) == 2 )
	{
		lpObj->Penalty |= 4;
		CLog.LogAdd("Penalty : Item Don't touch  %s %s", lpObj->AccountID, lpObj->Name);
	}

	// ---- Gen System
	lpObj->GenContribution		= lpMsg->GenContribution;
	lpObj->GenFamily			= lpMsg->GenFamily;
	lpObj->GenLevel				= lpMsg->GenLevel;
	lpObj->GenRanking			= lpMsg->GenRanking;
	lpObj->GenReward			= lpMsg->GenReward;
	lpObj->GenNPC				= 0;

	gCalCharacter.gObjCalCharacter(aIndex);

	// ---- Log System
	char CharacterClass[255];

	switch ( lpObj->DbClass )
	{
		case DB_DARK_WIZARD:		sprintf_s(CharacterClass,"Dark Wizard");		break;
		case DB_SOUL_MASTER:		sprintf_s(CharacterClass,"Soul Master");		break;
		case DB_GRAND_MASTER:		sprintf_s(CharacterClass,"Grand Master");		break;
		case DB_DARK_KNIGHT:		sprintf_s(CharacterClass,"Dark Knight");		break;
		case DB_BLADE_KNIGHT:		sprintf_s(CharacterClass,"Blade Knight");		break;
		case DB_BLADE_MASTER:		sprintf_s(CharacterClass,"Blade Master");		break;
		case DB_FAIRY_ELF:			sprintf_s(CharacterClass,"Fairy Elf");			break;
		case DB_MUSE_ELF:			sprintf_s(CharacterClass,"Muse Elf");			break;
		case DB_HIGH_ELF:			sprintf_s(CharacterClass,"High Elf");			break;
		case DB_MAGIC_GLADIATOR:	sprintf_s(CharacterClass,"Magic Gladiator");	break;
		case DB_DUEL_MASTER:		sprintf_s(CharacterClass,"Duel Master");		break;
		case DB_DARK_LORD:			sprintf_s(CharacterClass,"Dark Lord");			break;
		case DB_LORD_EMPEROR:		sprintf_s(CharacterClass,"Lord Emperor");		break;
		case DB_SUMMONER:			sprintf_s(CharacterClass,"Summoner");			break;
		case DB_BLOODY_SUMMONER:	sprintf_s(CharacterClass,"Bloody Summoner");	break;
		case DB_DIMENSION_MASTER:	sprintf_s(CharacterClass,"Dimension Master");	break;
		case DB_RAGE_FIGHTER:		sprintf_s(CharacterClass,"Rage Fighter");		break;
		case DB_FIST_MASTER:		sprintf_s(CharacterClass,"Fist Master");		break;
	}

	::gObjAuthorityCodeSet(lpObj);
	CLog.LogAdd("[Index: %d][AccountID: %s][Character: %s] Logged in Successfully!", lpObj->m_Index, lpObj->AccountID, lpObj->Name);
	CLog.LogAdd("[Character: %s] Entered Map [%d] Coords [X: %d][Y: %d]", lpObj->Name, lpObj->MapNumber, lpObj->X, lpObj->Y);
	CLog.LogAdd("[Character: %s] [Class: %d (%s)][Level: %d][Exp: %d][NextExp: %d]", lpObj->Name, lpObj->DbClass, CharacterClass, lpObj->Level, lpObj->Experience, lpObj->NextExp);
	
	// Need to be continued! More Details of character joining!

	BYTE weather = MapC[lpObj->MapNumber].GetWeather();

	if ( (weather >> 4) > 0 )
	{
		CGWeatherSend(aIndex, weather);
	}
	
#if GS_CASTLE == 1
	if ( lpObj->MapNumber == MAP_INDEX_CASTLESIEGE )
	{
		GCAnsCsNotifyStart(lpObj->m_Index, CHECK_CLASS(g_CastleSiege.GetCastleState(), CASTLESIEGE_STATE_STARTSIEGE));

		if ( g_CastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE ) 
			g_CastleSiege.NotifyCsSelfLeftTime(lpObj->m_Index);
	}
#endif

	::GCCheckMainExeKeySend(aIndex);

	if ( m_ObjBill[aIndex].GetCertify() >= 0 && m_ObjBill[aIndex].GetCertify() < 3 )
	{
		gLCount[m_ObjBill[aIndex].GetCertify()].Add();
	}

	if ( bAllItemExist == false )
	{
		//PMSG_NOTICE pNotice;
		//TNotice::MakeNoticeMsgEx((TNotice *)&pNotice, 1, "없는 아이템 발견 !!!");	// Deathway need Translation
		//gSendProto.DataSend(lpObj->m_Index, (UCHAR *)&pNotice, pNotice.h.size);
	}

	if ( lpObj->Level <= g_iCharacterRecuperationMaxLevel )
	{
		lpObj->m_iAutoRecuperationTime = GetTickCount();
	}
	else
	{
		lpObj->m_iAutoRecuperationTime = 0;
	}

	lpObj->dwShieldAutoRefillTimer = GetTickCount();

	CLog.LogAdd("[ShieldSystem][CalcSDPoint] [%s][%s] user SD Gage : %d", lpObj->AccountID,lpObj->Name,lpObj->iMaxShield+lpObj->iAddShield);

	g_CashShop.AddUser(lpObj);
	g_CashShop.CGCashPoint(lpObj);
	g_CashItemPeriodSystem.GDReqPeriodItemList(lpObj);

	return TRUE;
}

// --------------------------------------------------
// [Sunday, 20 Feb, 2011] WolF ~ Can Touch Item
// --------------------------------------------------
bool gObjCanItemTouch(LPOBJ lpObj, int Type )
{
	if( (lpObj->Penalty & 4) == 4 ) return false;
	// ------
	if( (lpObj->Penalty & 8) == 8 && Type != 6 && Type != 8) return false;
	// ------
	return true;
}

void ItemIsBufExOption(BYTE * buf, CItem * lpItem)
{
	for ( int i=0;i<MAX_EXOPTION_SIZE;i++ )
	{
		buf[i] = 0;
	}

	if ( lpItem->IsExtLifeAdd() != FALSE )
	{
		buf[0] = TRUE;
	}

	if ( lpItem->IsExtManaAdd() != FALSE )
	{
		buf[1] = TRUE;
	}

	if ( lpItem->IsExtDamageMinus() != FALSE )
	{
		buf[2] = TRUE;
	}

	if ( lpItem->IsExtDamageReflect() != FALSE )
	{
		buf[3] = TRUE;
	}

	if ( lpItem->IsExtDefenseSuccessfull() != FALSE )
	{
		buf[4] = TRUE;
	}

	if ( lpItem->IsExtMonsterMoney() != FALSE )
	{
		buf[5] = TRUE;
	}

}

void gObjItemTextSave(LPOBJ lpObj)
{
	//CLog.LogAdd(lMsg.Get(MSGGET(1, 249)), lpObj->AccountID, lpObj->Name, lpObj->Money);
}

void gObjWarehouseTextSave(LPOBJ lpObj)
{
	//CLog.LogAdd(lMsg.Get(MSGGET(1, 251)), lpObj->AccountID, lpObj->Name, lpObj->WarehouseMoney);
}

// --------------------------------------------------
// [Sunday, 20 Feb, 2011] WolF ~ Set Authority Code
// --------------------------------------------------
void gObjAuthorityCodeSet(LPOBJ lpObj)
{
	if( (lpObj->Authority&1) == 1 ) return;
	// -----
	if( (lpObj->Authority&2) == 2 )
	{
		lpObj->AuthorityCode = -1;
		return;
	}
	// -----
	if( (lpObj->Authority&4) == 4 ) return;
	// -----
	if( (lpObj->Authority&8) == 8 )
	{
		lpObj->AuthorityCode |= 1;
		lpObj->AuthorityCode |= 2;
		lpObj->AuthorityCode |= 4;
		lpObj->AuthorityCode |= 8;
		lpObj->AuthorityCode |= 16;
		lpObj->AuthorityCode |= 32;
		return;
	}
	// -----
	if( (lpObj->Authority&16) == 16 ) return;
	// -----
	if( (lpObj->Authority&32) == 32 )
	{
		lpObj->AuthorityCode |= 1;
		lpObj->AuthorityCode |= 2;
		lpObj->AuthorityCode |= 4;
		lpObj->AuthorityCode |= 8;
		lpObj->AuthorityCode |= 16;
		lpObj->AuthorityCode |= 32;
		return;
	}
}

bool gObjSetPosMonster(int aIndex, int PosTableNum)
{
	if (((aIndex<0)? false: (aIndex > OBJMAX-1)? false: true ) == false )
	{
		CLog.LogAdd("[gObjSetPosMonster]: Index Error %d", aIndex);
		return false;
	}

	
	if ( ((PosTableNum<0)? false: (PosTableNum > OBJ_MAXMONSTER-1)? false: true ) == false )
	{
		CLog.LogAdd("[gObjSetPosMonster]: Monster Index Error %d", PosTableNum);
		return false;
	}

	LPOBJ lpObj = &gObj[aIndex];
	lpObj->m_PosNum = PosTableNum;
	lpObj->X = gMSetBase.m_Mp[PosTableNum].m_X;
	lpObj->Y = gMSetBase.m_Mp[PosTableNum].m_Y;
	lpObj->MapNumber = gMSetBase.m_Mp[PosTableNum].m_MapNumber;
	lpObj->TX = lpObj->X;
	lpObj->TY = lpObj->Y;
	lpObj->m_OldX = lpObj->X;
	lpObj->m_OldY = lpObj->Y;
	lpObj->Dir = gMSetBase.m_Mp[PosTableNum].m_Dir;
	lpObj->StartX = lpObj->X;
	lpObj->StartY = lpObj->Y;

	if ( lpObj->Class == 44 ||  lpObj->Class == 53 ||  lpObj->Class == 54 ||  lpObj->Class == 55 ||  lpObj->Class == 56)
	{

	}
	else if ( (lpObj->Class >= 78 && lpObj->Class <= 83) || (lpObj->Class >= 493 && lpObj->Class <= 502) )
	{

	}
	else if ( gMSetBase.GetPosition(PosTableNum, lpObj->MapNumber, lpObj->X, lpObj->Y) == FALSE )
	{
		CLog.LogAdd("Error: Monster location setting error %s %d", __FILE__, __LINE__);
		CLog.LogAdd("[!URGENT REPORT TO HaRiBO-GS!] /Map:%d /X:%d /Y:%d /Index:%d /SetPos:%d",lpObj->MapNumber,lpObj->X,lpObj->Y,lpObj->m_Index,gMSetBase.m_Mp[PosTableNum].m_Type);
		return false;
	}

	lpObj->TX = lpObj->X;
	lpObj->TY = lpObj->Y;
	lpObj->m_OldX = lpObj->X;
	lpObj->m_OldY = lpObj->Y;
	lpObj->Dir = gMSetBase.m_Mp[PosTableNum].m_Dir;
	lpObj->StartX = lpObj->X;
	lpObj->StartY = lpObj->Y;

	return true;
}

BOOL gObjSetMonster(int aIndex, int MonsterClass)
{
	int Level;
	LPOBJ lpObj;

	if ( OBJMAX_RANGE(aIndex) == FALSE )
	{
		CLog.LogAdd("error : %s %d", __FILE__, __LINE__);
		return false;
	}

	lpObj = &gObj[aIndex];
	lpObj->ConnectCheckTime = GetTickCount();
	lpObj->ShopNumber = -1;
	lpObj->TargetNumber = -1;
	lpObj->m_RecallMon = -1;

	switch ( MonsterClass )
	{
		case 200:
			gBSGround[0]->m_BallIndex = aIndex;
			break;

		case NPC_HANZO_THE_BLACKSMITH:
			lpObj->ShopNumber = 0;
			break;

		case NPC_LUMEN_THE_BARMAID:
			lpObj->ShopNumber = 1;
			break;

		case NPC_PASI_THE_MAGE:
			lpObj->ShopNumber = 2;
			break;

		case NPC_MARTIN_MERCHANT:
			lpObj->ShopNumber = 3;
			break;

		case NPC_WANDERING_MERCHANT:
			lpObj->ShopNumber = 4;
			break;

		case NPC_POTION_GIRL:
			lpObj->ShopNumber = 5;
			break;

		case NPC_LIAM_THE_BARMAID:
			lpObj->ShopNumber = 6;
			break;

		case NPC_WIZARD:
			lpObj->ShopNumber = 7;
			break;

		case NPC_ZIENNA_WEAPON_MERCHANT:
			lpObj->ShopNumber = 8;
			break;

		case NPC_EO_THE_CRAFTSMAN:
			lpObj->ShopNumber = 9;
			break;

		case NPC_ELF_LALA:
			lpObj->ShopNumber = 10;
			break;

		case NPC_ALEX:
			lpObj->ShopNumber = 11;
			break;

		case NPC_THOMPSON_KENEL:
			lpObj->ShopNumber = 12;
			break;

		case NPC_LEO_THE_HELPER:
			lpObj->ShopNumber = 13;
			break;

		case NPC_PAMELA_THE_SUPPLIER:
			lpObj->ShopNumber = 14;
			break;

		case NPC_ANGELA_THE_SUPPLIER:
			lpObj->ShopNumber = 15;
			break;

		case NPC_NATASHA_FIREWORKS:
			lpObj->ShopNumber = 16;
			break;

		case NPC_HELPER_ELLEN:
			lpObj->ShopNumber = 17;
			break;

		case NPC_SILVIA:
			lpObj->ShopNumber = 18;
			break;

		case NPC_LEAH:
			lpObj->ShopNumber = 19;
			break;

		case NPC_MARSEILLE:
			lpObj->ShopNumber = 20;
			break;

		case NPC_ARENA_GUARD:
			lpObj->ShopNumber = 21;
			break;

		case NPC_ORACLE_LAYLA:
			lpObj->ShopNumber = 22;
			break;

		case NPC_CHRISTINE:
			lpObj->ShopNumber = 23;
			break;

		case NPC_RAUL:
			lpObj->ShopNumber = 24;
			break;

		case NPC_REINA_HABERDASHER:
			lpObj->ShopNumber = 25;
			break;

		case NPC_MERCENARY:
			lpObj->ShopNumber = 26;
			break;

		case NPC_DAVID:
			lpObj->ShopNumber = 27;
			break;

		case NPC_BAZ_THE_VAULT_KEEPER:
			lpObj->ShopNumber = 100;
			break;

		case NPC_GOLDEN_ARCHER:
			lpObj->ShopNumber = 102;
			break;

		case NPC_CHAOS_GOBLIN:
		case NPC_ELPIS:
		case NPC_OSBORNE:
		case NPC_JERRIDON:
			lpObj->ShopNumber = 101;
			break;
	}

	lpObj->Connected = PLAYER_PLAYING;
	lpObj->Live = FALSE;
	lpObj->m_State = 0;

	if ( MonsterClass != 76 )	// -- Dark Phoenix Shield
	{
		lpObj->Live = TRUE;
		lpObj->m_State = 1;
	}

	if ( (MonsterClass >= 204 && MonsterClass <= 259)
		|| (MonsterClass >= 368 && MonsterClass <= 385)
#if(!GS_CASTLE)
		|| MonsterClass == 367
#endif
		|| MonsterClass == 375
		|| (MonsterClass >= 406 && MonsterClass <= 408)
		|| (MonsterClass >= 415 && MonsterClass <= 417)
		|| (MonsterClass >= 450 && MonsterClass <= 453)
		|| MonsterClass == 464	// Initial Helper
		|| MonsterClass == 465	// Santa Claus
		|| MonsterClass == 478	// Delgado
		|| MonsterClass == 479	// Duel Arena Titus
		|| (MonsterClass >= 467 && MonsterClass <= 475)	// Snowman And Little Santas
		|| MonsterClass == 479	// Duel Arena
		|| MonsterClass == 492	// Moss Merchant
		|| MonsterClass == 522	// Imperial Fort NPC
		|| MonsterClass == 540	// Double Goer Npc
		|| MonsterClass == 541	// Silver Treasure Box
		|| MonsterClass == 542	// Golden Treasure Box
		|| MonsterClass == 543
		|| MonsterClass == 544
		|| MonsterClass == 545
		|| MonsterClass == 546
		|| MonsterClass == 547
		|| MonsterClass == 566
		|| MonsterClass == 567
		|| MonsterClass == 568
		|| MonsterClass == 577
		|| MonsterClass == 578)
	{
		lpObj->Type = OBJ_NPC;
	}
	else
	{
		lpObj->Type = OBJ_MONSTER;
	}

	if ( MonsterClass == 77 )
	{
		int iSL = gObjAddMonster(MAP_INDEX_ICARUS);

		if ( iSL >= 0 )
		{
			if ( gObjSetMonster(iSL, 76) == FALSE )
			{
				CLog.MsgBox("천공보스몹 설정 실패");
				return false;
			}

			lpObj->m_SkyBossMonSheildLinkIndex = iSL;
			CLog.LogAdd("SkyLand Boss Monster Setting!!");
		}
		else
		{
			CLog.MsgBox("천공보스몹 설정 실패");
			return false;
		}
	}

	if ( MonsterClass == 76 )
	{
		CLog.LogAdd("SkyLand Boss Shield Setting!!");
	}

	lpObj->Class = MonsterClass;
	Level = lpObj->Level;
	LPMONSTER_ATTRIBUTE lpm = gMAttr.GetAttr(lpObj->Class);

	if ( lpm == NULL )
	{
		CLog.MsgBox(lMsg.Get(MSGGET(1, 253) ), lpObj->Class, __FILE__, __LINE__);
		return false;
	}

	strncpy(gObj[aIndex].Name, lpm->m_Name , 10 );

	if (lpm->m_Level == 0)
	{
		CLog.LogAdd(lMsg.Get( MSGGET(1, 254)), lpObj->Class, __FILE__, __LINE__);
	}

	lpObj->Level = lpm->m_Level;
	lpObj->m_AttackSpeed = lpm->m_AttackSpeed;
	lpObj->m_AttackDamageMin = lpm->m_DamageMin;
	lpObj->m_AttackDamageMax = lpm->m_DamageMax;
	lpObj->m_Defense = lpm->m_Defense;
	lpObj->m_MagicDefense = lpm->m_MagicDefense;
	lpObj->m_AttackRating = lpm->m_AttackRating;
	lpObj->m_SuccessfulBlocking = lpm->m_Successfulblocking;
	lpObj->Life = lpm->m_Hp;
	lpObj->MaxLife = lpm->m_Hp;
	lpObj->Mana = lpm->m_Mp;
	lpObj->MaxMana = lpm->m_Mp;
	lpObj->m_MoveRange = lpm->m_MoveRange;
	lpObj->m_AttackSpeed = lpm->m_AttackSpeed;
	lpObj->m_MoveSpeed = lpm->m_MoveSpeed;
	lpObj->MaxRegenTime = lpm->m_RegenTime * 1000;
	lpObj->m_AttackRange = lpm->m_AttackRange;
	lpObj->m_ViewRange = lpm->m_ViewRange;
	lpObj->m_Attribute = lpm->m_Attribute;
	lpObj->m_AttackType = lpm->m_AttackType;
	lpObj->m_ItemRate = lpm->m_ItemRate;
	lpObj->m_MoneyRate = lpm->m_MoneyRate;
	lpObj->Dexterity = 0;

	lpObj->m_iScriptMaxLife = lpm->m_iScriptHP;
	lpObj->m_iBasicAI = lpm->m_iAINumber;
	lpObj->m_iCurrentAI = lpm->m_iAINumber;
	lpObj->m_iCurrentAIState = 0;
	lpObj->m_iLastAIRunTime = 0;
	lpObj->m_iGroupNumber = 0;
	lpObj->m_iSubGroupNumber = 0;
	lpObj->m_iGroupMemberGuid = -1;
	lpObj->m_iRegenType = 0;

	lpObj->m_Agro.ResetAll();

	lpObj->m_iLastAutomataRuntime = 0;
	lpObj->m_iLastAutomataDelay = 0;

	lpObj->pInventoryCount[0] = 0;
	memcpy(lpObj->m_Resistance, lpm->m_Resistance, MAX_RESISTENCE_TYPE );
	gObjSetInventory1Pointer(lpObj);

	if ( lpObj->m_AttackType != 0 )
	{
		if ( lpObj->m_AttackType >= 100 )
		{
			gObjMonsterMagicAdd(lpObj, lpObj->m_AttackType - 100, 1);
		}
		else if ( lpObj->Class != 534 && lpObj->Class != 535 && lpObj->Class != 536 && lpObj->Class != 537 && lpObj->Class != 538 && lpObj->Class != 539 )
		{
			gObjMonsterMagicAdd(lpObj, lpObj->m_AttackType, 1);
		}

		if ( lpObj->Class == 144 || lpObj->Class == 174 || lpObj->Class == 182 || lpObj->Class == 190 || lpObj->Class == 260 || lpObj->Class == 268 )
		{
			gObjMonsterMagicAdd(lpObj, 17, 1);
		}

		if ( lpObj->Class == 145 || lpObj->Class == 175 || lpObj->Class == 183 || lpObj->Class == 191 || lpObj->Class == 261 || lpObj->Class == 269 || 
			 lpObj->Class == 146 || lpObj->Class == 176 || lpObj->Class == 184 || lpObj->Class == 192 || lpObj->Class == 262 || lpObj->Class == 270 ||
			 lpObj->Class == 147 || lpObj->Class == 177 || lpObj->Class == 185 || lpObj->Class == 193 || lpObj->Class == 263 || lpObj->Class == 271 ||
			 lpObj->Class == 148 || lpObj->Class == 178	|| lpObj->Class == 186 || lpObj->Class == 194 || lpObj->Class == 264 || lpObj->Class == 272 ||
			 lpObj->Class == 160 || lpObj->Class == 180 || lpObj->Class == 188 || lpObj->Class == 196 || lpObj->Class == 266 || lpObj->Class == 274 )
		{
			gObjMonsterMagicAdd(lpObj, 17, 1);
		}

		if ( lpObj->Class == 161 || lpObj->Class == 181 || lpObj->Class == 189 || lpObj->Class == 197 || lpObj->Class == 267 || lpObj->Class == 275 || lpObj->Class == 338 )
		{
			gObjMonsterMagicAdd(lpObj, 200, 1);
			gObjMonsterMagicAdd(lpObj, 1, 1);
			gObjMonsterMagicAdd(lpObj, 201, 1);
			gObjMonsterMagicAdd(lpObj, 202, 1);
			gObjMonsterMagicAdd(lpObj, 55, 1);
		}

		if ( lpObj->Class == 531 )	// Doppleganger Ice Walker
		{
			gObjMonsterMagicAdd(lpObj, 7, 1);	// 0x19 Ice
		}
		else if ( lpObj->Class == 534 )	// Doppleganger Elf
		{
			gObjMonsterMagicAdd(lpObj, 24, 1);	// 0x19 Crosbow
			gObjMonsterMagicAdd(lpObj, 52, 1);	// 0x19 Penetration
			//gObjMonsterMagicAdd(lpObj, 51, 1);
			//gObjMonsterMagicAdd(lpObj, 235, 1);
		}
		else if ( lpObj->Class == 535 )	// Doppleganger Knight
		{
			gObjMonsterMagicAdd(lpObj, 19, 1);	// 0x19 Skill
			gObjMonsterMagicAdd(lpObj, 20, 1);	// 0x19 Skill
			gObjMonsterMagicAdd(lpObj, 21, 1);	// 0x19 Skill
			gObjMonsterMagicAdd(lpObj, 22, 1);	// 0x19 Skill
			gObjMonsterMagicAdd(lpObj, 23, 1);	// 0x19 Skill
			gObjMonsterMagicAdd(lpObj, 43, 1);	// 0x19 Death Stab
			//gObjMonsterMagicAdd(lpObj, 41, 1);	// 0x1E Twisting Slash
			//gObjMonsterMagicAdd(lpObj, 42, 1);	// 0x1E Rageful Blow
			//gObjMonsterMagicAdd(lpObj, 232, 1);	// 0x1E Explotion
		}
		else if ( lpObj->Class == 536 )	// Doppleganger Wizard
		{
			gObjMonsterMagicAdd(lpObj, 1, 1);	// 0x19 Poison
			gObjMonsterMagicAdd(lpObj, 2, 1);	// 0x19 Meteorite
			gObjMonsterMagicAdd(lpObj, 3, 1);	// 0x19 Lightning
			gObjMonsterMagicAdd(lpObj, 4, 1);	// 0x19 Fire Ball
			gObjMonsterMagicAdd(lpObj, 7, 1);	// 0x19 Ice
			//gObjMonsterMagicAdd(lpObj, 10, 1);	// 0x1E Hell Fire
			//gObjMonsterMagicAdd(lpObj, 14, 1);	// 0x1E Inferno
			//gObjMonsterMagicAdd(lpObj, 38, 1);	// 0x1E Decay
			//gObjMonsterMagicAdd(lpObj, 39, 1);	// 0x1E Ice Storm
		}
		else if ( lpObj->Class == 537 )	// Doppleganger Gladiator
		{
			gObjMonsterMagicAdd(lpObj, 55, 1);	// 0x19 Defense Down
			gObjMonsterMagicAdd(lpObj, 56, 1);	// 0x19 & 0x1E �? Power Slash
		}
		else if ( lpObj->Class == 538 )	// Doppleganger Lord
		{
			gObjMonsterMagicAdd(lpObj, 60, 1);	// 0x19 Force
			gObjMonsterMagicAdd(lpObj, 61, 1);	// 0x19 Fire Burst
			//gObjMonsterMagicAdd(lpObj, 62, 1);	// 0x19 & 0x1E Dark Horse
			//gObjMonsterMagicAdd(lpObj, 65, 1);	// 0x1E Electric Spark
		}
		else if ( lpObj->Class == 539 )	// Doppleganger Summoner
		{
			gObjMonsterMagicAdd(lpObj, 219, 1);	// 0x19 Sleep
			gObjMonsterMagicAdd(lpObj, 1, 1);
			gObjMonsterMagicAdd(lpObj, 2, 1);
			gObjMonsterMagicAdd(lpObj, 3, 1);
			//gObjMonsterMagicAdd(lpObj, 216, 1);	// 0x1E Electric Surge
			//gObjMonsterMagicAdd(lpObj, 214, 1);	// 0x1E Drain Life
			//gObjMonsterMagicAdd(lpObj, 215, 1);	// 0x1E Chain Lightning
		}
		else if ( lpObj->Class == 454 )	// Ice Walker
		{
			gObjMonsterMagicAdd(lpObj, 7, 1);
		}
		else if ( lpObj->Class == 457 )	// Coolutin
		{
			gObjMonsterMagicAdd(lpObj, 7, 1);
		}

		if (  lpObj->Class == 149 || lpObj->Class == 179 || lpObj->Class == 187 || lpObj->Class == 195 || lpObj->Class == 265 || lpObj->Class == 273 )
		{
			gObjMonsterMagicAdd(lpObj, 17, 1);
			gObjMonsterMagicAdd(lpObj, 1, 1);
		}

		if ( lpObj->Class == 66 || lpObj->Class == 73 || lpObj->Class == 77 || lpObj->Class == 89 || lpObj->Class == 95 || lpObj->Class == 112 || lpObj->Class == 118 || lpObj->Class == 124 || lpObj->Class == 130 || lpObj->Class == 143 || lpObj->Class == 163 || lpObj->Class == 165 || lpObj->Class == 167 || lpObj->Class == 169 || lpObj->Class == 171 || lpObj->Class == 173 || lpObj->Class == 433 || lpObj->Class == 427 )
		{
			gObjMonsterMagicAdd(lpObj, 17, 1);
		}
		
		if( lpObj->Class == 89 || lpObj->Class == 95 || lpObj->Class == 112 || lpObj->Class == 118 || lpObj->Class == 124 || lpObj->Class == 130 || lpObj->Class == 143 || lpObj->Class == 433 )
		{
			gObjMonsterMagicAdd(lpObj, 3, 1);
		}
	}

	//gObjGiveItemSearch(lpObj, lpm->m_MaxItemLevel);
	//gObjGiveItemWarehouseSearch(lpObj, lpm->m_MaxItemLevel);
	CreateFrustrum(lpObj->X, lpObj->Y, aIndex);
	MapC[lpObj->MapNumber].SetStandAttr(lpObj->X, lpObj->Y);
	lpObj->m_OldX = lpObj->X;
	lpObj->m_OldY = lpObj->Y;
	lpObj->LastAttackerID = -1;
	return true;
}

void gObjDestroy(unsigned int aSocket,int client)
{
	if ( client < 0 || client > OBJMAX-1)
	{
		return;
	}

	gObj[client].Connected = PLAYER_EMPTY;
}

short gObjAddSearch(SOCKET aSocket, char* ip)
{
	int count;
	int totalcount = 0;

	if ( gDisconnect == 1 )
	{
		return -1;
	}

	if ( gObjTotalUser > gServerMaxUser )
	{
		if (acceptIP.IsIp(ip) == 0 )
		{
			GCJoinSocketResult(4, aSocket);
			return -1;
		}
	}

	count = gObjCount;
	
	while ( true )
	{
		if ( gObj[count].Connected == PLAYER_EMPTY )
		{
			return count;
		}

		count++;

		if ( count >= OBJMAX )
		{
			count = OBJ_STARTUSERINDEX;
		}
		totalcount++;
		if ( totalcount >= OBJMAXUSER )
		{
			break;
		}
	}
	return -1;
}

short gObjAdd(SOCKET aSocket, char* ip, int aIndex)
{
	if ( gObj[aIndex].Connected != PLAYER_EMPTY )
	{
		return -1;
	}

	gObjCharZeroSet(aIndex);
	gNSerialCheck[aIndex].init();
	gObj[aIndex].LoginMsgSnd = FALSE;
	gObj[aIndex].LoginMsgCount = 0;
	memset(gObj[aIndex].AccountID, 0, sizeof(gObj[0].AccountID)-1);
	gObj[aIndex].m_Index = aIndex;
	gObj[aIndex].m_socket = aSocket;
	gObj[aIndex].ConnectCheckTime = GetTickCount();
	gObj[aIndex].AutoSaveTime = gObj[aIndex].ConnectCheckTime;
	gObj[aIndex].Connected = PLAYER_CONNECTED;
	gObj[aIndex].CheckSpeedHack = false;
	gObj[aIndex].LoginMsgCount = 0;
	gObj[aIndex].Magumsa = 0;
	gObj[aIndex].Type = OBJ_USER;
	gObj[aIndex].SaveTimeForStatics = GetTickCount() + 3600000;
	m_ObjBill[aIndex].Init();
	strcpy(gObj[aIndex].Ip_addr, ip);
	CLog.LogAdd("connect : [%d][%s]", aIndex, ip);
	gObjCount++;
	
	if ( gObjCount >= OBJMAX )
	{
		gObjCount = OBJ_STARTUSERINDEX;
	}

	return aIndex;
}

short gObjMonsterRecall(int iMapNumber)
{
	int number = gObjAddMonster(iMapNumber);

	if ( number < 0 )
	{
		return -1;
	}
	return -1;
}

short gObjAddMonster(int iMapNumber)
{
	if (g_MapServerManager.CheckMapCanMove(iMapNumber) == FALSE )
	{
		return -1;
	}

	int count;
	int totalcount=0;

	count = gObjMonCount;

	while ( true )
	{
		if ( gObj[count].Connected == PLAYER_EMPTY )
		{
			gObjCharZeroSet(count);
			gObj[count].m_Index = count;
			gObj[count].Connected = PLAYER_CONNECTED;
			gObjMonCount++;

			if ( gObjMonCount >= OBJ_MAXMONSTER )
			{
				gObjMonCount = 0;
			}
			
		return count;

		}

		count ++;
		
		if ( count >= OBJ_MAXMONSTER )
		{
			count = 0;
		}

		totalcount++;

		if ( totalcount >= OBJ_MAXMONSTER )
		{
			CLog.LogAdd( lMsg.Get( MSGGET ( 1, 255 )), __FILE__, __LINE__ );
			return -1;
		}
	}
	return -1;
}

short gObjAddCallMon()
{
	int count;
	int totalcount = 0;

	count = gObjCallMonCount;

	while ( true )
	{
		if ( gObj[count].Connected == PLAYER_EMPTY )
		{
			gObjCharZeroSet(count);
			gObj[count].m_Index = count;
			gObj[count].Connected = PLAYER_CONNECTED;
			gObjCallMonCount++;

			if ( gObjCallMonCount >= OBJ_STARTUSERINDEX )
			{
				gObjCallMonCount = OBJ_MAXMONSTER;

			}
			return count;
		}

		count++;

		if ( count >= OBJ_STARTUSERINDEX )
		{
			count = OBJ_MAXMONSTER;
		}

		totalcount++;

		if ( totalcount >= OBJ_CALLMONCOUNT )
		{
			CLog.LogAdd(lMsg.Get(MSGGET(1, 255)), __FILE__, __LINE__);
			return -1;
		}
	}

	return -1;

}

void gObjUserKill(int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];

	if ( lpObj->CloseCount < 1 )
	{
		lpObj->CloseCount = 6;
		lpObj->CloseType = 0;
	}
}

void gObjTradeSave(LPOBJ lpObj, int index)
{
	if ( lpObj->m_IfState.use != 0 && lpObj->m_IfState.type == 1 )
	{
		int tObjNum = lpObj->TargetNumber;

		if ( tObjNum > 0 )
		{
			char szTemp[256];

			CGTradeResult(tObjNum, 0);
			wsprintf(szTemp, lMsg.Get(MSGGET(4, 77)), lpObj->Name);
			GCServerMsgStringSend((char*)szTemp, tObjNum, 1);
			gObjTradeCancel(tObjNum);
			CLog.LogAdd(lMsg.Get(MSGGET(2, 1)), gObj[tObjNum].AccountID, gObj[tObjNum].Name);
		}

		gObjTradeCancel( index );
		CLog.LogAdd(lMsg.Get(MSGGET(2,2)), lpObj->AccountID, lpObj->Name);
	}
}

void gObjBillRequest(LPOBJ lpObj)
{
	char szMsg[128];

	if ( m_ObjBill[lpObj->m_Index].GetPayCode() == 0 )
	{
		if ( m_ObjBill[lpObj->m_Index].GetCertify() == 0 )
		{
			wsprintf(szMsg, lMsg.Get(MSGGET(4, 85)), m_ObjBill[lpObj->m_Index].GetEndTime());
		}
		else if ( m_ObjBill[lpObj->m_Index].GetCertify() == 1 )
		{
			if ( m_ObjBill[lpObj->m_Index].GetEndTime() != 0 )
			{
				wsprintf(szMsg, lMsg.Get(MSGGET(4, 86)), m_ObjBill[lpObj->m_Index].GetEndTime());
			}
		}
	}
	else if ( m_ObjBill[lpObj->m_Index].GetPayCode() == 1 )
	{
		char szTemp[20];
		szTemp[4] = 0;
		strncpy(szTemp, m_ObjBill[lpObj->m_Index].GetEndsDays(), 4);
		int Day = atoi(szTemp);

		strncpy(szTemp, m_ObjBill[lpObj->m_Index].GetEndsDays()+4, 2);
		szTemp[2] = 0;
		int Month = atoi(szTemp);

		strncpy(szTemp, m_ObjBill[lpObj->m_Index].GetEndsDays()+6, 2);
		szTemp[2] = 0;
		int Year = atoi(szTemp);

		if ( m_ObjBill[lpObj->m_Index].GetCertify() == 0 )	// Account Based
		{
			wsprintf(szMsg, lMsg.Get(MSGGET(4, 87)), Day, Month, Year);
		}
		else if ( m_ObjBill[lpObj->m_Index].GetCertify() == 1 )	// IP Based
		{
			wsprintf(szMsg, lMsg.Get(MSGGET(4, 88)), Day, Month, Year);
		}
	}
	else if ( m_ObjBill[lpObj->m_Index].GetPayCode() == 3)
	{
		char szYear[5] = "";
		char szMonth[3] = "";
		char szDay[3] = "";
		char szHour[3] = "";
		char szMin[3] = "";

		strncpy(szYear, m_ObjBill[lpObj->m_Index].GetEndsDays(), 4);
		strncpy(szMonth, m_ObjBill[lpObj->m_Index].GetEndsDays()+4, 2);
		strncpy(szDay, m_ObjBill[lpObj->m_Index].GetEndsDays()+6, 2);
		strncpy(szHour, m_ObjBill[lpObj->m_Index].GetEndsDays()+8, 2);
		strncpy(szMin, m_ObjBill[lpObj->m_Index].GetEndsDays()+10, 2);

		wsprintf(szMsg, lMsg.Get(MSGGET(5, 220)), m_ObjBill[lpObj->m_Index].GetEndTime(), szYear, szMonth,
			szDay, szHour, szMin);

		CLog.LogAdd("[%s][%s] BillType : (Time) RemainPoint : (%d)",
			lpObj->AccountID, lpObj->Name, m_ObjBill[lpObj->m_Index].GetEndTime());
	}
	else if ( m_ObjBill[lpObj->m_Index].GetPayCode() == 4)
	{
		char szYear[5] = "";
		char szMonth[3] = "";
		char szDay[3] = "";

		strncpy(szYear, m_ObjBill[lpObj->m_Index].GetEndsDays(), 4);
		strncpy(szMonth, m_ObjBill[lpObj->m_Index].GetEndsDays()+4, 2);
		strncpy(szDay, m_ObjBill[lpObj->m_Index].GetEndsDays()+6, 2);

		wsprintf(szMsg, lMsg.Get(MSGGET(5, 221)),  szYear, szMonth,	szDay);

		CLog.LogAdd("[%s][%s] BillType : (Date) RemainDate : (%s-%s-%s)",
			lpObj->AccountID, lpObj->Name, szYear, szMonth, szDay);
	}
	else if ( m_ObjBill[lpObj->m_Index].GetPayCode() == 5)		// FREE
	{
		wsprintf(szMsg, lMsg.Get(MSGGET(5, 222)));
		CLog.LogAdd("[%s][%s] BillType : (NoCharge)", lpObj->AccountID, lpObj->Name);
	}
	else	// Pospaid Account
	{
		wsprintf(szMsg, lMsg.Get(MSGGET(4, 89)));
	}

	CLog.LogAdd(szMsg);
	GCServerMsgStringSend(szMsg, lpObj->m_Index, 1);
}

short gObjMemFree(int index)
{
	int i = 0;
	LPOBJ lpObj;

	if ( gDisconnect == TRUE )
	{
		return -1;
	}

	if ( index < 0 || index > OBJMAX )
	{
		CLog.LogAdd("(%s)(%d) = index over error (%d)", __FILE__, __LINE__, index);
		return false;
	}

	lpObj = &gObj[index];

	if ( lpObj->Connected < PLAYER_CONNECTED )
	{
		return false;
	}

	gObjTradeSave(lpObj, index);

	if ( lpObj->Connected >= PLAYER_LOGGED )
	{
		if ( lpObj->Type == OBJ_USER )
		{
			if ( lpObj->Connected == PLAYER_PLAYING )
			{
				if (  gUser.BloodCastleMapRange(lpObj->MapNumber) == true )
				{
					g_BloodCastle.SearchUserDropQuestItem(lpObj->m_Index);
				}

				GJSetCharacterInfo(lpObj, index, 0);
			}
			
			JSProto.UserClose(lpObj);
		}
	}

	gObjViewportClose(lpObj);
	lpObj->Connected = PLAYER_EMPTY;
	return 1;
}

BOOL gObjGameClose(int aIndex)
{
	if ( aIndex < 0 || aIndex > OBJMAX )	// Deathway fix
	{
		CLog.LogAdd("(%s)(%d) = index over error (%d)", __FILE__, __LINE__, aIndex);

		return FALSE;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if ( lpObj->Connected != PLAYER_PLAYING )
	{
		return FALSE;
	}

	if ( lpObj->Type != OBJ_USER )
	{
		CLog.LogAdd(lMsg.Get(MSGGET(2, 3)), lpObj->AccountID, lpObj->Name);
		return FALSE;
	}

	if ( lpObj->m_RecallMon >= 0 )
	{
		gObjMonsterCallKill(lpObj->m_Index);
	}

	if ( (   (lpObj->m_IfState.use != 0 )
		&&  (lpObj->m_IfState.type == 7 )) 
		|| ((lpObj->m_IfState.use != 0)  
		&& (lpObj->m_IfState.type == 13)))
	{
		if ( lpObj->bIsChaosMixCompleted == true || lpObj->m_bIsCastleNPCUpgradeCompleted == true)
		{
			gObjInventoryCommit(aIndex);
		}
		else
		{
			gObjInventoryRollback(aIndex);
		}
	}

	gObjTradeSave(lpObj, aIndex);

	if ( (lpObj->m_IfState.use != 0 )
		&&  (lpObj->m_IfState.type == 10 ) 
		&& lpObj->TargetShopNumber == 229)
	{
		gQeustNpcTeleport.TalkRefDel();
	}

	if ( (lpObj->Authority&2) == 2 )
	{
		cGMMng.DelGameMaster(lpObj->Name);
	}

	if ( OBJMAX_RANGE(lpObj->m_iDuelUserRequested) )
	{
		//gObjSendDuelEnd(&gObj[lpObj->m_iDuelUserRequested]);
	}


	if ( gDuel.IsOnDuel(lpObj) == true )
	{
		//gObjSendDuelEnd(&gObj[lpObj->m_iDuelUser]);
	}


	//gObjResetDuel(lpObj);


	if ( lpObj->PartyNumber >= 0 )
	{
		int iPartyIndex= gParty.GetIndex(lpObj->PartyNumber, lpObj->m_Index, lpObj->DBNumber);

		if ( iPartyIndex >= 0 )
		{
			PMSG_PARTYDELUSER pMsg;

			pMsg.Number = iPartyIndex;

			CGPartyDelUser(&pMsg, lpObj->m_Index);
		}
	}


	if ( lpObj->GuildNumber > 0 )
	{
		if ( lpObj->lpGuild->WarState == 1 )
		{
			gObjGuildWarMasterClose(lpObj);
		}

		GDCharClose(0, lpObj->lpGuild->Name, lpObj->Name);
	}
	else
	{
		GDCharClose(1, NULL, lpObj->Name);
	}

	if ( lpObj->WarehouseSave == 1 )
	{
		GDSetWarehouseList(lpObj->m_Index);
	}

	if ( gUser.BloodCastleMapRange(lpObj->MapNumber) == true )
	{
		g_BloodCastle.SearchUserDropQuestItem(lpObj->m_Index);
	}

	gObjSaveChaosBoxItemList(lpObj);

	if ( gObjFixInventoryPointer(aIndex) == false )
	{
		CLog.LogAdd("[Fix Inv.Ptr] False Location - %s, %d", __FILE__, __LINE__);
	}

	g_CashItemPeriodSystem.GDReqPeriodItemUpdate(lpObj);
	g_CashItemPeriodSystem.ClearPeriodItemEffect(lpObj);


	GJSetCharacterInfo(lpObj, aIndex, 0);
	gObjViewportClose(lpObj);
	CLog.LogAdd(lMsg.Get(MSGGET(2, 4)), lpObj->m_Index, lpObj->AccountID, lpObj->Name);

	if(lpObj->Name)
	{
		WhisperCash.DelCash(lpObj->Name);
	}

	g_CashShop.DeleteUser(lpObj);

	if(lpObj->m_iPeriodItemEffectIndex != -1)
	{
		g_CashItemPeriodSystem.GDReqPeriodItemUpdate(lpObj);
		g_CashItemPeriodSystem.ClearPeriodItemEffect(lpObj);
	}

	memcpy(lpObj->BackName, lpObj->Name, sizeof(lpObj->Name)-1);
	lpObj->BackName[10] = 0;


	memset(lpObj->Name, 0, sizeof(lpObj->Name)-1);
	lpObj->Connected = PLAYER_LOGGED;

	if ( m_ObjBill[aIndex].GetCertify() >= 0 && m_ObjBill[aIndex].GetCertify() < 3)
	{
		gLCount[m_ObjBill[aIndex].GetCertify()].Delete();
	}

	return TRUE;
}

short gObjDel(int index)
{
	int i = 0;

	if ( index < 0 || index > OBJMAX )
	{
		CLog.LogAdd ("(%s)(%d) = index over error (%d)", __FILE__, __LINE__, index);
		return 0;
	}

	LPOBJ lpObj = &gObj[index];

	if ( lpObj->Connected < PLAYER_CONNECTED )
	{
		return 0;
	}

	if ( MAX_MAP_RANGE(lpObj->MapNumber) )
	{
		MapC[lpObj->MapNumber].ClearStandAttr(lpObj->m_OldX, lpObj->m_OldY);
		MapC[lpObj->MapNumber].ClearStandAttr(lpObj->X, lpObj->Y);
	}

	int UserConnectState = lpObj->Connected;

	if ( lpObj->Type == OBJ_USER )
	{
		gObjGameClose(index);

		if ( UserConnectState >= PLAYER_LOGGED )
		{
			if ( lpObj->m_bMapSvrMoveQuit == false )
			{
				JSProto.UserClose(lpObj);
				CLog.LogAdd("(%d)logout-send : %s [%s]", index, lpObj->AccountID, lpObj->Ip_addr);
			}
			else
			{
				CLog.LogAdd("[MapServerMng] User Quit without JoinServer logout-send [%s][%s]",
					lpObj->AccountID, lpObj->Name);
			}

			memset(lpObj->AccountID, 0, sizeof(lpObj->AccountID) -1);
			memset(lpObj->LastJoominNumber, 0, sizeof(lpObj->LastJoominNumber));
		}

		CLog.LogAdd("(%d)logout : %s [%s]", index, lpObj->AccountID, lpObj->Ip_addr);
		IpCache.DelIp(lpObj->Ip_addr);
	}

	lpObj->Connected = PLAYER_EMPTY;
	return 1;
}

short gObjSometimeClose(int index)
{
	if ( gDisconnect == 1 )
	{
		return -1;
	}

	if ( index < 0 || index > OBJMAX-1 )
	{
		CLog.LogAdd("(%s)(%d) = index over error (%d)", __FILE__, __LINE__, index);
		return 0;
	}

	LPOBJ lpObj = &gObj[index];
	gObjTradeSave(lpObj, index);

	if ( lpObj->Connected > PLAYER_CONNECTED )
	{
		if ( lpObj->Type == OBJ_USER )
		{
			if ( lpObj->Connected == PLAYER_PLAYING )
			{
				if ( lpObj->PartyNumber >= 0 )
				{
					PMSG_PARTYDELUSER pMsg;
					int iPartyIndex = gParty.GetIndex(lpObj->PartyNumber, lpObj->m_Index, lpObj->DBNumber);

					if ( iPartyIndex >= 0 )
					{
						pMsg.Number = iPartyIndex;
						CGPartyDelUser(&pMsg, lpObj->m_Index);
					}
				}
			}
		}
	}

	return 1;
}
//Done 100%
bool gObjIsGamePlaing(LPOBJ lpObj)
{
	if ( lpObj->Connected < PLAYER_PLAYING )
	{
		return false;
	}

	return true;
}

bool gObjIsConnectedGP(int aIndex)
{
	if (aIndex < 0 || aIndex > OBJMAX-1 )
	{
		return false;
	}
	
	if ( gObj[aIndex].Type != OBJ_USER )
	{
		return false;
	}

	if ( gObj[aIndex].Connected < PLAYER_PLAYING )
	{
		return false;
	}

	if ( gObj[aIndex].CloseCount > 0 )
	{
		return false;
	}

	return true;
}

bool gObjIsConnectedGP(int aIndex, char * CharName)
{
	if (aIndex < 0 || aIndex > OBJMAX-1 )
	{
		return false;
	}
	
	if ( gObj[aIndex].Type != OBJ_USER )
	{
		return false;
	}

	if ( gObj[aIndex].Connected < PLAYER_PLAYING )
	{
		return false;
	}

	if ( gObj[aIndex].CloseCount > 0 )
	{
		return false;
	}

	if ( strcmp(gObj[aIndex].Name, CharName) != 0 )
	{
		return false;
	}

	return true;
}

bool gObjIsAccontConnect(int aIndex, char* accountid)
{
	if (aIndex < 0 || aIndex > OBJMAX-1)
	{
		return false;
	}

	if (gObj[aIndex].Connected< PLAYER_CONNECTED)
	{
		return false;
	}

	if (strlen(&gObj[aIndex].AccountID[0]) < 1 )
	{
		return false;
	}

	if ( strcmp(accountid, &gObj[aIndex].AccountID[0]) != 0)
	{
		return false;
	}
	return true;
}

bool gObjJoominCheck(int aIndex, char* szInJN)
{
	LPOBJ lpObj = &gObj[aIndex];
	
	switch ( gLanguage )
	{
		case 0:
		if (strncmp(szInJN, lpObj->LastJoominNumber, 7) == 0)
		{
			return true;
		}
		break;
		case 1:
		case 5:
		case 6:
		case 8:
			if ( strncmp(szInJN, lpObj->LastJoominNumber, 7) == 0 )
			{
				return true;
			}
			break;
		case 2:
			if ( strncmp(szInJN, &lpObj->LastJoominNumber[5], 8) == 0 )
			{
				return true;
			}
			break;
		case 3:
			if ( _strcmpi(szInJN, &lpObj->LastJoominNumber[6]) == 0 )
			{
				return true;
			}
			break;
		case 4:
			if ( strlen(szInJN) < 2 )
			{
				return false;
			}

			if ( (szInJN[0] >= 'A' && szInJN[0] <= 'Z') || (szInJN[0] >= 'a' && szInJN[0] <= 'z') )
			{
				if ( szInJN[0] >= 'A' && szInJN[0] <= 'Z' )
				{
					szInJN[0] = szInJN[0] + 32;
				}

				if ( lpObj->LastJoominNumber[0] >= 'A' && lpObj->LastJoominNumber[0] <= 'Z' )
				{
					lpObj->LastJoominNumber[0] = lpObj->LastJoominNumber[0] + 32;
				}

				if ( strncmp(lpObj->LastJoominNumber, szInJN, 10) == 0 )
				{
					return true;
				}
			}
			break;

		default:
			if ( strncmp(szInJN, &lpObj->LastJoominNumber[6], 7) == 0 )
			{
				return true;
			}

	}

	return false;
}

bool gObjSetAccountLogin(int aIndex, char * szId, int aUserNumber, int aDBNumber, char* JoominNumber)
{
	if ( !OBJMAX_RANGE(aIndex) )
	{
		CLog.LogAdd("error : %s %d", __FILE__, __LINE__);
		return false;
	}

	if ( gObj[aIndex].Connected != PLAYER_CONNECTED )
	{
		CLog.LogAdd(lMsg.Get(MSGGET(2, 6)), aIndex, szId, aDBNumber, __FILE__, __LINE__);
		return false;
	}

	if ( strcmp(gObj[aIndex].AccountID, szId) != 0 )
	{
		CLog.LogAdd(lMsg.Get(MSGGET(2, 7)), szId);
		return false;
	}

	gObj[aIndex].UserNumber = aUserNumber;
	gObj[aIndex].DBNumber = aDBNumber;
	gObj[aIndex].Connected = PLAYER_LOGGED;
	strcpy(gObj[aIndex].LastJoominNumber, JoominNumber);
	return true;
}

int gObjGetUserNumber(int aIndex)
{
	int usernumber;

	if ( !OBJMAX_RANGE(aIndex))
	{
		CLog.LogAdd("error : %s %d",__FILE__, __LINE__);
		return 0;
	}

	usernumber=gObj[aIndex].UserNumber;
	return usernumber;
}

bool gObjGetNumber(int aIndex, int& UN, int& DBN)
{
	if ( aIndex < 0 || aIndex > OBJMAX-1)
	{
		CLog.LogAdd("[gObjGetNumber]: Index Error %d",aIndex);
		return false;
	}
	UN=gObj[aIndex].UserNumber;
	DBN=gObj[aIndex].DBNumber;
	return true;
}

char* gObjGetAccountId(int aIndex)
{
	if ( aIndex < 0 || aIndex > OBJMAX-1)
	{
		CLog.LogAdd("[gObjGetAccountId]: Index Error %d",aIndex);
		return 0;
	}

	if ( gObj[aIndex].Connected == 2)
	{
		return &gObj[aIndex].AccountID[0];
	}
	return 0;
}

int gObjGetIndex(char* szId)
{
	int n;

	for (n=OBJ_STARTUSERINDEX;n<OBJMAX; n++)
	{
		if ( gObj[n].Connected >= PLAYER_PLAYING)
		{
			if ( gObj[n].Name[0] == *szId )
			{
				if ( strcmp(&gObj[n].Name[0] , szId) == 0)
				{
					return n;
				}
			}
		}
	}
	return -1;
}

bool gObjUserIdConnectCheck(char* szId,int index)
{
	if ( index < 0 || index > OBJMAX)	// Deathway Fix Here
	{
		CLog.LogAdd("error : index error !! %s %d",__FILE__, __LINE__);
		return false;
	}

	if ( gObj[index].Connected < PLAYER_PLAYING)
	{
		return false;
	}

	if (gObj[index].Name[0] != *szId)
	{
		return false;
	}

	if ( strcmp(gObj[index].Name , szId) == 0)
	{
		return true;
	}
	return false;
}

BYTE GetPathPacketDirPos(int px, int py)
{
	BYTE pos = 0;

	if (px <= -1 && py <= -1)
	{
		pos=0;
	}
	else if (px <= -1 && py == 0)
	{
		pos=7;
	}
	else if ( px <= -1 && py >= 1)
	{
		pos=6;
	}
	else if ( px == 0 && py <= -1)
	{
		pos= 1;
	}
	else if ( px == 0 && py >= 1)
	{
		pos = 5;
	}
	else if ( px >= 1 && py <=-1)
	{
		pos=2;
	}
	else if ( px >= 1 && py == 0)
	{
		pos = 3;
	}
	else if ( px >=1  && py >= 1)
	{
		pos = 4;
	}

	return pos;
}

// --------------------------------------------------
// [Sunday, 20 Feb, 2011] WolF ~ Call Distance
// --------------------------------------------------
int gObjCalDistance(LPOBJ lpObj,LPOBJ lpTarget)
{
    if( lpObj->X == lpTarget->X && lpObj->Y == lpTarget->Y ) return FALSE;
	// ------
	float x = (float)(lpObj->X-lpTarget->X);
	float y = (float)(lpObj->Y-lpTarget->Y);
	// ------
    return sqrt((x*x)+(y*y));
}

void gObjSetInventory1Pointer(LPOBJ lpObj)
{
	lpObj->pInventory = lpObj->Inventory1;
	lpObj->pInventoryMap = lpObj->InventoryMap1;
	lpObj->pInventoryCount  = &lpObj->InventoryCount1;
}

void gObjSetInventory2Pointer(LPOBJ lpObj)
{
	lpObj->pInventory = lpObj->Inventory2;
	lpObj->pInventoryMap = lpObj->InventoryMap2;
	lpObj->pInventoryCount  = &lpObj->InventoryCount2;
}

void gObjAddMsgSend(LPOBJ lpObj, int aMsgCode, int aIndex, int  SubCode)
{
	for ( int n=0;n<MAX_MONSTER_SEND_MSG;n++)
	{
		if ( gSMMsg[lpObj->m_Index][n].MsgCode < 0 )
		{
			gSMMsg[lpObj->m_Index][n].MsgCode = aMsgCode;
			gSMMsg[lpObj->m_Index][n].MsgTime = GetTickCount();
			gSMMsg[lpObj->m_Index][n].SendUser = aIndex;
			gSMMsg[lpObj->m_Index][n].SubCode = SubCode;

			return;
		}
	}

	for ( int n=0;n<MAX_MONSTER_SEND_MSG;n++)
	{
		if ( gSMMsg[lpObj->m_Index][n].MsgCode == 2 || gSMMsg[lpObj->m_Index][n].MsgCode == 6 || gSMMsg[lpObj->m_Index][n].MsgCode == 7 )
		{
			gSMMsg[lpObj->m_Index][n].MsgCode = aMsgCode;
			gSMMsg[lpObj->m_Index][n].MsgTime = GetTickCount();
			gSMMsg[lpObj->m_Index][n].SendUser = aIndex;
			gSMMsg[lpObj->m_Index][n].SubCode = SubCode;
			
			return;
		}
	}
}

void gObjAddMsgSendDelay(LPOBJ lpObj, int aMsgCode, int aIndex, int delay, int SubCode)
{
	for ( int n=0;n<MAX_MONSTER_SEND_MSG;n++)
	{
		if ( gSMMsg[lpObj->m_Index][n].MsgCode < 0 )
		{
			gSMMsg[lpObj->m_Index][n].MsgCode = aMsgCode;
			gSMMsg[lpObj->m_Index][n].MsgTime = GetTickCount() + delay;
			gSMMsg[lpObj->m_Index][n].SendUser = aIndex;
			gSMMsg[lpObj->m_Index][n].SubCode = SubCode;

			return;
		}
	}

	for ( int n=0;n<MAX_MONSTER_SEND_MSG;n++)
	{
		if ( gSMMsg[lpObj->m_Index][n].MsgCode == 2 || gSMMsg[lpObj->m_Index][n].MsgCode == 6 || gSMMsg[lpObj->m_Index][n].MsgCode == 7 )
		{
			gSMMsg[lpObj->m_Index][n].MsgCode = aMsgCode;
			gSMMsg[lpObj->m_Index][n].MsgTime = GetTickCount();
			gSMMsg[lpObj->m_Index][n].SendUser= aIndex;
			gSMMsg[lpObj->m_Index][n].SubCode = SubCode;
			
			return;
		}
	}
}

void gObjAddMsgSendDelayInSpecificQPos(LPOBJ lpObj, int aMsgCode, int aIndex, int delay, int SubCode, int iQPosition)
{
	if ( iQPosition >= 0 &&  iQPosition < MAX_MONSTER_SEND_MSG )
	{
		gSMMsg[lpObj->m_Index][iQPosition].MsgCode = aMsgCode;
		gSMMsg[lpObj->m_Index][iQPosition].MsgTime = GetTickCount();
		gSMMsg[lpObj->m_Index][iQPosition].SendUser = aIndex;
		gSMMsg[lpObj->m_Index][iQPosition].SubCode = SubCode;
	}

}

void gObjAddAttackProcMsgSendDelay(LPOBJ lpObj, int aMsgCode, int aIndex, int delay, int SubCode, int SubCode2)
{
	for ( int n=0;n<MAX_MONSTER_SEND_ATTACK_MSG;n++)
	{
		int iIndex = lpObj->m_Index - OBJ_STARTUSERINDEX;

		if ( gSMAttackProcMsg[iIndex][n].MsgCode < 0 )
		{
			gSMAttackProcMsg[iIndex][n].MsgCode = aMsgCode;
			gSMAttackProcMsg[iIndex][n].MsgTime = GetTickCount() + delay;
			gSMAttackProcMsg[iIndex][n].SendUser = aIndex;
			gSMAttackProcMsg[iIndex][n].SubCode = SubCode;
			gSMAttackProcMsg[iIndex][n].SubCode2 = SubCode2;

			return;
		}
	}

}

void gObjMsgProc(LPOBJ lpObj)
{
	int aIndex = lpObj->m_Index;

	for ( int n=0;n<MAX_MONSTER_SEND_MSG;n++)
	{
		if ( gSMMsg[aIndex][n].MsgCode >= 0 )
		{
			if ( GetTickCount() > gSMMsg[aIndex][n].MsgTime )
			{
				if ( lpObj->Type == OBJ_MONSTER || lpObj->Type == OBJ_NPC )
				{
					gObjMonsterStateProc(lpObj, gSMMsg[aIndex][n].MsgCode, gSMMsg[aIndex][n].SendUser, gSMMsg[aIndex][n].SubCode);
				}
				else
				{
					gObjStateProc(lpObj, gSMMsg[aIndex][n].MsgCode, gSMMsg[aIndex][n].SendUser, gSMMsg[aIndex][n].SubCode);
				}

				gSMMsg[aIndex][n].MsgCode = -1;
			}
		}
	}
}

void gObjStateProc(LPOBJ lpObj, int aMsgCode, int aIndex, int SubCode)
{
	if ( (  (aIndex<0)?FALSE:(aIndex > OBJMAX-1)?FALSE:TRUE ) == FALSE )
	{
		CLog.LogAdd("error : %s %d", __FILE__, __LINE__ );
		return;
	}

	switch ( aMsgCode )
	{
		case  2:
			gObjBackSpring(lpObj, &gObj[aIndex]);
			break;

		case  3:
			gObjMonsterDieLifePlus(lpObj, &gObj[aIndex]);
			break;

		case 1000:
			gObjBillRequest(lpObj);
			break;

		case  4:
			if ( lpObj->lpGuild != NULL && lpObj->lpGuild->lpTargetGuildNode != NULL )
			{
				gObjGuildWarEnd(lpObj->lpGuild, lpObj->lpGuild->lpTargetGuildNode);
			}
			break;

		case  5:
			if ( lpObj->lpGuild != NULL && lpObj->lpGuild->WarState != NULL && lpObj->lpGuild->WarType == 1 )
			{
				BattleSoccerGoalStart(0);
			}
			break;

		case  7:
			if ( lpObj->lpGuild != NULL && lpObj->lpGuild->WarState != NULL && lpObj->lpGuild->WarType == 1)
			{
				if ( lpObj->lpGuild->BattleGroundIndex >= 0 )
				{
					GCManagerGuildWarEnd(lpObj->lpGuild->Name);
				}
			}
			break;

		case 10:
			if ( lpObj->DamageReflect != 0 )
			{
				gObjAttack(lpObj, &gObj[aIndex], 0, 0, 0, SubCode, 0);
			}
			break;
		case 54:
			if( lpObj->Live )
			{
				CMagicInf * lpMagic = &DefMagicInf[79];
				gObjAttack(lpObj, &gObj[aIndex], lpMagic, 1, 0, SubCode, 0);
			}
			break;
	}
}

void gObjStateAttackProc(LPOBJ lpObj, int aMsgCode, int aIndex, int SubCode, int SubCode2)
{
	if ( OBJMAX_RANGE(aIndex) == FALSE )
	{
		CLog.LogAdd("error : %s %d", __FILE__, __LINE__ );
		return;
	}

	switch ( aMsgCode )
	{
		case 50:
			if ( lpObj->Live != FALSE )
			{
				CMagicInf * lpMagic = gObjGetMagicSearch(lpObj, SubCode);
				gObjAttack(lpObj, &gObj[aIndex], lpMagic, 0, 1, 0, SubCode2);
			}
			break;
		case 53:
			if( lpObj->Live )
			{
				CMagicInf * lpMagic = gObjGetMagicSearch(lpObj, SubCode);
				gObjAttack(lpObj, &gObj[aIndex], lpMagic, 1, 1, 0, 0);
			}
			break;
		case 51:
			gDarkSpirit[lpObj->m_Index].Attack(lpObj, &gObj[aIndex], NULL, SubCode, SubCode2);
			break;
	}

}

BOOL gObjBackSpring(LPOBJ lpObj, LPOBJ lpTargetObj)
{
	int tdir;
	int x;
	int y;
	BYTE attr;
	PMSG_POSISTION_SET pMove;

	if ( MAX_MAP_RANGE(lpObj->MapNumber) == FALSE )
	{
		CLog.LogAdd("error : %s %d", __FILE__, __LINE__);
		return FALSE;
	}

	if ( lpObj->Type == OBJ_USER )
	{
		if ( lpObj->m_LastTeleportTime > 0 )
		{
			return FALSE;
		}
	}

#if(GS_CASTLE)
	
	if( lpObj->Type == OBJ_USER)
	{
		if((lpObj->Authority &0x02) == 2)
		{
			return FALSE;
		}
	}
	
#endif
	
	if ( lpObj->Class >= 131 && lpObj->Class <= 134 )
	{
		return FALSE;
	}
	
#if(GS_CASTLE)
	
	if( lpObj->Class == 277 ||//castle siege ??
		lpObj->Class == 283 ||
		lpObj->Class == 288 ||
		lpObj->Class == 278 ||
		lpObj->Class == 215 ||
		lpObj->Class == 216 ||
		lpObj->Class == 217 ||
		lpObj->Class == 218 ||
		lpObj->Class == 219 )
	{
		return FALSE;
	}
	
	if(g_CastleSiege.GetCrownUserIndex() == lpObj->m_Index)
	{
		if(lpObj->pInventory[8].m_Type == ITEMGET(13,4))
		{
			return FALSE;
		}
	}
	
	if(g_Crywolf.GetCrywolfState() == 4 || g_Crywolf.GetCrywolfState() == 3)
	{
		if(lpObj->Type == OBJ_USER)
		{
			for(int i=205;i<=209;i++)//crywolf altar
			{
				int iAltarIndex = g_CrywolfNPC_Altar.GetAltarUserIndex(i);
				
				if(iAltarIndex != -1)
				{
					if(iAltarIndex == lpObj->m_Index)
					{
						return FALSE;
					}
				}
			}
		}
	}
	
	if(CRYWOLF_ALTAR_CLASS_RANGE(lpObj->Class) == FALSE)
	{
		if( !CHECK_CLASS(204, lpObj->Class) )//crywolf statue
		{
			return FALSE;
		}
	}
	
	if(lpObj->Class == 348)//balista
	{
		return FALSE;
	}
	
#endif

	if ( (rand()%3) == 0 )
	{
		if ( lpTargetObj->Dir < 4 )
		{
			tdir = lpTargetObj->Dir + 4;
		}
		else
		{
			tdir = lpTargetObj->Dir - 4;
		}

		tdir *= 2;
	}
	else
	{
		tdir = lpTargetObj->Dir * 2;
	}

	x = lpObj->X;
	y = lpObj->Y;
	x += RoadPathTable[tdir];
	y += RoadPathTable[1+tdir];
	attr = MapC[lpObj->MapNumber].GetAttr(x, y);

	if ( (attr&1)==1 ||
		 (attr&2)==2 || 
		 (attr&4)==4 || 
		 (attr&8)==8 
#if(GS_CASTLE)
		||(attr&16)== 16 
#endif
		)
	{
		return FALSE;
	}

	pMove.h.c = 0xC1;
	pMove.h.headcode = PMSG_POSISTION_SET_HEADCODE;
	pMove.h.size = sizeof(pMove);
	pMove.X = x;
	pMove.Y = y;

	lpObj->m_Rest = 0;
	MapC[lpObj->MapNumber].ClearStandAttr(lpObj->m_OldX, lpObj->m_OldY);
	MapC[lpObj->MapNumber].SetStandAttr(x, y);
	lpObj->m_OldX = x;
	lpObj->m_OldY = y;
	RecvPositionSetProc(&pMove, lpObj->m_Index);

	if ( lpObj->Type == OBJ_USER )
	{
		lpObj->PathCount = 0;
	}

	return TRUE;
}

BOOL BackSpringCheck(int & x, int & y, int & dir, BYTE map)
{
	BYTE attr;
	int tx = x;
	int ty = y;
	int tdir = dir/2;

	tx = tx + RoadPathTable[dir];
	ty = ty + RoadPathTable[1+dir];

	attr = MapC[map].GetAttr(tx, ty);

	if	(  (attr&1)==1 || 
		   (attr&4)==4 || 
		   (attr&8)==8
#if(GS_CASTLE)
		||(attr&16)== 16 
#endif
		)
	{
		tdir += 4;

		if ( tdir > 7 )
		{
			tdir -= 8;
		}

		dir = tdir * 2;
		return FALSE;
	}

	x = tx;
	y = ty;

	return TRUE;
}

BOOL gObjBackSpring2(LPOBJ lpObj, LPOBJ lpTargetObj, int count)
{
	if ( MAX_MAP_RANGE(lpObj->MapNumber) == FALSE )
	{
		CLog.LogAdd("error : %s %d", __FILE__, __LINE__);
		return FALSE;
	}
	
	
#if(GS_CASTLE)
	
	if( lpObj->Type == OBJ_USER)
	{
		if((lpObj->Authority &0x02) == 2)
		{
			return FALSE;
		}
	}
	
#endif

	if ( lpObj->Class == 287 || lpObj->Class == 286 )
	{
		return FALSE;
	}

#if(GS_CASTLE)
	
	if ( lpObj->Class == 278 )
	{
		return FALSE;
	}
	
	
#endif

	if ( lpObj->Class >= 131 && lpObj->Class <= 134 )
	{
		return FALSE;
	}

#if(GS_CASTLE)
	
	if( lpObj->Class == 277 ||//castle siege ??
		lpObj->Class == 283 ||
		lpObj->Class == 288 ||
		lpObj->Class == 278 ||
		lpObj->Class == 215 ||
		lpObj->Class == 216 ||
		lpObj->Class == 217 ||
		lpObj->Class == 218 ||
		lpObj->Class == 219 )
	{
		return FALSE;
	}
	
	if(g_CastleSiege.GetCrownUserIndex() == lpObj->m_Index)
	{
		if(lpObj->pInventory[8].m_Type == ITEMGET(13,4))
		{
			return FALSE;
		}
	}
	
	if(g_Crywolf.GetCrywolfState() == 4 || g_Crywolf.GetCrywolfState() == 3)
	{
		if(lpObj->Type == OBJ_USER)
		{
			for(int i=205;i<=209;i++)//crywolf altar
			{
				int iAltarIndex = g_CrywolfNPC_Altar.GetAltarUserIndex(i);
				
				if(iAltarIndex != -1)
				{
					if(iAltarIndex == lpObj->m_Index)
					{
						return FALSE;
					}
				}
			}
		}
	}
	
	if(CRYWOLF_ALTAR_CLASS_RANGE(lpObj->Class) == FALSE)
	{
		if( !CHECK_CLASS(204, lpObj->Class) )//crywolf statue
		{
			return FALSE;
		}
	}
	
	if(lpObj->Class == 348)//balista
	{
		return FALSE;
	}
	
#endif
	
	int tdir = GetPathPacketDirPos(lpObj->X - lpTargetObj->X, lpObj->Y - lpTargetObj->Y) * 2;
	int x = lpObj->X;
	int y = lpObj->Y;

	for ( int n=0;n<count;n++)
	{
		if ( n >= 2 )
		{
			if ( lpObj->Class == CLASS_DARKLORD  || lpObj->Class == CLASS_MAGUMSA  || lpObj->Class == CLASS_KNIGHT  )
			{
				BackSpringCheck(x, y, tdir, lpObj->MapNumber);
			}
		}
		else
		{
			BackSpringCheck(x, y, tdir, lpObj->MapNumber);
		}
	}

	PMSG_POSISTION_SET pMove;

	pMove.h.c = 0xC1;
	pMove.h.headcode = PMSG_POSISTION_SET_HEADCODE;
	pMove.h.size = sizeof(pMove);
	pMove.X = x;
	pMove.Y = y;

	lpObj->m_Rest = 0;
	MapC[lpObj->MapNumber].ClearStandAttr(lpObj->m_OldX, lpObj->m_OldY);
	MapC[lpObj->MapNumber].SetStandAttr(x, y);
	lpObj->m_OldX = x;
	lpObj->m_OldY = y;
	RecvPositionSetProc(&pMove, lpObj->m_Index);

	return TRUE;
}
// Done 100%
bool gObjLevelUp(LPOBJ lpObj, int addexp, int iMonsterType, int iEventType)
{
	int isMasterClass = 0;

	/*if ( gUser.gObjIsMasteringLevel(lpObj) == true )
	{
	    isMasterClass = 1;
	}*/

	/*if ( isMasterClass == 1 )
	{
		if(lpObj->MasterLevel >= MAX_CHAR_MASTER_LEVEL )
		{
			lpObj->MLExperience = 0;
			lpObj->MLNextExp = 0;
			GCServerMsgStringSend("You have reached maximum Master Level",lpObj->m_Index,1);
		    return false;
		}
		else if ( lpObj->MLExperience < lpObj->MLNextExp )
		{
			return true;
		}
		else
		{
			lpObj->MasterLevel++;
			lpObj->MLExperience = lpObj->MLNextExp;
			// ----
			switch ( lpObj->Class )
			{
			case CLASS_WIZARD:		lpObj->MasterPoints += CLASS_WIZARD_MPOINTS;	break;
			case CLASS_KNIGHT:		lpObj->MasterPoints += CLASS_KNIGHT_MPOINTS;	break;
			case CLASS_ELF:			lpObj->MasterPoints += CLASS_ELF_MPOINTS;		break;
			case CLASS_MAGUMSA:		lpObj->MasterPoints += CLASS_MAGUMSA_MPOINTS;	break;
			case CLASS_DARKLORD:	lpObj->MasterPoints += CLASS_DARKLORD_MPOINTS;	break;
			case CLASS_SUMMONER:	lpObj->MasterPoints += CLASS_SUMMONER_MPOINTS;	break;
			case CLASS_FIGHTER:		lpObj->MasterPoints += CLASS_FIGHTER_MPOINTS;	break;
			}
		}
	}
	else
	{*/
		if ( lpObj->Level >= HRB_CharMaxLevel )
		{
			lpObj->Experience	= 0;
			lpObj->NextExp		= 0;
			GCServerMsgStringSend("You have reached maximum Level",lpObj->m_Index,1);
			return false;
		}
		/*else if ( lpObj->Experience < lpObj->NextExp )
		{
			return true;
		}*/
		else
		{
			lpObj->Level++;
			lpObj->Experience = lpObj->NextExp;
			// ----
			switch ( lpObj->Class )
			{
			case CLASS_WIZARD:		lpObj->LevelUpPoint += gCharInfo.CLASS_WIZARD_POINTS;	break;
			case CLASS_KNIGHT:		lpObj->LevelUpPoint += gCharInfo.CLASS_KNIGHT_POINTS;	break;
			case CLASS_ELF:			lpObj->LevelUpPoint += gCharInfo.CLASS_ELF_POINTS;		break;
			case CLASS_MAGUMSA:		lpObj->LevelUpPoint += gCharInfo.CLASS_MAGUMSA_POINTS;	break;
			case CLASS_DARKLORD:	lpObj->LevelUpPoint += gCharInfo.CLASS_DARKLORD_POINTS;	break;
			case CLASS_SUMMONER:	lpObj->LevelUpPoint += gCharInfo.CLASS_SUMMONER_POINTS;	break;
			case CLASS_FIGHTER:		lpObj->LevelUpPoint += gCharInfo.CLASS_FIGHTER_POINTS;	break;
			}
			if ( lpObj->PlusStatQuestClear != false )
			{
				switch ( lpObj->Class )
				{
					case CLASS_WIZARD:		lpObj->LevelUpPoint += gCharInfo.CLASS_WIZARD_POINTS_PLUS;	break;
					case CLASS_KNIGHT:		lpObj->LevelUpPoint += gCharInfo.CLASS_KNIGHT_POINTS_PLUS;	break;
					case CLASS_ELF:			lpObj->LevelUpPoint += gCharInfo.CLASS_ELF_POINTS_PLUS;		break;
					case CLASS_SUMMONER:	lpObj->LevelUpPoint += gCharInfo.CLASS_SUMMONER_POINTS_PLUS;	break;
				}
			}

			gCalCharacter.gObjCalCharacter(lpObj->m_Index);

			lpObj->MaxLife += gCharInfo.sCharInfo[ lpObj->Class ].LevelLife;
			lpObj->MaxMana += gCharInfo.sCharInfo[ lpObj->Class ].LevelMana;
			lpObj->Life = lpObj->MaxLife;
			lpObj->Mana = lpObj->MaxMana;

			lpObj->Life = lpObj->MaxLife + lpObj->AddLife;
			lpObj->Mana = lpObj->MaxMana + lpObj->AddMana;
		
			gObjCalcShieldPoint(lpObj);
			lpObj->iShield = lpObj->iMaxShield + lpObj->iAddShield;
			GCReFillSend(lpObj->m_Index,lpObj->Life,0xFF,0,lpObj->iShield);
		}
	//}

	gObjSetExpPetItem(lpObj->m_Index, addexp);

	gObjNextExpCal(lpObj);	

	if(lpObj->Class == CLASS_ELF)
	{
		if(lpObj->Level >= g_SkillAdditionInfo.GetInfinityArrowUseLevel() && lpObj->ChangeUP == 1)
		{
			int iAddSkillPosition = gObjMagicAdd(lpObj,0x4D,0);

			if(iAddSkillPosition >= 0)
			{
				GCMagicListOneSend(lpObj->m_Index,iAddSkillPosition,0x4D,0xDC,0,0);
				CLog.LogAdd("[%s][%s] Add Infinity Arrow Skill (Character Level : %d)(ChangeUp: %d)", lpObj->AccountID,lpObj->Name,lpObj->Level,lpObj->ChangeUP);
			}
		}
	}

	GJSetCharacterInfo(lpObj, lpObj->m_Index, 0);
	gObjSetBP(lpObj->m_Index);

	if( isMasterClass == 1)
	{
	    gSkillTree.GCMasterLevelUpSend( lpObj, 1 );
		gSkillTree.SendMasterData(lpObj->m_Index);
	}
	else
	{
	    GCLevelUpMsgSend( lpObj->m_Index, 1 );
	}

	gObjCalcMaxLifePower(lpObj->m_Index);

	return true;
}

// ---------------------------------------------------
// [Sunday, 20 Feb, 2011] WolF ~ Level Up Points Add
// ---------------------------------------------------
bool gObjLevelUpPointAdd(LPOBJ lpObj, BYTE Type)
{
	if( lpObj->Type != OBJ_USER ) return false;
	// -----
	if( lpObj->LevelUpPoint < 1 ) return false;
	// -----
	switch(Type)
	{
	    case 0:
		    lpObj->Strength++;
			break;
			// -----
		case 1:
			lpObj->Dexterity++;
			break;
			// -----
		case 2:
			lpObj->Vitality++;
			lpObj->MaxLife += lpObj->VitalityToLife;
			gObjCalcMaxLifePower(lpObj->m_Index);
			break;
			// -----
		case 3:
			lpObj->Energy++;
			lpObj->MaxMana += lpObj->EnergyToMana;
			break;
			// -----
		case 4:
			lpObj->Leadership++;
			break;
	}
	// -----
	lpObj->LevelUpPoint--;
	gCalCharacter.gObjCalCharacter(lpObj->m_Index);
	// -----
	return true;
}

void gObjMonsterItemLoadScript(LPOBJ lpObj, char* script_file)
{
	int Token;
	int type;
	int index;
	int level;
	int op1;
	int op2;
	int op3;
	int incount;

	BYTE Socket[5];
	Socket[0] = 0xFF;
	Socket[1] = 0xFF;
	Socket[2] = 0xFF;
	Socket[3] = 0xFF;
	Socket[4] = 0xFF;
	
	SMDFile=fopen(script_file, "r");

	if (SMDFile == 0)
	{
		CLog.MsgBox("file open error %s", script_file);
		return;
	}
	incount=0;

	while ( true )
	{
		Token=GetToken();
		
		if (Token == 2)
		{
			break;
		}

		if (Token == 1)
		{
			type=TokenNumber;

			Token=GetToken();
			index=TokenNumber;

			Token=GetToken();
			level=TokenNumber;

			Token=GetToken();
			op1=TokenNumber;

			Token=GetToken();
			op2=TokenNumber;

			Token=GetToken();
			op3=TokenNumber;

			if (  gObjMonsterInventoryInsertItem(lpObj, type, index, level, op1, op2, op3,Socket)  != 0xFF)
			{
				incount++;

				if (incount > 75)
				{
					break;
				}
			}
		}
		
	}

	fclose(SMDFile);
}

BYTE gObjWarehouseInsertItem(LPOBJ lpObj, int type, int index, int level, int pos)
{
	CItem item;

	BYTE Socket[5];
	Socket[0] = 0xFF;
	Socket[1] = 0xFF;
	Socket[2] = 0xFF;
	Socket[3] = 0xFF;
	Socket[4] = 0xFF;

	int item_type = ItemGetNumberMake(type, index);
	item.m_Level =level;
	item.Convert(item_type, 0 , 0, 0, 0, 0, 0,Socket);
	lpObj->pWarehouse[pos] = item;
	return pos;
}
// Done 100%
bool gObjGuildMasterCapacity(LPOBJ lpObj)
{
	if (lpObj->Level < gGuildCreateLevel)
	{
		return false;
	}
	return true;
}
// Done 100%
void gObjNextExpCal(LPOBJ lpObj)
{
	/*if ( gUser.gObjIsMasteringLevel(lpObj) == true )
	{
		if( lpObj->MasterLevel < MAX_CHAR_MASTER_LEVEL )
		{
			lpObj->MLNextExp = gLevelExperience[200+lpObj->MasterLevel];
		}
		else
		{
		    lpObj->MLNextExp = 0;
			lpObj->MLExperience = 0;
		}
	}
	else
	{
		if ( lpObj->Level < HRB_CharMaxLevel )
		{
			lpObj->NextExp = gLevelExperience[lpObj->Level];
		}
		else
		{
			lpObj->NextExp = 0;
			lpObj->Experience = 0;
		}
	}*/

	lpObj->NextExp = gLevelExperience[lpObj->Level];
}

bool retResistance(LPOBJ lpObj, int Resistance_Type)
{
	BYTE r = lpObj->m_Resistance[Resistance_Type];

	if( r == 0xFF)
	{
		return true;
	}

	if ( r > 0 && lpObj->m_iPotionSoulTime > 0 && (Resistance_Type == 2 || Resistance_Type == 0 ))
	{
		r += r * 50 / 100;
	}

	if ( r  == 0 )
	{
		return false;
	}

	if (  (rand()%(r+1)) == 0 )
	{
		return false;
	}

	return true;
}

bool retCalcSkillResistance(LPOBJ lpTargetObj, BOOL isDouble)
{
	if ( isDouble != FALSE )
	{
		if ( (rand()%100) < 45 )
		{
			return true;
		}
	}
	else
	{
		if ( (rand()%100) < 40 )
		{
			return true;
		}
	}

	return false;
}

bool gObjAttackQ(LPOBJ lpObj)
{
	if ( ATTRIBUTE_RANGE(lpObj->m_Attribute) )
	{
		return false;
	}

	if ( lpObj->Class == 221 || lpObj->Class == 222 )
	{
		return false;
	}

#if(GS_CASTLE)
	if ( lpObj->Class == 277 || lpObj->Class == 283 )
	{
		if(g_CastleSiege.GetCastleState() != 7)
		{
			return false;
		}
	}
	
	if( lpObj->Class == 277 )
	{
		if( lpObj->m_btCsGateOpen == 1)
		{
			return false;
		}
	}
#endif

	if ( gUser.BloodCastleMapRange(lpObj->MapNumber) == true )
	{
		if ( lpObj->Type >= OBJ_MONSTER )
		{
			if ( lpObj->Class == 131 && g_BloodCastle.CheckMonsterKillSuccess(g_BloodCastle.GetBloodCastleIndex(lpObj->MapNumber)) == false)
			{
				return false;
			}

			if ( (((lpObj->Class - 132) < 0)?FALSE:((lpObj->Class-132) > 2)?FALSE:TRUE) !=FALSE && g_BloodCastle.CheckBossKillSuccess(g_BloodCastle.GetBloodCastleIndex(lpObj->MapNumber)) == false )
			{
				return false;
			}
		}
	}

	if ( lpObj->Class != 247 && lpObj->Class != 249 && lpObj->Class  >= 100 && lpObj->Class <= 110 )
	{
		return false;
	}

	if ( lpObj->m_State != 2 )
	{
		return false;
	}

	if( lpObj->Live == false )
	{
		return false;
	}

	if ( lpObj->Teleport == 1 )
	{
		return false;
	}

	return true;
}
// Need to remake this function
void gObjPlayerKiller(LPOBJ lpObj, LPOBJ lpTargetObj)
{
	if ( lpObj->Type != OBJ_USER )
	{
		return;
	}

	if ( gObjGetRelationShip(lpObj, lpTargetObj) == 2 )
	{
		if ( lpObj->lpGuild!= NULL && lpTargetObj->lpGuild != NULL )
		{
			CLog.LogAdd("[U.System][Rival][Player Kill] ( Killer [%s][%s] / %s / U:%d / R:%d ) vs ( Victim [%s][%s] / %s / U:%d / R:%d )",
				lpObj->AccountID, lpObj->Name, lpObj->lpGuild->Name, lpObj->lpGuild->iGuildUnion, lpObj->lpGuild->iGuildRival,
				lpTargetObj->AccountID, lpTargetObj->Name, lpTargetObj->lpGuild->Name, lpTargetObj->lpGuild->iGuildUnion, lpTargetObj->lpGuild->iGuildRival);
		}
		else
		{
			CLog.LogAdd("[U.System][Rival][Player Kill][★ERROR : Can't find GuildInfo] (  [%s][%s] ) vs ( [%s][%s] )",
				lpObj->AccountID,lpObj->Name,lpTargetObj->AccountID,lpTargetObj->Name);
		}
		return;
	}

	if ( gUser.ChaosCastleMapRange(lpTargetObj->MapNumber) == true  )
	{
		if ( g_ChaosCastle.GetCurrentState(g_ChaosCastle.GetChaosCastleIndex(lpTargetObj->MapNumber)) == 2 )
		{
			return;
		}
	}

#if(GS_CASTLE)
	if(lpTargetObj->MapNumber == MAP_INDEX_CASTLESIEGE)
	{
		if(g_CastleSiege.GetCastleState() == 7)
		{
			if(lpObj->m_btCsJoinSide != 0)
			{
				return;
			}
		}
		else if(lpObj->Y > 113)
		{
			if(g_CastleSiege.CheckCastleOwnerUnionMember(lpObj->m_Index) == TRUE)
			{
				return;
			}
		}
		
		if(lpTargetObj->m_PK_Level >= 6)
		{
			return;
		}
	}
#endif

	if ( Gens.IsGenPVP(lpObj,lpTargetObj) == true )
	{
		return;
	}
	if ( lpTargetObj->MapNumber == MAP_INDEX_VULCANROOM )
	{
		return;
	}

#if(!GS_CASTLE)
	if ( IT_MAP_RANGE(lpTargetObj->MapNumber) )
	{ 
		g_IllusionTemple.ProcessUserKiller(lpObj, lpTargetObj);
		return;
	}
#endif

	if((lpObj->Authority & 16) || (lpTargetObj->Authority & 16))
	{
		return;
	}

	for ( int n=0;n<MAX_SELF_DEFENSE;n++)
	{
		if ( lpTargetObj->SelfDefense[n] >= 0 )
		{
			if ( lpTargetObj->SelfDefense[n] == lpObj->m_Index )
			{
				return;
			}
		}
	}

	int iOriginPkLevel = lpObj->m_PK_Level;
	int iOriginPkCount = lpObj->m_PK_Count;

	if( gPKSystem.IncreaseLevel == 0 )
	{
		return;
	}

	if ( lpObj->m_PK_Level == 3 )
	{
		if(lpTargetObj->m_PK_Level == 3)
		{
			lpObj->m_PK_Count = 1;
		}
		else if(lpTargetObj->m_PK_Level < 3)
		{
			lpObj->m_PK_Count = 1;
		}
		else if(lpTargetObj->m_PK_Level == 4)
		{
			lpObj->m_PK_Count = 1;
		}
		else
		{
			if(lpTargetObj->m_PK_Level >= 6 && lpTargetObj->Level > 20)
			{
				lpObj->m_PK_Count--;

				if(lpObj->m_PK_Count < -3)
				{
					lpObj->m_PK_Count = (BYTE)-3;
					return;
				}
			}
			else
			{
				return;
			}
		}
	}
	else if( lpObj->m_PK_Level > 3)
	{
		if(lpTargetObj->m_PK_Level <= 4)
		{
			lpObj->m_PK_Count++;
			if(lpObj->m_PK_Count > 100)
			{
				lpObj->m_PK_Count = 100;
			}
		}
		else
		{
			return;
		}
	}
	else if( lpObj->m_PK_Level < 3)
	{
		if(lpTargetObj->m_PK_Level == 3)
		{
			lpObj->m_PK_Count = 1;
		}
		else if(lpTargetObj->m_PK_Level < 3)
		{
			lpObj->m_PK_Count = 1;
		}
		else if(lpTargetObj->m_PK_Level == 4)
		{
			lpObj->m_PK_Count = 1;
		}
		else
		{
			if(lpTargetObj->m_PK_Level >= 6 && lpTargetObj->Level > 20)
			{
				lpObj->m_PK_Count--;

				if(lpObj->m_PK_Count < -3)
				{
					lpObj->m_PK_Count = (BYTE)-3;
				}
			}
			else
			{
				return;
			}
		}
	}

	if(lpObj->m_PK_Count == 0)
	{
		lpObj->m_PK_Level = 3;
		lpObj->m_PK_Time = 0;
	}
	else if(lpObj->m_PK_Count == 1)
	{
		lpObj->m_PK_Level = 4;
		lpObj->m_PK_Time = 0;
	}
	else if(lpObj->m_PK_Count == 2)
	{
		lpObj->m_PK_Level = 5;
		lpObj->m_PK_Time = 0;
	}
	else if(lpObj->m_PK_Count >= 3)
	{
		lpObj->m_PK_Level = 6;
		lpObj->m_PK_Time = 0;
	}
	else if(lpObj->m_PK_Count <= -3)
	{
		lpObj->m_PK_Level = 1;
		lpObj->m_PK_Time = 0;
	}
	
	GCPkLevelSend(lpObj->m_Index, lpObj->m_PK_Level);

	if ( lpObj->m_PK_Level == 4 )
	{
		GCServerMsgStringSend(lMsg.Get(MSGGET(4, 113)), lpObj->m_Index, 1);
	}

	CLog.LogAdd("[%s][%s] PK Info Set (Killed Player) Origin(PkLevel:%d, PkCount:%d) Changed(PkLevel:%d, PkCount:%d)",
		lpObj->AccountID, lpObj->Name, iOriginPkLevel, iOriginPkCount, lpObj->m_PK_Level, lpObj->m_PK_Count);
}
// We have to remake this Function // Added Some Modified By WolF!
void gObjUserDie(LPOBJ lpObj, LPOBJ lpTargetObj)
{
    int count = 76;
    int itemdrop = 1;
    int number = 0;
    int dropresult = 0;

	if(lpObj->Type != OBJ_USER)
    {
        return;
    }

    gObjSetKillCount(lpObj->m_Index, 0);
    gObjUseSkill.RemoveAllCharacterInvalidMagicAndSkillState(lpObj);
    lpObj->m_iPotionBlessTime = 0;
    lpObj->m_iPotionSoulTime = 0;
	gSantaEvent.LittleSantaCancelBuffer(lpObj);
	for ( int i=0;i<7;i++ )
	{
		if ( lpObj->SpecialBuff[i] == 1 )
		{
			gSpecialItem.RemoveEffect(lpObj,i,lpObj->SpecialBuffItem[i]);
		}
	}

    lpObj->m_ViewSkillState[91] = 0;

	if(gObjTargetGuildWarCheck(lpObj, lpTargetObj) == 1)
    {
        return;
    }

	if(DS_MAP_RANGE(lpObj->MapNumber))
    {
        if(lpTargetObj->Type == OBJ_MONSTER)
        {
            LPMONSTER_ATTRIBUTE mAttr = gMAttr.GetAttr(lpTargetObj->Class);

            if(mAttr != 0)
            {
                CLog.LogAddC(TColor.Blue(), "USER", "[DevilSquare] Dead In DevilSquare [%s][%s][%s]", lpObj->AccountID, lpObj->Name, mAttr->m_Name);
            }
        }
        return;
    }

	if( gUser.BloodCastleMapRange(lpObj->MapNumber) == true )
    {
        g_BloodCastle.SetUserState(lpObj->m_Index, 1);

        if(g_BloodCastle.GetCurrentState(g_BloodCastle.GetBloodCastleIndex(lpObj->MapNumber)) == BC_STATE_PLAYING)
        {
            CLog.LogAddC(TColor.Blue(), "USER", "[Blood Castle] (%d) Try to drop Ultimate Weapon [%s][%s]", g_BloodCastle.GetBloodCastleIndex(lpObj->MapNumber), lpObj->AccountID, lpObj->Name);
            g_BloodCastle.SearchUserDropQuestItem(lpObj->m_Index);
        }
        else
        {
            CLog.LogAddC(TColor.Blue(), "USER", "[Blood Castle] (%d) Try to delete Ultimate Weapon [%s][%s]", g_BloodCastle.GetBloodCastleIndex(lpObj->MapNumber), lpObj->AccountID, lpObj->Name);
            g_BloodCastle.SearchUserDeleteQuestItem(lpObj->m_Index);
        }

		if(lpTargetObj->Type == OBJ_MONSTER)
        {
            LPMONSTER_ATTRIBUTE mAttr = gMAttr.GetAttr(lpTargetObj->Class);
            if(mAttr != 0)
            {
                CLog.LogAddC(TColor.Blue(), "USER", "[Blood Castle] (%d) Dead In Blood Castle, Killed by Monster [%s][%s][%s]", g_BloodCastle.GetBloodCastleIndex(lpObj->MapNumber), lpObj->AccountID, lpObj->Name, mAttr->m_Name);
                g_BloodCastle.SearchUserDropQuestItem(lpObj->m_Index);
            }
        }
        else if(lpTargetObj->Type == OBJ_USER)
        {
            CLog.LogAddC(TColor.Blue(), "USER", "[Blood Castle] (%d) Dead In Blood Castle, Killed by Other User [%s][%s][%s]", g_BloodCastle.GetBloodCastleIndex(lpObj->MapNumber), lpObj->AccountID, lpObj->Name, lpTargetObj->Name);
            g_BloodCastle.SearchUserDropQuestItem(lpObj->m_Index);
        }
        return;
	}
	else if( gUser.ChaosCastleMapRange(lpObj->MapNumber) == true )
	{
		return;
	}

#if(GS_CASTLE)
    if(g_CastleSiege.GetCastleState() == 7 && lpObj->MapNumber == MAP_INDEX_CASTLESIEGE)
        return;
#endif

	if((lpObj->Authority & 0x10) || (lpTargetObj->Authority & 0x10))
		return;

	if(lpTargetObj->Type == OBJ_MONSTER)
    {
        CLog.LogAddC(TColor.Blue(), "USER", "[%s][%s] Killed [%s][%s] Pos:(%d,%d,%d)", "Monster", lpTargetObj->Name, lpObj->AccountID, lpObj->Name, lpObj->MapNumber, lpObj->X, lpObj->Y);
    }
    else
    {
        CLog.LogAddC(TColor.Blue(), "USER", "[%s][%s] (PkLevel = %d) Killed [%s][%s] (PkLevel = %d) Pos:(%d,%d,%d)", lpTargetObj->AccountID, lpTargetObj->Name, lpTargetObj->m_PK_Level, lpObj->AccountID, lpObj->Name, lpObj->m_PK_Level, lpObj->MapNumber, lpObj->X, lpObj->Y);
//        CLog.LogAddC(TColor.Blue(), "USER", "Killed by User State Victim:[%s][%s], Murderer:[%s][%s], Dead State (Duel:%d, GuildWar:%d, SeldDefense:%d)", lpObj->AccountID, lpObj->Name, lpTargetObj->AccountID, lpTargetObj->Name, !!IsOnDuel(lpTargetObj->m_Index, lpObj->m_Index), !!gObjTargetGuildWarCheck(lpObj, lpTargetObj), !!gObjIsSelfDefense(lpTargetObj, lpObj->m_Index));
    }

	/*if(gPkLimitFree == 0)
    {
        if(lpObj->m_PK_Level == 2)
        {
            itemdrop = rand() % 32;
        }
        else if(lpObj->m_PK_Level == 3)
        {
            itemdrop = rand() % 16;
        }
        else if(lpObj->m_PK_Level == 4)
        {
            itemdrop = rand() % 8;
        }
        else if(lpObj->m_PK_Level == 5)
        {
            itemdrop = rand() % 4;
        }
        else if(lpObj->m_PK_Level >= 6)
        {
            itemdrop = rand() % 2;
        }
        else
        {
            itemdrop = rand() % 64;
        }
    }
	else
    {
        if(lpObj->m_PK_Level == 2)
        {
            itemdrop = rand() % 128;
        }
        else if(lpObj->m_PK_Level == 3)
        {
            itemdrop = rand() % 64;
        }
        else if(lpObj->m_PK_Level == 4)
        {
            itemdrop = rand() % 16;
        }
        else if(lpObj->m_PK_Level == 5)
        {
            itemdrop = rand() % 4;
        }
        else if(lpObj->m_PK_Level >= 6)
        {
            itemdrop = rand() % 2;
        }
        else
        {
            itemdrop = rand() % 256;
        }
    }

	if(gPkItemDrop == 0)
    {
        if(lpTargetObj->m_PK_Level >= 4 && gPkLimitFree == 0)
        {
            itemdrop = 1;
        }
    }
    if(gLanguage == 0)
    {
        if(lpTargetObj->Type == OBJ_USER && gPkLimitFree == 0)
        {
            itemdrop = 1;
        }
    }*/

	/*if(gObjCanItemTouch(lpObj, 0) == 0)
    {
        itemdrop = 1;
    }*/

	/*if(lpObj->Type == OBJ_USER && lpTargetObj->Type == OBJ_USER)
	{
		if( gDuel.AreOnDuel(lpObj, lpTargetObj) == true)
        {
            itemdrop = 1;
        }
	}*/

/*#if(GS_CASTLE)
	if(g_Crywolf.GetCrywolfState() == 4)
	{
		if(CRYWOLF_MAP_RANGE(lpObj->MapNumber))
		{
			itemdrop = 1;
		}
	}
#endif*/

	/*if(itemdrop == 0)
    {
        if(lpObj->m_PK_Level >= 4)
        {
            count = 24;
            while(count-- != 0)
            {
                number = rand() % 12;
                if(lpObj->pInventory[number].IsItem() == 1)
                {
                    if(g_kJewelOfHarmonySystem.IsStrengthenByJewelOfHarmony(&lpObj->pInventory[number]) == 0)
                    {
                        PMSG_ITEMTHROW lpMsg;
                        lpMsg.Ipos = number;
                        lpMsg.px = lpObj->X;
                        lpMsg.py = lpObj->Y;
                        if(CGItemDropRequest(&lpMsg, lpObj->m_Index, 1) == 1)
                        {
                            dropresult = 1;
                            break;
                        }
                    }
                }
            }
        }

		if(dropresult == 0)
        {
            count = INVENTORY_MAP_SIZE;
            while(count-- != 0)
            {
                number = rand() % INVENTORY_MAP_SIZE + 12;
                if(lpObj->pInventory[number].IsItem() == 1)
                {
                    if(g_kJewelOfHarmonySystem.IsStrengthenByJewelOfHarmony(&lpObj->pInventory[number]) == 0)
                    {
                        if(lpObj->pInventory[number].m_Type >= ITEMGET(13, 20) && (lpObj->pInventory[number].m_Level >= 1 && lpObj->pInventory[number].m_Level <= 2))
                        {
                            continue;
                        }
                        PMSG_ITEMTHROW lpMsg;
                        lpMsg.Ipos = number;
                        lpMsg.px = lpObj->X;
                        lpMsg.py = lpObj->Y;
                        if(CGItemDropRequest(&lpMsg, lpObj->m_Index, 1) == 1)
                        {
                            dropresult = 1;
                            CLog.LogAddC(TColor.Blue(), "[%s][%s] User Die Item Drop, ItemName:%s", lpObj->AccountID, lpObj->Name, lpObj->pInventory[number].GetName());
                            break;
                        }
					}
				}
			}
		}
	}*/
	
	if( lpObj->Type == OBJ_USER && lpTargetObj->Type == OBJ_USER )
	{
		if( gDuel.AreOnDuel(lpObj, lpTargetObj) == true )
		{
			int DuelRoom = gDuel.GetUserDuelRoom(lpTargetObj);
			// ----
			if ( gDuel.m_Rooms[DuelRoom].lpObj01 == lpTargetObj )
			{
				gDuel.m_Rooms[DuelRoom].btPoints01 += 1;
			}
			else if ( gDuel.m_Rooms[DuelRoom].lpObj02 == lpTargetObj )
			{
				gDuel.m_Rooms[DuelRoom].btPoints02 += 1;
			}
			lpTargetObj->m_btDuelScore += 1;
			gDuel.UpdateDuelScore(DuelRoom);
		}
		else if ( Gens.IsGenPVP(lpObj,lpTargetObj) == true )
		{
			Gens.GenAddAndDecContribution(lpObj,lpTargetObj);
		}
		return;
	}

	DWORD minexp = gLevelExperience[-1 + lpObj->Level];
    DWORD maxexp = gLevelExperience[lpObj->Level];
    DWORD subexp = 0;
    DWORD decexp = 0;
    DWORD decexprate = 0;

	if(lpObj->Level <= 10)
    {
        decexprate = 0;
    }

	else if(lpObj->Level <= 150)
    {
        if(lpObj->m_PK_Level == 2)
        {
            decexprate = 2;
        }
        else if(lpObj->m_PK_Level == 3)
        {
            decexprate = 3;
        }
        else if(lpObj->m_PK_Level == 4)
        {
            decexprate = 5;
        }
        else if(lpObj->m_PK_Level == 5)
        {
            decexprate = 6;
        }
        else if(lpObj->m_PK_Level >= 6)
        {
            decexprate = 7;
        }
        else
        {
            decexprate = 2;
        }
    }

	else if(lpObj->Level <= 220)
    {
        if(lpObj->m_PK_Level == 2)
        {
            decexprate = 1;
        }
        else if(lpObj->m_PK_Level == 3)
        {
            decexprate = 2;
        }
        else if(lpObj->m_PK_Level == 4)
        {
            decexprate = 4;
        }
        else if(lpObj->m_PK_Level == 5)
        {
            decexprate = 5;
        }
        else if(lpObj->m_PK_Level >= 6)
        {
            decexprate = 6;
        }
        else
        {
            decexprate = 1;
        }
    }
	else
    {
        if(lpObj->m_PK_Level == 2)
        {
            decexprate = 1;
        }
        else if(lpObj->m_PK_Level == 3)
        {
            decexprate = 1;
        }
        else if(lpObj->m_PK_Level == 4)
        {
            decexprate = 3;
        }
        else if(lpObj->m_PK_Level == 5)
        {
            decexprate = 4;
        }
        else if(lpObj->m_PK_Level >= 6)
        {
            decexprate = 5;
        }
        else
        {
            decexprate = 1;
        }
    }

	decexp = (maxexp - minexp) * decexprate / 100;

#if(GS_CASTLE)
    if(g_Crywolf.GetCrywolfState() == 4)
    {
        if(lpObj->MapNumber == MAP_INDEX_CRYWOLF_FIRSTZONE)
        {
            decexp = 0;
        }
    }
#endif

	subexp = lpObj->Experience - decexp;

	if(subexp < minexp)
    {
        subexp = minexp;
    }

	CLog.LogAddC(TColor.Blue(), "[%s][%s] Death reduces Experience %u - %d", lpObj->AccountID, lpObj->Name, subexp, decexp);
   
	DWORD submoney = 0;

	if(lpObj->Money > 0)
    {
        if(lpObj->Level > 10 && lpObj->Level <= 99)
        {
            submoney = (DWORD)(lpObj->Money) / 100;
        }
        else if(lpObj->Level >= 100 && lpObj->Level <= 199)
        {
            submoney = (DWORD)(lpObj->Money * 2) / 100;
        }
        else if(lpObj->Level > 200)
        {
            submoney = (DWORD)(lpObj->Money * 3) / 100;
        }
        lpObj->Money = (lpObj->Money - submoney);
        if(lpObj->Money < 0)
        {
            lpObj->Money = 0;
        }
        CLog.LogAddC(TColor.Blue(), lMsg.Get(574), lpObj->AccountID, lpObj->Name, lpObj->Money, submoney);
    }

	if(lpObj->WarehouseMoney > 0)
    {
        if(lpObj->Level > 10 && lpObj->Level <= 99)
        {
            submoney = (DWORD)(lpObj->WarehouseMoney) / 100;
        }
        else if(lpObj->Level >= 100 && lpObj->Level <= 199)
        {
            submoney = (DWORD)(lpObj->WarehouseMoney * 2) / 100;
        }
        else if(lpObj->Level > 200)
        {
            submoney = (DWORD)(lpObj->WarehouseMoney * 3) / 100;
        }

        lpObj->WarehouseMoney = lpObj->WarehouseMoney - submoney;

        if(lpObj->WarehouseMoney < 0)
        {
            lpObj->WarehouseMoney = 0;
        }

        GDSetWarehouseMoney(lpObj->m_Index);

        CLog.LogAddC(TColor.Blue(), lMsg.Get(575), lpObj->AccountID, lpObj->Name, lpObj->WarehouseMoney, submoney);
    }

    lpObj->Experience = subexp;
}

void gObjInterfaceTimeCheck(LPOBJ lpObj)
{
	int tObjNum;
	BOOL TimeCheck = FALSE;

	if ( lpObj->m_IfState.use == 0 )
	{
		return;
	}

	if ( (GetTickCount() - lpObj->m_InterfaceTime) < 5000 )
	{
		return;
	}

	if ( lpObj->m_IfState.state == 1 )
	{
		return;
	}

	tObjNum = lpObj->TargetNumber;

	if ( tObjNum < 0 )
	{
		return;
	}

	if ( lpObj->m_IfState.type == 1 )
	{
		CGTradeResult(lpObj->m_Index, 3);
		CGTradeResult(tObjNum, 3);

		GCServerMsgStringSend(lMsg.Get(MSGGET(4, 114)), tObjNum, 1);
		TimeCheck = TRUE;
	}
	else
	{
		if ( lpObj->m_IfState.type == 2 )
		{
			lpObj->PartyTargetUser = -1;
			gObj[tObjNum].PartyTargetUser = -1;
			GCResultSend(tObjNum, 0x41, FALSE );
			TimeCheck = TRUE;
		}
	}

	if ( TimeCheck != FALSE )
	{
		lpObj->m_IfState.use = 0;
		lpObj->TargetNumber = -1;
		gObj[tObjNum].m_IfState.use = 0;
		gObj[tObjNum].TargetNumber = -1;
	}

}
// We have to remake this Function
void gObjSpriteDamage(LPOBJ lpObj, int damage)
{
	if ( lpObj->Type != OBJ_USER )
	{
		return;
	}

	int send_dur = 0;

	if ( lpObj->pInventory[8].IsItem() == FALSE )
	{
		return;
	}

	float fN = 10.0f;
	CItem * sprite = &lpObj->pInventory[8];
	float fdamage = damage;
	switch ( sprite->m_Type )
	{
	case ITEMGET(13,0): // Angel
		fdamage = (damage*3)/10.0f;
		fdamage /= fN;
		sprite->m_Durability -= fdamage;
		send_dur = 1;
		break;
	case ITEMGET(13,1): // Satan
		fdamage = (damage*2)/10.0f;
		fdamage /= fN;
		sprite->m_Durability -= fdamage;
		send_dur = 1;
		break;
	case ITEMGET(13,2): // Uniria
		fdamage = (damage)/10.0f;
		fdamage /= 10.0f;
		sprite->m_Durability -= fdamage;
		send_dur = 1;
		break;
	case ITEMGET(13,3): // Dinorant
		fdamage = (damage)/20.0f;
		fdamage /= 10.0f;
		sprite->m_Durability -= fdamage;
		send_dur = 1;
		break;
	case ITEMGET(13,4): // Dark Spirit
		if ( sprite->m_Durability < 1.0f )
		{
			return;
		}

		fdamage = (damage*2)/10.0f;
		fdamage /= 10.0f;
		fdamage += 1.0f;
		sprite->m_DurabilitySmall += (int)fdamage;

		if ( sprite->m_DurabilitySmall > 1500 )
		{
			sprite->m_Durability -= 1.0f;
			sprite->m_DurabilitySmall = 0;
			send_dur = 1;

			if ( sprite->m_Durability < 1.0f )
			{
				sprite->m_Durability = 0;

				int iPetExperience = lpObj->pInventory[1].m_PetItem_Exp;

				if ( sprite->DecPetItemExp(10) != FALSE )
				{
					CLog.LogAdd("[%s][%s][PetItemExpDown] [%s] Level:[%d]Exp:[%d]DecExp[%d]",
						lpObj->AccountID, lpObj->Name, lpObj->pInventory[8].GetName(),
						lpObj->pInventory[8].m_PetItem_Level, lpObj->pInventory[8].m_PetItem_Exp,
						iPetExperience - lpObj->pInventory[1].m_PetItem_Exp);
					CDarkSpirit::SendLevelmsg(lpObj->m_Index, 8, 1, (BYTE)-1);
				}

				CLog.LogAdd("[%s][%s] 다크호스의뿔 Item is Broken because durability is exhausted [%d]",
						lpObj->AccountID,lpObj->Name,lpObj->pInventory[8].m_Number);
			}

			GCItemDurSend(lpObj->m_Index, 8, sprite->m_Durability, 0);
		}
	
		return;
		break;
	case ITEMGET(13,64):
		fdamage = (damage)/20.0f;
		fdamage /= 10.0f;
		sprite->m_Durability -= fdamage;
		send_dur = 1;
		break;
	case ITEMGET(13,65):
		fdamage = (damage)/20.0f;
		fdamage /= 10.0f;
		sprite->m_Durability -= fdamage;
		send_dur = 1;
		break;
	case ITEMGET(13,67):
		fdamage = (damage)/20.0f;
		fdamage /= 10.0f;
		sprite->m_Durability -= fdamage;
		send_dur = 1;
		break;
	case ITEMGET(13,80):
		fdamage = (damage)/20.0f;
		fdamage /= 10.0f;
		sprite->m_Durability -= fdamage;
		send_dur = 1;
		break;
	case ITEMGET(13,123):
		fdamage = (damage)/20.0f;
		fdamage /= 10.0f;
		sprite->m_Durability -= fdamage;
		send_dur = 1;
		break;
	case  ITEMGET(13,37):
		if ( sprite->m_Durability < 1.0f )
		{
			return;
		}

		int iMaxDurSmall = gPets.FenrirMaxDurSmall[lpObj->Class];
		int iMultipleDmg = 2;

		fdamage = damage * iMultipleDmg / 10.0f;
		fdamage /= 10.0f;
		fdamage += 1.0f;

		sprite->m_DurabilitySmall += (int)fdamage;

		if(sprite->m_DurabilitySmall > iMaxDurSmall)
		{
			sprite->m_Durability -= 1.0f;
			sprite->m_DurabilitySmall = 0;

			if(sprite->m_Durability < 1.0f)
			{
				sprite->m_Durability = 0;
			}

			send_dur = 1;
		}
		break;
	}

	if ( send_dur != FALSE )
	{
		GCItemDurSend(lpObj->m_Index, 8, sprite->m_Durability, 0);

		if ( sprite->m_Durability < 1.0f )
		{
			int itemnum = sprite->m_Type;

			CLog.LogAdd(lMsg.Get(MSGGET(2, 10)), lpObj->AccountID, lpObj->Name, lpObj->pInventory[8].GetName(), lpObj->pInventory[8].m_Level);
			gObjInventoryDeleteItem(lpObj->m_Index, 8);
			GCInventoryItemDeleteSend(lpObj->m_Index, 8, 0);
			gObjMakePreviewCharSet(lpObj->m_Index);
			GCEquipmentChange(lpObj->m_Index, 8);
			
			if ( gUser.BloodCastleMapRange(lpObj->MapNumber) == true || gUser.ChaosCastleMapRange(lpObj->MapNumber) == true )
			{
				if ( itemnum == ITEMGET(13,0) || itemnum == ITEMGET(13,1) || itemnum == ITEMGET(13,64) || itemnum == ITEMGET(13,65) )
				{
					int pos = -1;

					for ( int i=MAX_PLAYER_EQUIPMENT;i<MAIN_INVENTORY_SIZE;i++)
					{
						if ( lpObj->pInventory[i].IsItem() == TRUE )
						{
							if ( lpObj->pInventory[i].m_Type == itemnum )
							{
								pos = i;
								break;
							}
						}
					}

					if ( pos != -1 )
					{
						BYTE ItemInfo[MAX_ITEM_INFO];
						BOOL DurSend;
						BOOL DurTargetSend;

						ItemByteConvert(ItemInfo, lpObj->pInventory[pos]);
						gObjInventoryMoveItem(lpObj->m_Index, pos, 8, DurSend, DurTargetSend, 0, 0, ItemInfo);
						GCItemMoveResultSend(lpObj->m_Index, 0, 8, ItemInfo);
						GCInventoryItemDeleteSend(lpObj->m_Index, pos, 0);
						GCServerMsgStringSend(lMsg.Get(MSGGET(4, 158)), lpObj->m_Index, 1);
					}
				}
			}
				
			if ( lpObj->MapNumber == MAP_INDEX_ICARUS && itemnum == ITEMGET(13,3) || lpObj->MapNumber == MAP_INDEX_ICARUS && itemnum == ITEMGET(13,37))
			{
				int pos = -1;

				for ( int i=MAX_PLAYER_EQUIPMENT;i<MAIN_INVENTORY_SIZE;i++)
				{
					if ( lpObj->pInventory[i].IsItem() == TRUE )
					{
						if ( lpObj->pInventory[i].m_Type == ITEMGET(13,3) )
						{
							pos = i;
							break;
						}
					}
				}

				if ( pos == -1 )
				{
					if ( lpObj->pInventory[7].IsItem() == FALSE )
					{
						gObjMoveGate(lpObj->m_Index, 22);
					}
				}
				else
				{
					BYTE ItemInfo[MAX_ITEM_INFO];
					BOOL DurSend;
					BOOL DurTargetSend;

					ItemByteConvert(ItemInfo, lpObj->pInventory[pos]);
					gObjInventoryMoveItem(lpObj->m_Index, pos, 8, DurSend, DurTargetSend, 0, 0, ItemInfo);
					GCItemMoveResultSend(lpObj->m_Index, 0, 8, ItemInfo);
					GCInventoryItemDeleteSend(lpObj->m_Index, pos, 0);
					GCServerMsgStringSend(lMsg.Get(MSGGET(4, 158)), lpObj->m_Index, 1);
				}
			}
			gCalCharacter.gObjCalCharacter(lpObj->m_Index);
		}

	}
}
// We have to remake this Function
void gObjSecondDurDown(LPOBJ lpObj)
{
	lpObj->m_TimeCount +=1;

	int ret = 0;
	int reCalCharacter = 0;

	if(lpObj->m_Change >= 0)
	{
		gObjChangeDurProc(lpObj);
	}

	BYTE attr = MapC[lpObj->MapNumber].GetAttr(lpObj->X,lpObj->Y);

	if(lpObj->m_TimeCount % 10 != 0)
	{
		return;
	}

	if(lpObj->pInventory[7].IsItem() == 1 )
	{
		ret = lpObj->pInventory[7].DurabilityDown(1,lpObj->m_Index);

		if(ret != 0)
		{
			GCItemDurSend2(lpObj->m_Index,7,lpObj->pInventory[7].m_Durability,0);
		}

		if(ret == 2)
		{
			reCalCharacter = 1;
		}
	}

	if((attr&1)==0)
	{
		if(lpObj->pInventory[10].IsItem() == 1)
		{
			if(lpObj->pInventory[10].m_Type == ITEMGET(13,20) && lpObj->pInventory[10].m_Level == 0)
			{
				ret = lpObj->pInventory[10].DurabilityDown(70,lpObj->m_Index);
				if(ret != 0)
				{
					GCItemDurSend2(lpObj->m_Index,10,lpObj->pInventory[10].m_Durability,0);
				}

				if(ret == 2)
				{
					reCalCharacter = 1;
				}
			}
			else if(lpObj->pInventory[10].m_Type == ITEMGET(13,38))
			{
				ret = lpObj->pInventory[10].DurabilityDown(63,lpObj->m_Index);
				if(ret != 0)
				{
					GCItemDurSend2(lpObj->m_Index,10,lpObj->pInventory[10].m_Durability,0);
				}
			}
			else if(lpObj->pInventory[10].m_Type != ITEMGET(13,10))
			{
				ret = lpObj->pInventory[10].DurabilityDown(1,lpObj->m_Index);
				if(ret != 0)
				{
					GCItemDurSend2(lpObj->m_Index,10,lpObj->pInventory[10].m_Durability,0);
				}

				if(ret == 2)
				{
					reCalCharacter = 1;
				}
			}
		}

		if(lpObj->pInventory[11].IsItem() == 1)
		{
			if(lpObj->pInventory[11].m_Type == ITEMGET(13,20) && lpObj->pInventory[11].m_Level == 0)
			{
				ret = lpObj->pInventory[11].DurabilityDown(70,lpObj->m_Index);
				if(ret != 0)
				{
					GCItemDurSend2(lpObj->m_Index,11,lpObj->pInventory[11].m_Durability,0);
				}

				if(ret == 2)
				{
					reCalCharacter = 1;
				}
			}
			else if(lpObj->pInventory[11].m_Type == ITEMGET(13,38))
			{
				ret = lpObj->pInventory[11].DurabilityDown(63,lpObj->m_Index);
				if(ret != 0)
				{
					GCItemDurSend2(lpObj->m_Index,11,lpObj->pInventory[11].m_Durability,0);
				}
			}
			else if(lpObj->pInventory[11].m_Type != ITEMGET(13,10))
			{
				ret = lpObj->pInventory[11].DurabilityDown(1,lpObj->m_Index);
				if(ret != 0)
				{
					GCItemDurSend2(lpObj->m_Index,11,lpObj->pInventory[11].m_Durability,0);
				}

				if(ret == 2)
				{
					reCalCharacter = 1;
				}
			}
		}

		if(lpObj->pInventory[9].IsItem() == 1)
		{
			if(lpObj->pInventory[9].m_Type != ITEMGET(13,10))
			{
				ret = lpObj->pInventory[9].DurabilityDown(1,lpObj->m_Index);

				if(ret != 0)
				{
					GCItemDurSend2(lpObj->m_Index,9,lpObj->pInventory[9].m_Durability,0);
				}

				if(ret == 2)
				{
					reCalCharacter = 1;
				}
			}
		}
	}

	if(reCalCharacter != 0)
	{
		gCalCharacter.gObjCalCharacter(lpObj->m_Index);
	}
}
// We have to remake this Function
void gObjChangeDurProc(LPOBJ lpObj)
{

	if(lpObj->pInventory[10].IsItem() == 1 && lpObj->pInventory[10].m_Type == ITEMGET(13,10))
	{
		int m_Durability = lpObj->pInventory[10].m_Durability;

		lpObj->pInventory[10].m_Durability -= (float)0.02;

		if(m_Durability > (int)lpObj->pInventory[10].m_Durability)
		{
			if(lpObj->pInventory[10].m_Durability < 1)
			{
				lpObj->pInventory[10].m_Durability = 0;
			}

			BYTE dur = lpObj->pInventory[10].m_Durability;

			GCItemDurSend(lpObj->m_Index,10,dur,0);

			if(dur == 0)
			{
				gObjUseSkill.SkillChangeUse(lpObj->m_Index);
				CLog.LogAdd(lMsg.Get(544),lpObj->AccountID,lpObj->Name,lpObj->pInventory[10].GetName(),lpObj->pInventory[10].m_Level,dur);
				lpObj->pInventory[10].Clear();
				GCInventoryItemDeleteSend(lpObj->m_Index,10,0);
			}
		}
	}
	else if(lpObj->pInventory[11].IsItem() == 1 && lpObj->pInventory[11].m_Type == ITEMGET(13,10))
	{
		int m_Durability = lpObj->pInventory[11].m_Durability;

		lpObj->pInventory[11].m_Durability -= (float)0.02;

		if(m_Durability > (int)lpObj->pInventory[11].m_Durability)
		{
			if(lpObj->pInventory[11].m_Durability < 1)
			{
				lpObj->pInventory[11].m_Durability = 0;
			}

			BYTE dur = lpObj->pInventory[11].m_Durability;

			GCItemDurSend(lpObj->m_Index,11,dur,0);

			if(dur == 0)
			{
				gObjUseSkill.SkillChangeUse(lpObj->m_Index);
				CLog.LogAdd(lMsg.Get(544),lpObj->AccountID,lpObj->Name,lpObj->pInventory[10].GetName(),lpObj->pInventory[11].m_Level,dur);
				lpObj->pInventory[11].Clear();
				GCInventoryItemDeleteSend(lpObj->m_Index,11,0);
			}
		}
	}
	else if(lpObj->pInventory[10].IsItem() == 1 && lpObj->pInventory[10].m_Type == ITEMGET(13,39))
	{
		int m_Durability = lpObj->pInventory[10].m_Durability;

		lpObj->pInventory[10].m_Durability -= (float)0.02;

		if(m_Durability > (int)lpObj->pInventory[10].m_Durability)
		{
			if(lpObj->pInventory[10].m_Durability < 1)
			{
				lpObj->pInventory[10].m_Durability = 0;
			}

			BYTE dur = lpObj->pInventory[10].m_Durability;

			GCItemDurSend(lpObj->m_Index,10,dur,0);

			if(dur == 0)
			{
				gObjUseSkill.SkillChangeUse(lpObj->m_Index);
				CLog.LogAdd(lMsg.Get(544),lpObj->AccountID,lpObj->Name,lpObj->pInventory[10].GetName(),lpObj->pInventory[10].m_Level,dur);
				lpObj->pInventory[10].Clear();
				GCInventoryItemDeleteSend(lpObj->m_Index,10,0);
			}
		}
	}
	else if(lpObj->pInventory[11].IsItem() == 1 && lpObj->pInventory[11].m_Type == ITEMGET(13,39))
	{
		int m_Durability = lpObj->pInventory[11].m_Durability;

		lpObj->pInventory[11].m_Durability -= (float)0.02;

		if(m_Durability > (int)lpObj->pInventory[11].m_Durability)
		{
			if(lpObj->pInventory[11].m_Durability < 1)
			{
				lpObj->pInventory[11].m_Durability = 0;
			}

			BYTE dur = lpObj->pInventory[11].m_Durability;

			GCItemDurSend(lpObj->m_Index,11,dur,0);

			if(dur == 0)
			{
				gObjUseSkill.SkillChangeUse(lpObj->m_Index);
				CLog.LogAdd(lMsg.Get(544),lpObj->AccountID,lpObj->Name,lpObj->pInventory[10].GetName(),lpObj->pInventory[11].m_Level,dur);
				lpObj->pInventory[11].Clear();
				GCInventoryItemDeleteSend(lpObj->m_Index,11,0);
			}
		}
	}
}

void gObjPenaltyDurDown(LPOBJ lpObj, LPOBJ lpTargetObj)	// Functio n Not USed
{
	int ret = 0;
	int reCalCharacter = 0;

	int irand = rand()%2;

	CItem * Weapon = &lpObj->pInventory[irand];
	int decdur = 1;

	switch(lpObj->Class)
	{
	case CLASS_WIZARD:	decdur = lpObj->Level - (lpTargetObj->Level * 38) / 20;	break;
	case CLASS_KNIGHT:	decdur = lpObj->Level - (lpTargetObj->Level * 37) / 20;	break;
	case CLASS_ELF:		decdur = lpObj->Level - (lpTargetObj->Level * 36) / 20;	break;
	case CLASS_MAGUMSA:	decdur = lpObj->Level - (lpTargetObj->Level * 37) / 20;	break;
	case CLASS_DARKLORD:decdur = lpObj->Level - (lpTargetObj->Level * 37) / 20;	break;
	case CLASS_SUMMONER:decdur = lpObj->Level - (lpTargetObj->Level * 38) / 20;	break;
	case CLASS_FIGHTER:	decdur = lpObj->Level - (lpTargetObj->Level * 38) / 20;	break;
	default : break;
	}

	decdur /= 10;

	if(Weapon->m_Type != ITEMGET(4,7) && Weapon->m_Type != ITEMGET(4,15))
	{
		if(Weapon->m_Type < ITEMGET(6,0) && Weapon->m_Type >= ITEMGET(7,0) )
		{
			if ( Weapon->IsItem() )
			{
				ret = Weapon->DurabilityDown2(decdur,lpObj->m_Index);
			}
		}
		else
		{
			CItem * Weapon = &lpObj->pInventory[irand];

			if(Weapon->IsItem())
			{
				ret = Weapon->DurabilityDown2(decdur,lpObj->m_Index);
			}
		}
	}

	if(ret != 0)
	{
		GCItemDurSend2(lpObj->m_Index,irand,lpObj->pInventory[irand].m_Durability,0);
	}

	if(ret == 2)
	{
		reCalCharacter = 1;
	}

	int item_num[7] = {0,1,2,3,4,5,6};
	int item_pos = rand()%7;

	CItem * DurItem = &lpObj->pInventory[item_num[item_pos]];

	if(DurItem->m_Type != ITEMGET(4,7) && DurItem->m_Type != ITEMGET(4,15))
	{
		if(item_pos < 2)
		{
			if(DurItem->m_Type >= ITEMGET(6,0) && DurItem->m_Type < ITEMGET(7,0))
			{
				ret = DurItem->DurabilityDown2(decdur,lpObj->m_Index);
	
				if(ret != 0)
				{
					GCItemDurSend2(lpObj->m_Index,item_num[item_pos],DurItem->m_Durability,0);
	
					if(ret == 2)
					{
						reCalCharacter = 1;
					}
				}
			}
		}
		else
		{
			ret = DurItem->DurabilityDown2(decdur,lpObj->m_Index);

			if(ret != 0)
			{
				GCItemDurSend2(lpObj->m_Index,item_num[item_pos],DurItem->m_Durability,0);

				if(ret == 2)
				{
					reCalCharacter = 1;
				}
			}
		}
	}

	CItem * Wing = &lpObj->pInventory[7];

	if(Wing->IsItem())
	{
		ret = DurItem->DurabilityDown2(decdur,lpObj->m_Index);

		if(ret != 0)
		{
			GCItemDurSend2(lpObj->m_Index,item_num[item_pos],DurItem->m_Durability,0);

			if(ret == 2)
			{
				reCalCharacter = 1;
			}
		}
	}

	if(reCalCharacter != 0)
	{
		gCalCharacter.gObjCalCharacter(lpObj->m_Index);
	}
}

void gObjWeaponDurDown(LPOBJ lpObj, LPOBJ lpTargetObj, int type)
{
	if(lpObj->Type == OBJ_MONSTER || lpObj->Type == OBJ_NPC)
	{
		return;
	}

	int itargetdefence = 0;
	CItem * Right = &lpObj->pInventory[0];
	CItem * Left = &lpObj->pInventory[1];
	int ret = 0;

	if(type == 0)
	{

		itargetdefence = lpTargetObj->m_Defense;
		itargetdefence += lpTargetObj->m_iSkillNPCDefense;

		if((lpObj->Class == CLASS_KNIGHT  || lpObj->Class == CLASS_MAGUMSA  || lpObj->Class == CLASS_DARKLORD || lpObj->Class == CLASS_FIGHTER )
			&& (Right->m_Type >= 0 && Right->m_Type < ITEMGET(4,0) 
				&& Left->m_Type >= 0 && Left->m_Type < ITEMGET(4,0)))
		{
			if(rand()%2==0)
			{
				ret = Left->NormalWeaponDurabilityDown(itargetdefence,lpObj->m_Index);
				
				if(ret != 0)
				{
					GCItemDurSend(lpObj->m_Index,1,Left->m_Durability,0);
					if(ret == 2)
					{
						gCalCharacter.gObjCalCharacter(lpObj->m_Index);
					}
				}
			}
			else
			{
				ret = Right->NormalWeaponDurabilityDown(itargetdefence,lpObj->m_Index);
				if(ret != 0)
				{
					GCItemDurSend(lpObj->m_Index,0,Right->m_Durability,0);
					if(ret==2)
					{
						gCalCharacter.gObjCalCharacter(lpObj->m_Index);
					}
				}
			}

			return;
		}
		else if(Left->m_Type >= ITEMGET(4,0) && Left->m_Type < ITEMGET(4,7) || Left->m_Type == ITEMGET(4,20) || (Left->m_Type >= ITEMGET(4,21) && Left->m_Type <= ITEMGET(4,23)))
		{
			ret = Left->BowWeaponDurabilityDown(itargetdefence,lpObj->m_Index);

			if(ret != 0)
			{
				GCItemDurSend(lpObj->m_Index,1,Left->m_Durability,0);
				if(ret==2)
				{
					gCalCharacter.gObjCalCharacter(lpObj->m_Index);
				}
			}
		}
		else if(Right->m_Type >= ITEMGET(4,8) && Right->m_Type < ITEMGET(4,15) || Right->m_Type >= ITEMGET(4,16) && Right->m_Type < ITEMGET(5,0))
		{
			ret = Right->BowWeaponDurabilityDown(itargetdefence,lpObj->m_Index);

			if(ret != 0)
			{
				GCItemDurSend(lpObj->m_Index,0,Right->m_Durability,0);
				if(ret==2)
				{
					gCalCharacter.gObjCalCharacter(lpObj->m_Index);
				}
			}
		}
		else if(Right->m_Type >= ITEMGET(0,0) && Right->m_Type < ITEMGET(4,0))
		{
			ret = Right->NormalWeaponDurabilityDown(itargetdefence,lpObj->m_Index);

			if(ret != 0)
			{
				GCItemDurSend(lpObj->m_Index,0,Right->m_Durability,0);
				if(ret==2)
				{
					gCalCharacter.gObjCalCharacter(lpObj->m_Index);
				}
			}
		}
		else if(Right->m_Type >= ITEMGET(5,0) && Right->m_Type < ITEMGET(6,0))
		{
			ret = Right->StaffWeaponDurabilityDown(itargetdefence,lpObj->m_Index);

			if(ret != 0)
			{
				GCItemDurSend(lpObj->m_Index,0,Right->m_Durability,0);
				if(ret==2)
				{
					gCalCharacter.gObjCalCharacter(lpObj->m_Index);
				}
			}
		}
	return;
	}


	if(lpTargetObj->Type == OBJ_MONSTER || lpTargetObj->Type == OBJ_NPC)
	{
		itargetdefence = lpTargetObj->m_MagicDefense;
	}
	else
	{
		itargetdefence = lpTargetObj->m_Defense;
	}

	itargetdefence += lpTargetObj->m_iSkillNPCDefense;

	if(Right->m_Type >= ITEMGET(5,0) && Right->m_Type < ITEMGET(6,0))
	{
		ret = Right->StaffWeaponDurabilityDown(itargetdefence,lpObj->m_Index);
		if(ret != 0)
		{
			GCItemDurSend(lpObj->m_Index,0,Right->m_Durability,0);
			if(ret==2)
			{
				gCalCharacter.gObjCalCharacter(lpObj->m_Index);
			}
		}
	}
}

void gObjArmorRandomDurDown(LPOBJ lpObj, LPOBJ lpAttackObj)
{
	int item_num[7] = {0,1,2,3,4,5,6};
	int item_pos = rand()%7;

	if(lpObj->Type != OBJ_USER)
	{
		return;
	}

	CItem * DurItem = &lpObj->pInventory[item_num[item_pos]];
	int damagemin = lpAttackObj->m_AttackDamageMin;
	int ret;

	if(item_pos < 2)
	{
		if(DurItem->m_Type >= ITEMGET(6,0) && DurItem->m_Type < ITEMGET(7,0))
		{
			ret = DurItem->ArmorDurabilityDown(damagemin,lpObj->m_Index);

			if(ret != 0)
			{
				GCItemDurSend(lpObj->m_Index,item_num[item_pos],DurItem->m_Durability,0);
				if(ret == 2)
				{
					gCalCharacter.gObjCalCharacter(lpObj->m_Index);
				}
			}
			return;
		}
	}
	else
	{
		if(DurItem->m_Type != ITEMGET(4,7) && DurItem->m_Type != ITEMGET(4,15))
		{
			ret = DurItem->ArmorDurabilityDown(damagemin,lpObj->m_Index);

			if(ret != 0)
			{
				GCItemDurSend(lpObj->m_Index,item_num[item_pos],DurItem->m_Durability,0);
				if(ret == 2)
				{
					gCalCharacter.gObjCalCharacter(lpObj->m_Index);
				}
			}
		}
	}
}

bool gObjIsSelfDefense(LPOBJ lpObj, int aTargetIndex)
{
	if ( !OBJMAX_RANGE(aTargetIndex) )
	{
		return false;
	}

	for ( int n=0;n<MAX_SELF_DEFENSE;n++)
	{
		if ( lpObj->SelfDefense[n] >= 0 )
		{
			if ( lpObj->SelfDefense[n] == aTargetIndex )
			{
				return true;
			}
		}
	}

	return false;
}

void gObjCheckSelfDefense(LPOBJ lpObj, int aTargetIndex)
{
	if(!OBJMAX_RANGE(aTargetIndex))
	{
		return;
	}

	if(gObj[aTargetIndex].m_PK_Level > 4)
	{
		return;
	}

	for(int n = 0; n < MAX_SELF_DEFENSE;n++)
	{
		if(gObj[aTargetIndex].SelfDefense[n] >= 0)
		{
			if(gObj[aTargetIndex].SelfDefense[n]==lpObj->m_Index)
			{
				return;
			}
		}
	}

	int blank = -1;

	for( int n = 0; n < MAX_SELF_DEFENSE;n++)
	{
		if(lpObj->SelfDefense[n] >= 0)
		{
			if(lpObj->SelfDefense[n] == aTargetIndex)
			{
				lpObj->SelfDefenseTime[n] = GetTickCount()+30000;
				return;
			}
		}
		else
		{
			blank = n;
		}
	}

	if(blank < 0)
	{
		CLog.LogAdd(lMsg.Get(523));
		return;
	}

	lpObj->MySelfDefenseTime = GetTickCount();
	lpObj->SelfDefense[blank] = aTargetIndex;
	lpObj->SelfDefenseTime[blank] = GetTickCount() + 30000;

	char szTemp[64];

	wsprintf(szTemp,lMsg.Get(1114),lpObj->Name,gObj[aTargetIndex].Name);

	GCServerMsgStringSend(szTemp,lpObj->m_Index,1);
	GCServerMsgStringSend(szTemp,aTargetIndex,1);

	CLog.LogAdd("[%s][%s] Set SelfDefence [%s][%s]",lpObj->AccountID,lpObj->Name,gObj[aTargetIndex].AccountID,gObj[aTargetIndex].Name);
}

void gObjTimeCheckSelfDefense(LPOBJ lpObj)
{
	char szTemp[64];

	for ( int n=0;n<MAX_SELF_DEFENSE;n++)
	{
		if ( lpObj->SelfDefense[n] >= 0 )
		{
			if ( GetTickCount() > lpObj->SelfDefenseTime[n] )
			{
				wsprintf(szTemp, lMsg.Get(MSGGET(4, 91)), lpObj->Name);
				GCServerMsgStringSend(szTemp, lpObj->m_Index, 1);
				GCServerMsgStringSend(szTemp, lpObj->SelfDefense[n], 1);
				lpObj->SelfDefense[n] = -1;
				
				CLog.LogAdd("[%s][%s] ReSet SelfDefence [%s][%s]",
					lpObj->AccountID, lpObj->Name,
					gObj[lpObj->SelfDefense[n]].AccountID, gObj[lpObj->SelfDefense[n]].Name);
			}
		}
	}
}

BOOL gObjAttack(LPOBJ lpObj, LPOBJ lpTargetObj, CMagicInf * lpMagic, BOOL magicsend, BYTE MSBFlag, int AttackDamage, BOOL bCombo)
{
	return gclassObjAttack.Attack(lpObj, lpTargetObj, lpMagic, magicsend, MSBFlag, AttackDamage, bCombo);
}

void gObjMonsterExpDivision(LPOBJ lpMonObj, LPOBJ lpObj, int AttackDamage, int MSBFlag)
{

	gObjMonsterHitDamageUserDel(lpMonObj);
	lpMonObj->Money = 0;


	int tObjNum;
	int exp;
	LPOBJ lpTargetObj;

	for(int n = 0; n < MAX_ST_HIT_DAMAGE;n++)
	{
		if(lpMonObj->sHD[n].number >= 0)
		{
			tObjNum = lpMonObj->sHD[n].number;
			lpTargetObj = &gObj[tObjNum];

			if(lpMonObj->sHD[n].HitDamage > 0)
			{
				bool bSendExp = 1;

				exp = gObjMonsterExpSingle(lpTargetObj,lpMonObj,lpMonObj->sHD[n].HitDamage,lpMonObj->MaxLife,bSendExp);

				if(exp > 0)
				{
					if(bSendExp == 1)
					{
						if(lpTargetObj->m_Index == lpObj->m_Index)
						{
							if(lpObj->m_wExprienceRate > 0)
							{
								GCKillPlayerExpSend(lpTargetObj->m_Index,lpMonObj->m_Index,exp,AttackDamage,MSBFlag);
							}
						}
						else if(lpTargetObj->m_wExprienceRate > 0)
						{

							GCKillPlayerExpSend(lpTargetObj->m_Index,lpMonObj->m_Index,exp,0,MSBFlag);
						}
					}
					lpMonObj->Money += exp;
				}
			}
		}
	}
}

int gObjMonsterExpSingle(LPOBJ lpObj, LPOBJ lpTargetObj, int dmg, int tot_dmg, bool& bSendExp)
{

	if(DS_MAP_RANGE(lpObj->MapNumber))
	{
		return g_DevilSquare.gObjMonsterExpSingle(lpObj,lpTargetObj,dmg,tot_dmg);
	}

	if ( gSpecialItem.CheckNoExp(lpObj) == true )
	{
		return 0;
	}

	int exp;
	int maxexp = 0;
	int level = (lpTargetObj->Level+25)*lpTargetObj->Level/3;

	if((lpTargetObj->Level+10) < lpObj->Level)
	{
		level = level*(lpTargetObj->Level+10)/lpObj->Level;
	}

	if(lpTargetObj->Level >= 65)
	{
		level = level + (lpTargetObj->Level-64)*(lpTargetObj->Level/4);
	}

	if(level > 0)
	{
		maxexp = level/2;
	}
	else
	{
		level = 0;
	}

	if(maxexp < 1)
	{
		exp = level;
	}
	else
	{
		exp = level + rand()%maxexp;
	}

	exp = dmg * exp / tot_dmg;

	if( gUser.BloodCastleMapRange(lpObj->MapNumber) == true )
	{
		exp = exp * 50 / 100;
	}

	// ----
	DWORD mymaxexp;
	if ( gUser.gObjIsMasteringLevel(lpObj) == true )
	{
		mymaxexp = gLevelExperience[lpObj->MasterLevel+200];
	}
	else
	{
		mymaxexp = gLevelExperience[lpObj->Level];
	}

	int rate;
	
	if ( gUser.gObjIsMasteringLevel(lpObj) == true )
	{
		rate = lpObj->m_wExprienceRate + lpObj->MasterSkillTreeEffect.IncreaseExperience;
	}
	else
	{
		rate = lpObj->m_wExprienceRate;
	}

	// FUCK

	// VERIFICAR
	if((DWORD)exp > mymaxexp)
	{
		exp = mymaxexp;
	}

	exp *= gAddExperience;

	if ( lpObj->m_wExprienceRate > 0 )
	{
		exp =  (float)exp * ((float)rate  / 100.0f);
	}

	if ( g_CrywolfSync.GetOccupationState() == 1 && g_iCrywolfApplyMvpPenalty != FALSE)
	{
		exp =  (exp * g_CrywolfSync.GetGettingExpPenaltyRate()) / 100;
	}

	if(exp > 0)
	{
		if(lpObj->Type == OBJ_USER)
		{
			if(lpObj->m_wExprienceRate > 0)
			{
				if ( gUser.gObjIsMasteringLevel(lpObj) == true )
				{
					if ( lpObj->MasterLevel < MAX_CHAR_MASTER_LEVEL )
					{
						lpObj->MLExperience += exp;
					}
				}
				else
				{
					if ( lpObj->Level < HRB_CharMaxLevel )
					{
						lpObj->Experience += exp;
					}
				}
			}

			if(gObjLevelUp(lpObj,exp,lpTargetObj->Class,0) == 0)
			{
				bSendExp = 0;
			}
		}
	}

	return exp;
}
// Check this Function
void gObjExpParty(LPOBJ lpObj , LPOBJ lpTargetObj, int AttackDamage, int MSBFlag )
{

	if(DS_MAP_RANGE(lpObj->MapNumber))
	{
		g_DevilSquare.gObjExpParty(lpObj,lpTargetObj,AttackDamage,MSBFlag);
		return;
	}

	int n;
	int exp;
	int maxexp = 0;
	int totalexp;
	int level = (lpTargetObj->Level+25)*lpTargetObj->Level / 3;
	int number;
	int partynum = 0;
	int totallevel = 0;
	int partylevel;
	int partycount;
	int dis[5];
	int viewplayer = 0;
	int viewpercent = 100;
	int bApplaySetParty = 0;
	BYTE bCheckSetParty[MAX_CLASS_TYPE];
	LPOBJ lpPartyObj;

	partynum = lpObj->PartyNumber;

	int toplevel = 0;

	for(n = 0; n < MAX_USER_IN_PARTY; n++)
	{
		number = gParty.m_PartyInfo[partynum].Number[n];

		if(number >= 0)
		{
			lpPartyObj = &gObj[number];

			if(lpPartyObj->Level > toplevel)
			{
				toplevel = lpPartyObj->Level;
			}
		}
	}

	if(!OBJMAX_RANGE(partynum))
	{
		CLog.LogAdd("error : %s %d",__FILE__,__LINE__);
		return;
	}

	memset(bCheckSetParty,0,sizeof(bCheckSetParty));

	partycount = gParty.m_PartyInfo[partynum].Count;

	for(n = 0; n < MAX_USER_IN_PARTY; n++)
	{
		number = gParty.m_PartyInfo[partynum].Number[n];

		if(number >= 0)
		{
			lpPartyObj = &gObj[number];

			if(lpTargetObj->MapNumber == lpPartyObj->MapNumber)
			{
				dis[n] = gObjCalDistance(lpTargetObj,&gObj[number]);

				if(dis[n] < 10)
				{
					lpPartyObj = &gObj[number];

					if(toplevel >= (lpPartyObj->Level+200))
					{
						totallevel += lpPartyObj->Level+200;
					}
					else
					{
						totallevel += lpPartyObj->Level;
					}
					viewplayer += 1;
					bCheckSetParty[lpPartyObj->Class] = 1;
				}
			}
		}
	}

	if(bCheckSetParty[0] != 0 && bCheckSetParty[1] != 0 && bCheckSetParty[2]!= 0)
	{
		bApplaySetParty = 1;
	}

	if(viewplayer > 1)
	{
		if(bApplaySetParty != 0)
		{
			if(viewplayer == 3)
			{
				viewpercent = 230;
			}
			else if(viewplayer == 4)
			{
				viewpercent = 270;
			}
			else if(viewplayer >= 5)
			{
				viewpercent = 300;
			}
			else
			{
				viewpercent = 120;
			}
		}
		else
		{
			if(viewplayer == 2)
			{
				viewpercent = 160;
			}
			else if(viewplayer == 3)
			{
				viewpercent = 180;
			}
			else if(viewplayer == 4)
			{
				viewpercent = 200;
			}
			else if(viewplayer >= 5)
			{
				viewpercent = 220;
			}
			else
			{
				viewpercent = 150;
			}
		}

		partylevel = totallevel / viewplayer;
	}
	else
	{
		partylevel = totallevel;
	}

	if((lpTargetObj->Level+10) < partylevel)
	{
		level = level * (lpTargetObj->Level+10) / partylevel;
	}

	if(lpTargetObj->Level >= 65)
	{
		if(viewplayer == 1)
		{
			level = level + (lpTargetObj->Level-64)*(lpTargetObj->Level/4);
		}
		else
		{
			level += (200.0 - (lpObj->Level * 0.2));
		}
	}

	if(level > 0)
	{
		maxexp = level / 2;
	}
	else
	{
		level = 0;
	}

	if(maxexp < 1)
	{
		totalexp = level;
	}
	else
	{
		totalexp = level + rand()%maxexp;
	}

	if(lpTargetObj->Type == OBJ_MONSTER)
	{
		lpTargetObj->Money = (totalexp * 8); // FIX Party Zen Bug
	}

	for(n = 0; n < MAX_USER_IN_PARTY; n++)
	{
		number = gParty.m_PartyInfo[partynum].Number[n];

		if(number >= 0)
		{
			lpPartyObj = &gObj[number];

			if(lpTargetObj->MapNumber == lpPartyObj->MapNumber && dis[n] < 10)
			{
				DWORD mymaxexp = gLevelExperience[lpPartyObj->Level];

				exp = (totalexp * viewpercent * lpPartyObj->Level)/totallevel / 100;

				if( gUser.BloodCastleMapRange(lpPartyObj->MapNumber) == true )
				{
					exp = exp * 50 / 100;
				}

				// VERIFICAR
				if((DWORD)exp > mymaxexp)
				{
					exp = mymaxexp;
				}

				if(lpPartyObj->Type == OBJ_USER && lpTargetObj->Type == OBJ_USER)
				{
					exp = 0;
				}

				exp *= gAddExperience;

				if(lpPartyObj->m_wExprienceRate > 0)
				{
					exp = (float)exp * ((float)lpPartyObj->m_wExprienceRate  / 100.0f);
				}

				if ( g_CrywolfSync.GetOccupationState() == 1 && g_iCrywolfApplyMvpPenalty != FALSE)
				{
					exp =  (exp * g_CrywolfSync.GetGettingExpPenaltyRate()) / 100;
				}

				if(exp > 0)
				{
					if(lpPartyObj->Type == OBJ_USER)
					{
						if(lpPartyObj->m_wExprienceRate > 0)
						{
							lpPartyObj->Experience += exp;
						}

						if(gObjLevelUp(lpPartyObj,exp,lpTargetObj->Class,1) == 0)
						{
							continue;
						}
					}
				}

				if(lpPartyObj->Type == OBJ_USER && lpPartyObj->m_wExprienceRate > 0)
				{
					GCKillPlayerExpSend(lpPartyObj->m_Index,lpTargetObj->m_Index,exp,AttackDamage,MSBFlag);
				}
			}
		}
	}
}

void gObjMonsterDieLifePlus(LPOBJ lpObj, LPOBJ lpTartObj)
{
	if(lpTartObj->Level < 0 || lpTartObj->Level > 255)
	{
		return;
	}

	lpObj->Life += lpTartObj->Level;

	if(lpObj->MonsterDieGetLife != 0)
	{
		lpObj->Life = lpObj->Life + ((lpObj->MaxLife + lpObj->AddLife)/8.0f) * lpObj->MonsterDieGetLife;
	}

	if(lpObj->MonsterDieGetMana != 0)
	{
		lpObj->Mana = lpObj->Mana + ((lpObj->MaxMana + lpObj->AddMana)/8.0f) * lpObj->MonsterDieGetMana;
	}

	if(lpObj->Life > (lpObj->MaxLife + lpObj->AddLife))
	{
		lpObj->Life = lpObj->MaxLife + lpObj->AddLife;
	}

	GCReFillSend(lpObj->m_Index,lpObj->Life,255,0,lpObj->iShield);

	if(lpObj->MonsterDieGetMana != 0)
	{
		if(lpObj->Mana > (lpObj->MaxMana + lpObj->AddMana))
		{
			lpObj->Mana = lpObj->MaxMana + lpObj->AddMana;
		}

		GCManaSend(lpObj->m_Index,lpObj->Mana,255,0,lpObj->BP);
	}

	if(lpObj->MasterSkillTreeEffect.IncreaseRecoverLife != 0)
	{
	    lpObj->Life += (lpObj->MaxLife+lpObj->AddLife)/lpObj->MasterSkillTreeEffect.IncreaseRecoverLife;
	}

	if(lpObj->MasterSkillTreeEffect.IncreaseRecoverMana != 0)
	{
	    lpObj->Mana += (lpObj->MaxMana+lpObj->AddMana)/lpObj->MasterSkillTreeEffect.IncreaseRecoverMana;
	}

	if(lpObj->MasterSkillTreeEffect.IncreaseRecoverSD != 0)
	{ 
	    lpObj->iShield += (lpObj->iMaxShield+lpObj->iMaxShield)/lpObj->MasterSkillTreeEffect.IncreaseRecoverSD;
	}
}

void gObjLifeCheck(LPOBJ lpTargetObj, LPOBJ lpObj, int AttackDamage, int DamageSendType, int MSBFlag, int MSBDamage, WORD Skill, int iShieldDamage)
{
	if(lpObj->Connected < PLAYER_PLAYING)
	{
		return;
	}

	if(ATTRIBUTE_RANGE(lpObj->m_Attribute) || ATTRIBUTE_RANGE(lpTargetObj->m_Attribute))
	{
		return;
	}

	LPOBJ lpCallObj = lpObj;

	if(lpObj->Type == OBJ_MONSTER && lpObj->m_RecallMon >= 0)
	{
		lpCallObj = &gObj[lpObj->m_RecallMon];
	}

	if(AttackDamage != 0 && lpTargetObj->Type == OBJ_MONSTER)
	{
		if(lpTargetObj->m_RecallMon >= 0)
		{
			GCRecallMonLife(lpTargetObj->m_RecallMon,lpTargetObj->MaxLife,lpTargetObj->Life);
		}

		gObjMonsterSetHitDamage(lpTargetObj,lpCallObj->m_Index,AttackDamage);
	}

	if(lpTargetObj->Life <= 0 && lpTargetObj->Live != 0)
	{
		if(lpObj->Type == OBJ_MONSTER)
		{
			gObjAddMsgSend(lpObj,3,lpTargetObj->m_Index,0);
			CreateFrustrum(lpObj->X,lpObj->Y,lpObj->m_Index);
			lpTargetObj->KillerType = 1;
		}

		if( lpObj->Type == OBJ_USER )
		{
			gDarkSpirit[lpObj->m_Index].ReSetTarget(lpTargetObj->m_Index);
		}

		if(lpObj->Type == OBJ_USER && lpTargetObj->Type == OBJ_USER)
		{
			gObjSetKillCount(lpObj->m_Index,1);
		}

		if(lpTargetObj->Type == OBJ_MONSTER)
		{
#if (GS_CASTLE)
			
			if(lpTargetObj->Class == 275 || lpTargetObj->Class == 295 || lpObj->MapNumber == MAP_INDEX_CRYWOLF_FIRSTZONE)
			{
				gObjAddMsgSendDelayInSpecificQPos(lpTargetObj,1,lpObj->m_Index,800,0,0);
			}
			else
			{
				gObjAddMsgSendDelay(lpTargetObj,1,lpObj->m_Index,800,0);
			}
			
			g_Crywolf.CrywolfMonsterDieProc(lpTargetObj->m_Index,lpObj->m_Index);
#else
			//QuestNew.MonsterKillManager(gObjMonsterTopHitDamageUser(lpTargetObj),lpTargetObj->m_Index);

			if(lpTargetObj->Class == 275 || lpTargetObj->Class == 295)
			{
				gObjAddMsgSendDelayInSpecificQPos(lpTargetObj,1,lpObj->m_Index,800,0,0);
			}
			else
			{
				gObjAddMsgSendDelay(lpTargetObj,1,lpObj->m_Index,800,0);
			}
#endif
			
#if(GS_CASTLE)
			if(lpTargetObj->m_Attribute == 62)
			{
				
				char szMsg[256];
				
				switch(lpTargetObj->Class)
				{
					case 300:// Hero Mobs
					case 301:
					case 302:
					case 303:
					{
						//PMSG_NOTICE pNotice;
						BYTE Socket[5];
						Socket[0] = 0xFF;
						Socket[1] = 0xFF;
						Socket[2] = 0xFF;
						Socket[3] = 0xFF;
						Socket[4] = 0xFF;

						ItemSerialCreateSend(lpObj->m_Index,lpObj->MapNumber,lpObj->X,lpObj->Y,ItemGetNumberMake(14,13),0,0,0,0,0,lpObj->m_Index,0,0,Socket);
						
						if(gObjIsConnected(lpObj->m_Index))
						{
							wsprintf(szMsg,lMsg.Get(MSGGET(12, 155)),lpTargetObj->Name,lpObj->Name);
							CLog.LogAdd("[CastleDeep Event] [%s][%s] Boss Monster Killed : %s",lpObj->AccountID,lpObj->Name,lpTargetObj->Name);
						}
						else
						{
							wsprintf(szMsg,lMsg.Get(MSGGET(12, 156)),lpTargetObj->Name);
							CLog.LogAdd("[CastleDeep Event] Boss Monster Killed : %s",lpTargetObj->Name);
						}
						
						TNotice::MakeNoticeMsg(&pNotice,0,szMsg);
						
						for(int i = OBJ_STARTUSERINDEX; i < OBJMAX;i++)
						{
							if(gObj[i].Connected == PLAYER_PLAYING && gObj[i].Type == OBJ_USER)
							{
								DataSend(i,(PBYTE)&pNotice,pNotice.h.size);
							}
						}
					}
					break;
					case 295: // Erohim
					{
						//PMSG_NOTICE pNotice;
						
						if(gObjIsConnected(lpObj->m_Index))
						{
							wsprintf(szMsg,lMsg.Get(MSGGET(12, 157)),lpObj->Name);
							CLog.LogAdd("[CastleDeep Event] [%s][%s] Boss Monster Killed : %s",lpObj->AccountID,lpObj->Name,lpTargetObj->Name);
						}
						else
						{
							wsprintf(szMsg,lMsg.Get(MSGGET(12, 158)));
							CLog.LogAdd("[CastleDeep Event] Boss Monster Killed : %s",lpTargetObj->Name);
						}
						
						TNotice::MakeNoticeMsg(&pNotice,0,szMsg);
						
						for(int i = OBJ_STARTUSERINDEX; i < OBJMAX;i++)
						{
							if(gObj[i].Connected == PLAYER_PLAYING && gObj[i].Type == OBJ_USER)
							{
								DataSend(i,(PBYTE)&pNotice,pNotice.h.size);
							}
						}
					}
					break;
					}
			}
#else
			g_Kanturu.KanturuMonsterDieProc(lpTargetObj->m_Index,lpObj->m_Index);
#endif
			if( gUser.BloodCastleMapRange(lpTargetObj->MapNumber) == true )
			{
				if(lpTargetObj->Class == 131)
				{
					int TopHitUser = gObjMonsterTopHitDamageUser(lpTargetObj);

					if(TopHitUser != -1)
					{
						char szMsg[256];

						wsprintf(szMsg,"%s has destroyed the castle gate",gObj[TopHitUser].Name);

						g_BloodCastle.SendNoticeMessage(g_BloodCastle.GetBloodCastleIndex(lpObj->MapNumber),szMsg);

						g_BloodCastle.m_BridgeData[lpObj->m_cBloodCastleIndex].m_iExtraEXP_Kill_Door_Party = gObj[TopHitUser].PartyNumber;
						g_BloodCastle.m_BridgeData[lpObj->m_cBloodCastleIndex].m_iExtraEXP_Kill_Door_Index = TopHitUser;

						memcpy(g_BloodCastle.m_BridgeData[lpObj->m_cBloodCastleIndex].m_szKill_Door_CharName,gObj[TopHitUser].Name,10);
						memcpy(g_BloodCastle.m_BridgeData[lpObj->m_cBloodCastleIndex].m_szKill_Door_AccountID,gObj[TopHitUser].AccountID,10);

						g_BloodCastle.m_BridgeData[lpObj->m_cBloodCastleIndex].m_szKill_Door_CharName[10] = 0;
						g_BloodCastle.m_BridgeData[lpObj->m_cBloodCastleIndex].m_szKill_Door_AccountID[10] = 0;

						CLog.LogAdd("[Blood Castle] (%d) Door Terminated -> %d [%s][%s]",g_BloodCastle.GetBloodCastleIndex(lpObj->MapNumber),g_BloodCastle.m_BridgeData[g_BloodCastle.GetBloodCastleIndex(lpObj->MapNumber)].m_iBC_BOSS_MONSTER_KILL_COUNT,gObj[TopHitUser].AccountID,gObj[TopHitUser].Name);
					}
					else
					{
						g_BloodCastle.SendNoticeMessage(g_BloodCastle.GetBloodCastleIndex(lpObj->MapNumber),lMsg.Get(1169));
						CLog.LogAdd("[Blood Castle] (%d) Door Terminated -> %d)",g_BloodCastle.GetBloodCastleIndex(lpObj->MapNumber),g_BloodCastle.m_BridgeData[g_BloodCastle.GetBloodCastleIndex(lpObj->MapNumber)].m_iBC_BOSS_MONSTER_KILL_COUNT);
					}
				}


				if( BC_STATUE_RANGE(lpTargetObj->Class - 132) == true )
				{
					int TopHitUser = gObjMonsterTopHitDamageUser(lpTargetObj);

					if(TopHitUser != -1)
					{
						char szMsg[256];

						wsprintf(szMsg,"%s has destroyed Saint Statue",gObj[TopHitUser].Name);

						g_BloodCastle.SendNoticeMessage(g_BloodCastle.GetBloodCastleIndex(lpObj->MapNumber),szMsg);


						g_BloodCastle.m_BridgeData[lpObj->m_cBloodCastleIndex].m_iExtraEXP_Kill_Statue_Party = gObj[TopHitUser].PartyNumber;
						g_BloodCastle.m_BridgeData[lpObj->m_cBloodCastleIndex].m_iExtraEXP_Kill_Statue_Index = gObj[TopHitUser].m_Index;

						memcpy(g_BloodCastle.m_BridgeData[lpObj->m_cBloodCastleIndex].m_szKill_Status_CharName,gObj[TopHitUser].Name,10);
						memcpy(g_BloodCastle.m_BridgeData[lpObj->m_cBloodCastleIndex].m_szKill_Status_AccountID,gObj[TopHitUser].AccountID,10);

						g_BloodCastle.m_BridgeData[lpObj->m_cBloodCastleIndex].m_szKill_Status_CharName[10] = 0;
						g_BloodCastle.m_BridgeData[lpObj->m_cBloodCastleIndex].m_szKill_Status_AccountID[10] = 0;

						CLog.LogAdd("[Blood Castle] (%d) a stone statue Of Saint Terminated -> %d [%s][%s]",g_BloodCastle.GetBloodCastleIndex(lpObj->MapNumber)+1,g_BloodCastle.m_BridgeData[g_BloodCastle.GetBloodCastleIndex(lpObj->MapNumber)].m_iBC_BOSS_MONSTER_KILL_COUNT,gObj[TopHitUser].AccountID,gObj[TopHitUser].Name);
					}
					else
					{
						g_BloodCastle.SendNoticeMessage(g_BloodCastle.GetBloodCastleIndex(lpObj->MapNumber),lMsg.Get(1170));
						CLog.LogAdd("[Blood Castle] (%d) a stone statue Of Saint Terminated -> %d",g_BloodCastle.GetBloodCastleIndex(lpObj->MapNumber)+1,g_BloodCastle.m_BridgeData[g_BloodCastle.GetBloodCastleIndex(lpObj->MapNumber)].m_iBC_BOSS_MONSTER_KILL_COUNT);
					}
				}

				for(int n = 0; n < MAX_BLOOD_CASTLE_SUB_BRIDGE; n++)
				{
					if(g_BloodCastle.m_BridgeData[lpObj->m_cBloodCastleIndex].m_UserData[n].m_iIndex != -1)
					{
						if(gObj[g_BloodCastle.m_BridgeData[lpObj->m_cBloodCastleIndex].m_UserData[n].m_iIndex].Connected > PLAYER_LOGGED)
						{
							g_BloodCastle.m_BridgeData[lpObj->m_cBloodCastleIndex].m_UserData[n].m_bLiveWhenDoorBreak = 1;
						}
					}
				}
			}
#if(!GS_CASTLE)
			else if ( DG_MAP_RANGE(lpTargetObj->MapNumber) == true )
			{
				int MaxTopHitUser;
				switch ( lpTargetObj->Class )
				{
				case 529:	// --- Angry Butcher
					MaxTopHitUser = gObjMonsterTopHitDamageUser(lpTargetObj);

					if ( MaxTopHitUser != -1 )
					{
						SendMsg.MessageOutMap(gDoubleGoer.EventMap,0x00, "[DoubleGoer]: Angry Butcher has been killed by %s",gObj[MaxTopHitUser].Name);
						gDoubleGoer.UserIndexMaxHit[1] = MaxTopHitUser;
						gDoubleGoer.BossStage++;
					}
					break;
				case 530:	// --- Butcher
					MaxTopHitUser = gObjMonsterTopHitDamageUser(lpTargetObj);

					if ( MaxTopHitUser != -1 )
					{
						SendMsg.MessageOutMap(gDoubleGoer.EventMap,0x00, "[DoubleGoer]: Butcher has been killed by %s",gObj[MaxTopHitUser].Name);
						gDoubleGoer.UserIndexMaxHit[0] = MaxTopHitUser;
						gDoubleGoer.BossStage++;
					}
					break;
				case 531:	// --- Ice Walker
					MaxTopHitUser = gObjMonsterTopHitDamageUser(lpTargetObj);

					if ( MaxTopHitUser != -1 )
					{
						SendMsg.MessageOutMap(gDoubleGoer.EventMap,0x00, "[DoubleGoer]: Ice Walker has been killed by %s",gObj[MaxTopHitUser].Name);
						gDoubleGoer.UserIndexMaxHit[2] = MaxTopHitUser;
						gDoubleGoer.BossStage++;
					}
					break;
				}
			}
			else if ( IF_MAP_RANGE(lpTargetObj->MapNumber ) == true )
			{
				int MaxHitUser = gObjMonsterTopHitDamageUser(lpTargetObj);
				// ----
				if ( MaxHitUser != -1 )
				{
					gImperialFort.MonsterKilledManager(lpTargetObj,MaxHitUser);
				}
			}
			else if ( lpTargetObj->MapNumber == MAP_INDEX_RAKLION_BOSS )	// Raklion Boss
			{
				int MaxHitUser = gObjMonsterTopHitDamageUser(lpTargetObj);

				if ( MaxHitUser != -1 )
				{
					gRaklion.MonsterKillManager(lpTargetObj,MaxHitUser);
				}
			}
			else if( gUser.ChaosCastleMapRange(lpTargetObj->MapNumber) == true )
			{
				if(rand()%100 < 50)
				{
					g_ChaosCastle.SearchNBlowObjs(lpTargetObj->MapNumber,lpTargetObj->X,lpTargetObj->Y);
				}

				g_ChaosCastle.DelMonsterList(g_ChaosCastle.GetChaosCastleIndex(lpTargetObj->MapNumber),lpTargetObj->m_Index);

				int TopHitUser = gObjMonsterTopHitDamageUser(lpTargetObj);

				if(TopHitUser != -1)
				{
					if( gUser.ChaosCastleMapRange(gObj[TopHitUser].MapNumber) == true )
					{
						if(gObjIsConnected(TopHitUser))
						{
							gObj[TopHitUser].m_cKillMonsterCount += 1;
						}
					}
				}
			}
#endif
			gPKSystem.PKDownTime(lpObj,lpTargetObj->Level);

			if(lpObj->Type == OBJ_USER)
			{
				gObjAddMsgSendDelay(lpObj,3,lpTargetObj->m_Index,2000,0);
			}
		}
		else if(lpTargetObj->Type == OBJ_USER)
		{
			if( gUser.BloodCastleMapRange(lpTargetObj->MapNumber) == true )
			{
				g_BloodCastle.SetUserState(lpTargetObj->m_Index,1);
				g_BloodCastle.SearchUserDropQuestItem(lpTargetObj->m_Index);
			}

			if( gUser.ChaosCastleMapRange(lpTargetObj->MapNumber) == true )
			{
				g_ChaosCastle.SetUserState(lpTargetObj->m_Index,1);

				if( gUser.ChaosCastleMapRange(lpObj->MapNumber) == true )
				{
					if(gObjIsConnected(lpObj->m_Index))
					{
						lpObj->m_cKillUserCount += 1;
					}
				}

				g_ChaosCastle.SendFailMessage(g_ChaosCastle.GetChaosCastleIndex(lpTargetObj->MapNumber),lpTargetObj->m_Index);

				CLog.LogAdd("[Chaos Castle] (%d) [%s][%s] User Dead In Chaos Castle : Killed by [%s][%s]",g_ChaosCastle.GetChaosCastleIndex(lpTargetObj->MapNumber)+1,lpTargetObj->AccountID,lpTargetObj->Name,lpObj->AccountID,lpObj->Name);
			}

			lpTargetObj->KillerType = 0;

			gObjUserDie(lpTargetObj,lpCallObj);

			int iGuildWar = gObjGuildWarCheck(lpCallObj,lpTargetObj);
			bool iDuel = gDuel.AreOnDuel(lpCallObj,lpTargetObj);

			if( iGuildWar == 0 && iDuel == false && Gens.IsGenPVP(lpCallObj,lpTargetObj) == false )
			{
				if(lpTargetObj->m_bAttackerKilled == 0)
				{
					gObjPlayerKiller(lpCallObj,lpTargetObj);
				}
			}

			lpTargetObj->m_bAttackerKilled = 0;

			if(lpTargetObj->m_RecallMon >= 0)
			{
				gObjMonsterCallKill(lpTargetObj->m_Index);
			}
		}

		lpTargetObj->Live = 0;
		lpTargetObj->m_State = 4;
		lpTargetObj->RegenTime = GetTickCount();
		lpTargetObj->DieRegen = 1;
		lpTargetObj->PathCount = 0;
		
		// ---- Guild War Check
		if(gObjTargetGuildWarCheck(lpTargetObj,lpCallObj)==1)
		{
			lpTargetObj->KillerType = 2;
		}
		// ---- Check Duel End
		if( gDuel.AreOnDuel(lpCallObj,lpTargetObj) == true )
		{
			gDuel.DuelCheckEnd(lpCallObj,lpTargetObj);
		}

		if(lpTargetObj->Teleport == 1)
		{
			lpTargetObj->Teleport = 0;
		}

		GCDiePlayerSend(lpTargetObj,lpTargetObj->m_Index,Skill,lpObj->m_Index);

		if( lpTargetObj->Class == 44 )
		{
			return;
		}
		else if( lpTargetObj->Class == 79 )
		{
			return;
		}
		else if( lpTargetObj->m_RecallMon >= 0 )
		{
			return;
		}
		else if( lpTargetObj->Class == 340 || lpTargetObj->Class == 348 || lpTargetObj->Class == 349 )
		{
			return;
		}
		else if( lpTargetObj->Class == 287 || lpTargetObj->Class == 286 )
		{
			return;
		}
		else if ( lpTargetObj->Class == 55 )	// Death King
		{
			int TopHitUser = gObjMonsterTopHitDamageUser(lpTargetObj);
			if ( TopHitUser != -1 )
			{
				SendMsg.MessageOutAll(0x00, "Death King has been killed by %s",gObj[TopHitUser].Name);
			}
			return;
		}
		else if ( lpTargetObj->Class == 56 )	// Death Bone
		{
			return;
		}
		else if ( lpTargetObj->Class == 135 )	// White Wizard
		{
			int TopHitUser = gObjMonsterTopHitDamageUser(lpTargetObj);
			if ( TopHitUser != -1 )
			{
				if ( lpTargetObj->MapNumber == 0 )
				{
					SendMsg.MessageOutAll(0x00, "White Wizard has been killed by %s in Lorencia",gObj[TopHitUser].Name);
				}
				else if ( lpTargetObj->MapNumber == 2 )
				{
					SendMsg.MessageOutAll(0x00, "White Wizard has been killed by %s in Devias",gObj[TopHitUser].Name);
				}
				else if ( lpTargetObj->MapNumber == 3 )
				{
					SendMsg.MessageOutAll(0x00, "White Wizard has been killed by %s in Noria",gObj[TopHitUser].Name);
				}
				
			}
			return;
		}
		else if ( lpTargetObj->Class == 476 )	// Evil Santa
		{
			int TopHitUser = gObjMonsterTopHitDamageUser(lpTargetObj);
			if ( TopHitUser != -1 )
			{
				SendMsg.MessageOutAll(0x00, "Evil Santa has been killed by %s",gObj[TopHitUser].Name);
				GCStateInfoSend(&gObj[TopHitUser],1,91);
			}
			return;
		}
		else if ( lpTargetObj->Class == 466 )	// Cursed Goblin
		{
			return;
		}

#if(GS_CASTLE)
		else if( lpTargetObj->Class == 288 )
		{
			return;
		}
#endif

		else if( lpTargetObj->Class == 275 )
		{
			return;
		}
		else if( lpTargetObj->Class == 288 )
		{
			return;
		}
		else if( lpTargetObj->Class == 295 )
		{
			return;
		}

#if(!GS_CASTLE)
		else if( lpTargetObj->Class == 362 || lpTargetObj->Class == 363 || lpTargetObj->Class == 361 )
		{
			return;
		}
#endif

		if(lpCallObj->Type == OBJ_USER)
		{
			if(lpCallObj->PartyNumber >= 0)
			{
				gObjExpParty(lpCallObj,lpTargetObj,AttackDamage,MSBFlag);
			}
			else if(lpTargetObj->Type == OBJ_MONSTER)
			{
				gObjMonsterExpDivision(lpTargetObj,lpCallObj,AttackDamage,MSBFlag);
			}

			if(lpTargetObj->Type == OBJ_MONSTER && DS_MAP_RANGE(lpTargetObj->MapNumber))
			{
				g_DevilSquare.gObjMonsterScoreDivision(lpTargetObj,lpCallObj,AttackDamage,MSBFlag);
			}
		}
		/*if(lpTargetObj->Type == OBJ_MONSTER)
		{
			if(lpTargetObj->m_bIsInMonsterHerd != 0 && lpTargetObj->m_lpMonsterHerd != 0)
			{
				lpTargetObj->m_lpMonsterHerd->MonsterDieAction(lpTargetObj);
			}
		}*/
	}
	else
	{
		if(DamageSendType == 0)
		{
			GCDamageSend(lpCallObj->m_Index,lpTargetObj->m_Index,AttackDamage,MSBFlag,MSBDamage,iShieldDamage);
		}
		else if(DamageSendType == 1)
		{
			GCDamageSend(lpTargetObj->m_Index,AttackDamage,iShieldDamage);
		}
		else if(DamageSendType == 2)
		{
			GCDamageSend(lpCallObj->m_Index,lpTargetObj->m_Index,AttackDamage,MSBFlag,5,iShieldDamage);
		}
	}
}

bool gObjInventoryTrans(int aIndex)
{
	if ( ((aIndex< 0)?FALSE:(aIndex > OBJMAX-1)?FALSE:TRUE) == FALSE )
	{
		CLog.LogAdd("error : %s %d", __FILE__, __LINE__);
		return false;
	}

	if ( gObj[aIndex].pTransaction == 1 )
	{
		CLog.LogAdd("[%s][%s] error-L3 : pTransaction(%d) status error %s %d",
			gObj[aIndex].AccountID, gObj[aIndex].Name,
			gObj[aIndex].pTransaction, __FILE__, __LINE__);
		return false;
	}

	for ( int n=0;n<MAX_MAGIC;n++)
	{
		gObj[aIndex].m_lpMagicBack[n] = gObj[aIndex].Magic[n]; 
	}

	for ( int n=0;n<MAIN_INVENTORY_SIZE;n++)
	{
		gObj[aIndex].Inventory2[n] = gObj[aIndex].Inventory1[n];
	}

	memcpy(gObj[aIndex].InventoryMap2, gObj[aIndex].InventoryMap1, INVENTORY_MAP_SIZE);	// #error Deathway Fix
	//memcpy(gObj[aIndex].InventoryMap2, gObj[aIndex].InventoryMap1, 64);
	gObj[aIndex].InventoryCount2 = gObj[aIndex].InventoryCount1;
	gObjSetInventory2Pointer(&gObj[aIndex]);
	gObj[aIndex].pTransaction = 1;
	
	CLog.LogAdd("Trade Transaction (%s)", gObj[aIndex].Name);

	return true;
}

bool gObjInventoryCommit(int aIndex)
{

	if(!OBJMAX_RANGE(aIndex))
	{
		CLog.LogAdd("error : gObjInventoryCommit() - aIndex out of bound %s %d",__FILE__,__LINE__);
		return false;
	}

	if(gObj[aIndex].pTransaction != 1)
	{
		CLog.LogAdd("[%s][%s] error-L3 : pTransaction(%d) status2",gObj[aIndex].AccountID,gObj[aIndex].Name,gObj[aIndex].pTransaction);
		return false;
	}

	for(int n = 0; n < MAIN_INVENTORY_SIZE; n++)
	{
		gObj[aIndex].Inventory1[n] = gObj[aIndex].Inventory2[n];
	}

	memcpy(gObj[aIndex].InventoryMap1,gObj[aIndex].InventoryMap2,160);

	gObj[aIndex].InventoryCount1 = gObj[aIndex].InventoryCount2;

	gObjSetInventory1Pointer(&gObj[aIndex]);

	gObj[aIndex].pTransaction = 2;

	CLog.LogAdd("Trade Commit (%s)",gObj[aIndex].Name);

	return true;
}

bool gObjInventoryRollback(int aIndex)
{
	if(!OBJMAX_RANGE(aIndex))
	{
		CLog.LogAdd("error : %s %d",__FILE__,__LINE__);
		return false;
	}

	if(gObj[aIndex].pTransaction != 1)
	{
		CLog.LogAdd("[%s][%s] error-L3 : pTransaction(%d) status2",gObj[aIndex].AccountID,gObj[aIndex].Name,gObj[aIndex].pTransaction);
		return false;
	}

	for(int n = 0; n < MAX_MAGIC; n++)
	{
		gObj[aIndex].Magic[n] = gObj[aIndex].m_lpMagicBack[n];
	}

	for(int n = 0; n < INVENTORY_SIZE; n++)
	{
		gObj[aIndex].Inventory2[n].Clear();
	}

	gObjSetInventory1Pointer(&gObj[aIndex]);

	gObj[aIndex].pTransaction = 3;

	CLog.LogAdd("Trade Rollback (%s)",gObj[aIndex].Name);

	return true;
}

void gObjInventoryItemSet(int aIndex, int itempos, BYTE set_byte)
{
	int width;
	int height;
	if(itempos < INVETORY_WEAR_SIZE)
	{
		return;
	}
	else if(itempos > (INVENTORY_SIZE-1))
	{
		return;
	}

	if(gObj[aIndex].pInventory[itempos].GetSize((int&)width,(int &)height)==0)
	{
		CLog.LogAdd(lMsg.Get(527),__FILE__,__LINE__);
		return;
	}

	gObjInventoryItemBoxSet(aIndex,itempos,width,height,set_byte);
}

void gObjInventoryItemBoxSet(int aIndex, int itempos, int xl, int yl, BYTE set_byte)
{
	LPOBJ lpObj = &gObj[aIndex];
	int itemposx = (itempos - INVETORY_WEAR_SIZE)%8;
	int itemposy = (itempos - INVETORY_WEAR_SIZE)/8;

	int xx,yy;

	for(int y = 0; y < yl; y ++)
	{
		yy = itemposy + y;

		for(int x = 0; x < xl; x++)
		{
			xx = itemposx + x;

			if( ExtentCheck(xx,yy,8,12+(lpObj->ExpandedInventory*4)) == true )
			{
				*(BYTE*)(gObj[aIndex].pInventoryMap + (itemposy + y)*8+(itemposx + x)) = set_byte;
			}
			else
			{
				return;
			}
		}
	}
}

bool gObjFixInventoryPointer(int aIndex)
{
	if(gObjIsConnected(aIndex)==0)
	{
		CLog.LogAdd("[Fix Inv.Ptr] [%s][%s] - disconnected",gObj[aIndex].AccountID,gObj[aIndex].Name);
		return false;
	}

	if(gObj[aIndex].pInventory == gObj[aIndex].Inventory1)
	{
		return true;
	}

	if(gObj[aIndex].pInventory == gObj[aIndex].Inventory2)
	{
		if(gObj[aIndex].pTransaction == 1)
		{
			CLog.LogAdd("[Fix Inv.Ptr] [%s][%s] - Transaction == 1, Do not change Pointer",gObj[aIndex].AccountID,gObj[aIndex].Name);
			return false;
		}
		else
		{
			CLog.LogAdd("[Fix Inv.Ptr] [%s][%s] - Inventory Pointer was 2",gObj[aIndex].AccountID,gObj[aIndex].Name);

			for(int n = 0; n < INVENTORY_SIZE; n++)
			{
				gObj[aIndex].Inventory2[n].Clear();
			}
		}
	}
	else
	{
		CLog.LogAdd("[Fix Inv.Ptr] [%s][%s] - Inventory Pointer was Wrong",gObj[aIndex].AccountID,gObj[aIndex].Name);
	}

	gObjSetInventory1Pointer(&gObj[aIndex]);

	return false;
}

void gObjInventoryItemSet_PShop(int aIndex, int itempos, BYTE set_byte)
{
	int width;
	int height;
	if(itempos < INVETORY_WEAR_SIZE)
	{
		return;
	}
	else if(itempos > (INVENTORY_SIZE-1))
	{
		return;
	}

	if(gObj[aIndex].Inventory1[itempos].GetSize((int&)width,(int &)height)==0)
	{
		CLog.LogAdd(lMsg.Get(527),__FILE__,__LINE__);
		return;
	}

	gObjInventoryItemBoxSet_PShop(aIndex,itempos,width,height,set_byte);
}

void gObjInventoryItemBoxSet_PShop(int aIndex, int itempos, int xl, int yl, BYTE set_byte)
{
	LPOBJ lpObj = &gObj[aIndex];
	int itemposx = (itempos - INVETORY_WEAR_SIZE)%8;
	int itemposy = (itempos - INVETORY_WEAR_SIZE)/8;

	int xx,yy;

	for(int y = 0; y < yl; y ++)
	{
		yy = itemposy + y;

		for(int x = 0; x < xl; x++)
		{
			xx = itemposx + x;

			if( ExtentCheck(xx,yy,8,12) == true )
			{
				*(BYTE*)(gObj[aIndex].InventoryMap1 + (itemposy + y)*8+(itemposx + x)) = set_byte;
			}
			else
			{
				CLog.LogAdd("error : %s %d",__FILE__,__LINE__);
				return;
			}
		}
	}
}

void gObjInventoryDeleteItem(int aIndex ,int itempos)
{
	if(itempos < INVETORY_WEAR_SIZE)
	{
		int MagicDel = gObjMagicDel(&gObj[aIndex],gObj[aIndex].pInventory[itempos].m_Special[0],gObj[aIndex].pInventory[itempos].m_Level);

		if(MagicDel >= 0)
		{
			GCMagicListOneDelSend(aIndex,MagicDel, gObj[aIndex].pInventory[itempos].m_Special[0], gObj[aIndex].pInventory[itempos].m_Level,0,0);
		}

		gObj[aIndex].pInventory[itempos].Clear();
	}
	else
	{
		gObjInventoryItemSet(aIndex,itempos,255);
		gObj[aIndex].pInventory[itempos].Clear();
	}

	return;
}

void gObjWarehouseDeleteItem(int aIndex, int itempos)
{
	gObjWarehouseItemSet(aIndex, itempos, -1);
	gObj[aIndex].pWarehouse[itempos].Clear();

	return;
}

void gObjChaosBoxDeleteItem(int aIndex, int itempos)
{
	gObjChaosItemSet(aIndex, itempos, -1);
	gObj[aIndex].pChaosBox[itempos].Clear();

	return;
}

BYTE gObjInventoryInsertItem(int aIndex, int item_num)
{
	LPOBJ lpObj = &gObj[aIndex];
	int w,h,map_num,iwidth,iheight;
	BYTE blank;

	blank = 0;
	map_num = gObj[aIndex].MapNumber;

	if(MapC[map_num].m_cItem[item_num].live == 0)
	{
		return -1;
	}

	if(MapC[map_num].m_cItem[item_num].GetSize((int &)iwidth,(int &)iheight) == 0)
	{
		CLog.LogAdd(lMsg.Get(527),__FILE__,__LINE__);
		return -1;
	}

	for(h = 0; h < 8; h++)
	{
		for(w = 0; w < 8; w++)
		{
			if(*(BYTE*)(gObj[aIndex].pInventoryMap+h*8+w) == 255)
			{
				blank = gObjInventoryRectCheck(aIndex,w,h,iwidth,iheight);

				if(blank == 254)
				{
					goto GOTO_EndFunc;
				}

				if(blank != 255)
				{
					gObj[aIndex].pInventory[blank] = MapC[map_num].m_cItem[item_num];
					gObjInventoryItemSet(aIndex,blank,gObj[aIndex].pInventory[blank].m_Type);
					return blank;
				}
			}
		}
	}
GOTO_EndFunc:
	return -1;

}

DWORD gGetItemNumber()	
{
	return gItemNumberCount;
}

void gPlusItemNumber() 
{
	return;	// Maybe here Apply deathway fix (gItemNumberCount++)
}

BYTE gObjInventoryInsertItem(int aIndex, CItem item)
{
	LPOBJ lpObj = &gObj[aIndex];
	int w,h,iwidth,iheight;
	BYTE blank = 0;

	if(item.GetSize((int&)iwidth,(int&)iheight)==0)
	{
		CLog.LogAdd(lMsg.Get(527),__FILE__,__LINE__);
		return -1;
	}

	for(h = 0; h < 8; h++)
	{
		for( w = 0; w < 8; w++)
		{
			if(*(BYTE*)(gObj[aIndex].pInventoryMap+h*8+w) == 255)
			{
				blank = gObjInventoryRectCheck(aIndex,w,h,iwidth,iheight);

				if(blank == 254)
				{
					goto GOTO_EndFunc;
				}

				if(blank != 255)
				{
					if(gObjInventorySearchSerialNumber(&gObj[aIndex],item.m_Number) == 0)
					{
						return -1;
					}

					gObj[aIndex].pInventory[blank] = item;

					gObjInventoryItemSet(aIndex,blank,gObj[aIndex].pInventory[blank].m_Type);
					return blank;
				}
			}
		}
	}

GOTO_EndFunc:
	return -1;
}

BYTE gObjOnlyInventoryInsertItem(int aIndex, CItem item)
{
	LPOBJ lpObj = &gObj[aIndex];
	int w,h,iwidth,iheight;
	BYTE blank = 0;

	if(item.GetSize((int&)iwidth,(int&)iheight)==0)
	{
		CLog.LogAdd(lMsg.Get(527),__FILE__,__LINE__);
		return -1;
	}

	for(h = 0; h < 8; h++)
	{
		for( w = 0; w < 8; w++)
		{
			if(*(BYTE*)(gObj[aIndex].pInventoryMap+h*8+w) == 255)
			{
				blank = gObjOnlyInventoryRectCheck(aIndex,w,h,iwidth,iheight);

				if(blank == 254)
				{
					goto GOTO_EndFunc;
				}

				if(blank != 255)
				{
					if(gObjInventorySearchSerialNumber(&gObj[aIndex],item.m_Number) == 0)
					{
						return -1;
					}

					gObj[aIndex].pInventory[blank] = item;

					gObjInventoryItemSet(aIndex,blank,gObj[aIndex].pInventory[blank].m_Type);
					return blank;
				}
			}
		}
	}

GOTO_EndFunc:
	return -1;
}

BYTE gObjInventoryInsertItem(int aIndex, CMapItem * item)
{
	LPOBJ lpObj = &gObj[aIndex];
	int w,h,iwidth,iheight;
	BYTE blank = 0;

	CItem copyitem;


	if(item->GetSize((int &)iwidth,(int &)iheight)==0)
	{
		CLog.LogAdd(lMsg.Get(527),__FILE__,__LINE__);
		return -1;
	}

	for(h = 0; h < 8; h++)
	{
		for(w = 0; w < 8; w++)
		{
			if(*(BYTE*)(gObj[aIndex].pInventoryMap+h*8+w) == 255)
			{
				blank = gObjOnlyInventoryRectCheck(aIndex,w,h,iwidth,iheight);
				if(blank == 254)
				{
					goto GOTO_EndFunc;
				}
	
				if(blank != 255)
				{
					copyitem.m_Level = item->m_Level;
					copyitem.m_Durability = item->m_Durability;
	
					copyitem.Convert(item->m_Type,item->m_Option1,item->m_Option2,item->m_Option3,item->m_NewOption,item->m_SetOption,item->m_ItemOptionEx,item->m_ItemSocket);
					copyitem.SetPetItemInfo(item->m_PetItem_Level,item->m_PetItem_Exp);
	
					copyitem.m_Number = item->m_Number;
	
					gObj[aIndex].pInventory[blank] = copyitem;
		
					gObjInventoryItemSet(aIndex,blank,gObj[aIndex].pInventory[blank].m_Type);
					return blank;
				}
			}
		}
	}

GOTO_EndFunc:
	return -1;
}

bool gObjSearchItem(LPOBJ lpObj, int item_type, int add_dur, int nLevel)
{
	for(int n = INVETORY_WEAR_SIZE; n < MAIN_INVENTORY_SIZE; n++)
	{
		if(lpObj->pInventory[n].IsItem() == 1)
		{
			if(lpObj->pInventory[n].m_Type == item_type)
			{
				if(lpObj->pInventory[n].m_Level == nLevel)
				{
					if((int)lpObj->pInventory[n].m_Durability + add_dur <= 3)
					{
						lpObj->pInventory[n].m_Durability += add_dur;
						GCItemDurSend(lpObj->m_Index,n,lpObj->pInventory[n].m_Durability,0);
						return true;
					}
				}
			}
		}
	}
	return false;
}

bool gObjSearchItemMinus(LPOBJ lpObj, int pos, int m_dur)
{
	int n = pos;

	if(n < 0 || n > (MAIN_INVENTORY_SIZE-1))
	{
		CLog.LogAdd("error-L4 : %s %d",__FILE__,__LINE__);
		return true;
	}

	if(lpObj->pInventory[n].IsItem() == 1)
	{
		if((int)lpObj->pInventory[n].m_Durability - m_dur > 0)
		{
			lpObj->pInventory[n].m_Durability -= m_dur;
			GCItemDurSend(lpObj->m_Index,n,lpObj->pInventory[n].m_Durability,1);
			return true;
		}
	}
	return false;
}

BYTE gObjShopBuyInventoryInsertItem(int aIndex, CItem item)
{
	int w,h,iwidth,iheight;
	BYTE blank = 0;
	LPOBJ lpObj = &gObj[aIndex];
	if(item.GetSize(iwidth,iheight)==0)
	{
		CLog.LogAdd(lMsg.Get(527),__FILE__,__LINE__);
		return -1;
	}

	for(h = 0; h < 8; h++)
	{
		for( w = 0; w < 8; w++)
		{
			if(*(BYTE*)(gObj[aIndex].pInventoryMap+h*8+w) == 255)
			{
				blank = gObjOnlyInventoryRectCheck(aIndex,w,h,iwidth,iheight);

				if(blank == 254)
				{
					goto GOTO_EndFunc;
				}

				if(blank != 255)
				{
					gObj[aIndex].pInventory[blank] = item;

					if(item.m_serial == 0)
					{
						gObj[aIndex].pInventory[blank].m_Number = 0;
					}
					else
					{
						gObj[aIndex].pInventory[blank].m_Number = gGetItemNumber();
						gPlusItemNumber();
					}

					gObjInventoryItemSet(aIndex,blank,gObj[aIndex].pInventory[blank].m_Type);
					return blank;
				}
			}
		}
	}
GOTO_EndFunc:
	return -1;
}

BYTE gObjShopBuyInventoryInsertItem(LPOBJ lpObj, int type, int index, int level, int iSerial, int iDur)
{
	int w,h,iwidth,iheight;
	BYTE blank = 0;
	CItem item;
	
	int item_type = ItemGetNumberMake(type,index);
	item.m_Level = level;
	item.m_Durability = iDur;

	BYTE Socket[5];
	Socket[0] = 0xFF;
	Socket[1] = 0xFF;
	Socket[2] = 0xFF;
	Socket[3] = 0xFF;
	Socket[4] = 0xFF;

	item.Convert(item_type,0,0,0,0,0,0,Socket);

	if(item.GetSize((int &)iwidth,(int &)iheight)==0)
	{
		CLog.LogAdd(lMsg.Get(527),__FILE__,__LINE__);
		return -1;
	}

	for(h = 0; h < 8; h++)
	{
		for(w = 0; w < 8; w++)
		{
			if(*(BYTE*)(lpObj->pInventoryMap+h*8+w) == 255)
			{
				blank = gObjOnlyInventoryRectCheck(lpObj->m_Index,w,h,iwidth,iheight);

				if(blank == 254)
				{
					goto GOTO_EndFunc;
				}

				if(blank != 255)
				{
					lpObj->pInventory[blank] = item;

					if ( !item.m_serial )
					{
						lpObj->pInventory[blank].m_Number = 0;
					}
					else
					{
						lpObj->pInventory[blank].m_Number = gGetItemNumber();
						gPlusItemNumber();
					}
					
					gObjInventoryItemSet(lpObj->m_Index,blank,lpObj->pInventory[blank].m_Type);
					return blank;
				}
			}
		}
	}
GOTO_EndFunc:
	return -1;
}

BYTE gObjInventoryInsertItemTemp(LPOBJ lpObj, CMapItem * Item)
{
	CItem item;
	int w,h,iwidth,iheight;
	BYTE blank = 0;

	if(Item->GetSize((int &)iwidth,(int &)iheight)==0)
	{
		CLog.LogAdd(lMsg.Get(527),__FILE__,__LINE__);
		return -1;
	}

	for(h = 0; h < 8; h++)
	{
		for(w = 0; w < 8; w++)
		{
			if(*(BYTE*)(lpObj->pInventoryMap+h*8+w) == 255)
			{
				blank = gObjInventoryRectCheck(lpObj->m_Index,w,h,iwidth,iheight);

				if(blank == 254)
				{
					goto GOTO_EndFunc;
				}

				if(blank != 255)
				{
					return blank;
				}
			}
		}
	}
GOTO_EndFunc:
	return -1;
}

BYTE gObjInventoryInsertItem(LPOBJ lpObj, int type, int index, int level)
{
	CItem item;
	int w,h,iwidth,iheight;
	BYTE blank = 0;

	int item_type = ItemGetNumberMake(type,index);

	item.m_Level = level;

	BYTE Socket[5];
	Socket[0] = 0xFF;
	Socket[1] = 0xFF;
	Socket[2] = 0xFF;
	Socket[3] = 0xFF;
	Socket[4] = 0xFF;

	item.Convert(item_type,0,0,0,0,0,0,Socket);

	if(item.GetSize((int &)iwidth,(int &)iheight)==0)
	{
		//CLog.LogAdd(lMsg.Get(527),__FILE__,__LINE__);
		return -1;
	}

	for(h = 0; h < 8; h++)
	{
		for( w = 0; w < 8; w++)
		{

			if(*(BYTE*)(lpObj->pInventoryMap+h*8+w) == 255)
			{
				blank = gObjOnlyInventoryRectCheck(lpObj->m_Index,w,h,iwidth,iheight);
	
				if(blank == 254)
				{
					goto GOTO_EndFunc;
				}
	
				if(blank != 255)
				{
					lpObj->pInventory[blank] = item;
					lpObj->pInventory[blank].m_Number = gGetItemNumber();
					gPlusItemNumber();

					gObjInventoryItemSet(lpObj->m_Index,blank,lpObj->pInventory[blank].m_Type);
					return blank;
				}
			}
		}
	}
GOTO_EndFunc:
	return -1;
}

BYTE gObjInventoryInsertItem(LPOBJ lpObj, int type, int index, int level, int iSerial, int iDur)
{
	CItem item;
	int w,h,iwidth,iheight;
	BYTE blank = 0;

	int item_type = ItemGetNumberMake(type,index);
	item.m_Level = level;
	item.m_Durability = iDur;

	BYTE Socket[5];
	Socket[0] = 0xFF;
	Socket[1] = 0xFF;
	Socket[2] = 0xFF;
	Socket[3] = 0xFF;
	Socket[4] = 0xFF;

	item.Convert(item_type,0,0,0,0,0,0,Socket);

	if(item.GetSize((int &)iwidth,(int &)iheight)==0)
	{
		CLog.LogAdd(lMsg.Get(527),__FILE__,__LINE__);
		return -1;
	}

	for(h = 0; h < 8; h++)
	{
		for(w = 0; w < 8; w++)
		{
			if(*(BYTE*)(lpObj->pInventoryMap+h*8+w) == 255)
			{
				blank = gObjInventoryRectCheck(lpObj->m_Index,w,h,iwidth,iheight);

				if(blank == 254)
				{
					goto GOTO_EndFunc;
				}

				if(blank != 255)
				{
					lpObj->pInventory[blank] = item;
					lpObj->pInventory[blank].m_Number = iSerial;
					gPlusItemNumber();

					gObjInventoryItemSet(lpObj->m_Index,blank,lpObj->pInventory[blank].m_Type);
					return blank;
				}
			}
		}
	}
GOTO_EndFunc:
	return -1;
}

BYTE gObjMonsterInventoryInsertItem(LPOBJ lpObj, int type, int index, int level, int op1, int op2, int op3, BYTE * SocketOption)
{
	CItem item;
	int w,h,iwidth,iheight;
	BYTE blank = 0;
	int item_type;
	int a = 0;

	item_type = ItemGetNumberMake(type,index);
	item.m_Level = level;

	if(type == 13 && index == 10)
	{
		item.m_Durability = rand()%100+100.0f;
	}

	item.Convert(item_type,op1,op2,op3,0,0,0,SocketOption);

	lpObj->pInventory[blank] = item;
	iwidth = 1;
	iheight = 1;

	for(h = 0; h < 8; h++)
	{
		for(w = 0; w <8; w++)
		{
			if(*(BYTE*)(lpObj->pInventoryMap+h*8+w) == 255)
			{
				blank = gObjInventoryRectCheck(lpObj->m_Index,w,h,iwidth,iheight);

				if(blank == 254)
				{
					goto GOTO_EndFunc;
				}

				if(blank != 255)
				{
					lpObj->pInventory[blank] = item;
					*lpObj->pInventoryCount += 1;

					gObjInventoryItemBoxSet(lpObj->m_Index,blank,iwidth,iwidth,lpObj->pInventory[blank].m_Type);
					return blank;
				}
			}
		}
	}
GOTO_EndFunc:
	return -1;
}

BYTE gObjInventoryRectCheck(int aIndex, int sx, int sy, int width, int height)
{
	LPOBJ lpObj = &gObj[aIndex];
	int x,y;
	int blank = 0;

	if(sx + width > 8)
	{
		return -1;
	}

	if(sy + height > (12+(lpObj->ExpandedInventory*4)))
	{
		return -2;
	}

	int xx,yy;

	for(y = 0; y < height; y ++)
	{

		yy = sy+y;

		for(x = 0; x < width; x++)
		{
			xx = sx + x;

			if( ExtentCheck(xx,yy,8,12+(lpObj->ExpandedInventory*4)) == true )
			{
				if(*(BYTE*)(gObj[aIndex].pInventoryMap+(sy+y)*8+(sx+x)) != 255)
				{
					blank += 1;
					return -1;
				}
			}
			else
			{
				CLog.LogAdd("error : %s %d",__FILE__,__LINE__);
				return -1;
			}
		}
	}

	if(blank == 0)
	{
		return sx+sy*8+12+(lpObj->ExpandedInventory*4);
	}
	return  -1;
}

BYTE gObjOnlyInventoryRectCheck(int aIndex, int sx, int sy, int width, int height)
{
	LPOBJ lpObj = &gObj[aIndex];
	int x,y;
	int blank = 0;

	if(sx + width > 8)
	{
		return -1;
	}

	if(sy + height > 8)
	{
		return -2;
	}

	int xx,yy;

	for(y = 0; y < height; y ++)
	{

		yy = sy+y;

		for(x = 0; x < width; x++)
		{
			xx = sx + x;

			if( ExtentCheck(xx,yy,8,8) == true )
			{
				if(*(BYTE*)(gObj[aIndex].pInventoryMap+(sy+y)*8+(sx+x)) != 255)
				{
					blank += 1;
					return -1;
				}
			}
			else
			{
				CLog.LogAdd("error : %s %d",__FILE__,__LINE__);
				return -1;
			}
		}
	}

	if(blank == 0)
	{
		return sx+sy*8+12;
	}
	return  -1;
}

bool CheckInventoryEmptySpace(LPOBJ lpObj, int iItemHeight, int iItemWidth)
{
	int h=0;
	int w=0;
	BYTE blank = 0;
	
	for(h=0;h<8;h++)
	{
		for(w=0;w<8;w++)
		{
			if(*(BYTE*)(lpObj->pInventoryMap+(h)*8+(w)) == 255)
			{
				blank = gObjOnlyInventoryRectCheck(lpObj->m_Index, w, h, iItemWidth, iItemHeight);

				if ( blank == 0xFE )
					return false;

				if ( blank != 0xFF )
					return true;
			}
		}
	}

	return false;
}

bool gObjIsItemPut(LPOBJ lpObj, CItem * lpItem, int pos )
{
	if(lpItem->m_TwoHand == 1)
	{
		if(pos == 0 || pos == 1)
		{
			if(lpObj->pInventory[0].IsItem() == 1)
			{
				if(lpObj->pInventory[0].m_Type == ITEMGET(4,7) || lpObj->pInventory[0].m_Type == ITEMGET(4,15))
				{
					
				}
				else
				{
					return false;
				}
			}

			if(lpObj->pInventory[1].IsItem() == 1)
			{
				if(lpObj->pInventory[1].m_Type == ITEMGET(4,7) || lpObj->pInventory[1].m_Type == ITEMGET(4,15))
				{
					
				}
				else
				{
					return false;
				}
			}
		}
	}

	if(pos == 1)
	{
		if(lpObj->pInventory[0].IsItem() == 1)
		{
			if(lpItem->m_Type == ITEMGET(4,7) || lpItem->m_Type == ITEMGET(4,15))
			{
					
			}
			else
			{
				if(lpObj->pInventory[0].m_TwoHand)
				{
					return false;
				}
			}
		}
	}

	if(lpItem->m_Part != pos)
	{
		if(pos == 1 || pos == 0)
		{
			if(lpItem->m_Part == 1 || lpItem->m_Part == 0)
			{

			}
			else
			{
				return false;
			}
		}
		else if(pos == 11)
		{
			if(lpItem->m_Part == 10)
			{

			}
			else
			{
				return false;
			}
		}
		else
		{
			return false;
		}
	}

	if((lpObj->Strength + lpObj->AddStrength) < (lpItem->m_RequireStrength - lpItem->m_HJOpStrength))
	{
		return false;
	}

	if((lpObj->Dexterity + lpObj->AddDexterity) < (lpItem->m_RequireDexterity - lpItem->m_HJOpDexterity))
	{
		return false;
	}

	if(lpObj->Level < lpItem->m_RequireLevel)
	{
		return false;
	}

	if(lpItem->m_Type >= 0 && lpItem->m_Type < ITEMGET(12,0))
	{
		if((lpObj->Vitality + lpObj->AddVitality) < lpItem->m_RequireVitality)
		{
			return false;
		}

		if((lpObj->Energy + lpObj->AddEnergy) < lpItem->m_RequireEnergy)
		{
			return false;
		}
	}

	if((lpObj->Leadership + lpObj->AddLeadership) < lpItem->m_RequireLeaderShip)
	{
		return false;
	}

	if(lpItem->m_Type >= ITEMGET(12,0) && lpItem->m_Type <= ITEMGET(12,6))
	{
		if(lpObj->Level < lpItem->m_RequireLevel)
		{
			return false;
		}
	}
	else if(lpItem->m_Type >= ITEMGET(13,0) && lpItem->m_Type <= ITEMGET(13,15))
	{
		if(lpItem->m_Type == ITEMGET(13,10))
		{
			if(lpItem->m_Level <= 2)
			{
				if(lpObj->Level < 20)
				{
					return false;
				}
			}
			else
			{
				if(lpObj->Level < 50)
				{
					return false;
				}
			}
		}
		else if(lpItem->m_Type == ITEMGET(13,2))
		{
			if(lpObj->MapNumber == 7 || lpObj->Level < lpItem->m_RequireLevel)
			{
				return false;
			}
		}
		else if(lpItem->m_Type == ITEMGET(13,3))
		{
			if(lpObj->MapNumber == 7 || lpObj->Level < lpItem->m_RequireLevel)
			{
				return false;
			}
		}
		else
		{
			if(lpObj->Level < lpItem->m_RequireLevel)
			{
				return false;
			}
		}
	}

	if(lpItem->m_Type == ITEMGET(13,20) && lpItem->m_Level == 0)
	{
		int count = gObjGetItemCountInEquipment(lpObj->m_Index,13,20,0);

		if(count != 0)
		{
			return false;
		}
	}

	if(lpItem->m_Type == ITEMGET(13,10))
	{
		if( gUser.ChaosCastleMapRange(lpObj->MapNumber) == true )
		{
			return false;
		}
	}

	if(lpItem->m_Type == ITEMGET(13,37))
	{
		if( gUser.ChaosCastleMapRange(lpObj->MapNumber) == true )
		{
			return false;
		}
	}

	if(lpItem->m_Type == ITEMGET(13,38))
	{
		int count = gObjGetItemCountInEquipment(lpObj->m_Index,13,38,0);

		if(count != 0)
		{
			return false;
		}
	}

	if(lpItem->m_Type == ITEMGET(13,39))
	{
		if( gUser.ChaosCastleMapRange(lpObj->MapNumber) == true )
		{
			return false;
		}

		int count = 0;

		for(int n = 0; n < 7; n++)
		{
			count = gObjGetItemCountInEquipment(lpObj->m_Index,13,10,n);

			if(count != 0)
			{
				return false;
			}
		}

		count = gObjGetItemCountInEquipment(lpObj->m_Index,13,39,0);

		if(count != 0)
		{
			return false;
		}
	}

	if(lpItem->m_Type == ITEMGET(13,10))
	{
		int count = 0;

		for(int n = 0; n < 7; n ++)
		{
			count = gObjGetItemCountInEquipment(lpObj->m_Index,13,10,n);

			if(count != 0)
			{
				return false;
			}
		}

		count = gObjGetItemCountInEquipment(lpObj->m_Index,13,39,0);

		if(count != 0)
		{
			return false;
		}
	}
	else if(lpItem->m_Type >= ITEMGET(14,00))
	{
		return false;
	}

	return true;
}

BYTE gObjWerehouseRectCheck(int aIndex, int sx, int sy, int width, int height)
{
	int x,y;
	int blank = 0;

	if(sx + width > 8)
	{
		return -1;
	}

	if(sx + width < 0)
	{
		return -1;
	}

	if(sy + height > (15+(gObj[aIndex].ExpandedWarehouse*15)))
	{
		return -1;
	}

	if(sy + height < 0)
	{
		return -1;
	}

	int xx,yy;

	LPOBJ lpObj = &gObj[aIndex];

	for(y = 0; y < height; y ++)
	{

		yy = sy+y;

		for(x = 0; x < width; x++)
		{
			xx = sx + x;

			if(ExtentCheck(xx,yy,8,(15+(gObj[aIndex].ExpandedWarehouse*15)))==1)
			{
				if(*(BYTE*)(gObj[aIndex].pWarehouseMap+(sy+y)*8+(sx+x)) != 255)
				{
					blank += 1;
					return -1;
				}
			}
			else
			{
				CLog.LogAdd("error : %s %d",__FILE__,__LINE__);
				return -1;
			}
		}
	}

	if(blank == 0)
	{
		return sx+sy*8;
	}
	return  -1;
}

BYTE gObjMapRectCheck(BYTE * lpMapBuf, int sx, int sy, int ex, int ey, int width, int height)
{
	int x,y;
	int blank = 0;

	if(sx + width > ex || sx + width < 0)
	{
		return -1;
	}

	if(sy + height > ey || sy + height < 0)
	{
		return -1;
	}

	int xx,yy;

	for(y = 0; y < height; y++)
	{
		yy = sy + y;

		for(x = 0; x < width; x++)
		{
			xx = sx + x;

			if(ExtentCheck(xx,yy,ex,ey)==1)
			{
				if(*(BYTE*)(lpMapBuf + (sy + y)*ex + (sx+x)) != 255)
				{
					blank += 1;
					return -1;
				}
			}
			else
			{
				CLog.LogAdd("error : %s %d",__FILE__,__LINE__);
				return -1;
			}
		}
	}

	if(blank == 0)
	{
		return sy*ex+sx;
	}
	return  -1;

	return -1;
}

void gObjWarehouseItemBoxSet(int aIndex, int itempos, int xl, int yl, BYTE set_byte)
{
	int itemposx = (itempos)%8;
	int itemposy = (itempos)/8;

	int xx,yy;

	for(int y = 0; y < yl; y ++)
	{
		yy = itemposy + y;

		for(int x = 0; x < xl; x++)
		{
			xx = itemposx + x;

			if(ExtentCheck(xx,yy,8,(15+(gObj[aIndex].ExpandedWarehouse*15)))==1)
			{
				*(BYTE*)(gObj[aIndex].pWarehouseMap + (itemposy + y)*8+(itemposx + x)) = set_byte;
			}
			else
			{
				CLog.LogAdd("error : %d/%d  %s %d",xx,yy,__FILE__,__LINE__);
				return;
			}
		}
	}
}

void gObjWarehouseItemSet(int aIndex, int itempos, BYTE set_byte)
{
	int width;
	int height;
	if(itempos < 0)
	{
		return;
	}
	else if(itempos > (WAREHOUSE_SIZE-1))
	{
		return;
	}

	if(gObj[aIndex].pWarehouse[itempos].GetSize((int&)width,(int &)height)==0)
	{
		CLog.LogAdd(lMsg.Get(527),__FILE__,__LINE__);
		return;
	}

	gObjWarehouseItemBoxSet(aIndex,itempos,width,height,set_byte);
}

void gObjChaosItemBoxSet(int aIndex, int itempos, int xl, int yl, BYTE set_byte)
{
	int itemposx = (itempos)%8;
	int itemposy = (itempos)/8;

	int xx,yy;

	for(int y = 0; y < yl; y ++)
	{
		yy = itemposy + y;

		for(int x = 0; x < xl; x++)
		{
			xx = itemposx + x;

			if(ExtentCheck(xx,yy,8,4)==1)
			{
				*(BYTE*)(gObj[aIndex].pChaosBoxMap + (itemposy + y)*8+(itemposx + x)) = set_byte;
			}
			else
			{
				CLog.LogAdd("error : %d/%d  %s %d",xx,yy,__FILE__,__LINE__);
				return;
			}
		}
	}
}

void gObjChaosItemSet(int aIndex, int itempos, BYTE set_byte)
{
	int width;
	int height;
	if(itempos < 0)
	{
		return;
	}
	else if(itempos > (CHAOS_BOX_SIZE-1))
	{
		return;
	}

	if(gObj[aIndex].pChaosBox[itempos].GetSize((int&)width,(int &)height)==0)
	{
		CLog.LogAdd(lMsg.Get(527),__FILE__,__LINE__);
		return;
	}

	gObjChaosItemBoxSet(aIndex,itempos,width,height,set_byte);
}

BYTE gObjChaosBoxInsertItemPos(int aIndex, CItem item, int pos, int source)
{

	LPOBJ lpObj;
	BYTE TempMap[CHAOS_BOX_MAP_SIZE];

	int w,h,iwidth,iheight;

	int blank,useClass;

	if(pos < 0 || pos > (CHAOS_BOX_SIZE-1))
	{
		return -1;
	}

	useClass = 0;

	lpObj = &gObj[aIndex];

	if(lpObj->pChaosBox[pos].IsItem() == 1)
	{
		return -1;
	}

	if(item.IsItem() == 0)
	{
		return -1;
	}

	w = pos%8;
	h = pos/8;

	if(ExtentCheck(w,h,8,4) == 0)
	{
		return -1;
	}

	item.GetSize((int &)iwidth,(int &)iheight);

	if(source >= 0)
	{
		memcpy(TempMap,lpObj->pChaosBoxMap,CHAOS_BOX_MAP_SIZE);
		gObjChaosItemBoxSet(lpObj->m_Index,source,iwidth,iheight,255);
	}

	if(*(BYTE*)(lpObj->pChaosBoxMap + h * 8 + w) != 255)
	{
		if(source >= 0)
		{
			memcpy(lpObj->pChaosBoxMap,TempMap,CHAOS_BOX_MAP_SIZE);
		}
		return -1;
	}

	blank = gObjMapRectCheck(lpObj->pChaosBoxMap,w,h,8,4,iwidth,iheight);

	if(blank == 255)
	{
		if(source >= 0)
		{
			memcpy(lpObj->pChaosBoxMap,TempMap,CHAOS_BOX_MAP_SIZE);
		}
		return -1;
	}

	lpObj->pChaosBox[pos] = item;
	return pos;
}

BYTE gObjWarehouseInsertItemPos(int aIndex, CItem item, int pos, int source)
{
	LPOBJ lpObj;
	BYTE TempMap[WAREHOUSE_SIZE];

	int w,h,iwidth,iheight;

	int blank,useClass;

	if(pos < 0 || pos > (WAREHOUSE_SIZE-1))
	{
		return -1;
	}

	useClass = 0;

	lpObj = &gObj[aIndex];

	if(lpObj->pWarehouse[pos].IsItem() == 1)
	{
		return -1;
	}

	if(item.IsItem() == 0)
	{
		return -1;
	}

	w = pos%8;
	h = pos/8;

	if(ExtentCheck(w,h,8,(15+(gObj[aIndex].ExpandedWarehouse*15))) == 0)
	{
		return -1;
	}

	item.GetSize((int &)iwidth,(int &)iheight);

	if(source >= 0)
	{
		memcpy(TempMap,lpObj->pWarehouseMap,WAREHOUSE_SIZE);
		gObjWarehouseItemBoxSet(lpObj->m_Index,source,iwidth,iheight,255);
	}

	if(*(BYTE*)(lpObj->pWarehouseMap + h * 8 + w) != 255)
	{
		if(source >= 0)
		{
			memcpy(lpObj->pWarehouseMap,TempMap,WAREHOUSE_SIZE);
		}
		return -1;
	}

	blank = gObjWerehouseRectCheck(lpObj->m_Index,w,h,iwidth,iheight);

	if(blank == 255)
	{
		if(source >= 0)
		{
			memcpy(lpObj->pWarehouseMap,TempMap,WAREHOUSE_SIZE);
		}
		return -1;
	}

	lpObj->pWarehouse[pos] = item;
	return pos;
}

BYTE gObjInventoryInsertItemPos(int aIndex, CItem item, int pos, BOOL RequestCheck)
{
	if(pos < 0 || pos > INVENTORY_SIZE)
	{
		return -1;
	}

	LPOBJ lpObj;
	BYTE TempInventoryMap[INVENTORY_MAP_SIZE];

	int w,h,iwidth,iheight;

	int blank,useClass = 0;

	lpObj = &gObj[aIndex];

	if(lpObj->pInventory[pos].IsItem() == 1)
	{
		return -1;
	}

	if(item.IsItem() == 0)
	{
		return -1;
	}

	if(pos < 12)
	{
		useClass = item.IsClass(lpObj->Class,lpObj->ChangeUP,lpObj->ChangeUP2);

		if(useClass == 0)
		{
			CLog.LogAdd(lMsg.Get(528));
			return -1;
		}

		if(RequestCheck != 0)
		{
			if(gObjIsItemPut(lpObj,&item,pos) == 0)
			{
				return -1;
			}
		}
	}
	else
	{
		w = (pos - 12)%8;
		h = (pos - 12)/8;

		if( ExtentCheck(w,h,8,12) == false )
		{
			return -1;
		}

		memcpy(TempInventoryMap,lpObj->pInventoryMap,INVENTORY_MAP_SIZE);
		item.GetSize((int &)iwidth,(int &)iheight);

		if(*(BYTE*)(lpObj->pInventoryMap + h * 8 + w) != 255 )
		{
			memcpy(lpObj->pInventoryMap,TempInventoryMap,INVENTORY_MAP_SIZE);
			return -1;
		}

		blank = gObjInventoryRectCheck(lpObj->m_Index,w,h,iwidth,iheight);

		if(blank >= 254)
		{
			memcpy(lpObj->pInventoryMap,TempInventoryMap,INVENTORY_MAP_SIZE);
			return false;
		}
	}

	lpObj->pInventory[pos] = item;

	if(pos > 11)
	{
		gObjInventoryItemSet(aIndex,pos,1);
	}

	if(pos < 12)
	{
		CItem * lpItem = &lpObj->pInventory[pos];

		if(lpItem->m_Option1)
		{
			//int result = gObjWeaponMagicAdd(lpObj,lpItem->m_Special[0],lpItem->m_Level);
			int result = gObjWeaponMagicAdd(lpObj,lpItem->m_Special[0],lpItem->m_Level);

			if(result >= 0)
			{
				GCMagicListOneSend(lpObj->m_Index,result,lpItem->m_Special[0],lpItem->m_Level,0,0);
			}
		}
	}
	return pos;
}

bool gObjInventorySearchSerialNumber(LPOBJ lpObj, DWORD serial)
{
	int s_num;
	int count = 0;

	if(gItemSerialCheck == 0)
	{
		return true;
	}

	if(serial == 0)
	{
		return true;
	}

	for(int n = 0; n < INVENTORY_SIZE; n++)
	{
		s_num = lpObj->pInventory[n].GetNumber();

		if(s_num != 0 && s_num == serial)
		{
			count++;
		}
	}

	if(count <= 1)
	{
		return true;
	}

	for(int n = 0; n < INVENTORY_SIZE; n++)
	{
		s_num = lpObj->pInventory[n].GetNumber();

		if(s_num != 0 && s_num == serial)
		{
			CLog.LogAdd(lMsg.Get(529),lpObj->AccountID,lpObj->Name,lpObj->pInventory[n].GetName(),n,s_num);
			GCServerMsgStringSend(lMsg.Get(1102),lpObj->m_Index,1);
			gObjUserKill(lpObj->m_Index);
		}
	}

	return false;
}

bool gObjWarehouseSearchSerialNumber(LPOBJ lpObj, DWORD sirial)
{
	int s_num;
	int count = 0;

	if(gItemSerialCheck == 0)
	{
		return true;
	}

	if(sirial == 0)
	{
		return true;
	}

	for(int n = 0; n < WAREHOUSE_SIZE; n++)
	{
		s_num = lpObj->pWarehouse[n].GetNumber();

		if(s_num != 0 && s_num == sirial)
		{
			count++;
		}
	}

	if(count <= 1)
	{
		return true;
	}

	for(int n = 0; n < WAREHOUSE_SIZE; n++)
	{
		s_num = lpObj->pWarehouse[n].GetNumber();

		if(s_num != 0 && s_num == sirial)
		{
			CLog.LogAdd(lMsg.Get(530),lpObj->AccountID,lpObj->Name,lpObj->pWarehouse[n].GetName(),n,s_num);
			GCServerMsgStringSend(lMsg.Get(1102),lpObj->m_Index,1);
			gObjUserKill(lpObj->m_Index);
		}
	}

	return false;
}

BYTE gObjInventoryMoveItem(int aIndex, unsigned char source, unsigned char target, int& durSsend, int& durTsend, unsigned char sFlag, unsigned char tFlag, unsigned char* siteminfo)
{
	LPOBJ lpObj;
	BYTE TempInventoryMap[64];
	int w,h;
	int iwidth,iheight;
	int blank;
	int s_num;
	BYTE Socket[5];
	Socket[0] = 0xFF;
	Socket[1] = 0xFF;
	Socket[2] = 0xFF;
	Socket[3] = 0xFF;
	Socket[4] = 0xFF;

	CItem * sitem;
	CItem * titem;

	int bPersonalShopTrans = 0;
	int bSourceIsPShop = 0;

	durSsend = 0;
	durTsend = 0;

	lpObj = &gObj[aIndex];

	int useClass = 0;
	__try
	{
		if(sFlag == 2 || tFlag == 2)
		{
			if(lpObj->m_IfState.type != 6)
			{
				CLog.LogAdd(lMsg.Get(531),lpObj->AccountID,lpObj->Name);
				return -1;
			}

			if(lpObj->m_IfState.state == 0)
			{
				CLog.LogAdd(lMsg.Get(531),lpObj->AccountID,lpObj->Name);
				return -1;
			}
		}

		switch(sFlag)
		{
		case 0:	// Inventory
			if(source < 0 || source > (MAIN_INVENTORY_SIZE-1))
			{
				CLog.LogAdd("[BUG TRACER]: Item Move -> [%s,%s] Source Flag:%d, Source:%d Main Inventory Size Error",lpObj->AccountID,lpObj->Name,sFlag,source);
				return -1;
			}

			if(lpObj->pInventory[source].IsItem() == 0)
			{
				CLog.LogAdd("[BUG TRACER]: Item Move -> [%s,%s] Source Flag:%d, Source:%d Main Inventory Item Not Exist",lpObj->AccountID,lpObj->Name,sFlag,source);
				return -1;
			}

			sitem = &lpObj->pInventory[source];

			if( lpObj->MapNumber == MAP_INDEX_ICARUS || lpObj->MapNumber == MAP_INDEX_KANTURU_BOSS )
			{
				if(source == 8)
				{
					if( sitem->m_Type == ITEMGET(13,3) || sitem->m_Type == ITEMGET(13,37) || sitem->m_Type == ITEMGET(13,4) )
					{
						if(lpObj->pInventory[7].IsItem() == 0)
						{
							return -1;
						}
					}
				}
				else if(source == 7)
				{
					if( lpObj->pInventory[8].m_Type != ITEMGET(13,3) && lpObj->pInventory[8].m_Type != ITEMGET(13,37) && lpObj->pInventory[8].m_Type != ITEMGET(13,4) )
					{
						return -1;
					}
				}
			}

			break;
		case 2:	// Warehouse
			if(source < 0 || source > (WAREHOUSE_SIZE-1))
			{
				CLog.LogAdd("[BUG TRACER]: Item Move -> [%s,%s] Source Flag:%d, Source:%d Warehouse Size Error",lpObj->AccountID,lpObj->Name,sFlag,source);
				return -1;
			}
			if(lpObj->pWarehouse[source].IsItem() == 0)
			{
				CLog.LogAdd("[BUG TRACER]: Item Move -> [%s,%s] Source Flag:%d, Source:%d Warehouse Item Not Exist",lpObj->AccountID,lpObj->Name,sFlag,source);
				return -1;
			}

			sitem = &lpObj->pWarehouse[source];
			break;
		case 3:
		case 5:	// Pet Trainer
		case 6:	// Jewel Of Harmony Mix
		case 7:	// Jewel Of Harmony Mix
		case 8:
		case 9:
		case 10:
		case 11:
		case 12:
		case 13:
		case 14:
			if(source < 0 || source > (CHAOS_BOX_SIZE -1))
			{
				CLog.LogAdd("[BUG TRACER]: Item Move -> [%s,%s] Source Flag:%d, Source:%d Chaos Box Size Error",lpObj->AccountID,lpObj->Name,sFlag,source);
				return -1;
			}
			if(lpObj->pChaosBox[source].IsItem() == 0)
			{
				CLog.LogAdd("[BUG TRACER]: Item Move -> [%s,%s] Source Flag:%d, Source:%d Chaos Box Item Not Exist",lpObj->AccountID,lpObj->Name,sFlag,source);
				return -1;
			}
			sitem = &lpObj->pChaosBox[source];
			break;
		case 4:
			if(gObj[aIndex].m_bPShopOpen == 1)
			{
				CLog.LogAdd("[BUG TRACER]: Item Move -> [%s,%s] Source Flag:%d, Source:%d Personal Shop Open Error",lpObj->AccountID,lpObj->Name,sFlag,source);
				return -1;
			}

			EnterCriticalSection(&gObj[aIndex].m_critPShopTrade);

			if(gObj[aIndex].m_bPShopTransaction == 1)
			{
				CLog.LogAdd("[PShop] [%s][%s] PShop Item Move Request Failed : Already Trade With Other",gObj[aIndex].AccountID,gObj[aIndex].Name);
				LeaveCriticalSection(&lpObj->m_critPShopTrade);
				return -1;
			}

			lpObj->m_bPShopTransaction = 1;
			bPersonalShopTrans = 1;
			bSourceIsPShop = 1;

			LeaveCriticalSection(&lpObj->m_critPShopTrade);

			if(source < MAIN_INVENTORY_SIZE || source > (INVENTORY_SIZE - 1))
			{
				CLog.LogAdd("[BUG TRACER]: Item Move -> [%s,%s] Source Flag:%d, Source:%d Personal Shop & Main Inventory Size Error",lpObj->AccountID,lpObj->Name,sFlag,source);
				return -1;
			}
			if(lpObj->pInventory[source].IsItem() == 0)
			{
				CLog.LogAdd(lMsg.Get(532),lpObj->AccountID,lpObj->Name,__LINE__);
				return -1;
			}
			sitem = &lpObj->pInventory[source];
			break;
		default:
			{
				CLog.LogAdd("[BUG TRACER]: Item Move -> [%s,%s] Source Flag:%d, Source:%d Error Source Flag",lpObj->AccountID,lpObj->Name,sFlag,source);
				return -1;
			}
		}

		BYTE level;
		BYTE op1;
		BYTE op2;
		BYTE op3;
		BYTE dur;
		int type;

		BufferItemtoConvert3(siteminfo,(int &)type,(BYTE &)level,(BYTE &)op1,(BYTE &)op2,(BYTE &)op3,(BYTE &)dur);

		if(sitem->m_Type != type || sitem->m_Level != level || sitem->m_Option1 != op1 || sitem->m_Option2 != op2 || sitem->m_Option3 != op3)
		{
			CLog.LogAddC(2,lMsg.Get(533),type,level,op1,op2,op3,sitem->m_Type,sitem->m_Level,sitem->m_Option1,sitem->m_Option2,sitem->m_Option3);
			return -1;
		}

		switch(tFlag)
		{
		case 0:	// Inventory
			if(target < 0 || target > (MAIN_INVENTORY_SIZE - 1))
			{
				CLog.LogAdd("[BUG TRACER]: Item Move -> [%s,%s] Target Flag:%d, Target:%d Main Inventory Size Error",lpObj->AccountID,lpObj->Name,tFlag,target);
				return -1;
			}
			else if ( (target >= EXTRA_INVENTORY1_START && target <= EXTRA_INVENTORY1_END) && lpObj->ExpandedInventory < 1 )
			{
				CLog.LogAdd("[BUG TRACER]: Item Move -> [%s,%s] Target Flag:%d, Target:%d Expanded Inventory 1 Error",lpObj->AccountID,lpObj->Name,tFlag,target);
				return -1;
			}
			else if ( (target >= EXTRA_INVENTORY2_START && target <= EXTRA_INVENTORY2_END) && lpObj->ExpandedInventory < 2 )
			{
				CLog.LogAdd("[BUG TRACER]: Item Move -> [%s,%s] Target Flag:%d, Target:%d Expanded Inventory 2 Error",lpObj->AccountID,lpObj->Name,tFlag,target);
				return -1;
			}
			titem = &lpObj->pInventory[target];
			break;
		case 2:	// Warehouse
			if( target < 0 || target > (WAREHOUSE_SIZE - 1) )
			{
				CLog.LogAdd("[BUG TRACER]: Item Move -> [%s,%s] Target Flag:%d, Target:%d Warehouse Size Error",lpObj->AccountID,lpObj->Name,tFlag,target);
				return -1;
			}
			else if( (target >= EXTRA_WAREHOUSE_START && target <= EXTRA_WAREHOUSE_END) && lpObj->ExpandedWarehouse < 1 )
			{
				CLog.LogAdd("[BUG TRACER]: Item Move -> [%s,%s] Target Flag:%d, Target:%d Expanded Warehouse Error",lpObj->AccountID,lpObj->Name,tFlag,target);
				return -1;
			}
			titem = &lpObj->pWarehouse[target];
			break;
		case 3:	// Chaos Box
		case 5:	// Pet Trainer
		case 6:	// Jewel Of Harmony Mix
		case 7:	// Jewel Of Harmony Mix
		case 8:
		case 9:
		case 10:
		case 11:
		case 12:
		case 13:
		case 14:
			if(target < 0 || target > (CHAOS_BOX_SIZE -1))
			{
				CLog.LogAdd("[BUG TRACER]: Item Move -> [%s,%s] Target Flag:%d, Target:%d Chaos Box Size Error",lpObj->AccountID,lpObj->Name,tFlag,target);
				return -1;
			}

			titem = &lpObj->pChaosBox[target];

			if(sFlag == 0)
			{
				if(tFlag == 5)	// Pet Trainer Items
				{
					if( sitem->m_Type != ITEMGET(12,15) &&
						sitem->m_Type != ITEMGET(14,13) &&
						sitem->m_Type != ITEMGET(14,14) &&
						sitem->m_Type != ITEMGET(14,22) &&
						sitem->m_Type != ITEMGET(13,31) &&
						sitem->m_Type != ITEMGET(14,53) )
					{
						return -1;
					}
				}
				else if(tFlag == 6)	// Jewel Of Harmony Mix
				{
					if(g_kJewelOfHarmonySystem.IsJewelOfHarmonyOriginal(sitem->m_Type) == 0)
					{
						return -1;
					}
				}
				else if(tFlag == 7)	// Jewel Of Harmony Mix
				{
					if(g_kJewelOfHarmonySystem.IsEnableToMakeSmeltingStoneItem(sitem) == 0)
					{
						GCServerMsgStringSend(lMsg.Get(3377),lpObj->m_Index,1);
						return -1;
					}
				}
				else if(tFlag == 8)
				{
					if(g_kJewelOfHarmonySystem.IsStrengthenByJewelOfHarmony(sitem) == 0)
					{
						return -1;
					}
				}
				else if(tFlag == 9)
				{
					if(sitem->m_Type != ITEMGET(14,54) && sitem->m_Type != ITEMGET(14,95) )
					{
						return -1;
					}
				}
				else if(tFlag == 10)
				{
					if ( sitem->m_Type != ITEMGET(14,90) )
					{
						return -1;
					}
				}
				else if(tFlag == 11)
				{
					if ( sitem->m_Type == ITEMGET(12,15) || sitem->m_Type == ITEMGET(14,22) || sitem->m_Type == ITEMGET(14,42) )
					{
						
					}
					else if ( (sitem->m_Type >= ITEMGET(0,0) && sitem->m_Type <= ITEMGET(11,255)) && sitem->m_Type != ITEMGET(4,15) && sitem->m_Type != ITEMGET(4,7)  )
					{
						if ( sitem->IsExtItem() == false )
						{
							if ( sitem->IsSetItem() == false )
							{
								return -1;
							}
						}
						if ( sitem->m_Level < 4 )
						{
							return -1;
						}
						else if ( sitem->m_Option3*4 < 4 )
						{
							return -1;
						}
					}
					else
					{
						return -1;
					}
				}
				else if(tFlag == 12)
				{
					if ( sitem->m_Type == ITEMGET(12,15) || (sitem->m_Type >= ITEMGET(12,60) && sitem->m_Type <= ITEMGET(12,74)) || sitem->m_Type == ITEMGET(14,22) )
					{
						
					}
					else
					{
						return -1;
					}
				}
				else if(tFlag == 13)
				{
					if ( sitem->m_Type == ITEMGET(12,15) || sitem->m_Type == ITEMGET(14,22) )
					{
						
					}
					else if ( sitem->m_Type >= ITEMGET(12,100) && sitem->m_Type <= ITEMGET(12,129) )
					{
						
					}
					else if ( (sitem->m_Type >= ITEMGET(0,0) && sitem->m_Type <= ITEMGET(11,255)) && sitem->m_Type != ITEMGET(4,15) && sitem->m_Type != ITEMGET(4,7)  )
					{
						if ( sitem->IsSocketItem() == false )
						{
							return -1;
						}
					}
					else
					{
						return -1;
					}
				}
				else if(tFlag == 14)
				{
					if ( sitem->m_Type == ITEMGET(12,15) || sitem->m_Type == ITEMGET(14,22) )
					{
						
					}
					else if ( sitem->m_Type >= ITEMGET(12,100) && sitem->m_Type <= ITEMGET(12,129) )
					{
						
					}
					else if ( (sitem->m_Type >= ITEMGET(0,0) && sitem->m_Type <= ITEMGET(11,255)) && sitem->m_Type != ITEMGET(4,15) && sitem->m_Type != ITEMGET(4,7)  )
					{
						if ( sitem->IsSocketItem() == false )
						{
							return -1;
						}
					}
					else
					{
						return -1;
					}
				}
				else
				{
					if(sitem->m_Level < 4 && sitem->m_Option3*4 < 4)
					{
						if(sitem->m_Type == ITEMGET(12,15) || sitem->m_Type == ITEMGET(14,13) || sitem->m_Type == ITEMGET(14,14) || sitem->m_Type == ITEMGET(14,22) || sitem->m_Type == ITEMGET(13,14))
						{
							
						}
						else if(sitem->m_Type >= ITEMGET(12,0) && sitem->m_Type <= ITEMGET(12,2))
						{

						}
						else if(sitem->m_Type == ITEMGET(14,17) || sitem->m_Type == ITEMGET(14,18))
						{

						}
						else if(sitem->m_Type == ITEMGET(13,2) && sitem->m_Durability == 255.0f)
						{

						}
						else if(sitem->m_Type == ITEMGET(13,16) || sitem->m_Type == ITEMGET(13,17))
						{

						}
						else if(sitem->m_Type == ITEMGET(14,31))
						{

						}
						else if(sitem->m_Type == ITEMGET(12,26))
						{

						}
						else if(
							sitem->m_Type == ITEMGET(13,32) ||
							sitem->m_Type == ITEMGET(13,33) ||
							sitem->m_Type == ITEMGET(13,34) ||
							sitem->m_Type == ITEMGET(13,35) ||
							sitem->m_Type == ITEMGET(13,36) ||
							sitem->m_Type == ITEMGET(13,37) ||
							sitem->m_Type == ITEMGET(14,16))
						{

						}
						else if(sitem->m_Type == ITEMGET(14,3) || sitem->m_Type == ITEMGET(14,38) || sitem->m_Type == ITEMGET(14,39))
						{

						}
						else if(g_kJewelOfHarmonySystem.IsJewelOfHarmonyPurity(sitem->m_Type) == 1)
						{

						}
						else if(sitem->m_Type == ITEMGET(14,31))
						{

						}
						else if(sitem->m_Type == ITEMGET(14,53))
						{

						}
						else if(sitem->m_Type >= ITEMGET(14,103) && sitem->m_Type <= ITEMGET(14,108))
						{
							// Secromicon Mix
						}
						else if ( sitem->m_Type == ITEMGET(14,112) || sitem->m_Type == ITEMGET(14,113) || sitem->m_Type == ITEMGET(14,121) || sitem->m_Type == ITEMGET(14,122) )
						{
							// Sealed Golden and Silver Box
						}
						else
						{
							return -1;
						}
					}
					else if(sitem->m_Type == ITEMGET(14,11))
					{
						CLog.LogAdd("[BUG TRACER]: Item Move -> Item Move Error 7");
						return -1;
					}
				}
			}
			break;
		case 4:	// Personal Shop
			if(gDoPShopOpen == 0)
			{
				CLog.LogAdd("[BUG TRACER]: Item Move -> [%s,%s] Target Flag:%d, Target:%d Personal Shop Error",lpObj->AccountID,lpObj->Name,tFlag,target);
				return -1;
			}

			if(gObj[aIndex].m_bPShopOpen == 1)
			{
				CLog.LogAdd("[BUG TRACER]: Item Move -> [%s,%s] Target Flag:%d, Target:%d Personal Shop Error",lpObj->AccountID,lpObj->Name,tFlag,target);
				return -1;
			}

			if (IsCashItem(sitem->m_Type) == 1)
			{
				CLog.LogAdd("[BUG TRACER]: Item Move -> [%s,%s] Target Flag:%d, Target:%d Personal Shop Is Cash Item",lpObj->AccountID,lpObj->Name,tFlag,target);
				return -1;
			}

			EnterCriticalSection(&gObj[aIndex].m_critPShopTrade);

			if(gObj[aIndex].m_bPShopTransaction == 1)
			{
				if(bSourceIsPShop == 1)
				{
					lpObj->m_bPShopTransaction = 1;
					bPersonalShopTrans = 1;
				}
				else
				{
					CLog.LogAdd("[PShop] [%s][%s] PShop Item Move Request Failed : Already Trade With Other",gObj[aIndex].AccountID,gObj[aIndex].Name);
					LeaveCriticalSection(&lpObj->m_critPShopTrade);
					return -1;
				}
			}
			else
			{
				lpObj->m_bPShopTransaction = 1;
				bPersonalShopTrans = 1;
			}

			LeaveCriticalSection(&lpObj->m_critPShopTrade);

			if( target < PERSONAL_SHOP_START || target > PERSONAL_SHOP_END )
			{
				CLog.LogAdd("[BUG TRACER]: Item Move -> [%s,%s] Target Flag:%d, Target:%d Personal Shop Error",lpObj->AccountID,lpObj->Name,tFlag,target);
				return -1;
			}

			titem = &lpObj->pInventory[target];

			break;
		default:
			{
				CLog.LogAdd("[BUG TRACER]: Item Move -> [%s,%s] Target Flag:%d, Target:%d Target Flag Error",lpObj->AccountID,lpObj->Name,tFlag,target);
				return -1;
			}
		}

		s_num = sitem->GetNumber();

		if(gObjInventorySearchSerialNumber(lpObj,s_num) == 0)
		{
			CLog.LogAdd("[BUG TRACER]: Item Move -> [%s,%s] Source Flag:%d, Source:%d Inventory Serial Number Error",lpObj->AccountID,lpObj->Name,sFlag,source);
			return -1;
		}

		if(gObjWarehouseSearchSerialNumber(lpObj,s_num) == 0)
		{
			CLog.LogAdd("[BUG TRACER]: Item Move -> [%s,%s] Source Flag:%d, Source:%d Warehouse Serial Number Error",lpObj->AccountID,lpObj->Name,sFlag,source);
			return -1;
		}

		if(titem->IsItem() == 1)
		{
			int max_count = 0;

			if( ( sitem->m_Type == ITEMGET(13,32) || sitem->m_Type == ITEMGET(13,33) ) && titem->m_Durability < 20.0f)
			{
				max_count = 20;
			}

			if(sitem->m_Type == ITEMGET(13,34) && titem->m_Durability < 10.0f)
			{
				max_count = 10;
			}

			if( ( sitem->m_Type >= ITEMGET(14,35) && sitem->m_Type <= ITEMGET(14,40) ) || sitem->m_Type == ITEMGET(14,133) )
			{
				if(titem->m_Durability < 255.0f)
				{
					max_count = 255;
				}
			}

			if(sitem->m_Type >= ITEMGET(14,46) && sitem->m_Type <= ITEMGET(14,50))
			{
				if(titem->m_Durability < 255.0f)
				{
					max_count = 255;
				}
			}

#if(GS_CASTLE)
			if(sitem->m_Type == ITEMGET(14,63) || sitem->m_Type == ITEMGET(14,64) )
			{
				if(titem->m_Durability < 100.0f)
				{
					max_count = 100;
				}
			}
#endif
			if(sitem->m_Type == ITEMGET(14,29))
			{
				if(sitem->m_Level == titem->m_Level)
				{
					max_count = 5;
				}
			}
			else if(sitem->m_Type == ITEMGET(14,110))
			{
				max_count = 5;
			}
			else if(sitem->m_Type == ITEMGET(14,100))
			{
				if(titem->m_Durability < 255.0f)
				{
					max_count = 255;
				}
			}
			else if(sitem->m_Type == ITEMGET(14,101))
			{
				max_count = 5;
			}
			else if(sitem->m_Type >= ITEMGET(14,0) && sitem->m_Type <= ITEMGET(14,8) )
			{
				if(titem->m_Durability < 255.0f)
				{
					max_count = 255;
				}
			}
			else if(sitem->m_Type == ITEMGET(4,15) || sitem->m_Type == ITEMGET(4,7))
			{
				if(titem->m_Durability < 255.0f)
				{
					max_count = 255;
				}
			}
			else if( sitem->m_Type == ITEMGET(14,90) )
			{
				if(titem->m_Durability < 255.0f)
				{
					max_count = 255;
				}
			}

#if(GS_CASTLE)
			if(sitem->m_Type == ITEMGET(14,70) || sitem->m_Type == ITEMGET(14,71))
			{
				max_count = 50;
			}
			
			if(sitem->m_Type == ITEMGET(14,78) ||
				sitem->m_Type == ITEMGET(14,79) ||
				sitem->m_Type == ITEMGET(14,80) ||
				sitem->m_Type == ITEMGET(14,81) ||
				sitem->m_Type == ITEMGET(14,82) )
			{
				max_count = 3;
			}
#endif	

			if(max_count != 0)
			{
				if(sFlag != 0)
				{
					return -1;
				}

				if(titem->m_Type == sitem->m_Type && titem->m_Level == sitem->m_Level)
				{
					int t_dur = titem->m_Durability;

					if(t_dur < max_count)
					{
						int dif_dur = max_count - int(titem->m_Durability);

						if(dif_dur > sitem->m_Durability)
						{
							dif_dur = sitem->m_Durability;
						}

						titem->m_Durability += dif_dur;
						sitem->m_Durability -= dif_dur;

						if(titem->m_Type == ITEMGET(14,29) && max_count <= titem->m_Durability)
						{
							titem->m_Durability -= max_count;

							if(titem->m_Durability == 0)
							{
								gObjInventoryItemSet(aIndex,target,255);
								gObj[aIndex].pInventory[target].Clear();
								GCInventoryItemDeleteSend(aIndex,target,1);
								durTsend = 0;
							}
							else
							{
								durTsend = 1;
							}

							ItemSerialCreateSend(aIndex,235,gObj[aIndex].X,gObj[aIndex].Y,ItemGetNumberMake(14,28),sitem->m_Level,0,0,0,0,aIndex,0,0,Socket);
						}
						if(titem->m_Type == ITEMGET(14,101) && max_count <= titem->m_Durability)
						{
							titem->m_Durability -= max_count;

							if(titem->m_Durability == 0)
							{
								gObjInventoryItemSet(aIndex,target,255);
								gObj[aIndex].pInventory[target].Clear();
								GCInventoryItemDeleteSend(aIndex,target,1);
								durTsend = 0;
							}
							else
							{
								durTsend = 1;
							}

							ItemSerialCreateSend(aIndex,235,gObj[aIndex].X,gObj[aIndex].Y,ItemGetNumberMake(14,102),0,0,0,0,0,aIndex,0,0,Socket);
						}
						if(titem->m_Type == ITEMGET(14,110) && max_count <= titem->m_Durability)
						{
							titem->m_Durability -= max_count;

							if(titem->m_Durability == 0)
							{
								gObjInventoryItemSet(aIndex,target,255);
								gObj[aIndex].pInventory[target].Clear();
								GCInventoryItemDeleteSend(aIndex,target,1);
								durTsend = 0;
							}
							else
							{
								durTsend = 1;
							}

							ItemSerialCreateSend(aIndex,235,gObj[aIndex].X,gObj[aIndex].Y,ItemGetNumberMake(14,111),0,0,0,0,0,aIndex,0,0,Socket);
						}

						if(sitem->m_Durability > 0)
						{
							durSsend = 1;
							durTsend = 1;
						}
						else
						{
							switch(sFlag)
							{
							case 0:
								gObjInventoryItemSet(lpObj->m_Index,source,255);
								sitem->Clear();
								GCInventoryItemDeleteSend(lpObj->m_Index,source,0);
								durTsend = 1;
							default:	return 0;
							}
						}
					}
				}
			}
			return -1;
		}

		switch(tFlag)
		{
		case 0:
		case 4:
			if(target < INVETORY_WEAR_SIZE)
			{
				if(lpObj->MapNumber == MAP_INDEX_ICARUS || lpObj->MapNumber == MAP_INDEX_KANTURU_BOSS)
				{
					if(target == 10 || target == 11 )
					{
						switch ( lpObj->pInventory[source].m_Type )
						{
						case ITEMGET(13,10):	// Transformation Ring
						case ITEMGET(13,39):	// Elite Skeleton Transformation Ring
						case ITEMGET(13,40):	// Jack O'Lantern Transformation Ring
						case ITEMGET(13,41):	// Santa Girl Transformation Ring
						case ITEMGET(13,42):	// GameMaster Transformation Ring
						case ITEMGET(13,68):	// Snowman Transformation Ring
						case ITEMGET(13,76):	// Panda Transformation Ring
						case ITEMGET(13,122):	// Skeleton Transformation Ring
							SendMsg.MessageOut(aIndex, 0x01, "Can't use Transformation Rings here");
							return -1;
						}
					}
				}
				useClass = sitem->IsClass(lpObj->Class,lpObj->ChangeUP,lpObj->ChangeUP2);

				if(useClass == 0)
				{
					return -1;
				}

				if( gObjIsItemPut(lpObj,sitem,target) == false )
				{
					return -1;
				}
			}
			else 
			{
				if ( tFlag == 0 )
				{
					w = (target - INVETORY_WEAR_SIZE)%8;
					h = (target - INVETORY_WEAR_SIZE)/8;

					if( ExtentCheck(w,h,8,12+(lpObj->ExpandedInventory*4)) == false )
					{
						return -1;
					}
				
					sitem->GetSize((int &)iwidth,(int &)iheight);
					memcpy(TempInventoryMap,lpObj->pInventoryMap,64);

					switch(sFlag)
					{
					case 0:
					case 4:
						if(source > (INVETORY_WEAR_SIZE - 1))
						{
							gObjInventoryItemBoxSet(lpObj->m_Index,source,iwidth,iheight,255);
						}
						break;
					default: break;
					}
				}
				else if ( tFlag == 4 )
				{

				}
			
				if(*(BYTE*)(gObj[aIndex].pInventoryMap + h * 8 + w) != 255)
				{
					memcpy(lpObj->pInventoryMap,TempInventoryMap,64);
					return -1;
				}

				blank = gObjInventoryRectCheck(lpObj->m_Index,w,h,iwidth,iheight);

				if(blank >= 254)
				{
					memcpy(lpObj->pInventoryMap,TempInventoryMap,64);
					return -1;
				}
			}

			s_num = sitem->GetNumber();
			titem = sitem;

				switch(sFlag)
				{
				case 0:
				case 4:
					lpObj->pInventory[target] = lpObj->pInventory[source];
					gObjInventoryDeleteItem(aIndex,source);
					break;
				case 2:
					lpObj->pInventory[target] = lpObj->pWarehouse[source];
					gObjWarehouseDeleteItem(aIndex,source);
					break;
				case 3:
				case 5:
				case 6:
				case 7:
				case 8:
				case 9:
				case 10:
				case 11:
				case 12:
				case 13:
				case 14:
					lpObj->pInventory[target] = lpObj->pChaosBox[source];
					gObjChaosBoxDeleteItem(aIndex,source);
					break;
				default : break;
				}

				if(target > (INVETORY_WEAR_SIZE - 1))
				{
					gObjInventoryItemSet(aIndex,target,1);
				}

				if(target < INVETORY_WEAR_SIZE)
				{
					CItem * lpItem = &lpObj->pInventory[target];
					if(lpItem->m_Option1)
					{
						// ---- CHECK HERE FOR WEAPON SKILL UPGRADED
						int s_pos = gObjWeaponMagicAdd(&gObj[aIndex],lpItem->m_Special[0],lpItem->m_Level);
						if(s_pos >= 0)
						{
							GCMagicListOneSend(aIndex,s_pos,lpItem->m_Special[0],lpItem->m_Level,0,0);
						}
					}
				}
			
			break;
		case 2:
			BYTE res_1;
			switch(sFlag)
			{
			case 0:
				if( lpObj->pInventory[source].m_Type == ITEMGET(13,20) )
				{
					if( lpObj->pInventory[source].m_Level == 0 ||
						lpObj->pInventory[source].m_Level == 1 )
					{
						return -1;
					}
				}
				else if ( lpObj->pInventory[source].m_Type == ITEMGET(14,151) || // Quest Scroll
						  lpObj->pInventory[source].m_Type == ITEMGET(14,152) || // 1 Level Confirmation
						  lpObj->pInventory[source].m_Type == ITEMGET(14,154) || // Jewel of Hydra
						  lpObj->pInventory[source].m_Type == ITEMGET(14,155) || // Armor of Tantalus
						  lpObj->pInventory[source].m_Type == ITEMGET(14,156) || // Ashes of Butcher Torch
						  lpObj->pInventory[source].m_Type == ITEMGET(14,162) || // Magic Backpack
						  lpObj->pInventory[source].m_Type == ITEMGET(14,163) || // Warehouse Expansion
						  lpObj->pInventory[source].m_Type == ITEMGET(14,164) || // Ancient Statue
						  lpObj->pInventory[source].m_Type == ITEMGET(14,165) || // Magic Cloth
						  lpObj->pInventory[source].m_Type == ITEMGET(14,166) )  // Inventory Expanion
				{
					SendMsg.MessageOut(aIndex, 0x01, "Can't save Quest Items");
					return -1;
				}

				res_1 = gObjWarehouseInsertItemPos(aIndex,lpObj->pInventory[source],target,-1);
				break;
			case 2:
				res_1 = gObjWarehouseInsertItemPos(aIndex,lpObj->pWarehouse[source],target,source);
				break;
			default : break;
			}

			if(res_1 == 255)
			{
				return -1;
			}

			switch(sFlag)
			{
			case 0:
				lpObj->pWarehouse[target] = lpObj->pInventory[source];
				gObjInventoryDeleteItem(aIndex,source);
				break;
			case 2:
				lpObj->pWarehouse[target] = lpObj->pWarehouse[source];
				gObjWarehouseDeleteItem(aIndex,source);
				break;
			default : break;
			}

			gObjWarehouseItemSet(aIndex,target,1);

			if(sFlag == 0)
			{
				if(source < INVETORY_WEAR_SIZE)
				{
					if(source == 10 || source == 11)
					{
						switch ( lpObj->pWarehouse[target].m_Type )
						{
						case ITEMGET(13,10):	// Transformation Ring
						case ITEMGET(13,39):	// Elite Skeleton Transformation Ring
						case ITEMGET(13,40):	// Jack O'Lantern Transformation Ring
						case ITEMGET(13,41):	// Santa Girl Transformation Ring
						case ITEMGET(13,42):	// GameMaster Transformation Ring
						case ITEMGET(13,68):	// Snowman Transformation Ring
						case ITEMGET(13,76):	// Panda Transformation Ring
						case ITEMGET(13,122):	// Skeleton Transformation Ring
							gObjUseSkill.SkillChangeUse(aIndex);
							break;
						}
					}
					gObjMakePreviewCharSet(aIndex);
					GCEquipmentChange(aIndex,source);
				}
			}
			return 2;
		case 3:
		case 5:
		case 6:
		case 7:
		case 8:
		case 9:
		case 10:
		case 11:
		case 12:
		case 13:
		case 14:
			BYTE res_2;
				switch(sFlag)
				{
				case 0:
					res_2 = gObjChaosBoxInsertItemPos(aIndex,lpObj->pInventory[source],target,-1);
					break;
				case 3:
				case 5:
				case 6:
				case 7:
				case 8:
				case 9:
				case 10:
				case 11:
				case 12:
				case 13:
				case 14:
					res_2 = gObjChaosBoxInsertItemPos(aIndex,lpObj->pChaosBox[source],target,source);
					break;
				default : break;
				}

				if(res_2 == 255)
				{
					return -1;
				}

				switch(sFlag)
				{
				case 0:
					lpObj->pChaosBox[target] = lpObj->pInventory[source];
					gObjInventoryDeleteItem(aIndex,source);
					break;
				case 3:
				case 5:
				case 6:
				case 7:
				case 8:
				case 9:
				case 10:
				case 11:
				case 12:
				case 13:
				case 14:
					lpObj->pChaosBox[target] = lpObj->pChaosBox[source];
					gObjChaosBoxDeleteItem(aIndex,source);
					break;
				default : break;
				}

				gObjChaosItemSet(aIndex,target,1);

				if(sFlag == 0)
				{
					if(source < INVETORY_WEAR_SIZE)
					{
						if(source == 10 || source == 11)
						{
							switch ( lpObj->pChaosBox[target].m_Type )
							{
							case ITEMGET(13,10):	// Transformation Ring
							case ITEMGET(13,39):	// Elite Skeleton Transformation Ring
							case ITEMGET(13,40):	// Jack O'Lantern Transformation Ring
							case ITEMGET(13,41):	// Santa Girl Transformation Ring
							case ITEMGET(13,42):	// GameMaster Transformation Ring
							case ITEMGET(13,68):	// Snowman Transformation Ring
							case ITEMGET(13,76):	// Panda Transformation Ring
							case ITEMGET(13,122):	// Skeleton Transformation Ring
								gObjUseSkill.SkillChangeUse(aIndex);
								break;
							}
						}

						gObjMakePreviewCharSet(aIndex);
						GCEquipmentChange(aIndex,source);
					}
				}
			return tFlag;
		default: break;
		}

		if(sFlag == 0 && target == 1)
		{
			if(lpObj->pInventory[1].IsItem())
			{
				if(	lpObj->pInventory[1].m_Type == ITEMGET(5,21) ||
					lpObj->pInventory[1].m_Type == ITEMGET(5,22) || 
					lpObj->pInventory[1].m_Type == ITEMGET(5,23) )
				{
					if(ItemAttribute[lpObj->pInventory[1].m_Type].SkillType != 0)
					{
						gObjMagicAdd(lpObj, ItemAttribute[lpObj->pInventory[1].m_Type].SkillType, 0);
						GCMagicListMultiSend(lpObj, 0);
					}
				}
			}
		}

		if(source == 1 && tFlag == 0)
		{
			if( (lpObj->pInventory[target].m_Type == ITEMGET(5,21) ||
				 lpObj->pInventory[target].m_Type == ITEMGET(5,22) || 
				 lpObj->pInventory[target].m_Type == ITEMGET(5,23) ) && (target != 1) )
			{
				if(ItemAttribute[lpObj->pInventory[target].m_Type].SkillType != 0)
				{
					gObjMagicDel(lpObj, ItemAttribute[lpObj->pInventory[target].m_Type].SkillType, 0);
					GCMagicListMultiSend(lpObj, 0);
				}
			}
		}

		if(sFlag == 0 && source < INVETORY_WEAR_SIZE)
		{
			if(lpObj->pInventory[source].IsItem() == 1)
			{
				switch ( lpObj->pInventory[source].m_Type )
				{
				case ITEMGET(13,10):	// Transformation Ring
				case ITEMGET(13,39):	// Elite Skeleton Transformation Ring
				case ITEMGET(13,40):	// Jack O'Lantern Transformation Ring
				case ITEMGET(13,41):	// Santa Girl Transformation Ring
				case ITEMGET(13,42):	// GameMaster Transformation Ring
				case ITEMGET(13,68):	// Snowman Transformation Ring
				case ITEMGET(13,76):	// Panda Transformation Ring
				case ITEMGET(13,122):	// Skeleton Transformation Ring
					gObjUseSkill.SkillChangeUse(aIndex);
					break;
				}
			}
			else
			{
				if(source == 10 || source == 11)
				{
					switch ( lpObj->pInventory[target].m_Type )
					{
					case ITEMGET(13,10):	// Transformation Ring
					case ITEMGET(13,39):	// Elite Skeleton Transformation Ring
					case ITEMGET(13,40):	// Jack O'Lantern Transformation Ring
					case ITEMGET(13,41):	// Santa Girl Transformation Ring
					case ITEMGET(13,42):	// GameMaster Transformation Ring
					case ITEMGET(13,68):	// Snowman Transformation Ring
					case ITEMGET(13,76):	// Panda Transformation Ring
					case ITEMGET(13,122):	// Skeleton Transformation Ring
						gObjUseSkill.SkillChangeUse(aIndex);
						break;
					}
				}
			}

			gObjMakePreviewCharSet(aIndex);
			GCEquipmentChange(aIndex,source);
		}

		if(tFlag == 0 && target < INVETORY_WEAR_SIZE)
		{
			switch ( lpObj->pInventory[target].m_Type )
			{
			case ITEMGET(13,10):	// Transformation Ring
			case ITEMGET(13,39):	// Elite Skeleton Transformation Ring
			case ITEMGET(13,40):	// Jack O'Lantern Transformation Ring
			case ITEMGET(13,41):	// Santa Girl Transformation Ring
			case ITEMGET(13,42):	// GameMaster Transformation Ring
			case ITEMGET(13,68):	// Snowman Transformation Ring
			case ITEMGET(13,76):	// Panda Transformation Ring
			case ITEMGET(13,122):	// Skeleton Transformation Ring
				gObjUseSkill.SkillChangeUse(aIndex);
				break;
			}

			gObjMakePreviewCharSet(aIndex);
			GCEquipmentChange(aIndex,target);
		}
	}
	__finally
	{
		if(bPersonalShopTrans == 1)
		{
			gObj[aIndex].m_bPShopTransaction = 0;
		}
	}
	return false;
}

BYTE gObjTradeRectCheck(int aIndex, int sx, int sy, int width, int height )
{
	int x;
	int y;
	int blank = 0;
	int xx;
	int yy;

	if ( ( sx + width) > 8 )
	{
		return -1;
	}

	if ( (sy + height) > 4 )
	{
		return -1;
	}

	for ( y=0;y<height;y++)
	{
		yy = sy + y;
		
		for ( x=0;x<width;x++)
		{
			xx = sx + x;

			if ( ::ExtentCheck(xx, yy, 8, 4) == TRUE )
			{
				if ( *(BYTE*)(gObj[aIndex].TradeMap + yy * 8 + xx) != (BYTE)-1)
				{
					blank++;
					return -1;
				}
			}
			else
			{
				CLog.LogAdd("error : %s %d", __FILE__ ,__LINE__);
				return -1;
			}
		}
	}

	if ( blank == 0 )
	{
		return (sy * 8 + sx);
	}

	return -1;
}

BOOL gObjTradeItemBoxSet(int aIndex, int itempos, int xl, int yl, BYTE set_byte)
{
	int itemposx = (itempos)%8;
	int itemposy = (itempos)/8;

	int xx,yy;

	for(int y = 0; y < yl; y ++)
	{
		yy = itemposy + y;

		for(int x = 0; x < xl; x++)
		{
			xx = itemposx + x;

			if(ExtentCheck(xx,yy,8,4)==1)
			{
				*(BYTE*)(gObj[aIndex].TradeMap + yy*8+xx) = set_byte;
			}
			else
			{
				CLog.LogAdd("error : %s %d",__FILE__,__LINE__);
				return false;
			}
		}
	}
	return true;
}

BYTE gObjTradeInventoryMove(LPOBJ lpObj, BYTE source, BYTE target)
{
	int iwidth;
	int iheight;
	int s_num;
	BYTE itembuf[MAX_ITEM_INFO];
	// ----
	if(source > TRADE_BOX_SIZE)
	{
		CLog.LogAdd("error : %s %d",__FILE__,__LINE__);
		return -1;
	}

	if(target > MAIN_INVENTORY_SIZE)
	{
		CLog.LogAdd("error : %s %d",__FILE__,__LINE__);
		return -1;
	}

	if(lpObj->TargetNumber < 0)
	{
		return -1;
	}

	if(lpObj->Trade[source].IsItem() == 0)
	{
		return -1;
	}

	if(lpObj->m_IfState.use == 0 || lpObj->m_IfState.type != 1)
	{
		return -1;
	}

	if(gObjInventoryInsertItemPos(lpObj->m_Index,lpObj->Trade[source],target,1) == 255)
	{
		return -1;
	}

	s_num = 0;
	s_num = lpObj->Trade[source].m_Number;

	lpObj->Trade[source].GetSize((int &)iwidth,(int &)iheight);

	gObjTradeItemBoxSet(lpObj->m_Index,source,iwidth,iheight,255);
	lpObj->Trade[source].Clear();

	ItemByteConvert(itembuf,lpObj->Trade[source]);

	GCTradeOtherDel(lpObj->TargetNumber,source);

	if(gObjInventorySearchSerialNumber(lpObj,s_num) == 0)
	{
		return -1;
	}

	if(target < INVETORY_WEAR_SIZE)
	{
		if(lpObj->pInventory[target].IsItem() == 1)
		{
			if(lpObj->pInventory[target].m_Type == ITEMGET(13,10))
			{
				gObjUseSkill.SkillChangeUse(lpObj->m_Index);
				CLog.LogAdd(lMsg.Get(534),lpObj->Name,lpObj->pInventory[target].m_Level);
			}
			if(lpObj->pInventory[target].m_Type == ITEMGET(13,39))
			{
				gObjUseSkill.SkillChangeUse(lpObj->m_Index);
				CLog.LogAdd(lMsg.Get(534),lpObj->Name,lpObj->pInventory[target].m_Level);
			}
		}

		gObjMakePreviewCharSet(lpObj->m_Index);
		GCEquipmentChange(lpObj->m_Index,target);
	}

	return false;
}

BYTE gObjInventoryTradeMove(LPOBJ lpObj, BYTE source, BYTE target)
{
	int h;
	int w;
	int iwidth;
	int iheight;
	int s_num;
	int blank;
	// ----
	if(source > MAIN_INVENTORY_SIZE)
	{
		CLog.LogAdd("error : %s %d",__FILE__,__LINE__);
		return -1;
	}

	if(lpObj->pInventory[source].IsItem() == 0)
	{
		return -1;
	}

	if(lpObj->TargetNumber < 0)
	{
		return -1;
	}

	if(lpObj->m_IfState.use == 0 || lpObj->m_IfState.type != 1)
	{
		return -1;
	}

	if(lpObj->pInventory[source].m_Type == ITEMGET(13,20))
	{
		if(lpObj->pInventory[source].m_Level == 0 ||
			lpObj->pInventory[source].m_Level == 1)
		{
			return -1;
		}
	}

	if(lpObj->pInventory[source].m_Type == ITEMGET(13,20))
	{
		if(lpObj->pInventory[source].m_Level == 2)
		{
			return -1;
		}
	}

	if(lpObj->pInventory[source].m_Type == ITEMGET(14,11))
	{
		if(lpObj->pInventory[source].m_Level == 13)
		{
			return -1;
		}
	}

	if(lpObj->pInventory[source].m_Type == ITEMGET(12,26))
	{
		if(lpObj->pInventory[source].m_Level == 1 ||
			lpObj->pInventory[source].m_Level == 2 ||
			lpObj->pInventory[source].m_Level == 3 ||
			lpObj->pInventory[source].m_Level == 4 ||
			lpObj->pInventory[source].m_Level == 5)
		{
			return -1;
		}
	}

	if(lpObj->pInventory[source].m_Type == ITEMGET(13,38))
	{
		return -1;
	}

	if(lpObj->pInventory[source].m_Type == ITEMGET(13,39))
	{
		return -1;
	}

	if(IsCashItem(lpObj->pInventory[source].m_Type) == 1)
	{
		return -1;
	}

	lpObj->pInventory[source].GetSize((int &)iwidth,(int &)iheight);
	s_num = lpObj->pInventory[source].GetNumber();

	if(gObjInventorySearchSerialNumber(lpObj,s_num) == 0)
	{
		return -1;
	}

	w = target % 8;
	h = target / 8;

	if(ExtentCheck(w,h,8,4) == 0)
	{
		return -1;
	}

	if(*(BYTE*)(lpObj->TradeMap + h * 8 + w) == 255)
	{
	BYTE itembuf[MAX_ITEM_INFO];
		blank = gObjTradeRectCheck(lpObj->m_Index,w,h,iwidth,iheight);

		if(blank == 255)
		{
			return -1;
		}

		lpObj->Trade[blank] = lpObj->pInventory[source];
		gObjInventoryDeleteItem(lpObj->m_Index,source);
		gObjTradeItemBoxSet(lpObj->m_Index,blank,iwidth,iheight,lpObj->Trade[blank].m_Type);
		ItemByteConvert(itembuf,lpObj->Trade[blank]);
		GCTradeOtherAdd(lpObj->TargetNumber,blank,itembuf);

		if(source < INVETORY_WEAR_SIZE)
		{
			if(source == 10 || source == 11)
			{
				if(lpObj->Trade[blank].m_Type == ITEMGET(13,10))
				{
					gObjUseSkill.SkillChangeUse(lpObj->m_Index);
				}
			}
			gObjMakePreviewCharSet(lpObj->m_Index);
			GCEquipmentChange(lpObj->m_Index,source);
		}
		return true;
	}
	return -1;
}

BYTE gObjTradeTradeMove(LPOBJ lpObj, BYTE source, BYTE target)
{
	int h;
	int w;
	int iwidth;
	int iheight;
	BYTE TempTradeMap[TRADE_BOX_MAP_SIZE];
	int blank;
	// ----
	if(source > TRADE_BOX_SIZE)
	{
		CLog.LogAdd("error : %s %d",__FILE__,__LINE__);
		return -1;
	}
	if(source == target)
	{
		CLog.LogAdd("error : %s %d",__FILE__,__LINE__);
		return -1;
	}
	if(lpObj->Trade[source].IsItem() == 0)
	{
		return -1;
	}
	if(lpObj->Trade[target].IsItem() == 1)
	{
		return -1;
	}
	if(lpObj->TargetNumber < 0)
	{
		return -1;
	}
	if(lpObj->m_IfState.use == 0 || lpObj->m_IfState.type != 1)
	{
		return -1;
	}

	lpObj->Trade[source].GetSize((int &)iwidth,(int &)iheight);
	memcpy(TempTradeMap,lpObj->TradeMap,TRADE_BOX_MAP_SIZE);
	gObjTradeItemBoxSet(lpObj->m_Index,source,iwidth,iheight,255);

	w = target % 8;
	h = target / 8;

	if(ExtentCheck(w,h,8,4) == 0)
	{
		memcpy(lpObj->TradeMap,TempTradeMap,TRADE_BOX_MAP_SIZE);
		return -1;
	}

	if(*(BYTE*)(lpObj->TradeMap + h * 8 + w) == 255)
	{
		BYTE itembuf[MAX_ITEM_INFO];
		blank = gObjTradeRectCheck(lpObj->m_Index,w,h,iwidth,iheight);

		if(blank == 255)
		{
			memcpy(lpObj->TradeMap,TempTradeMap,TRADE_BOX_MAP_SIZE);
			return -1;
		}

		lpObj->Trade[blank] = lpObj->Trade[source];
		lpObj->Trade[source].Clear();
		GCTradeOtherDel(lpObj->TargetNumber,source);
		gObjTradeItemBoxSet(lpObj->m_Index,blank,iwidth,iheight,lpObj->Trade[blank].m_Type);
		ItemByteConvert(itembuf,lpObj->Trade[blank]);
		GCTradeOtherAdd(lpObj->TargetNumber,blank,itembuf);
		return true;
	}

	memcpy(lpObj->TradeMap,TempTradeMap,TRADE_BOX_MAP_SIZE);
	return -1;
}

void gObjTradeCancel(int aIndex)
{
	if(OBJMAX_RANGE(aIndex) == 0)
	{
		CLog.LogAdd("error : index error %s %d",__FILE__,__LINE__);
		return;
	}
	if(gObj[aIndex].Type != OBJ_USER || gObj[aIndex].Connected != PLAYER_PLAYING)
	{
		CLog.LogAdd(lMsg.Get(535),gObj[aIndex].AccountID,gObj[aIndex].Name);
		return;
	}
	if(gObj[aIndex].m_IfState.use != 1)
	{
		CLog.LogAdd(lMsg.Get(536),gObj[aIndex].AccountID,gObj[aIndex].Name);
		return;
	}
	if(gObj[aIndex].m_IfState.type != 1)
	{
		CLog.LogAdd(lMsg.Get(537),gObj[aIndex].AccountID,gObj[aIndex].Name);
		return;
	}
	gObjInventoryRollback(aIndex);
	gObj[aIndex].TargetNumber = -1;
	gObj[aIndex].m_IfState.use = 0;
	gObjCharTradeClear(&gObj[aIndex]);
	GCMoneySend(aIndex,gObj[aIndex].Money);
	GCItemListSend(aIndex);
	GCEquipmentSend(aIndex);
	GCMagicListMultiSend(&gObj[aIndex],0);
	gObjUseSkill.SkillChangeUse(aIndex);

	if(gObj[aIndex].GuildNumber > 0)
	{
		GCGuildViewportNowPaint(aIndex,gObj[aIndex].GuildName,0,0);
		gObjNotifyUpdateUnionV1(&gObj[aIndex]);
		gObjNotifyUpdateUnionV2(&gObj[aIndex]);
	}
	CLog.LogAdd(lMsg.Get(514),gObj[aIndex].AccountID,gObj[aIndex].Name);
}

void gObjTempInventoryItemBoxSet(BYTE * TempMap, int itempos, int xl, int yl, BYTE set_byte)
{
	int itemposx = (itempos - INVETORY_WEAR_SIZE) % 8;
	int itemposy = (itempos - INVETORY_WEAR_SIZE) / 8;

	for(int y = 0 ; y < yl ; y++)
	{
		for(int x = 0; x < xl; x++)
		{
			*(BYTE*)(TempMap + (itemposy +y) * 8 + (itemposx + x)) = set_byte;
		}
	}
}

BYTE gObjTempInventoryRectCheck(BYTE * TempMap, int sx, int sy, int width, int height)
{
	int x,y,blank = 0;

	if(sx + width > 8)
	{
		return -1;
	}
	if(sy + height > 8)
	{
		return -2;
	}

	for(y = 0; y < height; y++)
	{
		for(x = 0; x < width; x++)
		{
			if(*(BYTE*)(TempMap + (sy+y)*8 + (sx+x))!= 255)
			{
				blank += 1;
				return -1;
			}
		}
	}
	if(blank == 0)
	{
		return sx+sy*8+INVETORY_WEAR_SIZE;
	}
	return -1;
}

BYTE gObjTempInventoryInsertItem(LPOBJ lpObj, CItem item, BYTE * TempMap)
{
	int w,h,iwidth,iheight;
	BYTE blank = 0;

	if(item.GetSize(iwidth,iheight) == 0)
	{
		CLog.LogAdd(lMsg.Get(532),__FILE__,__LINE__,item.m_Type);
		return -1;
	}
	for(h = 0; h < 8; h++)
	{
		for(w = 0; w < 8; w++)
		{
			if(*(BYTE*)(TempMap + h * 8 + w) == 255)
			{
				blank = gObjTempInventoryRectCheck(TempMap,w,h,iwidth,iheight);

				if(blank == 254)
				{
					goto GOTO_EndFunc;
				}
				if(blank != 255)
				{
					gObjTempInventoryItemBoxSet(TempMap,blank,iwidth,iheight,item.m_Type);
					return blank;
				}
			}
		}
	}
GOTO_EndFunc:
	return -1;
}

bool TradeItemInventoryPutTest(int aIndex)
{
	BYTE TempInventoryMap[64];
	int number,result = 1;
	number = gObj[aIndex].TargetNumber;

	if(gObjIsConnected(number) == 0)
	{
		return false;
	}
	memcpy(TempInventoryMap,gObj[aIndex].pInventoryMap,64);

	for(int n = 0; n < TRADE_BOX_SIZE; n++)
	{
		if(gObj[number].Trade[n].IsItem() == 1)
		{
			if(gObjTempInventoryInsertItem(&gObj[aIndex],gObj[number].Trade[n],TempInventoryMap) == 255)
			{
				return false;
			}
			result = 1;
		}
	}
	if(result == 1)
	{
		return true;
	}
	return false;
}

bool TradeitemInventoryPut(int aIndex)
{
	int number;
	BYTE insert;
	char szItemName[50] = "Item";
	BYTE ExOption[8];
	// ----
	number = gObj[aIndex].TargetNumber;

	if(gObjIsConnected(number) == 0)
	{
		return false;
	}

	for(int n = 0; n < TRADE_BOX_SIZE; n++)
	{
		if(gObj[number].Trade[n].IsItem() == 1)
		{
			insert = gObjInventoryInsertItem(aIndex,gObj[number].Trade[n]);
			if(insert != 255)
			{
				strcpy(szItemName,gObj[number].Trade[n].GetName());
				ItemIsBufExOption(ExOption,&gObj[number].Trade[n]);
				CLog.LogAdd(lMsg.Get(538),
					gObj[number].AccountID,gObj[number].Name,gObj[number].MapNumber,gObj[number].X,gObj[number].Y,
					gObj[aIndex].AccountID,gObj[aIndex].Name,gObj[aIndex].MapNumber,gObj[aIndex].X,gObj[aIndex].Y,
					szItemName,gObj[number].Trade[n].m_Number,gObj[number].Trade[n].m_Level,gObj[number].Trade[n].m_Option1,gObj[number].Trade[n].m_Option2,gObj[number].Trade[n].m_Option3,
					ExOption[0],ExOption[1],ExOption[2],ExOption[3],ExOption[4],ExOption[5],ExOption[6],gObj[number].Trade[n].m_SetOption);

			}
			else
			{
				CLog.LogAdd("error : 이건 큰일나는일!!");
				return false;
			}
		}
	}
	return true;
}

void gObjTradeOkButton(int aIndex)
{
	BYTE result = 1;
	int number = gObj[aIndex].TargetNumber;
	// ----
	if(number < 0)
	{
		return;
	}

	if(gObjIsConnected(number) == 0)
	{
		return;
	}

	if(gObj[number].TargetNumber != aIndex)
	{
		CLog.LogAdd(lMsg.Get(539),gObj[aIndex].AccountID,gObj[aIndex].Name,gObj[number].TargetNumber,gObj[number].TargetNumber);
		return;
	}

	if(gObj[aIndex].TradeOk != 1 || gObj[number].TradeOk != 1)
	{
		return;
	}

	if(TradeItemInventoryPutTest(aIndex) == 1)
	{
		if(TradeItemInventoryPutTest(number) == 1)
		{
			result = 1;
		}
		else
		{
			result = 2;
		}
	}
	else
	{
		result = 2;
	}

	if(gObjCheckMaxZen(aIndex,gObj[number].TradeMoney) == 0)
	{
		result = 2;
	}

	if(gObjCheckMaxZen(number,gObj[aIndex].TradeMoney) == 0)
	{
		result = 2;
	}

	if(g_kJewelOfHarmonySystem.IsEnableToTrade(&gObj[number]) == 0 || g_kJewelOfHarmonySystem.IsEnableToTrade(&gObj[aIndex]) == 0)
	{
		result = 4;
		gObjTradeCancel(aIndex);
		gObjTradeCancel(number);
		CGTradeResult(aIndex,result);
		CGTradeResult(number,result);
		return;
	}

	if(result == 1)
	{
		TradeitemInventoryPut(aIndex);
		TradeitemInventoryPut(number);
		gObjInventoryCommit(aIndex);
		gObjInventoryCommit(number);
		gObjMakePreviewCharSet(aIndex);
		gObjMakePreviewCharSet(number);

		gObj[aIndex].Money -= gObj[aIndex].TradeMoney;
		if(gObj[aIndex].Money < 0)
		{
			gObj[aIndex].Money = 0;
		}
		gObj[number].Money -= gObj[number].TradeMoney;
		if(gObj[number].Money < 0)
		{
			gObj[number].Money = 0;
		}

		gObj[aIndex].Money += gObj[number].TradeMoney;
		gObj[number].Money += gObj[aIndex].TradeMoney;

		CLog.LogAdd(lMsg.Get(540),gObj[aIndex].AccountID,gObj[aIndex].Name,gObj[number].AccountID,gObj[number].Name,gObj[aIndex].TradeMoney,gObj[number].TradeMoney);
		GJSetCharacterInfo(&gObj[aIndex],aIndex,0);
		GJSetCharacterInfo(&gObj[number],number,0);
	}
	else
	{
		result = 2;
		gObjTradeCancel(aIndex);
		gObjTradeCancel(number);
		CGTradeResult(aIndex,result);
		CGTradeResult(number,result);
		return;
	}

	gObj[aIndex].TargetNumber = -1;
	gObj[aIndex].m_IfState.use = 0;
	gObj[number].TargetNumber = -1;
	gObj[number].m_IfState.use = 0;
	gObj[aIndex].TradeOk = 0;
	gObj[number].TradeOk = 0;
	gObj[aIndex].TradeMoney = 0;
	gObj[number].TradeMoney = 0;

	GCMoneySend(aIndex,gObj[aIndex].Money);
	GCMoneySend(number,gObj[number].Money);
	CGTradeResult(aIndex,result);
	CGTradeResult(number,result);

	GCItemListSend(aIndex);
	GCItemListSend(number);

	for(int n = 0; n < TRADE_BOX_SIZE; n++)
	{
		gObj[aIndex].Trade[n].Clear();
		gObj[number].Trade[n].Clear();
	}

	gObjNotifyUpdateUnionV1(&gObj[aIndex]);
	gObjNotifyUpdateUnionV2(&gObj[aIndex]);
}

// ---------------------------------------------------
// [Sunday, 20 Feb, 2011] WolF ~ Level Small Convert
// ---------------------------------------------------
int LevelSmallConvert(int aIndex, int InventoryIndex)
{
	LPOBJ lpObj = &gObj[aIndex];
	// ------
	int Level = lpObj->pInventory[InventoryIndex].m_Level;
	// ------
	switch (Level)
	{
		case 15:
		case 14:
			Level = 7;
			break;
		// ------
		case 13:
		case 12:
			Level = 6;
			break;
		// ------
		case 11:
			Level = 5;
			break;
		// ------
		case 10:
		case 9:
			Level = 4;
			break;
		// ------
		case 8:
		case 7:
			Level = 3;
			break;
		// ------
		case 6:
		case 5:
			Level = 2;
			break;
		// ------
		case 4:
		case 3:
			Level = 1;
			break;
		// ------
		case 2:
		case 1:
		case 0:
			Level = 0;
			break;
	}
	// ------
	return Level;
}

// ---------------------------------------------------
// [Sunday, 20 Feb, 2011] WolF ~ Level Small Convert
// ---------------------------------------------------
int LevelSmallConvert(int LevelReturn)
{
	int Level = 0;

	switch (LevelReturn)
	{
		case 15:
		case 14:
			Level = 6;
			break;
		// ------
		case 13:
		case 12:
		case 11:
			Level = 5;
			break;
		// ------
		case 10:
		case 9:
			Level = 4;
			break;
		// ------
		case 8:
		case 7:
			Level = 3;
			break;
		// ------
		case 6:
		case 5:
			Level = 2;
			break;
		// ------
		case 4:
		case 3:
			Level = 1;
			break;
		// ------
		case 2:
		case 1:
		case 0:
			Level = 0;
			break;
	}
	// ------
	return Level;
}

// Reformulated
void gObjMakePreviewCharSet(int aIndex)
{
	if ( !OBJMAX_RANGE(aIndex ))
	{
		CLog.LogAdd("[gObjMakePreviewCharSet]: Object [%d] Out of Range. File: %s, Line: %d",aIndex, __FILE__, __LINE__);
		return;
	}

	BYTE index;
	LPOBJ lpObj = &gObj[aIndex];
	memset(lpObj->CharSet, 0, sizeof(lpObj->CharSet));

	lpObj->CharSet[CS_CLASS] = CS_SET_CLASS(lpObj->Class);
	lpObj->CharSet[CS_CLASS] |= CS_SET_CHANGEUP(lpObj->ChangeUP);
	lpObj->CharSet[CS_CLASS] |= CS_SET_CHANGEUP2(lpObj->ChangeUP2);


	// Set Class
	if ( lpObj->m_ActionNumber == 0x80 )
	{
		lpObj->CharSet[CS_CLASS] |= 2;
	}
	else if ( lpObj->m_ActionNumber == 0x81 )
	{
		lpObj->CharSet[CS_CLASS] |= 3;
	}
	else
	{
		lpObj->CharSet[CS_CLASS] = lpObj->CharSet[CS_CLASS];
	}

	// Set Right Hand Item
	if ( lpObj->pInventory[0].m_Type < ITEMGET(0,0) )
	{
		lpObj->CharSet[CS_WEAPON1_DATA] |= (BYTE)DBI_GET_TYPE(-1);
		lpObj->CharSet[CS_WEAPON1_TYPE] = (BYTE)DBI_GET_INDEX(-1);
	}
	else
	{
		lpObj->CharSet[CS_WEAPON1_DATA] |= DBI_GET_TYPE(lpObj->pInventory[0].m_Type);
		lpObj->CharSet[CS_WEAPON1_TYPE] = DBI_GET_INDEX(lpObj->pInventory[0].m_Type);
	}

	// Set Left Hand Item
	if ( lpObj->pInventory[1].m_Type < ITEMGET(0,0) )
	{
		lpObj->CharSet[CS_WEAPON2_DATA] |= (BYTE)DBI_GET_TYPE(-1);
		lpObj->CharSet[CS_WEAPON2_TYPE] = (BYTE)DBI_GET_INDEX(-1);
	}
	else
	{
		lpObj->CharSet[CS_WEAPON2_DATA] |= DBI_GET_TYPE(lpObj->pInventory[1].m_Type);
		lpObj->CharSet[CS_WEAPON2_TYPE] = DBI_GET_INDEX(lpObj->pInventory[1].m_Type);
	}

	// Set Helmet
	if ( lpObj->pInventory[2].m_Type < ITEMGET(0,0) )
	{
		lpObj->CharSet[13] |= CS_SET_HELMET1(-1);
		lpObj->CharSet[9]  |= CS_SET_HELMET2(-1);
		lpObj->CharSet[3]  |= CS_SET_HELMET3(-1);
	}
	else
	{
		lpObj->CharSet[13] |= CS_SET_HELMET1(lpObj->pInventory[2].m_Type);
		lpObj->CharSet[9]  |= CS_SET_HELMET2(lpObj->pInventory[2].m_Type);
		lpObj->CharSet[3]  |= CS_SET_HELMET3(lpObj->pInventory[2].m_Type);
	}

	// Set Armor
	if ( lpObj->pInventory[3].m_Type < ITEMGET(0,0) )
	{
		lpObj->CharSet[14] |= CS_SET_ARMOR1(-1);
		lpObj->CharSet[9]  |= CS_SET_ARMOR2(-1);
		lpObj->CharSet[3]  |= CS_SET_ARMOR3(-1);
	}
	else
	{
		lpObj->CharSet[14] |= CS_SET_ARMOR1(lpObj->pInventory[3].m_Type);
		lpObj->CharSet[9]  |= CS_SET_ARMOR2(lpObj->pInventory[3].m_Type);
		lpObj->CharSet[3]  |= CS_SET_ARMOR3(lpObj->pInventory[3].m_Type);
	}

	// Set Pants
	if ( lpObj->pInventory[4].m_Type < ITEMGET(0,0) )
	{
		lpObj->CharSet[14] |= CS_SET_PANTS1(-1);
		lpObj->CharSet[9]  |= CS_SET_PANTS2(-1);
		lpObj->CharSet[4]  |= CS_SET_PANTS3(-1);
	}
	else
	{
		lpObj->CharSet[14] |= CS_SET_PANTS1(lpObj->pInventory[4].m_Type);
		lpObj->CharSet[9]  |= CS_SET_PANTS2(lpObj->pInventory[4].m_Type);
		lpObj->CharSet[4]  |= CS_SET_PANTS3(lpObj->pInventory[4].m_Type);
	}

	// Set Gloves
	if ( lpObj->pInventory[5].m_Type < ITEMGET(0,0) )
	{
		lpObj->CharSet[15] |= CS_SET_GLOVES1(-1);
		lpObj->CharSet[9]  |= CS_SET_GLOVES2(-1);
		lpObj->CharSet[4]  |= CS_SET_GLOVES3(-1);
	}
	else
	{
		lpObj->CharSet[15] |= CS_SET_GLOVES1(lpObj->pInventory[5].m_Type);
		lpObj->CharSet[9]  |= CS_SET_GLOVES2(lpObj->pInventory[5].m_Type);
		lpObj->CharSet[4]  |= CS_SET_GLOVES3(lpObj->pInventory[5].m_Type);
	}

	// Set Boots
	if ( lpObj->pInventory[6].m_Type < ITEMGET(0,0) )
	{
		lpObj->CharSet[15] |= CS_SET_BOOTS1(-1);
		lpObj->CharSet[9]  |= CS_SET_BOOTS2(-1);
		lpObj->CharSet[5]  |= CS_SET_BOOTS3(-1);
	}
	else
	{
		lpObj->CharSet[15] |= CS_SET_BOOTS1(lpObj->pInventory[6].m_Type);
		lpObj->CharSet[9]  |= CS_SET_BOOTS2(lpObj->pInventory[6].m_Type);
		lpObj->CharSet[5]  |= CS_SET_BOOTS3(lpObj->pInventory[6].m_Type);
	}

	index = 0;

#define SLOTNEGATIVE -1

	// Set Wings
	switch (lpObj->pInventory[7].m_Type)
	{
		case ITEMGET(12,0): // Wings of Elf
			lpObj->CharSet[5] |= CS_SET_WING1(1);
			lpObj->CharSet[9] |= 1;
			break;
		case ITEMGET(12,1): // Wings of Heaven
			lpObj->CharSet[5] |= CS_SET_WING1(1);
			lpObj->CharSet[9] |= 2;
			break;
		case ITEMGET(12,2): // Wings of Satan
			lpObj->CharSet[5] |= CS_SET_WING1(1);
			lpObj->CharSet[9] |= 3;
			break;
		case ITEMGET(12,41): // Wings of Misery
			lpObj->CharSet[5] |= CS_SET_WING1(1);
			lpObj->CharSet[9] |= 4;
			break;
		case ITEMGET(12,3): // Muse Elf Wings
			lpObj->CharSet[5] |= CS_SET_WING1(2);
			lpObj->CharSet[9] |= 1;
			break;
		case ITEMGET(12,4): // Soul Master Wings
			lpObj->CharSet[5] |= CS_SET_WING1(2);
			lpObj->CharSet[9] |= 2;
			break;
		case ITEMGET(12,5): // Blade Knight Wings
			lpObj->CharSet[5] |= CS_SET_WING1(2);
			lpObj->CharSet[9] |= 3;
			break;
		case ITEMGET(12,6): // Magic Gladiator Wings
			lpObj->CharSet[5] |= CS_SET_WING1(2);
			lpObj->CharSet[9] |= 4;
			break;
		case ITEMGET(13,30): // Cape of Lord
			lpObj->CharSet[5] |= CS_SET_WING1(2);
			lpObj->CharSet[9] |= 5;
			break;
		case ITEMGET(12,42): // Bloody Summoner Wings
			lpObj->CharSet[5] |= CS_SET_WING1(2);
			lpObj->CharSet[9] |= 6;
			break;
		case ITEMGET(12,49): // Cloak of Warrior
			lpObj->CharSet[5] |= CS_SET_WING1(2);
			lpObj->CharSet[9] |= 7;
			break;
		case ITEMGET(12,36): // Blade Master Wings
			lpObj->CharSet[5] |= CS_SET_WING1(-1);
			lpObj->CharSet[9] |= 1;
			break;
		case ITEMGET(12,37): // Grand Master Wings
			lpObj->CharSet[5] |= CS_SET_WING1(-1);
			lpObj->CharSet[9] |= 2;
			break;
		case ITEMGET(12,38): // High Elf Wings
			lpObj->CharSet[5] |= CS_SET_WING1(-1);
			lpObj->CharSet[9] |= 3;
			break;
		case ITEMGET(12,39): // Duel Master Wings
			lpObj->CharSet[5] |= CS_SET_WING1(-1);
			lpObj->CharSet[9] |= 4;
			break;
		case ITEMGET(12,40): // Lord Emperor Cape
			lpObj->CharSet[5] |= CS_SET_WING1(-1);
			lpObj->CharSet[9] |= 5;
			break;
		case ITEMGET(12,43): // Dimension Master Wings
			lpObj->CharSet[5] |= CS_SET_WING1(-1);
			lpObj->CharSet[9] |= 6;
			break;
		case ITEMGET(12,50): // Fist Master Cloak
			lpObj->CharSet[5] |= CS_SET_WING1(-1);
			lpObj->CharSet[9] |= 7;
			break;
		case ITEMGET(12,130): // Small Cape of Lord
			lpObj->CharSet[5] |= CS_SET_WING1(3);
			lpObj->CharSet[17] |= 32;
			break;
		case ITEMGET(12,131): // Small Wings of Misery
			lpObj->CharSet[5] |= CS_SET_WING1(3);
			lpObj->CharSet[17] |= 64;
			break;
		case ITEMGET(12,132): // Small Wings of Elf
			lpObj->CharSet[5] |= CS_SET_WING1(3);
			lpObj->CharSet[17] |= 96;
			break;
		case ITEMGET(12,133): // Small Wings of Heaven
			lpObj->CharSet[5] |= CS_SET_WING1(3);
			lpObj->CharSet[17] |= 128;
			break;
		case ITEMGET(12,134): // Small Wings of Satan
			lpObj->CharSet[5] |= CS_SET_WING1(3);
			lpObj->CharSet[17] |= 160;
			break;
		case ITEMGET(12,135): // Small Cloak of Warrior
			lpObj->CharSet[5] |= CS_SET_WING1(3);
			lpObj->CharSet[17] |= 192;
			break;
		default:
			index |= 0;
	}

	int levelindex = CS_SET_SMALLLEVEL_RH(LevelSmallConvert(aIndex, 0));
	levelindex |= CS_SET_SMALLLEVEL_LH(LevelSmallConvert(aIndex, 1));
	levelindex |= CS_SET_SMALLLEVEL_HELMET(LevelSmallConvert(aIndex, 2));
	levelindex |= CS_SET_SMALLLEVEL_ARMOR(LevelSmallConvert(aIndex, 3));
	levelindex |= CS_SET_SMALLLEVEL_PANTS(LevelSmallConvert(aIndex, 4));
	levelindex |= CS_SET_SMALLLEVEL_GLOVES(LevelSmallConvert(aIndex, 5));
	levelindex |= CS_SET_SMALLLEVEL_BOOTS(LevelSmallConvert(aIndex, 6));

	lpObj->CharSet[6] = CS_SET_SMALLLEVEL1(levelindex);
	lpObj->CharSet[7] = CS_SET_SMALLLEVEL2(levelindex);
	lpObj->CharSet[8] = CS_SET_SMALLLEVEL3(levelindex);


	// Set Excellent Items
	lpObj->CharSet[10] = 0;

	if ( lpObj->pInventory[2].IsExtItem() != FALSE )
		lpObj->CharSet[10] = (char)0x80;

	if ( lpObj->pInventory[3].IsExtItem() != FALSE )
		lpObj->CharSet[10] |= 0x40;

	if ( lpObj->pInventory[4].IsExtItem() != FALSE )
		lpObj->CharSet[10] |= 0x20;

	if ( lpObj->pInventory[5].IsExtItem() != FALSE )
		lpObj->CharSet[10] |= 0x10;

	if ( lpObj->pInventory[6].IsExtItem() != FALSE )
		lpObj->CharSet[10] |= 0x8;

	if ( lpObj->pInventory[0].IsExtItem() != FALSE )
		lpObj->CharSet[10] |= 0x04;

	if ( lpObj->pInventory[1].IsExtItem() != FALSE )
		lpObj->CharSet[10] |= 0x02;

	// Set Set Items
	lpObj->CharSet[11] = 0;

	if ( lpObj->pInventory[2].IsSetItem() != FALSE )
		lpObj->CharSet[11] = (char)0x80;

	if ( lpObj->pInventory[3].IsSetItem() != FALSE )
		lpObj->CharSet[11] |= 0x40;

	if ( lpObj->pInventory[4].IsSetItem() != FALSE )
		lpObj->CharSet[11] |= 0x20;

	if ( lpObj->pInventory[5].IsSetItem() != FALSE )
		lpObj->CharSet[11] |= 0x10;

	if ( lpObj->pInventory[6].IsSetItem() != FALSE )
		lpObj->CharSet[11] |= 0x8;

	if ( lpObj->pInventory[0].IsSetItem() != FALSE )
		lpObj->CharSet[11] |= 0x04;

	if ( lpObj->pInventory[1].IsSetItem() != FALSE )
		lpObj->CharSet[11] |= 0x02;

	gCalCharacter.gObjCalCharacter(aIndex);

	if ( lpObj->IsFullSetItem != false )
	{
		lpObj->CharSet[11] |= 0x01;
	}

	// Pets Set
	switch ( lpObj->pInventory[8].m_Type )
	{
	case ITEMGET(13,0):
	case ITEMGET(13,1):
	case ITEMGET(13,2):
		index |= CS_SET_HELPER(lpObj->pInventory[8].m_Type);
		break;
	case ITEMGET(13,3): // Dinorant
		index |= CS_SET_HELPER(lpObj->pInventory[8].m_Type);
		lpObj->CharSet[10] |= 0x01;
		break;
	case ITEMGET(13,4): // Dark Horse
		index |= CS_SET_HELPER(-1);
		lpObj->CharSet[12] |= 0x01;
		break;
	case ITEMGET(13,80): // Panda Pet
		lpObj->CharSet[16] |= 224;
		break;
	case ITEMGET(13,123): // Skeleton Pet
		//lpObj->CharSet[5] -= 3;
		lpObj->CharSet[16] |= 96;
		break;
	case ITEMGET(13,67): // Rudolf Pet
		//lpObj->CharSet[5] -= 3;
		lpObj->CharSet[16] |= 0x80;
		break;
	case ITEMGET(13,65): // Spirit of Guardian
		lpObj->CharSet[16] |= 0x40;
		break;
	case ITEMGET(13,64): // Demon Pet
		lpObj->CharSet[16] |= 0x20;
		break;
	case ITEMGET(13,37): // Fenrir
		index |= 3;
		lpObj->CharSet[10] &= 0xFE;
		lpObj->CharSet[12] &= 0xFE;
		lpObj->CharSet[12] |= 0x04;
		lpObj->CharSet[16] = 0;

		if ( lpObj->pInventory[8].IsFenrirIncLastAttackDamage() != FALSE )
		{
			lpObj->CharSet[16] |= 0x01; // Red Fenrir
		}

		if ( lpObj->pInventory[8].IsFenrirDecLastAttackDamage() != FALSE )
		{
			lpObj->CharSet[16] |= 0x02; // Blue Fenrir
		}

		if ( lpObj->pInventory[8].IsFenrirIllusionOption() != FALSE )
		{
			lpObj->CharSet[17] |= 0x01; // Illusion Fenrir
		}
		break;
	default:
		index |= CS_SET_HELPER(-1);
		break;
	}
	lpObj->CharSet[5] |= index;
}

void gObjViewportPaint(HWND hWnd, short aIndex)
{
	HDC hdc;
	int PaintPlayer		= 0;
	int PaintMonster	= 0;
	int PaintNPC		= 0;
	int TotalPlayers	= 0;
	int count			= 0;
	int count2			= 0;
	int count3			= 0;
	char szTemp[256];
	HFONT chFont;

	if ( !OBJMAX_RANGE(aIndex))
		return;

	hdc = GetDC(hWnd);

	for ( int n = 0; n < OBJMAX; n++)
	{
		if ( gObj[n].Live != FALSE )
		{
			if ( gObj[n].Connected != PLAYER_EMPTY )
			{
				if ( gMainApp.bCurPaintMapNumber == gObj[n].MapNumber )
				{
					if( gObj[n].Type == OBJ_USER )
					{
						PaintPlayer++;
					}
					else if( gObj[n].Type == OBJ_USER )
					{
						PaintMonster++;
					}
					else if( gObj[n].Type == OBJ_NPC )
					{
						PaintNPC++;
					}
				}
			}
		}

		if( gObj[n].Type == OBJ_USER && gObj[n].Connected != PLAYER_EMPTY )
		{
			TotalPlayers++;
		}
		else if(gObj[n].Connected != PLAYER_EMPTY)
		{
			count++;
		}
	}
	// ----
	gObjTotalUser = TotalPlayers;
	// ----
	wsprintf(szTemp, "Total Monsters:(%d/%d) - TotalPlayers:(%d) - Player[%d]:(%d) - VipCount:(%d)[%d/%d] - Item Count:(%d) - Max Players:(%d)",
		count, OBJ_MAXMONSTER, TotalPlayers, aIndex, PaintPlayer, gObj[aIndex].VPCount, count3, count2, gItemLoop, gServerMaxUser);
	// ----
	chFont = CreateFontA(15, 0, 0, 0, FW_THIN, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
		DEFAULT_PITCH|FF_DONTCARE, "Lucida Sans");
	// ----
	int iOldBkMode = SetBkMode(hdc, TRANSPARENT);
	// ----
	HFONT pOldFont = (HFONT)SelectObject(hdc, chFont);
	// ----
	SetTextColor(hdc, TColor.Gray());
	// ----
	TextOutA(hdc, 360, 0, szTemp, strlen(szTemp));
	SelectObject(hdc, pOldFont);
	SetBkMode(hdc, iOldBkMode);
	ReleaseDC(hWnd, hdc);
}

static int FrustrumX[MAX_ARRAY_FRUSTRUM];
static int FrustrumY[MAX_ARRAY_FRUSTRUM];

void InitFrustrum()
{
	float CameraViewFar = 2400.0;
	float CameraViewNear = CameraViewFar * 0.19f;
	float CameraViewTarget = CameraViewFar * 0.53f;
	float WidthFar = 1190.0;
	float WidthNear =  550.0;

	float p[4][3];

	p[0][0] = -WidthFar;
	p[0][1] = CameraViewFar-CameraViewTarget;
	p[0][2] = 0;
	p[1][0] = WidthFar;
	p[1][1] = CameraViewFar-CameraViewTarget;
	p[1][2] = 0;
	p[2][0] = WidthNear;
	p[2][1] = CameraViewNear - CameraViewTarget;
	p[2][2] = 0;
	p[3][0]= -WidthNear;
	p[3][1] = CameraViewNear - CameraViewTarget;
	p[3][2] = 0;

	float Angle[3];
	Angle[0] = 0;
	Angle[1] = 0;
	Angle[2] = 45.0f;

	float Matrix[3][4];
	float Frustrum[4][3];

	AngleMatrix(Angle,Matrix);

	for(int i = 0; i < MAX_ARRAY_FRUSTRUM;i++)
	{
		VectorRotate((const float *)&p[i],(const float *)&Matrix[0],(float *)&Frustrum[i]);
		FrustrumX[i] = Frustrum[i][0]*0.01f;
		FrustrumY[i] = Frustrum[i][1]*0.01f;

	}
}

void CreateFrustrum(int x, int y, int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];

	for ( int i=0;i<MAX_ARRAY_FRUSTRUM;i++)
	{
		lpObj->FrustrumX[i] = FrustrumX[i] + x;
		lpObj->FrustrumY[i] = FrustrumY[i] + y;
	}
}

bool gObjCheckViewport(int aIndex, int x, int y)
{
	LPOBJ lpObj = &gObj[aIndex];

	if(x < lpObj->X - 15  || x > lpObj->X + 15 || y < lpObj->Y - 15 || y > lpObj->Y + 15)
	{
		return false;
	}

	int j = 3;

	for(int i = 0; i < MAX_ARRAY_FRUSTRUM; j = i, i++)
	{
		int frustrum = (lpObj->FrustrumX[i]- x) * (lpObj->FrustrumY[j]-y) - (lpObj->FrustrumX[j]-x) * (lpObj->FrustrumY[i]-y);
		if(frustrum < 0)
		{
			return false;
		}
	}
	return true;
}

void gObjViewportClose(LPOBJ lpObj)
{
	int tObjNum,i;
	int MVL = MAX_VIEWPORT;

	if(lpObj->Type == OBJ_MONSTER)
	{
		MVL = MAX_VIEWPORT_MONSTER;
	}

	if(lpObj->Connected == PLAYER_PLAYING)
	{
		for(int n = 0; n < MVL; n++)
		{
			if(lpObj->VpPlayer2[n].state != 0)
			{
				if(lpObj->VpPlayer2[n].type != 5)
				{
					tObjNum = lpObj->VpPlayer2[n].number;

					i = 0;

					while( true )
					{
						if(gObj[tObjNum].VpPlayer[i].number == lpObj->m_Index)
						{
							if(gObj[tObjNum].VpPlayer[i].type == lpObj->Type)
							{
								gObj[tObjNum].VpPlayer[i].state = 3;
								break;
							}
						}

						if(i < MVL-1)
						{
							i++;
						}
						else
						{
							break;
						}
					}

					i = 0;

					while( true )
					{
						if(gObj[tObjNum].VpPlayer2[i].number == lpObj->m_Index)
						{
							if(gObj[tObjNum].VpPlayer2[i].type == lpObj->Type)
							{
								gObj[tObjNum].VpPlayer2[i].state = 0;
								break;
							}
						}

						if(i < MVL-1)
						{
							i++;
						}
						else
						{
							break;
						}
					}
				}
			}
		}
	}
	gObjClearViewport(lpObj);
}

void gObjViewportListCreate(short aIndex)
{
	int result,n;
	LPOBJ lpObj;
	int mapnum;

	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return;
	}

	lpObj = &gObj[aIndex];

	if(lpObj->Connected < PLAYER_PLAYING)
	{
		return;
	}

	if(lpObj->RegenOk > 0)
	{
		return;
	}

	mapnum = lpObj->MapNumber;
	gItemLoop2 = 0;

	if(lpObj->Type == OBJ_USER)
	{
		MapClass * lpMap = &MapC[mapnum];

		for(n = 0; n < MAX_MAPITEM; n++)
		{
			if(lpMap->m_cItem[n].live)
			{
				gItemLoop2++;

				if(lpMap->m_cItem[n].m_State == 1 || lpMap->m_cItem[n].m_State == 2)
				{
					if(gObjCheckViewport(aIndex,lpMap->m_cItem[n].px,lpMap->m_cItem[n].py) == 1)
					{
						result = ViewportAdd(aIndex,n,5);
					}
				}
			}
		}
	}

	if(aIndex == 0)
	{
		gItemLoopMax = gItemLoop2;
		gItemLoop = gItemLoop2;
		gMainApp.bCurPaintPlayer = aIndex;
	}

	int a = 1;
	LPOBJ lpTempObj;

	if(lpObj->Type == OBJ_USER)
	{
		for(n = 0; n < OBJMAX; n++)
		{
			lpTempObj = &gObj[n];

			if(lpTempObj->Connected == PLAYER_PLAYING && aIndex != n)
			{
				if(lpTempObj->m_State == 1 || lpTempObj->m_State == 2)
				{
					if(mapnum == lpTempObj->MapNumber)
					{
						if(gObjCheckViewport(aIndex,lpTempObj->X,lpTempObj->Y) == 1)
						{
							result = ViewportAdd(aIndex,n,lpTempObj->Type);
							result = ViewportAdd2(n,aIndex,gObj[aIndex].Type);
						}
					}
				}
			}
		}
	}
	else if(lpObj->Type == OBJ_MONSTER)
	{
		for(n = OBJ_MAXMONSTER; n < OBJMAX; n++)
		{
			lpTempObj = &gObj[n];

			if(lpTempObj->Connected == PLAYER_PLAYING && aIndex != n)
			{
				if(lpTempObj->m_State == 1 || lpTempObj->m_State == 2)
				{
					if(mapnum == lpTempObj->MapNumber)
					{
						if(gObjCheckViewport(aIndex,lpTempObj->X,lpTempObj->Y) == 1)
						{
							result = ViewportAdd(aIndex,n,gObj[n].Type);
							result = ViewportAdd2(n,aIndex,gObj[aIndex].Type);
						}
					}
				}
			}
		}
	}
}

int  ViewportAdd(int aIndex, int aAddIndex, int aType)
{
	int blank = -1;
	int count = 0;
	LPOBJ lpObj = &gObj[aIndex];
	int MVL = MAX_VIEWPORT;

	if(lpObj->Type == OBJ_MONSTER)
	{
		MVL = MAX_VIEWPORT_MONSTER;
	}

	for(int n = 0; n < MVL; n++)
	{
		if(lpObj->VpPlayer[n].state != 0)
		{
			if(lpObj->VpPlayer[n].number == aAddIndex)
			{
				if(lpObj->VpPlayer[n].type == aType)
				{
					return -1;
				}
			}
		}
		else
		{
			if(blank < 0)
			{
				blank = n;
			}
		}
	}

	if(blank < 0)
	{
		return -1;
	}


	lpObj->VpPlayer[blank].state = 1;
	lpObj->VpPlayer[blank].number = aAddIndex;
	lpObj->VpPlayer[blank].type = aType;
	lpObj->VPCount ++;

	if(lpObj->Type == OBJ_MONSTER || lpObj->Type == OBJ_NPC)
	{
		if(aType == 1)
		{
			if(lpObj->m_iCurrentAI != 0)
			{
				lpObj->m_Agro.SetAgro(aAddIndex,1000);
			}
		}
	}
	return blank;
}

int  ViewportAdd2(int aIndex, int aAddIndex, int aType)
{
	int blank = -1;
	int count = 0;
	LPOBJ lpObj = &gObj[aIndex];
	int MVL = MAX_VIEWPORT;

	if(lpObj->Type == OBJ_MONSTER)
	{
		MVL = MAX_VIEWPORT_MONSTER;
	}

	for(int n = 0; n < MVL; n++)
	{
		if(lpObj->VpPlayer2[n].state != 0)
		{
			if(lpObj->VpPlayer2[n].number == aAddIndex)
			{
				if(lpObj->VpPlayer2[n].type == aType)
				{
					return -1;
				}
			}
		}
		else
		{
			if(blank < 0)
			{
				blank = n;
			}
		}
	}

	if(blank < 0)
	{
		return -1;
	}


	lpObj->VpPlayer2[blank].state = 1;
	lpObj->VpPlayer2[blank].number = aAddIndex;
	lpObj->VpPlayer2[blank].type = aType;
	lpObj->VPCount2 ++;

	return blank;
}

void ViewportDel(short aIndex, int aDelIndex)
{
	if(gObjIsConnected(aIndex) == 0)
	{
		return;
	}

	if(gObjIsConnected(aDelIndex) == 0)
	{
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];
	int MVL = MAX_VIEWPORT;

	if(lpObj->Type == OBJ_MONSTER)
	{
		MVL = MAX_VIEWPORT_MONSTER;
	}

	for(int n = 0; n < MVL; n++)
	{
		if(lpObj->VpPlayer[n].state == 1 || lpObj->VpPlayer[n].state == 2)
		{
			if(lpObj->VpPlayer[n].number == aDelIndex)
			{
				lpObj->VpPlayer[n].state = 0;
				lpObj->VpPlayer[n].number = -1;
				lpObj->VPCount --;
			}
		}
	}
}

void Viewport2Del(short aIndex, int aDelIndex)
{
	if(gObjIsConnected(aIndex) == 0)
	{
		return;
	}

	if(gObjIsConnected(aDelIndex) == 0)
	{
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];
	int MVL = MAX_VIEWPORT;

	if(lpObj->Type == OBJ_MONSTER)
	{
		MVL = MAX_VIEWPORT_MONSTER;
	}

	for(int n = 0; n < MVL; n++)
	{
		if(lpObj->VpPlayer2[n].state == 1 || lpObj->VpPlayer2[n].state == 2)
		{
			if(lpObj->VpPlayer2[n].number == aDelIndex)
			{
				lpObj->VpPlayer2[n].state = 0;
				lpObj->VpPlayer2[n].number = -1;
				lpObj->VPCount2 --;
			}
		}
	}
}

void gObjViewportAllDel(short aIndex)
{
	if(gObjIsConnected(aIndex) == 0)
	{
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];
	int MVL = MAX_VIEWPORT;

	if(lpObj->Type == OBJ_MONSTER)
	{
		MVL = MAX_VIEWPORT_MONSTER;
	}

	for(int n = 0; n < MVL; n++)
	{
		if(lpObj->VpPlayer[n].state == 1 || lpObj->VpPlayer[n].state == 2)
		{
			Viewport2Del(lpObj->VpPlayer[n].number,aIndex);
		}

		if(lpObj->VpPlayer2[n].state == 1 || lpObj->VpPlayer2[n].state == 2)
		{
			ViewportDel(lpObj->VpPlayer2[n].number,aIndex);
		}
	}
}

void gObjViewportListDestroy(short aIndex)
{
	short tObjNum;
	int n;
	LPOBJ lpObj;
	int MVL;

	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return;
	}

	if(gObj[aIndex].Connected < PLAYER_PLAYING)
	{
		return;
	}

	lpObj = &gObj[aIndex];
	MVL = MAX_VIEWPORT;

	if(lpObj->Type == OBJ_MONSTER)
	{
		MVL = MAX_VIEWPORT_MONSTER;
	}

	for(n = 0; n < MVL; n++)
	{
		int mapnum;
		if(lpObj->VpPlayer[n].state == 1 || lpObj->VpPlayer[n].state == 2)
		{
			tObjNum = lpObj->VpPlayer[n].number;

			switch(lpObj->VpPlayer[n].type)
			{
			case 5:
				mapnum = lpObj->MapNumber;

				if(MapC[mapnum].m_cItem[tObjNum].IsItem() == 1)
				{
					if(MapC[mapnum].m_cItem[tObjNum].m_State == 8)
					{
						lpObj->VpPlayer[n].state = 3;
					}
					else
					{
						if(gObjCheckViewport(aIndex,MapC[mapnum].m_cItem[tObjNum].px,MapC[mapnum].m_cItem[tObjNum].py) == 0)
						{
							lpObj->VpPlayer[n].state = 3;
						}
					}
				}
				else
				{
					if(MapC[mapnum].m_cItem[tObjNum].live == 0 || MapC[mapnum].m_cItem[tObjNum].Give == 0)
					{
						lpObj->VpPlayer[n].state = 3;
					}
					else
					{
						lpObj->VpPlayer[n].state = 3;
					}
				}
				break;
			default : 
				if(gObj[tObjNum].m_State == 1)
				{
					if(gObj[tObjNum].Live == 0)
					{
						lpObj->VpPlayer[n].state = 3;
					}
				}

				if(gObj[tObjNum].Connected  == PLAYER_EMPTY ||
					gObj[tObjNum].m_State == 8 ||
					gObj[tObjNum].Teleport != 0 ||
					gObj[tObjNum].m_State == 32)
				{
					lpObj->VpPlayer[n].state = 3;
				}
				else
				{
					if(gObjCheckViewport(aIndex,gObj[tObjNum].X,gObj[tObjNum].Y) == 0)
					{
						lpObj->VpPlayer[n].state = 3;

						if(lpObj->Type == OBJ_MONSTER || lpObj->Type == OBJ_NPC)
						{
							if(gObj[tObjNum].Type == OBJ_USER)
							{
								if(lpObj->m_iCurrentAI != 0)
								{
									lpObj->m_Agro.DelAgro(tObjNum);
								}
							}
						}
					}
				}
				break;
			}
		}

		if(lpObj->VpPlayer2[n].state == 1 || lpObj->VpPlayer2[n].state == 2)
		{
			tObjNum = lpObj->VpPlayer2[n].number;

			if(gObj[tObjNum].Connected < PLAYER_PLAYING)
			{
				lpObj->VpPlayer2[n].state = 0;
				lpObj->VpPlayer2[n].number = -1;
				lpObj->VPCount2 --;
			}
			else
			{
				if(gObjCheckViewport(tObjNum,lpObj->X,lpObj->Y) == 0)
				{
					lpObj->VpPlayer2[n].state = 0;
					lpObj->VpPlayer2[n].number = -1;
					lpObj->VPCount2 --;
				}
			}
		}
	}
}

void PShop_ViewportListRegenarate(short aIndex)
{
	if(gObjIsConnected(aIndex) == 0)
	{
		return ;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if(lpObj->Type != OBJ_USER)
	{
		return;
	}

	if(lpObj->RegenOk > 0)
	{
		return;
	}

	if(lpObj->CloseCount > -1)
	{
		return;
	}

	BYTE cBUFFER[6000];
	int iVpOpenCount = 0;
	int iVpAddCount = 0;

	PMSG_ANS_PSHOP_VIEWPORT_NOTIFY * lpMsg = (PMSG_ANS_PSHOP_VIEWPORT_NOTIFY *)&cBUFFER;
	PMSG_PSHOP_VIEWPORT_NOTIFY * lpMsgBody = (PMSG_PSHOP_VIEWPORT_NOTIFY *)&cBUFFER[6];

	int iPShopOpenIndex[MAX_VIEWPORT];

	if(lpObj->m_bPShopRedrawAbs != 0)
	{
		memset(lpObj->m_iVpPShopPlayer,0x00,sizeof(lpObj->m_iVpPShopPlayer));
		lpObj->m_wVpPShopPlayerCount = 0;
		lpObj->m_bPShopRedrawAbs = 0;
	}

	for(int n = 0; n < MAX_VIEWPORT; n++)
	{
		if(lpObj->VpPlayer[n].state == 2 && lpObj->VpPlayer[n].type == OBJ_USER && gObj[lpObj->VpPlayer[n].number].m_bPShopOpen == 1)
		{
			iPShopOpenIndex[iVpOpenCount] = lpObj->VpPlayer[n].number;
			iVpOpenCount++;

			if(lpObj->m_wVpPShopPlayerCount > 0 && lpObj->m_wVpPShopPlayerCount <= MAX_VIEWPORT)
			{
				int found = 0;

				for(int i = 0; i < lpObj->m_wVpPShopPlayerCount; i++)
				{
					if(lpObj->m_iVpPShopPlayer[i] == lpObj->VpPlayer[n].number)
					{
						found = 1;
						break;
					}
				}

				if(found == 0)
				{
					lpMsgBody[iVpAddCount].btNumberL = SET_NUMBERL(WORD(lpObj->VpPlayer[n].number));
					lpMsgBody[iVpAddCount].btNumberH = SET_NUMBERH(WORD(lpObj->VpPlayer[n].number));
					memcpy(lpMsgBody[iVpAddCount].szPShopText,gObj[lpObj->VpPlayer[n].number].m_szPShopText,sizeof(gObj[lpObj->VpPlayer[n].number].m_szPShopText));
					iVpAddCount++;
				}
			}
			else if(lpObj->m_wVpPShopPlayerCount == 0)
			{
				lpMsgBody[iVpAddCount].btNumberL = SET_NUMBERL(WORD(lpObj->VpPlayer[n].number));
				lpMsgBody[iVpAddCount].btNumberH = SET_NUMBERH(WORD(lpObj->VpPlayer[n].number));
				memcpy(lpMsgBody[iVpAddCount].szPShopText,gObj[lpObj->VpPlayer[n].number].m_szPShopText,sizeof(gObj[lpObj->VpPlayer[n].number].m_szPShopText));
				iVpAddCount++;
			}
			else
			{
				CLog.LogAdd("[PShop] ERROR : lpObj->m_wVpPShopPlayerCount is OUT of BOUND : %d",lpObj->m_wVpPShopPlayerCount);
			}
		}
	}

	memset(lpObj->m_iVpPShopPlayer,0x00,sizeof(lpObj->m_iVpPShopPlayer));
	lpObj->m_wVpPShopPlayerCount = 0;

	if(iVpOpenCount > 0 && iVpOpenCount <= MAX_VIEWPORT)
	{
		for( int n = 0; n < iVpOpenCount; n++)
		{
			lpObj->m_iVpPShopPlayer[n] = iPShopOpenIndex[n];
		}
		lpObj->m_wVpPShopPlayerCount = iVpOpenCount;
	}
	else if(iVpOpenCount != 0)
	{
		CLog.LogAdd("[PShop] ERROR : iVpOpenCount is OUT of BOUND: %d",iVpOpenCount);
	}

	if(iVpAddCount > 0 && iVpAddCount <= MAX_VIEWPORT)
	{
		lpMsg->btCount = iVpAddCount;
		C2SubHeadSet((unsigned char *)lpMsg,0x3F,0,sizeof(PMSG_ANS_PSHOP_VIEWPORT_NOTIFY)+sizeof(PMSG_PSHOP_VIEWPORT_NOTIFY)*iVpAddCount);
		gSendProto.DataSend(aIndex,(unsigned char *)lpMsg,((lpMsg->h.sizeL & 0xFF)& 0xFF | ((lpMsg->h.sizeH & 0xFF) & 0xFF) << 8) & 0xFFFF);
	}
	else if(iVpAddCount != 0)
	{
		CLog.LogAdd("[PShop] ERROR : iVpAddCount is OUT of BOUND: %d",iVpAddCount);
	}

	if(gObj[aIndex].m_bPShopWantDeal == 1)
	{
		if(gObjIsConnected(gObj[aIndex].m_iPShopDealerIndex) != 0)
		{
			if(gObj[gObj[aIndex].m_iPShopDealerIndex].Type == OBJ_USER)
			{
				if(gObj[gObj[aIndex].m_iPShopDealerIndex].m_bPShopOpen == 0)
				{
					gObj[aIndex].m_bPShopWantDeal = 0;
					gObj[aIndex].m_iPShopDealerIndex = -1;
					memset(gObj[aIndex].m_szPShopDealerName,0,sizeof(gObj[aIndex].m_szPShopDealerName));

					CPShop.SendPShopDealerClosedShop(aIndex,gObj[aIndex].m_iPShopDealerIndex);
				}
				else if(gObj[gObj[aIndex].m_iPShopDealerIndex].CloseCount >= 0 || gObj[aIndex].CloseCount >= 0)
				{
					gObj[aIndex].m_bPShopWantDeal = 0;
					gObj[aIndex].m_iPShopDealerIndex = -1;
					memset(gObj[aIndex].m_szPShopDealerName,0,sizeof(gObj[aIndex].m_szPShopDealerName));

					CPShop.SendPShopDealerClosedShop(aIndex,gObj[aIndex].m_iPShopDealerIndex);
				}
				else if(gObj[gObj[aIndex].m_iPShopDealerIndex].m_bPShopItemChange == 1)
				{
					CPShop.SendPlShopBuyListResult(aIndex,gObj[aIndex].m_iPShopDealerIndex,1,1);
				}
			}
			else
			{
				gObj[aIndex].m_bPShopWantDeal = 0;
				gObj[aIndex].m_iPShopDealerIndex = -1;
				memset(gObj[aIndex].m_szPShopDealerName,0,sizeof(gObj[aIndex].m_szPShopDealerName));

				CPShop.SendPShopDealerClosedShop(aIndex,gObj[aIndex].m_iPShopDealerIndex);
			}
		}
		else
		{
			gObj[aIndex].m_bPShopWantDeal = 0;
			gObj[aIndex].m_iPShopDealerIndex = -1;
			memset(gObj[aIndex].m_szPShopDealerName,0,sizeof(gObj[aIndex].m_szPShopDealerName));

			CPShop.SendPShopDealerClosedShop(aIndex,gObj[aIndex].m_iPShopDealerIndex);
		}
	}
}

bool PShop_CheckInventoryEmpty(short aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];

	for(int i = MAIN_INVENTORY_SIZE; i < INVENTORY_SIZE; i++)
	{
		if(lpObj->Inventory1[i].IsItem() == 1)
		{
			return false;
		}
	}
	return true;
}

void gObjStateSetCreate(int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];
	
	if(lpObj->Connected < PLAYER_PLAYING)
	{
		return;
	}
	
	DWORD dwNowTick = GetTickCount();
	
	if(lpObj->DieRegen == 1)
	{
		if(dwNowTick - lpObj->RegenTime > lpObj->MaxRegenTime + 1000)
		{
			lpObj->DieRegen = 2;
			lpObj->m_State = 8;
		}
	}
	
	if(lpObj->Type == OBJ_USER)
	{
		if(lpObj->m_State == 8)
		{
			gObjReady4Relife(lpObj);
		}
		else
		{
			gObjCheckTimeOutValue(lpObj,(DWORD &)dwNowTick);
		}
	}
	
	if(lpObj->Teleport == 1)
	{
		if(lpObj->DieRegen != 0)
		{
			lpObj->Teleport = 0;
		}
		else if(GetTickCount() - lpObj->TeleportTime > 100)
		{
			lpObj->Teleport = 2;
		}
	}
	else if(lpObj->Teleport == 2)
	{
		lpObj->Teleport = 3;
		
		if(lpObj->Type == OBJ_USER)
		{
			lpObj->TX = lpObj->X;
			lpObj->TY = lpObj->Y;
			
			CreateFrustrum(lpObj->X,lpObj->Y,aIndex);
			GCTeleportSend(&gObj[aIndex],0,lpObj->MapNumber,lpObj->X,lpObj->Y,lpObj->Dir);
			gObjViewportListProtocolCreate(lpObj);
			
#if(GS_CASTLE)
			if(g_CastleSiege.GetCastleState() == 7)
			{
				g_CastleSiege.NotifySelfCsJoinSide(lpObj->m_Index);
				g_CastleSiege.NotifyCsSelfLeftTime(lpObj->m_Index);
			}
#endif
			lpObj->Teleport = 0;
		}
		else if(lpObj->Type == OBJ_NPC && lpObj->Class == 229)
		{
			int x = lpObj->X;
			int y = lpObj->Y;
			
			lpObj->X = x;
			lpObj->Y = y;
			
			lpObj->TX = lpObj->X;
			lpObj->TY = lpObj->Y;
			
			CreateFrustrum(lpObj->X,lpObj->Y,aIndex);
			
			lpObj->m_State = 1;
			lpObj->PathStartEnd = 0;
		}
		else if(lpObj->Type == OBJ_MONSTER)
		{
			lpObj->TX = lpObj->X;
			lpObj->TY = lpObj->Y;
			
			CreateFrustrum(lpObj->X,lpObj->Y,aIndex);
			
			gObjViewportListCreate(lpObj->m_Index);
			gObjViewportListProtocolCreate(lpObj);
			
			lpObj->Teleport = 0;
			lpObj->PathStartEnd = 0;
			lpObj->PathCur = 0;
			lpObj->PathCount = 0;
		}
		else 
		{
			int x,y;
			
			if(lpObj->m_RecallMon >= 0)
			{
				x = lpObj->X;
				y = lpObj->Y;
			}
			else
			{
				x = lpObj->X + rand()%6 - 3;
				y = lpObj->Y + rand()%6 - 3;
			}
			
			lpObj->X = x;
			lpObj->Y = y;
			
			lpObj->TX = lpObj->X;
			lpObj->TY = lpObj->Y;
			
			if(lpObj->m_RecallMon >= 0)
			{
				gObjViewportListProtocolCreate(lpObj);
				lpObj->PathStartEnd = 0;
			}
		}
	}
	// NEED FIX HERE DUEL
	if(lpObj->RegenOk == 2 && lpObj->Type == OBJ_USER)
	{
		lpObj->m_State = 1;
		lpObj->RegenOk = 3;
		lpObj->X = lpObj->RegenMapX;
		lpObj->Y = lpObj->RegenMapY;
		lpObj->MapNumber = lpObj->RegenMapNumber;
		
		lpObj->TX = lpObj->X;
		lpObj->TY = lpObj->Y;
		
		CreateFrustrum(lpObj->X,lpObj->Y,aIndex);
		
#if(GS_CASTLE)
		if(lpObj->MapNumber == MAP_INDEX_CASTLESIEGE)
		{
			GCAnsCsNotifyStart(lpObj->m_Index,CASTLESIEGECHECKSTATE(g_CastleSiege.GetCastleState()) );
			
			if(g_CastleSiege.GetCastleState() == 7)
			{
				g_CastleSiege.NotifySelfCsJoinSide(lpObj->m_Index);
				g_CastleSiege.NotifyCsSelfLeftTime(lpObj->m_Index);
			}
		}
#endif
	}
}

void gObjSetState()
{
	int n;
	LPOBJ lpObj;

	BYTE Socket[5];
	Socket[0] = 0xFF;
	Socket[1] = 0xFF;
	Socket[2] = 0xFF;
	Socket[3] = 0xFF;
	Socket[4] = 0xFF;
	
	for(n = 0; n < OBJMAX; n++)
	{
		lpObj = &gObj[n];
		
		if(lpObj->Connected > PLAYER_LOGGED)
		{
			if(lpObj->m_State == 1)
			{
				lpObj->m_State = 2;
				if(lpObj->RegenOk == 3)
				{
					lpObj->RegenOk = 0;
				}
			}
			
			if(lpObj->Teleport == 3)
			{
				lpObj->Teleport = 0;
			}
			
			if(lpObj->DieRegen == 2)
			{
				int mapnum = lpObj->MapNumber;
				int x = lpObj->X;
				int y = lpObj->Y;
				
				lpObj->m_bAttackerKilled = 0;
				
				if(DS_MAP_RANGE(lpObj->MapNumber))
				{
					if(lpObj->Type >= OBJ_MONSTER)
					{
						g_DevilSquare.gDevilSquareMonsterRegen(lpObj);
						continue;
					}
				}
#if(!GS_CASTLE)
				if(lpObj->MapNumber == MAP_INDEX_KANTURU_BOSS && lpObj->Type == OBJ_MONSTER)
				{
					continue;
				}
#endif
				if(lpObj->m_iCurrentAI != 0 && lpObj->m_iRegenType != 0)
				{
					continue;
				}
				
#if(GS_CASTLE)
				if(lpObj->MapNumber == MAP_INDEX_CRYWOLF_FIRSTZONE)
				{
					if(g_CrywolfSync.GetCrywolfState() == 3)
					{
						if(lpObj->Type >= OBJ_MONSTER)
						{
							continue;
						}
					}
				}
#endif
				
				/*if(lpObj->m_bIsInMonsterHerd != 0)
				{
					if(lpObj->m_lpMonsterHerd != 0)
					{
						lpObj->m_lpMonsterHerd->MonsterRegenAction(lpObj);
					}
					continue;
				}*/
				
				if( gUser.BloodCastleMapRange(lpObj->MapNumber) == true && lpObj->Type >= OBJ_MONSTER)
				{
					if(lpObj->Class == 131)
					{
						g_BloodCastle.m_BridgeData[g_BloodCastle.GetBloodCastleIndex(lpObj->MapNumber)].m_bCASTLE_DOOR_LIVE = 0;
						g_BloodCastle.m_BridgeData[g_BloodCastle.GetBloodCastleIndex(lpObj->MapNumber)].m_bBC_DOOR_TERMINATE_COMPLETE = 1;
						g_BloodCastle.m_BridgeData[g_BloodCastle.GetBloodCastleIndex(lpObj->MapNumber)].m_iBC_BOSS_MONSTER_MAX_COUNT = g_BloodCastle.GetCurrentLiveUserCount(g_BloodCastle.GetBloodCastleIndex(lpObj->MapNumber)) * 2;
						
						g_BloodCastle.m_BridgeData[g_BloodCastle.GetBloodCastleIndex(lpObj->MapNumber)].m_iBC_BOSS_MONSTER_KILL_COUNT = 0;
						
						if(g_BloodCastle.m_BridgeData[g_BloodCastle.GetBloodCastleIndex(lpObj->MapNumber)].m_iBC_BOSS_MONSTER_MAX_COUNT > 10)
						{
							g_BloodCastle.m_BridgeData[g_BloodCastle.GetBloodCastleIndex(lpObj->MapNumber)].m_iBC_BOSS_MONSTER_MAX_COUNT = 10;
						}
						
						g_BloodCastle.SendCastleDoorBlockInfo(g_BloodCastle.GetBloodCastleIndex(lpObj->MapNumber),0);
						g_BloodCastle.ReleaseCastleDoor(g_BloodCastle.GetBloodCastleIndex(lpObj->MapNumber));
						
						if(g_BloodCastle.GetCurrentState(g_BloodCastle.GetBloodCastleIndex(lpObj->MapNumber)) == 2)
						{
							g_BloodCastle.SetBossMonster(g_BloodCastle.GetBloodCastleIndex(lpObj->MapNumber));
						}
					}
					
					if(BC_STATUE_RANGE(lpObj->Class - 132) && gUser.BloodCastleMapRange(lpObj->MapNumber) == true )
					{
						int TopHitUser = gObjMonsterTopHitDamageUser(lpObj);
						int level = lpObj->Class - 132;
						
						int type = ItemGetNumberMake(13,19);

						ItemSerialCreateSend(lpObj->m_Index,lpObj->MapNumber,lpObj->X,lpObj->Y,type,level,0,0,0,0,TopHitUser,0,0, Socket);
						
						g_BloodCastle.m_BridgeData[g_BloodCastle.GetBloodCastleIndex(lpObj->MapNumber)].m_btBC_QUEST_ITEM_NUMBER = level;

						g_BloodCastle.LogBloodCastleWeaponDrop(g_BloodCastle.GetBloodCastleIndex(lpObj->MapNumber),level);
					}
					
					if(lpObj->m_PosNum == -1)
					{
						gObjDel(lpObj->m_Index);
						continue;
					}
					
					if(lpObj->Type == OBJ_MONSTER)
					{
						if(lpObj->Class != 131 && BC_STATUE_RANGE(lpObj->Class - 132)== 0)
						{
							g_BloodCastle.ChangeMonsterState(g_BloodCastle.GetBloodCastleIndex(lpObj->MapNumber),lpObj->m_Index);
						}
					}
				}
				if( lpObj->MapNumber == 58 && lpObj->Type >= OBJ_MONSTER)
				{
					if ( lpObj->Class == 459 )
					{
						int TopHitUser = gObjMonsterTopHitDamageUser(lpObj);
						gRaklion.SelupanDrop(&gObj[TopHitUser],lpObj->X,lpObj->Y);
					}
				}
#if(GS_CASTLE)
				if(lpObj->m_Attribute == 60)
				{
					gObjDel(lpObj->m_Index);
					continue;
				}
				
				if(lpObj->m_Attribute == 61)
				{
					gObjDel(lpObj->m_Index);
					continue;
				}
				
				if(lpObj->m_Attribute == 62)
				{
					int m_iClass = lpObj->Class;
					
					if(m_iClass >= 300)
					{
						if(m_iClass > 303)
							continue;
						
						gObjDel(lpObj->m_Index);
						continue;
					}
				}
				
				if((lpObj->m_dwLastCheckTick + 600000) == GetTickCount())
				{
					gObjDel(lpObj->m_Index);
					continue;
				}
				
				
				if(lpObj->Type >= OBJ_MONSTER)
				{
					if(lpObj->Class == 287 || lpObj->Class == 286)
					{
						g_CsNPC_Mercenary.DeleteMercenary(lpObj->m_Index);
						gObjDel(lpObj->m_Index);
						continue;
					}
				}
				
				if(lpObj->Type >= OBJ_MONSTER)
				{
					if(lpObj->Class == 278)
					{
						g_CsNPC_LifeStone.DeleteLifeStone(lpObj->m_Index);
						gObjDel(lpObj->m_Index);
						continue;
					}					
				}
#endif
				
				int State = 1;
				
				if(lpObj->Type >= OBJ_MONSTER)
				{
					if(lpObj->Class == 44 )
					{
						State = 0;
					}
					
					if( lpObj->Class == 53 || lpObj->Class == 54 || lpObj->Class == 55 || lpObj->Class == 56 )
					{
						State = 0;
						lpObj->Life = lpObj->MaxLife;
						lpObj->DieRegen = 0;
					}
					
					if(lpObj->Class >= 78 && lpObj->Class <= 83)
					{
						State = 0;
						lpObj->Life = lpObj->MaxLife;
						lpObj->DieRegen = 0;
					}
					
					if(lpObj->Class == 77)
					{
						int i_shield = lpObj->m_SkyBossMonSheildLinkIndex;
						
						if(i_shield != -1)
						{	
							gObj[i_shield].Life = gObj[i_shield].MaxLife;
						}	
					}
				}
				
				PMSG_CHARREGEN pMsg;
				
				if(State != 0)
				{
					lpObj->Life = lpObj->MaxLife + lpObj->AddLife;
					lpObj->Mana = lpObj->MaxMana + lpObj->AddMana;
					lpObj->iShield = lpObj->iMaxShield + lpObj->iAddShield;
					
					lpObj->Live = 1;
					lpObj->m_PoisonBeattackCount = 0;
					lpObj->m_ColdBeattackCount = 0;
					lpObj->m_ViewState = 0;
					lpObj->Teleport = 0;
					
					for(int i = 0; i < MAX_SELF_DEFENSE; i++)
					{
						lpObj->SelfDefenseTime[i] = 0;
					}
					
					gObjTimeCheckSelfDefense(lpObj);
					
					lpObj->m_ViewSkillState[eVS_POISON] = 0;
					lpObj->m_ViewSkillState[eVS_ICE] = 0;
					
					gObjClearViewport(lpObj);
					
					if(lpObj->Type == OBJ_USER)
					{
						if(g_iUseCharacterAutoRecuperationSystem != 0 && lpObj->Level <= g_iCharacterRecuperationMaxLevel)
						{
							lpObj->m_iAutoRecuperationTime = GetTickCount();
						}
						else
						{
							lpObj->m_iAutoRecuperationTime = 0;
						}
						
						lpObj->dwShieldAutoRefillTimer = GetTickCount();
						
						if(lpObj->m_iSkillNPCHelpTime > 0)
						{
							lpObj->m_iSkillNPCHelpTime = 0;
							lpObj->m_iSkillNPCDefense = 0;
							lpObj->m_iSkillNPCAttack = 0;

							lpObj->m_ViewSkillState[eVS_NPC_HELP] = 0;
							GCStateInfoSend(lpObj,0,eVS_NPC_HELP);
						}
						
						gDarkSpirit[lpObj->m_Index].SetMode(CDarkSpirit::PetItem_Mode_Normal,-1);

						if(lpObj->KillerType == 3)
						{
							MapC[lpObj->MapNumber].GetRandomLengthPos((short &)lpObj->X,(short &)lpObj->Y,18);
						}
						else if(lpObj->KillerType == 2)
						{
							int w_Type = 0;
							
							if(lpObj->lpGuild != 0)
							{
								if(lpObj->lpGuild->WarType == 1)
								{
									w_Type = 1;
								}
							}
							
							if(w_Type == 0)
							{
								MapC[lpObj->MapNumber].GetRandomLengthPos((short &)lpObj->X,(short &)lpObj->Y,18);
							}
							else if(lpObj->lpGuild->WarState)
							{
								int BattleGroundIndex = lpObj->lpGuild->BattleGroundIndex;
								gBattleGetTeamPosition(BattleGroundIndex,lpObj->lpGuild->BattleTeamCode,(short &)lpObj->X,(short &)lpObj->Y);
							}
						}
						else if(lpObj->MapNumber == MAP_INDEX_LORENCIA || lpObj->MapNumber == MAP_INDEX_DUNGEON)
						{
							lpObj->MapNumber = MAP_INDEX_LORENCIA;
							MapC[lpObj->MapNumber].GetMapPos(lpObj->MapNumber,(short &)lpObj->X,(short &)lpObj->Y);
						}
						else if(DS_MAP_RANGE(lpObj->MapNumber))
						{
							g_DevilSquare.DieProcDevilSquare(lpObj);
							lpObj->MapNumber = MAP_INDEX_NORIA;
							MapC[lpObj->MapNumber].GetMapPos(lpObj->MapNumber,(short &)lpObj->X,(short &)lpObj->Y);
						}
						else if(lpObj->MapNumber == MAP_INDEX_ICARUS)
						{
							lpObj->MapNumber = MAP_INDEX_DEVIAS;
							MapC[lpObj->MapNumber].GetMapPos(lpObj->MapNumber,(short &)lpObj->X,(short &)lpObj->Y);
						}
						else if(gUser.BloodCastleMapRange(lpObj->MapNumber) == true)
						{
							lpObj->MapNumber = MAP_INDEX_DEVIAS;
							MapC[lpObj->MapNumber].GetMapPos(lpObj->MapNumber,(short &)lpObj->X,(short &)lpObj->Y);
						}
						else if( gUser.ChaosCastleMapRange(lpObj->MapNumber) == true )
						{
							lpObj->MapNumber = MAP_INDEX_DEVIAS;
							MapC[lpObj->MapNumber].GetMapPos(lpObj->MapNumber,(short &)lpObj->X,(short &)lpObj->Y);
						}
						else if(KALIMA_MAP_RANGE(lpObj->MapNumber))
						{
							lpObj->MapNumber = MAP_INDEX_DEVIAS;
							MapC[lpObj->MapNumber].GetMapPos(lpObj->MapNumber,(short &)lpObj->X,(short &)lpObj->Y);
						}
#if(GS_CASTLE)
						else if(lpObj->MapNumber == MAP_INDEX_CASTLEHUNTZONE)
						{
							int mgt = 106;
							short level;
							unsigned char x;
							unsigned char y;
							BYTE map = lpObj->MapNumber,dir;
							
							int result = GateC.GetGate(mgt,(unsigned char &)x,(unsigned char &)y,(BYTE &)map,(BYTE &)dir,(short &)level);
							
							if(result >= 0)
							{
								lpObj->MapNumber = map;
								lpObj->X = x;
								lpObj->Y = y;
							}
						}
						else if(lpObj->MapNumber == MAP_INDEX_CASTLESIEGE)
						{
							int mgt;
							short level;
							unsigned char x;
							unsigned char y;
							BYTE map = lpObj->MapNumber,dir;
							
							if(!g_CsNPC_LifeStone.SetReSpawnUserXY(lpObj->m_Index))
							{
								mgt = 106;
								if(lpObj->m_btCsJoinSide == 1)
								{
									mgt = 110;
								}
								else
								{
									mgt = 106;
								}
							}
							
							int result = GateC.GetGate(mgt,(unsigned char &)x,(unsigned char &)y,(BYTE &)map,(BYTE &)dir,(short &)level);
							
							if(result >= 0)
							{
								lpObj->MapNumber = map;
								lpObj->X = x;
								lpObj->Y = y;
							}
						}
#endif
						else if(lpObj->MapNumber == MAP_INDEX_AIDA)
						{
							MapC[lpObj->MapNumber].GetMapPos(lpObj->MapNumber,(short &)lpObj->X,(short &)lpObj->Y);
						}
						else if(lpObj->MapNumber == MAP_INDEX_CRYWOLF_FIRSTZONE)
						{
#if(!GS_CASTLE)
							MapC[lpObj->MapNumber].GetMapPos(lpObj->MapNumber,(short &)lpObj->X,(short &)lpObj->Y);
#else
							if(g_Crywolf.GetCrywolfState() == 4)
							{
								int mgt = 118;
								short level;
								unsigned char x;
								unsigned char y;
								BYTE map = lpObj->MapNumber,dir;
								
								int result = GateC.GetGate(mgt,(unsigned char &)x,(unsigned char &)y,(BYTE &)map,(BYTE &)dir,(short &)level);
								
								if(result >= 0)
								{
									lpObj->MapNumber = map;
									lpObj->X = x;
									lpObj->Y = y;
								}
								else
								{
									MapC[lpObj->MapNumber].GetMapPos(lpObj->MapNumber,(unsigned char &)lpObj->X,(unsigned char &)lpObj->Y);

								}
							}
#endif

						}
#if(!GS_CASTLE)
						else if(lpObj->MapNumber == MAP_INDEX_KANTURU_BOSS)
						{
							int mgt = 137;
							short level;
							unsigned char x;
							unsigned char y;
							BYTE map = lpObj->MapNumber,dir;
							int result = GateC.GetGate(mgt,(BYTE &)x,(BYTE &)y,(BYTE &)map,(BYTE &)dir,(short &)level);
							
							if(result >= 0)
							{
								lpObj->MapNumber = map;
								lpObj->X = x;
								lpObj->Y = y;
							}
							
							g_KanturuBattleUserMng.DeleteUserData(lpObj->m_Index);
						}
						else if(lpObj->MapNumber == MAP_INDEX_KANTURU1)
						{
							int mgt = 138;
							short level;
							unsigned char x;
							unsigned char y;
							BYTE map = lpObj->MapNumber,dir;
							
							int result = GateC.GetGate(mgt,(BYTE &)x,(BYTE &)y,(BYTE &)map,(BYTE &)dir,(short &)level);
							
							if(result >= 0)
							{
								lpObj->MapNumber = map;
								lpObj->X = x;
								lpObj->Y = y;
							}
						}
						else if(lpObj->MapNumber == MAP_INDEX_KANTURU2)
						{
							int mgt = 139;
							short level;
							unsigned char x;
							unsigned char y;
							BYTE map = lpObj->MapNumber,dir;
							
							int result = GateC.GetGate(mgt,(BYTE &)x,(BYTE &)y,(BYTE &)map,(BYTE &)dir,(short &)level);
							
							if(result >= 0)
							{
								lpObj->MapNumber = map;
								lpObj->X = x;
								lpObj->Y = y;
							}
						}
#endif
						else if(lpObj->MapNumber == MAP_INDEX_ELBELAND)
						{
							int mgt = 267;
							short level;
							unsigned char x;
							unsigned char y;
							BYTE map = lpObj->MapNumber,dir;

							int result = GateC.GetGate(mgt,(BYTE &)x,(BYTE &)y,(BYTE &)map,(BYTE &)dir,(short &)level);

							if(result >= 0)
							{
								lpObj->MapNumber = map;
								lpObj->X = x;
								lpObj->Y = y;
							}
						}
						else if(lpObj->MapNumber == MAP_INDEX_SWAMP_OF_CALMNESS)
						{
							int mgt = 273;
							short level;
							unsigned char x;
							unsigned char y;
							BYTE map = lpObj->MapNumber,dir;

							int result = GateC.GetGate(mgt,(BYTE &)x,(BYTE &)y,(BYTE &)map,(BYTE &)dir,(short &)level);

							if(result >= 0)
							{
								lpObj->MapNumber = map;
								lpObj->X = x;
								lpObj->Y = y;
							}
						}
						else if(lpObj->MapNumber == MAP_INDEX_RAKLION)
						{
							int mgt = 287;
							short level;
							unsigned char x;
							unsigned char y;
							BYTE map = lpObj->MapNumber,dir;

							int result = GateC.GetGate(mgt,(BYTE &)x,(BYTE &)y,(BYTE &)map,(BYTE &)dir,(short &)level);

							if(result >= 0)
							{
								lpObj->MapNumber = map;
								lpObj->X = x;
								lpObj->Y = y;
							}
						}
						else if(lpObj->MapNumber == MAP_INDEX_RAKLION_BOSS)
						{
							int mgt = 287;
							short level;
							unsigned char x;
							unsigned char y;
							BYTE map = lpObj->MapNumber,dir;

							int result = GateC.GetGate(mgt,(BYTE &)x,(BYTE &)y,(BYTE &)map,(BYTE &)dir,(short &)level);

							if(result >= 0)
							{
								lpObj->MapNumber = map;
								lpObj->X = x;
								lpObj->Y = y;
							}
							/*int mgt = 291;
							short x,y,level;
							BYTE map = lpObj->MapNumber,dir;

							int result = GateC.GetGate(mgt,(BYTE &)x,(BYTE &)y,(BYTE &)map,(BYTE &)dir,(short &)level);

							if(result >= 0)
							{
								lpObj->MapNumber = map;
								lpObj->X = x;
								lpObj->Y = y;
							}*/
						}
						else if( DG_MAP_RANGE(lpObj->MapNumber) == true )
						{
							int mgt = 267;
							short level;
							unsigned char x;
							unsigned char y;
							BYTE map = lpObj->MapNumber,dir;

							int result = GateC.GetGate(mgt,(BYTE &)x,(BYTE &)y,(BYTE &)map,(BYTE &)dir,(short &)level);

							if(result >= 0)
							{
								lpObj->MapNumber = map;
								lpObj->X = x;
								lpObj->Y = y;
							}
						}

						else if ( IF_MAP_RANGE(lpObj->MapNumber) == true )
						{
							int mgt = 22;
							short level;
							unsigned char x;
							unsigned char y;
							BYTE map = lpObj->MapNumber,dir;

							int result = GateC.GetGate(mgt,(BYTE &)x,(BYTE &)y,(BYTE &)map,(BYTE &)dir,(short &)level);

							if(result >= 0)
							{
								lpObj->MapNumber = map;
								lpObj->X = x;
								lpObj->Y = y;
							}
						}
#if(!GS_CASTLE)
						else if( IT_MAP_RANGE(lpObj->MapNumber) == TRUE ) // IllusionTemple Regen
						{
							g_IllusionTemple.ProcessDieUser(lpObj);
						}
						else if ( lpObj->MapNumber == MAP_INDEX_VULCANROOM ) // Duel Room
						{
							if ( gDuel.IsOnDuel(lpObj) == true )
							{
								gDuel.MoveDeadUser(lpObj);
							}
						}
#endif
						else
						{
							MapC[lpObj->MapNumber].GetMapPos(lpObj->MapNumber,(short &)lpObj->X,(short &)lpObj->Y);
						}
						
						short m_Result = g_MapServerManager.CheckMoveMapSvr(lpObj->m_Index,lpObj->MapNumber,lpObj->m_sPrevMapSvrCode);
						
						if(m_Result != gGameServerCode )
						{
							if(m_Result == -1)
							{
								CLog.LogAddC(2,"[MapServerMng] Map Server Move Fail : CheckMoveMapSvr() == -1 [%s][%s] (%d)",lpObj->AccountID,lpObj->Name,lpObj->m_Index);
								continue;
							}
							
							int _map = lpObj->MapNumber;
							int _x = lpObj->X;
							int _y = lpObj->Y;
							
							lpObj->MapNumber = mapnum;
							lpObj->X = x;
							lpObj->Y = y;
							
							GJReqMapSvrMove(lpObj->m_Index,m_Result,_map,_x,_y);
							CLog.LogAdd("[MapServerMng] Request to Move Map Server : (%d) - [%s][%s] (%d)",m_Result,lpObj->AccountID,lpObj->Name,lpObj->m_Index);
							continue;
						}
						
						lpObj->TX = lpObj->X;
						lpObj->TY = lpObj->Y;
						
						CreateFrustrum(lpObj->X,lpObj->Y,n);
						
						pMsg.h.c = 0xC3;
						pMsg.h.headcode = 0xF3;
						pMsg.h.size = 0x1C;
						pMsg.subcode = 4;
						pMsg.Dir = lpObj->Dir;
						pMsg.MapX = lpObj->X;
						pMsg.MapY = lpObj->Y;
						pMsg.Life = lpObj->Life;
						pMsg.Mana = lpObj->Mana;
						pMsg.MapNumber = lpObj->MapNumber;
						pMsg.wShield = lpObj->iShield;
						pMsg.BP = lpObj->BP;
						pMsg.Unk = 0;
						pMsg.Money = lpObj->Money;
						pMsg.Exp1 = HIBYTE(HIWORD(lpObj->Experience));
						pMsg.Exp2 = LOBYTE(HIWORD(lpObj->Experience));
						pMsg.Exp3 = HIBYTE(LOWORD(lpObj->Experience));
						pMsg.Exp4 = LOBYTE(LOWORD(lpObj->Experience));
						
						lpObj->PathCount = 0;
						
						gSendProto.DataSend(n,(unsigned char *)&pMsg,pMsg.h.size);

						gSkillTree.SendMasterData(lpObj->m_Index);

						gObjViewportListProtocolCreate(lpObj);

						Gens.GenCheck(lpObj);
						
#if(GS_CASTLE)
						if(lpObj->MapNumber == MAP_INDEX_CASTLESIEGE)
						{	
							::GCAnsCsNotifyStart(lpObj->m_Index,CASTLESIEGEREADY(g_CastleSiege.GetCastleState()));
							
							if(g_CastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE)
							{
								g_CastleSiege.NotifySelfCsJoinSide(lpObj->m_Index);
								g_CastleSiege.NotifyCsSelfLeftTime(lpObj->m_Index);
							}
							
						}
#endif
					}
					else
					{
						if(gObjMonsterRegen(lpObj)==0)
						{
							continue;
						}
						CreateFrustrum(lpObj->X,lpObj->Y,n);
					}
					
					lpObj->DieRegen = 0;
					lpObj->m_State = 1;
				}
			}
		}
	}
	
	for(n = 0; n < MAX_NUMBER_MAP; n++)
	{
		MapC[n].StateSetDestroy();
	}
}

void gObjSecondProc()
{
	if(gDisconnect == 1)
		return;
	
	int n;
	LPOBJ lpObj;
	
	for(n = 0; n < OBJMAX;n++)
	{
		lpObj = &gObj[n];

		if(lpObj->Connected > PLAYER_LOGGED)
		{
			if(lpObj->m_bMapSvrMoveQuit == 1)
			{
				if(GetTickCount() - lpObj->m_dwMapSvrQuitTick > 30000)
				{
					gObjDel(lpObj->m_Index);
					continue;
				}
			}
		}
		if ( lpObj->Type == OBJ_USER )
		{
			gUser.BuffTimer(lpObj);
		}
		gObjSkillUseProc(lpObj);
		gObjSkillBeAttackProc(lpObj);
		
		
		if(lpObj->Type == OBJ_NPC && lpObj->Class == 229)
		{
			gQeustNpcTeleport.Run(n);
		}

		
		if(lpObj->Type == OBJ_MONSTER)
		{
			if(lpObj->Class == 275)
			{
				gObjRefillMonsterHP(lpObj,giKundunRefillHPSec);
			}

#if (GS_CASTLE)
			if(lpObj->Class == 283)
			{
				g_CsNPC_GuardianStatue.GuardianStatueAct(lpObj->m_Index);
				continue;
			}
			
			if(lpObj->Class == 278)
			{
				g_CsNPC_LifeStone.LifeStoneAct(lpObj->m_Index);
				continue;
			}
#endif
			
			if(lpObj->Class == 285)
			{
				g_CsNPC_Guardian.GuardianAct(lpObj->m_Index);
				continue;
			}
#if (GS_CASTLE)
			if(lpObj->Class == 288)
			{
				g_CsNPC_CannonTower.CannonTowerAct(lpObj->m_Index);
				continue;
			}
#endif
		}

#if(GS_CASTLE)
		g_Crywolf.Init();
#endif

		if(lpObj->Type == OBJ_NPC)
		{
#if (GS_CASTLE)
			if(lpObj->Class >= 204 && lpObj->Class <= 209)
			{
				g_Crywolf.CrywolfNpcAct(lpObj->m_Index);
			}
			
			if(lpObj->Class == 216)
			{
				g_CsNPC_CastleCrown.CastleCrownAct(lpObj->m_Index);
				continue;
			}
			
			if(lpObj->Class >= 217 && lpObj->Class <= 218)
			{
				g_CsNPC_CastleCrownSwitch.CastleCrownSwitchAct(lpObj->m_Index);
				continue;
			}
#endif
			if(lpObj->Class == 221 || lpObj->Class == 222)
			{
				g_CsNPC_Weapon.WeaponAct(lpObj->m_Index);
			}
		}

		

		if(lpObj->Type == OBJ_USER)
		{
			gObjManaPotionFill(lpObj);
			gObjRestPotionFill(lpObj);
			
			if(lpObj->Connected >= PLAYER_PLAYING)
			{
				gObjCheckXYMapTile( lpObj,3 );
			}
			
			if(lpObj->ChatLimitTime > 0)
			{
				lpObj->ChatLimitTimeSec++;
				
				if(lpObj->ChatLimitTimeSec > 60)
				{
					lpObj->ChatLimitTimeSec = 0;
					lpObj->ChatLimitTime--;
					
					if(lpObj->ChatLimitTime < 1)
					{
						lpObj->ChatLimitTime = 0;
						MsgOutput(lpObj->m_Index,lMsg.Get(1141));
					}
				}
			}
			
			if ( lpObj->DelayTimeMSG > 0 )
			{
				lpObj->DelayTimeMSG--;
			}

			if(g_iUseCharacterAutoRecuperationSystem != 0)
			{
				if(lpObj->Level <= g_iCharacterRecuperationMaxLevel)
				{
					gProcessAutoRecuperation(lpObj);
				}
			}
				
			if(lpObj->Type == OBJ_USER)
			{
				gObjShieldAutoRefill(lpObj);
			}
				
			if(lpObj->Type == OBJ_USER && lpObj->m_LastTeleportTime > 0)
			{
				lpObj->m_LastTeleportTime--;
			}
				
			gObjDelayLifeCheck(n);
			gObjSecondDurDown(lpObj);
				
			if(lpObj->PartyNumber >= 0)
			{
				gParty.PartyMemberLifeSend(lpObj->PartyNumber);
			}
				
			if(lpObj->m_ShopTime >= 1 && lpObj->m_ShopTime < 255)
			{
				lpObj->m_ShopTime++;
			}
				
			int BattleGround = 1;
			int m_BattleTimer = gCheckBattleGroundTimer();
				
			if((lpObj->Authority & 2) == 2)
			{
				BattleGround = 0;
				GCTimeViewSend(lpObj->m_Index,m_BattleTimer);
			}
				
			if(lpObj->MapNumber == MAP_INDEX_BATTLESOCCER && BattleGround == 1)
			{
				BattleGround = 1;
					
				if(lpObj->lpGuild != 0)
				{
					if(lpObj->lpGuild->WarState != 0)
					{
						int CheckBattleGround = gCheckBattleGround(lpObj);
							
						if(CheckBattleGround != lpObj->lpGuild->BattleGroundIndex)
						{
							if(lpObj->lpGuild->WarType == 1)
							{
								BattleGround = 0;
							}
							else
							{
								BattleGround = 0;
							}
						}
						else
						{
							BattleGround = 0;
						}
					}
					else
					{
						BattleGround = 1;
					}
					
					if(BattleGround != 0)
					{
						int CheckBattleGround = gCheckBattleGround(lpObj);
						
						if(CheckBattleGround >= 0)
						{
							gObjMoveGate(lpObj->m_Index,17);
						}	
					}
				}
			}
		}
		
		if(lpObj->Connected >= PLAYER_LOGGED && 
			lpObj->Type == OBJ_USER &&
			lpObj->CloseCount > 0)
		{
			if(lpObj->CloseCount == 1)
			{
				if(lpObj->CloseType == 1)
				{
					if(gObjGameClose(lpObj->m_Index) == 1)
					{
						gSendProto.CloseMsgSend(lpObj->m_Index, 1);
					}
				}
				else if(lpObj->CloseType == 0)
				{
					gSendProto.CloseMsgSend(lpObj->m_Index, 0);
					gNet.CloseClient(lpObj->m_Index);
				}
				else if(lpObj->CloseType == 2)
				{
					gSendProto.CloseMsgSend(lpObj->m_Index, 2);
				}
			}
			else
			{
				char Msg[128];
				wsprintf(Msg,lMsg.Get(1116),lpObj->CloseCount-1);
				GCServerMsgStringSend(Msg,lpObj->m_Index,1);
			}
			lpObj->CloseCount--;
		}

		if(lpObj->Connected > PLAYER_LOGGED &&
			lpObj->Type == OBJ_USER)
		{
			if(GetTickCount() - lpObj->AutoSaveTime > 600000)
			{
				GJSetCharacterInfo(lpObj,n,0);
				lpObj->AutoSaveTime = GetTickCount();
			}
			
			if(lpObj->CheckSumTime > 0 && GetTickCount() - lpObj->CheckSumTime > 3000)
			{
				gNet.CloseClient(n);
			}
			
			gPKSystem.PKDownTime(lpObj,1);
			gObjInterfaceTimeCheck(lpObj);
			gObjTimeCheckSelfDefense(lpObj);
		}

		if(lpObj->Connected == PLAYER_CONNECTED ||
			lpObj->Connected == PLAYER_LOGGED ||
			lpObj->Connected == PLAYER_PLAYING)
		{
			if(lpObj->Type == OBJ_USER)
			{
				if(lpObj->Connected >= PLAYER_LOGGED)
				{
					if(GetTickCount() - lpObj->ConnectCheckTime > 60000)
					{
						gNet.ResponErrorCloseClient(n);
						CLog.LogAdd(lMsg.Get(542),lpObj->m_Index,lpObj->AccountID,lpObj->Name,lpObj->Ip_addr);
					}
				}
				else
				{
					if(GetTickCount() - lpObj->ConnectCheckTime > 30000)
					{
						gNet.ResponErrorCloseClient(n);
						CLog.LogAdd(lMsg.Get(543),lpObj->m_Index,lpObj->AccountID,lpObj->Name,lpObj->Ip_addr);
					}
				}
			}
		}
	}
}

void gObjManaPotionFill(LPOBJ lpObj)
{
	lpObj->m_ManaFillCount++;

	float tmana;
	int ChangeMana = 0;

	if(lpObj->m_ManaFillCount > 2)
	{
		lpObj->m_ManaFillCount = 0;

		int BP;
		int AddBP;
		BYTE MapAttr;
		int rBP;
		int CurBP;

		if(lpObj->MaxMana + lpObj->AddMana > lpObj->Mana)
		{
			tmana = lpObj->Mana;
			tmana += (lpObj->MaxMana + lpObj->AddMana)*0.037f;

			tmana += lpObj->m_JewelOfHarmonyEffect.HJOpAddRefillMP;

			if(lpObj->MaxMana + lpObj->AddMana < tmana)
			{
				tmana = lpObj->MaxMana + lpObj->AddMana;
			}

			lpObj->Mana = tmana;
			ChangeMana = 1;
		}

		BP = lpObj->MaxBP + lpObj->AddBP;

		if(lpObj->BP < BP)
		{
			MapAttr = MapC[lpObj->MapNumber].GetAttr((short &)lpObj->X,(short &)lpObj->Y);
			rBP = 2;

			if((MapAttr & 1) != 0)
			{
				rBP = 5;
			}

			if(lpObj->Class == CLASS_KNIGHT)
			{
				AddBP = rBP +(BP * 5 / 100);
			}
			else
			{
				AddBP = rBP +(BP * 3 / 100);
			}

			if(lpObj->m_iPotionSoulTime > 0)
			{
				AddBP += 8;
			}

			AddBP += lpObj->SetOpIncAGValue;

			if(AddBP < 1)
			{
				AddBP = 1;
			}

			CurBP = lpObj->BP;
			CurBP += AddBP;

			if(CurBP > BP)
			{
				CurBP = BP;
			}

			lpObj->BP = CurBP;

			ChangeMana = 1;
		}

		if(ChangeMana != 0)
		{
			GCManaSend(lpObj->m_Index,lpObj->Mana,255,0,lpObj->BP);
		}
	}
}

void gObjRestPotionFill(LPOBJ lpObj)
{
	if(lpObj->m_Rest != 0)
	{
		float tlife;
		float tmana;
		int percent = 3;
		lpObj->m_LifeFillCount++;

		if(lpObj->m_LifeFillCount > 4)
		{
			lpObj->m_LifeFillCount = 0;

			tlife = lpObj->Life;

			if(lpObj->pInventory[10].IsItem() == 1)
			{
				if(lpObj->pInventory[10].m_Option3 != 0 && lpObj->pInventory[10].m_Type != ITEMGET(13,24))
				{
					percent += lpObj->pInventory[10].m_Option3;
				}
			}
			else if(lpObj->pInventory[11].IsItem() == 1)
			{
				if(lpObj->pInventory[11].m_Option3 != 0 && lpObj->pInventory[11].m_Type != ITEMGET(13,24))
				{
					percent += lpObj->pInventory[11].m_Option3;
				}
			}
			else if(lpObj->pInventory[9].IsItem() == 1)
			{
				if(lpObj->pInventory[9].m_Option3 != 0 && lpObj->pInventory[9].m_Type != ITEMGET(13,28))
				{
					percent += lpObj->pInventory[9].m_Option3;
				}
			}

			tlife += (lpObj->MaxLife + lpObj->AddLife)*percent / 100.0f;
			tlife += lpObj->m_JewelOfHarmonyEffect.HJOpAddRefillHP;
			
			if((lpObj->MaxLife + lpObj->AddLife) < tlife)
			{
				tlife = lpObj->MaxLife + lpObj->AddLife;
			}

		lpObj->Life = tlife;

		GCReFillSend(lpObj->m_Index,lpObj->Life,255,0,lpObj->iShield);

		tmana = lpObj->Mana + lpObj->AddMana;
		tmana += (lpObj->MaxMana + lpObj->AddMana) * 3.0f / 100.0f;
		tmana += lpObj->m_JewelOfHarmonyEffect.HJOpAddRefillMP;

		if((lpObj->MaxMana + lpObj->AddMana) < tmana)
		{
			tmana = lpObj->MaxMana + lpObj->AddMana;
		}

		lpObj->Mana = tmana;

		GCManaSend(lpObj->m_Index,lpObj->Mana,255,0,lpObj->BP);
		}
	}
	else
	{
		int percent = 0;
		float tlife;
		if(lpObj->pInventory[10].IsItem() == 1)
		{
			if(lpObj->pInventory[10].m_Option3 != 0 && lpObj->pInventory[10].m_Durability != 0.0f && lpObj->pInventory[10].m_Type != ITEMGET(13,24))
			{
				percent += lpObj->pInventory[10].m_Option3;
			}
		}

		if(lpObj->pInventory[11].IsItem() == 1)
		{
			if(lpObj->pInventory[11].m_Option3 != 0 && lpObj->pInventory[11].m_Durability != 0.0f && lpObj->pInventory[11].m_Type != ITEMGET(13,24))
			{
				percent += lpObj->pInventory[11].m_Option3;
			}
		}

		if(lpObj->pInventory[9].IsItem() == 1)
		{
			if(lpObj->pInventory[9].m_Option3 != 0 && lpObj->pInventory[9].m_Durability != 0.0f && lpObj->pInventory[9].m_Type != ITEMGET(13,28))
			{
				percent += lpObj->pInventory[9].m_Option3;
			}
		}

		if(lpObj->pInventory[7].IsItem() == 1)
		{
			if(lpObj->pInventory[7].m_Option3 != 0 && lpObj->pInventory[7].m_Special[2] == 85 && lpObj->pInventory[7].m_Durability != 0.0f)
			{
				percent += lpObj->pInventory[7].m_Option3;
			}
		}

		if(percent != 0)
		{
			lpObj->m_LifeFillCount++;

			if(lpObj->m_LifeFillCount > 6)
			{
				lpObj->m_LifeFillCount = 0;
				tlife = lpObj->Life;

				tlife += (lpObj->MaxLife +lpObj->AddLife) * percent / 100.f;
				tlife += lpObj->m_JewelOfHarmonyEffect.HJOpAddRefillHP;

				if((lpObj->MaxLife + lpObj->AddLife) < tlife)
				{
					tlife = lpObj->MaxLife + lpObj->AddLife;
				}

				lpObj->Life = tlife;

				GCReFillSend(lpObj->m_Index,lpObj->Life,255,0,lpObj->iShield);
			}
		}
	}

	if(lpObj->iFillShield > 0)
	{
		int iFillSDPoint = 0;
		if(lpObj->iFillShieldCount <= 0)
		{
			lpObj->iFillShieldCount = 1;
		}

		iFillSDPoint = lpObj->iFillShieldMax / lpObj->iFillShieldCount;

		lpObj->iShield += iFillSDPoint;

		lpObj->iFillShield -= iFillSDPoint;

		if(lpObj->iFillShield < iFillSDPoint)
		{
			iFillSDPoint = lpObj->iFillShield;
			lpObj->iFillShield = 0;

			if(iFillSDPoint < 0)
			{
				iFillSDPoint = 0;
			}
		}

		if(lpObj->iShield > (lpObj->iMaxShield + lpObj->iAddShield))
		{
			lpObj->iShield = lpObj->iMaxShield + lpObj->iAddShield;

			lpObj->iFillShield = 0;
			lpObj->iFillShieldMax = 0;
		}

		GCReFillSend(lpObj->m_Index,lpObj->Life,255,0,lpObj->iShield);
	}

	if(lpObj->FillLife > 0)
	{
		float cur = lpObj->FillLifeMax * 34.0f / 100.0f;

		if(lpObj->FillLifeCount == 2)
		{
			cur = lpObj->FillLifeMax * 54.0f / 100.0f;
		}

		if(lpObj->FillLifeCount == 4)
		{
			cur = lpObj->FillLifeMax * 25.0f / 100.0f;
		}

		if(lpObj->FillLife < cur)
		{
			cur = lpObj->FillLife;
			lpObj->FillLife = 0;

			if(cur < 0)
			{
				cur = 0;
			}
		}
		else
		{
			lpObj->FillLife -= cur;
		}

		lpObj->Life += cur;

		if((lpObj->MaxLife + lpObj->AddLife) < lpObj->Life)
		{
			lpObj->Life = lpObj->MaxLife + lpObj->AddLife;
			lpObj->FillLife = 0;
		}

		GCReFillSend(lpObj->m_Index,lpObj->Life,255,0,lpObj->iShield);
	}
}

void gObjUseDrink(LPOBJ lpObj, int level)
{
	int time;

	if ( level == 2 )
	{
		time = 90;
		GCItemUseSpecialTimeSend(lpObj->m_Index, 1, time);
		lpObj->m_SkillAttack2 = 1;
		lpObj->m_SkillAttackTime2 = 90;
	}
	else if ( level == 1 )
	{
		time = 180;
		GCItemUseSpecialTimeSend(lpObj->m_Index, 0, time);
	}
	else if ( level == 0x4D )
	{
		time = 60;
		GCItemUseSpecialTimeSend(lpObj->m_Index, 2, time);
	}
	else
	{
		time = 80;
		GCItemUseSpecialTimeSend(lpObj->m_Index, 0, time);
	}
}

void gObjViewportListProtocolDestroy(LPOBJ lpObj)
{
	BYTE sendBuf[5000];
	int lOfs,count;
	count = 0;
	lOfs = 4;

	PBMSG_COUNT pCount;

	pViewportDestroy.NumberH = SET_NUMBERH(lpObj->m_Index);
	pViewportDestroy.NumberL = SET_NUMBERL(lpObj->m_Index);


	memcpy(&sendBuf[lOfs],&pViewportDestroy,2);

	lOfs += 2;
	count += 1;

	pCount.h.c = 0xC1;
	pCount.h.headcode = 20;
	pCount.h.size = lOfs;
	pCount.count = count;

	memcpy(sendBuf,&pCount,sizeof(pCount));

	if(count < 1)	{
		return;
	}

	gSendProto.VPMsgSend(lpObj,sendBuf,lOfs);
}

void gObjViewportListProtocolCreate(LPOBJ lpObj)
{
	int n,tObjNum;
	BYTE sendBuf[5000];
	int lOfs,count,moncount;
	PWMSG_COUNT pwCount;

	WORD btViewSkillState[MAX_VIEWSKILL] = {0};

	count = 0;
	moncount = 0;
	lOfs = sizeof(pwCount);

	int MVL = MAX_VIEWPORT;

	if(lpObj->Type == OBJ_MONSTER)
	{
		MVL = MAX_VIEWPORT_MONSTER;
	}


	if(lpObj->Type == OBJ_USER)
	{
		if(lpObj->m_Change >= 0)
		{
			pViewportCreateChange.NumberH = SET_NUMBERH(lpObj->m_Index);
			pViewportCreateChange.NumberL = SET_NUMBERL(lpObj->m_Index);

			pViewportCreateChange.NumberH |= 0x80;

			pViewportCreateChange.X = lpObj->X;
			pViewportCreateChange.Y = lpObj->Y;

			pViewportCreateChange.TX = lpObj->TX;
			pViewportCreateChange.TY = lpObj->TY;

			pViewportCreateChange.SkinH = SET_NUMBERH((lpObj->m_Change & 0xFFFF) & 0xFFFF);
			pViewportCreateChange.SkinL = SET_NUMBERL((lpObj->m_Change & 0xFFFF) & 0xFFFF);

			memcpy(&pViewportCreateChange.CharSet[0], &lpObj->CharSet[0], 18); // fixed xDD =P

			pViewportCreateChange.btViewSkillCount = gObjViewSkillBufferConvert(lpObj, btViewSkillState);

			pViewportCreateChange.DirAndPkLevel = lpObj->Dir << 4;
			pViewportCreateChange.DirAndPkLevel |= lpObj->m_PK_Level & 0x0F;

			if( gUser.ChaosCastleMapRange(lpObj->MapNumber) == true )
			{
				pViewportCreateChange.btViewSkillCount = 0;
			}

			memcpy(pViewportCreateChange.Id,lpObj->Name,sizeof(pViewportCreateChange.Id));
			memcpy(&sendBuf[lOfs],&pViewportCreateChange,sizeof(pViewportCreateChange));

			lOfs += sizeof(pViewportCreateChange);

			if ( pViewportCreateChange.btViewSkillCount )
			{
				memcpy(&sendBuf[lOfs], btViewSkillState, pViewportCreateChange.btViewSkillCount);
				lOfs += pViewportCreateChange.btViewSkillCount;
			}

			count += 1;

			if(count < 1)
			{
				return;
			}

			pwCount.h.headcode = 0x45;
		}
		else
		{
			pViewportCreate.NumberH = SET_NUMBERH(lpObj->m_Index);
			pViewportCreate.NumberL = SET_NUMBERL(lpObj->m_Index);

			lpObj->CharSet[0] &= 0xF8;

			if(lpObj->m_State == 1 && lpObj->Teleport==0)
			{
				pViewportCreate.NumberH |= 0x80;
			}

			pViewportCreate.btViewSkillCount = gObjViewSkillBufferConvert(lpObj, btViewSkillState);

			lpObj->CharSet[0] |= lpObj->m_ViewState & 0x0F;

			pViewportCreate.X = lpObj->X;
			pViewportCreate.Y = lpObj->Y;
			pViewportCreate.TX = lpObj->TX;
			pViewportCreate.TY = lpObj->TY;

			pViewportCreate.DirAndPkLevel = lpObj->Dir << 4;
			pViewportCreate.DirAndPkLevel |= lpObj->m_PK_Level & 0x0F;

			if( gUser.ChaosCastleMapRange(lpObj->MapNumber) == true )
			{
				pViewportCreate.btViewSkillCount = 0;
			}

			memcpy(pViewportCreate.CharSet,lpObj->CharSet,sizeof(pViewportCreate.CharSet));
			memcpy(pViewportCreate.Id,lpObj->Name,sizeof(pViewportCreate.Id));
			memcpy(&sendBuf[lOfs],&pViewportCreate,sizeof(pViewportCreate));

			lOfs += sizeof(pViewportCreate);

			if ( pViewportCreate.btViewSkillCount )
			{
				memcpy(&sendBuf[lOfs], btViewSkillState, pViewportCreate.btViewSkillCount);
				lOfs += pViewportCreate.btViewSkillCount;
			}

			count += 1;

			if(count < 1)
			{
				return;
			}

			pwCount.h.headcode = 0x12;
		}

		pwCount.h.c = 0xC2;
		pwCount.h.sizeH = SET_NUMBERH(lOfs);
		pwCount.h.sizeL = SET_NUMBERL(lOfs);
		pwCount.count = count;

		memcpy(sendBuf,&pwCount,sizeof(pwCount));

		gSendProto.DataSend(lpObj->m_Index,sendBuf,lOfs);

		if(lpObj->GuildNumber != 0)
		{
			GuildInfoOfs = 0;
			GuildInfoOfs = 5;

			PMSG_SIMPLE_GUILDVIEWPORT pGuild;

			pGuild.GuildNumber = lpObj->lpGuild->Number;
			pGuild.NumberH = SET_NUMBERH(lpObj->m_Index) & 0x7F;
			pGuild.NumberL = SET_NUMBERL(lpObj->m_Index);

			if(strcmp(lpObj->lpGuild->Names[0],lpObj->Name) == 0)
			{
				pGuild.NumberH |= 0x80;
			}

			pGuild.btGuildType = lpObj->lpGuild->btGuildType;
			pGuild.btGuildStatus = lpObj->GuildStatus;

			memcpy(&GuildInfoBuf[GuildInfoOfs],&pGuild,sizeof(pGuild));

			GuildInfoOfs += sizeof(pGuild);

			PMSG_SIMPLE_GUILDVIEWPORT_COUNT pwCount;

			pwCount.h.c = 0xC2;
			pwCount.h.headcode = 0x65;
			pwCount.h.sizeH = SET_NUMBERH(GuildInfoOfs);
			pwCount.h.sizeL = SET_NUMBERL(GuildInfoOfs);
			pwCount.Count = 1;

			memcpy(GuildInfoBuf,&pwCount,sizeof(pwCount));

			gSendProto.DataSend(lpObj->m_Index,GuildInfoBuf,GuildInfoOfs);
		}

		for(n = 0; n < MVL; n++)
		{
			if(lpObj->VpPlayer2[n].state != 0)
			{
				tObjNum = lpObj->VpPlayer2[n].number;

				if(tObjNum >= 0)
				{
					if(lpObj->VpPlayer2[n].type == OBJ_USER)
					{
						gSendProto.DataSend(tObjNum,sendBuf,lOfs);

						if(GuildInfoCount != 0)
						{
							gSendProto.DataSend(tObjNum,GuildInfoBuf,GuildInfoOfs);
						}

						if(GuildUserCount != 0)
						{
							gSendProto.DataSend(tObjNum,GuildUserBuf,GuildUserOfs);
						}
					}
				}
			}
		}
	}
	else
	{
		count = 0;
	
		if(lpObj->m_RecallMon >= 0)
		{
			pCallMonsterViewportCreate.NumberH = SET_NUMBERH(lpObj->m_Index);
			pCallMonsterViewportCreate.NumberL = SET_NUMBERL(lpObj->m_Index);
	
			if(lpObj->m_State == 1)
			{
				pCallMonsterViewportCreate.NumberH |= 0x80;
			}
	
			pCallMonsterViewportCreate.Type_HI = SET_NUMBERH(lpObj->Class);
			pCallMonsterViewportCreate.Type_LO = SET_NUMBERL(lpObj->Class);

			pCallMonsterViewportCreate.btViewSkillCount = gObjViewSkillBufferConvert(lpObj, btViewSkillState);
	
			pCallMonsterViewportCreate.X = lpObj->X;
			pCallMonsterViewportCreate.Y = lpObj->Y;
	
			pCallMonsterViewportCreate.TX = lpObj->TX;
			pCallMonsterViewportCreate.TY = lpObj->TY;
	
			pCallMonsterViewportCreate.Path = lpObj->Dir << 4;
	
			if(lpObj->m_RecallMon >= 0 && lpObj->m_RecallMon < OBJMAX-1)
			{
				memcpy(pCallMonsterViewportCreate.Id,gObj[lpObj->m_RecallMon].Name,sizeof(pCallMonsterViewportCreate.Id));
			}
			else
			{
				memset(pCallMonsterViewportCreate.Id,0x00,sizeof(pCallMonsterViewportCreate.Id));
			}
	
			memcpy(&sendBuf[lOfs],&pCallMonsterViewportCreate,sizeof(pCallMonsterViewportCreate));
	
			lOfs += sizeof(pCallMonsterViewportCreate);

			if ( pCallMonsterViewportCreate.btViewSkillCount )
			{
				memcpy(&sendBuf[lOfs], btViewSkillState, pCallMonsterViewportCreate.btViewSkillCount);
				lOfs += pCallMonsterViewportCreate.btViewSkillCount;
			}

			count += 1;
		}
		else
		{
			pMonsterViewportCreate.NumberH = SET_NUMBERH(lpObj->m_Index);
			pMonsterViewportCreate.NumberL = SET_NUMBERL(lpObj->m_Index);
	
			if(lpObj->m_State == 1 && lpObj->Teleport == 0)
			{
				pMonsterViewportCreate.NumberH |= 0x80;
				pMonsterViewportCreate.NumberH |= 0x40;
			}
	
			pMonsterViewportCreate.Type_HI = SET_NUMBERH(lpObj->Class);
			pMonsterViewportCreate.Type_LO = SET_NUMBERL(lpObj->Class);

			pMonsterViewportCreate.btViewSkillCount = gObjViewSkillBufferConvert(lpObj, btViewSkillState);
			pMonsterViewportCreate.X = lpObj->X;
			pMonsterViewportCreate.Y = lpObj->Y;
			pMonsterViewportCreate.TX = lpObj->TX;
			pMonsterViewportCreate.TY = lpObj->TY;
	
			pMonsterViewportCreate.Path = lpObj->Dir << 4;
	
			memcpy(&sendBuf[lOfs],&pMonsterViewportCreate,sizeof(pMonsterViewportCreate));
			lOfs += sizeof(pMonsterViewportCreate);

			if ( pMonsterViewportCreate.btViewSkillCount )
			{
				memcpy(&sendBuf[lOfs], btViewSkillState, pMonsterViewportCreate.btViewSkillCount);
				lOfs += pMonsterViewportCreate.btViewSkillCount;
			}

			moncount += 1;
		}
	
		if(count > 0)
		{
			PWMSG_COUNT pwCount;
	
			pwCount.h.c = 0xC2;
			pwCount.h.headcode = 0x1F;
			pwCount.h.sizeH = SET_NUMBERH(lOfs);
			pwCount.h.sizeL = SET_NUMBERL(lOfs);
			pwCount.count = count;
	
			memcpy(sendBuf,&pwCount,sizeof(pwCount));
			gSendProto.VPMsgSend(lpObj,sendBuf,lOfs);
		}
	
		if(moncount > 0)
		{
			PWMSG_COUNT pwCount;
	
			pwCount.h.c = 0xC2;
			pwCount.h.headcode = 0x13;
			pwCount.h.sizeH = SET_NUMBERH(lOfs);
			pwCount.h.sizeL = SET_NUMBERL(lOfs);
			pwCount.count = moncount;
	
			memcpy(sendBuf,&pwCount,sizeof(pwCount));
			gSendProto.VPMsgSend(lpObj,sendBuf,lOfs);
		}
	}
}

void gObjViewportListProtocol(short aIndex)
{
	if(gObj[aIndex].Connected < PLAYER_PLAYING)
	{
		return;
	}
	
	int n;
	PBMSG_COUNT pCount;
	LPOBJ lpTargetObj,lpObj = &gObj[aIndex];
	int lOfs_Item,count_Item,ItemStructSize = sizeof(PMSG_ITEMVIEWPORTCREATE),lOfs,MonlOfs,callMonlOfs;
	int pMsgSize;
	short tObjNum;
	BYTE count,monstercount,callmonstercount;
	
	BYTE sendBuf[5000];
	BYTE MonstersendBuf[5000];
	BYTE callMonstersendBuf[5000];
	BYTE ItemBuf[5000];

	WORD btViewSkillBuffer[MAX_VIEWSKILL] = {0}; // teste
	
	GuildInfoOfs = 0;
	GuildInfoCount = 0;
	GuildUserOfs = 0;
	GuildUserCount = 0;
	ChangeCount = 0;
	GuildInfoOfs = 5;
	GuildUserOfs = 5;
	lOfsChange = 5;
	
	lOfs = 4;
	lOfs_Item = 5;
	count = 0;
	count_Item = 0;
	
	if(lpObj->Type == OBJ_USER)
	{
		for(n = 0; n < MAX_VIEWPORT;n++)
		{
			if(lpObj->VpPlayer[n].state == 3)
			{
				tObjNum = lpObj->VpPlayer[n].number;
				
				if(tObjNum >= 0)
				{
					switch(lpObj->VpPlayer[n].type)
					{
					case 1:
					case 2:
					case 3:
						pViewportDestroy.NumberH = SET_NUMBERH(tObjNum);
						pViewportDestroy.NumberL = SET_NUMBERL(tObjNum);
						
						memcpy(&sendBuf[lOfs],&pViewportDestroy,sizeof(pViewportDestroy));
						lOfs += sizeof(pViewportDestroy);
						count += 1;
						break;
					case 5:
						pItemViewportDestroy.NumberH = SET_NUMBERH(tObjNum);
						pItemViewportDestroy.NumberL = SET_NUMBERL(tObjNum);
						
						memcpy(&ItemBuf[lOfs_Item],&pItemViewportDestroy,sizeof(pItemViewportDestroy));
						
						lOfs_Item += sizeof(pItemViewportDestroy);
						count_Item += 1;
						break;
					}
				}
				
				lpObj->VpPlayer[n].state = 0;
				lpObj->VpPlayer[n].number = -1;
				lpObj->VPCount -= 1;
			}
		}
	}
	else if(lpObj->Type == OBJ_MONSTER || lpObj->Type == OBJ_NPC)
	{
		for(n = 0; n < MAX_VIEWPORT_MONSTER;n++)
		{
			if(lpObj->VpPlayer[n].state == 3)
			{
				lpObj->VpPlayer[n].state = 0;
				lpObj->VpPlayer[n].number = -1;
				lpObj->VPCount -= 1;
			}
		}
	}
	
	if(lpObj->Type == OBJ_USER)
	{
		if(count > 0)
		{
			pCount.h.c = 0xC1;
			pCount.h.headcode = 0x14;
			pCount.h.size = lOfs;
			pCount.count = count;
			
			memcpy(sendBuf,&pCount,sizeof(pCount));
			gSendProto.DataSend(aIndex,sendBuf,lOfs);
		}
		
		if(count_Item > 0)
		{
			pItemCount.h.c = 0xC2;
			pItemCount.h.headcode = 0x21;
			pItemCount.h.sizeH = SET_NUMBERH(lOfs_Item);
			pItemCount.h.sizeL = SET_NUMBERL(lOfs_Item);
			pItemCount.count = count_Item;
			
			memcpy(ItemBuf,&pItemCount,sizeof(pItemCount));
			gSendProto.DataSend(aIndex,ItemBuf,lOfs_Item);
		}
	}
	
	if(lpObj->Type == OBJ_MONSTER || lpObj->Type == OBJ_NPC)
	{
		for(n = 0; n < MAX_VIEWPORT_MONSTER;n++)
		{
			if(lpObj->VpPlayer[n].state == 1)
			{
				lpObj->VpPlayer[n].state = 2;
			}
		}
	}
	else
	{
		count = 0;
		monstercount = 0;
		count_Item = 0;
		callmonstercount = 0;
		lOfs = 5;
		MonlOfs = lOfs;
		callMonlOfs = MonlOfs;
		lOfs_Item = 5;
		pMsgSize = 44;
		
		if(lpObj->Type == OBJ_USER)
		{
			for(n = 0; n < MAX_VIEWPORT;n++)
			{
				if(lpObj->VpPlayer[n].state == 1)
				{
					tObjNum = lpObj->VpPlayer[n].number;
					
					if(tObjNum >= 0)
					{
						switch(lpObj->VpPlayer[n].type)
						{
						case 1:
							lpTargetObj = &gObj[tObjNum];
							
							if(lpTargetObj->m_Change >= 0)
							{
								pViewportCreateChange.NumberH = SET_NUMBERH(tObjNum); // vc6 fdp
								pViewportCreateChange.NumberL = SET_NUMBERL(tObjNum);

								lpTargetObj->CharSet[0] &= 0xF8;
								
								if(lpTargetObj->m_State == 1 && lpTargetObj->Teleport == 0)
								{
									pViewportCreateChange.NumberH |= 0x80;
								}
								
								pViewportCreateChange.X = lpTargetObj->X;
								pViewportCreateChange.Y = lpTargetObj->Y;
								pViewportCreateChange.TX = lpTargetObj->TX;
								pViewportCreateChange.TY = lpTargetObj->TY;
								pViewportCreateChange.SkinH = SET_NUMBERH((lpTargetObj->m_Change & 0xFFFF) & 0xFFFF);
								pViewportCreateChange.SkinL = SET_NUMBERL((lpTargetObj->m_Change & 0xFFFF) & 0xFFFF);
								pViewportCreateChange.DirAndPkLevel = lpTargetObj->Dir << 4;
								pViewportCreateChange.DirAndPkLevel |= lpTargetObj->m_PK_Level & 0x0F;

								if( gUser.ChaosCastleMapRange(lpTargetObj->MapNumber) == true )
								{
									pViewportCreateChange.btViewSkillCount = 0;
								}
#if(GS_CASTLE)
								if( g_CastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE )
								{
									BYTE btJoinSide = lpTargetObj->m_btCsJoinSide;
									
									if( btJoinSide < 0 )
									{
										btJoinSide = 0;
									}
									
									if( btJoinSide > 2 )
									{
										btJoinSide = 2;
									}
									
									switch( btJoinSide )
									{
									case 1:
										// isso aki eu ainda n entendi pra q vc ta colocando... teria que ser antes do negocio ali de cima
										//pViewportCreateChange.ViewSkillState[eVS_ALTAR_OF_WOLF_HERO_CONTRACT_STATUS]; // isso vai dar crash no gs
										// isso aqui muda o estado do barato no sistema novo... ai a conversao vc faz depois s�
										lpTargetObj->m_ViewSkillState[eVS_ALTAR_OF_WOLF_HERO_CONTRACT_STATUS] = 1; // isso dai eh a parte do gscs a comp do gs normal ja arrumei toda
										//acho q eh soh isso...  FUCK vc bugo >.<? o.0
										break;
									case 2:
										{
											//int iViewSkillState = 0;
											
											if( lpTargetObj->m_btCsJoinSide == 2 )
											{
												//iViewSkillState = 0x80000;
												lpTargetObj->m_ViewSkillState[eVS_ALTAR_OF_WOLF_CONTRACT_ATTEMPT] = 1; 
											}
											else if( lpTargetObj->m_btCsJoinSide == 3 )
											{
												//iViewSkillState = 0x400000; << era pra ser 40000 e nao 400000 o.0
												//lpTargetObj->m_ViewSkillState[???2] = 1; //eVS_ALTAR_OF_WOLF_HERO_CONTRACT_STATUS??
											}
											else if( lpTargetObj->m_btCsJoinSide == 4 )
											{
												//iViewSkillState = 0x800000; << ta errado tb pq n tem na lista
												//lpTargetObj->m_ViewSkillState[???3] = 1; 
											}
											//pViewportCreateChange.ViewSkillState |= iViewSkillState;
											//lpTargetObj->m_ViewSkillState |= iViewSkillState;
										}
										break;
									default:
										{
											//pViewportCreateChange.ViewSkillState &= 0xFFF3FFFF;
											//lpTargetObj->m_ViewSkillState &= 0xFFF3FFFF;
											//pViewportCreateChange.ViewSkillState &= 0xFFF3FFFF;
											//lpTargetObj->m_ViewSkillState &= 0xFFF3FFFF;

											// nao entendi pq  q faz 2x a msm bosta o.0
											lpTargetObj->m_ViewSkillState[eVS_ALTAR_OF_WOLF_CONTRACT_ATTEMPT] = 0; 
											//lpTargetObj->m_ViewSkillState[???2] = 0; 
										}
										break;
									}
								}
								
#endif

								ZeroMemory(&btViewSkillBuffer, MAX_VIEWSKILL);
								pViewportCreateChange.btViewSkillCount = gObjViewSkillBufferConvert(lpTargetObj, &btViewSkillBuffer[0]);
								
								memcpy(pViewportCreateChange.Id,lpTargetObj->Name,sizeof(pViewportCreateChange.Id));
								memcpy(&SendGBufChange[lOfsChange],&pViewportCreateChange,sizeof(pViewportCreateChange));
								lOfsChange += sizeof(pViewportCreateChange);
								memcpy(&SendGBufChange[lOfsChange],&btViewSkillBuffer, pViewportCreateChange.btViewSkillCount);
								lOfsChange += pViewportCreateChange.btViewSkillCount;

								ChangeCount +=1;
							}
							else
							{
								pViewportCreate.NumberH = SET_NUMBERH(tObjNum);
								pViewportCreate.NumberL = SET_NUMBERL(tObjNum);

								lpTargetObj->CharSet[0] &= 0xFF;
								
								if(lpTargetObj->m_State == 1 && lpTargetObj->Teleport == 0)
								{
									pViewportCreate.NumberH |= 0x80;
								}
								
//								pViewportCreate.ViewSkillState = 0;

								lpTargetObj->CharSet[0] |= lpTargetObj->m_ViewState & 0xFF;
								
								pViewportCreate.X = lpTargetObj->X;
								pViewportCreate.Y = lpTargetObj->Y;
								pViewportCreate.TX = lpTargetObj->TX;
								pViewportCreate.TY = lpTargetObj->TY;
								
								pViewportCreate.DirAndPkLevel = lpTargetObj->Dir << 4;
								pViewportCreate.DirAndPkLevel |= lpTargetObj->m_PK_Level & 0x0F;

								
								if( gUser.ChaosCastleMapRange(lpTargetObj->MapNumber) == true )
								{
									if ( Gens.IsGenZone(lpTargetObj) == true )
									{
										pViewportCreate.btViewSkillCount = 0;
									}
								}
								
#if(GS_CASTLE)
								
								if( g_CastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE )
								{
									BYTE btJoinSide = lpTargetObj->m_btCsJoinSide;
									
									if( btJoinSide < 0 )
									{
										btJoinSide = 0;
									}
									
									if( btJoinSide > 2 )
									{
										btJoinSide = 2;
									}
									
									switch( btJoinSide )
									{
									case 1:
										//pViewportCreateChange.ViewSkillState |= 0x40000;
										//lpTargetObj->m_ViewSkillState |= 0x40000;
										lpTargetObj->m_ViewSkillState[eVS_ALTAR_OF_WOLF_CONTRACT_ATTEMPT] = 1;
										break;
									case 2:
										{
											//int iViewSkillState = 0;
											
											if( lpTargetObj->m_btCsJoinSide == 2 )
											{
												//iViewSkillState = 0x80000;
												//lpTargetObj->m_ViewSkillState[???] = 1;
											}
											else if( lpTargetObj->m_btCsJoinSide == 3 )
											{
												//iViewSkillState = 0x400000;
												//lpTargetObj->m_ViewSkillState[???] = 1;
											}
											else if( lpTargetObj->m_btCsJoinSide == 4 )
											{
												//iViewSkillState = 0x800000;
												//lpTargetObj->m_ViewSkillState[???] = 1;
											}
											//pViewportCreateChange.ViewSkillState |= iViewSkillState;
											//lpTargetObj->m_ViewSkillState |= iViewSkillState;
										}
										break;
									default:
										{
											//pViewportCreateChange.ViewSkillState &= 0xFFF3FFFF;
											//lpTargetObj->m_ViewSkillState &= 0xFFF3FFFF;
											//pViewportCreateChange.ViewSkillState &= 0xFFF3FFFF;
											//lpTargetObj->m_ViewSkillState &= 0xFFF3FFFF;
											//lpTargetObj->m_ViewSkillState[eVS_ALTAR_OF_WOLF_CONTRACT_ATTEMPT] = 0;
										}
										break;
									}
								}
								
#endif
								ZeroMemory(&btViewSkillBuffer, MAX_VIEWSKILL);
								pViewportCreate.btViewSkillCount = gObjViewSkillBufferConvert(lpTargetObj, &btViewSkillBuffer[0]);

								memcpy(pViewportCreate.CharSet,lpTargetObj->CharSet,sizeof(pViewportCreate.CharSet));
								memcpy(pViewportCreate.Id,lpTargetObj->Name,sizeof(pViewportCreate.Id));
								memcpy(&sendBuf[lOfs],&pViewportCreate,sizeof(pViewportCreate));
								lOfs += sizeof(pViewportCreate);
								memcpy(&sendBuf[lOfs], &btViewSkillBuffer, pViewportCreate.btViewSkillCount);
								lOfs += pViewportCreate.btViewSkillCount;
								
								lOfs += sizeof(pViewportCreate);
								count += 1;
							}
							
							if(lpObj->Type == OBJ_USER)
							{
								if( lpTargetObj->lpGuild != 0 && Gens.IsGenZone(lpTargetObj) == false )
								{
									PMSG_SIMPLE_GUILDVIEWPORT pGuild;
									
									pGuild.GuildNumber = lpTargetObj->lpGuild->Number;
									pGuild.NumberH = SET_NUMBERH(lpTargetObj->m_Index) & 0x7F;
									pGuild.NumberL = SET_NUMBERL(lpTargetObj->m_Index);
									
									pGuild.btGuildStatus = lpTargetObj->GuildStatus;
									pGuild.btGuildType = lpTargetObj->lpGuild->btGuildType;
									
									if(lpObj->lpGuild != 0)
									{
										pGuild.btGuildRelationShip = gObjGetRelationShip(lpObj,lpTargetObj);
									}
									else
									{
										pGuild.btGuildRelationShip = 0;
									}
									
									if(strcmp(lpTargetObj->lpGuild->Names[0],lpTargetObj->Name)==0)
									{
										pGuild.NumberH |= 0x80;
									}
									
									memcpy(&GuildInfoBuf[GuildInfoOfs],&pGuild,sizeof(pGuild));
									GuildInfoOfs += sizeof(pGuild);
									GuildInfoCount += 1;
								}
								
								if(false)
								{
									PMSG_GUILDVIEWPORT_USER pGuildUserViewport;
									
									if(ViewGuildMng.Add(lpTargetObj->GuildNumber,lpTargetObj->m_Index)==1 && lpTargetObj->lpGuild != 0)
									{
										PMSG_GUILDVIEWPORT pGuildViewport;
										
										pGuildViewport.NumberH = SET_NUMBERH(lpTargetObj->lpGuild->Number);
										pGuildViewport.NumberL = SET_NUMBERL(lpTargetObj->lpGuild->Number);
										
										strcpy(pGuildViewport.GuildName,lpTargetObj->lpGuild->Name);
										memcpy(pGuildViewport.Mark,lpTargetObj->lpGuild->Mark,sizeof(pGuildViewport.Mark));
										memcpy(&GuildInfoBuf[GuildInfoOfs],&pGuildViewport,sizeof(pGuildViewport));
										
										GuildInfoOfs += sizeof(pGuildViewport);
										GuildInfoCount += 1;
									}
									
									pGuildUserViewport.NumberH = SET_NUMBERH(lpTargetObj->m_Index) & 0x7F;
									pGuildUserViewport.NumberL = SET_NUMBERL(lpTargetObj->m_Index);
									
									if(lpTargetObj->lpGuild != 0 && strcmp(lpTargetObj->lpGuild->Names[0],lpTargetObj->Name)==0)
									{
										pGuildUserViewport.NumberH |= 0x80;
									}
									
									if(lpTargetObj->lpGuild != 0)
									{
										pGuildUserViewport.GNumberH = SET_NUMBERH(lpTargetObj->lpGuild->Number);
										pGuildUserViewport.GNumberL = SET_NUMBERL(lpTargetObj->lpGuild->Number);
										
										memcpy(&GuildUserBuf[GuildUserOfs],&pGuildUserViewport,sizeof(pGuildUserViewport));
										
										GuildUserOfs += sizeof(pGuildUserViewport);
										GuildUserCount += 1;
									}
								}
							}
							break;
						case 2:
						case 3:
							if(lpObj->Type == OBJ_USER)
							{
								lpTargetObj = &gObj[tObjNum];
								
								if(lpTargetObj->m_RecallMon >= 0)
								{
									pCallMonsterViewportCreate.NumberH = SET_NUMBERH(tObjNum);
									pCallMonsterViewportCreate.NumberL = SET_NUMBERL(tObjNum);
									
									if(lpTargetObj->m_State == 1)
									{
										pCallMonsterViewportCreate.NumberH |= 0x80;
									}
									
									pCallMonsterViewportCreate.Type_HI = SET_NUMBERH(lpTargetObj->Class);
									pCallMonsterViewportCreate.Type_LO = SET_NUMBERL(lpTargetObj->Class);

									pCallMonsterViewportCreate.X = lpTargetObj->X;
									pCallMonsterViewportCreate.Y = lpTargetObj->Y;
									pCallMonsterViewportCreate.TX = lpTargetObj->TX;
									pCallMonsterViewportCreate.TY = lpTargetObj->TY;
									pCallMonsterViewportCreate.Path = lpTargetObj->Dir << 4;

									ZeroMemory(&btViewSkillBuffer, MAX_VIEWSKILL);
									pCallMonsterViewportCreate.btViewSkillCount = gObjViewSkillBufferConvert(lpTargetObj, &btViewSkillBuffer[0]);
									
									if(lpTargetObj->m_RecallMon >= 0 && lpTargetObj->m_RecallMon < OBJMAX-1)
									{
										memcpy(pCallMonsterViewportCreate.Id,gObj[lpTargetObj->m_RecallMon].Name,sizeof(pCallMonsterViewportCreate.Id));
									}
									else
									{
										memset(pCallMonsterViewportCreate.Id,0x00,sizeof(pCallMonsterViewportCreate.Id));
									}
									
									memcpy(&callMonstersendBuf[callMonlOfs],&pCallMonsterViewportCreate,sizeof(pCallMonsterViewportCreate));
									callMonlOfs += sizeof(pCallMonsterViewportCreate);
									callmonstercount += 1;
								}
								else
								{
									pMonsterViewportCreate.NumberH = SET_NUMBERH(tObjNum);
									pMonsterViewportCreate.NumberL = SET_NUMBERL(tObjNum);
									
									if(lpTargetObj->m_State == 1)
									{
										pMonsterViewportCreate.NumberH |= 0x80;
										
										if(lpTargetObj->Teleport != 0)
										{
											pMonsterViewportCreate.NumberH |= 0x40;
										}
									}
									
									pMonsterViewportCreate.Type_HI = SET_NUMBERH(lpTargetObj->Class);
									pMonsterViewportCreate.Type_LO = SET_NUMBERL(lpTargetObj->Class);
									
#if (GS_CASTLE)
									if( lpTargetObj->Class == 278 )
									{
										if( g_CastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE )
										{
											if( lpTargetObj->m_btCsJoinSide == lpObj->m_btCsJoinSide )
											{
												pItemViewportCreate.NumberH |= 0x80;
											}
										}
										
										if( lpTargetObj->lpGuild && lpObj->lpGuild )
										{
											if( lpTargetObj->lpGuild == lpObj->lpGuild )
											{
												pItemViewportCreate.NumberH |= 0x80;
											}
										}
										
										pItemViewportCreate.NumberH |= (lpTargetObj->m_btCreationState << 4) & 0x70;
									}
#endif

									pMonsterViewportCreate.X = lpTargetObj->X;
									pMonsterViewportCreate.Y = lpTargetObj->Y;
									pMonsterViewportCreate.TX = lpTargetObj->TX;
									pMonsterViewportCreate.TY = lpTargetObj->TY;
									pMonsterViewportCreate.Path = lpTargetObj->Dir << 4;
#if (GS_CASTLE)
									if( lpTargetObj->Class == 277 )
									{
										if( lpTargetObj->m_btCsGateOpen == 0 )
										{
											pMonsterViewportCreate.btViewSkillCount &= 0xFFFDFFFF;
										}
										else
										{
											pMonsterViewportCreate.btViewSkillCount |= 0x20000;
										}
									}
									
									if( lpTargetObj->Class == 216 )
									{
										if( g_CastleSiege.GetRegCrownAvailable() == FALSE )
										{
											pMonsterViewportCreate.btViewSkillCount &= 0xFFFDFFFF;
										}
										else
										{
											pMonsterViewportCreate.btViewSkillCount |= 0x20000;
										}
									}
#endif
									ZeroMemory(&btViewSkillBuffer, MAX_VIEWSKILL);
									pMonsterViewportCreate.btViewSkillCount = gObjViewSkillBufferConvert(lpTargetObj, &btViewSkillBuffer[0]);
		
									memcpy(&MonstersendBuf[MonlOfs],&pMonsterViewportCreate,sizeof(pMonsterViewportCreate));
									MonlOfs += sizeof(pMonsterViewportCreate);
									memcpy(&MonstersendBuf[MonlOfs],&btViewSkillBuffer, pMonsterViewportCreate.btViewSkillCount);
									MonlOfs += pMonsterViewportCreate.btViewSkillCount;
									monstercount += 1;
								}
							}
							break;
						case 5:
							if(lpObj->Type == OBJ_USER)
							{
								pItemViewportCreate.NumberH = SET_NUMBERH(tObjNum);
								pItemViewportCreate.NumberL = SET_NUMBERL(tObjNum);
								
								if(MapC[lpObj->MapNumber].m_cItem[tObjNum].m_State == 1)
								{
									pItemViewportCreate.NumberH |= 0x80;
								}
								
								pItemViewportCreate.px = MapC[lpObj->MapNumber].m_cItem[tObjNum].px;
								pItemViewportCreate.py = MapC[lpObj->MapNumber].m_cItem[tObjNum].py;
								
								if(MapC[lpObj->MapNumber].m_cItem[tObjNum].m_Type == ITEMGET(14,15))
								{
									WORD MoneyHW = SET_NUMBERHW(MapC[lpObj->MapNumber].m_cItem[tObjNum].m_BuyMoney);
									WORD MoneyLW = SET_NUMBERLW(MapC[lpObj->MapNumber].m_cItem[tObjNum].m_BuyMoney);
									
									pItemViewportCreate.ItemInfo[0] = BYTE(MapC[lpObj->MapNumber].m_cItem[tObjNum].m_Type)%255;
									pItemViewportCreate.ItemInfo[1] = SET_NUMBERL(MoneyHW);
									pItemViewportCreate.ItemInfo[2] = SET_NUMBERH(MoneyLW);
									pItemViewportCreate.ItemInfo[4] = SET_NUMBERL(MoneyLW);
									pItemViewportCreate.ItemInfo[3] = 0;
									pItemViewportCreate.ItemInfo[5] = (MapC[lpObj->MapNumber].m_cItem[tObjNum].m_Type & 0x1E00) >> 5;
									pItemViewportCreate.ItemInfo[6] = 0;
									// ---- Sockets Added
									pItemViewportCreate.ItemInfo[7] = 0xFF;
									pItemViewportCreate.ItemInfo[8] = 0xFF;
									pItemViewportCreate.ItemInfo[9] = 0xFF;
									pItemViewportCreate.ItemInfo[10] = 0xFF;
									pItemViewportCreate.ItemInfo[11] = 0xFF;
									
									memcpy(&ItemBuf[lOfs_Item],&pItemViewportCreate,sizeof(pItemViewportCreate));
									lOfs_Item += ItemStructSize;
								}
								else
								{
									ItemByteConvert(pItemViewportCreate.ItemInfo, (MapC[lpObj->MapNumber].m_cItem[tObjNum]));
									memcpy(&ItemBuf[lOfs_Item],&pItemViewportCreate,sizeof(pItemViewportCreate));
									lOfs_Item += ItemStructSize;
								}
								
								count_Item += 1;
							}
							break;
						}
					}
					lpObj->VpPlayer[n].state = 2;
				}
			}
		}
		if(lpObj->Type == OBJ_USER)
		{
			if( count > 0 )
			{
				PWMSG_COUNT pCount;
				
				pCount.h.c = 0xC2;
				pCount.h.headcode = 0x12;
				pCount.h.sizeH = SET_NUMBERH(lOfs);
				pCount.h.sizeL = SET_NUMBERL(lOfs);
				pCount.count = count;
				
				memcpy(sendBuf,&pCount,sizeof(pCount));
				gSendProto.DataSend(aIndex,(unsigned char *)&sendBuf,lOfs);

				// --- New Added
				Gens.SendLogo(lpObj,sendBuf);
			}
			
			if(ChangeCount > 0)
			{
				PWMSG_COUNT pCount;
				
				pCount.h.c = 0xC2;
				pCount.h.headcode = 0x45;
				pCount.h.sizeH = SET_NUMBERH(lOfsChange);
				pCount.h.sizeL = SET_NUMBERL(lOfsChange);
				pCount.count = ChangeCount;
				
				memcpy(SendGBufChange,&pCount,sizeof(pCount));
				gSendProto.DataSend(aIndex,(unsigned char *)&SendGBufChange,lOfsChange);
			}
			
			if(monstercount > 0)
			{
				PWMSG_COUNT pCount;
				
				pCount.h.c = 0xC2;
				pCount.h.headcode = 0x13;
				pCount.h.sizeH = SET_NUMBERH(MonlOfs);
				pCount.h.sizeL = SET_NUMBERL(MonlOfs);
				pCount.count = monstercount;
				
				memcpy(MonstersendBuf,&pCount,sizeof(pCount));
				gSendProto.DataSend(aIndex,(unsigned char *)&MonstersendBuf,MonlOfs);
			}	
			
			if(callmonstercount > 0)
			{
				PWMSG_COUNT pCount;
				
				pCount.h.c = 0xC2;
				pCount.h.headcode = 0x1F;
				pCount.h.sizeH = SET_NUMBERH(callMonlOfs);
				pCount.h.sizeL = SET_NUMBERL(callMonlOfs);
				pCount.count = callmonstercount;
				
				memcpy(callMonstersendBuf,&pCount,sizeof(pCount));
				gSendProto.DataSend(aIndex,(unsigned char *)&callMonstersendBuf,callMonlOfs);
			}
			
			if(count_Item > 0)
			{
				PWMSG_COUNT pCount;
				
				pCount.h.c = 0xC2;
				pCount.h.headcode = 0x20;
				pCount.count = count_Item;
				pCount.h.sizeH = SET_NUMBERH(lOfs_Item);
				pCount.h.sizeL = SET_NUMBERL(lOfs_Item);
				
				memcpy(ItemBuf,&pCount,sizeof(pCount));
				gSendProto.DataSend(aIndex,(unsigned char *)&ItemBuf,lOfs_Item);
			}
			
			if( GuildInfoCount != 0 )
			{
				PWMSG_COUNT pCount;
				
				pCount.h.headcode = 0x65;
				pCount.h.c = 0xC2;
				pCount.h.sizeH = SET_NUMBERH(GuildInfoOfs);
				pCount.h.sizeL = SET_NUMBERL(GuildInfoOfs);
				pCount.count = GuildInfoCount;
				
				memcpy(GuildInfoBuf,&pCount,sizeof(pCount));
				gSendProto.DataSend(aIndex,(unsigned char *)&GuildInfoBuf,GuildInfoOfs);
			}
		}
	}
}
// Reformulated
void gObjSkillUseProc(LPOBJ lpObj)
{
	// ---- Elf Damage Increase
	if(lpObj->m_SkillAttackTime > 0)
	{
		lpObj->m_SkillAttackTime--;

		if(lpObj->m_SkillAttackTime == 0)
		{
			lpObj->m_SkillAttack = 0;
			GCStateInfoSend(lpObj,0,eVS_ATTACK_AURA);
			GCMagicCancelSend(lpObj,AT_SKILL_ATTACK);
		}
	}

	if(lpObj->m_SkillAttackTime2 > 0)
	{
		lpObj->m_SkillAttackTime2--;

		if(lpObj->m_SkillAttackTime2 <= 0)
		{
			lpObj->m_SkillAttack2 = 0;
			lpObj->m_SkillAttackTime2 = 0;
		}
	}

	// ---- Elf Defense Increase
	if(lpObj->m_SkillDefenseTime > 0)
	{
		lpObj->m_SkillDefenseTime--;

		if(lpObj->m_SkillDefenseTime == 0)
		{
			lpObj->m_SkillDefense = 0;
			GCStateInfoSend(lpObj,0,eVS_DEFENSE_AURA);
			GCMagicCancelSend(lpObj,AT_SKILL_DEFENSE);
		}
	}

	// ---- Dark Wizard Mana Shield
	if(lpObj->m_WizardSkillDefenseTime > 0)
	{
		lpObj->m_WizardSkillDefenseTime--;

		if(lpObj->m_WizardSkillDefenseTime <= 0)
		{
			lpObj->m_WizardSkillDefense = 0;
			GCStateInfoSend(lpObj,0,eVS_SOUL_BARRIER);
			GCMagicCancelSend(lpObj,AT_SKILL_MAGICDEFENSE);
		}
	}

	// ---- Dark Knight Greater Fortitued
	if(lpObj->m_SkillAddLifeTime > 0)
	{
		lpObj->m_SkillAddLifeTime--;

		if(lpObj->m_SkillAddLifeTime <= 0)
		{
			lpObj->AddLife -= lpObj->m_SkillAddLife;

			if(lpObj->AddLife < 0)
			{
				lpObj->AddLife = 0;
			}

			GCReFillSend(lpObj->m_Index,lpObj->MaxLife+lpObj->AddLife,0xFE,0,lpObj->iMaxShield+lpObj->iAddShield);

			if(lpObj->Life > lpObj->MaxLife + lpObj->AddLife)
			{
				lpObj->Life = lpObj->MaxLife + lpObj->AddLife;
				GCReFillSend(lpObj->m_Index,lpObj->Life,0xFF,0,lpObj->iShield);
			}

			lpObj->m_SkillAddLife = 0;
			GCStateInfoSend(lpObj,0,eVS_SWELL_LIFE);
			GCMagicCancelSend(lpObj,AT_SKILL_KNIGHTADDLIFE);
		}
	}

	// ---- Elf Ice Arrow
	if(lpObj->m_SkillHardenTime > 0)
	{
		lpObj->m_SkillHardenTime--;

		if(lpObj->m_SkillHardenTime <= 0)
		{
			lpObj->m_SkillHarden = 0;
			GCStateInfoSend(lpObj,0,eVS_ICE_ARROW);
			GCMagicCancelSend(lpObj,AT_SKILL_ELFHARDEN);
		}
	}

	// ---- Magic Gladiator Defense Down
	if(lpObj->m_SkillMagumReduceDefenseTime > 0)
	{
		lpObj->m_SkillMagumReduceDefenseTime--;

		if(lpObj->m_SkillMagumReduceDefenseTime <= 0)
		{
			lpObj->m_SkillMagumReduceDefense = 0;
			GCStateInfoSend(lpObj,0,eVS_DEFENSE_DOWN);
			GCMagicCancelSend(lpObj,AT_SKILL_DEFENSEDOWN);
		}
	}

	// ---- Summoner Sleep
	if( lpObj->m_SkillSleepTime > 0 )
	{
		lpObj->m_SkillSleepTime--;

		if( lpObj->m_SkillSleepTime <= 0)
		{
			GCStateInfoSend(lpObj,0,eVS_SKILL_SLEEP);
		}
	}

	// ---- Summoner Night
	if ( lpObj->m_SkillNightTime > 0 )
	{
		lpObj->m_SkillNightTime--;

		if ( lpObj->m_SkillNightTime <= 0 )
		{
			lpObj->m_Defense += lpObj->m_SkillNight;
			lpObj->m_MagicDefense += lpObj->m_SkillNight;
			lpObj->m_SkillNight = 0;
			lpObj->m_SkillNightTime = 0;
			GCStateInfoSend(lpObj,0,eVS_SKILL_BLIDNESS);
		}
	}

	// ---- Summoner Reflex
	if( lpObj->m_SkillReflexTime > 0 )
	{
		lpObj->m_SkillReflexTime--;

		if( lpObj->m_SkillReflexTime <= 0)
		{
			lpObj->DamageReflect -= lpObj->m_SkillReflex;
			lpObj->m_SkillReflex = 0;
			lpObj->m_SkillReflexTime = 0;
			GCStateInfoSend(lpObj,0,eVS_SKILL_DAMAGEREFLECTION);
		}
	}

	// ---- Weakness Skill
	if( lpObj->m_SkillWeaknessTime > 0 )
	{
		lpObj->m_SkillWeaknessTime--;

		if( lpObj->m_SkillWeaknessTime <= 0 )
		{
			lpObj->m_SkillWeakness = 0;
			lpObj->m_SkillWeaknessTime = 0;
			GCStateInfoSend(lpObj,0,eVS_SKILL_NICEWEEK);
		}
	}

	// ---- Innovation Skill
	if( lpObj->m_SkillInnovationTime > 0 )
	{
		lpObj->m_SkillInnovationTime--;

		if( lpObj->m_SkillInnovationTime <= 0 )
		{
			lpObj->m_SkillInnovationTime = 0;
			lpObj->m_SkillInnovation = 0;
			GCStateInfoSend(lpObj,0,eVS_SKILL_INNERBEIYSEYON);
		}
	}

	// ---- Summoner Berseker
	if( lpObj->m_SkillBerserkerTime > 0 )
	{
		lpObj->m_SkillBerserkerTime--;

		if( lpObj->m_SkillBerserkerTime <= 0 )
		{
			lpObj->m_SkillBerserkerTime = 0;
			lpObj->m_SkillBerserker = 0;
			GCStateInfoSend(lpObj,0,eVS_SKILL_SWORDPOWER);
		}
	}

	// ---- Rage Fighter Increase Defense Success Rate
	if( lpObj->m_RageFighterSkillDefenseRateTime > 0 )
	{
		lpObj->m_RageFighterSkillDefenseRateTime--;

		if( lpObj->m_RageFighterSkillDefenseRateTime <= 0 )
		{
			lpObj->m_Defense -= lpObj->m_RageFighterSkillDefenseRate;
			lpObj->m_RageFighterSkillDefenseRate = 0;
			lpObj->m_RageFighterSkillDefenseRateTime = 0;
			GCStateInfoSend(lpObj,0,eVS_DEFENSE_SUCCESS_RATE);
		}
	}

	// ---- Rage Fighter Fitness
	if( lpObj->m_RageFighterSkillFitnessTime > 0 )
	{
		lpObj->m_RageFighterSkillFitnessTime--;

		if( lpObj->m_RageFighterSkillFitnessTime <= 0 )
		{
			lpObj->AddLife -= lpObj->m_RageFighterSkillFitnessAddLife;
			if(lpObj->AddLife < 0)
			{
				lpObj->AddLife = 0;
			}

			GCReFillSend(lpObj->m_Index,lpObj->MaxLife+lpObj->AddLife,0xFE,0,lpObj->iMaxShield+lpObj->iAddShield);

			if(lpObj->Life > lpObj->MaxLife + lpObj->AddLife)
			{
				lpObj->Life = lpObj->MaxLife + lpObj->AddLife;
				GCReFillSend(lpObj->m_Index,lpObj->Life,0xFF,0,lpObj->iShield);
			}

			lpObj->m_RageFighterSkillFitnessAddLife = 0;
			lpObj->m_RageFighterSkillFitnessTime = 0;
			GCStateInfoSend(lpObj,0,eVS_FITNESS);
		}
	}

	// ---- Rage Fighter Red Armor Ignore
	if( lpObj->m_RageFighterSkillRedArmorIgnoreTime > 0 )
	{
		lpObj->m_RageFighterSkillRedArmorIgnoreTime--;

		if( lpObj->m_RageFighterSkillRedArmorIgnoreTime <= 0 )
		{
			lpObj->SetOpIgnoreDefense -= lpObj->m_RageFighterSkillRedArmorIgnore;
			lpObj->m_RageFighterSkillRedArmorIgnore = 0;
			lpObj->m_RageFighterSkillRedArmorIgnoreTime = 0;
			GCStateInfoSend(lpObj,0,eVS_IGNORE_DEFENSE);
		}
	}

	// ---- Dark Lord Add Critical Damage
	if(lpObj->SkillAddCriticalDamageTime > 0 && lpObj->Type == OBJ_USER)
	{
		lpObj->SkillAddCriticalDamageTime--;

		if(lpObj->SkillAddCriticalDamageTime <= 0)
		{
			lpObj->m_CriticalDamage -= lpObj->SkillAddCriticalDamage;
			lpObj->SkillAddCriticalDamageTime = 0;
			lpObj->SkillAddCriticalDamage = 0;
			GCStateInfoSend(lpObj,0,eVS_CRITICAL_DAMAGE);
			GCMagicCancelSend(lpObj,64);
		}
	}

	// ---- Magic Circle
	if ( lpObj->SkillMagicCircleTime > 0 )
	{
		lpObj->SkillMagicCircleTime--;
		// ----
		if ( lpObj->SkillMagicCircleTime <= 0 )
		{
			lpObj->SkillMagicCircleTime = 0;
			lpObj->m_MagicDamageMin -= lpObj->SkillMagicCircle;
			lpObj->SkillMagicCircle = 0;
			GCStateInfoSend(lpObj,0,eVS_SKILL_MAGICCIRCLE);
		}
	}

	

	if(lpObj->Class == 77) // have fun !
	{
		lpObj->m_SkyBossMonSheildTime++;

		if(lpObj->m_SkyBossMonSheildTime > 5)
		{
			lpObj->m_SkyBossMonSheildTime = 0;

			if(lpObj->m_SkyBossMonSheild != 0)
			{
				lpObj->m_SkyBossMonSheild = 0;
				GCStateInfoSend(lpObj,0,eVS_INMUNE_TO_MAGIC);
				GCMagicCancelSend(lpObj,16);
			}
			else
			{
				lpObj->m_SkyBossMonSheild = 1;
				GCStateInfoSend(lpObj,1,eVS_INMUNE_TO_MAGIC);
				GCMagicAttackNumberSend(lpObj,16,lpObj->m_Index,1);
			}
		}
	}

	if(lpObj->m_iSkillNPCHelpTime > 0)
	{
		// VERIFICAR
		if(GetTickCount() > (DWORD)(lpObj->m_iSkillNPCHelpTime + 1800000))
		{
			lpObj->m_iSkillNPCHelpTime = 0;
			lpObj->m_iSkillNPCDefense = 0;
			lpObj->m_iSkillNPCAttack = 0;

			GCStateInfoSend(lpObj,0,eVS_NPC_HELP);
		}
	}

	/*if(lpObj->SkillRecallParty_Time != 0 && lpObj->Type == OBJ_USER)
	{
		lpObj->SkillRecallParty_Time--;

		if(lpObj->SkillRecallParty_Time <= 0)
		{
			lpObj->SkillRecallParty_Time = 0;

			//int m_check1 = gMoveCommand.CheckMainToMove(lpObj);
			//int m_check2 = gMoveCommand.CheckEquipmentToMove(lpObj,lpObj->SkillRecallParty_MapNumber);
			//int m_check3 = gMoveCommand.CheckInterfaceToMove(lpObj);

			if(m_check1 == false
				|| m_check2 == false
				|| m_check3 == false)
			{
				char szMsg[256];
				wsprintf(szMsg,lMsg.Get(1254));

				GCServerMsgStringSend(szMsg,lpObj->m_Index,1);
			}
			else
			{
				gObjRecall(lpObj->m_Index,lpObj->SkillRecallParty_MapNumber,lpObj->SkillRecallParty_X,lpObj->SkillRecallParty_Y);
			//}
		}
		else if(lpObj->SkillRecallParty_Time <= 5)
		{
			char szMsg[256];
			wsprintf(szMsg,lMsg.Get(1243),lpObj->SkillRecallParty_Time);

			GCServerMsgStringSend(szMsg,lpObj->m_Index,1);
		}
	}*/



	TMonsterSkillElementInfo::CheckSkillElementInfoProc(lpObj);

	if(lpObj->m_iSkillStunTime != 0 && lpObj->Type == OBJ_USER)
	{
		lpObj->m_iSkillStunTime--;

		if(lpObj->m_iSkillStunTime <= 0)
		{
			lpObj->m_iSkillStunTime = 0;
			MsgOutput(lpObj->m_Index,lMsg.Get(1636));
			GCStateInfoSend(lpObj,0,eVS_STUN);
		}
	}

	if(lpObj->m_iSkillBrandOfSkillTime != 0 && lpObj->Type == OBJ_USER)
	{
		lpObj->m_iSkillBrandOfSkillTime--;

		if(lpObj->m_iSkillBrandOfSkillTime <= 0)
		{
			lpObj->m_iSkillBrandOfSkillTime = 0;
			MsgOutput(lpObj->m_Index,lMsg.Get(1637));
			GCStateInfoSend(lpObj,0,eVS_SACRIFICE);
		}
	}

	if(lpObj->m_iSkillInvisibleTime != 0 && lpObj->Type == OBJ_USER)
	{
		lpObj->m_iSkillInvisibleTime--;

		if(lpObj->m_iSkillInvisibleTime <= 0)
		{
			lpObj->m_iSkillInvisibleTime = 0;
			MsgOutput(lpObj->m_Index,lMsg.Get(1638));
			GCStateInfoSend(lpObj,0,eVS_TRANSPARENCY);
		}
	}

	if(lpObj->m_iSkillManaSwellTime != 0 && lpObj->Type == OBJ_USER)
	{
		lpObj->m_iSkillManaSwellTime--;

		if(lpObj->m_iSkillManaSwellTime <= 0)
		{
			lpObj->AddMana -= lpObj->m_iSkillManaSwell;

			if(lpObj->AddMana < 0)
			{
				lpObj->AddMana = 0;
			}

			GCManaSend(lpObj->m_Index,lpObj->MaxMana+lpObj->AddMana,0xFE,0,lpObj->MaxBP+lpObj->AddBP);

			if(lpObj->Mana > lpObj->MaxMana + lpObj->AddMana)
			{
				lpObj->Mana = lpObj->MaxMana + lpObj->AddMana;
				GCManaSend(lpObj->m_Index,lpObj->Mana,0xFF,0,lpObj->BP);
			}

			lpObj->m_iSkillManaSwellTime = 0;
			lpObj->m_iSkillManaSwell = 0;
			MsgOutput(lpObj->m_Index,lMsg.Get(1639));
			GCStateInfoSend(lpObj,0,eVS_SWELL_MANA);
		}
	}

	// ---- Siege Bless Potion
	if(lpObj->m_iPotionBlessTime != 0 && lpObj->Type == OBJ_USER)
	{
		lpObj->m_iPotionBlessTime--;

		if(lpObj->m_iPotionBlessTime <= 0)
		{
			lpObj->m_iPotionBlessTime = 0;
			GCStateInfoSend(lpObj,0,eVS_POTION_BLESS);
		}
	}

	// ---- Siege Soul Potion
	if(lpObj->m_iPotionSoulTime != 0 && lpObj->Type == OBJ_USER)
	{
		lpObj->m_iPotionSoulTime--;

		if(lpObj->m_iPotionSoulTime <= 0)
		{
			lpObj->m_iPotionSoulTime = 0;
			GCStateInfoSend(lpObj,0,eVS_POTION_SOUL);
		}
	}

	if(lpObj->Type == OBJ_USER && lpObj->m_iItemEffectValidTime > 0)
	{
		lpObj->m_iItemEffectValidTime--;

		if(lpObj->m_iItemEffectValidTime <= 0)
		{
			lpObj->m_iItemEffectValidTime = 0;

			g_ItemAddOption.ClearItemEffect(lpObj,1);

			GCReFillSend(lpObj->m_Index,lpObj->MaxLife+lpObj->AddLife,0xFE,0,lpObj->iMaxShield+lpObj->iAddShield);
			GCManaSend(lpObj->m_Index,lpObj->MaxMana+lpObj->AddMana,0xFE,0,lpObj->MaxBP);
		}
	}

	if(lpObj->m_SkillNumber < 1)
	{
		return;
	}

	if(lpObj->m_SkillNumber == 18)
	{
		if(GetTickCount() > lpObj->m_SkillTime)
		{
			lpObj->m_SkillNumber = 0;
		}
	}

	if(lpObj->m_iMuseElfInfinityArrowSkillTime != 0 && lpObj->Type == OBJ_USER)
	{
		lpObj->m_iMuseElfInfinityArrowSkillTime--;

		if(lpObj->m_iMuseElfInfinityArrowSkillTime <= 0)
		{
			lpObj->m_iMuseElfInfinityArrowSkillTime = 0;
			GCStateInfoSend(lpObj,0,eVS_INFINITY_ARROW);
		}
	}
}
// Review Skill Functions
void gObjSkillBeAttackProc(LPOBJ lpObj)
{
	int ad,iShieldDamage = 0;

	if(lpObj->m_PoisonBeattackCount > 0)
	{
		lpObj->m_PoisonBeattackCount--;

		if(lpObj->m_PoisonBeattackCount % 3 == 0)
		{
			if(lpObj->Life > 0.0f)
			{
				if(lpObj->m_PoisonType == 38)
				{
					ad = lpObj->Life * 3 / 100;
				}
				else
				{
					ad = lpObj->Life * 3 / 100;
				}
			}
			else
			{
				ad = 1;
			}

			if(ad < 0)
			{
				ad = 0;
			}

			if(g_ShieldSystemOn == 1)
			{
				if(lpObj->Type == OBJ_USER && lpObj->lpAttackObj->Type == OBJ_USER)
				{
					if(lpObj->iShield > ad)
					{
						lpObj->iShield -= ad;
						iShieldDamage = ad;
						ad = 0;
					}
					else
					{
						ad -= lpObj->iShield;
						iShieldDamage = lpObj->iShield;
						lpObj->iShield = 0;

						lpObj->Life -= ad;

						if(lpObj->Life < 0)
						{
							lpObj->Life = 0;
						}
					}
				}
				else
				{
					lpObj->Life -= ad;

					if(lpObj->Life < 0)
					{
						lpObj->Life = 0;
					}
				}
			}
			else
			{
				lpObj->Life -= ad;

				if(lpObj->Life < 0)
				{
					lpObj->Life = 0;
				}
			}

			if(lpObj->lpAttackObj != 0)
			{
				gObjLifeCheck(lpObj,lpObj->lpAttackObj,ad,2,0,0,1,iShieldDamage);
			}

			if(lpObj->m_PoisonBeattackCount <= 0)
			{
				lpObj->lpAttackObj = 0;
				lpObj->m_ViewSkillState[eVS_POISON] = 0;
				lpObj->m_ViewState = 0;

				if(lpObj->m_PoisonType == 38)
				{
					GCStateInfoSend(lpObj,0,eVS_POISON);
				}
				else
				{
					GCMagicCancelSend(lpObj,1);
				}

				lpObj->m_PoisonType = 0;
			}
		}
	}
	if(lpObj->m_ColdBeattackCount > 0)
	{
		lpObj->m_ColdBeattackCount--;

		if(lpObj->m_ColdBeattackCount <= 0)
		{
			lpObj->lpAttackObj = 0;
			lpObj->DelayActionTime = 0;
			lpObj->DelayLevel = 0;
			lpObj->m_ViewSkillState[eVS_ICE] = 0;
			lpObj->m_ViewState = 0;

			if(lpObj->m_IceType == 39)
			{
				GCStateInfoSend(lpObj,0,eVS_ICE);
			}
			else
			{
				GCMagicCancelSend(lpObj,7);
			}

			lpObj->m_IceType = 0;
		}
	}
	if(lpObj->m_ImmuneToMagicCount > 0)
	{
		lpObj->m_ImmuneToMagicCount--;

		if(lpObj->m_ImmuneToMagicCount <= 0)
		{
			GCStateInfoSend(lpObj,1,eVS_INMUNE_TO_MAGIC);
		}
	}
	if(lpObj->m_ImmuneToHarmCount > 0)
	{
		lpObj->m_ImmuneToHarmCount--;

		if(lpObj->m_ImmuneToHarmCount <= 0)
		{
			GCStateInfoSend(lpObj,0,eVS_DEFENSE_AURA);
		}
	}
	if (lpObj->m_SkillSleepTime > 0)
	{
		lpObj->m_SkillSleepTime--;

		if (lpObj->m_SkillSleepTime <= 0)
		{
			GCStateInfoSend(lpObj,0,eVS_SKILL_SLEEP);
		}
	}
}

// --------------------------------------------------
// [Sunday, 20 Feb, 2011] WolF ~ Teleport Magic Use
// --------------------------------------------------
void gObjTeleportMagicUse(int aIndex, BYTE x, BYTE y)
{
	LPOBJ lpObj = &gObj[aIndex];
	// ------
	if(lpObj->Teleport != 0) return;
	// ------
	lpObj->TeleportTime = GetTickCount();
	lpObj->PathCount	= 0;
	lpObj->Teleport		= 1;
	lpObj->m_StateSub	= 1;
	lpObj->m_ViewState	= 1;
	// ------
	lpObj->X			= x;
	lpObj->Y			= y;
	lpObj->TX			= x;
	lpObj->TY			= y;
	// ------
	MapC[lpObj->MapNumber].ClearStandAttr(lpObj->m_OldX,lpObj->m_OldY);
	MapC[lpObj->MapNumber].SetStandAttr(lpObj->TX,lpObj->TY);
	// ------
	lpObj->m_OldX = lpObj->TX;
	lpObj->m_OldY = lpObj->TY;
	// ------
	//gObjViewportListProtocolDestroy(lpObj);
}

BOOL gObjMoveGate(int aIndex, int gt)
{
	LPOBJ lpObj = &gObj[aIndex];
	int result;
	BYTE x;
	BYTE y;
	BYTE mapNumber = 0,dir;
	short level;
	int userlevel = gObj[aIndex].Level;
	int movefail = 0;
	
	/*if(gObj[aIndex].SkillRecallParty_Time > 0)
	{
		char szMsg[255];
		gObj[aIndex].SkillRecallParty_Time = 0;
		wsprintf(szMsg,lMsg.Get(1254));
		GCServerMsgStringSend(szMsg,aIndex,1);
	}*/
	
	if( lpObj->Type == OBJ_USER && lpObj->Class == CLASS_DARKLORD )
	{
		gDarkSpirit[lpObj->m_Index].SetMode(CDarkSpirit::PetItem_Mode_Normal,-1);
	}
	
	if( gUser.BloodCastleMapRange(lpObj->MapNumber) == true )
	{
		if(g_BloodCastle.GetCurrentState(g_BloodCastle.GetBloodCastleIndex(lpObj->MapNumber)) == BC_STATE_PLAYING)
		{
			g_BloodCastle.SearchUserDropQuestItem(lpObj->m_Index);
		}
		else
		{
			g_BloodCastle.SearchUserDeleteQuestItem(lpObj->m_Index);
		}
	}
	
	if ( gObj[aIndex].RegenOk != 0 )
	{
		CLog.LogAdd("[gObjMoveGate]: Regen OK %s %d",__FILE__,__LINE__);
		x = lpObj->X;
		y = lpObj->Y;
		mapNumber = lpObj->MapNumber;
		dir = lpObj->Dir;
		movefail = 1;
		
		lpObj->RegenMapNumber = lpObj->MapNumber;
		lpObj->RegenMapX = x;
		lpObj->RegenMapY = y;
		
		gObjClearViewport(&gObj[aIndex]);
		
		GCTeleportSend(&gObj[aIndex],gt,mapNumber,gObj[aIndex].X,gObj[aIndex].Y,gObj[aIndex].Dir);
		
		if(lpObj->m_Change >= 0)
		{
			gObjViewportListProtocolCreate(&gObj[aIndex]);
		}
		
		gObj[aIndex].RegenOk = 1;
		
		return false;
	}
	
	
	if(GateC.IsGate(gt) == false)
	{
		
		CLog.LogAdd(lMsg.Get(545),gt,__FILE__,__LINE__);
		
		x = lpObj->X;
		y = lpObj->Y;
		mapNumber = lpObj->MapNumber;
		dir = lpObj->Dir;
		movefail = 1;
		
		lpObj->RegenMapNumber = lpObj->MapNumber;
		lpObj->RegenMapX = x;
		lpObj->RegenMapY = y;
		
		gObjClearViewport(&gObj[aIndex]);
		
		GCTeleportSend(&gObj[aIndex],gt,mapNumber,gObj[aIndex].X,gObj[aIndex].Y,gObj[aIndex].Dir);
		
		if(lpObj->m_Change >= 0)
		{
			gObjViewportListProtocolCreate(&gObj[aIndex]);
		}
		
		gObj[aIndex].RegenOk = 1;
		
		return false;
	}
	
	if(gt >= 58 && gt <= 61)
	{
		if(gObj[aIndex].m_bDevilSquareAuth == 0)
		{
			CLog.LogAdd("[DevilSquare] [%s][%s] Not Used DevilSquare Invitation In DevilSquare",gObj[aIndex].AccountID,gObj[aIndex].Name);
			return false;
		}
		else
		{
			gObj[aIndex].m_bDevilSquareAuth = 0;
		}
	}
	
	lpObj->m_MoveGateNumber = gt;
	
	result = GateC.GetGate(gt,(BYTE &)x,(BYTE &)y,(BYTE &)mapNumber,(BYTE &)dir,(short &)level);
	
	if(result < 0)
	{
		CLog.LogAdd("error : %d (%s %d)",result,__FILE__,__LINE__);
		
		x = lpObj->X;
		y = lpObj->Y;
		mapNumber = lpObj->MapNumber;
		dir = lpObj->Dir;
		
		movefail = 1;
	}
	
	if(lpObj->m_bPShopOpen == 1)
	{
		int bMove = 1;
		
		if(lpObj->MapNumber == MAP_INDEX_CASTLESIEGE && mapNumber == MAP_INDEX_LORENCIA)
		{
			bMove = 0;
		}
		
		if(lpObj->MapNumber == MAP_INDEX_LORENCIA && mapNumber == MAP_INDEX_CASTLESIEGE)
		{
			bMove = 0;
		}
		
		if(bMove == 0)
		{
			CLog.LogAdd("[PSHOP ITEM CLONNING] Move Failed : %d (%s %s)",result,lpObj->AccountID,lpObj->Name);
			
			x = lpObj->X;
			y = lpObj->Y;
			mapNumber = lpObj->MapNumber;
			dir = lpObj->Dir;
			movefail = 1;
			
			lpObj->RegenMapX = x;
			lpObj->RegenMapY = y;
			
			lpObj->RegenMapNumber = mapNumber;
			lpObj->RegenMapNumber = lpObj->MapNumber;
			
			lpObj->RegenMapX = x;
			lpObj->RegenMapY = y;
			
			gObjClearViewport(&gObj[aIndex]);
			
			GCTeleportSend(&gObj[aIndex],gt,mapNumber,gObj[aIndex].X,gObj[aIndex].Y,gObj[aIndex].Dir);
			
			if(lpObj->m_Change >= 0)
			{
				gObjViewportListProtocolCreate(&gObj[aIndex]);
			}
			
			gObj[aIndex].RegenOk = 1;
			return false;
		}
	}
	
	if( gUser.BloodCastleMapRange(mapNumber) == true ||  gUser.ChaosCastleMapRange(mapNumber) == true || DS_MAP_RANGE(mapNumber) )
	{
		if(lpObj->m_iSkillNPCHelpTime > 0)
		{
			lpObj->m_iSkillNPCHelpTime = 0;
			lpObj->m_iSkillNPCDefense = 0;
			lpObj->m_iSkillNPCAttack = 0;
			
			lpObj->m_ViewSkillState [eVS_NPC_HELP] = 0;
			GCStateInfoSend(lpObj,0,eVS_NPC_HELP);
		}
	}

#if(GS_CASTLE)
	if(result == 97)
	{
		if(g_CastleSiege.GetCastleState() == 7)//Remover n�meros m�gicos
		{
			if(lpObj->m_btCsJoinSide != 1)
			{
				if(g_CastleSiege.GetCastleTowerAccessable() == FALSE)
				{
					::GCServerMsgStringSend(lMsg.Get(1624),lpObj->m_Index,1);
					
					x = lpObj->X;
					y = lpObj->Y;
					mapNumber = lpObj->MapNumber;
					dir = lpObj->Dir;
					movefail = 1;
					lpObj->RegenMapNumber = lpObj->MapNumber;
					lpObj->RegenMapX = x;
					lpObj->RegenMapY = y;
					::gObjClearViewport(lpObj);
					::GCTeleportSend(&gObj[aIndex],gt,mapNumber,gObj[aIndex].X,gObj[aIndex].Y,gObj[aIndex].Dir);
					
					if(lpObj->m_Change >= 0)
					{
						gObjViewportListProtocolCreate(&gObj[aIndex]);
					}

					gObj[aIndex].RegenOk = 1;
					return false;
				}
			}//lpObj->m_btCsJoinSide != 1
			else if(lpObj->m_btCsJoinSide == 0)
			{
				::GCServerMsgStringSend(lMsg.Get(1625),lpObj->m_Index,1);

				x = lpObj->X;
				y = lpObj->Y;
				mapNumber = lpObj->MapNumber;
				dir = lpObj->Dir;
				movefail = 1;
				lpObj->RegenMapNumber = lpObj->MapNumber;
				lpObj->RegenMapX = x;
				lpObj->RegenMapY = y;
				::gObjClearViewport(lpObj);
				::GCTeleportSend(&gObj[aIndex],gt,mapNumber,gObj[aIndex].X,gObj[aIndex].Y,gObj[aIndex].Dir);
				
				if(lpObj->m_Change >= 0)
				{
					gObjViewportListProtocolCreate(&gObj[aIndex]);
				}

				gObj[aIndex].RegenOk = 1;
				return false;
			}
			else
			{
				if( g_CastleSiege.CheckCastleOwnerUnionMember(lpObj->m_Index) == FALSE )
				{
					::GCServerMsgStringSend(lMsg.Get(1626),lpObj->m_Index,1);

					x = lpObj->X;
					y = lpObj->Y;
					mapNumber = lpObj->MapNumber;
					dir = lpObj->Dir;
					movefail = 1;
					lpObj->RegenMapNumber = lpObj->MapNumber;
					lpObj->RegenMapX = x;
					lpObj->RegenMapY = y;
					::gObjClearViewport(lpObj);
					::GCTeleportSend(&gObj[aIndex],gt,mapNumber,gObj[aIndex].X,gObj[aIndex].Y,gObj[aIndex].Dir);
					
					if(lpObj->m_Change >= 0)
					{
						gObjViewportListProtocolCreate(&gObj[aIndex]);
					}

					gObj[aIndex].RegenOk = 1;
					return false;					
				}
			}
		}
	}
#endif
	
	if(mapNumber == MAP_INDEX_ATLANS)
	{
		if(lpObj->pInventory[8].IsItem())
		{
			if(lpObj->pInventory[8].m_Type == ITEMGET(13,2))
			{
				GCServerMsgStringSend(lMsg.Get(702),lpObj->m_Index,1);
				
				x = lpObj->X;
				y = lpObj->Y;
				mapNumber = lpObj->MapNumber;
				dir = lpObj->Dir;
				movefail = 1;
				
				lpObj->RegenMapNumber = lpObj->MapNumber;
				lpObj->RegenMapX = x;
				lpObj->RegenMapY = y;
				gObjClearViewport(&gObj[aIndex]);
				GCTeleportSend(&gObj[aIndex],gt,mapNumber,gObj[aIndex].X,gObj[aIndex].Y,gObj[aIndex].Dir);
				if(lpObj->m_Change >= 0)
				{
					gObjViewportListProtocolCreate(&gObj[aIndex]);
				}
				gObj[aIndex].RegenOk = 1;
				return false;
			}
			
			if(lpObj->pInventory[8].m_Type == ITEMGET(13,3))
			{
				GCServerMsgStringSend(lMsg.Get(1604),lpObj->m_Index,1);
				
				x = lpObj->X;
				y = lpObj->Y;
				mapNumber = lpObj->MapNumber;
				dir = lpObj->Dir;
				movefail = 1;
				
				lpObj->RegenMapNumber = lpObj->MapNumber;
				lpObj->RegenMapX = x;
				lpObj->RegenMapY = y;
				gObjClearViewport(&gObj[aIndex]);
				GCTeleportSend(&gObj[aIndex],gt,mapNumber,gObj[aIndex].X,gObj[aIndex].Y,gObj[aIndex].Dir);
				if(lpObj->m_Change >= 0)
				{
					gObjViewportListProtocolCreate(&gObj[aIndex]);
				}
				gObj[aIndex].RegenOk = 1;
				return false;
			}
		}
	}
	
	if(mapNumber == MAP_INDEX_ICARUS)
	{
		if(!(lpObj->pInventory[8].m_Type == ITEMGET(13,3) ||
			 lpObj->pInventory[7].m_Type == ITEMGET(13,30)|| 
			 lpObj->pInventory[8].m_Type == ITEMGET(13,37)|| 
			 (lpObj->pInventory[7].m_Type >= ITEMGET(12,0)&& lpObj->pInventory[7].m_Type <=ITEMGET(12,6)) || 
			 (lpObj->pInventory[7].m_Type >= ITEMGET(12,36)&& lpObj->pInventory[7].m_Type <= ITEMGET(12,43)) ||
			 lpObj->pInventory[8].m_Type == ITEMGET(12,49) ||
			 lpObj->pInventory[8].m_Type == ITEMGET(12,50) ||
			 lpObj->pInventory[8].m_Type == ITEMGET(12,130) ||
			 lpObj->pInventory[8].m_Type == ITEMGET(12,131) ||
			 lpObj->pInventory[8].m_Type == ITEMGET(12,132) ||
			 lpObj->pInventory[8].m_Type == ITEMGET(12,133) ||
			 lpObj->pInventory[8].m_Type == ITEMGET(12,134) ||
			 lpObj->pInventory[8].m_Type == ITEMGET(12,135)
			 ) 
			|| lpObj->pInventory[8].m_Type == ITEMGET(13,2)
			|| lpObj->pInventory[11].m_Type == ITEMGET(13,10)
			|| lpObj->pInventory[10].m_Type == ITEMGET(13,10)
			|| lpObj->pInventory[10].m_Type == ITEMGET(13,39)
			|| lpObj->pInventory[11].m_Type == ITEMGET(13,39)
			|| lpObj->pInventory[10].m_Type == ITEMGET(13,40)
			|| lpObj->pInventory[11].m_Type == ITEMGET(13,40)
			|| lpObj->pInventory[10].m_Type == ITEMGET(13,41)
			|| lpObj->pInventory[11].m_Type == ITEMGET(13,41)
			|| lpObj->pInventory[10].m_Type == ITEMGET(13,42)
			|| lpObj->pInventory[11].m_Type == ITEMGET(13,42))
		{
			GCServerMsgStringSend(lMsg.Get(1604),lpObj->m_Index,1);
			
			x = lpObj->X;
			y = lpObj->Y;
			mapNumber = lpObj->MapNumber;
			dir = lpObj->Dir;
			movefail = 1;
			
			lpObj->RegenMapNumber = lpObj->MapNumber;
			lpObj->RegenMapX = x;
			lpObj->RegenMapY = y;
			
			GCTeleportSend(&gObj[aIndex],gt,mapNumber,gObj[aIndex].X,gObj[aIndex].Y,gObj[aIndex].Dir);
			gObjClearViewportOfMine(lpObj);
			
			if(lpObj->m_Change >= 0)
			{
				gObjViewportListProtocolCreate(&gObj[aIndex]);
			}
			gObj[aIndex].RegenOk = 1;
			return false;
		}
		
		if(lpObj->m_RecallMon >= 0)
		{
			GCRecallMonLife(aIndex,60,0);
			gObjMonsterCallKill(aIndex);
		}
	}
	
	if(!KALIMA_MAP_RANGE(mapNumber) && KALIMA_MAP_RANGE(lpObj->MapNumber))
	{
		int iKalimaGateIndex = lpObj->m_iKalimaGateIndex;
		
		if(g_KalimaGate.DeleteKalimaGate(lpObj->m_Index))
		{
			CLog.LogAdd("[Kalima] [%s][%s] Kalima Gate Vanished - User MoveGate (SummonIndex:%d, EnterCount:%d)",lpObj->AccountID,lpObj->Name,iKalimaGateIndex,gObj[iKalimaGateIndex].m_cKalimaGateEnterCount);
		}
	}

#if(!GS_CASTLE)
	if(mapNumber == MAP_INDEX_KANTURU_BOSS)
	{
		if(!(lpObj->pInventory[8].m_Type == ITEMGET(13,3) ||
			 lpObj->pInventory[7].m_Type == ITEMGET(13,30)|| 
			 lpObj->pInventory[8].m_Type == ITEMGET(13,37)|| 
			 (lpObj->pInventory[7].m_Type >= ITEMGET(12,0)&& lpObj->pInventory[7].m_Type <=ITEMGET(12,6)) || 
			 (lpObj->pInventory[7].m_Type >= ITEMGET(12,36)	&& lpObj->pInventory[7].m_Type <=ITEMGET(12,43)) ||
			  lpObj->pInventory[8].m_Type == ITEMGET(12,49) ||
			  lpObj->pInventory[8].m_Type == ITEMGET(12,50) ||
			  lpObj->pInventory[8].m_Type == ITEMGET(12,130) ||
			  lpObj->pInventory[8].m_Type == ITEMGET(12,131) ||
			  lpObj->pInventory[8].m_Type == ITEMGET(12,132) ||
			  lpObj->pInventory[8].m_Type == ITEMGET(12,133) ||
			  lpObj->pInventory[8].m_Type == ITEMGET(12,134) ||
			  lpObj->pInventory[8].m_Type == ITEMGET(12,135)
			 )
			|| lpObj->pInventory[11].m_Type == ITEMGET(13,10)
			|| lpObj->pInventory[10].m_Type == ITEMGET(13,10)
			|| lpObj->pInventory[11].m_Type == ITEMGET(13,39)
			|| lpObj->pInventory[10].m_Type == ITEMGET(13,39)
			|| lpObj->pInventory[10].m_Type == ITEMGET(13,40)
			|| lpObj->pInventory[11].m_Type == ITEMGET(13,40)
			|| lpObj->pInventory[10].m_Type == ITEMGET(13,41)
			|| lpObj->pInventory[11].m_Type == ITEMGET(13,41)
			|| lpObj->pInventory[10].m_Type == ITEMGET(13,42)
			|| lpObj->pInventory[11].m_Type == ITEMGET(13,42))
		{
			GCServerMsgStringSend(lMsg.Get(1604),lpObj->m_Index,1);
			
			x = lpObj->X;
			y = lpObj->Y;
			mapNumber = lpObj->MapNumber;
			dir = lpObj->Dir;
			movefail = 1;
			
			lpObj->RegenMapNumber = lpObj->MapNumber;
			lpObj->RegenMapX = x;
			lpObj->RegenMapY = y;
			
			GCTeleportSend(&gObj[aIndex],gt,mapNumber,gObj[aIndex].X,gObj[aIndex].Y,gObj[aIndex].Dir);
			gObjClearViewportOfMine(lpObj);
			
			if(lpObj->m_Change >= 0)
			{
				gObjViewportListProtocolCreate(&gObj[aIndex]);
			}
			gObj[aIndex].RegenOk = 1;
			return false;
		}
	}
#endif

	if( g_Kanturu.BlockKanturuMapEnter == 1 )
	{
		x = lpObj->X;
		y = lpObj->Y;
		mapNumber = lpObj->MapNumber;
		dir = lpObj->Dir;
		level = lpObj->Level;
		
		if(GateC.GetGate(gt,(BYTE &)x,(BYTE &)y,(BYTE &)mapNumber,(BYTE &)dir,(short &)level) != -1)
		{
			if(mapNumber >= MAP_INDEX_KANTURU1 && mapNumber <= MAP_INDEX_KANTURU_BOSS)
			{
				GCServerMsgStringSend(lMsg.Get(1604),lpObj->m_Index,1);
				x = lpObj->X;
				y = lpObj->Y;
				mapNumber = lpObj->MapNumber;
				dir = lpObj->Dir;
				movefail = 1;
				
				lpObj->RegenMapNumber = lpObj->MapNumber;
				lpObj->RegenMapX = x;
				lpObj->RegenMapY = y;
				
				GCTeleportSend(&gObj[aIndex],gt,mapNumber,gObj[aIndex].X,gObj[aIndex].Y,gObj[aIndex].Dir);
				gObjClearViewportOfMine(lpObj);
				
				if(lpObj->m_Change >= 0)
				{
					gObjViewportListProtocolCreate(&gObj[aIndex]);
				}
				gObj[aIndex].RegenOk = 1;
				return false;
			}
		}
	}
	
	if(g_iBlockCastleSiegeMapEnter == 1)
	{
		x = lpObj->X;
		y = lpObj->Y;
		mapNumber = lpObj->MapNumber;
		dir = lpObj->Dir;
		level = lpObj->Level;
		
		if(GateC.GetGate(gt,(BYTE &)x,(BYTE &)y,(BYTE &)mapNumber,(BYTE &)dir,(short &)level) != -1)
		{
			if(mapNumber == MAP_INDEX_CASTLESIEGE
				|| mapNumber == MAP_INDEX_CASTLEHUNTZONE
				|| mapNumber == MAP_INDEX_CRYWOLF_FIRSTZONE
				|| mapNumber == MAP_INDEX_CRYWOLF_SECONDZONE)
			{
				GCServerMsgStringSend(lMsg.Get(1604),lpObj->m_Index,1);
				x = lpObj->X;
				y = lpObj->Y;
				mapNumber = lpObj->MapNumber;
				dir = lpObj->Dir;
				movefail = 1;
				
				lpObj->RegenMapNumber = lpObj->MapNumber;
				lpObj->RegenMapX = x;
				lpObj->RegenMapY = y;
				
				GCTeleportSend(&gObj[aIndex],gt,mapNumber,gObj[aIndex].X,gObj[aIndex].Y,gObj[aIndex].Dir);
				gObjClearViewportOfMine(lpObj);
				
				if(lpObj->m_Change >= 0)
				{
					gObjViewportListProtocolCreate(&gObj[aIndex]);
				}
				gObj[aIndex].RegenOk = 1;
				return false;
			}
		}
	}
	
	if(lpObj->Class == CLASS_DARKLORD || lpObj->Class == CLASS_MAGUMSA || lpObj->Class == CLASS_FIGHTER)
	{
		if(level > 0)
		{
			level = (level / 3)*2;
		}
	}
	
	if(level != 0)
	{
		if(userlevel < level)
		{
			char szTemp[256];
			wsprintf(szTemp,lMsg.Get(1117),level);
			GCServerMsgStringSend(szTemp,aIndex,1);
			
			x = lpObj->X;
			y = lpObj->Y;
			mapNumber = lpObj->MapNumber;
			dir = lpObj->Dir;
			movefail = 1;
		}
	}
	
	if(gObj[aIndex].m_IfState.use != 0)
	{
		x = (BYTE)lpObj->X;
		y = (BYTE)lpObj->Y;
		mapNumber = lpObj->MapNumber;
		dir = lpObj->Dir;
		movefail = 1;
	}
	
	if(gObj[aIndex].DieRegen != 0)
	{
		x = (BYTE)lpObj->X;
		y = (BYTE)lpObj->Y;
		mapNumber = lpObj->MapNumber;
		dir = lpObj->Dir;
		movefail = 1;
	}
	
	lpObj->m_State = 32;
	
	if(movefail == 0)
	{
		if(GateC.GetGate(gt,(BYTE &)x,(BYTE &)y,(BYTE &)mapNumber,(BYTE &)dir,(short &)level) != -1)
		{
			if(gObj[aIndex].MapNumber != mapNumber)
			{
				if(MapNumberCheck(mapNumber)== 0)
				{
					CLog.LogAdd("error-L3 : %s %d",__FILE__,__LINE__);
					return false;
				}
				
				short sSvrCode = g_MapServerManager.CheckMoveMapSvr(lpObj->m_Index,mapNumber,lpObj->m_sPrevMapSvrCode);
				
				if(sSvrCode != gGameServerCode)
				{
					if(sSvrCode == -1)
					{
						CLog.LogAddC(2,"[MapServerMng] Map Server Move Fail : CheckMoveMapSvr() == -1 [%s][%s] (%d)",lpObj->AccountID,lpObj->Name,lpObj->m_Index);
						return false;
					}
					
					if(GateC.CheckGateLevel(aIndex,gt) == 0)
					{
						x = (BYTE)lpObj->X;
						y = (BYTE)lpObj->Y;
						mapNumber = lpObj->MapNumber;
						dir = lpObj->Dir;
						movefail = 1;
						
						lpObj->RegenMapNumber = lpObj->MapNumber;
						
						lpObj->RegenMapX = x;
						lpObj->RegenMapY = y;
						
						gObjClearViewport(&gObj[aIndex]);
						
						GCTeleportSend(&gObj[aIndex],gt,mapNumber,(BYTE)gObj[aIndex].X,(BYTE)gObj[aIndex].Y,gObj[aIndex].Dir);
						
						if(lpObj->m_Change >= 0)
						{
							gObjViewportListProtocolCreate(&gObj[aIndex]);
						}
						
						gObj[aIndex].RegenOk = 1;
#if(GS_CASTLE)
						if(gObj[aIndex].MapNumber == MAP_INDEX_CASTLESIEGE)
						{
							if(g_CastleSiege.GetCastleState() == 7)
							{
								g_CastleSiege.NotifySelfCsJoinSide(aIndex);
								g_CastleSiege.NotifyCsSelfLeftTime(aIndex);
							}
						}
#endif
						return false;
					}
					
					
					GJReqMapSvrMove(lpObj->m_Index,sSvrCode,mapNumber,x,y);
					CLog.LogAdd("[MapServerMng] Request to Move Map Server : (%d) - [%s][%s] (%d)",sSvrCode,lpObj->AccountID,lpObj->Name,lpObj->m_Index);
					return false;
				}
			}
		}
		
		CLog.LogAdd("[MoveMap][%s][%s] : (Before:Map:%d X:%d Y:%d) -> (New Map:%d X:%d Y:%d)",lpObj->AccountID,lpObj->Name,lpObj->MapNumber,lpObj->X,lpObj->Y,mapNumber,x,y);
		
		gObj[aIndex].X = x;
		gObj[aIndex].Y = y;
		gObj[aIndex].TX = x;
		gObj[aIndex].TY = y;
		gObj[aIndex].MapNumber = mapNumber;
		gObj[aIndex].Dir = dir;
		gObj[aIndex].PathCount = 0;
		gObj[aIndex].Teleport = 0;

		if ( gImperialFort.Enabled == 1 )
		{
			if ( gImperialFort.Running == 1 )
			{
				if ( mapNumber == gImperialFort.EventMap )
				{
					if ( gImperialFort.StandByRun == 0 )
					{
						gImperialFort.StandByRun = 1;
					}
				}
			}
		}
		
		gObjClearViewport(&gObj[aIndex]);
		GCTeleportSend(&gObj[aIndex],gt,mapNumber,x,y,dir);
		
		if(lpObj->m_Change >= 0)
		{
			gObjViewportListProtocolCreate(&gObj[aIndex]);
		}
		
		gObj[aIndex].RegenMapNumber = mapNumber;
		gObj[aIndex].RegenMapX = x;
		gObj[aIndex].RegenMapY = y;
		gObj[aIndex].RegenOk = 1;
		
	}
	else
	{
		GCTeleportSend(&gObj[aIndex],gt,mapNumber,(BYTE)gObj[aIndex].X,(BYTE)gObj[aIndex].Y,gObj[aIndex].Dir);
		gObj[aIndex].RegenOk = 1;

	#if(GS_CASTLE)
		if(gObj[aIndex].MapNumber == MAP_INDEX_CASTLESIEGE)
		{
			if(g_CastleSiege.GetCastleState() == 7)
			{
				g_CastleSiege.NotifySelfCsJoinSide(aIndex);
				g_CastleSiege.NotifyCsSelfLeftTime(aIndex);
			}
		}
#endif
	}
	
	if(movefail != 0)
	{
		return false;
	}
	
	if(gObj[aIndex].Type == OBJ_USER)
	{
		gObj[aIndex].m_LastTeleportTime = 10;
	}
	
	return true;
}

class CMagicInf* gObjGetMagic(LPOBJ lpObj, int mIndex)
{
	if ( mIndex < 0 || mIndex > MAX_MAGIC-1 )
	{
		CLog.LogAdd(lMsg.Get(MSGGET(2, 34)), __FILE__, __LINE__);
		return NULL;
	}

	if ( lpObj->Magic[mIndex].IsMagic() == FALSE )
	{
		return NULL;
	}
	return &lpObj->Magic[mIndex];
}

CMagicInf * gObjGetMagicSearch(LPOBJ lpObj, WORD skillnumber)
{

	if(skillnumber == 58)
	{
		skillnumber = 40;
	}

	for(int n = 0; n < MAGIC_SIZE;n++)
	{
		if(lpObj->Magic[n].IsMagic() == 1)
		{
			if(lpObj->Magic[n].m_Skill == skillnumber)
			{
				if ( skillnumber < 300 )
				{
					if( gObjMagicEnergyCheck(lpObj,lpObj->Magic[n].m_Skill) == false )
					{
						GCServerMsgStringSend(lMsg.Get(1255),lpObj->m_Index,1);
						return false;
					}
				}
				else
				{
					if ( gSkillTree.MasterSkillEnergyCheck(lpObj,lpObj->Magic[n].m_Skill) == false )
					{
						return false;
					}
				}
				return &lpObj->Magic[n];
			}
		}
	}
	return false;
}

int	gObjMagicManaUse(LPOBJ lpObj, CMagicInf * lpMagic)
{
	float mana = lpObj->Mana;

	if(lpObj->Type != OBJ_USER)
	{
		return false;
	}

	if(lpMagic == 0)
	{
		return false;
	}
	mana -= MagicDamageC.SkillGetMana(lpMagic->m_Skill);

	if(mana < 0)
	{
		return -1;
	}

	return mana;
}

int gObjMagicBPUse(LPOBJ lpObj, CMagicInf * lpMagic)
{
	int bp = lpObj->BP;

	if(lpObj->Type != OBJ_USER)
	{
		return false;
	}

	if(lpMagic == 0)
	{
		return false;
	}

	bp -= MagicDamageC.SkillGetBP(lpMagic->m_Skill);

	if(bp < 0)
	{
		return -1;
	}

	return bp;
}

void gObjMagicAddEnergyCheckOnOff(int flag)
{
	g_EnergyCheckOff=flag;
}
// Reformulated Function
int gObjMagicAdd(LPOBJ lpObj, BYTE Type, BYTE Index, BYTE Level, WORD & SkillNumber)
{
	int skill = -1;
	int n;
	int reqeng;
	int reqleadership;

	skill = GetSkillNumberInex(Type,Index,Level);

	// Skill Number Check
	if(skill < 0)
	{
		CLog.LogAdd("[gObjMagicAdd]:[Account:%s][Character:%s] Invalid Skill Number: %d",lpObj->AccountID,lpObj->Name,skill);
		return -1;
	}
	// Check if have master skill upgraded
	if (gSkillTree.CheckMasterSkillCanAdd(lpObj,skill) == false )
	{
		CLog.LogAdd("[gObjMagicAdd]:[Account:%s][Character:%s] Already have upgraded skill [%d]",lpObj->AccountID,lpObj->Name,lpObj->Class,skill);
		return -1;
	}
	// Requiered Class Check
	if(MagicDamageC.SkillGetRequireClass(lpObj->m_Index,skill) < 1)
	{
		CLog.LogAdd("[gObjMagicAdd]:[Account:%s][Character:%s] Class [%d] not alowed to add this skill [%d]",lpObj->AccountID,lpObj->Name,skill);
		return -1;
	}
	
	// Energy Check
	if(g_EnergyCheckOff == 1)
	{
		reqeng = MagicDamageC.SkillGetRequireEnergy(skill);

		if(reqeng < 0)
		{
			CLog.LogAdd("[gObjMagicAdd]:[Account:%s][Character:%s] Invalid Skill[%d] Req Energy ",lpObj->AccountID,lpObj->Name,skill);
			return -1;
		}

		if((lpObj->Energy + lpObj->AddEnergy) < reqeng)
		{
			CLog.LogAdd("[gObjMagicAdd]:[Account:%s][Character:%s] Not enought energy to add skill %d",lpObj->AccountID,lpObj->Name,skill);
			return -1;
		}
	}
	
	// Requiered Leadership Check
	reqleadership = MagicDamageC.GetskillRequireLeadership(skill);
	if(reqleadership < 0)
	{
		CLog.LogAdd("[gObjMagicAdd]:[Account:%s][Character:%s] Invalid Skill[%d] Req Leadership ",lpObj->AccountID,lpObj->Name,skill);
		return -1;
	}

	if((lpObj->Leadership + lpObj->AddLeadership) < reqleadership)
	{
		CLog.LogAdd("[gObjMagicAdd]:[Account:%s][Character:%s] Not enought leadership to add skill %d",lpObj->AccountID,lpObj->Name,skill);
		return -1;
	}


	for(n = 0; n < MAGIC_SIZE; n++)
	{
		if(lpObj->Magic[n].IsMagic() == 1)
		{
			skill = GetSkillNumberInex(Type,Index,Level);

			if(skill < 0)
			{
				CLog.LogAdd("[gObjMagicAdd]:[Account:%s][Character:%s] Invalid Skill Number: %d",lpObj->AccountID,lpObj->Name,skill);
				return -1;
			}

			if(lpObj->Magic[n].m_Skill == skill)
			{
				CLog.LogAdd("[gObjMagicAdd]:[Account:%s][Character:%s] Skill %d is already in magic list",lpObj->AccountID,lpObj->Name,skill);
				return -1;
			}
		}
	}

	skill = -1;

	for(n = 0; n < MAGIC_SIZE; n++)
	{
		if(lpObj->Magic[n].IsMagic() == 0)
		{
			skill = lpObj->Magic[n].Set(Type,Index,Level);
			if(skill < 0)
			{
				CLog.LogAdd("[gObjMagicAdd]:[Account:%s][Character:%s] Invalid Skill Number: %d",lpObj->AccountID,lpObj->Name,skill);
				return -1;
			}
			else
			{
				CLog.LogAdd("[gObjMagicAdd]:[Account:%s][Character:%s] Skill %d added successfully",lpObj->AccountID,lpObj->Name,skill);
				lpObj->MagicCount++;
				SkillNumber = skill;
				return n;
			}
		}
	}

	CLog.LogAdd("[gObjMagicAdd]:[Account:%s][Character:%s] Can't add more skills",lpObj->AccountID,lpObj->Name);
	return -1;
}

int gObjMagicDel(LPOBJ lpObj, WORD aSkill, BYTE Level)
{
	for(int n = 0; n < MAGIC_SIZE; n++)
	{
		if(lpObj->Magic[n].IsMagic() == 1)
		{
			if(lpObj->Magic[n].m_Skill == aSkill)
			{
				lpObj->Magic[n].Clear();
				return n;
			}
		}
	}
	return -1;
}

int gObjMagicAdd(LPOBJ lpObj, WORD aSkill, BYTE Level)
{
	int skill;
	int n;
	int reqeng;

	skill = -1;

	if(g_EnergyCheckOff == 1)
	{
		reqeng = MagicDamageC.SkillGetRequireEnergy(aSkill);

		if(reqeng < 0)
		{
			CLog.LogAdd(lMsg.Get(549));
			return -1;
		}

		if((lpObj->Energy + lpObj->AddEnergy) < reqeng)
		{
			CLog.LogAdd(lMsg.Get(549));
			return -1;
		}
	}

	if(MagicDamageC.SkillGetRequireClass(lpObj->m_Index,aSkill) < 1)
	{
		CLog.LogAdd(lMsg.Get(550));
		return -1;
	}

	for(n = 0; n < MAGIC_SIZE; n++)
	{
		if(lpObj->Magic[n].IsMagic() == TRUE)
		{
			if(lpObj->Magic[n].m_Skill == aSkill)
			{
				/*CLog.LogAdd("Same skill already exists. %d %d",lpObj->Magic[n].m_Skill,aSkill);*/
				return -1;
			}
		}
	}

	for(n = 0; n < MAGIC_SIZE; n++)
	{
		if(lpObj->Magic[n].IsMagic() == 0)
		{
			skill = lpObj->Magic[n].Set(aSkill,Level);
			if(skill < 0)
			{
				CLog.LogAdd(lMsg.Get(553),__FILE__,__LINE__);
				return -1;
			}
			else
			{
				lpObj->MagicCount++;
				return n;
			}
		}
	}

	CLog.LogAdd(lMsg.Get(554),__FILE__,__LINE__);
	return -1;
}

int gObjWeaponMagicAdd(LPOBJ lpObj, WORD aSkill, BYTE Level)
{
	int skill = -1;
	int n;

	if(MagicDamageC.SkillGetRequireClass(lpObj->m_Index,aSkill) < 1)
	{
		return -1;
	}
	for(n = 0; n < MAGIC_SIZE; n++)
	{
		if(lpObj->Magic[n].IsMagic() == 0)
		{
			skill = lpObj->Magic[n].Set(aSkill,Level);
			if(skill < 0)
			{
				CLog.LogAdd(lMsg.Get(553),__FILE__,__LINE__);
				return -1;
			}
			else
			{
				lpObj->MagicCount++;
				return n;
			}
		}
	}
	CLog.LogAdd(lMsg.Get(554),__FILE__,__LINE__);
	return -1;
}

int gObjMonsterMagicAdd(LPOBJ lpObj, WORD aSkill, BYTE Level)
{
	int skill = -1;

	for (int n=0; n<MAGIC_SIZE ; n++)
	{
		if ( lpObj->Magic[n].IsMagic() == FALSE )
		{
			skill = lpObj->Magic[n].Set(aSkill, Level);

			if ( skill < 0 )
			{
				CLog.LogAdd(lMsg.Get( MSGGET(2, 41)),__FILE__, __LINE__);
				return -1;
			}

			lpObj->MagicCount++;
			return n;
		}
	}

	CLog.LogAdd(lMsg.Get( MSGGET(2, 42)), __FILE__, __LINE__);
	return -1;
}

bool gObjMagicEnergyCheck(LPOBJ lpObj , WORD aSkill)
{
	int reqeng;
	int reqlevel;
	// ----
	reqeng = MagicDamageC.SkillGetRequireEnergy(aSkill);

	if(reqeng < 0)
	{
		return false;
	}

	if((lpObj->Energy + lpObj->AddEnergy) < reqeng)
	{
		return false;
	}

	reqlevel = MagicDamageC.SkillGetRequireLevel(aSkill);

	if(lpObj->Level < reqlevel)
	{
		return false;
	}
	return true;
}

// ---------------------------------------------------------------------------------------------------------
// [Sunday, 20 Feb, 2011] WolF ~ Item Level UP By Bless ( Fixed New Wings and Mini Wings Level Up By M.E.S )
// ---------------------------------------------------------------------------------------------------------
bool gObjItemLevelUp(LPOBJ lpObj,int SourceSlot,int TargetSlot)
{
	if(SourceSlot < 0 || SourceSlot >= MAIN_INVENTORY_SIZE) return false;
	// ------
	if(TargetSlot < 0 || TargetSlot >= MAIN_INVENTORY_SIZE) return false;
	// ------
	if(lpObj->pInventory[SourceSlot].IsItem() == 0) return false;
	// ------
	if(lpObj->pInventory[TargetSlot].IsItem() == 0) return false;
	// ------
	CItem* lpItem = &lpObj->pInventory[TargetSlot];
	// ------
	if( !(lpItem->m_Type < ITEMGET(12,7)  || lpItem->m_Type == ITEMGET(13,30))  || lpItem->m_Type == ITEMGET(4,7)    || 
		lpItem->m_Type == ITEMGET(4,15)   || lpItem->m_Type == ITEMGET(12,36)   || lpItem->m_Type == ITEMGET(12,37)  || 
		lpItem->m_Type == ITEMGET(12,38)  || lpItem->m_Type == ITEMGET(12,39)   || lpItem->m_Type == ITEMGET(12,40)  || 
		lpItem->m_Type == ITEMGET(12,41)  || lpItem->m_Type == ITEMGET(12,42)   || lpItem->m_Type == ITEMGET(12,43)  || 
		lpItem->m_Type == ITEMGET(12,49)  || lpItem->m_Type == ITEMGET(12,50)   || lpItem->m_Type == ITEMGET(12,130) || 
		lpItem->m_Type == ITEMGET(12,131) || lpItem->m_Type == ITEMGET(12,132)  || lpItem->m_Type == ITEMGET(12,133) || 
		lpItem->m_Type == ITEMGET(12,134) || lpItem->m_Type == ITEMGET(12,135)  )
		return false;
	// ------
	if( lpItem->m_Level >= 6 ) return false;
	// ------
	if( lpItem->m_Type == ITEMGET(13,37) ) // ---- Fenrir
	{
		if( lpItem->m_Durability >= 255 ) return false;
		// ------
		if( rand() % 10000 < gPets.FenrirRepairRate )
		{
			int AddDur = rand() % 150 + 50;
			// ------
			if( (lpItem->m_Durability + AddDur) > 255)
			{
				lpItem->m_Durability = 255.0f;
			}
			else
			{
			    lpItem->m_Durability += AddDur;
			}
			// ------
			SendMsg.MessageOut(lpObj->m_Index, 0x01, "[FENRIR REPAIR] It's life Successfully Regenerated (%d/255).", lpItem->m_Durability);
			// ------
			CLog.LogAddC(TColor.Green(), "[FENRIR REPAIR][SUCCESS] [%s][%s] - %d/255 (+%d)",lpObj->AccountID, lpObj->Name, lpItem->m_Durability,AddDur);
		}
		else
		{
			SendMsg.MessageOut(lpObj->m_Index, 0x01, "[FENRIR REPAIR] It's life Failed to Regenerate (%d).", lpItem->m_Durability);
			// ------
			CLog.LogAddC(TColor.Red(), "[FENRIR REPAIR][FAILED] [%s][%s] - %d/255.", lpObj->AccountID, lpObj->Name, lpItem->m_Durability);
		}
		return true;
	}
	// ------
	lpItem->m_Level++;
	// ------
	gObjMakePreviewCharSet(lpObj->m_Index);
	// ------
	float Dur = ItemGetDurability(lpItem->m_Type,lpItem->m_Level,lpItem->IsExtItem(),lpItem->IsSetItem());
	// ------
	lpObj->pInventory[TargetSlot].m_Durability = Dur*(lpItem->m_Durability/lpItem->m_BaseDurability);
	// ------
	lpObj->pInventory[TargetSlot].Convert( lpItem->m_Type, lpItem->m_Option1, lpItem->m_Option2, lpItem->m_Option3, lpItem->m_NewOption,
		lpItem->m_SetOption, lpItem->m_ItemOptionEx,lpItem->m_ItemSocket);
	// ------
	return true;
}

// ---------------------------------------------------------------------------------------------------------
// [Sunday, 20 Feb, 2011] WolF ~ Item Level UP By Soul ( Fixed New Wings and Mini Wings Level Up By M.E.S )
// ---------------------------------------------------------------------------------------------------------
bool gObjItemRandomLevelUp(LPOBJ lpObj, int SourceSlot, int TargetSlot)
{
	if( SourceSlot < 0 || SourceSlot >= MAIN_INVENTORY_SIZE ) return false;
	// ------
	if(TargetSlot < 0 || TargetSlot >= MAIN_INVENTORY_SIZE) return false;
	// ------
	if(lpObj->pInventory[SourceSlot].IsItem() == 0) return false;
	// ------
	if(lpObj->pInventory[TargetSlot].IsItem() == 0) return false;
	// ------
	CItem* lpItem = &lpObj->pInventory[TargetSlot];
	// ------
	if( !(lpItem->m_Type < ITEMGET(12,7)  || lpItem->m_Type == ITEMGET(13,30))  || lpItem->m_Type == ITEMGET(4,7)    || 
		lpItem->m_Type == ITEMGET(4,15)   || lpItem->m_Type == ITEMGET(12,36)   || lpItem->m_Type == ITEMGET(12,37)  || 
		lpItem->m_Type == ITEMGET(12,38)  || lpItem->m_Type == ITEMGET(12,39)   || lpItem->m_Type == ITEMGET(12,40)  || 
		lpItem->m_Type == ITEMGET(12,41)  || lpItem->m_Type == ITEMGET(12,42)   || lpItem->m_Type == ITEMGET(12,43)  || 
		lpItem->m_Type == ITEMGET(12,49)  || lpItem->m_Type == ITEMGET(12,50)   || lpItem->m_Type == ITEMGET(12,130) || 
		lpItem->m_Type == ITEMGET(12,131) || lpItem->m_Type == ITEMGET(12,132)  || lpItem->m_Type == ITEMGET(12,133) || 
		lpItem->m_Type == ITEMGET(12,134) || lpItem->m_Type == ITEMGET(12,135)  )
		return false;
	// ------
	if( lpItem->m_Level >= 9 ) return false;
	// ------
	int Rate	= rand() % 100;
	int Success = HRB_SoulSuccessRate;
	// ------
	if( lpItem->m_Option2 > 0 )
	{
	    Success += 25;
	}
	// ------
	if( Rate < Success )
	{
	    lpItem->m_Level++;
	}
	// ------
	else if(lpItem->m_Level >= 7)
	{
		lpItem->m_Level = 0;
	}
	// ------
	else
	{
		lpItem->m_Level--;
	
		if(lpItem->m_Level < 0)
		{
			lpItem->m_Level = 0;
		}
	}
	// ------
	gObjMakePreviewCharSet(lpObj->m_Index);
	// ------
	float Dur = ItemGetDurability(lpItem->m_Type,lpItem->m_Level,lpItem->IsExtItem(),lpItem->IsSetItem());
	// ------
	lpObj->pInventory[TargetSlot].m_Durability = Dur * (lpItem->m_Durability/lpItem->m_BaseDurability);
	// ------
	lpObj->pInventory[TargetSlot].Convert( lpItem->m_Type, lpItem->m_Option1, lpItem->m_Option2, lpItem->m_Option3, lpItem->m_NewOption,
		lpItem->m_SetOption, lpItem->m_ItemOptionEx,lpItem->m_ItemSocket);
	// ------
	return true;
}

// ---------------------------------------------------------------------------------------------------------
// [Sunday, 20 Feb, 2011] WolF ~ Item Level UP By Jol ( Fixed New Wings and Mini Wings Level Up By M.E.S )
// ---------------------------------------------------------------------------------------------------------
bool gObjItemRandomOption3Up(LPOBJ lpObj, int SourceSlot, int TargetSlot)
{
	if( SourceSlot < 0 || SourceSlot >= MAIN_INVENTORY_SIZE ) return false;
	// ------
	if(TargetSlot < 0 || TargetSlot >= MAIN_INVENTORY_SIZE) return false;
	// ------
	if(lpObj->pInventory[SourceSlot].IsItem() == 0) return false;
	// ------
	if(lpObj->pInventory[TargetSlot].IsItem() == 0) return false;
	// ------
	CItem* lpItem = &lpObj->pInventory[TargetSlot];
	// ------
	if( !(lpItem->m_Type < ITEMGET(12,7)  || lpItem->m_Type == ITEMGET(13,30))  || lpItem->m_Type == ITEMGET(4,7)    || 
		lpItem->m_Type == ITEMGET(4,15)   || lpItem->m_Type == ITEMGET(12,36)   || lpItem->m_Type == ITEMGET(12,37)  || 
		lpItem->m_Type == ITEMGET(12,38)  || lpItem->m_Type == ITEMGET(12,39)   || lpItem->m_Type == ITEMGET(12,40)  || 
		lpItem->m_Type == ITEMGET(12,41)  || lpItem->m_Type == ITEMGET(12,42)   || lpItem->m_Type == ITEMGET(12,43)  || 
		lpItem->m_Type == ITEMGET(12,49)  || lpItem->m_Type == ITEMGET(12,50)   || lpItem->m_Type == ITEMGET(12,130) || 
		lpItem->m_Type == ITEMGET(12,131) || lpItem->m_Type == ITEMGET(12,132)  || lpItem->m_Type == ITEMGET(12,133) || 
		lpItem->m_Type == ITEMGET(12,134) || lpItem->m_Type == ITEMGET(12,135)  )
		return false;
	// ------
	if( lpItem->m_Option3 >= 7 ) return false;
	// ------
	int Rate	= rand() % 100;
	int Success = HRB_LifeSucessRate;
	// ------
	if( lpItem->m_Option3 == 0 )
	{
	    if( (lpItem->m_Type < ITEMGET(12,7)   || lpItem->m_Type == ITEMGET(13,30)) || lpItem->m_Type == ITEMGET(12,36)  || 
			lpItem->m_Type == ITEMGET(12,37)  || lpItem->m_Type == ITEMGET(12,38)  || lpItem->m_Type == ITEMGET(12,39)  || 
			lpItem->m_Type == ITEMGET(12,40)  || lpItem->m_Type == ITEMGET(12,41)  || lpItem->m_Type == ITEMGET(12,42)  || 
			lpItem->m_Type == ITEMGET(12,43)  || lpItem->m_Type == ITEMGET(12,49)  || lpItem->m_Type == ITEMGET(12,50)  || 
			lpItem->m_Type == ITEMGET(12,130) || lpItem->m_Type == ITEMGET(12,131) || lpItem->m_Type == ITEMGET(12,132) || 
			lpItem->m_Type == ITEMGET(12,133) || lpItem->m_Type == ITEMGET(12,134) || lpItem->m_Type == ITEMGET(12,135)  )
		{
		    lpItem->m_NewOption &= 0xDF;

			if(rand()%2 != 0)
			{
			    lpItem->m_NewOption |= 0x20;
			}
		}
	}
	// ------
	if( Rate < Success )
	{
	    lpItem->m_Option3 = 0;
	}
	// ------
	else
	{
	    lpItem->m_Option3++;
	}
	// ------
	gObjMakePreviewCharSet(lpObj->m_Index);
	// ------
	float Dur = ItemGetDurability(lpItem->m_Type,lpItem->m_Level,lpItem->IsExtItem(),lpItem->IsSetItem());
	// ------
	lpObj->pInventory[TargetSlot].m_Durability = Dur * (lpItem->m_Durability/lpItem->m_BaseDurability);
	// ------
	lpObj->pInventory[TargetSlot].Convert( lpItem->m_Type, lpItem->m_Option1, lpItem->m_Option2, lpItem->m_Option3, lpItem->m_NewOption, 
		lpItem->m_SetOption, lpItem->m_ItemOptionEx, lpItem->m_ItemSocket);
	// ------
	BYTE NewOption[8];
	// ------
	ItemIsBufExOption(NewOption,lpItem);
	// ------
	CLog.LogAddC(TColor.Green(), "[%s][%s] JewelofLife Result [%d,%s,%d,%d,%d,%d]serial:[%d] dur:[%d] Ex:[%d,%d,%d,%d,%d,%d,%d]", lpObj->AccountID,
		lpObj->Name, TargetSlot, lpItem->GetName(), lpItem->m_Level, lpItem->m_Option1, lpItem->m_Option2, lpItem->m_Option3, lpItem->m_Number, 
		lpItem->m_Durability, NewOption[0], NewOption[1], NewOption[2], NewOption[3], NewOption[4], NewOption[5], NewOption[6] );
	// ------
	return true;
}

void gObjAbilityReSet(LPOBJ lpObj)
{
	lpObj->LevelUpPoint = (lpObj->Level - 1) * 5;

	lpObj->Strength = gCharInfo.sCharInfo[lpObj->Class].Strength;
	lpObj->Dexterity = gCharInfo.sCharInfo[lpObj->Class].Dexterity;
	lpObj->Vitality = gCharInfo.sCharInfo[lpObj->Class].Vitality;
	lpObj->Energy = gCharInfo.sCharInfo[lpObj->Class].Energy;

	lpObj->MaxLife = gCharInfo.sCharInfo[lpObj->Class].MaxLife;
	lpObj->MaxMana = gCharInfo.sCharInfo[lpObj->Class].MaxMana;

	if(lpObj->Class == CLASS_WIZARD)
	{
		lpObj->MaxLife += (lpObj->Level -1)*0.5f;
		lpObj->MaxMana += 2.0f * (lpObj->Level -1);

		lpObj->Life = lpObj->MaxLife;
		lpObj->Mana = lpObj->MaxMana;
	}
	else
	{
		lpObj->MaxLife += 2.0f * (lpObj->Level - 1);
		lpObj->MaxMana += (lpObj->Level - 1)*0.5f;

		lpObj->Life = lpObj->MaxLife;
		lpObj->Mana = lpObj->MaxMana;
	}

	gNet.CloseClient(lpObj->m_Index);
}

bool gObjTargetGuildWarCheck(LPOBJ lpObj, LPOBJ lpTargetObj)
{
	if(lpObj->Type != OBJ_USER)
	{
		return false;
	}

	if(lpTargetObj->Type != OBJ_USER)
	{
		return false;
	}

	if(lpObj->GuildNumber < 1 || lpTargetObj->GuildNumber < 1)
	{
		return false;
	}

	if(lpObj->lpGuild->WarState != 1 || lpTargetObj->lpGuild->WarState != 1)
	{
		return false;
	}

	if(strcmp(lpObj->lpGuild->TargetGuildName,lpTargetObj->lpGuild->Name))
	{
		return false;
	}
	return true;
}

void gObjGuildWarEndSend(LPOBJ lpObj, BYTE Result1, BYTE Result2)
{
	if(lpObj == 0)
	{
		CLog.LogAdd("error-L3 : %s %d",__FILE__,__LINE__);
		return;
	}

	if(lpObj->lpGuild == 0)
	{
		CLog.LogAdd("error-L3 : %s %d",__FILE__,__LINE__);
		return;
	}

	if(lpObj->lpGuild->WarState != 1)
	{
		CLog.LogAdd("error-L3 : %s %d",__FILE__,__LINE__);
		return;
	}

	if(lpObj->lpGuild->lpTargetGuildNode == 0)
	{
		CLog.LogAdd("error-L3 : %s %d",__FILE__,__LINE__);
		return;
	}

	if(Result1 == 1)
	{
		gObjGuildWarItemGive(lpObj->lpGuild,lpObj->lpGuild->lpTargetGuildNode);
	}
	else if(Result1 == 3)
	{
		gObjGuildWarItemGive(lpObj->lpGuild->lpTargetGuildNode,lpObj->lpGuild);
	}
	else if(Result1 == 1)
	{
		gObjGuildWarItemGive(lpObj->lpGuild->lpTargetGuildNode,lpObj->lpGuild);
	}

	for(int n = 0; n < MAX_USER_GUILD;n++)
	{
		if(lpObj->lpGuild->Use[n] && lpObj->lpGuild->Index[n] >= 0)
		{
			gObj[lpObj->lpGuild->Index[n]].IsInBattleGround = 0;
			GCGuildWarEnd(lpObj->lpGuild->Index[n],Result1,lpObj->lpGuild->lpTargetGuildNode->Name);
		}
	}

	for(int n = 0; n < MAX_USER_GUILD;n++)
	{
		if(lpObj->lpGuild->lpTargetGuildNode->Use[n] && lpObj->lpGuild->lpTargetGuildNode->Index[n] >= 0)
		{
			gObj[lpObj->lpGuild->lpTargetGuildNode->Index[n]].IsInBattleGround = 0;
			GCGuildWarEnd(lpObj->lpGuild->lpTargetGuildNode->Index[n],Result2,lpObj->lpGuild->Name);
		}
	}
}

void gObjGuildWarEndSend(_GUILD_INFO_STRUCT * lpGuild1, _GUILD_INFO_STRUCT * lpGuild2, BYTE Result1, BYTE Result2)
{
	if(lpGuild1 == 0 || lpGuild2 == 0)
	{
		return;
	}

	if(lpGuild1->WarState != 1 || lpGuild2->WarState != 1)
	{
		return;
	}

	for(int n = 0; n < MAX_USER_GUILD;n++)
	{
		if(lpGuild1->Use[n] && lpGuild1->Index[n] >= 0)
		{
			gObj[lpGuild1->Index[n]].IsInBattleGround = 0;
			GCGuildWarEnd(lpGuild1->Index[n],Result1,lpGuild2->Name);
		}
	}

	for(int n = 0; n < MAX_USER_GUILD;n++)
	{
		if(lpGuild2->Use[n] && lpGuild2->Index[n] >= 0)
		{
			gObj[lpGuild2->Index[n]].IsInBattleGround = 0;
			GCGuildWarEnd(lpGuild2->Index[n],Result2,lpGuild1->Name);
		}
	}
}

void gObjGuildWarEnd(_GUILD_INFO_STRUCT * lpGuild, _GUILD_INFO_STRUCT * lpTargetGuild)
{
	if(lpGuild == 0 || lpTargetGuild == 0)
	{
		return;
	}

	lpGuild->WarDeclareState = 0;
	lpGuild->WarState = 0;

	if(lpTargetGuild != 0)
	{
		lpTargetGuild->WarDeclareState = 0;
		lpTargetGuild->WarState = 0;

		if(lpGuild->lpTargetGuildNode != 0)
		{
			_GUILD_INFO_STRUCT * lpTguild = lpGuild->lpTargetGuildNode;

			lpGuild->lpTargetGuildNode = 0;
			lpTguild->lpTargetGuildNode = 0;
		}
	}
	else
	{
		lpGuild->lpTargetGuildNode = 0;
	}
}

bool gObjGuildWarProc(_GUILD_INFO_STRUCT * lpGuild1, _GUILD_INFO_STRUCT * lpGuild2, int score)
{
	int totalscore;
	int n;
	int maxscore;
	char szTemp[0x100];

	if(lpGuild1 == 0 || lpGuild2 == 0)
	{
		return false;
	}

	totalscore = 1;
	maxscore = 20;

	lpGuild1->PlayScore += score;

	wsprintf(szTemp,"%s ( %d ) VS %s ( %d )",lpGuild1->Name,lpGuild1->PlayScore,lpGuild2->Name,lpGuild2->PlayScore);
	CLog.LogAdd(szTemp);

	if(lpGuild1->WarType == 1)
	{
		SetBattleTeamScore(lpGuild1->BattleGroundIndex,lpGuild1->BattleTeamCode,lpGuild1->PlayScore);
		maxscore = 100;
	}
	else
	{
		maxscore = 20;
	}

	if(lpGuild1->PlayScore >= maxscore)
	{
		if(lpGuild1->PlayScore > maxscore && lpGuild2->PlayScore == 0)
		{
			totalscore = 3;
		}
		else if(lpGuild1->PlayScore > maxscore && lpGuild2->PlayScore <= 10)
		{
			totalscore = 2;
		}

		lpGuild1->TotalScore += totalscore;
		DGGuildScoreUpdate(lpGuild1->Name,lpGuild1->TotalScore);
		DGGuildScoreUpdate(lpGuild2->Name,lpGuild2->TotalScore);

		return true;
	}

	for(n = 0; n < MAX_USER_GUILD; n++)
	{
		if(lpGuild1->Use[n] && lpGuild1->Index[n] >= 0)
		{
			GCGuildWarScore(lpGuild1->Index[n]);
		}
	}

	for(n = 0; n < MAX_USER_GUILD; n++)
	{
		if(lpGuild2->Use[n] && lpGuild2->Index[n] >= 0)
		{
			GCGuildWarScore(lpGuild2->Index[n]);
		}
	}
	return false;
}

bool gObjGuildWarCheck(LPOBJ lpObj, LPOBJ lpTargetObj)
{
	int score = 1;

	if(gObjTargetGuildWarCheck(lpObj,lpTargetObj)==0)
	{
		return false;
	}

	if(strcmp(lpTargetObj->Name,lpTargetObj->lpGuild->Names[0])==0)
	{
		score = 2;
	}

	_GUILD_INFO_STRUCT * lpGuild = lpObj->lpGuild;
	_GUILD_INFO_STRUCT * lpTargetGuild = lpTargetObj->lpGuild;

	if(gObjGuildWarProc(lpGuild,lpTargetGuild,score)==1)
	{
		if(lpGuild->WarType == 1)
		{
			gObjGuildWarEndSend(lpGuild,lpTargetGuild,1,0);
			gBattleGroundEnable(lpGuild->BattleGroundIndex,0);
			gObjGuildWarEnd(lpGuild,lpTargetGuild);
		}
		else
		{
			gObjGuildWarEndSend(lpObj,1,0);
			gObjAddMsgSendDelay(lpObj,4,lpObj->m_Index,2000,0);
		}
	}
	return true;
}

bool gObjGuildWarMasterClose(LPOBJ lpObj)
{
	if(lpObj->GuildNumber < 1)
	{
		return false;
	}

	if(strcmp(lpObj->lpGuild->Names[0],lpObj->Name))
	{
		return false;
	}

	gObjGuildWarEndSend(lpObj,3,2);

	if(lpObj->lpGuild->WarType == 1 && lpObj->lpGuild->BattleGroundIndex < 1)
	{
		gBattleGroundEnable(lpObj->lpGuild->BattleGroundIndex,0);
	}
	gObjGuildWarEnd(lpObj->lpGuild,lpObj->lpGuild->lpTargetGuildNode);
	return true;
}

bool gObjGuildWarItemGive(_GUILD_INFO_STRUCT * lpWinGuild, _GUILD_INFO_STRUCT * lpLoseGuild)
{
	return true;
	int n;
	int r_userindex[MAX_USER_GUILD];
	int r_usercount;

	for(n = 0; n < MAX_USER_GUILD; n++)
	{
		if(lpLoseGuild->Use[n] > 0)
		{
			r_userindex[r_usercount] = lpLoseGuild->Index[n];
			r_usercount++;
		}
	}

	if(r_usercount < 1)
	{
		CLog.LogAdd(lMsg.Get(560),__FILE__,__LINE__);
		return false;
	}

	int lose_user = r_userindex[rand()%r_usercount];

	if(lose_user < 0)
	{
		CLog.LogAdd(lMsg.Get(560),__FILE__,__LINE__);
		return false;
	}

	if(gObj[lose_user].Connected < PLAYER_PLAYING)
	{
		CLog.LogAdd(lMsg.Get(560),__FILE__,__LINE__);
		return false;
	}

	r_usercount = 0;

	for(n = 0; n < MAX_USER_GUILD;n++)
	{
		if(lpWinGuild->Use[n] > 0)
		{
			r_userindex[r_usercount] = lpWinGuild->Index[n];
			r_usercount++;
		}
	}

	if(r_usercount < 1)
	{
		CLog.LogAdd(lMsg.Get(561));
		return false;
	}

	int win_user = r_userindex[rand()%r_usercount];

	if(win_user < 0)
	{
		CLog.LogAdd(lMsg.Get(561));
		return false;
	}

	if(gObj[win_user].Connected < PLAYER_PLAYING)
	{
		CLog.LogAdd(lMsg.Get(561));
		return false;
	}

	LPOBJ lpObj = &gObj[lose_user];

	int count = 24;

	int number;

	while(count--)
	{
		number = rand()%12;

		if(lpObj->pInventory[number].IsItem()==1)
		{
			if(gObjInventoryInsertItem(win_user,lpObj->pInventory[number]) != 0xFF)
			{
				return true;
			}
			break;
		}
	}

	count = 64;

	while(count--)
	{
		number = rand()%64+12;

		if(lpObj->pInventory[number].IsItem()==1)
		{
			if(gObjInventoryInsertItem(win_user,lpObj->pInventory[number]) != 0xFF)
			{
				return true;
			}
			break;
		}
	}
	return true;
}

bool gObjGetPkTime(LPOBJ lpObj, int& hour, int& min)
{
	int pktime = 0;

	if(lpObj->m_PK_Time > 0)
	{
		pktime = lpObj->m_PK_Time / 60;
	}

	if(lpObj->m_PK_Level == 4)
	{
		hour = (180 - pktime) / 60;
		min = (180 - pktime) % 60;
		return true;
	}

	if(lpObj->m_PK_Level == 5)
	{
		hour = (360 - pktime) / 60;
		min = (360 - pktime) % 60;
		return true;
	}

	if(lpObj->m_PK_Level >= 6)
	{
		hour = (((lpObj->m_PK_Count - 3) * 60 + 540) - pktime) / 60;
		min = (((lpObj->m_PK_Count - 3) * 60 + 540) - pktime) % 60;
		return true;
	}

	return false;
}

bool gObjMonsterCall(int aIndex, int MonsterType, int x, int y)
{

	if(gObj[aIndex].MapNumber == MAP_INDEX_ICARUS)
	{
		return false;
	}

	if(gObj[aIndex].m_RecallMon >= 0)
	{

		GCRecallMonLife(aIndex,60,0);
		gObjMonsterCallKill(aIndex);
		return false;
	}

	int result = gObjAddCallMon();

	if(result >= 0)
	{
		gObj[result].X = x;
		gObj[result].Y = y;

		gObj[result].MTX = x;
		gObj[result].MTY = y;
		gObj[result].Dir = 2;

		gObj[result].MapNumber = gObj[aIndex].MapNumber;

		gObjSetMonster(result,MonsterType);

		gObj[result].m_RecallMon = aIndex;
		gObj[result].m_Attribute = 100;

		gObj[result].TargetNumber = -1;
		gObj[result].m_ActState.Emotion = 0;
		gObj[result].m_ActState.Attack = 0;
		gObj[result].m_ActState.EmotionCount = 0;

		gObj[result].PathCount = 0;

		gObj[aIndex].m_RecallMon = result;
		gObj[result].m_MoveRange = 15;

		GCRecallMonLife(gObj[result].m_RecallMon,gObj[result].MaxLife,gObj[result].Life);
		CLog.LogAdd("[Summon Monster] [%s][%s] Try to Summon Monster - Succeed (SummonIndex:%d)",gObj[aIndex].AccountID,gObj[aIndex].Name,result);
		return true;
	}
	return false;
}

void gObjMonsterCallKill(int aIndex)
{

	CLog.LogAdd("[Summon Monster] [%s][%s] Try to Kill Summoned Monster",gObj[aIndex].AccountID,gObj[aIndex].Name);

	if(gObj[aIndex].m_RecallMon < 0)
	{
		return;
	}

	int callmon = gObj[aIndex].m_RecallMon;

	if(callmon < 0 || callmon > OBJMAX -1 )
	{
		return;
	}

	if(gObj[callmon].Type != OBJ_MONSTER)
	{
		return;
	}

	if(gObj[callmon].m_RecallMon != aIndex)
	{
		return;
	}

	if(gObj[aIndex].m_RecallMon >= 0)
	{
		if(gObjIsConnected(gObj[aIndex].m_RecallMon)==1)
		{
			CLog.LogAdd("[Summon Monster] [%s][%s] Try to Kill Summoned Monster - Found Summoned Monster (SummonIndex:%d)",gObj[aIndex].AccountID,gObj[aIndex].Name,gObj[aIndex].m_RecallMon);
			gObj[gObj[aIndex].m_RecallMon].m_RecallMon = -1;
		}
	}

	gObj[aIndex].m_RecallMon = -1;
	gObjDel(callmon);

	CLog.LogAdd("[Summon Monster] [%s][%s] Try to Kill Summoned Monster - Finished to Kill Summoned Monster (SummonIndex:%d)",gObj[aIndex].AccountID,gObj[aIndex].Name,callmon);
}

// --------------------------------------------------
// [Sunday, 20 Feb, 2011] WolF ~ Check Teleport Area
// --------------------------------------------------
bool gObjCheckTeleportArea(int aIndex, BYTE x, BYTE y)
{
	LPOBJ lpObj = &gObj[aIndex];
	// ------
	if( (lpObj->X - 8) > x || (lpObj->X + 8) < x || (lpObj->Y - 8) > y || (lpObj->Y + 8) < y) return false;
	// ------
	int Map = lpObj->MapNumber;
	// ------
	if( !MAP_RANGE(Map) ) return false;
	// ------
	BYTE Attribute = MapC[Map].GetAttr(x,y);
	// ------
	if( Attribute ) return false;
	// ------
	Attribute = MapC[Map].GetAttr(lpObj->X,lpObj->Y);
	// ------
	if( (Attribute & 1) ) return false;
	// ------
	return true;
}

bool gObjCheckAttackAreaUsedPath(int aIndex, int TarObjIndex)
{
	int X,Y;
	if(gObj[aIndex].PathCount > 1)
	{
		 X = gObj[aIndex].PathX[gObj[aIndex].PathCount-1];
		 Y = gObj[aIndex].PathY[gObj[aIndex].PathCount-1];
	}
	else
	{
		return false;
	}

	if(gObj[TarObjIndex].X < (X - 10) || gObj[TarObjIndex].X > (X + 10) || gObj[TarObjIndex].Y < (Y - 10) || gObj[TarObjIndex].Y > (Y + 10))
	{
		return false;
	}
	return true;
}

bool gObjCheckattackAreaUsedViewPort(int aIndex, int TarObjIndex)
{
	LPOBJ lpObj = &gObj[aIndex];
	LPOBJ lpTargetObj = &gObj[TarObjIndex];

	if(lpTargetObj->m_Index == lpObj->m_Index)
	{
		return true;
	}

	int MVL = MAX_VIEWPORT;

	if(lpObj->Type == OBJ_MONSTER)
	{
		MVL = MAX_VIEWPORT_MONSTER;
	}

	for(int n = 0; n < MVL;n++)
	{
		if(lpObj->VpPlayer[n].state)
		{
			if(lpObj->VpPlayer[n].number == lpTargetObj->m_Index)
			{
				return true;
			}
		}

		if(lpObj->VpPlayer2[n].state)
		{
			if(lpObj->VpPlayer2[n].number == lpTargetObj->m_Index)
			{
				return true;
			}
		}
	}
	return false;
}

int gObjCheckAttackArea(int aIndex, int TarObjIndex)
{

	if(gObjCheckattackAreaUsedViewPort(aIndex,TarObjIndex)==0)
	{
		return 1;
	}

	int mapnumber = gObj[aIndex].MapNumber;

	if(mapnumber < 0 || mapnumber >= MAX_NUMBER_MAP)
	{
		return 2;
	}

	int tarmapnumber = gObj[TarObjIndex].MapNumber;

	if(tarmapnumber < 0 || tarmapnumber >= MAX_NUMBER_MAP)
	{
		return 3;
	}

	if(tarmapnumber != mapnumber)
	{
		return 4;
	}

	BYTE attr = 0;
#if(!GS_CASTLE)
	attr = MapC[mapnumber].GetAttr(gObj[TarObjIndex].X,gObj[TarObjIndex].Y);
	
	if(attr && !(attr&2))
	{
		return 5;
	}
#endif	
	attr = MapC[mapnumber].GetAttr(gObj[aIndex].X,gObj[aIndex].Y);
	
	if(attr && !(attr&2))
	{
		return 6;
	}

	return false;
}
// Done 100%
bool gUserFindDevilSquareInvitation(int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];

	for(int n = 0; n < MAIN_INVENTORY_SIZE; n++)
	{
		if(lpObj->pInventory[n].IsItem() == 1)
		{
			if(lpObj->pInventory[n].m_Type == ITEMGET(14,19))
			{
				return true;
			}

			if(lpObj->pInventory[n].m_Type == ITEMGET(13,46))
			{
				return true;
			}
		}
	}
	return false;
}
// Done 100%
bool gUserFindDevilSquareKeyEyes(int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];
	int bFindKey = 0;
	int bFindEyes = 0;

	for(int n = 0; n < MAIN_INVENTORY_SIZE; n++)
	{
		if(lpObj->pInventory[n].IsItem() == 1)
		{
			if(lpObj->pInventory[n].m_Type == ITEMGET(14,17))
			{
				bFindEyes = 1;
			}

			if(lpObj->pInventory[n].m_Type == ITEMGET(14,18))
			{
				bFindKey = 1;
			}
		}
	}

	if(bFindKey != 0 && bFindEyes != 0)
	{
		return true;
	}
	return false;
}

void gObjSendUserStatistic(int aIndex, int startLevel, int endLevel)
{
	if(gDisconnect == 1)
	{
		return;
	}

	if(endLevel < startLevel)
	{
		endLevel = HRB_CharMaxLevel;
	}

	int n;
	LPOBJ lpObj;
	int sClassCount[7]; 

	sClassCount[CLASS_WIZARD] = 0;
	sClassCount[CLASS_KNIGHT] = 0;
	sClassCount[CLASS_ELF] = 0;
	sClassCount[CLASS_MAGUMSA] = 0;
	sClassCount[CLASS_DARKLORD] = 0;
	sClassCount[CLASS_SUMMONER] = 0;
	sClassCount[CLASS_FIGHTER] = 0;

	for(n = OBJ_STARTUSERINDEX; n < OBJMAX; n++)
	{
		lpObj = &gObj[n];

		if(lpObj->Connected > PLAYER_LOGGED)
		{
			if(lpObj->Level >= startLevel && lpObj->Level <= endLevel)
			{
				sClassCount[lpObj->Class]++;
			}
		}
	}
	MsgOutput(gObj[aIndex].m_Index,"[%d]-[%d] Total --> Dark Wizard:%d, Dark Knight:%d, Elf:%d, Magic Gladiator:%d, Dark Lord:%d, Summoner:%d, Rage Fighter:%d",
		startLevel,
		endLevel,
		sClassCount[CLASS_WIZARD],
		sClassCount[CLASS_KNIGHT],
		sClassCount[CLASS_ELF],
		sClassCount[CLASS_MAGUMSA],
		sClassCount[CLASS_DARKLORD],
	    sClassCount[CLASS_SUMMONER],
		sClassCount[CLASS_FIGHTER]);
}

LPOBJ gObjFind(char * targetcharname)
{
	if ( gDisconnect == TRUE )
	{
		return NULL;
	}

	int n;
	LPOBJ lpObj;

	for ( n = OBJ_STARTUSERINDEX ; n<OBJMAX;n++)
	{
		lpObj = &gObj[n];

		if ( lpObj->Connected > PLAYER_LOGGED )
		{
			if ( lpObj->Name[0] == targetcharname[0] )
			{
				if ( strcmp(lpObj->Name, targetcharname) == 0 )
				{
					return lpObj;
				}
			}
		}
	}

	return NULL;

}

bool gObjFind10EventChip(int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];
	int count = 0;

	for(int n = 0; n < MAIN_INVENTORY_SIZE;n++)
	{
		if(lpObj->pInventory[n].IsItem() == 1)
		{
			if(lpObj->pInventory[n].m_Type == ITEMGET(14,21))
			{
				count++;

				if(count >= 10)
				{
					return true;
				}
			}
		}
	}
	return false;
}

bool gObjDelete10EventChip(int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];
	int count = 0;

	for(int n = 0; n < MAIN_INVENTORY_SIZE;n++)
	{
		if(lpObj->pInventory[n].IsItem() == 1)
		{
			if(lpObj->pInventory[n].m_Type == ITEMGET(14,21))
			{
				gObjInventoryDeleteItem(aIndex,n);
				GCInventoryItemDeleteSend(aIndex,n,1);
				CLog.LogAdd("[EventChip] [%s][%s] Delete Rena (%d)",lpObj->AccountID,lpObj->Name,n);
				count++;

				if(count >= 10)
				{
					return true;
				}
			}
		}
	}
	return false;
}

// Done 100%
int gObjGetItemCountInEquipment(int aIndex, int itemtype, int itemindex, int itemlevel)
{
	LPOBJ lpObj = &gObj[aIndex];
	int count = 0;

	for ( int n =0;n<MAX_PLAYER_EQUIPMENT ; n++ )
	{
		if ( lpObj->pInventory[n].IsItem() == TRUE )
		{
			if ( lpObj->pInventory[n].m_Type == ((itemtype * MAX_SUBTYPE_ITEMS) + itemindex) )
			{
				if ( itemlevel != -1 )
				{
					if (  lpObj->pInventory[n].m_Level != itemlevel )
					{
						continue;
					}
				}
				
				count++;
			}
		}
	}

	return count;
}
// Done 100%
int gObjGetItemCountInIventory(int aIndex, int itemtype, int itemindex, int itemlevel)
{
	LPOBJ lpObj = &gObj[aIndex];
	int count = 0;

	for ( int n =0;n<MAIN_INVENTORY_SIZE ; n++ )
	{
		if ( lpObj->pInventory[n].IsItem() == true )
		{
			if ( lpObj->pInventory[n].m_Type == ((itemtype * MAX_SUBTYPE_ITEMS) + itemindex) )
			{
				if ( itemlevel != -1 )
				{
					if (  lpObj->pInventory[n].m_Level != itemlevel )
					{
						continue;
					}
				}
				
				count++;
			}
		}
	}

	return count;
}
// Done 100%
int gObjGetItemCountInIventory(int aIndex, int itemnum)
{
	LPOBJ lpObj = &gObj[aIndex];
	int count = 0;

	for ( int n=0;n<MAIN_INVENTORY_SIZE;n++)
	{
		if ( lpObj->pInventory[n].IsItem() == TRUE )
		{
			if ( lpObj->pInventory[n].m_Type == itemnum )
			{
				count++;
			}
		}
	}

	return count;

}
// Done 100%
int gObjGetManaItemPos(int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];
	
	for ( int n=0;n<MAIN_INVENTORY_SIZE;n++)
	{
		if ( lpObj->pInventory[n].IsItem() == TRUE )
		{
			if ( lpObj->pInventory[n].m_Type == ITEMGET(14,4) || lpObj->pInventory[n].m_Type == ITEMGET(14,5) || lpObj->pInventory[n].m_Type == ITEMGET(14,6) || lpObj->pInventory[n].m_Type == ITEMGET(14,71) )
			{
				return n;
			}
		}
	}
	return -1;
}
// Done 100%
void gObjDelteItemCountInInventory(int aIndex, int itemtype, int itemindex, int count)
{
	LPOBJ lpObj = &gObj[aIndex];
	int delcount = 0;

	for(int n = 0; n < MAIN_INVENTORY_SIZE; n++)
	{
		if(lpObj->pInventory[n].IsItem() == 1)
		{
			if(lpObj->pInventory[n].m_Type == itemtype * 512 + itemindex)
			{
				gObjInventoryDeleteItem(aIndex,n);
				GCInventoryItemDeleteSend(aIndex,n,1);
				CLog.LogAdd("[DeleteItem] [%s][%s] Delete (%d,%d)(%d)",lpObj->AccountID,lpObj->Name,itemtype,itemindex,n);
				delcount++;

				if(delcount >= count)
				{
					return;
				}
			}
		}
	}
}
// Done 100%
void gObjGetStatPointState(int aIndex, short& AddPoint, short& MaxAddPoint, short& MinusPoint, short& MaxMinusPoint)
{
	if(gObj[aIndex].Level <= 5)
	{
		AddPoint = 0;
		MaxAddPoint = 0;
	}

	LPOBJ lpObj = &gObj[aIndex];

	int total_point = lpObj->Strength + lpObj->Dexterity + lpObj->Vitality + lpObj->Energy + lpObj->Leadership + lpObj->LevelUpPoint;
	int ori_point;

	switch ( lpObj->Class )
	{
	case CLASS_WIZARD:		ori_point = gCharInfo.GetTotalPoint(lpObj->Class) + (lpObj->Level - 1) * gCharInfo.CLASS_WIZARD_POINTS;	break;
	case CLASS_KNIGHT:		ori_point = gCharInfo.GetTotalPoint(lpObj->Class) + (lpObj->Level - 1) * gCharInfo.CLASS_KNIGHT_POINTS;	break;
	case CLASS_ELF:			ori_point = gCharInfo.GetTotalPoint(lpObj->Class) + (lpObj->Level - 1) * gCharInfo.CLASS_ELF_POINTS;		break;
	case CLASS_MAGUMSA:		ori_point = gCharInfo.GetTotalPoint(lpObj->Class) + (lpObj->Level - 1) * gCharInfo.CLASS_MAGUMSA_POINTS;	break;
	case CLASS_DARKLORD:	ori_point = gCharInfo.GetTotalPoint(lpObj->Class) + (lpObj->Level - 1) * gCharInfo.CLASS_DARKLORD_POINTS;	break;
	case CLASS_SUMMONER:	ori_point = gCharInfo.GetTotalPoint(lpObj->Class) + (lpObj->Level - 1) * gCharInfo.CLASS_SUMMONER_POINTS;	break;
	case CLASS_FIGHTER:		ori_point = gCharInfo.GetTotalPoint(lpObj->Class) + (lpObj->Level - 1) * gCharInfo.CLASS_FIGHTER_POINTS;	break;
	}

	if(g_QuestInfo.GetQuestState(lpObj,0) == 2)
	{
		ori_point+= 10;
	}

	if(g_QuestInfo.GetQuestState(lpObj,1) == 2)
	{
		ori_point+= 10;
	}

	if(g_QuestInfo.GetQuestState(lpObj,2) == 2)
	{
		switch ( lpObj->Class )
		{
		case CLASS_WIZARD:		ori_point+= lpObj->Level - 220 * gCharInfo.CLASS_WIZARD_POINTS_PLUS;	break;
		case CLASS_KNIGHT:		ori_point+= lpObj->Level - 220 * gCharInfo.CLASS_KNIGHT_POINTS_PLUS;	break;
		case CLASS_ELF:			ori_point+= lpObj->Level - 220 * gCharInfo.CLASS_ELF_POINTS_PLUS;		break;
		case CLASS_SUMMONER:	ori_point+= lpObj->Level - 220 * gCharInfo.CLASS_SUMMONER_POINTS_PLUS;break;
		}
	}

	int addpoint = ori_point;

	addpoint += gStatMng.GetMaxStat(lpObj->Level,lpObj->Class);

	AddPoint = total_point - ori_point;
	MaxAddPoint = gStatMng.GetMaxStat(lpObj->Level,lpObj->Class);
	MinusPoint = lpObj->iFruitPoint;
	MaxMinusPoint = gStatMng.GetMaxMinusStat(lpObj->Level,lpObj->Class);
}
// Done 100%
bool gObjCheckStatPointUp(int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];

	int total_point = lpObj->Strength + lpObj->Dexterity + lpObj->Vitality + lpObj->Energy + lpObj->Leadership + lpObj->LevelUpPoint;
	int ori_point;
	switch ( lpObj->Class )
	{
	case CLASS_WIZARD:		ori_point = gCharInfo.GetTotalPoint(lpObj->Class) + (lpObj->Level - 1) * gCharInfo.CLASS_WIZARD_POINTS;	break;
	case CLASS_KNIGHT:		ori_point = gCharInfo.GetTotalPoint(lpObj->Class) + (lpObj->Level - 1) * gCharInfo.CLASS_KNIGHT_POINTS;	break;
	case CLASS_ELF:			ori_point = gCharInfo.GetTotalPoint(lpObj->Class) + (lpObj->Level - 1) * gCharInfo.CLASS_ELF_POINTS;		break;
	case CLASS_MAGUMSA:		ori_point = gCharInfo.GetTotalPoint(lpObj->Class) + (lpObj->Level - 1) * gCharInfo.CLASS_MAGUMSA_POINTS;	break;
	case CLASS_DARKLORD:	ori_point = gCharInfo.GetTotalPoint(lpObj->Class) + (lpObj->Level - 1) * gCharInfo.CLASS_DARKLORD_POINTS;	break;
	case CLASS_SUMMONER:	ori_point = gCharInfo.GetTotalPoint(lpObj->Class) + (lpObj->Level - 1) * gCharInfo.CLASS_SUMMONER_POINTS;	break;
	case CLASS_FIGHTER:		ori_point = gCharInfo.GetTotalPoint(lpObj->Class) + (lpObj->Level - 1) * gCharInfo.CLASS_FIGHTER_POINTS;	break;
	}

	if(g_QuestInfo.GetQuestState(lpObj,0) == 2)
	{
		ori_point+= 10;
	}

	if(g_QuestInfo.GetQuestState(lpObj,1) == 2)
	{
		ori_point+= 10;
	}

	if(g_QuestInfo.GetQuestState(lpObj,2) == 2)
	{
		switch ( lpObj->Class )
		{
		case CLASS_WIZARD:		ori_point+= lpObj->Level - 220 * gCharInfo.CLASS_WIZARD_POINTS_PLUS;	break;
		case CLASS_KNIGHT:		ori_point+= lpObj->Level - 220 * gCharInfo.CLASS_KNIGHT_POINTS_PLUS;	break;
		case CLASS_ELF:			ori_point+= lpObj->Level - 220 * gCharInfo.CLASS_ELF_POINTS_PLUS;		break;
		case CLASS_SUMMONER:	ori_point+= lpObj->Level - 220 * gCharInfo.CLASS_SUMMONER_POINTS_PLUS;break;
		}
	}

	ori_point += gStatMng.GetMaxStat(lpObj->Level,lpObj->Class);

	if(total_point < ori_point)
	{
		return true;
	}
	return false;
}
// Done 100%
bool gObjCheckStatPointDown(int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];

	int total_point = lpObj->Strength + lpObj->Dexterity + lpObj->Vitality + lpObj->Energy + lpObj->LevelUpPoint;
	int ori_point;

	switch ( lpObj->Class )
	{
	case CLASS_WIZARD:		ori_point = gCharInfo.GetTotalPoint(lpObj->Class) + (lpObj->Level - 1) * gCharInfo.CLASS_WIZARD_POINTS;	break;
	case CLASS_KNIGHT:		ori_point = gCharInfo.GetTotalPoint(lpObj->Class) + (lpObj->Level - 1) * gCharInfo.CLASS_KNIGHT_POINTS;	break;
	case CLASS_ELF:			ori_point = gCharInfo.GetTotalPoint(lpObj->Class) + (lpObj->Level - 1) * gCharInfo.CLASS_ELF_POINTS;		break;
	case CLASS_MAGUMSA:		ori_point = gCharInfo.GetTotalPoint(lpObj->Class) + (lpObj->Level - 1) * gCharInfo.CLASS_MAGUMSA_POINTS;	break;
	case CLASS_DARKLORD:	ori_point = gCharInfo.GetTotalPoint(lpObj->Class) + (lpObj->Level - 1) * gCharInfo.CLASS_DARKLORD_POINTS;	break;
	case CLASS_SUMMONER:	ori_point = gCharInfo.GetTotalPoint(lpObj->Class) + (lpObj->Level - 1) * gCharInfo.CLASS_SUMMONER_POINTS;	break;
	case CLASS_FIGHTER:		ori_point = gCharInfo.GetTotalPoint(lpObj->Class) + (lpObj->Level - 1) * gCharInfo.CLASS_FIGHTER_POINTS;	break;
	}

	if(g_QuestInfo.GetQuestState(lpObj,0) == 2)
	{
		ori_point+= 10;
	}

	if(g_QuestInfo.GetQuestState(lpObj,1) == 2)
	{
		ori_point+= 10;
	}

	if(g_QuestInfo.GetQuestState(lpObj,2) == 2)
	{
		switch ( lpObj->Class )
		{
		case CLASS_WIZARD:		ori_point+= lpObj->Level - 220 * gCharInfo.CLASS_WIZARD_POINTS_PLUS;	break;
		case CLASS_KNIGHT:		ori_point+= lpObj->Level - 220 * gCharInfo.CLASS_KNIGHT_POINTS_PLUS;	break;
		case CLASS_ELF:			ori_point+= lpObj->Level - 220 * gCharInfo.CLASS_ELF_POINTS_PLUS;		break;
		case CLASS_SUMMONER:	ori_point+= lpObj->Level - 220 * gCharInfo.CLASS_SUMMONER_POINTS_PLUS;break;
		}
	}

	ori_point += gStatMng.GetMaxStat(lpObj->Level,lpObj->Class);

	if(total_point > ori_point)
	{
		return true;
	}
	return false;
}

void gObjUseCircle(int aIndex, int pos)
{
	int level = gObj[aIndex].pInventory[pos].m_Level;
	int iSerial = gObj[aIndex].pInventory[pos].m_Number;

	PMSG_DEFRESULT pResult;

	C1HeadSet((LPBYTE)&pResult,0x2C,sizeof(pResult));

	pResult.result = 0;

	if(gObj[aIndex].Level <= 10 || level > 3)
	{
		pResult.result |= 0xC0;
		gSendProto.DataSend(aIndex,(unsigned char*)&pResult,pResult.h.size);
		return;
	}

	if(gObjCheckStatPointUp(aIndex)==0)
	{
		pResult.result |= 0xC0;
		gSendProto.DataSend(aIndex,(unsigned char*)&pResult,pResult.h.size);
		return;
	}

	CLog.LogAdd("[StatUp] [%s][%s] Use StatUpItem Level:%d Pos:%d serial:%d",gObj[aIndex].AccountID,gObj[aIndex].Name,level,pos,iSerial);

	gObjInventoryItemSet(aIndex,pos,0xFF);
	gObj[aIndex].pInventory[pos].Clear();
	GCInventoryItemDeleteSend(aIndex,pos,1);

	short AddPoint;
	short MaxAddPoint;
	short MinusPoint;
	short MaxMinusPoint;

	gObjGetStatPointState(aIndex,(short &)AddPoint,(short &)(MaxAddPoint),(short &)MinusPoint,(short &)MaxMinusPoint);

	int iSuccessRate = 0;

	if(AddPoint <= 10)
	{
		iSuccessRate = 100;
	}
	else if((AddPoint - 10) < MaxAddPoint * (double)0.1)
	{
		iSuccessRate = 90;
	}
	else if((AddPoint - 10) < MaxAddPoint * (double)0.3)
	{
		iSuccessRate = 80;
	}
	else if((AddPoint - 10) < MaxAddPoint * (double)0.5)
	{
		iSuccessRate = 70;
	}
	else if((AddPoint - 10) < MaxAddPoint * (double)0.8)
	{
		iSuccessRate = 60;
	}
	else
	{
		iSuccessRate = 50;
	}

	int incvalue = 0;

	if(rand()%100 < iSuccessRate)
	{
		iSuccessRate = rand()%100;

		int AddStat = 0;

		if(iSuccessRate < 70)
		{
			AddStat = 1;
		}
		else if(iSuccessRate < 95)
		{
			AddStat = 2;
		}
		else
		{
			AddStat = 3;
		}

		if((AddStat + AddPoint) > MaxAddPoint)
		{
			AddStat = 1;
		}

		pResult.result |= level * 16;
		pResult.result |= AddStat;

		gSendProto.DataSend(aIndex,(unsigned char *)&pResult,pResult.h.size);

		incvalue = AddStat;

		CLog.LogAdd("[StatUp] [%s][%s] Success [%d][%d] %d",gObj[aIndex].AccountID,gObj[aIndex].Name,level,incvalue,iSerial);
	}
	else
	{
		pResult.result |= 0x40;
		pResult.result |= level * 16;
		pResult.result = pResult.result;

		gSendProto.DataSend(aIndex,(unsigned char *)&pResult,pResult.h.size);
		CLog.LogAdd("[StatUp] [%s][%s] Fail [%d] %d",gObj[aIndex].AccountID,gObj[aIndex].Name,level,iSerial);
		return;
	}

	switch(level)
	{
	case 4:
		gObj[aIndex].Leadership += incvalue;
		break;
	case 3:
		gObj[aIndex].Strength += incvalue;
		break;
	case 2:
		gObj[aIndex].Dexterity += incvalue;
		break;
	case 1:
		gObj[aIndex].Vitality += incvalue;
		gObj[aIndex].MaxLife += gObj[aIndex].VitalityToLife * incvalue;
		GCReFillSend(gObj[aIndex].m_Index,gObj[aIndex].MaxLife + gObj[aIndex].AddLife,0xFE,0,gObj[aIndex].iMaxShield+gObj[aIndex].iAddShield);
		break;
	case 0:
		gObj[aIndex].Energy += incvalue;
		gObj[aIndex].MaxMana += gObj[aIndex].EnergyToMana * incvalue;
		gObjSetBP(aIndex);
		GCManaSend(gObj[aIndex].m_Index,gObj[aIndex].MaxMana + gObj[aIndex].AddMana,0xFE,0,gObj[aIndex].MaxBP);
		break;
	default: break;
	}

	gCalCharacter.gObjCalCharacter(aIndex);
}

void gObjUsePlusStatFruit(int aIndex,int pos)
{
	if(!OBJMAX_RANGE(aIndex))
	{
		return;
	}

	if(gObj[aIndex].Class >= MAX_CLASS_TYPE || gObj[aIndex].Class < 0)
	{
		return;
	}

	int iItemLevel = gObj[aIndex].pInventory[pos].m_Level;
	int iItemSerial = gObj[aIndex].pInventory[pos].m_Number;
	int iCharacterClass = gObj[aIndex].Class;

	PMSG_USE_STAT_FRUIT pResult;

	C1HeadSet((LPBYTE)&pResult,0x2C,sizeof(pResult));

	pResult.result = 2;
	pResult.btFruitType = iItemLevel;
	pResult.btStatValue = 0;

	if(gObj[aIndex].Level <= 9 || iItemLevel > 4)
	{
		pResult.result = 2;
		gSendProto.DataSend(aIndex,(unsigned char *)&pResult,pResult.h.size);
		return;
	}

	if(iItemLevel == 4 && gObj[aIndex].Class != CLASS_DARKLORD )
	{
		pResult.result = 2;
		gSendProto.DataSend(aIndex,(unsigned char *)&pResult,pResult.h.size);
		return;
	}

	for(int iItemIndex = 0; iItemIndex < INVETORY_WEAR_SIZE; iItemIndex++)
	{
		if(gObj[aIndex].pInventory[iItemIndex].IsItem())
		{
			pResult.result = 16;
			gSendProto.DataSend(aIndex,(unsigned char *)&pResult,pResult.h.size);
			return;
		}
	}

	if(gObjCheckStatPointUp(aIndex) == false)
	{
		pResult.result = 33;
		gSendProto.DataSend(aIndex,(unsigned char *)&pResult,pResult.h.size);
		return;
	}

	CLog.LogAdd("[StatUp] [%s][%s] Use StatUpItem Level:%d Pos:%d serial:%d",
		gObj[aIndex].AccountID,gObj[aIndex].Name,iItemLevel,pos,iItemSerial);

	gObjInventoryItemSet(aIndex,pos,0xFF);
	gObj[aIndex].pInventory[pos].Clear();
	GCInventoryItemDeleteSend(aIndex,pos,1);

	short AddPoint;
	short MaxAddPoint;
	short MinusPoint;
	short MaxMinusPoint;

	gObjGetStatPointState(aIndex,(short &)AddPoint,(short &)MaxAddPoint,(short &)MinusPoint,(short &)MaxMinusPoint);

	int iSuccessRate = 0;

	if(AddPoint <= 10)
	{
		iSuccessRate = 100;
	}
	else if((AddPoint - 10) < (MaxAddPoint * (double)0.1))
	{
		iSuccessRate = 90;
	}
	else if((AddPoint - 10) < (MaxAddPoint * (double)0.3))
	{
		iSuccessRate = 80;
	}
	else if((AddPoint - 10) < (MaxAddPoint * (double)0.5))
	{
		iSuccessRate = 70;
	}
	else if((AddPoint - 10) < (MaxAddPoint * (double)0.8))
	{
		iSuccessRate = 60;
	}
	else
	{
		iSuccessRate = 50;
	}

	int iIncStat = 0;
	int bSuccess = 0;

	if(rand()%100 < iSuccessRate)
	{
		bSuccess = 1;
	}

	if(bSuccess != 0)
	{
		iSuccessRate = rand()%100;

		int AddStat = 0;

		if(iSuccessRate < 70)
		{
			AddStat = 1;
		}
		else if(iSuccessRate < 95)
		{
			AddStat = 2;
		}
		else
		{
			AddStat = 3;
		}

		if((AddStat + AddPoint) > MaxAddPoint)
		{
			AddStat = 1;
		}

		pResult.result = 0;
		pResult.btStatValue = AddStat;
		pResult.btFruitType = iItemLevel;

		gSendProto.DataSend(aIndex,(unsigned char *)&pResult,pResult.h.size);
		iIncStat = AddStat;

		CLog.LogAdd("[StatUp] [%s][%s] Success [%d][%d] %d",
			gObj[aIndex].AccountID,gObj[aIndex].Name,iItemLevel,iIncStat,iItemSerial);
	}
	else
	{
		pResult.result = 1;
		pResult.btStatValue = 0;
		pResult.btFruitType = iItemLevel;

		gSendProto.DataSend(aIndex,(unsigned char *)&pResult,pResult.h.size);

		CLog.LogAdd("[StatUp] [%s][%s] Fail [%d] %d",
			gObj[aIndex].AccountID,gObj[aIndex].Name,iItemLevel,iItemSerial);
		return;
	}

	switch(iItemLevel)
	{
	case 4: gObj[aIndex].Leadership += iIncStat;	break;
	case 3: gObj[aIndex].Strength += iIncStat;	break;
	case 2: gObj[aIndex].Dexterity += iIncStat;	break;
	case 1:
		gObj[aIndex].Vitality += iIncStat;
		gObj[aIndex].MaxLife += gObj[aIndex].VitalityToLife * iIncStat;
		break;
	case 0:
		gObj[aIndex].Energy += iIncStat;
		gObj[aIndex].MaxMana += gObj[aIndex].EnergyToMana * iIncStat;
		gObjSetBP(aIndex);
		break;
	default : break;
	}

	gCalCharacter.gObjCalCharacter(aIndex);
	GCReFillSend(gObj[aIndex].m_Index,gObj[aIndex].MaxLife + gObj[aIndex].AddLife,0xFE,0,gObj[aIndex].iMaxShield + gObj[aIndex].iAddShield);
	gObjSetBP(aIndex);
	GCManaSend(gObj[aIndex].m_Index,gObj[aIndex].MaxMana + gObj[aIndex].AddMana,0xFE,0,gObj[aIndex].MaxBP + gObj[aIndex].AddBP);
}

void gObjUseMinusStatFruit(int aIndex, int pos)
{
	if(!OBJMAX_RANGE(aIndex))
	{
		return;
	}

	if(gObj[aIndex].Class >= MAX_CLASS_TYPE || gObj[aIndex].Class < 0)
	{
		return;
	}

	int iItemLevel = gObj[aIndex].pInventory[pos].m_Level;
	int iItemSerial = gObj[aIndex].pInventory[pos].m_Number;
	int iCharacterClass = gObj[aIndex].Class;

	PMSG_USE_STAT_FRUIT pResult;

	C1HeadSet((LPBYTE)&pResult,0x2C,sizeof(pResult));

	pResult.result = 5;
	pResult.btFruitType = iItemLevel;
	pResult.btStatValue = 0;

	if(gObj[aIndex].Level <= 9 || iItemLevel > 4)
	{
		pResult.result = 5;
		gSendProto.DataSend(aIndex,(unsigned char *)&pResult,pResult.h.size);
		return;
	}

	if(iItemLevel == 4 && gObj[aIndex].Class != CLASS_DARKLORD )
	{
		pResult.result = 5;
		gSendProto.DataSend(aIndex,(unsigned char *)&pResult,pResult.h.size);
		return;
	}

	for(int iItemIndex = 0; iItemIndex < INVETORY_WEAR_SIZE; iItemIndex++)
	{
		if(gObj[aIndex].pInventory[iItemIndex].IsItem())
		{
			pResult.result = 16;
			gSendProto.DataSend(aIndex,(unsigned char *)&pResult,pResult.h.size);
			return;
		}
	}

	int iDefaultStat = 0;
	int iPresentStat = 0;
	int bEnableUseFruit = 1;

	if(iItemLevel == 0)
	{
		iDefaultStat = gCharInfo.sCharInfo[iCharacterClass].Energy;
		iPresentStat = gObj[aIndex].Energy;

		if(iPresentStat <= iDefaultStat)
		{
			bEnableUseFruit = 0;
		}
	}
	else if(iItemLevel == 1)
	{
		iDefaultStat = gCharInfo.sCharInfo[iCharacterClass].Vitality;
		iPresentStat = gObj[aIndex].Vitality;

		if(iPresentStat <= iDefaultStat)
		{
			bEnableUseFruit = 0;
		}
	}
	else if(iItemLevel == 2)
	{
		iDefaultStat = gCharInfo.sCharInfo[iCharacterClass].Dexterity;
		iPresentStat = gObj[aIndex].Dexterity;

		if(iPresentStat <= iDefaultStat)
		{
			bEnableUseFruit = 0;
		}
	}
	else if(iItemLevel == 3)
	{
		iDefaultStat = gCharInfo.sCharInfo[iCharacterClass].Strength;
		iPresentStat = gObj[aIndex].Strength;

		if(iPresentStat <= iDefaultStat)
		{
			bEnableUseFruit = 0;
		}
	}
	else if(iItemLevel == 4)
	{
		iDefaultStat = gCharInfo.sCharInfo[iCharacterClass].Leadership;
		iPresentStat = gObj[aIndex].Leadership;

		if(iPresentStat <= iDefaultStat)
		{
			bEnableUseFruit = 0;
		}
	}

	if(bEnableUseFruit == 0)
	{
		pResult.result = 38;
		gSendProto.DataSend(aIndex,(unsigned char *)&pResult,pResult.h.size);
		return;
	}

	short AddPoint;
	short MaxAddPoint;
	short MinusPoint;
	short MaxMinusPoint = 0;

	gObjGetStatPointState(aIndex,(short &)AddPoint,(short &)MaxAddPoint,(short &)MinusPoint,(short &)MaxMinusPoint);

	if(MinusPoint >= MaxMinusPoint || MinusPoint < 0)
	{
		pResult.result = 37;
		gSendProto.DataSend(aIndex,(unsigned char *)&pResult,pResult.h.size);
		return;
	}

	CLog.LogAdd("[StatDown] [%s][%s] Use StatDownItem Level:%d Pos:%d serial:%d", gObj[aIndex].AccountID,gObj[aIndex].Name,iItemLevel,pos,iItemSerial);

	gObjInventoryItemSet(aIndex,pos,0xFF);
	gObj[aIndex].pInventory[pos].Clear();
	GCInventoryItemDeleteSend(aIndex,pos,1);

	int iDecStat = 0;
	int bSuccess = 0;

	int iSuccessRate = 100 - gObj[aIndex].Level / 6;

	if(iSuccessRate < 0)
	{
		iSuccessRate = 0;
	}

	if(rand()%100 < iSuccessRate)
	{
		bSuccess = 1;
	}

	if(MinusPoint <= 10)
	{
		bSuccess = 1;
	}

	if(bSuccess != 0)
	{
		iSuccessRate = rand()%100;

		if(iSuccessRate < 50)
		{
			iDecStat = 1;
		}
		else if(iSuccessRate < 75)
		{
			iDecStat = 3;
		}
		else if(iSuccessRate < 91)
		{
			iDecStat = 5;
		}
		else if(iSuccessRate < 98)
		{
			iDecStat = 7;
		}
		else
		{
			iDecStat = 9;
		}

		if((MinusPoint + iDecStat) >= MaxMinusPoint)
		{
			iDecStat = MaxMinusPoint - MinusPoint;
		}

		if((iPresentStat - iDecStat) < iDefaultStat)
		{
			iDecStat = iPresentStat - iDefaultStat;
		}

		pResult.result = 3;
		pResult.btFruitType = iItemLevel;
		pResult.btStatValue = iDecStat;

		gSendProto.DataSend(aIndex,(unsigned char *)&pResult,pResult.h.size);
		CLog.LogAdd("[StatDown] [%s][%s] Success [%d][%d] %d", gObj[aIndex].AccountID,gObj[aIndex].Name,iItemLevel,iDecStat,iItemSerial);
	}
	else
	{
		pResult.result = 4;
		pResult.btFruitType = iItemLevel;
		pResult.btStatValue = 0;
		gSendProto.DataSend(aIndex,(unsigned char *)&pResult,pResult.h.size);
		CLog.LogAdd("[StatDown] [%s][%s] Fail [%d] %d", gObj[aIndex].AccountID,gObj[aIndex].Name,iItemLevel,iItemSerial);
		return;
	}

	switch(iItemLevel)
	{
	case 4:	gObj[aIndex].Leadership -= iDecStat;	break;
	case 3: gObj[aIndex].Strength -= iDecStat;	break;
	case 2: gObj[aIndex].Dexterity -= iDecStat;	break;
	case 1:

		gObj[aIndex].Vitality -= iDecStat;
		gObj[aIndex].MaxLife -= gObj[aIndex].VitalityToLife *iDecStat;
		break;

	case 0:
		gObj[aIndex].Energy -= iDecStat;
		gObj[aIndex].MaxMana -= gObj[aIndex].EnergyToMana * iDecStat;
		gObjSetBP(aIndex);
		break;

	default:
		break;
	}

	int iOldLevelUpPoint = gObj[aIndex].LevelUpPoint;
	gObj[aIndex].LevelUpPoint += iDecStat;

	int iOldFruitPoint = gObj[aIndex].LevelUpPoint;
	gObj[aIndex].iFruitPoint += iDecStat;

	CLog.LogAdd("[StatDown] [%s][%s] MinusStat[%d] -> LevelUpPoint Old(%d)/New(%d)  FruitPoint Old(%d)/New(%d)", gObj[aIndex].AccountID,gObj[aIndex].Name,iDecStat,iOldLevelUpPoint,gObj[aIndex].LevelUpPoint,iOldFruitPoint,gObj[aIndex].iFruitPoint);

	gCalCharacter.gObjCalCharacter(aIndex);
	GCReFillSend(gObj[aIndex].m_Index,gObj[aIndex].MaxLife + gObj[aIndex].AddLife,0xFE,0,gObj[aIndex].iMaxShield+gObj[aIndex].iAddShield);
	gObjSetBP(aIndex);
	GCManaSend(gObj[aIndex].m_Index,gObj[aIndex].MaxMana + gObj[aIndex].AddMana,0xFE,0,gObj[aIndex].MaxBP);
}

// ------------------------------------------------------------
// [Sunday, 20 Feb, 2011] M.E.S ~ Calculate Maximum Life Power
// ------------------------------------------------------------
void gObjCalcMaxLifePower(int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];
	// ------
	switch(lpObj->Class)
	{
	case CLASS_WIZARD:		
		lpObj->m_MaxLifePower = lpObj->Level + lpObj->Vitality + 110;	
		break;
		// ------
	case CLASS_KNIGHT:		
		lpObj->m_MaxLifePower = (lpObj->Level * 2) + lpObj->Vitality + 60;	
		break;
		// ------
	case CLASS_ELF:			
		lpObj->m_MaxLifePower = lpObj->Level + lpObj->Vitality + 80;	
		break;
		// ------
	case CLASS_MAGUMSA:		
		lpObj->m_MaxLifePower = lpObj->Level + lpObj->Vitality + 110;	
		break;
		// ------
	case CLASS_DARKLORD:	
		lpObj->m_MaxLifePower = lpObj->Level + lpObj->Vitality + 110;	
		break;
		// ------
	case CLASS_SUMMONER:	
		lpObj->m_MaxLifePower = lpObj->Level + lpObj->Vitality + 110;	
		break;
		// ------
	case CLASS_FIGHTER:		
		lpObj->m_MaxLifePower = (lpObj->Level * 2) + lpObj->Vitality + 60;	
		break;
		// ------
	default :
		lpObj->m_MaxLifePower = 0;
		break;
	}
}

void gObjDelayLifeCheck(int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];

	if(lpObj->m_CheckLifeTime > 0)
	{
		lpObj->m_CheckLifeTime--;

		if(lpObj->m_CheckLifeTime <= 0)
		{
			lpObj->m_CheckLifeTime = 0;

			if(lpObj->Life < 0)
			{
				lpObj->Life = 0;
			}

			if(lpObj->lpAttackObj != 0)
			{
				gObjLifeCheck(lpObj,lpObj->lpAttackObj,0,1,0,0,0,0);
			}
		}
	}
}

void gObjCheckAllUserDuelStop()
{
	for(int i = OBJ_STARTUSERINDEX; i < OBJMAX; i++)
	{
		if(gObj[i].Connected == PLAYER_PLAYING)
		{
			if(gObj[i].Type == OBJ_USER)
			{
				if(OBJMAX_RANGE(gObj[i].m_iDuelUser))
				{
					if(GetTickCount() - gObj[i].m_iDuelTickCount > 60000)
					{
						//gObjSendDuelEnd(&gObj[gObj[i].m_iDuelUser]);
						//gObjSendDuelEnd(&gObj[i]);
						//gObjResetDuel(&gObj[i]);
					}
				}
			}
		}
	}
}
// Don't Know What is This
void SkillFrustrum(BYTE bangle, int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];

	float p[MAX_ARRAY_FRUSTRUM][3];

	p[0][0] = -6;
	p[0][1] = 6;
	p[0][2] = 0;
	p[1][0] = 6;
	p[1][1] = 6;
	p[1][2] = 0;
	p[2][0] = 1;
	p[2][1] = 0;
	p[2][2] = 0;
	p[3][0] = -1;
	p[3][1] = 0;
	p[3][2] = 0;

	float Angle[0x3];

	Angle[0] = 0;
	Angle[1] = 0;
	Angle[2] = bangle * 360 / 255;

	float Matrix[3][MAX_ARRAY_FRUSTRUM];

	AngleMatrix(Angle,Matrix);

	float vFrustrum[MAX_ARRAY_FRUSTRUM][3];

	for(int i = 0; i < MAX_ARRAY_FRUSTRUM; i++)
	{
		VectorRotate((const float *)&p[i],(const float *)&Matrix[0],(float *)&vFrustrum[i]);

		lpObj->fSkillFrustrumX[i] = (int)vFrustrum[i][0] + lpObj->X;
		lpObj->fSkillFrustrumY[i] = (int)vFrustrum[i][1] + lpObj->Y;
	}
}
// Don't Know What is This
bool SkillTestFrustrum(int x, int y,int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];

	int j = 3;

	for(int i = 0; i < MAX_ARRAY_FRUSTRUM; j = i,i++)
	{
		if((lpObj->fSkillFrustrumX[i]- x) * (lpObj->fSkillFrustrumY[j]-y) - (lpObj->fSkillFrustrumX[j]-x) * (lpObj->fSkillFrustrumY[i]-y) < 0.0f)
		{
			return false;
		}
	}
	return true;
}

// --------------------------------------------------
// [Sunday, 20 Feb, 2011] WolF ~ Check Maximum Zen
// --------------------------------------------------
bool gObjCheckMaxZen(int aIndex, int nAddZen)
{
	LPOBJ lpObj = &gObj[aIndex];
	// ------
	if( !OBJMAX_RANGE(aIndex) ) return false;
	// ------
	__int64 Money = (__int64)lpObj->Money + (__int64)nAddZen;
	// ------
	if ( Money > (__int64)MAX_ZEN )
	{
		SendMsg.MessageOut(aIndex, 0x01, "You have Exceeded The Maximum Zen.");
		return false;
	}
	// ------
	return true;
}

void MakeRandomSetItem(int aIndex)
{
	int itemnum = gSetItemOption.GenRandomItemNum();

	int SetOption = gSetItemOption.GenSetOption(itemnum);

	int option1rand;
	int option2rand;
	int option3rand;
	int optionc;
	int Option1 = 0;
	int Option2 = 0;
	int Option3 = 0;
	BYTE Socket[5];
	Socket[0] = 0xFF;
	Socket[1] = 0xFF;
	Socket[2] = 0xFF;
	Socket[3] = 0xFF;
	Socket[4] = 0xFF;

	option1rand = 6;
	option2rand = 4;

	option3rand = rand()%100;
	optionc = rand()%3;

	if(rand()%100 < option2rand)
	{
		Option2 = 1;
	}

	switch(optionc)
	{
	case 0: 
		if(option3rand < 4)
		{
			Option3 = 3;
		}
		break;
	case 1:
		if(option3rand < 8)
		{
			Option3 = 2;
		}
		break;
	case 2: 
		if(option3rand < 12)
		{
			Option3 = 1;
		}
		break;
	default: break;
	}

	Option1 = 1;

	ItemSerialCreateSend(aIndex,gObj[aIndex].MapNumber,(BYTE)gObj[aIndex].X,(BYTE)gObj[aIndex].Y,itemnum,0,0,Option1,Option2,Option3,aIndex,0,SetOption, Socket);
	CLog.LogAdd("[Chaos Castle] [%s][%s] Winner Set Item itemnum:[%d] skill:[%d] luck:[%d] option:[%d] SetOption:[%d]",gObj[aIndex].AccountID,gObj[aIndex].Name,
		itemnum,Option1,Option2,Option3,SetOption);
}

void MakeRewardSetItem(int aIndex, BYTE cDropX, BYTE cDropY, int iRewardType, BYTE iMapnumber)
{
	int itemnum = gSetItemOption.GenRandomItemNum();

	int SetOption = gSetItemOption.GenSetOption(itemnum);

	int option1rand;
	int option2rand;
	int option3rand;
	int optionc;
	int Option1 = 0;
	int Option2 = 0;
	int Option3 = 0;
	BYTE Socket[5];
	Socket[0] = 0xFF;
	Socket[1] = 0xFF;
	Socket[2] = 0xFF;
	Socket[3] = 0xFF;
	Socket[4] = 0xFF;

	option1rand = 6;
	option2rand = 4;

	option3rand = rand()%100;
	optionc = rand()%3;

	if(rand()%100 < option2rand)
	{
		Option2 = 1;
	}

	switch(optionc)
	{
	case 0: 
		if(option3rand < 4)
		{
			Option3 = 3;
		}
		break;
	case 1:
		if(option3rand < 8)
		{
			Option3 = 2;
		}
		break;
	case 2: 
		if(option3rand < 12)
		{
			Option3 = 1;
		}
		break;
	default: break;
	}

	Option1 = 1;

	if(cDropX == 0 && cDropY == 0)
	{
		cDropX = (BYTE)gObj[aIndex].X;
		cDropY = (BYTE)gObj[aIndex].Y;
	}

	ItemSerialCreateSend(aIndex,iMapnumber,cDropX,cDropY,itemnum,0,0,Option1,Option2,Option3,aIndex,0,SetOption, Socket);

	if(iRewardType == 1)
	{
		CLog.LogAdd("[★☆Reward][KUNDUN] [%s][%s] Set Item itemnum:[%d] skill:[%d] luck:[%d] option:[%d] SetOption:[%d]",
			gObj[aIndex].AccountID,gObj[aIndex].Name,itemnum,Option1,Option2,Option3,SetOption);
	}
	else
	{
		CLog.LogAdd("[Reward][Etc] [%s][%s] Set Item itemnum:[%d] skill:[%d] luck:[%d] option:[%d] SetOption:[%d]",
			gObj[aIndex].AccountID,gObj[aIndex].Name,itemnum,Option1,Option2,Option3,SetOption);
	}		
}

void gObjRecall(int aIndex, int mapnumber, int x, int y)
{
	if(gObj[aIndex].MapNumber != mapnumber)
	{
		gObj[aIndex].X = x;
		gObj[aIndex].Y = y;

		gObj[aIndex].TX = x;
		gObj[aIndex].TY = y;

		gObj[aIndex].MapNumber = mapnumber;

		gObj[aIndex].PathCount = 0;
		gObj[aIndex].Teleport = 0;

		gObjClearViewport(&gObj[aIndex]);
		GCTeleportSend(&gObj[aIndex],-1,mapnumber,(BYTE)gObj[aIndex].X,(BYTE)gObj[aIndex].Y,gObj[aIndex].Dir);

		if(gObj[aIndex].m_Change >= 0)
		{
			gObjViewportListProtocolCreate(&gObj[aIndex]);
		}

		gObj[aIndex].RegenMapNumber = mapnumber;
		gObj[aIndex].RegenMapX = x;
		gObj[aIndex].RegenMapY = y;
		gObj[aIndex].RegenOk = 1;
	}
	else
	{
		PMSG_MAGICATTACK_RESULT pAttack;

		C3HeadSet((unsigned char *)&pAttack,0x19,sizeof(pAttack));

		pAttack.MagicNumberH = SET_NUMBERH(6);
		pAttack.MagicNumberL = SET_NUMBERL(6);
		pAttack.SourceNumberH = SET_NUMBERH(aIndex);
		pAttack.SourceNumberL = SET_NUMBERL(aIndex);
		pAttack.TargetNumberH = SET_NUMBERH(aIndex);
		pAttack.TargetNumberL = SET_NUMBERL(aIndex);

		gSendProto.DataSend(aIndex,(unsigned char *)&pAttack,pAttack.h.size);
		gSendProto.VPMsgSend(&gObj[aIndex],(unsigned char*)&pAttack,pAttack.h.size);
		gObjTeleportMagicUse(aIndex,x,y);
	}
}

void gObjSetExpPetItem(int aIndex, int exp)
{
	LPOBJ lpObj = &gObj[aIndex];

	if(lpObj->Class != CLASS_DARKLORD )	
	{
		return;
	}

	int NewExp = 0;

	if( lpObj->pInventory[8].m_Type == ITEMGET(13,4) )
	{
		NewExp = exp * gPets.DarkHorseAddExperience;
		int addexp = NewExp * 20 / 100;

		if( lpObj->pInventory[8].AddPetItemExp(addexp) == true )
		{
			if(gObjIsItemPut(lpObj,&lpObj->pInventory[8],8) == 0)
			{
				lpObj->pInventory[8].PetItemLevelDown(addexp);
				MsgOutput(aIndex,lMsg.Get(1246));
			}
			else
			{
				CLog.LogAdd("[%s][%s][PetItemLevelUp] [%s] Level:[%d]Exp:[%d]AddExp:[%d]",
					lpObj->AccountID,lpObj->Name,lpObj->pInventory[8].GetName(),lpObj->pInventory[8].m_PetItem_Level,lpObj->pInventory[8].m_PetItem_Exp,addexp);
				CDarkSpirit::SendLevelmsg(lpObj->m_Index,8,1,0xFE);
			}
		}
	}
	if( lpObj->pInventory[1].m_Type == ITEMGET(13,5) )
	{
		NewExp = exp * gPets.DarkSpiritAddExperience;
		int addexp = NewExp * 20 / 100;

		if( lpObj->pInventory[1].AddPetItemExp(addexp) == true )
		{
			if(gObjIsItemPut(lpObj,&lpObj->pInventory[1],1) == 0)
			{
				lpObj->pInventory[1].PetItemLevelDown(addexp);
				MsgOutput(aIndex,lMsg.Get(1245));
			}
			else
			{
				CLog.LogAdd("[%s][%s][PetItemLevelUp] [%s] Level:[%d]Exp:[%d]AddExp:[%d]",
					lpObj->AccountID,lpObj->Name,lpObj->pInventory[1].GetName(),lpObj->pInventory[1].m_PetItem_Level,lpObj->pInventory[1].m_PetItem_Exp,addexp);
				CDarkSpirit::SendLevelmsg(lpObj->m_Index,1,0,0xFE);
			}
		}
	}
}

bool gObjGetRandomItemDropLocation(int iMapNumber, BYTE& cX, BYTE& cY, int iRangeX, int iRangeY, int iLoopCount)
{
	int iUX = cX;
	int iUY = cY;

	if ( iRangeX <= 0 )
	{
		iRangeX = 1;
	}

	if ( iRangeY <= 0 )
	{
		iRangeY = 1;
	}

	if ( iLoopCount <= 0 )
	{
		iLoopCount = 1;
	}

	while ( iLoopCount-- > 0 )
	{
		cX = ( rand() % (iRangeX+1) ) * (((rand()%2==0)?-1:1)) + iUX;
		cY = ( rand() % (iRangeY+1) ) * (((rand()%2==0)?-1:1)) + iUY;

		BYTE attr = MapC[iMapNumber].GetAttr(cX, cY);

		if ( (attr&4) != 4 && (attr&8) != 8)
		{
			return true;
		}
	}

	return false;
}

bool gObjGetRandomFreeLocation(int iMapNumber, BYTE &cX, BYTE & cY, int iRangeX, int iRangeY, int iLoopCount)
{
	int iUX = cX;
	int iUY = cY;

	if ( iRangeX <= 0 )
	{
		iRangeX = 1;
	}

	if ( iRangeY <= 0 )
	{
		iRangeY = 1;
	}

	if ( iLoopCount <= 0 )
	{
		iLoopCount = 1;
	}

	while ( iLoopCount-- > 0 )
	{
		cX = ( rand() % (iRangeX+1) ) * (((rand()%2==0)?-1:1)) + iUX;
		cY = ( rand() % (iRangeY+1) ) * (((rand()%2==0)?-1:1)) + iUY;

		BYTE attr = MapC[iMapNumber].GetAttr(cX, cY);

		if ( attr == 0 )
		{
			return true;
		}
	}

	return false;
}

int gObjCheckAttackTypeMagic(int iClass, int iSkill)
{
	if ( iSkill == 0 )
	{
		return 0;
	}
	if ( iSkill < 300 )
	{
		return MagicDamageC.GetSkillAttr2(iSkill);
	}
	else
	{
		return gSkillTree.GetMasterSkillType(iSkill);
	}
}

int gObjGetGuildUnionNumber(LPOBJ lpObj)
{
	int iUnion=0;

	if ( lpObj->lpGuild != NULL )
	{
		iUnion = (lpObj->lpGuild->iGuildUnion == 0)? lpObj->lpGuild->Number : lpObj->lpGuild->iGuildUnion;
	}

	return iUnion;

}

void gObjGetGuildUnionName(LPOBJ lpObj, char* szUnionName, int iUnionNameLen)
{
	szUnionName[0] = 0;
	
	if ( lpObj->lpGuild == NULL )
	{
		return;
	}

	if ( lpObj->lpGuild->iGuildUnion == 0 )
	{
		return;
	}

	TUnionInfo * pUnionInfo = UnionManager.SearchUnion(lpObj->lpGuild->iGuildUnion);

	if ( pUnionInfo != NULL )
	{
		memcpy(szUnionName, pUnionInfo->m_szMasterGuild, iUnionNameLen);
	}
}

BOOL gObjCheckRival(LPOBJ lpObj, LPOBJ lpTargetObj)
{
	if(lpTargetObj->Type != OBJ_USER)
	{
		return false;
	}

	if(lpObj->Type != OBJ_USER)
	{
		return false;
	}

	if(lpTargetObj->lpGuild == 0 || lpObj->lpGuild == 0)
	{
		return false;
	}

	if(lpTargetObj->lpGuild->iGuildRival == 0 || lpObj->lpGuild->iGuildRival == 0)
	{
		return false;
	}

	int iUnion = !lpObj->lpGuild->iGuildUnion ? lpObj->lpGuild->Number : lpObj->lpGuild->iGuildUnion;

	if(UnionManager.GetGuildRelationShip(iUnion,lpTargetObj->lpGuild->Number) == 2)
	{
		return true;
	}
	return false;
}

int gObjGetRelationShip(LPOBJ lpObj, LPOBJ lpTargetObj)
{
	if ( lpObj == NULL || lpTargetObj == NULL )
	{
		return false;
	}

	if ( lpObj->lpGuild == NULL || lpTargetObj->lpGuild == NULL )
	{
		return 0;
	}

	int iUnion = (!lpObj->lpGuild->iGuildUnion) ? lpObj->lpGuild->Number : lpObj->lpGuild->iGuildUnion;

	return UnionManager.GetGuildRelationShip(iUnion, lpTargetObj->lpGuild->Number);
}

void gObjNotifyUpdateUnionV1(LPOBJ lpObj)
{
	if(lpObj == 0)
	{
		return;
	}

	char cBUFFER_V1[6000];
	int iVp1Count = 0;

	memset(cBUFFER_V1,0x00,sizeof(cBUFFER_V1));

	PMSG_UNION_VIEWPORT_NOTIFY_COUNT * lpMsg = (PMSG_UNION_VIEWPORT_NOTIFY_COUNT *)&cBUFFER_V1;
	PMSG_UNION_VIEWPORT_NOTIFY * lpMsgBody = (PMSG_UNION_VIEWPORT_NOTIFY * )&cBUFFER_V1[sizeof(PMSG_UNION_VIEWPORT_NOTIFY_COUNT)];


	for(int n = 0; n < MAX_VIEWPORT; n++)
	{
		if(!OBJMAX_RANGE(lpObj->VpPlayer[n].number))
		{
			continue;
		}

		if(lpObj->VpPlayer[n].state == 2 && lpObj->VpPlayer[n].type == OBJ_USER)
		{
			LPOBJ lpTargetObj = &gObj[lpObj->VpPlayer[n].number];

			if(lpTargetObj == 0)
			{
				continue;
			}

			if(lpTargetObj->lpGuild == 0)
			{
				continue;
			}

			gObjGetGuildUnionName(lpTargetObj,lpMsgBody[iVp1Count].szUnionName,sizeof(lpMsgBody[iVp1Count].szUnionName));

			lpMsgBody[iVp1Count].btGuildRelationShip = gObjGetRelationShip(lpObj,lpTargetObj);
			lpMsgBody[iVp1Count].btNumberL = SET_NUMBERL(WORD(lpTargetObj->m_Index));
			lpMsgBody[iVp1Count].btNumberH = SET_NUMBERH(WORD(lpTargetObj->m_Index));
			lpMsgBody[iVp1Count].iGuildNumber = lpTargetObj->lpGuild->Number;
			iVp1Count++;
		}
	}

	if(iVp1Count > 0 && iVp1Count <= MAX_VIEWPORT)
	{
		lpMsg->btCount = iVp1Count;

		C2HeadSet((unsigned char *)lpMsg,0x67,iVp1Count * sizeof(PMSG_UNION_VIEWPORT_NOTIFY) + sizeof(PMSG_UNION_VIEWPORT_NOTIFY_COUNT));

		gSendProto.DataSend(lpObj->m_Index,(unsigned char *)lpMsg,((lpMsg->h.sizeL & 0xFF) & 0xFF | ((lpMsg->h.sizeH & 0xFF) & 0xFF) << 8) & 0xFFFF);
	}
	else
	{
		if(iVp1Count != 0)
		{
			CLog.LogAdd("[Union ViewPort] ERROR : iVp1Count is OUT of BOUND: %d",iVp1Count);
		}
	}
}

void gObjNotifyUpdateUnionV2(LPOBJ lpObj)
{
	if(lpObj == 0)
	{
		return;
	}

	if(lpObj->lpGuild == 0)
	{
		return;
	}

	char cBUFFER_V2[100] = {0};

	PMSG_UNION_VIEWPORT_NOTIFY_COUNT * lpMsg2 = (PMSG_UNION_VIEWPORT_NOTIFY_COUNT *)&cBUFFER_V2;
	PMSG_UNION_VIEWPORT_NOTIFY * lpMsgBody2 = (PMSG_UNION_VIEWPORT_NOTIFY *)&cBUFFER_V2[sizeof(PMSG_UNION_VIEWPORT_NOTIFY_COUNT)];

	gObjGetGuildUnionName(lpObj,lpMsgBody2->szUnionName,sizeof(lpMsgBody2->szUnionName));
	lpMsgBody2->btNumberL = SET_NUMBERL(WORD(lpObj->m_Index));
	lpMsgBody2->btNumberH = SET_NUMBERH(WORD(lpObj->m_Index));

	lpMsgBody2->iGuildNumber = lpObj->lpGuild->Number;
	lpMsgBody2->btGuildRelationShip = 0;
	lpMsg2->btCount = 1;

	C2HeadSet((unsigned char *)lpMsg2,0x67,sizeof(lpMsg2) + sizeof(lpMsgBody2[0])+1);

	gSendProto.DataSend(lpObj->m_Index,(unsigned char *)lpMsg2,((lpMsg2->h.sizeL & 0xFF) & 0xFF | ((lpMsg2->h.sizeH & 0xFF) & 0xFF) << 8) & 0xFFFF);

	for(int n = 0; n < MAX_VIEWPORT; n++)
	{
		if(lpObj->VpPlayer2[n].type == OBJ_USER && lpObj->VpPlayer2[n].state != 0)
		{
			LPOBJ lpTargetObj = &gObj[lpObj->VpPlayer2[n].number];

			if(lpTargetObj->lpGuild != 0)
			{
				lpMsgBody2->btGuildRelationShip = gObjGetRelationShip(lpTargetObj,lpObj);
			}

			if(lpMsgBody2->btGuildRelationShip != 1)
			{
				gSendProto.DataSend(lpObj->VpPlayer2[n].number,(unsigned char *)lpMsg2,((lpMsg2->h.sizeL & 0xFF) & 0xFF | ((lpMsg2->h.sizeH & 0xFF) & 0xFF) << 8) & 0xFFFF);
			}
		}
	}
}

void gObjUnionUpdateProc(int iIndex)
{

	if ( gObjIsConnected(iIndex) == FALSE )
	{
		return;
	}

	LPOBJ lpObj = &gObj[iIndex];

	if ( lpObj->Type != OBJ_USER )
	{
		return;
	}

	if ( lpObj->RegenOk > 0 )
	{
		return;
	}

	if ( lpObj->CloseCount > -1 )
		return;

	if ( lpObj->lpGuild == NULL )
	{
		return;
	}

	if ( lpObj->lpGuild->CheckTimeStamp(lpObj->iGuildUnionTimeStamp) != FALSE )
	{
		return;
	}

	lpObj->iGuildUnionTimeStamp = lpObj->lpGuild->GetTimeStamp();
	gObjNotifyUpdateUnionV1(lpObj);
	gObjNotifyUpdateUnionV2(lpObj);

}

void gObjSetKillCount(int aIndex, int iOption)	// Option : [0 : SetToZero] [1 : Increase] [2 : Decrease]
{
	if ( gObjIsConnected(aIndex) == FALSE )
	{
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if ( iOption== 1 )
	{
		if ( lpObj->m_btKillCount <= 254 )
		{
			lpObj->m_btKillCount++;
		}
	}
	else if ( iOption == 2 )
	{
		if ( lpObj->m_btKillCount > 0 )
		{
			lpObj->m_btKillCount--;
		}
	}
	else if ( iOption == 0 )
	{
		lpObj->m_btKillCount = 0;
	}

	PMSG_KILLCOUNT pMsg = {0};
	C1SubHeadSet((LPBYTE)&pMsg, 0xB8,0x01, sizeof(pMsg));
	pMsg.btKillCount = lpObj->m_btKillCount;
	
	gSendProto.DataSend( aIndex, (UCHAR*)&pMsg, sizeof(pMsg));
}

void gObjNotifyUseWeaponV1(LPOBJ lpOwnerObj, LPOBJ lpWeaponObj, int iTargetX, int iTargetY)
{
	WORD btViewSkillBuffer[MAX_VIEWSKILL] ={0};

	if(lpOwnerObj == 0)
	{
		return;
	}

	if(lpWeaponObj == 0)
	{
		return;
	}

	PMSG_NOTIFY_REGION_OF_WEAPON pNotifyRegionMsg = {0};

	C1SubHeadSet((unsigned char *)&pNotifyRegionMsg,0xB7,2,sizeof(pNotifyRegionMsg));

	pNotifyRegionMsg.btPointX = iTargetX;
	pNotifyRegionMsg.btPointY = iTargetY;

	PMSG_NOTIFY_TARGET_OF_WEAPON pNotifyTargetMsg = {0};

	C1SubHeadSet((unsigned char *)&pNotifyTargetMsg,0xB7,3,sizeof(pNotifyTargetMsg));

	if(lpWeaponObj->Class == 0xDD)
	{
		pNotifyRegionMsg.btWeaponType = 1;
		pNotifyTargetMsg.btWeaponType = 1;
	}
	else if(lpWeaponObj->Class == 0xDE)
	{
		pNotifyRegionMsg.btWeaponType = 2;
		pNotifyTargetMsg.btWeaponType = 2;
	}

	char cBUFFER_V1[6000];
	int iVp1Count = 0;

	memset(cBUFFER_V1,0x00,sizeof(cBUFFER_V1));

	PMSG_WEAPON_OWNER_VIEWPORT_NOTIFY_COUNT * lpMsg = (PMSG_WEAPON_OWNER_VIEWPORT_NOTIFY_COUNT *)&cBUFFER_V1;
	PMSG_WEAPON_OWNER_VIEWPORT_NOTIFY * lpMsgBody = (PMSG_WEAPON_OWNER_VIEWPORT_NOTIFY *)&cBUFFER_V1[sizeof(PMSG_WEAPON_OWNER_VIEWPORT_NOTIFY_COUNT)];

	for(int n = 0; n < OBJMAX;n++)
	{
		LPOBJ lpTargetObj = &gObj[n];

		if(gObjIsConnected(n) == 0)
		{
			continue;
		}

		if(lpOwnerObj->MapNumber != lpTargetObj->MapNumber)
		{
			continue;
		}

		if(abs(lpTargetObj->X - iTargetX) > 6)
		{
			continue;
		}

		if(abs(lpTargetObj->Y - iTargetY) > 6)
		{
			continue;
		}

		lpMsgBody[iVp1Count].btObjClassH = SET_NUMBERH(lpTargetObj->Class);
		lpMsgBody[iVp1Count].btObjClassL = SET_NUMBERL(lpTargetObj->Class);
		lpMsgBody[iVp1Count].btObjIndexH = SET_NUMBERH(lpTargetObj->m_Index);
		lpMsgBody[iVp1Count].btObjIndexH = SET_NUMBERL(lpTargetObj->m_Index);
		// lpMsgBody[iVp1Count].btObjIndexL = SET_NUMBERL(lpTargetObj->m_Index); #error need some testing here 
		lpMsgBody[iVp1Count].btX = lpTargetObj->X;
		lpMsgBody[iVp1Count].btY = lpTargetObj->Y;

		lpMsgBody[iVp1Count].btViewSkillCount = gObjViewSkillBufferConvert(lpTargetObj, btViewSkillBuffer);

		if ( lpMsgBody[iVp1Count].btViewSkillCount )
		{
			memcpy(lpMsgBody + sizeof(PMSG_WEAPON_OWNER_VIEWPORT_NOTIFY), btViewSkillBuffer, lpMsgBody[iVp1Count].btViewSkillCount);
		}

		if(lpTargetObj->Type == OBJ_USER)
		{
			lpMsgBody[iVp1Count].btObjType = OBJ_USER;
			lpMsgBody[iVp1Count].btObjClassH = CS_SET_CLASS(lpTargetObj->Class);
			lpMsgBody[iVp1Count].btObjClassL = 0;

			memcpy(lpMsgBody[iVp1Count].CharSet,&lpTargetObj->CharSet[1],sizeof(lpMsgBody[iVp1Count].CharSet));
		}
		else if(lpTargetObj->Type == OBJ_MONSTER)
		{
			lpMsgBody[iVp1Count].btObjType = OBJ_MONSTER;
			memset(lpMsgBody[iVp1Count].CharSet,0x00,sizeof(lpMsgBody[iVp1Count].CharSet));
		}
		else
		{
			lpMsgBody[iVp1Count].btObjType = OBJ_NPC;
			memset(lpMsgBody[iVp1Count].CharSet,0x00,sizeof(lpMsgBody[iVp1Count].CharSet));
		}

		iVp1Count++;

		if(lpTargetObj->Type == OBJ_USER)
		{
			gSendProto.DataSend(lpTargetObj->m_Index,(unsigned char *)&pNotifyRegionMsg,sizeof(pNotifyRegionMsg));

			if(abs(lpTargetObj->X - iTargetX) > 3)
			{
				continue;
			}

			if(abs(lpTargetObj->Y - iTargetY) > 3)
			{
				continue;
			}

			pNotifyTargetMsg.NumberH = SET_NUMBERH(lpTargetObj->m_Index);
			pNotifyTargetMsg.NumberL = SET_NUMBERL(lpTargetObj->m_Index);
		}
	}

	if(iVp1Count > 0 && iVp1Count <= 100)
	{
		lpMsg->btCount = iVp1Count;
		C2HeadSet((unsigned char *)lpMsg,0x68,iVp1Count * sizeof(PMSG_WEAPON_OWNER_VIEWPORT_NOTIFY) + sizeof(PMSG_WEAPON_OWNER_VIEWPORT_NOTIFY_COUNT));

		gSendProto.DataSend(lpOwnerObj->m_Index,(unsigned char *)lpMsg,((lpMsg->h.sizeL & 0xFF) & 0xFF | ((lpMsg->h.sizeH & 0xFF) & 0xFF) << 8) & 0xFFFF);
	}
}

void gObjNotifyUseWeaponDamage(LPOBJ lpWeaponObj, int iTargetX, int iTargetY)
{
	if ( lpWeaponObj->Class == 221 || lpWeaponObj->Class == 222)
	{
		for ( int i=0;i<OBJMAX;i++)
		{
			LPOBJ lpObj = &gObj[i];

			if ( lpWeaponObj->MapNumber != lpObj->MapNumber )
			{
				continue;
			}

			if ( abs(lpObj->X - iTargetX) > 3 )
			{
				continue;
			}

			if ( abs(lpObj->Y - iTargetY) > 3 )
			{
				continue;
			}

			g_CsNPC_Weapon.AddWeaponDamagedTargetInfo(lpWeaponObj->m_Index, i, 100);
		}
	}

}

void gObjUseBlessAndSoulPotion(int aIndex, int iItemLevel)
{
	LPOBJ lpObj = &gObj[aIndex];

	if ( iItemLevel == 0 )	// Bless Potion
	{
		lpObj->m_iPotionBlessTime = 120;
		lpObj->m_ViewSkillState[eVS_POTION_BLESS] = 1;
		GCStateInfoSend(lpObj, 1, eVS_POTION_BLESS);
	}
	else if ( iItemLevel == 1 ) // Soul Potion
	{
		lpObj->m_iPotionSoulTime = 60;
		lpObj->m_ViewSkillState[eVS_POTION_SOUL] = 1;
		GCStateInfoSend(lpObj,  1, eVS_POTION_SOUL);
		gObjUseDrink(lpObj, 0x4D);
	}
}
// CHECK THIS FUNCTION
void gObjWeaponDurDownInCastle(LPOBJ lpObj, LPOBJ lpTargetObj, int iDecValue)
{
	if(lpObj->Type == OBJ_MONSTER || lpObj->Type == OBJ_NPC)
	{
		return;
	}

	int itargetdefence = 0;

	CItem * Right = &lpObj->pInventory[0];
	CItem * Left = &lpObj->pInventory[1];

	int bIsRightDurDown = 0;
	int bIsLeftDurDown = 0;


	if(lpObj->Class == CLASS_KNIGHT  || lpObj->Class == CLASS_MAGUMSA  || lpObj->Class == CLASS_DARKLORD || lpObj->Class == CLASS_FIGHTER )
	{
		if(Right->m_Type >= ITEMGET(0,0) && Right->m_Type < ITEMGET(4,0) && Left->m_Type >= ITEMGET(0,0) && Left->m_Type < ITEMGET(4,0))
		{
			bIsRightDurDown = 1;
			bIsLeftDurDown = 1;
		}
	}

	if(Left->m_Type >= ITEMGET(4,0) && Left->m_Type < ITEMGET(4,7) || Left->m_Type <= ITEMGET(4,20) && Left->m_Type <= ITEMGET(4,23))
	{
		bIsLeftDurDown = 1;
	}
	else if(Right->m_Type >= ITEMGET(4,8) && Right->m_Type < ITEMGET(4,15) || Right->m_Type >= ITEMGET(4,16) && Right->m_Type < ITEMGET(5,0))
	{
		bIsRightDurDown = 1;
	}
	else if(Right->m_Type >= 0 && Right->m_Type < ITEMGET(4,0))
	{
		bIsRightDurDown = 1;
	}
	else if(Right->m_Type >= ITEMGET(5,0) && Right->m_Type < ITEMGET(6,0))
	{
		bIsRightDurDown = 1;
	}

	if(bIsRightDurDown != 0)
	{
		int iRet = Right->SimpleDurabilityDown(iDecValue);

		if(iRet != 0)
		{
			GCItemDurSend(lpObj->m_Index,0,Right->m_Durability,0);

			if(iRet == 2)
			{
				gCalCharacter.gObjCalCharacter(lpObj->m_Index);
			}
		}
	}

	if(bIsLeftDurDown != 0)
	{
		int iRet = Right->SimpleDurabilityDown(iDecValue); // again a stupid webzen error ? 

		if(iRet != 0)
		{
			GCItemDurSend(lpObj->m_Index,1,Left->m_Durability,0);

			if(iRet == 2)
			{
				gCalCharacter.gObjCalCharacter(lpObj->m_Index);
			}
		}
	}
}

void gObjReady4Relife(LPOBJ lpObj)
{
	gCalCharacter.gObjCalCharacter(lpObj->m_Index);
	GCReFillSend(lpObj->m_Index,lpObj->MaxLife + lpObj->AddLife,0xFE,0,lpObj->iMaxShield + lpObj->iAddShield);
}

void gObjCheckTimeOutValue(LPOBJ lpObj, DWORD& rNowTick)
{
	long lTick;
	long lSecond;

	lTick = rNowTick - lpObj->m_dwLastCheckTick;
	lSecond = lTick / 1000;

	if(lSecond == 0)
	{
		return;
	}

	lpObj->m_dwLastCheckTick = rNowTick;
}

void MsgOutput(int aIndex, char* msg, ...) 
{
	char szBuffer[512]="";
	va_list pArguments;

	va_start(pArguments, msg );
	vsprintf(&szBuffer[0], msg, pArguments);
	va_end(pArguments);
	
	GCServerMsgStringSend(&szBuffer[0], aIndex, 1);
}

void gProcessAutoRecuperation(LPOBJ lpObj)
{
	if(lpObj->Life == (lpObj->MaxLife + lpObj->AddLife)
		&& lpObj->Mana == (lpObj->MaxMana + lpObj->AddMana)
		&& lpObj->BP == (lpObj->MaxBP + lpObj->AddBP) )
	{
		lpObj->m_iAutoRecuperationTime = GetTickCount();
		return;
	}

	if(( GetTickCount() - lpObj->m_iAutoRecuperationTime ) >= 10000 && ( GetTickCount() - lpObj->m_iAutoRecuperationTime ) < 15000)
	{
		if(lpObj->Life < ( lpObj->MaxLife + lpObj->AddLife ) )
		{
			lpObj->Life += 1.0f;

			if(lpObj->Life > (lpObj->MaxLife + lpObj->AddLife))
			{
				lpObj->Life = lpObj->MaxLife + lpObj->AddLife;
			}

			GCReFillSend(lpObj->m_Index,lpObj->Life,0xFF,0,lpObj->iShield);
		}

		if(lpObj->Mana < ( lpObj->MaxMana + lpObj->AddMana ) || lpObj->BP < ( lpObj->MaxBP + lpObj->AddBP) )
		{
			lpObj->Mana += 1.0f;

			if(lpObj->Mana > (lpObj->MaxMana + lpObj->AddMana))
			{
				lpObj->Mana = lpObj->MaxMana + lpObj->AddMana;
			}

			lpObj->BP += 1;

			if(lpObj->BP > (lpObj->MaxBP + lpObj->AddBP))
			{
				lpObj->BP = lpObj->MaxBP + lpObj->AddBP;
			}

			GCManaSend(lpObj->m_Index,lpObj->Mana,0xFF,0,lpObj->BP);
		}
	}
	else if(( GetTickCount() - lpObj->m_iAutoRecuperationTime ) >= 15000 && ( GetTickCount() - lpObj->m_iAutoRecuperationTime ) < 25000)
	{
		if(lpObj->Life < ( lpObj->MaxLife + lpObj->AddLife ) )
		{
			lpObj->Life += 5.0f;

			if(lpObj->Life > (lpObj->MaxLife + lpObj->AddLife))
			{
				lpObj->Life = lpObj->MaxLife + lpObj->AddLife;
			}

			GCReFillSend(lpObj->m_Index,lpObj->Life,0xFF,0,lpObj->iShield);
		}

		if(lpObj->Mana < ( lpObj->MaxMana + lpObj->AddMana ) || lpObj->BP < ( lpObj->MaxBP + lpObj->AddBP) )
		{
			lpObj->Mana += 5.0f;

			if(lpObj->Mana > (lpObj->MaxMana + lpObj->AddMana))
			{
				lpObj->Mana = lpObj->MaxMana + lpObj->AddMana;
			}

			lpObj->BP += 5;

			if(lpObj->BP > (lpObj->MaxBP + lpObj->AddBP))
			{
				lpObj->BP = lpObj->MaxBP + lpObj->AddBP;
			}

			GCManaSend(lpObj->m_Index,lpObj->Mana,0xFF,0,lpObj->BP);
		}
	}
	else if(( GetTickCount() - lpObj->m_iAutoRecuperationTime ) >= 25000)
	{
		if(lpObj->Life < ( lpObj->MaxLife + lpObj->AddLife ) )
		{
			lpObj->Life += 10.0f;

			if(lpObj->Life > (lpObj->MaxLife + lpObj->AddLife))
			{
				lpObj->Life = lpObj->MaxLife + lpObj->AddLife;
			}

			GCReFillSend(lpObj->m_Index,lpObj->Life,0xFF,0,lpObj->iShield);
		}

		if(lpObj->Mana < ( lpObj->MaxMana + lpObj->AddMana ) || lpObj->BP < ( lpObj->MaxBP + lpObj->AddBP) )
		{
			lpObj->Mana += 10.0f;

			if(lpObj->Mana > (lpObj->MaxMana + lpObj->AddMana))
			{
				lpObj->Mana = lpObj->MaxMana + lpObj->AddMana;
			}

			lpObj->BP += 10;

			if(lpObj->BP > (lpObj->MaxBP + lpObj->AddBP))
			{
				lpObj->BP = lpObj->MaxBP + lpObj->AddBP;
			}

			GCManaSend(lpObj->m_Index,lpObj->Mana,0xFF,0,lpObj->BP);
		}
	}
}

void gObjShieldAutoRefill(LPOBJ lpObj)
{
	int iRefillPoint = 0;
	int iShieldRefillOption = 0;

	if(g_ShieldSystemOn == 0)
	{
		return;
	}

	if(g_ShieldAutoRefillOn == 0)
	{
		return;
	}

	if(g_ShieldAutoRefillOnSafeZone == 1)
	{
		unsigned char btMapAttribute = MapC[lpObj->MapNumber].GetAttr(lpObj->X,lpObj->Y);

		if((btMapAttribute & 1) != 1 && lpObj->m_ItemOptionExFor380.OpRefillOn == 0)
		{
			lpObj->dwShieldAutoRefillTimer = GetTickCount();
			return;
		}
	}

	if(lpObj->iShield >= (lpObj->iMaxShield + lpObj->iAddShield))
	{
		lpObj->dwShieldAutoRefillTimer = GetTickCount();
		return;
	}

	iShieldRefillOption = lpObj->m_ItemOptionExFor380.OpAddRefillSD;

	int iRefillExpression = 0;

	float fRefillExpressionA = (lpObj->iMaxShield + lpObj->iAddShield) / 30;
	float fRefillExpressionB = iShieldRefillOption + 100;

	iRefillExpression = ((fRefillExpressionA * fRefillExpressionB) / 100.0f) / 25.0f;

	unsigned long dwTick = GetTickCount() - lpObj->dwShieldAutoRefillTimer;

	if(dwTick >= 25000)
	{
		iRefillPoint = iRefillExpression * 3; 
	}
	else if(dwTick >= 15000)
	{
		iRefillPoint = (iRefillExpression * 25) / 10;
	}
	else if(dwTick >= 10000)
	{
		iRefillPoint = iRefillExpression * 2;
	}
	else
	{
		return;
	}

	iRefillExpression++;

	if(iRefillPoint == 0)
	{
		return;
	}

	lpObj->iShield += iRefillPoint;

	if(lpObj->iShield > (lpObj->iMaxShield + lpObj->iAddShield))
	{
		lpObj->iShield = lpObj->iMaxShield + lpObj->iAddShield;
	}

	GCReFillSend(lpObj->m_Index,lpObj->Life,0xFF,0,lpObj->iShield);
}

int gObjCheckOverlapItemUsingDur(int iUserIndex, int iMaxOverlapped, int iItemType, int iItemLevel)
{
	for(int x = INVETORY_WEAR_SIZE; x < MAIN_INVENTORY_SIZE; x++)
	{
		if(gObj[iUserIndex].pInventory[x].IsItem() == 1
			&& gObj[iUserIndex].pInventory[x].m_Type == (short)iItemType
			&& gObj[iUserIndex].pInventory[x].m_Level == (short)iItemLevel)
		{
			int iITEM_DUR = gObj[iUserIndex].pInventory[x].m_Durability;

			if((((iITEM_DUR)<0)?FALSE:((iITEM_DUR)>iMaxOverlapped-1)?FALSE:TRUE ))
			{
				return x;
			}
		}
	}
return -1;
}

int gObjOverlapItemUsingDur(class CItem* lpItem, int iMapNumber, int iItemNumber, int iUserIndex, int iMaxOverlapped, int iItemType, int iItemLevel)
{
	for(int iLoop = 0; iLoop < MAIN_INVENTORY_SIZE; iLoop ++)
	{
		int iInventoryIndex = gObjCheckOverlapItemUsingDur(iUserIndex,iMaxOverlapped,iItemType,iItemLevel);
		if(MAIN_INVENTORY_RANGE(iInventoryIndex))
		{
			int iItemDur = gObj[iUserIndex].pInventory[iInventoryIndex].m_Durability + lpItem->m_Durability;

			if(iItemDur <= iMaxOverlapped)
			{
				if(MapC[iMapNumber].ItemGive(iUserIndex,iItemNumber,1) == 1)
				{
					return iInventoryIndex;
				}
			}
			else
			{
				lpItem->m_Durability = iItemDur - iMaxOverlapped;
				gObj[iUserIndex].pInventory[iInventoryIndex].m_Durability = iMaxOverlapped;

				GCItemDurSend(iUserIndex,iInventoryIndex,gObj[iUserIndex].pInventory[iInventoryIndex].m_Durability,0);
			}
		}
		else
		{
			return -1;
		}
	}
	return -1;
}
// Done 100%
bool gCheckSkillDistance(int aIndex, int aTargetIndex, int iSkillNum)
{
	int iSkillDistance;
	// -----
	if ( !g_iSkillDistanceCheck )
	{
		return true;
	}

	if ( iSkillNum == 40 )
	{
		return true;
	}
	// -----
	if ( iSkillNum < 300 )
	{
		iSkillDistance = MagicDamageC.GetSkillDistance(iSkillNum);
	}
	else
	{
		iSkillDistance = gSkillTree.GetMasterSkillDistance(iSkillNum);
	}

	if ( iSkillDistance == -1 )
	{
		return false;
	}

	iSkillDistance += g_iSkillDistanceCheckTemp;
	LPOBJ lpObj = &gObj[aIndex];
	LPOBJ lpTargetObj = &gObj[aTargetIndex];

	if ( abs(lpObj->X - lpTargetObj->X) > iSkillDistance || abs(lpObj->Y - lpTargetObj->Y) > iSkillDistance )
	{
		return false;
	}
	return true;
}

void gObjSaveChaosBoxItemList(LPOBJ lpObj)
{
	unsigned char ExOption[0x8];
	for(int n = 0; n < CHAOS_BOX_SIZE;n++)
	{
		if(lpObj->pChaosBox[n].IsItem() == 1)
		{
			ItemIsBufExOption(ExOption,&lpObj->pChaosBox[n]);

			CLog.LogAdd("[ChaosBoxItemList][Lost ItemList] [%s][%s] [%d,%s,%d,%d,%d,%d] Serial:[%d][%d] Ex:[%d,%d,%d,%d,%d,%d,%d] Set:[%d] 380:[%d] HO:[%d,%d]",
				lpObj->AccountID,lpObj->Name,n,lpObj->pChaosBox[n].GetName(),lpObj->pChaosBox[n].m_Level,lpObj->pChaosBox[n].m_Option1,lpObj->pChaosBox[n].m_Option2,lpObj->pChaosBox[n].m_Option3,lpObj->pChaosBox[n].m_Number,
				(int)lpObj->pChaosBox[n].m_Durability,ExOption[0],ExOption[1],ExOption[2],ExOption[3],ExOption[4],ExOption[5],ExOption[6],lpObj->pChaosBox[n].m_SetOption,
				lpObj->pChaosBox[n].m_ItemOptionEx >> 7,g_kJewelOfHarmonySystem.GetItemStrengthenOption(&lpObj->pChaosBox[n]),g_kJewelOfHarmonySystem.GetItemOptionLevel(&lpObj->pChaosBox[n]));
		}
	}
}

#if (GS_CASTLE)
int gObjGetRandomFreeArea(int iMapNumber,unsigned char & cX,unsigned char & cY,int iSX,int iSY,int iDX,int iDY,int iLoopCount)
{
	if(iSX <= 0)
		iSX = 1;

	if(iSY <= 0)
		iSY = 1;

	if(iDX <= 0)
		iDX = 1;

	if(iDY <= 0)
		iDY = 1;

	if(iSX >= 256)
		iSX = 255;

	if(iSY >= 256)
		iSY = 255;

	if(iDX >= 256)
		iDX = 255;

	if(iDY >= 256)
		iDY = 255;

	if(iLoopCount <= 0)
		iLoopCount = 1;

	while(iLoopCount-- > 0)
	{
		cX = rand()%(iDX-iSX) + iSX;
		cY = rand()%(iDY-iSY) + iSY;

		BYTE btMapAttr = MapC[iMapNumber].GetAttr(cX,cY);

		if(!btMapAttr)
			return TRUE;
	}

	return FALSE;
}

#endif


#if(!GS_CASTLE)
void gObjSetIllusionKillCount(int aIndex, int iOption, int iCount = 1)	// Option : [0 : SetToZero] [1 : Increase] [2 : Decrease]
{

	if ( gObjIsConnected(aIndex) == FALSE )
	{
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if ( iOption== 1 )
	{
		if ( lpObj->m_IllusionKillCount <= 254 )
		{
			lpObj->m_IllusionKillCount += iCount;
		}
	}
	else if ( iOption == 2 )
	{
		if ( lpObj->m_IllusionKillCount > 0 )
		{
			lpObj->m_IllusionKillCount -= iCount;
		}
	}
	else if ( iOption == 0 )
	{
		lpObj->m_IllusionKillCount = 0;
	}

	if(lpObj->m_IllusionKillCount > 255) lpObj->m_IllusionKillCount = 255;
	if(lpObj->m_IllusionKillCount < 0) lpObj->m_IllusionKillCount = 0;

	PMSG_KILLCOUNT pMsg = {0};
	C1SubHeadSet((LPBYTE)&pMsg, 0xBF, 0x06, sizeof(pMsg));
	pMsg.btKillCount = lpObj->m_IllusionKillCount;
	
	gSendProto.DataSend( aIndex, (UCHAR*)&pMsg, sizeof(pMsg));

}
#endif
//-------------------------------------------------------------------------------------------------------------------------------------------
bool gObjCheckMagic(LPOBJ lpObj, WORD aSkill)
{
	for(int n = 0; n < MAGIC_SIZE; n++)
	{
		if(lpObj->Magic[n].IsMagic() == TRUE)
		{
			if(lpObj->Magic[n].m_Skill == aSkill)
			{
				return true;
			}
		}
	}
	return false;
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void AngleMatrix (const float *Angles, float (*Matrix)[4] )	// Good - Strange this0.0174532925199432955f
{
	float	angle;
	float	Sr, Sp, Sy;
	float	Cr, Cp, Cy;
	
	angle = Angles[ROLL] *	0.0174532925199432955f;		//0.0174532821447436905f;
	Sy = sin(angle);
	Cy = cos(angle);
	angle = Angles[YAW] *   0.0174532925199432955f;
	Sp = sin(angle);   //ST=0.0174532923847436905
	Cp = cos(angle);
	angle = Angles[PITCH] * 0.0174532925199432955f;		//0.0174532821247436905f;
	Sr = sin(angle);
	Cr = cos(angle);

	// matrix = (YAW * PITCH) * ROLL
	Matrix[0][0] = Cp*Cy;
	Matrix[1][0] = Cp*Sy;
	Matrix[2][0] = -Sp;
	Matrix[0][1] = Sr*Sp*Cy+Cr*-Sy;
	Matrix[1][1] = Sr*Sp*Sy+Cr*Cy;
	Matrix[2][1] = Sr*Cp;
	Matrix[0][2] = (Cr*Sp*Cy+-Sr*-Sy);
	Matrix[1][2] = (Cr*Sp*Sy+-Sr*Cy);
	Matrix[2][2] = Cr*Cp;
	Matrix[0][3] = 0.0;
	Matrix[1][3] = 0.0;
	Matrix[2][3] = 0.0;
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void VectorRotate (const float MT_IN1, const float MT_IN2, float MT_OUT)
{
	out[0*4+0] = in1[0*4+0] * in2[0*4+0] + in1[0*4+1] * in2[0*4+1] + in1[0*4+2] * in2[0*4+2];
	out[0*4+1] = in1[0*4+0] * in2[0*4+4] + in1[0*4+1] * in2[0*4+5] + in1[0*4+2] * in2[0*4+6];
	out[0*4+2] = in1[0*4+0] * in2[0*4+8] + in1[0*4+1] * in2[0*4+9] + in1[0*4+2] * in2[0*4+10];
}
//-------------------------------------------------------------------------------------------------------------------------------------------
bool gObjDenorantSprite(LPOBJ lpObj)
{
	if ( lpObj->Type != OBJ_USER )
	{
		return false;
	}

	if ( lpObj->pInventory[8].IsItem() == FALSE )
	{
		return false;
	}

	if ( lpObj->pInventory[8].m_Type == ITEMGET(13,3) && lpObj->pInventory[8].m_Durability > 0.0f )
	{
		return false;
	}
	
	return false;
}
//-------------------------------------------------------------------------------------------------------------------------------------------
bool gObjDarkHorse(LPOBJ lpObj)
{
	if ( lpObj->Type != OBJ_USER )
	{
		return false;
	}

	if ( lpObj->pInventory[8].IsItem() == FALSE )
	{
		return false;
	}

	if ( lpObj->pInventory[8].m_Type == ITEMGET(13,4) && lpObj->pInventory[8].m_Durability > 0.0f )
	{
		return true;
	}
	
	return false;
}
//-------------------------------------------------------------------------------------------------------------------------------------------
bool gObjFenrir(LPOBJ lpObj)
{
	if ( lpObj->Type != OBJ_USER )
	{
		return false;
	}

	if ( lpObj->pInventory[8].IsItem() == FALSE )
	{
		return false;
	}

	if ( lpObj->pInventory[8].m_Type == ITEMGET(13,37) && lpObj->pInventory[8].m_Durability > 0.0f )
	{
		return true;
	}

	return false;
}
//-------------------------------------------------------------------------------------------------------------------------------------------
bool gObjAngelSprite(LPOBJ lpObj)
{
	if ( lpObj->Type != OBJ_USER )
	{
		return false;
	}

	if ( lpObj->pInventory[8].IsItem() == FALSE )
	{
		return false;
	}

	if ( lpObj->pInventory[8].m_Type == ITEMGET(13,0) && lpObj->pInventory[8].m_Durability > 0.0f )
	{
		return true;
	}

	return false;
}
//-------------------------------------------------------------------------------------------------------------------------------------------
bool gObjSatanSprite(LPOBJ lpObj)
{
	if ( lpObj->Type != OBJ_USER )
	{
		return false;
	}

	if ( lpObj->pInventory[8].IsItem() == FALSE )
	{
		return false;
	}

	if ( lpObj->pInventory[8].m_Type == ITEMGET(13,1) && lpObj->pInventory[8].m_Durability > 0.0f )
	{
		return true;
	}

	return false;
}
//-------------------------------------------------------------------------------------------------------------------------------------------
bool gObjWingSprite(LPOBJ lpObj)
{
	if ( lpObj->Type != OBJ_USER )
	{
		return false;
	}

	if ( lpObj->pInventory[7].IsItem() == FALSE )
	{
		return false;
	}

	CItem * Wing = &lpObj->pInventory[7];

	if ( (Wing->m_Type >= ITEMGET(12,0) && Wing->m_Type <= ITEMGET(12,6)) ||
		  Wing->m_Type == ITEMGET(13,30) ||
		 (Wing->m_Type >= ITEMGET(12,36) && Wing->m_Type <= ITEMGET(12,43)) || 
		 Wing->m_Type == ITEMGET(12,49) ||
		 Wing->m_Type == ITEMGET(12,50) ||
		 (Wing->m_Type >= ITEMGET(12,130) && Wing->m_Type <= ITEMGET(12,135))) 
	{
		if ( Wing->m_Durability > 0.0f )
		{
			return true;
		}
	}
	
	return false;
}
//-------------------------------------------------------------------------------------------------------------------------------------------
bool gObjUniriaSprite(LPOBJ lpObj)
{
	if ( lpObj->Type != OBJ_USER )
	{
		return false;
	}

	if ( lpObj->pInventory[8].IsItem() == FALSE )
	{
		return false;
	}

	if ( lpObj->pInventory[8].m_Type == ITEMGET(13,2) && lpObj->pInventory[8].m_Durability > 0.0f )
	{
		return true;
	}

	return false;
}
//-------------------------------------------------------------------------------------------------------------------------------------------
bool gObjDemonSprite(LPOBJ lpObj)
{
	if ( lpObj->Type != OBJ_USER )
	{
		return false;
	}

	if ( lpObj->pInventory[8].IsItem() == FALSE )
	{
		return false;
	}

	if ( lpObj->pInventory[8].m_Type == ITEMGET(13,64) && lpObj->pInventory[8].m_Durability > 0.0f )
	{
		return true;
	}

	return false;
}
//-------------------------------------------------------------------------------------------------------------------------------------------
bool gObjGuardianSprite(LPOBJ lpObj)
{
	if ( lpObj->Type != OBJ_USER )
	{
		return false;
	}

	if ( lpObj->pInventory[8].IsItem() == FALSE )
	{
		return false;
	}

	if ( lpObj->pInventory[8].m_Type == ITEMGET(13,65) && lpObj->pInventory[8].m_Durability > 0.0f )
	{
		return true;
	}

	return false;
}
//-------------------------------------------------------------------------------------------------------------------------------------------
bool gObjRudolfSprite(LPOBJ lpObj)
{
	if ( lpObj->Type != OBJ_USER )
	{
		return false;
	}

	if ( lpObj->pInventory[8].IsItem() == FALSE )
	{
		return false;
	}

	if ( lpObj->pInventory[8].m_Type == ITEMGET(13,67) && lpObj->pInventory[8].m_Durability > 0.0f )
	{
		return true;
	}

	return false;
}
//-------------------------------------------------------------------------------------------------------------------------------------------
bool gObjPandaSprite(LPOBJ lpObj)
{
	if ( lpObj->Type != OBJ_USER )
	{
		return false;
	}

	if ( lpObj->pInventory[8].IsItem() == FALSE )
	{
		return false;
	}

	if ( lpObj->pInventory[8].m_Type == ITEMGET(13,80) && lpObj->pInventory[8].m_Durability > 0.0f )
	{
		return true;
	}

	return false;
}
//-------------------------------------------------------------------------------------------------------------------------------------------
bool gObjSkeletonSprite(LPOBJ lpObj)
{
	if ( lpObj->Type != OBJ_USER )
	{
		return false;
	}

	if ( lpObj->pInventory[8].IsItem() == FALSE )
	{
		return false;
	}

	if ( lpObj->pInventory[8].m_Type == ITEMGET(13,123) && lpObj->pInventory[8].m_Durability > 0.0f )
	{
		return true;
	}

	return false;
}
//-------------------------------------------------------------------------------------------------------------------------------------------
bool IsSelfDefense(LPOBJ lpObj,LPOBJ lpTargetObj)
{
	bool bResult = false;
	// ----
	if ( lpObj->Type == OBJ_USER && lpTargetObj->Type == OBJ_USER )
	{
		if ( gDuel.AreOnDuel(lpObj,lpTargetObj) == true )
		{
			bResult = false;
		}
		else if ( gUser.ChaosCastleMapRange(lpObj->MapNumber) == true || gUser.ChaosCastleMapRange(lpTargetObj->MapNumber) == true )
		{
			bResult = false;
		}
		else if (gObjGetRelationShip(lpObj, lpTargetObj) == 2 )
		{
			bResult = false;
		} 
		else if (Gens.IsGenPVP(lpObj,lpTargetObj) == true )
		{
			bResult = false;
		}
		else
		{
			bResult = true;
		}
	}
	else if ( lpTargetObj->Type == OBJ_MONSTER && lpObj->Type == OBJ_USER )
	{
		if ( lpTargetObj->m_RecallMon >= 0 )
		{
			bResult = true;
		}
	}
	return bResult;
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void ElfSoldierBuf(LPOBJ lpObj)
{
	if ( gObjIsConnected(lpObj) == false )
	{
		return;
	}

	if ( lpObj->Level > MAX_NPC_PHANTOM_BUFFER )
	{
		GCServerCmd(lpObj->m_Index, 0x0D, 0, 0);
		
		return;
	}

	lpObj->m_iSkillNPCHelpTime = GetTickCount();
	lpObj->m_iSkillNPCDefense = lpObj->Level / 5 + 50;	// FORMULA
	lpObj->m_iSkillNPCAttack = lpObj->Level / 3 + 45;	// FORMULA

	GCStateInfoSend(lpObj, 1, eVS_NPC_HELP);

	return;
}
//-------------------------------------------------------------------------------------------------------------------------------------------
int gObjGetCompleteItemCountInIventory(int aIndex, int itemtype, int itemindex, int itemlevel, int itemluck, int itemskill, int itemadd, int itemexe)
{
	LPOBJ lpObj = &gObj[aIndex];
	int count = 0;

	for ( int n=12;n<MAIN_INVENTORY_SIZE ; n++ )
	{
		if ( lpObj->pInventory[n].IsItem() == true )
		{
			if ( lpObj->pInventory[n].m_Type == ((itemtype * MAX_SUBTYPE_ITEMS) + itemindex) )
			{
				if ( itemlevel != -1 )
				{
					if (  lpObj->pInventory[n].m_Level != itemlevel )
					{
						continue;
					}
				}
				if ( itemskill != -1 )
				{
					if ( lpObj->pInventory[n].m_Option1 != itemskill )
					{
						continue;
					}
				}
				if ( itemluck != -1 )
				{
					if ( lpObj->pInventory[n].m_Option2 != itemluck )
					{
						continue;
					}
				}
				if ( itemadd != -1 )
				{
					if ( lpObj->pInventory[n].m_Option3 != itemadd )
					{
						continue;
					}
				}
				if ( itemexe != -1 )
				{
					if ( lpObj->pInventory[n].m_NewOption != itemexe )
					{
						continue;
					}
				}
				count++;
			}
		}
	}

	return count;
}

void CUser::WarehouseDBItemSetByte(LPOBJ lpObj, struct SDHP_GETWAREHOUSEDB_SAVE* lpMsg, int ItemDbByte)
{
	int n;
	int itype;
	int _type;
	CItem item;
	BYTE OptionData;
	WORD hiWord;
	WORD loWord;
	bool bIsItemExist = true;

	for(n = 0; n < WAREHOUSE_SIZE;n++)
	{
		bIsItemExist = true;
		lpObj->pInventory[n].Clear();
		itype = lpMsg->dbItems[n*ItemDbByte];

		if(lpMsg->dbItems[n*ItemDbByte] == 0xFF && (lpMsg->dbItems[n*ItemDbByte+DBI_NOPTION_DATA] & 0x80) == 0x80 && (lpMsg->dbItems[n*ItemDbByte+DBI_OPTION380_DATA] & 0xF0) == 0xF0)
		{
			continue;
		}

		itype |= (lpMsg->dbItems[n*ItemDbByte+DBI_OPTION380_DATA] & 0xF0) * 32;
		itype |= (lpMsg->dbItems[n*ItemDbByte+DBI_NOPTION_DATA] & 0x80) * 2;
		_type = itype;

		if(lpMsg->dbItems[n*ItemDbByte] == 0xFF
			&& (lpMsg->dbItems[n*ItemDbByte+DBI_NOPTION_DATA] & 0x80) == 0x80
			&& (lpMsg->dbItems[n*ItemDbByte+DBI_OPTION380_DATA] & 0xF0) == 0xF0)
		{
			itype = -1;
		}

		if(_type == ITEMGET(14,17) || _type == ITEMGET(14,18))
		{
			if(lpMsg->dbItems[n*ItemDbByte+DBI_DUR]==0)
			{
				itype = -1;
			}
		}

		if(_type == ITEMGET(14,19))
		{
			if(lpMsg->dbItems[n*ItemDbByte+DBI_DUR]==(BYTE)-1)
			{
				itype = -1;
			}
		}

		if ( IsItem(_type) == false )
		{
			itype = -1;
		}

		if(itype != -1)
		{
			item.m_Level = DBI_GET_LEVEL(lpMsg->dbItems[n*ItemDbByte+DBI_OPTION_DATA]);

			if(_type != ITEMGET(14,11))
			{
				if(item.m_Level > 15)
				{
					item.m_Level = 0;
				}
			}

			OptionData = lpMsg->dbItems[n*ItemDbByte+DBI_OPTION_DATA];
			item.m_Option1 = DBI_GET_SKILL(OptionData);
			item.m_Option2 = DBI_GET_LUCK(OptionData);
			item.m_Option3 = DBI_GET_OPTION(OptionData);

			if(_type == ITEMGET(13,3))
			{
				item.m_Option3 |= DBI_GET_OPTION28(lpMsg->dbItems[n*ItemDbByte+DBI_NOPTION_DATA]) >> 7;
			}
			else
			{
				if((lpMsg->dbItems[n*ItemDbByte+DBI_NOPTION_DATA] & 0x70)== 0x70)
				{
					item.m_Option3 = 7;
				}
			}

			item.m_Durability = lpMsg->dbItems[n*ItemDbByte+DBI_DUR];
			item.m_JewelOfHarmonyOption = lpMsg->dbItems[n*ItemDbByte+DBI_JOH_DATA];


			item.m_ItemOptionEx = DBI_GET_380OPTION(lpMsg->dbItems[n*ItemDbByte+DBI_OPTION380_DATA]);

			if(item.m_ItemOptionEx)
			{
				item.m_Type = itype;
				if(g_Item380System.Is380Item(&item)==false)
				{
					item.m_ItemOptionEx = 0;
					CLog.LogAdd("[380Item][%s][%s] Invalid 380 Item Option in Inventory pos[%d]", lpObj->AccountID,lpObj->Name,n);
				}
			}

			item.m_ItemSocket[0] = lpMsg->dbItems[n*ItemDbByte+DBI_SOCKET_1];
			item.m_ItemSocket[1] = lpMsg->dbItems[n*ItemDbByte+DBI_SOCKET_2];
			item.m_ItemSocket[2] = lpMsg->dbItems[n*ItemDbByte+DBI_SOCKET_3];
			item.m_ItemSocket[3] = lpMsg->dbItems[n*ItemDbByte+DBI_SOCKET_4];
			item.m_ItemSocket[4] = lpMsg->dbItems[n*ItemDbByte+DBI_SOCKET_5];

			item.Convert(itype,item.m_Option1,item.m_Option2,item.m_Option3,DBI_GET_NOPTION(lpMsg->dbItems[n*ItemDbByte+DBI_NOPTION_DATA]),lpMsg->dbItems[n*ItemDbByte+DBI_SOPTION_DATA],item.m_ItemOptionEx,item.m_ItemSocket);

			if(_type == ITEMGET(14,7))
			{

			}
			else
			{
				if(_type >= ITEMGET(14,0) && _type <= ITEMGET(14,8) || _type >= ITEMGET(14,38) && _type <= ITEMGET(14,40))
				{
					if(item.m_Durability == 0.0f)
					{
						item.m_Durability = 1.0f;
					}

					if (HRB_FixPotions == TRUE)
					{
						if(item.m_Durability > 255.0f)
						{
							item.m_Durability = 255.0f;
						}
					}
					else
					{
						if(item.m_Durability > 3.0f)
						{
							item.m_Durability = 3.0f;
						}
					}
				}
#if(GS_CASTLE)//Review the number of items that each one must have ...
				else if(_type == ITEMGET(14,70) || _type == ITEMGET(14,71))//cashshop
				{
					if(item.m_Durability == 0.0f)
					{
						item.m_Durability = 1.0f;
					}
					
					if(item.m_Durability > 100.0f)
					{
						item.m_Durability = 100.0f;
					}
				}
				
				if( _type == ITEMGET(14,78) || _type == ITEMGET(14,79)
					|| _type == ITEMGET(14,80) || _type == ITEMGET(14,81) 
					|| _type == ITEMGET(14,82) )
				{
					if(item.m_Durability == 0.0f)
					{
						item.m_Durability = 1.0f;
					}
					
					if(item.m_Durability > 3.0f)
					{
						item.m_Durability = 3.0f;
					}
				}
				
				if( _type == ITEMGET(14,63) || _type == ITEMGET(14,64) )
				{
					if(item.m_Durability == 0.0f)
					{
						item.m_Durability = 1.0f;
					}
					
					if(item.m_Durability > 1.0f)
					{
						item.m_Durability = 1.0f;
					}
				}
#endif
				else if(_type >= ITEMGET(14,46) && _type <= ITEMGET(14,50))
				{
					if(item.m_Durability == 0.0f)
					{
						item.m_Durability = 1.0f;
					}

					if(item.m_Durability > 3.0f)
					{
						item.m_Durability = 3.0f;
					}
				}
				else if(_type >= ITEMGET(14,35) && _type <= ITEMGET(14,37))
				{
					if(item.m_Durability == 0.0f)
					{
						item.m_Durability = 1.0f;
					}

					if(item.m_Durability > 1.0f)
					{
						item.m_Durability = 1.0f;
					}
				}
				else if(_type != ITEMGET(13,10) && _type != ITEMGET(14,29) && _type != ITEMGET(14,21))
				{
					if(item.m_Level == 3)
					{
						if(item.m_Durability > item.m_BaseDurability && bIsItemExist == 1)
						{
							item.m_Durability = item.m_BaseDurability;
						}
					}
				}
			}

			lpObj->pInventory[n].m_Option1 = item.m_Option1;
			lpObj->pInventory[n].m_Option2 = item.m_Option2;
			lpObj->pInventory[n].m_Option3 = item.m_Option3;
			lpObj->pInventory[n].m_JewelOfHarmonyOption = item.m_JewelOfHarmonyOption;
			lpObj->pInventory[n].m_ItemOptionEx = item.m_ItemOptionEx;
			lpObj->pInventory[n].m_ItemSocket[0] = item.m_ItemSocket[0];
			lpObj->pInventory[n].m_ItemSocket[1] = item.m_ItemSocket[1];
			lpObj->pInventory[n].m_ItemSocket[2] = item.m_ItemSocket[2];
			lpObj->pInventory[n].m_ItemSocket[3] = item.m_ItemSocket[3];
			lpObj->pInventory[n].m_ItemSocket[4] = item.m_ItemSocket[4];


			hiWord = ((lpMsg->dbItems[n*ItemDbByte+DBI_SERIAL2])&0xFF)&0xFF | (((lpMsg->dbItems[n*ItemDbByte+DBI_SERIAL1])&0xFF)&0xFF) * 256;
			loWord = ((lpMsg->dbItems[n*ItemDbByte+DBI_SERIAL4])&0xFF)&0xFF | (((lpMsg->dbItems[n*ItemDbByte+DBI_SERIAL3])&0xFF)&0xFF) * 256;

			item.m_Number = ((loWord &0xFFFF)&0xFFFF) | ((hiWord & 0xFFFF)&0xFFFF) << 16;

			gObjWarehouseInsertItemPos(lpObj->m_Index,item,n,0);
			gObjWarehouseItemSet(lpObj->m_Index, n, 1);
		}
	}
}