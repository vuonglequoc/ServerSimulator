//**********************************************************************//
// Project:	HaRiBO MU GameServer - Season 4								//
// Company: RealCoderZ MU Development © 2010							//
// Description: MU MonsterItemMng Functions & Structures				//
// Decompilation By: Deathway											//
// Coded & Modified By:	WolF & M.E.S									//
//**********************************************************************//
#ifndef MONSTERITEMMNG_H
#define MONSTERITEMMNG_H


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "zzzitem.h"



#define MAX_ITEM_IN_MONSTER 1000
#define MAX_LEVEL_MONSTER 150

class CMonsterItemMng
{

public:

	CMonsterItemMng();
	virtual ~CMonsterItemMng();

	void Init();
	void Clear();
	BYTE InsertItem(int monsterlevel, int type, int index, int itemlevel,  int op1, int op2, int op3, BYTE * SocketOption);
	CItem* GetItem(int monsterlevel);
	void gObjGiveItemSearch(int monsterlevel, int maxlevel);

private:

	CItem* m_MonsterInvenItems[MAX_LEVEL_MONSTER];	// 4
	int m_iMonsterInvenItemCount[MAX_LEVEL_MONSTER];	//25C
	char MonsterName[255];	// 4B4


};


extern CMonsterItemMng g_MonsterItemMng;

#endif