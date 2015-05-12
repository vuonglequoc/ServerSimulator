//**********************************************************************//
// Project:	HaRiBO MU GameServer - Season 6								//
// Company: RealCoderZ MU Development © 2010							//
// Description: MU StatManage Functions & Structures					//
// Decompilation By: Deathway											//
// Coded & Modified By:	WolF & M.E.S									//
//**********************************************************************//
#include "../Header Files/stdafx.h"
#include "../Header Files/StatMng.H"
#include "../Header Files/User.H"
#include "../Header Files/GameMain.h"


CStatMng gStatMng;
//-------------------------------------------------------------------------------------------------------------------------------------------
CStatMng::CStatMng()
{
	this->Init();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
CStatMng::~CStatMng()
{
	return;
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void CStatMng::Init()
{
	int FirstState			= 2;
	int MagunfirstState		= 2;
	int iDarkLordFirstState = 2;

	for (int i=0; i<STAT_TABLE_SIZE; i++)
	{
		if(( (i+1) % 10) == 0)
		{
			FirstState				= FirstState			+ ( i*2 + 22 )	/ 400	+ 2;
			MagunfirstState			= MagunfirstState		+ ( i*2 + 22 )	/ 700	+ 2;
			iDarkLordFirstState		= iDarkLordFirstState	+ ( i*2 + 22 )	/ 500	+ 2;
		}

		this->gObjNormalAddStatTable[i]		= FirstState;
		this->gObjMagumAddStatTable[i]		= MagunfirstState;
		this->gObjDarkLordAddStatTable[i]	= iDarkLordFirstState;
	}

	FirstState			= 2;
	MagunfirstState		= 2;
	iDarkLordFirstState = 2;

	for (int i=0; i<STAT_TABLE_SIZE; i++)
	{
		if(( (i+1) % 10) == 0)
		{
			FirstState				= FirstState			+ ( i*2 + 22 )	/ 400	+ 2;
			MagunfirstState			= MagunfirstState		+ ( i*2 + 22 )	/ 700	+ 2;
			iDarkLordFirstState		= iDarkLordFirstState	+ ( i*2 + 22 )	/ 500	+ 2;
		}

		this->gObjNormalMinusStatTable[i]		= FirstState;
		this->gObjMagumMinusStatTable[i]		= MagunfirstState;
		this->gObjDarkLordMinusStatTable[i]		= iDarkLordFirstState;
	}
}
//-------------------------------------------------------------------------------------------------------------------------------------------
int CStatMng::GetMaxStat(int level, int Class)
{
	if( (level < 1) || (level > HRB_CharMaxLevel) )
		return FALSE;
	// ------
	if( Class == CLASS_MAGUMSA )
		return this->gObjMagumAddStatTable[level-1];
	// ------
	else if ( Class == CLASS_DARKLORD )
		return this->gObjDarkLordAddStatTable[level-1];
	// ------
	return this->gObjNormalAddStatTable[level-1];
}
//-------------------------------------------------------------------------------------------------------------------------------------------
int CStatMng::GetMaxMinusStat(int iLevel, int iClass)
{
	if( (iLevel < 1) || (iLevel > HRB_CharMaxLevel) )
		return FALSE;
	// ------
	if ( iClass == CLASS_MAGUMSA )
		return this->gObjMagumMinusStatTable[iLevel-1];
	// ------
	else if ( iClass == CLASS_DARKLORD )
		return this->gObjDarkLordMinusStatTable[iLevel-1];
	// ------
	return this->gObjNormalMinusStatTable[iLevel-1];
}
//-------------------------------------------------------------------------------------------------------------------------------------------