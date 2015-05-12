#ifndef _MOSSMERCHANT_H_
#define _MOSSMERCHANT_H_

#define MAX_ITEM_SECTION	255

void MossEventRun(void * lpParam);

class CMossMerchant
{
public:
	CMossMerchant(void);
	~CMossMerchant(void);
	// ----
	void Init();
	void ReadFile();
	void ItemBuy(int aIndex,int Item);
	void Start();
	// ----
	bool Enabled;
	bool Running;
	int Time;
	int TimeAux;
	// ----
	int ItemCount[5];

	int LuckRate;
	int SkillRate;
	int LevelRate;
	int SocketRate[5];
	int ExeRate;
	int AncientRate;
	int OptionRate;

	int SpecialItem[5];
	int SpecialLuckRate[5];
	int SpecialSkillRate[5];
	int SpecialLevelRate[5];
	int SpecialSocketRate[5][5];
	int SpecialExeRate[5];
	int SpecialOptionRate[5];

	unsigned char Type[5][MAX_ITEM_SECTION];
	unsigned short Index[5][MAX_ITEM_SECTION];
	unsigned char Level[5][MAX_ITEM_SECTION];
	unsigned char Dur[5][MAX_ITEM_SECTION];
	unsigned char Skill[5][MAX_ITEM_SECTION];
	unsigned char Luck[5][MAX_ITEM_SECTION];
	unsigned char Opt[5][MAX_ITEM_SECTION];
	unsigned char Exe[5][MAX_ITEM_SECTION];
	unsigned char Anc[5][MAX_ITEM_SECTION];
	unsigned char Socket[5][5][MAX_ITEM_SECTION];
};

extern CMossMerchant gMossMerchant;

#endif