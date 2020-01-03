// ------------------------------
// Decompiled by Deathway
// Date : 2007-05-09
// ------------------------------
// Gs-N 0.99.60T 0x004E1EB0
//	GS-N	2.00.05	JPN	0x0050A790	-	Completed
//	GS-CS	2.00.05	JPN	0x0050A790	-	Completed
#include "stdafx.h"
#include "JewelMixSystem.h"

#include "Gamemain.h"
#include "DSprotocol.h"
#include "Functions.H"
#include "LogProc.H"
#include "user.h"

CJewelMixSystem::CJewelMixSystem()
{
	return;
}

CJewelMixSystem::~CJewelMixSystem()
{
	return;
}

int CJewelMixSystem::GetJewelCount(int iIndex, int iJewelType)
{
	if ( !gObjIsConnected(iIndex) )
		return -1;

	int iItemType;

	switch ( iJewelType )
	{
		case 0:
			iItemType = ITEMGET(14,13);
			break;
		case 1:
			iItemType = ITEMGET(14,14);
			break;
		default:
			return -1;
	}	
	
	int iItemCount = gObjGetItemCountInIventory(iIndex, iItemType);
	return iItemCount;
}



struct JEWELMIX_INFO
{
	int m_iJewelCount;	// 0
	int m_iMixMoney;	// 4
	int m_iSourceType;	// 8
	int m_iChangeType;	// C
};

static const JEWELMIX_INFO g_JewelMixInfo[10][3] = {
	10, 500000, ITEMGET(14,13), ITEMGET(12,30),	// Bless x 10
	20, 1000000, ITEMGET(14,13), ITEMGET(12,30), // Bless x 20
	30, 1500000, ITEMGET(14,13), ITEMGET(12,30), // Bless x 30
	10, 500000, ITEMGET(14,14), ITEMGET(12,31), // Soul x 10
	20, 1000000, ITEMGET(14,14), ITEMGET(12,31), // Soul x 20
	30, 1500000, ITEMGET(14,14), ITEMGET(12,31), // Soul x 30
	10, 500000, ITEMGET(14,16), ITEMGET(12,136), // Life x 10
	20, 1000000, ITEMGET(14,16), ITEMGET(12,136), // Life x 20
	30, 1500000, ITEMGET(14,16), ITEMGET(12,136), // Life x 30
	10, 500000, ITEMGET(14,22), ITEMGET(12,137), // Creation x 10
	20, 1000000, ITEMGET(14,22), ITEMGET(12,137), // Creation x 20
	30, 1500000, ITEMGET(14,22), ITEMGET(12,137), // Creation x 30
	10, 500000, ITEMGET(14,31), ITEMGET(12,138), // Guardian x 10
	20, 1000000, ITEMGET(14,31), ITEMGET(12,138), // Guardian x 20
	30, 1500000, ITEMGET(14,31), ITEMGET(12,138), // Guardian x 30
	10, 500000, ITEMGET(14,41), ITEMGET(12,139), // GemStome x 10
	20, 1000000, ITEMGET(14,41), ITEMGET(12,139), // GemStome x 20
	30, 1500000, ITEMGET(14,41), ITEMGET(12,139), // GemStome x 30
	10, 500000, ITEMGET(14,42), ITEMGET(12,140), // Harmony x 10
	20, 1000000, ITEMGET(14,42), ITEMGET(12,140), // Harmony x 20
	30, 1500000, ITEMGET(14,42), ITEMGET(12,140), // Harmony x 30
	10, 500000, ITEMGET(12,15), ITEMGET(12,141), // Chaos x 10
	20, 1000000, ITEMGET(12,15), ITEMGET(12,141), // Chaos x 20
	30, 1500000, ITEMGET(12,15), ITEMGET(12,141), // Chaos x 30
	10, 500000, ITEMGET(14,43), ITEMGET(12,142), // Lower x 10
	20, 1000000, ITEMGET(14,43), ITEMGET(12,142), // Lower x 20
	30, 1500000, ITEMGET(14,43), ITEMGET(12,142), // Lower x 30
	10, 500000, ITEMGET(14,44), ITEMGET(12,143), // Higher x 10
	20, 1000000, ITEMGET(14,44), ITEMGET(12,143), // Higher x 20
	30, 1500000, ITEMGET(14,44), ITEMGET(12,143) // Higher x 30
};
									 
