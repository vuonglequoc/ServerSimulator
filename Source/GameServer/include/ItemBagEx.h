//---------------------------------------------------------------------------
// # Project:		HaRiBO MU GameServer - Supported Season 6
// # Company:		RealCoderZ MU Development © 2011
// # Description:	Event Item Bags Ex System
// # Developed By:	WolF & M.E.S
//---------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------------------------------------------
#ifndef EVENT_ITEM_BAGEX_H
#define EVENT_ITEM_BAGEX_H
// -----------------------------------------------------------------------------------------------------------------------------------------

#include "StdAfx.H"
#include "ItemBag.H"
#include "MapClass.H"
#include "User.H"
#include "zzzitem.H"


class CEventItemBagAttrExc
{
public:
	CEventItemBagAttrExc()
	{
		this->m_Type		= 0;
		this->m_Index		= 0;
		this->m_MinLevel	= 0;
		this->m_MaxLevel	= 0;
		this->m_IsSkill		= 0;
		this->m_IsLuck		= 0;
		this->m_IsOption	= 0;
		this->m_IsExcItem	= 0;
	}
	// ------
	char	m_Type;
	short	m_Index;
	char	m_MinLevel;
	char	m_MaxLevel;
	char	m_IsSkill;
	char	m_IsLuck;
	char	m_IsOption;
	char	m_IsExcItem;
};
// -----------------------------------------------------------------------------------------------------------------------------------------
class CItemBagDropMapInfo
{
public:
	CItemBagDropMapInfo()
	{
		this->Init();
	};
	// ------
	void Init()
	{
		this->m_bIsDrop			= false;
		this->m_MinMonsterLevel = 0;
		this->m_MaxMonsterLevel = 0;
	};
	// ------
	char m_bIsDrop;

	BYTE m_MinMonsterLevel;
	BYTE m_MaxMonsterLevel;
	BYTE m_MonsterLevel;
};
// -----------------------------------------------------------------------------------------------------------------------------------------
class CItemBagEx
{
public:
	CItemBagEx();
	virtual ~CItemBagEx();
	// ------
	void Init(const char* EventItemBagExFile);
	void InitKundun(const char* EventItemBagExFile);
	void Load(const char* szFileName);
	void LoadKundun(const char* szFileName);
	#if(GS_CASTLE)
	int DropCastleHuntZoneBossReward(int aIndex,BYTE btMapNumber, BYTE cX, BYTE cY);
	#endif
	// ------
	bool DropEventItem(int aIndex);
	int DropItem(int aIndex);
	// ------
	int PickItem(CItem& objItem, int& nItemIndex);
	// ------
	int GetMinLevel(int n);
	int GetMaxLevel(int n);
	// ------
private:
	int GetBagCount()	{ return this->m_IBBagObjectCount; }
	// ------
	bool IsEnableEventItemDrop(int aIndex);
	BYTE GetLevel(int n);
	// ------
	bool						m_IBLoad;
	CItemBagDropMapInfo			DropMapInfo[MAX_NUMBER_MAP];
	CEventItemBagAttrExc		EventBagObject[MAX_ITEMBAG_ATTR];
	char						m_IBEventName[255];
	int							m_IBEventItemType;
	int							m_IBEventItemLevel;
	int							m_IBDropZen;
	int							m_IBDropRate;
	int							m_IBItemDropRate;
	int							m_IBExcItemDropRate;
	int							m_IBBagObjectCount;
	// ------
};
// -----------------------------------------------------------------------------------------------------------------------------------------
#endif
// -----------------------------------------------------------------------------------------------------------------------------------------