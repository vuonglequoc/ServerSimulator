// Eprotocol.cpp: implementation of the Eprotocol class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Eprotocol.h"
#include "giocp.h"
#include "LogProc.h"
#include "winutil.h"



CQuery g_EventDB;


void InitEventDB()
{
	if ( g_EventDB.Connect(g_EventServerDNS, g_UserID, g_Password) == TRUE )
	{
		LogAddC(4, "[EventDB] Connection Successfull!");
	}
	else
	{
		LogAddC(2, "[EventDB] Error On Connection!");
	}
}



void EProtocolCore(int aIndex, BYTE HeadCode, LPBYTE aRecv, int iSize)
{
#if (TRACE_PACKET == 1 )
	LogAddHeadHex("EVENT_SERVER", aRecv, iSize);
#endif
	g_ServerInfoDisplayer.CheckEVDSConnection(TRUE);
	switch ( HeadCode )
	{
	case 0x01:
		EGAnsEventChipInfo(aIndex,(PMSG_REQ_VIEW_EC_MN *)aRecv);
		break;
	case 0x02:
		EGAnsRegEventChipInfo(aIndex,(PMSG_REQ_REGISTER_EVENTCHIP *)aRecv);
		break;
	case 0x04:
		EGAnsResetEventChip(aIndex,(PMSG_REQ_RESET_EVENTCHIP *)aRecv);
		break;
	case 0x05:
		EGAnsEventStoneInfo(aIndex,(PMSG_REQ_VIEW_EC_MN *)aRecv);
		break;
	case 0x06:
		EGAnsRegEventStoneInfo(aIndex,(PMSG_REQ_REGISTER_STONES *)aRecv);
		break;
	case 0x07:
		EGAnsDeleteStones(aIndex,(PMSG_REQ_DELETE_STONES *)aRecv);
		break;
	case 0x08:
		EGAns2AnivRegSerial(aIndex,(PMSG_REQ_2ANIV_SERIAL *)aRecv);
		break;
	case 0x09:
		EGAnsResetStoneInfo(aIndex,(PMSG_REQ_RESET_EVENTCHIP *)aRecv);
		break;
	case 0x10:
		EGAnsRegRingGift(aIndex,(PMSG_REQ_REG_RINGGIFT *)aRecv);
		break;
	case 0x15:
		EGAnsRegCCOfflineGift(aIndex,(PMSG_REQ_REG_CC_OFFLINE_GIFT *)aRecv);
		break;
	case 0x16:
		EGAnsRegDLOfflineGift(aIndex,(PMSG_REQ_REG_DL_OFFLINE_GIFT *)aRecv);
		break;
	case 0x17:
		EGAnsRegHTOfflineGift(aIndex,(PMSG_REQ_REG_HT_OFFLINE_GIFT *)aRecv);
		break;
	}
	g_ServerInfoDisplayer.CheckEVDSConnection(FALSE);
}

void EGAnsEventChipInfo(int aIndex,PMSG_REQ_VIEW_EC_MN * lpMsg)
{
	int test = SQL_NO_DATA;
	PMSG_ANS_VIEW_EC_MN pMsg;

	pMsg.h.c = 0xC1;
	pMsg.h.headcode = 0x01;
	pMsg.h.size = sizeof(pMsg);

	pMsg.iINDEX = lpMsg->iINDEX;
	memcpy(pMsg.szUID,lpMsg->szUID,sizeof(pMsg.szUID));
	pMsg.szUID[10] = 0;

	if(g_EventDB.ExecQuery("SELECT EventChips, Check_Code, MuttoNumber FROM T_MU2003_EVENT WHERE AccountID='%s'",lpMsg->szUID) == FALSE)
	{
		pMsg.bSUCCESS = FALSE;
	}
	else
	{
		if(g_EventDB.Fetch() != SQL_NO_DATA)
		{
			if(g_EventDB.GetAsInteger("Check_Code") != 0)
			{
				pMsg.nEVENT_CHIPS = 0;
			}
			else
			{
				pMsg.nEVENT_CHIPS = (short)g_EventDB.GetAsInteger("EventChips");
			}

			pMsg.iMUTO_NUM = g_EventDB.GetAsInteger("MuttonNumber");
			pMsg.bSUCCESS = TRUE;
		}
		else
		{
			g_EventDB.Close();
			if(g_EventDB.ExecQuery("INSERT INTO T_MU2003_EVENT (AccountID) VALUES ('%s')",pMsg.szUID) == FALSE)
			{
				pMsg.bSUCCESS = FALSE;
			}
			else
			{
				pMsg.nEVENT_CHIPS = 0;
				pMsg.iMUTO_NUM = 0;
				pMsg.bSUCCESS = TRUE;
			}
		}
	}

	g_EventDB.Close();
	DataSend(aIndex,(LPBYTE)&pMsg,sizeof(pMsg));
}

