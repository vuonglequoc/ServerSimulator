//---------------------------------------------------------------------------
// # Project:		HaRiBO MU GameServer - Supported Season 6
// # Company:		RealCoderZ MU Development © 2011
// # Description:	MU Event Class
// # Developed By:	WolF & M.E.S
//---------------------------------------------------------------------------
#include "../Header Files/stdafx.h"
#include "../Header Files/Event.h"
#include "../Header Files/ItemBag.h"
#include "../Header Files/LogProc.H"
#include "../Header Files/gObjMonster.h"
#include "../Header Files/DSProtocol.h"
#include "../Header Files/Main.H"
#include "../Header Files/GameMain.h"
#include "../Header Files/Functions.H"
#include "../Header Files/Notice.H"
#include "../Header Files/SendProto.H"


void EventChipEventProtocolCore(BYTE protoNum, LPBYTE aRecv, int aLen)
{
	switch ( protoNum )
	{
		case 0x01:
			EGRecvEventChipInfo((PMSG_ANS_VIEW_EC_MN *)aRecv);
			break;
		case 0x02:
			EGResultRegEventChip((PMSG_ANS_REGISTER_EVENTCHIP *)aRecv);
			break;
		case 0x03:
			EGRecvRegMutoNum((PMSG_ANS_REGISTER_MUTONUM *)aRecv);
			break;
		case 0x04:
			EGRecvChangeRena((PMSG_ANS_RESET_EVENTCHIP *)aRecv);
			break;
		case 0x05:
			EGRecvStoneInfo((PMSG_ANS_VIEW_STONES *)aRecv);
			break;
		case 0x06:
			EGRecvRegStone((PMSG_ANS_REGISTER_STONES *)aRecv);
			break;
		case 0x07:
			EGRecvDeleteStone((PMSG_ANS_DELETE_STONES *)aRecv);
			break;
		case 0x09:
			EGRecvChangeStones((PMSG_ANS_RESET_EVENTCHIP *)aRecv);
			break;
		case 0x08:
			EGRecv2AnvRegSerial((PMSG_ANS_2ANIV_SERIAL *)aRecv);
			break;
		case 0x10:
			EGRecvRegRingGift((PMSG_ANS_REG_RINGGIFT *)aRecv);
			break;
		case 0x15:
			EGAnsRegCCOfflineGift((PMSG_ANS_REG_CC_OFFLINE_GIFT *)aRecv);
			break;
		case 0x16:
			EGAnsRegDLOfflineGift((PMSG_ANS_REG_DL_OFFLINE_GIFT *)aRecv);
			break;
		case 0x17:
			EGAnsRegHTOfflineGift((PMSG_ANS_REG_HT_OFFLINE_GIFT *)aRecv);
			break;
	}
}

void DataSendEventChip(PCHAR pMsg, int size)
{
	if ( Connected[5] == FALSE && ::Connected[4] != FALSE )
	{
		wsEvenChipServerCli.Close();
		wsEvenChipServerCli.CreateSocket(gMainApp.m_hWnd);
		if ( GMEventChipServerConnect(::gEventChipServerIp, WM_GM_EVENTCHIP_CLIENT_MSG_PROC) == FALSE )
		{
			Connected[5] = FALSE;
			CLog.LogAdd("Can not connect EventChip Server");

			return;
		}

		::Connected[5] = TRUE;

	}

	if ( ::Connected[5] != FALSE && ::Connected[4] != FALSE )
	{
		::wsEvenChipServerCli.DataSend((char *)pMsg, size);
	}
}

void EGRecvEventChipInfo(PMSG_ANS_VIEW_EC_MN * aRecv)
{
	LPOBJ lpObj = &gObj[aRecv->iINDEX];

	PMSG_EVENTCHIPINFO eventchipeventinfo;

	C1HeadSet((LPBYTE)&eventchipeventinfo, 0x94, sizeof(eventchipeventinfo));
	eventchipeventinfo.Type = 0;
	eventchipeventinfo.ChipCount = aRecv->nEVENT_CHIPS;
	lpObj->EventChipCount = aRecv->nEVENT_CHIPS;
	lpObj->MutoNumber = aRecv->iMUTO_NUM;

	gSendProto.DataSend(lpObj->m_Index, (LPBYTE)&eventchipeventinfo, eventchipeventinfo.h.size);

	lpObj->UseEventServer = FALSE;
}

void EGResultRegEventChip(PMSG_ANS_REGISTER_EVENTCHIP * aRecv)
{
	PMSG_REGEVENTCHIP_RESULT Result;
	LPOBJ lpObj;
	int aIndex;

	C1HeadSet((LPBYTE)&Result, 0x95, sizeof(Result));
	lpObj = &gObj[aRecv->iINDEX];
	aIndex = aRecv->iINDEX;

	if ( aRecv->bSUCCESS == FALSE )
	{
		Result.ChipCount = -1;
		CLog.LogAdd("[EventChip] [%s][%s] RegEventServer Fail (RegEventchip) %d",
			lpObj->AccountID, lpObj->Name, aRecv->Pos);

	}
	else
	{
		Result.ChipCount = aRecv->nEVENT_CHIPS;
		gObjInventoryDeleteItem(aIndex, aRecv->Pos);
		GCInventoryItemDeleteSend(aIndex, aRecv->Pos, 1);

		CLog.LogAdd("[EventChip] [%s][%s] Delete EventChip (%d)",
			lpObj->AccountID, lpObj->Name, aRecv->Pos);
	}

	Result.Type = 0;

	gSendProto.DataSend(aIndex, (LPBYTE)&Result, Result.h.size);

	lpObj->UseEventServer = FALSE;
}

void EGRecvRegMutoNum( PMSG_ANS_REGISTER_MUTONUM* aRecv)
{
	LPOBJ lpObj;
	int aIndex;
	

	lpObj = &gObj[aRecv->iINDEX];
	aIndex = aRecv->iINDEX;

	PMSG_GETMUTONUMBER_RESULT Result;

	C1HeadSet((LPBYTE)&Result, 0x96, sizeof(Result));
	
	if ( gObjFind10EventChip(aIndex) == FALSE )
	{
		Result.MutoNum[0] = -1;
		Result.MutoNum[1] = 0;
		Result.MutoNum[2] = 0;

		gSendProto.DataSend(aIndex, (LPBYTE)&Result, Result.h.size);

		lpObj->UseEventServer = FALSE;

		return;
	}

	gObjDelete10EventChip(aIndex);
	Result.MutoNum[0] = aRecv->iMUTO_NUM / 1000000;
	Result.MutoNum[1] = aRecv->iMUTO_NUM / 1000 - aRecv->iMUTO_NUM / 1000000 * 1000;
	Result.MutoNum[2] = aRecv->iMUTO_NUM % 1000;
	lpObj->MutoNumber = aRecv->iMUTO_NUM;
	
	CLog.LogAdd("[EventChip] [%s][%s] Make MutoNumber %d,%d,%d",
		lpObj->AccountID, lpObj->Name, 
		Result.MutoNum[0], Result.MutoNum[1], Result.MutoNum[2]);

	gSendProto.DataSend(aIndex, (LPBYTE)&Result, Result.h.size);
	lpObj->UseEventServer = FALSE;
}

void EGRecvChangeRena( PMSG_ANS_RESET_EVENTCHIP* aRecv)
{
	PMSG_REGEVENTCHIP_RESULT Result;
	LPOBJ lpObj;
	int aIndex;

	C1HeadSet((LPBYTE)&Result, 0x95, sizeof(Result));
	lpObj = &gObj[aRecv->iINDEX];
	aIndex = aRecv->iINDEX;

	if ( aRecv->bSUCCESS != FALSE )
	{
		lpObj->Money += lpObj->EventChipCount * 3000;
		GCMoneySend(aIndex, lpObj->Money);

		CLog.LogAdd("[EventChip] [%s][%s] ChangeRena AddMoney(%d)",
			lpObj->AccountID, lpObj->Name, lpObj->EventChipCount * 3000);
	}
	else
	{
		CLog.LogAdd("[EventChip] [%s][%s] ChangeRena Fail",
			lpObj->AccountID, lpObj->Name);
	}

	Result.ChipCount = 0;
	lpObj->EventChipCount = 0;

	gSendProto.DataSend(aIndex, (LPBYTE)&Result, Result.h.size);

	lpObj->UseEventServer = FALSE;
}

