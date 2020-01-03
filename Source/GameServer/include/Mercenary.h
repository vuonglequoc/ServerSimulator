//**********************************************************************//
// Project:	HaRiBO MU GameServer - Season 4								//
// Company: RealCoderZ MU Development © 2010							//
// Description: MU Mercenary Structures & Functions						//
// Decompilation By: Deathway											//
// Coded & Modified By:	WolF & M.E.S									//
//**********************************************************************//
#ifndef MERCENARY_H
#define MERCENARY_H

#include "user.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMercenary
{
public:
	CMercenary();
	virtual ~CMercenary();
	//int __thiscall UseLifeStoneScroll(int);
	BOOL CreateMercenary(int iIndex, int iMercenaryTypeIndex, BYTE cTX, BYTE cTY);
	BOOL DeleteMercenary(int iIndex);
	int SearchEnemy(struct OBJECTSTRUCT* lpObj);
	void MercenaryAct(int iIndex);

private:
	int  m_iMercenaryCount;
};
extern CMercenary g_CsNPC_Mercenary;


#endif