void EGAnsRegEventChipInfo(int aIndex,PMSG_REQ_REGISTER_EVENTCHIP * lpMsg)
{
	PMSG_ANS_REGISTER_EVENTCHIP pMsg;

	pMsg.h.c = 0xC1;
	pMsg.h.headcode = 0x02;
	pMsg.h.size = sizeof(pMsg);

	pMsg.iINDEX = lpMsg->iINDEX;
	pMsg.Pos = lpMsg->Pos;

//	strcpy(pMsg.szUID,lpMsg->szUID);
	strcpy_s(pMsg.szUID,lpMsg->szUID); // VuongLQ
	pMsg.szUID[10] = 0x00;
	pMsg.bSUCCESS = FALSE;

	if(g_EventDB.ExecQuery("UPDATE T_MU2003_EVENT SET EventChips = EventChips + 1 WHERE AccountID = '%s'",lpMsg->szUID))
	{
		g_EventDB.Close();

		if(g_EventDB.ExecQuery("SELECT EventChips FROM T_MU2003_EVENT WHERE AccountID='%s'",lpMsg->szUID))
		{
			if(g_EventDB.Fetch() != SQL_NO_DATA)
			{
				pMsg.nEVENT_CHIPS = (short)g_EventDB.GetAsInteger("EventChips");

				if(pMsg.nEVENT_CHIPS != -1)
				{
					pMsg.nEVENT_CHIPS = TRUE;
				}
			}
		}
	}

	g_EventDB.Close();

	DataSend(aIndex,(LPBYTE)&pMsg,sizeof(pMsg));
}

void EGAnsResetEventChip(int aIndex,PMSG_REQ_RESET_EVENTCHIP * lpMsg)
{
	PMSG_ANS_RESET_EVENTCHIP pMsg;

	pMsg.h.c = 0xC1;
	pMsg.h.headcode = 0x04;
	pMsg.h.size = sizeof(pMsg);

	pMsg.iINDEX = lpMsg->iINDEX;
//	strcpy(pMsg.szUID,lpMsg->szUID);
	strcpy_s(pMsg.szUID,lpMsg->szUID); // VuongLQ
	pMsg.szUID[10] = 0;

	if(g_EventDB.ExecQuery("UPDATE T_MU2003_EVENT SET Check_Code = 2 WHERE AccountID = '%s'",lpMsg->szUID))
	{
		pMsg.bSUCCESS = TRUE;
	}
	else
	{
		pMsg.bSUCCESS = FALSE;
	}

	g_EventDB.Close();

	DataSend(aIndex,(LPBYTE)&pMsg,sizeof(pMsg));
}

void EGAnsEventStoneInfo(int aIndex,PMSG_REQ_VIEW_EC_MN * lpMsg)
{
	PMSG_ANS_VIEW_STONES pMsg;

	pMsg.h.c = 0xC1;
	pMsg.h.headcode = 0x05;
	pMsg.h.size = sizeof(pMsg);

	pMsg.iINDEX = lpMsg->iINDEX;
//	strcpy(pMsg.szUID,lpMsg->szUID);
	strcpy_s(pMsg.szUID,lpMsg->szUID); // VuongLQ
	pMsg.szUID[10] = 0x00;

	if(g_EventDB.ExecQuery("SELECT StoneCount, Check_Code FROM T_BLOOD_CASTLE WHERE AccountID='%s'",lpMsg->szUID) == FALSE)
	{
		pMsg.bSUCCESS = FALSE;
	}
	else
	{
		if(g_EventDB.Fetch() != SQL_NO_DATA)
		{
			if(g_EventDB.GetAsInteger("Check_Code") != 0)
			{
				pMsg.iStoneCount = 0;
			}
			else
			{
				pMsg.iStoneCount = g_EventDB.GetAsInteger("StoneCount");
			}
			pMsg.bSUCCESS = TRUE;
		}
		else
		{
			g_EventDB.Close();

			if(g_EventDB.ExecQuery("INSERT INTO T_BLOOD_CASTLE (AccountID) VALUES ('%s')",lpMsg->szUID))
			{
				pMsg.bSUCCESS = TRUE;
				pMsg.iStoneCount = 0;
			}
		}
	}

	g_EventDB.Close();
	DataSend(aIndex,(LPBYTE)&pMsg,sizeof(pMsg));
}

