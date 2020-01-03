//---------------------------------------------------------------------------
// # Project:		HaRiBO MU GameServer - Supported Season 6
// # Company:		RealCoderZ MU Development © 2011
// # Description:	Event Item Bags System
// # Developed By:	WolF & M.E.S
//---------------------------------------------------------------------------
#include "stdafx.h"
#include "ItemBag.H"
#include "GameMain.H"
#include "ReadScript.h"
#include "LogProc.H"


// -----------------------------------------------------------------------------------------------------------------------------------------
CItemBag::CItemBag()
{
	return;
}
// -----------------------------------------------------------------------------------------------------------------------------------------
CItemBag::~CItemBag()
{
	return;
}
// -----------------------------------------------------------------------------------------------------------------------------------------
void CItemBag::Init(const char* EventItemBagFile)
{
	char gItemBagName[255];

	this->EventBagObjectCount = 0;
	this->EventBagNrItemCount = 0;
	this->EventBagExItemCount = 0;

	wsprintf(gItemBagName, "..//Data//EventItemBags//%s", EventItemBagFile);

	LoadItem(gItemBagName);
}
// -----------------------------------------------------------------------------------------------------------------------------------------
void CItemBag::LoadItem(const char* szFileName)
{
	int Token;
	int n = 0;

	if( (SMDFile = fopen(szFileName, "r")) == NULL )
	{
		CLog.LogAddC(TColor.DarkRed(), EVENT_ITEM_MODULE, "Error() Loading EventItemBag: [%s].", szFileName);
		return ;
	}

	while(true)
	{
		Token = GetToken();

		if ( Token == 2 )
			break;

		if ( Token == 1 )
		{
			n = this->EventBagObjectCount;

			this->EventBagObject[n].m_Type			= (BYTE)TokenNumber;

			Token = GetToken();
			this->EventBagObject[n].m_Index			= (BYTE)TokenNumber;

			Token = GetToken();
			this->EventBagObject[n].m_MinLevel		= (BYTE)TokenNumber;

			Token = GetToken();
			this->EventBagObject[n].m_MaxLevel		= (BYTE)TokenNumber;

			Token = GetToken();
			this->EventBagObject[n].m_Skill			= (BYTE)TokenNumber;

			Token = GetToken();
			this->EventBagObject[n].m_Luck			= (BYTE)TokenNumber;

			Token = GetToken();
			this->EventBagObject[n].m_AddOption		= (BYTE)TokenNumber;

			Token = GetToken();
			this->EventBagObject[n].m_Excelent		= (BYTE)TokenNumber;

			Token = GetToken();
			this->EventBagObject[n].m_Ancient		= (BYTE)TokenNumber;

			if ( this->EventBagObject[n].m_Luck == 1 && this->EventBagObject[n].m_Excelent != 0 )
			{
				this->EventBagExItemCount++;
			}
			else
			{
				this->EventBagNrItemCount++;
			}

			this->EventBagObjectCount++;

			if ( this->EventBagObjectCount > MAX_ITEMBAG_ATTR-1 )
			{
				break;
			}
		}
	}

	fclose(SMDFile);

	CLog.LogAddC(TColor.Green(), EVENT_ITEM_MODULE, "EventItemBag: [%s] Loaded Successfully.", szFileName);
}
// -----------------------------------------------------------------------------------------------------------------------------------------
BYTE CItemBag::GetType(int n)
{
	if ( n < 0 || n > MAX_ITEMBAG_ATTR-1 )
		return FALSE;

	return this->EventBagObject[n].m_Type;
}
// -----------------------------------------------------------------------------------------------------------------------------------------
BYTE CItemBag::GetIndex(int n)
{
	if ( n < 0 || n > MAX_ITEMBAG_ATTR-1 )
		return FALSE;

	return this->EventBagObject[n].m_Index;
}
// -----------------------------------------------------------------------------------------------------------------------------------------
BYTE CItemBag::GetLevel(int n)
{
	if ( n < 0 || n > MAX_ITEMBAG_ATTR-1 )
		return FALSE;

	if ( this->EventBagObject[n].m_MinLevel == this->EventBagObject[n].m_MaxLevel )
	{
		return this->EventBagObject[n].m_MinLevel;
	}

	int Sub = (this->EventBagObject[n].m_MaxLevel - this->EventBagObject[n].m_MinLevel) + 1;

	int level = this->EventBagObject[n].m_MinLevel + (rand() % Sub);

	return level;
}
// -----------------------------------------------------------------------------------------------------------------------------------------
BYTE CItemBag::GetMinLevel(int n)
{
	if ( n < 0 || n > MAX_ITEMBAG_ATTR-1 )
		return FALSE;

	return this->EventBagObject[n].m_MinLevel;
}
// -----------------------------------------------------------------------------------------------------------------------------------------
BYTE CItemBag::GetMaxLevel(int n)
{
	if ( n < 0 || n > MAX_ITEMBAG_ATTR-1 )
		return FALSE;

	return this->EventBagObject[n].m_MaxLevel;
}
// -----------------------------------------------------------------------------------------------------------------------------------------
BYTE CItemBag::GetSkill(int n)
{
	if ( n < 0 || n > MAX_ITEMBAG_ATTR-1 )
		return FALSE;

	return this->EventBagObject[n].m_Skill;
}
// -----------------------------------------------------------------------------------------------------------------------------------------
BYTE CItemBag::GetLuck(int n)
{
	if ( n < 0 || n > MAX_ITEMBAG_ATTR-1 )
		return FALSE;

	return this->EventBagObject[n].m_Luck;
}
// -----------------------------------------------------------------------------------------------------------------------------------------
BYTE CItemBag::GetAddOpt(int n)
{
	if ( n < 0 || n > MAX_ITEMBAG_ATTR-1 )
		return FALSE;

	return this->EventBagObject[n].m_AddOption;
}
// -----------------------------------------------------------------------------------------------------------------------------------------
BYTE CItemBag::GetExc(int n)
{
	if ( n < 0 || n > MAX_ITEMBAG_ATTR-1 )
		return FALSE;

	return this->EventBagObject[n].m_Excelent;
}
// -----------------------------------------------------------------------------------------------------------------------------------------