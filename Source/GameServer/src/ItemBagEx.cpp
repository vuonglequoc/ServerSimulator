//---------------------------------------------------------------------------
// # Project:		HaRiBO MU GameServer - Supported Season 6
// # Company:		RealCoderZ MU Development © 2011
// # Description:	Event Item Bags Ex System
// # Developed By:	WolF & M.E.S
//---------------------------------------------------------------------------
#include "stdafx.h"
#include "ItemBagEx.H"
#include "gObjMonster.H"
#include "DSProtocol.H"
#include "GameMain.H"
#include "ReadScript.h"
#include "LogProc.H"

// -----------------------------------------------------------------------------------------------------------------------------------------
CItemBagEx::CItemBagEx()
{
	return;
}
// -----------------------------------------------------------------------------------------------------------------------------------------
CItemBagEx::~CItemBagEx()
{
	return;
}
// -----------------------------------------------------------------------------------------------------------------------------------------
void CItemBagEx::Init(const char* EventItemBagExFile)
{
	char gItemBagName[255];

	this->m_IBLoad				= FALSE;
	this->m_IBEventName[0]		= 0;
	this->m_IBEventItemType		= -1;
	this->m_IBEventItemLevel	= 0;
	this->m_IBDropZen			= 0;
	this->m_IBDropRate			= 0;
	this->m_IBItemDropRate		= 0;
	this->m_IBExcItemDropRate	= 0;
	this->m_IBBagObjectCount	= 0;

	wsprintf(gItemBagName, "..//Data//EventItemBags//%s", EventItemBagExFile);

	this->Load(gItemBagName);
}
// -----------------------------------------------------------------------------------------------------------------------------------------
void CItemBagEx::InitKundun(const char* EventItemBagExFile)
{
	char gItemBagName[255];

	this->m_IBLoad				= FALSE;
	this->m_IBEventName[0]		= 0;
	this->m_IBEventItemType		= -1;
	this->m_IBEventItemLevel	= 0;
	this->m_IBDropZen			= 0;
	this->m_IBDropRate			= 0;
	this->m_IBItemDropRate		= 0;
	this->m_IBExcItemDropRate	= 0;
	this->m_IBBagObjectCount	= 0;

	wsprintf(gItemBagName, "..//Data//EventItemBags//%s", EventItemBagExFile);

	this->LoadKundun(gItemBagName);
}
// -----------------------------------------------------------------------------------------------------------------------------------------
void CItemBagEx::Load(const char* szFileName)
{
	int Token;
	int n = 0;

	this->m_IBLoad = FALSE;

	if( (SMDFile = fopen(szFileName, "r")) == NULL )
	{
		CLog.LogAddC(TColor.DarkRed(), EVENT_ITEM_MODULE, "Error() Loading EventItemBagEx: [%s].", szFileName);
		return ;
	}

	while ( true )
	{
		Token = GetToken();

		if ( Token == 2 )
			break;

		if ( Token == 1 )
		{
			int szLine = TokenNumber;

			if ( szLine == 0 )
			{
				while(true)
				{
					Token = GetToken();

					if ( Token == 0 )
					{
						if ( strcmp("end", TokenString) == 0 )
							break;
					}

					int Map = (int)TokenNumber;

					if ( MAX_MAP_RANGE(Map) == FALSE )
					{
						CLog.MsgBox("EventItemBagEx Loaded Wrong Data in: [%s]", szFileName);
						return;
					}

					Token = GetToken();
					this->DropMapInfo[Map].m_bIsDrop			= (BYTE)TokenNumber;

					Token = GetToken();
					this->DropMapInfo[Map].m_MinMonsterLevel	= (BYTE)TokenNumber;

					Token = GetToken();
					this->DropMapInfo[Map].m_MaxMonsterLevel	= (BYTE)TokenNumber;
				}
			}
			else if ( szLine == 1 )
			{
				while ( true )
				{
					Token = GetToken();

					if ( Token == 0 )
					{
						if ( strcmp("end", TokenString) == 0 )
							break;
					}

					strcpy(this->m_IBEventName, TokenString);

					Token = GetToken();
					this->m_IBDropZen							= (int)TokenNumber;

					Token = GetToken();
					int ItemType								= (int)TokenNumber;

					Token = GetToken();
					int ItemIndex								= (int)TokenNumber;

					this->m_IBEventItemType = ITEMGET(ItemType, ItemIndex);

					Token = GetToken();
					this->m_IBEventItemLevel					= (int)TokenNumber;

					Token = GetToken();
					this->m_IBDropRate							= (int)TokenNumber;

					Token = GetToken();
					this->m_IBItemDropRate						= (int)TokenNumber;

					Token = GetToken();
					this->m_IBExcItemDropRate					= (int)TokenNumber;

					CLog.LogAddC(TColor.AliceBlue(), "[%s Event] Item = %d,Level = %d, EventDropRate = %d, ItemDropRate = %d, ExItemDropRate = %d",
						this->m_IBEventName, this->m_IBEventItemType, this->m_IBEventItemLevel, this->m_IBDropRate, this->m_IBItemDropRate,
						this->m_IBExcItemDropRate);
				}
			}
			else if ( szLine == 2 )
			{
				while ( true )
				{
					Token = GetToken();

					if ( Token == 0 )
					{
						if ( strcmp("end", TokenString) == 0 )
							break;
					}

					this->EventBagObject[n].m_Type				= (BYTE)TokenNumber;

					Token = GetToken();
					this->EventBagObject[n].m_Index				= (BYTE)TokenNumber;

					Token = GetToken();
					this->EventBagObject[n].m_MinLevel			= (BYTE)TokenNumber;

					Token = GetToken();
					this->EventBagObject[n].m_MaxLevel			= (BYTE)TokenNumber;

					Token = GetToken();
					this->EventBagObject[n].m_IsSkill			= (BYTE)TokenNumber;

					Token = GetToken();
					this->EventBagObject[n].m_IsLuck			= (BYTE)TokenNumber;

					Token = GetToken();
					this->EventBagObject[n].m_IsOption			= (BYTE)TokenNumber;

					Token = GetToken();
					this->EventBagObject[n].m_IsExcItem			= (BYTE)TokenNumber;

					n++;

					this->m_IBBagObjectCount++;

					if ( this->m_IBBagObjectCount > MAX_ITEMBAG_ATTR-1 )
					{
						break;
					}
				}
			}
		}
	}

	fclose(SMDFile);
	
	CLog.LogAddC(TColor.Green(), EVENT_ITEM_MODULE, "EventItemBagEx: [%s] Loaded Successfully.", szFileName);

	this->m_IBLoad = TRUE;
}
// -----------------------------------------------------------------------------------------------------------------------------------------
void CItemBagEx::LoadKundun(const char* szFileName)
{
	int Token;
	int n = 0;

	this->m_IBLoad = FALSE;

	if( (SMDFile = fopen(szFileName, "r")) == NULL )
	{
		CLog.LogAddC(TColor.DarkRed(), EVENT_ITEM_MODULE, "Error() Loading EventItemBagEx: [%s].", szFileName);
		return ;
	}

	while ( true )
	{
		Token = GetToken();

		if ( Token == 2 )
			break;

		if ( Token == 1 )
		{
			int szLine = TokenNumber;

			if ( szLine == 0 )
			{
				while(true)
				{
					Token = GetToken();

					if ( Token == 0 )
					{
						if ( strcmp("end", TokenString) == 0 )
							break;
					}

					int Map = (int)TokenNumber;

					if ( MAX_MAP_RANGE(Map) == FALSE )
					{
						CLog.MsgBox("EventItemBagEx Loaded Wrong Data in: [%s]", szFileName);
						return;
					}

					Token = GetToken();
					this->DropMapInfo[Map].m_bIsDrop			= (BYTE)TokenNumber;

					Token = GetToken();
					this->DropMapInfo[Map].m_MonsterLevel		= (BYTE)TokenNumber;
				}
			}
			else if ( szLine == 1 )
			{
				while ( true )
				{
					Token = GetToken();

					if ( Token == 0 )
					{
						if ( strcmp("end", TokenString) == 0 )
							break;
					}

					strcpy(this->m_IBEventName, TokenString);

					strcpy(this->m_IBEventName, TokenString);

					Token = GetToken();
					this->m_IBDropZen							= (int)TokenNumber;

					Token = GetToken();
					int ItemType								= (int)TokenNumber;

					Token = GetToken();
					int ItemIndex								= (int)TokenNumber;

					this->m_IBEventItemType = ITEMGET(ItemType, ItemIndex);

					Token = GetToken();
					this->m_IBEventItemLevel					= (int)TokenNumber;

					Token = GetToken();
					this->m_IBDropRate							= (int)TokenNumber;

					Token = GetToken();
					this->m_IBItemDropRate						= (int)TokenNumber;

					Token = GetToken();
					this->m_IBExcItemDropRate					= (int)TokenNumber;

					CLog.LogAddC(TColor.AliceBlue(), "[%s Event] Item = %d,Level = %d, EventDropRate = %d, ItemDropRate = %d, ExItemDropRate = %d",
						this->m_IBEventName, this->m_IBEventItemType, this->m_IBEventItemLevel, this->m_IBDropRate, this->m_IBItemDropRate,
						this->m_IBExcItemDropRate);
				}
			}
			else if ( szLine == 2 )
			{
				while ( true )
				{
					Token = GetToken();

					if ( Token == 0 )
					{
						if ( strcmp("end", TokenString) == 0 )
							break;
					}

					this->EventBagObject[n].m_Type				= (BYTE)TokenNumber;

					Token = GetToken();
					this->EventBagObject[n].m_Index				= (BYTE)TokenNumber;

					Token = GetToken();
					this->EventBagObject[n].m_MinLevel			= (BYTE)TokenNumber;

					Token = GetToken();
					this->EventBagObject[n].m_MaxLevel			= (BYTE)TokenNumber;

					Token = GetToken();
					this->EventBagObject[n].m_IsSkill			= (BYTE)TokenNumber;

					Token = GetToken();
					this->EventBagObject[n].m_IsLuck			= (BYTE)TokenNumber;

					Token = GetToken();
					this->EventBagObject[n].m_IsOption			= (BYTE)TokenNumber;

					Token = GetToken();
					this->EventBagObject[n].m_IsExcItem			= (BYTE)TokenNumber;

					n++;

					this->m_IBBagObjectCount++;

					if ( this->m_IBBagObjectCount > MAX_ITEMBAG_ATTR-1 )
					{
						break;
					}
				}
			}
		}
	}

	fclose(SMDFile);
	
	CLog.LogAddC(TColor.Green(), EVENT_ITEM_MODULE, "EventItemBagEx: [%s] Loaded Successfully.", szFileName);

	this->m_IBLoad = TRUE;
}
// -----------------------------------------------------------------------------------------------------------------------------------------
BYTE CItemBagEx::GetLevel(int n)
{
	if ( n < 0 || n > MAX_ITEMBAG_ATTR-1 )
		return FALSE;

	if ( this->EventBagObject[n].m_MinLevel == this->EventBagObject[n].m_MaxLevel )
	{
		return this->EventBagObject[n].m_MinLevel;
	}

	int Sub = (this->EventBagObject[n].m_MaxLevel - this->EventBagObject[n].m_MinLevel) + 1;

	int level = this->EventBagObject[n].m_MinLevel + (rand() % Sub);

	return level;
}
// -----------------------------------------------------------------------------------------------------------------------------------------
int CItemBagEx::GetMinLevel(int n)
{
	if ( n < 0 || n > MAX_ITEMBAG_ATTR-1 )
		return FALSE;

	return this->EventBagObject[n].m_MinLevel;
}
// -----------------------------------------------------------------------------------------------------------------------------------------
int CItemBagEx::GetMaxLevel(int n)
{
	if ( n < 0 || n > MAX_ITEMBAG_ATTR-1 )
		return FALSE;

	return this->EventBagObject[n].m_MaxLevel;
}
// -----------------------------------------------------------------------------------------------------------------------------------------
bool CItemBagEx::IsEnableEventItemDrop(int aIndex)
{
	int iMapNumber = gObj[aIndex].MapNumber;

	if ( this->DropMapInfo[iMapNumber].m_bIsDrop == FALSE )
		return false;

	int iLevel = gObj[aIndex].Level;

	if ( iLevel < this->DropMapInfo[iMapNumber].m_MinMonsterLevel || iLevel > this->DropMapInfo[iMapNumber].m_MaxMonsterLevel)
		return false;

	return false;
}
// -----------------------------------------------------------------------------------------------------------------------------------------
bool CItemBagEx::DropEventItem(int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];
	BYTE Socket[5];
	Socket[0] = 0xFF;
	Socket[1] = 0xFF;
	Socket[2] = 0xFF;
	Socket[3] = 0xFF;
	Socket[4] = 0xFF;

	if ( this->m_IBLoad == FALSE )
	{
		return false;
	}

	if ( this->IsEnableEventItemDrop(aIndex) == false )
	{
		return false;
	}

	if ( (rand()%10000) < this->m_IBItemDropRate )
	{
		int CordX		= lpObj->X;
		int CordY		= lpObj->Y;
		int ItemLvl		= this->m_IBEventItemLevel;
		int ItemType	= this->m_IBEventItemType;
		float Durablity = 255.0;

		int CPlayer = gObjMonsterTopHitDamageUser(lpObj);

		ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, CordX, CordY, ItemType, ItemLvl, Durablity, 0, 0, 0, CPlayer, 0, 0, Socket);

		CLog.LogAddC(TColor.AliceBlue(), "[%s][%s]::[%s]::MonsterEventItemDrop (%d)(%d/%d)", lpObj->AccountID, lpObj->Name, this->m_IBEventName,
			lpObj->MapNumber, CordX, CordY);
		
		return true;
	}

	return false;
}
// -----------------------------------------------------------------------------------------------------------------------------------------
BOOL CItemBagEx::DropItem(int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];

	int Type;
	int Level;
	int CordX;
	int CordY;
	int szSkill		= 0;
	int szLuck		= 0;
	int szAddOpt	= 0;
	int ExOption	= 0;
	int RandomExe	= 0;
	int IsExeOption[6] = {0,0,0,0,0,0};
	int DropItemNum;
	float Durability;
	BYTE Socket[5];
	Socket[0] = 0xFF;
	Socket[1] = 0xFF;
	Socket[2] = 0xFF;
	Socket[3] = 0xFF;
	Socket[4] = 0xFF;
	//----------
	if(this->m_IBLoad == FALSE) return FALSE;
	//----------
	if ( this->GetBagCount() > 0 )
	{
		if ( (rand()%100) < this->m_IBItemDropRate )
		{
			if ( (rand() % 100 ) < this->m_IBExcItemDropRate )
			{
				DropItemNum =  rand()%this->GetBagCount();
				Durability = 0;
				CordX = lpObj->X;
				CordY = lpObj->Y;
				Level = this->GetLevel(DropItemNum);
				Type = ItemGetNumberMake(this->EventBagObject[DropItemNum].m_Type, this->EventBagObject[DropItemNum].m_Index);
				// -----
				if ( Type == -1 ) return FALSE;
				// -----
				if ( this->EventBagObject[DropItemNum].m_IsSkill != 0 )
				{
					szSkill = rand()%2;
				}
				// -----
				if ( this->EventBagObject[DropItemNum].m_IsLuck != 0 )
				{
					szLuck = rand()%2;
				}
				// -----
				if ( this->EventBagObject[DropItemNum].m_IsOption != 0 )
				{
					if ( rand()%5 < 1 )
					{
						szAddOpt = 3;
					}
					else
					{
						szAddOpt = rand()%3;
					}
				}
				// -----
				if ( this->EventBagObject[DropItemNum].m_IsExcItem != 0 )
				{
					ExOption = NewOptionRand(0);
					szSkill = 1;
				}
				// --- Chaos, Bless, Soul, Life, Creation, Harmony & Loch Feather
				if ( Type == ITEMGET(12,15) || 
					 Type == ITEMGET(14,13) || 
					 Type == ITEMGET(14,14) || 
					 Type == ITEMGET(14,16) ||
					 Type == ITEMGET(14,31) ||
					 Type == ITEMGET(14,42) ||
					 Type == ITEMGET(13,14) )
				{
					szSkill		= 0;
					szLuck		= 0;
					szAddOpt	= 0;
					Level		= 0;
					ExOption	= 0;
				}
				// -----
				if ( Type >= ITEMGET(13,0)   && Type <= ITEMGET(13,5)	||
					 Type >= ITEMGET(13,8)   && Type <= ITEMGET(13,13)	||
					 Type >= ITEMGET(13,20)  && Type <= ITEMGET(13,28)	||
					 Type == ITEMGET(13,64)  || Type == ITEMGET(13,65)	||
					 Type == ITEMGET(13,67)  || Type == ITEMGET(13,68)	||
					 Type == ITEMGET(13,76)  || Type == ITEMGET(13,80)	||
					 Type == ITEMGET(14,85)	 || Type == ITEMGET(14,86)	||
					 Type == ITEMGET(14,87)	 || Type == ITEMGET(14,88)	||
					 Type == ITEMGET(14,89)	 || Type == ITEMGET(14,90)	||
					 Type == ITEMGET(13,122) || Type == ITEMGET(13,123)	)
				{
					Level = 0;
				}
			}
			else
			{
				DropItemNum =  rand()%this->GetBagCount();
				Durability = 0;
				CordX = lpObj->X;
				CordY = lpObj->Y;
				Level = this->GetLevel(DropItemNum);
				Type = ItemGetNumberMake(this->EventBagObject[DropItemNum].m_Type, this->EventBagObject[DropItemNum].m_Index);
				// -----
				if ( Type == -1 ) return FALSE;
				// -----
				if ( this->EventBagObject[DropItemNum].m_IsSkill != 0 )
				{
				szSkill = rand()%2;
				}
				// -----
				if ( this->EventBagObject[DropItemNum].m_IsLuck != 0 )
				{
					szLuck = rand()%2;
				}
				// -----
				if ( this->EventBagObject[DropItemNum].m_IsOption != 0 )
				{
					if ( rand()%5 < 1 )
					{
						szAddOpt = 3;
					}
					else
					{
						szAddOpt = rand()%3;
					}
				}
				// -----
				ExOption = 0;
				// --- Chaos, Bless, Soul, Life, Creation, Harmony & Loch Feather
				if ( Type == ITEMGET(12,15) || 
					 Type == ITEMGET(14,13) || 
					 Type == ITEMGET(14,14) || 
					 Type == ITEMGET(14,16) ||
					 Type == ITEMGET(14,31) ||
					 Type == ITEMGET(14,42) ||
					 Type == ITEMGET(13,14) )
				{
					szSkill		= 0;
					szLuck		= 0;
					szAddOpt	= 0;
					Level		= 0;
					ExOption	= 0;
				}
				// -----
				if ( Type >= ITEMGET(13,0)   && Type <= ITEMGET(13,5)	||
					 Type >= ITEMGET(13,8)   && Type <= ITEMGET(13,13)	||
					 Type >= ITEMGET(13,20)  && Type <= ITEMGET(13,28)	||
					 Type == ITEMGET(13,64)  || Type == ITEMGET(13,65)	||
					 Type == ITEMGET(13,67)  || Type == ITEMGET(13,68)	||
					 Type == ITEMGET(13,76)  || Type == ITEMGET(13,80)	||
					 Type == ITEMGET(14,85)	 || Type == ITEMGET(14,86)	||
					 Type == ITEMGET(14,87)	 || Type == ITEMGET(14,88)	||
					 Type == ITEMGET(14,89)	 || Type == ITEMGET(14,90)	||
					 Type == ITEMGET(13,122) || Type == ITEMGET(13,123)	)
				{
					Level = 0;
				}
			}
			BYTE Socket[5];
	Socket[0] = 0xFF;
	Socket[1] = 0xFF;
	Socket[2] = 0xFF;
	Socket[3] = 0xFF;
	Socket[4] = 0xFF;
			// -----
			ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, CordX, CordY, Type, Level, Durability, szSkill, szLuck, szAddOpt, lpObj->m_Index, ExOption, 0, Socket);
			// -----
			CLog.LogAddC(TColor.Aqua(), "[%s][%s][%s Event Item Drop]: (%d)(X:%d/Y:%d) Item:%d(%s) Level:%d Skill:%d Luck:%d AddOpt:%d Exc:%d",
				lpObj->AccountID, lpObj->Name, this->m_IBEventName, lpObj->MapNumber, CordX, CordY, Type, ItemAttribute[Type].Name, Level, szSkill, szLuck, szAddOpt, ExOption, Socket);
			return TRUE;
		}
	}
	else
	{
		CordX = lpObj->X;
		CordY = lpObj->Y;
		MapC[lpObj->MapNumber].MoneyItemDrop(this->m_IBDropZen, CordX, CordY);
	}
	// -----
	return TRUE;
}
// -----------------------------------------------------------------------------------------------------------------------------------------
BOOL CItemBagEx::PickItem(CItem & ObjItem, int & nItemIndex)
{
	if ( this->m_IBLoad == FALSE )
		return FALSE;

	int DropItemNum;

	if ( this->GetBagCount() > 0 )
	{
		DropItemNum = rand() % this->GetBagCount();
		nItemIndex = DropItemNum;
		ObjItem.m_Type = ItemGetNumberMake(this->EventBagObject[DropItemNum].m_Type, this->EventBagObject[DropItemNum].m_Index);
		ObjItem.m_Level = this->GetLevel(DropItemNum);

		if ( ObjItem.m_Type == -1 )
			return FALSE;

		if ( this->EventBagObject[DropItemNum].m_IsSkill != 0 )
			ObjItem.m_Option1 = 1;

		if ( this->EventBagObject[DropItemNum].m_IsLuck != 0 )
			ObjItem.m_Option2 = 1;

		if ( this->EventBagObject[DropItemNum].m_IsOption != 0 )
			ObjItem.m_Option3 = 1;

		if ( this->EventBagObject[DropItemNum].m_IsExcItem != 0 )
		{
			ObjItem.m_NewOption = 1;
			ObjItem.m_Option1 = 0;
			ObjItem.m_Option2 = 0;
		}

		if ( ObjItem.m_Type == ITEMGET(12,15) ||
			 ObjItem.m_Type == ITEMGET(14,13) ||
			 ObjItem.m_Type == ITEMGET(14,14) )
		{
			ObjItem.m_Option1 = 0;
			ObjItem.m_Option2 = 0;
			ObjItem.m_Option3 = 0;
			ObjItem.m_Level = 0;
		}

		if ( ObjItem.m_Type == ITEMGET(13,0) ||
			 ObjItem.m_Type == ITEMGET(13,1) ||
			 ObjItem.m_Type == ITEMGET(13,2) ||
			 ObjItem.m_Type == ITEMGET(13,8) ||
			 ObjItem.m_Type == ITEMGET(13,9) ||
			 ObjItem.m_Type == ITEMGET(13,12) ||
			 ObjItem.m_Type == ITEMGET(13,13) )
		{
			ObjItem.m_Level = 0;
		}

		ObjItem.m_Durability = 0;

		return TRUE;
	}

	return FALSE;
}
// -----------------------------------------------------------------------------------------------------------------------------------------
#if(GS_CASTLE)
int CItemBagEx::DropCastleHuntZoneBossReward(int aIndex,BYTE btMapNumber, BYTE cX, BYTE cY)
{
	if ( this->m_IBLoad == FALSE )
	{
		return FALSE;
	}

	float dur;
	int type;
	int level;
	int x;
	int y;
	int Option1 = 0;
	int Option2 = 0;
	int Option3 = 0;
	int DropItemNum;
	int ExOption = 0;
	LPOBJ lpObj = &gObj[aIndex];
	
	if(this->GetBagCount() > 0 )
	{
		if(GS_CASTLE)
		{

			DropItemNum = rand() % this->GetBagCount();
			dur = 0;
			
			if(!cX && !cY )
			{
				 x = lpObj->X;
				 y = lpObj->Y;
			}
			else
			{
				x = cX;
				y = cY;
			}

			level = this->GetLevel(DropItemNum);

			type = ItemGetNumberMake(this->EventBagObject[DropItemNum].m_Type,this->EventBagObject[DropItemNum].m_Index);

			if(type == -1)
				return FALSE;

			if(this->EventBagObject[DropItemNum].m_IsSkill != FALSE)
			{
				Option1 = this->EventBagObject[DropItemNum].m_IsSkill;
			}

			if(this->EventBagObject[DropItemNum].m_IsLuck != FALSE)
			{
				Option2 = 0;

				if ( (rand()%2) == 0 )
				{
					Option2 = 1;
				}
			}

			if ( this->EventBagObject[DropItemNum].m_IsOption != 0 )
			{
				if ( rand()%5 < 1 )
				{
					Option3 = 3;
				}
				else
				{
					Option3 = rand()%3;
				}
			}

			if ( this->EventBagObject[DropItemNum].m_IsExcItem != 0 )
			{
				ExOption = NewOptionRand(0);
				Option2 = 0;
				Option1 = 1;
			}

			if ( type == ITEMGET(12,15) ||
				type == ITEMGET(14,13) || 
				type == ITEMGET(14,14))	// Chaos, Bless, Soul
			{
				Option1 = 0;
				Option2 = 0;
				Option3 = 0;
				level = 0;
			}

			if ( type == ITEMGET(13,0) ||
				 type == ITEMGET(13,1) ||
				 type == ITEMGET(13,2) ||
				 type == ITEMGET(13,8) ||
				 type == ITEMGET(13,9) ||
				 type == ITEMGET(13,12) ||
				 type ==ITEMGET(13,13) )	// Angel, imp, unirioa, dino, r and pendant of ice, poisonm
			{
				level = 0;
			}

			BYTE Socket[5];
			Socket[0] = 0xFF;
			Socket[1] = 0xFF;
			Socket[2] = 0xFF;
			Socket[3] = 0xFF;
			Socket[4] = 0xFF;

			ItemSerialCreateSend(lpObj->m_Index, btMapNumber, x, y, type, level, dur, Option1, Option2, Option3, lpObj->m_Index, ExOption, 0, Socket);

			CLog.LogAdd("[Castle HuntZone] Castle HuntZone Boss ItemDrop [%s][%s] [%s] : (%d)(%d/%d) Item:(%s)%d Level:%d op1:%d op2:%d op3:%d",
				lpObj->AccountID, lpObj->Name, this->m_IBEventName, btMapNumber, x, y, ItemAttribute[type].Name, type, level, Option1, Option2, Option3);
			return TRUE;
		}
	}
	
	return TRUE;
}
#endif
//------------------------------------------------------------------------------------------------------------------------------------------------------