int CJewelMixSystem::GetJewelCountPerLevel(int iJewelType, int iJewelLevel)
{
	if ( !CHECK_LIMIT(iJewelLevel, 3) )
		return -1;

	int iJewelCount = 0;

	switch ( iJewelType )
	{
		case 0:
			iJewelCount = g_JewelMixInfo[0][iJewelLevel].m_iJewelCount;
			break;
		case 1:
			iJewelCount = g_JewelMixInfo[1][iJewelLevel].m_iJewelCount;
			break;
		case 2:
			iJewelCount = g_JewelMixInfo[2][iJewelLevel].m_iJewelCount;
			break;
		case 3:
			iJewelCount = g_JewelMixInfo[3][iJewelLevel].m_iJewelCount;
			break;
		case 4:
			iJewelCount = g_JewelMixInfo[4][iJewelLevel].m_iJewelCount;
			break;
		case 5:
			iJewelCount = g_JewelMixInfo[5][iJewelLevel].m_iJewelCount;
			break;
		case 6:
			iJewelCount = g_JewelMixInfo[6][iJewelLevel].m_iJewelCount;
			break;
		case 7:
			iJewelCount = g_JewelMixInfo[7][iJewelLevel].m_iJewelCount;
			break;
		case 8:
			iJewelCount = g_JewelMixInfo[8][iJewelLevel].m_iJewelCount;
			break;
		case 9:
			iJewelCount = g_JewelMixInfo[9][iJewelLevel].m_iJewelCount;
			break;
		default:
			return -1;
	}

	return iJewelCount;
}

BOOL CJewelMixSystem::MixJewel( int iIndex, int iJewelType, int iMixType)
{
	if ( !gObjIsConnected(iIndex))
		return FALSE;

	if ( gObj[iIndex].ChaosLock == TRUE )
	{
		CLog.LogAdd("[JewelMix] [%s][%s] Chaos Mix is already working",	gObj[iIndex].AccountID, gObj[iIndex].Name);
		GCAnsJewelMix(iIndex, 0);
		return FALSE;
	}

	gObj[iIndex].ChaosLock = TRUE;

	if ( !CHECK_LIMIT(iJewelType, 10) )
	{
		CLog.LogAdd("[JewelMix] [%s][%s] Mix iJewelType is out of bound : %d", gObj[iIndex].AccountID, gObj[iIndex].Name, iJewelType);
		gObj[iIndex].ChaosLock = FALSE;
		GCAnsJewelMix(iIndex, 2);
		return FALSE;
	}

	if ( !CHECK_LIMIT(iMixType, 3))
	{
		CLog.LogAdd("[JewelMix] [%s][%s] iMixType is out of bound : %d", gObj[iIndex].AccountID, gObj[iIndex].Name, iMixType);
		gObj[iIndex].ChaosLock = FALSE;
		GCAnsJewelMix(iIndex, 3);
		return FALSE;
	}

	int iItemType = g_JewelMixInfo[iJewelType][iMixType].m_iSourceType;
	int iJewelCount = g_JewelMixInfo[iJewelType][iMixType].m_iJewelCount;
	int iMixMoney = g_JewelMixInfo[iJewelType][iMixType].m_iMixMoney;
	int iChangeType = g_JewelMixInfo[iJewelType][iMixType].m_iChangeType;

	if ( iJewelCount <= 0 )
	{
		gObj[iIndex].ChaosLock = FALSE;
		GCAnsJewelMix(iIndex, 0);
		return FALSE;
	}

	int iUserJewelCount = gObjGetItemCountInIventory(iIndex, iItemType);

	if ( iJewelCount > iUserJewelCount )
	{
		CLog.LogAdd("[JewelMix] [%s][%s] lack of jewel to mix : %d / %d", gObj[iIndex].AccountID, gObj[iIndex].Name, iUserJewelCount, iJewelCount);
		gObj[iIndex].ChaosLock = FALSE;
		GCAnsJewelMix(iIndex, 4);
		return FALSE;
	}

	if ( iMixMoney > gObj[iIndex].Money )
	{
		CLog.LogAdd("[JewelMix] [%s][%s] lack of money to mix : %d / %d", gObj[iIndex].AccountID, gObj[iIndex].Name, gObj[iIndex].Money, iMixMoney);
		gObj[iIndex].ChaosLock = FALSE;
		GCAnsJewelMix(iIndex, 5);
		return FALSE;
	}

	int iDelJewelCount = 0;
	BOOL bItemDelOK = FALSE;

	for ( int x= INVETORY_WEAR_SIZE ; x< MAIN_INVENTORY_SIZE; x++)
	{
		if ( gObj[iIndex].pInventory[x].IsItem() == TRUE )
		{
			if ( gObj[iIndex].pInventory[x].m_Type == iItemType )
			{
				CLog.LogAdd("[JewelMix] [%s][%s] Mix - Delete Jewel, Type:%d, Level:%d, Serial:%d",
					gObj[iIndex].AccountID, gObj[iIndex].Name,
					gObj[iIndex].pInventory[x].m_Type,
					gObj[iIndex].pInventory[x].m_Level,
					gObj[iIndex].pInventory[x].m_Number);

				gObjInventoryItemSet(iIndex, x, 0xFF);
				gObj[iIndex].pInventory[x].Clear();
				iDelJewelCount++;

				if ( iJewelCount <= iDelJewelCount )
				{
					bItemDelOK = TRUE;
					break;
				}
			}
		}
	}

	GCItemListSend(iIndex);

	if ( bItemDelOK == FALSE )
	{
		CLog.LogAdd("[JewelMix] [%s][%s] lack of jewel to mix (in deleting) : %d / %d",
			gObj[iIndex].AccountID, gObj[iIndex].Name,
			iDelJewelCount, iJewelCount);

		gObj[iIndex].ChaosLock = FALSE;
		GCAnsJewelMix(iIndex, 0);
		return FALSE;
	}

	BYTE Socket[5];
	Socket[0] = 0xFF;
	Socket[1] = 0xFF;
	Socket[2] = 0xFF;
	Socket[3] = 0xFF;
	Socket[4] = 0xFF;

	ItemSerialCreateSend(iIndex, 235, (BYTE)gObj[iIndex].X,
						(BYTE)gObj[iIndex].Y, iChangeType,
						iMixType, 0, 0, 0, 0,
						iIndex, 0, 0, Socket);

	gObj[iIndex].ChaosLock = FALSE;
	gObj[iIndex].Money -= iMixMoney;
	GCMoneySend(iIndex, gObj[iIndex].Money);
	GCAnsJewelMix(iIndex, 1);

	CLog.LogAdd("[JewelMix] [%s][%s] jewel mix succeed : ItemType:%d, JewelCount:%d",
		gObj[iIndex].AccountID, gObj[iIndex].Name,
		iItemType, iJewelCount);

	return TRUE;
}