void EGAnsRegEventStoneInfo(int aIndex,PMSG_REQ_REGISTER_STONES * lpMsg)
{
	PMSG_ANS_REGISTER_STONES pMsg;

	pMsg.h.c = 0xC1;
	pMsg.h.headcode = 0x06;
	pMsg.h.size = sizeof(pMsg);

	pMsg.iINDEX = lpMsg->iINDEX;
	pMsg.iPosition = lpMsg->iPosition;
//	strcpy(pMsg.szUID,lpMsg->szUID);
	strcpy_s(pMsg.szUID,lpMsg->szUID); // VuongLQ
	pMsg.szUID[10] = 0x00;

	if(g_EventDB.ExecQuery("UPDATE T_BLOOD_CASTLE SET StoneCount = StoneCount + 1 WHERE AccountID = '%s'",lpMsg->szUID))
	{
		g_EventDB.Close();
		if(g_EventDB.ExecQuery("SELECT StoneCount FROM T_BLOOD_CASTLE WHERE AccountID='%s'",lpMsg->szUID))
		{
			if(g_EventDB.Fetch() != SQL_NO_DATA)
			{
				pMsg.iStoneCount = g_EventDB.GetAsInteger("StoneCount");

				if(pMsg.iStoneCount != -1)
				{
					pMsg.bSUCCESS = TRUE;
				}
			}
			else
			{
				pMsg.bSUCCESS = FALSE;
			}
		}
	}
	else
	{
		pMsg.bSUCCESS = FALSE;
	}

	g_EventDB.Close();
	DataSend(aIndex,(LPBYTE)&pMsg,sizeof(pMsg));
}

void EGAnsDeleteStones(int aIndex,PMSG_REQ_DELETE_STONES * lpMsg)
{
	PMSG_ANS_DELETE_STONES pMsg;

	pMsg.h.c = 0xC1;
	pMsg.h.headcode = 0x07;
	pMsg.h.size = sizeof(pMsg);

	pMsg.iINDEX = lpMsg->iINDEX;
//	strcpy(pMsg.szUID,lpMsg->szUID);
	strcpy_s(pMsg.szUID,lpMsg->szUID); // VuongLQ
	pMsg.szUID[10] = 0x00;

	pMsg.bSUCCESS = FALSE;

	if(g_EventDB.ExecQuery("SELECT StoneCount FROM T_BLOOD_CASTLE WHERE AccountID='%s'",lpMsg->szUID) == FALSE || g_EventDB.Fetch() == SQL_NO_DATA)
	{
		pMsg.bSUCCESS = FALSE;
	}
	else
	{
		int Stones = g_EventDB.GetAsInteger("StoneCount");

		g_EventDB.Close();

		if(Stones != -1 && Stones >= lpMsg->iStoneCount)
		{
			if(g_EventDB.ExecQuery("UPDATE T_BLOOD_CASTLE SET StoneCount = StoneCount - %d WHERE AccountID='%s'",lpMsg->szUID) == FALSE)
			{

			}
			else
			{
				pMsg.bSUCCESS = TRUE;
				pMsg.iStoneCount = Stones - lpMsg->iStoneCount;
			}
		}
	}

	g_EventDB.Close();
	DataSend(aIndex,(LPBYTE)&pMsg,sizeof(pMsg));
}

