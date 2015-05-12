//**********************************************************************//
// Project:	HaRiBO MU GameServer - Season 6								//
// Company: RealCoderZ MU Development © 2010							//
// Description: MU Chaos Box Structures & Functions						//
// Coded & Modified By:	WolF & M.E.S									//
//**********************************************************************//
#include "../Header Files/stdafx.h"
#include "../Header Files/ChaosBox.h"
#include "../Header Files/LogProc.H"
#include "../Header Files/GameMain.h"
#include "../Header Files/Main.H"
#include "../Header Files/DSProtocol.h"
#include "../Header Files/CastleSiegeSync.h"
#include "../Header Files/BloodCastle.h"
#include "../Header Files/SetItemOption.h"
#include "../Header Files/JewelOfHarmonySystem.h"
#include "../Test/Network.H"
#include "../Header Files/CrywolfSync.h"
#include "../Header Files/CashLotterySystem.h"
#include "../Header Files/DevilSquare.h"
#include "../Header Files/CherryBlossom.h"
#include "../Header Files/Pets.h"
#include "../Header Files/Functions.H"
#include "../Header Files/Notice.H"
#include "../Header Files/SendProto.H"


CChaosBox  gChaosBox;

CChaosBox::CChaosBox(void)
{
	// ----
}

CChaosBox::~CChaosBox(void)
{
	// ----
}

bool CChaosBox::ChaosBoxCheck(LPOBJ lpObj)
{
	if ( lpObj->pChaosBox == NULL )
	{
		return false;
	}
	return true;
}

bool CChaosBox::ChaosBoxInit(LPOBJ lpObj)
{
	int n;

	if ( lpObj->pChaosBox != NULL )
	{
		for (n=0;n<CHAOS_BOX_SIZE;n++)
		{
			lpObj->pChaosBox[n].Clear();
		}

		for (n=0;n<CHAOS_BOX_SIZE;n++)
		{
			lpObj->pChaosBoxMap[n] = -1;
		}
		return true;
	}

	lpObj->pChaosBox = new CItem[CHAOS_BOX_SIZE];

	if ( lpObj->pChaosBox == NULL )
	{
		return false;
	}

	lpObj->pChaosBoxMap = new unsigned char[CHAOS_BOX_SIZE];

	if ( lpObj->pChaosBoxMap == NULL )
	{
		delete lpObj->pChaosBox;
		return false;
	}

	for (n=0;n<CHAOS_BOX_SIZE;n++)
	{
		lpObj->pChaosBoxMap[n] = -1;
	}

	return true;
}

BOOL ChaosBoxItemDown(LPOBJ lpObj)
{
	if ( lpObj->pChaosBox == NULL )
	{
		return FALSE;
	}

	for (int n=0;n<CHAOS_BOX_SIZE;n++)
	{
		if ( lpObj->pChaosBox[n].m_Type == ITEMGET(12,15) || lpObj->pChaosBox[n].m_Type == ITEMGET(14,13) || lpObj->pChaosBox[n].m_Type == ITEMGET(14,14))
		{
			lpObj->pChaosBox[n].Clear();
		}
		else
		{
			int op = lpObj->pChaosBox[n].m_Option1;

			if ( op > 0 )
			{
				if ( (rand() % 2) == 0 )
				{
					op--;
				}
			}

			lpObj->pChaosBox[n].m_Option1 = op;
			op = lpObj->pChaosBox[n].m_Option3;

			if ( op > 0 )
			{
				if ( (rand() % 2) == 0 )
				{
					op--;
				}
			}

			lpObj->pChaosBox[n].m_Option3 = op;

			if ( lpObj->pChaosBox[n].m_Level > 0 )
			{
				lpObj->pChaosBox[n].m_Level = rand() % lpObj->pChaosBox[n].m_Level;
			}

			float dur = ItemGetDurability(lpObj->pChaosBox[n].m_Type, lpObj->pChaosBox[n].m_Level, lpObj->pChaosBox[n].IsExtItem(), lpObj->pChaosBox[n].IsSetItem());
			lpObj->pChaosBox[n].m_Durability =  dur * lpObj->pChaosBox[n].m_Durability / lpObj->pChaosBox[n].m_BaseDurability;
			lpObj->pChaosBox[n].Convert(lpObj->pChaosBox[n].m_Type, lpObj->pChaosBox[n].m_Option1, lpObj->pChaosBox[n].m_Option2, lpObj->pChaosBox[n].m_Option3, lpObj->pChaosBox[n].m_NewOption, lpObj->pChaosBox[n].m_SetOption, lpObj->pChaosBox[n].m_ItemOptionEx,lpObj->pChaosBox[n].m_ItemSocket);
		}
	}

	return TRUE;
}

int ChaosBoxMix(LPOBJ lpObj, int & Result2)
{
	BYTE ExOption[8];
	int ChaosDiamond = 0;
	int ChaosItems = 0;

	if ( lpObj->pChaosBox == NULL )
	{
		return 0;
	}

	int value = 0;
	int add = 0;
	int nv = 0;	// NEW VALUE
	Result2 = 0;
	lpObj->ChaosSuccessRate = 0;
	lpObj->ChaosMoney = 0;
	int iCharmOfLuckCount = 0;

	for ( int n=0;n<CHAOS_BOX_SIZE;n++)
	{
		if ( lpObj->pChaosBox[n].IsItem() == TRUE )
		{
			lpObj->pChaosBox[n].OldValue();
			add = 0;
			nv = 0;

			if ( lpObj->pChaosBox[n].m_Level >= MIN_CHAOS_ITEM_LEVEL && (lpObj->pChaosBox[n].m_Option3 *4) >= MIN_CHAOS_ITEM_LEVEL )
			{
				nv = lpObj->pChaosBox[n].m_OldBuyMoney;
				value += lpObj->pChaosBox[n].m_OldBuyMoney;
				add = 1;

				if ( lpObj->pChaosBox[n].m_Type == ITEMGET(2,6) || lpObj->pChaosBox[n].m_Type == ITEMGET(4,6) || lpObj->pChaosBox[n].m_Type == ITEMGET(5,7) )	// Chaos Items
				{
					Result2 = 1;
				}
			}

			if ( lpObj->pChaosBox[n].m_Type == ITEMGET(12,15) )
			{
				value += lpObj->pChaosBox[n].m_OldBuyMoney;
				nv = lpObj->pChaosBox[n].m_OldBuyMoney;
				ChaosDiamond++;
				add = 1;
			}
			else if ( lpObj->pChaosBox[n].m_Type == ITEMGET(14,13) || lpObj->pChaosBox[n].m_Type == ITEMGET(14,14) )	// Jewel of Bless, Jewel of Soul
			{
				value += lpObj->pChaosBox[n].m_OldBuyMoney;
				nv = lpObj->pChaosBox[n].m_OldBuyMoney;
				add = 1;
			}

			if ( lpObj->pChaosBox[n].m_Type == ITEMGET(13,14) ) // Loch Feather
			{
				return FALSE;
			}

			if ( lpObj->pChaosBox[n].m_Type == ITEMGET(14,53) )	// Charm Of Luck
			{
				iCharmOfLuckCount += lpObj->pChaosBox[n].m_Durability;
			}

			if ( add != 0  )
			{
				ChaosItems++;
			}

			ItemIsBufExOption(ExOption, &lpObj->pChaosBox[n]);
			CLog.LogAdd("[%s][%s] CBMix [%d,%s,%d,%d,%d,%d]serial:[%d][%d][%d] Ex:[%d,%d,%d,%d,%d,%d,%d]",
				lpObj->AccountID, lpObj->Name, n, lpObj->pChaosBox[n].GetName(),
				lpObj->pChaosBox[n].m_Level, lpObj->pChaosBox[n].m_Option1,
				lpObj->pChaosBox[n].m_Option2, lpObj->pChaosBox[n].m_Option3,
				lpObj->pChaosBox[n].m_Number, (int)lpObj->pChaosBox[n].m_Durability,
				nv, ExOption[0], ExOption[1], ExOption[2], ExOption[3], ExOption[4], ExOption[5],
				ExOption[6], lpObj->pChaosBox[n].m_SetOption);
		}
	}

	if ( ChaosDiamond == 0 )
	{
		value = 0;	// 0% of success
	}

	if ( ChaosItems < 2 )
	{
		value = 0;// 0% of success
	}

	if ( iCharmOfLuckCount > 10 )
		return FALSE;

	lpObj->ChaosSuccessRate = value / 20000;
	lpObj->ChaosSuccessRate += iCharmOfLuckCount;

	if ( lpObj->ChaosSuccessRate  > 100 )
	{
		lpObj->ChaosSuccessRate = 100;
	}

	lpObj->ChaosMoney = lpObj->ChaosSuccessRate * 10000;

	CLog.LogAdd("[%s][%s] CBMix need Zen : %d SuccessRate : %d, CharmRate : %d", lpObj->AccountID, lpObj->Name, lpObj->ChaosMoney, lpObj->ChaosSuccessRate, iCharmOfLuckCount);
	return value;
}

struct CB_PMSG_REQ_PRESENT
{
	PBMSG_HEAD h;
	char AccountId[10];
	char GameId[10];
	int Sequence;
};

void CBUPS_ItemRequest(int aIndex, char* AccountId, char* Name)
{
	CB_PMSG_REQ_PRESENT pMsg;

	pMsg.h.c = 0xC1;
	pMsg.h.headcode = 0x01;
	pMsg.h.size = sizeof(pMsg);
	pMsg.Sequence = aIndex;
	memcpy(pMsg.AccountId, AccountId, sizeof(pMsg.AccountId));
	memcpy(pMsg.GameId, Name, sizeof(pMsg.GameId));

	gUdpSocCE.SendData((LPBYTE)&pMsg, pMsg.h.size);
}

void ChaosEventProtocolCore(BYTE protoNum, unsigned char* aRecv, int aLen)
{
	switch ( protoNum )
	{
		case 0x01:
			CBUPR_ItemRequestRecv( (CB_PMSG_REQ_PRESENT_RESULT *)aRecv);
			break;
	}
}

struct PMSG_PRIZE_INFO
{
	PBMSG_HEAD h;
	char AccountID[10];
	char GameID[10];
	int servernumber;
	char present;
	int Seq_present;
};

void CBUPR_ItemRequestRecv( CB_PMSG_REQ_PRESENT_RESULT * lpMsg)
{
	LPOBJ lpObj;
	char szId[11];
	char szName[11];
	char szPresentName[51];
	int aIndex;
	PMSG_PRIZE_INFO pMsg;

	szId[10] = 0;
	szName[10] = 0;
	memset(szPresentName, 0, sizeof(szPresentName));
	memcpy(szId, lpMsg->AccountId, sizeof(lpMsg->AccountId));
	memcpy(szName, lpMsg->GameId, sizeof(lpMsg->GameId));
	memcpy(szPresentName, lpMsg->presentname, sizeof(lpMsg->presentname));
	aIndex = lpMsg->Sequence;

	if (gObjIsConnectedGP(aIndex) == FALSE )
	{
		CLog.LogAddC(2, "[ChaosBox]: Error -> Player is Offline. File: %s, Line: %d", __FILE__, __LINE__);
		return;
	}

	lpObj = &gObj[aIndex];

	if ( strcmp(szId, lpObj->AccountID) != 0 )
	{
		CLog.LogAddC(2, "error-L3 [%s][%d][%s == %s]", __FILE__, __LINE__, szId, lpObj->AccountID);
		return;
	}

	if ( strcmp(szName, lpObj->Name) != 0 )
	{
		CLog.LogAddC(2, "error-L3 [%s][%d]", __FILE__, __LINE__);
		return;
	}

	if ( lpMsg->present != -1 )
	{
		char szTemp[256]="";

		wsprintf(szTemp, lMsg.Get(MSGGET(6, 65)), szName, szPresentName);
		SendMsg.NormalMsgOut(OBJ_EMPTY, OBJ_SEND_ALL_PLYRS, NO_SPC_MAP, szTemp);

		pMsg.h.c = 0xC1;
		pMsg.h.headcode = 0x02;
		pMsg.h.size = sizeof(pMsg);
		pMsg.present = lpMsg->present;
		pMsg.Seq_present = lpMsg->Seq_present;
		pMsg.servernumber = gGameServerCode;
		memcpy(pMsg.AccountID, lpMsg->AccountId, sizeof(pMsg.AccountID));
		memcpy(pMsg.GameID, lpMsg->GameId, sizeof(pMsg.GameID));

		gUdpSocCE.SendData((LPBYTE)&pMsg, pMsg.h.size);

		BYTE Socket[5];
		Socket[0] = 0xFF;
		Socket[1] = 0xFF;
		Socket[2] = 0xFF;
		Socket[3] = 0xFF;
		Socket[4] = 0xFF;

		CLog.LogAdd("[DevilSquare] Present Prize Notice %d(%s) %d %s %s", lpMsg->present, szPresentName, lpMsg->Seq_present, szId, szName);

		ItemSerialCreateSend(lpObj->m_Index, -1, 0, 0, 0x1FF, 0, 255, 0, 0, 0, -1, 0, 0, Socket);
		lpObj->Money -= 10000;
		GCMoneySend(lpObj->m_Index, lpObj->Money);

		PMSG_CHAOSMIXRESULT pMsg;

		C1HeadSet((LPBYTE)&pMsg, 0x86, sizeof(pMsg));
		pMsg.Result = 5;

		gSendProto.DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
	}
	else
	{
		BYTE Socket[5];
		Socket[0] = 0xFF;
		Socket[1] = 0xFF;
		Socket[2] = 0xFF;
		Socket[3] = 0xFF;
		Socket[4] = 0xFF;
		int in = ITEMGET(14,19); // Devil Square Invitation
		ItemSerialCreateSend(lpObj->m_Index, -1, 0, 0, in, 0, 0, 0, 0, 0, -1, 0, 0, Socket);
		lpObj->Money -= 10000;
		GCMoneySend(lpObj->m_Index, lpObj->Money);
		lpObj->ChaosLock = FALSE;

		CLog.LogAdd("[DevilSquare] [%s][%s] CBMix Success %d Money : %d-%d", lpObj->AccountID, lpObj->Name, lpObj->ChaosSuccessRate, lpObj->Money, lpObj->ChaosMoney);
	}
}

int  CheckDevilSquareItem(LPOBJ lpObj, int & eventitemcount, int & itemlevel)
{
	BOOL bChaoseGem = FALSE;
	BOOL bEyeOfDevil = FALSE;
	BOOL bKeyOfDevil = FALSE;
	eventitemcount = 0;
	BOOL FoundOtherItem = FALSE;
	int level = -1;
	BOOL bLevelCheck = FALSE;
	int iCharmOfLuckCount = 0;
	BOOL bCharmOfLuckOver = FALSE;

	CLog.LogAdd("[DevilSquare] Search DevilSquareItem");

	for ( int n=0;n<CHAOS_BOX_SIZE;n++)
	{
		if ( lpObj->pChaosBox[n].IsItem() == TRUE )
		{
			if ( lpObj->pChaosBox[n].m_Type == ITEMGET(12,15) )	// Chaos
			{
				bChaoseGem = TRUE;
			}
			else if ( lpObj->pChaosBox[n].m_Type == ITEMGET(14,17) )	// Eye
			{
				eventitemcount +=1;
				bEyeOfDevil = TRUE;

				if ( level != lpObj->pChaosBox[n].m_Level )
				{
					if ( level == -1 )
					{
						level = lpObj->pChaosBox[n].m_Level;
					}
					else
					{
						bLevelCheck = TRUE;
					}
				}
			}
			else if ( lpObj->pChaosBox[n].m_Type == ITEMGET(14,18) )	// Key
			{
				eventitemcount +=1;
				bKeyOfDevil = TRUE;

				if ( level != lpObj->pChaosBox[n].m_Level )
				{
					if ( level == -1 )
					{
						level = lpObj->pChaosBox[n].m_Level;
					}
					else
					{
						bLevelCheck = TRUE;
					}
				}
			}
			else if ( lpObj->pChaosBox[n].m_Type == ITEMGET(14,53) )		// Charm Of Luck
			{
				iCharmOfLuckCount += lpObj->pChaosBox[n].m_Durability;

				if ( iCharmOfLuckCount > 10 )
				{
					bCharmOfLuckOver = TRUE;
				}
			}
			else
			{
				FoundOtherItem = TRUE;
			}
		}
	}

	itemlevel = level;
	lpObj->ChaosSuccessRate = iCharmOfLuckCount;

	if ( FoundOtherItem != FALSE )
	{
		CLog.LogAdd("[DevilSquare] Other DQItems Found");
		return 0;
	}

	if ( bLevelCheck != FALSE )
	{
		CLog.LogAdd("[DevilSquare] DiffLevel DQItems Found");
		return 3;
	}

	if ( bCharmOfLuckOver == TRUE )
	{
	CLog.LogAdd("[DevilSquare] Charm Of Luck Count Over");
		return 4;
	}

	if ( bChaoseGem != FALSE && bEyeOfDevil != FALSE && bKeyOfDevil != FALSE )
	{
		LogDQChaosItem(lpObj);
		return 1;
	}

	if ( bEyeOfDevil != FALSE && bKeyOfDevil != FALSE )
	{
		LogDQChaosItem(lpObj);
		return 2;
	}

	CLog.LogAdd("[DevilSquare] DQItems Not Found");

	return 0;
}

void LogDQChaosItem(LPOBJ lpObj)
{
	BYTE ExOption[MAX_EXOPTION_SIZE];

	for ( int n=0;n<CHAOS_BOX_SIZE;n++)
	{
		if ( lpObj->pChaosBox[n].IsItem() == TRUE )
		{
			::ItemIsBufExOption(ExOption, &lpObj->pChaosBox[n] );

			CLog.LogAdd("[DevilSquare,%d] [%s][%s] CBMix [%d,%s,%d,%d,%d,%d]serial:[%d][%d] Ex:[%d,%d,%d,%d,%d,%d,%d] Set:[%d] 380Ex[%d] HO:[%d,%d]",
				lpObj->pChaosBox[n].m_Level,
				lpObj->AccountID, lpObj->Name, n, lpObj->pChaosBox[n].GetName(), lpObj->pChaosBox[n].m_Level,
				lpObj->pChaosBox[n].m_Option1, lpObj->pChaosBox[n].m_Option2, lpObj->pChaosBox[n].m_Option3, 
				lpObj->pChaosBox[n].m_Number, (int)lpObj->pChaosBox[n].m_Durability, 
				ExOption[0], ExOption[1], ExOption[2], ExOption[3], ExOption[4], ExOption[5], ExOption[6],
				lpObj->pChaosBox[n].m_SetOption, lpObj->pChaosBox[n].m_ItemOptionEx >> 7,
				g_kJewelOfHarmonySystem.GetItemStrengthenOption(&lpObj->pChaosBox[n]),
				g_kJewelOfHarmonySystem.GetItemOptionLevel(&lpObj->pChaosBox[n]));
		}
	}
}

