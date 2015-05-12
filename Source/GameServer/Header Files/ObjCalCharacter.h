//**********************************************************************//
// Project:	HaRiBO MU GameServer - Season 6								//
// Company: RealCoderZ MU Development © 2010							//
// Description: MU Object Cal Character Structures & Functions			//
// Coded & Modified By:	WolF & M.E.S									//
//**********************************************************************//
#ifndef OBJCALCHARACTER_H
#define OBJCALCHARACTER_H

#include "user.h"
#include "zzzitem.h"

class CCalCharacter
{
public:
	CCalCharacter(void);
	~CCalCharacter(void);
	// ----
	void gObjCalCharacter(int aIndex);
};

extern CCalCharacter gCalCharacter;


void GObjExtItemApply(LPOBJ lpObj);
void gObjCalcSetItemStat(LPOBJ lpObj);
void gObjGetSetItemOption(LPOBJ lpObj, LPBYTE pSetOptionTable , LPBYTE pSetOptionCountTable , int * pSetOptionCount);
void gObjCalcSetItemOption(LPOBJ lpObj);
void gObjSetItemStatPlusSpecial(LPOBJ lpObj, int option, int ivalue);
void gObjSetItemPlusSpecial(LPOBJ lpObj, int option, int ivalue);
void gObjSetItemApply(LPOBJ lpObj);
BOOL gObjValidItem(LPOBJ lpObj, CItem * lpItem, int pos);
void gObjCalcShieldPoint(LPOBJ lpObj);

#endif