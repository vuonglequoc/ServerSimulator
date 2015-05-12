#include "../Header Files/stdafx.h"
#include "../Header Files/ChaosCard.h"
#include "../Header Files/ChaosBox.h"
#include "../Header Files/LogProc.H"
#include "../Header Files/DSProtocol.h"
#include "../Header Files/Functions.H"
#include "../Header Files/SendProto.H"


CChaosCard gChaosCard;

CChaosCard::CChaosCard(void)
{

}

CChaosCard::~CChaosCard(void)
{

}

void CChaosCard::ChaosCardMix(LPOBJ lpObj)
{
	lpObj->ChaosLock = TRUE;
	int iChaosCardCount = 0;
	int iChaosMoney = 0;
	int iInvalidItemCount = 0;

	PMSG_CHAOSMIXRESULT pMsg;

	C1HeadSet((LPBYTE)&pMsg.h, 0x86, sizeof(PMSG_CHAOSMIXRESULT));
	pMsg.Result = CB_ERROR;

	for ( int n=0;n<CHAOS_BOX_SIZE;n++)
	{
		if ( lpObj->pChaosBox[n].IsItem() == TRUE )
		{
			if ( lpObj->pChaosBox[n].m_Type == ITEMGET(14,54) )
			{
				iChaosCardCount++;
			}
			else
			{
				iInvalidItemCount++;
			}
		}
	}

	if ( iChaosCardCount == 0 )
	{
		return;
	}

	if ( iInvalidItemCount > 0 )
	{
		return;
	}

	if ( iChaosCardCount > 1 )
	{
		MsgOutput(lpObj->m_Index,"[ChaosCard]: Can only use 1 card at the same time");
		gSendProto.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;
		return;
	}

	LogChaosItem(lpObj,"ChaosCard");
	CLog.LogAdd("[ChaosCard] Chaos Mix Start");
	lpObj->ChaosSuccessRate = 100;

	BYTE Socket[5];
	Socket[0] = 0xFF;
	Socket[1] = 0xFF;
	Socket[2] = 0xFF;
	Socket[3] = 0xFF;
	Socket[4] = 0xFF;

	if ( (rand()%100) < lpObj->ChaosSuccessRate )
	{
		int ItemNum = ITEMGET(14,14);
		int Level = 0;

		ItemSerialCreateSend(lpObj->m_Index, -1, 0, 0, ItemNum, Level, 0, 0, 0, 0, -1, 0, 0, Socket);
		::gObjInventoryCommit(lpObj->m_Index);

		CLog.LogAdd("[ChaosCard] [%s][%s] CBMix Success %d ", lpObj->AccountID, lpObj->Name, lpObj->ChaosSuccessRate);
	}
	else
	{
		gChaosBox.ChaosBoxInit(lpObj);
		::GCUserChaosBoxSend(lpObj, 1);
		gSendProto.DataSend(lpObj->m_Index, (UCHAR*)&pMsg, pMsg.h.size);
		CLog.LogAdd("[ChaosCard] [%s][%s] CBMix Fail %d ", lpObj->AccountID, lpObj->Name, lpObj->ChaosSuccessRate);
		lpObj->ChaosLock = FALSE;
	}
}