LPOBJ pEventObj;

void EGRecvStoneInfo( PMSG_ANS_VIEW_STONES* aRecv)
{
	LPOBJ lpObj = &gObj[aRecv->iINDEX];

	PMSG_EVENTCHIPINFO Result;
	
	C1HeadSet((LPBYTE)&Result, 0x94, sizeof(Result));

	if ( aRecv->bSUCCESS )
		lpObj->iStoneCount = aRecv->iStoneCount;
	else
		lpObj->iStoneCount = 0;

	lpObj->MutoNumber = 0;
	Result.Type = 3;
	Result.ChipCount = aRecv->iStoneCount;

	gSendProto.DataSend(lpObj->m_Index, (LPBYTE)&Result, Result.h.size);

	char msg[128];
	wsprintf(msg, lMsg.Get(MSGGET(4, 153)), Result.ChipCount);
	ChatTargetSend(pEventObj, msg, lpObj->m_Index);
}

void EGRecvRegStone( PMSG_ANS_REGISTER_STONES* aRecv)
{
	PMSG_REGEVENTCHIP_RESULT Result;
	LPOBJ lpObj;
	int aIndex;
	

	C1HeadSet((LPBYTE)&Result, 0x95, sizeof(Result));
	lpObj = &gObj[aRecv->iINDEX];
	aIndex = aRecv->iINDEX;

	if ( aRecv->bSUCCESS != FALSE )
	{
		Result.ChipCount = aRecv->iStoneCount;
		gObjInventoryDeleteItem(aIndex, aRecv->iPosition);
		GCInventoryItemDeleteSend(aIndex, aRecv->iPosition, 1);

		CLog.LogAdd("[Stone] [%s][%s] Delete Stones",
			lpObj->AccountID, lpObj->Name);
	}
	else
	{
		Result.ChipCount = -1;
		
		CLog.LogAdd("[Stone] [%s][%s] RegEventServer Fail (Stones : %d)",
			lpObj->AccountID, lpObj->Name, aRecv->iStoneCount);
	}

	gSendProto.DataSend(aIndex, (LPBYTE)&Result, Result.h.size);

	lpObj->UseEventServer = FALSE;
}

void EGRecvDeleteStone( PMSG_ANS_DELETE_STONES* aRecv)
{
	return;
}



void EGRecvChangeStones( PMSG_ANS_RESET_EVENTCHIP* aRecv)
{
	PMSG_REGEVENTCHIP_RESULT Result;
	LPOBJ lpObj;
	int aIndex;
	

	C1HeadSet((LPBYTE)&Result, 0x95, sizeof(Result));
	lpObj = &gObj[aRecv->iINDEX];
	aIndex = aRecv->iINDEX;

	if ( aRecv->bSUCCESS != FALSE )
	{
		lpObj->Money += lpObj->iStoneCount * 3000;
		GCMoneySend(aIndex, lpObj->Money);

		CLog.LogAdd("[Stones] [%s][%s] ChangeRena AddMoney(%d)",
			lpObj->AccountID, lpObj->Name, lpObj->iStoneCount*3000);
	}
	else
	{
		CLog.LogAdd("[Stones] [%s][%s] ChangeRena Fail",
			lpObj->AccountID, lpObj->Name);
	}

	Result.ChipCount = 0;
	lpObj->iStoneCount = 0;

	gSendProto.DataSend(aIndex, (LPBYTE)&Result, Result.h.size);

	lpObj->UseEventServer = FALSE;
}

struct PMSG_ANS_2ANV_LOTTO_EVENT
{
	PBMSG_HEAD h;	// C1:9D
	BYTE btIsRegistered;	// 3
	char szGIFT_NAME[64];	// 4
};

#define GIFT_2ANV_MAP	235
#define MAX_GIFT_2ANV	50
#define GIFT_2ANV_RANGE(x)  ( (((x))<0)?FALSE:(((x))>MAX_GIFT_2ANV-1)?FALSE:TRUE )


BOOL g_bRingEventItemTextLoad = FALSE;
char g_sz2ANV_GIFT_NAME[MAX_GIFT_2ANV][64];

