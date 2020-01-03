//**********************************************************************//
// Project:	HaRiBO MU GameServer - Season 6								//
// Company: RealCoderZ MU Development © 2010							//
// Description: MU ObjAttack Structures & Functions						//
// Coded & Modified By:	WolF & M.E.S									//
//**********************************************************************//
#ifndef OBJATTACK_H
#define OBJATTACK_H

#include "MagicInf.h"
#include "ObjBaseAttack.h"


class CObjAttack : public CObjBaseAttack
{
public:
	CObjAttack(void);
	~CObjAttack(void);
	bool Attack(LPOBJ lpObj, LPOBJ lpTargetObj, CMagicInf* lpMagic,  int magicsend, unsigned char MSBFlag, int AttackDamage, BOOL bCombo);
	int GetAttackDamage(LPOBJ lpObj, int targetDefense, BYTE& effect, bool bIsOnDuel, CMagicInf* lpMagic);
	int GetAttackDamageWizard(LPOBJ lpObj, int targetDefense, CMagicInf* lpMagic, BYTE& effect, bool bIsOnDuel);
	int GetShieldDamage(LPOBJ lpObj, LPOBJ lpTargetObj, int iAttackDamage);
};

extern CObjAttack gclassObjAttack;


#endif