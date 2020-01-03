//**********************************************************************//
// Project:	HaRiBO MU GameServer - Season 4								//
// Company: RealCoderZ MU Development © 2010							//
// Description: MU MonsterItemMng Functions & Structures				//
// Decompilation By: Deathway											//
// Coded & Modified By:	WolF & M.E.S									//
//**********************************************************************//
#include "stdafx.h"
#include "MonsterItemMng.h"
#include "MonsterAttr.h"
#include "GameMain.h"
#include "CashShop.h"
#include "LogProc.H"
#include "DevilSquare.h"

CMonsterItemMng g_MonsterItemMng;


CMonsterItemMng::CMonsterItemMng()
{
	return;
}


CMonsterItemMng::~CMonsterItemMng()
{
	this->Clear();
}


void CMonsterItemMng::Init()
{
	memset(this->m_iMonsterInvenItemCount, 0, sizeof(this->m_iMonsterInvenItemCount));

	for ( int i=0 ; i< MAX_LEVEL_MONSTER ; i++ )
	{
		this->m_MonsterInvenItems[i] = new CItem[MAX_ITEM_IN_MONSTER];
	}

	LPMONSTER_ATTRIBUTE lpm;

	for ( int i = 0; i< MAX_MONSTER_TYPE ; i++ )
	{
		lpm = gMAttr.GetAttr(i);

		if ( lpm != NULL )
		{
			strcpy( this->MonsterName, lpm->m_Name);

			if ( lpm->m_Level != 0 )
			{
				this->gObjGiveItemSearch(lpm->m_Level, lpm->m_MaxItemLevel);
			}
		}
	}
}


void CMonsterItemMng::Clear()
{
	memset(this->m_iMonsterInvenItemCount, 0, sizeof(this->m_iMonsterInvenItemCount));

	for ( int i =0 ; i< MAX_LEVEL_MONSTER ; i++ )
	{
		delete this->m_MonsterInvenItems[i];
	}
}

BYTE CMonsterItemMng::InsertItem(int monsterlevel, int type, int index, int itemlevel, int op1, int op2 ,int op3, BYTE * SocketOption )
{
	if ( monsterlevel > MAX_LEVEL_MONSTER || monsterlevel < 0 )
	{
		return -1;
	}

	int itemcount = this->m_iMonsterInvenItemCount[monsterlevel];

	if ( itemcount >= MAX_ITEM_IN_MONSTER )
	{
		return -1;
	}

	CItem * item = &this->m_MonsterInvenItems[monsterlevel][itemcount];
	int item_type = (type * MAX_SUBTYPE_ITEMS) + index;
	item->m_Level = itemlevel;
	item->Convert(item_type, op1, op2, op3, 0, 0,0,SocketOption);

	if ( type == 13 && index == 10 )
	{
		int rd = rand() % 100;
		item->m_Durability = rd + 100.0f;
	}
	else
	{
		item->m_Durability = item->m_BaseDurability;
	}

	itemcount ++;
	this->m_iMonsterInvenItemCount[monsterlevel] = itemcount;
	return 0;
}


CItem * CMonsterItemMng::GetItem(int monsterlevel)
{
	if ( monsterlevel > MAX_LEVEL_MONSTER || monsterlevel < 0 )
	{
		return NULL;
	}

	int itemcount = this->m_iMonsterInvenItemCount[monsterlevel];

	if ( itemcount <= 0 )
	{
		return NULL;
	}

	int itemindex = rand() % itemcount;
	return &this->m_MonsterInvenItems[monsterlevel][itemindex];
}