void EGRecv2AnvRegSerial( PMSG_ANS_2ANIV_SERIAL* aRecv)
{
	PMSG_ANS_2ANV_LOTTO_EVENT Result;

	C1HeadSet((LPBYTE)&Result, 0x9D, sizeof(Result));

	if ( !OBJMAX_RANGE(aRecv->iINDEX))
	{
		CLog.LogAdd("[Mu_2Anv_Event] Error : Index is out of bound [%d]", aRecv->iINDEX);
		return;
	}

	if ( gObj[aRecv->iINDEX].Connected <= PLAYER_LOGGED )
	{
		CLog.LogAdd("[Mu_2Anv_Event] Error : Index is out of bound [%d]", aRecv->iINDEX);
		return;
	}

	Result.szGIFT_NAME[0] = 0;

	if ( aRecv->btIsRegistered == FALSE )
	{
		Result.btIsRegistered = FALSE;
		
		if ( !GIFT_2ANV_RANGE(aRecv->iGiftNumber-1) )
		{
			CLog.LogAdd("[Mu_2Anv_Event] Error : Gift Index is out of bound [%d]", aRecv->iGiftNumber);
			Result.btIsRegistered = 2;
		}

		if ( gObj[aRecv->iINDEX].Connected > PLAYER_LOGGED )
		{
			PMSG_SERVERCMD ServerCmd;

			C1SubHeadSet((LPBYTE)&ServerCmd, 0xF3, 0x40, sizeof(ServerCmd));
			ServerCmd.CmdType = 0;
			ServerCmd.X = gObj[aRecv->iINDEX].X;
			ServerCmd.Y = gObj[aRecv->iINDEX].Y;

			gSendProto.VPMsgSend(&gObj[aRecv->iINDEX], (LPBYTE)&ServerCmd, sizeof(ServerCmd));
			gSendProto.DataSend(aRecv->iINDEX, (LPBYTE)&ServerCmd, sizeof(ServerCmd));
		}

		if ( g_bRingEventItemTextLoad )
		{
			strcpy(Result.szGIFT_NAME, g_sz2ANV_GIFT_NAME[aRecv->iGiftNumber-1]);
		}

		BYTE Socket[5];
		Socket[0] = 0xFF;
		Socket[1] = 0xFF;
		Socket[2] = 0xFF;
		Socket[3] = 0xFF;
		Socket[4] = 0xFF;

		switch ( aRecv->iGiftNumber )
		{
			case 1:
				ItemSerialCreateSend(gObj[aRecv->iINDEX].m_Index, GIFT_2ANV_MAP, 
					gObj[aRecv->iINDEX].X, gObj[aRecv->iINDEX].Y,
					ItemGetNumberMake(14, 13),
					0, 0, 0, 0, 0,
					aRecv->iINDEX,
					0, 0, Socket);

				if ( g_bRingEventItemTextLoad == FALSE )
				{
					strcpy(Result.szGIFT_NAME, ItemAttribute[ITEMGET(14, 13)].Name);
				}
				break;
			case 2:
				ItemSerialCreateSend(gObj[aRecv->iINDEX].m_Index, GIFT_2ANV_MAP, 
					gObj[aRecv->iINDEX].X, gObj[aRecv->iINDEX].Y,
					ItemGetNumberMake(14, 14),
					0, 0, 0, 0, 0,
					aRecv->iINDEX,
					0, 0, Socket);

				if ( g_bRingEventItemTextLoad == FALSE )
				{
					strcpy(Result.szGIFT_NAME, ItemAttribute[ITEMGET(14, 14)].Name);
				}
				break;
			case 3:
				ItemSerialCreateSend(gObj[aRecv->iINDEX].m_Index, GIFT_2ANV_MAP, 
					gObj[aRecv->iINDEX].X, gObj[aRecv->iINDEX].Y,
					ItemGetNumberMake(12, 15),
					0, 0, 0, 0, 0,
					aRecv->iINDEX,
					0, 0, Socket);

				if ( g_bRingEventItemTextLoad == FALSE )
				{
					strcpy(Result.szGIFT_NAME, ItemAttribute[ITEMGET(12, 15)].Name);
				}
				break;
			case 4:
				ItemSerialCreateSend(gObj[aRecv->iINDEX].m_Index, GIFT_2ANV_MAP, 
					gObj[aRecv->iINDEX].X, gObj[aRecv->iINDEX].Y,
					ItemGetNumberMake(14, 11),
					0, 0, 0, 0, 0,
					aRecv->iINDEX,
					0, 0, Socket);

				if ( g_bRingEventItemTextLoad == FALSE )
				{
					strcpy(Result.szGIFT_NAME, ItemAttribute[ITEMGET(14, 11)].Name);
				}
				break;
			case 5:
				ItemSerialCreateSend(gObj[aRecv->iINDEX].m_Index, GIFT_2ANV_MAP, 
					gObj[aRecv->iINDEX].X, gObj[aRecv->iINDEX].Y,
					ItemGetNumberMake(14, 11),
					3, 0, 0, 0, 0,
					aRecv->iINDEX,
					0, 0, Socket);

				if ( g_bRingEventItemTextLoad == FALSE )
				{
					strcpy(Result.szGIFT_NAME, "Heart of Love");
				}
				break;
			case 6:
				gObj[aRecv->iINDEX].Money += 500000;
				GCMoneySend(aRecv->iINDEX, gObj[aRecv->iINDEX].Money);

				if ( g_bRingEventItemTextLoad == FALSE )
				{
					strcpy(Result.szGIFT_NAME, "500,000 Zen");
				}
				break;
			case 7:
				gObj[aRecv->iINDEX].Money += 50000;
				GCMoneySend(aRecv->iINDEX, gObj[aRecv->iINDEX].Money);

				if ( g_bRingEventItemTextLoad == FALSE )
				{
					strcpy(Result.szGIFT_NAME, "50,000 Zen");
				}
				break;
			case 8:	case 9:	case 10:
				gObj[aRecv->iINDEX].Money += 30000;
				GCMoneySend(aRecv->iINDEX, gObj[aRecv->iINDEX].Money);

				if ( g_bRingEventItemTextLoad == FALSE )
				{
					strcpy(Result.szGIFT_NAME, "30,000 Zen");
				}
				break;
			case 11:
				ItemSerialCreateSend(gObj[aRecv->iINDEX].m_Index, GIFT_2ANV_MAP, 
					gObj[aRecv->iINDEX].X, gObj[aRecv->iINDEX].Y,
					ItemGetNumberMake(12, 19),
					0, 0, 0, 0, 0,
					aRecv->iINDEX,
					0, 0, Socket);

				if ( g_bRingEventItemTextLoad == FALSE )
				{
					strcpy(Result.szGIFT_NAME, ItemAttribute[ITEMGET(12, 19)].Name);	// #error Change 11 to 19
				}
				break;
			case 12:
				ItemSerialCreateSend(gObj[aRecv->iINDEX].m_Index, GIFT_2ANV_MAP, 
					gObj[aRecv->iINDEX].X, gObj[aRecv->iINDEX].Y,
					ItemGetNumberMake(12, 18),
					0, 0, 0, 0, 0,
					aRecv->iINDEX,
					0, 0, Socket);

				if ( g_bRingEventItemTextLoad == FALSE )
				{
					strcpy(Result.szGIFT_NAME, ItemAttribute[ITEMGET(12, 18)].Name);
				}
				break;
			case 13:
				ItemSerialCreateSend(gObj[aRecv->iINDEX].m_Index, GIFT_2ANV_MAP, 
					gObj[aRecv->iINDEX].X, gObj[aRecv->iINDEX].Y,
					ItemGetNumberMake(12, 17),
					0, 0, 0, 0, 0,
					aRecv->iINDEX,
					0, 0, Socket);

				if ( g_bRingEventItemTextLoad == FALSE )
				{
					strcpy(Result.szGIFT_NAME, ItemAttribute[ITEMGET(12, 17)].Name);
				}
				break;
			case 14:
				ItemSerialCreateSend(gObj[aRecv->iINDEX].m_Index, GIFT_2ANV_MAP, 
					gObj[aRecv->iINDEX].X, gObj[aRecv->iINDEX].Y,
					ItemGetNumberMake(12, 16),
					0, 0, 0, 0, 0,
					aRecv->iINDEX,
					0, 0, Socket);

				if ( g_bRingEventItemTextLoad == FALSE )
				{
					strcpy(Result.szGIFT_NAME, ItemAttribute[ITEMGET(12, 16)].Name);
				}
				break;
			case 15:
				ItemSerialCreateSend(gObj[aRecv->iINDEX].m_Index, GIFT_2ANV_MAP, 
					gObj[aRecv->iINDEX].X, gObj[aRecv->iINDEX].Y,
					ItemGetNumberMake(12, 14),
					0, 0, 0, 0, 0,
					aRecv->iINDEX,
					0, 0, Socket);

				if ( g_bRingEventItemTextLoad == FALSE )
				{
					strcpy(Result.szGIFT_NAME, ItemAttribute[ITEMGET(12, 14)].Name);
				}
				break;
			case 16:
				ItemSerialCreateSend(gObj[aRecv->iINDEX].m_Index, GIFT_2ANV_MAP, 
					gObj[aRecv->iINDEX].X, gObj[aRecv->iINDEX].Y,
					ItemGetNumberMake(12, 13),
					0, 0, 0, 0, 0,
					aRecv->iINDEX,
					0, 0, Socket);

				if ( g_bRingEventItemTextLoad == FALSE )
				{
					strcpy(Result.szGIFT_NAME, ItemAttribute[ITEMGET(12, 13)].Name);
				}
				break;
			case 17:
				ItemSerialCreateSend(gObj[aRecv->iINDEX].m_Index, GIFT_2ANV_MAP, 
					gObj[aRecv->iINDEX].X, gObj[aRecv->iINDEX].Y,
					ItemGetNumberMake(12, 12),
					0, 0, 0, 0, 0,
					aRecv->iINDEX,
					0, 0, Socket);

				if ( g_bRingEventItemTextLoad == FALSE )
				{
					strcpy(Result.szGIFT_NAME, ItemAttribute[ITEMGET(12, 12)].Name);
				}
				break;
			case 18:
				ItemSerialCreateSend(gObj[aRecv->iINDEX].m_Index, GIFT_2ANV_MAP, 
					gObj[aRecv->iINDEX].X, gObj[aRecv->iINDEX].Y,
					ItemGetNumberMake(13, 2),
					0, 255, 0, 0, 0,
					aRecv->iINDEX,
					0, 0, Socket);

				if ( g_bRingEventItemTextLoad == FALSE )
				{
					strcpy(Result.szGIFT_NAME, ItemAttribute[ITEMGET(13, 2)].Name);
				}
				break;
			case 19:
				ItemSerialCreateSend(gObj[aRecv->iINDEX].m_Index, GIFT_2ANV_MAP, 
					gObj[aRecv->iINDEX].X, gObj[aRecv->iINDEX].Y,
					ItemGetNumberMake(13, 3),
					0, 255, 0, 0, 0,
					aRecv->iINDEX,
					0, 0, Socket);

				if ( g_bRingEventItemTextLoad == FALSE )
				{
					strcpy(Result.szGIFT_NAME, ItemAttribute[ITEMGET(13, 3)].Name);
				}
				break;
			case 20:
				ItemSerialCreateSend(gObj[aRecv->iINDEX].m_Index, GIFT_2ANV_MAP, 
					gObj[aRecv->iINDEX].X, gObj[aRecv->iINDEX].Y,
					ItemGetNumberMake(14, 16),
					0, 0, 0, 0, 0,
					aRecv->iINDEX,
					0, 0, Socket);

				if ( g_bRingEventItemTextLoad == FALSE )
				{
					strcpy(Result.szGIFT_NAME, ItemAttribute[ITEMGET(14, 16)].Name);
				}
				break;
			case 21:
				ItemSerialCreateSend(gObj[aRecv->iINDEX].m_Index, GIFT_2ANV_MAP, 
					gObj[aRecv->iINDEX].X, gObj[aRecv->iINDEX].Y,
					ItemGetNumberMake(13, 0),
					0, 255, 0, 0, 0,
					aRecv->iINDEX,
					0, 0, Socket);

				if ( g_bRingEventItemTextLoad == FALSE )
				{
					strcpy(Result.szGIFT_NAME, ItemAttribute[ITEMGET(13, 0)].Name);
				}
				break;
			case 22:
				ItemSerialCreateSend(gObj[aRecv->iINDEX].m_Index, GIFT_2ANV_MAP, 
					gObj[aRecv->iINDEX].X, gObj[aRecv->iINDEX].Y,
					ItemGetNumberMake(13, 1),
					0, 255, 0, 0, 0,
					aRecv->iINDEX,
					0, 0, Socket);

				if ( g_bRingEventItemTextLoad == FALSE )
				{
					strcpy(Result.szGIFT_NAME, ItemAttribute[ITEMGET(13, 1)].Name);
				}
				break;
			case 23:
				ItemSerialCreateSend(gObj[aRecv->iINDEX].m_Index, GIFT_2ANV_MAP, 
					gObj[aRecv->iINDEX].X, gObj[aRecv->iINDEX].Y,
					ItemGetNumberMake(14, 11),
					1, 0, 0, 0, 0,
					aRecv->iINDEX,
					0, 0, Socket);

				if ( g_bRingEventItemTextLoad == FALSE )
				{
					strcpy(Result.szGIFT_NAME, ItemAttribute[ITEMGET(14, 11)].Name);
				}
				break;
			case 24:
				ItemSerialCreateSend(gObj[aRecv->iINDEX].m_Index, GIFT_2ANV_MAP, 
					gObj[aRecv->iINDEX].X, gObj[aRecv->iINDEX].Y,
					ItemGetNumberMake(14, 11),
					9, 0, 0, 0, 0,
					aRecv->iINDEX,
					0, 0, Socket);

				if ( g_bRingEventItemTextLoad == FALSE )
				{
					strcpy(Result.szGIFT_NAME, ItemAttribute[ITEMGET(14, 11)].Name);
				}
				break;
			case 25:
				ItemSerialCreateSend(gObj[aRecv->iINDEX].m_Index, GIFT_2ANV_MAP, 
					gObj[aRecv->iINDEX].X, gObj[aRecv->iINDEX].Y,
					ItemGetNumberMake(14, 11),
					10, 0, 0, 0, 0,
					aRecv->iINDEX,
					0, 0, Socket);

				if ( g_bRingEventItemTextLoad == FALSE )
				{
					strcpy(Result.szGIFT_NAME, ItemAttribute[ITEMGET(14, 11)].Name);
				}
				break;
			case 26:
				ItemSerialCreateSend(gObj[aRecv->iINDEX].m_Index, GIFT_2ANV_MAP, 
					gObj[aRecv->iINDEX].X, gObj[aRecv->iINDEX].Y,
					ItemGetNumberMake(14, 19),
					0, 0, 0, 0, 0,
					aRecv->iINDEX,
					0, 0, Socket);

				if ( g_bRingEventItemTextLoad == FALSE )
				{
					strcpy(Result.szGIFT_NAME, ItemAttribute[ITEMGET(14, 19)].Name);
				}
				break;
			case 27:
				ItemSerialCreateSend(gObj[aRecv->iINDEX].m_Index, GIFT_2ANV_MAP, 
					gObj[aRecv->iINDEX].X, gObj[aRecv->iINDEX].Y,
					ItemGetNumberMake(14, 19),
					1, 0, 0, 0, 0,
					aRecv->iINDEX,
					0, 0, Socket);

				if ( g_bRingEventItemTextLoad == FALSE )
				{
					strcpy(Result.szGIFT_NAME, ItemAttribute[ITEMGET(14, 19)].Name);
				}
				break;
			case 28:
				ItemSerialCreateSend(gObj[aRecv->iINDEX].m_Index, GIFT_2ANV_MAP, 
					gObj[aRecv->iINDEX].X, gObj[aRecv->iINDEX].Y,
					ItemGetNumberMake(14, 19),
					2, 0, 0, 0, 0,
					aRecv->iINDEX,
					0, 0, Socket);

				if ( g_bRingEventItemTextLoad == FALSE )
				{
					strcpy(Result.szGIFT_NAME, ItemAttribute[ITEMGET(14, 19)].Name);
				}
				break;
			case 29:
				ItemSerialCreateSend(gObj[aRecv->iINDEX].m_Index, GIFT_2ANV_MAP, 
					gObj[aRecv->iINDEX].X, gObj[aRecv->iINDEX].Y,
					ItemGetNumberMake(14, 19),
					3, 0, 0, 0, 0,
					aRecv->iINDEX,
					0, 0, Socket);

				if ( g_bRingEventItemTextLoad == FALSE )
				{
					strcpy(Result.szGIFT_NAME, ItemAttribute[ITEMGET(14, 19)].Name);
				}
				break;
			case 30:
				ItemSerialCreateSend(gObj[aRecv->iINDEX].m_Index, GIFT_2ANV_MAP, 
					gObj[aRecv->iINDEX].X, gObj[aRecv->iINDEX].Y,
					ItemGetNumberMake(14, 19),
					4, 0, 0, 0, 0,
					aRecv->iINDEX,
					0, 0, Socket);

				if ( g_bRingEventItemTextLoad == FALSE )
				{
					strcpy(Result.szGIFT_NAME, ItemAttribute[ITEMGET(14, 19)].Name);
				}
				break;
			case 31:
				ItemSerialCreateSend(gObj[aRecv->iINDEX].m_Index, GIFT_2ANV_MAP, 
					gObj[aRecv->iINDEX].X, gObj[aRecv->iINDEX].Y,
					ItemGetNumberMake(14, 11),
					2, 0, 0, 0, 0,
					aRecv->iINDEX,
					0, 0, Socket);

				if ( g_bRingEventItemTextLoad == FALSE )
				{
					strcpy(Result.szGIFT_NAME, ItemAttribute[ITEMGET(14, 11)].Name);
				}
				break;
			case 32:
				ItemSerialCreateSend(gObj[aRecv->iINDEX].m_Index, GIFT_2ANV_MAP, 
					gObj[aRecv->iINDEX].X, gObj[aRecv->iINDEX].Y,
					ItemGetNumberMake(14, 20),
					0, 0, 0, 0, 0,
					aRecv->iINDEX,
					0, 0, Socket);

				if ( g_bRingEventItemTextLoad == FALSE )
				{
					strcpy(Result.szGIFT_NAME, ItemAttribute[ITEMGET(14, 20)].Name);
				}
				break;
			case 33:
				ItemSerialCreateSend(gObj[aRecv->iINDEX].m_Index, GIFT_2ANV_MAP, 
					gObj[aRecv->iINDEX].X, gObj[aRecv->iINDEX].Y,
					ItemGetNumberMake(14, 22),
					0, 0, 0, 0, 0,
					aRecv->iINDEX,
					0, 0, Socket);

				if ( g_bRingEventItemTextLoad == FALSE )
				{
					strcpy(Result.szGIFT_NAME, ItemAttribute[ITEMGET(14, 22)].Name);
				}
				break;
			case 34:	case 35:	case 36:	case 37:
				ItemSerialCreateSend(gObj[aRecv->iINDEX].m_Index, GIFT_2ANV_MAP, 
					gObj[aRecv->iINDEX].X, gObj[aRecv->iINDEX].Y,
					ItemGetNumberMake(13, 15),
					aRecv->iGiftNumber - 34, 0, 0, 0, 0,
					aRecv->iINDEX,
					0, 0, Socket);

				if ( g_bRingEventItemTextLoad == FALSE )
				{
					strcpy(Result.szGIFT_NAME, ItemAttribute[ITEMGET(13, 15)].Name);
				}
				break;
			case 38:	case 39:
				ItemSerialCreateSend(gObj[aRecv->iINDEX].m_Index, GIFT_2ANV_MAP, 
					gObj[aRecv->iINDEX].X, gObj[aRecv->iINDEX].Y,
					ItemGetNumberMake(14, 11),
					aRecv->iGiftNumber - 27, 0, 0, 0, 0,
					aRecv->iINDEX,
					0, 0, Socket);

				if ( g_bRingEventItemTextLoad == FALSE )
				{
					strcpy(Result.szGIFT_NAME, ItemAttribute[ITEMGET(14, 11)].Name);
				}
				break;
			case 40:	case 41:	case 42:	case 43:	case 44:
				ItemSerialCreateSend(gObj[aRecv->iINDEX].m_Index, GIFT_2ANV_MAP, 
					gObj[aRecv->iINDEX].X, gObj[aRecv->iINDEX].Y,
					ItemGetNumberMake(14, 20),
					aRecv->iGiftNumber - 39, 0, 0, 0, 0,
					aRecv->iINDEX,
					0, 0, Socket);

				if ( g_bRingEventItemTextLoad == FALSE )
				{
					strcpy(Result.szGIFT_NAME, ItemAttribute[ITEMGET(14, 20)].Name);
				}
				break;
			case 45:
				ItemSerialCreateSend(gObj[aRecv->iINDEX].m_Index, GIFT_2ANV_MAP, 
					gObj[aRecv->iINDEX].X, gObj[aRecv->iINDEX].Y,
					ItemGetNumberMake(14, 11),
					8, 0, 0, 0, 0,
					aRecv->iINDEX,
					0, 0, Socket);

				if ( g_bRingEventItemTextLoad == FALSE )
				{
					strcpy(Result.szGIFT_NAME, ItemAttribute[ITEMGET(14, 11)].Name);
				}
				break;
			case 46:
				ItemSerialCreateSend(gObj[aRecv->iINDEX].m_Index, GIFT_2ANV_MAP, 
					gObj[aRecv->iINDEX].X, gObj[aRecv->iINDEX].Y,
					ItemGetNumberMake(14, 41),
					0, 0, 0, 0, 0,
					aRecv->iINDEX,
					0, 0, Socket);

				if ( g_bRingEventItemTextLoad == FALSE )
				{
					strcpy(Result.szGIFT_NAME, ItemAttribute[ITEMGET(14, 41)].Name);
				}
				break;
			case 47:
				ItemSerialCreateSend(gObj[aRecv->iINDEX].m_Index, GIFT_2ANV_MAP, 
					gObj[aRecv->iINDEX].X, gObj[aRecv->iINDEX].Y,
					ItemGetNumberMake(14, 42),
					0, 0, 0, 0, 0,
					aRecv->iINDEX,
					0, 0, Socket);

				if ( g_bRingEventItemTextLoad == FALSE )
				{
					strcpy(Result.szGIFT_NAME, ItemAttribute[ITEMGET(14, 42)].Name);
				}
				break;
			case 48:
				ItemSerialCreateSend(gObj[aRecv->iINDEX].m_Index, GIFT_2ANV_MAP, 
					gObj[aRecv->iINDEX].X, gObj[aRecv->iINDEX].Y,
					ItemGetNumberMake(14, 44),
					0, 0, 0, 0, 0,
					aRecv->iINDEX,
					0, 0, Socket);

				if ( g_bRingEventItemTextLoad == FALSE )
				{
					strcpy(Result.szGIFT_NAME, ItemAttribute[ITEMGET(14, 44)].Name);
				}
				break;
			case 49:
				ItemSerialCreateSend(gObj[aRecv->iINDEX].m_Index, GIFT_2ANV_MAP, 
					gObj[aRecv->iINDEX].X, gObj[aRecv->iINDEX].Y,
					ItemGetNumberMake(14, 43),
					0, 0, 0, 0, 0,
					aRecv->iINDEX,
					0, 0, Socket);

				if ( g_bRingEventItemTextLoad == FALSE )
				{
					strcpy(Result.szGIFT_NAME, ItemAttribute[ITEMGET(14, 43)].Name);
				}
				break;
			case 50:
				ItemSerialCreateSend(gObj[aRecv->iINDEX].m_Index, GIFT_2ANV_MAP, 
					gObj[aRecv->iINDEX].X, gObj[aRecv->iINDEX].Y,
					ItemGetNumberMake(14, 31),
					0, 0, 0, 0, 0,
					aRecv->iINDEX,
					0, 0, Socket);

				if ( g_bRingEventItemTextLoad == FALSE )
				{
					strcpy(Result.szGIFT_NAME, ItemAttribute[ITEMGET(14, 31)].Name);
				}
				break;
			default:
				CLog.LogAdd("[Mu_2Anv_Event] Error : iGiftNumber is Out of Boud [%d]", aRecv->iGiftNumber);
				break;
		}
	}
	else if ( aRecv->btIsRegistered == 1 ||
			  aRecv->btIsRegistered == 2 ||
			  aRecv->btIsRegistered == 3 ||
			  aRecv->btIsRegistered == 4 ||
			  aRecv->btIsRegistered == 5 )
	{
		Result.btIsRegistered = aRecv->btIsRegistered;
	}
	else
	{
		Result.btIsRegistered = 4;
		CLog.LogAdd("[Mu_2Anv_Event] Error : Result Value is Wrong [%d]", aRecv->btIsRegistered);
	}

	CLog.LogAdd("[Mu_2Anv_Event] Register Serial Result : %d [%s][%s]",
		aRecv->btIsRegistered, gObj[aRecv->iINDEX].AccountID, gObj[aRecv->iINDEX].Name);

	gSendProto.DataSend(aRecv->iINDEX, (LPBYTE)&Result, Result.h.size);

	gObj[aRecv->iINDEX].UseEventServer = FALSE;
}