void EGAns2AnivRegSerial(int aIndex,PMSG_REQ_2ANIV_SERIAL * lpMsg)
{
	PMSG_ANS_2ANIV_SERIAL pMsg;

	pMsg.h.c = 0xC1;
	pMsg.h.headcode = 0x08;
	pMsg.h.size = sizeof(pMsg);

	pMsg.iINDEX = lpMsg->iINDEX;
//	strcpy(pMsg.szUID,lpMsg->szUID);
	strcpy_s(pMsg.szUID,lpMsg->szUID); // VuongLQ

	if(g_EventDB.ExecQuery("EXEC SP_REG_SERIAL '%s', '%d', '%s', '%s', '%s'",lpMsg->szUID,lpMsg->iMEMB_GUID,lpMsg->SERIAL1,lpMsg->SERIAL2,lpMsg->SERIAL3) && g_EventDB.Fetch() != SQL_NO_DATA)
	{
		pMsg.btIsRegistered = (BYTE)g_EventDB.GetAsInteger("RegResult");
		pMsg.iGiftNumber = g_EventDB.GetAsInteger("F_Register_Section");

		if(pMsg.iGiftNumber == -1)
		{
			pMsg.btIsRegistered = 4;
		}
	}
	else
	{
		pMsg.btIsRegistered = 4;
	}

	g_EventDB.Close();
	DataSend(aIndex,(LPBYTE)&pMsg,sizeof(pMsg));
}

void EGAnsResetStoneInfo(int aIndex,PMSG_REQ_RESET_EVENTCHIP * lpMsg)
{
	PMSG_ANS_RESET_EVENTCHIP pMsg;

	pMsg.h.c = 0xC1;
	pMsg.h.headcode = 0x09;
	pMsg.h.size = sizeof(pMsg);

	pMsg.iINDEX = lpMsg->iINDEX;
//	strcpy(pMsg.szUID,lpMsg->szUID);
	strcpy_s(pMsg.szUID,lpMsg->szUID); // VuongLQ
	pMsg.szUID[10] = 0;

	if(g_EventDB.ExecQuery("UPDATE T_BLOOD_CASTLE SET Check_Code = 2 WHERE AccountID = '%s'",lpMsg->szUID) == FALSE)
	{
		pMsg.bSUCCESS = FALSE;
	}
	else
	{
		pMsg.bSUCCESS = TRUE;
	}

	g_EventDB.Close();
	DataSend(aIndex,(LPBYTE)&pMsg,sizeof(pMsg));
}

void EGAnsRegRingGift(int aIndex,PMSG_REQ_REG_RINGGIFT * lpMsg)
{
	PMSG_ANS_REG_RINGGIFT pMsg;

	pMsg.h.c = 0xC1;
	pMsg.h.headcode = 0x10;
	pMsg.h.size = sizeof(pMsg);

	pMsg.iINDEX = lpMsg->iINDEX;
//	strcpy(pMsg.szUID,lpMsg->szUID);
	strcpy_s(pMsg.szUID,lpMsg->szUID); // VuongLQ
	pMsg.szUID[10] = 0x10;

	pMsg.btIsRegistered = 0;

	if(g_EventDB.ExecQuery("EXEC SP_REG_RINGGIFT '%s', '%d'",lpMsg->szUID,lpMsg->btGiftKind) == FALSE || g_EventDB.Fetch() == SQL_NO_DATA)
	{

	}
	else
	{
		pMsg.btIsRegistered = (BYTE)g_EventDB.GetAsInteger("RegResult");
		pMsg.btGiftKind = (BYTE)g_EventDB.GetAsInteger("GiftKind");
		pMsg.btGiftSection = lpMsg->btGiftKind;

		if(pMsg.btIsRegistered == -1)
		{
			pMsg.btIsRegistered = 0;
		}
	}

	g_EventDB.Close();
	DataSend(aIndex,(LPBYTE)&pMsg,sizeof(pMsg));
}