void CMonsterItemMng::gObjGiveItemSearch(int monsterlevel, int maxlevel)
{
	int result;
	int incount = 0;
	int type;
	int index;

	if ( monsterlevel > MAX_LEVEL_MONSTER-1 )
	{
		CLog.LogAdd("error-L3 : Monster Level Overflow~ %s %d", __FILE__, __LINE__ );
		return;
	}

	int BallTable[17];

	BallTable[0] = 7;
	BallTable[1] = 8;
	BallTable[2] = 9;
	BallTable[3] = 10;
	BallTable[4] = 11;
	BallTable[5] = 12;
	BallTable[6] = 13;
	BallTable[7] = 14;
	BallTable[8] = 16;
	BallTable[9] = 17;
	BallTable[10] = 18;
	BallTable[11] = 19;
	BallTable[12] = 21;
	BallTable[13] = 22;
	BallTable[14] = 23;
	BallTable[15] = 24;
	BallTable[16] = 35;

	while ( true )
	{
		if ( (rand() % 20) ==  0 ) 
		{
			if ( (rand() % 2)	!= 0 )
			{
				type = MAX_TYPE_ITEMS-1;
				index = rand() % (g_MaxItemIndexOfEachItemType[type]+1);
			}
			else
			{
				type = 12;
				index = BallTable[rand() % 17];
			}
		}
		else
		{
			type = rand() % MAX_TYPE_ITEMS;
			int iMaxItemIndex = g_MaxItemIndexOfEachItemType[type]+1;
			index = rand() % iMaxItemIndex;

			if ( type == 15 || (type == 12 && index != 15 ) )
			{
				continue;
			}
		}

		if ( type == 13 && index == 3 )
		{
			continue;
		}

		if ( (type == 13 && index == 32)
          || (type == 13 && index == 33)
          || (type == 13 && index == 34)
          || (type == 13 && index == 35)
          || (type == 13 && index == 36)
          || (type == 13 && index == 37) )
		{
			continue;
		}

        if ( (type == 14 && index == 35)
          || (type == 14 && index == 36)
          || (type == 14 && index == 37)
          || (type == 14 && index == 38)
          || (type == 14 && index == 39)
          || (type == 14 && index == 40) )
		{
			continue;
		}

        if ( IsCashItem(ITEMGET(type, index)) == TRUE )
		{
			continue;
		}
        
		if ( (type == 13 && index < 8) || 
			( (type == 14) && (index == 9 || index == 10 ||	index == 13 || index == 14 || index ==16 || index == 17 || index == 18 || index == 22 ) ) ||
			(type == 12 && index == 15) || 
			(type == 13 && index == 14) || 
			(type == 14 && index == 31 ) )
		{
			int perc = rand() % 11;

			if ( type == 12 && index == 15 )
			{
				if ( monsterlevel >= 13 && monsterlevel <= 66 )
				{
					perc = rand() % 7 ;
					
					if ( perc < 3 )
					{
						perc = 0;
					}
				}
				else
				{
					perc = 1;
				}
			}

			if ( perc == 0 )
			{
				if ( zzzItemLevel(type, index, monsterlevel ) == TRUE )
				{
					BYTE Socket2[5];
					Socket2[0] = 0xFF;
					Socket2[1] = 0xFF;
					Socket2[2] = 0xFF;
					Socket2[4] = 0xFF;
					Socket2[5] = 0xFF;
					if ( this->InsertItem(monsterlevel, type, index, 0, 0, 0, 0,Socket2) != (BYTE)-1 )
					{
						incount++;

						if ( incount > MAX_ITEM_IN_MONSTER -1 )
						{
							return;
						}
					}
				}
			}
		}
		else
		{
			result = GetLevelItem(type, index, monsterlevel);

			if ( result >= 0 )
			{
				if ( (type == 13 && index == 10) || (type == 12 && index == 11 ) )
				{
					BYTE Socket3[5];
					Socket3[0] = 0xFF;
					Socket3[1] = 0xFF;
					Socket3[2] = 0xFF;
					Socket3[4] = 0xFF;
					Socket3[5] = 0xFF;
					if ( this->InsertItem(monsterlevel, type, index, result, 0,0,0,Socket3) != 0xFF )
					{
						incount++;

						if ( incount > MAX_ITEM_IN_MONSTER -1 )
						{
							return;
						}
					}
				}
				else if ( result <= maxlevel )
				{
					if ( type == 12 )
					{
						if ( index != 11 )
						{
							result = 0;
						}
					}

					if ( type == 12 && index == 11 )
					{

					}
					else
					{
						if ( result > maxlevel )
						{
							result = maxlevel;
						}
					}

					if ( (type == 4 && index == 7) || (type == 4 && index == 15) )
					{
						result = 0;
					}
				
					BYTE Socket4[5];
					Socket4[0] = 0xFF;
					Socket4[1] = 0xFF;
					Socket4[2] = 0xFF;
					Socket4[4] = 0xFF;
					Socket4[5] = 0xFF;
					if ( this->InsertItem(monsterlevel, type, index,result, 0,0,0,Socket4) != (BYTE)-1 )
					{
						incount++;

						if ( incount > MAX_ITEM_IN_MONSTER-1 )
						{
							return;
						}
					}
				}
			}
		}
		
		if ( this->m_iMonsterInvenItemCount[monsterlevel] >= MAX_ITEM_IN_MONSTER )
		{
			return;
		}
		
	}
}
