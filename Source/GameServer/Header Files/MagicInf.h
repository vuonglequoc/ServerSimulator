//**********************************************************************//
// Project:	HaRiBO MU GameServer - Season 6								//
// Company: RealCoderZ MU Development © 2010							//
// Description: MU Class Magic Information & Structures					//
// Coded & Modified By:	WolF & M.E.S									//
//**********************************************************************//
#ifndef MAGICINF_H
#define MAGICINF_H

#include "MagicDamage.h"

#define MAX_MAGIC_INFO	3
#define	DBM_TYPE		0
#define DBM_LEVEL		2



class CMagicInf
{
public:
	CMagicInf();
	virtual ~CMagicInf();
	bool IsMagic();
	void Clear();
	int Set(WORD aSkill, BYTE aLevel);
	int Set(WORD aType, WORD aIndex, BYTE aLevel);
	int GetDamage();
	
	CMagicInf & operator = (CMagicInf & __that)
	{
		this->m_Level = __that.m_Level;
		this->m_Skill = __that.m_Skill;
		this->m_DamageMin = __that.m_DamageMin;
		this->m_DamageMax = __that.m_DamageMax;

		return *this;
	};

public:
	BYTE m_Level;
	WORD m_Skill;
	int m_DamageMin;
	int m_DamageMax;
};
extern CMagicInf DefMagicInf[MAX_SKILL];

int GetSkillNumberInex(int type, int Index, int level);
void MagicByteConvert(BYTE* buf, CMagicInf* lpMagic, int maxmagic);


#endif