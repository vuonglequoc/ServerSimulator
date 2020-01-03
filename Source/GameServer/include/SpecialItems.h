#ifndef SPECIALITEMS_H
#define SPECIALITEMS_H

#include "User.h"

enum SPECIALITEMEFFECT
{
	INCREASE_MAX_MANA = 0,
	INCREASE_MAX_AG = 1,
	INCREASE_MAX_LIFE = 2,
	INCREASE_MAX_SD = 3,
	INCREASE_DROP_RATE = 4,
	INCREASE_EXE_DMG = 5,
	INCREASE_CRITICAL_DMG = 6,
	INCREASE_LIFE_AUTORECOVERY = 7,
	INCREASE_EXP_RATE = 8,
	INCREASE_ATTACK_DMG = 9,
	INCREASE_DEFENSE = 10,
	INCREASE_MANA_AUTORECOVERY = 11,
	INCREASE_ATTACK_SPEED = 12,
	DISABLE_EXP = 13,
	ENABLE_MOBILITY = 14,
	INCREASE_WIZARDY_DMG = 15,
	INCREASE_STRENGHT = 16,
	INCREASE_AGILITY = 17,
	INCREASE_HEALTH = 18,
	INCREASE_ENERGY = 19,
	INCREASE_LEADERSHIP = 20,
};

class CSpecialItem
{
public:
	CSpecialItem(void);
	~CSpecialItem(void);
	// ----
	void Init();
	void ReadFile();
	void UseItem(LPOBJ lpObj, char Type, int Item);
	void ApplyEffect(LPOBJ lpObj,char Type,int Item);
	void RemoveEffect(LPOBJ lpObj, char Type, int Item);
	int SearchItemPos(int Item, char Type);
	bool CheckNoExp(LPOBJ lpObj);
	void TimerEffect(LPOBJ lpObj);
	// ----
	short ItemCount[7];
	char ItemType[7][255];
	short ItemIndex[7][255];
	char EffectType1[7][255];
	short EffectValue1[7][255];
	char EffectType2[7][255];
	short EffectValue2[7][255];
	int BuffTime[7][255];
	short BuffNumber[7][255];

};

extern CSpecialItem gSpecialItem;

#endif