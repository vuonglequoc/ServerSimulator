//---------------------------------------------------------------------------
// # Project:		HaRiBO MU GameServer - Supported Season 6
// # Company:		RealCoderZ MU Development © 2011
// # Description:	Personal Shop Class
// # Developed By:	WolF & M.E.S
//---------------------------------------------------------------------------
#include "../Header Files/stdafx.h"
#include "../Header Files/PersonalShop.H"
#include "../Header Files/GameMain.H"
#include "../Header Files/LogProc.H"
#include "../Header Files/ChaosCastle.H"
#include "../Header Files/Notice.H"
#include "../Header Files/Functions.H"
#include "../Header Files/SendProto.H"


CPersonalShop CPShop;
// -----------------------------------------------------------------------------------------------------------------------------------------------------
CPersonalShop::CPersonalShop()
{
	// -----
}
// -----------------------------------------------------------------------------------------------------------------------------------------------------
CPersonalShop::~CPersonalShop()
{
	// -----
}
// -----------------------------------------------------------------------------------------------------------------------------------------------------
void CPersonalShop::SendPShopSetItemPriceResult(int aIndex, BYTE btResult, BYTE btItemPos)
{
	PMSG_ANS_PSHOP_SETITEMPRICE pMsg;
	// ------
	C1SubHeadSet((LPBYTE)&pMsg, 0x3F, 0x01, sizeof(pMsg));
	// ------
	pMsg.btResult	= btResult;
	pMsg.btItemPos	= btItemPos;
	// ------
	gSendProto.DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
}
// -----------------------------------------------------------------------------------------------------------------------------------------------------
void CPersonalShop::SetPersonalShopItemPrice(PMSG_REQ_PSHOP_SETITEMPRICE * lpMsg, int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];
	// ------
	if( gDoPShopOpen == FALSE )
	{
		CLog.LogAddC(TColor.Red(), PShopModule, "(Error): [%s][%s] is Trying to Open Personal Shop.", lpObj->AccountID, lpObj->Name);
		// ------
		SendPShopSetItemPriceResult(aIndex, 0, lpMsg->btItemPos);
		return;
	}
	// ------
	if( !gObjIsConnected(aIndex) )
	{
		CLog.LogAddC(TColor.Red(), PShopModule, "(Error): [%s][%s] is Offline Player.", lpObj->AccountID, lpObj->Name);
		return;
	}
	// ------
	if( lpObj->Type != OBJ_USER )
	{
		CLog.LogAddC(TColor.Red(), PShopModule, "(Error): [%s] isn't an Object User.", lpObj->m_Index);
		return;
	}
	// ------
	if( lpObj->Level <= 5 )
	{
		CLog.LogAddC(TColor.Red(), PShopModule, "(Error): [%s][%s] is Level: (%d)", lpObj->AccountID, lpObj->Name, lpObj->Level);
		// ------
		SendPShopSetItemPriceResult(aIndex, 5, lpMsg->btItemPos);
		return;
	}
	// ------
	if( INVENTORY_RANGE(lpMsg->btItemPos) == FALSE )
	{
		CLog.LogAddC(TColor.Red(), PShopModule, "(Error): [%s][%s] Item Position Out of Range: (%d)",lpObj->AccountID, lpObj->Name, lpMsg->btItemPos);
		// ------
		SendPShopSetItemPriceResult(aIndex, 2, lpMsg->btItemPos);
		return;
	}
	// ------
	if( lpObj->Inventory1[lpMsg->btItemPos].IsItem() == FALSE )
	{
		CLog.LogAddC(TColor.Red(), PShopModule, "(Error): [%s][%s] Item Doesn't Exist: (%d)",lpObj->AccountID, lpObj->Name, lpMsg->btItemPos);
		// ------
		SendPShopSetItemPriceResult(aIndex, 3, lpMsg->btItemPos);
		return;
	}
	// ------
	CItem * sitem = &lpObj->Inventory1[lpMsg->btItemPos];
	// ------
	int iItemPrice = MAKE_NUMBERDW(MAKE_NUMBERW(lpMsg->sItemPrice4, lpMsg->sItemPrice3), MAKE_NUMBERW(lpMsg->sItemPrice2, lpMsg->sItemPrice1));
	// ------
	if( gObjInventorySearchSerialNumber(lpObj, sitem->GetNumber()) == FALSE )
	{
		GCReFillSend(aIndex, lpObj->Life, 0xFD, TRUE, lpObj->iShield);
		// ------
		CLog.LogAddC(TColor.Red(), PShopModule, "(Error): [%s][%s] CopyItem :: [%s](%d)",lpObj->AccountID, lpObj->Name, __FILE__, __LINE__);
		return;
	}
	// ------
	if( iItemPrice <= 0 )
	{
		CLog.LogAddC(TColor.Red(), PShopModule, "(Error): [%s][%s] Item Price <= (0): (%d)",lpObj->AccountID, lpObj->Name, sitem->m_iPShopValue);
		// ------
		SendPShopSetItemPriceResult(aIndex, 4, lpMsg->btItemPos);
		return;
	}
	// ------
	if( (lpObj->Penalty&4) == 4 || (lpObj->Penalty&8) == 8 )
	{
		CLog.LogAddC(TColor.Red(), PShopModule, "(Error): [%s][%s] Item Price Change(%d), Item Block",lpObj->AccountID, lpObj->Name, sitem->m_iPShopValue);
		// ------
		SendPShopSetItemPriceResult(aIndex, 6, lpMsg->btItemPos);
		return;
	}
	// ------
	sitem->m_iPShopValue = iItemPrice;
	// ------
	ItemAttribute[sitem->m_Type].Name[31] = 0;
	// ------
	CLog.LogAddC(TColor.Green(), PShopModule, "[%s][%s]: Changing Item Price:(%d) - Type:(%d) [%s] - Level:(%d) - Serial:(%d) - Dur:(%d)",lpObj->AccountID,
		lpObj->Name, sitem->m_iPShopValue, sitem->m_Type, ItemAttribute[sitem->m_Type].Name, sitem->m_Level, sitem->m_Number, sitem->m_Durability);
	// ------
	SendPShopSetItemPriceResult(aIndex, 1, lpMsg->btItemPos);
}
// -----------------------------------------------------------------------------------------------------------------------------------------------------
void CPersonalShop::SendPShopOpenResult(int aIndex, BYTE btResult)
{
	PMSG_ANS_PSHOP_OPEN pMsg;
	// ------
	C1SubHeadSet((LPBYTE)&pMsg, 0x3F, 0x02, sizeof(pMsg));
	// ------
	pMsg.btResult = btResult;
	// ------
	gSendProto.DataSend(aIndex, (LPBYTE)&pMsg, sizeof(pMsg));
}
// -----------------------------------------------------------------------------------------------------------------------------------------------------
void CPersonalShop::OpenPersonalShop(PMSG_REQ_PSHOP_OPEN * lpMsg, int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];
	// ------
	if( gDoPShopOpen == FALSE )
	{
		CLog.LogAddC(TColor.Red(), PShopModule, "(Error): [%s][%s] is Trying to Open Personal Shop.", lpObj->AccountID, lpObj->Name);
		return;
	}
	// ------
	if( !gObjIsConnected(aIndex) )
	{
		CLog.LogAddC(TColor.Red(), PShopModule, "(Error): [%s][%s] is Offline Player.", lpObj->AccountID, lpObj->Name);
		return;
	}
	// ------
	if( lpObj->Type != OBJ_USER )
	{
		CLog.LogAddC(TColor.Red(), PShopModule, "(Error): [%s] isn't an Object User.", lpObj->m_Index);
		return;
	}
	// ------
	if( lpObj->Level <= 5 )
	{
		CLog.LogAddC(TColor.Red(), PShopModule, "(Error): [%s][%s] is Level: (%d)", lpObj->AccountID, lpObj->Name, lpObj->Level);
		// ------
		SendPShopOpenResult(aIndex, 2);
		return;
	}
	// ------
	if( (lpObj->Penalty&4) == 4 || (lpObj->Penalty&8) == 8 )
	{
		CLog.LogAddC(TColor.Red(), PShopModule, "(Error): [%s][%s] Can't Open Shop - Item Block",lpObj->AccountID, lpObj->Name);
		// ------
		SendPShopOpenResult(aIndex, 3);
		return;
	}
	// ------
	if( gUser.ChaosCastleMapRange(lpObj->MapNumber) == true )
	{
		SendMsg.MessageOut(aIndex,  0x01, "You are not Enable to Open Your PersonalShop in ChaosCastle.");
		// ------
		CLog.LogAddC(TColor.Red(), PShopModule, "(Error): [%s][%s] Trying to Open Shop in ChaosCastle",lpObj->AccountID, lpObj->Name);
		// ------
		SendPShopOpenResult(aIndex, 0);
		return;
	}	
	// ------
	bool bEnablePShopOpen = false;
	// ------
	if( lpObj->m_IfState.use == FALSE )
	{
		bEnablePShopOpen  = true;
	}
	else
	{
		if( !lpObj->m_IfState.use )
		{
			switch ( lpObj->m_IfState.type )
			{
				case 8:
				{
					bEnablePShopOpen = true; 
					break;
				}
			}
			// ------
			if( bEnablePShopOpen  == false )
			{
				CLog.LogAddC(TColor.Red(), PShopModule,"(Error): [%s][%s] m_IfState.Type Used: (%d)",lpObj->AccountID, lpObj->Name, lpObj->m_IfState.type);
			}
		}
		else
		{
			CLog.LogAddC(TColor.Red(), PShopModule, "(Error): [%s][%s] Unknown m_IfState.Type Used: (%d)",lpObj->AccountID, lpObj->Name,
				lpObj->m_IfState.type);
			return;
		}
	}
	// ------
	if( gObjFixInventoryPointer(aIndex) == FALSE )
	{
		CLog.LogAddC(TColor.Red(), PShopModule, "[Fix Inv.Ptr] False Location - [%s](%d)", __FILE__, __LINE__);
	}
	// ------
	if( lpObj->pTransaction == TRUE )
	{
		CLog.LogAddC(TColor.Red(), PShopModule, "(Error): Transaction == 1, IF_STATE_TYPE: (%d)", lpObj->AccountID, lpObj->Name, lpObj->m_IfState.type);
		// ------
		bEnablePShopOpen = false;
	}
	// ------
	if( bEnablePShopOpen )
	{
		if( lpObj->m_bPShopOpen == false )
		{
			lpObj->m_bPShopOpen = true;
			// ------
			memcpy(lpObj->m_szPShopText, lpMsg->szPShopText, sizeof(lpMsg->szPShopText));
			// ------
			CLog.LogAddC(TColor.Green(), PShopModule, "[%s][%s] Shop Opened", lpObj->AccountID, lpObj->Name);
		}
		else
		{
			memcpy(lpObj->m_szPShopText, lpMsg->szPShopText, sizeof(lpMsg->szPShopText));
			// ------
			PMSG_ANS_PSHOP_TEXT_CHANGED pMsg;
			// ------
			C1SubHeadSet((LPBYTE)&pMsg, 0x3F, 0x10, sizeof(pMsg));
			pMsg.NumberH = SET_NUMBERH(aIndex);
			pMsg.NumberL = SET_NUMBERL(aIndex);
			memcpy(pMsg.btPShopText, lpMsg->szPShopText, sizeof(pMsg.btPShopText));
			memcpy(pMsg.btName, lpObj->Name, sizeof(pMsg.btName));
			// ------
			gSendProto.VPMsgSend(&gObj[aIndex], (LPBYTE)&pMsg, pMsg.h.size);
			// ------
			CLog.LogAddC(TColor.Green(), PShopModule, "[%s][%s] Shop Already Opened - Changing Shop Name", lpObj->AccountID, lpObj->Name);
		}
	}
	else
	{
		SendPShopOpenResult(aIndex, 0);
		return;
	}
	// ------
	SendPShopOpenResult(aIndex, 1);
}
// -----------------------------------------------------------------------------------------------------------------------------------------------------
void CPersonalShop::SendPShopCloseResult(int aIndex, BYTE btResult)
{
	LPOBJ lpObj = &gObj[aIndex];
	// ------
	CLog.LogAddC(TColor.Blue(), PShopModule, "[%s][%s] Closed Shop.", lpObj->AccountID,lpObj->Name);
	// ------
	PMSG_ANS_PSHOP_CLOSE pMsg;
	// ------
	C1SubHeadSet((LPBYTE)&pMsg, 0x3F, 0x03, sizeof(pMsg));
	// ------
	pMsg.btResult	= btResult;
	pMsg.NumberH	= SET_NUMBERH(aIndex);
	pMsg.NumberL	= SET_NUMBERL(aIndex);
	// ------
	gSendProto.DataSend(aIndex, (LPBYTE)&pMsg, sizeof(pMsg));
	// ------
	if( btResult == 1 )
	{
		gSendProto.VPMsgSend(lpObj, (LPBYTE)&pMsg, pMsg.h.size);
	}
}
// -----------------------------------------------------------------------------------------------------------------------------------------------------
void CPersonalShop::ClosePersonalShop(int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];
	// ------
	if( !gObjIsConnected(aIndex) )
	{
		CLog.LogAddC(TColor.Red(), PShopModule, "(Error): [%s][%s] is Offline Player.", lpObj->AccountID, lpObj->Name);
		return;
	}
	// ------
	if( lpObj->Type != OBJ_USER )
	{
		CLog.LogAddC(TColor.Red(), PShopModule, "(Error): [%s] isn't an Object User.", lpObj->m_Index);
		return;
	}
	// ------
	CLog.LogAddC(TColor.Blue(), PShopModule, "[%s][%s] Requested to Close PShop", lpObj->AccountID, lpObj->Name);
	// ------
	lpObj->m_bPShopOpen = false;
	// ------
	memset(lpObj->m_szPShopText, 0, sizeof(lpObj->m_szPShopText));
	// ------
	SendPShopCloseResult(aIndex, 1);
}
// -----------------------------------------------------------------------------------------------------------------------------------------------------
void CPersonalShop::SendPlShopBuyListResult(int aSourceIndex, int aTargetIndex, BYTE btResult, bool bResend)
{
	if( gDoPShopOpen == FALSE ) return;
	// ------
	PMSG_ANS_BUYLIST_FROM_PSHOP		pMsgILC;
	PMSG_BUYLIST_FROM_PSHOP			pMsgIL;
	// ------
	BYTE sendBuf[1024];
	// ------
	int sOfs		= sizeof(pMsgILC);
	int pMsgILSize	= sizeof(pMsgIL);
	// ------
	if( btResult != 1 )
	{
		LPOBJ lpObj = &gObj[aSourceIndex];
		// ------
		pMsgILC.h.c			= 0xC2;
		pMsgILC.h.headcode	= 0x3F;
		// ------
		if( bResend == false )
		{
			pMsgILC.h.subcode = 0x05;
		}
		else
		{
			pMsgILC.h.subcode = 0x13;
		}
		// ------
		pMsgILC.h.sizeH		= SET_NUMBERH(sOfs);
		pMsgILC.h.sizeL		= SET_NUMBERL(sOfs);
		pMsgILC.btCount		= 0;
		pMsgILC.Result		= btResult;
		pMsgILC.NumberH		= SET_NUMBERH(aTargetIndex);
		pMsgILC.NumberL		= SET_NUMBERL(aTargetIndex);
		// ------
		memset(pMsgILC.btName, 0, sizeof(pMsgILC.btName));
		memset(pMsgILC.szPShopText, 0, sizeof(pMsgILC.szPShopText));
		memcpy(sendBuf, &pMsgILC, sizeof(pMsgILC));
		// ------
		gSendProto.DataSend(aSourceIndex, sendBuf, sOfs);
	}
	else
	{
		LPOBJ lpObjSource = &gObj[aSourceIndex];
		LPOBJ lpObjTarget = &gObj[aTargetIndex];
		// ------
		int iCount = 0;
		// ------
		for ( int n=MAIN_INVENTORY_SIZE;n<INVENTORY_SIZE;n++)
		{
			if( lpObjTarget->Inventory1[n].IsItem() == TRUE )
			{
				pMsgIL.Pos = n;
				// ------
				ItemByteConvert(pMsgIL.ItemInfo, lpObjTarget->Inventory1[n]);
				// ------
				pMsgIL.PShopItemValue = lpObjTarget->Inventory1[n].m_iPShopValue;
				// ------
				memcpy(&sendBuf[sOfs], &pMsgIL, pMsgILSize);
				// ------
				iCount++;
				sOfs += pMsgILSize;
				// ------
				CLog.LogAddC(TColor.Green(), PShopModule, "Item Type: (%d)", lpObjTarget->Inventory1[n].m_Type);
			}
		}
		// ------
		pMsgILC.h.c			= 0xC2;
		pMsgILC.h.headcode	= 0x3F;
		// ------
		if( bResend == false )
		{
			pMsgILC.h.subcode = 0x05;
		}
		else
		{
			pMsgILC.h.subcode = 0x13;
		}
		// ------
		pMsgILC.h.sizeH		= SET_NUMBERH(sOfs);
		pMsgILC.h.sizeL		= SET_NUMBERL(sOfs);
		pMsgILC.btCount		= iCount;
		pMsgILC.Result		= btResult;
		pMsgILC.NumberH		= SET_NUMBERH(aTargetIndex);
		pMsgILC.NumberL		= SET_NUMBERL(aTargetIndex);
		// ------
		memcpy(pMsgILC.btName, lpObjTarget->Name, sizeof(pMsgILC.btName));
		memcpy(pMsgILC.szPShopText, lpObjTarget->m_szPShopText, sizeof(pMsgILC.szPShopText));
		memcpy(sendBuf, &pMsgILC, sizeof(pMsgILC));
		// ------
		gSendProto.DataSend(aSourceIndex, sendBuf, sOfs);
	}
}
// -----------------------------------------------------------------------------------------------------------------------------------------------------
void CPersonalShop::BuyListPersonalShop(PMSG_REQ_BUYLIST_FROM_PSHOP * lpMsg, int aSourceIndex)
{
	LPOBJ pObj = &gObj[aSourceIndex];
	// ------
	if( gDoPShopOpen == FALSE ) return;
	// ------
	if( gObjIsConnected(MAKE_NUMBERW(lpMsg->NumberH, lpMsg->NumberL)) == FALSE)
	{
		CLog.LogAddC(TColor.Red(), PShopModule, "[%s][%s] Shop List Request Failed: Seller is Offline (%d).", pObj->AccountID, pObj->Name,
			MAKE_NUMBERW(lpMsg->NumberH, lpMsg->NumberL));
		// ------
		SendPlShopBuyListResult(aSourceIndex, -1, 2, 0);
		return;
	}
	// ------
	LPOBJ lpObj = &gObj[MAKE_NUMBERW(lpMsg->NumberH, lpMsg->NumberL)];
	// ------
	if( lpObj->Type != OBJ_USER )
	{
		CLog.LogAddC(TColor.Red(), PShopModule, "[%s][%s] Shop List Request Failed: Seller isn't an Object User (%d).", pObj->AccountID, pObj->Name,
			MAKE_NUMBERW(lpMsg->NumberH, lpMsg->NumberL));
		// ------
		SendPlShopBuyListResult(aSourceIndex, -1, 2, 0);
		return;
	}
	// ------
	if( lpObj->CloseCount >= 0 )	
	{
		CLog.LogAddC(TColor.Red(), PShopModule, "[%s][%s] Shop Item Buy Request Failed: Seller is Closing (%d).", pObj->AccountID, pObj->Name,
			MAKE_NUMBERW(lpMsg->NumberH, lpMsg->NumberL));
		// ------
		SendPShopBuyItemResult(aSourceIndex, -1, 0, 2);
		return;
	}
	// ------
	if( pObj->CloseCount >= 0 )	
	{
		CLog.LogAddC(TColor.Red(), PShopModule, "[%s][%s] Shop Item Buy Request Failed: Buyer is Closing (%d)", pObj->AccountID, pObj->Name,
			MAKE_NUMBERW(lpMsg->NumberH, lpMsg->NumberL));
		// ------
		SendPShopBuyItemResult(aSourceIndex, -1, 0, 2);
		return;
	}
	// ------
	if( lpObj->m_Index == aSourceIndex )
	{
		CLog.LogAddC(TColor.Red(), PShopModule, "[%s][%s] Shop List Requested to (Him/Her)Self.", pObj->AccountID, pObj->Name);
		// ------
		SendPlShopBuyListResult(aSourceIndex, -1, 2, 0);
		return;
	}
	// ------
	if( lpObj->m_bPShopOpen == false )
	{
		CLog.LogAddC(TColor.Red(), PShopModule, "[%s][%s] Shop List Request Failed: [%s][%s] Didn't Open Shop", pObj->AccountID, pObj->Name,
			lpObj->AccountID, lpObj->Name);
		// ------
		SendPlShopBuyListResult(aSourceIndex, -1, 3, 0);
		return;
	}
	// ------
	if( (lpObj->Penalty &4) == 4 || (lpObj->Penalty&8) == 8 )
	{
		CLog.LogAddC(TColor.Red(), PShopModule, "[%s][%s] Shop List Request Failed: [%s][%s] is in Item Block", pObj->AccountID, pObj->Name,
			lpObj->AccountID, lpObj->Name);
		// ------
		SendPlShopBuyListResult(aSourceIndex, -1, 4, 0);
		return;
	}
	// ------
	if( !gObjFixInventoryPointer(aSourceIndex))
	{
		CLog.LogAddC(TColor.Red(), PShopModule, "[Fix Inv.Ptr] False Location - [%s](%d)", __FILE__, __LINE__);
	}
	// ------
	if( pObj->pTransaction == 1 )
	{
		CLog.LogAddC(TColor.Red(), PShopModule, "[%s][%s] Shop List Request Failed: Requester Transaction == 1, IF_STAT_TYPE : %d", pObj->AccountID,
			pObj->Name, pObj->m_IfState.type);
		return;
	}
	// ------
	CLog.LogAddC(TColor.Red(), PShopModule, "[%s][%s] is Receiving Shop List From [%s][%s]", pObj->AccountID, pObj->Name, lpObj->AccountID, lpObj->Name);
	// ------
	pObj->m_bPShopWantDeal		= true;
	pObj->m_iPShopDealerIndex	= lpObj->m_Index;
	// ------
	memcpy(pObj->m_szPShopDealerName, lpObj->Name, MAX_ACCOUNT_LEN);
	// ------
	SendPlShopBuyListResult(aSourceIndex, lpObj->m_Index, 1, false);
}
// -----------------------------------------------------------------------------------------------------------------------------------------------------
void CPersonalShop::SendPShopBuyItemResult(int aSourceIndex, int aTargetIndex, int iItemPos, BYTE btResult)
{
	PMSG_ANS_BUYITEM_FROM_PSHOP pMsg;
	// ------
	C1SubHeadSet((LPBYTE)&pMsg, 0x3F, 0x06, sizeof(pMsg));
	// ------
	pMsg.NumberH	= SET_NUMBERH(aTargetIndex);
	pMsg.NumberL	= SET_NUMBERL(aTargetIndex);
	pMsg.btItemPos	= iItemPos;
	pMsg.Result		= btResult;
	// ------
	ItemByteConvert(pMsg.cItemInfo, gObj[aSourceIndex].Inventory1[iItemPos]);
	// ------
	gSendProto.DataSend(aSourceIndex, (LPBYTE)&pMsg, pMsg.h.size);
}
// -----------------------------------------------------------------------------------------------------------------------------------------------------
void CPersonalShop::SendPShopSoldItemResult(int aSourceIndex, int aTargetIndex, int iItemPos)
{
	PMSG_ANS_SOLDITEM_FROM_PSHOP pMsg;
	// ------
	C1SubHeadSet((LPBYTE)&pMsg, 0x3F, 0x08, sizeof(pMsg));
	// ------
	pMsg.btItemPos = iItemPos;
	memcpy(pMsg.btName, gObj[aTargetIndex].Name, MAX_ACCOUNT_LEN);
	// ------
	gSendProto.DataSend(aSourceIndex, (LPBYTE)&pMsg, pMsg.h.size);
}
// -----------------------------------------------------------------------------------------------------------------------------------------------------
void CPersonalShop::BuyItemPersonalShop(PMSG_REQ_BUYITEM_FROM_PSHOP * lpMsg, int aSourceIndex)
{
	LPOBJ SrcObj = &gObj[aSourceIndex];
	// ------
	if( gDoPShopOpen == FALSE ) return;
	// ------
	if( gObjIsConnected( MAKE_NUMBERW(lpMsg->NumberH, lpMsg->NumberL)) == FALSE )
	{
		CLog.LogAddC(TColor.Red(), PShopModule, "[%s][%s] Shop Item Buy Request Failed: Seller is Offline (%d).", SrcObj->AccountID, SrcObj->Name,
			MAKE_NUMBERW(lpMsg->NumberH, lpMsg->NumberL));
		// ------
		SendPShopBuyItemResult(aSourceIndex, -1, 0, 2);
		return;
	}
	// ------
	LPOBJ lpObj = &gObj[MAKE_NUMBERW(lpMsg->NumberH, lpMsg->NumberL)];
	// ------
	if( lpObj->Type != OBJ_USER )
	{
		CLog.LogAddC(TColor.Red(), PShopModule, "[%s][%s] Shop Item Buy Request Failed: Seller isn't an Object User (%d).", SrcObj->AccountID,
			SrcObj->Name, MAKE_NUMBERW(lpMsg->NumberH, lpMsg->NumberL));
		// ------
		SendPShopBuyItemResult(aSourceIndex, -1, 0, 2);
		return;
	}
	// ------
	if( lpObj->CloseCount >= 0 )	
	{
		CLog.LogAddC(TColor.Red(), PShopModule, "[%s][%s] Shop Item Buy Request Failed: Seller is Closing (%d).", SrcObj->AccountID, SrcObj->Name,
			MAKE_NUMBERW(lpMsg->NumberH, lpMsg->NumberL));
		// ------
		SendPShopBuyItemResult(aSourceIndex, -1, 0,2);
		return;
	}
	// ------
	if( SrcObj->CloseCount >= 0 )	
	{
		CLog.LogAddC(TColor.Red(), PShopModule, "[%s][%s] Shop Item Buy Request Failed: Buyer is Closing (%d).", SrcObj->AccountID, SrcObj->Name,
			MAKE_NUMBERW(lpMsg->NumberH, lpMsg->NumberL));
		// ------
		SendPShopBuyItemResult(aSourceIndex, -1, 0, 2);
		return;
	}
	// ------
	if( lpObj->m_bPShopOpen == false )
	{
		CLog.LogAddC(TColor.Red(), PShopModule, "[%s][%s] Shop Item Buy Request Failed: [%s][%s] Didn't Open Shop.", SrcObj->AccountID, SrcObj->Name,
			lpObj->AccountID, lpObj->Name);
		// ------
		SendPShopBuyItemResult(aSourceIndex, lpObj->m_Index, 0, 3);
		return;
	}
	// ------
	char szName[MAX_ACCOUNT_LEN+1] = {0};
	// ------
	memcpy(szName, lpMsg->btName, sizeof(lpMsg->btName));
	szName[MAX_ACCOUNT_LEN] = 0;
	// ------
	int iITEM_LOG_TYPE;
	int iITEM_LOG_LEVEL;
	int iITEM_LOG_DUR;
	int iITEM_LOG_SERIAL;
	// ------
	if( strcmp(szName, lpObj->Name) )
	{
		CLog.LogAddC(TColor.Red(), PShopModule, "[%s][%s] Shop Item Buy Request Failed: Name Mismatch [%s]-[%s]", SrcObj->AccountID, SrcObj->Name,
			szName, lpObj->Name);
		// ------
		SendPShopBuyItemResult(aSourceIndex, lpObj->m_Index, 0, 6);
		return;
	}
	// ------
	if( (lpObj->Penalty &4) == 4 || (lpObj->Penalty &8) == 8 )
	{
		CLog.LogAddC(TColor.Red(), PShopModule, "[%s][%s] Shop Item Buy Request Failed: [%s][%s] is in Item Block", SrcObj->AccountID, SrcObj->Name,
			lpObj->AccountID, lpObj->Name);
		// ------
		SendPShopBuyItemResult(aSourceIndex, lpObj->m_Index, 0,9);
		return;
	}
	// ------
	if( (SrcObj->Penalty &4) == 4 || (SrcObj->Penalty &8) == 8 )
	{
		CLog.LogAddC(TColor.Red(), PShopModule, "[%s][%s] Shop Item Buy Request Failed: [%s][%s] is in Item Block", SrcObj->AccountID, SrcObj->Name,
			lpObj->AccountID, lpObj->Name);
		// ------
		SendPShopBuyItemResult(aSourceIndex, lpObj->m_Index, 0,9);
		return;
	}
	// ------
	EnterCriticalSection(&lpObj->m_critPShopTrade);
	// ------
	if( lpObj->m_bPShopTransaction == true )
	{
		CLog.LogAddC(TColor.Red(), PShopModule, "[%s][%s] Shop Item Buy Request Failed: [%s][%s] Already In Trade.", SrcObj->AccountID, SrcObj->Name,
			lpObj->AccountID, lpObj->Name);
		// ------
		SendPShopBuyItemResult(aSourceIndex, lpObj->m_Index, 0,4);
		// ------
		LeaveCriticalSection(&lpObj->m_critPShopTrade);
	}
	else
	{
		lpObj->m_bPShopTransaction = true;
		// ------
		LeaveCriticalSection(&lpObj->m_critPShopTrade);
		// ------
		if( gObjInventorySearchSerialNumber(&gObj[aSourceIndex], lpObj->Inventory1[lpMsg->btItemPos].GetNumber()) == FALSE )
		{
			GCReFillSend(aSourceIndex, SrcObj->Life, -3, 1, SrcObj->iShield);
			// ------
			CLog.LogAddC(TColor.Red(), PShopModule, "(Error): [%s][%s] CopyItem :: [%s](%d)", SrcObj->AccountID, SrcObj->Name, __FILE__, __LINE__);
			return;
		}
		// ------
		if( gObjInventorySearchSerialNumber(lpObj, lpObj->Inventory1[lpMsg->btItemPos].GetNumber()) == FALSE )
		{
			GCReFillSend(lpObj->m_Index, lpObj->Life, -3, 1, lpObj->iShield);
			// ------
			CLog.LogAddC(TColor.Red(), PShopModule, "(Error): [%s][%s] CopyItem :: [%s](%d)", lpObj->AccountID, lpObj->Name, __FILE__, __LINE__);
			return;
		}
		// ------
		if(gObjFixInventoryPointer(aSourceIndex) == false )
		{
			CLog.LogAddC(TColor.Red(), PShopModule, "[Fix Inv.Ptr] False Location - [%s](%d)", __FILE__, __LINE__);
		}
		// ------
		if( SrcObj->pTransaction == 1 )
		{
			CLog.LogAddC(TColor.Red(), PShopModule, "[%s][%s] Item Buy Request Failed: Requester Transaction == 1, IF_STAT_TYPE : %d", SrcObj->AccountID,
				SrcObj->Name, SrcObj->m_IfState.type);
			return;
		}
		// ------
		iITEM_LOG_TYPE		= 0;
		iITEM_LOG_LEVEL		= 0;
		iITEM_LOG_DUR		= 0;
		iITEM_LOG_SERIAL	= 0;
		// ------
		__try
		{
			if( PSHOP_RANGE(lpMsg->btItemPos) == FALSE )
			{
				SendPShopBuyItemResult(aSourceIndex, lpObj->m_Index, 0, 5);
				return;
			}
			// ------
			if( lpObj->m_bMapSvrMoveQuit == true )
			{
				SendPShopBuyItemResult(aSourceIndex, -1, 0, 2);
				return;
			}
			// ------
			if( lpObj->Inventory1[lpMsg->btItemPos].IsItem() == TRUE )
			{
				if( lpObj->Inventory1[lpMsg->btItemPos].m_iPShopValue <= 0 )
				{
					CLog.LogAddC(TColor.Red(), PShopModule, "[%s][%s] Shop Item Buy Request Failed: [%s][%s] Didn't Choose Item Price.", SrcObj->AccountID,
						SrcObj->Name, lpObj->AccountID, lpObj->Name);
					// ------
					SendPShopBuyItemResult(aSourceIndex, lpObj->m_Index, 0, 6);
					return;
				}
				// ------
				if( SrcObj->Money < lpObj->Inventory1[lpMsg->btItemPos].m_iPShopValue )
				{
					CLog.LogAddC(TColor.Red(), PShopModule, "[%s][%s] Shop Item Buy Request Failed: [%s][%s] Lack of Zen.", SrcObj->AccountID,
						SrcObj->Name, lpObj->AccountID, lpObj->Name);
					// ------
					SendPShopBuyItemResult(aSourceIndex, lpObj->m_Index, 0, 7);
					return;
				}
				// ------
				DWORD dwCost = lpObj->Inventory1[lpMsg->btItemPos].m_iPShopValue;
				// ------
				if( gObjCheckMaxZen(lpObj->m_Index, dwCost) == FALSE )
				{
					CLog.LogAddC(TColor.Red(), PShopModule, "[%s][%s] Shop Item Buy Request Failed: [%s][%s] Exceeding Zen of the Host", SrcObj->AccountID,
						SrcObj->Name, lpObj->AccountID, lpObj->Name);
					// ------
					SendPShopBuyItemResult(aSourceIndex, lpObj->m_Index, 0, 8);
					return;
				}
				// ------
				BYTE btNewItemPos	= 0;
				btNewItemPos		= gObjOnlyInventoryInsertItem(aSourceIndex, lpObj->Inventory1[lpMsg->btItemPos]);
				// ------
				if( btNewItemPos == 0xFF )
				{
					CLog.LogAddC(TColor.Red(), PShopModule, "[%s][%s] Shop Item Buy Request Failed: [%s][%s] No Room to Buy Item.", SrcObj->AccountID,
						SrcObj->Name, lpObj->AccountID, lpObj->Name);
					// ------
					SendPShopBuyItemResult(aSourceIndex, lpObj->m_Index, 0, 8);
					return;
				}
				// ------
				iITEM_LOG_TYPE		= lpObj->Inventory1[lpMsg->btItemPos].m_Type;
				iITEM_LOG_LEVEL		= lpObj->Inventory1[lpMsg->btItemPos].m_Level;
				iITEM_LOG_DUR		= lpObj->Inventory1[lpMsg->btItemPos].m_Durability;
				iITEM_LOG_SERIAL	= lpObj->Inventory1[lpMsg->btItemPos].m_Number;
				// ------
				gObjInventoryItemSet_PShop(lpObj->m_Index, lpMsg->btItemPos, -1);
				// ------
				lpObj->Inventory1[lpMsg->btItemPos].Clear();
				GCInventoryItemDeleteSend(lpObj->m_Index, lpMsg->btItemPos, TRUE);
				// ------
				SrcObj->Money	-= dwCost;
				lpObj->Money	+= dwCost;
				// ------
				GCMoneySend(aSourceIndex, SrcObj->Money);
				GCMoneySend(lpObj->m_Index, lpObj->Money);
				// ------
				SendPShopBuyItemResult(aSourceIndex, lpObj->m_Index, btNewItemPos, 1);
				// ------
				SendPShopSoldItemResult(lpObj->m_Index, aSourceIndex, lpMsg->btItemPos);
				// ------
				CLog.LogAddC(TColor.Green(), PShopModule, "[%s][%s][%s] Shop Item Buy Request Succeed: [%s][%s][%s] Price = (%d), Type:(%d)[%s] Level:(%d) Dur:(%d) Serial:(%d).",
					SrcObj->AccountID, SrcObj->Name, SrcObj->Ip_addr, lpObj->AccountID, lpObj->Name, lpObj->Ip_addr, dwCost, iITEM_LOG_TYPE,
					ItemAttribute[iITEM_LOG_TYPE].Name, iITEM_LOG_LEVEL, iITEM_LOG_DUR, iITEM_LOG_SERIAL);
				// ------
				if( PShop_CheckInventoryEmpty(lpObj->m_Index) == true )
				{
					CLog.LogAddC(TColor.Green(), PShopModule, "[%s][%s] Sold All Items - Auto Closing Shop.", lpObj->AccountID, lpObj->Name);
					// ------
					SendMsg.MessageOut(lpObj->m_Index, 0x01, "[PersonalShop] All items in Your Shop have been sold!");
					// ------
					lpObj->m_bPShopOpen = false;
					// ------
					memset(lpObj->m_szPShopText, 0, sizeof(lpObj->m_szPShopText));
					// ------
					SendPShopCloseResult(lpObj->m_Index, 1);
				}
				else
				{
					lpObj->m_bPShopItemChange = true;
				}
			}
		}
		// ------
		__finally
		{
			lpObj->m_bPShopTransaction = false;
		}
	}
}
// -----------------------------------------------------------------------------------------------------------------------------------------------------
void CPersonalShop::SendPShopDealerClosedShop(int aSourceIndex, int aTargetIndex)
{
	PMSG_REQ_DEALER_CLOSED_SHOP pMsg;
	// ------
	C1SubHeadSet((LPBYTE)&pMsg, 0x3F, 0x12, sizeof(pMsg));
	// ------
	pMsg.NumberH = SET_NUMBERH(aTargetIndex);
	pMsg.NumberL = SET_NUMBERL(aTargetIndex);
	// ------
	gSendProto.DataSend(aSourceIndex, (LPBYTE)&pMsg, pMsg.h.size);
}
// -----------------------------------------------------------------------------------------------------------------------------------------------------