BOOL CJewelMixSystem::UnMixJewel(int iIndex, int iJewelType, int iJewelLevel, int iInventoryPos)
{
	if ( !gObjIsConnected(iIndex))
		return FALSE;

	if ( gObj[iIndex].ChaosLock == TRUE )
	{
		CLog.LogAdd("[JewelMix] [%s][%s] Chaos Mix is already working",
			gObj[iIndex].AccountID, gObj[iIndex].Name);

		GCAnsJewelUnMix(iIndex, 0);
		return FALSE;
	}

	gObj[iIndex].ChaosLock = TRUE;

	if ( !CHECK_LIMIT(iJewelType, 10) )
	{
		CLog.LogAdd("[JewelMix] [%s][%s] UnMix iJewelType is out of bound : %d", gObj[iIndex].AccountID, gObj[iIndex].Name, iJewelType);
		gObj[iIndex].ChaosLock = FALSE;
		GCAnsJewelUnMix(iIndex, 2);
		return FALSE;
	}

	if ( !CHECK_LIMIT(iInventoryPos, MAIN_INVENTORY_SIZE))
	{
		CLog.LogAdd("[JewelMix] [%s][%s] iInventoryPos is out of bound : %d", gObj[iIndex].AccountID, gObj[iIndex].Name, iInventoryPos);
		gObj[iIndex].ChaosLock = FALSE;
		GCAnsJewelUnMix(iIndex, 5);
		return FALSE;
	}

	int iInventoryItemType;
	int iInventoryItemLevel;

	if ( gObj[iIndex].pInventory[iInventoryPos].IsItem() == TRUE )
	{
		if ( gObj[iIndex].pInventory[iInventoryPos].m_Level == iJewelLevel )
		{
			iInventoryItemType = gObj[iIndex].pInventory[iInventoryPos].m_Type;
			iInventoryItemLevel = gObj[iIndex].pInventory[iInventoryPos].m_Level;
		}
		else
		{
			CLog.LogAdd("[JewelMix] [%s][%s] iJewelLevel is different from request : %d / %d",
				gObj[iIndex].AccountID, gObj[iIndex].Name,
				gObj[iIndex].pInventory[iInventoryPos].m_Level, iJewelLevel);

			gObj[iIndex].ChaosLock = FALSE;
			GCAnsJewelUnMix(iIndex, 3);
			return FALSE;
		}
	}
	else
	{
		CLog.LogAdd("[JewelMix] [%s][%s] Item to unmix is not exist",
			gObj[iIndex].AccountID, gObj[iIndex].Name);
	
		gObj[iIndex].ChaosLock = FALSE;
		GCAnsJewelUnMix(iIndex, 4);
		return FALSE;
	}

	if ( !CHECK_LIMIT(iInventoryItemLevel, 3))
	{
		CLog.LogAdd("[JewelMix] [%s][%s] iInventoryItemLevel is out of bound : %d",
			gObj[iIndex].AccountID, gObj[iIndex].Name, iInventoryItemLevel);

		gObj[iIndex].ChaosLock = FALSE;
		GCAnsJewelUnMix(iIndex, 3);
		return FALSE;
	}

	int iItemType = g_JewelMixInfo[iJewelType][iInventoryItemLevel].m_iChangeType;
	int iJewelCount = g_JewelMixInfo[iJewelType][iInventoryItemLevel].m_iJewelCount;
	int iChangeType = g_JewelMixInfo[iJewelType][iInventoryItemLevel].m_iSourceType;
	int iMixMoney = 1000000;

	if ( iInventoryItemType != iItemType )
	{
		CLog.LogAdd("[JewelMix] [%s][%s] iItemType is different from request : %d / %d",
			gObj[iIndex].AccountID, gObj[iIndex].Name,
			iInventoryItemType, iItemType);

		gObj[iIndex].ChaosLock = FALSE;
		GCAnsJewelUnMix(iIndex, 6);
		return FALSE;
	}

	if ( iMixMoney > gObj[iIndex].Money )
	{
		CLog.LogAdd("[JewelMix] [%s][%s] lack of money to unmix : %d / %d",
			gObj[iIndex].AccountID, gObj[iIndex].Name,
			gObj[iIndex].Money, iMixMoney);

		gObj[iIndex].ChaosLock = FALSE;
		GCAnsJewelUnMix(iIndex, 8);
		return FALSE;
	}

	int iEmptyCount = 0;

	for ( int x=0;x<(MAIN_INVENTORY_SIZE-INVETORY_WEAR_SIZE);x++)
	{
		if ( gObj[iIndex].pInventoryMap[x] == 0xFF )
		{
			iEmptyCount++;
		}
	}

	if ( iEmptyCount < iJewelCount )
	{
		CLog.LogAdd("[JewelMix] [%s][%s] lack of empty slot to unmix : %d / %d",
			gObj[iIndex].AccountID, gObj[iIndex].Name,
			iEmptyCount, iJewelCount);

		gObj[iIndex].ChaosLock = FALSE;
		GCAnsJewelUnMix(iIndex, 7);
		return FALSE;
	}

	CLog.LogAdd("[JewelMix] [%s][%s] UnMix - Delete Jewel, Type:%d, Level:%d, Serial:%d",
		gObj[iIndex].AccountID, gObj[iIndex].Name,
		gObj[iIndex].pInventory[iInventoryPos].m_Type,
		gObj[iIndex].pInventory[iInventoryPos].m_Level,
		gObj[iIndex].pInventory[iInventoryPos].m_Number);

	gObjInventoryItemSet(iIndex, iInventoryPos, 0xFF);
	gObjInventoryDeleteItem(iIndex, iInventoryPos);
	GCInventoryItemDeleteSend(iIndex, iInventoryPos, 1);

	int iCrtJewelCount = 0;
	BOOL bItemCrtOK = FALSE;

	BYTE Socket[5];
	Socket[0] = 0xFF;
	Socket[1] = 0xFF;
	Socket[2] = 0xFF;
	Socket[3] = 0xFF;
	Socket[4] = 0xFF;

	for ( int x=0;x<(MAIN_INVENTORY_SIZE-INVETORY_WEAR_SIZE);x++)
	{
		if ( gObj[iIndex].pInventoryMap[x] == 0xFF )
		{
			ItemSerialCreateSend(iIndex, 235, (BYTE)gObj[iIndex].X, (BYTE)gObj[iIndex].Y,
								iChangeType, 0, 0, 0, 0, 0,
								iIndex, 0, 0, Socket);

			iCrtJewelCount++;

			if ( iJewelCount <= iCrtJewelCount )
			{
				bItemCrtOK = TRUE;
				break;
			}
		}
	}

	gObj[iIndex].ChaosLock = FALSE;

	if ( bItemCrtOK == TRUE )
	{
		CLog.LogAdd("[JewelMix] [%s][%s] jewel unmix succeed : ItemType:%d, JewelCount:%d",
			gObj[iIndex].AccountID, gObj[iIndex].Name,
			iItemType, iJewelCount);

		gObj[iIndex].Money -= iMixMoney;
		GCMoneySend(iIndex, gObj[iIndex].Money);
		GCAnsJewelUnMix(iIndex, 1);
	}
	else
	{
		CLog.LogAdd("[JewelMix] [%s][%s] jewel unmix failed : ItemType:%d, JewelCount:%d",
			gObj[iIndex].AccountID, gObj[iIndex].Name,
			iItemType, iJewelCount);

		GCAnsJewelUnMix(iIndex, 0);
	}

	return TRUE;
}