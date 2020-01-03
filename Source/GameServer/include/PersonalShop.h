//---------------------------------------------------------------------------
// # Project:		HaRiBO MU GameServer - Supported Season 6
// # Company:		RealCoderZ MU Development © 2011
// # Description:	Personal Shop Class
// # Developed By:	WolF & M.E.S
//---------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------------------------------------------------------
#ifndef PERSONAL_SHOP_H
#define PERSONAL_SHOP_H
// -----------------------------------------------------------------------------------------------------------------------------------------------------

#include "StdAfx.H"
#include "prodef.H"

// -----------------------------------------------------------------------------------------------------------------------------------------------------
#define PShopModule		"PersonalShop"
// -----------------------------------------------------------------------------------------------------------------------------------------------------
struct PMSG_ANS_PSHOP_SETITEMPRICE
{
	PBMSG_HEAD2		h;
	BYTE			btResult;
	BYTE			btItemPos;
};
// -----------------------------------------------------------------------------------------------------------------------------------------------------
struct PMSG_ANS_PSHOP_OPEN
{
	PBMSG_HEAD2		h;
	BYTE			btResult;
};
// -----------------------------------------------------------------------------------------------------------------------------------------------------
struct PMSG_ANS_PSHOP_CLOSE
{
	PBMSG_HEAD2 h;
	BYTE btResult;
	BYTE NumberH;
	BYTE NumberL;
};
// -----------------------------------------------------------------------------------------------------------------------------------------------------
struct PMSG_REQ_PSHOP_SETITEMPRICE
{
	PBMSG_HEAD2		h;
	BYTE			btItemPos;
	BYTE			sItemPrice1;
	BYTE			sItemPrice2;
	BYTE			sItemPrice3;
	BYTE			sItemPrice4;
};
// -----------------------------------------------------------------------------------------------------------------------------------------------------
struct PMSG_REQ_PSHOP_OPEN
{
	PBMSG_HEAD2		h;
	char			szPShopText[36];
};
// -----------------------------------------------------------------------------------------------------------------------------------------------------
struct PMSG_ANS_PSHOP_TEXT_CHANGED
{
	PBMSG_HEAD2		h;
	BYTE			NumberH;
	BYTE			NumberL;
	BYTE			btPShopText[36];
	BYTE			btName[10];
};
// -----------------------------------------------------------------------------------------------------------------------------------------------------
struct PMSG_ANS_BUYLIST_FROM_PSHOP
{
	PWMSG_HEAD2 h;
	BYTE Result;
	BYTE NumberH;
	BYTE NumberL;
	BYTE btName[10];
	char szPShopText[36];
	BYTE btCount;
};
// -----------------------------------------------------------------------------------------------------------------------------------------------------
struct PMSG_BUYLIST_FROM_PSHOP
{
	BYTE Pos;
	BYTE ItemInfo[12];
	int PShopItemValue;
};
// -----------------------------------------------------------------------------------------------------------------------------------------------------
struct PMSG_REQ_BUYLIST_FROM_PSHOP
{
	PBMSG_HEAD2 h;
	BYTE NumberH;
	BYTE NumberL;
	BYTE btName[10];
};
// -----------------------------------------------------------------------------------------------------------------------------------------------------
struct PMSG_REQ_BUYITEM_FROM_PSHOP
{
	PBMSG_HEAD2 h;
	BYTE NumberH;
	BYTE NumberL;
	BYTE btName[10];
	BYTE btItemPos;
};
// -----------------------------------------------------------------------------------------------------------------------------------------------------
struct PMSG_ANS_BUYITEM_FROM_PSHOP
{
	PBMSG_HEAD2 h;
	BYTE Result;
	BYTE NumberH;
	BYTE NumberL;
	BYTE cItemInfo[12];
	BYTE btItemPos;
};
// -----------------------------------------------------------------------------------------------------------------------------------------------------
struct PMSG_ANS_SOLDITEM_FROM_PSHOP
{
	PBMSG_HEAD2 h;
	BYTE btItemPos;
	BYTE btName[10];
};
// -----------------------------------------------------------------------------------------------------------------------------------------------------
struct PMSG_REQ_DEALER_CLOSED_SHOP
{
	PBMSG_HEAD2 h;
	BYTE NumberH;
	BYTE NumberL;
};
// -----------------------------------------------------------------------------------------------------------------------------------------------------
class CPersonalShop
{
public:
	CPersonalShop();
	virtual ~CPersonalShop();
	// -----
	void	SendPShopSetItemPriceResult(int aIndex, BYTE btResult, BYTE btItemPos);
	void	SetPersonalShopItemPrice(PMSG_REQ_PSHOP_SETITEMPRICE* lpMsg, int aIndex);
	void	SendPShopOpenResult(int aIndex, BYTE btResult);
	void	OpenPersonalShop(PMSG_REQ_PSHOP_OPEN* lpMsg, int aIndex);
	void	SendPShopCloseResult(int aIndex, BYTE btResult);
	void	ClosePersonalShop(int aIndex);
	void	SendPlShopBuyListResult(int aSourceIndex, int aTargetIndex, BYTE btResult, bool bResend);
	void	BuyListPersonalShop(PMSG_REQ_BUYLIST_FROM_PSHOP * lpMsg, int aSourceIndex);
	void	SendPShopSoldItemResult(int aSourceIndex, int aTargetIndex, int iItemPos);
	void	SendPShopBuyItemResult(int aSourceIndex, int aTargetIndex, int iItemPos, BYTE btResult);
	void	BuyItemPersonalShop(PMSG_REQ_BUYITEM_FROM_PSHOP* lpMsg, int aSourceIndex);
	void	SendPShopDealerClosedShop(int aSourceIndex, int aTargetIndex);
};
// -----------------------------------------------------------------------------------------------------------------------------------------------------
extern CPersonalShop CPShop;
// -----------------------------------------------------------------------------------------------------------------------------------------------------
#endif
// -----------------------------------------------------------------------------------------------------------------------------------------------------