//**********************************************************************//
// Project:	HaRiBO MU GameServer - Season 6								//
// Company: RealCoderZ MU Development © 2010							//
// Description: MU ObjBaseAttack Structures & Functions					//
// Coded & Modified By:	WolF & M.E.S									//
//**********************************************************************//
#ifndef OBJBASEATTACK_H
#define OBJBASEATTACK_H

#include "user.h"


class CObjBaseAttack
{
public:
	CObjBaseAttack(void);
	~CObjBaseAttack(void);
	bool DecreaseArrow(LPOBJ lpObj);
	bool CheckAttackArea(LPOBJ lpObj, LPOBJ lpTargetObj);
	bool PkCheck(LPOBJ lpObj, LPOBJ lpTargetObj);
	bool ResistanceCheck(LPOBJ lpObj, LPOBJ lpTargetObj, int skill);
	bool MissCheckPvP(LPOBJ lpObj, LPOBJ lpTargetObj, int skill, int skillSuccess, int magicsend, BOOL& bAllMiss);
	int GetTargetDefense(LPOBJ lpObj, LPOBJ lpTargetObj, unsigned char& MsgDamage);
	int GetPartyMemberCount(LPOBJ lpObj);
	virtual int MissCheck(LPOBJ lpObj, LPOBJ lpTargetObj, int skill, int skillSuccess, int magicsend, BOOL& bAllMiss);	// 4
};


#endif