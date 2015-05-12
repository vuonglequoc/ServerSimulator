//**********************************************************************//
// Project:	HaRiBO MU GameServer - Season 6								//
// Company: RealCoderZ MU Development © 2010							//
// Description: MU MonsterAttr Structures & Functions					//
// Coded & Modified By:	WolF & M.E.S									//
//**********************************************************************//
#ifndef MONSTERATTR_H
#define MONSTERATTR_H

#include "user.h"

typedef struct MONSTER_ATTRIBUTE
{
  int m_Index;
  int m_Rate;
  char m_Name[20];
  int m_Level;
  int m_iAINumber;
  int m_iScriptHP;
  int m_Hp;
  int m_Mp;
  int m_DamageMin;
  int m_DamageMax;
  int m_Defense;
  int m_MagicDefense;
  int m_AttackRating;
  int m_Successfulblocking;
  int m_MoveRange;
  int m_AttackRange;
  int m_AttackType;
  int m_ViewRange;
  int m_MoveSpeed;
  int m_AttackSpeed;
  int m_RegenTime;
  int m_Attribute;
  int m_ItemRate;
  int m_MoneyRate;
  int m_MaxItemLevel;
  BYTE m_Resistance[MAX_RESISTENCE_TYPE];
  DWORD m_MonsterSkill;

} MONSTER_ATTRIBUTE, * LPMONSTER_ATTRIBUTE;


class CMonsterAttr
{
public:
	LPMONSTER_ATTRIBUTE GetAttr(int aClass);
	void LoadAttr(char* Buffer, int iSize);
	void LoadAttr(char* filename);
	CMonsterAttr(void);
	~CMonsterAttr(void);
private:
	MONSTER_ATTRIBUTE m_MonsterAttr[MAX_MONSTER_TYPE];
};


#endif