static const char g_szRingEventOfflineGift[4][32] = { "100µ· ¹Â¹ÝÁö",
													  "10µ· ¹Â¹ÝÁö",
													  "5µ· ¹Â¹ÝÁö",
													  "2µ· ¹Â¹ÝÁö"};
															
void EGRecvRegRingGift( PMSG_ANS_REG_RINGGIFT* aRecv)
{
	gObj[aRecv->iINDEX].UseEventServer = FALSE;

	if ( gObjIsConnected(aRecv->iINDEX) == FALSE )
		return;

	if ( strcmp(aRecv->szUID, gObj[aRecv->iINDEX].AccountID))
		return;

	if ( aRecv->btIsRegistered == 1 )
	{
		if ( CHECK_LIMIT(aRecv->btGiftKind-1, 4) )
		{
			char szTemp[256];
			wsprintf(szTemp, "%s´Ô ²²¼­ %s¿¡ ´çÃ·µÇ¼Ì½À´Ï´Ù.",
				gObj[aRecv->iINDEX].Name, g_szRingEventOfflineGift[aRecv->btGiftKind-1]);

			SendMsg.NormalMsgOut(OBJ_EMPTY, OBJ_SEND_ALL_PLYRS, NO_SPC_MAP, szTemp);

			CLog.LogAdd("[Ring Event] [%s][%s] Register Succeeded Result:%d, Gift:%d",
				gObj[aRecv->iINDEX].AccountID, gObj[aRecv->iINDEX].Name,
				aRecv->btIsRegistered, aRecv->btGiftKind);
		}
		else
		{
			CLog.LogAdd("[Ring Event] [%s][%s] Register Failed Result:%d, Gift:%d (out of bound, 1~4)",
				gObj[aRecv->iINDEX].AccountID, gObj[aRecv->iINDEX].Name,
				aRecv->btIsRegistered, aRecv->btGiftKind);
		}

		return;
	}

	CLog.LogAdd("[Ring Event] [%s][%s] Register Failed Result : %d",
		gObj[aRecv->iINDEX].AccountID, gObj[aRecv->iINDEX].Name,
		aRecv->btIsRegistered);

	if ( gObjIsConnected(aRecv->iINDEX) == TRUE )
	{
		MapC[gObj[aRecv->iINDEX].MapNumber].MoneyItemDrop(100000, (BYTE)gObj[aRecv->iINDEX].X, (BYTE)gObj[aRecv->iINDEX].Y);
	}
}

