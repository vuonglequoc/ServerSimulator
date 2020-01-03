//**********************************************************************//
// Project:	HaRiBO MU GameServer - Season 4								//
// Company: RealCoderZ MU Development © 2010							//
// Description: MU Mercenary Structures & Functions						//
// Decompilation By: Deathway											//
// Coded & Modified By:	WolF & M.E.S									//
//**********************************************************************//
#include "stdafx.h"
#include "Mercenary.h"
#include "protocol.h"
#include "GameMain.h"
#include "LogProc.H"
#include "Functions.H"


CMercenary g_CsNPC_Mercenary;

CMercenary::CMercenary()
{
	this->m_iMercenaryCount = 0;
}

CMercenary::~CMercenary()
{
	return;
}

BOOL CMercenary::CreateMercenary(int iIndex, int iMercenaryTypeIndex, BYTE cTX, BYTE cTY)
{
	LPOBJ lpObj = &gObj[iIndex];
	int iMonsterIndex = -1;
	BYTE cX = cTX;
	BYTE cY = cTY;

	BYTE btMapAttr = MapC[lpObj->MapNumber].GetAttr(cX, cY);

	if ( lpObj->MapNumber != MAP_INDEX_CASTLESIEGE )
	{
		::MsgOutput(iIndex, lMsg.Get(MSGGET(6, 91)));

		return FALSE;
	}

	return TRUE;
}

BOOL CMercenary::DeleteMercenary(int iIndex)
{
	if ( iIndex < 0 || iIndex > OBJMAX-1 )
	{
		return FALSE;
	}

	this->m_iMercenaryCount--;

	CLog.LogAdd("[CastleSiege] Mercenary is broken [%d] - [%d]", iIndex, this->m_iMercenaryCount);

	if ( this->m_iMercenaryCount < 0 )
	{
		this->m_iMercenaryCount = 0;
	}

	return TRUE;
}

int CMercenary::SearchEnemy(LPOBJ lpObj)
{
	return FALSE;
}

void CMercenary::MercenaryAct(int iIndex)
{
	if ( ::gObjIsConnected(iIndex) == FALSE )
	{
		return;
	}

	LPOBJ lpObj = &gObj[iIndex];


	if ( lpObj->VPCount2 < 1 )
	{
		return;
	}

	if ( this->SearchEnemy(lpObj) != 0 && lpObj->TargetNumber >= 0)
	{
		lpObj->m_ActState.Attack = 1;
		lpObj->NextActionTime = lpObj->m_AttackSpeed;
	}
	else
	{
		lpObj->NextActionTime = lpObj->m_MoveSpeed;
	}
}