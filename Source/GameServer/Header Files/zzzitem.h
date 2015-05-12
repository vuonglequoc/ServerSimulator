//**********************************************************************//
// Project:	HaRiBO MU GameServer - Season 6								//
// Company: RealCoderZ MU Development © 2010							//
// Description: MU Items Structures	& Functions							//
// Coded & Modified By:	WolF & M.E.S									//
//**********************************************************************//
#ifndef ZZZITEM_H
#define ZZZITEM_H

#include "Defines.H"


#define MAX_ITEM_LEVEL	15

// **************************
// Item Limits
// **************************
#define MAX_TYPE_ITEMS 16
#define MAX_SUBTYPE_ITEMS 512
#define MAX_EXOPTION_SIZE 8

// ********************
// MACRO for get Item
// ********************
#define ITEMGET(x,y) ( (x)*MAX_SUBTYPE_ITEMS + (y))

// ********************
//  GET ITEM INDEX
// ********************
enum PLAYER_WARDROBE
{
	WEAPON_01	= 0,	
	WEAPON_02	= 1,	
	HELMET		= 2,	
	ARMOR		= 3,	
	PANTS		= 4,	
	GLOVES		= 5,	
	BOOTS		= 6,	
	WINGS		= 7,	
	GUARDIAN	= 8,	
	PENDANT		= 9,	
	RING_01		= 10,	
	RING_02		= 11,	
};
// **************
// Pet Level
// **************
#define MAX_PET_LEVEL 50
#define MAX_ITEM_DURABILITY	255
#define MAX_ITEMS (MAX_TYPE_ITEMS*MAX_SUBTYPE_ITEMS)
#define MAX_ITEM_SPECIAL_ATTRIBUTE 7
#define DBI_GET_TYPE(x) ( ((x)&((MAX_TYPE_ITEMS-1)<<8))>>4 )
#define DBI_GET_INDEX(x) ( (x) & 0xFF )
#define DBI_GET_LEVEL(x)  ( ((x)>>3)& MAX_ITEM_LEVEL )
#define DBI_GET_SKILL(x)  ( ((x) >> 7) & 0x01)
#define DBI_GET_LUCK(x)  ( ((x) >> 2) & 0x01)
#define DBI_GET_OPTION(x)  ( ( (x) ) & 0x03 )
#define DBI_GET_OPTION28(x) ( (x) & 0x70  )
#define DBI_GET_DINO_OPTION(x)  ( DBI_GET_OPTION28((x)) >> 7 )
#define DBI_GET_NOPTION(x)  ( (x) & 0x3F  )
#define DBI_GET_380OPTION(x) ( ((x) & 0x08) << 4  )
#define ITEM_GET_TYPE(x)  ( (x)/MAX_SUBTYPE_ITEMS  )
#define ITEM_GET_INDEX(x) ( (x)%MAX_SUBTYPE_ITEMS  )


#define DBI_TYPE			0
#define DBI_OPTION_DATA		1
#define DBI_DUR				2
#define DBI_SERIAL1			3
#define DBI_SERIAL2			4
#define DBI_SERIAL3			5
#define DBI_SERIAL4			6
#define DBI_NOPTION_DATA	7
#define DBI_SOPTION_DATA	8
#define DBI_OPTION380_DATA	9
#define DBI_JOH_DATA		10
#define DBI_SOCKET_1		11
#define DBI_SOCKET_2		12
#define DBI_SOCKET_3		13
#define DBI_SOCKET_4		14
#define DBI_SOCKET_5		15


#define MAX_ITEM_INFO	12


#define I_TYPE		0
#define I_OPTION	1
#define I_DUR		2
#define I_NOPTION	3
#define I_SOPTION	4
#define I_380OPTION	5
#define I_JOHOPTION	6
#define I_SOCKETOPT1 7
#define I_SOCKETOPT2 8
#define I_SOCKETOPT3 9
#define I_SOCKETOPT4 10
#define I_SOCKETOPT5 11

#define ITEM_DUR_RANGE(x) (((x)<0)?FALSE:((x)>MAX_ITEM_DURABILITY-1)?FALSE:TRUE )