BOOL DevilSquareEventChaosMix(LPOBJ lpObj, BOOL bCheckType, int iItemLevel)
{
	BOOL fail = TRUE;
	int aIndex = lpObj->m_Index;
	PMSG_CHAOSMIXRESULT pMsg;

	C1HeadSet((LPBYTE)&pMsg.h, 0x86, sizeof(PMSG_CHAOSMIXRESULT));
	pMsg.Result = CB_ERROR;
	lpObj->ChaosLock = TRUE;

	CLog.LogAdd("[DevilSquare] Chaos Mix Start");
	INT nChaosNeedMoney = 0;
	int iCharmOfLuckCount = 0;
	iCharmOfLuckCount = lpObj->ChaosSuccessRate;

	if ( iCharmOfLuckCount > 10 )
	{
		pMsg.Result = 0xF0;
		lpObj->ChaosLock = FALSE;
		gSendProto.DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
	}

	switch ( iItemLevel )
	{
		case 0:
			lpObj->ChaosSuccessRate = 60;
			nChaosNeedMoney = 100000;
			break;

		case 1:
			lpObj->ChaosSuccessRate = g_DevilSquare.DS1ChaosSuccessRate;
			nChaosNeedMoney = 100000;
			break;

		case 2:
			lpObj->ChaosSuccessRate = g_DevilSquare.DS2ChaosSuccessRate;
			nChaosNeedMoney = 200000;
			break;

		case 3:
			lpObj->ChaosSuccessRate = g_DevilSquare.DS3ChaosSuccessRate;
			nChaosNeedMoney = 400000;
			break;

		case 4:
			lpObj->ChaosSuccessRate = g_DevilSquare.DS4ChaosSuccessRate;
			nChaosNeedMoney = 700000;
			break;

		case 5:
			lpObj->ChaosSuccessRate = g_DevilSquare.DS5ChaosSuccessRate;
			nChaosNeedMoney = 1100000;
			break;

		case 6:
			lpObj->ChaosSuccessRate = g_DevilSquare.DS6ChaosSuccessRate;
			nChaosNeedMoney = 1600000;
			break;

		case 7:
			lpObj->ChaosSuccessRate = g_DevilSquare.DS7ChaosSuccessRate;
			nChaosNeedMoney = 2100000;
			break;

		default:
			CLog.LogAdd("[DevilSquare] [%s][%s] Invalid DQItem Level [%d]", lpObj->AccountID, lpObj->Name, iItemLevel);

			pMsg.Result = CB_INVALID_ITEM_LEVEL;
			gSendProto.DataSend(aIndex, (BYTE *)&pMsg, pMsg.h.size);
			lpObj->ChaosLock = FALSE;
			return 1;
			break;
	}

	int iChaosTaxMoney = (int)((__int64)nChaosNeedMoney * (__int64)g_CastleSiegeSync.GetTaxRateChaos(lpObj->m_Index) / (__int64)100);

	if ( iChaosTaxMoney < 0 )
	{
		iChaosTaxMoney = 0;
	}

	nChaosNeedMoney += iChaosTaxMoney;

	if ( nChaosNeedMoney < 0 )
	{
		nChaosNeedMoney = 0;
	}

	if ( (lpObj->Money - nChaosNeedMoney) < 0 )
	{
		CLog.LogAdd("[DevilSquare] [%s][%s] CBMix Not Enough Money [%d] need zen [%d]", lpObj->AccountID, lpObj->Name, lpObj->Money, nChaosNeedMoney);
		pMsg.Result = CB_NOT_ENOUGH_ZEN;
		gSendProto.DataSend(aIndex, (BYTE *)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;
		return 1;

	}

	if ( g_CrywolfSync.GetOccupationState() == 0 && g_iCrywolfApplyMvpBenefit )
	{	
		lpObj->ChaosSuccessRate += g_CrywolfSync.GetPlusChaosRate();
	}

	lpObj->ChaosSuccessRate += iCharmOfLuckCount;

	int iRate = rand() % 100;

	BYTE Socket[5];
	Socket[0] = 0xFF;
	Socket[1] = 0xFF;
	Socket[2] = 0xFF;
	Socket[3] = 0xFF;
	Socket[4] = 0xFF;

	if ( bCheckType == TRUE )
	{
		if ( iRate < lpObj->ChaosSuccessRate )
		{
			int DevilInv = ITEMGET(14,19);	// Devil Ticket
			ItemSerialCreateSend(lpObj->m_Index, -1, 0, 0, DevilInv, iItemLevel, 0, 0, 0, 0, -1, 0, 0, Socket);
			fail = FALSE;
		}
	}

	lpObj->Money -= nChaosNeedMoney;
	g_CastleSiegeSync.AddTributeMoney(iChaosTaxMoney);
	GCMoneySend(lpObj->m_Index, lpObj->Money);

	if ( fail == TRUE )
	{
		gChaosBox.ChaosBoxInit(lpObj);
		GCUserChaosBoxSend(lpObj, 0);
		gSendProto.DataSend(aIndex, (BYTE *)&pMsg, pMsg.h.size);
		CLog.LogAdd("[DevilSquare,%d] [%s][%s] CBMix Fail %d Money : %d-%d", iItemLevel, lpObj->AccountID, lpObj->Name, lpObj->ChaosSuccessRate, lpObj->Money, nChaosNeedMoney);
		lpObj->ChaosLock = FALSE;
	}
	else
	{
		CLog.LogAdd("[DevilSquare,%d] [%s][%s] CBMix Success Rate:%d Money : %d", iItemLevel, lpObj->AccountID, lpObj->Name, lpObj->ChaosSuccessRate, nChaosNeedMoney);
	}

	::gObjInventoryCommit(lpObj->m_Index);

	return TRUE;
}

void LogPlusItemLevelChaosItem(LPOBJ lpObj, int iPlusMixLevel)
{
	int iMixLevel = 0;
	BYTE ExOption[MAX_EXOPTION_SIZE];

	for ( int n=0;n<CHAOS_BOX_SIZE;n++)
	{
		if ( lpObj->pChaosBox[n].IsItem() == TRUE )
		{
			::ItemIsBufExOption(ExOption, &lpObj->pChaosBox[n]);

			if ( iPlusMixLevel == 3 )
				iMixLevel = 1;
			else if ( iPlusMixLevel == 4 )
				iMixLevel = 2;
			else if ( iPlusMixLevel == 22 )
				iMixLevel = 3;
			else if ( iPlusMixLevel == 23 )
				iMixLevel = 4;
			else if ( iPlusMixLevel == 49 )
				iMixLevel = 5;
			else if ( iPlusMixLevel == 50 )
				iMixLevel = 6;

			CLog.LogAdd("[PlusItemLevel,%d] [%s][%s] CBMix [%d,%s,%d,%d,%d,%d]serial:[%d][%d] Ex:[%d,%d,%d,%d,%d,%d,%d] Set:[%d] 380Ex:[%d] HO:[%d,%d]", iMixLevel,
				lpObj->AccountID, lpObj->Name, n, lpObj->pChaosBox[n].GetName(), lpObj->pChaosBox[n].m_Level,
				lpObj->pChaosBox[n].m_Option1, lpObj->pChaosBox[n].m_Option2, lpObj->pChaosBox[n].m_Option3, 
				lpObj->pChaosBox[n].m_Number, (int)lpObj->pChaosBox[n].m_Durability, 
				ExOption[0], ExOption[1], ExOption[2], ExOption[3], ExOption[4], ExOption[5], ExOption[6], lpObj->pChaosBox[n].m_SetOption,
				lpObj->pChaosBox[n].m_ItemOptionEx >> 7,
				g_kJewelOfHarmonySystem.GetItemOptionLevel(&lpObj->pChaosBox[n]),	// #error Cahnge to GetItemStrengthenOption
				g_kJewelOfHarmonySystem.GetItemStrengthenOption(&lpObj->pChaosBox[n]));	// #error Cahnge to GetItemOptionLevel
		}
	}
}

void LogChaosItem(LPOBJ lpObj, LPSTR sLogType)
{
	BYTE ExOption[MAX_EXOPTION_SIZE];

	for ( int n=0;n<CHAOS_BOX_SIZE;n++)
	{
		if ( lpObj->pChaosBox[n].IsItem() == TRUE )
		{
			::ItemIsBufExOption(ExOption, &lpObj->pChaosBox[n]);

			CLog.LogAdd("[%s] [%s][%s] CBMix [%d,%s,%d,%d,%d,%d]serial:[%d][%d] Ex:[%d,%d,%d,%d,%d,%d,%d] Set:[%d] 380:[%d] HO:[%d,%d]",
				sLogType, lpObj->AccountID, lpObj->Name, n, lpObj->pChaosBox[n].GetName(), lpObj->pChaosBox[n].m_Level,
				lpObj->pChaosBox[n].m_Option1, lpObj->pChaosBox[n].m_Option2, lpObj->pChaosBox[n].m_Option3, 
				lpObj->pChaosBox[n].m_Number, (int)lpObj->pChaosBox[n].m_Durability, 
				ExOption[0], ExOption[1], ExOption[2], ExOption[3], ExOption[4], ExOption[5], ExOption[6], lpObj->pChaosBox[n].m_SetOption,
				lpObj->pChaosBox[n].m_ItemOptionEx >> 7,
				g_kJewelOfHarmonySystem.GetItemStrengthenOption(&lpObj->pChaosBox[n]),
				g_kJewelOfHarmonySystem.GetItemOptionLevel(&lpObj->pChaosBox[n]));
		}
	}
}
// +10, +11, +12, +13, +14, +15 Level Upgrade
BOOL PlusItemLevelChaosMix(LPOBJ lpObj, int mixType)
{
	int ChaosGemCount = 0;
	int BlessGemCount = 0;
	int SoulGemCount = 0;
	int PlusItemPos = -1;
	int OtherItemFound = 0;
	int Plus9ItemCount = 0;
	int Plus10ItemCount = 0;
	int Plus11ItemCount = 0;
	int Plus12ItemCount = 0;
	int Plus13ItemCount = 0;
	int Plus14ItemCount = 0;
	int ExtraBlessGemCount = 0;
	int ExtraSoulGemCount = 0;
	int iCharmOfLuckCount = 0;
	BYTE btItemLuckValue = 0;
	char MessageAux[255];
	sprintf_s(MessageAux,"Chaos Mix Type: %d",mixType);

	GCServerMsgStringSend(MessageAux,lpObj->m_Index,1);

	for ( int n=0;n<CHAOS_BOX_SIZE;n++)
	{
		if ( lpObj->pChaosBox[n].IsItem() == TRUE )
		{
			if ( lpObj->pChaosBox[n].m_Type == ITEMGET(12,15) ) // Jewel of Chaos
			{
				ChaosGemCount++;
			}
			else if (  lpObj->pChaosBox[n].m_Type == ITEMGET(14,13) ) // Jewel of Bless
			{
				BlessGemCount++;
			}
			else if (  lpObj->pChaosBox[n].m_Type == ITEMGET(14,14) ) // Jewel of Soul
			{
				SoulGemCount++;
			}
			else if ( lpObj->pChaosBox[n].m_Type == ITEMGET(14,53) ) // Charm Of Luck
			{
				iCharmOfLuckCount += lpObj->pChaosBox[n].m_Durability;
			}
			else if ( lpObj->pChaosBox[n].m_Level == 9 ) // Item +9
			{
				Plus9ItemCount++;
				PlusItemPos = n;
			}
			else if ( lpObj->pChaosBox[n].m_Level == 10 ) // Item +10
			{
				Plus10ItemCount++;
				PlusItemPos = n;
			}
			else if ( lpObj->pChaosBox[n].m_Level == 11 ) // Item +11
			{
				Plus11ItemCount++;
				PlusItemPos = n;
			}
			else if ( lpObj->pChaosBox[n].m_Level == 12 ) // Item +12
			{
				Plus12ItemCount++;
				PlusItemPos = n;
			}
			else if ( lpObj->pChaosBox[n].m_Level == 13 ) // Item +13
			{
				Plus13ItemCount++;
				PlusItemPos = n;
			}
			else if ( lpObj->pChaosBox[n].m_Level == 14 ) // Item +14
			{
				Plus14ItemCount++;
				PlusItemPos = n;
			}
			else
			{
				OtherItemFound ++;
			}
		}
	}

	if ( iCharmOfLuckCount > 10 )
	{
		PMSG_CHAOSMIXRESULT pResult;
		C1HeadSet((LPBYTE)&pResult, 0x86, sizeof(pResult));
		pResult.Result = 0xF0;

		gSendProto.DataSend(lpObj->m_Index, (LPBYTE)&pResult, pResult.h.size);

		CLog.LogAdd("[PlusItemLevel] [%s][%s] CBMix Charm of luck over 10% (%d)", lpObj->AccountID, lpObj->Name, iCharmOfLuckCount);
		lpObj->ChaosLock = FALSE;
		return 0;
	}

	if ( OtherItemFound != 0 )
	{
		return FALSE;
	}

	lpObj->ChaosMoney = 0;
	int MixType2 = 0;

	if ( Plus9ItemCount == 1 && ChaosGemCount == 1 && BlessGemCount == 1 && SoulGemCount == 1 && Plus10ItemCount == 0 && Plus11ItemCount == 0 && Plus12ItemCount == 0  && Plus13ItemCount == 0  && Plus14ItemCount == 0 )
	{
		MixType2 = CHAOS_TYPE_UPGRADE_10;
		lpObj->ChaosMoney = 2000000;
		ExtraBlessGemCount = BlessGemCount - 1;
		ExtraSoulGemCount = SoulGemCount - 1;
	}
	else if ( Plus10ItemCount == 1 && ChaosGemCount == 1 && BlessGemCount == 2 && SoulGemCount == 2 && Plus9ItemCount == 0 && Plus11ItemCount == 0 && Plus12ItemCount == 0  && Plus13ItemCount == 0  && Plus14ItemCount == 0 )
	{
		MixType2 = CHAOS_TYPE_UPGRADE_11;
		lpObj->ChaosMoney = 4000000;
		ExtraBlessGemCount = BlessGemCount - 2;
		ExtraSoulGemCount = SoulGemCount - 2;
	}
	else if ( Plus11ItemCount == 1 && ChaosGemCount == 1 && BlessGemCount == 3 && SoulGemCount == 3 && Plus9ItemCount == 0 && Plus10ItemCount == 0 && Plus12ItemCount == 0  && Plus13ItemCount == 0  && Plus14ItemCount == 0 )
	{
		MixType2 = CHAOS_TYPE_UPGRADE_12;
		lpObj->ChaosMoney = 6000000;
		ExtraBlessGemCount = BlessGemCount - 3;
		ExtraSoulGemCount = SoulGemCount - 3;
	}
	else if ( Plus12ItemCount == 1 && ChaosGemCount == 1 && BlessGemCount == 4 && SoulGemCount == 4 && Plus9ItemCount == 0 && Plus10ItemCount == 0 && Plus11ItemCount == 0 && Plus13ItemCount == 0  && Plus14ItemCount == 0 )
	{
		MixType2 = CHAOS_TYPE_UPGRADE_13;
		lpObj->ChaosMoney = 8000000;
		ExtraBlessGemCount = BlessGemCount - 4;
		ExtraSoulGemCount = SoulGemCount - 4;
	}
	else if ( Plus13ItemCount == 1 && ChaosGemCount == 1 && BlessGemCount == 5 && SoulGemCount == 5 && Plus9ItemCount == 0 && Plus10ItemCount == 0 && Plus11ItemCount == 0 && Plus12ItemCount == 0 && Plus14ItemCount == 0 )
	{
		MixType2 = CHAOS_TYPE_UPGRADE_14;
		lpObj->ChaosMoney = 10000000;
		ExtraBlessGemCount = BlessGemCount - 5;
		ExtraSoulGemCount = SoulGemCount - 5;
	}
	else if ( Plus14ItemCount == 1 && ChaosGemCount == 1 && BlessGemCount == 6 && SoulGemCount == 6 && Plus9ItemCount == 0 && Plus10ItemCount == 0 && Plus11ItemCount == 0 && Plus12ItemCount == 0 && Plus13ItemCount == 0 )
	{
		MixType2 = CHAOS_TYPE_UPGRADE_15;
		lpObj->ChaosMoney = 12000000;
		ExtraBlessGemCount = BlessGemCount - 6;
		ExtraSoulGemCount = SoulGemCount - 6;
	}

	if ( MixType2 != mixType )
	{
		MixType2 = 0;
	}

	if ( MixType2 == 0 )
	{
		return FALSE;
	}

	PMSG_CHAOSMIXRESULT pMsg;

	C1HeadSet((LPBYTE)&pMsg.h, 0x86, sizeof(PMSG_CHAOSMIXRESULT));
	pMsg.Result = CB_ERROR;
	lpObj->ChaosLock = TRUE;
	::LogPlusItemLevelChaosItem(lpObj, MixType2);
	CLog.LogAdd("[PlusItemLevel] Chaos Mix Start");

	int iChaosTaxMoney = (int)((__int64)lpObj->ChaosMoney * (__int64)g_CastleSiegeSync.GetTaxRateChaos(lpObj->m_Index) / (__int64)100);

	if ( iChaosTaxMoney < 0 )
	{
		iChaosTaxMoney = 0;
	}

	lpObj->ChaosMoney += iChaosTaxMoney;

	if ( lpObj->ChaosMoney < 0 )
	{
		lpObj->ChaosMoney = 0;
	}

	if ( lpObj->Money < lpObj->ChaosMoney )
	{
		CLog.LogAdd("[PlusItemLevel] [%s][%s] CBMix Not Enough Money [%d] need zen [%d]", lpObj->AccountID, lpObj->Name, lpObj->Money, lpObj->ChaosMoney);
		pMsg.Result = CB_NOT_ENOUGH_ZEN;
		gSendProto.DataSend(lpObj->m_Index, (BYTE *)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;
		
		return TRUE;
	}

	lpObj->Money -= lpObj->ChaosMoney;
	g_CastleSiegeSync.AddTributeMoney(iChaosTaxMoney);
	GCMoneySend(lpObj->m_Index, lpObj->Money);

	if ( lpObj->pChaosBox[PlusItemPos].m_Level == 9 )
	{
		lpObj->ChaosSuccessRate = 50;
	}
	else if ( lpObj->pChaosBox[PlusItemPos].m_Level == 10 )
	{
		lpObj->ChaosSuccessRate = 45;
	}
	else if ( lpObj->pChaosBox[PlusItemPos].m_Level == 11 )
	{
		lpObj->ChaosSuccessRate = 40;
	}
	else if ( lpObj->pChaosBox[PlusItemPos].m_Level == 12 )
	{
		lpObj->ChaosSuccessRate = 35;
	}
	else if ( lpObj->pChaosBox[PlusItemPos].m_Level == 13 )
	{
		lpObj->ChaosSuccessRate = 30;
	}
	else if ( lpObj->pChaosBox[PlusItemPos].m_Level == 14 )
	{
		lpObj->ChaosSuccessRate = 25;
	}

	if ( lpObj->pChaosBox[PlusItemPos].m_Option2 != FALSE )	// if Have Luck
	{
		lpObj->ChaosSuccessRate += 20;
	}

	if ( lpObj->ChaosSuccessRate > 75 )
	{
		lpObj->ChaosSuccessRate = 75;
	}

	lpObj->ChaosSuccessRate += iCharmOfLuckCount;

	if ( lpObj->ChaosSuccessRate > 100 )
	{
		lpObj->ChaosSuccessRate = 100;
	}

	btItemLuckValue = lpObj->pChaosBox[PlusItemPos].m_Option2;

	if ( (rand()%100) < lpObj->ChaosSuccessRate )
	{
		lpObj->pChaosBox[PlusItemPos].m_Level++;
		pMsg.Result = CB_SUCCESS;

		CItem Item;

		float Dur = ItemGetDurability(lpObj->pChaosBox[PlusItemPos].m_Type, lpObj->pChaosBox[PlusItemPos].m_Level, lpObj->pChaosBox[PlusItemPos].IsExtItem(), lpObj->pChaosBox[PlusItemPos].IsSetItem());

		Item.m_Level = lpObj->pChaosBox[PlusItemPos].m_Level;
		Item.m_Durability = Dur * lpObj->pChaosBox[PlusItemPos].m_Durability / lpObj->pChaosBox[PlusItemPos].m_BaseDurability;
		Item.m_JewelOfHarmonyOption = lpObj->pChaosBox[PlusItemPos].m_JewelOfHarmonyOption;

		Item.Convert(lpObj->pChaosBox[PlusItemPos].m_Type, lpObj->pChaosBox[PlusItemPos].m_Option1, lpObj->pChaosBox[PlusItemPos].m_Option2,
			lpObj->pChaosBox[PlusItemPos].m_Option3, lpObj->pChaosBox[PlusItemPos].m_NewOption, 
			lpObj->pChaosBox[PlusItemPos].m_SetOption, lpObj->pChaosBox[PlusItemPos].m_ItemOptionEx,lpObj->pChaosBox[PlusItemPos].m_ItemSocket);

		Item.m_Number = lpObj->pChaosBox[PlusItemPos].m_Number;

		ItemByteConvert(pMsg.ItemInfo, Item);
		gChaosBox.ChaosBoxInit(lpObj);
		::gObjChaosBoxInsertItemPos(lpObj->m_Index, Item, 0, -1);
		gObjChaosItemSet(lpObj->m_Index, 0, 1);

		gSendProto.DataSend(lpObj->m_Index, (BYTE *)&pMsg, pMsg.h.size);

		if ( btItemLuckValue )
		{
			CLog.LogAdd("[PlusItemLevel] [%s][%s] CBMix Success %d Money : %d-%d [%d], CharmRate : %d", lpObj->AccountID, lpObj->Name, lpObj->ChaosSuccessRate+5, lpObj->Money, lpObj->ChaosMoney, Item.m_Level, iCharmOfLuckCount);
		}
		else
		{
			CLog.LogAdd("[PlusItemLevel] [%s][%s] CBMix Success %d Money : %d-%d [%d], CharmRate : %d", lpObj->AccountID, lpObj->Name, lpObj->ChaosSuccessRate, lpObj->Money, lpObj->ChaosMoney, Item.m_Level, iCharmOfLuckCount);
		}
	}
	else
	{
		int FailLevel = lpObj->pChaosBox[PlusItemPos].m_Level+1;
		gChaosBox.ChaosBoxInit(lpObj);
		GCUserChaosBoxSend(lpObj, 0);

		gSendProto.DataSend(lpObj->m_Index, (BYTE *)&pMsg, pMsg.h.size);

		if ( btItemLuckValue )
		{
			CLog.LogAdd("[PlusItemLevel] [%s][%s] CBMix Fail %d Money : %d-%d [%d], CharmRate : %d", lpObj->AccountID, lpObj->Name, lpObj->ChaosSuccessRate+5, lpObj->Money, lpObj->ChaosMoney, FailLevel, iCharmOfLuckCount);
		}
		else
		{
			CLog.LogAdd("[PlusItemLevel] [%s][%s] CBMix Fail %d Money : %d-%d [%d], CharmRate : %d", lpObj->AccountID, lpObj->Name, lpObj->ChaosSuccessRate, lpObj->Money, lpObj->ChaosMoney, FailLevel, iCharmOfLuckCount);
		}
	}

	lpObj->ChaosLock = FALSE;
	return TRUE;
}
// Dinorant Mix
BOOL PegasiaChaosMix(LPOBJ lpObj)
{
	lpObj->ChaosLock = TRUE;
	int UniriaCount = 0;
	int ChoasGemCount = 0;
	int iCharmOfLuckCount = 0;

	for ( int n=0;n<CHAOS_BOX_SIZE;n++)
	{
		if ( lpObj->pChaosBox[n].IsItem() == TRUE )
		{
			if ( lpObj->pChaosBox[n].m_Type == ITEMGET(13,2) && lpObj->pChaosBox[n].m_Durability == 255.0f ) // Uniria
			{
				UniriaCount++;
			}
			else if ( lpObj->pChaosBox[n].m_Type == ITEMGET(12,15) ) // Jewel of Chaos
			{
				ChoasGemCount++;
			}
			else if ( lpObj->pChaosBox[n].m_Type == ITEMGET(14,53) ) // Charm Of Luck
			{
				iCharmOfLuckCount += lpObj->pChaosBox[n].m_Durability;
			}
		}
	}

	if ( UniriaCount != 10 && ChoasGemCount != 1 )
	{
		lpObj->ChaosLock = FALSE;
		return FALSE;
	}

	PMSG_CHAOSMIXRESULT pMsg;

	C1HeadSet((LPBYTE)&pMsg.h, 0x86, sizeof(PMSG_CHAOSMIXRESULT));
	pMsg.Result = CB_ERROR;

	if ( iCharmOfLuckCount > 10 )
	{
		lpObj->ChaosLock = FALSE;
		pMsg.Result = 0xF0;
		gSendProto.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);

		return 0;
	}

	int nChaosNeedMoney = 500000;
	int iChaosTaxMoney = (int)((__int64)nChaosNeedMoney * (__int64)g_CastleSiegeSync.GetTaxRateChaos(lpObj->m_Index) / (__int64)100);

	if ( iChaosTaxMoney < 0 )
	{
		iChaosTaxMoney = 0;
	}

	nChaosNeedMoney += iChaosTaxMoney;

	if ( nChaosNeedMoney < 0 )
	{
		nChaosNeedMoney = 0;
	}

	if ( lpObj->Money < nChaosNeedMoney )
	{
		lpObj->ChaosLock = FALSE;
		pMsg.Result = CB_NOT_ENOUGH_ZEN;
		
		gSendProto.DataSend(lpObj->m_Index, (BYTE *)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		return TRUE;
	}

	LogChaosItem(lpObj, "DinorantMix");
	CLog.LogAdd("[DinorantMix] Chaos Mix Start");
	lpObj->ChaosSuccessRate = 70;	// Succes Rate for Dinorant
	lpObj->ChaosSuccessRate += iCharmOfLuckCount;
	lpObj->Money -= nChaosNeedMoney;

	g_CastleSiegeSync.AddTributeMoney(iChaosTaxMoney);
	GCMoneySend(lpObj->m_Index, lpObj->Money);

	if ( (rand()%100 ) < lpObj->ChaosSuccessRate )
	{
		int Dinorant = ITEMGET(13,3);
		int Option3 = 0;

		if ( (rand()% 100) < 30 )
		{
			Option3 = 1 <<  ((rand()%3)) ;

			if ( (rand()%5) == 0 )
			{
				Option3 |= 1 << (rand()%3);
			}
		}
		BYTE Socket[5];
		Socket[0] = 0xFF;
		Socket[1] = 0xFF;
		Socket[2] = 0xFF;
		Socket[3] = 0xFF;
		Socket[4] = 0xFF;
		::ItemSerialCreateSend(lpObj->m_Index, -1, 0, 0, Dinorant, 0, 255, 1, 0, Option3, -1, 0, 0, Socket);
		::gObjInventoryCommit(lpObj->m_Index);

		CLog.LogAdd("[DinorantMix] [%s][%s] CBMix Success %d Money : %d-%d, CharmRate : %d", lpObj->AccountID, lpObj->Name, lpObj->ChaosSuccessRate, lpObj->Money, nChaosNeedMoney, iCharmOfLuckCount);

		return TRUE;
	}
	else
	{
		gChaosBox.ChaosBoxInit(lpObj);
		GCUserChaosBoxSend(lpObj, 0);
		gSendProto.DataSend(lpObj->m_Index, (BYTE *)&pMsg, pMsg.h.size);

		CLog.LogAdd("[DinorantMix] [%s][%s] CBMix Fail %d Money : %d-%d, CharmRate : %d", lpObj->AccountID, lpObj->Name, lpObj->ChaosSuccessRate, lpObj->Money, nChaosNeedMoney, iCharmOfLuckCount);

		lpObj->ChaosLock = FALSE;
		return FALSE;
	}
}
// Stats Fruits Mix
BOOL CircleChaosMix(LPOBJ lpObj)
{
	lpObj->ChaosLock = TRUE;

	int CreatureGemCount = 0;
	int ChoasGemCount = 0;
	int iCharmOfLuckCount = 0;

	for ( int n=0;n<CHAOS_BOX_SIZE;n++)
	{
		if ( lpObj->pChaosBox[n].IsItem() == TRUE )
		{
			if ( lpObj->pChaosBox[n].m_Type == ITEMGET(14,22) )	// Jewel of Creation
			{
				CreatureGemCount++;
			}
			else if ( lpObj->pChaosBox[n].m_Type == ITEMGET(12,15) ) // Jewel of Chaos
			{
				ChoasGemCount++;
			}
			else if ( lpObj->pChaosBox[n].m_Type == ITEMGET(14,53) ) // Charm Of Luck
			{
				iCharmOfLuckCount += lpObj->pChaosBox[n].m_Durability;
			}
		}
	}

	if ( CreatureGemCount != 1 || ChoasGemCount != 1 )
	{
		return FALSE;
	}

	PMSG_CHAOSMIXRESULT pMsg;

	C1HeadSet((LPBYTE)&pMsg.h, 0x86, sizeof(PMSG_CHAOSMIXRESULT));
	pMsg.Result = CB_ERROR;

	if ( iCharmOfLuckCount > 10 )
	{
		lpObj->ChaosLock = FALSE;
		pMsg.Result = 0xF0;
		gSendProto.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);

		return 1;
	}

	if ( lpObj->Level < 10 )
	{
		pMsg.Result = CB_LOW_LEVEL_USER;

		gSendProto.DataSend(lpObj->m_Index, (BYTE *)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		return TRUE;
	}

	int nChaosNeedMoney = 3000000;
	int iChaosTaxMoney = (int)((__int64)nChaosNeedMoney * (__int64)g_CastleSiegeSync.GetTaxRateChaos(lpObj->m_Index) / (__int64)100);

	if ( iChaosTaxMoney < 0 )
	{
		iChaosTaxMoney = 0;
	}

	nChaosNeedMoney += iChaosTaxMoney;

	if ( nChaosNeedMoney < 0 )
	{
		nChaosNeedMoney = 0;
	}

	if ( lpObj->Money < nChaosNeedMoney )
	{
		lpObj->ChaosLock = FALSE;
		pMsg.Result = CB_NOT_ENOUGH_ZEN;
		
		gSendProto.DataSend(lpObj->m_Index, (BYTE *)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		return TRUE;
	}

	LogChaosItem(lpObj, "CircleMix");
	CLog.LogAdd("[CircleMix] Chaos Mix Start");
	lpObj->ChaosSuccessRate = 90;	// Succes Rate for Fruit
	lpObj->ChaosSuccessRate += iCharmOfLuckCount;
	lpObj->Money -= nChaosNeedMoney;

	g_CastleSiegeSync.AddTributeMoney(iChaosTaxMoney);
	GCMoneySend(lpObj->m_Index, lpObj->Money);

	if ( (rand()%100 ) < lpObj->ChaosSuccessRate )
	{
		int Fruit = ITEMGET(13,15);	// Fruit
		int FruitType;
		int RandonValue = rand() % 100;

		if ( RandonValue < 30 )
		{
			FruitType = 0;
		}
		else if ( RandonValue < 55 )
		{
			FruitType = 1;
		}
		else if ( RandonValue < 75 )
		{
			FruitType = 2;
		}
		else if ( RandonValue < 95 )
		{
			FruitType = 3;
		}
		else
		{
			FruitType = 4;
		}
		BYTE Socket[5];
		Socket[0] = 0xFF;
		Socket[1] = 0xFF;
		Socket[2] = 0xFF;
		Socket[3] = 0xFF;
		Socket[4] = 0xFF;

		::ItemSerialCreateSend(lpObj->m_Index, -1, 0, 0, Fruit, FruitType, 255, 1, 0, 0, -1, 0, 0, Socket);
		::gObjInventoryCommit(lpObj->m_Index);

		CLog.LogAdd("[CircleMix] [%s][%s] CBMix Success %d Money : %d-%d, CharmRate : %d",
			lpObj->AccountID, lpObj->Name, lpObj->ChaosSuccessRate, lpObj->Money, nChaosNeedMoney, iCharmOfLuckCount);

		return TRUE;
	}
	else
	{
		gChaosBox.ChaosBoxInit(lpObj);
		GCUserChaosBoxSend(lpObj, 0);

		gSendProto.DataSend(lpObj->m_Index, (BYTE *)&pMsg, pMsg.h.size);
		CLog.LogAdd("[CircleMix] [%s][%s] CBMix Fail %d Money : %d-%d, CharmRate : %d",
			lpObj->AccountID, lpObj->Name, lpObj->ChaosSuccessRate, lpObj->Money, nChaosNeedMoney, iCharmOfLuckCount);
		lpObj->ChaosLock = FALSE;

		return FALSE;
	}
}

BOOL WingChaosMix(LPOBJ lpObj)
{
	lpObj->ChaosLock = TRUE;

	int WingCount = 0;
	int ChoasGemCount = 0;
	int LokesFeathersCount = 0;
	int WingIndex = -1;
	int iChaosMoney = 0;
	int iWingChaosMoney = 0;
	int iSleeveOfLord = 0;
	int iCharmOfLuckCount = 0;

	for ( int n=0;n<CHAOS_BOX_SIZE;n++)
	{
		if ( lpObj->pChaosBox[n].IsItem() == TRUE )
		{
			if ( (lpObj->pChaosBox[n].m_Type >= ITEMGET(12,3) && lpObj->pChaosBox[n].m_Type <= ITEMGET(12,6)) || lpObj->pChaosBox[n].m_Type == ITEMGET(14,30) )	
			{
				lpObj->ChaosLock = FALSE;

				return FALSE;
			}
			if ( lpObj->pChaosBox[n].m_Type >= ITEMGET(12,0) && lpObj->pChaosBox[n].m_Type <= ITEMGET(12,2) || lpObj->pChaosBox[n].m_Type == ITEMGET(12,41) ) 
			{
				WingCount++;
				WingIndex = n;
				iWingChaosMoney = lpObj->pChaosBox[n].m_BuyMoney;
			}
			else if ( lpObj->pChaosBox[n].m_Type == ITEMGET(12,15) ) // Chaos
			{
				ChoasGemCount++;
			}
			else if ( lpObj->pChaosBox[n].m_Type == ITEMGET(13,14) ) // Feather
			{
				if ( lpObj->pChaosBox[n].m_Level == 0 ) // Feather
				{
					LokesFeathersCount++;
				}
				else // Crest of Monarch
				{
					iSleeveOfLord ++;
				}
			}
			else if ( lpObj->pChaosBox[n].m_Type == ITEMGET(14,53) ) // Charm Of Luck
			{
				iCharmOfLuckCount += lpObj->pChaosBox[n].m_Durability;
			}
			else if ( lpObj->pChaosBox[n].IsExtItem() != FALSE )
			{
				if ( lpObj->pChaosBox[n].m_Level >= 4 )
				{
					iChaosMoney += lpObj->pChaosBox[n].m_BuyMoney;
				}
			}
		}
	}

	PMSG_CHAOSMIXRESULT pMsg;

	C1HeadSet((LPBYTE)&pMsg.h, 0x86, sizeof(PMSG_CHAOSMIXRESULT));
	pMsg.Result = CB_ERROR;

	if ( iSleeveOfLord == 1 )
	{
		if ( WingCount != 1 || ChoasGemCount != 1 || LokesFeathersCount != 0 )
		{
			lpObj->ChaosLock = FALSE;
			pMsg.Result = CB_INCORRECT_MIX_ITEMS;
			gSendProto.DataSend(lpObj->m_Index, (BYTE *)&pMsg, pMsg.h.size);
			return FALSE;
		}
	}
	else if ( LokesFeathersCount == 1 )
	{
		if ( WingCount != 1 || ChoasGemCount != 1 || iSleeveOfLord != 0 )
		{
			pMsg.Result = CB_INCORRECT_MIX_ITEMS;
			gSendProto.DataSend(lpObj->m_Index, (BYTE *)&pMsg, pMsg.h.size);
			lpObj->ChaosLock = FALSE;

			return FALSE;
		}
	}
	else
	{
		lpObj->ChaosLock = FALSE;
		pMsg.Result = CB_INCORRECT_MIX_ITEMS;
		gSendProto.DataSend(lpObj->m_Index, (BYTE *)&pMsg, pMsg.h.size);

		return FALSE;
	}

	if ( iCharmOfLuckCount > 10 )
	{
		lpObj->ChaosLock = FALSE;
		pMsg.Result = 0xF0;
		gSendProto.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
	}

	int nChaosNeedMoney = 5000000;
	int iChaosTaxMoney = (int)((__int64)nChaosNeedMoney * (__int64)g_CastleSiegeSync.GetTaxRateChaos(lpObj->m_Index) / (__int64)100);

	if ( iChaosTaxMoney < 0 )
	{
		iChaosTaxMoney = 0;
	}

	nChaosNeedMoney += iChaosTaxMoney;

	if ( nChaosNeedMoney < 0 )
	{
		nChaosNeedMoney = 0;
	}

	if ( lpObj->Money < nChaosNeedMoney )
	{
		pMsg.Result = CB_NOT_ENOUGH_ZEN;
		
		gSendProto.DataSend(lpObj->m_Index, (BYTE *)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		return TRUE;
	}

	CItem * pWing = &lpObj->pChaosBox[WingIndex];
	lpObj->ChaosSuccessRate = (DWORD)((DWORD)iWingChaosMoney / (DWORD)4000000);
	lpObj->ChaosSuccessRate += iChaosMoney / 40000;

	if ( lpObj->ChaosSuccessRate == 0 )
	{
		pMsg.Result = CB_INCORRECT_MIX_ITEMS;
		gSendProto.DataSend(lpObj->m_Index, (BYTE *)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		return FALSE;
	}

	LogChaosItem(lpObj, "WingMix,2");
	CLog.LogAdd("[WingMix,2] Chaos Mix Start");

	if ( iSleeveOfLord == 1 )
	{
		if ( lpObj->ChaosSuccessRate > 90)
		{
			lpObj->ChaosSuccessRate = 90;
		}
	}
	else if ( LokesFeathersCount == 1 )
	{
		if ( lpObj->ChaosSuccessRate > 100 )
		{
			lpObj->ChaosSuccessRate =  100;
		}
	}
	else if ( lpObj->ChaosSuccessRate > 100 )
	{
		lpObj->ChaosSuccessRate = 100;
	}

	lpObj->ChaosSuccessRate += iCharmOfLuckCount;
	lpObj->Money  -= nChaosNeedMoney;
	g_CastleSiegeSync.AddTributeMoney(iChaosTaxMoney);
	GCMoneySend(lpObj->m_Index, lpObj->Money);

	if ( (rand()%100) < lpObj->ChaosSuccessRate )
	{
		int iWingLevel = 0;
		int iItemType;
		int iItemSubType;

		int WingIndex = 0;

		if ( iSleeveOfLord != 0 )
		{
			if ( rand()%10 < 5)
			{
				iItemType = 13;
				iItemSubType = 30;
			}
			else
			{
				iItemType = 12;
				iItemSubType = 49;
			}
		}
		else
		{
			WingIndex = ITEMGET(12,3)+(rand()%5);

		    if(WingIndex == ITEMGET(12,7))
			{
			    WingIndex = ITEMGET(12,42);
			}
		}

		int iWingNum = ITEMGET(iItemType, iItemSubType);
		int iOption1 = 0;
		int iOption2 = 0;
		int iOption3 = 0;

		if ( (rand()%5) == 0 )
		{
			iOption1 = 1;
		}

		int iRandomValue = rand() % 100;
		int iRandomValue2 = rand() % 3;

		switch ( iRandomValue2 )
		{
			case 0:
				if ( iRandomValue < 4 )
				{
					iOption2 = 3;	// +12
				}
				break;

			case 1:
				if ( iRandomValue < 10 )
				{
					iOption2 = 2;	// +8
				}
				break;

			case 2:
				if ( iRandomValue < 20 )
				{
					iOption2 = 1;	// +4;
				}
		}

		if ( iSleeveOfLord != FALSE )
		{
			if ( (rand()%5) == 0 )
			{
				iOption3 = 1 << (rand()%4);
			}
				iOption3 |= 0x20;
		}
		else
		{
			if ( (rand()%5) == 0 )
			{
				iOption3 = 1 << (rand()%3);
			}

			if ( (rand()%2) != 0 )
			{
				iOption3 |= 0x20;
			}
		
		}

		BYTE Socket[5];
		Socket[0] = 0xFF;
		Socket[1] = 0xFF;
		Socket[2] = 0xFF;
		Socket[3] = 0xFF;
		Socket[4] = 0xFF;

		::ItemSerialCreateSend(lpObj->m_Index, -1, 0, 0, iWingNum, iWingLevel, 0, 0, iOption1, iOption2, -1, iOption3, 0, Socket);
		::gObjInventoryCommit(lpObj->m_Index);
		::CLog.LogAdd("[WingMix,2] [%s][%s] CBMix Success %d Money : %d-%d, CharmRate : %d", lpObj->AccountID, lpObj->Name, lpObj->ChaosSuccessRate, lpObj->Money, nChaosNeedMoney, iCharmOfLuckCount);
		return TRUE;
	}
	else
	{
		for ( int n=0;n<CHAOS_BOX_SIZE;n++)
		{
			lpObj->pChaosBox[n].Clear();
		}

		GCUserChaosBoxSend(lpObj, 0);
		gSendProto.DataSend(lpObj->m_Index, (BYTE *)&pMsg, pMsg.h.size);
		::CLog.LogAdd("[WingMix,2] [%s][%s] CBMix Fail %d Money : %d-%d, CharmRate : %d", lpObj->AccountID, lpObj->Name, lpObj->ChaosSuccessRate, lpObj->Money, nChaosNeedMoney, iCharmOfLuckCount);
		
		lpObj->ChaosLock = FALSE;

		return FALSE;
	}
}

void DefaultChaosMix(LPOBJ lpObj)
{
	PMSG_CHAOSMIXRESULT pMsg;

	C1HeadSet((LPBYTE)&pMsg.h, 0x86, sizeof(PMSG_CHAOSMIXRESULT));
	pMsg.Result = CB_ERROR;
	int aIndex = lpObj->m_Index;
	BOOL fail = TRUE;
	int MixResult2;
	lpObj->ChaosLock = TRUE;

	if ( ChaosBoxMix(lpObj, MixResult2) == 0 )
	{
		pMsg.Result = CB_INCORRECT_MIX_ITEMS;
		gSendProto.DataSend(aIndex, (BYTE *)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		return;
	}

	int iChaosTaxMoney = (int)((__int64)lpObj->ChaosMoney * (__int64)g_CastleSiegeSync.GetTaxRateChaos(lpObj->m_Index) / (__int64)100);

	if ( iChaosTaxMoney < 0 )
	{
		iChaosTaxMoney = 0;
	}

	lpObj->ChaosMoney += iChaosTaxMoney;

	if ( lpObj->ChaosMoney < 0 )
	{
		lpObj->ChaosMoney = 0;
	}

	if ( lpObj->Money < lpObj->ChaosMoney )
	{
		pMsg.Result = CB_NOT_ENOUGH_ZEN;
		
		gSendProto.DataSend(aIndex, (BYTE *)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;


	}
	else 
	{
		if ( lpObj->ChaosSuccessRate > 0 )
		{
			if ( lpObj->ChaosSuccessRate >= 100 )
			{
				lpObj->ChaosSuccessRate = 100;
			}

			if ( (rand()%100) <= (lpObj->ChaosSuccessRate-1) ) 
			{
				fail = FALSE;
				pMsg.Result = CB_SUCCESS;

				int Level = rand()%5;
				int Option1 = 0;
				int Option2 = 0;
				int Option3 = 0;

				if ( (rand()%100)< (lpObj->ChaosSuccessRate/5 + 6) )
				{
					Option1 = 1;
				}

				if ( (rand()%100)< (lpObj->ChaosSuccessRate/5 + 4) )
				{
					Option2 = 1;
				}

				int OpProb;
				int OpType = rand()%3;
				OpProb = rand()%100;

				switch ( OpType )
				{
					case 0:
						if ( OpProb < (lpObj->ChaosSuccessRate / 5 + 4) ) 
						{
							Option3 = 3;
						}
						break;

					case 1:
						if ( OpProb < (lpObj->ChaosSuccessRate / 5 + 8) ) 
						{
							Option3 = 2;
						}
						break;

					case 2:
						if ( OpProb < (lpObj->ChaosSuccessRate / 5 + 12) ) 
						{
							Option3 = 1;
						}
						break;
				}

				if ( MixResult2 == TRUE )
				{
					int WingIndex = ITEMGET(12,0) + (rand()%4);

					if(WingIndex == ITEMGET(12,3))
					{
						WingIndex = ITEMGET(12,41);
					}
					BYTE Socket[5];
					Socket[0] = 0xFF;
					Socket[1] = 0xFF;
					Socket[2] = 0xFF;
					Socket[3] = 0xFF;
					Socket[4] = 0xFF;

					::ItemSerialCreateSend(lpObj->m_Index, -1, 0, 0, WingIndex, 0, 255, Option1, Option2, Option3, -1, 0, 0, Socket);
				}
				else
				{
					int ChaosItemType = rand()%3;
					int ChaosItemNum = 0;

					if ( ChaosItemType == 0 )	// Chaos Dragon Axe
					{
						ChaosItemNum = ITEMGET(2,6);
					}
					else if ( ChaosItemType == 1 )	// Chaos Nature Bow
					{
						ChaosItemNum = ITEMGET(4,6);
					}
					else if ( ChaosItemType == 2 )	// Chaos Lighting Staff
					{
						ChaosItemNum = ITEMGET(5,7);
					}

					BYTE Socket[5];
					Socket[0] = 0xFF;
					Socket[1] = 0xFF;
					Socket[2] = 0xFF;
					Socket[3] = 0xFF;
					Socket[4] = 0xFF;

					::ItemSerialCreateSend(lpObj->m_Index, -1, 0, 0, ChaosItemNum, Level, 255, Option1, Option2, Option3, -1, 0, 0, Socket);
				}
			}
		}


		lpObj->Money -= lpObj->ChaosMoney;
		g_CastleSiegeSync.AddTributeMoney(iChaosTaxMoney);
		GCMoneySend(lpObj->m_Index, lpObj->Money);

		if ( fail == TRUE )
		{
			ChaosBoxItemDown(lpObj);
			GCUserChaosBoxSend(lpObj, 0);
			gSendProto.DataSend(aIndex, (BYTE*)&pMsg, pMsg.h.size);
			CLog.LogAdd("[%s][%s] CBMix Fail %d Money : %d-%d",
				lpObj->AccountID, lpObj->Name, lpObj->ChaosSuccessRate,
				lpObj->Money, lpObj->ChaosMoney);
			lpObj->ChaosLock = FALSE;
		}
		else
		{
			CLog.LogAdd("[%s][%s] CBMix Success Rate:%d Money : %d",
				lpObj->AccountID, lpObj->Name, lpObj->ChaosSuccessRate,
				lpObj->ChaosMoney);
		}

		if ( gChaosEvent != FALSE )
		{
			CBUPS_ItemRequest(lpObj->m_Index, lpObj->AccountID, lpObj->Name);
		}
	}

	::gObjInventoryCommit(lpObj->m_Index);
}

void DevilSquareItemChaosMix(LPOBJ lpObj)
{
	lpObj->ChaosLock = TRUE;

	if ( g_DevilSquare.Enabled == 1 )
	{
		PMSG_CHAOSMIXRESULT pMsg;
		C1HeadSet((LPBYTE)&pMsg.h, 0x86, sizeof(PMSG_CHAOSMIXRESULT));
		pMsg.Result = CB_ERROR;
		int aIndex = lpObj->m_Index;
		int eventitemcount;
		int itemlevel;
		int Ret = ::CheckDevilSquareItem(lpObj, eventitemcount, itemlevel);

		if ( Ret != FALSE )
		{
			if ( Ret == 3 )
			{
				pMsg.Result = CB_INCORRECT_MIX_ITEMS;
				gSendProto.DataSend(aIndex, (BYTE*)&pMsg, pMsg.h.size);
			CLog.LogAdd("[DevilSquare] DiffLevel Devil's Key or Eyes [%d]", eventitemcount);
				lpObj->ChaosLock = FALSE;

				return;
			}
			
			if ( Ret == 2 )
			{
				pMsg.Result = CB_INCORRECT_MIX_ITEMS;
				gSendProto.DataSend(aIndex, (BYTE*)&pMsg, pMsg.h.size);
				CLog.LogAdd("[DevilSquare] Not Found Chaos Gem [%d]", eventitemcount);
				lpObj->ChaosLock = FALSE;
				
				return;
			}

			if ( Ret == 4 )
			{
				pMsg.Result = 0xF0;
				gSendProto.DataSend(aIndex, (BYTE*)&pMsg, pMsg.h.size);
				CLog.LogAdd("[DevilSquare] Charm Of Luck Over 10% [%d]", eventitemcount);
				lpObj->ChaosLock = FALSE;
				
				return;
			}

			
			if ( eventitemcount > 2 )
			{
				pMsg.Result = CB_TOO_MANY_ITEMS;
				gSendProto.DataSend(aIndex, (BYTE*)&pMsg, pMsg.h.size);
				CLog.LogAdd("[DevilSquare] Too many Devil's Key or Eyes [%d]", eventitemcount);
				lpObj->ChaosLock = FALSE;

				return;
			}

			if ( lpObj->Level < 10 )
			{
				pMsg.Result = CB_LOW_LEVEL_USER;
				gSendProto.DataSend(aIndex, (BYTE*)&pMsg, pMsg.h.size);
				lpObj->ChaosLock = FALSE;

				return;
			}

			::DevilSquareEventChaosMix(lpObj, Ret, itemlevel);
			return;
		}
		
		if ( eventitemcount > 1 )
		{
			pMsg.Result = CB_LACKING_MIX_ITEMS;
			gSendProto.DataSend(aIndex, (BYTE*)&pMsg, pMsg.h.size);
			lpObj->ChaosLock = FALSE;

			return;
		}
	}

	lpObj->ChaosLock = FALSE;
}

void BloodCastleItemChaosMix(LPOBJ lpObj)
{
	lpObj->ChaosLock = TRUE;
	
	PMSG_CHAOSMIXRESULT pMsg;
	C1HeadSet((LPBYTE)&pMsg.h, 0x86, sizeof(PMSG_CHAOSMIXRESULT));
	pMsg.Result = CB_ERROR;

	int aIndex = lpObj->m_Index;

	int iRET_VAL = g_BloodCastle.CheckChoasMixItem(aIndex);

	if ( gUser.BloodCastleMapRange((iRET_VAL+(MAP_INDEX_BLOODCASTLE1-1))) == true )
	{
		if ( g_BloodCastle.BloodCastleChaosMix(aIndex, iRET_VAL) == false )
		{
			lpObj->ChaosLock = FALSE;
		}

		return;
	}

	switch ( iRET_VAL )
	{
		case 9:
			pMsg.Result = CB_NO_BC_CORRECT_ITEMS	;
			gSendProto.DataSend(aIndex, (UCHAR*)&pMsg, pMsg.h.size);
			lpObj->m_Index;
			lpObj->ChaosLock = FALSE;
			break;

		case 10:
			pMsg.Result = CB_NO_BC_CORRECT_ITEMS	;
			gSendProto.DataSend(aIndex, (UCHAR*)&pMsg, pMsg.h.size);
			lpObj->m_Index;
			lpObj->ChaosLock = FALSE;
			break;

		case 11:
			pMsg.Result = CB_NO_BC_CORRECT_ITEMS	;
			gSendProto.DataSend(aIndex, (UCHAR*)&pMsg, pMsg.h.size);
			lpObj->m_Index;
			lpObj->ChaosLock = FALSE;
			break;

		case 12:
			pMsg.Result = CB_INVALID_ITEM_LEVEL	;
			gSendProto.DataSend(aIndex, (UCHAR*)&pMsg, pMsg.h.size);
			lpObj->m_Index;
			lpObj->ChaosLock = FALSE;
			break;

		case 13:
			pMsg.Result = CB_BC_NOT_ENOUGH_ZEN	;
			gSendProto.DataSend(aIndex, (UCHAR*)&pMsg, pMsg.h.size);
			lpObj->m_Index;
			lpObj->ChaosLock = FALSE;
			break;

		case 14:
			pMsg.Result = CB_USER_CLASS_LOW_LEVEL	;
			gSendProto.DataSend(aIndex, (UCHAR*)&pMsg, pMsg.h.size);
			lpObj->m_Index;
			lpObj->ChaosLock = FALSE;
			break;
		case 15:
			pMsg.Result = 0xF0;
			gSendProto.DataSend(aIndex, (BYTE*)&pMsg, pMsg.h.size);
			lpObj->ChaosLock = FALSE;
			break;
		default:
			lpObj->ChaosLock = FALSE;
	}

}

void SetItemChaosMix(LPOBJ lpObj)
{
	lpObj->ChaosLock = TRUE;

	int ChoasGemCount = 0;
	int BlessGemCount = 0;
	int SoulGemCount = 0;
	int MetalOfAncientCount = 0;
	int MixItemCount = 0;
	int MixSetItemIndex = 0;
	int iMixItemChaosMoney = 0;
	int iChaosMoney = 0;
	int iMetalOfAncientItemLevel = -1;

	PMSG_CHAOSMIXRESULT pMsg;

	C1HeadSet((LPBYTE)&pMsg.h, 0x86, sizeof(PMSG_CHAOSMIXRESULT));
	pMsg.Result = CB_ERROR;


	for ( int n=0;n<CHAOS_BOX_SIZE;n++)
	{
		if ( lpObj->pChaosBox[n].IsItem() == TRUE )
		{
			if ( lpObj->pChaosBox[n].m_Type == ITEMGET(14,13) )
			{
				BlessGemCount++;

				if ( BlessGemCount > 3 )
				{
					iChaosMoney += lpObj->pChaosBox[n].m_BuyMoney;
				}
			}

			if ( lpObj->pChaosBox[n].m_Type == ITEMGET(14,14) )
			{
				SoulGemCount++;

				if ( SoulGemCount > 3 )
				{
					iChaosMoney += lpObj->pChaosBox[n].m_BuyMoney;
				}
			}

			if ( lpObj->pChaosBox[n].m_Type == ITEMGET(14,16) )
			{
				iChaosMoney += lpObj->pChaosBox[n].m_BuyMoney;
			}
			else if ( lpObj->pChaosBox[n].m_Type == ITEMGET(12,15) )
			{
				ChoasGemCount++;
			}
			else if ( lpObj->pChaosBox[n].m_Level >= 6 && lpObj->pChaosBox[n].IsExtItem() != FALSE)
			{
				if ( gSetItemOption.IsSetItem(lpObj->pChaosBox[n].m_Type) != FALSE )
				{
					MixItemCount++;
					MixSetItemIndex = n;
					iMixItemChaosMoney += lpObj->pChaosBox[n].m_BuyMoney;
				}
			}
			else if ( ((lpObj->pChaosBox[n].m_Type >= ITEMGET(13,8) && lpObj->pChaosBox[n].m_Type < ITEMGET(13,14)) || (lpObj->pChaosBox[n].m_Type >= ITEMGET(13,20) && lpObj->pChaosBox[n].m_Type < ITEMGET(13,28) ) ) && ( lpObj->pChaosBox[n].m_Option3 >= 2  && lpObj->pChaosBox[n].m_Level >= 3 ) )	
			{
				if ( gSetItemOption.IsSetItem(lpObj->pChaosBox[n].m_Type ) != FALSE )
				{
					MixItemCount++;
					MixSetItemIndex = n;
					iMixItemChaosMoney += lpObj->pChaosBox[n].m_BuyMoney;
				}
			}
			else if ( lpObj->pChaosBox[n].m_Type == ITEMGET(14,27) ) // Ancien Metal, Apply Deathway Fix here
			{
				MetalOfAncientCount++;
				iMetalOfAncientItemLevel = lpObj->pChaosBox[n].m_Level;
			}
			else
			{
				gSendProto.DataSend(lpObj->m_Index, (BYTE*)&pMsg, pMsg.h.size);
				lpObj->ChaosLock = FALSE;
			}
		}
	}


	if ( ChoasGemCount < 1 || BlessGemCount < 3 || SoulGemCount < 3 || MetalOfAncientCount != 1 || MixItemCount != 1 )
	{
		gSendProto.DataSend(lpObj->m_Index, (BYTE*)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;
		return;
	}

	LogChaosItem(lpObj, "SetItemMix");
	CLog.LogAdd("[SetItemMix] Chaos Mix Start");

	if ( gSetItemOption.CheckMixContition(lpObj->pChaosBox[MixSetItemIndex].m_Type, iMetalOfAncientItemLevel ) == FALSE )
	{
		gSendProto.DataSend(lpObj->m_Index, (BYTE*)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		return;
	}

	lpObj->ChaosSuccessRate = iMixItemChaosMoney / 40000;
	lpObj->ChaosSuccessRate = iChaosMoney / 400000;

	if ( lpObj->ChaosSuccessRate > 80 )
	{
		lpObj->ChaosSuccessRate = 80;
	}

	int nChaosNeedMoney = (lpObj->ChaosSuccessRate - 50 ) * 1000000;

	if ( lpObj->Money < nChaosNeedMoney )
	{
		pMsg.Result = CB_NOT_ENOUGH_ZEN;
		gSendProto.DataSend(lpObj->m_Index, (BYTE*)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		return;
	}

	int setoption = gSetItemOption.MakeSetOption(lpObj->pChaosBox[MixSetItemIndex].m_Type, iMetalOfAncientItemLevel);

	if ( setoption == 0 )
	{
		pMsg.Result = CB_ERROR;
		gSendProto.DataSend(lpObj->m_Index, (BYTE*)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		return;
	}

	lpObj->Money -= nChaosNeedMoney;
	GCMoneySend(lpObj->m_Index, lpObj->Money);

	if ( (rand()%100) < lpObj->ChaosSuccessRate )
	{
		lpObj->pChaosBox[MixSetItemIndex].m_SetOption = setoption;

		if ( (rand()%100 ) < 80 )
		{
			lpObj->pChaosBox[MixSetItemIndex].m_SetOption |= 4;
		}
		else
		{
			lpObj->pChaosBox[MixSetItemIndex].m_SetOption |= 8;
		}

		pMsg.Result = CB_SUCCESS;
		CItem Item;

		float Dur = ItemGetDurability(lpObj->pChaosBox[MixSetItemIndex].m_Type, lpObj->pChaosBox[MixSetItemIndex].m_Level,
			lpObj->pChaosBox[MixSetItemIndex].IsExtItem(), lpObj->pChaosBox[MixSetItemIndex].IsSetItem());

		Item.m_Level = lpObj->pChaosBox[MixSetItemIndex].m_Level;
		Item.m_Durability = Dur * lpObj->pChaosBox[MixSetItemIndex].m_Durability / lpObj->pChaosBox[MixSetItemIndex].m_BaseDurability;
		Item.m_JewelOfHarmonyOption = lpObj->pChaosBox[MixSetItemIndex].m_JewelOfHarmonyOption;

		Item.Convert(lpObj->pChaosBox[MixSetItemIndex].m_Type, lpObj->pChaosBox[MixSetItemIndex].m_Option1, lpObj->pChaosBox[MixSetItemIndex].m_Option2,
			lpObj->pChaosBox[MixSetItemIndex].m_Option3, lpObj->pChaosBox[MixSetItemIndex].m_NewOption, 
			lpObj->pChaosBox[MixSetItemIndex].m_SetOption, lpObj->pChaosBox[MixSetItemIndex].m_ItemOptionEx,lpObj->pChaosBox[MixSetItemIndex].m_ItemSocket);

		Item.m_Number = lpObj->pChaosBox[MixSetItemIndex].m_Number;

		ItemByteConvert(pMsg.ItemInfo, Item);
		gChaosBox.ChaosBoxInit(lpObj);
		::gObjChaosBoxInsertItemPos(lpObj->m_Index, Item, 0, -1);
		gObjChaosItemSet(lpObj->m_Index, 0, 1);

		gSendProto.DataSend(lpObj->m_Index, (BYTE *)&pMsg, pMsg.h.size);

		CLog.LogAdd("[SetItemMix] [%s][%s] CBMix Success %d Money : %d-%d (SetName:%s)", lpObj->AccountID, lpObj->Name, lpObj->ChaosSuccessRate,
			lpObj->Money, lpObj->ChaosMoney, gSetItemOption.GetSetOptionName(lpObj->pChaosBox[MixSetItemIndex].m_Type, setoption));
	}
	else
	{
		gChaosBox.ChaosBoxInit(lpObj);
		GCUserChaosBoxSend(lpObj, 0);

		gSendProto.DataSend(lpObj->m_Index, (BYTE *)&pMsg, pMsg.h.size);

		CLog.LogAdd("[SetItemMix] [%s][%s] CBMix Fail %d Money : %d-%d",
			lpObj->AccountID, lpObj->Name, lpObj->ChaosSuccessRate, lpObj->Money,
			nChaosNeedMoney);

		lpObj->ChaosLock = FALSE;
	}
}

void DarkHorseChaosMix(LPOBJ lpObj)
{
	lpObj->ChaosLock = TRUE;
	int ChoasGemCount = 0;
	int BlessGemCount = 0;
	int SoulGemCount = 0;
	int CreatureGemCount = 0;
	int SoulOfDarkHorse = 0;
	int iChaosMoney = 0;

	PMSG_CHAOSMIXRESULT pMsg;

	C1HeadSet((LPBYTE)&pMsg.h, 0x86, sizeof(PMSG_CHAOSMIXRESULT));
	pMsg.Result = CB_ERROR;
	int iCharmOfLuckCount = 0;
	int iInvalidItemCount = 0;

	for ( int n=0;n<CHAOS_BOX_SIZE;n++)
	{
		if ( lpObj->pChaosBox[n].IsItem() == TRUE )
		{
			if ( lpObj->pChaosBox[n].m_Type == ITEMGET(14,13) ) // Jewel of Bless
			{
				BlessGemCount++;
			}
			else if ( lpObj->pChaosBox[n].m_Type == ITEMGET(14,14) ) // Jewel of Soul
			{
				SoulGemCount++;
			}
			else if ( lpObj->pChaosBox[n].m_Type == ITEMGET(14,22) ) // Jewel of Creation
			{
				CreatureGemCount++;
			}
			else if ( lpObj->pChaosBox[n].m_Type == ITEMGET(12,15) ) // Jewel of Chaos
			{
				ChoasGemCount++;
			}
			else if ( lpObj->pChaosBox[n].m_Type == ITEMGET(13,31) && lpObj->pChaosBox[n].m_Level == 0 ) // Dark Horse Spirit
			{
				SoulOfDarkHorse++;
			}
			else if ( lpObj->pChaosBox[n].m_Type == ITEMGET(14,53) ) // Charm Of Luck
			{
				iCharmOfLuckCount += lpObj->pChaosBox[n].m_Durability;
			}
			else
			{
				iInvalidItemCount++;
			}
			

		}
	}

	if ( ChoasGemCount != 1 || BlessGemCount != 5 || SoulGemCount != 5 || CreatureGemCount != 1 || SoulOfDarkHorse != 1 || iInvalidItemCount > 0 )
	{
		gSendProto.DataSend(lpObj->m_Index, (UCHAR*)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		return;
	}

	if ( iCharmOfLuckCount > 10 )
	{
		pMsg.Result = 0xF0;
		gSendProto.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		return ;
	}

	LogChaosItem(lpObj, "DarkHorseMix");
	CLog.LogAdd("[DarkHorseMix] Chaos Mix Start");
	lpObj->ChaosSuccessRate = 60;
	lpObj->ChaosSuccessRate += iCharmOfLuckCount;

	int nChaosNeedMoney = 5000000;

	int iChaosTaxMoney = (int)((__int64)nChaosNeedMoney * (__int64)g_CastleSiegeSync.GetTaxRateChaos(lpObj->m_Index) / (__int64)100);

	if ( iChaosTaxMoney < 0 )
	{
		iChaosTaxMoney = 0;
	}

	nChaosNeedMoney += iChaosTaxMoney;

	if ( nChaosNeedMoney < 0 )
	{
		nChaosNeedMoney = 0;
	}

	if ( lpObj->Money < nChaosNeedMoney )
	{
		pMsg.Result = CB_NOT_ENOUGH_ZEN;
		
		gSendProto.DataSend(lpObj->m_Index, (BYTE *)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		return;
	}

	lpObj->Money -= nChaosNeedMoney;
	g_CastleSiegeSync.AddTributeMoney(iChaosTaxMoney);
	GCMoneySend(lpObj->m_Index, lpObj->Money);

	if ( (rand()%100) < lpObj->ChaosSuccessRate )
	{
		int Level = 0;
		int ItemType = 13;
		int ItemSubType = 4;
		int ItemNum = ITEMGET(ItemType, ItemSubType);
		int Option1 = 0;
		int Option2 = 0;
		int Option3 = 0;

		int Add = 0;

		if ( (rand()%100) < (lpObj->ChaosSuccessRate/5+6) )
		{
			Add = 1;
		}

		if ( (rand()%5) == 0 )
		{
			Option1 = 1;
		}

		int lc22 = rand()%100;	
		int lc23 = rand()%3;

		switch ( lc23 )
		{
			case 0:
				if ( lc22 <  4 )
				{
					Option2 = 3;
				}
				break;

			case 1:
				if ( lc22 <  10 )
				{
					Option2 = 2;
				}
				break;

			case 2:
				if ( lc22 <  20 )
				{
					Option2 = 1;
				}
				break;
		}

		::PetItemSerialCreateSend(lpObj->m_Index, -2, 0, 0, ItemNum, Level, 255,Add,  Option1, Option2, -1, 0, 0); 
		::gObjInventoryCommit(lpObj->m_Index);
		CLog.LogAdd("[DarkHorseMix] [%s][%s] CBMix Success %d Money : %d-%d, CharmRate : %d", lpObj->AccountID, lpObj->Name, lpObj->ChaosSuccessRate, lpObj->Money, nChaosNeedMoney, iCharmOfLuckCount);
	}
	else
	{
		gChaosBox.ChaosBoxInit(lpObj);
		GCUserChaosBoxSend(lpObj, 1);
		gSendProto.DataSend(lpObj->m_Index, (UCHAR*)&pMsg, pMsg.h.size);
		CLog.LogAdd("[DarkHorseMix] [%s][%s] CBMix Fail %d Money : %d-%d, CharmRate : %d", lpObj->AccountID, lpObj->Name, lpObj->ChaosSuccessRate, lpObj->Money, nChaosNeedMoney, iCharmOfLuckCount);
		lpObj->ChaosLock = FALSE;
	}
}

void DarkSpiritChaosMix(LPOBJ lpObj)
{
	lpObj->ChaosLock = TRUE;
	int ChoasGemCount = 0;
	int BlessGemCount = 0;
	int SoulGemCount = 0;
	int CreatureGemCount = 0;
	int SoulOfSpirit = 0;
	int iChaosMoney = 0;

	PMSG_CHAOSMIXRESULT pMsg;

	C1HeadSet((LPBYTE)&pMsg.h, 0x86, sizeof(PMSG_CHAOSMIXRESULT));
	pMsg.Result = CB_ERROR;
	int iCharmOfLuckCount = 0;
	int iInvalidItemCount = 0;

	for ( int n=0;n<CHAOS_BOX_SIZE;n++)
	{
		if ( lpObj->pChaosBox[n].IsItem() == TRUE )
		{
			if ( lpObj->pChaosBox[n].m_Type == ITEMGET(14,13) ) // Jewel of Bless
			{
				BlessGemCount++;
			}
			else if ( lpObj->pChaosBox[n].m_Type == ITEMGET(14,14) ) // Jewel of Soul
			{
				SoulGemCount++;
			}
			else if ( lpObj->pChaosBox[n].m_Type == ITEMGET(14,22) ) // Jewel of Creation
			{
				CreatureGemCount++;
			}
			else if ( lpObj->pChaosBox[n].m_Type == ITEMGET(12,15) ) // Jewel of Chaos
			{
				ChoasGemCount++;
			}
			else if ( lpObj->pChaosBox[n].m_Type == ITEMGET(13,31) && lpObj->pChaosBox[n].m_Level == 1 ) // Dark Raven Spirit
			{
				SoulOfSpirit++;
			}
			else if ( lpObj->pChaosBox[n].m_Type == ITEMGET(14,53) ) // Charm Of Luck
			{
				iCharmOfLuckCount += lpObj->pChaosBox[n].m_Durability;
			}
			else
			{
				iInvalidItemCount++;
			}
		}
	}

	if ( ChoasGemCount != 1 || BlessGemCount != 2 || SoulGemCount != 2 || CreatureGemCount != 1 || SoulOfSpirit != 1 || iInvalidItemCount > 0 )
	{
		gSendProto.DataSend(lpObj->m_Index, (UCHAR*)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		return;
	}

	if ( iCharmOfLuckCount > 10 )
	{
		pMsg.Result = 0xF0;
		gSendProto.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		return ;
	}

	LogChaosItem(lpObj, "DarkSpiritMix");
	CLog.LogAdd("[DarkSpiritMix] Chaos Mix Start");
	lpObj->ChaosSuccessRate = 60;
	lpObj->ChaosSuccessRate += iCharmOfLuckCount;

	int nChaosNeedMoney = 1000000;

	int iChaosTaxMoney = (int)((__int64)nChaosNeedMoney * (__int64)g_CastleSiegeSync.GetTaxRateChaos(lpObj->m_Index) / (__int64)100);

	if ( iChaosTaxMoney < 0 )
	{
		iChaosTaxMoney = 0;
	}

	nChaosNeedMoney += iChaosTaxMoney;

	if ( nChaosNeedMoney < 0 )
	{
		nChaosNeedMoney = 0;
	}

	if ( lpObj->Money < nChaosNeedMoney )
	{
		pMsg.Result = CB_NOT_ENOUGH_ZEN;
		
		gSendProto.DataSend(lpObj->m_Index, (BYTE *)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		return;
	}

	lpObj->Money -= nChaosNeedMoney;
	g_CastleSiegeSync.AddTributeMoney(iChaosTaxMoney);
	GCMoneySend(lpObj->m_Index, lpObj->Money);

	if ( (rand()%100) < lpObj->ChaosSuccessRate )
	{
		int Level = 0;
		int ItemType = 13;
		int ItemSubType = 5;
		int ItemNum = ITEMGET(ItemType, ItemSubType);
		int Option1 = 0;
		int Option2 = 0;
		int Option3 = 0;

		int Add = 0;

		if ( (rand()%100) < (lpObj->ChaosSuccessRate/5+6) )
		{
			Add = 1;
		}

		if ( (rand()%5) == 0 )
		{
			Option1 = 1;
		}

		int lc22 = rand()%100;	
		int lc23 = rand()%3;

		switch ( lc23 )
		{
			case 0:
				if ( lc22 <  4 )
				{
					Option2 = 3;
				}
				break;

			case 1:
				if ( lc22 <  10 )
				{
					Option2 = 2;
				}
				break;

			case 2:
				if ( lc22 <  20 )
				{
					Option2 = 1;
				}
				break;
		}

		::PetItemSerialCreateSend(lpObj->m_Index, -2, 0, 0, ItemNum, Level, 255, Add, Option1, Option2, -1, 0, 0); 
		::gObjInventoryCommit(lpObj->m_Index);
		CLog.LogAdd("[DarkSpiritMix] [%s][%s] CBMix Success %d Money : %d-%d, CharmRate : %d", lpObj->AccountID, lpObj->Name, lpObj->ChaosSuccessRate, lpObj->Money, nChaosNeedMoney, iCharmOfLuckCount);
	}
	else
	{
		gChaosBox.ChaosBoxInit(lpObj);
		GCUserChaosBoxSend(lpObj, 1);
		gSendProto.DataSend(lpObj->m_Index, (UCHAR*)&pMsg, pMsg.h.size);
		CLog.LogAdd("[DarkSpiritMix] [%s][%s] CBMix Fail %d Money : %d-%d, CharmRate : %d", lpObj->AccountID, lpObj->Name, lpObj->ChaosSuccessRate, lpObj->Money, nChaosNeedMoney, iCharmOfLuckCount);
		lpObj->ChaosLock = FALSE;
	}
}

void BlessPotionChaosMix(LPOBJ lpObj)
{
	lpObj->ChaosLock = TRUE;
	int iBlessGemCount = 0;
	int iChaosMoney = 0;
	int iInvalidItemCount = 0;

	PMSG_CHAOSMIXRESULT pMsg;

	C1HeadSet((LPBYTE)&pMsg.h, 0x86, sizeof(PMSG_CHAOSMIXRESULT));
	pMsg.Result = CB_ERROR;

	for ( int n=0;n<CHAOS_BOX_SIZE;n++)
	{
		if ( lpObj->pChaosBox[n].IsItem() == TRUE )
		{
			if ( lpObj->pChaosBox[n].m_Type == ITEMGET(14,13) )
			{
				iBlessGemCount++;
			}
			else
			{
				iInvalidItemCount++;
			}
		}
	}

	if ( iBlessGemCount == 0 )
		return;

	if ( iInvalidItemCount > 0 )
		return;

	if ( iBlessGemCount > 25 )
	{
		MsgOutput(lpObj->m_Index, lMsg.Get(MSGGET(6,201)));
		gSendProto.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		return;
	}

	LogChaosItem(lpObj, "BlessPotionMix");
	CLog.LogAdd("[BlessPotionMix] Chaos Mix Start");
	lpObj->ChaosSuccessRate = 100;

	int nChaosNeedMoney = 100000;

	int iChaosTaxMoney = (int)((__int64)nChaosNeedMoney * (__int64)g_CastleSiegeSync.GetTaxRateChaos(lpObj->m_Index) / (__int64)100);

	if ( iChaosTaxMoney < 0 )
	{
		iChaosTaxMoney = 0;
	}

	nChaosNeedMoney += iChaosTaxMoney;

	if ( nChaosNeedMoney < 0 )
	{
		nChaosNeedMoney = 0;
	}

	if ( lpObj->Money < nChaosNeedMoney )
	{
		pMsg.Result = CB_NOT_ENOUGH_ZEN;
		
		gSendProto.DataSend(lpObj->m_Index, (BYTE *)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		return;
	}

	lpObj->Money -= nChaosNeedMoney;
	g_CastleSiegeSync.AddTributeMoney(iChaosTaxMoney);
	GCMoneySend(lpObj->m_Index, lpObj->Money);

	BYTE Socket[5];
	Socket[0] = 0xFF;
	Socket[1] = 0xFF;
	Socket[2] = 0xFF;
	Socket[3] = 0xFF;
	Socket[4] = 0xFF;

	if ( (rand()%100) < lpObj->ChaosSuccessRate )
	{
		int ItemNum = ITEMGET(14,7);
		int Level = 0;
		int Dur = iBlessGemCount * 10;

		ItemSerialCreateSend(lpObj->m_Index, -1, 0, 0, ItemNum, Level, Dur, 0, 0, 0, -1, 0, 0, Socket);
		::gObjInventoryCommit(lpObj->m_Index);

		CLog.LogAdd("[BlessPotionMix] [%s][%s] CBMix Success %d Money : %d-%d",
			lpObj->AccountID, lpObj->Name, lpObj->ChaosSuccessRate, 
			lpObj->Money, nChaosNeedMoney);
	}
	else
	{
		gChaosBox.ChaosBoxInit(lpObj);
		::GCUserChaosBoxSend(lpObj, 1);
		gSendProto.DataSend(lpObj->m_Index, (UCHAR*)&pMsg, pMsg.h.size);

		CLog.LogAdd("[BlessPotionMix] [%s][%s] CBMix Fail %d Money : %d-%d",
			lpObj->AccountID, lpObj->Name, lpObj->ChaosSuccessRate, 
			lpObj->Money, nChaosNeedMoney);

		lpObj->ChaosLock = FALSE;
	}
}

void SoulPotionChaosMix(LPOBJ lpObj)
{
	lpObj->ChaosLock = TRUE;
	int iSoulGemCount = 0;
	int iChaosMoney = 0;
	int iInvalidItemCount = 0;

	PMSG_CHAOSMIXRESULT pMsg;

	C1HeadSet((LPBYTE)&pMsg.h, 0x86, sizeof(PMSG_CHAOSMIXRESULT));
	pMsg.Result = CB_ERROR;

	for ( int n=0;n<CHAOS_BOX_SIZE;n++)
	{
		if ( lpObj->pChaosBox[n].IsItem() == TRUE )
		{
			if ( lpObj->pChaosBox[n].m_Type == ITEMGET(14,14) )
			{
				iSoulGemCount++;
			}
			else
			{
				iInvalidItemCount++;
			}
		}
	}

	if ( iSoulGemCount == 0 )
		return;

	if ( iInvalidItemCount > 0 )
		return;

	if ( iSoulGemCount > 25 )
	{
		MsgOutput(lpObj->m_Index, lMsg.Get(MSGGET(6,201)));
		gSendProto.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		return;
	}

	LogChaosItem(lpObj, "SoulPotionMix");
	CLog.LogAdd("[SoulPotionMix] Chaos Mix Start");
	lpObj->ChaosSuccessRate = 100;

	int nChaosNeedMoney = 50000;

	int iChaosTaxMoney = (int)((__int64)nChaosNeedMoney * (__int64)g_CastleSiegeSync.GetTaxRateChaos(lpObj->m_Index) / (__int64)100);

	if ( iChaosTaxMoney < 0 )
	{
		iChaosTaxMoney = 0;
	}

	nChaosNeedMoney += iChaosTaxMoney;

	if ( nChaosNeedMoney < 0 )
	{
		nChaosNeedMoney = 0;
	}

	if ( lpObj->Money < nChaosNeedMoney )
	{
		pMsg.Result = CB_NOT_ENOUGH_ZEN;
		
		gSendProto.DataSend(lpObj->m_Index, (BYTE *)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		return;
	}

	lpObj->Money -= nChaosNeedMoney;
	g_CastleSiegeSync.AddTributeMoney(iChaosTaxMoney);
	GCMoneySend(lpObj->m_Index, lpObj->Money);

	BYTE Socket[5];
	Socket[0] = 0xFF;
	Socket[1] = 0xFF;
	Socket[2] = 0xFF;
	Socket[3] = 0xFF;
	Socket[4] = 0xFF;

	if ( (rand()%100) < lpObj->ChaosSuccessRate )
	{
		int ItemNum = ITEMGET(14,7);
		int Level = 1;
		int Dur = iSoulGemCount * 10;

		ItemSerialCreateSend(lpObj->m_Index, -1, 0, 0, ItemNum, Level, Dur, 0, 0, 0, -1, 0, 0, Socket);
		::gObjInventoryCommit(lpObj->m_Index);

		CLog.LogAdd("[BlessPotionMix] [%s][%s] CBMix Success %d Money : %d-%d",	// #error Change to SoulPotionMix
			lpObj->AccountID, lpObj->Name, lpObj->ChaosSuccessRate, 
			lpObj->Money, nChaosNeedMoney);
	}
	else
	{
		gChaosBox.ChaosBoxInit(lpObj);
		::GCUserChaosBoxSend(lpObj, 1);
		gSendProto.DataSend(lpObj->m_Index, (UCHAR*)&pMsg, pMsg.h.size);

		CLog.LogAdd("[SoulPotionMix] [%s][%s] CBMix Fail %d Money : %d-%d",
			lpObj->AccountID, lpObj->Name, lpObj->ChaosSuccessRate, 
			lpObj->Money, nChaosNeedMoney);

		lpObj->ChaosLock = FALSE;
	}
}

void LifeStoneChaosMix(LPOBJ lpObj)
{
	lpObj->ChaosLock = TRUE;
	int iChoasGemCount = 0;
	int iBlessGemCount = 0;
	int iSoulGemCount = 0;
	int iProtectionGemCount = 0;
	int iInvalidItemCount = 0;
	int iChaosMoney = 0;

	PMSG_CHAOSMIXRESULT pMsg;

	C1HeadSet((LPBYTE)&pMsg.h, 0x86, sizeof(PMSG_CHAOSMIXRESULT));
	pMsg.Result = CB_ERROR;


	for ( int n=0;n<CHAOS_BOX_SIZE;n++)
	{
		if ( lpObj->pChaosBox[n].IsItem() == TRUE )
		{
			if ( lpObj->pChaosBox[n].m_Type == ITEMGET(14,13) )
			{
				iBlessGemCount++;
			}
			else if ( lpObj->pChaosBox[n].m_Type == ITEMGET(14,14) )
			{
				iSoulGemCount++;
			}
			else if ( lpObj->pChaosBox[n].m_Type == ITEMGET(14,31) ) // Guardian Jewel
			{
				iProtectionGemCount++;
			}
			else if ( lpObj->pChaosBox[n].m_Type == ITEMGET(12,15) ) // Jewel of Chaos
			{
				iChoasGemCount++;
			}
			else 
			{
				iInvalidItemCount++;
			}
		}
	}

	if ( iChoasGemCount != 1 || iBlessGemCount != 5 || iSoulGemCount != 5 || iProtectionGemCount != 1 || iInvalidItemCount > 0 )
	{
		gSendProto.DataSend(lpObj->m_Index, (UCHAR*)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		return;
	}

	LogChaosItem(lpObj, "LifeStoneMix");
	CLog.LogAdd("[LifeStoneMix] Chaos Mix Start");
	lpObj->ChaosSuccessRate = 100;

	int nChaosNeedMoney = 5000000;

	int iChaosTaxMoney = (int)((__int64)nChaosNeedMoney * (__int64)g_CastleSiegeSync.GetTaxRateChaos(lpObj->m_Index) / (__int64)100);

	if ( iChaosTaxMoney < 0 )
	{
		iChaosTaxMoney = 0;
	}

	nChaosNeedMoney += iChaosTaxMoney;

	if ( nChaosNeedMoney < 0 )
	{
		nChaosNeedMoney = 0;
	}

	if ( lpObj->Money < nChaosNeedMoney )
	{
		pMsg.Result = CB_NOT_ENOUGH_ZEN;
		
		gSendProto.DataSend(lpObj->m_Index, (BYTE *)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		return;
	}

	lpObj->Money -= nChaosNeedMoney;
	g_CastleSiegeSync.AddTributeMoney(iChaosTaxMoney);
	GCMoneySend(lpObj->m_Index, lpObj->Money);

	BYTE Socket[5];
	Socket[0] = 0xFF;
	Socket[1] = 0xFF;
	Socket[2] = 0xFF;
	Socket[3] = 0xFF;
	Socket[4] = 0xFF;

	if ( (rand()%100) < lpObj->ChaosSuccessRate )
	{
		ItemSerialCreateSend(lpObj->m_Index, -1, 0, 0, ITEMGET(13,11) , 1, 0, 0, 0, 0, -1, 0, 0, Socket);
		::gObjInventoryCommit(lpObj->m_Index);
		CLog.LogAdd("[LifeStoneMix] [%s][%s] CBMix Success %d Money : %d-%d", lpObj->AccountID, lpObj->Name, lpObj->ChaosSuccessRate, lpObj->Money, nChaosNeedMoney);
	}
	else
	{
		gChaosBox.ChaosBoxInit(lpObj);
		::GCUserChaosBoxSend(lpObj, 1);
		gSendProto.DataSend(lpObj->m_Index, (UCHAR*)&pMsg, pMsg.h.size);
		CLog.LogAdd("[LifeStoneMix] [%s][%s] CBMix Fail %d Money : %d-%d", lpObj->AccountID, lpObj->Name, lpObj->ChaosSuccessRate, lpObj->Money, nChaosNeedMoney);
		lpObj->ChaosLock = FALSE;
	}
}

#if (GS_CASTLE)
void CastleSpecialItemMix(LPOBJ lpObj)
{
	lpObj->ChaosLock = TRUE;

	int iDefendGemCount = 0;
	int iBlessGemMixCount = 0;
	int iSoulGemMixCount = 0;
	int iInvalidItemCount = 0;
	int iChaosMoney = 0;

	PMSG_CHAOSMIXRESULT pMsg;

	C1HeadSet((LPBYTE)&pMsg.h, 0x86, sizeof(PMSG_CHAOSMIXRESULT));
	pMsg.Result = CB_ERROR;

	if( g_CastleSiegeSync.CheckCastleOwnerMember(lpObj->m_Index)  )
	{
		if( lpObj->GuildStatus != GUILD_MASTER )
		{
			CLog.LogAdd("[CastleSpecialMix] [%s][%s] Item Mix Failed - No Auth", lpObj->AccountID, lpObj->Name);
			gSendProto.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
			lpObj->ChaosLock = FALSE;
			return;
		}
	}
	else
	{
		CLog.LogAdd("[CastleSpecialMix] [%s][%s] Item Mix Failed - No Auth", lpObj->AccountID, lpObj->Name);
		gSendProto.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;
		return;	
	}

	if( g_iCastleItemMixLimit <= 0 )
	{
		CLog.LogAdd("[CastleSpecialMix] [%s][%s] Item Mix Failed - Mix Count Limit Over", lpObj->AccountID, lpObj->Name);
		gSendProto.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;
		return;		
	}

	for( int n = 0; n < CHAOS_BOX_SIZE; n++ )
	{
		if ( lpObj->pChaosBox[n].IsItem() == TRUE )
		{
			if ( lpObj->pChaosBox[n].m_Type == ITEMGET(12,30) )
			{
				iBlessGemMixCount += lpObj->pChaosBox[n].m_Level + 1;
			}
			else if( lpObj->pChaosBox[n].m_Type == ITEMGET(12,31) )
			{
				iSoulGemMixCount += lpObj->pChaosBox[n].m_Level + 1;
			}
			else if( lpObj->pChaosBox[n].m_Type == ITEMGET(14,31) )
			{
				iDefendGemCount++;
			}
			else
			{
				iInvalidItemCount++;
			}
		}
	}

	if( iBlessGemMixCount != 3 || iSoulGemMixCount != 3 || iDefendGemCount != 30 || iInvalidItemCount > 0 )
	{
		CLog.LogAdd("[CastleSpecialMix] [%s][%s] Item Mix Failed - Item Error (DG:%d, BGx10:%d, SGx10:%d, Other:%d)", lpObj->AccountID, lpObj->Name, iBlessGemMixCount, iSoulGemMixCount, iDefendGemCount, iInvalidItemCount);
		gSendProto.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;
		return;	
	}

	LogChaosItem(lpObj, "Castle Special Mix");
	CLog.LogAdd("[CastleSpecialMix] [%s][%s] Chaos Mix Start",lpObj->AccountID,lpObj->Name);

	lpObj->ChaosSuccessRate = 100;

	int nChaosNeedMoney = 1000000000;

	if( lpObj->Money < nChaosNeedMoney )
	{
		CLog.LogAdd("[CastleSpecialMix] [%s][%s] Item Mix Failed - Lack of Money (%d/%d)", lpObj->AccountID, lpObj->Name, lpObj->Money, nChaosNeedMoney);
		pMsg.Result = CB_NOT_ENOUGH_ZEN;
		gSendProto.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;
		return;	
	}

	CItem objTempItem;


	int iPickLoopCount = 500;
	bool bItemPickSuccess = false;
	int nItemIndex;

	while( iPickLoopCount-- )
	{
		if( CastleItemMixItemBag->PickItem(objTempItem, nItemIndex) == FALSE )
		{
			CLog.LogAdd("[CastleSpecialMix] [%s][%s] Item Pick Failed - Data Error", lpObj->AccountID, lpObj->Name);
			gSendProto.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
			lpObj->ChaosLock = FALSE;
			return;
		}
		
		int iRand = CastleItemMixItemBag->GetMinLevel(nItemIndex);
		
		if( iRand < rand() % 100 )
		{
			bItemPickSuccess = true;

			break;
		}
	}

	if( bItemPickSuccess == false )
	{
		CLog.LogAdd("[CastleSpecialMix] [%s][%s] Item Pick Failed - Loop Count Over", lpObj->AccountID, lpObj->Name);
		gSendProto.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;
		return;
	}

	objTempItem.m_Level = 0;

	if( objTempItem.m_NewOption )
	{
		objTempItem.m_NewOption = 0;

		int iOptionLoop = (rand()%3) + 2;

		while( iOptionLoop )
		{
			int iRandOp = rand()%6;

			BYTE btOption = 1 << iRandOp; 

			if( (objTempItem.m_NewOption & btOption) == 0 )
			{
				objTempItem.m_NewOption |= btOption;
			}
			
			iOptionLoop--;
		}
		objTempItem.m_Option1 = 1;

		if( (rand()%100) < 20 )
		{
			objTempItem.m_Option2 = 1;
		}
		else
		{
			objTempItem.m_Option2 = 0;
		}
	}
	else
	{
		objTempItem.m_Option2 = 1;
	}

	objTempItem.m_Durability = ItemGetDurability(objTempItem.m_Type, objTempItem.m_Level, objTempItem.m_NewOption, 0);

	lpObj->Money -= nChaosNeedMoney;

	GCMoneySend(lpObj->m_Index, lpObj->Money);

	BYTE Socket[5];
	Socket[0] = 0xFF;
	Socket[1] = 0xFF;
	Socket[2] = 0xFF;
	Socket[3] = 0xFF;
	Socket[4] = 0xFF;

	if( (rand()%100) < lpObj->ChaosSuccessRate )
	{
		ItemSerialCreateSend(lpObj->m_Index, 255, 0, 0, objTempItem.m_Type,
			objTempItem.m_Level, objTempItem.m_Durability,
			objTempItem.m_Option1, objTempItem.m_Option2, objTempItem.m_Option3,
			-1, objTempItem.m_NewOption, 0, Socket);

		gObjInventoryCommit(lpObj->m_Index);
		CLog.LogAdd("[CastleSpecialMix] [%s][%s] CBMix Success %d Money : %d-%d", lpObj->AccountID, lpObj->Name, lpObj->ChaosSuccessRate, lpObj->Money, nChaosNeedMoney);
		g_iCastleItemMixLimit--;

	}
	else
	{
		gChaosBox.ChaosBoxInit(lpObj);
		GCUserChaosBoxSend(lpObj, 1);
		gSendProto.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		CLog.LogAdd("[CastleSpecialMix] [%s][%s] CBMix Fail %d Money : %d-%d", lpObj->AccountID, lpObj->Name, lpObj->ChaosSuccessRate, lpObj->Money, nChaosNeedMoney);
	}

	lpObj->ChaosLock = FALSE;
}
#endif


void HiddenTreasureBoxItemMix(LPOBJ lpObj)
{
	lpObj->ChaosLock = TRUE;
	int iBlueCrystal = 0;
	int iRedCrystal = 0;
	int iBlackCrystal = 0;
	int iInvalidItemCount = 0;
	int iChaosMoney = 0;

	PMSG_CHAOSMIXRESULT pMsg;

	C1HeadSet((LPBYTE)&pMsg.h, 0x86, sizeof(PMSG_CHAOSMIXRESULT));
	pMsg.Result = CB_ERROR;


	for ( int n=0;n<CHAOS_BOX_SIZE;n++)
	{
		if ( lpObj->pChaosBox[n].IsItem() == TRUE )
		{
			if ( lpObj->pChaosBox[n].m_Type == ITEMGET(12,26) ) // Gem of Secret
			{
				switch ( lpObj->pChaosBox[n].m_Level )
				{
					case 1:
						iRedCrystal++;
						break;
					case 2:
						iBlueCrystal++;
						break;
					case 3:
						iBlackCrystal++;
						break;
				}
			}
			else
			{
				iInvalidItemCount++;
			}
		}
	}

	int iCrystalMixType = -1;

	if ( iInvalidItemCount == 0 )
	{
		if ( iBlueCrystal == 7 )
		{
			if ( iRedCrystal == 0 && iBlackCrystal == 0 )
			{
				iCrystalMixType = 0;
				lpObj->ChaosSuccessRate = 100;
			}
		}
		else if ( iRedCrystal == 5 )
		{
			if ( iBlueCrystal == 0 && iBlackCrystal == 0 )
			{
				iCrystalMixType = 1;
				lpObj->ChaosSuccessRate = 100;
			}
		}
		else if ( iBlackCrystal == 3 )
		{
			if ( iRedCrystal == 0 && iBlueCrystal == 0 )
			{
				iCrystalMixType = 2;
				lpObj->ChaosSuccessRate = 100;
			}
		}
		else if ( iBlueCrystal == 1 && iRedCrystal == 1 && iBlackCrystal == 1 )
		{
			{
				iCrystalMixType = 3;
				lpObj->ChaosSuccessRate = 100;
			}
		}
		else
		{
			CLog.LogAdd("[Hidden TreasureBox Event] [%s][%s] Item Mix Failed", lpObj->AccountID, lpObj->Name);
			gSendProto.DataSend(lpObj->m_Index, (UCHAR*)&pMsg, pMsg.h.size);
			lpObj->ChaosLock = FALSE;
		}
	}
	else
	{
		CLog.LogAdd("[Hidden TreasureBox Event] [%s][%s] Item Mix Failed", lpObj->AccountID, lpObj->Name);
		gSendProto.DataSend(lpObj->m_Index, (UCHAR*)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		return;
	}

	LogChaosItem(lpObj, "Hidden TreasureBox Mix");
	CLog.LogAdd("[Hidden TreasureBox Event] [%s][%s] Chaos Mix Start", lpObj->AccountID, lpObj->Name);

	BYTE Socket[5];
	Socket[0] = 0xFF;
	Socket[1] = 0xFF;
	Socket[2] = 0xFF;
	Socket[3] = 0xFF;
	Socket[4] = 0xFF;

	if ( (rand()%100) < lpObj->ChaosSuccessRate )
	{
		switch ( iCrystalMixType )
		{
			case 0:	case 1:	case 2:
				ItemSerialCreateSend(lpObj->m_Index, -1, 0, 0, ITEMGET(12,26) , 4, 1, 0, 0, 0, -1, 0, 0, Socket);
				break;
			case 3:
				ItemSerialCreateSend(lpObj->m_Index, -1, 0, 0, ITEMGET(12,26) , 5, 1, 0, 0, 0, -1, 0, 0, Socket);
				break;
			default:
				CLog.LogAdd("[Hidden TreasureBox Event] [%s][%s] CBMix Failed - iCrystalMixType is wrong : %d", lpObj->AccountID, lpObj->Name, iCrystalMixType);
				gSendProto.DataSend(lpObj->m_Index, (UCHAR*)&pMsg, pMsg.h.size);
				lpObj->ChaosLock = FALSE;
				return;
		}

		::gObjInventoryCommit(lpObj->m_Index);
		CLog.LogAdd("[Hidden TreasureBox Event] [%s][%s] CBMix Success:%d Type:%d", lpObj->AccountID, lpObj->Name, lpObj->ChaosSuccessRate, iCrystalMixType);
	}
	else
	{
		CLog.LogAdd("[Hidden TreasureBox Event] [%s][%s] CBMix Fail Rate:%d", lpObj->AccountID, lpObj->Name, lpObj->ChaosSuccessRate);
		gChaosBox.ChaosBoxInit(lpObj);
		::GCUserChaosBoxSend(lpObj, 1);
		gSendProto.DataSend(lpObj->m_Index, (UCHAR*)&pMsg, pMsg.h.size);

		lpObj->ChaosLock = FALSE;
	}

	lpObj->ChaosLock = FALSE;
}

void Fenrir_01Level_Mix(LPOBJ lpObj)
{
	lpObj->ChaosLock = TRUE;
	int iStuffCount_01 = 0;
	int iStuffCount_02 = 0;
	int iChaosGemCount = 0;
	int iInvalidItemCount = 0;
	int iChaosMoney = 0;
	int iCharmOfLuckCount = 0;

	PMSG_CHAOSMIXRESULT pMsg;

	C1HeadSet((LPBYTE)&pMsg.h, 0x86, sizeof(PMSG_CHAOSMIXRESULT));
	pMsg.Result = CB_ERROR;


	for ( int n=0;n<CHAOS_BOX_SIZE;n++)
	{
		if ( lpObj->pChaosBox[n].IsItem() == TRUE )
		{
			if ( lpObj->pChaosBox[n].m_Type == ITEMGET(13,32) )
			{
				iStuffCount_01 += lpObj->pChaosBox[n].m_Durability;
			}
			else if ( lpObj->pChaosBox[n].m_Type == ITEMGET(13,33) )
			{
				iStuffCount_02 += lpObj->pChaosBox[n].m_Durability;
			}
			else if ( lpObj->pChaosBox[n].m_Type == ITEMGET(12,15) )
			{
				iChaosGemCount++;
			}
			else if ( lpObj->pChaosBox[n].m_Type == ITEMGET(14,53) )	// Charm Of Luck
			{
				iCharmOfLuckCount += lpObj->pChaosBox[n].m_Durability;
			}
			else
			{
				iInvalidItemCount++;
			}
		}
	}

	if ( iStuffCount_01 != 20 || iStuffCount_02 != 20 || iChaosGemCount != 1 || iInvalidItemCount > 0 )
	{
		gSendProto.DataSend(lpObj->m_Index, (UCHAR*)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		return;
	}
	
	if ( iCharmOfLuckCount > 10 )
	{
		pMsg.Result = 0xF0;
		gSendProto.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		return;
	}

	LogChaosItem(lpObj, "Fenrir_01Level_Mix");
	CLog.LogAdd("[Fenrir Mix][Level 01] Chaos Mix Start");

	lpObj->ChaosSuccessRate = gPets.Fenrir01ChaosSuccessRate;
	lpObj->ChaosSuccessRate += iCharmOfLuckCount;
	int nChaosNeedMoney = 0;	// #error Set the Correct price before call the CstleSyegeTributes

	int iChaosTaxMoney = (int)((__int64)nChaosNeedMoney * (__int64)g_CastleSiegeSync.GetTaxRateChaos(lpObj->m_Index) / (__int64)100);

	if ( iChaosTaxMoney < 0 )
	{
		iChaosTaxMoney = 0;
	}

	nChaosNeedMoney += iChaosTaxMoney;

	if ( nChaosNeedMoney < 0 )
	{
		nChaosNeedMoney = 0;
	}

	if ( lpObj->Money < nChaosNeedMoney )
	{
		pMsg.Result = CB_NOT_ENOUGH_ZEN;
		
		gSendProto.DataSend(lpObj->m_Index, (BYTE *)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		return;
	}

	lpObj->Money -= nChaosNeedMoney;
	g_CastleSiegeSync.AddTributeMoney(iChaosTaxMoney);
	GCMoneySend(lpObj->m_Index, lpObj->Money);

	if ( (rand()%100) < lpObj->ChaosSuccessRate )
	{
		int type = ITEMGET(13,35);
		int level = 0;
		int dur = 1;

		BYTE Socket[5];
		Socket[0] = 0xFF;
		Socket[1] = 0xFF;
		Socket[2] = 0xFF;
		Socket[3] = 0xFF;
		Socket[4] = 0xFF;

		ItemSerialCreateSend(lpObj->m_Index, -1, 0, 0, type , level, dur, 0, 0, 0, -1, 0, 0, Socket);
		::gObjInventoryCommit(lpObj->m_Index);

		CLog.LogAdd("[Fenrir Mix][Level 01] [%s][%s] CBMix Success %d Money : %d-%d, CharmRate : %d",
			lpObj->AccountID, lpObj->Name, lpObj->ChaosSuccessRate, lpObj->Money, nChaosNeedMoney,
			iCharmOfLuckCount);

	}
	else
	{
		gChaosBox.ChaosBoxInit(lpObj);
		::GCUserChaosBoxSend(lpObj, 0);
		gSendProto.DataSend(lpObj->m_Index, (UCHAR*)&pMsg, pMsg.h.size);

		CLog.LogAdd("[Fenrir Mix][Level 01] [%s][%s] CBMix Fail %d Money : %d-%d, CharmRate : %d",
			lpObj->AccountID, lpObj->Name, lpObj->ChaosSuccessRate, lpObj->Money, nChaosNeedMoney,
			iCharmOfLuckCount);

		lpObj->ChaosLock = FALSE;
	}
}

void Fenrir_02Level_Mix(LPOBJ lpObj)
{
	lpObj->ChaosLock = TRUE;
	int iStuffCount_01 = 0;
	int iStuffCount_02 = 0;
	int iChaosGemCount = 0;
	int iInvalidItemCount = 0;
	int iChaosMoney = 0;
	int iCharmOfLuckCount = 0;

	PMSG_CHAOSMIXRESULT pMsg;

	C1HeadSet((LPBYTE)&pMsg.h, 0x86, sizeof(PMSG_CHAOSMIXRESULT));
	pMsg.Result = CB_ERROR;


	for ( int n=0;n<CHAOS_BOX_SIZE;n++)
	{
		if ( lpObj->pChaosBox[n].IsItem() == TRUE )
		{
			if ( lpObj->pChaosBox[n].m_Type == ITEMGET(13,34) )
			{
				iStuffCount_01 += lpObj->pChaosBox[n].m_Durability;
			}
			else if ( lpObj->pChaosBox[n].m_Type == ITEMGET(13,35) )
			{
				iStuffCount_02 ++;
			}
			else if ( lpObj->pChaosBox[n].m_Type == ITEMGET(12,15) )
			{
				iChaosGemCount++;
			}
			else if ( lpObj->pChaosBox[n].m_Type == ITEMGET(14,53) )	// Charm Of Luck
			{
				iCharmOfLuckCount += lpObj->pChaosBox[n].m_Durability;
			}
			else
			{
				iInvalidItemCount++;
			}
		}
	}

	if ( iStuffCount_01 != 10 || iStuffCount_02 != 5 || iChaosGemCount != 1 || iInvalidItemCount > 0 )
	{
		gSendProto.DataSend(lpObj->m_Index, (UCHAR*)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		return;
	}
	
	if ( iCharmOfLuckCount > 10 )
	{
		pMsg.Result = 0xF0;
		gSendProto.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		return;
	}

	LogChaosItem(lpObj, "Fenrir_02Level_Mix");
	CLog.LogAdd("[Fenrir Mix][Level 02] Chaos Mix Start");

	lpObj->ChaosSuccessRate = gPets.Fenrir02ChaosSuccessRate;
	lpObj->ChaosSuccessRate += iCharmOfLuckCount;
	int nChaosNeedMoney = 0;	// #error Set the Correct price before call the CstleSyegeTributes

	int iChaosTaxMoney = (int)((__int64)nChaosNeedMoney * (__int64)g_CastleSiegeSync.GetTaxRateChaos(lpObj->m_Index) / (__int64)100);

	if ( iChaosTaxMoney < 0 )
	{
		iChaosTaxMoney = 0;
	}

	nChaosNeedMoney += iChaosTaxMoney;

	if ( nChaosNeedMoney < 0 )
	{
		nChaosNeedMoney = 0;
	}

	if ( lpObj->Money < nChaosNeedMoney )
	{
		pMsg.Result = CB_NOT_ENOUGH_ZEN;
		
		gSendProto.DataSend(lpObj->m_Index, (BYTE *)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		return;
	}

	lpObj->Money -= nChaosNeedMoney;
	g_CastleSiegeSync.AddTributeMoney(iChaosTaxMoney);
	GCMoneySend(lpObj->m_Index, lpObj->Money);

	if ( (rand()%100) < lpObj->ChaosSuccessRate )
	{
		int type = ITEMGET(13,36);
		int level = 0;
		int dur = 1;

		BYTE Socket[5];
		Socket[0] = 0xFF;
		Socket[1] = 0xFF;
		Socket[2] = 0xFF;
		Socket[3] = 0xFF;
		Socket[4] = 0xFF;

		ItemSerialCreateSend(lpObj->m_Index, -1, 0, 0, type , level, dur, 0, 0, 0, -1, 0, 0, Socket);
		::gObjInventoryCommit(lpObj->m_Index);

		CLog.LogAdd("[Fenrir Mix][Level 02] [%s][%s] CBMix Success %d Money : %d-%d, CharmRate : %d",
			lpObj->AccountID, lpObj->Name, lpObj->ChaosSuccessRate, lpObj->Money, nChaosNeedMoney,
			iCharmOfLuckCount);

	}
	else
	{
		gChaosBox.ChaosBoxInit(lpObj);
		::GCUserChaosBoxSend(lpObj, 0);
		gSendProto.DataSend(lpObj->m_Index, (UCHAR*)&pMsg, pMsg.h.size);

		CLog.LogAdd("[Fenrir Mix][Level 02] [%s][%s] CBMix Fail %d Money : %d-%d, CharmRate : %d",
			lpObj->AccountID, lpObj->Name, lpObj->ChaosSuccessRate, lpObj->Money, nChaosNeedMoney,
			iCharmOfLuckCount);

		lpObj->ChaosLock = FALSE;
	}
}

void Fenrir_03Level_Mix(LPOBJ lpObj)
{
	lpObj->ChaosLock = TRUE;
	int iStuffCount_01 = 0;
	int iLifeGemCount = 0;
	int iChaosGemCount = 0;
	int iInvalidItemCount = 0;
	int iChaosMoney = 0;
	int iCharmOfLuckCount = 0;

	PMSG_CHAOSMIXRESULT pMsg;

	C1HeadSet((LPBYTE)&pMsg.h, 0x86, sizeof(PMSG_CHAOSMIXRESULT));
	pMsg.Result = CB_ERROR;


	for ( int n=0;n<CHAOS_BOX_SIZE;n++)
	{
		if ( lpObj->pChaosBox[n].IsItem() == TRUE )
		{
			if ( lpObj->pChaosBox[n].m_Type == ITEMGET(13,36) )
			{
				iStuffCount_01 ++;
			}
			else if ( lpObj->pChaosBox[n].m_Type == ITEMGET(14,16) )
			{
				iLifeGemCount ++;
			}
			else if ( lpObj->pChaosBox[n].m_Type == ITEMGET(12,15) )
			{
				iChaosGemCount++;
			}
			else if ( lpObj->pChaosBox[n].m_Type == ITEMGET(14,53) )	// Charm Of Luck
			{
				iCharmOfLuckCount += lpObj->pChaosBox[n].m_Durability;
			}
			else
			{
				iInvalidItemCount++;
			}
		}
	}

	if ( iStuffCount_01 != 1 || iLifeGemCount != 3 || iChaosGemCount != 1 || iInvalidItemCount > 0 )
	{
		gSendProto.DataSend(lpObj->m_Index, (UCHAR*)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		return;
	}
	
	if ( iCharmOfLuckCount > 10 )
	{
		pMsg.Result = 0xF0;
		gSendProto.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		return;
	}

	LogChaosItem(lpObj, "Fenrir_03Level_Mix");
	CLog.LogAdd("[Fenrir Mix][Level 03] Chaos Mix Start");

	lpObj->ChaosSuccessRate = gPets.Fenrir03ChaosSuccessRate;
	lpObj->ChaosSuccessRate += iCharmOfLuckCount;
	int nChaosNeedMoney = 10000000;

	int iChaosTaxMoney = (int)((__int64)nChaosNeedMoney * (__int64)g_CastleSiegeSync.GetTaxRateChaos(lpObj->m_Index) / (__int64)100);

	if ( iChaosTaxMoney < 0 )
	{
		iChaosTaxMoney = 0;
	}

	nChaosNeedMoney += iChaosTaxMoney;

	if ( nChaosNeedMoney < 0 )
	{
		nChaosNeedMoney = 0;
	}

	if ( lpObj->Money < nChaosNeedMoney )
	{
		pMsg.Result = CB_NOT_ENOUGH_ZEN;
		
		gSendProto.DataSend(lpObj->m_Index, (BYTE *)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		return;
	}

	lpObj->Money -= nChaosNeedMoney;
	g_CastleSiegeSync.AddTributeMoney(iChaosTaxMoney);
	GCMoneySend(lpObj->m_Index, lpObj->Money);

	if ( (rand()%100) < lpObj->ChaosSuccessRate )
	{
		int type = ITEMGET(13,37);
		int level = 0;
		int dur = 255;
		int op1 = 1;

		BYTE Socket[5];
		Socket[0] = 0xFF;
		Socket[1] = 0xFF;
		Socket[2] = 0xFF;
		Socket[3] = 0xFF;
		Socket[4] = 0xFF;

		ItemSerialCreateSend(lpObj->m_Index, -1, 0, 0, type , level, dur, op1, 0, 0, -1, 0, 0, Socket);
		::gObjInventoryCommit(lpObj->m_Index);

		CLog.LogAdd("[Fenrir Mix][Level 03] [%s][%s] CBMix Success %d Money : %d-%d, CharmRate : %d",
			lpObj->AccountID, lpObj->Name, lpObj->ChaosSuccessRate, lpObj->Money, nChaosNeedMoney,
			iCharmOfLuckCount);

	}
	else
	{
		gChaosBox.ChaosBoxInit(lpObj);
		::GCUserChaosBoxSend(lpObj, 0);
		gSendProto.DataSend(lpObj->m_Index, (UCHAR*)&pMsg, pMsg.h.size);

		CLog.LogAdd("[Fenrir Mix][Level 03] [%s][%s] CBMix Fail %d Money : %d-%d, CharmRate : %d",
			lpObj->AccountID, lpObj->Name, lpObj->ChaosSuccessRate, lpObj->Money, nChaosNeedMoney,
			iCharmOfLuckCount);

		lpObj->ChaosLock = FALSE;
	}
}

void Fenrir_04Upgrade_Mix(LPOBJ lpObj)
{
	lpObj->ChaosLock = TRUE;
	int iStuffCount_01 = 0;
	int iLifeGemCount = 0;
	int iChaosGemCount = 0;
	int iAttackStuffCount = 0;
	int iDefendStuffCount = 0;
	int iAttackStuffPrice = 0;
	int iDefendStuffPrice = 0;
	int iInvalidItemCount = 0;
	int iChaosMoney = 0;
	BOOL bFenrirDamageInc = FALSE;
	BOOL bFenrirDamageDec = FALSE;
	int iCharmOfLuckCount = 0;

	PMSG_CHAOSMIXRESULT pMsg;

	C1HeadSet((LPBYTE)&pMsg.h, 0x86, sizeof(PMSG_CHAOSMIXRESULT));
	pMsg.Result = CB_ERROR;


	for ( int n=0;n<CHAOS_BOX_SIZE;n++)
	{
		if ( lpObj->pChaosBox[n].IsItem() == TRUE )
		{
			if ( lpObj->pChaosBox[n].m_Type == ITEMGET(13,37) )
			{
				iStuffCount_01 ++;
			}
			else if ( lpObj->pChaosBox[n].m_Type == ITEMGET(14,16) )
			{
				iLifeGemCount ++;
			}
			else if ( lpObj->pChaosBox[n].m_Type == ITEMGET(12,15) )
			{
				iChaosGemCount++;
			}
			else if ( lpObj->pChaosBox[n].m_Type >= ITEMGET(0,0) &&
					  lpObj->pChaosBox[n].m_Type < ITEMGET(6,0) &&
					  lpObj->pChaosBox[n].m_Level >= 4 &&
					  lpObj->pChaosBox[n].m_Option3 >= 1)
			{
				iAttackStuffCount++;
				iAttackStuffPrice += lpObj->pChaosBox[n].m_BuyMoney;
			}
			else if ( lpObj->pChaosBox[n].m_Type >= ITEMGET(6,0) &&
					  lpObj->pChaosBox[n].m_Type < ITEMGET(12,0) &&
					  lpObj->pChaosBox[n].m_Level >= 4 &&
					  lpObj->pChaosBox[n].m_Option3 >= 1)
			{
				iDefendStuffCount++;
				iDefendStuffPrice += lpObj->pChaosBox[n].m_BuyMoney;
			}
			else if ( lpObj->pChaosBox[n].m_Type == ITEMGET(14,53) )	// Charm Of Luck
			{
				iCharmOfLuckCount += lpObj->pChaosBox[n].m_Durability;
			}
			else
			{
				iInvalidItemCount++;
			}
		}
	}

	if ( iStuffCount_01 != 1 || iLifeGemCount != 5 || iChaosGemCount != 1 || ( iAttackStuffCount == 0 && iDefendStuffCount == 0 ) || iInvalidItemCount > 0 )
	{
		gSendProto.DataSend(lpObj->m_Index, (UCHAR*)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		return;
	}
	
	if ( iCharmOfLuckCount > 10 )
	{
		pMsg.Result = 0xF0;
		gSendProto.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		return;
	}

	if ( iAttackStuffCount > 0 )
	{
		bFenrirDamageInc = TRUE;
		iChaosMoney = iAttackStuffPrice;
	}

	if ( iDefendStuffCount > 0 )
	{
		bFenrirDamageDec = TRUE;
		iChaosMoney = iDefendStuffPrice;
	}

	if ( bFenrirDamageInc && bFenrirDamageDec )
	{
		gSendProto.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		return;
	}

	if ( !bFenrirDamageInc && !bFenrirDamageDec )
	{
		gSendProto.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		return;
	}

	LogChaosItem(lpObj, "Fenrir_04Level_Mix");
	CLog.LogAdd("[Fenrir Mix][Level 04] Chaos Mix Start");

	int nChaosNeedMoney = 10000000;

	int iChaosTaxMoney = (int)((__int64)nChaosNeedMoney * (__int64)g_CastleSiegeSync.GetTaxRateChaos(lpObj->m_Index) / (__int64)100);

	if ( iChaosTaxMoney < 0 )
	{
		iChaosTaxMoney = 0;
	}

	nChaosNeedMoney += iChaosTaxMoney;

	if ( nChaosNeedMoney < 0 )
	{
		nChaosNeedMoney = 0;
	}

	if ( lpObj->Money < nChaosNeedMoney )
	{
		pMsg.Result = CB_NOT_ENOUGH_ZEN;
		
		gSendProto.DataSend(lpObj->m_Index, (BYTE *)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		return;
	}

	lpObj->Money -= nChaosNeedMoney;
	g_CastleSiegeSync.AddTributeMoney(iChaosTaxMoney);
	GCMoneySend(lpObj->m_Index, lpObj->Money);

	if ( iChaosMoney > 1000000 )
		iChaosMoney = 1000000;

	lpObj->ChaosSuccessRate = iChaosMoney * 100 / 1000000;

	if ( lpObj->ChaosSuccessRate > 79 )
		lpObj->ChaosSuccessRate = 79;

	lpObj->ChaosSuccessRate += iCharmOfLuckCount;

	if ( (rand()%100) < lpObj->ChaosSuccessRate )
	{
		int type = ITEMGET(13,37);
		int level = 0;
		int dur = 255;
		int op1 = 1;
		int nop = 0;

		if ( bFenrirDamageInc )
		{
			nop |= 1;
		}

		if ( bFenrirDamageDec )
		{
			nop |= 2;
		}

		BYTE Socket[5];
		Socket[0] = 0xFF;
		Socket[1] = 0xFF;
		Socket[2] = 0xFF;
		Socket[3] = 0xFF;
		Socket[4] = 0xFF;

		ItemSerialCreateSend(lpObj->m_Index, -1, 0, 0, type , level, dur, op1, 0, 0, -1, nop, 0, Socket);
		::gObjInventoryCommit(lpObj->m_Index);

		CLog.LogAdd("[Fenrir Mix][Level 04] [%s][%s] CBMix Success %d Money : %d-%d, CharmRate : %d",
			lpObj->AccountID, lpObj->Name, lpObj->ChaosSuccessRate, lpObj->Money, nChaosNeedMoney,
			iCharmOfLuckCount);

	}
	else
	{
		gChaosBox.ChaosBoxInit(lpObj);
		::GCUserChaosBoxSend(lpObj, 0);
		gSendProto.DataSend(lpObj->m_Index, (UCHAR*)&pMsg, pMsg.h.size);

		CLog.LogAdd("[Fenrir Mix][Level 04] [%s][%s] CBMix Fail %d Money : %d-%d, CharmRate : %d",
			lpObj->AccountID, lpObj->Name, lpObj->ChaosSuccessRate, lpObj->Money, nChaosNeedMoney,
			iCharmOfLuckCount);

		lpObj->ChaosLock = FALSE;
	}
}

void ShieldPotionLv1_Mix(LPOBJ lpObj)
{
	int iHealthPotionCount = 0;
	int iInvalidItemCount = 0;
	int iChaosMixPrice = 0;
	int iCharmOfLuckCount = 0;

	if ( g_ShieldSystemOn == FALSE )
		return;

	PMSG_CHAOSMIXRESULT pMsg;

	C1HeadSet((LPBYTE)&pMsg.h, 0x86, sizeof(PMSG_CHAOSMIXRESULT));
	pMsg.Result = CB_ERROR;

	for ( int n=0;n<CHAOS_BOX_SIZE;n++)
	{
		if ( lpObj->pChaosBox[n].IsItem() == TRUE )
		{
			if ( lpObj->pChaosBox[n].m_Type == ITEMGET(14,3) )
			{
				iHealthPotionCount+=lpObj->pChaosBox[n].m_Durability;
			}
			else if ( lpObj->pChaosBox[n].m_Type == ITEMGET(14,53) )	// Charm Of Luck
			{
				iCharmOfLuckCount += lpObj->pChaosBox[n].m_Durability;
			}
			else
			{
				iInvalidItemCount++;
			}
		}
	}

	if ( iInvalidItemCount > 0 || iHealthPotionCount != 3 )
	{
		gSendProto.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		return;
	}

	if ( iCharmOfLuckCount > 10 )
	{
		pMsg.Result = 0xF0;
		gSendProto.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		return;
	}

	iChaosMixPrice = g_iShieldPotionLv1MixMoney;
	int iChaosTaxMoney = iChaosMixPrice * g_CastleSiegeSync.GetTaxRateChaos(lpObj->m_Index) / 100;

	if ( iChaosTaxMoney < 0 )
	{
		iChaosTaxMoney = 0;
	}

	iChaosMixPrice += iChaosTaxMoney;

	if ( iChaosMixPrice < 0 )
	{
		iChaosMixPrice = 0;
	}

	if ( lpObj->Money < iChaosMixPrice )
	{
		pMsg.Result = CB_NOT_ENOUGH_ZEN;
		
		gSendProto.DataSend(lpObj->m_Index, (BYTE *)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		return;
	}

	lpObj->Money -= iChaosMixPrice;
	g_CastleSiegeSync.AddTributeMoney(iChaosTaxMoney);
	GCMoneySend(lpObj->m_Index, lpObj->Money);

	LogChaosItem(lpObj, "PotionMix][ShieldPotion Lv1 Mix");
	CLog.LogAdd("[PotionMix][ShieldPotion Lv1 Mix] - Mix Start");
	
	int iRate = rand() % 100;
	iRate -= iCharmOfLuckCount;

	if ( iRate < g_iShieldPotionLv1MixSuccessRate )
	{
		int ItemNum = ITEMGET(14,35);

		BYTE Socket[5];
		Socket[0] = 0xFF;
		Socket[1] = 0xFF;
		Socket[2] = 0xFF;
		Socket[3] = 0xFF;
		Socket[4] = 0xFF;

		ItemSerialCreateSend(lpObj->m_Index, -1, 0, 0, ItemNum, 0, 1, 0, 0, 0, lpObj->m_Index, 0, 0, Socket);
		::gObjInventoryCommit(lpObj->m_Index);

		CLog.LogAdd("[ChaosMix][Shield Potion] Lv1 Potion Mix Success [%s][%s], Money(%d-%d), CharmRate : %d",
			lpObj->AccountID, lpObj->Name, 	lpObj->Money, iChaosMixPrice, iCharmOfLuckCount);
	}
	else
	{
		gChaosBox.ChaosBoxInit(lpObj);
		::GCUserChaosBoxSend(lpObj, 0);
		gSendProto.DataSend(lpObj->m_Index, (UCHAR*)&pMsg, pMsg.h.size);

		CLog.LogAdd("[ChaosMix][Shield Potion] Lv1 Potion Mix [%s][%s] CBMix Fail %d Money : %d-%d, CharmRate : %d",
			lpObj->AccountID, lpObj->Name, 	g_iShieldPotionLv1MixSuccessRate,
			lpObj->Money, iChaosMixPrice, iCharmOfLuckCount);
	}

	lpObj->ChaosLock = FALSE;
}

void ShieldPotionLv2_Mix(LPOBJ lpObj)
{
	int iHealthPotionCount = 0;
	int iInvalidItemCount = 0;
	int iChaosMixPrice = 0;
	int iCharmOfLuckCount = 0;

	PMSG_CHAOSMIXRESULT pMsg;

	C1HeadSet((LPBYTE)&pMsg.h, 0x86, sizeof(PMSG_CHAOSMIXRESULT));
	pMsg.Result = CB_ERROR;

	for ( int n=0;n<CHAOS_BOX_SIZE;n++)
	{
		if ( lpObj->pChaosBox[n].IsItem() == TRUE )
		{
			if ( lpObj->pChaosBox[n].m_Type == ITEMGET(14,38) )
			{
				iHealthPotionCount+=lpObj->pChaosBox[n].m_Durability;
			}
			else if ( lpObj->pChaosBox[n].m_Type == ITEMGET(14,53) )	// Charm Of Luck
			{
				iCharmOfLuckCount += lpObj->pChaosBox[n].m_Durability;
			}
			else
			{
				iInvalidItemCount++;
			}
		}
	}

	if ( iInvalidItemCount > 0 || iHealthPotionCount != 3 )
	{
		gSendProto.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		return;
	}

	if ( iCharmOfLuckCount > 10 )
	{
		pMsg.Result = 0xF0;
		gSendProto.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		return;
	}

	iChaosMixPrice = g_iShieldPotionLv2MixMoney;
	int iChaosTaxMoney = iChaosMixPrice * g_CastleSiegeSync.GetTaxRateChaos(lpObj->m_Index) / 100;

	if ( iChaosTaxMoney < 0 )
	{
		iChaosTaxMoney = 0;
	}

	iChaosMixPrice += iChaosTaxMoney;

	if ( iChaosMixPrice < 0 )
	{
		iChaosMixPrice = 0;
	}

	if ( lpObj->Money < iChaosMixPrice )
	{
		pMsg.Result = CB_NOT_ENOUGH_ZEN;
		
		gSendProto.DataSend(lpObj->m_Index, (BYTE *)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		return;
	}

	lpObj->Money -= iChaosMixPrice;
	g_CastleSiegeSync.AddTributeMoney(iChaosTaxMoney);
	
	LogChaosItem(lpObj, "PotionMix][ShieldPotion Lv2 Mix");
	CLog.LogAdd("[PotionMix][ShieldPotion Lv2 Mix] - Mix Start");

	GCMoneySend(lpObj->m_Index, lpObj->Money);
	int iRate = rand() % 100;
	iRate -= iCharmOfLuckCount;

	if ( iRate < g_iShieldPotionLv2MixSuccessRate )
	{
		int ItemNum = ITEMGET(14,36);

		BYTE Socket[5];
		Socket[0] = 0xFF;
		Socket[1] = 0xFF;
		Socket[2] = 0xFF;
		Socket[3] = 0xFF;
		Socket[4] = 0xFF;

		ItemSerialCreateSend(lpObj->m_Index, -1, 0, 0, ItemNum, 0, 1, 0, 0, 0, lpObj->m_Index, 0, 0, Socket);
		::gObjInventoryCommit(lpObj->m_Index);

		CLog.LogAdd("[ChaosMix][Shield Potion] Lv2 Potion Mix Success [%s][%s], Money(%d-%d), CharmRate : %d",
			lpObj->AccountID, lpObj->Name, 	lpObj->Money, iChaosMixPrice, iCharmOfLuckCount);
	}
	else
	{
		gChaosBox.ChaosBoxInit(lpObj);
		::GCUserChaosBoxSend(lpObj, 0);
		gSendProto.DataSend(lpObj->m_Index, (UCHAR*)&pMsg, pMsg.h.size);

		CLog.LogAdd("[ChaosMix][Shield Potion] Lv2 Potion Mix [%s][%s] CBMix Fail %d Money : %d-%d, CharmRate : %d",
			lpObj->AccountID, lpObj->Name, 	g_iShieldPotionLv2MixSuccessRate,
			lpObj->Money, iChaosMixPrice, iCharmOfLuckCount);
	}

	lpObj->ChaosLock = FALSE;
}

void ShieldPotionLv3_Mix(LPOBJ lpObj)
{
	int iHealthPotionCount = 0;
	int iInvalidItemCount = 0;
	int iChaosMixPrice = 0;
	int iCharmOfLuckCount = 0;

	PMSG_CHAOSMIXRESULT pMsg;

	C1HeadSet((LPBYTE)&pMsg.h, 0x86, sizeof(PMSG_CHAOSMIXRESULT));
	pMsg.Result = CB_ERROR;

	for ( int n=0;n<CHAOS_BOX_SIZE;n++)
	{
		if ( lpObj->pChaosBox[n].IsItem() == TRUE )
		{
			if ( lpObj->pChaosBox[n].m_Type == ITEMGET(14,39) )
			{
				iHealthPotionCount+=lpObj->pChaosBox[n].m_Durability;
			}
			else if ( lpObj->pChaosBox[n].m_Type == ITEMGET(14,53) )	// Charm Of Luck
			{
				iCharmOfLuckCount += lpObj->pChaosBox[n].m_Durability;
			}
			else
			{
				iInvalidItemCount++;
			}
		}
	}

	if ( iInvalidItemCount > 0 || iHealthPotionCount != 3 )
	{
		gSendProto.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		return;
	}

	if ( iCharmOfLuckCount > 10 )
	{
		pMsg.Result = 0xF0;
		gSendProto.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		return;
	}

	iChaosMixPrice = g_iShieldPotionLv3MixMoney;
	int iChaosTaxMoney = iChaosMixPrice * g_CastleSiegeSync.GetTaxRateChaos(lpObj->m_Index) / 100;

	if ( iChaosTaxMoney < 0 )
	{
		iChaosTaxMoney = 0;
	}

	iChaosMixPrice += iChaosTaxMoney;

	if ( iChaosMixPrice < 0 )
	{
		iChaosMixPrice = 0;
	}

	if ( lpObj->Money < iChaosMixPrice )
	{
		pMsg.Result = CB_NOT_ENOUGH_ZEN;
		
		gSendProto.DataSend(lpObj->m_Index, (BYTE *)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		return;
	}

	lpObj->Money -= iChaosMixPrice;
	g_CastleSiegeSync.AddTributeMoney(iChaosTaxMoney);
	GCMoneySend(lpObj->m_Index, lpObj->Money);
	
	LogChaosItem(lpObj, "PotionMix][ShieldPotion Lv3 Mix");
	CLog.LogAdd("[PotionMix][ShieldPotion Lv3 Mix] - Mix Start");

	int iRate = rand() % 100;
	iRate -= iCharmOfLuckCount;

	if ( iRate < g_iShieldPotionLv3MixSuccessRate )
	{
		int ItemNum = ITEMGET(14,37);

		BYTE Socket[5];
		Socket[0] = 0xFF;
		Socket[1] = 0xFF;
		Socket[2] = 0xFF;
		Socket[3] = 0xFF;
		Socket[4] = 0xFF;

		ItemSerialCreateSend(lpObj->m_Index, -1, 0, 0, ItemNum, 0, 1, 0, 0, 0, lpObj->m_Index, 0, 0, Socket);
		::gObjInventoryCommit(lpObj->m_Index);

		CLog.LogAdd("[ChaosMix][Shield Potion] Lv3 Potion Mix Success [%s][%s], Money(%d-%d), CharmRate : %d",
			lpObj->AccountID, lpObj->Name, 	lpObj->Money, iChaosMixPrice, iCharmOfLuckCount);
	}
	else
	{
		gChaosBox.ChaosBoxInit(lpObj);
		::GCUserChaosBoxSend(lpObj, 0);
		gSendProto.DataSend(lpObj->m_Index, (UCHAR*)&pMsg, pMsg.h.size);

		CLog.LogAdd("[ChaosMix][Shield Potion] Lv3 Potion Mix [%s][%s] CBMix Fail %d Money : %d-%d, CharmRate : %d",
			lpObj->AccountID, lpObj->Name, 	g_iShieldPotionLv3MixSuccessRate,
			lpObj->Money, iChaosMixPrice, iCharmOfLuckCount);
	}

	lpObj->ChaosLock = FALSE;
}

void LotteryItemMix(LPOBJ lpObj)
{
	PMSG_CHAOSMIXRESULT pMsg;

	C1HeadSet((LPBYTE)&pMsg.h, 0x86, sizeof(PMSG_CHAOSMIXRESULT));
	pMsg.Result = CB_ERROR;

	int iLotteryItemCount = 0;
	int iInvalidItemCount = 0;
	int iItemHeight = 0;
	int iItemWidth = 0;

	for ( int n=0;n<CHAOS_BOX_SIZE;n++)
	{
		if ( lpObj->pChaosBox[n].IsItem() == TRUE )
		{
			if ( lpObj->pChaosBox[n].m_Type == ITEMGET(14,54) )
			{
				iLotteryItemCount++;
			}
			else
			{
				iInvalidItemCount++;
			}
		}
	}

	if ( iInvalidItemCount > 0 || iLotteryItemCount > 1 )
	{
		gSendProto.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		return;
	}

	LogChaosItem(lpObj, "CashLottery");
	iItemHeight = 4;
	iItemWidth = 2;

	if ( !CheckInventoryEmptySpace(lpObj, iItemHeight, iItemWidth))
	{
		pMsg.Result = 0xF1;
		gSendProto.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		return;
	}

	CItem LotteryItem;

	if ( g_CashLotterySystem.GetItem(&LotteryItem) == -1 )
	{
		gSendProto.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;
		CLog.LogAdd("[ChaosMix][LotteryItemMix] Lottery Item Mix Failed. Can't Get Item from List.");

		return;
	}

	ItemSerialCreateSend(lpObj->m_Index, 255, 0, 0, LotteryItem.m_Type,
		LotteryItem.m_Level, LotteryItem.m_Durability,
		LotteryItem.m_Option1, LotteryItem.m_Option2, LotteryItem.m_Option3,
		lpObj->m_Index, LotteryItem.m_NewOption, 0, LotteryItem.m_ItemSocket);

	gObjInventoryCommit(lpObj->m_Index);

	CLog.LogAdd("[ChaosMix][ChaosCardMix] Chaos Card Item Mix Success [%s][%s]",
		lpObj->AccountID, lpObj->Name);

	BYTE btExOption[MAX_EXOPTION_SIZE];

	ItemIsBufExOption(btExOption, &LotteryItem);
	
	CLog.LogAdd("[CashShop][ChaosCardMix] - User(ID:%s,Name:%s) Item(Name:%s,Type:%d,Level:%d,Dur:%d,Skill:%d,Luck:%d,AddOption:%d,Ex:(%d,%d,%d,%d,%d,%d))",
		lpObj->AccountID, lpObj->Name, ItemAttribute[LotteryItem.m_Type].Name,
		LotteryItem.m_Type, LotteryItem.m_Level, (int)LotteryItem.m_Durability,
		LotteryItem.m_Option1, LotteryItem.m_Option2, LotteryItem.m_Option3,
		btExOption[0], btExOption[1], btExOption[2], btExOption[3], btExOption[4], btExOption[5]);
}

void CondorFeatherMix(LPOBJ lpObj)
{
	PMSG_CHAOSMIXRESULT pMsg;

	C1HeadSet((LPBYTE)&pMsg.h, 0x86, sizeof(PMSG_CHAOSMIXRESULT));
	pMsg.Result = CB_SUCCESS;

	if (( lpObj->ChaosLock == 0 ) && ( lpObj->m_bPShopOpen == 0 )) 
	{
		if ( lpObj->Money >= 2000000 ) 
		{
			lpObj->ChaosLock = TRUE;
			
			int iBlundedSoulCount = 0;
			int iChaosCount	= 0;
			int iCreationCount = 0;
			int iInvalidItemCount = 0;
			int iSetItemCount = 0;
			int iWingsCount = 0;
			
			iSetItemCount = TRUE;
			
			for (int i = 0; i < CHAOS_BOX_SIZE; i++) 
			{
				if( lpObj->pChaosBox[i].m_Type != -1) 
				{
					if (lpObj->pChaosBox[i].m_Type == ITEMGET(12,15) )
					{
						iChaosCount++;
					} 
					else if (lpObj->pChaosBox[i].m_Type == ITEMGET(14,22) ) 
					{
						iCreationCount++;
					} 
					else if ((lpObj->pChaosBox[i].m_Type == ITEMGET(12,31) ) && (lpObj->pChaosBox[i].m_Level == FALSE))
					{
						iBlundedSoulCount++;
					} 
					else if ((lpObj->pChaosBox[i].m_Type >= ITEMGET(12,3) && lpObj->pChaosBox[i].m_Type <= ITEMGET(12,6)) || (lpObj->pChaosBox[i].m_Type == ITEMGET(13,30)) || (lpObj->pChaosBox[i].m_Type == ITEMGET(12,42)) )
					{
						if(lpObj->pChaosBox[i].m_Level > 9)
							iWingsCount++;
					} 
					else 
					{
						iInvalidItemCount++;
					}
				}
			}
			if( /*(iInvalidItemCount == 0) && */ (iChaosCount == 1) && (iCreationCount == 1) && (iBlundedSoulCount == 1) && (iWingsCount == 1) && (iSetItemCount == 1)) 
			{
				int iRand = rand()%100;
				int ItemNum = ITEMGET(13,53);

				BYTE Socket[5];
				Socket[0] = 0xFF;
				Socket[1] = 0xFF;
				Socket[2] = 0xFF;
				Socket[3] = 0xFF;
				Socket[4] = 0xFF;
			
				if(iRand < 65)
				{
					ItemSerialCreateSend(lpObj->m_Index, 0xff, 0, 0, ItemNum, 0, 255, 0, 0, 0, -1, 0, 0, Socket);
					lpObj->Money -= 2000000;
					GCMoneySend(lpObj->m_Index, lpObj->Money);

					CLog.LogAdd("[Feather Of Condor][Feather Of Condor Mix] Feather Of Condor Item Mix Success [%s][%s]",
						lpObj->AccountID, lpObj->Name);
				} 
				else
				{
					gChaosBox.ChaosBoxInit(lpObj);
					GCUserChaosBoxSend(lpObj, 0);
					gSendProto.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
				}
			}
			lpObj->ChaosLock = 0;
		}
	}
}

void WingsLv3ChaosMix(LPOBJ lpObj)
{
	int ChaosCount = 0;
	int CreationCount = 0;
	int SoulPack10 = 0;
	int BlessPack10 = 0;
	int FlameCount = 0;
	int FeatherCount = 0;
	int ItemCount = 0;
	int ItemMoney = 0;

	lpObj->ChaosLock = 1;

	for(int n=0;n < CHAOS_BOX_SIZE;n++)
	{
	    if(lpObj->pChaosBox[n].IsItem() == 0)
		{
		    continue;
		}

	    if(lpObj->pChaosBox[n].m_Type == ITEMGET(12,15))
		{
		    ChaosCount++;
		}
		else if(lpObj->pChaosBox[n].m_Type == ITEMGET(14,22))
		{
		    CreationCount++;
		}
		else if(lpObj->pChaosBox[n].m_Type == ITEMGET(12,31) && lpObj->pChaosBox[n].m_Level == 0)
		{
		    SoulPack10++;
		}
		else if(lpObj->pChaosBox[n].m_Type == ITEMGET(12,30) && lpObj->pChaosBox[n].m_Level == 0)
		{
		    BlessPack10++;
		}
		else if(lpObj->pChaosBox[n].m_Type == ITEMGET(13,52))
		{
		    FlameCount++;
		}
	    else if(lpObj->pChaosBox[n].m_Type == ITEMGET(13,53))
		{
		    FeatherCount++;
		}
		else if(lpObj->pChaosBox[n].IsExtItem() != 0 && lpObj->pChaosBox[n].m_Level >= 9 && lpObj->pChaosBox[n].m_Option3 != 0)
		{
		    ItemCount++;
		    ItemMoney += lpObj->pChaosBox[n].m_BuyMoney;
		}
	}

    PMSG_CHAOSMIXRESULT pMsg;

    C1HeadSet((LPBYTE)&pMsg.h,0x86,sizeof(pMsg));

	if(ChaosCount != 1 || CreationCount != 1 || SoulPack10 != 1 || BlessPack10 != 1 || FlameCount != 1 || FeatherCount != 1 || ItemCount == 0)
	{
	    lpObj->ChaosLock = 0;

        pMsg.Result = 7;
        gSendProto.DataSend(lpObj->m_Index,(BYTE *)&pMsg, pMsg.h.size);
	    CLog.LogAdd("[WingMix,3] [%s][%s] Invalid Item",lpObj->AccountID,lpObj->Name);
		return;
	}

    lpObj->ChaosSuccessRate = (ItemMoney/3000000)+1;

    if(lpObj->ChaosSuccessRate > 40)
	{
	    lpObj->ChaosSuccessRate = 40;
	}

	lpObj->ChaosMoney = lpObj->ChaosSuccessRate*200000;

	int TaxMoney = (lpObj->ChaosMoney*g_CastleSiegeSync.GetTaxRateChaos(lpObj->m_Index))/100;

	lpObj->ChaosMoney += TaxMoney;

    if(lpObj->Money < lpObj->ChaosMoney)
	{
	    lpObj->ChaosLock = 0;

        pMsg.Result = 2;
        gSendProto.DataSend(lpObj->m_Index,(BYTE *)&pMsg, pMsg.h.size);
		CLog.LogAdd("[WingMix,3] [%s][%s] Lacking Money",lpObj->AccountID,lpObj->Name);
		return;
	}

    lpObj->Money -= lpObj->ChaosMoney;
    GCMoneySend(lpObj->m_Index,lpObj->Money);

	g_CastleSiegeSync.AddTributeMoney(TaxMoney);

	CLog.LogAdd("[WingMix,3] Chaos Mix Start");

    if((rand()%100) < lpObj->ChaosSuccessRate)
	{
	    int WingIndex = ITEMGET(12,36) + (rand()%6);

		if(WingIndex == ITEMGET(12,41))
		{
		    WingIndex = ITEMGET(12,43);
		}

		int Option2 = 0;
		int Option3 = 0;
		int NewOption = 0;

		GetWingAttr(0,&Option2,&Option3,&NewOption);

		BYTE Socket[5];
		Socket[0] = 0xFF;
		Socket[1] = 0xFF;
		Socket[2] = 0xFF;
		Socket[3] = 0xFF;
		Socket[4] = 0xFF;

        ItemSerialCreateSend(lpObj->m_Index,-1,0,0,WingIndex,0,0,0,Option2,Option3,-1,NewOption,0, Socket);
		CLog.LogAdd("[WingMix,3] [%s][%s] CBMix Success",lpObj->AccountID,lpObj->Name);
	}
    else
	{
	   gChaosBox.ChaosBoxInit(lpObj);
        GCUserChaosBoxSend(lpObj,0);

	    pMsg.Result = CB_ERROR;
        gSendProto.DataSend(lpObj->m_Index,(BYTE*)&pMsg, pMsg.h.size);
	    CLog.LogAdd("[WingMix,3] [%s][%s] CBMix Fail",lpObj->AccountID,lpObj->Name);
	}

	lpObj->ChaosLock = 0;
}

void GetWingAttr(int type,int* Option2,int* Option3,int* NewOption)
{
    if((rand()%5) == 0)
	{
	    *Option2 = 1;
	}

    switch(rand()%3)
    {
		case 0:
			if((rand()%100) < 4)
			{
			    *Option3 = 3;
			}
		    break;
		case 1:
		    if((rand()%100) < 10)
			{
			    *Option3 = 2;
			}
			break;
		case 2:
		    if((rand()%100) < 20)
			{
			    *Option3 = 1;
			}
	        break;
	}

	if(type == 0)
	{
	    if((rand()%5) == 0)
		{
		    *NewOption = 1 << (rand()%3);
		}

	    if((rand()%2) != 0)
		{
		    *NewOption |= 32;
		}
	}
	else if(type = 1)
	{
	    if((rand()%5) == 0)
		{
		    *NewOption = 1 << (rand()%4);
		}

		*NewOption |= 32;
	}
}

BOOL IllusionTempleEventChaosMix(LPOBJ lpObj, BOOL bCheckType, int iItemLevel)
{
	BOOL fail = TRUE;
	int aIndex = lpObj->m_Index;
	PMSG_CHAOSMIXRESULT pMsg;

	C1HeadSet((LPBYTE)&pMsg.h, 0x86, sizeof(PMSG_CHAOSMIXRESULT));
	pMsg.Result = CB_ERROR;
	lpObj->ChaosLock = TRUE;

	//CLog.LogAdd("[Illusion Temple] Chaos Mix Start");
	//CLog.LogAddC(3, "[Illusion Temple] Chaos Mix Start");

	INT nChaosNeedMoney = 0;
	int iCharmOfLuckCount = 0;
	iCharmOfLuckCount = lpObj->ChaosSuccessRate;

	if ( iCharmOfLuckCount > 10 )
	{
		pMsg.Result = 0xF0;
		lpObj->ChaosLock = FALSE;
		gSendProto.DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
	}

	switch ( iItemLevel )
	{
		case 0:
			lpObj->ChaosSuccessRate = 60;
			nChaosNeedMoney = 100000;
			break;

		case 1:
			lpObj->ChaosSuccessRate = 90;
			nChaosNeedMoney = 100000;
			break;

		case 2:
			lpObj->ChaosSuccessRate = 85;
			nChaosNeedMoney = 200000;
			break;

		case 3:
			lpObj->ChaosSuccessRate = 80;
			nChaosNeedMoney = 400000;
			break;

		case 4:
			lpObj->ChaosSuccessRate = 75;
			nChaosNeedMoney = 700000;
			break;

		case 5:
			lpObj->ChaosSuccessRate = 70;
			nChaosNeedMoney = 1100000;
			break;

		case 6:
			lpObj->ChaosSuccessRate = 65;
			nChaosNeedMoney = 1600000;
			break;

		default:
			//CLog.LogAdd("[Illusion Temple] [%s][%s] Invalid ITItem Level [%d]",
			//lpObj->AccountID, lpObj->Name, iItemLevel);

			pMsg.Result = CB_INVALID_ITEM_LEVEL;
			gSendProto.DataSend(aIndex, (BYTE *)&pMsg, pMsg.h.size);
			lpObj->ChaosLock = FALSE;
			return 1;
			break;
	}

	if ( (lpObj->Money - nChaosNeedMoney) < 0 )
	{
		//CLog.LogAdd("[Illusion Temple] [%s][%s] CBMix Not Enough Money [%d] need zen [%d]",
		//	lpObj->AccountID, lpObj->Name, lpObj->Money, nChaosNeedMoney);

		//CLog.LogAddC(3, "[Illusion Temple] [%s][%s] CBMix Not Enough Money [%d] need zen [%d]",
		//	lpObj->AccountID, lpObj->Name, lpObj->Money, nChaosNeedMoney);

		pMsg.Result = CB_NOT_ENOUGH_ZEN;
		gSendProto.DataSend(aIndex, (BYTE *)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;
		return 1;

	}

	lpObj->ChaosSuccessRate += iCharmOfLuckCount;

	int iRate = rand() % 100;

	BYTE Socket[5];
	Socket[0] = 0xFF;
	Socket[1] = 0xFF;
	Socket[2] = 0xFF;
	Socket[3] = 0xFF;
	Socket[4] = 0xFF;

	if ( bCheckType == TRUE )
	{
		if ( iRate < lpObj->ChaosSuccessRate )
		{
			int IllusionInv = ITEMGET(13,51);	// Illusion Ticket
			ItemSerialCreateSend(lpObj->m_Index, -1, 0, 0, IllusionInv, iItemLevel, 0, 0, 0, 0, -1, 0, 0, Socket);
			fail = FALSE;
		}
	}

	lpObj->Money -= nChaosNeedMoney;
	GCMoneySend(lpObj->m_Index, lpObj->Money);

	if ( fail == TRUE )
	{
		gChaosBox.ChaosBoxInit(lpObj);
		GCUserChaosBoxSend(lpObj, 0);
		gSendProto.DataSend(aIndex, (BYTE *)&pMsg, pMsg.h.size);

		//CLog.LogAdd("[Illusion Temple Invite,%d] [%s][%s] CBMix Fail %d Money : %d-%d", iItemLevel,
		//	lpObj->AccountID, lpObj->Name, lpObj->ChaosSuccessRate,
		//	lpObj->Money, nChaosNeedMoney);

		//CLog.LogAddC(3, "[Illusion Temple Invite,%d] [%s][%s] CBMix Fail %d Money : %d-%d", iItemLevel,
		//	lpObj->AccountID, lpObj->Name, lpObj->ChaosSuccessRate,
		//	lpObj->Money, nChaosNeedMoney);

		lpObj->ChaosLock = FALSE;
	}
	else
	{
		//CLog.LogAdd("[Illusion Temple Invite,%d] [%s][%s] CBMix Success Rate:%d Money : %d", iItemLevel,
		//	lpObj->AccountID, lpObj->Name, lpObj->ChaosSuccessRate,
		//	nChaosNeedMoney);

		//CLog.LogAddC(3, "[Illusion Temple Invite,%d] [%s][%s] CBMix Success Rate:%d Money : %d", iItemLevel,
		//	lpObj->AccountID, lpObj->Name, lpObj->ChaosSuccessRate,
		//	nChaosNeedMoney);
	}

	gObjInventoryCommit(lpObj->m_Index);

	return TRUE;
}

void IllusionTempleItemChaosMix(OBJECTSTRUCT* lpObj)
{
	lpObj->ChaosLock = TRUE;

	PMSG_CHAOSMIXRESULT pMsg;
	C1HeadSet((LPBYTE)&pMsg.h, 0x86, sizeof(PMSG_CHAOSMIXRESULT));
	pMsg.Result = CB_ERROR;
	int aIndex = lpObj->m_Index;
	int eventitemcount = 0;
	int itemlevel = 0;
	int Ret = IllusionTempleEventChaosMix(lpObj, eventitemcount, itemlevel);

	if ( Ret != FALSE )
	{
		if ( Ret == 3 )
		{
			pMsg.Result = CB_INCORRECT_MIX_ITEMS;
			gSendProto.DataSend(aIndex, (BYTE*)&pMsg, pMsg.h.size);
			//CLog.LogAdd("[Illusion Temple] DiffLevel Illusion Potion or Scroll [%d]", eventitemcount);
			lpObj->ChaosLock = FALSE;

			return;
		}
		
		if ( Ret == 2 )
		{
			pMsg.Result = CB_INCORRECT_MIX_ITEMS;
			gSendProto.DataSend(aIndex, (BYTE*)&pMsg, pMsg.h.size);
			//CLog.LogAdd("[Illusion Temple] Not Found Chaos Gem [%d]", eventitemcount);
			lpObj->ChaosLock = FALSE;
			
			return;
		}

		if ( Ret == 4 )
		{
			pMsg.Result = 0xF0;
			gSendProto.DataSend(aIndex, (BYTE*)&pMsg, pMsg.h.size);
			//CLog.LogAdd("[Illusion Temple] Charm Of Luck Over 10% [%d]", eventitemcount);
			lpObj->ChaosLock = FALSE;
			
			return;
		}

		
		if ( eventitemcount > 2 )
		{
			pMsg.Result = CB_TOO_MANY_ITEMS;
			gSendProto.DataSend(aIndex, (BYTE*)&pMsg, pMsg.h.size);
			//CLog.LogAdd("[Illusion Temple] Too many Illusions's Potions or Scrolls [%d]", eventitemcount);
			lpObj->ChaosLock = FALSE;

			return;
		}

		if ( lpObj->Level < 10 )
		{
			pMsg.Result = CB_LOW_LEVEL_USER;
			gSendProto.DataSend(aIndex, (BYTE*)&pMsg, pMsg.h.size);
			lpObj->ChaosLock = FALSE;

			return;
		}

		::IllusionTempleEventChaosMix(lpObj, Ret, itemlevel);
		lpObj->pTransaction = 1;
		return;
	}
	
	if ( eventitemcount > 1 )
	{
		pMsg.Result = CB_LACKING_MIX_ITEMS;
		gSendProto.DataSend(aIndex, (BYTE*)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		return;
	}
	lpObj->ChaosLock = FALSE;
}

void CherryBlossomMix(LPOBJ lpObj)
{
	lpObj->ChaosLock = true;
	int iGoldenCherryBlossomCount = 0;
	int iWhiteCherryBlossomCount = 0;
	int iRedCherryBlossomCount = 0;
	int TotalDurability = 0;
	int iChaosMoney = 0;
	char Type = -1;
	int iInvalidItemCount = 0;
	int RandomItem = 0;

	PMSG_CHAOSMIXRESULT pMsg;

	C1HeadSet((LPBYTE)&pMsg.h, 0x86, sizeof(PMSG_CHAOSMIXRESULT));
	pMsg.Result = CB_ERROR;

	for ( int n=0;n<CHAOS_BOX_SIZE;n++)
	{
		if ( lpObj->pChaosBox[n].IsItem() == true )
		{
			if ( lpObj->pChaosBox[n].m_Type == ITEMGET(14,90) )
			{
				Type = 2;
				iGoldenCherryBlossomCount++;
				TotalDurability += lpObj->pChaosBox[n].m_Durability;
			}
			else if ( lpObj->pChaosBox[n].m_Type == ITEMGET(14,88) )
			{
				Type = 0;
				iWhiteCherryBlossomCount++;
				TotalDurability += lpObj->pChaosBox[n].m_Durability;
			}
			else if ( lpObj->pChaosBox[n].m_Type == ITEMGET(14,89) )
			{
				Type = 1;
				iRedCherryBlossomCount++;
				TotalDurability += lpObj->pChaosBox[n].m_Durability;
			}
			else
			{
				iInvalidItemCount++;
			}
		}
	}

	if ( iGoldenCherryBlossomCount == 0 && iWhiteCherryBlossomCount == 0 && iRedCherryBlossomCount == 0 )
	{
		pMsg.Result = CB_INCORRECT_MIX_ITEMS;
		gSendProto.DataSend(lpObj->m_Index, (BYTE*)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = false;
		return;
	}

	if ( iInvalidItemCount > 0 )
	{
		pMsg.Result = CB_INCORRECT_MIX_ITEMS;
		gSendProto.DataSend(lpObj->m_Index, (BYTE*)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = false;
		return;
	}

	RandomItem = rand()%gCherryBlossom.ItemCount[Type];

	LogChaosItem(lpObj, "CherryBlossomMix");
	CLog.LogAdd("[CherryBlossomMix] Chaos Mix Start");
	lpObj->ChaosSuccessRate = 100;

	int ItemType = (gCherryBlossom.ItemType[Type][RandomItem] * 512) + gCherryBlossom.ItemIndex[Type][RandomItem];
	unsigned char Level = rand()%(gCherryBlossom.ItemLevel[Type][RandomItem]+1);
	unsigned char Opt = rand()%(gCherryBlossom.ItemOpt[Type][RandomItem]+1);
	unsigned char Luck = 0;
	unsigned char Skill = 0;
	unsigned char Anc = 0;
	unsigned char Exe = 0;
	unsigned char Dur = 0;
	unsigned char Socket[5];
	Socket[0] = 0xFF;
	Socket[1] = 0xFF;
	Socket[2] = 0xFF;
	Socket[3] = 0xFF;
	Socket[4] = 0xFF;

	if ( (rand()%100) < lpObj->ChaosSuccessRate )
	{
		ItemSerialCreateSend(lpObj->m_Index, -1, 0, 0, ItemType, Level, Dur, Luck, Skill, Opt, -1, Exe, Anc, Socket);
		::gObjInventoryCommit(lpObj->m_Index);

		CLog.LogAdd("[CherryBlossomMix] [%s][%s] CBMix Success %d",lpObj->AccountID, lpObj->Name, lpObj->ChaosSuccessRate);
	}
	else
	{
		gChaosBox.ChaosBoxInit(lpObj);
		::GCUserChaosBoxSend(lpObj, 1);
		gSendProto.DataSend(lpObj->m_Index, (UCHAR*)&pMsg, pMsg.h.size);

		CLog.LogAdd("[CherryBlossomMix] [%s][%s] CBMix Fail %d",lpObj->AccountID, lpObj->Name, lpObj->ChaosSuccessRate);
	}
	lpObj->ChaosLock = false;
}

void SecromiconMix(LPOBJ lpObj)
{
	lpObj->ChaosLock = true;
	char Part1Count = 0;
	char Part2Count = 0;
	char Part3Count = 0;
	char Part4Count = 0;
	char Part5Count = 0;
	char Part6Count = 0;
	char InvalidItemCount = 0;

	
	PMSG_CHAOSMIXRESULT pMsg;

	C1HeadSet((LPBYTE)&pMsg.h, 0x86, sizeof(PMSG_CHAOSMIXRESULT));
	pMsg.Result = CB_ERROR;

	for ( int n=0;n<CHAOS_BOX_SIZE;n++)
	{
		if ( lpObj->pChaosBox[n].IsItem() == true )
		{
			if ( lpObj->pChaosBox[n].m_Type == ITEMGET(14,103) )	// First Secromicon Part
			{
				Part1Count++;
			}
			else if ( lpObj->pChaosBox[n].m_Type == ITEMGET(14,104) )	// Second Secromicon Part
			{
				Part2Count++;
			}
			else if ( lpObj->pChaosBox[n].m_Type == ITEMGET(14,105) )	// Third Secromicon Part
			{
				Part3Count++;
			}
			else if ( lpObj->pChaosBox[n].m_Type == ITEMGET(14,106) )	// Fourth Secromicon Part
			{
				Part4Count++;
			}
			else if ( lpObj->pChaosBox[n].m_Type == ITEMGET(14,107) )	// Fifth Secromicon Part
			{
				Part5Count++;
			}
			else if ( lpObj->pChaosBox[n].m_Type == ITEMGET(14,108) )	// Sixth Secromicon Part
			{
				Part6Count++;
			}
			else
			{
				InvalidItemCount++;
			}
		}
	}

	if ( Part1Count != 1 || Part2Count != 1 || Part3Count != 1 || Part4Count != 1 || Part5Count != 1 || Part6Count != 1 || InvalidItemCount != 0 )
	{
		pMsg.Result = CB_INCORRECT_MIX_ITEMS;
		gSendProto.DataSend(lpObj->m_Index, (BYTE*)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = false;
		return;
	}

	if ( lpObj->Money < 1000000 )
	{
		pMsg.Result = CB_NOT_ENOUGH_ZEN;
		gSendProto.DataSend(lpObj->m_Index, (BYTE *)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = false;
		return;
	}

	lpObj->Money -= 1000000;
	g_CastleSiegeSync.AddTributeMoney(1000000);
	GCMoneySend(lpObj->m_Index, lpObj->Money);

	LogChaosItem(lpObj, "SecromiconMix");
	CLog.LogAdd("[SecromiconMix] Chaos Mix Start");
	lpObj->ChaosSuccessRate = 100;

	int ItemType = ITEMGET(14,109);

	unsigned char Socket[5] = {0xFF,0xFF,0xFF,0xFF,0xFF};

	ItemSerialCreateSend(lpObj->m_Index, -1, 0, 0, ItemType, 0, 0, 0, 0, 0, -1, 0, 0, Socket);
	::gObjInventoryCommit(lpObj->m_Index);

	CLog.LogAdd("[SecromiconMix] [%s][%s] CBMix Success %d",lpObj->AccountID, lpObj->Name, lpObj->ChaosSuccessRate);

	lpObj->ChaosLock = false;
}

void SealedBoxMix(LPOBJ lpObj)
{
	lpObj->ChaosLock = true;
	char SilverKey = 0;
	char SilverSealedBox = 0;
	char GoldenKey = 0;
	char GoldenSealedBox = 0;
	char InvalidItemCount = 0;
	char Type = 0;

	
	PMSG_CHAOSMIXRESULT pMsg;

	C1HeadSet((LPBYTE)&pMsg.h, 0x86, sizeof(PMSG_CHAOSMIXRESULT));
	pMsg.Result = CB_ERROR;

	for ( int n=0;n<CHAOS_BOX_SIZE;n++)
	{
		if ( lpObj->pChaosBox[n].IsItem() == true )
		{
			if ( lpObj->pChaosBox[n].m_Type == ITEMGET(14,112) )	// Silver Key
			{
				SilverKey++;
			}
			else if ( lpObj->pChaosBox[n].m_Type == ITEMGET(14,122) )	// Silver Sealed Box
			{
				SilverSealedBox++;
			}
			else if ( lpObj->pChaosBox[n].m_Type == ITEMGET(14,113) )	// Golden Key
			{
				GoldenKey++;
			}
			else if ( lpObj->pChaosBox[n].m_Type == ITEMGET(14,121) )	// Golden Sealed Box
			{
				GoldenSealedBox++;
			}
			else
			{
				InvalidItemCount++;
			}
		}
	}
	if ( InvalidItemCount != 0 )
	{
		pMsg.Result = CB_INCORRECT_MIX_ITEMS;
		gSendProto.DataSend(lpObj->m_Index, (BYTE*)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = false;
		return;
	}
	if ( SilverKey == 0 && SilverSealedBox == 0 )
	{
		if ( GoldenKey != 1 || GoldenSealedBox != 1 )
		{
			pMsg.Result = CB_INCORRECT_MIX_ITEMS;
			gSendProto.DataSend(lpObj->m_Index, (BYTE*)&pMsg, pMsg.h.size);
			lpObj->ChaosLock = false;
			return;
		}
		else
		{
			Type = 1;
		}
	}
	else if ( GoldenKey == 0 && GoldenSealedBox == 0 )
	{
		if ( SilverKey != 1 || SilverSealedBox != 1 )
		{
			pMsg.Result = CB_INCORRECT_MIX_ITEMS;
			gSendProto.DataSend(lpObj->m_Index, (BYTE*)&pMsg, pMsg.h.size);
			lpObj->ChaosLock = false;
			return;
		}
		else
		{
			Type = 2;
		}
	}
	else
	{
		pMsg.Result = CB_INCORRECT_MIX_ITEMS;
		gSendProto.DataSend(lpObj->m_Index, (BYTE*)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = false;
		return;
	}

	LogChaosItem(lpObj, "SealedBoxMix");
	CLog.LogAdd("[SealedBoxMix] Chaos Mix Start");
	lpObj->ChaosSuccessRate = 100;

	int ItemType = 0;

	if ( Type == 1 )
	{
		ItemType = ITEMGET(14,123);	// Golden Box
	}
	else if ( Type == 2 )
	{
		ItemType = ITEMGET(14,124);	// Silver Box
	}

	unsigned char Socket[5] = {0xFF,0xFF,0xFF,0xFF,0xFF};

	ItemSerialCreateSend(lpObj->m_Index, -1, 0, 0, ItemType, 0, 0, 0, 0, 0, -1, 0, 0, Socket);
	::gObjInventoryCommit(lpObj->m_Index);

	CLog.LogAdd("[SealedBoxMix] [%s][%s] CBMix Success %d",lpObj->AccountID, lpObj->Name, lpObj->ChaosSuccessRate);

	lpObj->ChaosLock = false;
}