struct PMSG_REQ_BLOODCASTLE_ENTERCOUNT
{
	PBMSG_HEAD h;	// C1:0B
	char AccountID[10];	// 3
	char GameID[10];	// D
	int ServerCode;	// 18
	int iObjIndex;	// 1C
};

void EGReqBloodCastleEnterCount(int iIndex)
{
	if ( !gObjIsConnected(iIndex) )
		return;

	PMSG_REQ_BLOODCASTLE_ENTERCOUNT pMsg;

	pMsg.h.c = 0xC1;
	pMsg.h.headcode = 0x0B;
	pMsg.h.size = sizeof(pMsg);
	memcpy(pMsg.AccountID, gObj[iIndex].AccountID, 10);
	memcpy(pMsg.GameID, gObj[iIndex].Name, 10);
	pMsg.ServerCode = gGameServerCode / 20;
	pMsg.iObjIndex = iIndex;

	if ( !Connected[3] && !Connected[2] )
	{
		wsRServerCli.Close();
		wsRServerCli.CreateSocket(gMainApp.m_hWnd);

		if ( !GMRankingServerConnect(gDevilSquareEventServerIp, WM_GM_RANKING_CLIENT_MSG_PROC) )
		{
			Connected[3] = 0;
			CLog.LogAdd("Can not connect Ranking Server");
			return;
		}

		Connected[3] = TRUE;
	}

	if ( !Connected[2] && Connected[3] )
	{
		wsRServerCli.DataSend((PCHAR)&pMsg, pMsg.h.size);
	}
}

