//**********************************************************************//
// Project:	HaRiBO MU GameServer - Season 6								//
// Company: RealCoderZ MU Development © 2010							//
// Description: MU Class Magic Information & Structures					//
// Coded & Modified By:	WolF & M.E.S									//
//**********************************************************************//
#include "stdafx.h"
#include "MagicInf.h"
#include "LogProc.H"
#include "GameMain.h"


CMagicInf DefMagicInf[MAX_SKILL];
// -----------------------------------------------------------------------------------------------------------------------
CMagicInf::CMagicInf()
{
	this->Clear();
}
// -----------------------------------------------------------------------------------------------------------------------
CMagicInf::~CMagicInf()
{
	this->m_DamageMin  = 0;
	this->m_DamageMax  = 0;
}
// -----------------------------------------------------------------------------------------------------------------------
void CMagicInf::Clear()
{
	this->m_Skill  = -1;
	this->m_Level  = 0;
}
// -----------------------------------------------------------------------------------------------------------------------
bool CMagicInf::IsMagic()
{
	if ( this->m_Skill == 0xFFFF )
	{
		return FALSE;
	}
	return TRUE;
}
// -----------------------------------------------------------------------------------------------------------------------
int GetSkillNumberInex(int type, int Index, int level)
{
	int skillnumber = -1;
	// ----
	switch ( type )
	{
	case 15: // Scrolls
		switch ( Index )
		{
		case 0:		skillnumber = 1;	break;
		case 1:		skillnumber = 2;	break;
		case 2:		skillnumber = 3;	break;
		case 3:		skillnumber = 4;	break;
		case 4:		skillnumber = 5;	break;
		case 5:		skillnumber = 6;	break;
		case 6:		skillnumber = 7;	break;
		case 7:		skillnumber = 8;	break;
		case 8:		skillnumber = 9;	break;
		case 9:		skillnumber = 10;	break;
		case 10:	skillnumber = 11;	break;
		case 11:	skillnumber = 12;	break;
		case 12:	skillnumber = 13;	break;
		case 13:	skillnumber = 14;	break;
		case 14:	skillnumber = 15;	break;
		case 15:	skillnumber = 16;	break;
		case 16:	skillnumber = 38;	break;
		case 17:	skillnumber = 39;	break;
		case 18:	skillnumber = 40;	break;
		case 19:	skillnumber = 215;	break;
		case 20:	skillnumber = 214;	break;
		case 21:	skillnumber = 230;	break;
		case 22:	skillnumber = 217;	break;
		case 23:	skillnumber = 218;	break;
		case 24:	skillnumber = 219;	break;
		case 25:	skillnumber = 220;	break;
		case 26:	skillnumber = 221;	break;
		case 27:	skillnumber = 222;	break;
		case 28:	skillnumber = 233;	break;
		case 29:	skillnumber = 237;	break;
		case 30:	skillnumber = 262;	break;
		case 31:	skillnumber = 263;	break;
		case 32:	skillnumber = 264;	break;
		case 33:	skillnumber = 265;	break;
		case 34:	skillnumber = 266;	break;
		case 35:	skillnumber = 267;	break;
		case 36:	skillnumber = 268;	break;
		}
		break;
	case 12: // Orbs
		switch ( Index )
		{
			case 7:		skillnumber = 41;	break;
			case 8:		skillnumber = 26;	break;
			case 9:		skillnumber = 27;	break;
			case 10:	skillnumber = 28;	break;
			case 12:	skillnumber = 42;	break;
			case 13:	skillnumber = 47;	break;
			case 14:	skillnumber = 48;	break;
			case 19:	skillnumber = 43;	break;
			case 17:	skillnumber = 52;	break;
			case 18:	skillnumber = 51;	break;
			case 16:	skillnumber = 55;	break;
			case 21:	skillnumber = 61;	break;
			case 22:	skillnumber = 63;	break;
			case 23:	skillnumber = 64;	break;
			case 24:	skillnumber = 65;	break;
			case 11:	skillnumber = level + 30;	break;
			case 35:	skillnumber = 78;	break;
			case 44:    skillnumber = 232;  break;
			case 45:    skillnumber = 235;  break;
			case 46:    skillnumber = 234;  break;
			case 47:    skillnumber = 236;  break;
			case 48:    skillnumber = 238;  break;
		}
		break;
	}
	return skillnumber;
}
// -----------------------------------------------------------------------------------------------------------------------
int CMagicInf::Set(WORD aType, WORD aIndex, BYTE aLevel)
{
	if ( this->m_Skill != 0xFFFF )
	{
		CLog.LogAdd(lMsg.Get(MSGGET(1, 203)), __FILE__, __LINE__);
		return -1;
	}

	this->m_Level = aLevel;
	this->m_Skill = GetSkillNumberInex(aType, aIndex, aLevel);
	
	if ( this->m_Skill < 0 )
	{
		this->m_Skill = 1;
	}

	int damage = MagicDamageC.SkillGet(this->m_Skill);
	this->m_DamageMin = damage;
	this->m_DamageMax = damage + damage/2;

	return this->m_Skill;
}
// -----------------------------------------------------------------------------------------------------------------------
int CMagicInf::GetDamage()
{
	int damage = 0;

	if ( this->m_Skill == 0xFFFF )
	{
		CLog.LogAdd(lMsg.Get(MSGGET(1, 202)), __FILE__, __LINE__);
		return FALSE;
	}

	int subd = this->m_DamageMax - this->m_DamageMin;

	__try
	{
		damage = this->m_DamageMin + (rand() % 1);
	}
	__except(subd=1,1)
	{
		CLog.LogAdd(lMsg.Get(MSGGET(0, 111)), __FILE__, __LINE__);
	}

	return damage;
}
// -----------------------------------------------------------------------------------------------------------------------
int CMagicInf::Set(WORD aSkill, BYTE aLevel)
{
	if ( this->m_Skill != 0xFFFF )
	{
		return -1;
	}

	this->m_Skill = aSkill;
	this->m_Level = aLevel;
	int damage = MagicDamageC.SkillGet(this->m_Skill);
	this->m_DamageMin = damage;
	this->m_DamageMax = damage + damage/2;

	return this->m_Skill;
}
// -----------------------------------------------------------------------------------------------------------------------
void MagicByteConvert(BYTE* buf, CMagicInf* lpMagic, int maxmagic)
{
	int n = 0;

	for (int index = 0;index < maxmagic;index++)
	{
		if (lpMagic[index].IsMagic() == TRUE )
		{
			buf[n] = HIBYTE(lpMagic[index].m_Skill);
			n++;

			buf[n] = LOBYTE(lpMagic[index].m_Skill);
			n++;

			buf[n] = lpMagic[index].m_Level << 3;
			n++;
		}
		else
		{
			buf[n] = 0xFF;
			n++;

			buf[n] = 0xFF;
			n++;

			buf[n] = 0xFF;
			n++;
		}
	}
}
// -----------------------------------------------------------------------------------------------------------------------