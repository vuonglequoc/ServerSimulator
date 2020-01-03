#ifndef __ITEMDROPMANAGER_H__
#define __ITEMDROPMANAGER_H__

#include "User.h"

class CItemDropManager
{
public:
	CItemDropManager(void);
	~CItemDropManager(void);

	void Init();
	bool ProccessItemDrop(LPOBJ lpMobObj);

private:
	int ItemsCount;
	char ItemType[500];
	short ItemIndex[500];
	char ItemLevel[500];
	unsigned char ItemDur[500];
	char ItemOption[500];
	char ItemLuck[500];
	char ItemSkill[500];
	char ItemExe[500];
	char ItemAncient[500];
	unsigned char Socket[5][500];
	char DropMap[500];
	unsigned char MobMinLvl[500];
	unsigned char MobMaxLvl[500];
	short DropRate[500];
};

extern CItemDropManager gItemDropManager;

#endif