struct PMSG_ANS_CL_ENTERCOUNT
{
	PBMSG_HEAD h;	// C1:9F
	BYTE btEventType;	// 3
	BYTE btLeftEnterCount;	// 4
};

void EGAnsBloodCastleEnterCount( PMSG_ANS_BLOODCASTLE_ENTERCOUNT* lpMsg)
{
	if ( !lpMsg)
		return;

	if ( !gObjIsConnected(lpMsg->iObjIndex))
		return;

	char szAccountID[11] = {0};
	char szName[11] = {0};
	memcpy(szAccountID, lpMsg->AccountID, 10);
	memcpy(szName, lpMsg->GameID, 10);

	if ( strcmp(gObj[lpMsg->iObjIndex].AccountID, szAccountID) ||
		 strcmp(gObj[lpMsg->iObjIndex].Name, szName) )
		 return;

	PMSG_ANS_CL_ENTERCOUNT pMsgSend;

	pMsgSend.h.c = 0xC1;
	pMsgSend.h.headcode = 0x9F;
	pMsgSend.h.size = sizeof(pMsgSend);
	pMsgSend.btEventType = 2;
	pMsgSend.btLeftEnterCount = lpMsg->iLeftCount;

	gSendProto.DataSend(lpMsg->iObjIndex, (LPBYTE)&pMsgSend, sizeof(pMsgSend));
}

struct PMSG_REQ_REG_CC_OFFLINE_GIFT
{
	PBMSG_HEAD h;	// C1:15
	int iINDEX;	// 4
	char szUID[11];	// 8
	WORD wServerCode;	// 14
	char szNAME[11];	// 16
};

void EGReqRegCCOfflineGift(int iIndex)
{
	if ( !gObjIsConnected(iIndex) )
		return;

	PMSG_REQ_REG_CC_OFFLINE_GIFT pMsg;

	pMsg.h.c = 0xC1;
	pMsg.h.headcode = 0x15;
	pMsg.h.size = sizeof(pMsg);
	memcpy(pMsg.szUID, gObj[iIndex].AccountID, 11);
	memcpy(pMsg.szNAME, gObj[iIndex].Name, 11);
	pMsg.wServerCode = gGameServerCode / 20;
	pMsg.iINDEX = iIndex;
	pMsg.szUID[10] = 0;
	pMsg.szNAME[10] = 0;

	DataSendEventChip((PCHAR)&pMsg, sizeof(pMsg));
}

void EGAnsRegCCOfflineGift( PMSG_ANS_REG_CC_OFFLINE_GIFT* lpMsg)
{
	if ( !lpMsg)
		return;

	if ( !lpMsg->iResultCode )
		return;

	char szAccountID[11] = {0};
	char szName[11] = {0};
	char szGIFT_NAME[50] = {0};
	memset(szAccountID, 0, sizeof(szAccountID));
	memset(szName, 0, sizeof(szName));
	memset(szGIFT_NAME, 0, sizeof(szGIFT_NAME));
	memcpy(szAccountID, lpMsg->szUID, sizeof(szAccountID));
	memcpy(szName, lpMsg->szNAME, sizeof(szName));
	memcpy(szGIFT_NAME, lpMsg->szGIFT_NAME, sizeof(szGIFT_NAME));
	szAccountID[10] = 0;
	szName[10] = 0;
	szGIFT_NAME[49] = 0;
	char szText[256] = {0};

	wsprintf(szText, lMsg.Get(MSGGET(6,74)), szName, szGIFT_NAME);

	SendMsg.NormalMsgOut(OBJ_EMPTY, OBJ_SEND_ALL_PLYRS, NO_SPC_MAP, szText);

	CLog.LogAdd("[Chaos Castle] [%s][%s] Success to Register OffLine Gift (GIFT:%s)",
		szAccountID, szName, szGIFT_NAME);
}

struct PMSG_REQ_REG_DL_OFFLINE_GIFT
{
	PBMSG_HEAD h;	// C1:16
	int iINDEX;	// 4
	char szUID[11];	// 8
	WORD wServerCode;	// 14
	char szNAME[11];	// 16
};

void EGReqRegDLOfflineGift(int iIndex)
{
	if ( !gObjIsConnected(iIndex) )
		return;

	PMSG_REQ_REG_DL_OFFLINE_GIFT pMsg;

	pMsg.h.c = 0xC1;
	pMsg.h.headcode = 0x16;
	pMsg.h.size = sizeof(pMsg);
	memcpy(pMsg.szUID, gObj[iIndex].AccountID, 11);
	memcpy(pMsg.szNAME, gObj[iIndex].Name, 11);
	pMsg.wServerCode = gGameServerCode / 20;
	pMsg.iINDEX = iIndex;
	pMsg.szUID[10] = 0;
	pMsg.szNAME[10] = 0;

	DataSendEventChip((PCHAR)&pMsg, sizeof(pMsg));
}