void EGAnsRegCCOfflineGift(int aIndex,PMSG_REQ_REG_CC_OFFLINE_GIFT * lpMsg)
{
	PMSG_ANS_REG_CC_OFFLINE_GIFT pMsg;

	pMsg.h.c = 0xC1;
	pMsg.h.headcode = 0x15;
	pMsg.h.size = sizeof(pMsg);

	pMsg.iINDEX = lpMsg->iINDEX;
	pMsg.wServerCode = lpMsg->wServerCode;
//	strcpy(pMsg.szUID,lpMsg->szUID);
	strcpy_s(pMsg.szUID,lpMsg->szUID); // VuongLQ
//	strcpy(pMsg.szNAME,lpMsg->szNAME);
	strcpy_s(pMsg.szNAME,lpMsg->szNAME); // VuongLQ
	pMsg.szUID[10] = 0x00;
	pMsg.szNAME[10] = 0x00;
	
	pMsg.iResultCode = 0;

	if(g_EventDB.ExecQuery("EXEC SP_REG_CC_OFFLINE_GIFT '%s', '%s', %d",lpMsg->szUID,lpMsg->szNAME,lpMsg->wServerCode) == FALSE || g_EventDB.Fetch() == SQL_NO_DATA)
	{

	}
	else
	{
		pMsg.iResultCode = g_EventDB.GetAsInteger("ResultCode");
		if(pMsg.iResultCode != -1)
		{
			g_EventDB.GetAsString("GiftName",pMsg.szGIFT_NAME);
		}
	}

	g_EventDB.Close();
	DataSend(aIndex,(LPBYTE)&pMsg,sizeof(pMsg));
}

void EGAnsRegDLOfflineGift(int aIndex,PMSG_REQ_REG_DL_OFFLINE_GIFT * lpMsg)
{
	PMSG_ANS_REG_DL_OFFLINE_GIFT pMsg;

	pMsg.h.c = 0xC1;
	pMsg.h.headcode = 0x16;
	pMsg.h.size = sizeof(pMsg);

	pMsg.iINDEX = lpMsg->iINDEX;
	pMsg.wServerCode = lpMsg->wServerCode;
//	strcpy(pMsg.szUID,lpMsg->szUID);
	strcpy_s(pMsg.szUID,lpMsg->szUID); // VuongLQ
//	strcpy(pMsg.szNAME,lpMsg->szNAME);
	strcpy_s(pMsg.szNAME,lpMsg->szNAME); // VuongLQ
	pMsg.szUID[10] = 0x00;
	pMsg.szNAME[10] = 0x00;
	
	pMsg.iResultCode = 0;

	if(g_EventDB.ExecQuery("EXEC SP_REG_DL_OFFLINE_GIFT '%s', '%s', %d",lpMsg->szUID,lpMsg->szNAME,lpMsg->wServerCode) == FALSE || g_EventDB.Fetch() == SQL_NO_DATA)
	{

	}
	else
	{
		pMsg.iResultCode = g_EventDB.GetAsInteger("ResultCode");
		if(pMsg.iResultCode != -1)
		{
			g_EventDB.GetAsString("GiftName",pMsg.szGIFT_NAME);
		}
	}

	g_EventDB.Close();
	DataSend(aIndex,(LPBYTE)&pMsg,sizeof(pMsg));
}

void EGAnsRegHTOfflineGift(int aIndex,PMSG_REQ_REG_HT_OFFLINE_GIFT * lpMsg)
{
	PMSG_ANS_REG_HT_OFFLINE_GIFT pMsg;

	pMsg.h.c = 0xC1;
	pMsg.h.headcode = 0x17;
	pMsg.h.size = sizeof(pMsg);

	pMsg.iINDEX = lpMsg->iINDEX;
	pMsg.wServerCode = lpMsg->wServerCode;
//	strcpy(pMsg.szUID,lpMsg->szUID);
	strcpy_s(pMsg.szUID,lpMsg->szUID); // VuongLQ
//	strcpy(pMsg.szNAME,lpMsg->szNAME);
	strcpy_s(pMsg.szNAME,lpMsg->szNAME); // VuongLQ
	pMsg.szUID[10] = 0x00;
	pMsg.szNAME[10] = 0x00;
	
	pMsg.iResultCode = 0;

	if(g_EventDB.ExecQuery("EXEC SP_REG_HT_OFFLINE_GIFT '%s', '%s', %d",lpMsg->szUID,lpMsg->szNAME,lpMsg->wServerCode) == FALSE || g_EventDB.Fetch() == SQL_NO_DATA)
	{

	}
	else
	{
		pMsg.iResultCode = g_EventDB.GetAsInteger("ResultCode");
		if(pMsg.iResultCode != -1)
		{
			g_EventDB.GetAsString("GiftName",pMsg.szGIFT_NAME);
		}
	}

	g_EventDB.Close();
	DataSend(aIndex,(LPBYTE)&pMsg,sizeof(pMsg));
}
