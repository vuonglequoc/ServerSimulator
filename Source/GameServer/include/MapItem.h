//---------------------------------------------------------------------------
// # Project:		HaRiBO MU GameServer - Supported Season 6
// # Company:		RealCoderZ MU Development © 2011
// # Description:	Map Item
// # Developed By:	M.E.S & WolF
//---------------------------------------------------------------------------

#ifndef MAPITEM_H
#define MAPITEM_H

#include "zzzitem.h"
#include "user.h"


class CMapItem : public CItem
{
public:
	CMapItem();
	virtual ~CMapItem();
	void Init();
	void CreateItem(int type, int level, int x, int y, float dur, BYTE Option1, BYTE Option2, BYTE Option3, BYTE NOption, BYTE SOption, DWORD number, BYTE ItemEffectEx, BYTE * SocketOption);
	void DropCreateItem(int type, int level, int x, int y, float dur, BYTE Option1, BYTE Option2, BYTE Option3, BYTE NOption, BYTE SOption, DWORD number, int PetLevel, int PetExp, BYTE ItemEffectEx, BYTE * SocketOptions);
public:
	BYTE px;
	BYTE py;
	bool live;
	bool Give;
	DWORD m_State;
	DWORD m_Time;
	int m_UserIndex;
	DWORD m_LootTime;
	VIEWPORT_PLAYER_STRUCT VpPlayer[MAX_VIEWPORT];
	int VPCount;
};


#endif