void EGAnsRegDLOfflineGift( PMSG_ANS_REG_DL_OFFLINE_GIFT* lpMsg)
{
	if ( !lpMsg)
		return;

	if ( !lpMsg->iResultCode )
		return;

	char szAccountID[11] = {0};
	char szName[11] = {0};
	char szGIFT_NAME[50] = {0};
	memset(szAccountID, 0, sizeof(szAccountID));
	memset(szName, 0, sizeof(szName));
	memset(szGIFT_NAME, 0, sizeof(szGIFT_NAME));
	memcpy(szAccountID, lpMsg->szUID, sizeof(szAccountID));
	memcpy(szName, lpMsg->szNAME, sizeof(szName));
	memcpy(szGIFT_NAME, lpMsg->szGIFT_NAME, sizeof(szGIFT_NAME));
	szAccountID[10] = 0;
	szName[10] = 0;
	szGIFT_NAME[49] = 0;
	char szText[256] = {0};

	wsprintf(szText, "[´ÙÅ©·Îµå ±â³ä ÀÌº¥Æ®] %s ´Ô²²¼­ %s °æÇ°¿¡ ´çÃ·µÇ¼Ì½À´Ï´Ù.", szName, szGIFT_NAME);
	SendMsg.NormalMsgOut(OBJ_EMPTY, OBJ_SEND_ALL_PLYRS, NO_SPC_MAP, szText);

	CLog.LogAdd("[DarkLord Heart Event] [%s][%s] Success to Register OffLine Gift (GIFT:%s)",
		szAccountID, szName, szGIFT_NAME);
}

struct PMSG_REQ_REG_HT_OFFLINE_GIFT
{
	PBMSG_HEAD h;	// C1:17
	int iINDEX;	// 4
	char szUID[11];	// 8
	WORD wServerCode;	// 14
	char szNAME[11];	// 16
};

void EGReqRegHTOfflineGift(int iIndex)
{
	if ( !gObjIsConnected(iIndex) )
		return;

	PMSG_REQ_REG_HT_OFFLINE_GIFT pMsg;

	pMsg.h.c = 0xC1;
	pMsg.h.headcode = 0x17;
	pMsg.h.size = sizeof(pMsg);
	memcpy(pMsg.szUID, gObj[iIndex].AccountID, 11);
	memcpy(pMsg.szNAME, gObj[iIndex].Name, 11);
	pMsg.wServerCode = gGameServerCode / 20;
	pMsg.iINDEX = iIndex;
	pMsg.szUID[10] = 0;
	pMsg.szNAME[10] = 0;

	DataSendEventChip((PCHAR)&pMsg, sizeof(pMsg));
}

void EGAnsRegHTOfflineGift( PMSG_ANS_REG_HT_OFFLINE_GIFT* lpMsg)
{
	if ( !lpMsg)
		return;

	if ( !lpMsg->iResultCode )
	{
		if ( gObjIsConnected(lpMsg->iINDEX))
		{
			LPOBJ lpObj = &gObj[lpMsg->iINDEX];

			MapC[lpObj->MapNumber].MoneyItemDrop(1000000, lpObj->X, lpObj->Y);
		}
		return;
	}

	char szAccountID[11] = {0};
	char szName[11] = {0};
	char szGIFT_NAME[50] = {0};
	memset(szAccountID, 0, sizeof(szAccountID));
	memset(szName, 0, sizeof(szName));
	memset(szGIFT_NAME, 0, sizeof(szGIFT_NAME));
	memcpy(szAccountID, lpMsg->szUID, sizeof(szAccountID));
	memcpy(szName, lpMsg->szNAME, sizeof(szName));
	memcpy(szGIFT_NAME, lpMsg->szGIFT_NAME, sizeof(szGIFT_NAME));
	szAccountID[10] = 0;
	szName[10] = 0;
	szGIFT_NAME[49] = 0;
	char szText[256] = {0};

	wsprintf(szText, "[¼û°ÜÁø º¸¹°»óÀÚ ÀÌº¥Æ®] %s ´Ô²²¼­ %s °æÇ°¿¡ ´çÃ·µÇ¼Ì½À´Ï´Ù.", szName, szGIFT_NAME);
	SendMsg.NormalMsgOut(OBJ_EMPTY, OBJ_SEND_ALL_PLYRS, NO_SPC_MAP, szText);

	CLog.LogAdd("[Hidden TreasureBox Event] [%s][%s] Success to Register OffLine Gift (GIFT:%s)",
		szAccountID, szName, szGIFT_NAME);
}

void Japan1StAnivBoxOpen(LPOBJ lpObj, int iBoxLevel)
{
	PMSG_SERVERCMD ServerCmd;

	C1SubHeadSet((LPBYTE)&ServerCmd, 0xF3, 0x40, sizeof(ServerCmd));
	ServerCmd.CmdType = 0;
	ServerCmd.X = lpObj->X;
	ServerCmd.Y = lpObj->Y;

	gSendProto.VPMsgSend(lpObj, (LPBYTE)&ServerCmd, sizeof(ServerCmd));
	gSendProto.DataSend(lpObj->m_Index, (LPBYTE)&ServerCmd, sizeof(ServerCmd));

	srand(time(NULL));

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
	int iMinLevel = 0;
	int iAddLevel = 0;
	int iItemDropRate = 30;
	int iDropMoney = 50000;
	BYTE Socket[5];
	Socket[0] = 0xFF;
	Socket[1] = 0xFF;
	Socket[2] = 0xFF;
	Socket[3] = 0xFF;
	Socket[4] = 0xFF;
	CItemBag * Japan1StAnivBox = NULL;

	switch ( iBoxLevel )
	{
		case 1:
			//Japan1StAnivBox = GoldGoblenItemBag;
			iMinLevel = 5;
			iAddLevel = 2;
			break;
		case 2:
			//Japan1StAnivBox = TitanItemBag;
			iMinLevel = 4;
			iAddLevel = 2;
			break;
		case 3:
			//Japan1StAnivBox = GoldDerconItemBag;
			iMinLevel = 4;
			iAddLevel = 2;
			break;
		case 4:
			//Japan1StAnivBox = DevilLizardKingItemBag;
			iMinLevel = 4;
			iAddLevel = 1;
			break;
		case 5:
			//Japan1StAnivBox = KanturItemBag;
			iMinLevel = 4;
			iAddLevel = 0;
			break;
	}

	if ( (rand()%100) < iItemDropRate )
	{
		if ( Japan1StAnivBox->GetEventBagCount() > 0 )
		{
			int NormalItemCount = Japan1StAnivBox->GetNrItemCount();
			int ExItemCount = Japan1StAnivBox->GetExItemCount();
			DropItemNum = rand()%NormalItemCount;
			dur=0;
			x = lpObj->X;
			y = lpObj->Y;

			if ( iAddLevel )
				level = iMinLevel + rand() % iAddLevel;
			else
				level = iMinLevel;

			type = ItemGetNumberMake(Japan1StAnivBox->GetType(DropItemNum), Japan1StAnivBox->GetIndex(DropItemNum));

			Option1 = rand() % 2;
			Option2 = rand() % 2;

			if ( Option2 == 0 || Option1  == 0 )
			{
				if ( (rand() % 5 ) < 1 )
				{
					Option3 = 3;
				}
				else
				{
					Option3 = rand() % 3;
				}
			}

			if ( type == ITEMGET(12,15) || type == ITEMGET(14,13) || type == ITEMGET(14,14) || type == ITEMGET(14,16) )
			{
				Option1 = 0;
				Option2 = 0;
				Option3 = 0;
				level = 0;
				ExOption = 0;
			}

			if ( type == ITEMGET(13,0) ||
				 type == ITEMGET(13,1) ||
				 type == ITEMGET(13,2) ||
				 type == ITEMGET(13,8) ||
				 type == ITEMGET(13,9) ||
				 type == ITEMGET(13,12) ||
				 type == ITEMGET(13,13) )
			{
				level = 0;
			}

			ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, x, y,
				type, level, dur, Option1, Option2, Option3, lpObj->m_Index, ExOption, 0, Socket);

			CLog.LogAdd("[Japan1StAnivBox] BoxLevel-%d Event ItemDrop : Item:%d Level:%d op1:%d op2:%d op3:%d ex:%d",
				iBoxLevel, type, level, Option1, Option2, Option3, ExOption);

			return;
		}
	}
	else
	{
		MapC[lpObj->MapNumber].MoneyItemDrop(iDropMoney*iBoxLevel, lpObj->X, lpObj->Y);
	}
}

