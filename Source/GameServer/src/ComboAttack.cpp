//**********************************************************************//
// Project:	HaRiBO MU GameServer - Season 6								//
// Company: RealCoderZ MU Development © 2010							//
// Description: MU ComboAttack Structures & Functions					//
// Coded & Modified By:	WolF & M.E.S									//
//**********************************************************************//
#include "stdafx.h"
#include "ComboAttack.h"
#include "user.h"


CComboAttack gComboAttack;

CComboAttack::CComboAttack(void)
{
	return;
}

CComboAttack::~CComboAttack(void)
{
	return;
}

int CComboAttack::GetSkillPos(int skillnum)
{
	switch (skillnum)
	{
		case 19:
		case 20:
		case 21:
		case 22:
		case 23:
			return 0;
			break;
		case 41:
		case 42:
		case 43:
		case 455:
		case 456:
		case 457:
		case 458:
		case 459:
		case 460:
		case 461:
		case 462:
		case 463:
		case 464:
		case 465:
		case 466:
		case 467:
		case 468:
		case 469:
			return 1;
			break;
		default:
			return -1;
			break;
	}
}

bool CComboAttack::CheckCombo(int aIndex, int skillnum)
{
	LPOBJ lpObj = &gObj[aIndex];
	
	if ( lpObj->ComboSkillquestClear  != false )
	{
		int sp = this->GetSkillPos(skillnum);

		if ( sp == 0 )
		{
			lpObj->comboSkill.ProgressIndex  = 0;
			lpObj->comboSkill.dwTime = GetTickCount() + 3000;
			lpObj->comboSkill.Skill[0]  = skillnum;
		}
		else if ( sp == 1 )
		{
			if ( lpObj->comboSkill.Skill[0]  == 0xFF )
			{
				lpObj->comboSkill.Init();
				return false;
			}

			int Time =GetTickCount();

			if (lpObj->comboSkill.dwTime < GetTickCount() )
			{
				lpObj->comboSkill.Init();
				return false;
			}

			if ( lpObj->comboSkill.ProgressIndex == 0 )
			{
				lpObj->comboSkill.ProgressIndex = 1;
				lpObj->comboSkill.dwTime = GetTickCount() + 3000;
				lpObj->comboSkill.Skill[1] = skillnum;
			}
			else if ( lpObj->comboSkill.Skill[1]  != skillnum )
			{
				lpObj->comboSkill.Init();
				return true;
			}
			else
			{
				lpObj->comboSkill.Init();
			}
		}
		else
		{
			lpObj->comboSkill.ProgressIndex= -1;
			lpObj->comboSkill.dwTime = 0;
			lpObj->comboSkill.Skill[0] = -1;
		}
	}

	return false;
}