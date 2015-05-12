// ------------------------------
// Decompiled by Deathway
// Date : 2007-05-09
// ------------------------------
#ifndef JEWELMIXSYSTEM_H
#define JEWELMIXSYSTEM_H

enum JEWEL_TYPE
{
	JEWEL_BLESS = 0,
	JEWEL_SOUL = 1,
	JEWEL_LIFE = 2,
	JEWEL_CREATION = 3,
	JEWEL_GUARDIAN = 4,
	JEWEL_STONE = 5,
	JEWEL_HARMONY = 6,
	JEWEL_CHAOS = 7,
	JEWEL_LOWREF = 8,
	JEWEL_HIREF = 9,
};

class CJewelMixSystem
{

public:

	CJewelMixSystem();
	virtual ~CJewelMixSystem();

	static int GetJewelCount(int iIndex, int iJewelType);
	static int GetJewelCountPerLevel(int iJewelType, int iJewelLevel);
	static int MixJewel(int iIndex, int iJewelType, int iMixType);
	static int UnMixJewel(int iIndex, int iJewelType, int iJewelLevel, int iInventoryPos);

private:

};


#endif