// -----------------------------------------------------------------------------------------------------------------------------------------------------
void LuckyBoxOpenEvent(LPOBJ lpObj)
{
	LuckyBoxItemBag->DropItem(lpObj->m_Index);
}
//----------
void StarOfXMasOpenEvent(LPOBJ lpObj)
{
	StarOfXMasItemBag->DropItem(lpObj->m_Index);
}
//----------
void FireCrackerOpenEvent(LPOBJ lpObj)
{
	FireCrackerItemBag->DropItem(lpObj->m_Index);
}
//----------
void HeartOfLoveOpenEvent(LPOBJ lpObj)
{
	HeartOfLoveItemBag->DropItem(lpObj->m_Index);
}
//----------
void SilverMedalOpenEvent(LPOBJ lpObj)
{
	SilverMedalItemBag->DropItem(lpObj->m_Index);
}
//----------
void GoldMedalOpenEvent(LPOBJ lpObj)
{
	GoldMedalItemBag->DropItem(lpObj->m_Index);
}
//----------
void HeavenBoxOpenEvent(LPOBJ lpObj)
{
	HeavenBoxItemBag->DropItem(lpObj->m_Index);
}
//----------
void KundunBox01OpenEvent(LPOBJ lpObj)
{
	KudnunBox01ItemBag->DropItem(lpObj->m_Index);
}
//----------
void KundunBox02OpenEvent(LPOBJ lpObj)
{
	KudnunBox02ItemBag->DropItem(lpObj->m_Index);
}
//----------
void KundunBox03OpenEvent(LPOBJ lpObj)
{
	KudnunBox03ItemBag->DropItem(lpObj->m_Index);
}
//----------
void KundunBox04OpenEvent(LPOBJ lpObj)
{
	KudnunBox04ItemBag->DropItem(lpObj->m_Index);
}
//----------
void KundunBox05OpenEvent(LPOBJ lpObj)
{
	KudnunBox05ItemBag->DropItem(lpObj->m_Index);
}
//----------
void DarkLordHeartBoxOpenEvent(LPOBJ lpObj)
{
	DarkLordHeartItemBag->DropItem(lpObj->m_Index);
}
//----------
void WizardRing40OpenEvent(LPOBJ lpObj)
{
	WizRing40ItemBag->DropItem(lpObj->m_Index);
}
//----------
void WizardRing80OpenEvent(LPOBJ lpObj)
{
	WizRing80ItemBag->DropItem(lpObj->m_Index);
}
//----------
void SealedGoldenOpenEvent(LPOBJ lpObj)
{
	SealedGoldenItemBag->DropItem(lpObj->m_Index);
}
//----------
void SealedSilverOpenEvent(LPOBJ lpObj)
{
	SealedSilverItemBag->DropItem(lpObj->m_Index);
}
//----------
void ShinningJewelryOpenEvent(LPOBJ lpObj)
{
	ShiningJwlryItemBag->DropItem(lpObj->m_Index);
}
//----------
void ElegantJewelryOpenEvent(LPOBJ lpObj)
{
	ElegantJwlryItemBag->DropItem(lpObj->m_Index);
}
//----------
void SteelJewelryOpenEvent(LPOBJ lpObj)
{
	SteelJewelryItemBag->DropItem(lpObj->m_Index);
}
//----------
void OldJewelryOpenEvent(LPOBJ lpObj)
{
	OldJewelryItemBag->DropItem(lpObj->m_Index);
}
//-------------------------- START -----------------------------------
void RedRibbonBoxOpenEvent(LPOBJ lpObj)
{
	RedRibbonBoxItemBag->DropItem(lpObj->m_Index);
}
//----------
void GreenRibbonBoxOpenEvent(LPOBJ lpObj)
{
	GreenRibbonBoxItemBag->DropItem(lpObj->m_Index);
}
//----------
void BlueRibbonBoxOpenEvent(LPOBJ lpObj)
{
	BlueRibbonBoxItemBag->DropItem(lpObj->m_Index);
}
//----------
void PinkChocolateBoxOpenEvent(LPOBJ lpObj)
{
	PinkChocolateBoxItemBag->DropItem(lpObj->m_Index);
}
//----------
void RedChocolateBoxOpenEvent(LPOBJ lpObj)
{
	RedChocolateBoxItemBag->DropItem(lpObj->m_Index);
}
//----------
void BlueChocolateBoxOpenEvent(LPOBJ lpObj)
{
	BlueChocolateBoxItemBag->DropItem(lpObj->m_Index);
}
//----------
void LightPurpleCandyBoxOpenEvent(LPOBJ lpObj)
{
	LightPurpleCandyBoxItemBag->DropItem(lpObj->m_Index);
}
//----------
void VermilionCandyBoxOpenEvent(LPOBJ lpObj)
{
	VermilionCandyBoxItemBag->DropItem(lpObj->m_Index);
}
//----------
void DeepBlueCandyBoxOpenEvent(LPOBJ lpObj)
{
	DeepBlueCandyBoxItemBag->DropItem(lpObj->m_Index);
}
//----------
void HalloweenDayBoxOpenEvent(LPOBJ lpObj)
{
	HalloweenDayBoxItemBag->DropItem(lpObj->m_Index);
}
//----------
void CherryBlossomOpenEvent(LPOBJ lpObj)
{
	CherryBlossomBoxItemBag->DropItem(lpObj->m_Index);
}
void GreenBoxOpenEvent(LPOBJ lpObj)
{
	GreenBoxItemBag->DropItem(lpObj->m_Index);
}
void RedBoxOpenEvent(LPOBJ lpObj)
{
	RedBoxItemBag->DropItem(lpObj->m_Index);
}
void PurpleBoxOpenEvent(LPOBJ lpObj)
{
	PurpleBoxItemBag->DropItem(lpObj->m_Index);
}
void GreenChaosBoxOpenEvent(LPOBJ lpObj)
{
	GreenChaosBoxItemBag->DropItem(lpObj->m_Index);
}
void RedChaosBoxOpenEvent(LPOBJ lpObj)
{
	RedChaosBoxItemBag->DropItem(lpObj->m_Index);
}
void PurpleChaosBoxOpenEvent(LPOBJ lpObj)
{
	PurpleChaosBoxItemBag->DropItem(lpObj->m_Index);
}
void GameMasterBoxOpenEvent(LPOBJ lpObj)
{
	GameMasterBoxItemBag->DropItem(lpObj->m_Index);
}
void BlueHappyPouchOpenEvent(LPOBJ lpObj)
{
	BlueHappyPouchBag->DropItem(lpObj->m_Index);
}
void RedHappyPouchOpenEvent(LPOBJ lpObj)
{
	RedHappyPouchBag->DropItem(lpObj->m_Index);
}

#if(GS_CASTLE)
void CastleHuntZoneBossRewardOpen(LPOBJ lpObj,BYTE btMapNumber,BYTE cX, BYTE cY)
{
	CastleHuntZoneBossItemBag->DropCastleHuntZoneBossReward(lpObj->m_Index,btMapNumber,cX,cY);
}
#endif