typedef struct
{
	char Name[32];
	BYTE HaveItemInfo;
	BYTE TwoHand;
	BYTE Level;
	BYTE Width;
	BYTE Height;
	char Serial;
	BYTE OptionFlag;
	BYTE MondownFlag;
	BYTE AttackSpeed;
	BYTE WalkSpeed;
	BYTE DamageMin;
	BYTE DamageMax;
	BYTE SuccessfulBlocking;
	BYTE Defense;
	BYTE MagicDefense;
	BYTE Speed;
	BYTE Durability;
	BYTE MagicDurability;
	BYTE AttackDur;
	BYTE DefenceDur;
	WORD RequireStrength;
	WORD RequireDexterity;
	WORD RequireEnergy;
	WORD RequireLevel;
	WORD Value;
	BYTE RequireClass[MAX_CLASS_TYPE];
	BYTE Resistance[MAX_ITEM_SPECIAL_ATTRIBUTE];
	WORD RequireVitality;
	int BuyMoney;
	int MagicPW;
	float RepaireMoneyRate;
	float AllRepaireMoneyRate;
	bool QuestItem;
	BYTE SetAttr;
	BYTE ResistanceType;
	int ItemSlot;
	int SkillType;
	int RequireLeadership;
	BYTE SocketItem;

}  ITEM_ATTRIBUTE, * LPITEM_ATTRIBUTE;
extern  ITEM_ATTRIBUTE ItemAttribute[MAX_ITEMS];


class CItem
{
public:
	CItem(void);
	void Convert(int type, BYTE Option1, BYTE Option2, BYTE Option3,BYTE Attribute2, BYTE SetOption, BYTE ItemEffectEx, BYTE * SocketOption);
	void Value();
	void OldValue();
	int GetSize(int& w, int& h);
	void Clear();
	bool IsItem();
	int IsSetItem();
	int GetAddStatType();
	void SetPetItemInfo(int petlevel, int petexp);
	bool AddPetItemExp(int petexp);
	int DecPetItemExp(int percent);
	void PetValue();
	int PetItemLevelDown(int exp);
	int ItemDamageMin();
	int ItemDefense();
	bool IsClass(char aClass, int ChangeUP, int ChangeUP2);
	LPSTR GetName();
	int GetLevel();

	void PlusSpecial(int* Value, int Special);
	void PlusSpecialPercent(int* Value, int Special, WORD Percent);
	void PlusSpecialPercentEx(int* Value, int SourceValue, int Special);
	void SetItemPlusSpecialStat(short* Value, int Special);
	int GetWeaponType();
	void PlusSpecialSetRing(unsigned char* Value);
	DWORD GetNumber();
	bool IsExtItem();
	int IsExtLifeAdd();
	int IsExtManaAdd();
	int IsExtDamageMinus();
	int IsExtDamageReflect();
	int IsExtDefenseSuccessfull();
	int IsExtMonsterMoney();
	int IsExtExcellentDamage();
	int IsExtAttackRate();
	int IsExtAttackRate2();
	int IsExtAttackSpeed();
	int IsExtMonsterDieLife();
	int IsExtMonsterDieMana();
	int IsWingOpGetOnePercentDamage();
	int IsWingOpGetManaToMoster();
	int IsDinorantReduceAttackDamaege();
	int IsFenrirIncLastAttackDamage();
	int IsFenrirDecLastAttackDamage();
	int IsFenrirIllusionOption();
	int SimpleDurabilityDown(int iDur);
	int DurabilityDown(int dur, int aIndex);
	int DurabilityDown2(int dur, int aIndex);
	int NormalWeaponDurabilityDown(int defence, int aIndex);
	int BowWeaponDurabilityDown(int defence, int aIndex);
	int StaffWeaponDurabilityDown(int defence, int aIndex);
	int ArmorDurabilityDown(int damagemin, int aIndex);
	bool CheckDurabilityState();
	bool CheckThirdWingsSuccessfulBlocking();
	bool IsSocketItem();

