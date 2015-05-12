//**********************************************************************//
// Project:	HaRiBO MU GameServer - Season 6								//
// Company: RealCoderZ MU Development © 2010							//
// Description: Game Shops & NPCs Structures							//
// Coded & Modified By:	WolF & M.E.S									//
//**********************************************************************//
#ifndef __SHOP_H__
#define __SHOP_H__

#include "zzzitem.h"

#define MAX_SHOP 29
#define MAX_ITEM_IN_SHOP 120

class CShop
{
public:
	CShop(void);
	~CShop(void);

	void	Init();
	BOOL	LoadShopItem(char* filename);
	BOOL	InsertItem(int Type, int Index, int Level, int Dur, int Opt1, int Opt2 ,int Opt3, int Exc, int Anc, BYTE Socket[5]);
	int		InentoryMapCheck(int CordX, int CordY, int Width, int Height);

public:
	BYTE ShopInventoryMap[MAX_ITEM_IN_SHOP];
	int ItemCount;
	CItem m_item[MAX_ITEM_IN_SHOP];
	BYTE SendItemData[MAX_ITEM_IN_SHOP*(MAX_ITEM_INFO+1)];
	int SendItemDataLen;
};

BOOL ShopDataLoad();
extern CShop ShopC[MAX_SHOP];

#endif