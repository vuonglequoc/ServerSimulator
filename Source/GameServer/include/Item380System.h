#ifndef _ITEM380SYSTEM_H_
#define _ITEM380SYSTEM_H_

#include "zzzitem.h"

#define ITEMOPTION_FOR380ITEM_EFFECT_NONE							0
#define ITEMOPTION_FOR380ITEM_EFFECT_OPADDATTACKSUCCESSRATEPVP		1
#define ITEMOPTION_FOR380ITEM_EFFECT_OPADDDAMAGE					2
#define ITEMOPTION_FOR380ITEM_EFFECT_OPADDDEFENSESUCCESSRATEPVP		3
#define ITEMOPTION_FOR380ITEM_EFFECT_OPADDDEFENSE					4
#define ITEMOPTION_FOR380ITEM_EFFECT_OPADDMAXHP						5
#define ITEMOPTION_FOR380ITEM_EFFECT_OPADDMAXSD						6
#define ITEMOPTION_FOR380ITEM_EFFECT_OPREFILLON						7
#define ITEMOPTION_FOR380ITEM_EFFECT_OPADDREFILLSD					8

struct ITEMOPTION_FOR380ITEM
{
	BOOL IsLoad;	// 0
	BYTE m_Type;	// 4
	BYTE m_Index;	// 5
	BYTE m_Option1;	// 6
	WORD m_Value1;	// 8
	BYTE m_Option2;	// A
	WORD m_Value2;	// C
};

struct ITEMOPTION_FOR380ITEM_EFFECT
{
	short OpAddAttackSuccessRatePVP;	// 0
	short OpAddDamage;	// 2
	short OpAddDefenseSuccessRatePvP;	// 4
	short OpAddDefense;	// 6
	short OpAddMaxHP;	// 8
	short OpAddMaxSD;	// A
	short OpRefillOn;	// C
	short OpAddRefillSD;	// E
};

class CItem380System
{
public:
	CItem380System(void);
	~CItem380System(void);
	void Init();

	void Load380ItemOptionInfo(const char* filename);
	BOOL Is380Item( CItem const *  pItem);
	BOOL Is380OptionItem( CItem const * pItem);
	void InitEffectValue( ITEMOPTION_FOR380ITEM_EFFECT* pItemEffect);
	int ApplyFor380Option(struct OBJECTSTRUCT* lpObj);
	void SetOptionItemByMacro(struct OBJECTSTRUCT* lpObj, BYTE invenrotyTargetPos, int bOption);
	int ChaosMix380ItemOption(struct OBJECTSTRUCT* lpObj);

private:
	int _CalcItemEffectValue(int iItemOptionType, int iItemEffectValue, ITEMOPTION_FOR380ITEM_EFFECT* pItemEffect);
	int _SetOption(CItem* pItem, int bOption);

private:
	ITEMOPTION_FOR380ITEM m_itemOption[MAX_ITEMS];
	BOOL m_bSystemFor380ItemOption;
	int m_iNeedZenFor380Option;
	int m_iRateSuccessRateForMix1;
	int m_iRateSuccessRateForMix2;
	int m_iRateSuccessRateForMix3;
};

extern CItem380System g_Item380System;


inline void DebugLog(LPSTR szLog, ...)
{
	char szBuffer[512] = "";
	va_list pArguments;

	va_start(pArguments, szLog);
	vsprintf(szBuffer, szLog, pArguments);
	va_end(pArguments);

	OutputDebugString(szBuffer);
}

#endif