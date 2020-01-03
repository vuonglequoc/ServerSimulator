//**********************************************************************//
// Project:	HaRiBO MU GameServer - Season 6								//
// Company: RealCoderZ MU Development © 2010							//
// Description: MU ComboAttack Structures & Functions					//
// Coded & Modified By:	WolF & M.E.S									//
//**********************************************************************//
#ifndef COMBOATTACK_H
#define COMBOATTACK_H

struct ComboSkillData
{
public:

	void Init()
	{
		this->dwTime = 0;
		this->Skill[0] = -1;
		this->Skill[1] = -1;
		this->Skill[2] = -1;
		this->ProgressIndex = -1;
	};

	DWORD dwTime;
	WORD Skill[3];
	int ProgressIndex;
};

class CComboAttack
{
public:
	CComboAttack(void);
	~CComboAttack(void);
private:
	int GetSkillPos(int skillnum);
public:
	bool CheckCombo(int aIndex, int skillnum);
};
extern CComboAttack gComboAttack;


#endif