	DWORD m_Number;
	char m_serial;
	short m_Type;
	BYTE m_Level;
	BYTE m_Part;
	BYTE m_Class;
	BYTE m_TwoHand;
	BYTE m_AttackSpeed;
	BYTE m_WalkSpeed;
	WORD m_DamageMin;
	WORD m_DamageMax;
	BYTE m_SuccessfulBlocking;
	WORD m_Defense;
	WORD m_MagicDefense;
	BYTE m_Speed;
	WORD m_DamageMinOrigin;
	WORD m_DefenseOrigin;
	WORD m_Magic;
	float m_Durability;
	WORD m_DurabilitySmall;
	float m_BaseDurability;
	BYTE m_SpecialNum;
	WORD m_Special[8];
	BYTE m_SpecialValue[8];
	WORD m_RequireStrength;
	WORD m_RequireDexterity;
	WORD m_RequireEnergy;
	WORD m_RequireLevel;
	WORD m_RequireVitality;
	WORD m_RequireLeaderShip;
	WORD m_Leadership;
	BYTE m_RequireClass[MAX_CLASS_TYPE];
	BYTE m_Resistance[MAX_ITEM_SPECIAL_ATTRIBUTE];
	int m_Value;
	DWORD m_SellMoney;
	DWORD m_BuyMoney;
	int m_iPShopValue;
	bool m_bItemExist;
	int m_OldSellMoney;
	int m_OldBuyMoney;
	BYTE m_Option1;					// Skill
	BYTE m_Option2;					// Luck
	BYTE m_Option3;					// Option
	BYTE m_NewOption;				// Excellent Option
	float m_DurabilityState[4];
	float m_CurrentDurabilityState;
	bool m_SkillChange;
	bool m_QuestItem;
	BYTE m_SetOption;
	BYTE m_SetAddStat;
	bool m_IsValidItem;
	BYTE m_SkillResistance[MAX_ITEM_SPECIAL_ATTRIBUTE];
	BOOL m_IsLoadPetItemInfo;
	int  m_PetItem_Level;
	int  m_PetItem_Exp;
	BYTE m_JewelOfHarmonyOption;
	WORD m_HJOpStrength;
	WORD m_HJOpDexterity;
	BYTE m_ItemOptionEx;
	BYTE m_ItemSocket[5];
};

class CPetItemExp
{
public:
	int m_DarkSpiritExpTable[MAX_PET_LEVEL+2];
	int m_DarkHorseExpTable[MAX_PET_LEVEL+2];

public:
	CPetItemExp()
	{
		this->m_DarkSpiritExpTable[0] = 0;
		this->m_DarkSpiritExpTable[1] = 0;

		for ( int i = 2 ; i < MAX_PET_LEVEL+2 ; i++ )
		{
			this->m_DarkSpiritExpTable[i] = (i+10) * i * i * i * 100;
		}

		this->m_DarkHorseExpTable[0] = 0;
		this->m_DarkHorseExpTable[1] = 0;

		for ( int i = 2 ; i < MAX_PET_LEVEL+2 ; i++ )
		{
			this->m_DarkHorseExpTable[i] = (i+10) * i * i * i * 100;
		}
	};
};

extern int g_MaxItemIndexOfEachItemType[MAX_TYPE_ITEMS];

void BufferItemtoConvert3(unsigned char* buf, int& type, BYTE& level, BYTE& op1, BYTE& op2, BYTE& op3, BYTE& dur);
void ItemByteConvert16(LPBYTE buf, CItem * const item , int maxitem);
void ItemByteConvert(unsigned char* buf, CItem item);
void ItemByteConvert(unsigned char* buf, int type, BYTE Option1, BYTE Option2, BYTE Option3, BYTE level, BYTE dur, BYTE Noption, BYTE SetOption, BYTE JewelOfHarmonyOption, BYTE ItemEffectEx, BYTE* ItemSocket);
int ItemGetNumberMake(int type, int index);
void ItemGetSize(int index, int & width, int & height);
BOOL HasItemDurability(int index);
int ItemGetDurability(int index, int itemLevel, int ExcellentItem, int SetItem);
int ItemGetAttackDurability(int index);
int ItemGetDefenseDurability(int index);
float GetRepairItemRate(int index);
float GetAllRepairItemRate(int index);
void CalRepairRate( int itemtype, int itemsubtype, LPITEM_ATTRIBUTE p);
BOOL OpenItemScript(char* FileName);
BOOL OpenItemNameScript(char* FileName);
BOOL OpenItemScript(char* Buffer, int iSize);
BOOL OpenItemNameScript(char* Buffer, int iSize);
int zzzItemLevel(int type, int index, int level);
int GetLevelItem(int type, int index, int level);
int GetSerialItem(int type);
int IsItem(int item_num);
LPITEM_ATTRIBUTE GetItemAttr(int item_num);


#endif