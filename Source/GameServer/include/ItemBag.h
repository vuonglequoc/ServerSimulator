//---------------------------------------------------------------------------
// # Project:		HaRiBO MU GameServer - Supported Season 6
// # Company:		RealCoderZ MU Development © 2011
// # Description:	Event Item Bags System
// # Developed By:	WolF & M.E.S
//---------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------------------------------------------
#ifndef EVENT_ITEM_BAGS_H
#define EVENT_ITEM_BAGS_H
// -----------------------------------------------------------------------------------------------------------------------------------------

#include "StdAfx.H"

// -----------------------------------------------------------------------------------------------------------------------------------------
#define MAX_ITEMBAG_ATTR	150
#define EVENT_ITEM_MODULE	"ITEM_BAGS"
// -----------------------------------------------------------------------------------------------------------------------------------------

class CEventItemBagAttr
{
public:
	CEventItemBagAttr()
	{
		this->m_Type		= 0;
		this->m_Index		= 0;
		this->m_MinLevel	= 0;
		this->m_MaxLevel	= 0;
		this->m_Skill		= 0;
		this->m_Luck		= 0;	
		this->m_AddOption	= 0;
		this->m_Excelent	= 0;
	}
	// ------
	BYTE	m_Type;
	BYTE	m_Index;
	BYTE	m_MinLevel;
	BYTE	m_MaxLevel;
	BYTE	m_Level;
	BYTE	m_Skill;
	BYTE	m_Luck;
	BYTE	m_AddOption;
	BYTE	m_Excelent;
	BYTE	m_Ancient;
};
// -----------------------------------------------------------------------------------------------------------------------------------------
class CItemBag
{
public:
	CItemBag();
	virtual ~CItemBag();
	// ------
	void Init(const char* EventItemBagFile);
	void LoadItem(const char* szFileName);
	// ------
	int GetEventBagCount()
	{
		return this->EventBagObjectCount;
	};
	// ------
	BYTE GetType(int n);
	BYTE GetIndex(int n);
	BYTE GetLevel(int n);
	BYTE GetMinLevel(int n);
	BYTE GetMaxLevel(int n);
	BYTE GetSkill(int n);
	BYTE GetLuck(int n);
	BYTE GetAddOpt(int n);
	BYTE GetExc(int n);
	// ------
	int GetNrItemCount()	{return this->EventBagNrItemCount;}
	int GetExItemCount()	{return this->EventBagExItemCount;}
	// ------
private:
	CEventItemBagAttr	EventBagObject[MAX_ITEMBAG_ATTR];
	int					EventBagObjectCount;
	int					EventBagNrItemCount;
	int					EventBagExItemCount;
};
// -----------------------------------------------------------------------------------------------------------------------------------------
#endif
// -----------------------------------------------------------------------------------------------------------------------------------------